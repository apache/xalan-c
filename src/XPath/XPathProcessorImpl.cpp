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
 *
 * @author <a href="mailto:david_n_bertoni@lotus.com">David N. Bertoni</a>
 */

// Class header file...
#include "XPathProcessorImpl.hpp"



#include <xercesc/sax/Locator.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/DOMStringPrintWriter.hpp>
#include <PlatformSupport/DoubleSupport.hpp>
#include <PlatformSupport/PrefixResolver.hpp>
#include <PlatformSupport/XalanUnicode.hpp>
#include <PlatformSupport/XalanXMLChar.hpp>



#include <DOMSupport/DOMServices.hpp>



#include "XalanQName.hpp"
#include "XPathEnvSupport.hpp"
#include "XPathExecutionContext.hpp"
#include "XPathParserException.hpp"



XPathProcessorImpl::XPathProcessorImpl() :
	m_token(),
	m_tokenChar(0),
	m_xpath(0),
	m_expression(0),
	m_prefixResolver(0),
	m_requireLiterals(false),
	m_isMatchPattern(false),
	m_positionPredicateStack(),
	m_namespaces()
{
}



XPathProcessorImpl::~XPathProcessorImpl()
{
}



void
XPathProcessorImpl::initXPath(
			XPath&					pathObj,
			const XalanDOMString&	expression,
			const PrefixResolver&	prefixResolver,
			const Locator*			locator)
{
	m_requireLiterals = false;

	m_xpath = &pathObj;

	m_expression = &m_xpath->getExpression();

	m_prefixResolver = &prefixResolver;

	m_locator = locator;

	m_expression->reset();

	tokenize(expression);

	m_expression->appendOpCode(XPathExpression::eOP_XPATH);

	nextToken();

	Expr();

	if (length(m_token) != 0)
	{
		error("Extra illegal tokens!");
	}

	m_xpath = 0;
	m_expression = 0;
	m_prefixResolver = 0;
	m_locator = 0;
	m_positionPredicateStack.clear();
	m_namespaces.clear();
}



void
XPathProcessorImpl::initMatchPattern(
			XPath&					pathObj,
			const XalanDOMString&	expression,
			const PrefixResolver&	prefixResolver,
			const Locator*			locator)
{
	m_isMatchPattern = true;

	m_xpath = &pathObj;

	m_expression = &m_xpath->getExpression();

	m_prefixResolver = &prefixResolver;

	m_locator = locator;

	m_expression->reset();

	tokenize(expression);

	m_expression->appendOpCode(XPathExpression::eOP_MATCHPATTERN);

	nextToken();

	Pattern();

	if (length(m_token) != 0)
	{
		error("Extra illegal tokens!");
	}

	// Terminate for safety.
	m_expression->appendOpCode(XPathExpression::eENDOP);

	m_expression->shrink();

	m_xpath = 0;
	m_expression = 0;
	m_prefixResolver = 0;
	m_locator = 0;
	m_positionPredicateStack.clear();
	m_namespaces.clear();
}



void
XPathProcessorImpl::tokenize(const XalanDOMString&	pat)
{
	m_expression->setCurrentPattern(pat);

	const int	nChars = length(pat);

	int 		startSubstring = -1;
	int 		posOfNSSep = -1;

	bool		isStartOfPat = true;
	bool		isAttrName = false;

	// Nesting of '[' so we can know if the given element should be 
	// counted inside the m_patternMap.
	int nesting = 0;

	XalanDOMString	theToken;

	for(int i = 0; i < nChars; i++)
	{
		XalanDOMChar	c = charAt(pat, i);

		switch(c)
		{
		case XalanUnicode::charQuoteMark: 
			{
				if(startSubstring != -1)
				{
					isStartOfPat = mapPatternElemPos(nesting, isStartOfPat, isAttrName);

					isAttrName = false;

					if(-1 != posOfNSSep)
					{	   
						posOfNSSep = mapNSTokens(pat, startSubstring, posOfNSSep, i);
					}
					else
					{
						substring(pat, theToken, startSubstring, i);

						addToTokenQueue(theToken);
					}
				}

				startSubstring = i;

				for(++i; i < nChars && (c = charAt(pat, i)) != XalanUnicode::charQuoteMark; ++i);

				if(c == XalanUnicode::charQuoteMark)
				{
					substring(pat, theToken, startSubstring, i + 1);

					addToTokenQueue(theToken);

					startSubstring = -1;
				}
				else
				{
					error("misquoted literal... expected double quote!");
				}
			}
			break;

		case XalanUnicode::charApostrophe:
			{
				if(startSubstring != -1)
				{
					isStartOfPat = mapPatternElemPos(nesting, isStartOfPat, isAttrName);
					isAttrName = false;

					if(-1 != posOfNSSep)
					{	 
						posOfNSSep = mapNSTokens(pat, startSubstring, posOfNSSep, i);
					}
					else
					{
						substring(pat, theToken, startSubstring, i);

						addToTokenQueue(theToken);
					}
				}

				startSubstring = i;

				for(++i; i < nChars && (c = charAt(pat, i)) != XalanUnicode::charApostrophe; ++i);

				if(c == XalanUnicode::charApostrophe)
				{
					substring(pat, theToken, startSubstring, i + 1);

					addToTokenQueue(theToken);

					startSubstring = -1;
				}
				else
				{
					error("misquoted literal... expected single quote!");
				}
			}
			break;
		
		case XalanUnicode::charLF:
		case XalanUnicode::charCR:
		case XalanUnicode::charSpace:
		case XalanUnicode::charHTab:
			{
				if(startSubstring != -1)
				{
					isStartOfPat = mapPatternElemPos(nesting, isStartOfPat, isAttrName);
					isAttrName = false;

					if(-1 != posOfNSSep)
					{	 
						posOfNSSep = mapNSTokens(pat, startSubstring, posOfNSSep, i);
					}
					else
					{
						substring(pat, theToken, startSubstring, i);

						addToTokenQueue(theToken);
					}

					startSubstring = -1;
				}
			}
			break;
		
		case XalanUnicode::charCommercialAt:
			isAttrName = true;
			// fall-through on purpose

		case XalanUnicode::charHyphenMinus:
			{
				if(XalanUnicode::charHyphenMinus == c)
				{
					if(!(startSubstring == -1))
					{
						break;
					}
				}
			}
			// fall-through on purpose

		case XalanUnicode::charLeftParenthesis:
		case XalanUnicode::charLeftSquareBracket:
		case XalanUnicode::charRightParenthesis:
		case XalanUnicode::charRightSquareBracket:
		case XalanUnicode::charVerticalLine:
		case XalanUnicode::charSolidus:
		case XalanUnicode::charAsterisk:
		case XalanUnicode::charPlusSign:
		case XalanUnicode::charEqualsSign:
		case XalanUnicode::charComma:
		case XalanUnicode::charReverseSolidus: // Unused at the moment
		case XalanUnicode::charCircumflexAccent: // Unused at the moment
		case XalanUnicode::charExclamationMark: // Unused at the moment
		case XalanUnicode::charDollarSign:
		case XalanUnicode::charLessThanSign:
		case XalanUnicode::charGreaterThanSign:
			{
				if(startSubstring != -1)
				{
					isStartOfPat = mapPatternElemPos(nesting, isStartOfPat, isAttrName);
					isAttrName = false;

					if(-1 != posOfNSSep)
					{	 
						posOfNSSep = mapNSTokens(pat, startSubstring, posOfNSSep, i);
					}
					else
					{
						substring(pat, theToken, startSubstring, i);

						addToTokenQueue(theToken);
					}

					startSubstring = -1;
				}
				else if(XalanUnicode::charSolidus == c && isStartOfPat == true)
				{
					isStartOfPat = mapPatternElemPos(nesting, isStartOfPat, isAttrName);
				}
				else if(XalanUnicode::charAsterisk == c)
				{
					isStartOfPat = mapPatternElemPos(nesting, isStartOfPat, isAttrName);
					isAttrName = false;
				}

				if(0 == nesting)
				{
					if(XalanUnicode::charVerticalLine == c)
					{
						isStartOfPat = true;
					}
				}

				if(XalanUnicode::charRightParenthesis == c || XalanUnicode::charRightSquareBracket == c)
				{
					nesting--;
				}
				else if(XalanUnicode::charLeftParenthesis == c || XalanUnicode::charLeftSquareBracket == c)
				{
					nesting++;
				}

				substring(pat, theToken, i, i + 1);

				addToTokenQueue(theToken);
			}		
			break;

		case XalanUnicode::charColon:
			{
				if(posOfNSSep == i - 1 && i > 0)
				{ 
					if(startSubstring != -1)
					{
						if (startSubstring < i - 1)
						{
							substring(pat, theToken, startSubstring, i - 1);

							addToTokenQueue(theToken);
						}
					}

					isAttrName = false;
					startSubstring = -1;
					posOfNSSep = -1;

					substring(pat, theToken, i - 1, i + 1);

					addToTokenQueue(theToken);
					break;
				}
				else
				{
					posOfNSSep = i;
				}
			}
			// fall through on purpose

		
		default:
			{
				if(-1 == startSubstring)
				{
					startSubstring = i;

					if (XalanXMLChar::isDigit(c) == true)
					{
						bool	gotFullStop = false;

						while(i < nChars - 1)
						{
							++i;

							const XalanDOMChar	currentChar = charAt(pat, i);

							if (currentChar == XalanUnicode::charFullStop)
							{
								if (gotFullStop == false)
								{
									gotFullStop = true;
								}
								else
								{
									--i;

									break;
								}
							}
							else if (XalanXMLChar::isDigit(currentChar) == false)
							{
								--i;

								break;
							}
						}

						substring(pat, theToken, startSubstring, i + 1);

						addToTokenQueue(theToken);

						startSubstring = -1;
					}
				}
			}
		}
	}

	if(startSubstring != -1)
	{
		isStartOfPat = mapPatternElemPos(nesting, isStartOfPat, isAttrName);

		if(-1 != posOfNSSep)
		{	 
			posOfNSSep = mapNSTokens(pat, startSubstring, posOfNSSep, nChars);
		}
		else
		{
			substring(pat, theToken, startSubstring, nChars);

			addToTokenQueue(theToken);
		}
	}

	if (0 == m_expression->tokenQueueSize())
	{
		error("Empty expression!");
	}

	m_expression->setTokenPosition(0);
}



