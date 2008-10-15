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
#if !defined(TESTHARNESS_HEADER_GUARD_1357924680)
#define TESTHARNESS_HEADER_GUARD_1357924680

// Base header file.  Must be first.
#include <xalanc/Include/PlatformDefinitions.hpp>



#include <climits>
#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <fstream.h>
#else
#include <fstream>
#endif



#include <xalanc/Include/XalanMemoryManagement.hpp>
#include <xalanc/Include/XalanVector.hpp>
#include <xalanc/Include/XalanMap.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



#include <xalanc/XalanDOM/XalanNode.hpp>
#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/Harness/XalanFileUtility.hpp>
#include <xalanc/Harness/XalanXMLFileReporter.hpp>



#include "Utils.hpp"
#include "Logger.hpp"
#include "Timer.hpp"



XALAN_USING_XALAN(XalanMemMgrs)
XALAN_USING_XALAN(XalanVector)
XALAN_USING_XALAN(XalanMap)
XALAN_USING_XALAN(XalanNode)
XALAN_USING_XALAN(XalanDOMString)
XALAN_USING_XALAN(XalanFileUtility)
XALAN_USING_XALAN(XalanXMLFileReporter)



/**
 * Processor interface options
 */
struct ProcessorOptions
{
    XalanNode*  initOptions;
    XalanNode*  compileOptions;
    XalanNode*  parseOptions;
    XalanNode*  resultOptions;
    XalanNode*  transformOptions;

    ProcessorOptions() :
            initOptions(0),
            compileOptions(0),
            parseOptions(0),
            resultOptions(0),
            transformOptions(0)
    {
    }
};



/**
 * Test case
 */
class TestCase
{
public:
    
    TestCase();

    TestCase(const TestCase& theRhs);

    XalanDOMString  stylesheet;
    XalanDOMString  inputDocument;
    XalanDOMString  resultDocument;
    XalanDOMString  resultDirectory;
    XalanDOMString  goldResult;
    long            numIterations;
    long            minTimeToExecute;
    bool            verifyResult;
    XalanDOMString  inputMode;

    typedef XalanMap<XalanDOMString, ProcessorOptions>  ProcessorOptionsMap;

    ProcessorOptionsMap processorOptions;
};



typedef TestCase                    TestCaseType;
typedef XalanVector<TestCaseType>   TestCasesType;



/**
 *  Test harness
 */
template <class Processor>
class TestHarness
{
public:

    typedef typename Processor::CompiledStylesheetType  CompiledStylesheetType;
    typedef typename Processor::ParsedInputSourceType   ParsedInputSourceType;
    typedef typename Processor::ResultTargetType        ResultTargetType;

    typedef typename XalanXMLFileReporter::Hashtable    TestAttributesType;

    TestHarness();

    void init(
            XalanFileUtility&       fileUtility,
            XalanXMLFileReporter&   reporter,
            Logger&                 logger);

    void terminate();

    void executeTestCase(const TestCaseType& testCase);

    void executeTestCases(const TestCasesType& testCases);


protected:

    Processor m_processor;

    XalanFileUtility*       m_fileUtility;

    XalanXMLFileReporter*   m_reporter;

    Logger*                 m_logger;
};



template <class Processor>
void
TestHarness<Processor>::executeTestCases(const TestCasesType& testCases)
{
    TestCasesType::const_iterator testCaseIter = testCases.begin();
    while (testCaseIter != testCases.end())
    {
        executeTestCase(*testCaseIter);
        ++testCaseIter;
    }
}



