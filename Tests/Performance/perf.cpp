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
//#include <stdlib.h>


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


// This is here for memory leak testing.
#if defined(_DEBUG)
#include <crtdbg.h>
#endif


#if defined(XALAN_NO_NAMESPACES)
	typedef vector<XalanDOMString>		FileNameVectorType;
#else
	typedef std::vector<XalanDOMString>	FileNameVectorType;
#endif


#if defined(XALAN_NO_NAMESPACES)
	typedef map<XalanDOMString, XalanDOMString, less<XalanDOMString> >	Hashtable;
#else
	typedef std::map<XalanDOMString, XalanDOMString>	Hashtable;
#endif

const char* const 	excludeStylesheets[] =
{
	//"basic-all_well.xsl",
	"large-evans_large.xsl",
	"thruput-flat.xsl",
	//"sort-cem-big.xsl",
	//"large-cem10k.xsl",
	0
};
const XalanDOMString	pathSep(XalanDOMString("\\"));


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
addMetricToAttrs(char* desc, double theMetric, Hashtable& attrs)
{
	XalanDOMString	temp;

	DoubleToDOMString(theMetric, temp);
	attrs.insert(Hashtable::value_type(XalanDOMString(desc), temp));

	return;
}

void
printArgOptions()
{
	cerr << endl
		 << "Perf dirname [-out -category -i -iter]"
		 << endl
		 << endl
		 << "dirname		(base directory for testcases)"
		 << endl
		 << "-out dirname	(base directory for output)"
		 << endl
		 << "-category dirname (run files only from a specific directory)"
		 << endl
		 << "-i                (include all testcases)"
		 << endl
		 << "-iter n           (specifies number of iterations; must be > 0)"
		 << endl;
}

void
checkAndCreateDir(XalanDOMString directory )
{
char buffer[_MAX_PATH]; // buffer2[_MAX_PATH], buffer3[_MAX_PATH];
const char* buf;

	_getcwd( buffer, _MAX_PATH );


	if ( (_chdir(c_str(TranscodeToLocalCodePage(directory)))) )
	{
		//cout << "Couldn't change to " << directory << ", will create it." << endl;
		if ( !(_mkdir(c_str(TranscodeToLocalCodePage(directory)))))
		{
			cout << directory << " created." << endl;
		}
	}

	_chdir(buffer);
}


bool
getParams(int argc, 
		  const char*	argv[],
		  XalanDOMString& basedir,
		  XalanDOMString& outdir,
		  XalanDOMString& category,
		  bool& skip,
		  long& iterCount)
{
bool fSuccess = true;	// Used to continue argument loop
bool fSetOut = true;	// Set default output directory


	// Insure that required "-base" argument is there.
	if (argc == 1 || argv[1][0] == '-')
	{
		printArgOptions(); 
		return false;
	}
	else
	{
		assign(basedir, XalanDOMString(argv[1]));
		insert(basedir, 0, pathSep);
	}

	// Get the rest of the arguments in any order.
	for (int i = 2; i < argc && fSuccess == true; ++i)
	{
		if(!stricmp("-out", argv[i]))
		{
			++i;
			if(i < argc && argv[i][0] != '-')
			{
				assign(outdir, XalanDOMString(argv[i]));
				insert(outdir, 0, XalanDOMString("\\"));
				append(outdir, XalanDOMString("\\"));
				checkAndCreateDir(outdir);
				fSetOut = false;
			}
			else
			{
				printArgOptions();
				fSuccess = false;
			}
		}
		else if(!stricmp("-category", argv[i]))
		{
			++i;
			if(i < argc && argv[i][0] != '-')
			{
				assign(category, XalanDOMString(argv[i]));
			}
			else
			{
				printArgOptions();
				fSuccess = false;
			}
		}
		else if(!stricmp("-i", argv[i]))
		{
			skip = false;
		}
		else if(!stricmp("-iter", argv[i]))
		{
			++i;
			
			// Make sure number is there and is greater then zero
			if(i < argc && atol(argv[i]) > 0)
			{
				iterCount = atol(argv[i]);
			}
			else
			{
				printArgOptions();
				fSuccess = false;
			}
		}
		else
		{
			printArgOptions();
			fSuccess = false;
		}

	} // End of for-loop

	// Do we need to set the default output directory??
	if (fSetOut)
	{
		unsigned int ii = lastIndexOf(basedir,charAt(pathSep,0));
		outdir = substring(basedir, 0, ii+1);
		append(outdir,XalanDOMString("PERF-RESULTS\\"));
		checkAndCreateDir(outdir);
	}
	
	// Add the path seperator to the end of the base directory
	append(basedir,pathSep);
	return fSuccess;
}


