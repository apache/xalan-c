/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2000 The Apache Software Foundation.  All rights 
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
#include <algorithm>



#include <sax/SAXException.hpp>



#include <XalanDOM/XalanDOMException.hpp>



#include <PlatformSupport/XalanOutputStreamPrintWriter.hpp>
#include <PlatformSupport/DOMStringPrintWriter.hpp>



#include <XalanTransformer/XalanTransformer.hpp>



XSLTInit*	XalanTransformer::m_xsltInit = 0;



XalanTransformer::XalanTransformer():
	m_stylesheetExecutionContext(),
	m_compiledStylesheets(),
	m_parsedSources(),
	m_errorMessage()
{
	m_errorMessage.push_back(0);
}



XalanTransformer::~XalanTransformer()
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::for_each;
#endif

	// Clean up all entries in the compliledStylesheets vector.
	for_each(m_compiledStylesheets.begin(),
			 m_compiledStylesheets.end(),
			 DeleteFunctor<XalanCompiledStylesheet>());

	// Clean up all entries in the compliledStylesheets vector.
	for_each(m_parsedSources.begin(),
			 m_parsedSources.end(),
			 DeleteFunctor<XalanParsedSource>());
}



void
XalanTransformer::initialize()
{
	// Initialize Xalan. 
	m_xsltInit = new XSLTInit;
}



void
XalanTransformer::terminate()
{
	// Terminate Xalan and release memory.
	delete m_xsltInit;
}



int
XalanTransformer::transform(
	XalanParsedSource&			theParsedXML, 
	const XSLTInputSource&		theStylesheetSource,
	const XSLTResultTarget&		theResultTarget)
{
	int		theResult = 0;

	// Clear the error message.
	m_errorMessage.clear();
	m_errorMessage.push_back(0);

	// Store error messages from problem listener.
	XalanDOMString	theErrorMessage;

	try
	{		
		// Create some support objects that are necessary for running the processor...
		DOMSupport&			theDOMSupport = *theParsedXML.getDOMSupport();
		XMLParserLiaison&	theParserLiaison = *theParsedXML.getParserLiaison();

		// Create some more support objects...
		XSLTProcessorEnvSupportDefault	theXSLTProcessorEnvSupport;
		XObjectFactoryDefault			theXObjectFactory;
		XPathFactoryDefault				theXPathFactory;

		// Create a processor...
		XSLTEngineImpl	theProcessor(
				theParserLiaison,
				theXSLTProcessorEnvSupport,
				theDOMSupport,
				theXObjectFactory,
				theXPathFactory);

		// Create a problem listener and send output to a XalanDOMString.
		DOMStringPrintWriter	thePrintWriter(theErrorMessage);
		
		ProblemListenerDefault	theProblemListener(&thePrintWriter);

		theProcessor.setProblemListener(&theProblemListener);

		theParserLiaison.setExecutionContext(m_stylesheetExecutionContext);

		// Create a stylesheet construction context, using the
		// stylesheet's factory support objects.
		StylesheetConstructionContextDefault	theStylesheetConstructionContext(
					theProcessor,
					theXSLTProcessorEnvSupport,
					theXPathFactory);

		XSLTResultTarget	tempResultTarget(theResultTarget);

		m_stylesheetExecutionContext.setXPathEnvSupport(&theXSLTProcessorEnvSupport);
		m_stylesheetExecutionContext.setDOMSupport(&theDOMSupport);
		m_stylesheetExecutionContext.setXObjectFactory(&theXObjectFactory);
		m_stylesheetExecutionContext.setXSLTProcessor(&theProcessor);

		// Do the transformation...
		theProcessor.process(
					theParsedXML.getParsedSource(),
					theStylesheetSource,
					tempResultTarget,
					theStylesheetConstructionContext,
					m_stylesheetExecutionContext);

		reset();
	}
	catch (XSLException& e)
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
	catch (SAXException& e)
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
	catch (XMLException& e)
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
			XalanDOMString theMessage("XalanDOMException caught.  The code is ");
			
			append(theMessage,  LongToDOMString(long(e.getExceptionCode())));
			append(theMessage,  XalanDOMString("."));						 

			TranscodeToLocalCodePage(theMessage, m_errorMessage, true);
		}

		theResult = -4;
	}

	m_stylesheetExecutionContext.setXPathEnvSupport(0);
	m_stylesheetExecutionContext.setDOMSupport(0);
	m_stylesheetExecutionContext.setXObjectFactory(0);
	m_stylesheetExecutionContext.setXSLTProcessor(0);

	return theResult;
}



