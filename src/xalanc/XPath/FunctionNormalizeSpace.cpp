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

#include "FunctionNormalizeSpace.hpp"



#include "xalanc/XalanDOM/XalanDOMString.hpp"



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "XObjectFactory.hpp"



XALAN_CPP_NAMESPACE_BEGIN



FunctionNormalizeSpace::FunctionNormalizeSpace() :
    Function()
{
}



FunctionNormalizeSpace::~FunctionNormalizeSpace()
{
}



static const XalanDOMString     s_emptyString(XalanMemMgrs::getDummyMemMgr());



XObjectPtr
FunctionNormalizeSpace::execute(
            XPathExecutionContext&  executionContext,
            XalanNode*              context,
            const Locator*          locator) const
{
    if (context == 0)
    {
        const GetCachedString   theGuard(executionContext);

        XalanDOMString&         theResult = theGuard.get();

        executionContext.problem(
            XPathExecutionContext::eXPath,
            XPathExecutionContext::eError,
            XalanMessageLoader::getMessage(
                theResult,
                XalanMessages::FunctionRequiresNonNullContextNode_1Param,
                "normalize-space()"),
            locator,
            context);

        // Dummy return value...
        return XObjectPtr(0);
    }
    else
    {
        // The XPath standard says that if there are no arguments,
        // the default is to turn the context node into a string value.
        // DOMServices::getNodeData() will give us the data.

        // Get a cached string...
        const GetCachedString    theData(executionContext);

        XalanDOMString&     theString = theData.get();

        DOMServices::getNodeData(*context, executionContext, theString);

        return normalize(executionContext, theString);
    }
}



XObjectPtr
FunctionNormalizeSpace::execute(
            XPathExecutionContext&  executionContext,
            XalanNode*              /* context */,
            const XObjectPtr        arg1,
            const Locator* const    /* locator */) const
{
    assert(arg1.null() == false);   
        
    return normalize(executionContext, arg1);
}



XObjectPtr
FunctionNormalizeSpace::normalize(
            XPathExecutionContext&  executionContext,
            const XalanDOMString&   theString,
            GetCachedString&        theResult) const
{
    const XalanDOMString::size_type     theStringLength =
        theString.length();

    XalanDOMString&     theNewString = theResult.get();
    assert(theNewString.length() == 0);

    // The result string can only be as large as the source string, so
    // just reserve the space now.
    theNewString.reserve(theStringLength);

    enum  eLastCharState { eNonSpace, eSpace, eSpaceAppended };

    eLastCharState  theState = eSpace;

    // OK, strip out any multiple spaces...
    for (XalanDOMString::const_iterator i = theString.begin();
            i != theString.end(); ++i)
    {
        const XalanDOMChar  theCurrentChar = *i;

        if (isXMLWhitespace(theCurrentChar) == false)
        {
            theNewString.push_back(theCurrentChar);

            theState = eNonSpace;
        }
        else if (theState == eNonSpace)
        {
            theNewString.push_back(XalanDOMChar(XalanUnicode::charSpace));

            theState = eSpaceAppended;
        }
    }

    if (theNewString.empty() == true)
    {
        return executionContext.getXObjectFactory().createStringReference(s_emptyString);
    }
    else
    {
        // We may have a space character at end, since we don't look ahead,
        // so removed it now...
        if (theState == eSpaceAppended)
        {
            theNewString.erase(theNewString.end() - 1);
        }

        return executionContext.getXObjectFactory().createString(theResult);
    }
}



XObjectPtr
FunctionNormalizeSpace::normalize(
            XPathExecutionContext&  executionContext,
            const XObjectPtr&       theArg) const
{
    const XalanDOMString&   theString = theArg->str(executionContext);

    if (needsNormalization(theString) == false)
    {
        if (theArg->getType() == XObject::eTypeString)
        {
            return theArg;
        }
        else
        {
            return executionContext.getXObjectFactory().createStringAdapter(theArg, executionContext);
        }
    }
    else
    {
        return normalize(executionContext, theString);
    }
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
FunctionNormalizeSpace*
#endif
FunctionNormalizeSpace::clone(MemoryManager& theManager) const
{
    return XalanCopyConstruct(theManager, *this);
}



const XalanDOMString&
FunctionNormalizeSpace::getError(XalanDOMString& theResult) const
{
    
    return XalanMessageLoader::getMessage(
                theResult,
                XalanMessages::FunctionTakesZeroOrOneArg_1Param,
                "normalize-space()");
}



bool
FunctionNormalizeSpace::needsNormalization(const XalanDOMString&    theString) const
{
    const XalanDOMString::size_type     theStringLength = theString.length();

    bool    fNormalize = false;

    bool    fPreviousIsSpace = false;

    // OK, search for multiple spaces, or whitespace that is not the
    // space character...
    for (XalanDOMString::size_type i = 0; i < theStringLength && fNormalize == false; ++i)
    {
        const XalanDOMChar  theCurrentChar = theString[i];

        if (isXMLWhitespace(theCurrentChar) == false)
        {
            fPreviousIsSpace = false;
        }
        else
        {
            if (i == 0 ||
                i == theStringLength - 1 ||
                theCurrentChar != XalanDOMChar(XalanUnicode::charSpace) ||
                fPreviousIsSpace == true)
            {
                fNormalize = true;
            }
            else
            {
                fPreviousIsSpace = true;
            }
        }
    }

    return fNormalize;
}



XALAN_CPP_NAMESPACE_END
