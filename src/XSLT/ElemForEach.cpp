/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
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



#include "ElemSort.hpp"
#include "NodeSorter.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"



ElemForEach::ElemForEach(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const AttributeList&			atts,
			int								lineNumber,
			int								columnNumber) :
	ElemTemplateElement(constructionContext,
						stylesheetTree,
						lineNumber,
						columnNumber,
						Constants::ELEMNAME_FOREACH),
	m_selectPattern(0)
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
	m_selectPattern(0)
{
}



ElemForEach::~ElemForEach()
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::for_each;
#endif

	for_each(m_sortElems.begin(),
			 m_sortElems.end(),
			 DeleteFunctor<ElemSort>());
}



const XalanDOMString&
ElemForEach::getElementName() const
{
	return Constants::ELEMNAME_FOREACH_WITH_PREFIX_STRING;
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
				"There is no current node in ElemForEach::execute()!",
				sourceNode, 
				this);
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
	const SortElemsVectorType&				sortElements = getSortElems();
	const SortElemsVectorType::size_type	nChildren = sortElements.size();

	if (nChildren == 0)
	{
		ElemTemplateElement::transformSelectedChildren(
					executionContext,
					*this,
					theTemplate,
					sourceNodeContext,
					*m_selectPattern,
					0,
					selectStackFrameIndex);
	}
	else
	{
		typedef NodeSorter::NodeSortKeyVectorType					NodeSortKeyVectorType;
		typedef StylesheetExecutionContext::BorrowReturnNodeSorter	BorrowReturnNodeSorter;

		BorrowReturnNodeSorter	sorter(executionContext);

		NodeSortKeyVectorType&	keys = sorter->getSortKeys();
		assert(keys.size() == 0);

		CollectionClearGuard<NodeSortKeyVectorType>		guard(keys);

		// Reserve the space now...
		keys.reserve(nChildren);

		// Get some temporary strings to use for evaluting the AVTs...
		XPathExecutionContext::GetAndReleaseCachedString	theTemp1(executionContext);

		XalanDOMString&		langString = theTemp1.get();

		XPathExecutionContext::GetAndReleaseCachedString	theTemp2(executionContext);

		XalanDOMString&		scratchString = theTemp2.get();

		// March backwards, performing a sort on each xsl:sort child.
		// Probably not the most efficient method.
		for(SortElemsVectorType::size_type	i = 0; i < nChildren; i++)
		{
			const ElemSort* const	sort = sortElements[i];
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

			const bool	treatAsNumbers = !isEmpty(scratchString) && equals(scratchString, Constants::ATTRVAL_DATATYPE_NUMBER) ?
					true : false;

			clear(scratchString);

			avt = sort->getOrderAVT();

			if(0 != avt)
			{
				avt->evaluate(scratchString, sourceNodeContext, *this, executionContext);
			}			

			const bool	descending = !isEmpty(scratchString) && equals(scratchString, Constants::ATTRVAL_ORDER_DESCENDING) ?
					true : false;

			clear(scratchString);

			assert(sort->getSelectPattern() != 0);

			keys.push_back(
					NodeSortKey(
						executionContext, 
						*sort->getSelectPattern(), 
						treatAsNumbers, 
						descending, 
						langString, 
						*this));
		}

		ElemTemplateElement::transformSelectedChildren(
					executionContext,
					*this,
					theTemplate,
					sourceNodeContext,
					*m_selectPattern,
					sorter.get(),
					selectStackFrameIndex);
	}
}
