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
 */
#include <cstdio>
#include <cstring>
#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>

#include <string>

#include <dom/DOM_Node.hpp>
#include <dom/DOM_Element.hpp>
#include <dom/DOM_NodeList.hpp>
#include <util/PlatformUtils.hpp>
#include <sax/SAXException.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/DOMStringPrintWriter.hpp>
#include <PlatformSupport/NullPrintWriter.hpp>

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
#include <XercesPlatformSupport/NullTextOutputStream.hpp>

#include <XercesParserLiaison/XercesParserLiaison.hpp>

#include <XMLSupport/Formatter.hpp>
#include <XMLSupport/FormatterToHTML.hpp>
#include <XMLSupport/FormatterToText.hpp>
#include <XMLSupport/FormatterToXML.hpp>


#include <XSLT/XSLTEngineImpl.hpp>
#include <XSLT/XSLTInputSource.hpp>
#include <XSLT/XSLTResultTarget.hpp>
#include <XSLT/StylesheetRoot.hpp>
#include <XSLT/StylesheetConstructionContextDefault.hpp>
#include <XSLT/StylesheetExecutionContextDefault.hpp>
#include <XSLT/XSLTProcessorEnvSupportDefault.hpp>



/**
 * Print argument options.
 */ 
void printArgOptions(XercesDOMPrintWriter&		writer)
{
	writer.println("xslproc options: ");
	writer.println("		-IN inputXMLURL");
	writer.println("	 [-XSL XSLTransformationURL]");
	writer.println("	 [-OUT outputFileName]");
	writer.println("	 [-E (Do not expand entity refs)]");
	writer.println("	 [-V (Version info)]");
	writer.println("	 [-QC (Quiet Pattern Conflicts Warnings)]");
	writer.println("	 [-Q	(Quiet Mode)]");
	writer.println("	 [-ESCAPE (Which characters to escape {default is <>&\"\'\\r\\n}]");
	writer.println("	 [-INDENT (Control how many spaces to indent {default is 0})]");
	writer.println("	 [-TT (Trace the templates as they are being called.)]");
	writer.println("	 [-TG (Trace each generation event.)]");
	writer.println("	 [-TS (Trace each selection event.)]");
	writer.println("	 [-TTC (Trace the template children as they are being processed.)]");
	writer.println("	 [-TCLASS (TraceListener class for trace extensions.)]");
	writer.println("	 [-VALIDATE (Set whether validation occurs.	Validation is off by default.)]");
	writer.println("	 [-XML (Use XML formatter and add XML header.)]");
	writer.println("	 [-TEXT (Use simple Text formatter.)]");
	writer.println("	 [-HTML (Use HTML formatter.)]");
	writer.println("	 [-PARAM name expression (Set a stylesheet parameter)]");
}

typedef std::map<std::string, std::string> String2StringMapType;

struct CmdLineParams
{
	std::string dumpFileName;
	DOMString specialCharacters;
	DOMString treedumpFileName;
	DOMString xslFileName;
	String2StringMapType paramsMap;
	bool doStackDumpOnError;
	bool escapeCData;
	bool formatOutput;
	bool setQuietConflictWarnings;
	bool setQuietMode;
	bool shouldExpandEntityRefs;
	bool stripCData;
	int indentAmount;
	int outputType;
	std::vector <std::string> inFileNames;
	std::string outFileName;
};

typedef CmdLineParams CmdLineParamsType;


void getArgs(int argc, const char* argv[], CmdLineParamsType& p) throw()
{
	p.outputType = -1;
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
			else
				p.indentAmount = 0;
		} 
		else if (!stricmp("-IN", argv[i])) 
		{
			p.inFileNames.push_back(argv[++i]);
			while (! (*argv[i+1] == '-'))	// Multiple entries
				p.inFileNames.push_back(argv[++i]);
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
			std::string name = argv[++i];
			std::string expression = argv[++i];
			p.paramsMap[name] = expression;
		}
		else if (!stricmp("-treedump", argv[i])) 
		{
			p.treedumpFileName = argv[++i];
		}
		else if(!stricmp("-F", argv[i]))
		{
			p.formatOutput = true;
		}
		else if(!stricmp("-E", argv[i]))
		{
			p.shouldExpandEntityRefs = false;	//??
		}
		else if(!stricmp("-V", argv[i])) { }
		else if(!stricmp("-QC", argv[i]))
		{
			p.setQuietConflictWarnings = true;
		}
		else if(!stricmp("-Q", argv[i]))
		{
			p.setQuietMode = true;
		}
		// Not used
		else if(!stricmp("-VALIDATE", argv[i]))
		{
			DOMString shouldValidate;
			if(((i+1) < argc) && (argv[i+1][0] != '-'))
				shouldValidate = argv[++i];
			else
				shouldValidate = "yes";

		}
		else if(!stricmp("-PARSER", argv[i]))
		{
			i++; // Handled above
		}
		else if(!stricmp("-XML", argv[i]))
		{
			p.outputType = Formatter::OUTPUT_METH_XML;
		}
		else if(!stricmp("-TEXT", argv[i]))
		{
			p.outputType = Formatter::OUTPUT_METH_TEXT;
		}
		else if(!stricmp("-HTML", argv[i]))
		{
			p.outputType = Formatter::OUTPUT_METH_HTML;
		}
		else if(!stricmp("-STRIPCDATA", argv[i]))
		{
			p.stripCData = true;
		}
		else if(!stricmp("-ESCAPECDATA", argv[i]))
		{
			p.escapeCData = true;
		}
		else if(!stricmp("-EDUMP", argv[i]))
		{
			p.doStackDumpOnError = true;
			if(((i+1) < argc) && (argv[i+1][0] != '-'))
				p.dumpFileName = argv[++i];
		}
	}
}

