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

#include "XPathCAPI.h"



#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/sax/SAXException.hpp>

#include <xalanc/Include/XalanAutoPtr.hpp>

#include <xalanc/XalanDOM/XalanDocument.hpp>



#include <xalanc/PlatformSupport/XalanTranscodingServices.hpp>



#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/XPath.hpp>
#include <xalanc/XPath/XPathConstructionContextDefault.hpp>
#include <xalanc/XPath/XPathEvaluator.hpp>
#include <xalanc/XPath/XPathExecutionContextDefault.hpp>
#include <xalanc/XPath/XPathFactoryDefault.hpp>



#include <xalanc/XalanSourceTree/XalanSourceTreeInit.hpp>
#include <xalanc/XalanSourceTree/XalanSourceTreeDOMSupport.hpp>
#include <xalanc/XalanSourceTree/XalanSourceTreeParserLiaison.hpp>



static bool fInitialized = false;
static bool fTerminated = false;




XALAN_USING_XALAN(XalanDOMChar)
XALAN_USING_XALAN(XalanDOMString)
XALAN_USING_XALAN(XalanOutputTranscoder)
XALAN_USING_XALAN(XPath)
XALAN_USING_XALAN(XPathEvaluator)
XALAN_USING_XALAN(XalanSourceTreeInit)
XALAN_USING_XALAN(XalanTranscodingServices)
XALAN_USING_XALAN(XalanMemMgrs)


static XalanSourceTreeInit*     theSourceTreeInit = 0;



XALAN_XPATHCAPI_EXPORT_FUNCTION(int)
XalanXPathAPIInitialize()
{
    if (fInitialized == true)
    {
        return XALAN_XPATH_API_ERROR_ALREADY_INITIALIZED;
    }
    else if (fTerminated == true)
    {
        return XALAN_XPATH_API_ERROR_CANNOT_REINITIALIZE;
    }
    else
    {
        int theResult = XALAN_XPATH_API_SUCCESS;

        try
        {
            XALAN_USING_XERCES(XMLPlatformUtils)

            XMLPlatformUtils::Initialize();

            try
            {
                XPathEvaluator::initialize(XalanMemMgrs::getDefaultXercesMemMgr());

                try
                {
                    theSourceTreeInit = new XalanSourceTreeInit(XalanMemMgrs::getDefaultXercesMemMgr());
                }
                catch(...)
                {
                    XPathEvaluator::terminate();

                    throw;
                }
            }
            catch(...)
            {
                XMLPlatformUtils::Terminate();

                throw;
            }

            fInitialized = true;
        }
        catch(...)
        {
            fTerminated = true;

            theResult = XALAN_XPATH_API_ERROR_INITIALIZATION_FAILED;
        }

        return theResult;
    }
}



XALAN_XPATHCAPI_EXPORT_FUNCTION(int)
XalanXPathAPITerminate()
{
    if (fInitialized == false)
    {
        return XALAN_XPATH_API_ERROR_NOT_INITIALIZED;
    }
    else if (fTerminated == true)
    {
        return XALAN_XPATH_API_ERROR_ALREADY_TERMINATED;
    }
    else
    {
        int theResult = XALAN_XPATH_API_SUCCESS;

        try
        {
            XALAN_USING_XERCES(XMLPlatformUtils)

            delete theSourceTreeInit;

            theSourceTreeInit = 0;

            XPathEvaluator::terminate();

            XMLPlatformUtils::Terminate();

            fTerminated = true;
        }
        catch(...)
        {
            theResult = XALAN_XPATH_API_ERROR_TERMINATION_FAILED;
        }

        return theResult;
    }
}



XALAN_XPATHCAPI_EXPORT_FUNCTION(int)
XalanCreateXPathEvaluator(XalanXPathEvaluatorHandle*    theHandle)
{
    if (fInitialized == false)
    {
        return XALAN_XPATH_API_ERROR_NOT_INITIALIZED;
    }
    else if (fTerminated == true)
    {
        return XALAN_XPATH_API_ERROR_ALREADY_TERMINATED;
    }
    else if (theHandle == 0)
    {
        return XALAN_XPATH_API_ERROR_INVALID_PARAMETER;
    }
    else
    {
        int theResult = XALAN_XPATH_API_SUCCESS;

        try
        {
            *theHandle = new XPathEvaluator(XalanMemMgrs::getDefaultXercesMemMgr());
        }
        catch(...)
        {
            theResult = XALAN_XPATH_API_ERROR_UNKNOWN;
        }

        return theResult;
    }
}



