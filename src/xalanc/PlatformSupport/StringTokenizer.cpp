/*
 * Copyright 1999-2004 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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
