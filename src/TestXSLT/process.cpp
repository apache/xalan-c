/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2000 The Apache Software Foundation.  All rights 
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

#include <Include/PlatformDefinitions.hpp>



#include <cstdio>
#include <cstring>



#if defined(XALAN_OLD_STREAM_HEADERS)
#include <iostream.h>
#include <strstream.h>
#else
#include <iostream>
#include <strstream>
#endif



#if !defined(NDEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif



#include <util/PlatformUtils.hpp>
#include <sax/SAXException.hpp>



#include <XalanDOM/XalanDOMException.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/DOMStringPrintWriter.hpp>
#include <PlatformSupport/XalanOutputStreamPrintWriter.hpp>
#include <PlatformSupport/XalanFileOutputStream.hpp>
#include <PlatformSupport/XalanStdOutputStream.hpp>



#include <Include/XalanAutoPtr.hpp>



#include <DOMSupport/DOMSupportDefault.hpp>



#include <XPath/XObjectFactoryDefault.hpp>
#include <XPath/XPathEnvSupportDefault.hpp>
#include <XPath/XPathSupportDefault.hpp>
#include <XPath/XPath.hpp>
#include <XPath/XPathExecutionContextDefault.hpp>
#include <XPath/XPathFactoryDefault.hpp>
#include <XPath/XPathProcessorImpl.hpp>



#include <XercesParserLiaison/XercesParserLiaison.hpp>
#include <XercesParserLiaison/XercesDOMSupport.hpp>



#include <XMLSupport/FormatterToDOM.hpp>
#include <XMLSupport/FormatterToHTML.hpp>
#include <XMLSupport/FormatterToNull.hpp>
#include <XMLSupport/FormatterToText.hpp>
#include <XMLSupport/FormatterToXML.hpp>
#include <XMLSupport/FormatterTreeWalker.hpp>



#include <XSLT/XSLTEngineImpl.hpp>
#include <XSLT/XSLTInit.hpp>
#include <XSLT/XSLTInputSource.hpp>
#include <XSLT/XSLTResultTarget.hpp>
#include <XSLT/StylesheetRoot.hpp>
#include <XSLT/StylesheetConstructionContextDefault.hpp>
#include <XSLT/StylesheetExecutionContextDefault.hpp>
#include <XSLT/TraceListenerDefault.hpp>
#include <XSLT/XSLTProcessorEnvSupportDefault.hpp>



//#define XALAN_USE_ICU
#if defined(XALAN_USE_ICU)
#include <ICUBridge/ICUBridge.hpp>
#include <ICUBridge/FunctionICUFormatNumber.hpp>
#include <ICUBridge/ICUXalanNumberFormatFactory.hpp>
#include <ICUBridge/ICUBridgeCollationCompareFunctor.hpp>
#endif



//#define XALAN_VQ_SPECIAL_TRACE
#if defined(XALAN_VQ_SPECIAL_TRACE)
#include "d:/Rational/Quantify/pure.h"
#endif



#if !defined (XALAN_NO_NAMESPACES)
using std::cerr;
using std::cout;
using std::endl;
using std::hex;
using std::less;
using std::map;
using std::vector;
#endif



/**
 * Print argument options.
 */ 
