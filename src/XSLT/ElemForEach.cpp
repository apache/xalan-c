/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */
#include "ElemForEach.hpp"



#include <algorithm>
#include <cassert>



#include <xercesc/sax/AttributeList.hpp>



#include <Include/STLHelper.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>



#include <XPath/XPath.hpp>



#include "ElemSort.hpp"
#include "NodeSorter.hpp"
#include "SelectionEvent.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



ElemForEach::ElemForEach(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const AttributeListType&		atts,
			int								lineNumber,
			int								columnNumber) :
	ElemTemplateElement(constructionContext,
						stylesheetTree,
						lineNumber,
						columnNumber,
						StylesheetConstructionContext::ELEMNAME_FOR_EACH),
	m_selectPattern(0),
	m_sortElems(),
	m_sortElemsCount(0)
{
	const unsigned int	nAttrs = atts.getLength();
		
	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar*	const	aname = atts.getName(i);

		if(equals(aname, Constants::ATTRNAME_SELECT))
		{
			m_selectPattern = constructionContext.createXPath(getLocator(), atts.getValue(i), *this);
		}
		else if(!(isAttrOK(aname, atts, i, constructionContext) ||
				processSpaceAttr(aname, atts, i, constructionContext)))
		{
			constructionContext.error(
				"xsl:for-each has an illegal attribute",
				0,
				this);
		}
	}

	if(0 == m_selectPattern)
	{
		constructionContext.error(
			"xsl:for-each must have a 'select' attribute",
			0,
			this);
	}
}



ElemForEach::ElemForEach(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			int								lineNumber,
			int								columnNumber,
			int								xslToken) :
	ElemTemplateElement(constructionContext,
						stylesheetTree,
						lineNumber,
						columnNumber,
						xslToken),
	m_selectPattern(0),
	m_sortElems(),
	m_sortElemsCount(0)
{
}



ElemForEach::~ElemForEach()
{
	XALAN_USING_STD(for_each)

	for_each(m_sortElems.begin(),
			 m_sortElems.end(),
			 DeleteFunctor<ElemSort>());
}



void
ElemForEach::processSortElement(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						theStylesheet,
			const AttributeListType&		atts,
			const LocatorType*				locator)
{
	const int	lineNumber = XalanLocator::getLineNumber(locator);
	const int	columnNumber = XalanLocator::getColumnNumber(locator);

	m_sortElems.reserve(m_sortElems.size() + 1);

	ElemSort* sortElem = new ElemSort(
		constructionContext,
		theStylesheet,
		atts,
		lineNumber,
		columnNumber);

	m_sortElems.push_back(sortElem);
}



const XalanDOMString&
ElemForEach::getElementName() const
{
	return Constants::ELEMNAME_FOREACH_WITH_PREFIX_STRING;
}



void
ElemForEach::postConstruction(
			StylesheetConstructionContext&	constructionContext,
			const NamespacesHandler&		theParentHandler)
{
	ElemTemplateElement::postConstruction(constructionContext, theParentHandler);

	m_sortElemsCount = m_sortElems.size();
}



void
ElemForEach::execute(StylesheetExecutionContext&	executionContext) const
{
	assert(m_selectPattern != 0);

	StylesheetExecutionContext::SetAndRestoreCurrentTemplate	theSetAndRestore(executionContext, 0);

	if (hasChildren() == true)
	{
		XalanNode* const	sourceNode = executionContext.getCurrentNode();

		if (sourceNode != 0)
		{
			transformSelectedChildren(
				executionContext,
				this,
				sourceNode,
				executionContext.getCurrentStackFrameIndex());
		}
		else
		{
			executionContext.error(
				"There is no current node in ElemForEach::execute()",
				sourceNode, 
				getLocator());
		}
	}
}



