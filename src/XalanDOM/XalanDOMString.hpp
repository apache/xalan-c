/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights 
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
#if !defined(XALANDOMSTRING_HEADER_GUARD_1357924680)
#define XALANDOMSTRING_HEADER_GUARD_1357924680



#include <XalanDOM/XalanDOMDefinitions.hpp>



#include <cassert>
#include <vector>



#include <XalanDOM/XalanDOMException.hpp>



XALAN_CPP_NAMESPACE_BEGIN



#if defined(XALAN_USE_NATIVE_WCHAR_T)
typedef wchar_t			XalanDOMChar;
#else
// UTF-16 character...
typedef unsigned short	XalanDOMChar;
#endif



class XALAN_DOM_EXPORT XalanDOMString
{
public:

#if defined(XALAN_NO_STD_NAMESPACE)
	typedef vector<XalanDOMChar>		XalanDOMCharVectorType;
	typedef vector<char>				CharVectorType;
	typedef vector<wchar_t>				WideCharVectorType;
#else
	typedef std::vector<XalanDOMChar>	XalanDOMCharVectorType;
	typedef std::vector<char>			CharVectorType;
	typedef std::vector<wchar_t>		WideCharVectorType;
#endif

	typedef XalanDOMChar				value_type;
	typedef XalanDOMChar&				reference;
	typedef const XalanDOMChar&			const_reference;

	typedef XalanDOMCharVectorType::size_type				size_type;

	typedef XalanDOMCharVectorType::iterator				iterator;
	typedef XalanDOMCharVectorType::const_iterator			const_iterator;
	typedef XalanDOMCharVectorType::reverse_iterator		reverse_iterator;
	typedef XalanDOMCharVectorType::const_reverse_iterator	const_reverse_iterator;

#if defined(XALAN_INLINE_INITIALIZATION)
	static const size_type	npos = ~0u;
#else
	enum { npos = -1 };
#endif

	explicit
	XalanDOMString();

	explicit
	XalanDOMString(
			const char*		theString,
			size_type		theCount = size_type(npos));

	XalanDOMString(
			const XalanDOMString&	theSource,
			size_type				theStartPosition = 0,
			size_type				theCount = size_type(npos));

	explicit
	XalanDOMString(
			const XalanDOMChar*		theString,
			size_type				theCount = size_type(npos));

	XalanDOMString(
			size_type		theCount,
			XalanDOMChar	theChar);

	~XalanDOMString()
	{
	}

	XalanDOMString&
	operator=(const XalanDOMString&	theRHS)
	{
		return assign(theRHS);
	}

	XalanDOMString&
	operator=(const XalanDOMChar*	theRHS)
	{
		return assign(theRHS);
	}

	XalanDOMString&
	operator=(const char*	theRHS)
	{
		return assign(theRHS);
	}

	XalanDOMString&
	operator=(XalanDOMChar	theRHS)
	{
		return assign(1, theRHS);
	}

	iterator
	begin()
	{
		invariants();

		return m_data.begin();
	}

	const_iterator
	begin() const
	{
		invariants();

		return m_data.begin();
	}

	iterator
	end()
	{
		invariants();

		return m_data.end();
	}

	const_iterator
	end() const
	{
		invariants();

		return m_data.end();
	}

	reverse_iterator
	rbegin()
	{
		invariants();

		return m_data.rbegin();
	}

	const_reverse_iterator
	rbegin() const
	{
		invariants();

		return m_data.rbegin();
	}

	reverse_iterator
	rend()
	{
		invariants();

		return m_data.rend();
	}

	const_reverse_iterator
	rend() const
	{
		invariants();

		return m_data.rend();
	}

	size_type
	size() const
	{
		invariants();

		return m_size;
	}

	size_type
	length() const
	{
		invariants();

		return size();
	}

	size_type
	max_size() const
	{
		invariants();

		return size_type(~0);
	}

	void
	resize(
			size_type		theCount,
			XalanDOMChar	theChar);

	void
	resize(size_type	theCount)
	{
		invariants();

		resize(theCount, XalanDOMChar(0));
	}

	size_type
	capacity() const
	{
		invariants();

		return m_data.capacity() - 1;
	}

	void
	reserve(size_type	theCount = 0)
	{
		invariants();

		m_data.reserve(theCount + 1);
	}

	void
	clear()
	{
		invariants();

		m_data.erase(m_data.begin(), m_data.end());

		m_size = 0;

		invariants();
	}

