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
#else
#include <iostream>
#endif



#if !defined(NDEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif



#include <util/PlatformUtils.hpp>
#include <sax/SAXParseException.hpp>



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
#include <XPath/XPath.hpp>
#include <XPath/XPathExecutionContextDefault.hpp>
#include <XPath/XPathFactoryBlock.hpp>
#include <XPath/XPathFactoryDefault.hpp>
#include <XPath/XPathProcessorImpl.hpp>



#include <XalanExtensions/XalanExtensions.hpp>



#include <XercesParserLiaison/XercesParserLiaison.hpp>
#include <XercesParserLiaison/XercesDOMSupport.hpp>



#include <XMLSupport/FormatterToDOM.hpp>
#include <XMLSupport/FormatterToHTML.hpp>
#include <XMLSupport/FormatterToNull.hpp>
#include <XMLSupport/FormatterToText.hpp>
#include <XMLSupport/FormatterToXML.hpp>
#include <XMLSupport/FormatterTreeWalker.hpp>



#include <XalanSourceTree/FormatterToSourceTree.hpp>
#include <XalanSourceTree/XalanSourceTreeDOMSupport.hpp>
#include <XalanSourceTree/XalanSourceTreeDocument.hpp>
#include <XalanSourceTree/XalanSourceTreeInit.hpp>
#include <XalanSourceTree/XalanSourceTreeParserLiaison.hpp>



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
#include <ICUBridge/ICUBridgeCollationCompareFunctor.hpp>
#endif



//#define XALAN_VQ_SPECIAL_TRACE
#if defined(XALAN_VQ_SPECIAL_TRACE)
#include "C:/Program Files/Rational/Quantify/pure.h"
#endif



#if !defined (XALAN_NO_NAMESPACES)
using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::hex;
using std::pair;
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
		 << " [-EN (Specify the namespace URI for Xalan extension functions.  The default is 'http://xml.apache.org/xalan')]"
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
	StringPairVectorType params;

	bool		escapeCData;
	bool		setQuietConflictWarnings;
	bool		setQuietMode;
	bool		stripCData;
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
	const char*		extentionsNamespace;

	CmdLineParams() :
		params(),
		escapeCData(false),
		setQuietConflictWarnings(false),
		setQuietMode(false),
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
		formatToSourceTree(false),
		useDOM(false),
		disableExtensions(false),
		indentAmount(-1),
		outputType(-1),
		outFileName(0),
		xslFileName(0),
		inFileName(0),
		extentionsNamespace(0)
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
			const char*		argv[],
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
				p.indentAmount = atoi(argv[i]);
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
		else if(!compareNoCase("-STRIPCDATA", argv[i]))
		{
			p.stripCData = true;
		}
		else if(!compareNoCase("-ESCAPECDATA", argv[i]))
		{
			p.escapeCData = true;
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
		else if (!compareNoCase("-EN", argv[i]))
		{
			++i;

			if(i < argc)
			{
				p.extentionsNamespace = argv[i];

				if (strlen(p.extentionsNamespace) == 0)
				{
					fSuccess = false;
				}
			}
			else
			{
				fSuccess = false;
			}
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
			bool							stripCData,
			bool							escapeCData,
			bool							noIndent,
			bool							formatToNull,
			bool							formatToSourceTree,
			PrintWriter&					resultWriter,
			int								indentAmount,
			const XalanDOMString&			mimeEncoding,
			const StylesheetRoot*			stylesheet,
			XMLParserLiaison&				parserLiaison,
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
			version = stylesheet->m_version;

			mediatype = stylesheet->m_mediatype;
			doctypeSystem = stylesheet->getOutputDoctypeSystem();
			doctypePublic = stylesheet->getOutputDoctypePublic();
			standalone = stylesheet->m_standalone;
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

		FormatterToHTML* const	fToHTML =
				new FormatterToHTML(
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
			XalanDocument* const	theDocument =
				parserLiaison.createDOMFactory();
			assert(theDocument != 0);

			theResultDocument = theDocument;

			FormatterToDOM* const	fToDOM =
				new FormatterToDOM(theDocument, 0);

			fToDOM->setPrefixResolver(&prefixResolver);

			formatter = fToDOM;
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



void
installExtensions(
			const CmdLineParams&				params,
			XSLTProcessorEnvSupportDefault&		theXSLProcessorSupport)
{
	XalanDOMString	theXalanNamespace;

	if (params.extentionsNamespace != 0)
	{
		theXalanNamespace = XalanDOMString(params.extentionsNamespace);
		assert(length(theXalanNamespace) > 0);
	}
	else
	{
		theXalanNamespace = XALAN_STATIC_UCODE_STRING("http://xml.apache.org/xalan");
	}

	theXSLProcessorSupport.installExternalFunctionLocal(
			theXalanNamespace,
			StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("difference")),
			FunctionDifference());

	theXSLProcessorSupport.installExternalFunctionLocal(
			theXalanNamespace,
			StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("distinct")),
			FunctionDistinct());

	theXSLProcessorSupport.installExternalFunctionLocal(
			theXalanNamespace,
			StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("evaluate")),
			FunctionEvaluate());

	theXSLProcessorSupport.installExternalFunctionLocal(
			theXalanNamespace,
			StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("hasSameNodes")),
			FunctionHasSameNodes());

	theXSLProcessorSupport.installExternalFunctionLocal(
			theXalanNamespace,
			StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("intersection")),
			FunctionIntersection());

	theXSLProcessorSupport.installExternalFunctionLocal(
			theXalanNamespace,
			StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("nodeset")),
			FunctionNodeSet());
}



