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
#if !defined(DOMSTRINGHELPER_HEADER_GUARD_1357924680)
#define DOMSTRINGHELPER_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <PlatformSupport/PlatformSupportDefinitions.hpp>



#include <cassert>
#include <functional>
#include <iosfwd>
#include <vector>
#include <string>

#if defined(__GNUC__)
#include <wctype.h>
#endif


#include <dom/DOMString.hpp>



class TextOutputStream;



// This macro has been defined to deal with certain C++ compilers which
// do not create Unicode strings when the "L" string constant prefix is
// used.  It is meant _only_ for use with static strings.
// It is _not_ designed to be thread-safe, because there will always be
// at least one global static transcoded string that will trigger the
// code at startup.
#if defined(XML_LSTRSUPPORT)

#define XALAN_STATIC_UCODE_STRING(str) L##str

#else

// Makes sure the Xerces platform is initialized, then
// transcodes the string.
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(const DOMString)
initializeAndTranscode(const char*	theString);

#define XALAN_STATIC_UCODE_STRING(str) initializeAndTranscode(str)

#endif


/**
 * Simulates the java String method indexOf().
 * 
 * @param theString string to search
 * @param theChar   character searched for
 * @return the index of theChar in theString, or -1 if the character is not
 * found.    
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(int)
indexOf(
			const DOMString&	theString,
			XMLCh				theChar);



/**
 * Simulates the java String method indexOf().
 * 
 * @param theString string to search
 * @param theSubString substring searched for
 * @return the index of theSubString in theString, or -1 if the string is not
 * found.
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(int)
indexOf(
			const DOMString&	theString,
			const DOMString&	theSubString);


/**
 * Simulates the java String method lastIndexOf().
 * 
 * @param theString string to search
 * @param theChar   character searched for
 * @return the index of theChar in theString, or -1 if the character is not
 * found.    
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(int)
lastIndexOf(
			const DOMString&	theString,
			XMLCh				theChar);



/**
 * Simulates the java String method startsWith().
 * 
 * @param theDOMString target string to search
 * @param theSubString substring searched for
 * @return true if the target string begins with the substring
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
startsWith(
			const DOMString&	theDOMString,
			const DOMString&	theSubString);



/**
 * Simulates the java String method startsWith().
 * 
 * @param theDOMString target string to search
 * @param theSubString substring searched for
 * @return true if the target string begins with the substring
 */
inline bool
startsWith(
			const DOMString&	theDOMString,
			const char*			theSubString)
{
	return startsWith(theDOMString,
					  DOMString(theSubString));
}


/**
 * Simulates the java String method endsWith().
 * 
 * @param theDOMString target string to search
 * @param theSubString substring searched for
 * @return true if the target string ends with the substring
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
endsWith(
			const DOMString&	theDOMString,
			const DOMString&	theSubString);



/**
 * Simulates the java String method length() for a DOMString
 * 
 * @param theDOMString target string
 * @return the length of the target string
 */
inline unsigned int
length(const DOMString&		theString)
{
	return theString.length();
}



/**
 * Simulates the java String method length() for a null-terminated buffer of
 * XMLCh characters
 * 
 * @param theBuffer target string
 * @return the length of the target string
 */
inline unsigned int
length(const XMLCh*		theBuffer)
{
	assert(theBuffer != 0);

	const XMLCh*	theBufferPointer = theBuffer;

	while(*theBufferPointer != 0)
	{
		theBufferPointer++;
	}

	return theBufferPointer - theBuffer;
}



/**
 * Determines if the target string contains any elements
 * 
 * @param str target string
 * @return true if the target string has a non-zero length
 */
inline bool 
isEmpty(const DOMString& str)
{
	return length(str) == 0 ? true : false; 
}




