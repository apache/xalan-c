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

#include <xalanc/Include/PlatformDefinitions.hpp>



#include <cstdio>
#include <cstring>

#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#else
#include <iostream>
#endif



#if !defined(NDEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif



#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/sax/SAXParseException.hpp>



#include <xalanc/XalanDOM/XalanDOMException.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/DOMStringPrintWriter.hpp>
#include <xalanc/PlatformSupport/XalanOutputStreamPrintWriter.hpp>
#include <xalanc/PlatformSupport/XalanFileOutputStream.hpp>
#include <xalanc/PlatformSupport/XalanStdOutputStream.hpp>



#include <xalanc/Include/XalanAutoPtr.hpp>



#include <xalanc/DOMSupport/DOMSupportDefault.hpp>



#include <xalanc/XPath/XObjectFactoryDefault.hpp>
#include <xalanc/XPath/XPathEnvSupportDefault.hpp>
#include <xalanc/XPath/XPath.hpp>
#include <xalanc/XPath/XPathExecutionContextDefault.hpp>
#include <xalanc/XPath/XPathFactoryBlock.hpp>
#include <xalanc/XPath/XPathFactoryDefault.hpp>
#include <xalanc/XPath/XPathProcessorImpl.hpp>



#include <xalanc/XalanExtensions/XalanExtensions.hpp>



#include <xalanc/XalanEXSLT/XalanEXSLTCommon.hpp>
#include <xalanc/XalanEXSLT/XalanEXSLTMath.hpp>
#include <xalanc/XalanEXSLT/XalanEXSLTSet.hpp>
#include <xalanc/XalanEXSLT/XalanEXSLTString.hpp>



#include <xalanc/XercesParserLiaison/XercesParserLiaison.hpp>
#include <xalanc/XercesParserLiaison/XercesDOMSupport.hpp>



#include <xalanc/XMLSupport/FormatterToHTML.hpp>
#include <xalanc/XMLSupport/FormatterToNull.hpp>
#include <xalanc/XMLSupport/FormatterToText.hpp>
#include <xalanc/XMLSupport/FormatterToXML.hpp>
#include <xalanc/XMLSupport/FormatterTreeWalker.hpp>



#include <xalanc/XercesParserLiaison/FormatterToXercesDOM.hpp>



#include <xalanc/XalanSourceTree/FormatterToSourceTree.hpp>
#include <xalanc/XalanSourceTree/XalanSourceTreeDOMSupport.hpp>
#include <xalanc/XalanSourceTree/XalanSourceTreeDocument.hpp>
#include <xalanc/XalanSourceTree/XalanSourceTreeInit.hpp>
#include <xalanc/XalanSourceTree/XalanSourceTreeParserLiaison.hpp>



#include <xalanc/XSLT/XSLTEngineImpl.hpp>
#include <xalanc/XSLT/XSLTInit.hpp>
#include <xalanc/XSLT/XSLTInputSource.hpp>
#include <xalanc/XSLT/XSLTResultTarget.hpp>
#include <xalanc/XSLT/StylesheetRoot.hpp>
#include <xalanc/XSLT/StylesheetConstructionContextDefault.hpp>
#include <xalanc/XSLT/StylesheetExecutionContextDefault.hpp>
#include <xalanc/XSLT/TraceListenerDefault.hpp>
#include <xalanc/XSLT/XSLTProcessorEnvSupportDefault.hpp>



//#define XALAN_USE_ICU
#if defined(XALAN_USE_ICU)
#include <xalanc/ICUBridge/ICUBridgeCollationCompareFunctor.hpp>
#endif



//#define XALAN_VQ_SPECIAL_TRACE
#if defined(XALAN_VQ_SPECIAL_TRACE)
#include "C:/Program Files/Rational/Quantify/pure.h"
#endif



