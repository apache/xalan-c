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

#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>



#include <util/PlatformUtils.hpp>
#include <sax/SAXException.hpp>



#include <XalanDOM/XalanDOMException.hpp>


#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/DOMStringPrintWriter.hpp>


#include <DOMSupport/DOMSupportDefault.hpp>


#include <XPath/XObjectFactoryDefault.hpp>
#include <XPath/XPathEnvSupportDefault.hpp>
#include <XPath/XPathSupportDefault.hpp>
#include <XPath/XPath.hpp>
#include <XPath/XPathExecutionContextDefault.hpp>
#include <XPath/XPathFactoryDefault.hpp>
#include <XPath/XPathProcessorImpl.hpp>


#include <XercesPlatformSupport/XercesDOMPrintWriter.hpp>
#include <XercesPlatformSupport/TextFileOutputStream.hpp>
#include <XercesPlatformSupport/XercesStdTextOutputStream.hpp>


#include <XercesParserLiaison/XercesParserLiaison.hpp>


#include <XMLSupport/FormatterToHTML.hpp>
#include <XMLSupport/FormatterToText.hpp>
#include <XMLSupport/FormatterToXML.hpp>


#include <XSLT/XSLTEngineImpl.hpp>
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
#endif



#if !defined (XALAN_NO_NAMESPACES)
using std::auto_ptr;
using std::cerr;
using std::cout;
using std::endl;
using std::hex;
using std::map;
using std::string;
using std::vector;
#endif



/**
 * Print argument options.
 */ 
void
printArgOptions()
{
	cerr << "TestXSLT options: "
		 << endl
		 << "		-IN inputXMLURL"
		 << endl
		 << "	 [-XSL XSLTransformationURL]"
		 << endl
		 << "	 [-OUT outputFileName]"
		 << endl
		 << "	 [-V (Version info)]"
		 << endl
		 << "	 [-QC (Quiet Pattern Conflicts Warnings)]"
		 << endl
		 << "	 [-Q (Quiet Mode)]"
		 << endl
		 << "	 [-ESCAPE (Which characters to escape {default is <>&\"\'\\r\\n}]"
		 << endl
		 << "	 [-INDENT n(Control how many spaces to indent {default is 0})]"
		 << endl
		 << "	 [-TT (Trace the templates as they are being called.)]"
		 << endl
		 << "	 [-TG (Trace each generation event.)]"
		 << endl
		 << "	 [-TS (Trace each selection event.)]"
		 << endl
		 << "	 [-TTC (Trace the template children as they are being processed.)]"
		 << endl
		 << "	 [-VALIDATE (Set whether validation occurs. Validation is off by default.)]"
		 << endl
		 << "	 [-XML (Use XML formatter and add XML header.)]"
		 << endl
		 << "	 [-NH (Don't write XML header. Works only with previous option.)]"
		 << endl
		 << "	 [-TEXT (Use simple Text formatter.)]"
		 << endl
		 << "	 [-HTML (Use HTML formatter.)]"
		 << endl
		 << "	 [-PARAM name expression (Set a stylesheet parameter)]"
		 << endl;
}



typedef map<string, string> String2StringMapType;



struct CmdLineParams
{
	String2StringMapType paramsMap;
	bool doStackDumpOnError;
	bool escapeCData;
	bool formatOutput;
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
	int indentAmount;
	int outputType;
	string dumpFileName;
	string  outFileName;
	string specialCharacters;
	string treedumpFileName;
	string xslFileName;
	string inFileName;

	CmdLineParams() :
		paramsMap(),
		doStackDumpOnError(false),
		escapeCData(false),
		formatOutput(false),
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
		indentAmount(0),
		outputType(-1),
		dumpFileName(),
		specialCharacters(),
		treedumpFileName(),
		outFileName(),
		xslFileName(),
		inFileName()
	{
	}
};