void
printArgOptions()
{
	cerr << endl
		 << "TestXSLT options: "
		 << endl
		 << endl
		 << "Options are not case-sensitive."
		 << endl
		 << endl
		 << "	-IN inputXMLURL"
		 << endl
		 << endl
		 << " [-XSL XSLTransformationURL]"
		 << endl
		 << " [-OUT outputFileName]"
		 << endl
		 << " [-V (Show version information only.)]"
		 << endl
		 << " [-QC (Quiet pattern conflicts warnings.)]"
		 << endl
		 << " [-Q (Use quiet mode.)]"
		 << endl
		 << " [-ESCAPE (Specifies which characters to escape. {default is <>&\"\'\\r\\n}]"
		 << endl
		 << " [-EER (Expand entity references.  By default, they are not expanded.)]"
		 << endl
		 << " [-INDENT n (Controls how many spaces to indent. {default is 0})]"
		 << endl
		 << " [-VALIDATE (Controls whether validation occurs. Validation is off by default.)]"
		 << endl
		 << " [-TT (Trace the templates as they are being called.)]"
		 << endl
		 << " [-TG (Trace each generation event.)]"
		 << endl
		 << " [-TS (Trace each selection event.)]"
		 << endl
		 << " [-TTC (Trace the template children as they are being processed.)]"
		 << endl
		 << " [-XML (Use XML formatter and add XML header.)]"
		 << endl
		 << " [-TEXT (Use simple Text formatter.)]"
		 << endl
		 << " [-HTML (Use HTML formatter.)]"
		 << endl
		 << " [-DOM (Use DOM formatter.  Formats to DOM, then formats XML for output.)]"
		 << endl
		 << " [-PARAM name expression (Sets a stylesheet parameter.)]"
		 << endl
		 << endl
		 << "The following options are valid only with -HTML or -XML."
		 << endl
		 << endl
		 << " [-STRIPCDATA (Strip CDATA sections of their brackets, but don't escape.)"
		 << endl
		 << " [-ESCAPECDATA (Strip CDATA sections of their brackets, but escape.)"
		 << endl
		 << endl
		 << "The following option is valid only with -HTML."
		 << endl
		 << endl
		 << " [-NOINDENT (Turns off HTML indenting..]"
		 << endl
		 << "The following option is valid only with -XML."
		 << endl
		 << endl
		 << " [-NH (Don't write XML header.)]"
		 << endl;
}




#if defined(XALAN_NEEDS_EXPLICIT_TEMPLATE_INSTANTIATION)
#include<stl/_tree.c>
#endif

typedef map<CharVectorType, CharVectorType, less<CharVectorType> > String2StringMapType;



struct CmdLineParams
{
	String2StringMapType paramsMap;
	bool doStackDumpOnError;
	bool escapeCData;
	bool setQuietConflictWarnings;
	bool setQuietMode;
	bool shouldExpandEntityRefs;
	bool stripCData;
	bool versionOnly;
	bool traceTemplates;
	bool traceGenerationEvent;
	bool traceSelectionEvent;
	bool traceTemplateChildren;
	bool shouldWriteXMLHeader;
	bool doValidation;
	bool noIndent;
	bool formatToNull;
	int indentAmount;
	int outputType;
	CharVectorType outFileName;
	CharVectorType specialCharacters;
	CharVectorType xslFileName;
	CharVectorType inFileName;

	CmdLineParams() :
		paramsMap(),
		doStackDumpOnError(false),
		escapeCData(false),
		setQuietConflictWarnings(false),
		setQuietMode(false),
		shouldExpandEntityRefs(false),
		stripCData(false),
		versionOnly(false),
		traceTemplates(false),
		traceGenerationEvent(false),
		traceSelectionEvent(false),
		traceTemplateChildren(false),
		shouldWriteXMLHeader(true),
		doValidation(false),
		noIndent(false),
		formatToNull(false),
		indentAmount(-1),
		outputType(-1),
		outFileName(),
		specialCharacters(),
		xslFileName(),
		inFileName()
	{
	}
};



void
warnPreviousOutputMethod(int	outputMethod)
{
	cerr << endl << "Warning: Ignoring previous output method switch ";

	switch(outputMethod)
	{
	case FormatterListener::OUTPUT_METHOD_XML:
		cerr << "-XML.";
		break;

	case FormatterListener::OUTPUT_METHOD_TEXT:
		cerr << "-TEXT.";
		break;

	case FormatterListener::OUTPUT_METHOD_HTML:
		cerr << "-HTML.";
		break;

	case FormatterListener::OUTPUT_METHOD_DOM:
		cerr << "-DOM.";
		break;		
	}

	cerr << endl << endl;
}



