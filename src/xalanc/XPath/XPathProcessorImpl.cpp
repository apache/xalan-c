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

// Class header file...
#include "XPathProcessorImpl.hpp"



#include <xercesc/sax/Locator.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/DOMStringPrintWriter.hpp>
#include <xalanc/PlatformSupport/DoubleSupport.hpp>
#include <xalanc/PlatformSupport/PrefixResolver.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>
#include <xalanc/PlatformSupport/XalanUnicode.hpp>
#include <xalanc/PlatformSupport/XalanXMLChar.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "XalanQName.hpp"
#include "XPathEnvSupport.hpp"
#include "XPathConstructionContext.hpp"
#include "XPathExecutionContext.hpp"
#include "XPathParserException.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XPathProcessorImpl::XPathProcessorImpl() :
	m_token(),
	m_tokenChar(0),
	m_xpath(0),
	m_constructionContext(0),
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
			XPath&						pathObj,
			XPathConstructionContext&	constructionContext,
			const XalanDOMString&		expression,
			const PrefixResolver&		resolver,
			const LocatorType*			locator)
{
	m_requireLiterals = false;

	m_xpath = &pathObj;

	m_constructionContext = &constructionContext;

	m_expression = &m_xpath->getExpression();

	m_prefixResolver = &resolver;

	m_locator = locator;

	m_expression->reset();

	tokenize(expression);

	m_expression->appendOpCode(XPathExpression::eOP_XPATH);

	nextToken();

	Expr();

	if (length(m_token) != 0)
	{
		error(XalanMessageLoader::getMessage(XalanMessages::ExtraIllegalTokens));
	}

	m_xpath = 0;
	m_constructionContext = 0;
	m_expression = 0;
	m_prefixResolver = 0;
	m_locator = 0;
	m_positionPredicateStack.clear();
	m_namespaces.clear();
}



void
XPathProcessorImpl::initMatchPattern(
			XPath&						pathObj,
			XPathConstructionContext&	constructionContext,
			const XalanDOMString&		expression,
			const PrefixResolver&		resolver,
			const LocatorType*			locator)
{
	m_isMatchPattern = true;

	m_xpath = &pathObj;

	m_constructionContext = &constructionContext;

	m_expression = &m_xpath->getExpression();

	m_prefixResolver = &resolver;

	m_locator = locator;

	m_expression->reset();

	tokenize(expression);

	m_expression->appendOpCode(XPathExpression::eOP_MATCHPATTERN);

	nextToken();

	Pattern();

	if (length(m_token) != 0)
	{
		error(XalanMessageLoader::getMessage(XalanMessages::ExtraIllegalTokens));
	}

	// Terminate for safety.
	m_expression->appendOpCode(XPathExpression::eENDOP);

	m_expression->shrink();

	m_xpath = 0;
	m_constructionContext = 0;
	m_expression = 0;
	m_prefixResolver = 0;
	m_locator = 0;
	m_positionPredicateStack.clear();
	m_namespaces.clear();
}



