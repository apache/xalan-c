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
#include "XSLTProcessorEnvSupportDefault.hpp"



#include <algorithm>



#include <xercesc/sax/EntityResolver.hpp>
#include <xercesc/util/XMLURL.hpp>



#include <xalanc/Include/STLHelper.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/URISupport.hpp>



#include <xalanc/XPath/ElementPrefixResolverProxy.hpp>
#include <xalanc/XPath/XPathExecutionContext.hpp>



#include <xalanc/XMLSupport/XMLParserLiaison.hpp>



#include "KeyTable.hpp"
#include "StylesheetRoot.hpp"
#include "XSLTProcessor.hpp"
#include "XSLTInputSource.hpp"



XALAN_CPP_NAMESPACE_BEGIN



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
		const XalanDOMString	urlText(xslURL->getURLText());

		// First see if it's already been parsed...
		XalanDocument*		theDocument =
			getSourceDocument(urlText);

		if (theDocument == 0)
		{
			XMLParserLiaison&	parserLiaison =
				m_processor->getXMLParserLiaison();

			EntityResolverType* const	theResolver = 
				parserLiaison.getEntityResolver();

			if (theResolver == 0)
			{
				const XSLTInputSource	inputSource(c_wstr(urlText));

				theDocument = parserLiaison.parseXMLStream(inputSource);
			}
			else
			{
				const XalanAutoPtr<InputSourceType>		resolverInputSource =
					theResolver->resolveEntity(0, c_wstr(urlText));

				if (resolverInputSource.get() != 0)
				{
					theDocument = parserLiaison.parseXMLStream(*resolverInputSource.get());
				}
				else
				{
					const XSLTInputSource	inputSource(c_wstr(urlText));

					theDocument = parserLiaison.parseXMLStream(inputSource);
				}
			}

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
			const LocatorType*				locator) const
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
			eSource						/* where */,
			eClassification				classification,
			const XalanNode*			sourceNode,
			const ElemTemplateElement*	styleNode,
			const XalanDOMString&		msg,
			const XalanDOMChar*			/* uri */,
			int							/* lineNo */,
			int							/* charOffset */) const
{
	if (classification == XPathEnvSupport::eError)
	{
		m_processor->error(
					msg,
					sourceNode,
					styleNode);

		return true;
	}
	else if (classification == XPathEnvSupport::eWarning)
	{
		m_processor->warn(
					msg,
					sourceNode,
					styleNode);

		return false;
	}
	else
	{
		m_processor->message(
					msg,
					sourceNode,
					styleNode);

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
					sourceNode);

		return true;
	}
	else if (classification == XPathEnvSupport::eWarning)
	{
		m_processor->warn(
					msg,
					sourceNode);

		return false;
	}
	else
	{
		m_processor->message(
					msg,
					sourceNode);

		return false;
	}
}



XALAN_CPP_NAMESPACE_END
