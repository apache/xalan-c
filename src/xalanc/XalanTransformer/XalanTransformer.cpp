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
#include "XalanTransformer.hpp"



#include <algorithm>
#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#else
#include <iostream>
#endif



#include <xercesc/sax/SAXParseException.hpp>



#include <xalanc/XalanDOM/XalanDOMException.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/DOMStringPrintWriter.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>
#include <xalanc/PlatformSupport/XalanOutputStreamPrintWriter.hpp>



#include <xalanc/XPath/XObjectFactoryDefault.hpp>
#include <xalanc/XPath/XPathFactoryBlock.hpp>



#include <xalanc/XSLT/StylesheetConstructionContextDefault.hpp>
#include <xalanc/XSLT/StylesheetExecutionContextDefault.hpp>
#include <xalanc/XSLT/StylesheetRoot.hpp>
#include <xalanc/XSLT/XSLTEngineImpl.hpp>
#include <xalanc/XSLT/XSLTInit.hpp>
#include <xalanc/XSLT/XSLTProcessorEnvSupportDefault.hpp>



#include <xalanc/XalanSourceTree/XalanSourceTreeDOMSupport.hpp>
#include <xalanc/XalanSourceTree/XalanSourceTreeParserLiaison.hpp>



#include <xalanc/XalanExtensions/XalanExtensions.hpp>



#include <xalanc/XalanEXSLT/XalanEXSLTCommon.hpp>
#include <xalanc/XalanEXSLT/XalanEXSLTDynamic.hpp>
#include <xalanc/XalanEXSLT/XalanEXSLTMath.hpp>
#include <xalanc/XalanEXSLT/XalanEXSLTSet.hpp>
#include <xalanc/XalanEXSLT/XalanEXSLTString.hpp>
#include <xalanc/XalanEXSLT/XalanEXSLTDateTime.hpp>



//#define XALAN_USE_ICU
#if defined(XALAN_USE_ICU)
#include <xalanc/ICUBridge/ICUBridgeCleanup.hpp>
#include <xalanc/ICUBridge/ICUBridgeCollationCompareFunctor.hpp>
#include <xalanc/ICUBridge/ICUFormatNumberFunctor.hpp>
#endif



#include "XalanCompiledStylesheetDefault.hpp"
#include "XalanDefaultDocumentBuilder.hpp"
#include "XalanDefaultParsedSource.hpp"
#include "XalanTransformerOutputStream.hpp"
#include "XalanTransformerProblemListener.hpp"
#include "XercesDOMParsedSource.hpp"



XALAN_CPP_NAMESPACE_BEGIN


const XSLTInputSource*	XalanTransformer::s_emptyInputSource = 0;

const XSLTInit*			XalanTransformer::s_xsltInit = 0;



XalanTransformer::XalanTransformer():
	m_compiledStylesheets(),
	m_parsedSources(),
	m_paramPairs(),
	m_functionPairs(),
	m_traceListeners(),
	m_errorMessage(1, '\0'),
	m_useValidation(false),
	m_entityResolver(0),
	m_errorHandler(0),
	m_externalSchemaLocation(),
	m_externalNoNamespaceSchemaLocation(),
	m_problemListener(0),
#if defined(XALAN_NO_STD_NAMESPACE)
	m_warningStream(&cerr),
#else
	m_warningStream(&std::cerr),
#endif
	m_outputEncoding(),
	m_stylesheetExecutionContext(new StylesheetExecutionContextDefault)
{
#if defined(XALAN_USE_ICU)
	// Create a collation function for the ICU, and have it
	// cache collators...
	XalanAutoPtr<ICUBridgeCollationCompareFunctor>	theICUFunctor(new ICUBridgeCollationCompareFunctor(true));

	m_stylesheetExecutionContext->installCollationCompareFunctor(theICUFunctor.get());

	XalanAutoPtr<ICUFormatNumberFunctor>  theFormatNumberFunctor(new ICUFormatNumberFunctor());
	m_stylesheetExecutionContext->installFormatNumberFunctor(theFormatNumberFunctor.get());
	theICUFunctor.release();
	theFormatNumberFunctor.release();

#endif
}