/**
 * Converts a double value into a DOMString
 * 
 * @param theDouble number to be converted
 * @return decimal string representation of the number
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(DOMString)
DoubleToDOMString(double	theDouble);



/**
 * Converts a long value into a DOMString
 * 
 * @param theInt number to be converted
 * @return hexadecimal string representation of the number
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(DOMString)
LongToHexDOMString(long		theInt);



/**
 * Converts a long value into a DOMString
 * 
 * @param theInt number to be converted
 * @return decimal string representation of the number
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(DOMString)
LongToDOMString(long	theInt);



/**
 * Converts an unsigned long value into a DOMString
 * 
 * @param theInt number to be converted
 * @return decimal string representation of the number
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(DOMString)
UnsignedLongToDOMString(unsigned long	theInt);



/**
 * Converts a DOMString into an integer value
 * 
 * @param theString target string
 * @return integer value of target string
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(int)
DOMStringToInt(const DOMString&	theString);



/**
 * Converts a DOMString into a long value
 * 
 * @param theString target string
 * @return long value of target string
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(long)
DOMStringToLong(const DOMString&	theString);



/**
 * Converts a DOMString into a double value
 * 
 * @param theString target string
 * @return double value of target string
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(double)
DOMStringToDouble(const DOMString&	theString);


/**
 * Outputs the target string to the specified stream
 * 
 * @param theStream output stream
 * @param theString target string
 * @see operator<<
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(void)
OutputString(
			TextOutputStream&	theStream,
			const DOMString&	theString);



/**
 * Outputs the target string to the specified stream
 * 
 * @param theStream output stream
 * @param theString target string
 * @see operator<<
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(void)
OutputString(
#if defined(XALAN_NO_NAMESPACES)
			 ostream&			theStream,
#else
			 std::ostream&		theStream,
#endif
			 const DOMString&	theString);



/**
 * Outputs the target string to the specified stream
 * 
 * @param theStream output stream
 * @param theString target string
 * @see OutputString
 */
inline TextOutputStream&
operator<<(
			TextOutputStream&	theStream,
			const DOMString&	theString)
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
#if defined(XALAN_NO_NAMESPACES)
inline ostream&
#else
inline std::ostream&
#endif
operator<<(
			std::ostream&		theStream,
			const DOMString&	theString)
{
	OutputString(theStream,
				 theString);

	return theStream;
}



/**
 * Creates a copy of the target string
 * 
 * @param theString target string
 * @return copy of string
 */
inline DOMString
clone(const DOMString&	theString)
{
	return theString.clone();
}



/**
 * Determines whether character represents white space
 * 
 * @param theChar target character
 * @return true if character represents white space
 */
inline bool
isSpace(XMLCh	theChar)
{
	return iswspace(theChar) ? true : false;
}



/**
 * Retrieves a character at a specified index in the target string
 * 
 * @param theString target string
 * @param theIndex  index of character
 * @return character at specified index
 */
inline XMLCh
charAt(
			const DOMString&	theString,
			unsigned int		theIndex)
{
	return theString.charAt(theIndex);
}



/**
 * Determines whether character represents a digit
 * 
 * @param theChar target character
 * @return true if character represents a digit
 */
inline bool
isDigit(XMLCh	theChar)
{
	return iswdigit(theChar) ? true : false;
}



/**
 * Determines whether character represents a letter or digit
 * 
 * @param theChar target character
 * @return true if character represents a letter or digit
 */
inline bool
isLetterOrDigit(XMLCh	theChar)
{
	return iswalnum(theChar) ? true : false;
}



/**
 * Simulates the java String method substring(). Returns a new string that is
 * a substring of this string. The substring begins at the specified
 * theStartIndex and extends to the character at index theEndIndex - 1. Thus
 * the length of the substring is theEndIndex-theStartIndex.
 * 
 * @param theString     target string
 * @param theStartIndex starting index, inclusive
 * @param theEndIndex   ending index, exclusive
 * @return string containing the specified range of characters from target
 */
inline DOMString
substring(
			const DOMString&	theString,
			unsigned int		theStartIndex,
			unsigned int		theEndIndex = UINT_MAX)
{
	const unsigned int	theStringLength = length(theString);

	// $$$ ToDo: In Java-land, any failing of these
	// assertions would result in an exception being thrown.
	assert(theStartIndex <= theStringLength && theStartIndex >= 0);

	if (theStartIndex == theStringLength)
	{
		// This is allowed, and should return an empty string.
		return DOMString();
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
			const XMLCh *ptr = theString.rawBuffer();
			XMLCh *newStr = new XMLCh[theLength+1];
			for (size_t u = 0; u < theLength; u++)
				newStr[u] = ptr[u];
			newStr[u] = 0;
			DOMString domStr = newStr;
			delete []newStr;
			return domStr;
		}
		return theString.substringData(theStartIndex, theLength);
	}
}