bool
XPathProcessorImpl::mapPatternElemPos(
			int 	nesting,
			bool	isStart,
			bool	isAttrName) const
{
	if(0 == nesting)
	{
		if(!isStart)
		{
			m_expression->adjustPattern(m_expression->patternMapSize() - 1,
										-TARGETEXTRA);
		}

		const int	theValue =
			m_expression->tokenQueueSize() - (isAttrName ? 1 : 0) + TARGETEXTRA;

		m_expression->pushPattern(theValue);

		isStart = false;
	}

	return isStart;
}



void
XPathProcessorImpl::addToTokenQueue(const XalanDOMString&	s) const
{
	assert(m_xpath != 0);
	assert(m_expression != 0);

	m_expression->pushToken(s);
}



void
XPathProcessorImpl::replaceTokenWithNamespaceToken() const
{
	assert(m_xpath != 0);
	assert(m_expression != 0);
	assert(m_token.size() > 0);

	StringToStringMapType::const_iterator	i = m_namespaces.find(m_token);
	assert(i != m_namespaces.end());

	const XalanDOMString&	theNamespaceURI = (*i).second;
	assert(theNamespaceURI.size() != 0);

	m_expression->replaceRelativeToken(
			-1,
			theNamespaceURI);
}



int
XPathProcessorImpl::mapNSTokens(
			const XalanDOMString&	pat,
			int 					startSubstring,
			int 					posOfNSSep,
			int 					posOfScan)
{
	assert(m_prefixResolver != 0);

	const XalanDOMString 	prefix(pat, startSubstring, posOfNSSep - startSubstring);

	if (XalanQName::isValidNCName(prefix) == false)
	{
		error(XalanDOMString("'") + prefix + XalanDOMString("' is not a valid NCName"));
	}

	const XalanDOMString* const		uName =
				m_prefixResolver->getNamespaceForPrefix(prefix);

	if(uName == 0)
	{
		error(
			TranscodeFromLocalCodePage("Unable to resolve prefix '") +
			prefix +
			TranscodeFromLocalCodePage("'."));
	}
	else if (length(*uName) == 0)
	{
		error(
			TranscodeFromLocalCodePage("The prefix '") +
			prefix +
			TranscodeFromLocalCodePage("' is bound to a zero-length URI."));
	}
	else
	{
		m_namespaces[prefix] = *uName;

		addToTokenQueue(prefix);

		addToTokenQueue(DOMServices::s_XMLNamespaceSeparatorString);

		// If there's no local part, then don't bother.  We need to check
		// this because '*' tokenizes separately, which means "ns:*" tokenizes
		// differently from "ns:foo".  In the first case, '*' will be tokenized
		// _after_ this code, in the second 'ns:foo' will be split into tokens
		// here...
		if(posOfNSSep + 1 < posOfScan)
		{
			const XalanDOMString 	s(pat, posOfNSSep + 1, posOfScan - (posOfNSSep + 1));

			assert(length(s) > 0);

			if (XalanQName::isValidNCName(s) == false)
			{
				error(XalanDOMString("'") + s + XalanDOMString("' is not a valid NCName"));
			}
			else
			{
				addToTokenQueue(s);
			}
		}
	}

	return -1;
}



int
XPathProcessorImpl::getTokenQueuePosFromMap(int 	i) const
{
	assert(m_expression != 0);

	const int	pos = m_expression->getPattern(i);

	return pos >= TARGETEXTRA ? pos - TARGETEXTRA : pos;
}



bool
XPathProcessorImpl::tokenIs(const XalanDOMString&	s) const
{
	return equals(m_token, s);
}



bool
XPathProcessorImpl::tokenIs(const XalanDOMChar*		s) const
{
	return equals(m_token, s);
}



bool
XPathProcessorImpl::tokenIs(const char*		s) const
{
	const unsigned int	theTokenLength = length(m_token);

	const unsigned int	theStringLength = XalanDOMString::length(s);

	if (theTokenLength != theStringLength)
	{
		return false;
	}
	else
	{
		unsigned int	i = 0;

		while(i < theStringLength)
		{
			if (charAt(m_token, i ) != s[i])
			{
				break;
			}
			else
			{
				++i;
			}
		}

		return i == theStringLength ? true : false;
	}
}



bool
XPathProcessorImpl::tokenIs(char	c) const
{
	return m_tokenChar == c ? true : false;
}



bool
XPathProcessorImpl::lookahead(
			XalanDOMChar	c,
			int 			n) const
{
	const XalanDOMString& 	tok =
		getTokenRelative(n - 1);

	if (length(tok) == 1 &&
		charAt(tok, 0) == c)
	{
		return true;
	}
	else
	{
		return false;
	}
}



bool
XPathProcessorImpl::lookahead(
			const XalanDOMChar* 	s,
			int 					n) const
{
	assert(s != 0);

	const XalanDOMString& 	tok =
		getTokenRelative(n - 1);

	return equals(tok, s);
}



bool
XPathProcessorImpl::lookahead(
			const XalanDOMString&	s,
			int						n) const
{
	const XalanDOMString& 	tok =
		getTokenRelative(n - 1);

	return equals(tok, s);
}



bool
XPathProcessorImpl::lookbehind(
			char	c,
			int 	n) const
{
	const XalanDOMString& 	tok =
		getTokenRelative(-(n + 1));

	if (length(tok) == 1 &&
		charAt(tok, 0) == c)
	{
		return true;
	}
	else
	{
		return false;
	}
}



bool
XPathProcessorImpl::lookbehindHasToken(int	n) const
{
	const XalanDOMString& 	tok =
		getTokenRelative(-(n + 1));

	const XalanDOMChar 		c0 = length(tok) == 0 ? XalanUnicode::charVerticalLine : charAt(tok, 0);

	return c0 == XalanUnicode::charVerticalLine ? false : true;
}



void
XPathProcessorImpl::nextToken()
{
	assert(m_expression != 0);

	const XObject* const	theNextToken =
			m_expression->getNextToken();

	if (theNextToken == 0)
	{
		clear(m_token);
	}
	else
	{
		m_token = theNextToken->str();
	}

	if(length(m_token) > 0)
	{
		m_tokenChar = charAt(m_token, 0);
	}
	else
	{
		m_tokenChar = 0;
	}
}



