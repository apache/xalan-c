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
 *
 * @author <a href="mailto:david_n_bertoni@lotus.com">David N. Bertoni</a>
 */

// Class header file...
#include "XPathProcessorImpl.hpp"



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/DOMStringPrintWriter.hpp>
#include <PlatformSupport/DoubleSupport.hpp>



#include <DOMSupport/DOMServices.hpp>



#include "PrefixResolver.hpp"
#include "XPathEnvSupport.hpp"
#include "XPathExecutionContext.hpp"
#include "XPathParserException.hpp"
#include "XPathSupport.hpp"



XPathProcessorImpl::XPathProcessorImpl() :
	m_token(),
	m_tokenChar(0),
	m_xpath(0),
	m_expression(0),
	m_prefixResolver(0),
	m_envSupport(0)
{
}



XPathProcessorImpl::~XPathProcessorImpl()
{
}



void
XPathProcessorImpl::initXPath(
			XPath&						pathObj,
			const XalanDOMString&		expression,
			const PrefixResolver&		prefixResolver,
			const XPathEnvSupport&		envSupport)
{
	m_xpath = &pathObj;
	m_expression = &m_xpath->getExpression();

	m_prefixResolver = &prefixResolver;
	m_envSupport = &envSupport;

	tokenize(expression);

	m_expression->appendOpCode(XPathExpression::eOP_XPATH);

	nextToken();

	Expr();

	m_xpath = 0;
	m_expression = 0;
	m_prefixResolver = 0;
	m_envSupport = 0;
}



void
XPathProcessorImpl::initMatchPattern(
			XPath&					pathObj,
			const XalanDOMString&	expression,
			const PrefixResolver&	prefixResolver,
			const XPathEnvSupport&	envSupport)
{
	m_xpath = &pathObj;
	m_expression = &m_xpath->getExpression();

	m_prefixResolver = &prefixResolver;
	m_envSupport = &envSupport;
	m_expression->reset();

	tokenize(expression);

	m_expression->appendOpCode(XPathExpression::eOP_MATCHPATTERN);

	nextToken();

	Pattern();

	// Terminate for safety.
	m_expression->appendOpCode(XPathExpression::eENDOP);

	m_expression->shrink();

	m_xpath = 0;
	m_expression = 0;
	m_prefixResolver = 0;
	m_envSupport = 0;
}



