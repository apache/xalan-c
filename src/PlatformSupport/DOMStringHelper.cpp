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
#include <cstdlib>
#include <iosfwd>



#include <strstream>
#include <ostream>
#include <vector>



#if !defined(XALAN_NO_NAMESPACES)
using std::hex;
using std::ostream;
using std::ostrstream;
using std::string;
//using std::stringstream;
using std::vector;
using std::wstring;
#endif



// Xerces header files
#include <util/Janitor.hpp>
#include <util/TextOutputStream.hpp>
#include <util/XMLString.hpp>

#if !defined(XML_LSTRSUPPORT)
#include <util/PlatformUtils.hpp>
#endif


#include "DoubleSupport.hpp"



#if !defined(XML_LSTRSUPPORT)

// This string is defined just to make sure that
// _something_ trips the initialization code
// before main() is entered.
const DOMString		theDummy(XALAN_STATIC_UCODE_STRING("dummy"));


XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(const DOMString)
initializeAndTranscode(const char*	theString)
{
	static bool		fInitialized = false;

	if (fInitialized == false)
	{
		XMLPlatformUtils::Initialize();

		fInitialized = true;
	}

	return DOMString(theString);
}

#endif



// Simulates the java String method indexOf().  Returns the index of theChar
// in theString, or -1 if the character is not found.
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(int)
indexOf(const DOMString&	theString,
		XMLCh				theChar)
{
	const int	theLength = length(theString);

	if (theLength == 0)
	{
		return -1;
	}
	else
	{
		int			theIndex = 0;

		while(theIndex < theLength &&
			  charAt(theString, theIndex) != theChar)
		{
			theIndex++;
		}

		return theIndex == theLength ? -1 : theIndex;
	}
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(int)
indexOf(const DOMString&	theFirstString,
		const DOMString&	theSecondString)
{
	const int	theFirstStringLength = length(theFirstString);
	const int	theSecondStringLength = length(theSecondString);

	// If the substring is longer than the string, then
	// it's not a substring.
	if (theFirstStringLength < theSecondStringLength)
	{
		return -1;
	}
	else
	{
		bool			fMatch = false;

		int				theFirstStringIndex = 0;

		// While we haven't matched, and we haven't finished with the
		// first string, and the number of characters left in the first
		// string is greater than the length of the second string, try
		// to match the strings.
		while(fMatch == false &&
			  theFirstStringIndex < theFirstStringLength &&
			  theFirstStringLength - theFirstStringIndex >= theSecondStringLength)
		{
			// We always start over from the beginning of the second string.
			int		theSecondStringIndex = 0;

			// This variable will be incremented to index into the first
			// string.  That way, we preserve the first string index for
			// when we have to restart the following loop with the next
			// position in the first string.
			int		theOffset = 0;

			// Compare the characters in the two strings, at the
			// current indices, until the characters don't match.
			while(theFirstStringIndex < theFirstStringLength &&
				  theSecondStringIndex < theSecondStringLength &&
				  charAt(theFirstString, theFirstStringIndex + theOffset) ==
						charAt(theSecondString, theSecondStringIndex))
			{
				theOffset++;
				theSecondStringIndex++;
			}

			// If we've reached the end of the second string,
			// then we've found a match.
			if (theSecondStringIndex == theSecondStringLength)
			{
				fMatch = true;
			}
			else
			{
				theFirstStringIndex++;
			}
		}

		return fMatch == false ? -1 : theFirstStringIndex;
	}
}


// Simulates the java String method lastIndexOf().  Returns the index of theChar
// in theString, or -1 if the character is not found.
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(int)
lastIndexOf(const DOMString& theString, XMLCh theChar)
{
	const int theLength = length(theString);

	if (theLength == 0)
	{
		return -1;
	}
	else
	{
		int theIndex = theLength-1;

		while(theIndex >=0 &&  charAt(theString, theIndex) != theChar)
		{
			theIndex--;
		}

		return theIndex == -1 ? -1 : theIndex;
	}
}







XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
startsWith(const DOMString&		theDOMString,
		   const DOMString&		theSubString)
{
	bool		fResult = false;

	const int	theStringLength = length(theDOMString);
	assert(theStringLength >= 0);

	const int	theSubStringLength = length(theSubString);
	assert(theSubStringLength >= 0);

	// If either string is of length 0, or if the substring
	// is longer, there's no point in continuing.
	if (theStringLength >= theSubStringLength)
	{
		// Compare each character...
		for (int i = 0;
				i < theSubStringLength &&
						charAt(theDOMString, i) == charAt(theSubString, i);
					i++)
		{
			;
		}

		// If we've gotten to the end of the substring, then
		// return true.
		if (i == theSubStringLength)
		{
			fResult = true;
		}
	}

	return fResult;
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
endsWith(
			const DOMString&	theDOMString,
			const DOMString&	theSubString)
{
	bool		fResult = false;

	const int	theStringLength = length(theDOMString);
	assert(theStringLength >= 0);

	const int	theSubStringLength = length(theSubString);
	assert(theSubStringLength >= 0);

	// If either string is of length 0, or if the substring
	// is longer, there's no point in continuing.
	if (theStringLength >= theSubStringLength)
	{
		int		i = theStringLength - 1;

		// Compare each character...
		for (int j = theSubStringLength - 1;
				j >= 0 &&
						charAt(theDOMString, i) == charAt(theSubString, j);
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
OutputString(TextOutputStream&	theStream,
			 const DOMString&	theString)
{
	if (isEmpty(theString) == false)
	{
		theStream << c_wstr(theString);
	}
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(void)
OutputString(
			 ostream&			theStream,
			 const DOMString&	theString)
{
	char* const		theTranscodedString =
		theString.transcode();

	ArrayJanitor<char>	theJanitor(theTranscodedString);

	theStream << theTranscodedString;
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(DOMString)
toLowerCase(const DOMString&	theString)
{
	const int	theStringLength = length(theString);
	assert(theStringLength >= 0);

	if (theStringLength == 0)
	{
		return theString;
	}
	else
	{
		DOMString	theLowerCaseString(theString.clone());

		XMLCh* const	theBuffer = theLowerCaseString.rawBuffer();
		assert(theBuffer != 0);

		for(int i = 0; i < theStringLength; i++)
		{
			theBuffer[i] = towlower(theBuffer[i]);
		}

		return theLowerCaseString;
	}
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(DOMString)
toUpperCase(const DOMString&	theString)
{
	const int	theStringLength = length(theString);
	assert(theStringLength >= 0);

	if (theStringLength == 0)
	{
		return theString;
	}
	else
	{
		DOMString	theLowerCaseString(theString.clone());

		XMLCh* const	theBuffer = theLowerCaseString.rawBuffer();
		assert(theBuffer != 0);

		for(int i = 0; i < theStringLength; i++)
		{
			theBuffer[i] = towupper(theBuffer[i]);
		}

		return theLowerCaseString;
	}
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
equals(const XMLCh*	theLHS,
	   const XMLCh*	theRHS)
{
	assert(theLHS != 0 && theRHS != 0);

	while(*theLHS != 0 && *theRHS != 0 && *theLHS == *theRHS)
	{
		theLHS++;
		theRHS++;
	}

	return *theLHS == *theRHS ? true : false;
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
equalsIgnoreCase(const DOMString&	theLHS,
				 const DOMString&	theRHS)
{
	bool				fResult = false;

	const unsigned int	theLength = length(theLHS);

	// If they are equal, then compare
	if (theLength == length(theRHS))
	{
		// Check each character, converting to uppercase
		// for the test.
		for(unsigned int i = 0; i < theLength; i++)
		{
			if (towupper(charAt(theLHS, i)) !=
						towupper(charAt(theRHS, i)))
			{
				break;
			}
		}

		// Did we reach the end of the string?
		if (i == theLength)
		{
			fResult = true;
		}
	}

	return fResult;
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(int)
compare(
			const DOMString&	theLHS,
			const DOMString&	theRHS)
{
	const int	theLHSLength = theLHS.length();
	const int	theRHSLength = theRHS.length();

	int			theResult = 0;

	XMLCh		theLHSChar = 0;
	XMLCh		theRHSChar = 0;

	if (theLHSLength == 0 && theRHSLength == 0) return 0;

	for(int i = 0; i < theLHSLength && i < theRHSLength; i++)
	{
		theLHSChar = charAt(theLHS, i);
		theRHSChar = charAt(theRHS, i);

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

	return theResult;
}



static void
CopyDOMStringToVector(const DOMString&		theString,
					  vector<char>&			theVector)
{
	const int	theLength = length(theString);

	if (theLength != 0)
	{
		theVector.reserve(theLength + 1);

		for(int	i = 0; i < theLength; i++)
		{
#if defined(XALAN_OLD_STYLE_CASTS)
			// Assert that the truncation will not affect the resulting character.
			assert(charAt(theString, i) == (char)charAt(theString, i));

			theVector.push_back((char)charAt(theString, i));
#else
			// Assert that the truncation will not affect the resulting character.
			assert(charAt(theString, i) == static_cast<char>(charAt(theString, i)));

			theVector.push_back(static_cast<char>(charAt(theString, i)));
#endif
		}

		// Put a terminating 0 byte.
		theVector.push_back(0);
	}
}




XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(int)
DOMStringToInt(const DOMString&	theString)
{
	int				theResult = 0;

	vector<char>	theVector;

	CopyDOMStringToVector(theString,
						  theVector);

	if (theVector.size() > 0)
	{
		theResult = atoi(theVector.begin());
	}

	return theResult;
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(long)
DOMStringToLong(const DOMString&	theString)
{
	long			theResult = 0;

	vector<char>	theVector;

	CopyDOMStringToVector(theString,
						  theVector);

	if (theVector.size() > 0)
	{
		theResult = atol(theVector.begin());
	}

	return theResult;
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(double)
DOMStringToDouble(const DOMString&	theString)
{
	double			theResult = DoubleSupport::getNaN();

	if (length(theString) > 0)
	{
		typedef vector<char>	VectorType;

		VectorType				theVector;

		CopyDOMStringToVector(theString,
							  theVector);

		if (theVector.size() > 0)
		{
			// $$$ ToDo: We really need better validation here.  And better
			// localization as well.
			bool	fError = false;

			VectorType::const_iterator	i = theVector.begin();
			VectorType::const_iterator	j = theVector.end();

			j--;

			do
			{
				if ((*i < '0' || *i > '9') && !(*i == '.' || *i == 'e' || *i == 'E' || *i == 'f' || *i == 'F' || *i == '-'))
				{
					fError = true;
				}
				else
				{
					i++;
				}
			}
			while(i != j && fError == false);

			if (fError == false)
			{
				theResult = atof(theVector.begin());
			}
		}
	}

	return theResult;
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(DOMString)
trim(const DOMString&	theString)
{
	if (isEmpty(theString))
		return theString;

	const int strLen = theString.length();
	
	// index of first non-whitespace character
	int leadingSpace = 0;
	for (leadingSpace=0; leadingSpace<strLen; leadingSpace++)
		if (!isSpace(theString.charAt(leadingSpace)))
			break;

	// index of last non-whitespace character
	int trailingSpace = 0;
	for (trailingSpace=strLen-1; trailingSpace>=0; trailingSpace--)
		if (!isSpace(theString.charAt(trailingSpace)))
			break;

	return substring(theString,leadingSpace,trailingSpace+1);
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(DOMString)
DoubleToDOMString(double	theDouble)
{
	if (DoubleSupport::isNaN(theDouble) == true)
	{
		return "NaN";
	}
	else if (DoubleSupport::isPositiveInfinity(theDouble) == true)
	{
		return "Infinity";
	}
	else if (DoubleSupport::isNegativeInfinity(theDouble) == true)
	{
		return "-Infinity";
	}
	else
	{
		ostrstream	theFormatter;

		// $$$ ToDo: this is all temporary, until we get the NumberFormat and DecimalFormat
		// classes working.
		// According to the XPath standard, any values without
		// a fractional part are printed as integers.
		double	intPart = 0;

		double	fracPart = fabs(modf(theDouble, &intPart));

		string	theResult;

		if (fracPart == 0)
		{
#if defined(XALAN_OLD_STYLE_CASTS)
			theFormatter << (long)theDouble << '\0';

#else
			theFormatter << static_cast<long>(theDouble) << '\0';

#endif
			theResult = theFormatter.str();
		}
		else
		{
			theFormatter << theDouble << '\0';

			// OK, now we have to clean up the output for
			// the XPath standard, which says no trailing
			// '0's for the decimal portion.  So start with
			// the last digit, and replace any '0's with the
			// null character.  We know at this point that
			// we have at least 1 digit before the decimal
			// point, and and least 1 non-zero digit after
			// the decimal point, since any values with no
			// fractional part were printed as integers
			theResult = theFormatter.str();

			string::iterator	thePosition = theResult.end();

			while(*--thePosition == '0')
			{
			}

			theResult.erase(++thePosition);
		}

		return theResult.c_str();
	}
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(DOMString)
LongToHexDOMString(long		theLong)
{
	ostrstream	theFormatter;

	theFormatter << hex << theLong << '\0';

	return theFormatter.str();
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(DOMString)
LongToDOMString(long	theLong)
{
	ostrstream	theFormatter;

	theFormatter << theLong << '\0';

	return theFormatter.str();
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(DOMString)
UnsignedLongToDOMString(unsigned long	theUnsignedLong)
{
	ostrstream	theFormatter;

	theFormatter << theUnsignedLong << '\0';

	return theFormatter.str();
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XMLCharVectorType)
MakeXMLChVector(const char*		data)
{
	assert(data != 0);

	XMLCh*	theTranscodedData = XMLString::transcode(data);

	ArrayJanitor<XMLCh>		theJanitor(theTranscodedData);

	// Create a vector which includes the terminating 0.
	return MakeXMLChVector(theTranscodedData);
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XMLCharVectorType)
MakeXMLChVector(const XMLCh*		data)
{
	assert(data != 0);

	const XMLCh*	theEnd = data;

	// Find the terminating 0.
	while(*theEnd)
	{
		theEnd++;
	}

	// Create a vector which includes the terminating 0.
	return XMLCharVectorType(data, theEnd + 1);
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
isWhiteSpace(const DOMString& string)
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
			const XMLCh* const	ch,
			unsigned int		start,
			unsigned int		length)
{
	const unsigned int	end = start + length;

	for(unsigned int s = start; s < end; s++) 
	{
		if (!isSpace(ch[s]))	
			return false;
	}

	return true;
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(string)
DOMStringToStdString(const DOMString& domString)
{
	XMLCh* toTranscode = domString.rawBuffer();
	unsigned int len = domString.length();


    // Short circuit if its a null pointer
    if (!toTranscode || (!toTranscode[0]))
    {
        return string();
	}

    // See if our XMLCh and wchar_t as the same on this platform
    const bool isSameSize = (sizeof(XMLCh) == sizeof(wchar_t));

    //
    //  Get the actual number of chars. If the passed len is zero, its null
    //  terminated. Else we have to use the len.
    //
    wchar_t realLen = (wchar_t)len;
    if (!realLen)
    {
        //
        //  We cannot just assume we can use wcslen() because we don't know
        //  if our XMLCh is the same as wchar_t on this platform.
        //
        const XMLCh* tmpPtr = toTranscode;
        while (*(tmpPtr++))
            realLen++;
    }

    //
    //  If either the passed length was non-zero or our char sizes are not 
    //  same, we have to use a temp buffer. Since this is common in these
    //  samples, we just do it anyway.
    //
    wchar_t* const	tmpSource = new wchar_t[realLen + 1];

	ArrayJanitor<wchar_t>	tmpSourceJanitor(tmpSource);

    if (isSameSize)
    {
        memcpy(tmpSource, toTranscode, realLen * sizeof(wchar_t));
    }
	else
    {
        for (unsigned int index = 0; index < realLen; index++)
            tmpSource[index] = (wchar_t)toTranscode[index];
    }
    tmpSource[realLen] = 0;

    // See now many chars we need to transcode this guy
    const unsigned int targetLen = ::wcstombs(0, tmpSource, 0);

    // Allocate out storage member
    char* const		localForm = new char[targetLen + 1];

	ArrayJanitor<char>	localFormJanitor(localForm);

    //
    //  And transcode our temp source buffer to the local buffer. Cap it
    //  off since the converter won't do it (because the null is beyond
    //  where the target will fill up.)
    //
    ::wcstombs(localForm, tmpSource, targetLen);
    localForm[targetLen] = 0;

	return localForm;
}