void
XPathProcessorImpl::prevToken()
{
	assert(m_expression != 0);

	const XObject* const	thePreviousToken =
			m_expression->getPreviousToken();

	m_token = thePreviousToken == 0 ? XalanDOMString() : thePreviousToken->str();

	if(length(m_token) > 0)
	{
		m_tokenChar = charAt(m_token, 0);
	}
	else
	{
		m_tokenChar = 0;
	}
}



const XalanDOMString&
XPathProcessorImpl::getTokenRelative(int	theOffset) const
{
	assert(m_expression != 0);

	const XObject* const	theToken =
		m_expression->getRelativeToken(theOffset);

	return theToken == 0 ? s_emptyString : theToken->str();
}



void
XPathProcessorImpl::resetTokenMark(int	mark)
{
	m_expression->setTokenPosition(mark);

	nextToken();
}



void
XPathProcessorImpl::consumeExpected(const char* 	expected)
{
	if(tokenIs(expected) == true)
	{
		nextToken();
	}
	else
	{
		error(TranscodeFromLocalCodePage("Expected ") +
			  TranscodeFromLocalCodePage(expected) +
			  TranscodeFromLocalCodePage(", but found: ") +
			  m_token);
	}
}



void
XPathProcessorImpl::consumeExpected(char	expected)
{
	if(tokenIs(expected) == true)
	{
		nextToken();
	}
	else
	{
		XalanDOMString	theMsg(TranscodeFromLocalCodePage("Expected "));

		append(theMsg, expected);
		append(theMsg, ", but found: ");
		append(theMsg, m_token);

		error(theMsg);
	}
}



void
XPathProcessorImpl::error(
			const XalanDOMString&	msg,
			XalanNode*				/* sourceNode */) const
{
	XalanDOMString	emsg;

	if (m_expression == 0)
	{
		emsg = msg;
	}
	else
	{
		const XalanDOMString&	theCurrentPattern =
				m_expression->getCurrentPattern();

		DOMStringPrintWriter	thePrintWriter(emsg);

		thePrintWriter.print(msg);

		thePrintWriter.println();

		if (length(theCurrentPattern) != 0)
		{
			if (m_isMatchPattern == true)
			{
				thePrintWriter.print(XALAN_STATIC_UCODE_STRING("pattern = '"));
			}
			else
			{
				thePrintWriter.print(XALAN_STATIC_UCODE_STRING("expression = '"));
			}

			thePrintWriter.print(theCurrentPattern);

			thePrintWriter.print("'");

			if (m_locator != 0)
			{
				const XalanDOMChar* const	theSystemID =
					m_locator->getSystemId();

				thePrintWriter.print("(");

				if (theSystemID == 0)
				{
					thePrintWriter.print("Unknown URI");
				}
				else
				{
					thePrintWriter.print(theSystemID);
				}

				thePrintWriter.print(", ");
				thePrintWriter.print(m_locator->getLineNumber());
				thePrintWriter.print(", ");
				thePrintWriter.print(m_locator->getColumnNumber());

				thePrintWriter.print(")");
			}

			thePrintWriter.println();
		}

		// Back up one token, since we've consumed one...
		m_expression->getPreviousToken();

		// Ask the expression to dump the remaining tokens...
		m_expression->dumpRemainingTokenQueue(thePrintWriter);
	}

	if (m_locator != 0)
	{
		const XalanDOMChar* const	theSystemID =
					m_locator->getSystemId();

		XalanDOMString	theURI;

		if (theSystemID != 0)
		{
			theURI = theSystemID;
		}

		throw XPathParserException(
					emsg,
					theURI,
					m_locator->getLineNumber(),
					m_locator->getColumnNumber());
	}
	else
	{
		throw XPathParserException(emsg);
	}
}



void
XPathProcessorImpl::error(
			const char*		msg,
			XalanNode*		sourceNode) const
{
	error(TranscodeFromLocalCodePage(msg), sourceNode);
}


#if 0
int
XPathProcessorImpl::getFunctionToken(const XalanDOMString&	key) const
{
	if (equals(key, s_commentString) == true)
	{
		return XPathExpression::eNODETYPE_COMMENT;
	}
	else if (equals(key, s_piString) == true)
	{
		return XPathExpression::eNODETYPE_PI;
	}
	else if (equals(key, s_nodeString) == true)
	{
		return XPathExpression::eNODETYPE_NODE;
	}
	else if (equals(key, s_textString) == true)
	{
		return XPathExpression::eNODETYPE_TEXT;
	}
	else
	{
		return -1;
	}
}



XPathExpression::eOpCodes
XPathProcessorImpl::getNodeTypeToken(const XalanDOMString&	key) const
{
	if (equals(key, s_asteriskString) == true)
	{
		return XPathExpression::eNODETYPE_ANYELEMENT;
	}
	else if (equals(key, s_commentString) == true)
	{
		return XPathExpression::eNODETYPE_COMMENT;
	}
	else if (equals(key, s_piString) == true)
	{
		return XPathExpression::eNODETYPE_PI;
	}
	else if (equals(key, s_nodeString) == true)
	{
		return XPathExpression::eNODETYPE_NODE;
	}
	else if (equals(key, s_textString) == true)
	{
		return XPathExpression::eNODETYPE_TEXT;
	}
	else
	{
		return XPathExpression::eENDOP;
	}
}
#endif


void
XPathProcessorImpl::Expr()
{
	OrExpr();
}



void
XPathProcessorImpl::OrExpr()
{
	const int	opPos = m_expression->opCodeMapLength();

	AndExpr();

	if(tokenIs(s_orString) == true)
	{
		nextToken();

		m_expression->insertOpCode(XPathExpression::eOP_OR,
								   opPos);

		OrExpr();

		m_expression->updateOpCodeLength(XPathExpression::eOP_OR,
										 opPos);
	}
}



void
XPathProcessorImpl::AndExpr() 
{
	const int	opPos = m_expression->opCodeMapLength();

	EqualityExpr();

	if(tokenIs(s_andString) == true)
	{
		nextToken();

		m_expression->insertOpCode(XPathExpression::eOP_AND,
								   opPos);

		AndExpr();

		m_expression->updateOpCodeLength(XPathExpression::eOP_AND,
										 opPos);
	}
}



int
XPathProcessorImpl::EqualityExpr(int	opCodePos)
{
	int 		theOpDisplacement = 0;

	const int	opPos = opCodePos != -1 ? opCodePos : m_expression->opCodeMapLength();

	RelationalExpr();

	XPathExpression::eOpCodes	theOpCode =
			XPathExpression::eENDOP;

	if(tokenIs(XalanUnicode::charExclamationMark) && lookahead(XalanUnicode::charEqualsSign, 1))
	{
		nextToken();
		nextToken();

		theOpCode = XPathExpression::eOP_NOTEQUALS;
	}
	else if(tokenIs(XalanUnicode::charEqualsSign))
	{
		nextToken();

		theOpCode = XPathExpression::eOP_EQUALS;
	}

	if (theOpCode != XPathExpression::eENDOP)
	{
		// Save the number of bytes we inserted
		// into the map.
		const int	theLocalDisplacement =
				m_expression->insertOpCode(theOpCode,
										   opPos);

		// Update the length
		m_expression->updateOpCodeLength(theOpCode,
										 opPos);

		// Do the right term of the expression.
		theOpDisplacement += EqualityExpr(opPos);

		// If there's any displacement from the right
		// term, update the length for a shift. Otherwise,
		// just update the length.
		if (theOpDisplacement > 0)
		{
			m_expression->updateShiftedOpCodeLength(theOpCode,
													opPos,
													opPos + theOpDisplacement);
		}
		else
		{
			m_expression->updateOpCodeLength(theOpCode,
											 opPos);
		}

		// Accumulate the displacement.
		theOpDisplacement += theLocalDisplacement;
	}

	return theOpDisplacement;
}