XALAN_USING_STD(cerr)
XALAN_USING_STD(cin)
XALAN_USING_STD(cout)
XALAN_USING_STD(endl)
XALAN_USING_STD(hex)
XALAN_USING_STD(pair)
XALAN_USING_STD(vector)



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
		 << " [-? Shows this message.]"
		 << endl
		 << endl
		 << " [-h Shows this message.]"
		 << endl
		 << endl
		 << " [-IN inputXMLURL (If not specified, stdin is used.)]"
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
		 << " [-INDENT n (Controls how many spaces to indent. {default is 0})]"
		 << endl
		 << " [-VALIDATE (Controls whether validation occurs. Validation is off by default.)]"
		 << endl
		 << endl
		 << " [-TT (Trace the templates as they are being called.)]"
		 << endl
		 << " [-TG (Trace each generation event.)]"
		 << endl
		 << " [-TS (Trace each selection event.)]"
		 << endl
		 << " [-TTC (Trace the template children as they are being processed.)]"
		 << endl
		 << endl
		 << " [-XML (Use XML formatter and add XML header.)]"
		 << endl
		 << " [-TEXT (Use Text formatter.)]"
		 << endl
		 << " [-HTML (Use HTML formatter.)]"
		 << endl
		 << " [-DOM (Use DOM formatter.  Formats to DOM, then formats XML for output.)]"
		 << endl
		 << " [-XST (Use source tree formatter.  Formats to Xalan source tree, then formats XML for output.)]"
		 << endl
		 << endl
		 << " [-PARAM name expression (Sets a stylesheet parameter.)]"
		 << endl
		 << endl
		 << " [-XD (Use Xerces DOM instead of Xalan source tree.)]"
		 << endl
		 << endl
		 << " [-DE (Disable built-in extension functions.)]"
		 << endl
		 << "The following options are valid only with -HTML or -XML."
		 << endl
		 << endl
		 << "The following option is valid only with -HTML."
		 << endl
		 << endl
		 << " [-NOINDENT (Turns off HTML indenting..]"
		 << endl
		 << endl
		 << "The following option is valid only with -XML."
		 << endl
		 << endl
		 << " [-NH (Don't write XML header.)]"
		 << endl;
}



typedef vector<pair<const char*, const char*> >	StringPairVectorType;



struct CmdLineParams
{
	StringPairVectorType	params;

	bool		setQuietConflictWarnings;
	bool		setQuietMode;
	bool		versionOnly;
	bool		traceTemplates;
	bool		traceGenerationEvent;
	bool		traceSelectionEvent;
	bool		traceTemplateChildren;
	bool		shouldWriteXMLHeader;
	bool		doValidation;
	bool		noIndent;
	bool		formatToNull;
	bool		formatToSourceTree;
	bool		useDOM;
	bool		disableExtensions;

	int			indentAmount;
	int			outputType;

	const char*		outFileName;
	const char*		xslFileName;
	const char*		inFileName;

	CmdLineParams() :
		params(),
		setQuietConflictWarnings(false),
		setQuietMode(false),
		versionOnly(false),
		traceTemplates(false),
		traceGenerationEvent(false),
		traceSelectionEvent(false),
		traceTemplateChildren(false),
		shouldWriteXMLHeader(true),
		doValidation(false),
		noIndent(false),
		formatToNull(false),
		formatToSourceTree(false),
		useDOM(false),
		disableExtensions(false),
		indentAmount(-1),
		outputType(-1),
		outFileName(0),
		xslFileName(0),
		inFileName(0)
	{
	}
};



// We only need a few things from the Xerces namespace...
XALAN_USING_XERCES(XMLPlatformUtils)
XALAN_USING_XERCES(SAXParseException)
XALAN_USING_XERCES(SAXException)
XALAN_USING_XERCES(XMLException)



// We need lots of things from the Xalan namespace, so hoist everything...
XALAN_CPP_NAMESPACE_USE



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



#if defined(OS390)
#include <strings.h>                                             
                                                                  
int
compareNoCase(
			const char*		str1,
			const char*		str2)     
{
	return strcasecmp(str1, str2);
}

#else

int
compareNoCase(
			const char*		str1,
			const char*		str2)     
{
	return stricmp(str1, str2);
}

#endif



