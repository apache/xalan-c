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

// Base header file.  Must be first.
#include <xalanc/Include/PlatformDefinitions.hpp>



#include <cstdio>

#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#else
#include <iostream>
#endif

// This is here for memory leak testing.
#if !defined(NDEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif


#include <xercesc/util/PlatformUtils.hpp>

#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanFileOutputStream.hpp>
#include <xalanc/PlatformSupport/XalanOutputStreamPrintWriter.hpp>

#include <xalanc/XalanSourceTree/XalanSourceTreeDOMSupport.hpp>
#include <xalanc/XalanSourceTree/XalanSourceTreeParserLiaison.hpp>

#include <xalanc/XPath/XObjectFactoryDefault.hpp>
#include <xalanc/XPath/XPathFactoryDefault.hpp>

#include <xalanc/XSLT/StylesheetConstructionContextDefault.hpp>
#include <xalanc/XSLT/StylesheetExecutionContextDefault.hpp>
#include <xalanc/XSLT/StylesheetRoot.hpp>
#include <xalanc/XSLT/XSLTEngineImpl.hpp>
#include <xalanc/XSLT/XSLTInit.hpp>
#include <xalanc/XSLT/XSLTInputSource.hpp>
#include <xalanc/XSLT/XSLTProcessorEnvSupportDefault.hpp>
#include <xalanc/XSLT/XSLTResultTarget.hpp>  

#include <Harness/XMLFileReporter.hpp>
#include <Harness/FileUtility.hpp>
#include <Harness/HarnessInit.hpp>



XALAN_USING_STD(cerr)
XALAN_USING_STD(cout)
XALAN_USING_STD(endl)



const char* const 	excludeStylesheets[] =
{
	"large-evans_large.xsl",
	0
};



// Just hoist everything...
XALAN_CPP_NAMESPACE_USE



inline bool
checkForExclusion(const XalanDOMString&		currentFile)
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
	const XSLTInputSource	theInputSource(theFileName);

	return theProcessor.processStylesheet(theInputSource, theConstructionContext);
}



inline XalanNode*
parseSourceDocument(
			const XalanDOMString&	theFileName,
			XSLTProcessor&			theProcessor)
{
	const XSLTInputSource	theInputSource(theFileName);

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
transformWUnparsedSource(
			const XalanDOMString&				theFileName,
			XSLTProcessor&						theProcessor,
			const StylesheetRoot*				theStylesheetRoot,
			XSLTResultTarget&					theResults,
			StylesheetExecutionContextDefault&  theExecutionContext)
{
	const XSLTInputSource	csSourceXML(c_wstr(theFileName));

	theProcessor.setStylesheetRoot(theStylesheetRoot);

	const clock_t startTime = clock();

	theProcessor.process(csSourceXML, theResults, theExecutionContext);

	return clock() - startTime;

}



inline clock_t
transformWParsedSource(
			XalanNode*							theParsedSource,
			XSLTProcessor&						theProcessor,
			const StylesheetRoot*				theStylesheetRoot,
			XSLTResultTarget&					theResults,
			StylesheetExecutionContextDefault&	theExecutionContext)
{
	// Put the parsed document into an XSLTInputSource, 
	// and set stylesheet root in the processor
	const XSLTInputSource	csSourceDocument(theParsedSource);

	theProcessor.setStylesheetRoot(theStylesheetRoot);

	const clock_t startTime = clock();

	theProcessor.process(csSourceDocument, theResults, theExecutionContext);
	
	return clock() - startTime;
}



inline long
eTOeTransform(
			const XSLTInputSource&			inputSource,
	        const XSLTInputSource&			stylesheetSource,
	        XSLTResultTarget&				outputTarget,
			StylesheetConstructionContext&	constructionContext,
			StylesheetExecutionContext&		executionContext,
			XSLTProcessor&					theProcessor)
{
	const clock_t startTime=clock();

	theProcessor.process(
				inputSource, 
			    stylesheetSource,
				outputTarget,
				constructionContext,
				executionContext);

	const clock_t endTime=clock();

	return endTime - startTime;
}



void
setHelp(FileUtility&	h)
{
	h.args.getHelpStream() << endl
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
main(
			int		argc,
			char*	argv[])
{
#if !defined(NDEBUG) && defined(_MSC_VER)
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif


	const XalanDOMString	processorType(XALAN_STATIC_UCODE_STRING("XalanC"));
	bool skip = true;		// Default will skip long tests
	bool setGold = false;

	HarnessInit		xmlPlatformUtils;

	FileUtility		h;

	// Set the program help string,  then get the command line parameters.
	//
	setHelp(h);

	if (h.getParams(argc, argv, "PERF-RESULTS", setGold) == true)
	{

		// Generate Unique Run id and processor info
		const XalanDOMString	UniqRunid = h.generateUniqRunid();
		const XalanDOMString	resultFilePrefix(XalanDOMString("cpp"));
		const XalanDOMString	resultsFile(h.args.output + resultFilePrefix + UniqRunid + FileUtility::s_xmlSuffix);

		XMLFileReporter	logFile(resultsFile);

		logFile.logTestFileInit("Performance Testing - Reports performance times for single transform, and average for multiple transforms using compiled stylesheet");

		try
		{
			// Call the static initializers... and define file suffixes
			// Having xmlplatformutils in it's own class like this means that if there are
			// exceptions then terminate() is sure to run because it will automatically get
			// cleaned up when this instance goes out of scope.
			bool foundDir = false;	// Flag indicates directory found. Used in conjunction with -sub cmd-line arg.
			{
				XSLTInit	theInit;  
		
				typedef FileUtility::FileNameVectorType		FileNameVectorType;

				// Get the list of Directories that are below perf and iterate through them
				const FileNameVectorType dirs = h.getDirectoryNames(h.args.base);

				const long 	iterCount = h.args.iters;

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

						typedef XMLFileReporter::Hashtable	Hashtable;

						Hashtable attrs;
					
						if (skip && checkForExclusion(files[i]))
						{
							continue;
						}

						const XalanDOMString  theXSLFile= h.args.base + dirs[j] + FileUtility::s_pathSep + files[i];
						const XalanDOMString  theXMLFile = h.generateFileName(theXSLFile,"xml");

						const XalanDOMString  theOutput =  h.args.output + dirs[j] + FileUtility::s_pathSep + files[i]; 
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
						const XSLTInputSource	xslInputSource(theXSLFile);
						const XSLTInputSource	xmlInputSource(theXMLFile);

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
		}
		catch(const XalanFileOutputStream::XalanFileOutputStreamOpenException& ex)
		{
			cerr << ex.getMessage() << endl << endl;
		}
		catch(...)
		{
			cerr << "Exception caught!!!" << endl  << endl;
		}
	}

	return 0;
}