int
XPathProcessorImpl::RelationalExpr(int	opCodePos)
{
	int 		theOpDisplacement = 0;

	const int	opPos = opCodePos != -1 ? opCodePos : m_expression->opCodeMapLength();

	AdditiveExpr();

	if(0 != length(m_token))
	{
		XPathExpression::eOpCodes	theOpCode =
			XPathExpression::eENDOP;

		if(tokenIs(XalanUnicode::charLessThanSign) == true)
		{
			nextToken();

			if(tokenIs(XalanUnicode::charEqualsSign) == true)
			{
				nextToken();

				theOpCode = XPathExpression::eOP_LTE;
			}
			else
			{
				theOpCode = XPathExpression::eOP_LT;
			}
		}
		else if(tokenIs(XalanUnicode::charGreaterThanSign) == true)
		{
			nextToken();

			if(tokenIs(XalanUnicode::charEqualsSign) == true)
			{
				nextToken();

				theOpCode = XPathExpression::eOP_GTE;
			}
			else
			{
				theOpCode = XPathExpression::eOP_GT;
			}
		}

		if (theOpCode != XPathExpression::eENDOP)
		{
			// Save the number of bytes we inserted
			// into the map.
			const int	theLocalDisplacement =
				m_expression->insertOpCode(theOpCode,
										   opPos);

			// Update the length
			m_expression->updateOpCodeLength(theOpCode,
											 opPos);

			// Do the right term of the expression.
			theOpDisplacement += RelationalExpr(opPos);

			// If there's any displacement from the right
			// term, update the length for a shift. Otherwise,
			// just update the length.
			if (theOpDisplacement > 0)
			{
				m_expression->updateShiftedOpCodeLength(theOpCode,
														opPos,
														opPos + theOpDisplacement);
			}
			else
			{
				m_expression->updateOpCodeLength(theOpCode,
												 opPos);
			}

			// Accumulate the displacement.
			theOpDisplacement += theLocalDisplacement;
		}
	}

	return theOpDisplacement;
}



int
XPathProcessorImpl::AdditiveExpr(int	opCodePos)
{
	int 		theOpDisplacement = 0;

	const int	opPos = opCodePos != -1 ? opCodePos : m_expression->opCodeMapLength();

	MultiplicativeExpr();

	if(0 != length(m_token))
	{
		XPathExpression::eOpCodes	theOpCode =
			XPathExpression::eENDOP;

		if(tokenIs(XalanUnicode::charPlusSign) == true)
		{
			theOpCode = XPathExpression::eOP_PLUS;
		}
		else if(tokenIs(XalanUnicode::charHyphenMinus) == true)
		{
			theOpCode = XPathExpression::eOP_MINUS;
		}

		if (theOpCode != XPathExpression::eENDOP)
		{
			nextToken();

			// Save the number of bytes we inserted
			// into the map.
			const int	theLocalDisplacement =
				m_expression->insertOpCode(theOpCode,
										   opPos);

			// Update the length
			m_expression->updateOpCodeLength(theOpCode,
											 opPos);

			// Do the right term of the expression.
			theOpDisplacement += AdditiveExpr(opPos);

			// If there's any displacement from the right
			// term, update the length for a shift. Otherwise,
			// just update the length.
			if (theOpDisplacement > 0)
			{
				m_expression->updateShiftedOpCodeLength(theOpCode,
														opPos,
														opPos + theOpDisplacement);
			}
			else
			{
				m_expression->updateOpCodeLength(theOpCode,
												 opPos);
			}

			// Accumulate the displacement.
			theOpDisplacement += theLocalDisplacement;
		}
	}

	return theOpDisplacement;
}



int
XPathProcessorImpl::MultiplicativeExpr(int	opCodePos)
{
	int 		theOpDisplacement = 0;

	const int	opPos = opCodePos != -1 ? opCodePos : m_expression->opCodeMapLength();

	UnaryExpr();

	if(0 != length(m_token))
	{
		XPathExpression::eOpCodes	theOpCode =
			XPathExpression::eENDOP;

		if(tokenIs(XalanUnicode::charAsterisk) == true)
		{
			theOpCode = XPathExpression::eOP_MULT;
		}
		else if(tokenIs(s_divString) == true)
		{
			theOpCode = XPathExpression::eOP_DIV;
		}
		else if(tokenIs(s_modString) == true)
		{
			theOpCode = XPathExpression::eOP_MOD;
		}

		if (theOpCode != XPathExpression::eENDOP)
		{
			nextToken();

			// Save the number of bytes we inserted
			// into the map.
			const int	theLocalDisplacement =
				m_expression->insertOpCode(theOpCode,
										   opPos);

			// Update the length
			m_expression->updateOpCodeLength(theOpCode,
											 opPos);

			// Do the right term of the expression.
			theOpDisplacement += MultiplicativeExpr(opPos);

			// If there's any displacement from the right
			// term, update the length for a shift. Otherwise,
			// just update the length.
			if (theOpDisplacement > 0)
			{
				m_expression->updateShiftedOpCodeLength(theOpCode,
														opPos,
														opPos + theOpDisplacement);
			}
			else
			{
				m_expression->updateOpCodeLength(theOpCode,
												 opPos);
			}

			// Accumulate the displacement.
			theOpDisplacement += theLocalDisplacement;
		}
	}

	return theOpDisplacement;
}



void
XPathProcessorImpl::UnaryExpr()
{
	const int	opPos = m_expression->opCodeMapLength();

	bool		isNeg = false;

	if(tokenIs(XalanUnicode::charHyphenMinus) == true)
	{
		nextToken();

		m_expression->insertOpCode(XPathExpression::eOP_NEG,
								   opPos);

		isNeg = true;
	}

	UnionExpr();

	if(isNeg == true)
	{
		m_expression->updateOpCodeLength(XPathExpression::eOP_NEG,
										 opPos);
	}
}
  


void
XPathProcessorImpl::BooleanExpr()
{
	const int	opPos = m_expression->opCodeMapLength();

	m_expression->appendOpCode(XPathExpression::eOP_BOOL);

	Expr();

	const int	opLen = m_expression->opCodeMapLength() - opPos;

	if(opLen == 2)
	{
		error("boolean(...) argument is no longer optional with 19990709 XPath draft.");
	}

	m_expression->updateOpCodeLength(XPathExpression::eOP_BOOL,
									 opPos);
}



void
XPathProcessorImpl::UnionExpr()
{
	const int	opPos = m_expression->opCodeMapLength();

	bool		continueOrLoop = true;
	bool		foundUnion = false;

	do 
	{
		PathExpr();

		if(tokenIs(XalanUnicode::charVerticalLine) == true)
		{
			if(false == foundUnion)
			{
				foundUnion = true;

				m_expression->insertOpCode(XPathExpression::eOP_UNION,
										   opPos);
			}

			nextToken();
		}
		else
		{
			if (foundUnion == true)
			{
				// Terminate for safety.
				m_expression->appendOpCode(XPathExpression::eENDOP);
			}

			break;
		}
	}
	while(continueOrLoop == true);

	m_expression->updateOpCodeLength(opPos);
}

 

void
XPathProcessorImpl::PathExpr()
{
	assert(m_expression != 0);

	const int	opPos = m_expression->opCodeMapLength();

	FilterExpr();

	if(tokenIs(XalanUnicode::charSolidus) == true)
	{
		nextToken();

		m_expression->insertOpCode(XPathExpression::eOP_LOCATIONPATH,
								   opPos);

		RelativeLocationPath();

		m_expression->appendOpCode(XPathExpression::eENDOP);

		m_expression->updateOpCodeLength(XPathExpression::eOP_LOCATIONPATH,
										 opPos);
	}
}



void
XPathProcessorImpl::FilterExpr()
{
	assert(m_expression != 0);

	const int	opPos = m_expression->opCodeMapLength();

	//	const bool	isFunc = lookahead(XalanUnicode::charLeftParenthesis, 1);

	PrimaryExpr();

	if(tokenIs(XalanUnicode::charLeftSquareBracket) == true)
	{
		m_expression->insertOpCode(XPathExpression::eOP_LOCATIONPATH,
								   opPos);
	  
		while(tokenIs(XalanUnicode::charLeftSquareBracket) == true)
		{
			Predicate();
		}

		if(tokenIs(XalanUnicode::charSolidus) == true)
		{
			nextToken();

			RelativeLocationPath();
		}

		// Terminate for safety.
		m_expression->appendOpCode(XPathExpression::eENDOP);

		m_expression->updateOpCodeLength(XPathExpression::eOP_LOCATIONPATH,
										 opPos);
	}
}
  