inline XPathEvaluator*
getEvaluator(XalanXPathEvaluatorHandle  theHandle)
{
    assert(theHandle != 0);

    return static_cast<XPathEvaluator*>(theHandle);
}



inline XPath*
getXPath(XalanXPathHandle   theHandle)
{
    assert(theHandle != 0);

    return static_cast<XPath*>(theHandle);
}



XALAN_XPATHCAPI_EXPORT_FUNCTION(int)
XalanDestroyXPathEvaluator(XalanXPathEvaluatorHandle    theXalanHandle)
{
    if (fInitialized == false)
    {
        return XALAN_XPATH_API_ERROR_NOT_INITIALIZED;
    }
    else if (fTerminated == true)
    {
        return XALAN_XPATH_API_ERROR_ALREADY_TERMINATED;
    }
    else if (theXalanHandle == 0)
    {
        return XALAN_XPATH_API_ERROR_INVALID_PARAMETER;
    }
    else
    {
        int theResult = XALAN_XPATH_API_SUCCESS;

        try
        {
            delete getEvaluator(theXalanHandle);
        }
        catch(...)
        {
            theResult = XALAN_XPATH_API_ERROR_UNKNOWN;
        }

        return theResult;
    }
}


typedef XalanTranscodingServices::size_type     size_type;

inline int
transcodeString(
            XalanOutputTranscoder*  theTranscoder,
            const char*             theString,
            XalanDOMChar*           theChars,
            unsigned char*          theCharsCount,
            size_type               theLength,
            XalanDOMString&         theResultString)
{
    assert(theTranscoder != 0);
    assert(theString != 0);
    assert(theChars != 0);
    assert(theCharsCount != 0);

    size_type   theSourceCharsTranscoded = 0;
    size_type   theTargetBytesUsed = 0;

    const XalanTranscodingServices::eCode   theCode = theTranscoder->transcode(
                    reinterpret_cast<const XalanOutputTranscoder::XalanXMLByte*>(theString),
                    theLength,
                    theChars,
                    theLength,
                    theSourceCharsTranscoded,
                    theTargetBytesUsed,
                    theCharsCount);

    if (theCode != XalanTranscodingServices::OK)
    {
        return XALAN_XPATH_API_ERROR_TRANSCODING;
    }
    else
    {
        assert(XalanDOMString::size_type(theTargetBytesUsed) == theTargetBytesUsed);

        theResultString.assign(theChars, XalanDOMString::size_type(theTargetBytesUsed));

        return XALAN_XPATH_API_SUCCESS;
    }
}