bool
getArgs(
			int				argc,
			char*			argv[],
			CmdLineParams&	p)
{
	bool fSuccess = true;

	for (int i = 1; i < argc && fSuccess == true; ++i)
	{
		if (!compareNoCase("-h", argv[i]) || !compareNoCase("-?", argv[i]))
		{
			fSuccess = false;
		}
		else if (!compareNoCase("-IN", argv[i]))
		{
			++i;

			if(i < argc && argv[i][0] != '-')
			{
				p.inFileName = argv[i];
			}
			else
			{
				fSuccess = false;
			}
		}
		else if (!compareNoCase("-XSL", argv[i]))
		{
			++i;

			if(i < argc && argv[i][0] != '-')
			{
				p.xslFileName = argv[i];
			}
			else
			{
				fSuccess = false;
			}
		}
		else if (!compareNoCase("-OUT", argv[i]))
		{
			++i;

			if(i < argc && argv[i][0] != '-')
			{
				p.outFileName = argv[i];
			}
			else
			{
				fSuccess = false;
			}
		}
		else if (!compareNoCase("-NOINDENT", argv[i]))
		{
			p.noIndent = true;
		} 
		else if (!compareNoCase("-INDENT", argv[i]))
		{
			++i;

			if(i < argc && argv[i][0] != '-')
			{
#if defined(XALAN_STRICT_ANSI_HEADERS)
				p.indentAmount = std::atoi(argv[i]);
#else
				p.indentAmount = atoi(argv[i]);
#endif
			}
			else
			{
				fSuccess = false;
			}
		}
		else if(!compareNoCase("-VALIDATE", argv[i]))
		{
			p.doValidation = true;
		}
		else if (!compareNoCase("-PARAM", argv[i])) 
		{
			++i;

			if(i < argc && argv[i][0] != '-')
			{
				const char* const	name = argv[i];

				++i;

				// Don't check for '-' here, since that might
				// be a valid character in a parameter value.
				if(i < argc)
				{
					typedef StringPairVectorType::value_type	value_type;

					p.params.push_back(value_type(name, argv[i]));
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
		else if(!compareNoCase("-V", argv[i]))
		{
			p.versionOnly = true;
		}
		else if(!compareNoCase("-QC", argv[i]))
		{
			p.setQuietConflictWarnings = true;
		}
		else if(!compareNoCase("-Q", argv[i]))
		{
			p.setQuietMode = true;
		}
		else if(!compareNoCase("-XML", argv[i]))
		{
			if (p.outputType != -1)
			{
				warnPreviousOutputMethod(p.outputType);
			}

			p.outputType = FormatterListener::OUTPUT_METHOD_XML;
		}
		else if(!compareNoCase("-TEXT", argv[i]))
		{
			if (p.outputType != -1)
			{
				warnPreviousOutputMethod(p.outputType);
			}

			p.outputType = FormatterListener::OUTPUT_METHOD_TEXT;
		}
		else if(!compareNoCase("-HTML", argv[i]))
		{
			if (p.outputType != -1)
			{
				warnPreviousOutputMethod(p.outputType);
			}

			p.outputType = FormatterListener::OUTPUT_METHOD_HTML;
		}
		else if(!compareNoCase("-DOM", argv[i]))
		{
			if (p.outputType != -1)
			{
				warnPreviousOutputMethod(p.outputType);
			}

			p.outputType = FormatterListener::OUTPUT_METHOD_DOM;
		}
		else if(!compareNoCase("-XST", argv[i]))
		{
			if (p.outputType != -1)
			{
				warnPreviousOutputMethod(p.outputType);
			}

			p.outputType = FormatterListener::OUTPUT_METHOD_DOM;

			p.formatToSourceTree = true;
		}
		else if(!compareNoCase("-NULL", argv[i]))
		{
			p.formatToNull = true;
		}
		else if (!compareNoCase("-NH", argv[i]))
		{
			p.shouldWriteXMLHeader = false;
		}
		else if(!compareNoCase("-TT", argv[i]))
		{
			p.traceTemplates = true;
		}
		else if(!compareNoCase("-TG", argv[i]))
		{
			p.traceGenerationEvent = true;
		}
		else if(!compareNoCase("-TS", argv[i]))
		{
			p.traceSelectionEvent = true;
		}
		else if(!compareNoCase("-TTC", argv[i]))
		{
			p.traceTemplateChildren = true;
		}
		else if (!compareNoCase("-XD", argv[i]))
		{
			p.useDOM = true;
		}
		else if (!compareNoCase("-DE", argv[i]))
		{
			p.disableExtensions = true;
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
			int								outputType,
			bool							shouldWriteXMLHeader,
			bool							noIndent,
			bool							formatToNull,
			bool							formatToSourceTree,
			PrintWriter&					resultWriter,
			int								indentAmount,
			const XalanDOMString&			mimeEncoding,
			const StylesheetRoot*			stylesheet,
			XercesParserLiaison&		    parserLiaison,
			XalanSourceTreeParserLiaison&	sourceTreeParserLiaison,
			const PrefixResolver&			prefixResolver,
			const XalanDocument*&			theResultDocument)
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
			version = stylesheet->getOutputVersion();

			mediatype = stylesheet->getOutputMediaType();
			doctypeSystem = stylesheet->getOutputDoctypeSystem();
			doctypePublic = stylesheet->getOutputDoctypePublic();
			standalone = stylesheet->getOutputStandalone();
		}

		FormatterToXML* const	fToXML =
			new FormatterToXML(
					resultWriter,
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
			version = stylesheet->getOutputVersion();

			if (noIndent == false)
			{
				outputIndent = stylesheet->getOutputIndent();
			}

			mediatype = stylesheet->getOutputMediaType();
			doctypeSystem = stylesheet->getOutputDoctypeSystem();
			doctypePublic = stylesheet->getOutputDoctypePublic();
			standalone = stylesheet->getOutputStandalone();
		}

		FormatterToHTML* const	fToHTML =
				new FormatterToHTML(
						resultWriter,
						mimeEncoding,
						mediatype,
						doctypeSystem,
						doctypePublic,
						outputIndent,
						indentAmount);

		fToHTML->setPrefixResolver(&prefixResolver);

		formatter = fToHTML;
	}
	else if(FormatterListener::OUTPUT_METHOD_DOM == outputType)
	{
		if (formatToSourceTree == true)
		{
			XalanSourceTreeDocument*	theDocument =
				sourceTreeParserLiaison.createXalanSourceTreeDocument();
			assert(theDocument != 0);

			theResultDocument = theDocument;

			FormatterToSourceTree* const	fToSourceTree =
				new FormatterToSourceTree(theDocument);

			fToSourceTree->setPrefixResolver(&prefixResolver);

			formatter = fToSourceTree;
		}
		else
		{
			DOMDocument_Type* const     theDocument =
				parserLiaison.createDOMFactory();
			assert(theDocument != 0);

			FormatterToXercesDOM* const     fToDOM =
				new FormatterToXercesDOM(theDocument, 0);

			fToDOM->setPrefixResolver(&prefixResolver);

			formatter = fToDOM;

            theResultDocument =
                parserLiaison.createDocument(
                    theDocument,
                    false,
                    false,
                    false);
		}
	}

	return formatter;
}



XalanOutputStream*
createOutputStream(const CmdLineParams&		params)
{
	if (params.outFileName == 0)
	{
		return new XalanStdOutputStream(cout);
	}
	else
	{
		return new XalanFileOutputStream(TranscodeFromLocalCodePage(params.outFileName));
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
		params.traceSelectionEvent == true)
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



DOMSupport&
getDOMSupport(
		XalanSourceTreeDOMSupport&	theXalanSourceTreeDOMSupport,
		XercesDOMSupport&			theXercesDOMSupport,
		const CmdLineParams&		params)
{
	if (params.useDOM == false)
	{
		return theXalanSourceTreeDOMSupport;
	}
	else
	{
		return theXercesDOMSupport;
	}
}



XMLParserLiaison&
getParserLiaison(
		XalanSourceTreeParserLiaison&	theXalanSourceTreeParserLiaison,
		XercesParserLiaison&			theXercesParserLiaison,
		const CmdLineParams&			params)
{
	if (params.useDOM == false)
	{
		return theXalanSourceTreeParserLiaison;
	}
	else
	{
		return theXercesParserLiaison;
	}
}



int
xsltMain(const CmdLineParams&	params)
{
	// Initialize the XSLT subsystem.  This must stay in scope until
	// we're done with the subsystem, since its destructor shuts down
	// the subsystem.
	XSLTInit	theInit;

	const XalanDOMString	mimeEncoding(XALAN_STATIC_UCODE_STRING("UTF-8"));
	const XalanDOMString	encoding(XALAN_STATIC_UCODE_STRING("UTF-8"));

	/**
	 * The default diagnostic writer...
	 */
	XalanStdOutputStream				theStdErr(cerr);
	XalanOutputStreamPrintWriter		diagnosticsWriter(theStdErr);

	// Make sure that error reporting, which includes any TraceListener output
	// does not throw exceptions when transcoding, since that could result in
	// an exception being thrown will another exception is active.  In particular,
	// characters that the TraceListener writes might not be representable in the
	// local code page.
	theStdErr.setThrowTranscodeException(false);

	// Initialize the XalanSourceTree subsystem.  This must stay in scope until
	// we're done with the subsystem, since its destructor shuts down the
	// subsystem.
	XalanSourceTreeInit				theXalanSourceTreeInit;

	XalanSourceTreeDOMSupport		theXalanSourceTreeDOMSupport;
	XalanSourceTreeParserLiaison	theXalanSourceTreeParserLiaison(theXalanSourceTreeDOMSupport);

	// Hookup the parser liaison instance to the support instance.
	theXalanSourceTreeDOMSupport.setParserLiaison(&theXalanSourceTreeParserLiaison);


	XercesDOMSupport		theXercesDOMSupport;
	XercesParserLiaison		theXercesParserLiaison;

	DOMSupport&				theDOMSupport = getDOMSupport(
		theXalanSourceTreeDOMSupport,
		theXercesDOMSupport,
		params);

	XMLParserLiaison&		xmlParserLiaison = getParserLiaison(
		theXalanSourceTreeParserLiaison,
		theXercesParserLiaison,
		params);

	XSLTProcessorEnvSupportDefault	theXSLProcessorSupport;

	if (params.disableExtensions == false)
	{
		XalanExtensionsInstaller::installGlobal();
		XalanEXSLTCommonFunctionsInstaller::installGlobal();
		XalanEXSLTMathFunctionsInstaller::installGlobal();
		XalanEXSLTSetFunctionsInstaller::installGlobal();
		XalanEXSLTStringFunctionsInstaller::installGlobal();
	}

	XObjectFactoryDefault	theXObjectFactory;

	XPathFactoryDefault		theXPathFactory;

	const XalanAutoPtr<TraceListener>		theTraceListener(
			createTraceListener(
				params,
				diagnosticsWriter));

	XSLTEngineImpl	processor(
			xmlParserLiaison,
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

	// Use a different factory type for the stylesheet.  This is an optimization, since
	// stylesheet XPath instances are built all at once and are deleted all at once when
	// the stylesheet is destroyed.
	XPathFactoryBlock	theStylesheetXPathFactory;

	StylesheetConstructionContextDefault	theConstructionContext(
			processor,
			theStylesheetXPathFactory);

	/*
	 * Set specified processor flags
	 */
	processor.setQuietConflictWarnings(params.setQuietConflictWarnings);

	if (params.params.empty() == false)
	{
		StringPairVectorType::const_iterator	it = params.params.begin();

		for ( ; it != params.params.end(); ++it)
		{
			assert((*it).first != 0 && (*it).second != 0);

			processor.setStylesheetParam(
					XalanDOMString((*it).first),
					XalanDOMString((*it).second));
		}
	}

	/*
	 * Set specified parser flags
	 */
	if (params.indentAmount != 0)
	{
		xmlParserLiaison.setIndent(params.indentAmount);
	}

	xmlParserLiaison.setUseValidation(params.doValidation);

	if (!params.setQuietMode)
	{
		processor.setDiagnosticsOutput(&diagnosticsWriter);
	}

	XalanDOMString	xslFileName;

	if(params.xslFileName != 0)
	{
		xslFileName = params.xslFileName;
	}

	const StylesheetRoot*	stylesheet = 0;

	if (!xslFileName.empty())
	{
		stylesheet = processor.processStylesheet(xslFileName, theConstructionContext);
	}

	XalanAutoPtr<XalanOutputStream>		outputFileStream(createOutputStream(params));
	assert(outputFileStream.get() != 0);

	XalanOutputStreamPrintWriter	resultWriter(*outputFileStream.get());

	const XalanDocument*	theResultDocument = 0;

	const XalanAutoPtr<FormatterListener>	formatter(
			createFormatter(
				params.outputType,
				params.shouldWriteXMLHeader,
				params.noIndent,
				params.formatToNull,
				params.formatToSourceTree,
				resultWriter,
				xmlParserLiaison.getIndent(),
				mimeEncoding,
				stylesheet,
				theXercesParserLiaison,
				theXalanSourceTreeParserLiaison,
				processor,
				theResultDocument));

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
	XSLTInputSource		theInputSource;

	if (params.inFileName != 0)
	{
		theInputSource.setSystemId(XalanDOMString(params.inFileName).c_str());
	}
	else
	{
		theInputSource.setStream(&cin);

		cerr << "Reading input document from stdin..." << endl;
	}

	StylesheetExecutionContextDefault	theExecutionContext(processor,
			theXSLProcessorSupport,
			theDOMSupport,
			theXObjectFactory);

#if defined(XALAN_USE_ICU)
	// Create a collation function for the ICU, and have it
	// cache collators...
	ICUBridgeCollationCompareFunctor	theICUFunctor(true);

	theExecutionContext.installCollationCompareFunctor(&theICUFunctor);
#endif

	if (params.useDOM == false)
	{
		theXalanSourceTreeParserLiaison.setExecutionContext(theExecutionContext);
	}
	else
	{
		theXercesParserLiaison.setExecutionContext(theExecutionContext);
	}

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

		if (theResultDocument == 0)
		{
			cerr << endl << "Error: No document to format!!!" << endl;
		}
		else
		{
			// Create a FormaterToXML with the required output
			// options...
			const XalanAutoPtr<FormatterListener>	formatter(
					createFormatter(
						FormatterListener::OUTPUT_METHOD_XML,
						params.shouldWriteXMLHeader,
						params.noIndent,
						false,
						false,
						resultWriter,
						xmlParserLiaison.getIndent(),
						mimeEncoding,
						stylesheet,
						theXercesParserLiaison,
						theXalanSourceTreeParserLiaison,
						processor,
						theResultDocument));

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
	theDOMSupport.reset();

	xmlParserLiaison.reset();

	if (params.disableExtensions == false)
	{
		XalanExtensionsInstaller::uninstallGlobal();
		XalanEXSLTCommonFunctionsInstaller::uninstallGlobal();
		XalanEXSLTMathFunctionsInstaller::uninstallGlobal();
		XalanEXSLTSetFunctionsInstaller::uninstallGlobal();
		XalanEXSLTStringFunctionsInstaller::uninstallGlobal();
	}

	return 0;
}



int
main(
			int		argc,
			char*	argv[])
{
#if !defined(NDEBUG) && defined(_MSC_VER)
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif

#if defined(XALAN_VQ_SPECIAL_TRACE)
	QuantifyStopRecordingData();
#endif

	int				theResult = 0;

	CmdLineParams	theParams;

	/*
	 *		Get command line arguments
	 */
	if (getArgs(argc, argv, theParams) == false)
	{
		printArgOptions();
	}
	else if (theParams.versionOnly == true)
	{
		cout << endl
			 << "Xalan version "
			 << XALAN_FULLVERSIONDOT
			 << endl
			 << "Xerces version "
			 << XERCES_FULLVERSIONDOT
			 << endl;
	}
	else
	{
		XMLPlatformUtils::Initialize();

		try
		{
			theResult = xsltMain(theParams);
		}
		catch (const XSLException&	e)
		{
			cout << "\nXSLException ";

			cout << "Type is: " << e.getType() << endl;

			cout << "Message is: " << e.getMessage() << " (";

			const XalanDOMString&	theURI = e.getURI();

			if (theURI.length() != 0)
			{
				cout << theURI;
			}
			else
			{
				cout << "Unknown URI";
			}

			cout << ", line "
				 << e.getLineNumber()
				 << ", column "
				 << e.getColumnNumber()
				 << ")"
				 << endl;

			theResult = -1;
		}
		catch (const SAXParseException&		e)
		{
			cout << "\nSAXParseException ";

			cout << "Message is: " << e.getMessage() << " (";

			const XMLCh* const	theSystemID = e.getSystemId();

			if (theSystemID != 0)
			{
				cout << XalanDOMString(theSystemID);
			}
			else
			{
				cout << "Unknown system ID";
			}

			cout << ", line "
				 << e.getLineNumber()
				 << ", column "
				 << e.getColumnNumber()
				 << ")"
				 << endl;

			theResult = -2;
		}
		catch (const SAXException&	e)
		{
			cout << "\nSAXException ";

			cout << "Message is: " << e.getMessage() << endl;

			theResult = -2;
		}
		catch (const XMLException&	e)
		{
			cout << "\nXMLException ";

			cout << "Type is: " << e.getType() << endl;

			cout << "Message is: " << e.getMessage() << endl;

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

		XMLPlatformUtils::Terminate();
	}

	return theResult;
}