void
XPathProcessorImpl::PrimaryExpr()
{
	assert(m_expression != 0);

	const int	opPos = m_expression->opCodeMapLength();

	if(tokenIs(XalanUnicode::charApostrophe) == true ||
	   tokenIs(XalanUnicode::charQuoteMark) == true)
	{
		m_expression->appendOpCode(XPathExpression::eOP_LITERAL);

		Literal();

		m_expression->updateOpCodeLength(XPathExpression::eOP_LITERAL,
										 opPos);
	}
	else if(tokenIs(XalanUnicode::charDollarSign) == true)
	{
		nextToken(); // consume '$'

		m_expression->appendOpCode(XPathExpression::eOP_VARIABLE);

		QName();

		m_expression->updateOpCodeLength(XPathExpression::eOP_VARIABLE,
											 opPos);
	}
	else if(tokenIs(XalanUnicode::charLeftParenthesis) == true)
	{
		nextToken();

		m_expression->appendOpCode(XPathExpression::eOP_GROUP);

		Expr();

		consumeExpected(XalanUnicode::charRightParenthesis);

		m_expression->updateOpCodeLength(XPathExpression::eOP_GROUP,
										 opPos);
	}
	else if((tokenIs(XalanUnicode::charFullStop) == true &&
				length(m_token) > 1 &&
				XalanXMLChar::isDigit(charAt(m_token, 1)) == true) ||
				XalanXMLChar::isDigit(m_tokenChar) == true)
	{
		m_expression->appendOpCode(XPathExpression::eOP_NUMBERLIT);

		Number();

		m_expression->updateOpCodeLength(XPathExpression::eOP_NUMBERLIT,
										 opPos);
	}
	else if(lookahead(XalanUnicode::charLeftParenthesis, 1) == true ||
			(lookahead(XalanUnicode::charColon, 1) == true && lookahead(XalanUnicode::charLeftParenthesis, 3) == true))
	{
		FunctionCall();
	}
	else
	{
		LocationPath();
	}
}



void
XPathProcessorImpl::Argument()
{
	assert(m_expression != 0);

	const int	opPos = m_expression->opCodeMapLength();

	m_expression->appendOpCode(XPathExpression::eOP_ARGUMENT);

	if (m_requireLiterals == false ||
		isCurrentLiteral() == true)
	{
		Expr();
	}
	else
	{
		error(TranscodeFromLocalCodePage("A literal argument is required!"));
	}

	m_expression->updateOpCodeLength(XPathExpression::eOP_ARGUMENT,
									 opPos);
}



int
XPathProcessorImpl::FunctionCallArguments()
{
	int		argCount = 0;

	consumeExpected(XalanUnicode::charLeftParenthesis);

	while(tokenIs(XalanUnicode::charRightParenthesis) == false && isEmpty(m_token) == false)
	{
		if(tokenIs(XalanUnicode::charComma) == true)
		{
			error("Found ',' but no preceding argument!");
		}

		Argument();

		++argCount;

		if(tokenIs(XalanUnicode::charRightParenthesis) == false)
		{
			consumeExpected(XalanUnicode::charComma);

			if(tokenIs(XalanUnicode::charRightParenthesis) == true)
			{
				error("Found ',' but no following argument!");
			}
		}
	}

	consumeExpected(XalanUnicode::charRightParenthesis);

	return argCount;
}


void
XPathProcessorImpl::FunctionCall()
{
	assert(m_expression != 0);

	const int	opPos = m_expression->opCodeMapLength();

	if(lookahead(XalanUnicode::charColon, 1) == true)
	{
		m_expression->appendOpCode(XPathExpression::eOP_EXTFUNCTION);

		XPathExpression::OpCodeMapValueVectorType	theArgs(2, 0);

		// Replace the token in the queue with the actual namespace URI...
		replaceTokenWithNamespaceToken();

		theArgs[0] = m_expression->getTokenPosition() - 1;

		nextToken();

		consumeExpected(XalanUnicode::charColon);

		theArgs[1] = m_expression->getTokenPosition() - 1;

		m_expression->setOpCodeArgs(XPathExpression::eOP_EXTFUNCTION,
									opPos,
									theArgs);

		nextToken();

		FunctionCallArguments();
	}
	else
	{
		if (isValidFunction(m_token) == false)
		{
			error(TranscodeFromLocalCodePage("Could not find function: ") +
				 m_token +
				 TranscodeFromLocalCodePage("()"));
		}

		const XPathExpression::eOpCodes		funcTok = getFunctionToken(m_token);

		switch(funcTok)
		{
		case XPathExpression::eNODETYPE_PI:
		case XPathExpression::eNODETYPE_COMMENT:
		case XPathExpression::eNODETYPE_TEXT:
		case XPathExpression::eNODETYPE_NODE:
			LocationPath();
			// ************** Nasty return here!!! *********************** //
			return;
			break;

		default:
			{
				// The position must be at least zero, since
				// we've looked at a token.
				assert(m_expression->getTokenPosition() > 0);

				int		theFunctionID =
					XPath::getFunctionTable().nameToID(m_token);

				if (equals(m_token, s_positionString) == true &&
					m_positionPredicateStack.empty() == false)
				{
					m_positionPredicateStack.back() = true;
				}

				XPathExpression::OpCodeMapValueVectorType	theArgs(2, 0);
		
				theArgs[0] = theFunctionID;
				theArgs[1] = 0;

				m_expression->appendOpCode(
						XPathExpression::eOP_FUNCTION,
						theArgs);
			}
		}

		nextToken();

		// Get the arguments, and the argument count...
		const int	argCount = FunctionCallArguments();

		assert(m_expression->m_opMap[opPos + 3] == 0);

		// update the arg count in the op map...
		m_expression->m_opMap[opPos + 3] = argCount;
	}

	// Terminate for safety.
	m_expression->appendOpCode(XPathExpression::eENDOP);

	m_expression->updateOpCodeLength(opPos);
}



void
XPathProcessorImpl::LocationPath()
{
	const int	opPos = m_expression->opCodeMapLength();

	m_expression->appendOpCode(XPathExpression::eOP_LOCATIONPATH);

	if(tokenIs(XalanUnicode::charSolidus) == true)
	{
		const int	newOpPos = m_expression->opCodeMapLength();

		// Tell how long the step is without the predicate
		const XPathExpression::OpCodeMapValueVectorType		theArgs(1, 4);

		m_expression->appendOpCode(XPathExpression::eFROM_ROOT,
								   theArgs);

		m_expression->appendOpCode(XPathExpression::eNODETYPE_ROOT);

		// Tell how long the entire step is.
		m_expression->updateOpCodeLength(newOpPos);

		nextToken();
	}

	if(length(m_token) != 0)
	{
		RelativeLocationPath();
	}

	// Terminate for safety.
	m_expression->appendOpCode(XPathExpression::eENDOP);

	m_expression->updateOpCodeLength(XPathExpression::eOP_LOCATIONPATH,
									 opPos);
}


  
void
XPathProcessorImpl::RelativeLocationPath()
{
	Step();

	while(tokenIs(XalanUnicode::charSolidus) == true)
	{
		nextToken();

		Step();
	}
}
  


