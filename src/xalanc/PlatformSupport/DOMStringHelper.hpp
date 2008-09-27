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
#if !defined(DOMSTRINGHELPER_HEADER_GUARD_1357924680)
#define DOMSTRINGHELPER_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



#include <algorithm>
#include <cassert>
#include <functional>
#if defined(XALAN_CLASSIC_IOSTREAMS)
class ostream;
#else
#include <iosfwd>
#endif



#include <xalanc/Include/XalanVector.hpp>
#include <xalanc/Include/XalanMap.hpp>
#include <xalanc/Include/XalanMemoryManagement.hpp>
#include <xalanc/Include/STLHelper.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/PlatformSupport/FormatterListener.hpp>
#include <xalanc/PlatformSupport/XalanUnicode.hpp>
#include <xalanc/PlatformSupport/XalanXMLChar.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XALAN_USING_XERCES(MemoryManager)



class XalanOutputStream;



template<class InputIteratorType, class OutputIteratorType>
inline OutputIteratorType
XalanCopy(
            InputIteratorType   begin,
            InputIteratorType   end,
            OutputIteratorType  iterator)
{
    return XALAN_STD_QUALIFIER copy(begin, end, iterator);
}



template<class InputIteratorType, class OutputIteratorType, class UnaryFunction>
inline OutputIteratorType
XalanTransform(
            InputIteratorType   begin,
            InputIteratorType   end,
            OutputIteratorType  iterator,
            UnaryFunction       function)
{
    return XALAN_STD_QUALIFIER transform(begin, end, iterator);
}



/**
 * Get the underlying representation of the target XalanDOMString as a
 * null-terminated string
 *
 * @deprecated This function is deprecated.
 *
 * @param theString target string
 * @return null-terminated string of XalanDOMChar
 */
inline const XalanDOMChar*
c_wstr(const XalanDOMString&    theString)
{
    return theString.c_str();
}



/**
 * Get the underlying representation of the target CharVectorType as a
 * null-terminated string
 * 
 * @param theString target string
 * @return null-terminated string of chars
 */
inline const char*
c_str(const CharVectorType&     theString)
{
    if (theString.empty() == true)
    {
        return 0;
    }
    else
    {
        const char* const   ptr = &theString[0];

        assert(ptr[theString.size() - 1] == '\0');

        return ptr;
    }
}



/**
 * Get the underlying representation of the wide string as a
 * UNICODE null-terminated string.  This is here simply for
 * consistency in the code.  On certain platforms, compiler-
 * generated wide strings will not contain Unicode code
 * points.  Another macro converts those into XalanDOMStrings,
 * which are then transcoded.  In these cases, the previous
 * defined c_sstr() function gets called.
 *
 * On platforms where the compiler does generate Unicode wide
 * strings, this function will be called instead.
 *
 * @deprecated This function is deprecated.
 *
 * @param theString target string
 * @return null-terminated string of XalanDOMChar
 */
inline const XalanDOMChar*
c_wstr(const XalanDOMChar*  theString)
{
    return theString;
}



/**
 * Get the underlying representation of the target XalanDOMString as an array of
 * XalanDOMChar, not guaranteed to be null-terminated.
 *
 * @deprecated This function is deprecated.
 *
 * @param theString target string
 * @return array of XalanDOMChar
 */
inline const XalanDOMChar*
toCharArray(const XalanDOMString&   theString)
{
    return theString.c_str();
}



/**
 * Get the underlying representation of a XalanDOMChar.
 *
 * @deprecated This function is deprecated.
 *
 * @param theString target string
 * @return array of XalanDOMChar
 */
inline const XalanDOMChar*
toCharArray(const XalanDOMChar*     theString)
{
    return theString;
}



/**
 * Get the underlying representation of the target CharVectorType as a
 * pointer to an array of characters
 *
 * @deprecated This function is deprecated.
 *
 * @param theString target string
 * @return the pointer
 */
inline const char*
toCharArray(const CharVectorType&   theString)
{
    return theString.empty() == true ? 0 : &theString[0];
}



/**
 * Reserve some space in the string for more efficient
 * concatenation...
 *
 * @deprecated This function is deprecated.
 *
 * @param theString target string
 * @param theCount The amount of space to reserve
 */
inline void
reserve(
            XalanDOMString&             theString,
            XalanDOMString::size_type   theCount)
{
    theString.reserve(theCount);
}



/**
 * Get the length of a XalanDOMString
 * 
 * @deprecated This function is deprecated.
 *
 * @param theString target string
 * @return the length of the target string
 */
inline XalanDOMString::size_type
length(const XalanDOMString&    theString)
{
    return theString.length();
}



/**
 * Get the length of a null-terminated string of
 * XalanDOMChar characters
 * 
 * @param theString target string
 * @return the length of the target string
 */
inline XalanDOMString::size_type
length(const XalanDOMChar*  theString)
{
    assert(theString != 0);

    const XalanDOMChar*     theBufferPointer = theString;

    while(*theBufferPointer != 0)
    {
        theBufferPointer++;
    }

    return XalanDOMString::size_type(theBufferPointer - theString);
}



/**
 * Get the length of a null-terminated string.
 * 
 * @param theString target string
 * @return the length of the target string
 */
inline XalanDOMString::size_type
length(const char*  theString)
{
    assert(theString != 0);

    return XalanDOMString::length(theString);
}



/**
 * Determines if the target string contains any elements
 * 
 * @deprecated This function is deprecated.
 *
 * @param str target string
 * @return true if the target string has a non-zero length
 */
inline bool 
isEmpty(const XalanDOMString&   str)
{
    return str.empty(); 
}



/**
 * Simulates the java String method indexOf().
 * 
 * @param theString string to search
 * @param theChar   character searched for
 * @return the index of theChar in theString,
 * or length(theString) if the character is not
 * found.    
 */
inline XalanDOMString::size_type
indexOf(
            const XalanDOMChar*     theString,
            XalanDOMChar            theChar)
{
    assert(theString != 0);

    const XalanDOMChar*     thePointer = theString;

    while(*thePointer != theChar && *thePointer != 0)
    {
        ++thePointer;
    }

    return XalanDOMString::size_type(thePointer - theString);
}



/**
 * Simulates the java String method indexOf().
 * 
 * @param theString string to search
 * @param theStringLength the length of theString
 * @param theChar   character searched for
 * @return the index of theChar in theString,
 * or length(theString) if the character is not
 * found.    
 */
inline XalanDOMString::size_type
indexOf(
            const XalanDOMChar*         theString,
            XalanDOMString::size_type   theStringLength,
            XalanDOMChar                theChar)
{
    assert(theString != 0);

    const XalanDOMChar*         thePointer = theString;
    const XalanDOMChar* const   theEndPointer = theString + theStringLength;

    while(*thePointer != theChar && thePointer != theEndPointer)
    {
        ++thePointer;
    }

    return XalanDOMString::size_type(thePointer - theString);
}



/**
 * Simulates the java String method indexOf().
 * 
 * @param theString string to search
 * @param theChar   character searched for
 * @return the index of theChar in theString,
 * or length(theString) if the character is not
 * found.    
 */
inline XalanDOMString::size_type
indexOf(
            const XalanDOMString&   theString,
            XalanDOMChar            theChar)
{
    return theString.length() == 0 ? 0 : indexOf(theString.c_str(), theChar);
}



/**
 * Simulates the java String method indexOf().
 * 
 * @param theString string to search
 * @param theStringLength length of the string to search
 * @param theSubstring substring searched for
 * @param theSubstringLength length of the substring searched for
 * @return the index of theSubstring in theString, 
 * or length(theString) if the string is not
 * found.
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString::size_type)
indexOf(
            const XalanDOMChar*         theString,
            XalanDOMString::size_type   theStringLength,
            const XalanDOMChar*         theSubstring,
            XalanDOMString::size_type   theSubstringLength);



/**
 * Simulates the java String method indexOf().
 * 
 * @param theString string to search
 * @param theSubstring substring searched for
 * @return the index of theSubstring in theString, 
 * or length(theString) if the string is not
 * found.
 */
