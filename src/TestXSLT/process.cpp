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

#if defined(WIN32)
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#	define THREADFUNCTIONRETURN DWORD WINAPI
#else

#	define THREADFUNCTIONRETURN void *

#	if defined(__GNUC__)
#		include <pthread.h>
#	endif

#endif

#include <cstdio>
#include <cstring>
#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>

#include <string>
#include <strstream>

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
	writer.println("	 [-INDENT n(Control how many spaces to indent {default is 0})]");
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
	writer.println("	 [-MT n(Set number of threads to 'n' {default is 1})]");
}

typedef std::map<std::string, std::string> String2StringMapType;

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
	int indentAmount;
	int nThreads;
	int outputType;
	std::string dumpFileName;
	std::string  outFileName;
	std::string specialCharacters;
	std::string treedumpFileName;
	std::string xslFileName;
	std::vector <std::string> inFileNames;
	CmdLineParams() :
		paramsMap(), doStackDumpOnError(false), escapeCData(false),
		formatOutput(false), setQuietConflictWarnings(false), setQuietMode(false),
		shouldExpandEntityRefs(false), stripCData(false), indentAmount(0),
		nThreads(1), outputType(-1), dumpFileName(),
		specialCharacters(), treedumpFileName(),
		outFileName(), xslFileName(), inFileNames() { }
private:	
	CmdLineParams(const CmdLineParams& other);
};