void
XPathProcessorImpl::Step()
{
	const int	opPos = m_expression->opCodeMapLength();

	if(tokenIs(s_dotString) == true)
	{
		nextToken();

		if(tokenIs(XalanUnicode::charLeftSquareBracket) == true)
		{
			error("'..[predicate]' or '.[predicate]' is illegal syntax.  Use 'self::node()[predicate]' instead.");
		}

		const XPathExpression::OpCodeMapValueVectorType		theArgs(1, 4);

		m_expression->appendOpCode(XPathExpression::eFROM_SELF,
								   theArgs);

		m_expression->appendOpCode(XPathExpression::eNODETYPE_NODE);

		// Tell how long the entire step is.
		m_expression->updateOpCodeLength(opPos);
	}
	else if(tokenIs(s_dotDotString) == true)
	{
		nextToken();

		// Tell how long the step is without the predicate
		const XPathExpression::OpCodeMapValueVectorType		theArgs(1, 4);

		m_expression->appendOpCode(XPathExpression::eFROM_PARENT,
								   theArgs);

		m_expression->appendOpCode(XPathExpression::eNODETYPE_NODE);

		// Tell how long the entire step is.
		m_expression->updateOpCodeLength(opPos);
	}
	else if (tokenIs(XalanUnicode::charAsterisk) ||
			 tokenIs(XalanUnicode::charCommercialAt) ||
			 tokenIs(XalanUnicode::charSolidus) ||
			 (tokenIs(XalanUnicode::charLowLine) ||
			  XalanXMLChar::isLetter(charAt(m_token, 0))))
	{
		Basis();

		while(tokenIs(XalanUnicode::charLeftSquareBracket) == true)
		{
			Predicate();
		}

		// Tell how long the entire step is.
		m_expression->updateOpCodeLength(opPos);
	}
	else if (tokenIs(XalanUnicode::charRightParenthesis) == false)
	{
		error("Unexpected token!");
	}
}



void
XPathProcessorImpl::Basis()
{
	assert(m_xpath != 0);
	assert(m_expression != 0);

	const int	opPos = m_expression->opCodeMapLength();

	XPathExpression::eOpCodes	axisType = XPathExpression::eENDOP;

	// The next blocks guarantee that a FROM_XXX will be added.
	if(lookahead(s_axisString, 1) == true)
	{
		axisType = AxisName();

		nextToken();
		nextToken();
	}
	else if(tokenIs(XalanUnicode::charCommercialAt) == true)
	{
		axisType = XPathExpression::eFROM_ATTRIBUTES;

		m_expression->appendOpCode(XPathExpression::eFROM_ATTRIBUTES);

		nextToken();
	}
	else if(tokenIs(XalanUnicode::charSolidus) == true)
	{
		// Check the current token in the expression.  It's
		// actually the next token in this context.
		//
		const XalanDOMString&	theNextToken = getTokenRelative(0);

		if (isAxis(theNextToken) == false && isNodeTest(theNextToken) == false)
		{
			nextToken();

			error("Expected axis or node test!");
		}
		else
		{
			// Tell how long the step is without the predicate
			const XPathExpression::OpCodeMapValueVectorType		theArgs(1, 4);

			m_expression->appendOpCode(XPathExpression::eFROM_DESCENDANTS_OR_SELF,
									   theArgs);

			m_expression->appendOpCode(XPathExpression::eNODETYPE_NODE);

			// Tell how long the step is without the predicate
			m_expression->updateOpCodeLengthAfterNodeTest(opPos);

			return; // make a quick exit...
		}
	}
	else
	{
		axisType = XPathExpression::eFROM_CHILDREN;

		m_expression->appendOpCode(XPathExpression::eFROM_CHILDREN);
	}

	NodeTest(axisType);

	// Tell how long the step is without the predicate
	m_expression->updateOpCodeLengthAfterNodeTest(opPos);
}



XPathExpression::eOpCodes
XPathProcessorImpl::AxisName()
{
	assert(m_xpath != 0);
	assert(m_expression != 0);

	const XPathExpression::eOpCodes		theOpCode =
		getAxisToken(m_token);

	if (theOpCode == XPathExpression::eENDOP)
	{
		error(TranscodeFromLocalCodePage("Illegal axis name: ") + m_token);
	}
	else
	{
		m_expression->appendOpCode(theOpCode);
	}

	return theOpCode;
}



int
XPathProcessorImpl::NodeTest(XPathExpression::eOpCodes	axisType)
{
	assert(m_xpath != 0);
	assert(m_expression != 0);

	int		nodeTestPos = -1;

	if(lookahead(XalanUnicode::charLeftParenthesis, 1) == true)
	{
		const XPathExpression::eOpCodes		theOpCode = getNodeTypeToken(m_token);

		if (theOpCode == XPathExpression::eENDOP)
		{
			error(TranscodeFromLocalCodePage("Unknown nodetype: ") +
				  m_token);
		}
		else
		{
			nextToken();

			nodeTestPos = m_expression->appendOpCode(theOpCode);

			consumeExpected(XalanUnicode::charLeftParenthesis);

			if(XPathExpression::eNODETYPE_PI == theOpCode)
			{
				if(tokenIs(XalanUnicode::charRightParenthesis) == false)
				{
					Literal();
				}
			}

			consumeExpected(XalanUnicode::charRightParenthesis);
		}
	}
	else
	{
		// Assume name of attribute or element.
		m_expression->appendOpCode(XPathExpression::eNODENAME);

		if(lookahead(XalanUnicode::charColon, 1) == true)
		{
			if(tokenIs(XalanUnicode::charAsterisk) == true)
			{
				m_expression->appendOpCode(XPathExpression::eELEMWILDCARD);
			}
			else
			{
				// Replace the token in the queue with the actual namespace...
				replaceTokenWithNamespaceToken();

				m_expression->pushCurrentTokenOnOpCodeMap();
			}

			nextToken();

			consumeExpected(XalanUnicode::charColon);
		}
		else
		{
			m_expression->appendOpCode(XPathExpression::eEMPTY);
		}

		if(tokenIs(XalanUnicode::charAsterisk) == true)
		{
			m_expression->appendOpCode(XPathExpression::eELEMWILDCARD);
		}
		else if (isNodeTest(m_token) == false)
		{
			error("Expected node test!");
		}
		else
		{
			m_expression->pushCurrentTokenOnOpCodeMap();
		}

		nextToken();
	}

	return nodeTestPos;
}



void
XPathProcessorImpl::Predicate()
{
	if(tokenIs(XalanUnicode::charLeftSquareBracket) == true)
	{
		nextToken();

		PredicateExpr();		

		consumeExpected(XalanUnicode::charRightSquareBracket);
	}
}



void
XPathProcessorImpl::PredicateExpr()
{
	assert(m_xpath != 0);
	assert(m_expression != 0);

	const int	opPos = m_expression->opCodeMapLength();

	m_expression->appendOpCode(XPathExpression::eOP_PREDICATE);

	m_positionPredicateStack.push_back(false);

	Expr();

	// Terminate for safety.
	m_expression->appendOpCode(XPathExpression::eENDOP);

	m_expression->updateOpCodeLength(XPathExpression::eOP_PREDICATE,
									 opPos);

	assert(m_positionPredicateStack.empty() == false);

	if (m_positionPredicateStack.back() == true)
	{
		m_expression->replaceOpCode(
			opPos,
			XPathExpression::eOP_PREDICATE,
			XPathExpression::eOP_PREDICATE_WITH_POSITION);
	}

	m_positionPredicateStack.pop_back();
}



void
XPathProcessorImpl::QName()
{
	assert(m_xpath != 0);
	assert(m_expression != 0);

	// If there is no prefix, we have to fake things out...
	if (lookahead(XalanUnicode::charColon, 1) == false)
	{
		m_expression->insertToken(XalanDOMString());

		m_expression->pushCurrentTokenOnOpCodeMap();

		nextToken();
	}
	else
	{
		// Replace the token in the queue with the actual namespace...
		replaceTokenWithNamespaceToken();

		m_expression->pushCurrentTokenOnOpCodeMap();

		nextToken();

		consumeExpected(XalanUnicode::charColon);
	}

	m_expression->pushCurrentTokenOnOpCodeMap();

	nextToken();
}



void
XPathProcessorImpl::NCName()
{
	assert(m_xpath != 0);
	assert(m_expression != 0);

	m_expression->pushCurrentTokenOnOpCodeMap();

	nextToken();
}



void
XPathProcessorImpl::Literal()
{
	assert(m_xpath != 0);
	assert(m_expression != 0);

	if(isCurrentLiteral() == true)
	{
		const XalanDOMString	theArgument(m_token, 1, length(m_token) - 2);

		m_expression->pushArgumentOnOpCodeMap(theArgument);

		nextToken();
	}
	else
	{
		error(TranscodeFromLocalCodePage("Pattern literal (") +
			  m_token +
			  TranscodeFromLocalCodePage(") needs to be quoted!"));
	}
}



