/*
 * Copyright 1999-2004 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "ElemForEach.hpp"



#include <algorithm>
#include <cassert>



#include <xercesc/sax/AttributeList.hpp>



#include <xalanc/Include/STLHelper.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/XPath/XPath.hpp>



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
					XalanMessageLoader::getMessage(
						XalanMessages::TemplateHasIllegalAttribute_2Param,
							Constants::ELEMNAME_FOREACH_WITH_PREFIX_STRING.c_str(),
							aname),
					0,
					this);
		}
	}

	if(0 == m_selectPattern)
	{
		constructionContext.error(
			XalanMessageLoader::getMessage(
				XalanMessages::TemplateMustHaveAttribute_2Param,
				Constants::ELEMNAME_FOREACH_WITH_PREFIX_STRING,
				Constants::ATTRNAME_SELECT),
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
	assert(executionContext.getCurrentNode() != 0);

	StylesheetExecutionContext::PushAndPopCurrentTemplate	thePushAndPop(executionContext, 0);

	if (hasChildren() == true)
	{
		transformSelectedChildren(
			executionContext,
			this);
	}
}



const XPath*
ElemForEach::getXPath(unsigned int	index) const
{
	return index == 0 ? m_selectPattern : 0;
}



void
ElemForEach::transformSelectedChildren(
			StylesheetExecutionContext&		executionContext,
			const ElemTemplateElement*		theTemplate) const
{
	assert(m_selectPattern != 0);
	assert(m_sortElemsCount == m_sortElems.size());

	if (m_sortElemsCount == 0)
	{
		selectAndSortChildren(
					executionContext,
					theTemplate,
					0,
					executionContext.getCurrentStackFrameIndex());
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
				avt->evaluate(langString, *this, executionContext);
			}

			avt = sort->getDataTypeAVT();

			if(0 != avt)
			{
				avt->evaluate(scratchString, *this, executionContext);
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
							XalanMessageLoader::getMessage(XalanMessages::XslSortDataTypeMustBe),
							executionContext.getCurrentNode(),
							sort->getLocator());
					}
					else
					{
						executionContext.warn(
							XalanMessageLoader::getMessage(XalanMessages::XslSortHasUnlnownDataType),
							executionContext.getCurrentNode(),
							sort->getLocator());
					}
				}
			}

			clear(scratchString);

			avt = sort->getOrderAVT();

			if(0 != avt)
			{
				avt->evaluate(scratchString, *this, executionContext);
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
						XalanMessageLoader::getMessage(XalanMessages::XslSortMustBeAscendOrDescend),
						executionContext.getCurrentNode(),
						sort->getLocator());
				}
			}

			clear(scratchString);

			avt = sort->getCaseOrderAVT();

			if(0 != avt)
			{
				avt->evaluate(scratchString, *this, executionContext);
			}			

			XalanCollationServices::eCaseOrder	caseOrder = XalanCollationServices::eDefault;

			if (isEmpty(scratchString) == false)
			{
				if (equals(scratchString, Constants::ATTRVAL_CASEORDER_UPPER) == true)
				{
					caseOrder = XalanCollationServices::eUpperFirst;
				}
				else if (equals(scratchString, Constants::ATTRVAL_CASEORDER_LOWER) == true)
				{
					caseOrder = XalanCollationServices::eLowerFirst;
				}
				else
				{
					executionContext.error(
						XalanMessageLoader::getMessage(XalanMessages::XslSortCaseOrderMustBe),
						executionContext.getCurrentNode(),
						sort->getLocator());
				}
			}

			clear(scratchString);

			keys.push_back(
					NodeSortKey(
						executionContext,
						sort->getSelectPattern(),
						treatAsNumbers,
						descending,
						caseOrder,
						langString,
						*this));
		}

		selectAndSortChildren(
					executionContext,
					theTemplate,
					sorter.get(),
					executionContext.getCurrentStackFrameIndex());
	}
}



void
ElemForEach::selectAndSortChildren(
			StylesheetExecutionContext&		executionContext,
			const ElemTemplateElement*		theTemplate,
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
					executionContext.getCurrentNode(),
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
			typedef StylesheetExecutionContext::ContextNodeListPushAndPop				ContextNodeListPushAndPop;
			typedef StylesheetExecutionContext::BorrowReturnMutableNodeRefList			BorrowReturnMutableNodeRefList;

			BorrowReturnMutableNodeRefList	sortedSourceNodes(executionContext);

			*sortedSourceNodes = *sourceNodes;

			{
				SetAndRestoreCurrentStackFrameIndex		theStackFrameSetAndRestore(
						executionContext,
						selectStackFrameIndex);

				ContextNodeListPushAndPop	theContextNodeListPushAndPop(
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
	const StylesheetExecutionContext::ContextNodeListPushAndPop		theContextNodeLisPushAndPop(
				executionContext,
				sourceNodes);

	for(NodeRefListBase::size_type i = 0; i < sourceNodesCount; i++) 
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
