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
#include "FunctionTranslate.hpp"



#include <xalanc/Include/XalanVector.hpp>


#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include "XObjectFactory.hpp"



XALAN_CPP_NAMESPACE_BEGIN



FunctionTranslate::FunctionTranslate()
{
}



FunctionTranslate::~FunctionTranslate()
{
}



XObjectPtr
FunctionTranslate::execute(
            XPathExecutionContext&  executionContext,
            XalanNode*              /* context */,          
            const XObjectPtr        arg1,
            const XObjectPtr        arg2,
            const XObjectPtr        arg3,
            const Locator* const    /* locator */) const
{
    assert(arg1.null() == false && arg2.null() == false && arg3.null() == false);

    const XalanDOMString&   theFirstString = arg1->str(executionContext);
    const XalanDOMString&   theSecondString = arg2->str(executionContext);
    const XalanDOMString&   theThirdString = arg3->str(executionContext);

    const XalanDOMString::size_type     theFirstStringLength = theFirstString.length();
    const XalanDOMString::size_type     theSecondStringLength = theSecondString.length();
    const XalanDOMString::size_type     theThirdStringLength = theThirdString.length();

    typedef XalanVector<XalanDOMChar>       VectorType;

    // A string to hold the result.
    GetCachedString     theResult(executionContext);

    XalanDOMString&     theString = theResult.get();

    // The result string can only be as large as the first string, so
    // just reserve the space now.  Also reserve space for the
    // terminating 0.
    theString.reserve(theFirstStringLength + 1);

    for (XalanDOMString::size_type i = 0; i < theFirstStringLength; i++)
    {
        const XalanDOMChar                  theCurrentChar = theFirstString[i];

        const XalanDOMString::size_type     theIndex = indexOf(theSecondString, theCurrentChar);

        if (theIndex >= theSecondStringLength)
        {
            // Didn't find the character in the second string, so it
            // is not translated.
            theString.append(1, theCurrentChar);
        }
        else if (theIndex < theThirdStringLength)
        {
            // OK, there's a corresponding character in the
            // third string, so do the translation...
            theString.append(1, theThirdString[theIndex]);
        }
        else
        {
            // There's no corresponding character in the
            // third string, since it's shorter than the
            // second string.  In this case, the character
            // is removed from the output string, so don't
            // do anything.
        }
    }

    return executionContext.getXObjectFactory().createString(theResult);
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
FunctionTranslate*
#endif
FunctionTranslate::clone(MemoryManager&     theManager) const
{
    return XalanCopyConstruct(theManager, *this);
}



const XalanDOMString&
FunctionTranslate::getError(XalanDOMString&     theResult) const
{
    return XalanMessageLoader::getMessage(
                theResult,
                XalanMessages::FunctionTakesThreeArguments_1Param,
                "translate()");
}



XALAN_CPP_NAMESPACE_END
