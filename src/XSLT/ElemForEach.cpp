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



#include <sax/AttributeList.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/STLHelper.hpp>



#include "ElemSort.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"



ElemForEach::ElemForEach(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const XalanDOMChar*				name,
			const AttributeList&			atts,
			int								lineNumber,
			int								columnNumber,
			int								xslToken) :
	ElemTemplateElement(constructionContext,
						stylesheetTree,
						name,
						lineNumber,
						columnNumber,
						xslToken),	
	m_pSelectPattern(0)
{
	if(xslToken == Constants::ELEMNAME_FOREACH)
	{
		const unsigned int	nAttrs = atts.getLength();
		
		for(unsigned int i = 0; i < nAttrs; i++)
		{
			const XalanDOMChar*	const	aname = atts.getName(i);

			if(equals(aname, Constants::ATTRNAME_SELECT))
			{
				m_pSelectPattern = constructionContext.createXPath(atts.getValue(i), *this);
			}
			else if(!(isAttrOK(aname, atts, i, constructionContext) || processSpaceAttr(aname, atts, i)))
			{
				constructionContext.error(XalanDOMString(name) + " has an illegal attribute: " + aname);
			}
		}

		if(0 == m_pSelectPattern)
		{
			constructionContext.error(XalanDOMString(name) + " requires attribute: " + Constants::ATTRNAME_SELECT);
		}
	}
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



void
ElemForEach::execute(
			StylesheetExecutionContext&		executionContext,
			XalanNode*						sourceTree,
			XalanNode*						sourceNode,
			const QName&					mode) const
{
	if (sourceNode != 0)
	{
		assert(m_pSelectPattern != 0);

		transformSelectedChildren(
			executionContext,
			getStylesheet(),
			*this,
			this,
			sourceTree,
			sourceNode,
			mode,
			m_pSelectPattern,
			Constants::ELEMNAME_FOREACH,
			executionContext.getCurrentStackFrameIndex());
	}
    else
    {
		executionContext.error("sourceNode is null in handleApplyTemplatesInstruction!",
							   sourceNode, 
							   this);
    }
}
