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
#include "FunctionSubstring.hpp"



#include <xalanc/PlatformSupport/DoubleSupport.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include "XObjectFactory.hpp"



XALAN_CPP_NAMESPACE_BEGIN



const XObjectPtr        FunctionSubstring::s_nullXObjectPtr;



FunctionSubstring::FunctionSubstring()
{
}



FunctionSubstring::~FunctionSubstring()
{
}



/*
 * Get the value for the start index (C-style, not XPath).  This
 * function expects theSecondArgValue to be already rounded.
 */
inline XalanDOMString::size_type
getStartIndex(
            double                      theSecondArgValue,
            XalanDOMString::size_type   theStringLength)
{
    // We always subtract 1 for C-style index, since
    // XPath indexes from 1.  

    // If we end up with NaN, INF, or -INF, then no possible index
    // can be greater than or equal to that, so just return
    // the start index as the length of the string.  That
    // will result in an empty string, which is what we want.
    if (DoubleSupport::isNaN(theSecondArgValue) == true ||
        DoubleSupport::isPositiveInfinity(theSecondArgValue) == true ||
        DoubleSupport::isNegativeInfinity(theSecondArgValue) == true)
    {
        return theStringLength;
    }
    // Anything less than, or equal to 1 is 0.
    else if (DoubleSupport::lessThanOrEqual(theSecondArgValue, 1) == true)
    {
        assert(DoubleSupport::round(theSecondArgValue) == theSecondArgValue);

        return 0;
    }
    else
    {
        assert(DoubleSupport::round(theSecondArgValue) == theSecondArgValue);

        // Since we have filtered out anything less than
        // 1, and any special values, we can do this without
        // calling DoubleSupport::subtract().
        const double    theResult = theSecondArgValue - 1;

        assert(
            DoubleSupport::equal(
                DoubleSupport::subtract(
                    theSecondArgValue,
                    1),
                theResult));

        return XalanDOMString::size_type(theResult);
    }
}



/*
 * Get the length of the substring.
 */
inline XalanDOMString::size_type
getSubstringLength(
            XPathExecutionContext&      executionContext,
            XalanDOMString::size_type   theSourceStringLength,
            XalanDOMString::size_type   theStartIndex,
            double                      theSecondArgValue,
            const XObjectPtr&           arg3)
{
    assert(theStartIndex < theSourceStringLength);
    assert(DoubleSupport::isNaN(theSecondArgValue) == false);
    assert(DoubleSupport::isNegativeInfinity(theSecondArgValue) == false);
    assert(DoubleSupport::isPositiveInfinity(theSecondArgValue) == false);

    typedef XalanDOMString::size_type   size_type;

    const size_type     theMaxLength =
        theSourceStringLength - theStartIndex;

    // Total the second and third arguments.  If the third argument is
    // missing, make it the length of the string + 1 (for XPath
    // indexing style).
    if (arg3.null() == true)
    {
        return theMaxLength;
    }
    else
    {
        const double    theThirdArgValue = arg3->num(executionContext);

        if (DoubleSupport::isNaN(theThirdArgValue) == true ||
            DoubleSupport::isNegativeInfinity(theThirdArgValue) == true)
        {
            return 0;
        }
        else if (DoubleSupport::isPositiveInfinity(theThirdArgValue) == true)
        {
            return theMaxLength;
        }
        else
        {
            const double    theRoundedValue =
                DoubleSupport::round(theThirdArgValue);
            assert(DoubleSupport::isNaN(theRoundedValue) == false);
            assert(DoubleSupport::isNegativeInfinity(theRoundedValue) == false);
            assert(DoubleSupport::isPositiveInfinity(theRoundedValue) == false);

            // The XPath recommendation states that following:
            //
            // http://www.w3.org/TR/xpath#function-substring
            //
            // "The returned substring contains those characters for which the
            // position of the character is greater than or equal to the rounded
            // value of the second argument and, if the third argument is specified,
            // less than the sum of the rounded value of the second argument and the
            // rounded value of the third argument; the comparisons and addition used
            // for the above follow the standard IEEE 754 rules; rounding is done as
            // if by a call to the round function."
            //
            // Note that this is indexing from 1.
            const double    theTotal =
                theRoundedValue + theSecondArgValue;

            const size_type     theXPathStartIndex =
                theStartIndex + 1;

            // If the total is less than or equal to
            // the starting index, or greater
            // than or equal to the starting index, the
            // substring is empty.
            if (theTotal <= theXPathStartIndex)
            {
                return 0;
            }
            else
            {
                const size_type     theSubstringLength =
                    size_type(theTotal) - theXPathStartIndex;

                return theSubstringLength > theMaxLength ? theMaxLength : theSubstringLength;
            }
        }
    }
}



static const XalanDOMString     theEmptyString(XalanMemMgrs::getDummyMemMgr());


inline XObjectPtr
createEmptyString(XPathExecutionContext&    executionContext)
{
    return executionContext.getXObjectFactory().createStringReference(theEmptyString);
}



XObjectPtr
FunctionSubstring::execute(
            XPathExecutionContext&  executionContext,
            XalanNode*              context,
            const XObjectPtr        arg1,
            const XObjectPtr        arg2,
            const Locator*          locator) const
{
    assert(arg1.null() == false && arg2.null() == false);

    return execute(executionContext, context, arg1, arg2, s_nullXObjectPtr, locator);
}



XObjectPtr
FunctionSubstring::execute(
            XPathExecutionContext&  executionContext,
            XalanNode*              /* context */,
            const XObjectPtr        arg1,
            const XObjectPtr        arg2,
            const XObjectPtr        arg3,
            const Locator* const    /* locator */) const
{
    assert(arg1.null() == false && arg2.null() == false);   

    const XalanDOMString&               theSourceString = arg1->str(executionContext);
    const XalanDOMString::size_type     theSourceStringLength = theSourceString.length();

    if (theSourceStringLength == 0)
    {
        return createEmptyString(executionContext);
    }
    else
    {
        // Get the value of the second argument...
        const double    theSecondArgValue =
            DoubleSupport::round(arg2->num(executionContext));

        // XPath indexes from 1, so this is the first XPath index....
        const XalanDOMString::size_type     theStartIndex =
            getStartIndex(theSecondArgValue, theSourceStringLength);

        if (theStartIndex >= theSourceStringLength)
        {
            return createEmptyString(executionContext);
        }
        else
        {
            assert(DoubleSupport::isNaN(theSecondArgValue) == false);
            assert(DoubleSupport::isPositiveInfinity(theSecondArgValue) == false);

            const XalanDOMString::size_type     theSubstringLength =
                getSubstringLength(
                    executionContext,
                    theSourceStringLength,
                    theStartIndex,
                    theSecondArgValue,
                    arg3);

            if (theSubstringLength == 0)
            {
                return createEmptyString(executionContext);
            }
            else
            {
                GetCachedString     theResult(executionContext);

                XalanDOMString&     theString = theResult.get();

                theString.assign(
                        theSourceString.c_str() + theStartIndex,
                        theSubstringLength);

                return executionContext.getXObjectFactory().createString(theResult);
            }
        }
    }
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
FunctionSubstring*
#endif
FunctionSubstring::clone(MemoryManager&     theManager) const
{
    return XalanCopyConstruct(theManager, *this);
}



const XalanDOMString&
FunctionSubstring::getError(XalanDOMString&     theResult) const
{
    return XalanMessageLoader::getMessage(
                theResult,
                XalanMessages::FunctionTakesTwoOrThreeArguments_1Param,
                "substring()");
}



XALAN_CPP_NAMESPACE_END
