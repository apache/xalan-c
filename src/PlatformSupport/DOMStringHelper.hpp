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
#include <cctype>
#include <cstdio>
#include <functional>
#include <iosfwd>
#include <vector>
#include <string>



#include <dom/DOMString.hpp>
#include <util/XMLString.hpp>


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


// Simulates the java String method indexOf().  Returns the index of theChar
// in theString, or -1 if the character is not found.
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(int)
indexOf(
			const DOMString&	theString,
			XMLCh				theChar);



// Simulates the java String method indexOf().  Returns the index of theSubString
// in theString, or -1 if the string is not found.
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(int)
indexOf(
			const DOMString&	theString,
			const DOMString&	theSubString);


// Simulates the java String method lastIndexOf().  Returns the index of theChar
// in theString, or -1 if the character is not found.
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(int)
lastIndexOf(
			const DOMString&	theString,
			XMLCh				theChar);



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
startsWith(
			const DOMString&	theDOMString,
			const DOMString&	theSubString);



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
endsWith(
			const DOMString&	theDOMString,
			const DOMString&	theSubString);



inline int
length(const DOMString&		theString)
{
	return theString.length();
}



inline int
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



inline bool 
isEmpty(const DOMString& str)
{
#if defined(__GNUC__)
	// @@ JMD: linux had problems with this 
 	DOM_NullPtr *nil = 0;
 	assert( (length(str)>0 && str.operator!=(nil)) ||
 			(length(str)==0 && str.operator==(nil)) );
#else			
	assert( (length(str)>0 && str!=0) || (length(str)==0 && str==0) );
#endif	

	return length(str) == 0; 
}



