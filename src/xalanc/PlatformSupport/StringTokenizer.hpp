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
#if !defined(STRINGTOKENIZER_HEADER_GUARD_1357924680)
#define STRINGTOKENIZER_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_PLATFORMSUPPORT_EXPORT StringTokenizer
{
public:

	static const XalanDOMChar	s_defaultTokens[];

	typedef size_t	size_type;

	/**
	 * Constructs a tokenizer for the target string.  No copies of any input
	 * strings are made.
	 *
	 * @param theString     string to tokenize
	 * @param theTokens     string of delimiters used to parse target, default
	 *                      is " \t\n\r"
	 * @param fReturnTokens if true, delimiter characters are also returned
	 *                      as tokens, default is false
	 */
	StringTokenizer(const XalanDOMString&	theString,
					const XalanDOMString&	theTokens,
					bool					fReturnTokens = false);

	/**
	 * Constructs a tokenizer for the target string.  No copies of any input
	 * strings are made.
	 * 
	 * @param theString     string to tokenize
	 * @param theTokens     string of delimiters used to parse target, default
	 *                      is " \t\n\r"
	 * @param fReturnTokens if true, delimiter characters are also returned
	 *                      as tokens, default is false
	 */
	StringTokenizer(const XalanDOMString&	theString,
					const XalanDOMChar*		theTokens = s_defaultTokens,
					bool					fReturnTokens = false);

	/**
	 * Constructs a tokenizer for the target string.  No copies of any input
	 * strings are made.
	 *
	 * @param theString     string to tokenize
	 * @param theTokens     string of delimiters used to parse target.
	 * @param fReturnTokens if true, delimiter characters are also returned
	 *                      as tokens, default is false
	 */
	StringTokenizer(const XalanDOMChar*		theString,
					const XalanDOMChar*		theTokens = s_defaultTokens,
					bool					fReturnTokens = false);

	/**
	 * Constructs a tokenizer for the target string.  No copies of any input
	 * strings are made.
	 *
	 * @param theString     string to tokenize
	 * @param theTokens     string of delimiters used to parse target.
	 * @param fReturnTokens if true, delimiter characters are also returned
	 *                      as tokens, default is false
	 */
	StringTokenizer(const XalanDOMChar*		theString,
					const XalanDOMString&	theTokens,
					bool					fReturnTokens = false);

	~StringTokenizer();

	/**
	 * Determine if there are tokens remaining
	 * 
	 * @return true if there are more tokens
	 */
	bool
	hasMoreTokens() const;


	/**
	 * Retrieve the next token to be parsed; behavior is undefined if there are
	 * no more tokens
	 * 
	 * @param theToken next token string
	 */
	void
	nextToken(XalanDOMString&	theToken);

	/**
	 * Count the number of tokens yet to be parsed
	 * 
	 * @return number of remaining tokens
	 */
	size_type
	countTokens() const;

	void
	reset()
	{
		m_currentIndex = 0;
	}

protected:

	XalanDOMString::size_type
	FindNextDelimiterIndex(XalanDOMString::size_type	theStartIndex) const;

private:

    // These are not implemented...
    StringTokenizer(const StringTokenizer&);

    StringTokenizer&
    operator=(const StringTokenizer&);

    bool
    operator==(const StringTokenizer&) const;


    // Data members...
    const XalanDOMChar* const			m_string;

	const XalanDOMChar*	const			m_tokens;

	const bool							m_returnTokens;

	XalanDOMString::size_type			m_currentIndex;

	const XalanDOMString::size_type		m_stringLength;

	const XalanDOMString::size_type		m_tokensLength;
};



XALAN_CPP_NAMESPACE_END



#endif	// STRINGTOKENIZER_HEADER_GUARD_1357924680