/**
 * Simulates the java String method toLowerCase(). Returns a new string
 * containing only lower case characters of target string.
 * 
 * @param theString target string
 * @return string containing lower case characters
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(DOMString)
toLowerCase(const DOMString&	theString);



/**
 * Simulates the java String method toUpperCase(). Returns a new string
 * containing only upper case characters of target string.
 * 
 * @param theString target string
 * @return string containing upper case characters
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(DOMString)
toUpperCase(const DOMString&	theString);



// These two function are specifically not defined, and
// should produce ambiguity during compilation.  This
// is necessary because the Xerces DOMString class
// defines == as referring to the same underlying
// handle, not identical strings, as in Java.
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
operator==(
			const DOMString&		theLHS,
			const DOMString&		theRHS);



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
operator!=(
			const DOMString&		theLHS,
			const DOMString&		theRHS);




/**
 * Get the underlying representation of the target DOMString as a
 * null-terminated string                         
 * 
 * @param theString target string
 * @return null-terminated string of XMLCh
 */
inline const XMLCh*
c_wstr(const DOMString&		theString)
{
	XMLCh *ptr = theString.rawBuffer();

	assert(!ptr || ptr[theString.length()] == '\0');

	return ptr;
}


/**
 * Get the underlying representation of the target DOMString as an array of
 * XMLCh, not guaranteed to be null-terminated.
 * 
 * @param theString target string
 * @return array of XMLCh
 */
inline const XMLCh*
toCharArray(const DOMString&	theString)
{
	return theString.rawBuffer();
}



/**
 * Get the underlying representation of the target DOMString as an array of
 * XMLCh, not guaranteed to be null-terminated.
 * 
 * @param theString target string
 * @return array of XMLCh
 */
inline XMLCh*
toCharArray(DOMString&	theString)
{
	return theString.rawBuffer();
}



/**
 * Compare the contents of two strings for equality
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return true if the contents of both strings are identical
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
equals(const XMLCh*	theLHS,
	   const XMLCh*	theRHS);



/**
 * Compare the contents of two strings for equality
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return true if the contents of both strings are identical
 */
inline bool
equals(const XMLCh*			theLHS,
	   const DOMString&		theRHS)
{
	return equals(theLHS, c_wstr(theRHS));
}



/**
 * Compare the contents of two strings for equality
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return true if the contents of both strings are identical
 */
inline bool
equals(const DOMString&	theLHS,
	   const XMLCh*		theRHS)
{
	return equals(c_wstr(theLHS), theRHS);
}



/**
 * Compare the contents of two strings for equality
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return true if the contents of both strings are identical
 */
inline bool
equals(const DOMString&		theLHS,
	   const char*			theRHS)
{
	return theLHS.equals(theRHS) ? true : false;
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
			const DOMString&		theLHS,
			const DOMString&		theRHS)
{
	return theLHS.equals(theRHS) ? true : false;
}



/**
 * Compare the contents of two strings for equality, without regard for case
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return true if the case-insensitive contents of both strings are identical
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
equalsIgnoreCase(
			const DOMString&	theLHS,
			const DOMString&	theRHS);



/**
 * Compare the contents of two strings for equality
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return Returns 0 for equal strings, less than 0 if theLHS is lexically
 * less than theRHS, or greater than 0 if theRHS is lexically greater than
 * theLHS.
 * @see operator<
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(int)
compare(
			const DOMString&	theLHS,
			const DOMString&	theRHS);



/**
 * Compare the contents of two strings for equality
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return Returns true if theLHS is lexically
 * less than theRHS
 * @see compare
 */
inline bool
operator<(
			const DOMString&	theLHS,
			const DOMString&	theRHS)
{
	return compare(theLHS, theRHS) < 0 ? true : false;
}



/**
 * Concatenate two strings
 * 
 * @param theString         target string
 * @param theStringToAppend string to add to target
 * @return string with contents of 'theStringToAppend' added to target string
 */
inline DOMString&
append(
			DOMString&			theString,
			const DOMString&	theStringToAppend)
{
	theString.appendData(theStringToAppend);

	return theString;
}



/**
 * Remove trailing whitespace
 * 
 * @param theString         target string
 * @return string with contents of target string less trailing whitespace
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(DOMString)
trim(const DOMString&	theString);



/**
 * Remove all elements from target string
 * 
 * @param theString target string
 */