bool
getArgs(
			int				argc,
			const char*		argv[],
			CmdLineParams&	p)
{
	bool fSuccess = true;

	for (int i = 1; i < argc && fSuccess == true; ++i)
	{
		if (!stricmp("-IN", argv[i]))
		{
			++i;

			if(i < argc && argv[i][0] != '-')
			{
				CopyStringToVector(argv[i], p.inFileName);
			}
			else
			{
				fSuccess = false;
			}
		}
		else if (!stricmp("-XSL", argv[i]))
		{
			++i;

			if(i < argc && argv[i][0] != '-')
			{
				CopyStringToVector(argv[i], p.xslFileName);
			}
			else
			{
				fSuccess = false;
			}
		}
		else if (!stricmp("-OUT", argv[i]))
		{
			++i;

			if(i < argc && argv[i][0] != '-')
			{
				CopyStringToVector(argv[i], p.outFileName);
			}
			else
			{
				fSuccess = false;
			}
		}
		else if (!stricmp("-ESCAPE", argv[i]))
		{
			++i;

			if(i < argc && argv[i][0] != '-')
			{
				CopyStringToVector(argv[i], p.specialCharacters);
			}
			else
			{
				fSuccess = false;
			}
		}
		else if (!stricmp("-NOINDENT", argv[i]))
		{
			p.noIndent = true;
		} 
		else if (!stricmp("-INDENT", argv[i]))
		{
			++i;

			if(i < argc && argv[i][0] != '-')
			{
				p.indentAmount = atoi(argv[i]);
			}
			else
			{
				fSuccess = false;
			}
		}
		else if(!stricmp("-VALIDATE", argv[i]))
		{
			p.doValidation = true;
		}
		else if(!stricmp("-EER", argv[i]))
		{
			p.shouldExpandEntityRefs = true;
		}
		else if (!stricmp("-PARAM", argv[i])) 
		{
			++i;

			if(i < argc && argv[i][0] != '-')
			{
				CharVectorType	name;

				CopyStringToVector(argv[i], name);

				++i;

				// Don't check for '-' here, since that might
				// be a valid character in a parameter value.
				if(i < argc)
				{
					CharVectorType	expression;

					CopyStringToVector(argv[i], expression);

					p.paramsMap[name] = expression;
				}
				else
				{
					fSuccess = false;
				}
			}
			else
			{
				fSuccess = false;
			}
		}
		else if(!stricmp("-V", argv[i]))
		{
			p.versionOnly = true;
		}
		else if(!stricmp("-QC", argv[i]))
		{
			p.setQuietConflictWarnings = true;
		}
		else if(!stricmp("-Q", argv[i]))
		{
			p.setQuietMode = true;
		}
		else if(!stricmp("-XML", argv[i]))
		{
			if (p.outputType != -1)
			{
				warnPreviousOutputMethod(p.outputType);
			}

			p.outputType = FormatterListener::OUTPUT_METHOD_XML;
		}
		else if(!stricmp("-TEXT", argv[i]))
		{
			if (p.outputType != -1)
			{
				warnPreviousOutputMethod(p.outputType);
			}

			p.outputType = FormatterListener::OUTPUT_METHOD_TEXT;
		}
		else if(!stricmp("-HTML", argv[i]))
		{
			if (p.outputType != -1)
			{
				warnPreviousOutputMethod(p.outputType);
			}

			p.outputType = FormatterListener::OUTPUT_METHOD_HTML;
		}
		else if(!stricmp("-DOM", argv[i]))
		{
			if (p.outputType != -1)
			{
				warnPreviousOutputMethod(p.outputType);
			}

			p.outputType = FormatterListener::OUTPUT_METHOD_DOM;
		}
		else if(!stricmp("-NULL", argv[i]))
		{
			p.formatToNull = true;
		}
		else if(!stricmp("-STRIPCDATA", argv[i]))
		{
			p.stripCData = true;
		}
		else if(!stricmp("-ESCAPECDATA", argv[i]))
		{
			p.escapeCData = true;
		}
		else if (!stricmp("-NH", argv[i]))
		{
			p.shouldWriteXMLHeader = false;
		}
		else if(!stricmp("-TT", argv[i]))
		{
			p.traceTemplates = true;
		}
		else if(!stricmp("-TG", argv[i]))
		{
			p.traceGenerationEvent = true;
		}
		else if(!stricmp("-TS", argv[i]))
		{
			p.traceSelectionEvent = true;
		}
		else if(!stricmp("-TTC", argv[i]))
		{
			p.traceTemplateChildren = true;
		}
		else
		{
			cerr << endl << "Warning: Ignoring unknown option \"" << argv[i] << "\"." << endl << endl;
		}
	}

	return fSuccess;
}