XalanTransformer::~XalanTransformer()
{
	XALAN_USING_STD(for_each)

	// Clean up all entries in the compliledStylesheets vector.
	for_each(m_compiledStylesheets.begin(),
			 m_compiledStylesheets.end(),
			 DeleteFunctor<XalanCompiledStylesheet>());

	// Clean up all entries in the compliledStylesheets vector.
	for_each(m_parsedSources.begin(),
			 m_parsedSources.end(),
			 DeleteFunctor<XalanParsedSource>());

	for (FunctionParamPairVectorType::size_type i = 0; i < m_functionPairs.size(); ++i)
	{
		delete m_functionPairs[i].second;
	}

#if defined(XALAN_USE_ICU)
	// Uninstall the ICU collation compare functor, and destroy it...
	delete m_stylesheetExecutionContext->uninstallCollationCompareFunctor();
	delete m_stylesheetExecutionContext->uninstallFormatNumberFunctor();
#endif

	delete m_stylesheetExecutionContext;
}



void
XalanTransformer::initialize()
{
	// Initialize Xalan. 
	XalanAutoPtr<XSLTInit>			initGuard(new XSLTInit);
	XalanAutoPtr<XSLTInputSource>	inputSourceGuard(new XSLTInputSource);

	XalanExtensionsInstaller::installGlobal();
	XalanEXSLTCommonFunctionsInstaller::installGlobal();
	XalanEXSLTDynamicFunctionsInstaller::installGlobal();
	XalanEXSLTMathFunctionsInstaller::installGlobal();
	XalanEXSLTSetFunctionsInstaller::installGlobal();
	XalanEXSLTStringFunctionsInstaller::installGlobal();
	XalanEXSLTDateTimeFunctionsInstaller::installGlobal();

	s_xsltInit = initGuard.release();
	s_emptyInputSource = inputSourceGuard.release();
}



void
XalanTransformer::terminate()
{
	XalanExtensionsInstaller::uninstallGlobal();
	XalanEXSLTCommonFunctionsInstaller::uninstallGlobal();
	XalanEXSLTDynamicFunctionsInstaller::uninstallGlobal();
	XalanEXSLTMathFunctionsInstaller::uninstallGlobal();
	XalanEXSLTSetFunctionsInstaller::uninstallGlobal();
	XalanEXSLTStringFunctionsInstaller::uninstallGlobal();
	XalanEXSLTDateTimeFunctionsInstaller::uninstallGlobal();

#if defined(XALAN_USE_ICU)
	XPath::uninstallFunction(XPathFunctionTable::s_formatNumber);
#endif

	// Terminate Xalan and release memory.
#if defined(XALAN_CANNOT_DELETE_CONST)
	delete (XSLTInputSource*) s_emptyInputSource;
	delete (XSLTInit*) s_xsltInit;
#else
	delete s_emptyInputSource;
	delete s_xsltInit;
#endif

	s_emptyInputSource = 0;
	s_xsltInit = 0;

#if defined(XALAN_USE_ICU)
	ICUBridgeCleanup::cleanup();
#endif
}



void
XalanTransformer::ICUCleanUp()
{
#if defined(XALAN_USE_ICU)
	ICUBridgeCleanup::cleanup();
#endif
}



static void
addTraceListeners(
			 const XalanTransformer::TraceListenerVectorType&	theTraceListeners,
			 XSLTEngineImpl&									theEngine)
{
	if (theTraceListeners.empty() == false)
	{
		typedef XalanTransformer::TraceListenerVectorType	TraceListenerVectorType;

		TraceListenerVectorType::const_iterator 	theEnd = theTraceListeners.end();

		for(TraceListenerVectorType::const_iterator i = theTraceListeners.begin(); i != theEnd; ++i)
		{
			theEngine.addTraceListener(*i);
		}

		theEngine.setTraceSelects(true);
	}
}



int
XalanTransformer::transform(
			const XSLTInputSource&			theInputSource, 
			const XalanCompiledStylesheet*	theCompiledStylesheet,
			const XSLTResultTarget& 		theResultTarget)
{
	const XalanParsedSource*	theParsedSource = 0;
 
	const int	theResult = parseSource(theInputSource, theParsedSource);
 
	if (theResult != 0)
	{
		return theResult;
	}
	else
	{
		assert(theParsedSource != 0);
 
		// Make sure the parsed source is destroyed when
		// the transformation is finished...
		EnsureDestroyParsedSource	theGuard(*this, theParsedSource);

		// Do the transformation...
		return transform(
						*theParsedSource,
						theCompiledStylesheet,
						theResultTarget);
	}
}