inline XalanDOMString::size_type
indexOf(
            const XalanDOMChar*     theString,
            const XalanDOMChar*     theSubstring)
{
    assert(theString != 0 && theSubstring != 0);

    return indexOf(theString, length(theString), theSubstring, length(theSubstring));
}



/**
 * Simulates the java String method indexOf().
 * 
 * @param theString string to search
 * @param theSubstring substring searched for
 * @return the index of theSubstring in theString, 
 * or length(theString) if the string is not
 * found.
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString::size_type)
indexOf(
            const XalanDOMString&   theString,
            const XalanDOMString&   theSubstring);



/**
 * Simulates the java String method lastIndexOf().
 * 
 * @param theString string to search
 * @param theChar   character searched for
 * @return the index of theChar in theString,
 * or length(theString) if the character is not
 * found.
 */

XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString::size_type)
lastIndexOf(
            const XalanDOMChar*     theString,
            XalanDOMChar            theChar);



/**
 * Simulates the java String method lastIndexOf().
 * 
 * @param theString string to search
 * @param theChar   character searched for
 * @return the index of theChar in theString,
 * or length(theString) if the character is not
 * found.
 */
inline XalanDOMString::size_type
lastIndexOf(
            const XalanDOMString&   theString,
            XalanDOMChar            theChar)
{
    return lastIndexOf(theString.c_str(), theChar);
}



/**
 * Simulates the java String method startsWith().
 * 
 * @param theString target string to search
 * @param theStringLength the length of theString
 * @param theSubstring substring searched for
 * @param theSubstringLength the length of theSubstring
 * @return true if the target string begins with the substring
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
startsWith(
            const XalanDOMChar*         theString,
            XalanDOMString::size_type   theStringLength,
            const XalanDOMChar*         theSubstring,
            XalanDOMString::size_type   theSubstringLength);



/**
 * Simulates the java String method startsWith().
 * 
 * @param theDOMString target string to search
 * @param theSubstring substring searched for
 * @return true if the target string begins with the substring
 */
inline bool
startsWith(
            const XalanDOMChar*     theString,
            const XalanDOMChar*     theSubstring)
{
    assert(theString != 0 && theSubstring != 0);

    return startsWith(theString, length(theString), theSubstring, length(theSubstring));
}



/**
 * Simulates the java String method startsWith().
 * 
 * @param theDOMString target string to search
 * @param theSubstring substring searched for
 * @return true if the target string begins with the substring
 */
inline bool
startsWith(
            const XalanDOMChar*     theString,
            const XalanDOMString&   theSubstring)
{
    assert(theString != 0);

    return startsWith(
                theString,
                length(theString),
                theSubstring.c_str(),
                theSubstring.length());
}



/**
 * Simulates the java String method startsWith().
 * 
 * @param theDOMString target string to search
 * @param theSubstring substring searched for
 * @return true if the target string begins with the substring
 */
inline bool
startsWith(
            const XalanDOMString&   theString,
            const XalanDOMChar*     theSubstring)
{
    assert(theSubstring != 0);

    return startsWith(
                theString.c_str(),
                theString.length(),
                theSubstring,
                length(theSubstring));
}



/**
 * Simulates the java String method startsWith().
 * 
 * @param theDOMString target string to search
 * @param theSubstring substring searched for
 * @param theSubstringLength the length of theSubstring
 * @return true if the target string begins with the substring
 */
inline bool
startsWith(
            const XalanDOMString&       theString,
            const XalanDOMChar*         theSubstring,
            XalanDOMString::size_type   theSubstringLength)
{
    assert(theSubstring != 0);

    return startsWith(
                theString.c_str(),
                theString.length(),
                theSubstring,
                theSubstringLength);
}



/**
 * Simulates the java String method startsWith().
 * 
 * @param theDOMString target string to search
 * @param theSubstring substring searched for
 * @return true if the target string begins with the substring
 */
inline bool
startsWith(
            const XalanDOMString&   theString,
            const XalanDOMString&   theSubstring)
{
    return startsWith(
                theString.c_str(),
                theString.length(),
                theSubstring.c_str(),
                theSubstring.length());
}



/**
 * Simulates the java String method endsWith().
 * 
 * @param theString target string to search
 * @param theSubstring substring searched for
 * @return true if the target string ends with the substring
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
endsWith(
            const XalanDOMChar*         theString,
            XalanDOMString::size_type   theStringLength,
            const XalanDOMChar*         theSubstring,
            XalanDOMString::size_type   theSubstringLength);



/**
 * Simulates the java String method endsWith().
 * 
 * @param theString target string to search
 * @param theSubstring substring searched for
 * @return true if the target string ends with the substring
 */
inline bool
endsWith(
            const XalanDOMChar*     theString,
            const XalanDOMChar*     theSubstring)
{
    assert(theString != 0 && theSubstring != 0);

    return endsWith(
                theString,
                length(theString),
                theSubstring,
                length(theSubstring));
}



/**
 * Simulates the java String method endsWith().
 * 
 * @param theString target string to search
 * @param theSubstring substring searched for
 * @return true if the target string ends with the substring
 */
inline bool
endsWith(
            const XalanDOMString&   theString,
            const XalanDOMString&   theSubstring)
{
    return endsWith(
                theString.c_str(),
                theString.length(),
                theSubstring.c_str(),
                theSubstring.length());
}



/**
 * Converts a pointer into a XalanDOMString
 * 
 * @param theValue pointer to be converted
 * @param theResult the string to append with the result
 * @return a reference to the passed string result.
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString&)
PointerToDOMString(
            const void*         theValue,
            XalanDOMString&     theResult);



class XALAN_PLATFORMSUPPORT_EXPORT DOMStringHelper
{
public:

    typedef void (FormatterListener::*MemberFunctionPtr)(const XMLCh* const, const FormatterListener::size_type);


    static void
    initialize(MemoryManager&    theMemoryManager);

    static void
    terminate();

    static void
    NumberToCharacters(
            double              theValue,
            FormatterListener&  formatterListener,
            MemberFunctionPtr   function);

    static void
    NumberToCharacters(
            long                theValue,
            FormatterListener&  formatterListener,
            MemberFunctionPtr   function);

    static void
    NumberToCharacters(
            XALAN_INT64         theValue,
            FormatterListener&  formatterListener,
            MemberFunctionPtr   function);
};



/**
 * Converts a double value into a XalanDOMString
 * 
 * @param theValue number to be converted
 * @param theResult the string to append with the result
 * @return a reference to the passed string result.
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString&)
NumberToDOMString(
            double              theValue,
            XalanDOMString&     theResult);



/**
 * Converts an 64-bit unsigned value into a XalanDOMString
 * 
 * @param theValue number to be converted
 * @param theResult the string to append with the result
 * @return a reference to the passed string result.
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString&)
NumberToDOMString(
            XALAN_UINT64        theValue,
            XalanDOMString&     theResult);



/**
 * Converts an 64-bit signed value into a XalanDOMString
 * 
 * @param theValue number to be converted
 * @param theResult the string to append with the result
 * @return a reference to the passed string result.
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString&)
NumberToDOMString(
            XALAN_INT64         theValue,
            XalanDOMString&     theResult);



/**
 * Converts an unsigned long value into a XalanDOMString
 * 
 * @param theValue number to be converted
 * @param theResult the string to append with the result
 * @return a reference to the passed string result.
 */
inline XalanDOMString&
NumberToDOMString(
            unsigned long       theValue,
            XalanDOMString&     theResult)
{
    return NumberToDOMString(
                static_cast<XALAN_UINT64>(theValue),
                theResult);
}



