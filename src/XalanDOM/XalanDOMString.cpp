/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2000 The Apache Software Foundation.  All rights 
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

#include "XalanDOMString.hpp"



#include <cassert>



#include <Include/XalanAutoPtr.hpp>



#if defined(XALAN_USE_CUSTOM_STRING)



#include <cstdlib>



const XalanDOMChar	XalanDOMString::s_empty = 0;



XalanDOMString::XalanDOMString() :
	m_data()
#if defined(XALAN_DOMSTRING_CACHE_SIZE)
	, m_size(0)
#endif
{
}



XalanDOMString::XalanDOMString(
			const XalanDOMString&	theSource,
			size_type				theStartPosition,
			size_type				theCount) :
	m_data()
#if defined(XALAN_DOMSTRING_CACHE_SIZE)
	, m_size(0)
#endif
{
	if (theSource.length() != 0)
	{
		append(theSource, theStartPosition, theCount);
	}
}



XalanDOMString::XalanDOMString(
			const XalanDOMChar*		theString,
			size_type				theCount) :
	m_data()
#if defined(XALAN_DOMSTRING_CACHE_SIZE)
	, m_size(0)
#endif
{
	assert(theString != 0);

	if (*theString != 0)
	{
		append(theString, theCount);
	}
}



XalanDOMString::XalanDOMString(
			const char*		theString,
			size_type		theCount) :
	m_data()
#if defined(XALAN_DOMSTRING_CACHE_SIZE)
	, m_size(0)
#endif
{
	assert(theString != 0);

	if (*theString != 0)
	{
		if (theCount == size_type(npos))
		{
            TranscodeFromLocalCodePage(theString, m_data, true);
		}
		else
		{
			TranscodeFromLocalCodePage(theString, theCount, m_data, true);
		}

#if defined(XALAN_DOMSTRING_CACHE_SIZE)
		m_size = m_data.size() - 1;
#endif
	}

	invariants();
}



XalanDOMString::XalanDOMString(
			size_type		theCount,
			XalanDOMChar	theChar) :
	m_data()
#if defined(XALAN_DOMSTRING_CACHE_SIZE)
	, m_size(0)
#endif
{
	if (theCount != 0)
	{
		XalanDOMCharVectorType(theCount + 1, theChar).swap(m_data);

		// Null-terminate it...
		m_data.back() = 0;

#if defined(XALAN_DOMSTRING_CACHE_SIZE)
		m_size = theCount;
#endif
	}

	invariants();
}



XalanDOMString&
XalanDOMString::operator=(const XalanDOMChar*	theRHS)
{
	erase();

	append(theRHS, length(theRHS));

	invariants();

	return *this;
}



XalanDOMString&
XalanDOMString::append(
			const XalanDOMChar*		theString,
			size_type				theCount)
{
	const size_type		theLength =
			theCount == size_type(npos) ? length(theString) : theCount;

	if (theLength != 0)
	{
		if (m_data.size() == 0)
		{
			m_data.reserve(theLength + 1);

			m_data.insert(m_data.end(), theString, theString + theLength);

			m_data.push_back(0);

#if defined(XALAN_DOMSTRING_CACHE_SIZE)
			m_size = theLength;
#endif

			assert(length() == theLength);
		}
		else
		{
			m_data.insert(getBackInsertIterator(), theString, theString + theLength);

#if defined(XALAN_DOMSTRING_CACHE_SIZE)
			m_size += theCount;
#endif
		}
	}

	invariants();

	return *this;
}



XalanDOMString&
XalanDOMString::append(
			size_type		theCount,
			XalanDOMChar	theChar)
{
	if (m_data.size() == 0)
	{
		m_data.insert(m_data.end(), theCount + 1, theChar);

		m_data.back() = 0;

#if defined(XALAN_DOMSTRING_CACHE_SIZE)
		m_size = theCount;
#endif

		assert(length() == theCount);
	}
	else
	{
		m_data.insert(getBackInsertIterator(), theCount, theChar);

#if defined(XALAN_DOMSTRING_CACHE_SIZE)
		m_size += theCount;
#endif
	}

	invariants();

	return *this;
}



XalanDOMString&
XalanDOMString::insert(
			size_type				thePosition,
			const XalanDOMChar*		theString,
			size_type				theCount)
{
	m_data.insert(getIteratorForPosition(thePosition), theString, theString + theCount);

#if defined(XALAN_DOMSTRING_CACHE_SIZE)
	m_size += theCount;
#endif

	invariants();

	return *this;
}



XalanDOMString&
XalanDOMString::insert(
			size_type		thePosition,
			size_type		theCount,
			XalanDOMChar	theChar)
{
	m_data.insert(getIteratorForPosition(thePosition), theCount, theChar);

#if defined(XALAN_DOMSTRING_CACHE_SIZE)
	m_size += theCount;
#endif

	invariants();

	return *this;
}