int
XalanTransformer::transform(
			const XSLTInputSource&		theInputSource, 
			const XSLTInputSource&		theStylesheetSource,
			const XSLTResultTarget& 	theResultTarget)
{
	// Parse the source document.
	const XalanParsedSource*	theParsedSource = 0;

	const int	theResult = parseSource(theInputSource, theParsedSource);

	if (theResult != 0)
	{
		return theResult;
	}
	else
	{
		assert(theParsedSource != 0);

		// Make sure the parsed source is destroyed when
		// the transformation is finished...
		EnsureDestroyParsedSource	theGuard(*this, theParsedSource);

		// Do the transformation...
		return transform(
						*theParsedSource,
						theStylesheetSource,
						theResultTarget);
	}
}



int
XalanTransformer::transform(
			const XSLTInputSource&	theInputSource, 
			const XSLTInputSource&	theStylesheetSource,
			void*					theOutputHandle, 
			XalanOutputHandlerType	theOutputHandler,
			XalanFlushHandlerType	theFlushHandler)
{
	// Set to output target to the callback 
	XalanTransformerOutputStream	theOutputStream(theOutputHandle, theOutputHandler, theFlushHandler);

	XalanOutputStreamPrintWriter	thePrintWriter(theOutputStream);

	XSLTResultTarget				theResultTarget(&thePrintWriter);

	// Do the transformation...
	return transform(
					theInputSource, 
					theStylesheetSource,
					theResultTarget);
}



int
XalanTransformer::transform(
			const XalanParsedSource&		theParsedSource, 
			const XalanCompiledStylesheet*	theCompiledStylesheet,
			void*							theOutputHandle, 
			XalanOutputHandlerType			theOutputHandler,
			XalanFlushHandlerType			theFlushHandler)
{
	// Set to output target to the callback 
	XalanTransformerOutputStream	theOutputStream(theOutputHandle, theOutputHandler, theFlushHandler);

	XalanOutputStreamPrintWriter	thePrintWriter(theOutputStream);

	XSLTResultTarget				theResultTarget(&thePrintWriter);

	// Do the transformation...
	return transform(
					theParsedSource,
					theCompiledStylesheet,
					theResultTarget);
}



int
XalanTransformer::transform(
			const XSLTInputSource&		theInputSource, 		
			void*						theOutputHandle, 
			XalanOutputHandlerType		theOutputHandler,
			XalanFlushHandlerType		theFlushHandler)
{
	assert(s_emptyInputSource != 0);

	// Set to output target to the callback 
	XalanTransformerOutputStream	theOutputStream(theOutputHandle, theOutputHandler, theFlushHandler);

	XalanOutputStreamPrintWriter	thePrintWriter(theOutputStream);

	XSLTResultTarget				theResultTarget(&thePrintWriter);

	// Do the transformation...
	return transform(
					theInputSource, 
					*s_emptyInputSource,
					theResultTarget);
}



XALAN_USING_XERCES(XMLException)
XALAN_USING_XERCES(SAXParseException)
XALAN_USING_XERCES(SAXException)



void
FormatSAXParseException(
			const SAXParseException&	theException,
			XalanDOMString&				theErrorMessage)
{
	assert(theException.getMessage() != 0);

	const XalanDOMChar	theDummy = 0;

	const XalanDOMChar* const	theSystemID = theException.getSystemId();

	XSLException::defaultFormat(
				theException.getMessage(),
				theSystemID == 0 ? &theDummy : theSystemID,
				theException.getLineNumber(),
				theException.getColumnNumber(),
				c_wstr(XALAN_STATIC_UCODE_STRING("SAXParseException")),
				theErrorMessage);
}



void
FormatSAXParseException(
			const SAXParseException&	theException,
			CharVectorType&				theErrorMessage)
{
	XalanDOMString	theBuffer;

	FormatSAXParseException(
				theException,
				theBuffer);

	TranscodeToLocalCodePage(theBuffer, theErrorMessage, true);
}



void
FormatXalanDOMException(
			const XalanDOMException&	theException,
			CharVectorType&				theErrorMessage)
{
	XalanDOMString	theBuffer;

	append(theBuffer, LongToDOMString(long(theException.getExceptionCode())));

	append(theBuffer, XalanDOMChar(XalanUnicode::charFullStop));
	XalanDOMString	theMessage = XalanMessageLoader::getMessage(XalanMessages::XalanDOMExceptionCaught_1Param,theBuffer);

	TranscodeToLocalCodePage(theMessage, theErrorMessage, true);
}



