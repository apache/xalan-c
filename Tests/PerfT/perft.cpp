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

// XALAN HEADERS...
#include <util/PlatformUtils.hpp>
#include <PlatformSupport/DOMStringHelper.hpp>
#include <XalanTransformer/XalanTransformer.hpp>

// HARNESS HEADERS...
#include <XMLFileReporter.hpp>
#include <FileUtility.hpp>
#include <HarnessInit.hpp>

#if defined(XALAN_NO_NAMESPACES)
	typedef map<XalanDOMString, XalanDOMString, less<XalanDOMString> >	Hashtable;
#else
	typedef std::map<XalanDOMString, XalanDOMString>	Hashtable;
#endif

// This is here for memory leak testing.
#if !defined(NDEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif


const char* const 	excludeStylesheets[] =
{
	//"basic-all_well.xsl",
	"large-evans_large.xsl",
	//"sort-cem-big.xsl",
	//"large-cem10k.xsl",
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

bool
getParams(int argc, 
		  const char*	argv[],
		  FileUtility& f,
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
		if (f.checkDir(pathSep + XalanDOMString(argv[1])))
		{
			assign(basedir, XalanDOMString(argv[1]));
			insert(basedir, 0, pathSep);
		}
		else
		{
			cout << endl << "Given base directory \"" << argv[1] << "\" does not exist" << endl;
			printArgOptions();
			return false;
		}
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
				f.checkAndCreateDir(outdir);
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
		append(outdir,XalanDOMString("PERFT-RESULTS\\"));
		f.checkAndCreateDir(outdir);
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

	Hashtable runAttrs;		// Attribute list for perfdata element
	long iterCount = 5;		// Default number of iterations
	bool skip = true;		// Default will skip long tests

	XalanDOMString  category;	// Test all of base dir by default
	XalanDOMString  baseDir;	
	XalanDOMString  outputRoot;	

	FileUtility futil;

	if (getParams(argc, argv, futil, baseDir, outputRoot, category, skip, iterCount) == true)
	{
		//
		// Call the static initializers for xerces and xalan, and create a transformer
		//
		HarnessInit xmlPlatformUtils;
		XalanTransformer::initialize();
		XalanTransformer xalan;

		// Generate Unique Run id and processor info
		const XalanDOMString UniqRunid = futil.GenerateUniqRunid();


		// Defined basic constants for file manipulation and open results file
		const XalanDOMString  resultFilePrefix(XalanDOMString("cpp"));
		const XalanDOMString  resultsFile(outputRoot + resultFilePrefix + UniqRunid + XMLSuffix);


		XMLFileReporter	logFile(resultsFile);
		logFile.logTestFileInit("Performance Testing - Reports various performance metrics using the Transformer");


		// Create run entry that contains runid and number of iterations used for averages.
		runAttrs.insert(Hashtable::value_type(XalanDOMString("UniqRunid"), UniqRunid));
		logFile.addMetricToAttrs("Iterations",iterCount, runAttrs);
		logFile.logElement(10, "perfdata", runAttrs, "xxx");


		// Get the list of sub-directories below "base" and iterate through them
		const FileNameVectorType dirs = futil.getDirectoryNames(baseDir);

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
			futil.checkAndCreateDir(theOutputDir);

			logFile.logTestCaseInit(XalanDOMString("Performance Directory: ") + dirs[j] ); 
			const FileNameVectorType files = futil.getTestFileNames(baseDir, dirs[j], false);

			for(FileNameVectorType::size_type i = 0; i < files.size(); i++)
			{
				// Define  variables used for timing and reporting ...
				clock_t startTime, endTime, accmTime, avgEtoe;
				double timeinMilliseconds, theAverage;
				int transformResult;
				Hashtable attrs;

				attrs.insert(Hashtable::value_type(XalanDOMString("idref"), files[i]));
				attrs.insert(Hashtable::value_type(XalanDOMString("UniqRunid"),UniqRunid));
				attrs.insert(Hashtable::value_type(XalanDOMString("processor"),processorType));
				logFile.addMetricToAttrs("Iterations",iterCount, attrs);
						
				if (skip)
				{
					if (checkForExclusion(files[i]))
						continue;
				}

				const XalanDOMString  theXSLFile= baseDir + dirs[j] + pathSep + files[i];
				const XalanDOMString  theXMLFile = futil.GenerateFileName(theXSLFile,"xml");

				const XalanDOMString  outbase =  outputRoot + dirs[j] + pathSep + files[i]; 
				const XalanDOMString  theOutputFile = futil.GenerateFileName(outbase, "out");

				const XSLTInputSource	xslInputSource(c_wstr(theXSLFile));
				const XSLTInputSource	xmlInputSource(c_wstr(theXMLFile));
				const XSLTResultTarget	theResultTarget(theOutputFile);

				attrs.insert(Hashtable::value_type(XalanDOMString("href"), theXSLFile));

				cout << endl << files[i] << endl;

				//
				// Time the parsing(compile) of the stylesheet and report the results..
				//
				startTime = clock();
					const XalanCompiledStylesheet* const compiledSS = xalan.compileStylesheet(xslInputSource);
				endTime = clock();
				//	assert(glbStylesheetRoot != 0);

				timeinMilliseconds = calculateElapsedTime(startTime, endTime);
				cout << "   XSL parse: " << timeinMilliseconds << " milliseconds." << endl;
				logFile.addMetricToAttrs("parsexsl",timeinMilliseconds, attrs);	

				//
				// Time the parsing of the input XML and report the results..
				//
				startTime = clock();
					 XalanParsedSource*  parsedSource = xalan.parseSource(xslInputSource);
				endTime = clock();
				//	assert(glbSourceXML != 0);

				timeinMilliseconds = calculateElapsedTime(startTime, endTime);
				cout << "   XML parse: " << timeinMilliseconds << " milliseconds." << endl;
				logFile.addMetricToAttrs("parsexml",timeinMilliseconds, attrs);
/*
				//
				// Do a total END to END transform with no pre parsing of either xsl or xml files.
				// And output metrics to console and result log
				//
				startTime = clock();
					transformResult = xalan.transform(xmlInputSource, xslInputSource, theResultTarget);
				endTime = clock();
				if(!transformResult)
				{
					timeinMilliseconds = calculateElapsedTime(startTime, endTime);
					cout << "   Single eTOe: " << timeinMilliseconds << " milliseconds." << endl;
					logFile.addMetricToAttrs("etoe", timeinMilliseconds, attrs);	
				}
				else
				{
					cout << xalan.getLastError();
					return 0;
				}

				//
				// Perform a single transform using parsed stylesheet and unparsed xml source, report results...
				// 
				startTime = clock();
					transformResult = xalan.transform(xmlInputSource, compiledSS, theResultTarget);
				endTime = clock();
				if(!transformResult)
				{
					timeinMilliseconds = calculateElapsedTime(startTime, endTime);
					cout << "   Single w/parsed XSL: " << timeinMilliseconds << " milliseconds." << endl;
					logFile.addMetricToAttrs("single", timeinMilliseconds, attrs);	
				}
				else
				{
					cout << xalan.getLastError();
					return 0;
				}

*/				//
				// Perform multiple transforms and calculate the average time ..
				// These are done 3 different ways.
				//
				// FIRST: Parsed XSL Stylesheet and Parsed XML Source.
				//
				accmTime = 0;
				for(int j = 0; j < iterCount; ++j)
				{	
					startTime = clock();
						transformResult = xalan.transform(*parsedSource, compiledSS, theResultTarget);
					endTime = clock();
			
					accmTime += endTime - startTime;
				}

				theAverage = calculateAvgTime(accmTime, iterCount); 

				// Output average transform time to console and result log
				cout << "   Avg: " << theAverage << " for " << iterCount << " iter's w/Parsed files" << endl;
				logFile.addMetricToAttrs("avgparsedxml",theAverage, attrs);

				// SECOND: Parsed Stylesheet and UnParsed XML Source.
				// This is currently how the XalanJ 2.0 is performing transforms,
				// i.e. with the unparsed XML Source.
					
				accmTime = 0;
				for(int k = 0; k < iterCount; ++k)
				{
//
				}

				theAverage = calculateAvgTime(accmTime, iterCount);
				cout << "   Avg: " << theAverage << " for " << iterCount << " iter's w/UnParsed XML" << endl;

				logFile.addMetricToAttrs("avgunparsedxml",theAverage, attrs);

				// THIRD: Neither Stylesheet nor XML Source are parsed.
				// Perform multiple etoe transforms and calculate the average ...
		
				avgEtoe = 0;
				for(int jj = 0; jj < iterCount; ++jj)
				{	
//						
				}

				theAverage = calculateAvgTime(avgEtoe,iterCount);

				// Output average transform time to console and result log
				cout << "   Avg: " << theAverage << " for " << iterCount << " iter's of eToe" << endl;

				logFile.addMetricToAttrs("avgetoe",theAverage, attrs);


				logFile.logElement(10, "perf", attrs, "xxx");
			}//for files

		logFile.logTestCaseClose(XalanDOMString("Performance Directory: ") + dirs[j], XalanDOMString("Done") );
		}//for dirs

	logFile.logTestFileClose("Performance", "Done");
	logFile.close();

	} //if getParams

	return 0;
}
