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
#include "DoubleSupport.hpp"



#include <clocale>
#include <cmath>
#include <climits>
#if !defined(XALAN_NO_STD_NUMERIC_LIMITS)
#include <limits>
#endif


#include "DOMStringHelper.hpp"
#include "XalanUnicode.hpp"



XALAN_CPP_NAMESPACE_BEGIN



#if defined(XALAN_NO_STD_NUMERIC_LIMITS)
DoubleSupport::NumberUnion          DoubleSupport::s_NaN;
#else
const DoubleSupport::NumberUnion    DoubleSupport::s_NaN =
{
#if defined(XALAN_NO_STD_NAMESPACE)
    numeric_limits<double>::quiet_NaN()
#else
    std::numeric_limits<double>::quiet_NaN()
#endif
};
#endif

const DoubleSupport::NumberUnion    DoubleSupport::s_positiveInfinity = { HUGE_VAL };

const DoubleSupport::NumberUnion    DoubleSupport::s_negativeInfinity = { -HUGE_VAL };

const DoubleSupport::NumberUnion    DoubleSupport::s_positiveZero = { 0.0 };

const DoubleSupport::NumberUnion    DoubleSupport::s_negativeZero = { -s_positiveZero.d };



void
DoubleSupport::initialize()
{
#if defined(XALAN_NO_STD_NUMERIC_LIMITS)
    // We initialize this at here because some
    // platforms have had issues with signals
    // if we call sqrt(-2.01) during static
    // initialization.
#if defined(XALAN_STRICT_ANSI_HEADERS)
    s_NaN.d = std::sqrt(-2.01);
#else
    s_NaN.d = sqrt(-2.01);
#endif
#elif !defined(XALAN_NO_STD_NAMESPACE)
    // There seems to be problems with various standard libraries, so
    // this is disabled for now.  We need to revisit this when we
    // update our autoconf/automake system to detect the right value
    // for NaN at configuration time.
    // XALAN_STATIC_ASSERT(std::numeric_limits<double>::has_quiet_NaN);
#endif
}



void
DoubleSupport::terminate()
{
#if defined(XALAN_NO_STD_NUMERIC_LIMITS)
    s_NaN.d = 0.0L;
#endif
}



bool
DoubleSupport::equal(
            double  theLHS,
            double  theRHS)
{
    if (isNaN(theLHS) == true || isNaN(theRHS) == true)
    {
        return false;
    }
    else
    {
        return theLHS == theRHS;
    }
}



bool
DoubleSupport::lessThan(
            double  theLHS,
            double  theRHS)
{
    if (isNaN(theLHS) == true || isNaN(theRHS) == true)
    {
        return false;
    }
    else
    {
        return theLHS < theRHS;
    }
}



bool
DoubleSupport::lessThanOrEqual(
            double  theLHS,
            double  theRHS)
{
    if (isNaN(theLHS) == true || isNaN(theRHS) == true)
    {
        return false;
    }
    else
    {
        return theLHS <= theRHS;
    }
}



bool
DoubleSupport::greaterThan(
            double  theLHS,
            double  theRHS)
{
    if (isNaN(theLHS) == true || isNaN(theRHS) == true)
    {
        return false;
    }
    else
    {
        return theLHS > theRHS;
    }
}



bool
DoubleSupport::greaterThanOrEqual(
            double  theLHS,
            double  theRHS)
{
    if (isNaN(theLHS) == true || isNaN(theRHS) == true)
    {
        return false;
    }
    else
    {
        return theLHS >= theRHS;
    }
}



double
DoubleSupport::add(
            double  theLHS,
            double  theRHS)
{
    if (isNaN(theLHS) == true)
    {
        return theLHS;
    }
    else if (isNaN(theRHS) == true)
    {
        return theRHS;
    }
    else
    {
        return theLHS + theRHS;
    }
}



double
DoubleSupport::subtract(
            double  theLHS,
            double  theRHS)
{
    if (isNaN(theLHS) == true)
    {
        return theLHS;
    }
    else if (isNaN(theRHS) == true)
    {
        return theRHS;
    }
    else
    {
        return theLHS - theRHS;
    }
}