void
XPathProcessorImpl::Number()
{
	assert(m_xpath != 0);
	assert(m_expression != 0);

	if(0 != length(m_token))
	{
		const double	num = DoubleSupport::toDouble(m_token);

		m_expression->pushNumberLiteralOnOpCodeMap(num);

		m_expression->pushArgumentOnOpCodeMap(num);

		nextToken();
	}
}



void
XPathProcessorImpl::Pattern()
{
	while(true)
	{
		LocationPathPattern();

		if(tokenIs(XalanUnicode::charVerticalLine) == true)
		{
			nextToken();
		}
		else
		{
			break;
		}
	}
}



void
XPathProcessorImpl::LocationPathPattern()
{
	assert(m_xpath != 0);
	assert(m_expression != 0);

	const int	opPos = m_expression->opCodeMapLength();

	m_expression->appendOpCode(XPathExpression::eOP_LOCATIONPATHPATTERN);

	if(lookahead(XalanUnicode::charLeftParenthesis, 1) == true &&
				(tokenIs(s_functionIDString) == true ||
				 tokenIs(s_functionKeyString) == true))
	{
		IdKeyPattern();

		if(tokenIs(XalanUnicode::charSolidus) == true && lookahead(XalanUnicode::charSolidus, 1) == true)
		{
			const int	newOpPos = m_expression->opCodeMapLength();

			// Tell how long the step is without the predicate
			const XPathExpression::OpCodeMapValueVectorType		theArgs(1, 4);

			m_expression->appendOpCode(XPathExpression::eMATCH_ANY_ANCESTOR_WITH_FUNCTION_CALL,
									   theArgs);

			m_expression->updateOpCodeLength(newOpPos);

			nextToken();
		}
	}
	else if(tokenIs(XalanUnicode::charSolidus) == true)
	{
		const int	newOpPos = m_expression->opCodeMapLength();

		// Tell how long the step is without the predicate
		const XPathExpression::OpCodeMapValueVectorType		theArgs(1, 4);

		if(lookahead(XalanUnicode::charSolidus, 1) == true)
		{
			m_expression->appendOpCode(
				XPathExpression::eMATCH_ANY_ANCESTOR_WITH_PREDICATE,
				theArgs);

			m_expression->appendOpCode(XPathExpression::eNODETYPE_NODE);

			nextToken();
		}
		else
		{
			m_expression->appendOpCode(XPathExpression::eFROM_ROOT,
									   theArgs);

			m_expression->appendOpCode(XPathExpression::eNODETYPE_ROOT);
		}

		m_expression->updateOpCodeLength(newOpPos);

		nextToken();
	}

	if(!tokenIs(XalanUnicode::charVerticalLine) == true && length(m_token) != 0)
	{
		RelativePathPattern();
	}

	// Terminate for safety.
	m_expression->appendOpCode(XPathExpression::eENDOP);

	m_expression->updateOpCodeLength(XPathExpression::eOP_LOCATIONPATHPATTERN,
									 opPos);
}



void
XPathProcessorImpl::IdKeyPattern()
{
	m_requireLiterals = true;

	FunctionCall();

	m_requireLiterals = false;
}



void
XPathProcessorImpl::RelativePathPattern()
{	 
	StepPattern();

	while(tokenIs(XalanUnicode::charSolidus) == true)
	{
		nextToken();

		StepPattern();
	}
}



void
XPathProcessorImpl::StepPattern()
{
	AbbreviatedNodeTestStep();
}



void
XPathProcessorImpl::AbbreviatedNodeTestStep()
{
	assert(m_xpath != 0);
	assert(m_expression != 0);

	const int	opPos = m_expression->opCodeMapLength();

	int			matchTypePos = -1;

	XPathExpression::eOpCodes	axisType = XPathExpression::eENDOP;

	// The next blocks guarantee that a MATCH_XXX will be added.
	if(tokenIs(XalanUnicode::charCommercialAt) == true)
	{
		axisType = XPathExpression::eMATCH_ATTRIBUTE;

		m_expression->appendOpCode(axisType);

		nextToken();
	}
	else if(lookahead(s_axisString, 1) == true)
	{
		if(tokenIs(s_attributeString) == true)
		{
			axisType = XPathExpression::eMATCH_ATTRIBUTE;

			m_expression->appendOpCode(axisType);
		}
		else if(tokenIs(s_childString) == true)
		{
			matchTypePos = m_expression->opCodeMapLength();

			axisType = XPathExpression::eMATCH_IMMEDIATE_ANCESTOR;

			m_expression->appendOpCode(axisType);
		}
		else
		{
			error("Only child:: and attribute:: axes are allowed in match patterns!");
		}

		nextToken();
		nextToken();
	}
	else if(tokenIs(XalanUnicode::charSolidus) == true)
	{
		if(lookahead(s_axisString, 2) == false)
		{
			matchTypePos = m_expression->opCodeMapLength();

			axisType = XPathExpression::eMATCH_IMMEDIATE_ANCESTOR;

			m_expression->appendOpCode(axisType);
		}
		else
		{
			nextToken();

			if(tokenIs(s_attributeString) == true)
			{
				axisType = XPathExpression::eMATCH_ATTRIBUTE;

				m_expression->appendOpCode(axisType);
			}
			else if(tokenIs(s_childString) == true)
			{
				matchTypePos = m_expression->opCodeMapLength();

				axisType = XPathExpression::eMATCH_IMMEDIATE_ANCESTOR;

				m_expression->appendOpCode(axisType);
			}
			else
			{
				error("Only child:: and attribute:: axes are allowed in match patterns!");
			}

			nextToken();

		}

		nextToken();
	}
	else
	{
		if(tokenIs(XalanUnicode::charSolidus) == true)
		{
			nextToken();
		}

		matchTypePos = m_expression->opCodeMapLength();

		axisType = XPathExpression::eMATCH_IMMEDIATE_ANCESTOR;

		m_expression->appendOpCode(axisType);
	}

    // Make room for telling how long the step is without the predicate.
	// This will be replaced by the right value.
	m_expression->appendOpCode(XPathExpression::eENDOP);

	NodeTest(axisType);

	m_expression->updateOpCodeLengthAfterNodeTest(opPos);

	while(tokenIs(XalanUnicode::charLeftSquareBracket) == true)
	{
		Predicate();
	}

	if(matchTypePos > -1 && tokenIs(XalanUnicode::charSolidus) == true && lookahead(XalanUnicode::charSolidus, 1) == true)
	{
		assert(m_expression->opCodeMapLength() > matchTypePos);

		m_expression->m_opMap[matchTypePos] = XPathExpression::eMATCH_ANY_ANCESTOR;
	}

	m_expression->updateOpCodeLength(opPos);
}



bool
XPathProcessorImpl::isValidFunction(const XalanDOMString&	key)
{
	bool	fResult = true;

	if(XPath::isInstalledFunction(key) == false)
	{
		if (searchTable(s_functionTable, s_functionTableSize, key).m_opCode == XPathExpression::eENDOP)
		{
			fResult = false;
		}
	}

	return fResult;
}