inline int
transcodeString(
            const char*         theString,
            const char*         theStringEncoding,
            XalanDOMString&     theResultString)
{
    assert(theString != 0);

    int     theResult = XALAN_XPATH_API_SUCCESS;

    if (theStringEncoding == 0 || XalanDOMString::length(theStringEncoding) == 0)
    {
        theResultString = theString;
    }
    else
    {
        XalanTranscodingServices::eCode     theCode = XalanTranscodingServices::OK;

        XalanOutputTranscoder* const    theTranscoder = 
            XalanTranscodingServices::makeNewTranscoder(
                        XalanMemMgrs::getDefaultXercesMemMgr(),
                        XalanDOMString(theStringEncoding, XalanMemMgrs::getDefaultXercesMemMgr()),
                        theCode,
                        1024);

        if (theCode == XalanTranscodingServices::UnsupportedEncoding)
        {
            theResult = XALAN_XPATH_API_ERROR_UNSUPPORTED_ENCODING;
        }
        else if (theCode != XalanTranscodingServices::OK)
        {
            theResult = XALAN_XPATH_API_ERROR_UNKNOWN;
        }
        else
        {
            assert(theTranscoder != 0);

            // Since UTF-16 can represent any Unicode value in
            // one 16-bit value, we'll use the length of the
            // string as the maximum length of the resulting
            // transcoded string.  This doesn't account for
            // surrogate pairs, but those are for private use
            // only right now, so we don't really need to
            // worry about them.
            const XalanDOMString::size_type     theLength = XalanDOMString::length(theString);

            // Only use a dynamically-allocated array for very long
            // XPath expressions.
            const XalanDOMString::size_type     maxStackArraySize = 100;

            if (theLength >= maxStackArraySize)
            {
                XALAN_USING_XALAN(XalanArrayAutoPtr)

                XalanArrayAutoPtr<unsigned char>    theCharsCount(new unsigned char[theLength + 1]);
                XalanArrayAutoPtr<XalanDOMChar>     theChars(new XalanDOMChar[theLength + 1]);

                theResult = transcodeString(
                                theTranscoder,
                                theString,
                                theChars.get(),
                                theCharsCount.get(),
                                theLength,
                                theResultString);
            }
            else
            {
                unsigned char   theCharsCount[maxStackArraySize];
                XalanDOMChar    theChars[maxStackArraySize];

                theResult = transcodeString(
                                theTranscoder,
                                theString,
                                theChars,
                                theCharsCount,
                                theLength,
                                theResultString);
            }

            XalanTranscodingServices::destroyTranscoder(theTranscoder);
        }
    }

    return theResult;
}



XALAN_XPATHCAPI_EXPORT_FUNCTION(int)
XalanCreateXPath(
            XalanXPathEvaluatorHandle   theXalanHandle,
            const char*                 theXPathExpression,
            const char*                 theXPathExpressionEncoding,
            XalanXPathHandle*           theXPathHandle)
{
    if (fInitialized == false)
    {
        return XALAN_XPATH_API_ERROR_NOT_INITIALIZED;
    }
    else if (fTerminated == true)
    {
        return XALAN_XPATH_API_ERROR_ALREADY_TERMINATED;
    }
    else if (theXalanHandle == 0 || theXPathHandle == 0 || theXPathExpression == 0 || XalanDOMString::length(theXPathExpression) == 0)
    {
        return XALAN_XPATH_API_ERROR_INVALID_PARAMETER;
    }
    else
    {
        int     theResult = XALAN_XPATH_API_SUCCESS;

        try
        {
            XPathEvaluator* const   theEvaluator = getEvaluator(theXalanHandle);
            assert(theEvaluator != 0);

            XalanDOMString  theExpressionString(XalanMemMgrs::getDefaultXercesMemMgr());

            theResult = transcodeString(
                theXPathExpression,
                theXPathExpressionEncoding,
                theExpressionString);

            if (theResult == XALAN_XPATH_API_SUCCESS)
            {
                const XalanDOMChar* const   thePointer = theExpressionString.c_str();

                XALAN_USING_XALAN(length)

                if (length(thePointer) == 0)
                {
                    theResult = XALAN_XPATH_API_ERROR_TRANSCODING;
                }
                else
                {
                    *theXPathHandle = theEvaluator->createXPath(thePointer);
                }
            }
        }
        catch(...)
        {
            theResult = XALAN_XPATH_API_ERROR_INVALID_XPATH;
        }

        return theResult;
    }
}



XALAN_XPATHCAPI_EXPORT_FUNCTION(int)
XalanDestroyXPath(
            XalanXPathEvaluatorHandle   theXalanHandle,
            XalanXPathHandle            theXPathHandle)
{
    if (fInitialized == false)
    {
        return XALAN_XPATH_API_ERROR_NOT_INITIALIZED;
    }
    else if (fTerminated == true)
    {
        return XALAN_XPATH_API_ERROR_ALREADY_TERMINATED;
    }
    else if (theXalanHandle == 0 || theXPathHandle == 0)
    {
        return XALAN_XPATH_API_ERROR_INVALID_PARAMETER;
    }
    else
    {
        int theResult = XALAN_XPATH_API_SUCCESS;

        try
        {
            XPathEvaluator* const   theEvaluator = getEvaluator(theXalanHandle);
            assert(theEvaluator != 0);

            if (theEvaluator->destroyXPath(getXPath(theXPathHandle)) == false)
            {
                theResult = XALAN_XPATH_API_ERROR_INVALID_XPATH;
            }
        }
        catch(...)
        {
            theResult = XALAN_XPATH_API_ERROR_UNKNOWN;
        }

        return theResult;
    }
}