void
XPathProcessorImpl::tokenize(
			const XalanDOMString&	pat,
			DOMStringVectorType* 	targetStrings)
{
	m_expression->setCurrentPattern(pat);

	const int	nChars = length(pat);

	int 		startSubstring = -1;
	int 		posOfNSSep = -1;

	bool		isStartOfPat = true;
	bool		isAttrName = false;
	bool		isNum = false;

	// Nesting of '[' so we can know if the given element should be 
	// counted inside the m_patternMap.
	int nesting = 0;

	// char[] chars = pat.toCharArray();
	for(int i = 0; i < nChars; i++)
	{
		XalanDOMChar	c = charAt(pat, i);

		switch(c)
		{
		case XalanUnicode::charQuoteMark: 
			{
				if(startSubstring != -1)
				{
					isNum = false;
					isStartOfPat = mapPatternElemPos(nesting, isStartOfPat, isAttrName);

					isAttrName = false;

					if(-1 != posOfNSSep)
					{	   
						posOfNSSep = mapNSTokens(pat, startSubstring, posOfNSSep, i);
					}
					else
					{
						addToTokenQueue(substring(pat, startSubstring, i));
					}
				}

				startSubstring = i;

				for(++i; i < nChars && (c = charAt(pat, i)) != XalanUnicode::charQuoteMark; ++i);

				if(c == XalanUnicode::charQuoteMark)
				{
					addToTokenQueue(substring(pat, startSubstring, i + 1));
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
					isNum = false;
					isStartOfPat = mapPatternElemPos(nesting, isStartOfPat, isAttrName);
					isAttrName = false;

					if(-1 != posOfNSSep)
					{	 
						posOfNSSep = mapNSTokens(pat, startSubstring, posOfNSSep, i);
					}
					else
					{
						addToTokenQueue(substring(pat, startSubstring, i));
					}
				}

				startSubstring = i;

				for(++i; i < nChars && (c = charAt(pat, i)) != XalanUnicode::charApostrophe; ++i);

				if(c == XalanUnicode::charApostrophe)
				{
					addToTokenQueue(substring(pat, startSubstring, i + 1));
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
					isNum = false;
					isStartOfPat = mapPatternElemPos(nesting, isStartOfPat, isAttrName);
					isAttrName = false;

					if(-1 != posOfNSSep)
					{	 
						posOfNSSep = mapNSTokens(pat, startSubstring, posOfNSSep, i);
					}
					else
					{
						addToTokenQueue(substring(pat, startSubstring, i));
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
					if(!(isNum || startSubstring == -1))
					{
						break;
					}

					isNum = false;
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
					isNum = false;
					isStartOfPat = mapPatternElemPos(nesting, isStartOfPat, isAttrName);
					isAttrName = false;

					if(-1 != posOfNSSep)
					{	 
						posOfNSSep = mapNSTokens(pat, startSubstring, posOfNSSep, i);
					}
					else
					{
						addToTokenQueue(substring(pat, startSubstring, i));
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
						if(0 != targetStrings)
						{
							recordTokenString(*targetStrings);
						}

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

				addToTokenQueue(substring(pat, i, i + 1));
			}		
			break;

		case XalanUnicode::charColon:
			{
				if(posOfNSSep == i - 1)
				{ 
					if(startSubstring != -1)
					{
						if (startSubstring < i - 1)
						{
							addToTokenQueue(substring(pat, startSubstring, i - 1));
						}
					}

					isNum = false;
					isAttrName = false;
					startSubstring = -1;
					posOfNSSep = -1;

					addToTokenQueue(substring(pat, i - 1, i + 1));
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

					isNum = isXMLDigit(c);
				}
				else if(isNum == true)
				{
					isNum = isXMLDigit(c);
				}
			}
		}
	}

	if(startSubstring != -1)
	{
		isNum = false;

		isStartOfPat = mapPatternElemPos(nesting, isStartOfPat, isAttrName);

		if(-1 != posOfNSSep)
		{	 
			posOfNSSep = mapNSTokens(pat, startSubstring, posOfNSSep, nChars);
		}
		else
		{
			addToTokenQueue(substring(pat, startSubstring, nChars));
		}
	}

	if(0 == m_expression->tokenQueueSize())
	{
		error("Empty expression!");
	}
	else if(0 != targetStrings)
	{
		recordTokenString(*targetStrings);
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
			m_expression->adjustPattern(m_expression->patternMapSize(),
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
XPathProcessorImpl::recordTokenString(DOMStringVectorType&	targetStrings)
{
	assert(m_expression != 0);

	int tokPos = getTokenQueuePosFromMap(m_expression->patternMapSize() - 1);

	resetTokenMark(tokPos + 1);

	if(lookahead(XalanUnicode::charLeftParenthesis, 1) == true)
	{
		const int	tok = getKeywordToken(m_token);

		switch(tok)
		{
		case XPathExpression::eNODETYPE_COMMENT:
			targetStrings.push_back(XPath::PSEUDONAME_COMMENT);
			break;

		case XPathExpression::eNODETYPE_TEXT:
			targetStrings.push_back(XPath::PSEUDONAME_TEXT);
			break;

		case XPathExpression::eNODETYPE_NODE:
			targetStrings.push_back(XPath::PSEUDONAME_ANY);
			break;

		case XPathExpression::eNODETYPE_ROOT:
			targetStrings.push_back(XPath::PSEUDONAME_ROOT);
			break;

		case XPathExpression::eNODETYPE_ANYELEMENT:
			targetStrings.push_back(XPath::PSEUDONAME_ANY);
			break;

		case XPathExpression::eNODETYPE_PI:
			targetStrings.push_back(XPath::PSEUDONAME_ANY);
			break;

		default:
			targetStrings.push_back(XPath::PSEUDONAME_ANY);
			break;
		}
	}
	else
	{
		if(tokenIs(XalanUnicode::charCommercialAt) == true)
		{
			tokPos++;

			resetTokenMark(tokPos + 1);
		}

		if(lookahead(XalanUnicode::charColon, 1) == true)
		{
			tokPos += 2;
		}

		assert(m_expression->getToken(tokPos) != 0);

		targetStrings.push_back(m_expression->getToken(tokPos)->str());
	}
}




void
XPathProcessorImpl::addToTokenQueue(const XalanDOMString&	s) const
{
	assert(m_xpath != 0);
	assert(m_expression != 0);

	m_expression->pushToken(s);
}



int
XPathProcessorImpl::mapNSTokens(
			const XalanDOMString&	pat,
			int 					startSubstring,
			int 					posOfNSSep,
			int 					posOfScan) const
{
	assert(m_prefixResolver != 0);

	const XalanDOMString 	prefix = substring(pat, startSubstring, posOfNSSep);

	const XalanDOMString 	uName =
				m_prefixResolver->getNamespaceForPrefix(prefix);

	if(length(uName) > 0)
	{
		addToTokenQueue(uName);

		addToTokenQueue(DOMServices::s_XMLNamespaceSeparatorString);

		const XalanDOMString 	s = substring(pat, posOfNSSep + 1, posOfScan);
	  
		if(length(s) > 0)
		{
			addToTokenQueue(s);
		}
	}
	else
	{
		// error(XalanDOMString("Could not locate namespace for prefix: ") + prefix);
		addToTokenQueue(prefix);

		addToTokenQueue(DOMServices::s_XMLNamespaceSeparatorString);

		const XalanDOMString 	s = substring(pat, posOfNSSep + 1, posOfScan);

		if(s.length() > 0)
		{
			addToTokenQueue(s);
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

	const unsigned int	theStringLength = strlen(s);

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
	const XalanDOMString 	tok =
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

	const XalanDOMString 	tok =
		getTokenRelative(n - 1);

	return equals(tok, s);
}



bool
XPathProcessorImpl::lookahead(
			const XalanDOMString&	s,
			int						n) const
{
	const XalanDOMString 	tok =
		getTokenRelative(n - 1);

	return equals(tok, s);
}



bool
XPathProcessorImpl::lookbehind(
			char	c,
			int 	n) const
{
	const XalanDOMString 	tok =
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
	const XalanDOMString 	tok =
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

	m_token = theNextToken == 0 ? XalanDOMString() : theNextToken->str();

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



XalanDOMString
XPathProcessorImpl::getTokenRelative(int	theOffset) const
{
	assert(m_expression != 0);

	const XObject* const	theToken =
		m_expression->getRelativeToken(theOffset);

	return theToken == 0 ? XalanDOMString() : theToken->str();
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
XPathProcessorImpl::warn(
			const XalanDOMString&	msg,
			XalanNode* 				sourceNode) const
{
	assert(m_envSupport != 0);

	const bool	shouldThrow =
		m_envSupport->problem(XPathEnvSupport::eXPATHParser, 
							  XPathEnvSupport::eWarning,
							  m_prefixResolver, 
							  sourceNode,
							  msg,
							  0,
							  0);

	if(shouldThrow == true)
	{
	  throw XPathParserException(msg);
	}
}
 


void
XPathProcessorImpl::warn(
			const char*		msg,
			XalanNode*		sourceNode) const
{
	warn(TranscodeFromLocalCodePage(msg), sourceNode);
}



void
XPathProcessorImpl::error(
			const XalanDOMString&	msg,
			XalanNode*				sourceNode) const
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

		if (length(theCurrentPattern) != 0)
		{
			thePrintWriter.print(XALAN_STATIC_UCODE_STRING("pattern = '"));
			thePrintWriter.println(theCurrentPattern);
		}

		thePrintWriter.print(msg);

		m_expression->dumpRemainingTokenQueue(thePrintWriter);
	}

	assert(m_envSupport != 0);

	const bool	shouldThrow =
		m_envSupport->problem(XPathEnvSupport::eXPATHParser, 
							  XPathEnvSupport::eError,
							  m_prefixResolver, 
							  sourceNode,
							  emsg,
							  0,
							  0);

	if(shouldThrow == true)
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



int
XPathProcessorImpl::getKeywordToken(const XalanDOMString&	key) const
{
	KeywordsMapType::const_iterator 	i =
		s_keywords.find(key);

	if (i == s_keywords.end())
	{
		return 0;
	}
	else
	{
		return (*i).second;
	}
}



int
XPathProcessorImpl::getFunctionToken(const XalanDOMString&	key) const
{
	FunctionNameMapType::const_iterator 	i = s_functions.find(key);

	if (i != s_functions.end())
	{
		return (*i).second;
	}
	else
	{
		return 0;
	}
}



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
		else if(tokenIs(s_quoString) == true)
		{
			theOpCode = XPathExpression::eOP_QUO;
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

	if(m_tokenChar == XalanUnicode::charHyphenMinus)
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
XPathProcessorImpl::StringExpr()
{
	const int	opPos = m_expression->opCodeMapLength();

	m_expression->appendOpCode(XPathExpression::eOP_STRING);

	Expr();

	// $$$ ToDo: Is this really necessary?
	m_expression->updateOpCodeLength(XPathExpression::eOP_STRING,
									 opPos);
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
XPathProcessorImpl::NumberExpr()
{
	const int	opPos = m_expression->opCodeMapLength();

	m_expression->appendOpCode(XPathExpression::eOP_NUMBER);

	Expr();

	m_expression->updateOpCodeLength(XPathExpression::eOP_NUMBER,
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

	/*
	if(tokenIs(XalanUnicode::charLeftSquareBracket) == true)
	{
		Predicate();

		m_expression->updateOpCodeLength(XPathExpression::eOP_LOCATIONPATH,
										 opPos);
	}  
	*/
}
  


void
XPathProcessorImpl::PrimaryExpr()
{
	assert(m_expression != 0);

	const int	opPos = m_expression->opCodeMapLength();

	if(m_tokenChar == XalanUnicode::charApostrophe || m_tokenChar == XalanUnicode::charQuoteMark)
	{
		m_expression->appendOpCode(XPathExpression::eOP_LITERAL);

		Literal();

		m_expression->updateOpCodeLength(XPathExpression::eOP_LITERAL,
										 opPos);
	}
	else if(m_tokenChar == XalanUnicode::charDollarSign)
	{
		nextToken(); // consume '$'

		m_expression->appendOpCode(XPathExpression::eOP_VARIABLE);

		QName();

		m_expression->updateOpCodeLength(XPathExpression::eOP_VARIABLE,
											 opPos);
	}
	else if(m_tokenChar == XalanUnicode::charLeftParenthesis)
	{
		nextToken();

		m_expression->appendOpCode(XPathExpression::eOP_GROUP);

		Expr();

		consumeExpected(XalanUnicode::charRightParenthesis);

		m_expression->updateOpCodeLength(XPathExpression::eOP_GROUP,
										 opPos);
	}
	else if((XalanUnicode::charFullStop == m_tokenChar &&
				m_token.length() > 1 &&
				isXMLDigit(charAt(m_token, 1)) == true)
			 || isXMLDigit(m_tokenChar) == true)
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

	Expr();

	m_expression->updateOpCodeLength(XPathExpression::eOP_ARGUMENT,
									 opPos);
}



int
XPathProcessorImpl::FunctionCallArguments()
{
	int		argCount = 0;

	consumeExpected(XalanUnicode::charLeftParenthesis);

	while(tokenIs(XalanUnicode::charRightParenthesis) == false)
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
			warn(TranscodeFromLocalCodePage("Could not find function: ") +
				 m_token +
				 TranscodeFromLocalCodePage("()"));
		}

		// $$$ ToDo: I believe that this is XSLT functionality.  We
		// need to figure out how to do the extensions for this stuff.
		const int funcTok = getFunctionToken(m_token);

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

				XPathExpression::OpCodeMapValueVectorType	theArgs(2, 0);
		
				theArgs[0] = theFunctionID;
				theArgs[1] = 0;

				m_expression->appendOpCode(XPathExpression::eOP_FUNCTION,
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

		XPathExpression::OpCodeMapValueVectorType	theArgs(1, 0);

		// Tell how long the step is without the predicate
		theArgs[0] = 4;

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

		XPathExpression::OpCodeMapValueVectorType	theArgs(1, 0);

		// Tell how long the step is without the predicate
		theArgs[0] = 4;

		m_expression->appendOpCode(XPathExpression::eFROM_SELF,
								   theArgs);

		m_expression->appendOpCode(XPathExpression::eNODETYPE_NODE);
	}
	else if(tokenIs(s_dotDotString) == true)
	{
		nextToken();

		XPathExpression::OpCodeMapValueVectorType	theArgs(1, 0);

		// Tell how long the step is without the predicate
		theArgs[0] = 4;

		m_expression->appendOpCode(XPathExpression::eFROM_PARENT,
								   theArgs);

		m_expression->appendOpCode(XPathExpression::eNODETYPE_NODE);
	}
	else
	{
		Basis();

		while(tokenIs(XalanUnicode::charLeftSquareBracket) == true)
		{
			Predicate();
		}
	}

	// Tell how long the entire step is.
	m_expression->updateOpCodeLength(opPos);
}



void
XPathProcessorImpl::Basis()
{
	assert(m_xpath != 0);
	assert(m_expression != 0);

	const int	opPos = m_expression->opCodeMapLength();

	int			axisType = 0;

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
		axisType = XPathExpression::eFROM_DESCENDANTS_OR_SELF;

		// Have to fix up for patterns such as '//@foo' or '//attribute::foo',
		// which translate to 'descendant-or-self::node()/attribute::foo'.
		// notice I leave the '/' on the queue, so the next will be processed 
		// by a regular step pattern.
		// if(lookahead(XalanUnicode::charCommercialAt, 1) == true || lookahead("::", 2) == true)
		{
			XPathExpression::OpCodeMapValueVectorType	theArgs(1, 0);

			// Tell how long the step is without the predicate
			theArgs[0] = 4;

			m_expression->appendOpCode(XPathExpression::eFROM_DESCENDANTS_OR_SELF,
									   theArgs);

			m_expression->appendOpCode(XPathExpression::eNODETYPE_NODE);

			// Tell how long the step is without the predicate
			m_expression->updateOpCodeLengthAfterNodeTest(opPos);

			return; // make a quick exit...
		}
		//else
		//{
		//	nextToken();
		//}
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



int
XPathProcessorImpl::AxisName()
{
	assert(m_xpath != 0);
	assert(m_expression != 0);

	AxisNamesMapType::const_iterator	i =
		s_axisNames.find(m_token);

	if (i == s_axisNames.end())
	{
		error(TranscodeFromLocalCodePage("illegal axis name: ") +
			  m_token);
	}
	else
	{
		m_expression->appendOpCode((*i).second);
	}

	return (*i).second;
}



void
XPathProcessorImpl::NodeTest(int	axisType)
{
	assert(m_xpath != 0);
	assert(m_expression != 0);

	if(lookahead(XalanUnicode::charLeftParenthesis, 1) == true)
	{
		NodeTypesMapType::const_iterator	i =
			s_nodeTypes.find(m_token);

		if (i == s_nodeTypes.end())
		{
			error(TranscodeFromLocalCodePage("Unknown nodetype: ") +
				  m_token);
		}
		else
		{
			nextToken();

			m_expression->appendOpCode((*i).second);

			consumeExpected(XalanUnicode::charLeftParenthesis);

			if(XPathExpression::eNODETYPE_PI == (*i).second)
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
				if (axisType == XPathExpression::eFROM_NAMESPACE)
				{
					const XObject* const	theToken =
						m_expression->getRelativeToken(-1);
					assert(theToken != 0);

					const XalanDOMString&	theString = theToken->str();

					const XalanDOMString&	theNamespace =
						m_prefixResolver->getNamespaceForPrefix(theString);

					m_expression->replaceRelativeToken(
									-1,
									theNamespace);
				}

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
		else
		{
			m_expression->pushCurrentTokenOnOpCodeMap();
		}

		nextToken();
	}
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

	Expr();

	// Terminate for safety.
	m_expression->appendOpCode(XPathExpression::eENDOP);

	m_expression->updateOpCodeLength(XPathExpression::eOP_PREDICATE,
									 opPos);
}



void
XPathProcessorImpl::QName()
{
	assert(m_xpath != 0);
	assert(m_expression != 0);

	// If there is no prefix, we have to fake things out...
	if (lookahead(':', 1) == false)
	{
		m_expression->insertToken(XalanDOMString());

		m_expression->pushCurrentTokenOnOpCodeMap();

		nextToken();
	}
	else
	{
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

	const int last = length(m_token) - 1;

	assert(last > 0);

	const XalanDOMChar 	c0 = m_tokenChar;
	const XalanDOMChar 	cX = charAt(m_token, last);

	if((c0 == XalanUnicode::charQuoteMark && cX == XalanUnicode::charQuoteMark) ||
	   (c0 == XalanUnicode::charApostrophe && cX == XalanUnicode::charApostrophe))
	{
		const XalanDOMString	theArgument = substring(m_token, 1, last);

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

	// These token s_functionKeyString should not be here, as it is really
	// part of the XSLT standard, and not the XPATH standard.
	if(lookahead(XalanUnicode::charLeftParenthesis, 1) == true &&
					tokenIs(s_functionIDString) == true ||
				  tokenIs(s_functionKeyString) == true)
	{
		IdKeyPattern();

		if(tokenIs(XalanUnicode::charSolidus) == true && lookahead(XalanUnicode::charSolidus, 1) == true)
		{
			const int	newOpPos = m_expression->opCodeMapLength();

			XPathExpression::OpCodeMapValueVectorType	theArgs(1, 0);

			// Tell how long the step is without the predicate
			theArgs[0] = 4;

			m_expression->appendOpCode(XPathExpression::eMATCH_ANY_ANCESTOR_WITH_PREDICATE,
									   theArgs);

			m_expression->appendOpCode(XPathExpression::eNODETYPE_ROOT);

			m_expression->updateOpCodeLength(newOpPos);

			nextToken();
		}
	}
	else if(tokenIs(XalanUnicode::charSolidus) == true)
	{
		const int	newOpPos = m_expression->opCodeMapLength();

		XPathExpression::OpCodeMapValueVectorType	theArgs(1, 0);

		// Tell how long the step is without the predicate
		theArgs[0] = 4;

		if(lookahead(XalanUnicode::charSolidus, 1) == true)
		{
			m_expression->appendOpCode(XPathExpression::eMATCH_ANY_ANCESTOR_WITH_PREDICATE,
									   theArgs);
		}
		else
		{
			m_expression->appendOpCode(XPathExpression::eFROM_ROOT,
									   theArgs);
		}

		m_expression->appendOpCode(XPathExpression::eNODETYPE_ROOT);

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
	FunctionCall();
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

	int			axisType = 0;

	int			matchTypePos = -1;

	// The next blocks guarantee that a MATCH_XXX will be added.
	if(tokenIs(XalanUnicode::charCommercialAt) == true)
	{
		axisType = XPathExpression::eMATCH_ATTRIBUTE;

		m_expression->appendOpCode(XPathExpression::eMATCH_ATTRIBUTE);

		nextToken();
	}
	else if(lookahead(s_axisString, 1) == true)
	{
		// $$$ To Do: Perhaps these strings should be in the
		// axis table?
		if(tokenIs(s_attributeString) == true)
		{
			axisType = XPathExpression::eMATCH_ATTRIBUTE;

			m_expression->appendOpCode(XPathExpression::eMATCH_ATTRIBUTE);
		}
		else if(tokenIs(s_childString) == true)
		{
			axisType = XPathExpression::eMATCH_IMMEDIATE_ANCESTOR;

			m_expression->appendOpCode(XPathExpression::eMATCH_IMMEDIATE_ANCESTOR);
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
		axisType = XPathExpression::eMATCH_ANY_ANCESTOR;

		m_expression->appendOpCode(XPathExpression::eMATCH_ANY_ANCESTOR);

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

		m_expression->appendOpCode(XPathExpression::eMATCH_IMMEDIATE_ANCESTOR);
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
XPathProcessorImpl::isValidFunction(const XalanDOMString&	key) const
{
	bool	fResult = true;

	if(XPath::isInstalledFunction(key) == false)
	{
		if (getFunctionToken(key) == 0)
		{
			fResult = false;
		}
	}

	return fResult;
}



void
XPathProcessorImpl::initializeKeywordsTable(KeywordsMapType&	/* theKeywords */)
{
	// $$$ ToDo: This is very confusing.  This table is only used
	// by getKeywordToken().  But if you look at the switch
	// statement there, none of these values are considered in the
	// case statement.  So what's the point?

	// theKeywords[FROM_SELF_ABBREVIATED_STRING] = XPathExpression::eFROM_SELF;
	// theKeywords[FROM_ATTRIBUTE_STRING] = XPathExpression::eFROM_ATTRIBUTE;
	// theKeywords[FROM_DOC_STRING] = XPathExpression::eFROM_DOC;
	// theKeywords[FROM_DOCREF_STRING] = XPathExpression::eFROM_DOCREF;
	// theKeywords[FROM_ID_STRING] = XPathExpression::eFROM_ID;
	// theKeywords[FROM_IDREF_STRING] = XPathExpression::eFROM_IDREF;
	// theKeywords[FUNC_ID_STRING] = XPathExpression::eFUNC_ID;
	// theKeywords[FUNC_KEY_STRING] = XPathExpression::eFUNC_KEY;
	// theKeywords[FUNC_DOCUMENT_STRING] = XPathExpression::eFUNC_DOC;
}



void
XPathProcessorImpl::initializeFunctionTable(FunctionNameMapType&	theFunctions)
{
	theFunctions[StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("processing-instruction"))] = XPathExpression::eNODETYPE_PI;
	theFunctions[StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("comment"))] = XPathExpression::eNODETYPE_COMMENT;
	theFunctions[StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("text"))] = XPathExpression::eNODETYPE_TEXT;
	theFunctions[StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("node"))] = XPathExpression::eNODETYPE_NODE;
}



void
XPathProcessorImpl::initializeAxisNamesTable(AxisNamesMapType&		theAxisNames)
{
	theAxisNames[StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("ancestor"))] = XPathExpression::eFROM_ANCESTORS;
	theAxisNames[StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("ancestor-or-self"))] = XPathExpression::eFROM_ANCESTORS_OR_SELF;
	theAxisNames[StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("attribute"))] = XPathExpression::eFROM_ATTRIBUTES;
	theAxisNames[StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("child"))] = XPathExpression::eFROM_CHILDREN;
	theAxisNames[StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("descendant"))] = XPathExpression::eFROM_DESCENDANTS;
	theAxisNames[StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("descendant-or-self"))] = XPathExpression::eFROM_DESCENDANTS_OR_SELF;
	theAxisNames[StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("following"))] = XPathExpression::eFROM_FOLLOWING;
	theAxisNames[StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("following-sibling"))] = XPathExpression::eFROM_FOLLOWING_SIBLINGS;
	theAxisNames[StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("parent"))] = XPathExpression::eFROM_PARENT;
	theAxisNames[StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("preceding"))] = XPathExpression::eFROM_PRECEDING;
	theAxisNames[StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("preceding-sibling"))] = XPathExpression::eFROM_PRECEDING_SIBLINGS;
	theAxisNames[StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("self"))] = XPathExpression::eFROM_SELF;
	theAxisNames[StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("namespace"))] = XPathExpression::eFROM_NAMESPACE;
}



void
XPathProcessorImpl::initializeNodeTypesTable(NodeTypesMapType&		theNodeTypes)
{
	theNodeTypes[StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("comment"))] = XPathExpression::eNODETYPE_COMMENT;
	theNodeTypes[StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("text"))] = XPathExpression::eNODETYPE_TEXT;
	theNodeTypes[StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("processing-instruction"))] = XPathExpression::eNODETYPE_PI;
	theNodeTypes[StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("node"))] = XPathExpression::eNODETYPE_NODE;
	theNodeTypes[StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("*"))] = XPathExpression::eNODETYPE_ANYELEMENT;
}