/**
 * Converts a XalanSSize_t value into a XalanDOMString
 * 
 * @param theValue number to be converted
 * @param theResult the string to append with the result
 * @return a reference to the passed string result.
 */
inline XalanDOMString&
NumberToDOMString(
            long                theValue,
            XalanDOMString&     theResult)
{
    return NumberToDOMString(
                static_cast<XALAN_INT64>(theValue),
                theResult);
}



/**
 * Converts an unsigned int value into a XalanDOMString
 * 
 * @param theValue number to be converted
 * @param theResult the string to append with the result
 * @return a reference to the passed string result.
 */
inline XalanDOMString&
NumberToDOMString(
            unsigned int        theValue,
            XalanDOMString&     theResult)
{
    return NumberToDOMString(
                static_cast<XALAN_UINT64>(theValue),
                theResult);
}



/**
 * Converts an int value into a XalanDOMString
 * 
 * @param theValue number to be converted
 * @param theResult the string to append with the result
 * @return a reference to the passed string result.
 */
inline XalanDOMString&
NumberToDOMString(
            int                 theValue,
            XalanDOMString&     theResult)
{
    return NumberToDOMString(
                static_cast<XALAN_INT64>(theValue),
                theResult);
}



/**
 * Converts an unsigned short value into a XalanDOMString
 * 
 * @param theValue number to be converted
 * @param theResult the string to append with the result
 * @return a reference to the passed string result.
 */
inline XalanDOMString&
NumberToDOMString(
            unsigned short      theValue,
            XalanDOMString&     theResult)
{
    return NumberToDOMString(
                static_cast<XALAN_UINT64>(theValue),
                theResult);
}



/**
 * Converts a short value into a XalanDOMString
 * 
 * @param theValue number to be converted
 * @param theResult the string to append with the result
 * @return a reference to the passed string result.
 */
inline XalanDOMString&
NumberToDOMString(
            short               theValue,
            XalanDOMString&     theResult)
{
    return NumberToDOMString(
                static_cast<XALAN_INT64>(theValue),
                theResult);
}



/**
 * Converts an 64-bit unsigned value into a XalanDOMString
 * 
 * @param theValue number to be converted
 * @param theResult the string to append with the result
 * @return a reference to the passed string result.
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString&)
NumberToHexDOMString(
            XALAN_UINT64        theValue,
            XalanDOMString&     theResult);



/**
 * Converts an 64-bit signed value into a XalanDOMString
 * 
 * @param theValue number to be converted
 * @param theResult the string to append with the result
 * @return a reference to the passed string result.
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString&)
NumberToHexDOMString(
            XALAN_INT64         theValue,
            XalanDOMString&     theResult);



/**
 * Converts a XalanSSize_t value into a XalanDOMString
 * 
 * @param theValue number to be converted
 * @param theResult the string to append with the result
 * @return a reference to the passed string result.
 */
inline XalanDOMString&
NumberToHexDOMString(
            unsigned long       theValue,
            XalanDOMString&     theResult)
{
    return NumberToHexDOMString(
                static_cast<XALAN_UINT64>(theValue),
                theResult);
}



/**
 * Converts a XalanSSize_t value into a XalanDOMString
 * 
 * @param theValue number to be converted
 * @param theResult the string to append with the result
 * @return a reference to the passed string result.
 */
inline XalanDOMString&
NumberToHexDOMString(
            long                theValue,
            XalanDOMString&     theResult)
{
    return NumberToHexDOMString(
                static_cast<XALAN_INT64>(theValue),
                theResult);
}



/**
 * Converts an unsigned int value into a XalanDOMString
 * 
 * @param theValue number to be converted
 * @param theResult the string to append with the result
 * @return a reference to the passed string result.
 */
inline XalanDOMString&
NumberToHexDOMString(
            unsigned int        theValue,
            XalanDOMString&     theResult)
{
    return NumberToHexDOMString(
                static_cast<XALAN_UINT64>(theValue),
                theResult);
}



/**
 * Converts an int value into a XalanDOMString
 * 
 * @param theValue number to be converted
 * @param theResult the string to append with the result
 * @return a reference to the passed string result.
 */
inline XalanDOMString&
NumberToHexDOMString(
            int                 theValue,
            XalanDOMString&     theResult)
{
    return NumberToHexDOMString(
                static_cast<XALAN_INT64>(theValue),
                theResult);
}



/**
 * Converts an unsigned short value into a XalanDOMString
 * 
 * @param theValue number to be converted
 * @param theResult the string to append with the result
 * @return a reference to the passed string result.
 */
inline XalanDOMString&
NumberToHexDOMString(
            unsigned short      theValue,
            XalanDOMString&     theResult)
{
    return NumberToHexDOMString(
                static_cast<XALAN_UINT64>(theValue),
                theResult);
}



/**
 * Converts a short value into a XalanDOMString
 * 
 * @param theValue number to be converted
 * @param theResult the string to append with the result
 * @return a reference to the passed string result.
 */
inline XalanDOMString&
NumberToHexDOMString(
            short               theValue,
            XalanDOMString&     theResult)
{
    return NumberToHexDOMString(
                static_cast<XALAN_INT64>(theValue),
                theResult);
}



/**
 * Converts a wide string into an integer value
 * 
 * @param theString target string
 * @return integer value of target string
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(int)
WideStringToInt(const XalanDOMChar*     theString);



/**
 * Converts a wide string into a long value
 * 
 * @param theString target string
 * @return long value of target string
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(long)
WideStringToLong(const XalanDOMChar*    theString);



/**
 * Converts a wide string into an unsigned long value
 * 
 * @param theString target string
 * @return unsigned long value of target string
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(unsigned long)
WideStringToUnsignedLong(const XalanDOMChar*    theString);



/**
 * Converts a wide string into a double value
 * 
 * @param theString target string
 * @param theMemoryManager The MemoryManager instance to use.
 * @return double value of target string
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(double)
WideStringToDouble(
            const XalanDOMChar*     theString,
            MemoryManager&          theMemoryManager);



/**
 * Converts a XalanDOMString into an integer value
 * 
 * @param theString target string
 * @return integer value of target string
 */
inline int
DOMStringToInt(const XalanDOMString&    theString)
{
    return WideStringToInt(theString.c_str());
}



/**
 * Converts a XalanDOMString into a long value
 * 
 * @param theString target string
 * @return long value of target string
 */
inline long
DOMStringToLong(const XalanDOMString&   theString)
{
    return WideStringToLong(theString.c_str());
}



/**
 * Converts a XalanDOMString into a long value
 * 
 * @param theString target string
 * @return unsigned long value of target string
 */
inline unsigned long
DOMStringToUnsignedLong(const XalanDOMString&   theString)
{
    return WideStringToUnsignedLong(theString.c_str());
}



/**
 * Converts a XalanDOMString into a double value
 * 
 * @param theString target string
 * @param theMemoryManager The MemoryManager instance to use.
 * @return double value of target string
 */
inline double
DOMStringToDouble(
            const XalanDOMString&   theString,
            MemoryManager&          theMemoryManager)
{
    return WideStringToDouble(
                theString.c_str(),
                theMemoryManager);
}



/**
 * Outputs the target string to the specified stream
 * 
 * @param theStream output stream
 * @param theString target string
 * @see operator<<
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(void)
OutputString(
            XalanOutputStream&      theStream,
            const CharVectorType&   theString);



/**
 * Outputs the target string to the specified stream
 * 
 * @param theStream output stream
 * @param theString target string
 * @see operator<<
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(void)
OutputString(
#if defined(XALAN_NO_STD_NAMESPACE)
            ostream&                theStream,
#else
            std::ostream&           theStream,
#endif
            const CharVectorType&   theString);



/**
 * Outputs the target string to the specified stream
 * 
 * @param theStream output stream
 * @param theString target string
 * @see operator<<
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(void)
OutputString(
            XalanOutputStream&      theStream,
            const XalanDOMChar*     theString);



/**
 * Outputs the target string to the specified stream
 * 
 * @param theStream output stream
 * @param theString target string
 * @see operator<<
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(void)
OutputString(
#if defined(XALAN_NO_STD_NAMESPACE)
            ostream&                theStream,
#else
            std::ostream&           theStream,
#endif
            const XalanDOMChar*     theString,
            MemoryManager&          theMemoryManager);



/**
 * Outputs the target string to the specified stream
 * 
 * @param theStream output stream
 * @param theString target string
 * @see operator<<
 */
