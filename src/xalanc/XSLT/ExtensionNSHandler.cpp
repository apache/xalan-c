/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2003 The Apache Software Foundation.  All rights 
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
 *
 * $ Id: $
 *
 */

#include "ExtensionNSHandler.hpp"



#include <cassert>


#include <xalanc/XalanDOM/XalanElement.hpp>



#include <xalanc/PlatformSupport/StringTokenizer.hpp>
#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



#include "StylesheetExecutionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



class XSLTProcessor;



struct XSLProcessorContext
{
	const XSLTProcessor&	m_processor;
	const Stylesheet&		m_stylesheetTree;
	const XalanNode*		m_sourceTree;
	const XalanNode*		m_sourceNode;
	const XalanQName&		m_mode;

	XSLProcessorContext(
			const XSLTProcessor&	processor,
			const Stylesheet&		stylesheetTree, 
			const XalanNode*		sourceTree,
			const XalanNode*		sourceNode,
			const XalanQName&		mode) :
		m_processor(processor),
		m_stylesheetTree(stylesheetTree),
		m_sourceTree(sourceTree),
		m_sourceNode(sourceNode),
		m_mode(mode)
	{
	}
};



/**
 * Construct a new extension namespace handler for a given extension NS.
 * This doesn't do anything - just hang on to the namespace URI.
 * 
 * @param namespaceUri the extension namespace URI that I'm implementing
 */
ExtensionNSHandler::ExtensionNSHandler(const XalanDOMString&	namespaceUri) :
	ExtensionFunctionHandler(namespaceUri),
	m_elements(),
	m_componentDescLoaded(false)
{
}



ExtensionNSHandler::ExtensionNSHandler (
			const XalanDOMString& namespaceUri,
			const XalanDOMString& elemNames,
			const XalanDOMString& funcNames,
			const XalanDOMString& lang,
			const XalanDOMString& srcURL,
			const XalanDOMString& src) :
	ExtensionFunctionHandler(namespaceUri, funcNames, lang, srcURL, src),
	m_elements(),
	m_componentDescLoaded(true)
		 
{
	setElements(elemNames);
}



void
ExtensionNSHandler::setFunctions(const XalanDOMString&	funcNames)
{
    ExtensionFunctionHandler::setFunctions(funcNames);

    m_componentDescLoaded = true;
}



void
ExtensionNSHandler::setScript(
			const XalanDOMString&	lang,
			const XalanDOMString&	srcURL,
			const XalanDOMString&	scriptSrc)
{
    ExtensionFunctionHandler::setScript(lang, srcURL, scriptSrc);

    m_componentDescLoaded = true;
}



void
ExtensionNSHandler::setElements(const XalanDOMString&	elemNames)
{
    if (length(elemNames) != 0)
	{
		StringTokenizer		st(elemNames, s_tokenDelimiterCharacters, false);

		while (st.hasMoreTokens() == true)
		{
			m_elements.insert(st.nextToken()); // just stick it in there basically
		}
	
		m_componentDescLoaded = true;
	}
}



bool
ExtensionNSHandler::isElementAvailable(const XalanDOMString&	element) const
{
    return (m_elements.find(element) != m_elements.end());
}



void
ExtensionNSHandler::processElement(
			StylesheetExecutionContext&		/* executionContext */,
			const XalanDOMString&			/* localPart */,
			const XalanElement*				/* element */,
			Stylesheet&						/* stylesheetTree */, 
			const XalanNode*				/* sourceTree */,
			const XalanNode*				/* sourceNode */,
			const XalanQName&				/* mode */)
{
}



void
ExtensionNSHandler::startupComponent()
{
	ExtensionFunctionHandler::startupComponent();
}



void
ExtensionNSHandler::loadComponentDescription()
{
	assert(0);		// Don't try this in C++
}



XalanDOMString
ExtensionNSHandler::getScriptString(const XalanElement&		elem)
{
	return XalanDOMString();
}



XALAN_CPP_NAMESPACE_END