int
xsltMain(const CmdLineParams&	params)
{
	// Initialize the XSLT subsystem.  This must stay in scope until
	// we're done with the subsystem, since its destructor shuts down
	// the subsystem.
	XSLTInit	theInit;

#if defined(XALAN_USE_ICU)
	// Create an installer to install the substitute format-number() function.
	FunctionICUFormatNumber::FunctionICUFormatNumberInstaller	theInstaller;
#endif

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
	XercesParserLiaison		theXercesParserLiaison(theXercesDOMSupport);

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
		installExtensions(params, theXSLProcessorSupport);
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

	if (params.params.size() > 0)
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

	if (!isEmpty(xslFileName))
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
				params.stripCData,
				params.escapeCData,
				params.noIndent,
				params.formatToNull,
				params.formatToSourceTree,
				resultWriter,
				xmlParserLiaison.getIndent(),
				mimeEncoding,
				stylesheet,
				xmlParserLiaison,
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
		theInputSource.setSystemId(c_wstr(XalanDOMString(params.inFileName)));
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
	ICUBridgeCollationCompareFunctor	theICUFunctor;

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
						params.stripCData,
						params.escapeCData,
						params.noIndent,
						false,
						false,
						resultWriter,
						xmlParserLiaison.getIndent(),
						mimeEncoding,
						stylesheet,
						xmlParserLiaison,
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
			 << "TestXSLT version 1.2.1 (Xalan C++ version 1.2.1)"
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

			if (length(theURI) != 0)
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
		catch (const SAXParseException&	e)
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

#if !defined(NDEBUG)
		const size_t	theInstanceCount =
				XalanNode::getInstanceCount();

		if (theInstanceCount > 0)
		{
			cout << "There are "
				 << theInstanceCount
				 << " XalanNode instances still alive!"
				 << endl
				 << endl
				 << "A dump of these instances follows..."
				 << endl
				 << endl;

			typedef vector<XalanNode*>	NodeVectorType;

			NodeVectorType	theNodes(theInstanceCount, NodeVectorType::value_type(0));

			XalanNode::getLiveInstances(&*theNodes.begin());

			for(size_t i = 0; i < theInstanceCount; ++i)
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

		XMLPlatformUtils::Terminate();
	}

	return theResult;
}
