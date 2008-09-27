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



#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iomanip.h>
#else
#include <iomanip>
#endif



#include <xalanc/Include/XalanMemoryManagement.hpp>



#include <xalanc/XalanTransformer/XalanParsedSource.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



#include <xalanc/XalanDOM/XalanDocument.hpp>
#include <xalanc/XalanDOM/XalanNode.hpp>
#include <xalanc/XalanDOM/XalanNamedNodeMap.hpp>
#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/Harness/XalanFileUtility.hpp>



#include "Parameters.hpp"
#include "Utils.hpp"



XALAN_USING_XALAN(XalanTransformer)
XALAN_USING_XALAN(XalanParsedSource)
XALAN_USING_XALAN(XalanDocument)
XALAN_USING_XALAN(XalanNode)
XALAN_USING_XALAN(XalanNamedNodeMap)
XALAN_USING_XALAN(XalanDOMString)
XALAN_USING_XALAN(WideStringToLong)
XALAN_USING_XALAN(compareIgnoreCaseASCII)
XALAN_USING_XALAN(XalanMemMgrs)
XALAN_USING_XALAN(MemoryManager)
XALAN_USING_XALAN(XalanFileUtility)
XALAN_USING_XALAN(XSLTInputSource)





#if !defined(XALAN_NO_STD_NAMESPACE)
using std::endl;
#endif



/**
 * Processes parameters for the test harness
 */
Parameters::Parameters(
        const XalanDOMString&   runFileName,
        const XalanDOMString&   testDirectory,
        const XalanDOMString&   resultDirectory,
        const XalanDOMString&   baselineDirectory,
        const XalanDOMString&   reportDirectory,
        XalanFileUtility&       fileUtility,
        Logger&                 logger) :
    m_name("default"),
    m_description(""),
    m_resultDirectory(resultDirectory),
    m_resultFile("results"),
    m_threshold("5"),
    m_baselineDirectory(baselineDirectory),
    m_baselineFile(""),
    m_defaultTestCase(),
    m_testDirectory(testDirectory),
    m_goldDirectory(""),
    m_reportDirectory(reportDirectory),
    m_transformer(),
    m_uniqId(),
    m_initialized(false)
{
    fileUtility.generateUniqRunid(m_uniqId);

    if (parseConfigurationFile(runFileName, logger))
    {
        if (m_testDirectory.empty())
        {
            logger.error() << "No test directory specified on command line or in configuration file" << endl;
        }
        else
        {
            // configure directories
            // result directory
            if (m_resultDirectory.empty())
            {
                m_resultDirectory = getWorkingDirectory();
            }
            m_resultDirectory += getPathSep();

            // gold directory
            if (m_goldDirectory.empty())
            {
                m_goldDirectory = m_testDirectory;
                m_goldDirectory += XalanDOMString("-gold");
            }
            
            // test directory
            m_testDirectory += getPathSep();

            if (readTestCases(fileUtility, logger))
            {
                m_initialized = true;
            }

            // baseline directory
            if (m_baselineDirectory.empty())
            {
                m_baselineDirectory = m_resultDirectory;
            }
            else
            {
                m_baselineDirectory += getPathSep();
            }

            if (m_baselineFile.empty())
            {
                m_baselineFile = m_resultFile;
                m_baselineFile += XalanDOMString("_latest.xml");
            }

            // report directory
            if (m_reportDirectory.empty())
            {
                m_reportDirectory = getWorkingDirectory();
            }
            m_reportDirectory += getPathSep();
        }
    }
}



const TestCasesType& 
Parameters::getTestCases()
{
    return m_testCases;
}