inline bool
startsWith(
			const DOMString&	theDOMString,
			const char*			theSubString)
{
	return startsWith(theDOMString,
					  DOMString(theSubString));
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(void)
OutputString(
			TextOutputStream&	theStream,
			const DOMString&	theString);



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(void)
OutputString(
			 std::ostream&		theStream,
			 const DOMString&	theString);



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(DOMString)
DoubleToDOMString(double	theDouble);



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(DOMString)
LongToHexDOMString(long		theInt);



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(DOMString)
LongToDOMString(long	theInt);



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(DOMString)
UnsignedLongToDOMString(unsigned long	theInt);



inline TextOutputStream&
operator<<(
			TextOutputStream&	theStream,
			const DOMString&	theString)
{
	OutputString(theStream,
				 theString);

	return theStream;
}



inline std::ostream&
operator<<(
			std::ostream&		theStream,
			const DOMString&	theString)
{
	OutputString(theStream,
				 theString);

	return theStream;
}



inline DOMString
clone(const DOMString&	theString)
{
	return theString.clone();
}


#if 0
@@ JMD: no longer needed, exist in base class
inline DOMString
operator+(
			const DOMString&	theLHS,
			const DOMString&	theRHS)
{
	return DOMString(theLHS) + theRHS;
}


inline DOMString&
operator+=(
			DOMString&			theLHS,
			const DOMString&	theRHS)
{
	theLHS.appendData(theRHS);

	return theLHS;
}



inline DOMString&
operator+=(
			DOMString&	theLHS,
			XMLCh		theRHS)
{
	theLHS += DOMString(&theRHS, 1);

	return theLHS;
}
#endif // 0



inline bool
isSpace(XMLCh	theChar)
{
	return iswspace(theChar) ? true : false;
}



inline XMLCh
charAt(
			const DOMString&	theString,
			int					theIndex)
{
	return theString.charAt(theIndex);
}



inline bool
isDigit(XMLCh	theChar)
{
	return iswdigit(theChar) ? true : false;
}



inline bool
isLetterOrDigit(XMLCh	theChar)
{
	return iswalnum(theChar) ? true : false;
}



inline DOMString
substring(
			const DOMString&	theString,
			int					theStartIndex,
			int					theEndIndex = -1)
{
	const int	theStringLength = length(theString);

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
		const int	theLength = theEndIndex == -1 ? theStringLength - theStartIndex :
													theEndIndex - theStartIndex;
		assert(theStartIndex + theLength <= theStringLength);

		return theString.substringData(theStartIndex, theLength);
	}
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(DOMString)
toLowerCase(const DOMString&	theString);



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(DOMString)
toUpperCase(const DOMString&	theString);



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(int)
DOMStringToInt(const DOMString&	theString);



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(long)
DOMStringToLong(const DOMString&	theString);



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(double)
DOMStringToDouble(const DOMString&	theString);


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


// Get the underlying representation of the wide string.
// returns a null-terminated string
inline const XMLCh*
c_wstr(const DOMString&		theString)
{
	XMLCh *ptr = theString.rawBuffer();

	assert(!ptr || ptr[theString.length()] == '\0');

	return ptr;
}


// not guaranteed to be null-terminated
inline const XMLCh*
toCharArray(const DOMString&	theString)
{
	return theString.rawBuffer();
}



inline XMLCh*
toCharArray(DOMString&	theString)
{
	return theString.rawBuffer();
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
equals(const XMLCh*	theLHS,
	   const XMLCh*	theRHS);



inline bool
equals(const XMLCh*			theLHS,
	   const DOMString&		theRHS)
{
	return equals(theLHS, c_wstr(theRHS));
}



inline bool
equals(const DOMString&	theLHS,
	   const XMLCh*		theRHS)
{
	return equals(c_wstr(theLHS), theRHS);
}



inline bool
equals(const DOMString&		theLHS,
	   const char*			theRHS)
{
	return theLHS.equals(theRHS) ? true : false;
}



inline bool
equals(
			const DOMString&		theLHS,
			const DOMString&		theRHS)
{
	return theLHS.equals(theRHS) ? true : false;
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
equalsIgnoreCase(
			const DOMString&	theLHS,
			const DOMString&	theRHS);



// Returns 0 for equal strings, less than 0 if theLHS
// is less than theRHS, or greater than 0 if theRHS
// is greater than theLHS.
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(int)
compare(
			const DOMString&	theLHS,
			const DOMString&	theRHS);



inline bool
operator<(
			const DOMString&	theLHS,
			const DOMString&	theRHS)
{
	return compare(theLHS, theRHS) < 0 ? true : false;
}



inline DOMString&
append(
			DOMString&			theString,
			const DOMString&	theStringToAppend)
{
	theString.appendData(theStringToAppend);

	return theString;
}



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(DOMString)
trim(const DOMString&	theString);



inline void
clear(
			DOMString&	theString,
			int			theOffset = 0,
			int			theLength = -1)
{
	assert(theOffset >= 0);
	assert(theLength == -1 ||
			theLength <= length(theString) - theOffset);

	theString.deleteData(theOffset, theLength >= 0 ? 
										theLength :
										length(theString) - theOffset);
}



inline void
setCharAt(
			DOMString&	theString,
			int			theIndex,
			XMLCh		theChar)
{
	assert(theIndex < length(theString));

	XMLCh* const	theBuffer = toCharArray(theString);

	theBuffer[theIndex] = theChar;
}



// A standard vector of XMLChs
typedef std::vector<XMLCh>	XMLCharVectorType;



// Utility function to make a 0-terminated vector of XMLChs, from
// a 0-terminated array chars, via transcoding.
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XMLCharVectorType)
MakeXMLChVector(const char*		data);



// Utility function to make a 0-terminated vector of XMLChs, from
// a 0-terminated array of them.
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(XMLCharVectorType)
MakeXMLChVector(const XMLCh*		data);



// An inline version for DOMStrings....
inline XMLCharVectorType
MakeXMLChVector(const DOMString&		data)
{
	return MakeXMLChVector(c_wstr(data));
}



struct c_wstr_functor : public std::unary_function<DOMString, const XMLCh*>
{
	result_type
	operator() (const argument_type&	theString) const
	{
		return c_wstr(theString);
	}
};



// Hash functor for DOMStrings
struct DOMStringHashFunction : public std::unary_function<const DOMString&, size_t>
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



// Equals functor for DOMStrings
struct DOMStringEqualsFunction : public std::binary_function<const DOMString&, const DOMString&, bool>
{
	result_type
	operator() (first_argument_type		theLHS,
				second_argument_type	theRHS) const
	{
		return theLHS.equals(theRHS);
	}
};



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(std::string)
DOMStringToStdString(const DOMString& domString);


#if defined(__GNUC__)

// Linux GNU C++ doesn't support wstring
#else			

inline std::wstring
DOMStringToStdWString(const DOMString& domString)
{
	return (c_wstr(domString));
}

#endif	



//Is the string just whitespace?
XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
isWhiteSpace(const DOMString&	string);



XALAN_PLATFORMSUPPORT_EXPORT_FUNCTION(bool)
isWhiteSpace(
			const XMLCh*	ch,
			int				start,
			int				length);



#endif	// DOMSTRINGHELPER_HEADER_GUARD_1357924680