int
XalanTransformer::transform(
			XalanParsedSource&				theParsedXML, 
			const XalanCompiledStylesheet*	theCompiledStylesheet,
			const XSLTResultTarget&			theResultTarget)
{
	int		theResult = 0;

	// Clear the error message.
	m_errorMessage.clear();
	m_errorMessage.push_back(0);

	// Store error messages from problem listener.
	XalanDOMString	theErrorMessage;

	try
	{
		// Create some support objects that are necessary for running the processor...
		DOMSupport&			theDOMSupport = *theParsedXML.getDOMSupport();
		XMLParserLiaison&	theParserLiaison = *theParsedXML.getParserLiaison();

		// Create some more support objects...
		XSLTProcessorEnvSupportDefault	theXSLTProcessorEnvSupport;
		XObjectFactoryDefault			theXObjectFactory;
		XPathFactoryDefault				theXPathFactory;

		// Create a processor...
		XSLTEngineImpl	theProcessor(
				theParserLiaison,
				theXSLTProcessorEnvSupport,
				theDOMSupport,
				theXObjectFactory,
				theXPathFactory);

		// Create a problem listener and send output to a XalanDOMString.
		DOMStringPrintWriter	thePrintWriter(theErrorMessage);
		
		ProblemListenerDefault	theProblemListener(&thePrintWriter);

		theProcessor.setProblemListener(&theProblemListener);

		XSLTResultTarget	tempResultTarget(theResultTarget);

		m_stylesheetExecutionContext.setXPathEnvSupport(&theXSLTProcessorEnvSupport);
		m_stylesheetExecutionContext.setDOMSupport(&theDOMSupport);
		m_stylesheetExecutionContext.setXObjectFactory(&theXObjectFactory);
		m_stylesheetExecutionContext.setXSLTProcessor(&theProcessor);

		// Set the compiled stylesheet.
		theCompiledStylesheet->setStylesheetRoot(m_stylesheetExecutionContext);

		// Do the transformation...
		theProcessor.process(
					theParsedXML.getParsedSource(),		
					tempResultTarget,					
					m_stylesheetExecutionContext);
	}
	catch (XSLException& e)
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
	catch (SAXException& e)
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
	catch (XMLException& e)
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
			XalanDOMString theMessage("XalanDOMException caught.  The code is ");
			
			append(theMessage,  LongToDOMString(long(e.getExceptionCode())));
			append(theMessage,  XalanDOMString("."));						 

			TranscodeToLocalCodePage(theMessage, m_errorMessage, true);
		}

		theResult = -4;
	}

	m_stylesheetExecutionContext.setXPathEnvSupport(0);
	m_stylesheetExecutionContext.setDOMSupport(0);
	m_stylesheetExecutionContext.setXObjectFactory(0);
	m_stylesheetExecutionContext.setXSLTProcessor(0);

	reset();
	
	return theResult;
}



int
XalanTransformer::transform(
	const XSLTInputSource&		theInputSource, 
	const XSLTInputSource&		theStylesheetSource,
	const XSLTResultTarget&		theResultTarget)
{
	// Parse the source document.
	XalanParsedSource* theParsedXML = parseSource(theInputSource);

	// Do the transformation...
	return transform(
					*theParsedXML, 
					theStylesheetSource,
					theResultTarget);
}



int
XalanTransformer::transform(
			const XSLTInputSource&			theInputSource, 
			const XalanCompiledStylesheet*	theCompiledStylesheet,
			const XSLTResultTarget&			theResultTarget)
{
	// Parse the source document.
	XalanParsedSource* theParsedXML = parseSource(theInputSource);

	// Do the transformation...
	return transform(
					*theParsedXML, 
					theCompiledStylesheet,
					theResultTarget);
}