int
XalanTransformer::compileStylesheet(
			const XSLTInputSource&				theStylesheetSource,
			const XalanCompiledStylesheet*& 	theCompiledStylesheet)
{
	// Clear the error message.
	m_errorMessage.resize(1, '\0');

	// Store error messages from problem listener.
	XalanDOMString	theErrorMessage;

	int 	theResult = 0;

	try
	{
		// Create some support objects that are necessary for running the processor...
		XalanSourceTreeDOMSupport		theDOMSupport;

		XalanSourceTreeParserLiaison	theParserLiaison(theDOMSupport);

		theParserLiaison.setEntityResolver(m_entityResolver);
		theParserLiaison.setErrorHandler(m_errorHandler);

		// Hook the two together...
		theDOMSupport.setParserLiaison(&theParserLiaison);

		// Create some more support objects...
		XSLTProcessorEnvSupportDefault	theXSLTProcessorEnvSupport;

		XObjectFactoryDefault	theXObjectFactory;

		XPathFactoryBlock 		theXPathFactory;

		// Create a processor...
		XSLTEngineImpl	theProcessor(
				theParserLiaison,
				theXSLTProcessorEnvSupport,
				theDOMSupport,
				theXObjectFactory,
				theXPathFactory);

		// Create a problem listener and send output to a XalanDOMString.
		DOMStringPrintWriter	thePrintWriter(theErrorMessage);

		XalanTransformerProblemListener		theProblemListener(m_warningStream, &thePrintWriter);

		if (m_problemListener == 0)
		{
			theProcessor.setProblemListener(&theProblemListener);
		}
		else
		{
			theProcessor.setProblemListener(m_problemListener);
		}

		// Allocate the memory now, to avoid leaking if push_back() fails.
		m_compiledStylesheets.reserve(m_compiledStylesheets.size() + 1);

		// Create a new XalanCompiledStylesheet.
		theCompiledStylesheet =
			new XalanCompiledStylesheetDefault(
						theStylesheetSource,
						theProcessor,
						m_errorHandler,
						m_entityResolver);

		// Store it in a vector.
		m_compiledStylesheets.push_back(theCompiledStylesheet);
	}
	catch(const XSLException&	e)
	{
		if (length(theErrorMessage) != 0)
		{
			TranscodeToLocalCodePage(theErrorMessage, m_errorMessage, true);
		}
		else
		{
			TranscodeToLocalCodePage(e.getMessage(), m_errorMessage, true);
		}

		theResult = -1;
	}
	catch(const SAXParseException&	e)
	{
		if (length(theErrorMessage) != 0)
		{
			TranscodeToLocalCodePage(theErrorMessage, m_errorMessage, true);
		}
		else
		{
			FormatSAXParseException(e, m_errorMessage);
		}

		theResult = -2;
	}
	catch(const SAXException&	e)
	{
		if (length(theErrorMessage) != 0)
		{
			TranscodeToLocalCodePage(theErrorMessage, m_errorMessage, true);
		}
		else
		{
			TranscodeToLocalCodePage(e.getMessage(), m_errorMessage, true);
		}

		theResult = -2;
	}
	catch(const XMLException&	e)
	{
		if (length(theErrorMessage) != 0)
		{
			TranscodeToLocalCodePage(theErrorMessage, m_errorMessage, true);
		}
		else
		{
			TranscodeToLocalCodePage(e.getMessage(), m_errorMessage, true);
		}

		theResult = -3;
	}
	catch(const XalanDOMException&	e)
	{
		if (length(theErrorMessage) != 0)
		{
			TranscodeToLocalCodePage(theErrorMessage, m_errorMessage, true);
		}
		else
		{
			XalanDOMString	theBuffer;
			append(theBuffer, LongToDOMString(long(e.getExceptionCode())));

			append(theBuffer, XalanDOMChar(XalanUnicode::charFullStop));

			XalanDOMString theMessage = XalanMessageLoader::getMessage(XalanMessages::XalanDOMExceptionCaught_1Param,theBuffer);

			TranscodeToLocalCodePage(theMessage, m_errorMessage, true);
		}

		theResult = -4;
	}

	return theResult;
}