static XalanDOMString	s_functionIDString;

static XalanDOMString	s_functionKeyString;

static XalanDOMString	s_orString;

static XalanDOMString	s_andString;

static XalanDOMString	s_divString;

static XalanDOMString	s_modString;

static XalanDOMString	s_quoString;

static XalanDOMString	s_dotString;

static XalanDOMString	s_dotDotString;

static XalanDOMString	s_axisString;

static XalanDOMString	s_attributeString;

static XalanDOMString	s_childString;


const XalanDOMString&	XPathProcessorImpl::s_functionIDString = ::s_functionIDString;


	// This shouldn't really be here, since it's not part of the XPath standard,
	// but rather a part ofthe XSLT standard.
const XalanDOMString&	XPathProcessorImpl::s_functionKeyString = ::s_functionKeyString;

const XalanDOMString&	XPathProcessorImpl::s_orString = ::s_orString;

const XalanDOMString&	XPathProcessorImpl::s_andString = ::s_andString;

const XalanDOMString&	XPathProcessorImpl::s_divString = ::s_divString;

const XalanDOMString&	XPathProcessorImpl::s_modString = ::s_modString;

const XalanDOMString&	XPathProcessorImpl::s_quoString = ::s_quoString;

const XalanDOMString&	XPathProcessorImpl::s_dotString = ::s_dotString;

