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
#include <cassert>
#include <fstream>
#include <iostream>
#include <strstream>

#if !defined(NDEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif

//These came from the debug test.
#include <cstdio>
#include <ctime>
#include <string>
#include <string.h>
#include <vector>

#include <util/PlatformUtils.hpp>

#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/XalanFileOutputStream.hpp>
#include <PlatformSupport/XalanOutputStreamPrintWriter.hpp>

#include <XalanSourceTree/XalanSourceTreeDOMSupport.hpp>
#include <XalanSourceTree/XalanSourceTreeParserLiaison.hpp>

#include <XPath/XObjectFactoryDefault.hpp>
#include <XPath/XPathFactoryDefault.hpp>

#include <XSLT/StylesheetConstructionContextDefault.hpp>
#include <XSLT/StylesheetExecutionContextDefault.hpp>
#include <XSLT/StylesheetRoot.hpp>
#include <XSLT/XSLTEngineImpl.hpp>
#include <XSLT/XSLTInit.hpp>
#include <XSLT/XSLTInputSource.hpp>
#include <XSLT/XSLTProcessorEnvSupportDefault.hpp>
#include <XSLT/XSLTResultTarget.hpp>

#include <XMLFileReporter.hpp>
#include <FileUtility.hpp>


//This is here for the threads.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winbase.h>
#define THREADFUNCTIONRETURN DWORD WINAPI

#if !defined(XALAN_NO_NAMESPACES)
	using std::cerr;
	using std::cout;
	using std::cin;
	using std::endl;
	using std::ifstream;
	using std::ios_base;
	using std::ostrstream;
	using std::string;
#endif


// This is here for memory leak testing.
#if defined(_DEBUG)
#include <crtdbg.h>
#endif


const char* const 	excludeStylesheets[] =
{
	"large-evans_large.xml",
	0
};

inline bool
checkForExclusion(XalanDOMString currentFile)
{

		for (int i=0; excludeStylesheets[i] != 0; i++)
			{	if (equals(currentFile, XalanDOMString(excludeStylesheets[i])))
				return true;
			}
		return false;
}

inline StylesheetRoot*
processStylesheet(
			const XalanDOMString&			theFileName,
			XSLTProcessor&					theProcessor,
			StylesheetConstructionContext&	theConstructionContext)
{
	const XSLTInputSource	theInputSource(c_wstr(theFileName));

	return theProcessor.processStylesheet(theInputSource, theConstructionContext);
}



inline XalanNode*
parseSourceDocument(
			const XalanDOMString&	theFileName,
			XSLTProcessor&			theProcessor)
{
	const XSLTInputSource	theInputSource(c_wstr(theFileName));

	return theProcessor.getSourceTreeFromInput(theInputSource);
}



inline double
calculateElapsedTime(
			clock_t		theStartTime,
			clock_t		theEndTime)
{
	return double(theEndTime - theStartTime) / CLOCKS_PER_SEC * 1000.0;
}

inline double
calculateAverageElapsedTime(
			clock_t			theStartTime,
			clock_t			theEndTime,
			long			theIterationCount)
{
	assert(theIterationCount > 0);

	return calculateElapsedTime(theStartTime, theEndTime) / theIterationCount;
}

inline clock_t
transformWUnparsedSource(const XalanDOMString&	theFileName,
				 XSLTProcessor&			theProcessor,
				 const StylesheetRoot*	theStylesheetRoot,
				 XSLTResultTarget&	theResults,
				 StylesheetExecutionContextDefault&  theExecutionContext)
{
	const XSLTInputSource	csSourceXML(c_wstr(theFileName));	// Creates source document
	theProcessor.setStylesheetRoot(theStylesheetRoot);

	const clock_t startTime = clock();
	theProcessor.process(csSourceXML, theResults, theExecutionContext);
	const clock_t endTime = clock();

	return endTime - startTime;

}

inline clock_t
transformWParsedSource(XalanNode*		theParsedSource,
				 XSLTProcessor&			theProcessor,
				 const StylesheetRoot*	theStylesheetRoot,
				 XSLTResultTarget&		theResults,
				 StylesheetExecutionContextDefault&  theExecutionContext)
{
	// Put the parsed document into an XSLTInputSource, 
	// and set stylesheet root in the processor
	const XSLTInputSource	csSourceDocument(theParsedSource);
	theProcessor.setStylesheetRoot(theStylesheetRoot);

	const clock_t startTime = clock();
	theProcessor.process(csSourceDocument, theResults, theExecutionContext);
	const clock_t endTime = clock();
	
	return endTime - startTime;

}
inline long
eTOeTransform(const XSLTInputSource&		inputSource, 
	        const XSLTInputSource&			stylesheetSource,
	        XSLTResultTarget&				outputTarget,
			StylesheetConstructionContext&	constructionContext,
			StylesheetExecutionContext&		executionContext,
			XSLTProcessor&					theProcessor)
{
	const clock_t startTime=clock();
	theProcessor.process(inputSource, 
			        	stylesheetSource,
				    	outputTarget,
						constructionContext,
						executionContext);
	const clock_t endTime=clock();

	return endTime - startTime;
}

