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

#include <iostream>
#include <strstream>
#include <stdio.h>
#include <direct.h>


// This is here for memory leak testing.
#if !defined(NDEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif

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
#include <HarnessInit.hpp>

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


#if defined(XALAN_NO_NAMESPACES)
	typedef map<XalanDOMString, XalanDOMString, less<XalanDOMString> >	Hashtable;
#else
	typedef std::map<XalanDOMString, XalanDOMString>	Hashtable;
#endif


FileUtility h;


const char* const 	excludeStylesheets[] =
{
	"large-evans_large.xsl",
	0
};


inline bool
checkForExclusion(XalanDOMString currentFile)
{

		for (int i=0; excludeStylesheets[i] != 0; i++)
			{	
				if (equals(currentFile, XalanDOMString(excludeStylesheets[i])))
					{
						return true;
					}
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
calculateAvgTime(
			clock_t		accTime,
			long		theIterationCount)
{
	assert(theIterationCount > 0);

	return double(accTime) / theIterationCount;
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
setHelp()
{
	h.args.help << endl
		 << "Perf dir [-out -sub -i -iter]"
		 << endl
		 << endl
		 << "dir		(base directory for testcases)"
		 << endl
		 << "-out dir	(base directory for output)"
		 << endl
		 << "-sub dir (run files only from a specific directory)"
		 << endl
		 << "-i                (include all testcases)"
		 << endl
		 << "-iter n           (specifies number of iterations; must be > 0)"
		 << endl;
}

int
main(int			argc,
	 const char*	argv[])
{
#if !defined(NDEBUG) && defined(_MSC_VER)
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif

	const XalanDOMString	processorType(XALAN_STATIC_UCODE_STRING("XalanC"));
	long iterCount;			// Default number of iterations
	bool skip = true;		// Default will skip long tests
	bool setGold = false;

	// Set the program help string,  then get the command line parameters.
	//
	setHelp();

	if (h.getParams(argc, argv, "PERF-RESULTS", setGold) == true)
	{

		// Generate Unique Run id and processor info
		const XalanDOMString UniqRunid = h.generateUniqRunid();
		const XalanDOMString  resultFilePrefix(XalanDOMString("cpp"));
		const XalanDOMString  resultsFile(h.args.output + resultFilePrefix + UniqRunid + XMLSuffix);

		XMLFileReporter	logFile(resultsFile);
		logFile.logTestFileInit("Performance Testing - Reports performance times for single transform, and average for multiple transforms using compiled stylesheet");

		try
		{
			// Call the static initializers... and define file suffixes
			// Having xmlplatformutils in it's own class like this means that if there are
			// exceptions then terminate() is sure to run because it will automatically get
			// cleaned up when this instance goes out of scope.
			HarnessInit xmlPlatformUtils;
			bool foundDir = false;	// Flag indicates directory found. Used in conjunction with -sub cmd-line arg.
			{
				XSLTInit	theInit;  
		
				// Get the list of Directories that are below perf and iterate through them
				const FileNameVectorType dirs = h.getDirectoryNames(h.args.base);

				for(FileNameVectorType::size_type	j = 0; j < dirs.size(); j++)
				{
					// Run specific sub of files from given directory
					if (length(h.args.sub) > 0 && !equals(dirs[j], h.args.sub))
					{
						continue;
					}

					// Check that output directory is there.
					const XalanDOMString  theOutputDir = h.args.output + dirs[j];
					h.checkAndCreateDir(theOutputDir);

					logFile.logTestCaseInit(XalanDOMString("Performance Directory: ") + dirs[j] );
					
					// Indicate that directory was processed and get test files from the directory
					foundDir = true;
					const FileNameVectorType files = h.getTestFileNames(h.args.base, dirs[j], false);

					for(FileNameVectorType::size_type i = 0; i < files.size(); i++)
					{
						// Define  variables used for timing and reporting ...
						clock_t startTime, endTime, accmTime, avgEtoe;
						double timeinMilliseconds, theAverage;
						Hashtable attrs;
					
						if (skip)
						{
							if (checkForExclusion(files[i]))
								continue;
						}

						const XalanDOMString  theXSLFile= h.args.base + dirs[j] + pathSep + files[i];
						const XalanDOMString  theXMLFile = h.generateFileName(theXSLFile,"xml");

						const XalanDOMString  theOutput =  h.args.output + dirs[j] + pathSep + files[i]; 
						const XalanDOMString  theOutputFile = h.generateFileName(theOutput, "out");


						attrs.insert(Hashtable::value_type(XalanDOMString("href"), theXSLFile));
						// Create the necessary support objects to instantiate a processor.

						XercesDOMSupport				csDOMSupport;
						XercesParserLiaison				csParserLiaison(csDOMSupport);

						/*XalanSourceTreeDOMSupport		csDOMSupport;
						XalanSourceTreeParserLiaison	csParserLiaison(csDOMSupport);

						csDOMSupport.setParserLiaison(&csParserLiaison);
						*/

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
						cout << "   XSL: " << timeinMilliseconds << " milliseconds, Parse" << endl;
						logFile.addMetricToAttrs("parsexsl",timeinMilliseconds, attrs);						

						
						// Parse the input XML and report how long it took...                             
						startTime = clock();
						XalanNode* const  glbSourceXML = parseSourceDocument(theXMLFile, 
																	csProcessor);
						endTime = clock();
						assert(glbSourceXML != 0);

						// Calculate & report performance on source document parse to console and log file.
						timeinMilliseconds = calculateElapsedTime(startTime, endTime);
						cout << "   XML: " << timeinMilliseconds << " milliseconds, Parse" << endl;
						logFile.addMetricToAttrs("parsexml",timeinMilliseconds, attrs);



						// The execution context uses the same factory support objects as
						// the processor, since those objects have the same lifetime as
						// other objects created as a result of the execution.

						XSLTResultTarget		theResultTarget(theOutputFile);
						const XSLTInputSource	xslInputSource(c_wstr(theXSLFile));
						const XSLTInputSource	xmlInputSource(c_wstr(theXMLFile));

						StylesheetExecutionContextDefault	psExecutionContext(
											csProcessor,
											csXSLTProcessorEnvSupport,
											csDOMSupport,
											csXObjectFactory);

						// Perform a single transform using parsed stylesheet and unparsed xml source, report results...
						csProcessor.setStylesheetRoot(glbStylesheetRoot);
						//const XSLTInputSource	csSourceDocument(glbSourceXML);

						startTime = clock();
						csProcessor.process(xmlInputSource, 
									theResultTarget, 
									psExecutionContext);
						endTime = clock();

						psExecutionContext.reset();	// Reset the execution context...
						timeinMilliseconds = calculateElapsedTime(startTime, endTime);

						// Output single transform time to console and result log
						cout << endl << "   One: " << timeinMilliseconds << " w/Parsed XSL" << endl;
						logFile.addMetricToAttrs("single",timeinMilliseconds, attrs);


						// Do a total end to end transform with no pre parsing of either xsl or xml files.
						const etoetran = eTOeTransform(xmlInputSource, 
													xslInputSource,
													theResultTarget,
													csConstructionContext,
													psExecutionContext,
													csProcessor);
	
						// Output single etoe transform time to console and result log
						cout << "   One: " << etoetran << " eToe" << endl;
						logFile.addMetricToAttrs("etoe", etoetran, attrs);


						// Perform multiple transforms and calculate the average time ..
						// These are done 3 different ways.
						// FIRST: Parsed XSL Stylesheet and Parsed XML Source.

						iterCount = h.args.iters;
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
						theAverage = calculateAvgTime(accmTime, iterCount); 

						// Output average transform time to console and result log
						cout << endl  << "   Avg: " << theAverage << " for " << iterCount << " iter's w/Parsed files" << endl;
						logFile.addMetricToAttrs("avgparsedxml",theAverage, attrs);

						// SECOND: Parsed Stylesheet and UnParsed XML Source.
						// This is currently how the XalanJ 2.0 is performing transforms,
						// i.e. with the unparsed XML Source.
						
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

						theAverage = calculateAvgTime(accmTime, iterCount);
						cout << "   Avg: " << theAverage << " for " << iterCount << " iter's w/UnParsed XML" << endl;

						logFile.addMetricToAttrs("avgunparsedxml",theAverage, attrs);

						// THIRD: Neither Stylesheet nor XML Source are parsed.
						// Perform multiple etoe transforms and calculate the average ...
		
						avgEtoe = 0;
						for(int jj = 0; jj < iterCount; ++jj)
						{	
						avgEtoe += eTOeTransform(xmlInputSource, 
													xslInputSource,
													theResultTarget,
													csConstructionContext,
													psExecutionContext,
													csProcessor);
						psExecutionContext.reset();	
						csParserLiaison.reset();						
						}

						theAverage = calculateAvgTime(avgEtoe,iterCount);

						// Output average transform time to console and result log
						cout << "   Avg: " << theAverage << " for " << iterCount << " iter's of eToe" << endl;
						logFile.addMetricToAttrs("avgetoe",theAverage, attrs);
						logFile.logElementWAttrs(10, "perf", attrs, "xxx");
					}

					logFile.logTestCaseClose(XalanDOMString("Performance Directory: ") + dirs[j], XalanDOMString("Done") );
				}

			}

		// Check to see if -sub cmd-line directory was processed correctly.
		if (!foundDir)
		{
			cout << "Specified test directory: \"" << c_str(TranscodeToLocalCodePage(h.args.sub)) << "\" not found" << endl;
		}

		h.reportPassFail(logFile, UniqRunid);
		logFile.logTestFileClose("Performance", "Done");
		logFile.close();

		}//try

		catch(const XalanFileOutputStream::XalanFileOutputStreamOpenException& ex)
		{
			cerr << ex.getMessage() << endl << endl;
		}

		catch(...)
		{
			cerr << "Exception caught!!!" << endl  << endl;
		}
		

	} //if getParams

	return 0;
}
