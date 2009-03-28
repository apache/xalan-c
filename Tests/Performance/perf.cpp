/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the  "License");
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



#include <cstring>
#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#else
#include <iostream>
#endif



#if defined(XALAN_STRICT_ANSI_HEADERS)
using std::strcmp;
#endif



#include <xercesc/util/PlatformUtils.hpp>



#include <xalanc/XalanTransformer/XalanTransformer.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/Harness/XalanXMLFileReporter.hpp>
#include <xalanc/Harness/XalanFileUtility.hpp>



#include "Parameters.hpp"
#include "TestHarness.hpp"
#include "XalanCProcessor.hpp"
#include "Logger.hpp"
#include "Utils.hpp"



XALAN_USING_STD(cout);
XALAN_USING_STD(cerr);
XALAN_USING_STD(endl);


XALAN_USING_XERCES(XMLPlatformUtils)



XALAN_USING_XALAN(XalanTransformer)
XALAN_USING_XALAN(XalanDOMString)
XALAN_USING_XALAN(XalanXMLFileReporter)
XALAN_USING_XALAN(XalanFileUtility)



void usage()
{
    cerr << "Usage: perf"
         << " [options]"
         << " -test [test directory ]"
         << " -result [result directory ]"
         << " -baseline [baseline directory]"
         << " -report [report directory ]"
         << " configfile" << endl
         << "Options:" << endl
         << "        -?    Display this message" << endl;
}



void generateReports(
        Parameters&             params,
        const XalanDOMString&   reportFile,
        Logger&                 logger)
{
    XalanTransformer transformer;

    // report XSL file
    XalanDOMString reportFileXSL = params.getReportDirectory();
    reportFileXSL += XalanDOMString("report.xsl");

    // html result
    XalanDOMString htmlReport = params.getResultDirectory();
    htmlReport += params.getResultFile();
    htmlReport += params.getUniqId();
    htmlReport += XalanDOMString(".html");

    if (checkFileExists(reportFileXSL))
    {
        if (transformer.transform(reportFile, reportFileXSL, htmlReport) < 0)
        {
            logger.warning() 
                << "Failed to generate HTML report: "
                << htmlReport.c_str()
                << ", error: " 
                << transformer.getLastError() << endl;
        }
        else
        {
            logger.message() << "Generated HTML report: " << htmlReport.c_str() << endl;
        }
    }
    else
    {
        logger.warning()
            << "Could not generate HTML report, stylesheet: "
            << reportFileXSL.c_str()
            << ", file not found" << endl;
    }

    XalanDOMString baselineFile = params.getBaselineDirectory();
    baselineFile += params.getBaselineFile();

    if (checkFileExists(baselineFile))
    {
        // comparison report XSL file
        XalanDOMString compareReportFileXSL = params.getReportDirectory();
        compareReportFileXSL += XalanDOMString("comparereport.xsl");

        // html result
        XalanDOMString htmlCompareReport = params.getResultDirectory();
        htmlCompareReport += XalanDOMString("compare");
        htmlCompareReport += params.getResultFile();
        htmlCompareReport += params.getUniqId();
        htmlCompareReport += XalanDOMString(".html");

        XalanDOMString baselineFileStr = XalanDOMString("'");
                    baselineFileStr += baselineFile;
                    baselineFileStr += XalanDOMString("'");

        transformer.setStylesheetParam(XalanDOMString("threshold"),params.getThreshold());
        transformer.setStylesheetParam(XalanDOMString("baseline"), baselineFileStr);

        if (checkFileExists(compareReportFileXSL))
        {
            if (transformer.transform(reportFile, compareReportFileXSL, htmlCompareReport) < 0)
            {
                logger.warning() 
                    << "Failed to generate HTML report: " 
                    << htmlCompareReport.c_str()
                    << ", error: " 
                    << transformer.getLastError() << endl;
            }
            else
            {
                logger.message() << "Generated HTML report: " << htmlCompareReport.c_str() << endl;
            }
        }
        else
        {
            logger.warning()
                << "Could not generate HTML report, stylesheet: "
                << compareReportFileXSL.c_str()
                << ", file not found" << endl;
        }
    }
    else
    {
        logger.warning()
            << "No baseline file found: "
            << baselineFile.c_str()
            << endl;
    }
}


int main(int argc, char* argv[])
{
    XMLPlatformUtils::Initialize();
    XalanTransformer::initialize();

    Logger logger(cerr);

    if (argc < 2 ||
        XalanDOMString("-?").compare(XalanDOMString(argv[1])) == 0)
    {
        usage();
    }
    else
    {
        XalanDOMString testDirectory;
        XalanDOMString resultDirectory;
        XalanDOMString baselineDirectory;
        XalanDOMString reportDirectory;
        XalanDOMString runFileName;

        // process command line parameters
        int i = 1;
        while (i < argc)
        {
            if (strcmp(argv[i],"-test") == 0)
            {
                ++i;
                if (i >= argc)
                {
                    logger.error() << "Test directory missing" << endl;
                    usage();
                    exit(1);
                }
                testDirectory.assign(argv[i]);
            }
            else if (strcmp(argv[i],"-result") == 0)
            {
                ++i;
                if (i >= argc)
                {
                    logger.error() << "Result directory missing" << endl;
                    usage();
                    exit(1);
                }
                resultDirectory.assign(argv[i]);
            }
            else if (strcmp(argv[i],"-baseline") == 0)
            {
                ++i;
                if (i >= argc)
                {
                    logger.error() << "Baseline directory missing" << endl;
                    usage();
                    exit(1);
                }
                baselineDirectory.assign(argv[i]);
            }
            else if (strcmp(argv[i],"-report") == 0)
            {
                ++i;
                if (i >= argc)
                {
                    logger.error() << "Report directory missing" << endl;
                    usage();
                    exit(1);
                }
                reportDirectory.assign(argv[i]);
            }
            else
            {
                break;
            }

            ++i;
        }

        if (i >= argc)
        {
            logger.error() << "Run file not specified" << endl;
            usage();
            exit(1);
        }
        else
        {
            runFileName.assign(argv[i]);
        }


        logger.message() << "Loading configuration file: " << runFileName.c_str() << endl;

        XalanFileUtility fileUtility(XalanMemMgrs::getDefaultXercesMemMgr());

        // setup testing parameters
        Parameters params(
                runFileName,
                testDirectory,
                resultDirectory,
                baselineDirectory,
                reportDirectory,
                fileUtility,
                logger);

        if (!params.initialized())
        {
            exit(1);
        }

        // create report file 
        XalanDOMString reportFile = params.getResultDirectory();
        reportFile += params.getResultFile();
        reportFile += params.getUniqId();
        reportFile += XalanDOMString(".xml");

        XalanXMLFileReporter reporter(XalanMemMgrs::getDefaultXercesMemMgr(), reportFile);

        reporter.logTestFileInit(params.getDescription());

        // run test harness
        typedef TestHarness<XalanCProcessor> XalanCTestHarness;

        XalanCTestHarness testHarness;

        testHarness.init(fileUtility, reporter, logger);

        testHarness.executeTestCases(params.getTestCases());

        reporter.logTestFileClose("","");

        reporter.close();

        // generate reports
        generateReports(params, reportFile, logger);

        // create latest copy for baseline testing
        XalanDOMString latestReportFile = params.getResultDirectory();
        latestReportFile += params.getResultFile();
        latestReportFile += XalanDOMString("_latest.xml");
        copyFile(latestReportFile, reportFile);
    }

    XalanTransformer::terminate();
    XMLPlatformUtils::Terminate();

    return 0;
}
