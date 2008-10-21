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
#include "FunctionSubstringBefore.hpp"



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include "XObjectFactory.hpp"



XALAN_CPP_NAMESPACE_BEGIN



FunctionSubstringBefore::FunctionSubstringBefore()
{
}



FunctionSubstringBefore::~FunctionSubstringBefore()
{
}



static const XalanDOMString     theEmptyString(XalanMemMgrs::getDummyMemMgr());


inline XObjectPtr
createEmptyString(XPathExecutionContext&    executionContext)
{
    return executionContext.getXObjectFactory().createStringReference(theEmptyString);
}



XObjectPtr
FunctionSubstringBefore::execute(
            XPathExecutionContext&  executionContext,
            XalanNode*              /* context */,
            const XObjectPtr        arg1,
            const XObjectPtr        arg2,
            const Locator* const    /* locator */) const
{
    assert(arg1.null() == false && arg2.null() == false);

    const XalanDOMString&               theFirstString = arg1->str(executionContext);

    const XalanDOMString::size_type     theFirstStringLength = theFirstString.length();

    if (theFirstStringLength == 0)
    {
        return createEmptyString(executionContext);
    }
    else
    {
        const XalanDOMString&               theSecondString = arg2->str(executionContext);

        const XalanDOMString::size_type     theSecondStringLength = theSecondString.length();

        if (theSecondStringLength == 0)
        {
            return createEmptyString(executionContext);
        }
        else
        {
            const XalanDOMString::size_type     theIndex = indexOf(theFirstString,
                                                       theSecondString);

            if (theIndex == theFirstStringLength)
            {
                return createEmptyString(executionContext);
            }
            else
            {
                GetCachedString     theResult(executionContext);

                XalanDOMString&     theString = theResult.get();

                theString.assign(
                        theFirstString.c_str(),
                        theIndex);

                // Create a string of the appropriate length...
                return executionContext.getXObjectFactory().createString(theResult);
            }
        }
    }
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
FunctionSubstringBefore*
#endif
FunctionSubstringBefore::clone(MemoryManager&   theManager) const
{
    return XalanCopyConstruct(theManager, *this);
}



const XalanDOMString&
FunctionSubstringBefore::getError(XalanDOMString&   theResult) const
{
    return XalanMessageLoader::getMessage(
                theResult,
                XalanMessages::FunctionTakesTwoArguments_1Param,
                "substring-before()");
}



XALAN_CPP_NAMESPACE_END
