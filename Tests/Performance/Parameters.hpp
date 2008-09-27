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
#if !defined(XALANCPARAMETERS_HEADER_GUARD_1357924680)
#define XALANCPARAMETERS_HEADER_GUARD_1357924680

// Base header file.  Must be first.
#include <xalanc/Include/PlatformDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>




#include <xalanc/XalanTransformer/XalanTransformer.hpp>



#include <xalanc/Harness/XalanFileUtility.hpp>



#include "TestHarness.hpp"
#include "Logger.hpp"



XALAN_USING_XALAN(XalanDOMChar)
XALAN_USING_XALAN(XalanDOMString)
XALAN_USING_XALAN(XalanTransformer)
XALAN_USING_XALAN(XalanFileUtility)



class Parameters
{
public:
    Parameters(
        const XalanDOMString&   runFileName,
        const XalanDOMString&   testDirectory,
        const XalanDOMString&   resultDirectory,
        const XalanDOMString&   baselineDirectory,
        const XalanDOMString&   reportDirectory,
        XalanFileUtility&       fileUtility,
        Logger&                 log);
/*
    const XalanDOMString& 
    getRunName() { return m_name;}
*/
    bool
    initialized() {return m_initialized;}

    XalanDOMString&
    getName() {return m_name;}

    XalanDOMString&
    getUniqId() {return m_uniqId;}

    XalanDOMString&
    getDescription() {return m_description;}

    XalanDOMString&
    getResultDirectory() {return m_resultDirectory;}

    XalanDOMString&
    getResultFile() {return m_resultFile;}

    XalanDOMString&
    getThreshold() { return m_threshold;}

    XalanDOMString&
    getBaselineDirectory() {return m_baselineDirectory;}

    XalanDOMString&
    getBaselineFile() {return m_baselineFile;}

    XalanDOMString&
    getReportDirectory() {return m_reportDirectory;}
    
    const TestCasesType& 
    getTestCases();

protected:

    bool parseConfigurationFile(
            const XalanDOMString& runFileName, 
            Logger& logger);

    bool readTestCases(
            XalanFileUtility&   fileUtility,
            Logger &            logger);

    XalanDOMString  m_name;
    XalanDOMString  m_description;

    XalanDOMString  m_resultDirectory;
    XalanDOMString  m_resultFile;

    XalanDOMString  m_threshold;
    XalanDOMString  m_baselineDirectory;
    XalanDOMString  m_baselineFile;
    
    TestCase        m_defaultTestCase;

    XalanDOMString  m_testDirectory;
    XalanDOMString  m_goldDirectory;
    XalanDOMString  m_reportDirectory;

    TestCasesType   m_testCases;

    XalanTransformer m_transformer;

    XalanDOMString  m_uniqId;

    bool m_initialized;
};



#endif // XALANCPARAMETERS_HEADER_GUARD_1357924680