void
getArgs(int argc, const char* argv[], CmdLineParams& p)
{
	for (int i = 1;	i < argc;	i ++) 
	{
		if (!stricmp("-ESCAPE", argv[i])) 
		{
			p.specialCharacters = argv[++i];
		}
		else if (!stricmp("-INDENT", argv[i])) 
		{
			if(((i+1) < argc) && (argv[i+1][0] != '-'))
				p.indentAmount = atoi( argv[++i] );
		} 
		else if (!stricmp("-IN", argv[i])) 
		{
			p.inFileName = argv[++i];
		}
		else if (!stricmp("-OUT", argv[i])) 
		{
			p.outFileName = argv[++i];
		}
		else if (!stricmp("-XSL", argv[i])) 
		{
			p.xslFileName = argv[++i];
		}
		else if (!stricmp("-PARAM", argv[i])) 
		{
			string name = argv[++i];
			string expression = argv[++i];
			p.paramsMap[name] = expression;
		}
		else if(!stricmp("-F", argv[i]))
		{
			p.formatOutput = true;
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
			p.outputType = FormatterListener::OUTPUT_METHOD_XML;
		}
		else if(!stricmp("-TEXT", argv[i]))
		{
			p.outputType = FormatterListener::OUTPUT_METHOD_TEXT;
		}
		else if(!stricmp("-HTML", argv[i]))
		{
			p.outputType = FormatterListener::OUTPUT_METHOD_HTML;
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
			cout << endl << "Warning: Ignoring unknown option \"" << argv[i] << "\"" << endl << endl;
		}
	}
}



FormatterListener*
createFormatter(
			const CmdLineParams&		params,
			PrintWriter&				resultWriter,
			int							indentAmount,
			const XalanDOMString&		mimeEncoding,
			const StylesheetRoot*		stylesheet)
{
	FormatterListener*	formatter = 0;

	const int	outputType = params.outputType;

	if(FormatterListener::OUTPUT_METHOD_XML == outputType)
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
			outputIndent = stylesheet->getOutputIndent();
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

		fToXML->setShouldWriteXMLHeader(params.shouldWriteXMLHeader);
		fToXML->setStripCData(params.stripCData);
		fToXML->setEscapeCData(params.escapeCData);
		formatter = fToXML;
	}
	else if(FormatterListener::OUTPUT_METHOD_TEXT == outputType)
	{
		formatter = new FormatterToText(resultWriter);
	}
	else if(FormatterListener::OUTPUT_METHOD_HTML == outputType)
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
			outputIndent = stylesheet->getOutputIndent();
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

		fToHTML->setStripCData(params.stripCData);
		formatter = fToHTML;
	}

	return formatter;
}



