/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2004 The Apache Software Foundation.  All rights 
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
#include "ElemMessage.hpp"



#include <xercesc/sax/AttributeList.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include "Constants.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



ElemMessage::ElemMessage(
			StylesheetConstructionContext&	constructionContext,
			Stylesheet&						stylesheetTree,
			const AttributeListType&		atts,
			int								lineNumber,
			int								columnNumber) :
	ElemTemplateElement(constructionContext,
						stylesheetTree,
						lineNumber,
						columnNumber,
						StylesheetConstructionContext::ELEMNAME_MESSAGE),
	m_terminate(false)
{
	const unsigned int	nAttrs = atts.getLength();

	for(unsigned int i = 0; i < nAttrs; i++)
	{
		const XalanDOMChar*	const	aname = atts.getName(i);

		if (equals(aname, Constants::ATTRNAME_TERMINATE) == true)
		{
			const XalanDOMChar*	const	avalue = atts.getValue(i);

			if (equals(avalue, Constants::ATTRVAL_YES) == true)
			{
				m_terminate = true;
			}
			else if (equals(avalue, Constants::ATTRVAL_NO) == false)
			{
				constructionContext.error(
					XalanMessageLoader::getMessage(XalanMessages::AttributeHasIllegalValue_1Param,Constants::ATTRNAME_TERMINATE),
					0,
					this);
			}
		}
		else if(isAttrOK(aname, atts, i, constructionContext) == false ||
				processSpaceAttr(aname, atts, i, constructionContext))
		{
			constructionContext.error(
					XalanMessageLoader::getMessage(
						XalanMessages::TemplateHasIllegalAttribute_2Param,
							Constants::ELEMNAME_MESSAGE_WITH_PREFIX_STRING.c_str(),
							aname),
					0,
					this);
		}
	}
}



const XalanDOMString&
ElemMessage::getElementName() const
{
	return Constants::ELEMNAME_MESSAGE_WITH_PREFIX_STRING;
}



void
ElemMessage::execute(StylesheetExecutionContext&	executionContext) const
{
	ElemTemplateElement::execute(executionContext);

	StylesheetExecutionContext::GetAndReleaseCachedString	theResult(executionContext);

	const XalanDOMString&	theString =
		childrenToString(
			executionContext,
			theResult.get());

	const LocatorType* const	theLocator = getLocator();

	executionContext.message(
			theString,
			executionContext.getCurrentNode(),
			theLocator);

	if (m_terminate == true)
	{
		if (theLocator != 0)
		{
			throw ElemMessageTerminateException(*theLocator, theString);
		}
		else
		{
			throw ElemMessageTerminateException(theString);
		}
	}
}



ElemMessage::ElemMessageTerminateException::ElemMessageTerminateException(const XalanDOMString&		theMessage) :
	XSLTProcessorException(
			theMessage,
			TranscodeFromLocalCodePage("ElemMessageTerminateException"))
{
}



ElemMessage::ElemMessageTerminateException::ElemMessageTerminateException(
			const LocatorType&		theLocator,
			const XalanDOMString&	theMessage) :
	XSLTProcessorException(
			theLocator,
			theMessage,
			TranscodeFromLocalCodePage("ElemMessageTerminateException"))
{
}



ElemMessage::ElemMessageTerminateException::~ElemMessageTerminateException()
{
}



XALAN_CPP_NAMESPACE_END