double
DoubleSupport::multiply(
            double  theLHS,
            double  theRHS)
{
    if (isNaN(theLHS) == true)
    {
        return theLHS;
    }
    else if (isNaN(theRHS) == true)
    {
        return theRHS;
    }
    else
    {
        return theLHS * theRHS;
    }
}



double
DoubleSupport::divide(
            double  theLHS,
            double  theRHS)
{
    if (isNaN(theLHS) == true)
    {
        return theLHS;
    }
    else if (isNaN(theRHS) == true)
    {
        return theRHS;
    }
    else if (theRHS != 0.0L)
    {
        return theLHS / theRHS;
    }
    else if (theLHS == 0.0L)
    {
        // This is NaN...
        return getNaN();
    }
    else if (theLHS > 0.0L && isPositiveZero(theRHS) == true)
    {
        // This is positive infinity...
        return getPositiveInfinity();
    }
    else
    {
        // This is negative infinity...
        return getNegativeInfinity();
    }
}



double
DoubleSupport::modulus(
            double  theLHS,
            double  theRHS)
{
    if (isNaN(theLHS) == true)
    {
        return theLHS;
    }
    else if (isNaN(theRHS) == true)
    {
        return theRHS;
    }
    else if (theRHS == 0.0)
    {
        return getNaN();
    }
    else if (long(theLHS) == theLHS && long(theRHS) == theRHS)
    {
        return long(theLHS) % long(theRHS);
    }
    else
    {
        double  theDummy;

        double  theResult = divide(theLHS, theRHS);

#if defined(XALAN_STRICT_ANSI_HEADERS)
        return std::modf(theResult, &theDummy) * theRHS;
#else
        return modf(theResult, &theDummy) * theRHS;
#endif
    }
}



double
DoubleSupport::negative(double  theDouble)
{
    if (isNaN(theDouble) == true)
    {
        return getNaN();
    }
    else
    {
        return -theDouble;
    }
}



double
DoubleSupport::abs(double theDouble)
{
    if (isNaN(theDouble) == true)
    {
        return getNaN();
    }
    else
    {
#if defined(XALAN_STRICT_ANSI_HEADERS)
    using std::fabs;
#endif
        return fabs(theDouble);
    }
}



double
DoubleSupport::toDouble(
            const XalanDOMString&   theString,
            MemoryManager&          theManager)
{
    return toDouble(theString.c_str(), theManager);
}



inline void
consumeWhitespace(const XalanDOMChar*&  theString)
{
    while(*theString != 0 &&
          isXMLWhitespace(*theString))
    {
        ++theString;
    }
}



inline void
consumeWhitespace(
            const XalanDOMChar*&        theString,
            XalanDOMString::size_type&  theLength)
{
    while(*theString != 0 &&
          isXMLWhitespace(*theString))
    {
        ++theString;
        --theLength;
    }
}



inline static void
consumeNumbers(const XalanDOMChar*& theString)
{
    while(*theString &&
          *theString >= XalanUnicode::charDigit_0 &&
          *theString <= XalanUnicode::charDigit_9)
    {
            ++theString;
    }
}