	void
	erase(
			size_type	theStartPosition = 0,
			size_type	theCount = size_type(npos));

	bool
	empty() const
	{
		invariants();

		return m_size == 0 ? true : false;
	}

	const_reference
	operator[](size_type	theIndex) const
	{
		invariants();

		return m_data[theIndex];
	}

	reference
	operator[](size_type	theIndex)
	{
		invariants();

		return m_data[theIndex];
	}

#if 0
	// $$$ ToDo: at() is not supported in the current version of GCC's vector<>
	// implementation.  Eventually, it should be.
	const_reference
	at(size_type	theIndex) const
	{
		invariants();

		return m_data.at(theIndex);
	}

	reference
	at(size_type	theIndex)
	{
		invariants();

		return m_data.at(theIndex);
	}
#endif

	const XalanDOMChar*
	c_str() const
	{
		invariants();

		// $$$ ToDo: Do we really want to do this?
		// for convenience, we will return a pointer to
		// a default empty string so that c_str() never
		// returns a null pointer...
		return m_data.empty() == true ? &s_empty : &m_data[0];
	}

	const XalanDOMChar*
	data() const
	{
		invariants();

		return c_str();
	}

	void
	swap(XalanDOMString&	theOther)
	{
		invariants();

		m_data.swap(theOther.m_data);

#if defined(XALAN_NO_STD_NAMESPACE)
		::swap(m_size, theOther.m_size);
#else
		std::swap(m_size, theOther.m_size);
#endif
	}

	XalanDOMString&
	operator+=(const XalanDOMString&	theSource)
	{
		return append(theSource);
	}

	XalanDOMString&
	operator+=(const XalanDOMChar*	theString)
	{
		return append(theString);
	}

	XalanDOMString&
	operator+=(XalanDOMChar theChar)
	{
		append(1, theChar);

		return *this;
	}

	XalanDOMString&
	assign(const XalanDOMChar*	theSource)
	{
		invariants();

		erase();

		invariants();

		return append(theSource);
	}

	XalanDOMString&
	assign(
			const XalanDOMChar*		theSource,
			size_type				theCount)
	{
		invariants();

		erase();

		invariants();

		return append(theSource, theCount);
	}

	XalanDOMString&
	assign(const char*	theSource)
	{
		invariants();

		erase();

		invariants();

		return append(theSource);
	}

	XalanDOMString&
	assign(
			const char*		theSource,
			size_type		theCount)
	{
		invariants();

		erase();

		invariants();

		return append(theSource, theCount);
	}

	XalanDOMString&
	assign(
			const XalanDOMString&	theSource,
			size_type				thePosition,
			size_type				theCount);

	XalanDOMString&
	assign(const XalanDOMString&	theSource)
	{
		invariants();

		if (&theSource != this)
		{
			m_data = theSource.m_data;

			m_size = theSource.m_size;
		}

		invariants();

		return *this;
	}

	XalanDOMString&
	assign(
			size_type		theCount,
			XalanDOMChar	theChar)
	{
		invariants();

		erase();

		invariants();

		return append(theCount, theChar);
	}

	XalanDOMString&
	assign(
		const_iterator	theFirstPosition,
		const_iterator	theLastPosition);

	XalanDOMString&
	append(const XalanDOMString&	theSource)
	{
		return append(theSource.c_str(), theSource.length());
	}

	XalanDOMString&
	append(
			const XalanDOMString&	theSource,
			size_type				thePosition,
			size_type				theCount)
	{
		assert(thePosition < theSource.length() &&
			   (theCount == size_type(npos) || thePosition + theCount <= theSource.length()));

		return append(theSource.c_str() + thePosition, theCount);
	}

	XalanDOMString&
	append(
			const XalanDOMChar*		theString,
			size_type				theCount);

	XalanDOMString&
	append(const XalanDOMChar*	theString)
	{
		return append(theString, length(theString));
	}

	XalanDOMString&
	append(
			const char*		theString,
			size_type		theCount);

	XalanDOMString&
	append(const char*	theString)
	{
		return append(theString, length(theString));
	}

	XalanDOMString&
	append(
			size_type		theCount,
			XalanDOMChar	theChar);

	void
	push_back(XalanDOMChar	theChar)
	{
		invariants();

		append(1, theChar);

		invariants();
	}