void
XPathProcessorImpl::tokenize(const XalanDOMString&	pat)
{
	assert(m_xpath != 0);
	assert(m_expression != 0);
	assert(m_constructionContext != 0);

	m_expression->setCurrentPattern(m_constructionContext->getPooledString(pat));

	const int	nChars = length(pat);

	int 		startSubstring = -1;
	int 		posOfNSSep = -1;

	const XPathConstructionContext::GetAndReleaseCachedString	theGuard(*m_constructionContext);

	XalanDOMString&		theToken = theGuard.get();

	for(int i = 0; i < nChars; i++)
	{
		XalanDOMChar	c = charAt(pat, i);

		switch(c)
		{
		case XalanUnicode::charQuoteMark: 
			{
				if(startSubstring != -1)
				{
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

				if(c == XalanUnicode::charQuoteMark && i < nChars)
				{
					substring(pat, theToken, startSubstring, i + 1);

					addToTokenQueue(theToken);

					startSubstring = -1;
				}
				else
				{
					error(XalanMessageLoader::getMessage(XalanMessages::ExpectedDoubleQuote));
				}
			}
			break;

		case XalanUnicode::charApostrophe:
			{
				if(startSubstring != -1)
				{
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

				if(c == XalanUnicode::charApostrophe && i < nChars)
				{
					substring(pat, theToken, startSubstring, i + 1);

					addToTokenQueue(theToken);

					startSubstring = -1;
				}
				else
				{
					error(XalanMessageLoader::getMessage(XalanMessages::ExpectedSingleQuote));

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
		
		case XalanUnicode::charHyphenMinus:
			{
				if(!(startSubstring == -1))
				{
					break;
				}
			}
			// fall-through on purpose

		case XalanUnicode::charCommercialAt:
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

		error(XalanMessageLoader::getMessage(XalanMessages::EmptyExpression));

	}

	m_expression->resetTokenPosition();
}



void
XPathProcessorImpl::addToTokenQueue(const XalanDOMString&	s) const
{
	assert(m_xpath != 0);
	assert(m_expression != 0);

	m_expression->pushToken(m_constructionContext->getPooledString(s));
}



void
XPathProcessorImpl::replaceTokenWithNamespaceToken() const
{
	assert(m_xpath != 0);
	assert(m_expression != 0);
	assert(m_token.empty() == false);

	StringToStringMapType::const_iterator	i = m_namespaces.find(m_token);
	assert(i != m_namespaces.end());

	const XalanDOMString* const		theNamespaceURI = (*i).second;
	assert(theNamespaceURI != 0 && theNamespaceURI->empty() == false);

	m_expression->replaceRelativeToken(
			1,
            XPathExpression::eRelativeBackward,
			m_constructionContext->getPooledString(*theNamespaceURI));
}



int
XPathProcessorImpl::mapNSTokens(
			const XalanDOMString&	pat,
			int 					startSubstring,
			int 					posOfNSSep,
			int 					posOfScan)
{
	assert(m_prefixResolver != 0);

	const XPathConstructionContext::GetAndReleaseCachedString	theGuard(*m_constructionContext);

	XalanDOMString& 	scratchString = theGuard.get();

	// Get the prefix of the QName...
	scratchString.assign(pat, startSubstring, posOfNSSep - startSubstring);

	if (XalanQName::isValidNCName(scratchString) == false)
	{
	
		error(XalanMessageLoader::getMessage(XalanMessages::IsNotValidNCName_1Param,scratchString));

	}

	const XalanDOMString* const		uName =
				m_prefixResolver->getNamespaceForPrefix(scratchString);

	if(uName == 0)
	{
		error(XalanMessageLoader::getMessage(XalanMessages::PrefixMustResolveToNamespace_1Param, scratchString));
	}
	else if (length(*uName) == 0)
	{
		error(XalanMessageLoader::getMessage(XalanMessages::PrefixIsBoundToZeroLengthURI_1Param, scratchString));
	}
	else
	{
		m_namespaces[scratchString] = uName;

		addToTokenQueue(scratchString);

		addToTokenQueue(DOMServices::s_XMLNamespaceSeparatorString);

		// If there's no local part, then don't bother.  We need to check
		// this because '*' tokenizes separately, which means "ns:*" tokenizes
		// differently from "ns:foo".  In the first case, '*' will be tokenized
		// _after_ this code, in the second 'ns:foo' will be split into tokens
		// here...
		if(posOfNSSep + 1 < posOfScan)
		{
			scratchString.assign(pat, posOfNSSep + 1, posOfScan - (posOfNSSep + 1));

			assert(length(scratchString) > 0);

			if (XalanQName::isValidNCName(scratchString) == false)
			{
				error(XalanMessageLoader::getMessage(XalanMessages::NotValidNCName_1Param, scratchString));
			}
			else
			{
				addToTokenQueue(scratchString);
			}
		}
	}

	return -1;
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
XPathProcessorImpl::tokenIs(XalanDOMChar	c) const
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

	m_token = thePreviousToken == 0 ? s_emptyString : thePreviousToken->str();

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

    const XObject*  theToken = 0;

    if (theOffset < 0)
    {
        theToken = 
            m_expression->getRelativeToken(
                    XPathExpression::TokenQueueSizeType(-theOffset),
                    XPathExpression::eRelativeBackward);
    }
    else
    {
        theToken =
            m_expression->getRelativeToken(
                    XPathExpression::TokenQueueSizeType(theOffset),
                    XPathExpression::eRelativeForward);
    }

	return theToken == 0 ? s_emptyString : theToken->str();
}



void
XPathProcessorImpl::consumeExpected(XalanDOMChar	expected)
{
	if(tokenIs(expected) == true)
	{
		nextToken();
	}
	else
	{
		const XPathConstructionContext::GetAndReleaseCachedString	theGuard(*m_constructionContext);

		XalanDOMString	theMsg = theGuard.get();

		theMsg = XalanMessageLoader::getMessage(XalanMessages::NotFoundWhatExpected_2Param, XalanDOMString(&expected), m_token);
		error(theMsg);
	}
}



void
XPathProcessorImpl::error(const XalanDOMString&		msg) const
{
	const XPathConstructionContext::GetAndReleaseCachedString	theGuard(*m_constructionContext);

	XalanDOMString& 	emsg = theGuard.get();

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
				thePrintWriter.print(XalanMessageLoader::getMessage(XalanMessages::PatternIs_1Param,theCurrentPattern));
			}
			else
			{
				thePrintWriter.print(XalanMessageLoader::getMessage(XalanMessages::ExpressionIs_1Param,theCurrentPattern));
			}

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
XPathProcessorImpl::error(const char*	msg) const
{
	error(TranscodeFromLocalCodePage(msg));
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

	if (m_requireLiterals == false ||
		isCurrentLiteral() == true)
	{
		Expr();
	}
	else
	{
		error(XalanMessageLoader::getMessage(XalanMessages::LiteralArgumentIsRequired));
	}
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
			error(XalanMessageLoader::getMessage(XalanMessages::NoPrecedingArgument));
		}

		Argument();

		++argCount;

		if(tokenIs(XalanUnicode::charRightParenthesis) == false)
		{
			consumeExpected(XalanUnicode::charComma);

			if(tokenIs(XalanUnicode::charRightParenthesis) == true)
			{
				error(XalanMessageLoader::getMessage(XalanMessages::NoFollowingArgument));
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
			error(XalanMessageLoader::getMessage(XalanMessages::CouldNotFindFunction_1Param, m_token));

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

		case XPathExpression::eOP_FUNCTION_POSITION:
			FunctionPosition();
			break;

		case XPathExpression::eOP_FUNCTION_LAST:
			FunctionLast();
			break;

		case XPathExpression::eOP_FUNCTION_COUNT:
			FunctionCount();
			break;

		case XPathExpression::eOP_FUNCTION_NOT:
			FunctionNot();
			break;

		case XPathExpression::eOP_FUNCTION_TRUE:
			FunctionTrue();
			break;

		case XPathExpression::eOP_FUNCTION_FALSE:
			FunctionFalse();
			break;

		case XPathExpression::eOP_FUNCTION_BOOLEAN:
			FunctionBoolean();
			break;

		case XPathExpression::eOP_FUNCTION_NAME_0:
			FunctionName(opPos);
			break;

		case XPathExpression::eOP_FUNCTION_LOCALNAME_0:
			FunctionLocalName(opPos);
			break;

		case XPathExpression::eOP_FUNCTION_NUMBER_0:
			FunctionNumber(opPos);
			break;

		case XPathExpression::eOP_FUNCTION_FLOOR:
			FunctionFloor();
			break;

		case XPathExpression::eOP_FUNCTION_CEILING:
			FunctionCeiling();
			break;

		case XPathExpression::eOP_FUNCTION_ROUND:
			FunctionRound();
			break;

		case XPathExpression::eOP_FUNCTION_STRINGLENGTH_0:
			FunctionStringLength(opPos);
			break;

		case XPathExpression::eOP_FUNCTION_SUM:
			FunctionSum();
			break;

		default:
			{
				// The position must be at least 1, since
				// we've looked at a token.
				assert(m_expression->getTokenPosition() > 0);

				int		theFunctionID =
					XPath::getFunctionTable().nameToID(m_token);

				if ((equals(m_token, s_positionString) == true ||
					 equals(m_token, s_lastString) == true) &&
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

				nextToken();

				// Get the arguments, and the argument count...
				const int	argCount = FunctionCallArguments();

				assert(m_expression->getOpCodeMapValue(opPos + 3) == 0);

				// update the arg count in the op map...
				m_expression->setOpCodeMapValue(opPos + 3, argCount);
			}
		}
	}

	// Terminate for safety.
	m_expression->appendOpCode(XPathExpression::eENDOP);

	m_expression->updateOpCodeLength(opPos);
}



void
XPathProcessorImpl::FunctionPosition()
{
	m_expression->appendOpCode(XPathExpression::eOP_FUNCTION_POSITION);

	// Consume the name...
	nextToken();

	// Get the arguments, and the argument count...
	const int	argCount = FunctionCallArguments();

	if (argCount != 0)
	{
		error(XalanMessageLoader::getMessage(XalanMessages::FunctionDoesNotAcceptAnyArguments_1Param,"position()"));
	}
	else
	{
		if (m_positionPredicateStack.empty() == false)
		{
			m_positionPredicateStack.back() = true;
		}
	}
}



void
XPathProcessorImpl::FunctionLast()
{
	m_expression->appendOpCode(XPathExpression::eOP_FUNCTION_LAST);

	// Consume the name...
	nextToken();

	// Get the arguments, and the argument count...
	const int	argCount = FunctionCallArguments();

	if (argCount != 0)
	{
		error(XalanMessageLoader::getMessage(XalanMessages::FunctionDoesNotAcceptAnyArguments_1Param, XalanDOMString("last()")));
	}
	else
	{
		if (m_positionPredicateStack.empty() == false)
		{
			m_positionPredicateStack.back() = true;
		}
	}
}



void
XPathProcessorImpl::FunctionCount()
{
	m_expression->appendOpCode(XPathExpression::eOP_FUNCTION_COUNT);

	// Consume the name...
	nextToken();

	// Get the arguments, and the argument count...
	const int	argCount = FunctionCallArguments();

	if (argCount != 1)
	{
		error(XalanMessageLoader::getMessage(XalanMessages::FunctionAcceptsOneArgument_1Param,"count()")); 
	}
}



void
XPathProcessorImpl::FunctionNot()
{
	m_expression->appendOpCode(XPathExpression::eOP_FUNCTION_NOT);

	// Consume the name...
	nextToken();

	// Get the arguments, and the argument count...
	const int	argCount = FunctionCallArguments();

	if (argCount != 1)
	{

		error(XalanMessageLoader::getMessage(XalanMessages::FunctionAcceptsOneArgument_1Param,"not()")); 

	}
}



void
XPathProcessorImpl::FunctionTrue()
{
	m_expression->appendOpCode(XPathExpression::eOP_FUNCTION_TRUE);

	// Consume the name...
	nextToken();

	// Get the arguments, and the argument count...
	const int	argCount = FunctionCallArguments();

	if (argCount != 0)
	{
		error(XalanMessageLoader::getMessage(XalanMessages::FunctionDoesNotAcceptAnyArguments_1Param,"true()"));
	}
}



void
XPathProcessorImpl::FunctionFalse()
{
	m_expression->appendOpCode(XPathExpression::eOP_FUNCTION_FALSE);

	// Consume the name...
	nextToken();

	// Get the arguments, and the argument count...
	const int	argCount = FunctionCallArguments();

	if (argCount != 0)
	{

		error(XalanMessageLoader::getMessage(XalanMessages::FunctionDoesNotAcceptAnyArguments_1Param,"false()")); 

	}
}



void
XPathProcessorImpl::FunctionBoolean()
{
	m_expression->appendOpCode(XPathExpression::eOP_FUNCTION_BOOLEAN);

	// Consume the name...
	nextToken();

	// Get the arguments, and the argument count...
	const int	argCount = FunctionCallArguments();

	if (argCount != 1)
	{
		error(XalanMessageLoader::getMessage(XalanMessages::FunctionAcceptsOneArgument_1Param,"boolean()")); 
	}
}



void
XPathProcessorImpl::FunctionName(int	opPos)
{
	m_expression->appendOpCode(XPathExpression::eOP_FUNCTION_NAME_0);

	// Consume the name...
	nextToken();

	// Get the arguments, and the argument count...
	const int	argCount = FunctionCallArguments();

	if (argCount != 0)
	{
		if (argCount == 1)
		{
			m_expression->replaceOpCode(
				opPos,
				XPathExpression::eOP_FUNCTION_NAME_0,
				XPathExpression::eOP_FUNCTION_NAME_1);
		}
		else
		{
			error(XalanMessageLoader::getMessage(XalanMessages::FunctionTakesZeroOrOneArg_1Param,"name()")); 
		}
	}
}



void
XPathProcessorImpl::FunctionLocalName(int	opPos)
{
	m_expression->appendOpCode(XPathExpression::eOP_FUNCTION_LOCALNAME_0);

	// Consume the name...
	nextToken();

	// Get the arguments, and the argument count...
	const int	argCount = FunctionCallArguments();

	if (argCount != 0)
	{
		if (argCount == 1)
		{
			m_expression->replaceOpCode(
				opPos,
				XPathExpression::eOP_FUNCTION_LOCALNAME_0,
				XPathExpression::eOP_FUNCTION_LOCALNAME_1);
		}
		else
		{
			error(XalanMessageLoader::getMessage(XalanMessages::FunctionTakesZeroOrOneArg_1Param,"locale-name()")); 
		}
	}
}



void
XPathProcessorImpl::FunctionNumber(int	opPos)
{
	m_expression->appendOpCode(XPathExpression::eOP_FUNCTION_NUMBER_0);

	// Consume the name...
	nextToken();

	// Get the arguments, and the argument count...
	const int	argCount = FunctionCallArguments();

	if (argCount != 0)
	{
		if (argCount == 1)
		{
			m_expression->replaceOpCode(
				opPos,
				XPathExpression::eOP_FUNCTION_NUMBER_0,
				XPathExpression::eOP_FUNCTION_NUMBER_1);
		}
		else
		{
			error(XalanMessageLoader::getMessage(XalanMessages::FunctionTakesZeroOrOneArg_1Param,"number()")); 
		}
	}
}



void
XPathProcessorImpl::FunctionFloor()
{
	m_expression->appendOpCode(XPathExpression::eOP_FUNCTION_FLOOR);

	// Consume the name...
	nextToken();

	// Get the arguments, and the argument count...
	const int	argCount = FunctionCallArguments();

	if (argCount != 1)
	{

		error(XalanMessageLoader::getMessage(XalanMessages::FunctionAcceptsOneArgument_1Param,"floor()")); 

	}
}



void
XPathProcessorImpl::FunctionCeiling()
{
	m_expression->appendOpCode(XPathExpression::eOP_FUNCTION_CEILING);

	// Consume the name...
	nextToken();

	// Get the arguments, and the argument count...
	const int	argCount = FunctionCallArguments();

	if (argCount != 1)
	{
		error(XalanMessageLoader::getMessage(XalanMessages::FunctionAcceptsOneArgument_1Param,"ceiling()")); 
	}
}



void
XPathProcessorImpl::FunctionRound()
{
	m_expression->appendOpCode(XPathExpression::eOP_FUNCTION_ROUND);

	// Consume the name...
	nextToken();

	// Get the arguments, and the argument count...
	const int	argCount = FunctionCallArguments();

	if (argCount != 1)
	{
		error(XalanMessageLoader::getMessage(XalanMessages::FunctionAcceptsOneArgument_1Param,"round()")); 
	}
}



void
XPathProcessorImpl::FunctionString(int	opPos)
{
	m_expression->appendOpCode(XPathExpression::eOP_FUNCTION_STRING_0);

	// Consume the name...
	nextToken();

	// Get the arguments, and the argument count...
	const int	argCount = FunctionCallArguments();

	if (argCount != 0)
	{
		if (argCount == 1)
		{
			m_expression->replaceOpCode(
				opPos,
				XPathExpression::eOP_FUNCTION_STRING_0,
				XPathExpression::eOP_FUNCTION_STRING_1);
		}
		else
		{
			error(XalanMessageLoader::getMessage(XalanMessages::FunctionTakesZeroOrOneArg_1Param,"string()")); 
		}
	}
}



void
XPathProcessorImpl::FunctionSum()
{
	m_expression->appendOpCode(XPathExpression::eOP_FUNCTION_SUM);

	// Consume the name...
	nextToken();

	// Get the arguments, and the argument count...
	const int	argCount = FunctionCallArguments();

	if (argCount != 1)
	{

		error(XalanMessageLoader::getMessage(XalanMessages::FunctionAcceptsOneArgument_1Param,"sum()")); 

	}
}



void
XPathProcessorImpl::FunctionStringLength(int	opPos)
{
	m_expression->appendOpCode(XPathExpression::eOP_FUNCTION_STRINGLENGTH_0);

	// Consume the name...
	nextToken();

	// Get the arguments, and the argument count...
	const int	argCount = FunctionCallArguments();

	if (argCount != 0)
	{
		if (argCount == 1)
		{
			m_expression->replaceOpCode(
				opPos,
				XPathExpression::eOP_FUNCTION_STRINGLENGTH_0,
				XPathExpression::eOP_FUNCTION_STRINGLENGTH_1);
		}
		else
		{
			error(XalanMessageLoader::getMessage(XalanMessages::FunctionTakesZeroOrOneArg_1Param,"string-length()")); 
		}
	}
}



void
XPathProcessorImpl::FunctionNamespaceURI(int	opPos)
{
	m_expression->appendOpCode(XPathExpression::eOP_FUNCTION_NAMESPACEURI_0);

	// Consume the name...
	nextToken();

	// Get the arguments, and the argument count...
	const int	argCount = FunctionCallArguments();

	if (argCount != 0)
	{
		if (argCount == 1)
		{
			m_expression->replaceOpCode(
				opPos,
				XPathExpression::eOP_FUNCTION_NAMESPACEURI_0,
				XPathExpression::eOP_FUNCTION_NAMESPACEURI_1);
		}
		else
		{
			error(XalanMessageLoader::getMessage(XalanMessages::FunctionTakesZeroOrOneArg_1Param,"namespace-uri()")); 
		}
	}
}



void
XPathProcessorImpl::LocationPath()
{
	const int	opPos = m_expression->opCodeMapLength();

	m_expression->appendOpCode(XPathExpression::eOP_LOCATIONPATH);

	if(tokenIs(XalanUnicode::charSolidus) == true)
	{
		nextToken();

		const int	newOpPos = m_expression->opCodeMapLength();

		// Tell how long the step is without the predicate
		const XPathExpression::OpCodeMapValueVectorType		theArgs(1, 4);

		m_expression->appendOpCode(
			XPathExpression::eFROM_ROOT,
			theArgs);

		m_expression->appendOpCode(XPathExpression::eNODETYPE_ROOT);

		// Tell how long the entire step is.
		m_expression->updateOpCodeLength(newOpPos);
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
			error(XalanMessageLoader::getMessage(XalanMessages::IllegalSyntaxOfPredicatesSelf));
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

		if(tokenIs(XalanUnicode::charLeftSquareBracket) == true)
		{
			error(XalanMessageLoader::getMessage(XalanMessages::IllegalSyntaxOfPredicatesParent));
		}

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
		error(XalanMessageLoader::getMessage(XalanMessages::UnexpectedTokenFound_1Param, m_token)); 
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

			error(XalanMessageLoader::getMessage(XalanMessages::ExpectedAxis)); 

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

	NodeTest();

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
		error(XalanMessageLoader::getMessage(XalanMessages::IllegalAxisName_1Param,m_token)); 
	}
	else
	{
		m_expression->appendOpCode(theOpCode);
	}

	return theOpCode;
}



int
XPathProcessorImpl::NodeTest()
{
	assert(m_xpath != 0);
	assert(m_expression != 0);

	int		nodeTestPos = -1;

	if (lookahead(XalanUnicode::charLeftParenthesis, 1) == true)
	{
		const XPathExpression::eOpCodes		theOpCode = getNodeTypeToken(m_token);

		if (theOpCode == XPathExpression::eENDOP)
		{
			error(XalanMessageLoader::getMessage(XalanMessages::UnknownNodeType_1Param, m_token)); 

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

		if (tokenIs(XalanUnicode::charAsterisk) == true)
		{
			m_expression->appendOpCode(XPathExpression::eELEMWILDCARD);
		}
		else if (isNodeTest(m_token) == false)
		{
			error(XalanMessageLoader::getMessage(XalanMessages::ExpectedNodeTest)); 

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
		m_expression->insertToken(s_emptyString);

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
		m_expression->pushArgumentOnOpCodeMap(
			m_constructionContext->getPooledString(m_token.c_str() + 1, m_token.length() - 2));

		nextToken();
	}
	else
	{
		error(XalanMessageLoader::getMessage(XalanMessages::LiteralArgumentIsRequired)); 

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

 		const XPathConstructionContext::GetAndReleaseCachedString	theGuard(*m_constructionContext);
 
 		XalanDOMString& 	theStringValue = theGuard.get();
 
		DoubleToDOMString(num, theStringValue);

		m_expression->pushNumberLiteralOnOpCodeMap(num);

		m_expression->pushArgumentOnOpCodeMap(num, m_constructionContext->getPooledString(theStringValue));

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

	if(length(m_token) != 0)
	{
		if (!tokenIs(XalanUnicode::charVerticalLine) == true)
		{
			RelativePathPattern();
		}
		else if (lookahead(XalanUnicode::charVerticalLine, -1) == true)
		{
			error(XalanMessageLoader::getMessage(XalanMessages::UnexpectedTokenFound_1Param, m_token));
	
		}
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
			error(XalanMessageLoader::getMessage(XalanMessages::OnlyChildAndAttributeAxesAreAllowed)); 
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
				error(XalanMessageLoader::getMessage(XalanMessages::OnlyChildAndAttributeAxesAreAllowed)); 

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

	NodeTest();

	m_expression->updateOpCodeLengthAfterNodeTest(opPos);

	while(tokenIs(XalanUnicode::charLeftSquareBracket) == true)
	{
		Predicate();
	}

	if(matchTypePos > -1 && tokenIs(XalanUnicode::charSolidus) == true && lookahead(XalanUnicode::charSolidus, 1) == true)
	{
		assert(m_expression->opCodeMapLength() > matchTypePos);

		m_expression->setOpCodeMapValue(matchTypePos, XPathExpression::eMATCH_ANY_ANCESTOR);
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
	else if (equals(theToken, s_dotString) == true ||
			 equals(theToken, s_dotDotString) == true)
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

const XalanDOMChar	XPathProcessorImpl::s_lastString[] =
{
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_t,
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
	{ XPathFunctionTable::s_not, XPathExpression::eOP_FUNCTION_NOT },
	{ XPathFunctionTable::s_sum, XPathExpression::eOP_FUNCTION_SUM },
	{ XPathProcessorImpl::s_lastString, XPathExpression::eOP_FUNCTION_LAST },
	{ XPathFunctionTable::s_name, XPathExpression::eOP_FUNCTION_NAME_0 },
	{ XPathProcessorImpl::s_nodeString, XPathExpression::eNODETYPE_NODE },
	{ XPathProcessorImpl::s_textString, XPathExpression::eNODETYPE_TEXT },
	{ XPathFunctionTable::s_true, XPathExpression::eOP_FUNCTION_TRUE },
	{ XPathFunctionTable::s_count, XPathExpression::eOP_FUNCTION_COUNT  },
	{ XPathFunctionTable::s_false, XPathExpression::eOP_FUNCTION_FALSE },
	{ XPathFunctionTable::s_floor, XPathExpression::eOP_FUNCTION_FLOOR },
	{ XPathFunctionTable::s_round, XPathExpression::eOP_FUNCTION_ROUND },
	{ XPathFunctionTable::s_number, XPathExpression::eOP_FUNCTION_NUMBER_0 },
	{ XPathFunctionTable::s_boolean, XPathExpression::eOP_FUNCTION_BOOLEAN },
	{ XPathFunctionTable::s_ceiling, XPathExpression::eOP_FUNCTION_CEILING },
	{ XPathProcessorImpl::s_commentString, XPathExpression::eNODETYPE_COMMENT },
	{ XPathProcessorImpl::s_positionString, XPathExpression::eOP_FUNCTION_POSITION },
	{ XPathFunctionTable::s_localName, XPathExpression::eOP_FUNCTION_LOCALNAME_0 },
	{ XPathFunctionTable::s_stringLength, XPathExpression::eOP_FUNCTION_STRINGLENGTH_0 },
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



XALAN_CPP_NAMESPACE_END