XALAN_XPATHCAPI_EXPORT_FUNCTION(int)
XalanEvaluateXPathAsBoolean(
            XalanXPathEvaluatorHandle   theXalanHandle,
            XalanXPathHandle            theXPathHandle,
            const char*                 theXML,
            int*                        theResult)
{
    if (fInitialized == false)
    {
        return XALAN_XPATH_API_ERROR_NOT_INITIALIZED;
    }
    else if (fTerminated == true)
    {
        return XALAN_XPATH_API_ERROR_ALREADY_TERMINATED;
    }
    else if (theXalanHandle == 0 || theXPathHandle == 0 || theXML == 0)
    {
        return XALAN_XPATH_API_ERROR_INVALID_PARAMETER;
    }
    else
    {
        int theError = XALAN_XPATH_API_SUCCESS;

        XALAN_USING_XERCES(SAXException)

        try
        {
            XPathEvaluator* const   theEvaluator = getEvaluator(theXalanHandle);
            assert(theEvaluator != 0);

            XPath* const    theXPath = getXPath(theXPathHandle);
            assert(theXPath != 0);

            XALAN_USING_XALAN(XalanDocument)
            XALAN_USING_XALAN(XalanSourceTreeDOMSupport)
            XALAN_USING_XALAN(XalanSourceTreeParserLiaison)

            XalanSourceTreeDOMSupport       theDOMSupport;
            XalanSourceTreeParserLiaison    theLiaison(theDOMSupport, XalanMemMgrs::getDefaultXercesMemMgr());

            // Hook the two together...
            theDOMSupport.setParserLiaison(&theLiaison);

            XALAN_USING_XERCES(MemBufInputSource)

            // Create an input source...
            const MemBufInputSource     theInputSource(
                                            reinterpret_cast<const XMLByte*>(theXML),
                                            XalanDOMString::length(theXML),
                                            "SourceXML",
                                            false);

            // Parse the document...
            XalanDocument* const    theDocument =
                            theLiaison.parseXMLStream(theInputSource);
            assert(theDocument != 0);

            if (theEvaluator->evaluate(
                    theDOMSupport,
                    theDocument,
                    *theXPath)->boolean(theEvaluator->getExecutionContext()) == true)
            {
                *theResult = 1;
            }
            else
            {
                *theResult = 0;
            }
        }
        catch(const SAXException&)
        {
            theError = XALAN_XPATH_API_ERROR_BAD_XML;
        }
        catch(...)
        {
            theError = XALAN_XPATH_API_ERROR_UNKNOWN;
        }

        return theError;
    }
}



XALAN_XPATHCAPI_EXPORT_FUNCTION(int)
XalanEvaluateXPathExpressionAsBoolean(
            XalanXPathEvaluatorHandle   theXalanHandle,
            const char*                 theXPathExpression,
            const char*                 theXPathExpressionEncoding,
            const char*                 theXML,
            int*                        theResult)
{
    XalanXPathHandle    theXPathHandle = 0;

    int     theError = XalanCreateXPath(
                    theXalanHandle,
                    theXPathExpression,
                    theXPathExpressionEncoding,
                    &theXPathHandle);

    if (theError == XALAN_XPATH_API_SUCCESS)
    {
        assert(theXPathHandle != 0);

        theError = XalanEvaluateXPathAsBoolean(theXalanHandle, theXPathHandle, theXML, theResult);

        const int   theDestroyResult =
            XalanDestroyXPath(theXalanHandle, theXPathHandle);

        if (theDestroyResult != XALAN_XPATH_API_SUCCESS)
        {
            theError = theDestroyResult;
        }
    }

    return theError;
}