FormatterListener*
createFormatter(
			int							outputType,
			bool						shouldWriteXMLHeader,
			bool						stripCData,
			bool						escapeCData,
			bool						noIndent,
			bool						formatToNull,
			PrintWriter&				resultWriter,
			int							indentAmount,
			const XalanDOMString&		mimeEncoding,
			const StylesheetRoot*		stylesheet,
			XMLParserLiaison&			parserLiaison)
{
	FormatterListener*	formatter = 0;

	if (formatToNull == true)
	{
		formatter = new FormatterToNull;
	}
	else if(FormatterListener::OUTPUT_METHOD_XML == outputType)
	{
		XalanDOMString	version;
		bool			outputIndent = false;
		XalanDOMString	mediatype;
		XalanDOMString	doctypeSystem;
		XalanDOMString	doctypePublic;
		XalanDOMString	standalone;

		if (stylesheet != 0)
		{
			version = stylesheet->m_version;

			mediatype = stylesheet->m_mediatype;
			doctypeSystem = stylesheet->getOutputDoctypeSystem();
			doctypePublic = stylesheet->getOutputDoctypePublic();
			standalone = stylesheet->m_standalone;
		}

		FormatterToXML* fToXML = new FormatterToXML(resultWriter,
					version,
					outputIndent,
					indentAmount,
					mimeEncoding,
					mediatype,
					doctypeSystem,
					doctypePublic,
					true,	// xmlDecl
					standalone);

		fToXML->setShouldWriteXMLHeader(shouldWriteXMLHeader);
		fToXML->setStripCData(stripCData);
		fToXML->setEscapeCData(escapeCData);

		formatter = fToXML;
	}
	else if(FormatterListener::OUTPUT_METHOD_TEXT == outputType)
	{
		formatter = new FormatterToText(resultWriter, mimeEncoding);
	}
	else if(FormatterListener::OUTPUT_METHOD_HTML == outputType)
	{
		XalanDOMString	version;
		bool			outputIndent = !noIndent;
		XalanDOMString	mediatype;
		XalanDOMString	doctypeSystem;
		XalanDOMString	doctypePublic;
		XalanDOMString	standalone;

		if (stylesheet != 0)
		{
			version = stylesheet->m_version;

			if (noIndent == false)
			{
				outputIndent = stylesheet->getOutputIndent();
			}

			mediatype = stylesheet->m_mediatype;
			doctypeSystem = stylesheet->getOutputDoctypeSystem();
			doctypePublic = stylesheet->getOutputDoctypePublic();
			standalone = stylesheet->m_standalone;
		}

		FormatterToHTML* fToHTML
				= new FormatterToHTML(
						resultWriter,
						mimeEncoding,
						mediatype,
						doctypeSystem,
						doctypePublic,
						outputIndent,
						indentAmount,
						version,
						standalone,
						false);	// xmlDecl

		fToHTML->setStripCData(stripCData);

		formatter = fToHTML;
	}
	else if(FormatterListener::OUTPUT_METHOD_DOM == outputType)
	{
		formatter = new FormatterToDOM(parserLiaison.getDOMFactory(), 0);
	}

	return formatter;
}



XalanOutputStream*
createOutputStream(const CmdLineParams&		params)
{
	if (params.outFileName.empty())
	{
		return new XalanStdOutputStream(cout);
	}
	else
	{
		return new XalanFileOutputStream(TranscodeFromLocalCodePage(c_str(params.outFileName)));
	}
}



TraceListener*
createTraceListener(
			const CmdLineParams&	params,
			PrintWriter&			diagnosticsWriter)
{
	if (params.traceTemplates == true ||
		params.traceTemplateChildren == true ||
		params.traceGenerationEvent == true ||
		params.traceSelectionEvent)
	{
		return new TraceListenerDefault(
				diagnosticsWriter,
				params.traceTemplates,
				params.traceTemplateChildren,
				params.traceGenerationEvent,
				params.traceSelectionEvent);
	}
	else
	{
		return 0;
	}

}