int
XalanTransformer::destroyStylesheet(const XalanCompiledStylesheet*	theStylesheet)
{
	XALAN_USING_STD(find)

	const CompiledStylesheetPtrVectorType::iterator 	i =
		find(
			m_compiledStylesheets.begin(),
			m_compiledStylesheets.end(),
			theStylesheet);

	if (i == m_compiledStylesheets.end())
	{
		XalanDOMString theStylesheetErrorMessage = XalanMessageLoader::getMessage(XalanMessages::InvalidCompiledStylesheetProvided);

		try
		{
			m_errorMessage = theStylesheetErrorMessage.transcode();
		}
		catch(...)
		{
			const char* const	theParsedSourceErrorMessage =
				"Failure in fetching/transcoding of an error message";
			
			const XalanDOMString::size_type		theLength =
				length(theParsedSourceErrorMessage);
			
			m_errorMessage.resize(theLength + 1, '\0');
			
#if defined(XALAN_STRICT_ANSI_HEADERS)
			std::strncpy(&*m_errorMessage.begin(), theParsedSourceErrorMessage, theLength);
#else
			strncpy(&*m_errorMessage.begin(), theParsedSourceErrorMessage, theLength);
#endif
		}

		return -1;
	}
	else
	{
		m_compiledStylesheets.erase(i);

#if defined(XALAN_CANNOT_DELETE_CONST)
		delete (XalanCompiledStylesheet*) theStylesheet;
#else
		delete theStylesheet;
#endif

		return 0;
	}
}



int
XalanTransformer::parseSource(
			const XSLTInputSource&		theInputSource,
			const XalanParsedSource*&	theParsedSource,
			bool						useXercesDOM)
{
	// Clear the error message.
	m_errorMessage.clear();
	m_errorMessage.push_back(0);

	int theResult = 0;

	try
	{
		// Allocate the memory now, to avoid leaking if push_back() fails.
		m_parsedSources.reserve(m_parsedSources.size() + 1);

		if(useXercesDOM == true)
		{
			theParsedSource =
				new XercesDOMParsedSource(
						theInputSource,
						m_useValidation,
						m_errorHandler,
						m_entityResolver,
						getExternalSchemaLocation(),
						getExternalNoNamespaceSchemaLocation());
		}
		else
		{
			theParsedSource =
				new XalanDefaultParsedSource(
						theInputSource,
						m_useValidation,
						m_errorHandler,
						m_entityResolver,
						getExternalSchemaLocation(),
						getExternalNoNamespaceSchemaLocation());
		}

		// Store it in a vector.
		m_parsedSources.push_back(theParsedSource);
	}
	catch(const XSLException&	e)
	{
		TranscodeToLocalCodePage(e.defaultFormat(), m_errorMessage, true);

		theResult = -1;
	}
	catch(const SAXParseException&	e)
	{
		FormatSAXParseException(e, m_errorMessage);

		theResult = -2;
	}
	catch(const SAXException&	e)
	{
		TranscodeToLocalCodePage(e.getMessage(), m_errorMessage, true);

		theResult = -2;
	}
	catch(const XMLException&	e)
	{
		TranscodeToLocalCodePage(e.getMessage(), m_errorMessage, true);

		theResult = -3;
	}
	catch(const XalanDOMException&	e)
	{
		FormatXalanDOMException(e, m_errorMessage);

		theResult = -4;
	}

	return theResult;
}



int
XalanTransformer::destroyParsedSource(const XalanParsedSource*	theParsedSource)
{
	XALAN_USING_STD(find)

	const ParsedSourcePtrVectorType::iterator	i =
		find(
			m_parsedSources.begin(),
			m_parsedSources.end(),
			theParsedSource);

	if (i == m_parsedSources.end())
	{
		XalanDOMString theParserErrorMessage = XalanMessageLoader::getMessage(XalanMessages::InvalidParsedSourceProvided);

		try
		{
			m_errorMessage = theParserErrorMessage.transcode();
		}
		catch(...)
		{
			const char* const	theParsedSourceErrorMessage =
				"Failure in fetching/transcoding of an error message";
			
			const XalanDOMString::size_type		theLength =
				length(theParsedSourceErrorMessage);
			
			m_errorMessage.resize(theLength + 1, '\0');
			
#if defined(XALAN_STRICT_ANSI_HEADERS)
			std::strncpy(&*m_errorMessage.begin(), theParsedSourceErrorMessage, theLength);
#else
			strncpy(&*m_errorMessage.begin(), theParsedSourceErrorMessage, theLength);
#endif
		}
		return -1;
	}
	else
	{
		m_parsedSources.erase(i);

#if defined(XALAN_CANNOT_DELETE_CONST)
		delete (XalanParsedSource*) theParsedSource;
#else
		delete theParsedSource;
#endif

		return 0;
	}
}



