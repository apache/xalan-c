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
 *
 * @author <a href="mailto:david_n_bertoni@lotus.com">David N. Bertoni</a>
 */

#include "StylesheetConstructionContextDefault.hpp"



#include <algorithm>



#include <Include/STLHelper.hpp>



#include <PlatformSupport/URISupport.hpp>



#include <XPath/XObjectFactory.hpp>
#include <XPath/XPathEnvSupport.hpp>
#include <XPath/XPathFactory.hpp>
#include <XPath/XPathProcessorImpl.hpp>



#include "ElemTemplateElement.hpp"
#include "StylesheetRoot.hpp"
#include "XSLTEngineImpl.hpp"
#include "XSLTInputSource.hpp"



StylesheetConstructionContextDefault::StylesheetConstructionContextDefault(
			XSLTEngineImpl&		processor,
			XPathEnvSupport&	/* xpathEnvSupport */,
			XPathFactory&		xpathFactory) :
	StylesheetConstructionContext(),
	m_processor(processor),
	m_xpathFactory(xpathFactory),
	m_xpathProcessor(new XPathProcessorImpl),
	m_stylesheets(),
	m_tempBuffer()
{
}



StylesheetConstructionContextDefault::StylesheetConstructionContextDefault(
			XSLTEngineImpl&		processor,
			XPathFactory&		xpathFactory) :
	StylesheetConstructionContext(),
	m_processor(processor),
	m_xpathFactory(xpathFactory),
	m_xpathProcessor(new XPathProcessorImpl),
	m_stylesheets(),
	m_tempBuffer()
{
}



StylesheetConstructionContextDefault::~StylesheetConstructionContextDefault()
{
	reset();
}



void
StylesheetConstructionContextDefault::error(
			const XalanDOMString&		msg,
			const XalanNode* 			sourceNode,
			const ElemTemplateElement*	styleNode) const
{
	m_processor.error(msg, sourceNode, styleNode);
}



void
StylesheetConstructionContextDefault::error(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode,
			const Locator* 			locator) const
{
	if (locator != 0)
	{
		m_processor.error(msg, *locator, sourceNode);
	}
	else
	{
		m_processor.error(msg, sourceNode);
	}
}



void
StylesheetConstructionContextDefault::error(
			const char*					msg,
			const XalanNode* 			sourceNode,
			const ElemTemplateElement*	styleNode) const
{
	error(TranscodeFromLocalCodePage(msg), sourceNode, styleNode);
}



void
StylesheetConstructionContextDefault::error(
			const char*			msg,
			const XalanNode* 	sourceNode,
			const Locator* 		locator) const
{
	error(TranscodeFromLocalCodePage(msg), sourceNode, locator);
}



void
StylesheetConstructionContextDefault::warn(
			const XalanDOMString&		msg,
			const XalanNode* 			sourceNode,
			const ElemTemplateElement*	styleNode) const
{
	m_processor.warn(msg, sourceNode, styleNode);
}



void
StylesheetConstructionContextDefault::warn(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode,
			const Locator* 			locator) const
{
	if (locator != 0)
	{
		m_processor.warn(msg, *locator, sourceNode);
	}
	else
	{
		m_processor.warn(msg, sourceNode);
	}
}



void
StylesheetConstructionContextDefault::warn(
			const char*					msg,
			const XalanNode* 			sourceNode,
			const ElemTemplateElement*	styleNode) const
{
	warn(TranscodeFromLocalCodePage(msg), sourceNode, styleNode);
}



void
StylesheetConstructionContextDefault::warn(
			const char*			msg,
			const XalanNode* 	sourceNode,
			const Locator* 		locator) const
{
	warn(TranscodeFromLocalCodePage(msg), sourceNode, locator);
}



void
StylesheetConstructionContextDefault::message(
			const XalanDOMString&		msg,
			const XalanNode* 			sourceNode,
			const ElemTemplateElement*	styleNode) const
{
	m_processor.message(msg, sourceNode, styleNode);
}



void
StylesheetConstructionContextDefault::message(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode,
			const Locator* 			locator) const
{
	if (locator != 0)
	{
		m_processor.message(msg, *locator, sourceNode);
	}
	else
	{
		m_processor.message(msg, sourceNode);
	}
}



void
StylesheetConstructionContextDefault::message(
			const char*					msg,
			const XalanNode* 			sourceNode,
			const ElemTemplateElement*	styleNode) const
{
	message(TranscodeFromLocalCodePage(msg), sourceNode, styleNode);
}



void
StylesheetConstructionContextDefault::message(
			const char*			msg,
			const XalanNode* 	sourceNode,
			const Locator* 		locator) const
{
	message(TranscodeFromLocalCodePage(msg), sourceNode, locator);
}



void
StylesheetConstructionContextDefault::reset()
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::for_each;
#endif

	for_each(m_stylesheets.begin(),
			 m_stylesheets.end(),
			 DeleteFunctor<StylesheetRoot>());

	m_stylesheets.clear();

	m_xpathFactory.reset();
}



StylesheetRoot*
StylesheetConstructionContextDefault::create(const XalanDOMString&	theBaseIdentifier)
{
	StylesheetRoot* const	theStylesheetRoot =
		new StylesheetRoot(theBaseIdentifier, *this);

	m_stylesheets.insert(theStylesheetRoot);

	return theStylesheetRoot;
}