void
ElemForEach::transformSelectedChildren(
			StylesheetExecutionContext&		executionContext,
			const ElemTemplateElement*		theTemplate,
			XalanNode*						sourceNodeContext,
			int								selectStackFrameIndex) const
{
	assert(m_selectPattern != 0);
	assert(m_sortElemsCount == m_sortElems.size());

	if (m_sortElemsCount == 0)
	{
		transformSelectedChildren(
					executionContext,
					theTemplate,
					sourceNodeContext,
					0,
					selectStackFrameIndex);
	}
	else
	{
		typedef NodeSorter::NodeSortKeyVectorType					NodeSortKeyVectorType;
		typedef StylesheetExecutionContext::BorrowReturnNodeSorter	BorrowReturnNodeSorter;

		BorrowReturnNodeSorter	sorter(executionContext);

		NodeSortKeyVectorType&	keys = sorter->getSortKeys();
		assert(keys.empty() == true);

		CollectionClearGuard<NodeSortKeyVectorType>		guard(keys);

		// Reserve the space now...
		keys.reserve(m_sortElemsCount);

		// Get some temporary strings to use for evaluting the AVTs...
		XPathExecutionContext::GetAndReleaseCachedString	theTemp1(executionContext);

		XalanDOMString&		langString = theTemp1.get();

		XPathExecutionContext::GetAndReleaseCachedString	theTemp2(executionContext);

		XalanDOMString&		scratchString = theTemp2.get();

		// March backwards, performing a sort on each xsl:sort child.
		// Probably not the most efficient method.
		for(SortElemsVectorType::size_type	i = 0; i < m_sortElemsCount; i++)
		{
			const ElemSort* const	sort = m_sortElems[i];
			assert(sort != 0);

			const AVT* avt = sort->getLangAVT();

			if(0 != avt)
			{
				avt->evaluate(langString, sourceNodeContext, *this, executionContext);
			}

			avt = sort->getDataTypeAVT();

			if(0 != avt)
			{
				avt->evaluate(scratchString, sourceNodeContext, *this, executionContext);
			}			

			bool	treatAsNumbers = false;

			if (isEmpty(scratchString) == false)
			{
				if (equals(scratchString, Constants::ATTRVAL_DATATYPE_NUMBER) == true)
				{
					treatAsNumbers = true;
				}
				else if (equals(scratchString, Constants::ATTRVAL_DATATYPE_TEXT) == false)
				{
					const XalanQNameByValue		theQName(scratchString, this);

					if (theQName.getNamespace().length() == 0)
					{
						executionContext.error(
							"xsl:sort data-type must be 'text', 'number' or a prefixed name",
							sourceNodeContext,
							sort->getLocator());
					}
					else
					{
						executionContext.warn(
							"xsl:sort has an unknown data-type.  The data-type will be 'text'",
							sourceNodeContext,
							sort->getLocator());
					}
				}
			}

			clear(scratchString);

			avt = sort->getOrderAVT();

			if(0 != avt)
			{
				avt->evaluate(scratchString, sourceNodeContext, *this, executionContext);
			}			

			bool	descending = false;
			
			if (isEmpty(scratchString) == false)
			{
				if (equals(scratchString, Constants::ATTRVAL_ORDER_DESCENDING) == true)
				{
					descending = true;
				}
				else if (equals(scratchString, Constants::ATTRVAL_ORDER_ASCENDING) == false)
				{
					executionContext.error(
						"xsl:sort order must be 'ascending' or 'descending'",
						sourceNodeContext,
						sort->getLocator());
				}
			}

			clear(scratchString);

			avt = sort->getCaseOrderAVT();

			if(0 != avt)
			{
				avt->evaluate(scratchString, sourceNodeContext, *this, executionContext);
			}			

			NodeSortKey::eCaseOrder		caseOrder = NodeSortKey::eDefault;

			if (isEmpty(scratchString) == false)
			{
				if (equals(scratchString, Constants::ATTRVAL_CASEORDER_UPPER) == true)
				{
					caseOrder = NodeSortKey::eUpperFirst;
				}
				else if (equals(scratchString, Constants::ATTRVAL_CASEORDER_LOWER) == true)
				{
					caseOrder = NodeSortKey::eLowerFirst;
				}
				else
				{
					executionContext.error(
						"xsl:sort case-order must be 'upper-first' or 'lower-first'",
						sourceNodeContext,
						sort->getLocator());
				}
			}

			clear(scratchString);

			assert(sort->getSelectPattern() != 0);

			keys.push_back(
					NodeSortKey(
						executionContext,
						*sort->getSelectPattern(),
						treatAsNumbers,
						descending,
						caseOrder,
						langString,
						*this));
		}

		transformSelectedChildren(
					executionContext,
					theTemplate,
					sourceNodeContext,
					sorter.get(),
					selectStackFrameIndex);
	}
}