int
xsltMain(const CmdLineParams&	params)
{
#if defined(XALAN_USE_ICU)
	// Create an installer to install the substitue format-number() function.
	FunctionICUFormatNumber::FunctionICUFormatNumberInstaller	theInstaller;

	// Create a factory for creating XalanNumberFormat instances using
	// the ICU as implementation...
	ICUXalanNumberFormatFactory		theXalanNumberFormatFactory;

	// Install the ICU-based factory...
	StylesheetExecutionContextDefault::installXalanNumberFormatFactory(&theXalanNumberFormatFactory);
#endif

	const XalanDOMString	mimeEncoding(XALAN_STATIC_UCODE_STRING("UTF-8"));
	const XalanDOMString	encoding(XALAN_STATIC_UCODE_STRING("UTF-8"));

	/**
	 * The default diagnostic writer...
	 */
	XalanStdOutputStream				theStdErr(cerr);
	XalanOutputStreamPrintWriter		diagnosticsWriter(theStdErr);

	XercesDOMSupport		theDOMSupport;
	XercesParserLiaison		xmlParserLiaison(theDOMSupport);

	XPathSupportDefault				theXPathSupport(theDOMSupport);
	XSLTProcessorEnvSupportDefault	theXSLProcessorSupport;

	XObjectFactoryDefault			theXObjectFactory;

	XPathFactoryDefault theXPathFactory;

	const XalanAutoPtr<TraceListener>		theTraceListener(
			createTraceListener(
				params,
				diagnosticsWriter));

	XSLTEngineImpl processor(
			xmlParserLiaison,
			theXPathSupport,
			theXSLProcessorSupport,
			theDOMSupport,
			theXObjectFactory,
			theXPathFactory);

	theXSLProcessorSupport.setProcessor(&processor);

	if (theTraceListener.get() != 0)
	{
		processor.setTraceSelects(params.traceSelectionEvent);
		processor.addTraceListener(theTraceListener.get());
	}

	// Use a separate factory instance for the stylesheet.  This is really only necessary
	// if you want to use the stylesheet with another processor, or you want to use
	// it multiple times.
	XPathFactoryDefault		theStylesheetXPathFactory;

	StylesheetConstructionContextDefault	theConstructionContext(processor,
			theXSLProcessorSupport,
			theStylesheetXPathFactory);

	/*
	 * Set specified processor flags
	 */
	processor.setQuietConflictWarnings(params.setQuietConflictWarnings);

	if (params.paramsMap.size())	
	{
		String2StringMapType::const_iterator	it = params.paramsMap.begin();

		for ( ; it != params.paramsMap.end(); ++it)
		{
			processor.setStylesheetParam(
					TranscodeFromLocalCodePage(c_str((*it).first)),
					TranscodeFromLocalCodePage(c_str((*it).second)));
		}
	}

	/*
	 * Set specified parser flags
	 */
	if (params.indentAmount != 0)
	{
		xmlParserLiaison.setIndent(params.indentAmount);
	}

	if (params.specialCharacters.size() != 0)
	{
		xmlParserLiaison.setSpecialCharacters(TranscodeFromLocalCodePage(c_str(params.specialCharacters)));
	}

	xmlParserLiaison.SetShouldExpandEntityRefs(params.shouldExpandEntityRefs);
	xmlParserLiaison.setUseValidation(params.doValidation);

	assert(params.inFileName.size() > 0);

	if (!params.setQuietMode)
	{
		processor.setDiagnosticsOutput(&diagnosticsWriter);
	}

	XalanDOMString	xslFileName;

	if(0 != params.xslFileName.size())
	{
		xslFileName = TranscodeFromLocalCodePage(c_str(params.xslFileName));
	}

	const StylesheetRoot*	stylesheet = 0;

	if (!isEmpty(xslFileName))
	{
		stylesheet = processor.processStylesheet(xslFileName, theConstructionContext);
	}

	XalanAutoPtr<XalanOutputStream>		outputFileStream(createOutputStream(params));
	assert(outputFileStream.get() != 0);

	XalanOutputStreamPrintWriter	resultWriter(*outputFileStream.get());

	const XalanAutoPtr<FormatterListener>	formatter(
			createFormatter(
				params.outputType,
				params.shouldWriteXMLHeader,
				params.stripCData,
				params.escapeCData,
				params.noIndent,
				params.formatToNull,
				resultWriter,
				xmlParserLiaison.getIndent(),
				mimeEncoding,
				stylesheet,
				xmlParserLiaison));

	XSLTResultTarget	rTreeTarget;

	if(formatter.get() == 0)
	{
		rTreeTarget.setCharacterStream(&resultWriter);
	}
	else
	{
		rTreeTarget.setFormatterListener(formatter.get());
	}


	// Do the transformation...
	XSLTInputSource		theInputSource(c_str(params.inFileName));

	StylesheetExecutionContextDefault	theExecutionContext(processor,
			theXSLProcessorSupport,
			theXPathSupport,
			theXObjectFactory);

#if defined(XALAN_USE_ICU)
	ICUBridgeCollationCompareFunctor	theICUFunctor;

	theExecutionContext.installCollationCompareFunctor(&theICUFunctor);
#endif

	if (stylesheet == 0)
	{
		// No stylesheet, so our only hope is that the xml file has
		// PI with the stylesheet...

		// Dummy input source...
		XSLTInputSource		theStylesheetSource;

		processor.process(
				theInputSource,
				theStylesheetSource,
				rTreeTarget,
				theConstructionContext,
				theExecutionContext);
	}
	else
	{
		theExecutionContext.setStylesheetRoot(stylesheet);

		processor.process(
				theInputSource,
				rTreeTarget,
				theExecutionContext);
	}

	if (params.outputType == FormatterListener::OUTPUT_METHOD_DOM)
	{
		// Output is to DOM, so we have to format to XML to
		// produce output...
		assert(rTreeTarget.getFormatterListener() != 0 &&
			   rTreeTarget.getFormatterListener()->getOutputFormat() ==
					FormatterListener::OUTPUT_METHOD_DOM);

		// Get the FormatterToDOM that produced the result document...
		const FormatterToDOM* const	theResultFormatter =
#if defined(XALAN_OLD_STYLE_CASTS)
			(FormatterToDOM*)rTreeTarget.getFormatterListener();
#else
			static_cast<FormatterToDOM*>(rTreeTarget.getFormatterListener());
#endif

		// Get the document...
		const XalanDocument* const	theResultDocument =
			theResultFormatter->getDocument();

		if (theResultDocument == 0)
		{
			cerr << endl << "Warning: No DOM document to format!!!" << endl;
		}
		else
		{
			// Create a FormaterToDOM with the required output
			// options...
			const XalanAutoPtr<FormatterListener>	formatter(
					createFormatter(
						FormatterListener::OUTPUT_METHOD_XML,
						params.shouldWriteXMLHeader,
						params.stripCData,
						params.escapeCData,
						params.noIndent,
						false,
						resultWriter,
						xmlParserLiaison.getIndent(),
						mimeEncoding,
						stylesheet,
						xmlParserLiaison));

			// Create a FormatterTreeWalker with the the
			// new formatter...
			FormatterTreeWalker theTreeWalker(*formatter.get());

			// Walk the document and produce the XML...
			theTreeWalker.traverse(theResultDocument);
		}
	}

	theExecutionContext.reset();

	theConstructionContext.reset();
	theStylesheetXPathFactory.reset();

	processor.reset();

	theXPathFactory.reset();
	theXObjectFactory.reset();
	theXSLProcessorSupport.reset();
	theXPathSupport.reset();

	xmlParserLiaison.reset();

	return 0;
}