XalanDOMString::iterator
XalanDOMString::insert(
			iterator		thePosition,
			XalanDOMChar	theChar)
{
	m_data.insert(thePosition, theChar);

#if defined(XALAN_DOMSTRING_CACHE_SIZE)
	++m_size;
#endif

	invariants();

	return thePosition;
}



void
XalanDOMString::insert(
			iterator		thePosition,
			size_type		theCount,
			XalanDOMChar	theChar)
{
	m_data.insert(thePosition, theCount, theChar);

#if defined(XALAN_DOMSTRING_CACHE_SIZE)
	m_size += theCount;
#endif

	invariants();
}



void
XalanDOMString::insert(
		iterator		theInsertPosition,
		const_iterator	theFirstPosition,
		const_iterator	theLastPosition)
{
	m_data.insert(theInsertPosition, theFirstPosition, theLastPosition);

#if defined(XALAN_DOMSTRING_CACHE_SIZE)
	m_size = m_data.size() - 1;
#endif

	invariants();
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



int
XalanDOMString::compare(const XalanDOMChar*		theString) const
{
	invariants();

	return doCompare(c_str(), length(), theString, length(theString));
}



int
XalanDOMString::compare(
			size_type				thePosition1,
			size_type				theCount1,
			const XalanDOMChar*		theString,
			size_type				theCount2) const
{
	invariants();

	return doCompare(c_str() + thePosition1, theCount1, theString, theCount2);
}



XalanDOMString::CharVectorType
XalanDOMString::transcode() const
{
	invariants();

	CharVectorType	theResult;

	TranscodeToLocalCodePage(c_str(), length(), theResult, true);

	return theResult;
}



#endif



static inline unsigned int
length(const XalanDOMChar*	theString)
{
	assert(theString != 0);

	const XalanDOMChar*		theStringPointer = theString;

	while(*theStringPointer != 0)
	{
		theStringPointer++;
	}

	return theStringPointer - theString;
}



#if defined(XALAN_USE_CUSTOM_STRING)



bool
XalanDOMString::equals(
			const XalanDOMChar*		theLHS,
			unsigned int			theLHSLength,
			const XalanDOMChar*		theRHS,
			unsigned int			theRHSLength)
{
	if (theLHSLength != theRHSLength)
	{
		return false;
	}
	else if (theLHSLength == 0)
	{
		return true;
	}
	else
	{
		const XalanDOMChar* const	theEnd = theLHS + theLHSLength;

		while(*theLHS == *theRHS)
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
}



XalanDOMString::size_type
XalanDOMString::length(const XalanDOMChar*	theString)
{
	return ::length(theString);
}



#endif



static bool
doTranscodeToLocalCodePage(
			const XalanDOMChar*		theSourceString,
			unsigned int			theSourceStringLength,
			bool					theSourceStringIsNullTerminated,
			CharVectorType&			theTargetVector,
			bool					terminate)
{
    // Short circuit if it's a null pointer, or of length 0.
    if (!theSourceString || (!theSourceString[0]))
    {
		if (terminate == true)
		{
			theTargetVector.resize(1);

			theTargetVector.back() = '\0';
		}
		else
		{
			theTargetVector.resize(0);
		}

        return true;
	}

	const wchar_t*	theTempSource = 0;

	// If our char sizes are not the same, or the input string is not null-terminated,
	// we have to use a temporary buffer.
	XalanArrayAutoPtr<wchar_t>	theTempSourceJanitor;

#if !defined(XALAN_XALANDOMCHAR_USHORT_MISMATCH)
	// This is a short-cut for when the theSourceString is mull-terminated _and_
	// XalanDOMChar and wchar_t are the same thing.
	if (theSourceStringIsNullTerminated == true)
	{
		theTempSource = theSourceString;
	}
	else
#endif
	{
		if (theSourceStringIsNullTerminated == true)
		{
			theSourceStringLength = length(theSourceString);
		}

		theTempSourceJanitor.reset(new wchar_t[theSourceStringLength + 1]);

		for (unsigned int index = 0; index < theSourceStringLength; ++index)
		{
			theTempSourceJanitor[index] = wchar_t(theSourceString[index]);
		}

		theTempSourceJanitor[theSourceStringLength] = 0;

		theTempSource = theTempSourceJanitor.get();
	}

    // See how many chars we need to transcode.
    const size_t	targetLen = ::wcstombs(0, theTempSource, 0);

	if (targetLen == size_t(-1))
	{
		return false;
	}
	else
	{
		// Resize, adding one byte if terminating...
		theTargetVector.resize(terminate == true ? targetLen + 1 : targetLen);

		//  And transcode our temp source buffer to the local buffer. Terminate
		//
		if (wcstombs(&theTargetVector[0], theTempSource, targetLen) == size_t(-1))
		{
			return false;
		}
		else
		{
			if (terminate == true)
			{
				theTargetVector.back() = '\0';
			}

			return true;
		}
	}
}



XALAN_DOM_EXPORT_FUNCTION(bool)
TranscodeToLocalCodePage(
			const XalanDOMChar*		theSourceString,
			unsigned int			theSourceStringLength,
			CharVectorType&			theTargetVector,
			bool					terminate)
{
	return doTranscodeToLocalCodePage(theSourceString, theSourceStringLength, false, theTargetVector, terminate);
}



XALAN_DOM_EXPORT_FUNCTION(bool)
TranscodeToLocalCodePage(
			const XalanDOMChar*		theSourceString,
			CharVectorType&			theTargetVector,
			bool					terminate)
{
	return doTranscodeToLocalCodePage(theSourceString, 0, true, theTargetVector, terminate);
}



static bool
doTranscodeFromLocalCodePage(
			const char*					theSourceString,
			unsigned int				theSourceStringLength,
			bool						theSourceStringIsNullTerminated,
			XalanDOMCharVectorType&		theTargetVector,
			bool						terminate)
{
    // Short circuit if it's a null pointer, or of length 0.
    if (!theSourceString || (!theSourceString[0]))
    {
		if (terminate == true)
		{
			theTargetVector.resize(1);

			theTargetVector.back() = '\0';
		}
		else
		{
			theTargetVector.resize(0);
		}

        return true;
	}

	if (theSourceStringIsNullTerminated == true)
	{
		theSourceStringLength = strlen(theSourceString);
	}

    // See how many chars we need to transcode.
	const size_t	theTargetLength = ::mbstowcs(0, theSourceString, theSourceStringLength);

	if (theTargetLength == size_t(-1))
	{
		return false;
	}
	else
	{
#if defined(XALAN_XALANDOMCHAR_USHORT_MISMATCH)
		typedef XalanDOMString::WideCharVectorType	WideCharVectorType;

		WideCharVectorType	theTempResult;

		theTempResult.resize(terminate == true ? theTargetLength + 1 : theTargetLength);

		wchar_t* const	theTargetPointer = &theTempResult[0];
#else
		theTargetVector.resize(terminate == true ? theTargetLength + 1 : theTargetLength);

		wchar_t* const	theTargetPointer = &theTargetVector[0];
#endif

		if (mbstowcs(theTargetPointer, theSourceString, theSourceStringLength) == size_t(-1))
		{
			return false;
		}
		else
		{
#if defined(XALAN_XALANDOMCHAR_USHORT_MISMATCH)
			const WideCharVectorType::size_type		theTempSize = theTempResult.size();

			theTargetVector.reserve(theTempSize);

			for(WideCharVectorType::size_type i = 0; i < theTempSize; ++i)
			{
				theTargetVector.push_back(WideCharVectorType::value_type(theTempResult[i]));
			}
#endif

			if (terminate == true)
			{
				theTargetVector.back() = '\0';
			}

			return true;
		}
	}
}



XALAN_DOM_EXPORT_FUNCTION(bool)
TranscodeFromLocalCodePage(
			const char*					theSourceString,
			unsigned int				theSourceStringLength,
			XalanDOMCharVectorType&		theTargetVector,
			bool						terminate)
{
	return doTranscodeFromLocalCodePage(theSourceString, theSourceStringLength, false, theTargetVector, terminate);
}



XALAN_DOM_EXPORT_FUNCTION(bool)
TranscodeFromLocalCodePage(
			const char*					theSourceString,
			XalanDOMCharVectorType&		theTargetVector,
			bool						terminate)
{
	return doTranscodeFromLocalCodePage(theSourceString, 0, true, theTargetVector, terminate);
}



#if defined(XALAN_USE_STD_STRING)
XALAN_DOM_EXPORT_FUNCTION(const XalanDOMString)
TranscodeFromLocalCodePage(
			const char*		theSourceString,
			unsigned int	theSourceStringLength)
{
	XalanDOMCharVectorType	theResult;

	if (theSourceStringLength == unsigned(-1))
	{
		TranscodeFromLocalCodePage(theSourceString, theResult, false);
	}
	else
	{
		TranscodeFromLocalCodePage(theSourceString, theSourceStringLength, theResult, false);
	}

	const XalanDOMCharVectorType::size_type		theSize = theResult.size();

	if (theSize == 0)
	{
		return XalanDOMString();
	}
	else
	{
		return XalanDOMString(&theResult[0], theSize);
	}
}
#endif