int
XalanTransformer::transform(
			const XSLTInputSource&		theInputSource, 		
			const XSLTResultTarget&		theResultTarget)
{
	// Do the transformation...
	return transform(
					theInputSource, 
					XSLTInputSource(),
					theResultTarget);
}



int
XalanTransformer::transform(
			const XSLTInputSource&			theInputSource, 
			const XSLTInputSource&			theStylesheetSource,
			const void*						theOutputHandle, 
			XalanOutputHandlerType			theOutputHandler,
			XalanFlushHandlerType			theFlushHandler)
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
			const XSLTInputSource&			theInputSource, 
			const XalanCompiledStylesheet*	theCompiledStylesheet,
			const void*						theOutputHandle, 
			XalanOutputHandlerType			theOutputHandler,
			XalanFlushHandlerType			theFlushHandler)
{
	// Set to output target to the callback 
	XalanTransformerOutputStream	theOutputStream(theOutputHandle, theOutputHandler, theFlushHandler);
	XalanOutputStreamPrintWriter	thePrintWriter(theOutputStream);
	XSLTResultTarget				theResultTarget(&thePrintWriter);

	// Do the transformation...
	return transform(
					theInputSource, 
					theCompiledStylesheet,
					theResultTarget);
}



int
XalanTransformer::transform(
			const XSLTInputSource&		theInputSource, 		
			const void*					theOutputHandle, 
			XalanOutputHandlerType		theOutputHandler,
			XalanFlushHandlerType		theFlushHandler)
{
	// Set to output target to the callback 
	XalanTransformerOutputStream	theOutputStream(theOutputHandle, theOutputHandler, theFlushHandler);
	XalanOutputStreamPrintWriter	thePrintWriter(theOutputStream);
	XSLTResultTarget				theResultTarget(&thePrintWriter);

	// Do the transformation...
	return transform(
					theInputSource, 
					XSLTInputSource(),
					theResultTarget);
}



XalanCompiledStylesheet*
XalanTransformer::compileStylesheet(const XSLTInputSource&		theStylesheetSource)
{
	// Clear the error message.
	m_errorMessage.clear();
	m_errorMessage.push_back(0);

	// Store error messages from problem listener.
	XalanDOMString	theErrorMessage;

	try
	{	

		// Create some support objects that are necessary for running the processor...
		XalanSourceTreeDOMSupport		theDOMSupport;
		XalanSourceTreeParserLiaison	theParserLiaison(theDOMSupport);

		// Hook the two together...
		theDOMSupport.setParserLiaison(&theParserLiaison);

		// Create some more support objects...
		XSLTProcessorEnvSupportDefault	theXSLTProcessorEnvSupport;
		XObjectFactoryDefault			theXObjectFactory;
		XPathFactoryDefault				theXPathFactory;

		// Create a processor...
		XSLTEngineImpl	theProcessor(
				theParserLiaison,
				theXSLTProcessorEnvSupport,
				theDOMSupport,
				theXObjectFactory,
				theXPathFactory);

		// Create a new XalanCompiledStylesheet.
		XalanCompiledStylesheet* const theCompiledStylesheet =
			new XalanCompiledStylesheet(
						theStylesheetSource, 
						theXSLTProcessorEnvSupport,
						theProcessor);

		// Store it in a vector.
		m_compiledStylesheets.push_back(theCompiledStylesheet);

		return theCompiledStylesheet;
	}
	catch (XSLException& e)
	{
		if (length(theErrorMessage) != 0)
		{
			TranscodeToLocalCodePage(theErrorMessage, m_errorMessage, true);
		}
		else
		{
			TranscodeToLocalCodePage(e.getMessage(), m_errorMessage, true);
		}
		
	}
	catch (SAXException& e)
	{
		if (length(theErrorMessage) != 0)
		{
			TranscodeToLocalCodePage(theErrorMessage, m_errorMessage, true);
		}
		else
		{
			TranscodeToLocalCodePage(e.getMessage(), m_errorMessage, true);
		}
		
	}
	catch (XMLException& e)
	{
		if (length(theErrorMessage) != 0)
		{
			TranscodeToLocalCodePage(theErrorMessage, m_errorMessage, true);
		}
		else
		{
			TranscodeToLocalCodePage(e.getMessage(), m_errorMessage, true);
		}
		
	}
	catch(const XalanDOMException&	e)
	{
		if (length(theErrorMessage) != 0)
		{
			TranscodeToLocalCodePage(theErrorMessage, m_errorMessage, true);
		}
		else
		{
			XalanDOMString theMessage("XalanDOMException caught.  The code is ");
			
			append(theMessage,  LongToDOMString(long(e.getExceptionCode())));
			append(theMessage,  XalanDOMString("."));						 

			TranscodeToLocalCodePage(theMessage, m_errorMessage, true);
		}
	}
	return 0;
}



