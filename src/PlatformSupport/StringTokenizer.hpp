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
#if !defined(STRINGTOKENIZER_HEADER_GUARD_1357924680)
#define STRINGTOKENIZER_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <PlatformSupport/PlatformSupportDefinitions.hpp>



#include <XalanDOM/XalanDOMString.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>



class XALAN_PLATFORMSUPPORT_EXPORT StringTokenizer
{
public:

	/**
	 * Constructs a tokenizer for the target string
	 * 
	 * @param theString     string to tokenize
	 * @param theTokens     string of delimiters used to parse target, default
	 *                      is "\t\n\r\f"
	 * @param fReturnTokens if true, delimiter characters are also returned
	 *                      as tokens, default is false
	 */
	StringTokenizer(const XalanDOMString&	theString,
					const XalanDOMString&	theTokens = XalanDOMString(XALAN_STATIC_UCODE_STRING("\t\n\r\f")),
					bool					fReturnTokens = false);

#if defined(XALAN_LSTRSUPPORT)
	/**
	 * Constructs a tokenizer for the target string.  This version constructs
	 * from null-terminated wide strings, like those we get from SAX-like APIs.
	 *
	 * @param theString     string to tokenize
	 * @param theTokens     string of delimiters used to parse target, default
	 *                      is "\t\n\r\f"
	 * @param fReturnTokens if true, delimiter characters are also returned
	 *                      as tokens, default is false
	 */
	StringTokenizer(const XalanDOMChar*		theString,
					const XalanDOMChar*		theTokens = XALAN_STATIC_UCODE_STRING("\t\n\r\f"),
					bool					fReturnTokens = false);
#endif

	virtual
	~StringTokenizer();

	/**
	 * Determine if there are tokens remaining
	 * 
	 * @return true if there are more tokens
	 */
	virtual bool
	hasMoreTokens() const;

	/**
	 * Retrieve the next token to be parsed; behavior is undefined if there are
	 * no more tokens
	 * 
	 * @return next token string
	 */
	virtual XalanDOMString
	nextToken();

	/**
	 * Count the number of tokens yet to be parsed
	 * 
	 * @return number of remaining tokens
	 */
	virtual unsigned int
	countTokens() const;

protected:

	unsigned int
	FindNextDelimiterIndex(unsigned int	theStartIndex) const;

private:

	const XalanDOMString	m_String;

	const XalanDOMString	m_Tokens;

	const bool				m_fReturnTokens;

	unsigned int			m_CurrentIndex;

	const unsigned int		m_StringLength;

	const unsigned int		m_tokensLength;
};



#endif	// STRINGTOKENIZER_HEADER_GUARD_1357924680