const XalanDOMString&	XPathProcessorImpl::s_dotDotString = ::s_dotDotString;

const XalanDOMString&	XPathProcessorImpl::s_axisString = ::s_axisString;

const XalanDOMString&	XPathProcessorImpl::s_attributeString = ::s_attributeString;

const XalanDOMString&	XPathProcessorImpl::s_childString = ::s_childString;


static XPathProcessorImpl::KeywordsMapType 		s_keywords;
static XPathProcessorImpl::FunctionNameMapType 	s_functions;
static XPathProcessorImpl::AxisNamesMapType		s_axisNames;
static XPathProcessorImpl::NodeTypesMapType		s_nodeTypes;



const XPathProcessorImpl::KeywordsMapType& 		XPathProcessorImpl::s_keywords = ::s_keywords;
const XPathProcessorImpl::FunctionNameMapType& 	XPathProcessorImpl::s_functions = ::s_functions;
const XPathProcessorImpl::AxisNamesMapType&		XPathProcessorImpl::s_axisNames = ::s_axisNames;
const XPathProcessorImpl::NodeTypesMapType&		XPathProcessorImpl::s_nodeTypes = ::s_nodeTypes;



void
XPathProcessorImpl::initialize()
{
	initializeKeywordsTable(::s_keywords);
	initializeFunctionTable(::s_functions);
	initializeAxisNamesTable(::s_axisNames);
	initializeNodeTypesTable(::s_nodeTypes);

	::s_functionIDString = XALAN_STATIC_UCODE_STRING("id");
	::s_functionKeyString = XALAN_STATIC_UCODE_STRING("key");
	::s_orString = XALAN_STATIC_UCODE_STRING("or");
	::s_andString = XALAN_STATIC_UCODE_STRING("and");
	::s_divString = XALAN_STATIC_UCODE_STRING("div");
	::s_modString = XALAN_STATIC_UCODE_STRING("mod");
	::s_quoString = XALAN_STATIC_UCODE_STRING("quo");
	::s_dotString = XALAN_STATIC_UCODE_STRING(".");
	::s_dotDotString = XALAN_STATIC_UCODE_STRING("..");
	::s_axisString = XALAN_STATIC_UCODE_STRING("::");
	::s_attributeString = XALAN_STATIC_UCODE_STRING("attribute");
	::s_childString = XALAN_STATIC_UCODE_STRING("child");
}



void
XPathProcessorImpl::terminate()
{
	KeywordsMapType().swap(::s_keywords);
	FunctionNameMapType().swap(::s_functions);
	AxisNamesMapType().swap(::s_axisNames);
	NodeTypesMapType().swap(::s_nodeTypes);

	clear(::s_functionIDString);
	clear(::s_functionKeyString);
	clear(::s_orString);
	clear(::s_andString);
	clear(::s_divString);
	clear(::s_modString);
	clear(::s_quoString);
	clear(::s_dotString);
	clear(::s_dotDotString);
	clear(::s_axisString);
	clear(::s_attributeString);
	clear(::s_childString);
}
