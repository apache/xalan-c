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
#include "StringTokenizer.hpp"



#include <cassert>



#include "DOMStringHelper.hpp"



StringTokenizer::StringTokenizer(
			const DOMString&	theString,
			const DOMString&	theTokens,
			bool				fReturnTokens) :
	m_String(theString),
	m_Tokens(theTokens),
	m_fReturnTokens(fReturnTokens),
	m_CurrentIndex(0),
	m_StringLength(theString.length())
{
}



StringTokenizer::~StringTokenizer()
{
}



bool
StringTokenizer::hasMoreTokens() const
{
	return countTokens() > 0 ? true : false;
}



bool
FindCharInTokens(XMLCh				theChar,
				 const DOMString&	theTokens)
{
	bool		fFound = false;

	const int	theLength = theTokens.length();

	for(int i = 0; i < theLength; i++)
	{
		if (charAt(theTokens, i) == theChar)
		{
			fFound = true;
			break;
		}
	}

	return fFound;
}


#if 0
DOMString
StringTokenizer::nextToken()
{
	assert(m_CurrentIndex < m_StringLength);

	DOMString	theToken;

	// Find the index of the next delimiter.
	int	theIndex = FindNextDelimiterIndex(m_CurrentIndex);

	if (theIndex == m_CurrentIndex &&
		m_fReturnTokens == true)
	{
		// The next delimiter is at the current index.  If we're
		// returning delimiters as tokens, then make that the
		// return value.  Otherwise, return an empty string.
		theToken = substring(m_String,
							 theIndex,
							 theIndex + 1);

		m_CurrentIndex = theIndex + 1;
	}
	else
	{
		if (theIndex == m_CurrentIndex)
		{
			theIndex = FindNextDelimiterIndex(m_CurrentIndex + 1);
		}
		assert(theIndex > m_CurrentIndex);

		theToken = substring(m_String,
							 m_CurrentIndex,
							 theIndex);

		if (m_fReturnTokens == true)
		{
			m_CurrentIndex = theIndex;
		}
		else
		{
			m_CurrentIndex = theIndex + 1;
		}
	}

	return theToken;
}
#else

DOMString
StringTokenizer::nextToken()
{
	assert(m_CurrentIndex < m_StringLength);

	DOMString	theToken;

	// Find the index of the next delimiter.
	int	theIndex = FindNextDelimiterIndex(m_CurrentIndex);

	if (theIndex == m_CurrentIndex)
	{
		m_CurrentIndex = theIndex + 1;

		if (m_fReturnTokens == true)
		{
			// The next delimiter is at the current index.  If we're
			// returning delimiters as tokens, then make that the
			// return value.  Otherwise, return an empty string.
			theToken = substring(m_String,
								 theIndex,
								 theIndex + 1);
		}
		else if (m_CurrentIndex < m_StringLength)
		{
			theToken = nextToken();
		}
	}
	else
	{
		if (theIndex == m_CurrentIndex)
		{
			theIndex = FindNextDelimiterIndex(m_CurrentIndex + 1);
		}
		assert(theIndex > m_CurrentIndex);

		theToken = substring(m_String,
							 m_CurrentIndex,
							 theIndex);

		m_CurrentIndex = theIndex;
	}

	return theToken;
}
#endif


int
StringTokenizer::countTokens() const
{
	int		theCount = 0;
	int		theCurrentIndex = m_CurrentIndex;

	if (theCurrentIndex < m_StringLength)
	{
		while(theCurrentIndex < m_StringLength)
		{
			const int	theNextIndex = FindNextDelimiterIndex(theCurrentIndex);

			if (theNextIndex == theCurrentIndex)
			{
				theCurrentIndex = theNextIndex + 1;

				if (m_fReturnTokens == true)
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



int
StringTokenizer::FindNextDelimiterIndex(int		theStartIndex) const
{
	bool		fTokenFound = false;
	int			theIndex = theStartIndex;

	while(theIndex < m_StringLength &&
		  fTokenFound == false)
	{
		const XMLCh		theCurrentChar = charAt(m_String,
												theIndex);

		if (FindCharInTokens(theCurrentChar,
							 m_Tokens) == true)
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
