/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */
// Class header file.
#include "DOMStringHelper.hpp"



#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>



#include <algorithm>
#include <vector>



#if !defined(XALAN_NO_IOSFWD)
#include <iosfwd>
#endif

#if defined(XALAN_OLD_STREAM_HEADERS)
#include <strstream.h>
#else
#include <strstream>
#endif



#if !defined(XALAN_NO_NAMESPACES)
using std::back_inserter;
using std::copy;
using std::hex;
using std::ios;
using std::istrstream;
using std::ostream;
using std::ostrstream;
using std::transform;
using std::vector;
#endif



// Xerces header files
#include <util/XMLString.hpp>

#if !defined(XML_LSTRSUPPORT)
#include <util/PlatformUtils.hpp>
#endif



#include <Include/XalanAutoPtr.hpp>
#include <Include/STLHelper.hpp>



#include "DoubleSupport.hpp"
#include "XalanOutputStream.hpp"
#include "XalanUnicode.hpp"



#if defined(OS390)
//                                                                       
//  Cannot use the OS/390 c/c++ towupper and towlower functions in the   
//  Unicode environment. We will use mytowupper and mytowlower here.     
//                                                                       
#undef towupper                                                          
#undef towlower                                                          
#define towupper mytowupper                                              
#define towlower mytowlower                                              

#endif



// The maximum number of digits that sprintf can put in a buffer.
// 100 for now.  We're using this because we want to avoid transcoding
// number strings when we don't have to,
const size_t	MAX_PRINTF_DIGITS = 100;

// The maximum number of characters for a floating point number.
const size_t	MAX_FLOAT_CHARACTERS = 100;



static XalanDOMString	theNaNString;

static XalanDOMString	theNegativeInfinityString;

static XalanDOMString	thePositiveInfinityString;

static XalanDOMString	theNegativeZeroString;

static XalanDOMString	thePositiveZeroString;