int
main(
			int				argc,
			const char*		argv[])
{
#if !defined(XALAN_USE_ICU) && !defined(NDEBUG) && defined(_MSC_VER)
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif

#if defined(XALAN_VQ_SPECIAL_TRACE)
	QuantifyStopRecordingData();
#endif

   /**
	 * Command line interface to transform the XML according to 
	 * the instructions found in the XSL document.
	 *		-in inputXMLURL
	 *		-xsl XSLTransformationURL
	 *		-out outputFileName
	 *		-F (Format output pretty-printed)
	 */

	XMLPlatformUtils::Initialize();

	int				theResult = 0;

	CmdLineParams	theParams;
	

	/*
	 *		Get command line arguments
	 */
	if (argc == 1)
	{
		printArgOptions();
	}
	else
	{
		if (getArgs(argc, argv, theParams) == false)
		{
			printArgOptions();
		}
		else if (theParams.versionOnly == true)
		{
			cout << endl
				 << "TestXSLT version 1.0.0 (Xalan C++ version 1.0.0)"
				 << endl;
		}
		else if (theParams.inFileName.size() == 0)
		{
			printArgOptions();
		}
		else
		{
			try
			{
				XSLTInit	theInit;

				theResult = xsltMain(theParams);

			}
			catch (XSLException& e)
			{
				cout << "\nXSLException ";

#if defined(XALAN_OSTREAM_HAS_WCHAR_T)
				cout << "Type is : ";

				OutputString(cout, e.getType());

				cout << endl;

				cout << "Message is : ";

				OutputString(cout, e.getMessage());

				cout << endl;
#else
				cout << "Type is : " << e.getType() << endl;

				cout << "Message is : " << e.getMessage() << endl;
#endif
				theResult = -1;
			}
			catch (SAXException& e)
			{
				cout << "\nSAXException ";

#if defined(XALAN_OSTREAM_HAS_WCHAR_T)
				cout << "Message is : ";

				OutputString(cout, e.getMessage());

				cout << endl;
#else
				cout << "Message is : " << e.getMessage() << endl;

#endif
				theResult = -2;
			}
			catch (XMLException& e)
			{
				cout << "\nXMLException ";

#if defined(XALAN_OSTREAM_HAS_WCHAR_T)
				cout << "Type is : ";

				OutputString(cout, e.getType());

				cout << endl;

				cout << "Message is : ";

				OutputString(cout, e.getMessage());

				cout << endl;
#else
				cout << "Type is : " << e.getType() << endl;

				cout << "Message is : " << e.getMessage() << endl;
#endif
				theResult = -3;
			}
			catch(const XalanDOMException&	e)
			{
				cout << endl
					 << "XalanDOMException caught.  The code is "
					 << int(e.getExceptionCode())
					 << "."
					 << endl;

				theResult = -4;
			}
			catch (...)
			{
				cout << "\nUnhandled Exception\n";

				theResult = -5;
			}

#if !defined(NDEBUG)
			const size_t	theInstanceCount =
				XalanNode::getInstanceCount();

			if (theInstanceCount > 0)
			{
				cout << "There are "
					 << XalanNode::getInstanceCount()
					 << " XalanNode instances still alive!"
					 << endl
					 << endl
					 << "A dump of these instances follows..."
					 << endl
					 << endl;

				vector<XalanNode*>	theNodes(theInstanceCount, 0);

				XalanNode::getLiveInstances(&*theNodes.begin());

				for(unsigned int i = 0; i < theInstanceCount; ++i)
				{
					const XalanNode* const	theInstance = theNodes[i];

					if(theInstance == 0)
					{
						cout << "No instance information is available..."
							 << endl;
					}
					else
					{
						cout << "("
							 << hex
							 << theInstance
							 << ")  Node name: \""
							 << theInstance->getNodeName()
							 << "\"  Node value: \""
							 << theInstance->getNodeValue()
							 << "\""
#if defined(XALAN_RTTI_AVAILABLE) && !defined(XALAN_NO_TYPEINFO)
							 << "  Type: \""
							 << typeid(*theInstance).name()
							 << "\""
#endif
							 << endl
							 << endl;
					}
				}
			}
#endif

		}
	}

	XMLPlatformUtils::Terminate();

	return theResult;
}