XalanParsedSource*
XalanTransformer::parseSource(
			const XSLTInputSource&	theInputSource, 
			bool					useXercesDOM)
{
	// Clear the error message.
	m_errorMessage.clear();
	m_errorMessage.push_back(0);

	// Store error messages from problem listener.
	XalanDOMString	theErrorMessage;

	try
	{	
		XalanParsedSource* theParsedDocument = 0;

		if(useXercesDOM == 1)
		{
			theParsedDocument = new XercesDOMParsedSource(theInputSource);
		}
		else
		{
			theParsedDocument = new XalanDefaultParsedSource(theInputSource);
		}

		// Store it in a vector.
		m_parsedSources.push_back(theParsedDocument);		

		return theParsedDocument;
	}	
	catch (XSLException& e)
	{
		if (length(theErrorMessage) != 0)
		{
			TranscodeToLocalCodePage(theErrorMessage, m_errorMessage, true);
		}
		else
		{
			TranscodeToLocalCodePage(e.getMessage(), m_errorMessage, true);
		}
		
	}
	catch (SAXException& e)
	{
		if (length(theErrorMessage) != 0)
		{
			TranscodeToLocalCodePage(theErrorMessage, m_errorMessage, true);
		}
		else
		{
			TranscodeToLocalCodePage(e.getMessage(), m_errorMessage, true);
		}
		
	}
	catch (XMLException& e)
	{
		if (length(theErrorMessage) != 0)
		{
			TranscodeToLocalCodePage(theErrorMessage, m_errorMessage, true);
		}
		else
		{
			TranscodeToLocalCodePage(e.getMessage(), m_errorMessage, true);
		}
		
	}
	catch(const XalanDOMException&	e)
	{
		if (length(theErrorMessage) != 0)
		{
			TranscodeToLocalCodePage(theErrorMessage, m_errorMessage, true);
		}
		else
		{
			XalanDOMString theMessage("XalanDOMException caught.  The code is ");
			
			append(theMessage,  LongToDOMString(long(e.getExceptionCode())));
			append(theMessage,  XalanDOMString("."));						 

			TranscodeToLocalCodePage(theMessage, m_errorMessage, true);
		}
	}
	return 0;
}



void
XalanTransformer::setStylesheetParam(
			const XalanDOMString&	key,
			const XalanDOMString&	expression)
{
	// Set the stylesheet parameter.
//	m_processor.setStylesheetParam(key,  expression);
}



void
XalanTransformer::setStylesheetParam(
			const char*				key,
			const char*				expression)
{
	// Set the stylesheet parameter.
//	m_processor.setStylesheetParam(
//					XalanDOMString(key),  
//					XalanDOMString(expression));
}



const char*
XalanTransformer::getLastError() const
{
	return &m_errorMessage[0]; 
}



void 
XalanTransformer::reset()
{
	try
	{
		// Reset objects.
		m_stylesheetExecutionContext.setXPathEnvSupport(0);
		m_stylesheetExecutionContext.setDOMSupport(0);
		m_stylesheetExecutionContext.setXObjectFactory(0);
		m_stylesheetExecutionContext.setXSLTProcessor(0);

		m_stylesheetExecutionContext.reset();
	}
	catch(...)
	{
	}
}