void
XalanTransformer::setStylesheetParam(
			const XalanDOMString&	key,
			const XalanDOMString&	expression)
{
	// Store the stylesheet parameter in a vector.
	m_paramPairs.push_back(ParamPairType(key,  expression));
}



void
XalanTransformer::setStylesheetParam(
			const char*		key,
			const char*		expression)
{
	setStylesheetParam(XalanDOMString(key), XalanDOMString(expression));
}



bool
XalanTransformer::removeTraceListener(TraceListener*	theTraceListener)
{
	XALAN_USING_STD(find)

	const TraceListenerVectorType::iterator 	i =
		find(
			m_traceListeners.begin(),
			m_traceListeners.end(),
			theTraceListener);

	if (i == m_traceListeners.end())
	{
		return false;
	}
	else
	{
		m_traceListeners.erase(i);

		return true;
	}
}



XalanDocumentBuilder*
XalanTransformer::createDocumentBuilder(const XalanDOMString&	theURI)
{
	m_parsedSources.reserve(m_parsedSources.size() + 1);

	XalanDocumentBuilder* const 	theNewBuilder = new XalanDefaultDocumentBuilder(theURI);

	m_parsedSources.push_back(theNewBuilder);

	return theNewBuilder;
}



void
XalanTransformer::destroyDocumentBuilder(XalanDocumentBuilder*	theDocumentBuilder)
{
	destroyParsedSource(theDocumentBuilder);
}



void
XalanTransformer::installExternalFunction(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	functionName,
			const Function& 		function)
{
	m_functionPairs.push_back(FunctionPairType(XalanQNameByValue(theNamespace, functionName), function.clone()));
}



void
XalanTransformer::installExternalFunctionGlobal(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	functionName,
			const Function& 		function)
{
	XSLTProcessorEnvSupportDefault::installExternalFunctionGlobal(
			theNamespace,
			functionName,
			function);
}



void
XalanTransformer::uninstallExternalFunction(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	functionName)
{
	for (FunctionParamPairVectorType::size_type i = 0; i < m_functionPairs.size(); ++i)
	{
		if(XalanQNameByReference(theNamespace, functionName).equals(m_functionPairs[i].first))
		{
			delete m_functionPairs[i].second;

			m_functionPairs.erase(m_functionPairs.begin() + i); 	
		}
	}	
}



void
XalanTransformer::uninstallExternalFunctionGlobal(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	functionName)
{
	XSLTProcessorEnvSupportDefault::uninstallExternalFunctionGlobal(
			theNamespace,
			functionName);
}



int
XalanTransformer::getIndent() const
{
	return m_stylesheetExecutionContext->getIndent();
}



void
XalanTransformer::setIndent(int	indentAmount)
{
	m_stylesheetExecutionContext->setIndent(indentAmount);
}



XalanTransformer::eEscapeURLs
XalanTransformer::getEscapeURLs() const
{
	eEscapeURLs		escapeValue = eEscapeURLsDefault;

	switch(m_stylesheetExecutionContext->getEscapeURLs())
	{
	case StylesheetExecutionContextDefault::eEscapeURLsDefault:
		break;

	case StylesheetExecutionContextDefault::eEscapeURLsNo:
		escapeValue = eEscapeURLsNo;
		break;

	case StylesheetExecutionContextDefault::eEscapeURLsYes:
		escapeValue = eEscapeURLsYes;
		break;

	default:
		assert(false);
		break;
	}

	return escapeValue;
}



void
XalanTransformer::setEscapeURLs(eEscapeURLs		value)
{
	StylesheetExecutionContextDefault::eEscapeURLs	escapeValue =
		StylesheetExecutionContextDefault::eEscapeURLsDefault;

	switch(value)
	{
	case eEscapeURLsDefault:
		break;

	case eEscapeURLsNo:
		escapeValue = StylesheetExecutionContextDefault::eEscapeURLsNo;
		break;

	case eEscapeURLsYes:
		escapeValue = StylesheetExecutionContextDefault::eEscapeURLsYes;
		break;

	default:
		assert(false);
		break;
	}

	m_stylesheetExecutionContext->setEscapeURLs(escapeValue);
}