template <class Processor>
void
TestHarness<Processor>::executeTestCase(const TestCaseType& testCase)
{
    TestAttributesType testAttributes(XalanMemMgrs::getDefaultXercesMemMgr());
    
    testAttributes.insert(TestAttributesType::value_type(XalanDOMString("stylesheet"), testCase.stylesheet));
    testAttributes.insert(TestAttributesType::value_type(XalanDOMString("input-document"), testCase.inputDocument));

    try {

        CompiledStylesheetType  compiledStylesheet = 0;
        ParsedInputSourceType   parsedInputSource = 0;

        static const ProcessorOptions defaultProcessor;
        
        TestCase::ProcessorOptionsMap::const_iterator iter = testCase.processorOptions.find(m_processor.getName());

        const ProcessorOptions&  processor = iter != testCase.processorOptions.end() ? iter->second : defaultProcessor;

        m_fileUtility->checkAndCreateDir(testCase.resultDirectory);

        Timer timeCompile;

        if (testCase.inputMode == XalanDOMString("stream"))
        {
#if defined(XALAN_CLASSIC_IOSTREAMS)
            XALAN_USING_XALAN(CharVectorType)
            XALAN_USING_XALAN(c_str)

            XALAN_USING_STD(istringstream)

            CharVectorType  buffer;
            fileToStream(testCase.stylesheet, buffer);

            istrstream      compilerStream(c_str(buffer));
#else
            XALAN_USING_STD(istringstream)

            istringstream compilerStream; 
            fileToStream(testCase.stylesheet, compilerStream);
#endif
            timeCompile.start();
            compiledStylesheet = m_processor.compileStylesheet(
                    compilerStream,
                    processor.compileOptions);
            timeCompile.stop();
        }
        else if (testCase.inputMode == XalanDOMString("file"))
        {
            timeCompile.start();
            compiledStylesheet = m_processor.compileStylesheet(
                    testCase.stylesheet,
                    processor.compileOptions);
            timeCompile.stop();
        }
        else 
        {
            XALAN_USING_STD(endl)

            m_logger->error() 
                    << "Mode: " 
                    << testCase.inputMode.c_str()
                    << " is invalid for stylesheet: "
                    << testCase.stylesheet
                    << endl;
        }

        m_reporter->addMetricToAttrs("compile-xsl", timeCompile.getElapsedTime(), testAttributes);

        long numIterations = 0;

        long totalParseInputTime = 0;
        long minParseInputTime = LONG_MAX;
        long maxParseInputTime = 0 ;

        long totalTransformTime = 0;
        long minTransformTime = LONG_MAX;
        long maxTransformTime = 0;


        Timer timeTotalRun;
        timeTotalRun.start();

        while (compiledStylesheet &&
               numIterations < testCase.numIterations &&
               timeTotalRun.getElapsedTime() < testCase.minTimeToExecute)
        {
            Timer timeInput;
            if (testCase.inputMode == XalanDOMString("stream") &&
                !parsedInputSource)
            {
#if defined(XALAN_CLASSIC_IOSTREAMS)
                XALAN_USING_XALAN(CharVectorType)
                XALAN_USING_XALAN(c_str)

                XALAN_USING_STD(istringstream)

                CharVectorType  buffer;
                fileToStream(testCase.inputDocument, buffer);

                istrstream      inputStream(c_str(buffer));
#else
                XALAN_USING_STD(istringstream)

                istringstream inputStream;
                fileToStream(testCase.inputDocument, inputStream);
#endif

                timeInput.start();
                parsedInputSource = m_processor.parseInputSource(
                        inputStream, 
                        processor.parseOptions);
                timeInput.stop();
            }
            else if (testCase.inputMode == XalanDOMString("file"))
            {
                timeInput.start();
                parsedInputSource = m_processor.parseInputSource(
                        testCase.inputDocument, 
                        processor.parseOptions);
                timeInput.stop();
            }
            else
            {
                XALAN_USING_STD(endl)

                m_logger->error() 
                    << "Mode: " 
                    << testCase.inputMode.c_str()
                    << " is inavlid for input document: "
                    << testCase.inputDocument
                    << endl;
                break;
            }

            totalParseInputTime += timeInput.getElapsedTime();
            minParseInputTime = timeInput.getElapsedTime() < minParseInputTime ? timeInput.getElapsedTime() : minParseInputTime;
            maxParseInputTime = timeInput.getElapsedTime() > maxParseInputTime ? timeInput.getElapsedTime() : maxParseInputTime;


            ResultTargetType    resultTarget =
                m_processor.createResultTarget(
                    testCase.resultDocument, 
                    processor.resultOptions);

            Timer timeTransform;
            timeTransform.start();
            m_processor.transform(
                    compiledStylesheet,
                    parsedInputSource,
                    resultTarget);
            timeTransform.stop();

            totalTransformTime += timeTransform.getElapsedTime();
            minTransformTime = timeTransform.getElapsedTime() < minTransformTime ? timeTransform.getElapsedTime() : minTransformTime;
            maxTransformTime = timeTransform.getElapsedTime() > maxTransformTime ? timeTransform.getElapsedTime() : maxTransformTime;

            ++numIterations;
            m_processor.releaseResultTarget(resultTarget);
        }

        timeTotalRun.stop();

        if (compiledStylesheet)
        {
            m_processor.releaseStylesheet(compiledStylesheet);
        }

        if (parsedInputSource)
        {
            m_processor.releaseInputSource(parsedInputSource);
        }

        if (true == testCase.verifyResult)
        {
            testAttributes.insert(TestAttributesType::value_type(XalanDOMString("verify"), XalanDOMString("yes")));
            if (checkFileExists(testCase.resultDocument))
            {
                if (checkFileExists(testCase.goldResult))
                {
                    if (m_fileUtility->compareSerializedResults(
                            testCase.resultDocument,
                            testCase.goldResult))
                    {
                        testAttributes.insert(TestAttributesType::value_type(XalanDOMString("result"), XalanDOMString("pass")));
                    }
                    else
                    {
                        testAttributes.insert(TestAttributesType::value_type(XalanDOMString("result"), XalanDOMString("fail")));
                    }
                }
                else
                {
                    testAttributes.insert(TestAttributesType::value_type(XalanDOMString("result"), XalanDOMString("incomplete")));
                }
            }
            else
            {
                testAttributes.insert(TestAttributesType::value_type(XalanDOMString("result"), XalanDOMString("incomplete")));
            }
        }
        else
        {
            testAttributes.insert(TestAttributesType::value_type(XalanDOMString("verify"), XalanDOMString("no")));
        }


        m_reporter->addMetricToAttrs("num-iterations", numIterations, testAttributes);
        m_reporter->addMetricToAttrs("elapsed-time", timeTotalRun.getElapsedTime(), testAttributes);
        m_reporter->addMetricToAttrs("min-parse-input", minParseInputTime, testAttributes);
        m_reporter->addMetricToAttrs("max-parse-input", maxParseInputTime, testAttributes);
        m_reporter->addMetricToAttrs("avg-parse-input", totalParseInputTime / numIterations, testAttributes);
        m_reporter->addMetricToAttrs("min-transform", minTransformTime, testAttributes);
        m_reporter->addMetricToAttrs("max-transform", maxTransformTime, testAttributes);
        m_reporter->addMetricToAttrs("avg-transform", totalTransformTime / numIterations, testAttributes);

        testAttributes.insert(TestAttributesType::value_type(XalanDOMString("complete"), XalanDOMString("yes")));

    }
    catch (const XalanDOMString& exception)
    {
        XALAN_USING_STD(endl)

        m_logger->error()  
                << "Error encountered during transformation: "
                << testCase.stylesheet.c_str()
                << ", error: "
                << exception.c_str()
                << endl;

        testAttributes.insert(TestAttributesType::value_type(XalanDOMString("complete"), XalanDOMString("no")));
    }
    catch (int)
    {
        testAttributes.insert(TestAttributesType::value_type(XalanDOMString("complete"), XalanDOMString("no")));
    }

    m_reporter->logElementWAttrs(1, "testcase", testAttributes, "");
}



template <class Processor>
TestHarness<Processor>::TestHarness()
{

}


template <class Processor>
void 
TestHarness<Processor>::init(
        XalanFileUtility&       fileUtility,
        XalanXMLFileReporter&   reporter,
        Logger&                 logger)
{
    m_processor.init();

    m_fileUtility = &fileUtility;

    m_reporter = &reporter;

    m_logger = &logger;
}



template <class Processor>
void
TestHarness<Processor>::terminate()
{
   m_processor.terminate(); 
}


#endif // TESTHARNESS_HEADER_GUARD_1357924680
