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
#include "ElemCopy.hpp"



#include <xercesc/sax/AttributeList.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "Constants.hpp"
#include "Stylesheet.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"
#include "StylesheetRoot.hpp"
#include "TracerEvent.hpp"



XALAN_CPP_NAMESPACE_BEGIN



ElemCopy::ElemCopy(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const AttributeListType&		atts,
			int								lineNumber,
			int								columnNumber) :
	ElemUse(constructionContext,
			stylesheetTree,
			lineNumber,
			columnNumber,
			StylesheetConstructionContext::ELEMNAME_COPY)
{
	const unsigned int	nAttrs = atts.getLength();

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar*	const	aname = atts.getName(i);

		if(!(processUseAttributeSets(constructionContext, aname, atts, i) ||
				processSpaceAttr(aname, atts, i, constructionContext) ||
				isAttrOK(aname, atts, i, constructionContext)))
		{
			constructionContext.error(
				"xsl:copy has an illegal attribute",
				0,
				this);
		}
	}
}



const XalanDOMString&
ElemCopy::getElementName() const
{
	return Constants::ELEMNAME_COPY_WITH_PREFIX_STRING;
}



void
ElemCopy::execute(StylesheetExecutionContext&	executionContext) const
{
	XalanNode* const	sourceNode = executionContext.getCurrentNode();

	assert(sourceNode != 0);

	const XalanNode::NodeType	nodeType = sourceNode->getNodeType();

	if(XalanNode::DOCUMENT_NODE != nodeType)
	{
		executionContext.cloneToResultTree(
			*sourceNode,
			nodeType,
			false,
			false,
			false,
			this);

		if(XalanNode::ELEMENT_NODE == nodeType)
		{
			ElemUse::execute(executionContext);

			executionContext.copyNamespaceAttributes(*sourceNode);

			executeChildren(executionContext);

			executionContext.endElement(c_wstr(sourceNode->getNodeName()));
		}
		else
		{
			if(0 != executionContext.getTraceListeners())
			{
				executionContext.fireTraceEvent(TracerEvent(executionContext,
					*this));
			}
		}
	}
	else
	{
		if(0 != executionContext.getTraceListeners())
		{
			executionContext.fireTraceEvent(TracerEvent(executionContext, 				
				*this));
		}

		ElemUse::execute(executionContext);

		executeChildren(executionContext);
	}  
}



XALAN_CPP_NAMESPACE_END