static inline bool exists(std::string &filename)
{

	struct stat statBuffer;
	return (0 == stat(filename.c_str(), &statBuffer));
}

void xsltMain(int argc, const char* argv[] ) throw(XMLException)
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

	
	// @@ This should become a command line switch
	bool shouldWriteXMLHeader = false;
	
	// Runtime.getRuntime().traceMethodCalls(false);	
	// Runtime.getRuntime().traceInstructions(false);
	/**
	* The default diagnostic writer...
	*/
	XercesStdTextOutputStream				theStdOut(std::cout);
	XercesStdTextOutputStream				theStdErr(std::cerr);
	NullTextOutputStream					theNullStream;
	XercesDOMPrintWriter					diagnosticsWriter(theStdErr);
	XercesDOMPrintWriter					dumpWriter(theStdErr);

	
	if(argc < 2)
	{
		printArgOptions(diagnosticsWriter);
	}
	else
	{
		CmdLineParamsType params;
		/*
		 *		Get command line arguments
		 */
		getArgs(argc, argv, params);

		std::auto_ptr<TextFileOutputStream>	outputFileStream;
		TextOutputStream*					outputStream = &theStdOut;


		DOMStringPrintWriter pw;

		DOMSupportDefault theDOMSupport;
		XercesParserLiaison xmlParserLiaison(theDOMSupport);

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
		/*
		 * Set specified processor flags
		 */
		if (params.setQuietConflictWarnings)
			processor.setQuietConflictWarnings(true);

		if (! params.paramsMap.empty())	
		{
			String2StringMapType::iterator it = params.paramsMap.begin();
			for ( ; it != params.paramsMap.end(); it++)
				processor.setStylesheetParam((*it).first.c_str(),
						(*it).second.c_str());
		}

		/*
		 * Set specified parser flags
		 */
		xmlParserLiaison.setIndent(params.indentAmount);
		xmlParserLiaison.setSpecialCharacters(params.specialCharacters);
		xmlParserLiaison.SetShouldExpandEntityRefs(params.shouldExpandEntityRefs);


		processor.setFormatter(&xmlParserLiaison);


		// Use separate factory instances for the stylesheet.  This is really only necessary
		// if you want to use the stylesheet with another processor, or you want to use
		// it multiple times.
		XObjectFactoryDefault	theStylesheetXObjectFactory(theXSLProcessorSupport, theXPathSupport);
		XPathFactoryDefault		theStylesheetXPathFactory;

		StylesheetConstructionContextDefault	theConstructionContext(processor,
				theXSLProcessorSupport,
				theStylesheetXObjectFactory,
				theStylesheetXPathFactory);

		XPathExecutionContextDefault			theXPathExecutionContext(theXSLProcessorSupport,
				theXPathSupport,
				theXObjectFactory);

		StylesheetExecutionContextDefault		theExecutionContext(theXPathExecutionContext,
				processor);

		// The main XSL transformation occurs here!
		if (! params.setQuietMode)
			processor.setDiagnosticsOutput( &diagnosticsWriter );


		StylesheetRoot* stylesheet = 0;

		if(0 != params.xslFileName.length())
		{
			stylesheet = processor.processStylesheet(params.xslFileName, theConstructionContext);
		}

		/*
		 * If no output file specified, and multiple input files, generate an
		 * output file based on the root of each input file; otherwise construct
		 * as many unique filenames as required using the original output file
		 * name as a base
		 */
		bool noOutputFileSpecified =  params.outFileName.empty();

		DOMString mimeEncoding("UTF-8");
		DOMString encoding("UTF-8");

		FormatterListener* formatter = 0;

		assert(! params.inFileNames.empty());


		/*
		 * Main loop to process multiple documents with a single stylesheet
		 */
		int nInputFiles = params.inFileNames.size();
		for (int i=0; i< nInputFiles; i++)
		{

			std::string theInputFileName = params.inFileNames[i];
			std::string outputFileName;
			XSLTInputSource theInputSource(theInputFileName.c_str());
			DOM_Node sourceTree = processor.getSourceTreeFromInput(&theInputSource);

			if (noOutputFileSpecified)
			{
				if (nInputFiles > 1)
				{
					outputFileName =
						theInputFileName.substr(0, theInputFileName.find_last_of('.'));
					outputFileName += ".out";
					//	Strip off protocol, if its a file protocol for local machine,
					//	otherwise we're out of luck
					std::string LOCALFILE = "file:///";
					if (0 == outputFileName.find(LOCALFILE))
						outputFileName = outputFileName.substr(LOCALFILE.size());
				}
			}
			else
			{
				outputFileName = params.outFileName;
				if (exists(outputFileName))		// Make sure it's unique
					outputFileName += '0'+i;

			}
			if (! outputFileName.empty())	
			{
				outputFileStream = 
					std::auto_ptr<TextFileOutputStream>(new TextFileOutputStream(
								outputFileName.c_str()));
				outputStream = outputFileStream.get();
			}
			XercesDOMPrintWriter	resultWriter(*outputStream);

			assert(0 != stylesheet);
			if(Formatter::OUTPUT_METH_XML == params.outputType)
			{
				FormatterToXML* fToXML = new FormatterToXML(resultWriter,
						stylesheet->m_version,
						stylesheet->getOutputIndent(),
						xmlParserLiaison.getIndent(),
						mimeEncoding,
						stylesheet->m_mediatype,
						stylesheet->getOutputDoctypeSystem(),
						stylesheet->getOutputDoctypePublic(),
						true,	// xmlDecl
						stylesheet->m_standalone,
						&(stylesheet->getCdataSectionElems()));
				fToXML->m_shouldWriteXMLHeader = shouldWriteXMLHeader;
				fToXML->m_attrSpecialChars = xmlParserLiaison.getSpecialCharacters();
				fToXML->m_stripCData = params.stripCData;
				fToXML->m_escapeCData = params.escapeCData;
				formatter = fToXML;
			}
			else if(Formatter::OUTPUT_METH_TEXT == params.outputType)
			{
				FormatterToText* fToText = new FormatterToText(resultWriter);
				formatter = fToText;
			}
			else if(Formatter::OUTPUT_METH_HTML == params.outputType)
			{
				FormatterToHTML* fToHTML
					= new FormatterToHTML(resultWriter, 
							stylesheet->m_version,
							stylesheet->getOutputIndent(),
							xmlParserLiaison.getIndent(), mimeEncoding,
							stylesheet->m_mediatype,
							stylesheet->getOutputDoctypeSystem(),
							stylesheet->getOutputDoctypePublic(),
							false,	// xmlDecl
							stylesheet->m_standalone,
							&(stylesheet->getCdataSectionElems()));

				fToHTML->m_attrSpecialChars = xmlParserLiaison.getSpecialCharacters();
				fToHTML->m_stripCData = params.stripCData;
				formatter = fToHTML;
			}
			XSLTResultTarget* rTreeTarget = 0;
			if(0 == formatter)
			{
				rTreeTarget = new XSLTResultTarget(&resultWriter);
			}
			else
			{
				rTreeTarget = new XSLTResultTarget();
				rTreeTarget->setFormatterListener(formatter);
				xmlParserLiaison.setFormatterListener(formatter);
			}

			stylesheet->process(sourceTree, *rTreeTarget, theExecutionContext);
			delete rTreeTarget;
		}

		delete formatter;
	}
}	


int main(int argc, const char* argv[]) throw()
{
	try
	{
		xsltMain(argc, argv);
	}

	catch (XSLException& e)
	{
		std::cout << "\nXSLException ";

		std::string type = DOMStringToStdString(e.getType());

		if (!type.empty())
			std::cout << "Type is : " << type << std::endl;

		std::string msg = DOMStringToStdString(e.getMessage());

		if (!msg.empty())
			std::cout << "Message is : " << msg << std::endl;

		return -1;
	}

	catch (SAXException& e)
	{
		std::cout << "\nSAXException ";

		std::string msg = DOMStringToStdString(e.getMessage());

		if (!msg.empty())
			std::cout << "Message is : " << msg << std::endl;

		return -1;
	}



	catch (XMLException& e)
	{
		std::cout << "\nXMLException ";

		std::string type = DOMStringToStdString(e.getType());

		if (!type.empty())
			std::cout << "Type is : " << type << std::endl;

		std::string msg = DOMStringToStdString(e.getMessage());

		if (!msg.empty())
			std::cout << "Message is : " << msg << std::endl;

		return -1;
	}

	return 0;
}