void
ElemForEach::transformSelectedChildren(
			StylesheetExecutionContext&		executionContext,
			const ElemTemplateElement*		theTemplate,
			XalanNode*						sourceNodeContext,
			NodeSorter*						sorter,
			int								selectStackFrameIndex) const
{
	typedef StylesheetExecutionContext::SetAndRestoreCurrentStackFrameIndex		SetAndRestoreCurrentStackFrameIndex;

	assert(m_selectPattern != 0);

	typedef XPathExecutionContext::BorrowReturnMutableNodeRefList	BorrowReturnMutableNodeRefList;

	BorrowReturnMutableNodeRefList	theGuard(executionContext);

	const NodeRefListBase*	sourceNodes = 0;

	XObjectPtr				xobjectResult;

	{
		SetAndRestoreCurrentStackFrameIndex		theSetAndRestore(
					executionContext,
					selectStackFrameIndex);

		xobjectResult = m_selectPattern->execute(
						sourceNodeContext,
						*this,
						executionContext,
						*theGuard);

		if (xobjectResult.null() == true)
		{
			sourceNodes = &*theGuard;
		}
		else
		{
			theGuard.release();

			sourceNodes = &xobjectResult->nodeset();
		}
	}

	if(0 != executionContext.getTraceListeners())
	{
		executionContext.fireSelectEvent(
				SelectionEvent(
					executionContext, 
					sourceNodeContext,
					*this,
					StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("select")),
					*m_selectPattern,
					*sourceNodes));
	}

	const NodeRefListBase::size_type	nNodes = sourceNodes->getLength();

	if (nNodes > 0)
	{
		// If there's not NodeSorter, or we've only selected one node,
		// then just do the transform...
		if (sorter == 0 || nNodes == 1)
		{
			transformSelectedChildren(
				executionContext,
				theTemplate,
				*sourceNodes,
				nNodes);
		}
		else
		{
			typedef StylesheetExecutionContext::SetAndRestoreCurrentStackFrameIndex		SetAndRestoreCurrentStackFrameIndex;
			typedef StylesheetExecutionContext::ContextNodeListSetAndRestore			ContextNodeListSetAndRestore;
			typedef StylesheetExecutionContext::BorrowReturnMutableNodeRefList			BorrowReturnMutableNodeRefList;

			BorrowReturnMutableNodeRefList	sortedSourceNodes(executionContext);

			*sortedSourceNodes = *sourceNodes;

			{
				SetAndRestoreCurrentStackFrameIndex		theStackFrameSetAndRestore(
						executionContext,
						selectStackFrameIndex);

				ContextNodeListSetAndRestore			theContextNodeListSetAndRestore(
						executionContext,
						*sourceNodes);

				sorter->sort(executionContext, *sortedSourceNodes);
			}

			transformSelectedChildren(
				executionContext,
				theTemplate,
				*sortedSourceNodes,
				nNodes);
		}
	}
}



void
ElemForEach::transformSelectedChildren(
			StylesheetExecutionContext&		executionContext,
			const ElemTemplateElement*		theTemplate,
			const NodeRefListBase&			sourceNodes,
			NodeRefListBase::size_type		sourceNodesCount) const
{
	if(executionContext.getTraceSelects() == true)
	{
		executionContext.traceSelect(
			*this,
			sourceNodes,
			m_selectPattern);
	}

	// Create an object to set and restore the context node list...
	StylesheetExecutionContext::ContextNodeListSetAndRestore	theSetAndRestore(
				executionContext,
				sourceNodes);

	for(unsigned int i = 0; i < sourceNodesCount; i++) 
	{
		XalanNode* const		childNode = sourceNodes.item(i);
		assert(childNode != 0);

		transformChild(
				executionContext,
				*this,
				theTemplate,
				childNode);
	}
}



XALAN_CPP_NAMESPACE_END