	XalanDOMString&
	insert(
			size_type				thePosition,
			const XalanDOMString&	theString)
	{
		return insert(thePosition, theString.c_str(), theString.length());
	}

	XalanDOMString&
	insert(
			size_type				thePosition1,
			const XalanDOMString&	theString,
			size_type				thePosition2,
			size_type				theCount)
	{
		return insert(thePosition1, theString.c_str() + thePosition2, theCount);
	}

	XalanDOMString&
	insert(
			size_type				thePosition,
			const XalanDOMChar*		theString,
			size_type				theCount);

	XalanDOMString&
	insert(
			size_type				thePosition,
			const XalanDOMChar*		theString)
	{
		return insert(thePosition, theString, length(theString));
	}

	XalanDOMString&
	insert(
			size_type		thePosition,
			size_type		theCount,
			XalanDOMChar	theChar);

	iterator
	insert(
			iterator		thePosition,
			XalanDOMChar	theChar);

	void
	insert(
			iterator		thePosition,
			size_type		theCount,
			XalanDOMChar	theChar);

	void
	insert(
		iterator		theInsertPosition,
		const_iterator	theFirstPosition,
		const_iterator	theLastPosition);

	XalanDOMString
	substr(
			size_type	thePosition = 0,
			size_type	theCount = size_type(npos)) const
	{
		assert(theCount == size_type(npos) && thePosition < length() ||
			   thePosition + theCount <= length());

		invariants();

		return XalanDOMString(*this, thePosition, theCount);
	}

	XalanDOMString&
	substr(
			XalanDOMString&		theSubstring,
			size_type			thePosition = 0,
			size_type			theCount = size_type(npos)) const
	{
		assert(theCount == size_type(npos) && thePosition < length() ||
			   thePosition + theCount <= length());

		invariants();

		return theSubstring.assign(*this, thePosition, theCount);
	}

	int
	compare(const XalanDOMString&	theString) const
	{
		invariants();

		return compare(theString.c_str());
	}

	int
	compare(
			size_type				thePosition1,
			size_type				theCount1,
			const XalanDOMString&	theString) const
	{
		invariants();

		return compare(thePosition1, theCount1, theString.c_str(), theString.length());
	}

	int
	compare(
			size_type				thePosition1,
			size_type				theCount1,
			const XalanDOMString&	theString,
			size_type				thePosition2,
			size_type				theCount2) const
	{
		invariants();

		return compare(thePosition1, theCount1, theString.c_str() + thePosition2, theCount2);
	}

	int
	compare(const XalanDOMChar*		theString) const;

	int
	compare(
			size_type				thePosition1,
			size_type				theCount1,
			const XalanDOMChar*		theString,
			size_type				theCount2 = size_type(npos)) const;

	int
	compare(const char*		theString) const
	{
		invariants();

		return compare(XalanDOMString(theString));
	}

	int
	compare(
			size_type		thePosition1,
			size_type		theCount1,
			const char*		theString,
			size_type		theCount2 = size_type(npos)) const
	{
		invariants();

		return compare(thePosition1, theCount1, XalanDOMString(theString, theCount2));
	}

	CharVectorType
	transcode() const;

	static bool
	equals(
			const XalanDOMChar*		theLHS,
			size_type				theLHSLength,
			const XalanDOMChar*		theRHS,
			size_type				theRHSLength);

	static bool
	equals(
			const XalanDOMChar*		theLHS,
			const XalanDOMChar*		theRHS)
	{
		return equals(theLHS, length(theLHS), theRHS, length(theRHS));
	}

	static bool
	equals(
			const XalanDOMString&	theLHS,
			const XalanDOMString&	theRHS);

	static bool
	equals(
			const XalanDOMString&	theLHS,
			const XalanDOMChar*		theRHS)
	{
		return equals(theLHS.c_str(), theRHS);
	}

	static bool
	equals(
			const XalanDOMChar*		theLHS,
			const XalanDOMString&	theRHS)
	{
		return equals(theLHS, theRHS.c_str());
	}

	/*
	 * Helper function to determine the length of a null-
	 * terminated string.
	 *
	 * @theString The string
	 * @return the length
	 */
	static size_type
	length(const XalanDOMChar*	theString);

	/*
	 * Helper function to determine the length of a null-
	 * terminated string.
	 *
	 * @theString The string
	 * @return the length
	 */
	static size_type
	length(const char*	theString);

	class TranscodingError : public XalanDOMException
	{
	public:

		TranscodingError() :
			XalanDOMException(TRANSCODING_ERR)
		{
		}

		virtual
		~TranscodingError()
		{
		}
	};

protected:

	/*
	 * Function to assert invariant conditions for the class.
	 *
	 * @return the iterator
	 */
	void
	invariants() const
	{
#if !defined(NDEBUG)
		assert((m_data.empty() == true && m_size == 0) || m_size == m_data.size() - 1);
		assert(m_data.empty() == true || m_data.back() == 0);
#endif
	}

	/*
	 * Get an iterator to the position of the terminating null.
	 *
	 * @return the iterator
	 */
	iterator
	getBackInsertIterator()
	{
		invariants();

		return m_data.empty() == true ? m_data.end() : m_data.end() - 1;
	}

	const_iterator
	getBackInsertIterator() const
	{
		invariants();

		return m_data.empty() == true ? m_data.end() : m_data.end() - 1;
	}

	iterator
	getIteratorForPosition(size_type	thePosition)
	{
		invariants();

		return m_data.begin() + thePosition;
	}

	const_iterator
	getIteratorForPosition(size_type	thePosition) const
	{
		invariants();

		return m_data.begin() + thePosition;
	}

private:

	XalanDOMCharVectorType		m_data;

	size_type					m_size;

	static const XalanDOMChar	s_empty;
};



inline bool
operator==(
			const XalanDOMString&	theLHS,
			const XalanDOMString&	theRHS)
{
	return XalanDOMString::equals(theLHS, theRHS);
}



inline bool
operator==(
			const XalanDOMString&	theLHS,
			const XalanDOMChar*		theRHS)
{
	return XalanDOMString::equals(theLHS, theRHS);
}



inline bool
operator==(
			const XalanDOMChar*		theLHS,
			const XalanDOMString&	theRHS)
{
	// Note reversing of operands...
	return XalanDOMString::equals(theLHS, theRHS);
}



inline bool
operator!=(
			const XalanDOMString&	theLHS,
			const XalanDOMString&	theRHS)
{
	return !(theLHS == theRHS);
}



inline bool
operator!=(
			const XalanDOMChar*		theLHS,
			const XalanDOMString&	theRHS)
{
	return !(theLHS == theRHS);
}



inline bool
operator!=(
			const XalanDOMString&	theLHS,
			const XalanDOMChar*		theRHS)
{
	return !(theRHS == theLHS);
}



inline XalanDOMString
operator+(
			const XalanDOMString&	theLHS,
			const XalanDOMString&	theRHS)
{
	XalanDOMString	theTemp(theLHS);

	return theTemp += theRHS;
}



inline XalanDOMString
operator+(
			const XalanDOMString&	theLHS,
			const XalanDOMChar*		theRHS)
{
	XalanDOMString	theTemp(theLHS);

	return theTemp += theRHS;
}



inline XalanDOMString
operator+(
			const XalanDOMChar*		theLHS,
			const XalanDOMString&	theRHS)
{
	XalanDOMString	theTemp(theLHS);

	return theTemp += theRHS;
}



inline const XalanDOMString
operator+(
			const char*				theLHS,
			const XalanDOMString&	theRHS)
{
	return XalanDOMString(theLHS) + theRHS;
}



inline const XalanDOMString
operator+(
			const XalanDOMString&	theLHS,
			const char*				theRHS)
{
	return theLHS + XalanDOMString(theRHS);
}



// Standard vector of XalanDOMChars and chars
#if defined(XALAN_NO_STD_NAMESPACE)
typedef vector<XalanDOMChar>		XalanDOMCharVectorType;

typedef vector<char>				CharVectorType;
#else
typedef std::vector<XalanDOMChar>	XalanDOMCharVectorType;

typedef std::vector<char>			CharVectorType;
#endif



/**
 * Convert a XalanDOMChar string to C++ standard library
 * vector, transcoding to the default local code
 * page.
 * 
 * @param sourceString The source string
 * @param sourceStringLength The source string length.
 * @param targetVector The target string
 * @param terminate If true, the transcoded string will be null-terminated
 * @return true if successful, false if not.
 */
XALAN_DOM_EXPORT_FUNCTION(bool)
TranscodeToLocalCodePage(
			const XalanDOMChar*			theSourceString,
			XalanDOMString::size_type	theSourceStringLength,
			CharVectorType&				targetVector,
			bool						terminate = false);