inline void
clear(DOMString&	theString)
{
	theString.deleteData(0, length(theString));
}



/**
 * Replaces a character at a specified index in a DOMString
 * 
 * @param theString target string
 * @param theIndex  index of character
 * @param theChar target character
 */
inline void
setCharAt(
			DOMString&		theString,
			unsigned int	theIndex,
			XMLCh			theChar)
{
	assert(theIndex < length(theString));

	XMLCh* const	theBuffer = toCharArray(theString);

	theBuffer[theIndex] = theChar;
}



// A standard vector of XMLChs
#if defined(XALAN_NO_NAMESPACES)
typedef vector<XMLCh>		XMLCharVectorType;
#else
typedef std::vector<XMLCh>	XMLCharVectorType;
#endif





/**
 * Utility function to make a null-terminated vector of XMLChs, from a
 * null-terminated array of chars, via transcoding.               
 * 
 * @param data array to be converted
 * @return null-terminated vector of XMLCh
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XMLCharVectorType)
MakeXMLChVector(const char*		data);



/**
 * Utility function to make a null-terminated vector of XMLChs, from a
 * null-terminated array of XMLCh.
 * 
 * @param data array to be converted
 * @return null-terminated vector of XMLCh
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XMLCharVectorType)
MakeXMLChVector(const XMLCh*		data);



/**
 * Utility function to make a null-terminated vector of XMLChs, from a
 * DOMString
 * 
 * @param data DOMString to be converted
 * @return null-terminated vector of XMLCh
 */
inline XMLCharVectorType
MakeXMLChVector(const DOMString&		data)
{
	return MakeXMLChVector(c_wstr(data));
}



#if defined(XALAN_NO_NAMESPACES)
struct c_wstr_functor : public unary_function<DOMString, const XMLCh*>
#else
struct c_wstr_functor : public std::unary_function<DOMString, const XMLCh*>
#endif
{
	result_type
	operator() (const argument_type&	theString) const
	{
		return c_wstr(theString);
	}
};



/**
 * Hash functor for DOMStrings
 * 
 * @param theKey DOMString to be hashed
 * @return hash value for DOMString
 */
#if defined(XALAN_NO_NAMESPACES)
struct DOMStringHashFunction : public unary_function<const DOMString&, size_t>
#else
struct DOMStringHashFunction : public std::unary_function<const DOMString&, size_t>
#endif
{
	result_type
	operator() (argument_type	theKey) const
	{
		const XMLCh*		theRawBuffer = c_wstr(theKey);

		unsigned long		theHashValue = 0L; 

		if (theRawBuffer != 0)
		{
			while (*theRawBuffer)
			{
				theHashValue = 5 * theHashValue + *theRawBuffer;

				theRawBuffer++;
			}
		}

		return static_cast<result_type>(theHashValue++);
	}
};



/**
 * Equals functor for DOMStrings
 * 
 * @param theLHS first string to compare
 * @param theRHS second string to compare
 * @return true if the contents of both strings are identical
 */
#if defined(XALAN_NO_NAMESPACES)
struct DOMStringEqualsFunction : public binary_function<const DOMString&, const DOMString&, bool>
#else
struct DOMStringEqualsFunction : public std::binary_function<const DOMString&, const DOMString&, bool>
#endif
{
	result_type
	operator() (first_argument_type		theLHS,
				second_argument_type	theRHS) const
	{
		return theLHS.equals(theRHS);
	}
};



/**
 * Convert DOMString to C++ standard library string
 * 
 * @param theString target string
 * @return C++ standard library string representation of target
 */
#if defined(XALAN_NO_NAMESPACES)
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(string)
#else
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(std::string)
#endif
DOMStringToStdString(const DOMString& domString);



/**
 * Determines if the string contains only whitespace
 * 
 * @param theString target string
 * @return true if string contains only whitespace
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
isWhiteSpace(const DOMString&	string);



/**
 * Determines if a range in an array contains only whitespace
 * 
 * @param ch target array
 * @param start starting index to examine
 * @param length number of characters to examine
 * @return true if specified range contains only whitespace
 */
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
isWhiteSpace(
			const XMLCh*	ch,
			unsigned int	start,
			unsigned int	length);



#endif	// DOMSTRINGHELPER_HEADER_GUARD_1357924680
