/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2003 The Apache Software Foundation.  All rights 
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
#include "StringTokenizer.hpp"



#include <cassert>



#include "DOMStringHelper.hpp"
#include "XalanUnicode.hpp"



XALAN_CPP_NAMESPACE_BEGIN



const XalanDOMChar	StringTokenizer::s_defaultTokens[] =
{
	XalanUnicode::charSpace,
	XalanUnicode::charHTab,
	XalanUnicode::charLF,
	XalanUnicode::charCR,
	0,
};



StringTokenizer::StringTokenizer(
			const XalanDOMString&	theString,
			const XalanDOMString&	theTokens,
			bool					fReturnTokens) :
	m_string(theString.c_str()),
	m_tokens(theTokens.c_str()),
	m_returnTokens(fReturnTokens),
	m_currentIndex(0),
	m_stringLength(length(theString)),
	m_tokensLength(length(theTokens))
{
}



StringTokenizer::StringTokenizer(
			const XalanDOMString&	theString,
			const XalanDOMChar*		theTokens,
			bool					fReturnTokens) :
	m_string(theString.c_str()),
	m_tokens(theTokens),
	m_returnTokens(fReturnTokens),
	m_currentIndex(0),
	m_stringLength(length(theString)),
	m_tokensLength(length(theTokens))
{
	assert(m_string != 0 && m_tokens != 0);
}



StringTokenizer::StringTokenizer(
			const XalanDOMChar*		theString,
			const XalanDOMChar*		theTokens,
			bool					fReturnTokens) :
	m_string(theString),
	m_tokens(theTokens),
	m_returnTokens(fReturnTokens),
	m_currentIndex(0),
	m_stringLength(length(theString)),
	m_tokensLength(length(theTokens))
{
	assert(m_string != 0 && m_tokens != 0);
}



StringTokenizer::StringTokenizer(
			const XalanDOMChar*		theString,
			const XalanDOMString&	theTokens,
			bool					fReturnTokens) :
	m_string(theString),
	m_tokens(theTokens.c_str()),
	m_returnTokens(fReturnTokens),
	m_currentIndex(0),
	m_stringLength(length(theString)),
	m_tokensLength(length(theTokens))
{
	assert(m_string != 0 && m_tokens != 0);
}



StringTokenizer::~StringTokenizer()
{
}



bool
StringTokenizer::hasMoreTokens() const
{
	return countTokens() > 0 ? true : false;
}



XalanDOMString
StringTokenizer::nextToken()
{
	assert(m_currentIndex < m_stringLength);

	XalanDOMString	theToken;

	// Find the index of the next delimiter.
	XalanDOMString::size_type	theIndex = FindNextDelimiterIndex(m_currentIndex);

	if (theIndex == m_currentIndex)
	{
		m_currentIndex = theIndex + 1;

		if (m_returnTokens == true)
		{
			// The next delimiter is at the current index.  If we're
			// returning delimiters as tokens, then make that the
			// return value.  Otherwise, return an empty string.
			substring(
				m_string,
				theToken,
				theIndex,
				theIndex + 1);
		}
		else if (m_currentIndex < m_stringLength)
		{
			theToken = nextToken();
		}
	}
	else
	{
		if (theIndex == m_currentIndex)
		{
			theIndex = FindNextDelimiterIndex(m_currentIndex + 1);
		}
		assert(theIndex > m_currentIndex);

		substring(
			m_string,
			theToken,
			m_currentIndex,
			theIndex);

		m_currentIndex = theIndex;
	}

	return theToken;
}



void
StringTokenizer::nextToken(XalanDOMString&	theToken)
{
	assert(m_currentIndex < m_stringLength);

	// Find the index of the next delimiter.
	XalanDOMString::size_type	theIndex = FindNextDelimiterIndex(m_currentIndex);

	if (theIndex == m_currentIndex)
	{
		m_currentIndex = theIndex + 1;

		if (m_returnTokens == true)
		{
			// The next delimiter is at the current index.  If we're
			// returning delimiters as tokens, then make that the
			// return value.  Otherwise, return an empty string.
			substring(
				m_string,
				theToken,
				theIndex,
				theIndex + 1);
		}
		else if (m_currentIndex < m_stringLength)
		{
			theToken = nextToken();
		}
	}
	else
	{
		if (theIndex == m_currentIndex)
		{
			theIndex = FindNextDelimiterIndex(m_currentIndex + 1);
		}
		assert(theIndex > m_currentIndex);

		substring(
				m_string,
				theToken,
				m_currentIndex,
				theIndex);

		m_currentIndex = theIndex;
	}
}



StringTokenizer::size_type
StringTokenizer::countTokens() const
{
	size_t						theCount = 0;

	XalanDOMString::size_type	theCurrentIndex = m_currentIndex;

	if (theCurrentIndex < m_stringLength)
	{
		while(theCurrentIndex < m_stringLength)
		{
			const XalanDOMString::size_type		theNextIndex =
				FindNextDelimiterIndex(theCurrentIndex);

			if (theNextIndex == theCurrentIndex)
			{
				theCurrentIndex = theNextIndex + 1;

				if (m_returnTokens == true)
				{
					theCount++;
				}
			}
			else
			{
				theCount++;

				theCurrentIndex = theNextIndex;
			}
		}
	}

	return theCount;
}



XalanDOMString::size_type
StringTokenizer::FindNextDelimiterIndex(XalanDOMString::size_type	theStartIndex) const
{
	bool						fTokenFound = false;

	XalanDOMString::size_type	theIndex = theStartIndex;

	while(theIndex < m_stringLength &&
		  fTokenFound == false)
	{
		const XalanDOMChar	theCurrentChar =
			m_string[theIndex];

		if (indexOf(m_tokens,
					theCurrentChar) < m_tokensLength)
		{
			fTokenFound = true;
		}
		else
		{
			theIndex++;
		}
	}

	return theIndex;
}



XALAN_CPP_NAMESPACE_END
