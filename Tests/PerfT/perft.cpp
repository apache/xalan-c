/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2001 The Apache Software Foundation.  All rights 
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

#include <ctime>
#include <iostream>
#include <strstream>
#include <stdio.h>
#include <direct.h>

#if !defined(XALAN_NO_NAMESPACES)
	using std::cerr;
	using std::cout;
	using std::endl;
#endif

#include <util/PlatformUtils.hpp>

#include <PlatformSupport/DOMStringHelper.hpp>
#include <XalanTransformer/XalanTransformer.hpp>

// HARNESS HEADERS...
#include <Harness/XMLFileReporter.hpp>
#include <Harness/FileUtility.hpp>
#include <Harness/HarnessInit.hpp>

#if defined(XALAN_NO_NAMESPACES)
	typedef map<XalanDOMString, XalanDOMString, less<XalanDOMString> >	Hashtable;
#else
	typedef std::map<XalanDOMString, XalanDOMString>  Hashtable;
#endif

// This is here for memory leak testing.
#if !defined(NDEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif



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
	assert(theIterationCount > 1);

	return double(accTime) / theIterationCount;
}


void
setHelp(FileUtility&	h)
{
	h.args.getHelpStream() << endl
		 << "Perft dir [-out -sub -i -iter]"
		 << endl
		 << endl
		 << "dir		( base directory for testcases)"
		 << endl
		 << "-out dir	( base directory for output)"
		 << endl
		 << "-sub dir	( run files only from a specific directory)"
		 << endl
		 << "-i		( include all testcases )"
		 << endl
		 << "-iter num	(specifies number of iterations; must be > 0)"
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

	HarnessInit		xmlPlatformUtils;

	FileUtility		h;

	// Set the program help string,  then get the command line parameters.
	//
	setHelp(h);

	bool setGold = false;
	{
		const XalanDOMString	processorType(XALAN_STATIC_UCODE_STRING("XalanC"));
		long iterCount;			// Default number of iterations
		bool skip = true;		// Default will skip long tests

		if (h.getParams(argc, argv, "PERFT-RESULTS", setGold) == true)
		{
			//
			// Call the static initializers for xerces and xalan, and create a transformer
			//
			XalanTransformer::initialize();
			{
				XalanTransformer xalan;

				// Generate Unique Run id and processor info
				const XalanDOMString UniqRunid = h.generateUniqRunid();


				// Defined basic constants for file manipulation and open results file
				const XalanDOMString  resultFilePrefix("cpp");
				const XalanDOMString  resultsFile(h.args.output + resultFilePrefix + UniqRunid + FileUtility::s_xmlSuffix);


				XMLFileReporter	logFile(resultsFile);
				logFile.logTestFileInit("Performance Testing - Reports various performance metrics using the Transformer");


				// Get the list of sub-directories below "base" and iterate through them
				bool foundDir = false;		// Flag indicates directory found. Used in conjunction with -sub cmd-line arg.
				const FileNameVectorType dirs = h.getDirectoryNames(h.args.base);

				for(FileNameVectorType::size_type	j = 0; j < dirs.size(); j++)
				{
					// Run specific category of files from given directory
					if (length(h.args.sub) > 0 && !equals(dirs[j], h.args.sub))
					{
						continue;
					}

					cout << "Processing files in Directory: " << dirs[j] << endl;

					// Check that output directory is there.
					const XalanDOMString  theOutputDir = h.args.output + dirs[j];
					h.checkAndCreateDir(theOutputDir);

					
					// Indicate that directory was processed and get test files from the directory
					foundDir = true;
					const FileNameVectorType files = h.getTestFileNames(h.args.base, dirs[j], false);
					logFile.logTestCaseInit(XalanDOMString("Performance Directory: ") + dirs[j] ); 

					for(FileNameVectorType::size_type i = 0; i < files.size(); i++)
					{
						// Define  variables used for timing and reporting ...
						clock_t startTime, endTime, accmTime, avgEtoe;
						double timeinMilliseconds = 0, theAverage =0;
						int transformResult = 0;
						Hashtable attrs;

						attrs.insert(Hashtable::value_type(XalanDOMString("idref"), files[i]));
						attrs.insert(Hashtable::value_type(XalanDOMString("UniqRunid"),UniqRunid));
						attrs.insert(Hashtable::value_type(XalanDOMString("processor"),processorType));
						logFile.addMetricToAttrs("Iterations",iterCount, attrs);
								
						if (h.args.skip)
						{
							if (checkForExclusion(files[i]))
								continue;
						}

						const XalanDOMString  theXSLFile= h.args.base + dirs[j] + FileUtility::s_pathSep + files[i];
						const XalanDOMString  theXMLFile = h.generateFileName(theXSLFile,"xml");

						const XalanDOMString  outbase =  h.args.output + dirs[j] + FileUtility::s_pathSep + files[i]; 
						const XalanDOMString  theOutputFile = h.generateFileName(outbase, "out");

						const XSLTInputSource	xslInputSource(c_wstr(theXSLFile));
						const XSLTInputSource	xmlInputSource(c_wstr(theXMLFile));
						const XSLTResultTarget	theResultTarget(theOutputFile);

						attrs.insert(Hashtable::value_type(XalanDOMString("href"), theXSLFile));
						cout << endl << files[i] << endl;

						// Time the parsing(compile) of the XSL stylesheet and report the results..
						//
						startTime = clock();
						const XalanCompiledStylesheet*	compiledSS = 0;
						xalan.compileStylesheet(xslInputSource, compiledSS);
						endTime = clock();

						if (compiledSS == 0)
						{
							continue;
						}

						timeinMilliseconds = calculateElapsedTime(startTime, endTime);
						cout << "   XSL: " << timeinMilliseconds << " milliseconds, Parse" << endl;
						logFile.addMetricToAttrs("parsexsl",timeinMilliseconds, attrs);	

						// Time the parsing of the input XML and report the results..
						//
						startTime = clock();
						const XalanParsedSource*	parsedSource = 0;
						xalan.parseSource(xmlInputSource, parsedSource);
						endTime = clock();

						if (parsedSource == 0)
						{
							continue;
						}

						timeinMilliseconds = calculateElapsedTime(startTime, endTime);
						cout << "   XML: " << timeinMilliseconds << " milliseconds, Parse" <<endl;
						logFile.addMetricToAttrs("parsexml",timeinMilliseconds, attrs);

						// Perform One transform using parsed stylesheet and unparsed xml source, report results...
						// 
						startTime = clock();
							transformResult = xalan.transform(xmlInputSource, compiledSS, theResultTarget);
						endTime = clock();
						if(!transformResult)
						{
							timeinMilliseconds = calculateElapsedTime(startTime, endTime);
							cout << endl << "   One: " << timeinMilliseconds << " w/Parsed XSL." << endl;
							logFile.addMetricToAttrs("single", timeinMilliseconds, attrs);	
						}
						else
						{
							cout << xalan.getLastError();
							return 0;
						}

						// Do One eTOe transform with no pre parsing of either xsl or xml files.
						// And output metrics to console and result log
						//
						startTime = clock();
							transformResult = xalan.transform(xmlInputSource, xslInputSource, theResultTarget);
						endTime = clock();
						if(!transformResult)
						{
							timeinMilliseconds = calculateElapsedTime(startTime, endTime);
							cout << "   One: " << timeinMilliseconds << " eTOe." << endl;
							logFile.addMetricToAttrs("etoe", timeinMilliseconds, attrs);	
						}
						else
						{
							cout << xalan.getLastError();
							return 0;
						}


						// Perform multiple transforms and calculate the average time ..
						// These are done 3 different ways.
						//
						// FIRST: Parsed XSL Stylesheet and Parsed XML Source.
						//
						iterCount = h.args.iters;
						accmTime = 0;
						for(int j = 0; j < iterCount; ++j)
						{	
							startTime = clock();
								transformResult = xalan.transform(*parsedSource, compiledSS, theResultTarget);
							endTime = clock();
					
							accmTime += endTime - startTime;
						}

						theAverage = calculateAvgTime(accmTime, iterCount); 
						cout << endl << "   Avg: " << theAverage << " for " << iterCount << " iter's w/Parsed files" << endl;
						logFile.addMetricToAttrs("avgparsedxml",theAverage, attrs);
				

						// SECOND: Parsed Stylesheet and UnParsed XML Source.
						// This is currently how the XalanJ 2.0 is performing transforms
						//
						accmTime = 0;
						for(int k = 0; k < iterCount; ++k)
						{
							startTime = clock();
								transformResult = xalan.transform(xmlInputSource, compiledSS, theResultTarget);
							endTime = clock();
					
							accmTime += endTime - startTime;						
						}
						theAverage = calculateAvgTime(accmTime, iterCount);
						cout << "   Avg: " << theAverage << " for " << iterCount << " iter's w/UnParsed XML" << endl;
						logFile.addMetricToAttrs("avgunparsedxml",theAverage, attrs);


						// THIRD: Neither Stylesheet nor XML Source are parsed.
						// Perform multiple etoe transforms and calculate the average ...
						//
						avgEtoe = 0;
						for(int jj = 0; jj < iterCount; ++jj)
						{	
							startTime = clock();
								transformResult = xalan.transform(xmlInputSource, xslInputSource, theResultTarget);
							endTime = clock();
					
							avgEtoe += endTime - startTime;						
						}
						theAverage = calculateAvgTime(avgEtoe,iterCount);

						// Output average transform time to console and result log
						cout << "   Avg: " << theAverage << " for " << iterCount << " iter's of eToe" << endl;
						logFile.addMetricToAttrs("avgetoe",theAverage, attrs);

						logFile.logElementWAttrs(10, "perf", attrs, "xxx");

						xalan.destroyParsedSource(parsedSource);
						xalan.destroyStylesheet(compiledSS);

					}

				logFile.logTestCaseClose(XalanDOMString("Performance Directory: ") + dirs[j], XalanDOMString("Done") );
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
		}
	
		XalanTransformer::terminate();
	}

	return 0;
}