void
getParams(int argc, 
		  const char*	argv[], 
		  long& iterCount, 
		  bool& skip)
{
	if (argc >= 4 )
	{
		cout << "Usage perf {count, -s(kip)} " << endl;
		exit(1);
	}
	if (argc >= 2)
	{
		iterCount = atol(argv[1]);
		if (iterCount <= 0)
		{
			cerr << "Usage: perf <count, -s(kip)>" << endl  << endl;
			exit(1);
		}
		if (argc >= 3 && !stricmp(argv[2], "-s"))
		{
			skip = true;
		}
		return;
	}			
}

#if defined(XALAN_NO_NAMESPACES)
	typedef vector<XalanDOMString>		FileNameVectorType;
#else
	typedef std::vector<XalanDOMString>	FileNameVectorType;
#endif

int
main(
	 int			argc,
	 const char*	argv[])
{

	long iterCount = 5;	// Default number of iterations
	bool skip = false;	// Default will not skip long tests

	// Defined root for performance directory. Based on PD's machine. 
	const XalanDOMString	perfDir(XALAN_STATIC_UCODE_STRING("d:\\xslt\\xsl-test\\perf\\"));


#if !defined(NDEBUG) && defined(_MSC_VER)
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif

	FileUtility f;
	FileNameVectorType dirs, files;

	// Get the list of Directories that are below perf
	dirs = f.getDirectoryNames(perfDir);

	XMLFileReporter	logFile("cpp.xml");
	logFile.logTestFileInit("Performance Testing - Reports performance times for single transform, and average for multiple transforms using compiled stylesheet");

	getParams(argc, argv, iterCount, skip);
	try
	{
		// Call the static initializers... and define file suffixes
		XMLPlatformUtils::Initialize();
		{
			XSLTInit	theInit;
			
			// Define some constants for file suffixes ...
			const XalanDOMString  XSLSuffix(".xsl");
			const XalanDOMString  XMLSuffix(".xml");
			const XalanDOMString  outputDir("\\xslt-results\\perf\\test\\");
			const XalanDOMString  pathSep(XALAN_STATIC_UCODE_STRING("\\"));  
			const XalanDOMString  outputSuffix(".out");

			// Define some variables used for timing...
			clock_t startTime, endTime, accmTime;
			double timeinMilliseconds, theAverage;


			for(FileNameVectorType::size_type	j = 0; j < dirs.size(); j++)
			{
			  files = f.getTestFileNames(perfDir, dirs[j]);
			  for(FileNameVectorType::size_type i = 0; i < files.size(); i++)
			  {
				if (skip)
				{
					if (checkForExclusion(files[i]))
					{
						continue;
					}
				}
				// Output file name to result log.
				logFile.logTestCaseInit(files[i]);

				const XalanDOMString  theXMLFile= perfDir + dirs[j] + pathSep + files[i];
				const XalanDOMString  theXSLFile = f.GenerateFileName(theXMLFile,"xsl");
				const XalanDOMString  theOutputFile = f.GenerateFileName(theXMLFile, "out");


				// Create the necessary support objects to instantiate a processor.
				XalanSourceTreeDOMSupport		csDOMSupport;
				XalanSourceTreeParserLiaison	csParserLiaison(csDOMSupport);

				csDOMSupport.setParserLiaison(&csParserLiaison);

				XSLTProcessorEnvSupportDefault	csXSLTProcessorEnvSupport;
				XObjectFactoryDefault			csXObjectFactory;
				XPathFactoryDefault				csXPathFactory;

				// Create a processor and connect to ProcessorEnvSupport object
				XSLTEngineImpl	csProcessor(
							csParserLiaison,
							csXSLTProcessorEnvSupport,
							csDOMSupport,
							csXObjectFactory,
							csXPathFactory);

				// Hook up the processor the the support object.
				csXSLTProcessorEnvSupport.setProcessor(&csProcessor);

				// Create separate factory support object, so the stylesheet's
				// factory-created XPath instance are independent from processor's.
				XPathFactoryDefault			ssXPathFactory;

				// Create a stylesheet construction context, using the
				// stylesheet's factory support objects.
				StylesheetConstructionContextDefault	csConstructionContext(
														csProcessor,
														csXSLTProcessorEnvSupport,
														ssXPathFactory);
				cout << endl << files[i] << endl;

				// Create a parsed stylesheet (StylesheetRoot) for the
				// specified input XSL. We don't have to delete it, since
				// it is owned by the StylesheetConstructionContextDefault
				// instance. Time it as well...

				startTime = clock();
				const StylesheetRoot* const glbStylesheetRoot = processStylesheet(
																theXSLFile,
																csProcessor,
																csConstructionContext);
				endTime = clock();
				assert(glbStylesheetRoot != 0);

				// Calculate & report performance on stylesheet parse to console and log file.
				timeinMilliseconds = calculateElapsedTime(startTime, endTime);
				cout << "   XSL parse: " << timeinMilliseconds << " milliseconds." << endl;
				logFile.logStatistic(60,
									long(timeinMilliseconds),
									timeinMilliseconds,
									"Parse of XSL: ");

				// Parse the input XML and report how long it took...                             
				startTime = clock();
				XalanNode* const  glbSourceXML = parseSourceDocument(theXMLFile, 
																	csProcessor);
				endTime = clock();

				// Calculate & report performance on source document parse to console and log file.
				timeinMilliseconds = calculateElapsedTime(startTime, endTime);
				cout << "   XML parse: " << timeinMilliseconds << " milliseconds." << endl;
				logFile.logStatistic(60,
									long(timeinMilliseconds),
									timeinMilliseconds,
									"Parse of XML: ");

				assert(glbSourceXML != 0);

				// The execution context uses the same factory support objects as
				// the processor, since those objects have the same lifetime as
				// other objects created as a result of the execution.
				StylesheetExecutionContextDefault	psExecutionContext(
									csProcessor,
									csXSLTProcessorEnvSupport,
									csDOMSupport,
									csXObjectFactory);

			

				// Do a total end to end transform with no pre parsing of either xsl or xml files.
				XSLTResultTarget		theResultTarget(theOutputFile);
				const XSLTInputSource	xslInputSource(c_wstr(theXSLFile));
				const XSLTInputSource	xmlInputSource(c_wstr(theXMLFile));
				const etoetran = eTOeTransform(xmlInputSource, 
												xslInputSource,
												theResultTarget,
												csConstructionContext,
												psExecutionContext,
												csProcessor);

				// Output single transform time to console and result log
				cout << "   eTOe transform: " << etoetran << " milliseconds." << endl;
				logFile.logStatistic(60,
									etoetran,
									etoetran,
									"end To end transform: ");


				// Perform a single transform using compiled stylesheet and report results...
				csProcessor.setStylesheetRoot(glbStylesheetRoot);
				const XSLTInputSource	csSourceDocument(glbSourceXML);

				startTime = clock();
				csProcessor.process(csSourceDocument, 
									theResultTarget, 
									psExecutionContext);
				endTime = clock();

				psExecutionContext.reset();	// Reset the execution context...
				timeinMilliseconds = calculateElapsedTime(startTime, endTime);

				// Output single transform time to console and result log
				cout << "   One transform w/Parsed XSL: " << timeinMilliseconds << " milliseconds." << endl;
				logFile.logStatistic(60,
									long(timeinMilliseconds),
									timeinMilliseconds,
									"Single transform took: ");

				// Perform multiple transforms and calculate the average time ..
				accmTime = 0;
				for(int j = 0; j < iterCount; ++j)
				{	
					accmTime += transformWParsedSource(glbSourceXML,
														 csProcessor,
														 glbStylesheetRoot,
														 theResultTarget,
														 psExecutionContext);
					psExecutionContext.reset();							
				}
				csParserLiaison.reset();
				theAverage = accmTime / iterCount;

				// Output average transform time to console and result log
				cout << "   Avg: " << theAverage << " for " << iterCount << " iter's w/Parsed XML" << endl;

				char tmp[100];
				sprintf(tmp, "%s%d%s","Avg: transform of ", iterCount, " iter's w/Parsed XML.");
				logFile.logStatistic(60, 
									long(theAverage), 
									theAverage, 
									tmp);

				//	This is currently how the XalanJ 2.0 is performing transforms,
				//	i.e. with the unparsed XML Source.
						
				accmTime = 0;
				for(int k = 0; k < iterCount; ++k)
				{
					accmTime += transformWUnparsedSource(theXMLFile,
														 csProcessor,
														 glbStylesheetRoot,
														 theResultTarget,
														 psExecutionContext);
					psExecutionContext.reset();		// Resets the execution context
					csParserLiaison.reset();		// This deletes the document
				}
				theAverage = accmTime / iterCount;
				cout << "   Avg: " << theAverage << " for " << iterCount << " iter's w/Source XML" << endl;

				sprintf(tmp, "%s%d%s","Avg: transform of ", iterCount, " iter's w/Source XML.");
				logFile.logStatistic(60, 
									long(theAverage), 
									theAverage, 
									tmp);

				logFile.logTestCaseClose(files[i], XalanDOMString("Done"));
			  }
			}
		}

		logFile.logTestFileClose("Performance", "Done");
		logFile.close();

		XMLPlatformUtils::Terminate();
	}
	catch(...)
	{
		cerr << "Exception caught!!!" << endl  << endl;
	}


	return 0;
}