static bool
doValidate(
            const XalanDOMChar*     theString,
            bool&                   fGotDecimalPoint)
{
    assert(theString != 0);

    bool    fError = false;
    bool    fGotDigit = false;
    bool    fGotMinus = false;
    bool    fGotWhitespace = false;

    const XalanDOMChar*     theCurrent = theString;

    // trim any whitespace
    consumeWhitespace(theCurrent);

    while(*theCurrent != 0 && fError == false)
    {
        switch(*theCurrent)
        {
        case XalanUnicode::charFullStop:
            if (fGotDecimalPoint == true || // can't have more than one...
                fGotWhitespace == true) // can't have one after whitespace...
            {
                fError = true;
            }
            else
            {
                fGotDecimalPoint = true;

                ++theCurrent;
            }
            break;

        case XalanUnicode::charHyphenMinus:
            if (fGotDecimalPoint == true ||
                fGotMinus == true ||
                fGotDigit == true ||
                fGotWhitespace == true)
            {
                // Error -- more than one, or in bad position.
                fError = true;
            }
            else
            {
                fGotMinus = true;

                ++theCurrent;
            }
            break;

        case XalanUnicode::charDigit_0:
        case XalanUnicode::charDigit_1:
        case XalanUnicode::charDigit_2:
        case XalanUnicode::charDigit_3:
        case XalanUnicode::charDigit_4:
        case XalanUnicode::charDigit_5:
        case XalanUnicode::charDigit_6:
        case XalanUnicode::charDigit_7:
        case XalanUnicode::charDigit_8:
        case XalanUnicode::charDigit_9:
            if (fGotWhitespace == true)
            {
                fError = true;
            }
            else
            {
                fGotDigit = true;

                consumeNumbers(theCurrent);
            }
            break;

        case XalanUnicode::charSpace:
        case XalanUnicode::charCR:
        case XalanUnicode::charHTab:
        case XalanUnicode::charLF:
            if (fGotWhitespace == true)
            {
                fError = true;
            }
            else
            {
                fGotWhitespace = true;

                consumeWhitespace(theCurrent);
            }
            break;

        default:
            fError = true;
            break;
        }
    }

    // If there was no error, check to see that we got
    // at least one digit.  Otherwise, return false if
    // there was an error.
    return fError == false ? fGotDigit : false;
}



static bool
doValidate(const XalanDOMChar*  theString)
{
    bool    fDummy = false;

    return doValidate(theString, fDummy);
}



#if defined(XALAN_NON_ASCII_PLATFORM)
static void
translateWideString(
            const XalanDOMChar*         theWideString,
            char*                       theNarrowString,
            XalanDOMString::size_type   theStringLength,
            char                        theDecimalPointCharacter)
{
    for(XalanDOMString::size_type i = 0; i < theStringLength; ++i)
    {
        switch(theWideString[i])
        {
        case XalanUnicode::charHyphenMinus:
            theNarrowString[i] = '-';
            break;

        case XalanUnicode::charFullStop:
            theNarrowString[i] = theDecimalPointCharacter;
            break;

        case XalanUnicode::charDigit_0:
            theNarrowString[i] = '0';
            break;

        case XalanUnicode::charDigit_1:
            theNarrowString[i] = '1';
            break;

        case XalanUnicode::charDigit_2:
            theNarrowString[i] = '2';
            break;

        case XalanUnicode::charDigit_3:
            theNarrowString[i] = '3';
            break;

        case XalanUnicode::charDigit_4:
            theNarrowString[i] = '4';
            break;

        case XalanUnicode::charDigit_5:
            theNarrowString[i] = '5';
            break;

        case XalanUnicode::charDigit_6:
            theNarrowString[i] = '6';
            break;

        case XalanUnicode::charDigit_7:
            theNarrowString[i] = '7';
            break;

        case XalanUnicode::charDigit_8:
            theNarrowString[i] = '8';
            break;

        case XalanUnicode::charDigit_9:
            theNarrowString[i] = '9';
            break;

        default:
            theNarrowString[i] = char(0);
            break;
        }
    }
    
    theNarrowString[theStringLength] = char(0);
}
#endif