/**
 * Convert a XalanDOMChar string to C++ standard library
 * vector, transcoding to the default local code
 * page.  The string _must_ be null-terminated.
 * 
 * @param theSourceString The source string
 * @param targetVector The target string
 * @param terminate If true, the transcoded string will be null-terminated
 * @return true if successful, false if not.
 */
XALAN_DOM_EXPORT_FUNCTION(bool)
TranscodeToLocalCodePage(
			const XalanDOMChar*		theSourceString,
			CharVectorType&			targetVector,
			bool					terminate = false);



/**
 * Convert XalanDOMString to C++ standard library
 * vector, transcoding to the default local code
 * page.  Null-terminate the sttring...
 *
 * @param theSourceString source string
 * @return The transcoded string.
 */
inline const CharVectorType
TranscodeToLocalCodePage(const XalanDOMChar*	theSourceString)
{
	CharVectorType	theResult;

	TranscodeToLocalCodePage(theSourceString, theResult, true);

	return theResult;
}



/**
 * Convert XalanDOMString to C++ standard library
 * vector, transcoding to the default local code
 * page.
 * 
 * @param theSourceString The source string
 * @param targetVector The target string
 * @return true if successful, false if not.
 */
inline bool
TranscodeToLocalCodePage(
			const XalanDOMString&	theSourceString,
			CharVectorType&			targetVector,
			bool					terminate = false)
{
	return TranscodeToLocalCodePage(theSourceString.c_str(), targetVector, terminate);
}



/**
 * Convert XalanDOMString to C++ standard library
 * vector, transcoding to the default local code
 * page.
 *
 * @param thetheSourceString source string
 * @return The transcoded string.
 */
inline const CharVectorType
TranscodeToLocalCodePage(const XalanDOMString&	theSourceString)
{
	CharVectorType	theResult;

	TranscodeToLocalCodePage(theSourceString, theResult, true);

	return theResult;
}



/**
 * Convert a string to a XalanDOMString, transcoding from
 * the default local code page.
 * 
 * @param theSourceString The source string
 * @param theSourceStringLength The source string length.
 * @return The new string.
 */
inline const XalanDOMString
TranscodeFromLocalCodePage(
			const char*					theSourceString,
			XalanDOMString::size_type	theSourceStringLength = XalanDOMString::npos)
{
	return XalanDOMString(theSourceString, theSourceStringLength);
}



/**
 * Convert a string to a C++ standard library
 * vector, transcoding from the default local code
 * page.
 * 
 * @param theSourceString The source string
 * @param theSourceStringLength The source string length.
 * @param targetVector The target string
 * @param terminate If true, the transcoded string will be null-terminated
 * @return true if successful, false if not.
 */
XALAN_DOM_EXPORT_FUNCTION(bool)
TranscodeFromLocalCodePage(
			const char*					theSourceString,
			XalanDOMString::size_type	theSourceStringLength,
			XalanDOMCharVectorType&		theTargetVector,
			bool						terminate = false);



/**
 * Convert a string to a C++ standard library
 * vector, transcoding from the default local code
 * page.  The string _must_ be null-terminated.
 * 
 * @param sourceString The source string
 * @param targetVector The target string
 * @param terminate If true, the transcoded string will be null-terminated
 * @return true if successful, false if not.
 */
XALAN_DOM_EXPORT_FUNCTION(bool)
TranscodeFromLocalCodePage(
			const char*					theSourceString,
			XalanDOMCharVectorType&		theTargetVector,
			bool						terminate = false);



/**
 * Convert a vector of characters to a XalanDOMString,
 * transcoding from the default local code
 *
 * @param theSourceString source string
 * @return The transcoded string.
 */
inline const XalanDOMString
TranscodeFromLocalCodePage(const CharVectorType&	theSourceString)
{
	typedef XalanDOMString::size_type		size_type;

	const CharVectorType::size_type		theSize = theSourceString.size();

	if (theSourceString[theSize - 1] == CharVectorType::value_type(0))
	{
		return TranscodeFromLocalCodePage(&*theSourceString.begin(), size_type(theSize) - 1);
	}
	else
	{
		return TranscodeFromLocalCodePage(&*theSourceString.begin(), size_type(theSize));
	}
}



XALAN_CPP_NAMESPACE_END



#endif	// !defined(XALANDOMSTRING_HEADER_GUARD_1357924680)