TextOutputStream*
createOutputStream(const CmdLineParams&		params)
{
	if (params.outFileName.empty())
	{
		return new XercesStdTextOutputStream(cout);
	}
	else
	{
		return new TextFileOutputStream(params.outFileName.c_str());
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
	XercesStdTextOutputStream				theStdErr(cerr);
	XercesDOMPrintWriter					diagnosticsWriter(theStdErr);

	DOMSupportDefault theDOMSupport;
	XercesParserLiaison xmlParserLiaison(theDOMSupport);

	DOMStringPrintWriter pw;

	XPathSupportDefault theXPathSupport(theDOMSupport);
	XSLTProcessorEnvSupportDefault	theXSLProcessorSupport;
	XObjectFactoryDefault theXObjectFactory(theXSLProcessorSupport, theXPathSupport);
	XPathFactoryDefault theXPathFactory;

	XSLTEngineImpl processor(
			xmlParserLiaison, theXPathSupport,
			theXSLProcessorSupport,
			theXObjectFactory,
			theXPathFactory);

	theXSLProcessorSupport.setProcessor(&processor);


	auto_ptr<TraceListener>		theTraceListener(
			createTraceListener(
				params,
				diagnosticsWriter));

	if (theTraceListener.get() != 0)
	{
		processor.setTraceSelects(params.traceSelectionEvent);
		processor.addTraceListener(theTraceListener.get());
	}

	// Use separate factory instances for the stylesheet.  This is really only necessary
	// if you want to use the stylesheet with another processor, or you want to use
	// it multiple times.
	XObjectFactoryDefault	theStylesheetXObjectFactory(theXSLProcessorSupport, theXPathSupport);
	XPathFactoryDefault		theStylesheetXPathFactory;

	StylesheetConstructionContextDefault	theConstructionContext(processor,
			theXSLProcessorSupport,
			theStylesheetXObjectFactory,
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
					(*it).first.c_str(),
					(*it).second.c_str());
		}
	}

	/*
	 * Set specified parser flags
	 */
	if (params.indentAmount != 0)
	{
		xmlParserLiaison.setIndent(params.indentAmount);
	}

	xmlParserLiaison.setSpecialCharacters(params.specialCharacters.c_str());
	xmlParserLiaison.SetShouldExpandEntityRefs(params.shouldExpandEntityRefs);

	assert(params.inFileName.size() > 0);

	if (!params.setQuietMode)
	{
		processor.setDiagnosticsOutput(&diagnosticsWriter);
	}

	XalanDOMString	xslFileName;

	if(0 != params.xslFileName.size())
	{
		xslFileName = params.xslFileName.c_str();
	}

	const StylesheetRoot*	stylesheet = 0;

	StylesheetExecutionContextDefault		theExecutionContext(processor,
			theXSLProcessorSupport,
			theXPathSupport,
			theXObjectFactory);

	if (!isEmpty(xslFileName))
	{
		stylesheet = processor.processStylesheet(xslFileName, theConstructionContext);
	}

	auto_ptr<TextOutputStream>	outputFileStream(createOutputStream(params));
	assert(outputFileStream.get() != 0);

	XercesDOMPrintWriter	resultWriter(*outputFileStream.get());

	const auto_ptr<FormatterListener>	formatter(
			createFormatter(
				params,
				resultWriter,
				xmlParserLiaison.getIndent(),
				mimeEncoding,
				stylesheet));

	XSLTResultTarget	rTreeTarget;

	if(formatter.get() == 0)
	{
		rTreeTarget.setCharacterStream(&resultWriter);
	}
	else
	{
		rTreeTarget.setFormatterListener(formatter.get());

		xmlParserLiaison.setFormatterListener(formatter.get());
	}


	// Do the transformation...
	XSLTInputSource		theInputSource(params.inFileName.c_str());

	if (stylesheet == 0)
	{
		// No stylesheet, so the only hope is that the xml file has
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

	return 0;
}



int main(int argc, const char* argv[]) throw()
{
	/**
	 * Command line interface to transform the XML according to 
	 * the instructions found in the XSL document.
	 *		-in inputXMLURL
	 *		-xsl XSLTransformationURL
	 *		-out outputFileName
	 *		-F (Format output pretty-printed)
	 */
	
	XMLPlatformUtils::Initialize();
	XSLTEngineImpl::Initialize();

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
		getArgs(argc, argv, theParams);

		if (theParams.versionOnly == true)
		{
			cout << endl
				 << "TestXSLT version 0.31.0 (Xalan C++ version 0.31.0)"
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
				theResult = xsltMain(theParams);
			}
			catch (XSLException& e)
			{
				cout << "\nXSLException ";

				const string	type = DOMStringToStdString(e.getType());

				if (!type.empty())
					cout << "Type is : " << type << endl;

				const string	msg = DOMStringToStdString(e.getMessage());

				if (!msg.empty())
					cout << "Message is : " << msg << endl;

				theResult = -1;
			}
			catch (SAXException& e)
			{
				cout << "\nSAXException ";

				const string	msg = DOMStringToStdString(e.getMessage());

				if (!msg.empty())
					cout << "Message is : " << msg << endl;

				theResult = -1;
			}
			catch (XMLException& e)
			{
				cout << "\nXMLException ";

				const string	type = DOMStringToStdString(e.getType());

				if (!type.empty())
					cout << "Type is : " << type << endl;

				const string	msg = DOMStringToStdString(e.getMessage());

				if (!msg.empty())
					cout << "Message is : " << msg << endl;

				theResult = -1;
			}
			catch(const XalanDOMException&	e)
			{
				cout << endl
					 << "XalanDOMException caught.  The code is "
					 << e.getExceptionCode()
					 << "."
					 << endl;

				theResult = -1;
			}
			catch (...)
			{
				cout << "\nUnhandled Exception\n";
			}

#if !defined(NDEBUG)
			const unsigned long		theInstanceCount =
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

				vector<XalanNode*>	theNodes(theInstanceCount);

				XalanNode::getLiveInstances(theNodes.begin());

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

	return theResult;
}