inline double
convertHelper(
            const XalanDOMChar*     theString,
            bool                    fGotDecimalPoint,
            MemoryManager&          theManager)
{
    // This is a big hack.  If the length of the
    // string is less than n characters, we'll convert
    // it as a long and coerce that to a double.  This
    // is _much_ cheaper...
    const XalanDOMString::size_type     theLongHackThreshold = 10;

    XalanDOMString::size_type   theLength = length(theString);

    if (fGotDecimalPoint == false && theLength < theLongHackThreshold)
    {
        return double(WideStringToLong(theString));
    }
    else
    {
#if defined(XALAN_STRICT_ANSI_HEADERS)
        XALAN_USING_STD(localeconv)
        XALAN_USING_STD(atof)
#endif

        const char  theDecimalPointChar =
            localeconv()->decimal_point[0];

        // trim any whitespace
        consumeWhitespace(theString, theLength);

        // Use a stack-based buffer, when possible...
        const XalanDOMString::size_type     theBufferSize = 200u;

        if (theLength < theBufferSize)
        {
            char    theBuffer[theBufferSize];

#if defined(XALAN_NON_ASCII_PLATFORM)
            translateWideString(
                theString,
                theBuffer,
                theLength,
                theDecimalPointChar);
#else
            for(XalanDOMString::size_type i = 0; i < theLength; ++i)
            {
                if (theString[i] == XalanUnicode::charFullStop)
                {
                    theBuffer[i] = theDecimalPointChar;
                }
                else
                {
                    theBuffer[i] = char(theString[i]);
                }
            }

            theBuffer[theLength] = '\0';
#endif

            return atof(theBuffer);
        }
        else
        {
            CharVectorType  theVector(theManager);

#if !defined(XALAN_NON_ASCII_PLATFORM)
            theVector.reserve(theLength + 1);

            CopyWideStringToVector(theString, theVector);
#else
            theVector.resize(
                theLength + 1,
                CharVectorType::value_type(0));

            translateWideString(
                theString,
                &*theVector.begin(),
                theLength,
                theDecimalPointChar);
#endif

            return atof(&*theVector.begin());
        }
    }
}



inline double
doConvert(
            const XalanDOMChar*     theString,
            MemoryManager&          theManager)
{
    assert(theString != 0);
    assert(*theString != 0);

    bool    fGotDecimalPoint = false;

    if (doValidate(theString, fGotDecimalPoint) == false)
    {
        return DoubleSupport::getNaN();
    }
    else
    {
        return convertHelper(
                    theString,
                    fGotDecimalPoint,
                    theManager);
    }
}



double
DoubleSupport::toDouble(
            const XalanDOMChar*     theString,
            MemoryManager&          theManager)
{
    if (theString == 0 ||
        *theString == 0)
    {
        return getNaN();
    }
    else
    {
        return doConvert(theString, theManager);
    }
}



bool
DoubleSupport::isValid(const XalanDOMString&    theString)
{
    return isValid(theString.c_str());
}



bool
DoubleSupport::isValid(const XalanDOMChar*      theString)
{
    return doValidate(theString);
}



inline double
modfRound(double  theValue)
{
    double          intPart = 0;

#if defined(XALAN_STRICT_ANSI_HEADERS)
        std::modf(theValue + 0.5, &intPart);
#else
        modf(theValue + 0.5, &intPart);
#endif

    return intPart;
}



double
DoubleSupport::round(double     theValue)
{
    if (isNaN(theValue))
    {
        return getNaN();
    }
    else if (isPositiveInfinity(theValue))
    {
        return getPositiveInfinity();
    }
    else if (isNegativeInfinity(theValue))
    {
        return getNegativeInfinity();
    }
    else if (theValue == 0)
    {
        return 0.0;
    }
    else if (theValue > 0)
    {
        // If the value is less than the maximum value for
        // a long, this is the fastest way to do it.
        if (theValue < LONG_MAX)
        {
            return long(theValue + 0.5);
        }
        else
        {
            return modfRound(theValue);
        }
    }
    else
    {
        // Negative numbers are a special case.  Any time we
        // have -0.5 as the fractional part, we have to
        // round up (toward 0), rather than down.
        double          intPart = 0;

        const double    fracPart = 
#if defined(XALAN_STRICT_ANSI_HEADERS)
            std::modf(theValue, &intPart);
#else
            modf(theValue, &intPart);
#endif

        const double    theAdjustedValue =
            fracPart == -0.5 ? theValue + 0.5 : theValue - 0.5;

        // If the value is greater than the minimum value for
        // a long, this is the fastest way to do it.
        if (theAdjustedValue > LONG_MIN)
        {
            return long(theAdjustedValue);
        }
        else
        {
            return modfRound(theAdjustedValue);
        }
    }
}



XALAN_CPP_NAMESPACE_END
