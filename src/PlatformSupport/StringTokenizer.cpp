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
			const XalanDOMString&	theString,
			const XalanDOMString&	theTokens,
			bool					fReturnTokens) :
	m_String(theString),
	m_Tokens(theTokens),
	m_fReturnTokens(fReturnTokens),
	m_CurrentIndex(0),
	m_StringLength(length(theString)),
	m_tokensLength(length(theTokens))
{
}



StringTokenizer::StringTokenizer(
			const XalanDOMChar*		theString,
			const XalanDOMChar*		theTokens,
			bool					fReturnTokens) :
	m_String(theString),
	m_Tokens(theTokens),
	m_fReturnTokens(fReturnTokens),
	m_CurrentIndex(0),
	m_StringLength(length(theString)),
	m_tokensLength(length(theTokens))
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



XalanDOMString
StringTokenizer::nextToken()
{
	assert(m_CurrentIndex < m_StringLength);

	XalanDOMString	theToken;

	// Find the index of the next delimiter.
	unsigned int	theIndex = FindNextDelimiterIndex(m_CurrentIndex);

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



unsigned int
StringTokenizer::countTokens() const
{
	unsigned int	theCount = 0;
	unsigned int	theCurrentIndex = m_CurrentIndex;

	if (theCurrentIndex < m_StringLength)
	{
		while(theCurrentIndex < m_StringLength)
		{
			const unsigned int	theNextIndex = FindNextDelimiterIndex(theCurrentIndex);

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



unsigned int
StringTokenizer::FindNextDelimiterIndex(unsigned int	theStartIndex) const
{
	bool			fTokenFound = false;
	unsigned int	theIndex = theStartIndex;

	while(theIndex < m_StringLength &&
		  fTokenFound == false)
	{
		const XalanDOMChar	theCurrentChar =
			charAt(m_String,
				   theIndex);

		if (indexOf(m_Tokens,
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