XalanTransformer::eOmitMETATag
XalanTransformer::getOmitMETATag() const
{
	eOmitMETATag	omitValue = eOmitMETATagDefault;

	switch(m_stylesheetExecutionContext->getOmitMETATag())
	{
	case StylesheetExecutionContextDefault::eOmitMETATagDefault:
		break;

	case StylesheetExecutionContextDefault::eOmitMETATagNo:
		omitValue = eOmitMETATagNo;
		break;

	case StylesheetExecutionContextDefault::eOmitMETATagYes:
		omitValue = eOmitMETATagYes;
		break;

	default:
		assert(false);
		break;
	}

	return omitValue;
}



void
XalanTransformer::setOmitMETATag(eOmitMETATag		value)
{
	StylesheetExecutionContextDefault::eOmitMETATag		omitValue =
		StylesheetExecutionContextDefault::eOmitMETATagDefault;

	switch(value)
	{
	case eOmitMETATagDefault:
		break;

	case eOmitMETATagNo:
		omitValue = StylesheetExecutionContextDefault::eOmitMETATagNo;
		break;

	case eOmitMETATagYes:
		omitValue = StylesheetExecutionContextDefault::eOmitMETATagYes;
		break;

	default:
		assert(false);
		break;
	}

	m_stylesheetExecutionContext->setOmitMETATag(omitValue);
}



void
XalanTransformer::reset()
{
	try
	{
		// Reset objects.
		m_stylesheetExecutionContext->setXPathEnvSupport(0);

		m_stylesheetExecutionContext->setDOMSupport(0);
		
		m_stylesheetExecutionContext->setXObjectFactory(0);
		
		m_stylesheetExecutionContext->setXSLTProcessor(0);

		m_stylesheetExecutionContext->reset();

		// Clear the ParamPairVectorType.
		m_paramPairs.clear();
	}
	catch(...)
	{
	}
}



XalanTransformer::EnsureReset::~EnsureReset()
{
	m_transformer.m_stylesheetExecutionContext->reset();

	m_transformer.reset();
}