/**
 * Initialize static data.  Must be called before any
 * other functions are called.  See PlatformSupportInit.
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(void)
DOMStringHelperInitialize()
{
	theNaNString = XALAN_STATIC_UCODE_STRING("NaN");
	theNegativeInfinityString = XALAN_STATIC_UCODE_STRING("-Infinity");
	thePositiveInfinityString = XALAN_STATIC_UCODE_STRING("Infinity");
	theNegativeZeroString = XALAN_STATIC_UCODE_STRING("-0");
	thePositiveZeroString = XALAN_STATIC_UCODE_STRING("0");
}



/**
 * Destroy static data.  After thus function is called,
 * no other functions can be called.  See PlatformSupportInit.
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(void)
DOMStringHelperTerminate()
{
	releaseMemory(theNaNString);
	releaseMemory(theNegativeInfinityString);
	releaseMemory(thePositiveInfinityString);
	releaseMemory(theNegativeZeroString);
	releaseMemory(thePositiveZeroString);
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(unsigned int)
indexOf(
			const XalanDOMChar*		theString,
			const XalanDOMChar*		theSubstring)
{
	assert(theString != 0);
	assert(theSubstring != 0);

	const unsigned int	theStringLength = length(theString);
	const unsigned int	theSubstringLength = length(theSubstring);

	// If the substring is longer than the string, then
	// it's not a substring.
	if (theStringLength < theSubstringLength)
	{
		return theStringLength;
	}
	else
	{
		bool			fMatch = false;

		unsigned int	theStringIndex = 0;

		// While we haven't matched, and we haven't finished with the
		// first string, and the number of characters left in the first
		// string is greater than the length of the second string, try
		// to match the strings.
		while(fMatch == false &&
			  theStringIndex < theStringLength &&
			  theStringLength - theStringIndex >= theSubstringLength)
		{
			// We always start over from the beginning of the second string.
			unsigned int	theSubstringIndex = 0;

			// This variable will be incremented to index into the first
			// string.  That way, we preserve the first string index for
			// when we have to restart the following loop with the next
			// position in the first string.
			unsigned int	theOffset = 0;

			// Compare the characters in the two strings, at the
			// current indices, until the characters don't match.
			while(theStringIndex < theStringLength &&
				  theSubstringIndex < theSubstringLength &&
				  theString[theStringIndex + theOffset] ==
						theSubstring[theSubstringIndex])
			{
				theOffset++;
				theSubstringIndex++;
			}

			// If we've reached the end of the second string,
			// then we've found a match.
			if (theSubstringIndex == theSubstringLength)
			{
				fMatch = true;
			}
			else
			{
				theStringIndex++;
			}
		}

		return fMatch == false ? theStringLength : theStringIndex;
	}
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(unsigned int)
indexOf(
			const XalanDOMString&	theString,
			const XalanDOMString&	theSubstring)
{
	if (isEmpty(theString) == true)
	{
		return 0;
	}
	else if (isEmpty(theSubstring) == true)
	{
		return theString.length();
	}
	else
	{
		return indexOf(c_wstr(theString), c_wstr(theSubstring));
	}
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(unsigned int)
lastIndexOf(
			const XalanDOMChar*		theString,
			XalanDOMChar			theChar)
{
	const unsigned int	theLength = length(theString);

	if (theLength == 0)
	{
		return theLength;
	}
	else
	{
		unsigned int	theIndex = theLength - 1;

		// Rely on wrap-around...
		while(theIndex < theLength && theString[theIndex] != theChar)
		{
			theIndex--;
		}

		return theIndex > theLength ? theLength : theIndex;
	}
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
startsWith(
			const XalanDOMChar*		theString,
			const XalanDOMChar*		theSubstring)
{
	bool		fResult = false;

	const unsigned int	theStringLength = length(theString);

	const unsigned int	theSubstringLength = length(theSubstring);

	if (theSubstringLength == 0)
	{
		// Make this work like Java...
		return true;
	}
	else if (theStringLength >= theSubstringLength)
	{
		unsigned int	i = 0;

		// Compare each character...
		for (;
				i < theSubstringLength &&
						theString[i] == theSubstring[i];
					i++)
		{
			;
		}

		// If we've gotten to the end of the substring, then
		// return true.
		if (i == theSubstringLength)
		{
			fResult = true;
		}
	}

	return fResult;
}



static const XalanDOMChar	theDummyEmptyString = 0;



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
startsWith(
			const XalanDOMChar*		theString,
			const XalanDOMString&	theSubstring)
{
	const XalanDOMChar*	const	theBuffer =
		c_wstr(theSubstring);

	return startsWith(theString, theBuffer == 0 ? &theDummyEmptyString : theBuffer);
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
startsWith(
			const XalanDOMString&	theString,
			const XalanDOMChar*		theSubstring)
{
	const XalanDOMChar*	const	theBuffer =
		c_wstr(theString);

	return startsWith(theBuffer == 0 ? &theDummyEmptyString : theBuffer, theSubstring);
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
startsWith(
			const XalanDOMString&	theString,
			const XalanDOMString&	theSubstring)
{
	const XalanDOMChar*	const	theStringBuffer =
		c_wstr(theString);

	const XalanDOMChar*	const	theSubstringBuffer =
		c_wstr(theSubstring);

	return startsWith(
		theStringBuffer == 0 ? &theDummyEmptyString : theStringBuffer,
		theSubstringBuffer == 0 ? &theDummyEmptyString : theSubstringBuffer);
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
endsWith(
			const XalanDOMChar*		theString,
			const XalanDOMChar*		theSubstring)
{
	bool				fResult = false;

	const unsigned int	theStringLength = length(theString);

	const unsigned int	theSubStringLength = length(theSubstring);

	// If the substring is longer, there's no point in continuing.
	if (theStringLength >= theSubStringLength)
	{
		int		i = theStringLength - 1;
		int		j = theSubStringLength - 1;

		// Compare each character...
		for (;
				j >= 0 &&
						theString[i] == theSubstring[j];
					--j, --i)
		{
			;
		}

		// If we've gotten to the beginning of the substring, then
		// return true.
		if (j == -1)
		{
			fResult = true;
		}
	}

	return fResult;
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(void)
OutputString(XalanOutputStream&		theStream,
			 const CharVectorType&	theString)
{
	if (theString.size() > 0)
	{
		theStream.write(c_str(theString));
	}
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(void)
OutputString(
#if defined(XALAN_NO_NAMESPACES)
			ostream&				theStream,
#else
			std::ostream&			theStream,
#endif
			const CharVectorType&	theString)
{
	if (theString.size() > 0)
	{
		theStream << c_str(theString);
	}
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(void)
OutputString(XalanOutputStream&		theStream,
			 const XalanDOMChar*	theString)
{
	if (theString != 0)
	{
		theStream.write(theString);
	}
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(void)
OutputString(
#if defined(XALAN_NO_NAMESPACES)
			ostream&				theStream,
#else
			std::ostream&			theStream,
#endif
			const XalanDOMChar*		theString)
{
	OutputString(theStream, TranscodeToLocalCodePage(theString));
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString)
substring(
			const XalanDOMChar*		theString,
			unsigned int			theStartIndex,
			unsigned int			theEndIndex)
{
	assert(theString != 0);

	const unsigned int	theStringLength = length(theString);

	// $$$ ToDo: In Java-land, any failing of this
	// assertion would result in an exception being thrown.
	assert(theStartIndex <= theStringLength);

	if (theStartIndex == theStringLength)
	{
		// This is allowed, and should return an empty string.
		return XalanDOMString();
	}
	else
	{
		const unsigned int	theLength = theEndIndex == UINT_MAX ? theStringLength - theStartIndex :
													theEndIndex - theStartIndex;
		assert(theStartIndex + theLength <= theStringLength);

		return XalanDOMString(theString + theStartIndex, theLength);
	}
}



#if defined(XALAN_NO_ALGORITHMS_WITH_BUILTINS)

template<class InputIteratorType, class OutputIteratorType>
inline OutputIteratorType
XalanCopy(
			InputIteratorType	begin,
			InputIteratorType	end,
			OutputIteratorType	iterator)
{
	for(; begin != end; ++iterator, ++begin)
	{
		*iterator = *begin;
	}

	return iterator;
}



template<class InputIteratorType, class OutputIteratorType, class UnaryFunction>
inline OutputIteratorType
XalanTransform(
			InputIteratorType	begin,
			InputIteratorType	end,
			OutputIteratorType	iterator,
			UnaryFunction		function)
{
	for(; begin != end; ++iterator, ++begin)
	{
		*iterator = function(*begin);
	}

	return iterator;
}

#endif



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString)
substring(
			const XalanDOMString&	theString,
			unsigned int			theStartIndex,
			unsigned int			theEndIndex)
{
	const unsigned int	theStringLength = length(theString);

	// $$$ ToDo: In Java-land, any failing of this
	// assertion would result in an exception being thrown.
	assert(theStartIndex <= theStringLength);

	if (theStartIndex == theStringLength)
	{
		// This is allowed, and should return an empty string.
		return XalanDOMString();
	}
	else
	{
		const unsigned int	theLength = theEndIndex == UINT_MAX ? theStringLength - theStartIndex :
													theEndIndex - theStartIndex;

		if (theLength == 0)
		{
			return XalanDOMString();
		}
		else
		{
			assert(theStartIndex + theLength <= theStringLength);

			return theString.substr(theStartIndex, theLength);
		}
	}
}



template <class InputIteratorType, class OutputIteratorType, class FunctionType>
OutputIteratorType
TransformString(
			InputIteratorType	theInputBegin,
			InputIteratorType	theInputEnd,
			OutputIteratorType	theOutputIterator,
			FunctionType		theFunction)
{
#if defined(XALAN_NO_ALGORITHMS_WITH_BUILTINS)
	return XalanTransform(
			theInputBegin,
			theInputEnd,
			theOutputIterator,
			theFunction);
#else
	return transform(
			theInputBegin,
			theInputEnd,
			theOutputIterator,
			theFunction);
#endif
}



template <class SizeType, class FunctionType>
XalanDOMString
TransformString(
			const XalanDOMChar*		theInputString,
			SizeType				theInputStringLength,
			FunctionType			theFunction)
{
	assert(theInputString != 0);

	XalanDOMString	theConvertedString;

	TransformString(
			theInputString,
			theInputString + theInputStringLength,
			back_inserter(theConvertedString),
			theFunction);

	return theConvertedString;
}



template <class FunctionType>
XalanDOMString
TransformXalanDOMString(
			const XalanDOMString&	theInputString,
			FunctionType			theFunction)
{
	const unsigned int	theStringLength = length(theInputString);

	if (theStringLength == 0)
	{
		return theInputString;
	}
	else
	{
		const XalanDOMChar* const	theBuffer = c_wstr(theInputString);
		assert(theBuffer != 0);

		return TransformString(theBuffer, unsigned(theStringLength), theFunction);
	}
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString)
toLowerCase(const XalanDOMChar*		theString)
{
	return TransformString(theString, length(theString), towlower);
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString)
toLowerCase(const XalanDOMString&	theString)
{
	return TransformXalanDOMString(theString, towlower);
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString)
toUpperCase(const XalanDOMChar*		theString)
{
	return TransformString(theString, length(theString), towupper);
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString)
toUpperCase(const XalanDOMString&	theString)
{
	return TransformXalanDOMString(theString, towupper);
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString)
toLowerCaseASCII(const XalanDOMChar*	theString)
{
	return TransformString(theString, length(theString), toLowerASCII);
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString)
toLowerCaseASCII(const XalanDOMString&	theString)
{
	return TransformXalanDOMString(theString, toLowerASCII);
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString)
toUpperCaseASCII(const XalanDOMChar*	theString)
{
	return TransformString(theString, length(theString), toUpperASCII);
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString)
toUpperCaseASCII(const XalanDOMString&	theString)
{
	return TransformXalanDOMString(theString, toUpperASCII);
}



template <class InputCharType, class OutputCharType>
class IdentityTransform
{
public:

	OutputCharType
	operator()(InputCharType	theChar) const
	{
		return OutputCharType(theChar);
	}
};



template<class InputCharType, class OutputCharType>
IdentityTransform<InputCharType, OutputCharType>
makeIdentityTransform(
			const InputCharType*,
			const OutputCharType*)
{
	return IdentityTransform<InputCharType, OutputCharType>();
}



IdentityTransform<char, char>
makeCharIdentityTransform()
{
	char	theDummy;

	return makeIdentityTransform(&theDummy, &theDummy);
}


IdentityTransform<XalanDOMChar, XalanDOMChar>
makeXalanDOMCharIdentityTransform()
{
	XalanDOMChar	theDummy;

	return makeIdentityTransform(&theDummy, &theDummy);
}


template <class Type, class SizeType, class FunctionType>
int
doCompare(
			const Type*		theLHS,
			SizeType		theLHSLength,
			const Type*		theRHS,
			SizeType		theRHSLength,
			FunctionType	theTransformFunction)
{
	// We don't really have to order, so save some time...
	if (theLHSLength < theRHSLength)
	{
		return -1;
	}
	else if (theRHSLength < theLHSLength)
	{
		return 1;
	}
	else
	{
		Type	theLHSChar = Type(0);
		Type	theRHSChar = Type(0);

		for(SizeType i = 0; i < theLHSLength; i++)
		{
			theLHSChar = theTransformFunction(theLHS[i]);
			theRHSChar = theTransformFunction(theRHS[i]);

			if (theLHSChar != theRHSChar)
			{
				break;
			}
		}

		return int(theLHSChar - theRHSChar);
	}
}



template <class Type, class SizeType, class FunctionType>
int
doCollationCompare(
			const Type*		theLHS,
			SizeType		theLHSLength,
			const Type*		theRHS,
			SizeType		theRHSLength,
			FunctionType	theTransformFunction)
{
	int		theResult = 0;

	if (theLHSLength != 0 || theRHSLength != 0)
	{
		Type		theLHSChar = Type(0);
		Type		theRHSChar = Type(0);

		SizeType	i = 0;

		for(; i < theLHSLength && i < theRHSLength; i++)
		{
			theLHSChar = theTransformFunction(theLHS[i]);
			theRHSChar = theTransformFunction(theRHS[i]);

			if (theLHSChar != theRHSChar)
			{
				break;
			}
		}

		if (i == theLHSLength)
		{
			// We reached the end of theLHS...
			if (i != theRHSLength)
			{
				// but not the end of theRHS.
				theResult = -1;
			}
		}
		else if (i == theRHSLength)
		{
			// We reached the end of theRHS string...
			if (i != theLHSLength)
			{
				// but not the end of theLHS string.
				theResult = 1;
			}
		}
		else
		{
			// We didn't reach the end of _either_ string, so
			// return the difference between the two characters
			// that caused the problem.
			theResult = theLHSChar - theRHSChar;
		}
	}

	return theResult;
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(int)
compare(
			const CharVectorType&	theLHS,
			const CharVectorType&	theRHS)
{
	return doCompare(
				toCharArray(theLHS),
				theLHS.size(),
				toCharArray(theRHS),
				theRHS.size(),
				makeCharIdentityTransform());
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(int)
compare(
			const XalanDOMChar*		theLHS,
			unsigned int			theLHSLength,
			const XalanDOMChar*		theRHS,
			unsigned int			theRHSLength)
{
	return doCompare(
				theLHS,
				theLHSLength,
				theRHS,
				theRHSLength,
				makeXalanDOMCharIdentityTransform());
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(int)
compareIgnoreCase(
			const XalanDOMChar*		theLHS,
			unsigned int			theLHSLength,
			const XalanDOMChar*		theRHS,
			unsigned int			theRHSLength)
{
	return doCompare(
				theLHS,
				theLHSLength,
				theRHS,
				theRHSLength,
				towupper);
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(int)
compareIgnoreCaseASCII(
			const XalanDOMChar*		theLHS,
			unsigned int			theLHSLength,
			const XalanDOMChar*		theRHS,
			unsigned int			theRHSLength)
{
	return doCompare(
				theLHS,
				theLHSLength,
				theRHS,
				theRHSLength,
				toUpperASCII);
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(int)
collationCompare(
			const XalanDOMChar*		theLHS,
			unsigned int			theLHSLength,
			const XalanDOMChar*		theRHS,
			unsigned int			theRHSLength)
{
	return doCollationCompare(
				theLHS,
				theLHSLength,
				theRHS,
				theRHSLength,
				makeXalanDOMCharIdentityTransform());
}



template <class Type, class SizeType, class FunctionType>
bool
doEquals(
			const Type*		theLHS,
			const Type*		theRHS,
			SizeType		theLength,
			FunctionType	theTransformFunction)
{
	assert(theLHS != 0 && theRHS != 0);

#if 1
	if (theLength == 0)
	{
		return true;
	}
	else
	{
		const Type* const	theEnd = theLHS + theLength;

		while(theTransformFunction(*theLHS) == theTransformFunction(*theRHS))
		{
			++theLHS;

			if (theLHS == theEnd)
			{
				return true;
			}
			else
			{
				++theRHS;
			}
		}

		return false;
	}
#else
	for(SizeType i = 0; i < theLength; ++i)
	{
		const Type	theLHSChar = theTransformFunction(theLHS[i]);
		const Type	theRHSChar = theTransformFunction(theRHS[i]);

		if (theLHSChar != theRHSChar)
		{
			return false;
		}
	}

	return true;
#endif
}


				   
template <class Type, class SizeType, class FunctionType>
bool
doEqualsIgnoreCase(
			const Type*		theLHS,
			const Type*		theRHS,
			SizeType		theLength,
			FunctionType	theToUpperFunction)
{
	// Check each character, converting to uppercase
	// for the test.
	for(SizeType i = 0; i < theLength; i++)
	{
		const Type	charLHS = theLHS[i];
		const Type	charRHS = theRHS[i];

		if (charLHS != charRHS &&
			Type(theToUpperFunction(charLHS)) != charRHS &&
			charLHS != Type(theToUpperFunction(charRHS)))
		{
			return false;
		}
	}

	return true;
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
equals(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS,
			unsigned int			theLength)
{
	return doEquals(
				theLHS,
				theRHS,
				theLength,
				makeXalanDOMCharIdentityTransform());
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
equalsIgnoreCase(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS,
			unsigned int			theLength)
{
	return doEqualsIgnoreCase(
				theLHS,
				theRHS,
				theLength,
				towupper);
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
equalsIgnoreCaseASCII(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS,
			unsigned int			theLength)
{
	return doEqualsIgnoreCase(
				theLHS,
				theRHS,
				theLength,
				toUpperASCII);
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMCharVectorType)
MakeXalanDOMCharVector(
			const char*		data,
			bool			fTranscode)
{
	assert(data != 0);

	XalanDOMCharVectorType	theResult;

	if (fTranscode == true)
	{
		// Create a vector which includes the terminating 0.
		TranscodeFromLocalCodePage(data, theResult, true);
	}
	else
	{
		// Include the terminating null byte...
		const unsigned int	theLength = strlen(data) + 1;

		theResult.reserve(theLength);

#if defined(XALAN_NO_ALGORITHMS_WITH_BUILTINS)
		XalanCopy(
			data,
			data + theLength,
			back_inserter(theResult));
#else
		copy(
			data,
			data + theLength,
			back_inserter(theResult));
#endif
	}

	return theResult;
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMCharVectorType)
MakeXalanDOMCharVector(const XalanDOMChar*	data)
{
	assert(data != 0);

	unsigned int	theLength = length(data);

	// Create a vector which includes the terminating 0.
	return XalanDOMCharVectorType(data, data + theLength + 1);
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(void)
CopyWideStringToVector(
			const XalanDOMChar*		theString,
			CharVectorType&			theVector)
{
	const unsigned int	theLength = length(theString);

	if (theLength != 0)
	{
		theVector.reserve(theVector.size() + theLength + 1);

		for(unsigned int i = 0; i < theLength; i++)
		{
			// Assert that the truncation will not affect the resulting character.
			assert(theString[i] == char(theString[i]));

			theVector.push_back(char(theString[i]));
		}

		// Put a terminating 0 byte.
		theVector.push_back(0);
	}
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(void)
CopyStringToVector(
			const char*			theString,
			CharVectorType&		theVector)
{
	theVector.insert(
		theVector.end(),
		theString,
		theString + strlen(theString) + 1);
}



template <class Type>
Type
WideStringToIntegral(
			const XalanDOMChar*		theString,
			Type					/* theDummy */)
{
	if (theString == 0 || DoubleSupport::isValid(theString) == false)
	{
		return Type(0);
	}
	else
	{
		// OK, now we know we have a valid string, so start converting...
		Type	theResult = 0;

		// Consume any leading whitespace (which we allow)
		while(isXMLWhitespace(*theString) == true)
		{
			++theString;
		}

		const bool	isNegative = *theString == XalanUnicode::charHyphenMinus ? true : false;

		if (isNegative == true)
		{
			++theString;
		}

		while(*theString != 0)
		{
			if (*theString >= XalanUnicode::charDigit_0 && *theString <= XalanUnicode::charDigit_9)
			{
				theResult *= 10;

				theResult += *theString - XalanUnicode::charDigit_0;

				++theString;
			}
			else if (isXMLWhitespace(*theString) == true)
			{
				// This must be trailing whitespace...
				break;
			}
			else
			{
				// An non-numeric character was encountered, so stop...
				return 0;
			}
		}

		return isNegative == true ? -theResult : theResult;
	}
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(int)
WideStringToInt(const XalanDOMChar*		theString)
{
	return WideStringToIntegral(theString, int(0));
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(long)
WideStringToLong(const XalanDOMChar*	theString)
{
	return WideStringToIntegral(theString, long(0));
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(unsigned long)
WideStringToUnsignedLong(const XalanDOMChar*	theString)
{
	return WideStringToIntegral(theString, (unsigned long)0);
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(double)
WideStringToDouble(const XalanDOMChar*	theString)
{
	return DoubleSupport::toDouble(theString);
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString)
trim(const XalanDOMString&	theString)
{
	if (isEmpty(theString))
		return theString;

	const int	strLen = length(theString);
	
	// index of first non-whitespace character
	int			leadingSpace = 0;

	for (; leadingSpace < strLen; ++leadingSpace)
		if (!isXMLWhitespace(charAt(theString, leadingSpace)))
			break;

	// index of last non-whitespace character
	int trailingSpace = strLen - 1;

	for (; trailingSpace>=0; --trailingSpace)
		if (!isXMLWhitespace(charAt(theString, trailingSpace)))
			break;

	return substring(theString, leadingSpace, trailingSpace + 1);
}



// A very cheap decimal number transcoder...
template <class InputCharType, class OutputCharType>
class DecimalNumberTranscodeTransform
{
public:

	OutputCharType
	operator()(InputCharType	theChar) const
	{
		switch(theChar)
		{
		case '-':
			return OutputCharType(XalanUnicode::charHyphenMinus);
			break;

		case '.':
			return OutputCharType(XalanUnicode::charFullStop);
			break;

		case '0':
			return OutputCharType(XalanUnicode::charDigit_0);
			break;

		case '1':
			return OutputCharType(XalanUnicode::charDigit_1);
			break;

		case '2':
			return OutputCharType(XalanUnicode::charDigit_2);
			break;

		case '3':
			return OutputCharType(XalanUnicode::charDigit_3);
			break;

		case '4':
			return OutputCharType(XalanUnicode::charDigit_4);
			break;

		case '5':
			return OutputCharType(XalanUnicode::charDigit_5);
			break;

		case '6':
			return OutputCharType(XalanUnicode::charDigit_6);
			break;

		case '7':
			return OutputCharType(XalanUnicode::charDigit_7);
			break;

		case '8':
			return OutputCharType(XalanUnicode::charDigit_8);
			break;

		case '9':
			return OutputCharType(XalanUnicode::charDigit_9);
			break;

		default:
			return OutputCharType(0);
			break;
		}
	}
};



template<class InputCharType, class OutputCharType>
DecimalNumberTranscodeTransform<InputCharType, OutputCharType>
makeDecimalNumberTranscodeTransform(
			const InputCharType*,
			const OutputCharType*)
{
	return DecimalNumberTranscodeTransform<InputCharType, OutputCharType>();
}



// A very cheap hex number transcoder...
template <class InputCharType, class OutputCharType>
class HexadecimalNumberTranscodeTransform : public DecimalNumberTranscodeTransform<InputCharType, OutputCharType>
{
public:

	typedef DecimalNumberTranscodeTransform<InputCharType, OutputCharType>	BaseClassType;

	OutputCharType
	operator()(InputCharType	theChar) const
	{
		switch(theChar)
		{
		case 'A':
			return OutputCharType(XalanUnicode::charLetter_A);
			break;

		case 'a':
			return OutputCharType(XalanUnicode::charLetter_a);
			break;

		case 'B':
			return OutputCharType(XalanUnicode::charLetter_B);
			break;

		case 'b':
			return OutputCharType(XalanUnicode::charLetter_b);
			break;

		case 'C':
			return OutputCharType(XalanUnicode::charLetter_C);
			break;

		case 'c':
			return OutputCharType(XalanUnicode::charLetter_c);
			break;

		case 'D':
			return OutputCharType(XalanUnicode::charLetter_D);
			break;

		case 'd':
			return OutputCharType(XalanUnicode::charLetter_d);
			break;

		case 'E':
			return OutputCharType(XalanUnicode::charLetter_E);
			break;

		case 'e':
			return OutputCharType(XalanUnicode::charLetter_e);
			break;

		case 'F':
			return OutputCharType(XalanUnicode::charLetter_F);
			break;

		case 'f':
			return OutputCharType(XalanUnicode::charLetter_f);
			break;

		default:
			return BaseClassType::operator()(theChar);
			break;
		}
	}
};



template <class InputIteratorType, class OutputIteratorType>
OutputIteratorType
TranscodeNumber(
			InputIteratorType	theInputBegin,
			InputIteratorType	theInputEnd,
			OutputIteratorType	theOutputIterator)
{
	return TransformString(
				theInputBegin,
				theInputEnd,
				theOutputIterator,
#if defined(XALAN_NON_ASCII_PLATFORM)
				DecimalNumberTranscodeTransform<char, XalanDOMChar>());
#else
				IdentityTransform<char, XalanDOMChar>());
#endif
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString)&
DoubleToDOMString(
			double				theDouble,
			XalanDOMString&		theResult)
{
	if (DoubleSupport::isNaN(theDouble) == true)
	{
		theResult = theNaNString;
	}
	else if (DoubleSupport::isPositiveInfinity(theDouble) == true)
	{
		theResult = thePositiveInfinityString;
	}
	else if (DoubleSupport::isNegativeInfinity(theDouble) == true)
	{
		theResult = theNegativeInfinityString;
	}
	else if (DoubleSupport::isNegativeZero(theDouble) == true)
	{
		theResult = theNegativeZeroString;
	}
	else if (DoubleSupport::isPositiveZero(theDouble) == true)
	{
		theResult = thePositiveZeroString;
	}
	else if (long(theDouble) == theDouble)
	{
		LongToDOMString(long(theDouble), theResult);
	}
	else
	{
		char			theBuffer[MAX_PRINTF_DIGITS + 1];

		unsigned int	theCharsWritten = sprintf(theBuffer, "%f", theDouble);
		assert(theCharsWritten != 0);

		// First, cleanup the output to conform to the XPath standard,
		// which says no trailing '0's for the decimal portion.
		// So start with the last digit, and search until we find
		// the last correct character for the output.
		// Also, according to the XPath standard, any values without
		// a fractional part are printed as integers.  There's always
		// a decimal point, so we have to strip stuff away...

		// Now, move back while there are zeros...
		while(theBuffer[--theCharsWritten] == '0')
		{
		}

		// If a decimal point stopped the loop, then
		// we don't want to preserve it.  Otherwise,
		// another digit stopped the loop, so we must
		// preserve it.
		if(theBuffer[theCharsWritten] != '.')
		{
			++theCharsWritten;
		}

		reserve(theResult, theCharsWritten + 1);

		TranscodeNumber(
				theBuffer,
				theBuffer + theCharsWritten,
				back_inserter(theResult));
	}

	return theResult;
}



template <class ScalarType>
XalanDOMChar*
ScalarToDecimalString(
			ScalarType		theValue,
			XalanDOMChar*	theOutput)
{
	// Null terminate it...
	*theOutput = 0;

	if (theValue < 0)
	{
		do
		{
			*--theOutput = XalanDOMChar(-(theValue % 10) + XalanUnicode::charDigit_0);

			// OK, we're done with it...
			theValue /= 10;
		}
		while(theValue != 0);

		*--theOutput = XalanUnicode::charHyphenMinus;
	}
	else
	{
		do
		{
			*--theOutput = XalanDOMChar(theValue % 10 + XalanUnicode::charDigit_0);

			// OK, we're done with it...
			theValue /= 10;
		}
		while(theValue != 0);
	}

	return theOutput;
}



template <class ScalarType>
void
ScalarToDecimalString(
			ScalarType			theValue,
			XalanDOMString&		theResult)
{
	// We don't need to transcode, so just make it a
	// wide character string...
	XalanDOMChar			theBuffer[MAX_PRINTF_DIGITS + 1];

	XalanDOMChar* const		theEnd = &theBuffer[MAX_PRINTF_DIGITS];

	XalanDOMChar* const		theBegin = ScalarToDecimalString(theValue, theEnd);

	append(theResult, theBegin, theEnd - theBegin);
}



template <class ScalarType>
XalanDOMChar*
UnsignedScalarToHexadecimalString(
			ScalarType		theValue,
			XalanDOMChar*	theOutput)
{
	if (theValue >= 0)
	{
		// Null terminate it...
		*theOutput = 0;

		do
		{
			// Next spot...
			--theOutput;

			const ScalarType	theTemp = theValue % 16;

			// Isolate the left most character.
			if (theTemp >= 0 && theTemp <= 9)
			{
				*theOutput = XalanDOMChar(theTemp + XalanUnicode::charDigit_0);
			}
			else
			{
				assert(theTemp >= 10 && theTemp <= 15);

				*theOutput = XalanDOMChar(theTemp - 10 + XalanUnicode::charLetter_A);
			}

			// OK, we're done with it...
			theValue /= 16;
		}
		while(theValue != 0);
	}

	return theOutput;
}



template <class ScalarType>
void
UnsignedScalarToHexadecimalString(
			ScalarType			theValue,
			XalanDOMString&		theResult)
{
	if (theValue >= 0)
	{
		// We don't need to transcode, so just make it a
		// wide character string...
		XalanDOMChar			theBuffer[MAX_PRINTF_DIGITS + 1];

		XalanDOMChar* const		theEnd = &theBuffer[MAX_PRINTF_DIGITS];

		XalanDOMChar* const		theBegin = UnsignedScalarToHexadecimalString(theValue, theEnd);

		append(theResult, theBegin, theEnd - theBegin);
	}
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString)&
LongToHexDOMString(
			long				theValue,
			XalanDOMString&		theResult)
{
	UnsignedScalarToHexadecimalString(theValue, theResult);

	return theResult;
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString)&
UnsignedLongToHexDOMString(
			unsigned long		theValue,
			XalanDOMString&		theResult)
{
	UnsignedScalarToHexadecimalString(theValue, theResult);

	return theResult;
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString)&
LongToDOMString(
			long				theValue,
			XalanDOMString&		theResult)
{
	ScalarToDecimalString(theValue, theResult);

	return theResult;
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString)&
UnsignedLongToDOMString(
			unsigned long		theValue,
			XalanDOMString&		theResult)
{
	ScalarToDecimalString(theValue, theResult);

	return theResult;
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
isXMLWhitespace(const XalanDOMString&	string)
{
	const unsigned int	theLength = length(string);

	if (theLength == 0)
	{
		return true;
	}
	else
	{
		const XalanDOMChar* const	theBuffer =
			c_wstr(string);

		assert(theBuffer != 0);

		return isXMLWhitespace(theBuffer, 0, theLength);
	}
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
isXMLWhitespace(
			const XalanDOMChar		ch[],
			unsigned int			start,
			unsigned int			length)
{
	const unsigned int	end = start + length;

	for(unsigned int s = start; s < end; s++) 
	{
		if (!isXMLWhitespace(ch[s]))	
			return false;
	}

	return true;
}