bool
Parameters::parseConfigurationFile(
        const XalanDOMString&   runFileName,
        Logger&                 logger)
{
    // parse the configuration file and get default settings

    const XalanParsedSource* theParsedSource;

    XSLTInputSource confFile(runFileName, m_transformer.getMemoryManager());

    if (m_transformer.parseSource(confFile, theParsedSource) < 0)
    {
        logger.error() << "Failed to parse: " 
                << runFileName.c_str() 
                << ", error: " 
                << m_transformer.getLastError() 
                << endl;

        return false;
    }

    XalanDocument* document = theParsedSource->getDocument();

    XalanNode* runNode = document->getFirstChild();

    while (runNode->getNodeType() != XalanNode::ELEMENT_NODE)
    {
        runNode = runNode->getNextSibling();
        if (0 == runNode)
        {
            logger.error() << "Failed to parse: " 
                << runFileName.c_str() 
                << ", error: no <testconfig> found"
                << endl;

            return false;
        }
    }

    if(!(runNode->getNodeName() == XalanDOMString("testconfig")))
    {
        logger.error() << "Invalid configuration file: "
                << runFileName.c_str()
                << ", error: Missing <testconfig> tag" 
                << endl;

        return false;
    }

    const XalanNamedNodeMap* attributes = runNode->getAttributes();

    if (attributes != 0)
    {
        XalanNode* nameAttribute = attributes->getNamedItem(XalanDOMString("name"));

        if (nameAttribute != 0 &&
            !nameAttribute->getNodeValue().empty())
        {
            m_name = nameAttribute->getNodeValue();
        }
        else
        {
            logger.error() << "Invalid configuration file: "
                << runFileName.c_str()
                << ", error: Missing name attribute" 
                << endl;

            return false;
        }
    }

    XalanNode* currentNode = runNode->getFirstChild();

    while(currentNode != 0)
    {
        if (currentNode->getNodeType() != XalanNode::ELEMENT_NODE)
        {
            currentNode = currentNode->getNextSibling();
            continue;
        }

        // description element 
        if (currentNode->getNodeName() == XalanDOMString("description") &&
            currentNode->getFirstChild() != 0 &&
            currentNode->getFirstChild()->getNodeType() == XalanNode::TEXT_NODE)
        {
            m_description = currentNode->getFirstChild()->getNodeValue();
        }
        // results element
        else if (currentNode->getNodeName() == XalanDOMString("results"))
        {
            // file-path
            const XalanNamedNodeMap* resultAttributes = currentNode->getAttributes();
            XalanNode * attributeNode = resultAttributes->getNamedItem(XalanDOMString("file-path"));
            if (attributeNode != 0 &&
                attributeNode->getNodeType() == XalanNode::ATTRIBUTE_NODE)
            {
                if (!m_resultDirectory.empty())
                {
                    m_resultDirectory += getPathSep();
                }
                m_resultDirectory += attributeNode->getNodeValue();
            }

            // result report file name
            XalanNode * resultsNode = currentNode->getFirstChild();
            if (resultsNode != 0 
                && resultsNode->getNodeType() == XalanNode::TEXT_NODE
                && !resultsNode->getNodeValue().empty())
            {
                m_resultFile = resultsNode->getNodeValue();
            }
        }
        // baseline element
        else if (currentNode->getNodeName() == XalanDOMString("baseline"))
        {
            const XalanNamedNodeMap* attributeParams = currentNode->getAttributes();
            XalanNode* attributeNode;
            // threshold
            attributeNode = attributeParams->getNamedItem(XalanDOMString("threshold"));
            if (attributeNode != 0
                && !attributeNode->getNodeValue().empty())
            {
                m_threshold = attributeNode->getNodeValue().c_str();
            }
            // file-path
            attributeNode = attributeParams->getNamedItem(XalanDOMString("file-path"));
            if (attributeNode != 0)
            {
                if (!m_baselineDirectory.empty())
                {
                    m_baselineDirectory += getPathSep();
                }
                m_baselineDirectory += attributeNode->getNodeValue();
            }
            // baseline file
            XalanNode * baselineNode = currentNode->getFirstChild();
            if (baselineNode != 0
                && baselineNode->getNodeType() == XalanNode::TEXT_NODE
                && !baselineNode->getNodeValue().empty())
            {
                m_baselineFile += baselineNode->getNodeValue();
            }
        }
        // default parameters
        else if (currentNode->getNodeName() == XalanDOMString("default-parameter-set"))
        {
            const XalanNamedNodeMap* attributeParams = currentNode->getAttributes();
            XalanNode* attributeNode;
            attributeNode = attributeParams->getNamedItem(XalanDOMString("input-mode"));
            if (attributeNode != 0 &&
                attributeNode->getNodeType() == XalanNode::ATTRIBUTE_NODE &&
                !attributeNode->getNodeValue().empty())
            {
                m_defaultTestCase.inputMode = attributeNode->getNodeValue(); 
            }
            // number of iterations
            attributeNode = attributeParams->getNamedItem(XalanDOMString("num-iterations"));
            if (attributeNode != 0 &&
                attributeNode->getNodeType() == XalanNode::ATTRIBUTE_NODE &&
                WideStringToLong(attributeNode->getNodeValue().c_str()) > 0)
            {
                m_defaultTestCase.numIterations = WideStringToLong(attributeNode->getNodeValue().c_str()); 
            }
            // minimum time to execute
            attributeNode = attributeParams->getNamedItem(XalanDOMString("min-time-to-execute"));
            if (attributeNode != 0 &&
                attributeNode->getNodeType() == XalanNode::ATTRIBUTE_NODE &&
                WideStringToLong(attributeNode->getNodeValue().c_str()) > 0)
            {
                m_defaultTestCase.minTimeToExecute = WideStringToLong(attributeNode->getNodeValue().c_str());
            }
            // verify the result
            attributeNode = attributeParams->getNamedItem(XalanDOMString("verify-result"));
            if (attributeNode != 0 &&
                attributeNode->getNodeType() == XalanNode::ATTRIBUTE_NODE &&
                compareIgnoreCaseASCII(attributeNode->getNodeValue().c_str(),XalanDOMString("yes").c_str()) == 0)
            {
                m_defaultTestCase.verifyResult = true;
            }
            // get processor specific parameters
            XalanNode * options = currentNode->getFirstChild();
            while (options != 0)
            {
                if (options->getNodeType() != XalanNode::ELEMENT_NODE)
                {
                    options = options->getNextSibling();
                    continue;
                }

                const XalanDOMString* processorName = 0;

                const XalanNamedNodeMap* optionAttributes = options->getAttributes();
                XalanNode* processorOption = optionAttributes->getNamedItem(XalanDOMString("processor"));
                if (processorOption != 0 &&
                    processorOption->getNodeType() == XalanNode::ATTRIBUTE_NODE)
                {
                    processorName = &(processorOption->getNodeValue());
                }

                if (processorName != 0 && processorName->empty() == true)
                {
                    options = options->getNextSibling();
                    continue;
                }

                if (options->getNodeName() == XalanDOMString("init-options"))
                {
                    m_defaultTestCase.processorOptions[*processorName].initOptions = options->getFirstChild();
                }
                else if (options->getNodeName() == XalanDOMString("compile-options"))
                {
                    m_defaultTestCase.processorOptions[*processorName].compileOptions = options->getFirstChild();
                }
                else if (options->getNodeName() == XalanDOMString("parse-options"))
                {
                    m_defaultTestCase.processorOptions[*processorName].parseOptions = options->getFirstChild();
                }
                else if (options->getNodeName() == XalanDOMString("result-options"))
                {
                    m_defaultTestCase.processorOptions[*processorName].resultOptions = options->getFirstChild();
                }
                else if (options->getNodeName() == XalanDOMString("transform-options"))
                {
                    m_defaultTestCase.processorOptions[*processorName].transformOptions = options->getFirstChild();
                }

                options = options->getNextSibling();
            }
        }
        else if (currentNode->getNodeName() == XalanDOMString("testcases"))
        {
            // file-path
            const XalanNamedNodeMap* testCasesAttributes = currentNode->getAttributes();
            XalanNode * attributeNode = testCasesAttributes->getNamedItem(XalanDOMString("file-path"));
            if (attributeNode != 0 &&
                attributeNode->getNodeType() == XalanNode::ATTRIBUTE_NODE)
            {
                if (!m_testDirectory.empty())
                {
                    m_testDirectory += getPathSep();
                }
                m_testDirectory += attributeNode->getNodeValue();
            }
            // gold file-path
            attributeNode = testCasesAttributes->getNamedItem(XalanDOMString("gold-file-path"));
            if (attributeNode != 0 &&
                attributeNode->getNodeType() == XalanNode::ATTRIBUTE_NODE)
            {
                m_goldDirectory = attributeNode->getNodeValue();
            }
        }
        currentNode = currentNode->getNextSibling();
    }

    return true;
}