int
XalanTransformer::doTransform(
			const XalanParsedSource&		theParsedXML,
			const XalanCompiledStylesheet*	theCompiledStylesheet,
			const XSLTInputSource*			theStylesheetSource,
			const XSLTResultTarget&			theResultTarget)
{
	int 	theResult = 0;

	// Clear the error message.
	m_errorMessage.resize(1, '\0');

	// Store error messages from problem listener.
	XalanDOMString	theErrorMessage;

	try
	{
		XalanDocument* const	theSourceDocument = theParsedXML.getDocument();
		assert(theSourceDocument != 0);

		// Create the helper object that is necessary for running the processor...
		XalanAutoPtr<XalanParsedSourceHelper>	theHelper(theParsedXML.createHelper());
		assert(theHelper.get() != 0);

		DOMSupport& 		theDOMSupport = theHelper->getDOMSupport();

		XMLParserLiaison&	theParserLiaison = theHelper->getParserLiaison();

		theParserLiaison.setExecutionContext(*m_stylesheetExecutionContext);

		theParserLiaison.setEntityResolver(m_entityResolver);
		theParserLiaison.setErrorHandler(m_errorHandler);
		theParserLiaison.setUseValidation(m_useValidation);

		// Create some more support objects...
		XSLTProcessorEnvSupportDefault	theXSLTProcessorEnvSupport;

		const XalanDOMString&	theSourceURI = theParsedXML.getURI();

		if (length(theSourceURI) > 0)
		{
			theXSLTProcessorEnvSupport.setSourceDocument(theSourceURI, theSourceDocument);
		}

		// Set the functions if any.
		{
			for (FunctionParamPairVectorType::size_type i = 0; i < m_functionPairs.size(); ++i)
			{
				theXSLTProcessorEnvSupport.installExternalFunctionLocal(
						m_functionPairs[i].first.getNamespace(),
						m_functionPairs[i].first.getLocalPart(),
						*m_functionPairs[i].second);
			}
		}

		XObjectFactoryDefault	theXObjectFactory;

		XPathFactoryBlock 		theXPathFactory;

		// Create a processor...
		XSLTEngineImpl	theProcessor(
					theParserLiaison,
					theXSLTProcessorEnvSupport,
					theDOMSupport,
					theXObjectFactory,
					theXPathFactory);

		theXSLTProcessorEnvSupport.setProcessor(&theProcessor);

		const EnsureReset	theReset(*this);

		// Set up the stylesheet execution context.
		m_stylesheetExecutionContext->setXPathEnvSupport(&theXSLTProcessorEnvSupport);

		m_stylesheetExecutionContext->setDOMSupport(&theDOMSupport);

		m_stylesheetExecutionContext->setXObjectFactory(&theXObjectFactory);

		m_stylesheetExecutionContext->setXSLTProcessor(&theProcessor);

		// Create a problem listener and send output to a XalanDOMString.  Do this before
		// pushing params, since there could be a problem resolving a QName.
		DOMStringPrintWriter	thePrintWriter(theErrorMessage);

		XalanTransformerProblemListener		theProblemListener(m_warningStream, &thePrintWriter);

		if (m_problemListener == 0)
		{
			theProcessor.setProblemListener(&theProblemListener);
		}
		else
		{
			theProcessor.setProblemListener(m_problemListener);
		}

		{
			// Set the parameters if any.
			for (ParamPairVectorType::size_type i = 0; i < m_paramPairs.size(); ++i)
			{
				theProcessor.setStylesheetParam(
						m_paramPairs[i].first,
						m_paramPairs[i].second);
			}
		}

		// Create an input source for the source document...
		XSLTInputSource		theDocumentInputSource(theSourceDocument);

		// Set the system ID, so relative URIs are resolved properly...
		theDocumentInputSource.setSystemId(c_wstr(theSourceURI));

		addTraceListeners(m_traceListeners, theProcessor);

		// We may need to change the output encoding, so
		// we make a copy of the result target.
		XSLTResultTarget	tempResultTarget(theResultTarget);

		if (tempResultTarget.getEncoding().length() == 0 && m_outputEncoding.length() != 0)
		{
			tempResultTarget.setEncoding(m_outputEncoding);
		}

		if (theCompiledStylesheet != 0)
		{
			assert(theStylesheetSource == 0 &&
				   theCompiledStylesheet->getStylesheetRoot() != 0);

			m_stylesheetExecutionContext->setStylesheetRoot(theCompiledStylesheet->getStylesheetRoot());

			// Do the transformation...
			theProcessor.process(
						theDocumentInputSource,
						tempResultTarget,
						*m_stylesheetExecutionContext);
		}
		else
		{
			assert(theStylesheetSource != 0);

			// Create a stylesheet construction context, 
			// using the stylesheet's factory support objects.
			StylesheetConstructionContextDefault	theStylesheetConstructionContext(
							theProcessor,
							theXPathFactory);

			// Do the transformation...
			theProcessor.process(
							theDocumentInputSource,
							*theStylesheetSource,
							tempResultTarget,
							theStylesheetConstructionContext,
							*m_stylesheetExecutionContext);
		}
	}
	catch(const XSLException&	e)
	{
		if (length(theErrorMessage) == 0)
		{
			e.defaultFormat(theErrorMessage);
		}

		TranscodeToLocalCodePage(theErrorMessage, m_errorMessage, true);

		theResult = -1;
	}
	catch(const SAXParseException&	e)
	{
		if (length(theErrorMessage) != 0)
		{
			TranscodeToLocalCodePage(theErrorMessage, m_errorMessage, true);
		}
		else
		{
			FormatSAXParseException(e, m_errorMessage);
		}

		theResult = -2;
	}
	catch(const SAXException&	e)
	{
		if (length(theErrorMessage) != 0)
		{
			TranscodeToLocalCodePage(theErrorMessage, m_errorMessage, true);
		}
		else
		{
			TranscodeToLocalCodePage(e.getMessage(), m_errorMessage, true);
		}

		theResult = -2;
	}
	catch(const XMLException&	e)
	{
		if (length(theErrorMessage) != 0)
		{
			TranscodeToLocalCodePage(theErrorMessage, m_errorMessage, true);
		}
		else
		{
			TranscodeToLocalCodePage(e.getMessage(), m_errorMessage, true);
		}

		theResult = -3;
	}
	catch(const XalanDOMException&	e)
	{
		if (length(theErrorMessage) != 0)
		{
			TranscodeToLocalCodePage(theErrorMessage, m_errorMessage, true);
		}
		else
		{
			FormatXalanDOMException(e, m_errorMessage);
		}

		theResult = -4;
	}

	return theResult;
}



XALAN_CPP_NAMESPACE_END