inline void
OutputString(
            XalanOutputStream&      theStream,
            const XalanDOMString&   theString)
{
    if (theString.empty() == false)
    {
        OutputString(
            theStream,
            theString.c_str());
    }
}



/**
 * Outputs the target string to the specified stream
 * 
 * @param theStream output stream
 * @param theString target string
 * @param theMemoryManager The MemoryManager instance to use.
 * @see operator<<
 */
inline void
OutputString(
#if defined(XALAN_NO_STD_NAMESPACE)
            ostream&                theStream,
#else
            std::ostream&           theStream,
#endif
            const XalanDOMString&   theString,
            MemoryManager&          theMemoryManager)
{
    OutputString(
        theStream,
        theString.c_str(),
        theMemoryManager);
}



/**
 * Outputs the string to the specified stream
 * 
 * @param theStream output stream
 * @param theString the string to output
 * @see OutputString
 */
inline XalanOutputStream&
operator<<(
            XalanOutputStream&      theStream,
            const CharVectorType&   theString)
{
    OutputString(
        theStream,
        theString);

    return theStream;
}



/**
 * Outputs the string to the specified stream
 * 
 * @param theStream output stream
 * @param theString the string to output
 * @see OutputString
 */
#if defined(XALAN_NO_STD_NAMESPACE)
inline ostream&
operator<<(
            ostream&                theStream,
#else
inline std::ostream&
operator<<(
            std::ostream&           theStream,
#endif
            const CharVectorType&   theString)
{
    OutputString(
        theStream,
        theString);

    return theStream;
}



/**
 * Outputs the target string to the specified stream
 * 
 * @param theStream output stream
 * @param theString target string
 * @see OutputString
 */
inline XalanOutputStream&
operator<<(
            XalanOutputStream&      theStream,
            const XalanDOMChar*     theString)
{
    OutputString(
        theStream,
        theString);

    return theStream;
}



/**
 * Outputs the target string to the specified stream
 * 
 * @param theStream output stream
 * @param theString target string
 * @see OutputString
 */
#if defined(XALAN_NO_STD_NAMESPACE)
inline ostream&
operator<<(
            ostream&                theStream,
#else
inline std::ostream&
operator<<(
            std::ostream&           theStream,
#endif
            const XalanDOMChar*     theString)
{
    OutputString(
        theStream,
        theString,
        XalanMemMgrs::getDefault());

    return theStream;
}



/**
 * Outputs the target string to the specified stream
 * 
 * @param theStream output stream
 * @param theString target string
 * @see OutputString
 */
inline XalanOutputStream&
operator<<(
            XalanOutputStream&      theStream,
            const XalanDOMString&   theString)
{
    OutputString(theStream,
                 theString);

    return theStream;
}



/**
 * Outputs the target string to the specified stream
 * 
 * @param theStream output stream
 * @param theString target string
 * @see OutputString
 */
#if defined(XALAN_NO_STD_NAMESPACE)
inline ostream&
operator<<(
            ostream&                theStream,
#else
inline std::ostream&
operator<<(
            std::ostream&           theStream,
#endif
            const XalanDOMString&   theString)
{
    OutputString(
        theStream,
        theString,
        XalanMemMgrs::getDefault());

    return theStream;
}



/**
 * Outputs the target string to the specified stream
 * 
 * @param theStream output stream
 * @param theString target string
 * @see OutputString
 */
#if defined(XALAN_NO_STD_NAMESPACE)
inline ostream&
operator<<(
            ostream&            theStream,
#else
inline std::ostream&
operator<<(
            std::ostream&       theStream,
#endif
            XalanDOMString&     theString)
{
    OutputString(
        theStream,
        theString,
        theString.getMemoryManager());

    return theStream;
}



/**
 * Retrieves a character at a specified index in the target string
 * 
 * @deprecated This function is deprecated.
 *
 * @param theString target string
 * @param theIndex  index of character
 * @return character at specified index
 */
inline XalanDOMChar
charAt(
            const XalanDOMString&       theString,
            XalanDOMString::size_type   theIndex)
{
    return theString[theIndex];
}



/**
 * Determines whether character represents white space
 * 
 * @param theChar target character
 * @return true if character represents white space
 */
inline bool
isXMLWhitespace(XalanDOMChar    theChar)
{
    return XalanXMLChar::isWhitespace(theChar);
}



/**
 * Determines whether character represents a digit
 * 
 * @param theChar target character
 * @return true if character represents a digit
 */
inline bool
isXMLDigit(XalanDOMChar     theChar)
{   
    return XalanXMLChar::isDigit(theChar);
}



/**
 * Determines whether character represents a letter or digit
 * 
 * @param theChar target character
 * @return true if character represents a letter or digit
 */
inline bool
isXMLLetterOrDigit(XalanDOMChar     theChar)
{
    return  XalanXMLChar::isDigit(theChar) || 
            XalanXMLChar::isLetter(theChar);
}






/**
 * Simulates the java String method substring(). Returns a new string that is
 * a substring of this string. The substring begins at the specified
 * theStartIndex and extends to the character at index theEndIndex - 1. Thus
 * the length of the substring is theEndIndex - theStartIndex.
 * 
 * @param theString     source string
 * @param theSubstring  target string
 * @param theStartIndex starting index, inclusive
 * @param theEndIndex   ending index, exclusive
 * @return A reference to theSubstring
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString&)
substring(
            const XalanDOMChar*         theString,
            XalanDOMString&             theSubstring,
            XalanDOMString::size_type   theStartIndex,
            XalanDOMString::size_type   theEndIndex = XalanDOMString::npos);



/**
 * Simulates the java String method substring(). Returns a new string that is
 * a substring of this string. The substring begins at the specified
 * theStartIndex and extends to the character at index theEndIndex - 1. Thus
 * the length of the substring is theEndIndex - theStartIndex.
 * 
 * @param theString     source string
 * @param theSubstring  target string
 * @param theStartIndex starting index, inclusive
 * @param theEndIndex   ending index, exclusive
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(void)
substring(
            const XalanDOMString&       theString,
            XalanDOMString&             theSubstring,
            XalanDOMString::size_type   theStartIndex,
            XalanDOMString::size_type   theEndIndex = XalanDOMString::npos);



/**
 * Simulates the java String method substring(). Returns a new string that is
 * a substring of this string. The substring begins at the specified
 * theStartIndex and extends to the character at index theEndIndex - 1. Thus
 * the length of the substring is theEndIndex-theStartIndex.
 * 
 * @param theString     source string
 * @param theStartIndex starting index, inclusive
 * @param theEndIndex   ending index, exclusive
 * @return string containing the specified range of characters from target
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString&)
substring(
            const XalanDOMString&       theString,
            XalanDOMString::size_type   theStartIndex,
            XalanDOMString&             theResult,
            XalanDOMString::size_type   theEndIndex = XalanDOMString::npos);



/**
 * Converts ASCII alphabetic characters from upper case to
 * lower case.  This function works only with the Unicode
 * characters A-Z.
 *
 * @param theString target string
 * @return string containing lower case characters
 */
inline XalanDOMChar
toLowerASCII(XalanDOMChar   theChar)
{
    if (theChar >= XalanUnicode::charLetter_A && theChar <= XalanUnicode::charLetter_Z)
    {
        return XalanDOMChar(theChar - (XalanUnicode::charLetter_A - XalanUnicode::charLetter_a));
    }
    else
    {
        return theChar;
    }
}



/**
 * Converts ASCII alphabetic characters from lower case to
 * upper case.  This function works only with the Unicode
 * characters a-z.
 *
 * @param theString target string
 * @return string containing upper case characters
 */
inline XalanDOMChar
toUpperASCII(XalanDOMChar   theChar)
{
    if (theChar >= XalanUnicode::charLetter_a && theChar <= XalanUnicode::charLetter_z)
    {
        return XalanDOMChar(theChar + (XalanUnicode::charLetter_A - XalanUnicode::charLetter_a));
    }
    else
    {
        return theChar;
    }
}



/**
 * Flips the case to of the supplied character.  This function works only with
 * the Unicode characters A-Z and a-z.
 *
 * @param theString target string
 * @return string containing lower case characters
 */
inline XalanDOMChar
flipCaseASCII(XalanDOMChar   theChar)
{
    if (theChar >= XalanUnicode::charLetter_A && theChar <= XalanUnicode::charLetter_Z)
    {
        return XalanDOMChar(theChar - (XalanUnicode::charLetter_A - XalanUnicode::charLetter_a));
    }
    else if (theChar >= XalanUnicode::charLetter_a && theChar <= XalanUnicode::charLetter_z)
    {
        return XalanDOMChar(theChar + (XalanUnicode::charLetter_A - XalanUnicode::charLetter_a));
    }
    else
    {
        return theChar;
    }
}



/**
 * Converts ASCII alphabetic characters from upper case to
 * lower case.  This function works only with the characters
 * a-z and A-Z.
 *
 * @param theString The source string
 * @param theResult The target string
 * @return A reference to theResult
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString&)
toLowerCaseASCII(
            const XalanDOMChar*     theString,
            XalanDOMString&         theResult);



/**
 * Converts ASCII alphabetic characters from upper case to
 * lower case.  This function works only with the characters
 * a-z and A-Z.
 *
 * @param theString The source string
 * @param theResult The target string
 * @return A reference to theResult
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString&)
toLowerCaseASCII(
            const XalanDOMString&   theString,
            XalanDOMString&         theResult);



/**
 * Converts ASCII alphabetic characters from upper case to
 * lower case.  This function works only with the characters
 * a-z and A-Z.
 *
 * @param theString The string to convert
 * @return A reference to theString
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString&)
toLowerCaseASCII(XalanDOMString&    theString);



/**
 * Converts ASCII alphabetic characters from lower case to
 * upper case.  This function works only with the characters
 * a-z and A-Z.
 *
 * @param theString The source string
 * @param theResult The target string
 * @return A reference to theResult
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString&)
toUpperCaseASCII(
            const XalanDOMChar*     theString,
            XalanDOMString&         theResult);



/**
 * Converts ASCII alphabetic characters from lower case to
 * upper case.  This function works only with the characters
 * a-z and A-Z.
 *
 * @param theString The source string
 * @param theResult The target string
 * @return A reference to theResult
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString&)
toUpperCaseASCII(
            const XalanDOMString&   theString,
            XalanDOMString&         theResult);



/**
 * Converts ASCII alphabetic characters from lower case to
 * upper case.  This function works only with the characters
 * a-z and A-Z.
 *
 * @param theString The string to convert
 * @return A reference to theString
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString&)
toUpperCaseASCII(XalanDOMString&    theString);



/**
 * Compare the contents of two strings.
 * 
 * THIS FUNCTION DOES NOT COMPARE STRINGS LIKE strcmp() OR ANY
 * OTHER "COLLATION" ALGORITHM.
 *
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return Returns 0 for equal strings, less than 0 if theLHS is less
 * than theRHS, or greater than 0 if theRHS is greater than theLHS.
 * @see operator<()
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(int)
compare(
            const CharVectorType&   theLHS,
            const CharVectorType&   theRHS);



/**
 * Compare the contents of two character arrays.
 * 
 * THIS FUNCTION DOES NOT COMPARE STRINGS LIKE strcmp() OR ANY
 * OTHER "COLLATION" ALGORITHM.
 *
 * @param theLHS first array to compare
 * @param theLHSLength the length of the first array
 * @param theRHS second array to compare
 * @param theRHSLength the length of the second array
 * @return Returns 0 for equal arrays, less than 0 if theLHS is less
 * than theRHS, or greater than 0 if theRHS is greater than theLHS.
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(int)
compare(
            const XalanDOMChar*         theLHS,
            XalanDOMString::size_type   theLHSLength,
            const XalanDOMChar*         theRHS,
            XalanDOMString::size_type   theRHSLength);



/**
 * Compare the contents of two null-terminated strings.
 * 
 * THIS FUNCTION DOES NOT COMPARE STRINGS LIKE strcmp() OR ANY
 * OTHER "COLLATION" ALGORITHM.
 *
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return Returns 0 for equal strings, less than 0 if theLHS is less
 * than theRHS, or greater than 0 if theRHS is greater than theLHS.
 */
inline int
compare(
            const XalanDOMChar*     theLHS,
            const XalanDOMChar*     theRHS)
{
    return compare(theLHS, length(theLHS), theRHS, length(theRHS));
}



/**
 * Compare the contents of two strings.
 * 
 * THIS FUNCTION DOES NOT COMPARE STRINGS LIKE strcmp() OR ANY
 * OTHER "COLLATION" ALGORITHM.
 *
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return Returns 0 for equal strings, less than 0 if theLHS is less
 * than theRHS, or greater than 0 if theRHS is greater than theLHS.
 * @see operator<()
 * @see collationCompare()
 */
inline int
compare(
            const XalanDOMString&   theLHS,
            const XalanDOMString&   theRHS)
{
    return compare(
        theLHS.c_str(),
        theLHS.length(),
        theRHS.c_str(),
        theRHS.length());
}



/**
 * Compare the contents of two strings.
 * 
 * THIS FUNCTION DOES NOT COMPARE STRINGS LIKE strcmp() OR ANY
 * OTHER "COLLATION" ALGORITHM.
 *
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return Returns 0 for equal strings, less than 0 if theLHS is less
 * than theRHS, or greater than 0 if theRHS is greater than theLHS.
 */
inline int
compare(
            const XalanDOMChar*     theLHS,
            const XalanDOMString&   theRHS)
{
    return compare(
            theLHS,
            length(theLHS),
            theRHS.c_str(),
            theRHS.length());
}



/**
 * Compare the contents of two strings.
 * 
 * THIS FUNCTION DOES NOT COMPARE STRINGS LIKE strcmp() OR ANY
 * OTHER "COLLATION" ALGORITHM.
 *
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return Returns 0 for equal strings, less than 0 if theLHS is less
 * than theRHS, or greater than 0 if theRHS is greater than theLHS.
 */
inline int
compare(
            const XalanDOMString&   theLHS,
            const XalanDOMChar*     theRHS)
{
    return compare(
            theLHS.c_str(),
            theLHS.length(),
            theRHS,
            length(theRHS));
}



/**
 * Compare the contents of two arrays in a case insensitive
 * manner.  Only the characters a-z and A-Z are considered as
 * characters with "case".
 *
 * THIS FUNCTION DOES NOT COMPARE STRINGS LIKE strcmp() OR ANY
 * OTHER "COLLATION" ALGORITHM.
 *
 * @param theLHS first array to compare
 * @param theLHSLength the length of the first array
 * @param theRHS second array to compare
 * @param theRHSLength the length of the second array
 * @return Returns 0 for equal arrays, less than 0 if theLHS is less
 * than theRHS, or greater than 0 if theRHS is greater than theLHS.
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(int)
compareIgnoreCaseASCII(
            const XalanDOMChar*         theLHS,
            XalanDOMString::size_type   theLHSLength,
            const XalanDOMChar*         theRHS,
            XalanDOMString::size_type   theRHSLength);



/**
 * Compare the contents of two strings, in a case insensitive
 * manner.  Only the characters a-z and A-Z are considered as
 * characters with "case".
 *
 * THIS FUNCTION DOES NOT COMPARE STRINGS LIKE strcmp() OR ANY
 * OTHER "COLLATION" ALGORITHM.
 *
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return Returns 0 for equal strings, less than 0 if theLHS is less
 * than theRHS, or greater than 0 if theRHS is greater than theLHS.
 */
inline int
compareIgnoreCaseASCII(
            const XalanDOMChar*     theLHS,
            const XalanDOMChar*     theRHS)
{
    return compareIgnoreCaseASCII(theLHS, length(theLHS), theRHS, length(theRHS));
}



/**
 * Compare the contents of two strings, in a case insensitive
 * manner.  Only the characters a-z and A-Z are considered as
 * characters with "case".
 *
 * THIS FUNCTION DOES NOT COMPARE STRINGS LIKE strcmp() OR ANY
 * OTHER "COLLATION" ALGORITHM.
 *
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return Returns 0 for equal strings, less than 0 if theLHS is less
 * than theRHS, or greater than 0 if theRHS is greater than theLHS.
 * @see operator<
 * @see collationCompare
 */
inline int
compareIgnoreCaseASCII(
            const XalanDOMString&   theLHS,
            const XalanDOMString&   theRHS)
{
    return compareIgnoreCaseASCII(
                theLHS.c_str(),
                theLHS.length(),
                theRHS.c_str(),
                theRHS.length());
}



/**
 * Compare the contents of two strings, in a case insensitive
 * manner.  Only the characters a-z and A-Z are considered as
 * characters with "case".
 *
 * THIS FUNCTION DOES NOT COMPARE STRINGS LIKE strcmp() OR ANY
 * OTHER "COLLATION" ALGORITHM.
 *
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return Returns 0 for equal strings, less than 0 if theLHS is less
 * than theRHS, or greater than 0 if theRHS is greater than theLHS.
 */
inline int
compareIgnoreCaseASCII(
            const XalanDOMString&   theLHS,
            const XalanDOMChar*     theRHS)
{
    return compareIgnoreCaseASCII(
                theLHS.c_str(),
                theLHS.length(),
                theRHS,
                length(theRHS));
}



/**
 * Compare the contents of two strings, in a case insensitive
 * manner.  Only the characters a-z and A-Z are considered for
 * the comparison.
 *
 * THIS FUNCTION DOES NOT COMPARE STRINGS LIKE strcmp() OR ANY
 * OTHER "COLLATION" ALGORITHM.
 *
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return Returns 0 for equal strings, less than 0 if theLHS is less
 * than theRHS, or greater than 0 if theRHS is greater than theLHS.
 */
inline int
compareIgnoreCaseASCII(
            const XalanDOMChar*     theLHS,
            const XalanDOMString&   theRHS)
{
    return compareIgnoreCaseASCII(
                theLHS,
                length(theLHS),
                theRHS.c_str(),
                theRHS.length());
}



/**
 * Compare the contents of two character arrays.
 * 
 * @param theLHS first array to compare
 * @param theLHSLength the length of the first array
 * @param theRHS second array to compare
 * @param theRHSLength the length of the second array
 * @return Returns 0 for equal arrays, less than 0 if theLHS is less
 * than theRHS, or greater than 0 if theRHS is greater than theLHS.
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(int)
collationCompare(
            const XalanDOMChar*         theLHS,
            XalanDOMString::size_type   theLHSLength,
            const XalanDOMChar*         theRHS,
            XalanDOMString::size_type   theRHSLength);

 
 
/**
 * Compare the contents of two strings.
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return Returns 0 for equal strings, less than 0 if theLHS is less
 * than theRHS, or greater than 0 if theRHS is greater than theLHS.
 * @see operator<()
 * @see compare()
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(int)
collationCompare(
            const XalanDOMChar*     theLHS,
            const XalanDOMChar*     theRHS);



/**
 * Compare the contents of two strings.
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return Returns 0 for equal strings, less than 0 if theLHS is less
 * than theRHS, or greater than 0 if theRHS is greater than theLHS.
 * @see operator<()
 * @see compare()
 */
inline int
collationCompare(
            const XalanDOMString&   theLHS,
            const XalanDOMString&   theRHS)
{
    return collationCompare(theLHS.c_str(), theRHS.c_str());
}



/**
 * Compare the contents of two strings.
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return Returns 0 for equal strings, less than 0 if theLHS is less
 * than theRHS, or greater than 0 if theRHS is greater than theLHS.
 */
inline int
collationCompare(
            const XalanDOMChar*     theLHS,
            const XalanDOMString&   theRHS)
{
    return collationCompare(theLHS, theRHS.c_str());
}



/**
 * Compare the contents of two strings.
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return Returns 0 for equal strings, less than 0 if theLHS is less
 * than theRHS, or greater than 0 if theRHS is greater than theLHS.
 */
inline int
collationCompare(
            const XalanDOMString&   theLHS,
            const XalanDOMChar*     theRHS)
{
    return collationCompare(
                theLHS.c_str(),
                theLHS.length(),
                theRHS,
                length(theRHS));
}



/**
 * Compare the contents of two arrays for equality
 * 
 * @param theLHS first array to compare
 * @param theRHS second array to compare
 * @param theLength the length of the arrays
 * @return true if the contents of both arrays are identical
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
equals(
            const XalanDOMChar*         theLHS,
            const XalanDOMChar*         theRHS,
            XalanDOMString::size_type   theLength);



/**
 * Compare the contents of two arrays for equality
 * 
 * @param theLHS first array to compare
 * @param theLHSLength the length of the theLHS
 * @param theRHS second array to compare
 * @param theRHSLength the length of the theRHS
 * @return true if the contents of both arrays are identical
 */
inline bool
equals(
            const XalanDOMChar*         theLHS,
            XalanDOMString::size_type   theLHSLength,
            const XalanDOMChar*         theRHS,
            XalanDOMString::size_type   theRHSLength)
{
    return theLHSLength != theRHSLength ? false : equals(theLHS, theRHS, theLHSLength);
}



/**
 * Compare the contents of two strings for equality
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return true if the contents of both strings are identical
 */
inline bool
equals(
            const XalanDOMChar*     theLHS,
            const XalanDOMChar*     theRHS)
{
    const XalanDOMString::size_type     theLHSLength = length(theLHS);

    return theLHSLength != length(theRHS) ? false : equals(theLHS, theRHS, theLHSLength);
}



/**
 * Compare the contents of two strings for equality
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return true if the contents of both strings are identical
 */
inline bool
equals(
            const XalanDOMString&   theLHS,
            const XalanDOMString&   theRHS)
{
    return theLHS == theRHS;
}



/**
 * Compare the contents of two strings for equality
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return true if the contents of both strings are identical
 */
inline bool
equals(
            const XalanDOMChar*     theLHS,
            const XalanDOMString&   theRHS)
{
    assert(theLHS != 0);

    // Swap them...
    return theRHS == theLHS;
}



/**
 * Compare the contents of two strings for equality
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return true if the contents of both strings are identical
 */
inline bool
equals(const XalanDOMString&    theLHS,
       const XalanDOMChar*      theRHS)
{
    return equals(theRHS, theLHS);
}



/**
 * Compare the contents of two strings for equality
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @param theRHSLength the length of the theRHS
 * @return true if the contents of both strings are identical
 */
inline bool
equals(
            const XalanDOMString&       theLHS,
            const XalanDOMChar*         theRHS,
            XalanDOMString::size_type   theRHSLength)
{
    return theRHSLength != theLHS.length() ? false : equals(theLHS.c_str(), theRHSLength, theRHS, theRHSLength);
}




/**
 * Compare the contents of two arrays for equality, without regard for case.
 * Only the characters a-z and A-Z are considered characters with "case".
 *
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return true if the case-insensitive contents of both strings are identical
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
equalsIgnoreCaseASCII(
            const XalanDOMChar*         theLHS,
            const XalanDOMChar*         theRHS,
            XalanDOMString::size_type   theLength);



/**
 * Compare the contents of two strings for equality, without regard for case.
 * Only the characters a-z and A-Z are considered characters with "case".
 *
 * @param theLHS first string to compare
 * @param theLHSLength the length of the theLHS
 * @param theRHS second string to compare
 * @param theRHSLength the length of the theRHS
 * @return true if both strings are identical
 */
inline bool
equalsIgnoreCaseASCII(
            const XalanDOMChar*         theLHS,
            XalanDOMString::size_type   theLHSLength,
            const XalanDOMChar*         theRHS,
            XalanDOMString::size_type   theRHSLength)
{
    return theLHSLength != theRHSLength ? false :
        equalsIgnoreCaseASCII(theLHS, theRHS, theLHSLength);
}



/**
 * Compare the contents of two strings for equality, without regard for case.
 * Only the characters a-z and A-Z are considered characters with "case".
 *
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return true if both strings are identical
 */
inline bool
equalsIgnoreCaseASCII(
            const XalanDOMChar*     theLHS,
            const XalanDOMChar*     theRHS)
{
    const XalanDOMString::size_type     theLength = length(theLHS);

    return theLength != length(theRHS) ? false :
        equalsIgnoreCaseASCII(theLHS, theRHS, theLength);
}



/**
 * Compare the contents of two strings for equality, without regard for case
 * Only the characters A-Z and a-z are considered.
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return true if the case-insensitive contents of both strings are identical
 */
inline bool
equalsIgnoreCaseASCII(
            const XalanDOMString&   theLHS,
            const XalanDOMString&   theRHS)
{
    const XalanDOMString::size_type     theLength = theLHS.length();

    return theLength != theRHS.length() ? false :
        equalsIgnoreCaseASCII(
            theLHS.c_str(),
            theRHS.c_str(),
            theLength);
}



/**
 * Compare the contents of two strings for equality, without regard for case.
 * Only the characters a-z and A-Z are considered characters with "case".
 *
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return true if the case-insensitive contents of both strings are identical
 */
inline bool
equalsIgnoreCaseASCII(
            const XalanDOMChar*     theLHS,
            const XalanDOMString&   theRHS)
{
    const XalanDOMString::size_type     theRHSLength = theRHS.length();

    return theRHSLength != length(theLHS) ? false :
        equalsIgnoreCaseASCII(
            theLHS,
            theRHS.c_str(),
            theRHSLength);
}



/**
 * Compare the contents of two strings for equality, without regard for case.
 * Only the characters A-Z and a-z are considered.
 *
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return true if the case-insensitive contents of both strings are identical
 */
inline bool
equalsIgnoreCaseASCII(
            const XalanDOMString&   theLHS,
            const XalanDOMChar*     theRHS)
{
    return equalsIgnoreCaseASCII(theRHS, theLHS);
}



/**
 * Implements operator< for CharVectorType.
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return Returns true if theLHS is lexically
 * less than theRHS
 * @see compare
 */
inline bool
operator<(
            const CharVectorType&   theLHS,
            const CharVectorType&   theRHS)
{
    return compare(theLHS, theRHS) < 0 ? true : false;
}



/**
 * Implements operator< for DOMStrings.
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return Returns true if theLHS is lexically
 * less than theRHS
 * @see compare
 */
inline bool
operator<(
            const XalanDOMString&   theLHS,
            const XalanDOMString&   theRHS)
{
    return compare(theLHS, theRHS) < 0 ? true : false;
}



/**
 * Assign one string to another
 *
 * @deprecated This function is deprecated.
 *
 * @param theString         target string
 * @param theStringToAppend string to assign
 * @param theStringToAppendLength length of the string (XalanDOMString::npos implies the string is null-terminated)
 * @return a reference to the target string
 */
inline XalanDOMString&
assign(
            XalanDOMString&         theString,
            const XalanDOMString&   theStringToAssign)
{
    theString = theStringToAssign;

    return theString;
}



/**
 * Assign one string to another
 *
 * @deprecated This function is deprecated.
 *
 * @param theString         target string
 * @param theStringToAppend string to assign
 * @param theStringToAppendLength length of the string (XalanDOMString::npos implies the string is null-terminated)
 * @return a reference to the target string
 */
inline XalanDOMString&
assign(
            XalanDOMString&             theString,
            const XalanDOMChar*         theStringToAssign,
            XalanDOMString::size_type   theStringToAssignLength = XalanDOMString::npos)
{
    if (theStringToAssignLength == XalanDOMString::npos)
    {
        theString.assign(theStringToAssign);
    }
    else
    {
        theString.assign(theStringToAssign, theStringToAssignLength);
    }

    return theString;
}



/**
 * Concatenate two strings
 * 
 * @deprecated This function is deprecated.
 *
 * @param theString         target string
 * @param theStringToAppend string to add to target
 * @return a reference to the target string
 */
inline XalanDOMString&
append(
            XalanDOMString&         theString,
            const XalanDOMString&   theStringToAppend)
{
    theString.append(theStringToAppend);

    return theString;
}



/**
 * Concatenate two strings
 *
 * @deprecated This function is deprecated.
 *
 * @param theString         target string
 * @param theStringToAppend string to add to target
 * @param theStringToAppendLength length of the string (XalanDOMString::npos implies the string is null-terminated)
 * @return a reference to the target string
 */
inline XalanDOMString&
append(
            XalanDOMString&             theString,
            const XalanDOMChar*         theStringToAppend,
            XalanDOMString::size_type   theStringToAppendLength = XalanDOMString::npos)
{
    assert(theStringToAppend != 0);

    if (theStringToAppendLength == XalanDOMString::npos)
    {
        theString.append(theStringToAppend);
    }
    else
    {
        theString.append(theStringToAppend, theStringToAppendLength);
    }

    return theString;
}



/**
 * Concatenate two strings
 *
 * @deprecated This function is deprecated.
 *
 * @param theString         target string
 * @param theStringToAppend string to add to target
 * @param theStringToAppendLength length of the string (XalanDOMString::npos implies the string is null-terminated)
 * @return string with contents of 'theStringToAppend' added to target string
 */
inline XalanDOMString&
append(
            XalanDOMString&             theString,
            const char*                 theStringToAppend,
            XalanDOMString::size_type   theStringToAppendLength = XalanDOMString::npos)
{
    XalanDOMString tmp(theString.getMemoryManager());

    TranscodeFromLocalCodePage(theStringToAppend, tmp, theStringToAppendLength);

    theString.append(tmp);

    return theString;
}



/**
 * Concatenate a string and a character
 *
 * @deprecated This function is deprecated.
 *
 * @param theString target string
 * @param theCharToAppend the character to add to the target
 * @return string with the character appended
 */
inline XalanDOMString&
append(
            XalanDOMString&     theString,
            const XalanDOMChar  theCharToAppend)
{
    theString.append(1, theCharToAppend);

    return theString;
}



/**
 * Concatenate a string and a character
 * 
 * @deprecated This function is deprecated.
 *
 * @param theString target string
 * @param theCharToAppend the character to add to the target
 * @return string with the character appended
 */
inline XalanDOMString&
append(
            XalanDOMString&     theString,
            char                theCharToAppend)
{
    // We have to transcode before appending...
    char    theTempBuffer[] = { theCharToAppend, '\0' };

    return append(theString, theTempBuffer);
}



/**
 * Insert a string into another string.
 *
 * @deprecated This function is deprecated.
 *
 * @param theString target string
 * @param thePosition The position in the target string to insert
 * @param theStringToInsert The string to insert
 * @return A reference to the target string
 */
inline XalanDOMString&
insert(
            XalanDOMString&             theString,
            XalanDOMString::size_type   thePosition,
            const XalanDOMString&       theStringToInsert)
{
    theString.insert(thePosition, theStringToInsert);

    return theString;
}



/**
 * Insert a string into another string.
 *
 * @deprecated This function is deprecated.
 *
 * @param theString target string
 * @param thePosition The position in the target string to insert
 * @param theStringToInsert The string to insert
 * @return A reference to the target string
 */
inline XalanDOMString&
insert(
            XalanDOMString&             theString,
            XalanDOMString::size_type   thePosition,
            const XalanDOMChar*         theStringToInsert)
{
    theString.insert(thePosition, theStringToInsert);

    return theString;
}



/**
 * Remove leading and trailing whitespace.
 * 
 * @param theString The string to trim.
 * @param theResult The result string.
 * @return A reference to theResult.
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString&)
trim(
            const XalanDOMString&   theString,
            XalanDOMString&         theResult);



/**
 * Remove all elements from target string
 * 
 * @deprecated This function is deprecated.
 *
 * @param theString target string
 */
inline void
clear(XalanDOMString&   theString)
{
    theString.clear();
}



/**
 * Remove all elements from target string
 *
 * @deprecated This function is deprecated.
 *
 * @param theString target string
 */
inline void
erase(XalanDOMString&   theString)
{
    theString.erase();
}



/**
 * Remove all elements from target string
 * and frees all allocated memory.
 * 
 * @param theString target string
 */
inline void
releaseMemory(XalanDOMString&   theString,MemoryManager&  theManager)
{
    XalanDOMString(theManager).swap(theString);
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(void)
CopyWideStringToVector(
            const XalanDOMChar*     theString,
            CharVectorType&         theVector);



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(void)
CopyStringToVector(
            const char*         theString,
            CharVectorType&     theVector);



/**
 * Utility function to make a null-terminated vector of XMLChs, from a
 * null-terminated array of chars, via transcoding, if requested.
 * 
 * @param data array to be converted
 * @param whether or not to transcode
 * @return null-terminated vector of XalanDOMChar
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMCharVectorType&)
MakeXalanDOMCharVector(
            const char*     data,
            XalanDOMCharVectorType& result,
            bool            fTranscode = true);



/**
 * Utility function to make a null-terminated vector of XMLChs, from a
 * null-terminated array of XalanDOMChar.
 * 
 * @param data array to be converted
 * @return null-terminated vector of XalanDOMChar
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMCharVectorType&)
MakeXalanDOMCharVector(const XalanDOMChar*  data,
                       XalanDOMCharVectorType& result);



/**
 * Utility function to make a null-terminated vector of XMLChs, from a
 * XalanDOMString
 * 
 * @param data XalanDOMString to be converted
 * @return null-terminated vector of XalanDOMChar
 */
inline XalanDOMCharVectorType&
MakeXalanDOMCharVector(const XalanDOMString&    data,
                       XalanDOMCharVectorType& result)
{
    return MakeXalanDOMCharVector(data.c_str(),result);
}



#if defined(XALAN_NO_STD_NAMESPACE)
struct c_wstr_functor : public unary_function<XalanDOMString, const XalanDOMChar*>
#else
struct c_wstr_functor : public std::unary_function<XalanDOMString, const XalanDOMChar*>
#endif
{
    result_type
    operator() (const argument_type&    theString) const
    {
        return theString.c_str();
    }
};



/**
 * Less than functor for DOMStrings which ignores case for the characters a-z and A-Z
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return true if the theLHS is less than theRHS, without respect to case.
 */
#if defined(XALAN_NO_STD_NAMESPACE)
struct DOMStringLessThanIgnoreCaseASCIIFunction : public binary_function<const XalanDOMString&, const XalanDOMString&, bool>
#else
struct DOMStringLessThanIgnoreCaseASCIIFunction : public std::binary_function<const XalanDOMString&, const XalanDOMString&, bool>
#endif
{
    result_type
    operator() (first_argument_type     theLHS,
                second_argument_type    theRHS) const
    {
        return compareIgnoreCaseASCII(theLHS, theRHS) < 0 ? true : false;
    }
};



/**
 * Less than or equal functor for DOMStrings
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return true if the theLHS is less than or equal to theRHS
 */
#if defined(XALAN_NO_STD_NAMESPACE)
struct DOMStringLessThanOrEqualFunction : public binary_function<const XalanDOMString&, const XalanDOMString&, bool>
#else
struct DOMStringLessThanOrEqualFunction : public std::binary_function<const XalanDOMString&, const XalanDOMString&, bool>
#endif
{
    result_type
    operator() (first_argument_type     theLHS,
                second_argument_type    theRHS) const
    {
        return compare(theLHS, theRHS) <= 0 ? true : false;
    }
};



/**
 * Greater than functor for DOMStrings
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return true if the theLHS is greater than theRHS
 */
#if defined(XALAN_NO_STD_NAMESPACE)
struct DOMStringGreaterThanFunction : public binary_function<const XalanDOMString&, const XalanDOMString&, bool>
#else
struct DOMStringGreaterThanFunction : public std::binary_function<const XalanDOMString&, const XalanDOMString&, bool>
#endif
{
    result_type
    operator() (first_argument_type     theLHS,
                second_argument_type    theRHS) const
    {
        return compare(theLHS, theRHS) > 0 ? true : false;
    }
};



/**
 * Greater than or equal functor for DOMStrings
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return true if the theLHS is greater than or equal to theRHS
 */
#if defined(XALAN_NO_STD_NAMESPACE)
struct DOMStringGreaterThanOrEqualFunction : public binary_function<const XalanDOMString&, const XalanDOMString&, bool>
#else
struct DOMStringGreaterThanOrEqualFunction : public std::binary_function<const XalanDOMString&, const XalanDOMString&, bool>
#endif
{
    result_type
    operator() (first_argument_type     theLHS,
                second_argument_type    theRHS) const
    {
        return compare(theLHS, theRHS) >= 0 ? true : false;
    }
};



/**
 * This functor is designed to compare 0-terminated wide strings in a case-insensitive
 * manner.  It substitutes for the default less<type*> so that the contents of wide strings
 * can be compared, rather than just the pointers.
 */
#if defined(XALAN_NO_STD_NAMESPACE)
struct less_no_case_ascii_wide_string : public binary_function<const XalanDOMChar*, const XalanDOMChar*, bool>
#else
struct less_no_case_ascii_wide_string : public std::binary_function<const XalanDOMChar*, const XalanDOMChar*, bool>
#endif
{
    /**
     * Compare the values of two objects.
     *
     *
     * @param theLHS first object to compare
     * @param theRHS second object to compare
     * @return true if objects are the same
     */
    result_type
    operator()(
            first_argument_type     theLHS,
            second_argument_type    theRHS) const
    {
        return compareIgnoreCaseASCII(theLHS, theRHS) < 0 ? true : false;
    }
};



/**
 * Determines if the string contains only whitespace
 * 
 * @param theString target string
 * @return true if string contains only whitespace
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
isXMLWhitespace(const XalanDOMString&   string);



/**
 * Determines if a range in an array contains only whitespace
 * 
 * @param ch target array
 * @param start starting index to examine
 * @param length number of characters to examine
 * @return true if specified range contains only whitespace
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
isXMLWhitespace(
            const XalanDOMChar          ch[],
            XalanDOMString::size_type   start,
            XalanDOMString::size_type   length);



/**
 * Determines if a null-terminated string contains only whitespace
 * 
 * @param theString target string
 * @return true if the string contains only whitespace
 */
inline bool
isXMLWhitespace(const XalanDOMChar*     theString)
{
    assert(theString != 0);

    return isXMLWhitespace(theString, 0, length(theString));
}



XALAN_CPP_NAMESPACE_END



#endif  // DOMSTRINGHELPER_HEADER_GUARD_1357924680