bool
Parameters::readTestCases(
    XalanFileUtility&   fileUtility,
    Logger&             logger)
{
    if (!fileUtility.checkDir(m_testDirectory))
    {
        logger.error() << "Invalid test directory: " << m_testDirectory.c_str() << endl;
        return false;
    }

    if (m_goldDirectory.empty())
    {
        logger.message() << "No gold directory specified" << endl;
        if (!fileUtility.checkDir(m_goldDirectory))
        {
            logger.warning() << "Invalid gold directory: " << m_goldDirectory.c_str() << endl;
        }
    }

    fileUtility.checkAndCreateDir(m_resultDirectory);

    typedef XalanFileUtility::FileNameVectorType FileNamesType;
    FileNamesType dirNames;

    fileUtility.getDirectoryNames(m_testDirectory, dirNames);

    FileNamesType::iterator dirIter = dirNames.begin();
    
    // for each test directory
    while (dirIter != dirNames.end())
    {
       
        FileNamesType xslTestFiles;
        fileUtility.getTestFileNames(m_testDirectory, *dirIter, true, xslTestFiles);

        FileNamesType::const_iterator xslIter = xslTestFiles.begin();

        // for each stylesheet
        while (xslIter != xslTestFiles.end())
        {
            // configure the stylesheet
            TestCase testCase = m_defaultTestCase;
            testCase.stylesheet = m_testDirectory;
            testCase.stylesheet += *dirIter;
            testCase.stylesheet += getPathSep();
            testCase.stylesheet += *xslIter;

            
            bool status = true;

            // configure the input document
            fileUtility.generateFileName(testCase.stylesheet, "xml", testCase.inputDocument, &status);
            if (status != true)
            {
                logger.warning() << "No matching input file for: " << testCase.stylesheet.c_str() << endl;
                ++xslIter;
                continue;
            }

            // configure result directory
            testCase.resultDirectory = m_resultDirectory;
            testCase.resultDirectory += *dirIter;
 
            // configure result document        
            XalanDOMString outFile = testCase.resultDirectory;
            outFile += getPathSep();
            outFile += *xslIter;

            fileUtility.generateFileName(outFile, "out", testCase.resultDocument);

            // configure gold result
            outFile = m_goldDirectory;
            outFile += getPathSep();
            outFile += *dirIter;
            outFile += getPathSep();
            outFile += *xslIter;

            status = true;
            fileUtility.generateFileName(outFile, "out", testCase.goldResult, &status);
            if (true == testCase.verifyResult && !status)
            {
                logger.warning() << "Verification on, but no matching gold file: " << testCase.goldResult.c_str() << endl;
            }

            m_testCases.push_back(testCase);
            
            ++xslIter;
        }

        ++dirIter;
    }

    return true;
}