void getArgs(int argc, const char* argv[], CmdLineParams& p) throw()
{
	p.outputType = -1;
	p.nThreads = 1;
	p.indentAmount = 0;
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
		else if (!stricmp("-MT", argv[i])) 
		{
			if(((i+1) < argc) && (argv[i+1][0] != '-'))
				p.nThreads = atoi( argv[++i] );
		} 
		else if (!stricmp("-IN", argv[i])) 
		{
			p.inFileNames.push_back(argv[++i]);
			while ((i+1) < argc && ! (*argv[i+1] == '-'))	// Multiple entries
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

/*
 * Support for multiple threads
 */

THREADFUNCTIONRETURN xsltMain(void *vptr) throw(XMLException);

#if defined(WIN32)

void xsltMultiThreadedMain(CmdLineParams& params) throw(XMLException)
{
	DWORD dwStackSize = 4096;              	// initial thread stack size
	LPTHREAD_START_ROUTINE lpStartAddress = (LPTHREAD_START_ROUTINE)xsltMain;
	DWORD dwCreationFlags = 0;             	// creation flags
 	int nThreads = params.nThreads;
	DWORD *ThreadIds = new DWORD[nThreads];   // array to receive thread IDs
	HANDLE *hThreads = new HANDLE[nThreads];	// array to receive thread handles
 	int i=0;

 	for (i=0; i< nThreads; i++)
	{
		HANDLE hThread;

		hThread = CreateThread(
				0, dwStackSize,
				lpStartAddress,     // pointer to thread function
				(LPVOID)&params,    // argument for new thread
				dwCreationFlags,    // creation flags
				&ThreadIds[i]);
		if (! hThread)
		{
			LPTSTR lpBuffer;    // pointer to message buffer

			FormatMessage(
					FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
					LPCVOID (0),   // pointer to  message source
					GetLastError(),
					DWORD (0), // language identifier for requested message
					(LPSTR)&lpBuffer,    // pointer to message buffer
					DWORD (0),        // maximum size of message buffer
					0  // pointer to array of message inserts
									 );
			MessageBox(0, lpBuffer, "Error", MB_ICONSTOP);
		}
		else 
			hThreads[i] = hThread;
	}
 	for (i=0; i< nThreads; i++)
	{
		DWORD exitCode = STILL_ACTIVE;
		while (exitCode == STILL_ACTIVE)
			GetExitCodeThread(hThreads[i], &exitCode);
		std::cout << "Process " << hThreads[i] << " finished with exit code " <<
		exitCode << std::endl;
	}
 	for (i=0; i< nThreads; i++)
		CloseHandle(hThreads[i]);
	delete []ThreadIds;
	delete []hThreads;
}

#else		

#if defined(__GNUC__)

void xsltMultiThreadedMain(CmdLineParams& params) throw(XMLException)
{
 	int nThreads = params.nThreads;
	pthread_t *threads = new pthread_t[nThreads];   // array to receive thread IDs
 	for (int i=0; i< nThreads; i++)
	{
		pthread_t  thread;
		int retVal = pthread_create(&thread, 0,
		       (void * (*)(void *))xsltMain, (void *) &params);
		if (retVal)
			std::cout << "Thread creation failed." << std::endl;
		else	
		{
			std::cout << "Created thread: " << thread << std::endl;
			threads[i] = thread;
		}
	}
	// Wait for them to finish
 	for (int i=0; i< nThreads; i++)
		pthread_join(threads[i], 0);
}

#else

void xsltMultiThreadedMain(CmdLineParams& params) throw(XMLException)
{
	std::cout << "xsltMultiThreadedMain: Not valid on this platform" << std::endl;
}

#endif

#endif



static inline bool exists(std::string &filename)
{
	struct stat statBuffer;
	return (0 == stat(filename.data(), &statBuffer));
}

static	CmdLineParams theParams;
	
THREADFUNCTIONRETURN xsltMain(void *vptr) throw(XMLException)
{
 	const CmdLineParams& params = *((CmdLineParams *)vptr);
   const std::string outputFileNameBase = theParams.outFileName;

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

	std::auto_ptr<TextFileOutputStream>	outputFileStream;
	TextOutputStream*					outputStream = &theStdOut;

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

	/*
	 * Set specified processor flags
	 */
	if (params.setQuietConflictWarnings)
		processor.setQuietConflictWarnings(true);

	if (params.paramsMap.size())	
	{
		String2StringMapType::const_iterator it = params.paramsMap.begin();
		for ( ; it != params.paramsMap.end(); it++)
			processor.setStylesheetParam((*it).first.data(),
					(*it).second.data());
	}

	/*
	 * Set specified parser flags
	 */
	if (params.indentAmount)
		xmlParserLiaison.setIndent(params.indentAmount);
	xmlParserLiaison.setSpecialCharacters(params.specialCharacters.data());
	xmlParserLiaison.SetShouldExpandEntityRefs(params.shouldExpandEntityRefs);

	bool noOutputFileSpecified =  outputFileNameBase.empty();
	assert(! params.inFileNames.empty());
	int nInputFiles = params.inFileNames.size();
 	int nThreads = params.nThreads;
	int outputType = params.outputType;
	bool stripCData = params.stripCData;
	bool escapeCData = params.escapeCData;

	// The main XSL transformation occurs here!
	if (! params.setQuietMode)
		processor.setDiagnosticsOutput( &diagnosticsWriter );


	StylesheetRoot* stylesheet = 0;

	DOMString xslFileName;
	if(0 != params.xslFileName.size())
	{
		xslFileName = params.xslFileName.data();
	}

	stylesheet = processor.processStylesheet(xslFileName, theConstructionContext);

	DOMString mimeEncoding("UTF-8");
	DOMString encoding("UTF-8");

	FormatterListener* formatter = 0;


	/*
	 * Main loop to process multiple documents with a single stylesheet
	 */
	for (int i=0; i< nInputFiles; i++)
	{

		std::string theInputFileName = params.inFileNames[i];
		std::string outputFileName;
		XSLTInputSource theInputSource(theInputFileName.data());
		DOM_Node sourceTree = processor.getSourceTreeFromInput(&theInputSource);

	/*
	 * If no output file specified, and multiple input files, generate an
	 * output file based on the root of each input file; otherwise construct
	 * as many unique filenames as required using the original output file
	 * name as a base.
	 */
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
			outputFileName = outputFileNameBase;
			if (nInputFiles > 1)
			{
				int ix = 0;
				while (exists(outputFileName))		// Make sure it's unique
				{
					char buffer[16];
               sprintf(buffer, "%d", ix++);
					outputFileName = outputFileNameBase;
               outputFileName += buffer;
				}
			}
		}
	/*
	 * If multithreaded option specified, append the thread id
	 * to the output file name
	 */
		if (nThreads > 1)
		{
			long pid = 
#if defined(WIN32)
				GetCurrentThreadId();
#else
#if defined(__GNUC__)
			pthread_self();
#else
			0;
#endif				
#endif				
			char buffer[16];
         sprintf(buffer, ".%d", pid);
         outputFileName += buffer;
		}
		if (! outputFileName.empty())	
		{
			outputFileStream = 
				std::auto_ptr<TextFileOutputStream>(new TextFileOutputStream(
							outputFileName.data()));
			outputStream = outputFileStream.get();
		}
		XercesDOMPrintWriter	resultWriter(*outputStream);


		assert(0 != stylesheet);

		if(Formatter::OUTPUT_METH_XML == outputType)
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
			fToXML->m_stripCData = stripCData;
			fToXML->m_escapeCData = escapeCData;
			formatter = fToXML;
		}
		else if(Formatter::OUTPUT_METH_TEXT == outputType)
		{
			FormatterToText* fToText = new FormatterToText(resultWriter);
			formatter = fToText;
		}
		else if(Formatter::OUTPUT_METH_HTML == outputType)
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
			fToHTML->m_stripCData = stripCData;
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
		delete formatter;
		delete rTreeTarget;
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

	/*
	 *		Get command line arguments
	 */
	getArgs(argc, argv, theParams);
	
	try
	{
		if (theParams.nThreads > 1)
			xsltMultiThreadedMain(theParams);
		else		
			xsltMain(&theParams);
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

