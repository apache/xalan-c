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
#include "XSLTProcessorEnvSupportDefault.hpp"



#include <algorithm>



#include <util/XMLURL.hpp>



#include <Include/STLHelper.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/URISupport.hpp>



#include <XPath/ElementPrefixResolverProxy.hpp>
#include <XPath/XPathExecutionContext.hpp>



#include <XMLSupport/XMLParserLiaison.hpp>



#include "KeyTable.hpp"
#include "StylesheetRoot.hpp"
#include "XSLTProcessor.hpp"
#include "XSLTInputSource.hpp"



XSLTProcessorEnvSupportDefault::XSLTProcessorEnvSupportDefault(XSLTProcessor*	theProcessor) :
	XSLTProcessorEnvSupport(),
	m_defaultSupport(),
	m_processor(theProcessor)
{
}



XSLTProcessorEnvSupportDefault::~XSLTProcessorEnvSupportDefault()
{
	reset();
}



void
XSLTProcessorEnvSupportDefault::installExternalFunctionGlobal(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	functionName,
			const Function&			function)
{
	XPathEnvSupportDefault::installExternalFunctionGlobal(theNamespace, functionName, function);
}



void
XSLTProcessorEnvSupportDefault::uninstallExternalFunctionGlobal(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	functionName)
{
	XPathEnvSupportDefault::uninstallExternalFunctionGlobal(theNamespace, functionName);
}



void
XSLTProcessorEnvSupportDefault::installExternalFunctionLocal(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	functionName,
			const Function&			function)
{
	m_defaultSupport.installExternalFunctionLocal(theNamespace, functionName, function);
}



void
XSLTProcessorEnvSupportDefault::uninstallExternalFunctionLocal(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	functionName)
{
	m_defaultSupport.uninstallExternalFunctionLocal(theNamespace, functionName);
}



void
XSLTProcessorEnvSupportDefault::reset()
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::for_each;
#endif

	m_defaultSupport.reset();
}



XalanDocument*
XSLTProcessorEnvSupportDefault::parseXML(
		const XalanDOMString&	urlString,
		const XalanDOMString&	base)
{
	if (m_processor == 0)
	{
		return m_defaultSupport.parseXML(urlString, base);
	}
	else
	{
		typedef URISupport::URLAutoPtrType	URLAutoPtrType;

		// $$$ ToDo: we should re-work this code to only use
		// XMLRUL when necessary.
		const URLAutoPtrType	xslURL =
			URISupport::getURLFromString(urlString, base);

		// $$$ ToDo: Explicit XalanDOMString constructor
		const XalanDOMString	urlText(XalanDOMString(xslURL->getURLText()));

		// First see if it's already been parsed...
		XalanDocument*		theDocument =
			getSourceDocument(urlText);

		if (theDocument == 0)
		{
			XMLParserLiaison&	parserLiaison =
				m_processor->getXMLParserLiaison();

			XSLTInputSource		inputSource(c_wstr(urlText));

			theDocument = parserLiaison.parseXMLStream(inputSource);

			if (theDocument != 0)
			{
				setSourceDocument(urlText, theDocument);
			}
		}

		return theDocument;
	}
}



XalanDocument*
XSLTProcessorEnvSupportDefault::getSourceDocument(const XalanDOMString&		theURI) const
{
	return m_defaultSupport.getSourceDocument(theURI);
}



void
XSLTProcessorEnvSupportDefault::setSourceDocument(
			const XalanDOMString&	theURI,
			XalanDocument*			theDocument)
{
	m_defaultSupport.setSourceDocument(theURI, theDocument);
}



XalanDOMString
XSLTProcessorEnvSupportDefault::findURIFromDoc(const XalanDocument*		owner) const
{
	return m_defaultSupport.findURIFromDoc(owner);
}



XalanDocument*
XSLTProcessorEnvSupportDefault::getDOMFactory() const
{
	if (m_processor == 0)
	{
		return m_defaultSupport.getDOMFactory();
	}
	else
	{
		return m_processor->getDOMFactory();
	}
}



bool
XSLTProcessorEnvSupportDefault::elementAvailable(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	functionName) const
{
	return m_defaultSupport.elementAvailable(theNamespace,
											 functionName);
}



bool
XSLTProcessorEnvSupportDefault::functionAvailable(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	functionName) const
{
	return m_defaultSupport.functionAvailable(theNamespace,
											  functionName);
}



XObjectPtr
XSLTProcessorEnvSupportDefault::extFunction(
			XPathExecutionContext&			executionContext,
			const XalanDOMString&			theNamespace,
			const XalanDOMString&			functionName,
			XalanNode*						context,
			const XObjectArgVectorType&		argVec,
			const Locator*					locator) const
{
	return m_defaultSupport.extFunction(
			executionContext,
			theNamespace,
			functionName,
			context,
			argVec,
			locator);
}



bool
XSLTProcessorEnvSupportDefault::problem(
			eSource					/* where */,
			eClassification			classification,
			const XalanNode*		styleNode,
			const XalanNode*		sourceNode,
			const XalanDOMString&	msg,
			const XalanDOMChar*		/* uri */,
			int						/* lineNo */,
			int						/* charOffset */) const
{
	if (classification == XPathEnvSupport::eError)
	{
		m_processor->error(
					msg,
					styleNode,
					sourceNode);

		return true;
	}
	else if (classification == XPathEnvSupport::eWarning)
	{
		m_processor->warn(
					msg,
					styleNode,
					sourceNode);

		return false;
	}
	else
	{
		m_processor->message(
					msg,
					styleNode,
					sourceNode);

		return false;
	}
}



bool
XSLTProcessorEnvSupportDefault::problem(
			eSource					/* where */,
			eClassification			classification,
			const PrefixResolver*	/* resolver */,
			const XalanNode*		sourceNode,
			const XalanDOMString&	msg,
			const XalanDOMChar*		/* uri */,
			int						/* lineNo */,
			int						/* charOffset */) const
{
	if (classification == XPathEnvSupport::eError)
	{
		m_processor->error(
					msg,
					0,
					sourceNode);

		return true;
	}
	else if (classification == XPathEnvSupport::eWarning)
	{
		m_processor->warn(
					msg,
					0,
					sourceNode);

		return false;
	}
	else
	{
		m_processor->message(
					msg,
					0,
					sourceNode);

		return false;
	}
}