StylesheetRoot*
StylesheetConstructionContextDefault::create(const XSLTInputSource&		theInputSource)
{
	const XMLCh* const	theSystemID =
				theInputSource.getSystemId();

	const XalanDOMString	theBaseIdentifier =
				theSystemID == 0 ? XalanDOMString() :
				XalanDOMString(theSystemID);

	return create(theBaseIdentifier);
}



Stylesheet*
StylesheetConstructionContextDefault::create(
			StylesheetRoot&			theStylesheetRoot,
			const XalanDOMString&	theBaseIdentifier)
{
	Stylesheet* const	theStylesheet =
		new Stylesheet(
			theStylesheetRoot,
			theBaseIdentifier,
			*this);

	return theStylesheet;
}



void
StylesheetConstructionContextDefault::destroy(StylesheetRoot*	theStylesheetRoot)
{
	const StylesheetSetType::iterator	i =
		m_stylesheets.find(theStylesheetRoot);

	if (i != m_stylesheets.end())
	{
		m_stylesheets.erase(i);

		delete theStylesheetRoot;
	}
}



StylesheetConstructionContextDefault::URLAutoPtrType
StylesheetConstructionContextDefault::getURLFromString(const XalanDOMString&	urlString)
{
	return URISupport::getURLFromString(urlString);
}



XalanDOMString
StylesheetConstructionContextDefault::getURLStringFromString(const XalanDOMString&	urlString)
{
	return URISupport::getURLStringFromString(urlString);
}



StylesheetConstructionContextDefault::URLAutoPtrType
StylesheetConstructionContextDefault::getURLFromString(
			const XalanDOMString&	urlString,
			const XalanDOMString&	base)
{
	return URISupport::getURLFromString(urlString, base);
}



XalanDOMString
StylesheetConstructionContextDefault::getURLStringFromString(
			const XalanDOMString&	urlString,
			const XalanDOMString&	base)
{
	return URISupport::getURLStringFromString(urlString, base);
}



const XalanDOMString&
StylesheetConstructionContextDefault::getXSLTNamespaceURI() const
{
	return XSLTEngineImpl::getXSLNameSpaceURL();
}



XPath*
StylesheetConstructionContextDefault::createMatchPattern(
			const Locator*			locator,
			const XalanDOMString&	str,
			const PrefixResolver&	resolver)
{
	XPath* const	xpath = m_xpathFactory.create();

	// Note that we use the current locator from the
	// processing stack, and not the locator passed in.
	// This is because the locator on the stack is active,
	// during construction, while the locator passed in
	// will be used at run-time.
	m_xpathProcessor->initMatchPattern(
			*xpath,
			str,
			resolver,
			getLocatorFromStack());

	xpath->setInStylesheet(true);

	xpath->setLocator(locator);

	return xpath;
}



XPath*
StylesheetConstructionContextDefault::createMatchPattern(
			const Locator*			locator,
			const XalanDOMChar*		str,
			const PrefixResolver&	resolver)
{
	assert(str != 0);

	assign(m_tempBuffer, str);

	return createMatchPattern(locator, m_tempBuffer, resolver);
}



XPath*
StylesheetConstructionContextDefault::createXPath(
			const Locator*			locator,
			const XalanDOMString&	str,
			const PrefixResolver&	resolver)
{
	XPath* const	xpath = m_xpathFactory.create();

	// Note that we use the current locator from the
	// processing stack, and not the locator passed in.
	// This is because the locator on the stack is active,
	// during construction, while the locator passed in
	// will be used at run-time.
	m_xpathProcessor->initXPath(
			*xpath,
			str,
			resolver,
			getLocatorFromStack());

	xpath->setInStylesheet(true);

	xpath->setLocator(locator);

	return xpath;
}



XPath*
StylesheetConstructionContextDefault::createXPath(
			const Locator*			locator,
			const XalanDOMChar*		str,
			const PrefixResolver&	resolver)
{
	assert(str != 0);

	assign(m_tempBuffer, str);

	return createXPath(locator, m_tempBuffer, resolver);
}



const Locator*
StylesheetConstructionContextDefault::getLocatorFromStack() const
{
	return m_processor.getLocatorFromStack();
}



void
StylesheetConstructionContextDefault::pushLocatorOnStack(const Locator*		locator)
{
	m_processor.pushLocatorOnStack(locator);
}



void
StylesheetConstructionContextDefault::popLocatorStack()
{
	m_processor.popLocatorStack();
}



const XalanDOMString&
StylesheetConstructionContextDefault::getXalanXSLNameSpaceURL() const
{
	return XSLTEngineImpl::getXalanXSLNameSpaceURL();
}



XalanDocument*
StylesheetConstructionContextDefault::parseXML(
			const XalanDOMString&	urlString,
			DocumentHandler*		docHandler, 
			XalanDocument*			docToRegister)
{
	return m_processor.parseXML(urlString, docHandler, docToRegister);
}



int
StylesheetConstructionContextDefault::getElementToken(const XalanDOMString&	name) const
{
	return m_processor.getElementToken(name);
}



double
StylesheetConstructionContextDefault::getXSLTVersionSupported() const
{
	return XSLTEngineImpl::getXSLTVerSupported();
}