bool
XPathProcessorImpl::isCurrentLiteral() const
{
	const int last = length(m_token) - 1;

	if (last <= 0)
	{
		return false;
	}
	else
	{
		assert(last > 0);

		const XalanDOMChar 	c0 = m_tokenChar;
		const XalanDOMChar 	cX = charAt(m_token, last);

		if((c0 == XalanUnicode::charQuoteMark && cX == XalanUnicode::charQuoteMark) ||
		   (c0 == XalanUnicode::charApostrophe && cX == XalanUnicode::charApostrophe))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}



bool
XPathProcessorImpl::isAxis(const XalanDOMString&	theToken)
{
	const XalanDOMString::size_type		theLength = length(theToken);

	if (theLength == 0)
	{
		return false;
	}
	else if (theLength == 1 &&
			 charAt(theToken, 0) == XalanUnicode::charCommercialAt)
	{
		return true;
	}
	else
	{
		const XPathExpression::eOpCodes		theOpCode =
			getAxisToken(theToken);

		if (theOpCode != XPathExpression::eENDOP)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}



bool
XPathProcessorImpl::isNodeTest(const XalanDOMString&	theToken)
{
	const XalanDOMString::size_type		theLength = length(theToken);

	if (theLength == 0)
	{
		return false;
	}
	else if (theLength == 1 &&
			 charAt(theToken, 0) == XalanUnicode::charAsterisk)
	{
		return true;
	}
	else if (charAt(theToken, 0) == XalanUnicode::charLowLine ||
			 XalanXMLChar::isLetter(charAt(theToken, 0)) == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}



const XPathProcessorImpl::TableEntry&
XPathProcessorImpl::searchTable(
		const TableEntry		theTable[],
		size_type				theTableSize,
		const XalanDOMString&	theString)
{
	const TableEntry*	theFirst = theTable;
	const TableEntry*	theLast = &theTable[theTableSize - 1];

	while(theFirst <= theLast)
	{
		const TableEntry*	theCurrent = theFirst + (theLast - theFirst) / 2;
		assert(theCurrent->m_string[0] != 0);

		const int	theResult = compare(c_wstr(theString), theCurrent->m_string);

		if (theResult < 0)
		{
			theLast = theCurrent - 1;
		}
		else if (theResult > 0)
		{
			theFirst = theCurrent + 1;
		}
		else
		{
			return *theCurrent;
		}
	}

	return s_dummyEntry;
}



const XalanDOMString	XPathProcessorImpl::s_emptyString;



const XalanDOMChar	XPathProcessorImpl::s_functionIDString[] =
{
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_d,
	0
};

// This shouldn't really be here, since it's not part of the XPath standard,
// but rather a part ofthe XSLT standard.
const XalanDOMChar	XPathProcessorImpl::s_functionKeyString[] =
{
	XalanUnicode::charLetter_k,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_y,
	0
};

const XalanDOMChar	XPathProcessorImpl::s_orString[] =
{
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_r,
	0
};

const XalanDOMChar	XPathProcessorImpl::s_andString[] =
{
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_d,
	0
};

const XalanDOMChar	XPathProcessorImpl::s_divString[] =
{
	XalanUnicode::charLetter_d,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_v,
	0
};

const XalanDOMChar	XPathProcessorImpl::s_modString[] =
{
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_d,
	0
};

const XalanDOMChar	XPathProcessorImpl::s_dotString[] =
{
	XalanUnicode::charFullStop,
	0
};

const XalanDOMChar	XPathProcessorImpl::s_dotDotString[] =
{
	XalanUnicode::charFullStop,
	XalanUnicode::charFullStop,
	0
};

const XalanDOMChar	XPathProcessorImpl::s_axisString[] =
{
	XalanUnicode::charColon,
	XalanUnicode::charColon,
	0
};

const XalanDOMChar	XPathProcessorImpl::s_attributeString[] =
{
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_b,
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_e,
	0
};

const XalanDOMChar	XPathProcessorImpl::s_childString[] =
{
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_h,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_d,
	0
};

const XalanDOMChar	XPathProcessorImpl::s_positionString[] =
{
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_n,
	0
};

const XalanDOMChar	XPathProcessorImpl::s_asteriskString[] =
{
	XalanUnicode::charAsterisk,
	0
};

const XalanDOMChar	XPathProcessorImpl::s_commentString[] =
{
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_t,
	0
};

const XalanDOMChar	XPathProcessorImpl::s_piString[] =
{
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_g,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_n,
	0
};

const XalanDOMChar	XPathProcessorImpl::s_nodeString[] =
{
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_d,
	XalanUnicode::charLetter_e,
	0
};

const XalanDOMChar	XPathProcessorImpl::s_textString[] =
{
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_x,
	XalanUnicode::charLetter_t,
	0
};

const XalanDOMChar		XPathProcessorImpl::s_ancestorString[] =
{
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_r,
	0
};

const XalanDOMChar		XPathProcessorImpl::s_ancestorOrSelfString[] =
{
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_r,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_r,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_f,
	0
};

const XalanDOMChar		XPathProcessorImpl::s_descendantString[] =
{
	XalanUnicode::charLetter_d,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_d,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_t,
	0
};

const XalanDOMChar		XPathProcessorImpl::s_descendantOrSelfString[] =
{
	XalanUnicode::charLetter_d,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_d,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_t,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_r,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_f,
	0
};

const XalanDOMChar		XPathProcessorImpl::s_followingString[] =
{
	XalanUnicode::charLetter_f,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_w,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_g,
	0
};

const XalanDOMChar		XPathProcessorImpl::s_followingSiblingString[] =
{
	XalanUnicode::charLetter_f,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_w,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_g,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_b,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_g,
	0
};

const XalanDOMChar		XPathProcessorImpl::s_parentString[] =
{
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_t,
	0
};

const XalanDOMChar		XPathProcessorImpl::s_precedingString[] =
{
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_d,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_g,
	0
};

const XalanDOMChar		XPathProcessorImpl::s_precedingSiblingString[] =
{
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_d,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_g,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_b,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_g,
	0
};

const XalanDOMChar		XPathProcessorImpl::s_selfString[] =
{
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_f,
	0
};

const XalanDOMChar		XPathProcessorImpl::s_namespaceString[] =
{
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_e,
	0
};



const XPathProcessorImpl::TableEntry	XPathProcessorImpl::s_functionTable[] =
{
	{ XPathProcessorImpl::s_nodeString, XPathExpression::eNODETYPE_NODE },
	{ XPathProcessorImpl::s_textString, XPathExpression::eNODETYPE_TEXT },
	{ XPathProcessorImpl::s_commentString, XPathExpression::eNODETYPE_COMMENT },
	{ XPathProcessorImpl::s_piString, XPathExpression::eNODETYPE_PI },
};

const XPathProcessorImpl::size_type		XPathProcessorImpl::s_functionTableSize =
	sizeof(s_functionTable) / sizeof(s_functionTable[0]);



const XPathProcessorImpl::TableEntry	XPathProcessorImpl::s_nodeTypeTable[] =
{
	{ XPathProcessorImpl::s_asteriskString, XPathExpression::eNODETYPE_ANYELEMENT },
	{ XPathProcessorImpl::s_nodeString, XPathExpression::eNODETYPE_NODE },
	{ XPathProcessorImpl::s_textString, XPathExpression::eNODETYPE_TEXT },
	{ XPathProcessorImpl::s_commentString, XPathExpression::eNODETYPE_COMMENT },
	{ XPathProcessorImpl::s_piString, XPathExpression::eNODETYPE_PI },
};

const XPathProcessorImpl::size_type		XPathProcessorImpl::s_nodeTypeTableSize =
	sizeof(s_nodeTypeTable) / sizeof(s_nodeTypeTable[0]);



const XPathProcessorImpl::TableEntry	XPathProcessorImpl::s_axisTable[] =
{
	{ XPathProcessorImpl::s_selfString, XPathExpression::eFROM_SELF },
	{ XPathProcessorImpl::s_childString, XPathExpression::eFROM_CHILDREN },
	{ XPathProcessorImpl::s_parentString, XPathExpression::eFROM_PARENT },
	{ XPathProcessorImpl::s_ancestorString, XPathExpression::eFROM_ANCESTORS },
	{ XPathProcessorImpl::s_attributeString, XPathExpression::eFROM_ATTRIBUTES },
	{ XPathProcessorImpl::s_followingString, XPathExpression::eFROM_FOLLOWING },
	{ XPathProcessorImpl::s_namespaceString, XPathExpression::eFROM_NAMESPACE },
	{ XPathProcessorImpl::s_precedingString, XPathExpression::eFROM_PRECEDING },
	{ XPathProcessorImpl::s_descendantString, XPathExpression::eFROM_DESCENDANTS },
	{ XPathProcessorImpl::s_ancestorOrSelfString, XPathExpression::eFROM_ANCESTORS_OR_SELF },
	{ XPathProcessorImpl::s_followingSiblingString, XPathExpression::eFROM_FOLLOWING_SIBLINGS },
	{ XPathProcessorImpl::s_precedingSiblingString, XPathExpression::eFROM_PRECEDING_SIBLINGS },
	{ XPathProcessorImpl::s_descendantOrSelfString, XPathExpression::eFROM_DESCENDANTS_OR_SELF },
};

const XPathProcessorImpl::size_type		XPathProcessorImpl::s_axisTableSize =
	sizeof(s_axisTable) / sizeof(s_axisTable[0]);


const XPathProcessorImpl::TableEntry	XPathProcessorImpl::s_dummyEntry =
{
	0, XPathExpression::eENDOP
};
