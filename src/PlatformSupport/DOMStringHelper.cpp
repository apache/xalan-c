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
#include <iosfwd>



#include <algorithm>
#include <strstream>
#include <vector>



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



#include "DoubleSupport.hpp"
#include "STLHelper.hpp"
#include "XalanOutputStream.hpp"
#include "XalanAutoPtr.hpp"
#include "XalanUnicode.hpp"



// The maximum number of digits that sprintf can put in a buffer.
// 100 for now.  We're using this because we want to avoid transcoding
// number strings when we don't have to,
const size_t	MAX_PRINTF_DIGITS = 100;



#if !defined(XALAN_LSTRSUPPORT)



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(const XalanDOMString)
initializeAndTranscode(const char*	theString)
{
	static bool		fInitialized = false;

	if (fInitialized == false)
	{
		XMLPlatformUtils::Initialize();

		fInitialized = true;
	}

	return XalanDOMString(theString);
}

#endif



static XalanDOMString	theNaNString;

static XalanDOMString	theNegativeInfinityString;

static XalanDOMString	thePositiveInfinityString;



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
}



/**
 * Destroy static data.  After thus function is called,
 * no other functions can be called.  See PlatformSupportInit.
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(void)
DOMStringHelperTerminate()
{
	clear(theNaNString);
	clear(theNegativeInfinityString);
	clear(thePositiveInfinityString);
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
	assert(theStringLength >= 0);

	const unsigned int	theSubstringLength = length(theSubstring);
	assert(theSubstringLength >= 0);

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



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
startsWith(
			const XalanDOMString&	theDOMString,
			const XalanDOMString&	theSubstring)
{
	const bool	fStringIsEmpty = isEmpty(theDOMString);
	const bool	fSubstringIsEmpty = isEmpty(theSubstring);

	// $$$ ToDo: Are these cases with the empty strings
	// correct?
	if (fStringIsEmpty == true)
	{
		if (fSubstringIsEmpty == false)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else if (isEmpty(theSubstring) == true)
	{
		// Apparently, Java believes this to be true;
		return true;
	}
	else
	{
		return startsWith(c_wstr(theDOMString), c_wstr(theSubstring));
	}
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
endsWith(
			const XalanDOMChar*		theString,
			const XalanDOMChar*		theSubstring)
{
	bool				fResult = false;

	const unsigned int	theStringLength = length(theString);

	const unsigned int	theSubStringLength = length(theSubstring);

	// If either string is of length 0, or if the substring
	// is longer, there's no point in continuing.
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

	// $$$ ToDo: In Java-land, any failing of these
	// assertions would result in an exception being thrown.
	assert(theStartIndex <= theStringLength && theStartIndex >= 0);

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

template<class OutputIteratorType>
inline void
XalanCopy(
			const char*			begin,
			const char*			end,
			OutputIteratorType	iterator)
{
	for(; begin != end; ++iterator, ++begin)
	{
		*iterator = *begin;
	}
}



template<class OutputIteratorType>
inline void
XalanCopy(
			const XalanDOMChar*		begin,
			const XalanDOMChar*		end,
			OutputIteratorType		iterator)
{
	for(; begin != end; ++iterator, ++begin)
	{
		*iterator = *begin;
	}
}



template<class OutputIteratorType, class UnaryFunction>
inline void
XalanTransform(
			const XalanDOMChar*		begin,
			const XalanDOMChar*		end,
			OutputIteratorType		iterator,
			UnaryFunction			function)
{
	for(; begin != end; ++iterator, ++begin)
	{
		*iterator = function(*begin);
	}
}

#endif



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString)
substring(
			const XalanDOMString&	theString,
			unsigned int			theStartIndex,
			unsigned int			theEndIndex)
{
	const unsigned int	theStringLength = length(theString);

	// $$$ ToDo: In Java-land, any failing of these
	// assertions would result in an exception being thrown.
	assert(theStartIndex <= theStringLength && theStartIndex >= 0);

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

		// @@ JMD:
		// If this is the case, the DOMString class doesn't create a new string,
		// and in any case, does not null terminate the string, just points to
		// the beginning, so we have to manually extract 'theLength' characters
		// and create a new buffer
		if (0 == theStartIndex)
		{
			vector<XalanDOMChar>	theBuffer;

			// Reserve the buffer now.  We don't have to null-terminate,
			// because the XalanDOMString constructor will take a size
			// parameter.
			theBuffer.reserve(theLength);

			const XalanDOMChar* const	ptr = theString.rawBuffer();

#if defined(XALAN_NO_ALGORITHMS_WITH_BUILTINS)
			XalanCopy(
				ptr,
				ptr + theLength,
				back_inserter(theBuffer));
#else
			copy(
				ptr,
				ptr + theLength,
				back_inserter(theBuffer));
#endif

			return XalanDOMString(theBuffer.begin(), theBuffer.size());
		}
		else
		{
			return theString.substringData(theStartIndex, theLength);
		}
	}
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
		vector<XalanDOMChar>	theConvertedString;

		const XalanDOMChar* const	theBuffer = c_wstr(theInputString);
		assert(theBuffer != 0);

#if defined(XALAN_NO_ALGORITHMS_WITH_BUILTINS)
		XalanTransform(
			theBuffer,
			theBuffer + theStringLength,
			back_inserter(theConvertedString),
			theFunction);
#else
		transform(
			theBuffer,
			theBuffer + theStringLength,
			back_inserter(theConvertedString),
			theFunction);
#endif

		return XalanDOMString(theConvertedString.begin(), theConvertedString.size());
	}
}




XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString)
toLowerCase(const XalanDOMString&	theString)
{
	return TransformXalanDOMString(theString, towlower);
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString)
toUpperCase(const XalanDOMString&	theString)
{
	return TransformXalanDOMString(theString, towupper);
}



inline bool
doEqualsIgnoreCase(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS,
			unsigned int			theLength)
{
	// Check each character, converting to uppercase
	// for the test.
	unsigned int	i = 0;

	for(; i < theLength; i++)
	{
		const XalanDOMChar	charLHS = theLHS[i];
		const XalanDOMChar	charRHS = theRHS[i];

		if (charLHS != charRHS &&
			towupper(charLHS) != charRHS &&
			charLHS != towupper(charRHS))
		{
			break;
		}
	}

	// Did we reach the end of the string?
	if (i == theLength)
	{
		return true;
	}
	else
	{
		return false;
	}
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
equalsIgnoreCase(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS)
{
	assert(theLHS != 0);
	assert(theRHS != 0);

	bool				fResult = false;

	const unsigned int	theLength = length(theLHS);

	// If they are equal, then compare
	if (theLength == length(theRHS))
	{
		fResult = doEqualsIgnoreCase(theLHS, theRHS, theLength);
	}

	return fResult;
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
equalsIgnoreCase(
			const XalanDOMString&	theLHS,
			const XalanDOMString&	theRHS)
{
	const bool	fLHSIsEmpty = isEmpty(theLHS);
	const bool	fRHSIsEmpty = isEmpty(theRHS);

	if (fLHSIsEmpty == true)
	{
		// If theRHS is empty, then they're equal, or if the
		// length of theRHS is 0, they're equal as well.
		return fRHSIsEmpty == true ? true : length(theRHS) == 0 ? true : false;
	}
	else if (fRHSIsEmpty == true)
	{
		// It the length of theRHS is 0, they're equal.
		return length(theLHS) == 0 ? true : false;
	}
	else
	{
		assert(c_wstr(theLHS) != 0);
		assert(c_wstr(theRHS) != 0);

		unsigned const int	theLHSLength = length(theLHS);

		if (theLHSLength == length(theRHS))
		{
			return doEqualsIgnoreCase(c_wstr(theLHS), c_wstr(theRHS), theLHSLength);
		}
		else
		{
			return false;
		}
	}
}



template <class Type, class SizeType>
int
doCompare(
			const Type*		theLHS,
			SizeType		theLHSLength,
			const Type*		theRHS,
			SizeType		theRHSLength)
{
	int					theResult = 0;

	if (theLHSLength != 0 || theRHSLength != 0)
	{
		Type		theLHSChar = Type(0);
		Type		theRHSChar = Type(0);

		SizeType	i = 0;

		for(; i < theLHSLength && i < theRHSLength; i++)
		{
			theLHSChar = theLHS[i];
			theRHSChar = theRHS[i];

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
	return doCompare(c_str(theLHS), theLHS.size(), c_str(theRHS), theRHS.size());
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(int)
compare(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS)
{
#if 1
	return doCompare(theLHS, length(theLHS), theRHS, length(theRHS));
#else
	const unsigned int	theLHSLength = length(theLHS);
	const unsigned int	theRHSLength = length(theRHS);

	int					theResult = 0;

	if (theLHSLength != 0 || theRHSLength != 0)
	{
		XalanDOMChar		theLHSChar = 0;
		XalanDOMChar		theRHSChar = 0;

		unsigned int	i = 0;

		for(; i < theLHSLength && i < theRHSLength; i++)
		{
			theLHSChar = theLHS[i];
			theRHSChar = theRHS[i];

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
#endif
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(int)
compareIgnoreCase(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS)
{
	unsigned const int	theLHSLength = length(theLHS);
	unsigned const int	theRHSLength = length(theRHS);

	int					theResult = 0;

	if (theLHSLength != 0 || theRHSLength != 0)
	{
		XalanDOMChar		theLHSChar = 0;
		XalanDOMChar		theRHSChar = 0;

		unsigned int	i = 0;

		for(; i < theLHSLength && i < theRHSLength; i++)
		{
			theLHSChar = towupper(theLHS[i]);
			theRHSChar = towupper(theRHS[i]);

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



struct WideStringLexicalCompare
{
	int
	operator()(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS) const
	{
		return compare(theLHS, theRHS);
	}
};



struct WideStringIgnoreCaseCompare
{
	int
	operator()(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS) const
	{
		return compareIgnoreCase(theLHS, theRHS);
	}
};



struct WideStringCollationCompare
{
	int
	operator()(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS) const
	{
		return collationCompare(theLHS, theRHS);
	}
};



template<class CompareFunctionType>
int
DOMStringCompare(
			CompareFunctionType		theCompareFunction,
			const XalanDOMString&	theLHS,
			const XalanDOMString&	theRHS)
{
	const bool	fLHSIsEmpty = isEmpty(theLHS);
	const bool	fRHSIsEmpty = isEmpty(theRHS);

	// correct?
	if (fLHSIsEmpty == true)
	{
		if (fRHSIsEmpty == true || length(theRHS) == 0)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else if (isEmpty(theRHS) == true)
	{
		if (length(theLHS) == 0)
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		assert(c_wstr(theLHS) != 0 && c_wstr(theRHS) != 0);

		return theCompareFunction(c_wstr(theLHS), c_wstr(theRHS));
	}
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(int)
compare(
			const XalanDOMString&	theLHS,
			const XalanDOMString&	theRHS)
{
	return DOMStringCompare(WideStringLexicalCompare(),
							theLHS,
							theRHS);
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(int)
compareIgnoreCase(
			const XalanDOMString&	theLHS,
			const XalanDOMString&	theRHS)
{
	return DOMStringCompare(WideStringIgnoreCaseCompare(),
							theLHS,
							theRHS);
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(int)
collationCompare(
			const XalanDOMString&	theLHS,
			const XalanDOMString&	theRHS)
{
	return DOMStringCompare(WideStringCollationCompare(),
							theLHS,
							theRHS);
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMCharVectorType)
MakeXalanDOMCharVector(
			const char*		data,
			bool			fTranscode)
{
	assert(data != 0);

	if (fTranscode == true)
	{
		XalanDOMChar*	theTranscodedData = XMLString::transcode(data);

		const XalanArrayAutoPtr<XalanDOMChar>	theJanitor(theTranscodedData);

		// Create a vector which includes the terminating 0.

		return MakeXalanDOMCharVector(theTranscodedData);
	}
	else
	{
		// Include the terminating null byte...
		const unsigned int	theLength = strlen(data) + 1;

		XalanDOMCharVectorType	theResult;
		
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

		return theResult;
	}
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



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(int)
WideStringToInt(const XalanDOMChar*		theString)
{
	int				theResult = 0;

	vector<char>	theVector;

	CopyWideStringToVector(theString,
						   theVector);

	if (theVector.size() > 0)
	{
		theResult = atoi(theVector.begin());
	}

	return theResult;
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(long)
WideStringToLong(const XalanDOMChar*	theString)
{
	long			theResult = 0;

	CharVectorType	theVector;

	CopyWideStringToVector(theString,
						   theVector);

	if (theVector.size() > 0)
	{
		theResult = atol(theVector.begin());
	}

	return theResult;
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(unsigned long)
WideStringToUnsignedLong(const XalanDOMChar*	theString)
{
	unsigned long	theResult = 0;

	CharVectorType	theVector;

	CopyWideStringToVector(theString,
						   theVector);

	if (theVector.size() > 0)
	{
		istrstream	theFormatter(&theVector[0]);

		theFormatter >> theResult;
	}

	return theResult;
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
	int leadingSpace = 0;

	for (; leadingSpace < strLen; ++leadingSpace)
		if (!isSpace(charAt(theString, leadingSpace)))
			break;

	// index of last non-whitespace character
	int trailingSpace = strLen - 1;

	for (; trailingSpace>=0; --trailingSpace)
		if (!isSpace(charAt(theString, trailingSpace)))
			break;

	return substring(theString, leadingSpace, trailingSpace + 1);
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString)
DoubleToDOMString(double	theDouble)
{
	if (DoubleSupport::isNaN(theDouble) == true)
	{
		return theNaNString;
	}
	else if (DoubleSupport::isPositiveInfinity(theDouble) == true)
	{
		return thePositiveInfinityString;
	}
	else if (DoubleSupport::isNegativeInfinity(theDouble) == true)
	{
		return theNegativeInfinityString;
	}
	else
	{
		// $$$ ToDo: this is all temporary, until we get the NumberFormat and DecimalFormat
		// classes working.
		// According to the XPath standard, any values without
		// a fractional part are printed as integers.
		double	intPart = 0;

		double	fracPart = fabs(modf(theDouble, &intPart));

		char		theBuffer[MAX_PRINTF_DIGITS + 1];

#if 1
		sprintf(theBuffer, "%f", theDouble);
#else
		ostrstream	theFormatter(theBuffer, sizeof(theBuffer));

		// Ensure that we get fixed point results, and that there's enough precision.
		theFormatter.flags((theFormatter.flags() & ~ios::scientific) | ios::fixed);

		theFormatter.precision(20);

		theFormatter << theDouble << '\0';
#endif
		// OK, now we have to clean up the output for
		// the XPath standard, which says no trailing
		// '0's for the decimal portion.  So start with
		// the last digit, and replace any '0's with the
		// null character.  We know at this point that
		// we have at least 1 digit before the decimal
		// point, and and least 1 non-zero digit after
		// the decimal point, since any values with no
		// fractional part were printed as integers
		XalanDOMCharVectorType		theResult =
#if defined(XALAN_NON_ASCII_PLATFORM)
				MakeXalanDOMCharVector(theBuffer, true);
#else
				MakeXalanDOMCharVector(theBuffer, false);
#endif

		XalanDOMCharVectorType::iterator	thePosition = theResult.end();

		// Move to the terminating null byte...
		--thePosition;

		// Now, move back while there are zeros.
		while(*--thePosition == XalanUnicode::charDigit_0)
		{
		}

		// If there's no fractional part, make sure we get rid
		// of the decimal point...
		if (fracPart != 0 ||
			*thePosition != XalanUnicode::charFullStop)
		{
			// Move up one, since we need to keep at least one...
			++thePosition;
		}

		// Terminate it...
		*thePosition = 0;
			
		return XalanDOMString(theResult.begin());
	}
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString)
LongToHexDOMString(long		theLong)
{
// I'm 99% sure that we don't need to use swprintf
// to format, since strings of numbers don't to be
// generated as wide strings.
#if defined(XALAN_USE_WCHAR_SUPPORT)

	wchar_t		theBuffer[MAX_PRINTF_DIGITS + 1];

	swprintf(theBuffer,
			 L"%lx",
			 theLong);

	return XalanDOMString(theBuffer, length(theBuffer));

#else

	char		theBuffer[MAX_PRINTF_DIGITS + 1];

	ostrstream	theFormatter(theBuffer, sizeof(theBuffer));

	theFormatter << hex << theLong << '\0';

	// We don't need to transcode, so just make it a
	// wide character string...
	wchar_t		theResult[MAX_PRINTF_DIGITS + 1];

	const unsigned int	theLength = length(theBuffer);

#if defined(XALAN_NO_ALGORITHMS_WITH_BUILTINS)
	XalanCopy(theBuffer, theBuffer + theLength, theResult);
#else
	copy(theBuffer, theBuffer + theLength, theResult);
#endif

	return XalanDOMString(theResult, theLength);

#endif
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString)
UnsignedLongToHexDOMString(unsigned long	theUnsignedLong)
{
	char		theBuffer[MAX_PRINTF_DIGITS + 1];

	ostrstream	theFormatter(theBuffer, sizeof(theBuffer));

	theFormatter << hex << theUnsignedLong << '\0';

	// We don't need to transcode, so just make it a
	// wide character string...
	wchar_t		theResult[MAX_PRINTF_DIGITS + 1];

	const unsigned int	theLength = length(theBuffer);

#if defined(XALAN_NO_ALGORITHMS_WITH_BUILTINS)
	XalanCopy(theBuffer, theBuffer + theLength, theResult);
#else
	copy(theBuffer, theBuffer + theLength, theResult);
#endif

	return XalanDOMString(theResult, theLength);
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString)
LongToDOMString(long	theLong)
{
#if defined(XALAN_USE_WCHAR_SUPPORT)

	wchar_t		theBuffer[MAX_PRINTF_DIGITS + 1];

	swprintf(theBuffer,
			 L"%ld",
			 theLong);

	return XalanDOMString(theBuffer, length(theBuffer));

#else

	char		theBuffer[MAX_PRINTF_DIGITS + 1];

	ostrstream	theFormatter(theBuffer, sizeof(theBuffer));

	theFormatter << theLong << '\0';

	// We don't need to transcode, so just make it a
	// wide character string...
	wchar_t		theResult[MAX_PRINTF_DIGITS + 1];

	const unsigned int	theLength = length(theBuffer);

#if defined(XALAN_NO_ALGORITHMS_WITH_BUILTINS)
	XalanCopy(theBuffer, theBuffer + theLength, theResult);
#else
	copy(theBuffer, theBuffer + theLength, theResult);
#endif

	return XalanDOMString(theResult, theLength);
#endif
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XalanDOMString)
UnsignedLongToDOMString(unsigned long	theUnsignedLong)
{
#if 1

	XalanDOMChar				theBuffer[MAX_PRINTF_DIGITS + 1];

	XalanDOMChar*				thePointer = &theBuffer[MAX_PRINTF_DIGITS];
	const XalanDOMChar* const	theEnd = thePointer;

	// Null terminate it...
	*thePointer = 0;

	do
	{
		// Next spot...
		--thePointer;

		// Isolate the left most character.
		*thePointer = XalanDOMChar(theUnsignedLong % 10 + XalanUnicode::charDigit_0);

		// OK, we're done with it...
		theUnsignedLong /= 10;
	}
	while(theUnsignedLong != 0);

	return XalanDOMString(thePointer, theEnd - thePointer);

#elif defined(XALAN_USE_WCHAR_SUPPORT)

	wchar_t		theBuffer[MAX_PRINTF_DIGITS + 1];

	swprintf(theBuffer,
			 L"%lu",
			 theUnsignedLong);

	return XalanDOMString(theBuffer, length(theBuffer));

#else

	char		theBuffer[MAX_PRINTF_DIGITS + 1];

	ostrstream	theFormatter(theBuffer, sizeof(theBuffer));

	theFormatter << theUnsignedLong << '\0';

	wchar_t		theResult[MAX_PRINTF_DIGITS + 1];

	const unsigned int	theLength = length(theBuffer);

#if defined(XALAN_NO_ALGORITHMS_WITH_BUILTINS)
	XalanCopy(theBuffer, theBuffer + theLength, theResult);
#else
	copy(theBuffer, theBuffer + theLength, theResult);
#endif

	return XalanDOMString(theResult, theLength);
#endif
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
isWhiteSpace(const XalanDOMString&	string)
{
	const unsigned int	theLength = length(string);

	for(unsigned int s = 0; s < theLength;  s++) 
	{
		if (!isSpace(charAt(string, s)))
			return false;
	}

	return true;
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
isWhiteSpace(
			const XalanDOMChar*		ch,
			unsigned int			start,
			unsigned int			length)
{
	const unsigned int	end = start + length;

	for(unsigned int s = start; s < end; s++) 
	{
		if (!isSpace(ch[s]))	
			return false;
	}

	return true;
}



static bool
doTranscodeToLocalCodePage(
			const XalanDOMChar*		sourceString,
			unsigned int			sourceStringLength,
			bool					sourceStringIsNullTerminated,
			CharVectorType&			targetVector,
			bool					terminate)
{
    // Short circuit if it's a null pointer, or of length 0.
    if (!sourceString || (!sourceString[0]))
    {
		if (terminate == true)
		{
			targetVector.resize(1);

			targetVector.back() = '\0';
		}
		else
		{
			targetVector.resize(0);
		}

        return true;
	}

	const XalanDOMChar*		tempSource = 0;

	// If our char sizes are not the same, we have to use a temp buffer.
	XalanArrayAutoPtr<wchar_t>	tempSourceJanitor;

#if !defined(XALAN_XALANDOMCHAR_USHORT_MISMATCH)
	// This is a short-cut for when the sourceString is mull-terminated _and_
	// XalanDOMChar and wchar_t are the same thing.
	if (sourceStringIsNullTerminated == true)
	{
		tempSource = sourceString;
	}
	else
#endif
	{
		if (sourceStringIsNullTerminated == true)
		{
			sourceStringLength = length(sourceString);
		}

		tempSourceJanitor.reset(new wchar_t[sourceStringLength + 1]);

		for (unsigned int index = 0; index < sourceStringLength; ++index)
		{
			tempSourceJanitor[index] = wchar_t(sourceString[index]);
		}

		tempSourceJanitor[sourceStringLength] = 0;

		tempSource = tempSourceJanitor.get();
	}

    // See now many chars we need to transcode.
    const unsigned int targetLen = ::wcstombs(0, tempSource, 0);

	// Resize, adding one byte if terminating...
	targetVector.resize(terminate == true ? targetLen + 1 : targetLen);

    //  And transcode our temp source buffer to the local buffer. Terminate
    //
    if (wcstombs(&targetVector[0], tempSource, targetLen) == size_t(-1))
	{
		return false;
	}
	else
	{
		if (terminate == true)
		{
			targetVector.back() = '\0';
		}

		return true;
	}
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
TranscodeToLocalCodePage(
			const XalanDOMChar*		sourceString,
			unsigned int			sourceStringLength,
			CharVectorType&			targetVector,
			bool					terminate)
{
#if 1
	return doTranscodeToLocalCodePage(sourceString, sourceStringLength, false, targetVector, terminate);
#else
    // Short circuit if it's a null pointer, or of length 0.
    if (!sourceString || (!sourceString[0]))
    {
		if (terminate == true)
		{
			targetVector.resize(1);

			targetVector.back() = '\0';
		}
		else
		{
			targetVector.resize(0);
		}

        return true;
	}

	// If our char sizes are not the same, we have to use a temp buffer.
	XalanArrayAutoPtr<wchar_t>	tempSourceJanitor(new wchar_t[sourceStringLength + 1]);

	for (unsigned int index = 0; index < sourceStringLength; ++index)
	{
		tempSourceJanitor[index] = wchar_t(sourceString[index]);
	}

	tempSourceJanitor[sourceStringLength] = 0;

	const XalanDOMChar*	const	tempSource = tempSourceJanitor.get();

    // See now many chars we need to transcode.
    const unsigned int targetLen = ::wcstombs(0, tempSource, 0);

	// Resize, adding one byte if terminating...
	targetVector.resize(terminate == true ? targetLen + 1 : targetLen);

    //  And transcode our temp source buffer to the local buffer. Terminate
    //
    if (wcstombs(&targetVector[0], tempSource, targetLen) == size_t(-1))
	{
		return false;
	}
	else
	{
		if (terminate == true)
		{
			targetVector.back() = '\0';
		}

		return true;
	}
#endif
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
TranscodeToLocalCodePage(
			const XalanDOMChar*		sourceString,
			CharVectorType&			targetVector,
			bool					terminate)
{
#if 1
	return doTranscodeToLocalCodePage(sourceString, 0, true, targetVector, terminate);
#else
    // Short circuit if it's a null pointer, or of length 0.
    if (!sourceString || (!sourceString[0]))
    {
		if (terminate == true)
		{
			targetVector.resize(1);

			targetVector.back() = '\0';
		}
		else
		{
			targetVector.resize(0);
		}

        return true;
	}

#if !defined(XALAN_XALANDOMCHAR_USHORT_MISMATCH)
	const XalanDOMChar*	const	tempSource = sourceString;
#else
    assert(sizeof(XalanDOMChar) != sizeof(wchar_t));

	const unsigned int			len = length(sourceString);

	// If our char sizes are not the same, we have to use a temp buffer.
	XalanArrayAutoPtr<wchar_t>	tempSourceJanitor(new wchar_t[len + 1]);

	for (unsigned int index = 0; index < len; ++index)
	{
		tempSourceJanitor[index] = wchar_t(sourceString[index]);
	}

	tempSourceJanitor[len] = 0;

	const XalanDOMChar*	const	tempSource = tempSourceJanitor.get();
#endif

    // See now many chars we need to transcode.
    const unsigned int targetLen = ::wcstombs(0, tempSource, 0);

	// Resize, adding one byte if terminating...
	targetVector.resize(terminate == true ? targetLen + 1 : targetLen);

    //  And transcode our temp source buffer to the local buffer. Terminate
    //
    if (wcstombs(&targetVector[0], tempSource, targetLen) == size_t(-1))
	{
		return false;
	}
	else
	{
		if (terminate == true)
		{
			targetVector.back() = '\0';
		}

		return true;
	}
#endif
}