int
main(
	 int			argc,
	 const char*	argv[])
{
#if !defined(NDEBUG) && defined(_MSC_VER)
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif

	Hashtable runAttrs;
	long iterCount = 5;	// Default number of iterations
	bool skip = true;	// Default will skip long tests

	XalanDOMString  category;	// Test all of base dir by default
	XalanDOMString  baseDir;	//(XALAN_STATIC_UCODE_STRING("\\xslt\\xsl-test\\perf\\"));
	XalanDOMString  outputRoot;	//(XALAN_STATIC_UCODE_STRING(""));
	XalanDOMString  resultsRoot(XALAN_STATIC_UCODE_STRING("\\xslt\\xsl-test\\perf-dataxml\\"));
	//const XalanDOMString  pathSep(XALAN_STATIC_UCODE_STRING("\\"));

	if (getParams(argc, argv, baseDir, outputRoot, category, skip, iterCount) == true)
	{

		FileUtility f;

		// Generate Unique Run id and processor info
		const XalanDOMString UniqRunid = f.GenerateUniqRunid();

		const XalanDOMString processorType(XALAN_STATIC_UCODE_STRING("XalanC"));

		// Defined basic constants for file manipulation 

		const XalanDOMString  XSLSuffix(XALAN_STATIC_UCODE_STRING(".xsl"));
		const XalanDOMString  XMLSuffix(XALAN_STATIC_UCODE_STRING(".xml"));
		const XalanDOMString  resultFilePrefix(XalanDOMString("cpp"));
		const XalanDOMString  resultsFile(resultsRoot + resultFilePrefix + UniqRunid + XMLSuffix);



		// Get the list of Directories that are below perf
		const FileNameVectorType dirs = f.getDirectoryNames(baseDir);

		XMLFileReporter	logFile(resultsFile);
		logFile.logTestFileInit("Performance Testing - Reports performance times for single transform, and average for multiple transforms using compiled stylesheet");

		// Create initial entry in results file that has info somewhat equivlent to what XalanJ
		// reports in the hashtable entries.
		runAttrs.insert(Hashtable::value_type(XalanDOMString("UniqRunid"), UniqRunid));
		addMetricToAttrs("Iterations",iterCount, runAttrs);
		logFile.logElement(10, "perfdata", runAttrs, "xxx");

		try
		{
			// Call the static initializers... and define file suffixes
			// Having xmlplatformutils in it's own class like this means that if there are
			// exceptions then terminate() is sure to run because it will automatically get
			// cleaned up when this instance goes out of scope.
			HarnessInit xmlPlatformUtils;

			{
				XSLTInit	theInit;  

				for(FileNameVectorType::size_type	j = 0; j < dirs.size(); j++)
				{
					// Run specific category of files from given directory
					if (length(category) > 0 && !equals(dirs[j], category))
					{
						continue;
					}

					cout << "Processing files in Directory: " << dirs[j] << endl;

					// Check that output directory is there.
					const XalanDOMString  theOutputDir = outputRoot + dirs[j];
					checkAndCreateDir(theOutputDir);

					logFile.logTestCaseInit(XalanDOMString("Performance Directory: ") + dirs[j] ); 
					const FileNameVectorType files = f.getTestFileNames(baseDir, dirs[j], false);
					for(FileNameVectorType::size_type i = 0; i < files.size(); i++)
					{
						// Define  variables used for timing and reporting ...
						clock_t startTime, endTime, accmTime, avgEtoe;
						double timeinMilliseconds, theAverage;
						Hashtable attrs;

						attrs.insert(Hashtable::value_type(XalanDOMString("idref"), files[i]));
						attrs.insert(Hashtable::value_type(XalanDOMString("UniqRunid"),UniqRunid));
						attrs.insert(Hashtable::value_type(XalanDOMString("processor"),processorType));
						
						if (skip)
						{
							if (checkForExclusion(files[i]))
								continue;
						}

						const XalanDOMString  theXSLFile= baseDir + dirs[j] + pathSep + files[i];
						const XalanDOMString  theXMLFile = f.GenerateFileName(theXSLFile,"xml");

						const XalanDOMString  theOutput =  outputRoot + dirs[j] + pathSep + files[i]; 
						const XalanDOMString  theOutputFile = f.GenerateFileName(theOutput, "out");


						attrs.insert(Hashtable::value_type(XalanDOMString("href"), theXSLFile));
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
						addMetricToAttrs("parsexsl",timeinMilliseconds, attrs);						

						
						// Parse the input XML and report how long it took...                             
						startTime = clock();
						XalanNode* const  glbSourceXML = parseSourceDocument(theXMLFile, 
																	csProcessor);
						endTime = clock();
						assert(glbSourceXML != 0);

						// Calculate & report performance on source document parse to console and log file.
						timeinMilliseconds = calculateElapsedTime(startTime, endTime);
						cout << "   XML parse: " << timeinMilliseconds << " milliseconds." << endl;
						addMetricToAttrs("parsexml",timeinMilliseconds, attrs);



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
	
						// Output single etoe transform time to console and result log
						cout << "   Single eTOe: " << etoetran << " milliseconds." << endl;
						addMetricToAttrs("etoe", etoetran, attrs);


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
						cout << "   One transform w/Parsed XSL: " << timeinMilliseconds << " milliseconds." << endl;
						addMetricToAttrs("single",timeinMilliseconds, attrs);



						// Perform multiple transforms and calculate the average time ..
						// These are done 3 different ways.
						// FIRST: Parsed XSL Stylesheet and Parsed XML Source.

						// addMetricToAttrs("Iterations",iterCount, attrs);

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
						cout << "   Avg: " << theAverage << " for " << iterCount << " iter's w/Parsed XML" << endl;

						addMetricToAttrs("avgparsedxml",theAverage, attrs);

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

						addMetricToAttrs("avgunparsedxml",theAverage, attrs);

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

						addMetricToAttrs("avgetoe",theAverage, attrs);


						logFile.logElement(10, "perf", attrs, "xxx");
					}//for files

					logFile.logTestCaseClose(XalanDOMString("Performance Directory: ") + dirs[j], XalanDOMString("Done") );
				}//for dirs

			}//xsltinit

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
		


		//XMLPlatformUtils::Terminate();

	} //if getParams

	return 0;
}
