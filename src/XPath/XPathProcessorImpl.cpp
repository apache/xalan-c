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
// Class header file...
#include "XPathProcessorImpl.hpp"



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/DOMStringPrintWriter.hpp>



#include "PrefixResolver.hpp"
#include "XObjectFactory.hpp"
#include "XPathEnvSupport.hpp"
#include "XPathExecutionContext.hpp"
#include "XPathParserException.hpp"
#include "XPathSupport.hpp"



static XalanDOMString	FROM_ANCESTORS_STRING(XALAN_STATIC_UCODE_STRING("ancestor"));
static XalanDOMString	FROM_ANCESTORS_OR_SELF_STRING(XALAN_STATIC_UCODE_STRING("ancestor-or-self"));
static XalanDOMString	FROM_ATTRIBUTES_STRING(XALAN_STATIC_UCODE_STRING("attribute"));
static XalanDOMString	FROM_CHILDREN_STRING(XALAN_STATIC_UCODE_STRING("child"));
static XalanDOMString	FROM_DESCENDANTS_STRING(XALAN_STATIC_UCODE_STRING("descendant"));
static XalanDOMString	FROM_DESCENDANTS_OR_SELF_STRING(XALAN_STATIC_UCODE_STRING("descendant-or-self"));
static XalanDOMString	FROM_FOLLOWING_STRING(XALAN_STATIC_UCODE_STRING("following"));
static XalanDOMString	FROM_FOLLOWING_SIBLINGS_STRING(XALAN_STATIC_UCODE_STRING("following-sibling"));
static XalanDOMString	FROM_PARENT_STRING(XALAN_STATIC_UCODE_STRING("parent"));
static XalanDOMString	FROM_PRECEDING_STRING(XALAN_STATIC_UCODE_STRING("preceding"));
static XalanDOMString	FROM_PRECEDING_SIBLINGS_STRING(XALAN_STATIC_UCODE_STRING("preceding-sibling"));
static XalanDOMString	FROM_SELF_STRING(XALAN_STATIC_UCODE_STRING("self"));
static XalanDOMString	FROM_SELF_ABBREVIATED_STRING(XALAN_STATIC_UCODE_STRING("."));
static XalanDOMString	FROM_NAMESPACE_STRING(XALAN_STATIC_UCODE_STRING("namespace"));

// This shouldn't really be here, since it duplicates a string that is part
// of the information that is maintained by the class XPathFunctionTable,
// but this is a reasonable optimization.
static XalanDOMString	FUNC_ID_STRING(XALAN_STATIC_UCODE_STRING("id"));


// These shouldn't really be here, since they are not part of the XPath standard,
// but rather a part ofthe XSLT standard.
static XalanDOMString	FUNC_KEY_STRING(XALAN_STATIC_UCODE_STRING("key"));
static XalanDOMString	FUNC_DOC_STRING(XALAN_STATIC_UCODE_STRING("doc"));
static XalanDOMString	FUNC_DOCUMENT_STRING(XALAN_STATIC_UCODE_STRING("document"));

static XalanDOMString	NODETYPE_COMMENT_STRING(XALAN_STATIC_UCODE_STRING("comment"));
static XalanDOMString	NODETYPE_TEXT_STRING(XALAN_STATIC_UCODE_STRING("text"));
static XalanDOMString	NODETYPE_PI_STRING(XALAN_STATIC_UCODE_STRING("processing-instruction"));
static XalanDOMString	NODETYPE_NODE_STRING(XALAN_STATIC_UCODE_STRING("node"));
static XalanDOMString	NODETYPE_ANYELEMENT_STRING(XALAN_STATIC_UCODE_STRING("*"));



XPathProcessorImpl::KeywordsMapType 			XPathProcessorImpl::s_keywords;
XPathProcessorImpl::FunctionNameMapType 		XPathProcessorImpl::s_functions;
XPathProcessorImpl::AxisNamesMapType			XPathProcessorImpl::s_axisNames;
XPathProcessorImpl::NodeTypesMapType			XPathProcessorImpl::s_nodeTypes;



XPathProcessorImpl::XPathProcessorImpl() :
	m_token(),
	m_tokenChar(0),
	m_xpath(0),
	m_expression(0),
	m_prefixResolver(0),
	m_xobjectFactory(0),
	m_envSupport(0)
{
	// $$$ ToDo: This is not thread-safe!!!
	static StaticInitializer	theInitializer;
}



XPathProcessorImpl::~XPathProcessorImpl()
{
}



void
XPathProcessorImpl::initXPath(
			XPath&						pathObj,
			const XalanDOMString&		expression,
			const PrefixResolver&		prefixResolver,
			XObjectFactory&				xobjectFactory,
			const XPathEnvSupport&		envSupport)
{
	m_xpath = &pathObj;
	m_expression = &m_xpath->getExpression();
	m_prefixResolver = &prefixResolver;
	m_xobjectFactory = &xobjectFactory;
	m_envSupport = &envSupport;

	tokenize(expression);

	m_expression->appendOpCode(XPathExpression::eOP_XPATH);

	nextToken();

	Expr();

	m_xpath = 0;
	m_expression = 0;
	m_prefixResolver = 0;
	m_xobjectFactory = 0;
	m_envSupport = 0;
}



void
XPathProcessorImpl::initMatchPattern(
			XPath&					pathObj,
			const XalanDOMString&	expression,
			const PrefixResolver&	prefixResolver,
			XObjectFactory&			xobjectFactory,
			const XPathEnvSupport&	envSupport)
{
	m_xpath = &pathObj;
	m_expression = &m_xpath->getExpression();
	m_prefixResolver = &prefixResolver;
	m_xobjectFactory = &xobjectFactory;
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
	m_xobjectFactory = 0;
	m_envSupport = 0;
}



void
XPathProcessorImpl::tokenize(
			const XalanDOMString&			pat,
			std::vector<XalanDOMString>* 	targetStrings)
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
		case '\"': 
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

				for(i++; i < nChars && (c = charAt(pat, i)) != '\"'; i++);

				if(c == '\"')
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

		case '\'':
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

				for(i++; i < nChars && (c = charAt(pat, i)) != '\''; i++);

				if(c == '\'')
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
		
		case 0x0A:
		case 0x0D:
		case ' ':
		case '\t':
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
		
		case '@':
			isAttrName = true;
			// fall-through on purpose

		case '-':
			{
				if('-' == c)
				{
					if(!(isNum || (startSubstring == -1)))
					{
						break;
					}

					isNum = false;
				}
			}
			// fall-through on purpose

		case '(':
		case '[':
		case ')':
		case ']':
		case '|':
		case '/':
		case '*':
		case '+':
		case '=':
		case ',':
		case '\\': // Unused at the moment
		case '^': // Unused at the moment
		case '!': // Unused at the moment
		case '$':
		case '<':
		case '>':
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
				else if('/' == c && isStartOfPat == true)
				{
					isStartOfPat = mapPatternElemPos(nesting, isStartOfPat, isAttrName);
				}
				else if('*' == c)
				{
					isStartOfPat = mapPatternElemPos(nesting, isStartOfPat, isAttrName);
					isAttrName = false;
				}

				if(0 == nesting)
				{
					if('|' == c)
					{
						if(0 != targetStrings)
						{
							recordTokenString(*targetStrings);
						}

						isStartOfPat = true;
					}
				}

				if(')' == c || ']' == c)
				{
					nesting--;
				}
				else if('(' == c || '[' == c)
				{
					nesting++;
				}

				addToTokenQueue(substring(pat, i, i + 1));
			}		
			break;

		case ':':
			{
				if(posOfNSSep == (i - 1))
				{ 
					if(startSubstring != -1)
					{
						addToTokenQueue(substring(pat, startSubstring, i - 1));
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

					isNum = isDigit(c);
				}
				else if(isNum == true)
				{
					isNum = isDigit(c);
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
XPathProcessorImpl::recordTokenString(std::vector<XalanDOMString>&	targetStrings)
{
	assert(m_expression != 0);

	int tokPos = getTokenQueuePosFromMap(m_expression->patternMapSize() - 1);

	resetTokenMark(tokPos + 1);

	if(lookahead('(', 1) == true)
	{
		const int	tok = getKeywordToken(m_token);

		switch(tok)
		{
		case XPathExpression::eNODETYPE_COMMENT:
			targetStrings.push_back(m_xpath->PSEUDONAME_COMMENT);
			break;

		case XPathExpression::eNODETYPE_TEXT:
			targetStrings.push_back(m_xpath->PSEUDONAME_TEXT);
			break;

		case XPathExpression::eNODETYPE_NODE:
			targetStrings.push_back(m_xpath->PSEUDONAME_ANY);
			break;

		case XPathExpression::eNODETYPE_ROOT:
			targetStrings.push_back(m_xpath->PSEUDONAME_ROOT);
			break;

		case XPathExpression::eNODETYPE_ANYELEMENT:
			targetStrings.push_back(m_xpath->PSEUDONAME_ANY);
			break;

		case XPathExpression::eNODETYPE_PI:
			targetStrings.push_back(m_xpath->PSEUDONAME_ANY);
			break;

		default:
			targetStrings.push_back(m_xpath->PSEUDONAME_ANY);
			break;
		}
	}
	else
	{
		if(tokenIs('@') == true)
		{
			tokPos++;

			resetTokenMark(tokPos + 1);
		}

		if(lookahead(':', 1) == true)
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
	assert(m_xobjectFactory != 0);

	m_expression->pushToken(m_xobjectFactory->createString(s));
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

		addToTokenQueue(XALAN_STATIC_UCODE_STRING(":"));

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

		addToTokenQueue(XALAN_STATIC_UCODE_STRING(":"));

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
XPathProcessorImpl::tokenIs(char	c) const
{
	return m_tokenChar == c ? true : false;
}



bool
XPathProcessorImpl::lookahead(
			char	c,
			int 	n) const
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

	const XalanDOMChar 		c0 = length(tok) == 0 ? '|' : charAt(tok, 0);

	return c0 == '|' ? false : true;
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
		error(XalanDOMString("Expected ") +
			  XalanDOMString(expected) +
			  XalanDOMString(", but found: ") +
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
		XalanDOMString	theMsg("Expected ");

		theMsg += expected;
		theMsg += ", but found: ";
		theMsg += m_token;

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

		DOMStringPrintWriter	thePrintWriter;

		if (length(theCurrentPattern) != 0)
		{
			thePrintWriter.print(XALAN_STATIC_UCODE_STRING("pattern = '"));
			thePrintWriter.println(theCurrentPattern);
		}

		thePrintWriter.print(msg);

		m_expression->dumpRemainingTokenQueue(thePrintWriter);

		emsg = thePrintWriter.getString();
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

	if(tokenIs(XALAN_STATIC_UCODE_STRING("or")) == true)
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

	if(tokenIs(XALAN_STATIC_UCODE_STRING("and")) == true)
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

	if(tokenIs('!') && lookahead('=', 1))
	{
		nextToken();
		nextToken();

		theOpCode = XPathExpression::eOP_NOTEQUALS;
	}
	else if(tokenIs('='))
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

		if(tokenIs('<') == true)
		{
			nextToken();

			if(tokenIs('=') == true)
			{
				nextToken();

				theOpCode = XPathExpression::eOP_LTE;
			}
			else
			{
				theOpCode = XPathExpression::eOP_LT;
			}
		}
		else if(tokenIs('>') == true)
		{
			nextToken();

			if(tokenIs('=') == true)
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

		if(tokenIs('+') == true)
		{
			theOpCode = XPathExpression::eOP_PLUS;
		}
		else if(tokenIs('-') == true)
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

		if(tokenIs('*') == true)
		{
			theOpCode = XPathExpression::eOP_MULT;
		}
		else if(tokenIs(XALAN_STATIC_UCODE_STRING("div")) == true)
		{
			theOpCode = XPathExpression::eOP_DIV;
		}
		else if(tokenIs(XALAN_STATIC_UCODE_STRING("mod")) == true)
		{
			theOpCode = XPathExpression::eOP_MOD;
		}
		else if(tokenIs(XALAN_STATIC_UCODE_STRING("quo")) == true)
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

	if(m_tokenChar == '-')
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

		if(tokenIs('|') == true)
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
			break;
	  }

	  // this.m_testForDocOrder = true;
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

	if(tokenIs('/') == true)
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

	//	const bool	isFunc = lookahead('(', 1);

	PrimaryExpr();

	if(tokenIs('[') == true)
	{
		m_expression->insertOpCode(XPathExpression::eOP_LOCATIONPATH,
								   opPos);
	  
		while(tokenIs('[') == true)
		{
			Predicate();
		}

		if(tokenIs('/') == true)
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
	if(tokenIs('[') == true)
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

	if(m_tokenChar == '\'' || m_tokenChar == '"')
	{
		m_expression->appendOpCode(XPathExpression::eOP_LITERAL);

		Literal();

		m_expression->updateOpCodeLength(XPathExpression::eOP_LITERAL,
										 opPos);
	}
	else if(m_tokenChar == '$')
	{
		nextToken(); // consume '$'

		m_expression->appendOpCode(XPathExpression::eOP_VARIABLE);

		NCName();

		m_expression->updateOpCodeLength(XPathExpression::eOP_VARIABLE,
										 opPos);
	}
	else if(m_tokenChar == '(')
	{
		nextToken();

		m_expression->appendOpCode(XPathExpression::eOP_GROUP);

		Expr();

		consumeExpected(')');

		m_expression->updateOpCodeLength(XPathExpression::eOP_GROUP,
										 opPos);
	}
	else if(('.' == m_tokenChar && m_token.length() > 1 && isDigit(charAt(m_token, 1)) == true)
			 || isDigit(m_tokenChar) == true)
	{
		m_expression->appendOpCode(XPathExpression::eOP_NUMBERLIT);

		Number();

		m_expression->updateOpCodeLength(XPathExpression::eOP_NUMBERLIT,
										 opPos);
	}
	else if(lookahead('(', 1) == true ||
			(lookahead(':', 1) == true && lookahead('(', 3) == true))
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



void
XPathProcessorImpl::FunctionCall()
{
	assert(m_expression != 0);

	const int	opPos = m_expression->opCodeMapLength();

	if(lookahead(':', 1) == true)
	{
		m_expression->appendOpCode(XPathExpression::eOP_EXTFUNCTION);

		std::vector<XPathExpression::OpCodeMapValueType>	theArgs(2);

		theArgs[0] = m_expression->getTokenPosition();

		nextToken();

		consumeExpected(':');

		theArgs[1] = m_expression->getTokenPosition();

		nextToken();

		m_expression->setOpCodeArgs(XPathExpression::eOP_EXTFUNCTION,
									opPos,
									theArgs);
	}
	else
	{
		if (isValidFunction(m_token) == false)
		{
			warn(XalanDOMString("Could not find function: ") +
				 m_token +
				 XalanDOMString("()"));
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

				XPathExpression::OpCodeMapValueType 	thePosition =
					static_cast<XPathExpression::OpCodeMapValueType>(m_expression->getTokenPosition()) - 1;

				assert(m_expression->getToken(thePosition) != 0 &&
					   equals(m_expression->getToken(thePosition)->str(), m_token));

				std::vector<XPathExpression::OpCodeMapValueType>	theArgs(1, thePosition);

				m_expression->appendOpCode(XPathExpression::eOP_FUNCTION,
										   theArgs);
			}
		}

		nextToken();
	}

	consumeExpected('(');

	while(tokenIs(')') == false)
	{
		if(tokenIs(',') == true)
		{
			error("Found ',' but no preceding argument!");
		}

		Argument();

		if(tokenIs(')') == false)
		{

			consumeExpected(',');

			if(tokenIs(')') == true)
			{
				error("Found ',' but no following argument!");
			}
		}
	}

	consumeExpected(')');

	// Terminate for safety.
	m_expression->appendOpCode(XPathExpression::eENDOP);

	m_expression->updateOpCodeLength(opPos);
}



void
XPathProcessorImpl::LocationPath()
{
	const int	opPos = m_expression->opCodeMapLength();

	m_expression->appendOpCode(XPathExpression::eOP_LOCATIONPATH);

	if(tokenIs('/') == true)
	{
		const int	newOpPos = m_expression->opCodeMapLength();

		std::vector<XPathExpression::OpCodeMapValueType>	theArgs(1);

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

	while(tokenIs('/') == true)
	{
		nextToken();

		Step();
	}
}
  


void
XPathProcessorImpl::Step()
{
	const int	opPos = m_expression->opCodeMapLength();

	if(tokenIs('.') == true)
	{
		nextToken();

		if(tokenIs('[') == true)
		{
			error("'..[predicate]' or '.[predicate]' is illegal syntax.  Use 'self::node()[predicate]' instead.");
		}

		std::vector<XPathExpression::OpCodeMapValueType>	theArgs(1);

		// Tell how long the step is without the predicate
		theArgs[0] = 4;

		m_expression->appendOpCode(XPathExpression::eFROM_SELF,
								   theArgs);

		m_expression->appendOpCode(XPathExpression::eNODETYPE_NODE);
	}
	else if(tokenIs(XALAN_STATIC_UCODE_STRING("..")) == true)
	{
		nextToken();

		std::vector<XPathExpression::OpCodeMapValueType>	theArgs(1);

		// Tell how long the step is without the predicate
		theArgs[0] = 4;

		m_expression->appendOpCode(XPathExpression::eFROM_PARENT,
								   theArgs);

		m_expression->appendOpCode(XPathExpression::eNODETYPE_NODE);
	}
	else
	{
		Basis();

		while(tokenIs('[') == true)
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
	
	// The next blocks guarantee that a FROM_XXX will be added.
	if(lookahead(XALAN_STATIC_UCODE_STRING("::"), 1) == true)
	{
		AxisName();

		nextToken();
		nextToken();
	}
	else if(tokenIs('@') == true)
	{
		m_expression->appendOpCode(XPathExpression::eFROM_ATTRIBUTES);

		nextToken();
	}
	else if(tokenIs('/') == true)
	{
		// Have to fix up for patterns such as '//@foo' or '//attribute::foo',
		// which translate to 'descendant-or-self::node()/attribute::foo'.
		// notice I leave the '/' on the queue, so the next will be processed 
		// by a regular step pattern.
		// if(lookahead('@', 1) == true || lookahead("::", 2) == true)
		{
			std::vector<XPathExpression::OpCodeMapValueType>	theArgs(1);

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
		m_expression->appendOpCode(XPathExpression::eFROM_CHILDREN);
	}

	NodeTest();

	// Tell how long the step is without the predicate
	m_expression->updateOpCodeLengthAfterNodeTest(opPos);
}



void
XPathProcessorImpl::AxisName()
{
	assert(m_xpath != 0);
	assert(m_expression != 0);

	AxisNamesMapType::const_iterator	i =
		s_axisNames.find(m_token);

	if (i == s_axisNames.end())
	{
		error(XalanDOMString("illegal axis name: ") +
			  m_token);
	}
	else
	{
		m_expression->appendOpCode((*i).second);
	}
}



void
XPathProcessorImpl::NodeTest()
{
	assert(m_xpath != 0);
	assert(m_expression != 0);

	if(lookahead('(', 1) == true)
	{
		NodeTypesMapType::const_iterator	i =
			s_nodeTypes.find(m_token);

		if (i == s_nodeTypes.end())
		{
			error(XalanDOMString("Unknown nodetype: ") +
				  m_token);
		}
		else
		{
			nextToken();

			m_expression->appendOpCode((*i).second);
		
			consumeExpected('(');

			if(XPathExpression::eNODETYPE_PI == (*i).second)
			{
				if(tokenIs(')') == false)
				{
					Literal();
				}
			}

			consumeExpected(')');
		}
	}
	else
	{
		// Assume name of attribute or element.
		m_expression->appendOpCode(XPathExpression::eNODENAME);

		if(lookahead(':', 1) == true)
		{
			if(tokenIs('*') == true)
			{
				m_expression->appendOpCode(XPathExpression::eELEMWILDCARD);
			}
			else
			{
				m_expression->pushCurrentTokenOnOpCodeMap();
			}

			nextToken();

			consumeExpected(':');
		}
		else
		{
			m_expression->appendOpCode(XPathExpression::eEMPTY);
		}

		if(tokenIs('*') == true)
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
	if(tokenIs('[') == true)
	{
		nextToken();

		PredicateExpr();		

		consumeExpected(']');
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

	m_expression->pushCurrentTokenOnOpCodeMap();

	nextToken();

	consumeExpected(':');

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
	assert(m_xobjectFactory != 0);

	const int last = length(m_token) - 1;

	assert(last > 0);

	const XalanDOMChar 	c0 = m_tokenChar;
	const XalanDOMChar 	cX = charAt(m_token, last);

	if((c0 == '\"' && cX == '\"') ||
	   (c0 == '\'' && cX == '\''))
	{
		XObject* const	theArgument =
			m_xobjectFactory->createString(substring(m_token, 1, last));

		m_expression->pushArgumentOnOpCodeMap(theArgument);

		nextToken();
	}
	else
	{
		error(XalanDOMString("Pattern literal (") +
			  m_token +
			  XalanDOMString(") needs to be quoted!"));
	}
}
  


void
XPathProcessorImpl::Number()
{
	assert(m_xpath != 0);
	assert(m_expression != 0);
	assert(m_xobjectFactory != 0);

	if(0 != length(m_token))
	{
		// Mutate the token to remove the quotes and have the XNumber object
		// already made.
		const double	num = DOMStringToDouble(m_token);

		XObject* const	theArgument =
			m_xobjectFactory->createNumber(num);

		m_expression->pushArgumentOnOpCodeMap(theArgument);

		nextToken();
	}
}



void
XPathProcessorImpl::Pattern()
{	 
	while(true)
	{
		LocationPathPattern();

		if(tokenIs('|') == true)
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

	// These token FUNC_KEY_STRING should not be here, as it is really
	// part of the XSLT standard, and not the XPATH standard.
	if(lookahead('(', 1) == true &&
					tokenIs(FUNC_ID_STRING) == true ||
				  tokenIs(FUNC_KEY_STRING) == true)
	{
		IdKeyPattern();
	}

	if(tokenIs('/') == true)
	{
		const int	newOpPos = m_expression->opCodeMapLength();

		std::vector<XPathExpression::OpCodeMapValueType>	theArgs(1);

		// Tell how long the step is without the predicate
		theArgs[0] = 4;

		if(lookahead('/', 1) == true)
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

	if(!tokenIs('|') == true && length(m_token) != 0)
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

	while(tokenIs('/') == true)
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

	// The next blocks guarantee that a MATCH_XXX will be added.
	if(tokenIs('@') == true)
	{
		m_expression->appendOpCode(XPathExpression::eMATCH_ATTRIBUTE);

		nextToken();
	}
	else if(tokenIs('/') == true)
	{
		m_expression->appendOpCode(XPathExpression::eMATCH_ANY_ANCESTOR);

		nextToken();
	}
	else
	{
		if(tokenIs('/') == true)
		{
			nextToken();
		}

		matchTypePos = m_expression->opCodeMapLength();

		m_expression->appendOpCode(XPathExpression::eMATCH_IMMEDIATE_ANCESTOR);
	}

    // Make room for telling how long the step is without the predicate.
	// This will be replaced by the right value.
	m_expression->appendOpCode(XPathExpression::eENDOP);

	NodeTest();

	m_expression->updateOpCodeLengthAfterNodeTest(opPos);

	while(tokenIs('[') == true)
	{
		Predicate();
	}

	if(matchTypePos > -1 && tokenIs('/') == true && lookahead('/', 1) == true)
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
XPathProcessorImpl::initializeKeywordsTable()
{
	// $$$ ToDo: This is very confusing.  This table is only used
	// by getKeywordToken().  But if you look at the switch
	// statement there, none of these values are considered in the
	// case statement.  So what's the point?

	// s_keywords[FROM_SELF_ABBREVIATED_STRING] = XPathExpression::eFROM_SELF;
	// s_keywords[FROM_ATTRIBUTE_STRING] = XPathExpression::eFROM_ATTRIBUTE;
	// s_keywords[FROM_DOC_STRING] = XPathExpression::eFROM_DOC;
	// s_keywords[FROM_DOCREF_STRING] = XPathExpression::eFROM_DOCREF;
	// s_keywords[FROM_ID_STRING] = XPathExpression::eFROM_ID;
	// s_keywords[FROM_IDREF_STRING] = XPathExpression::eFROM_IDREF;
	// s_keywords[FUNC_ID_STRING] = XPathExpression::eFUNC_ID;
	// s_keywords[FUNC_KEY_STRING] = XPathExpression::eFUNC_KEY;
	// s_keywords[FUNC_DOCUMENT_STRING] = XPathExpression::eFUNC_DOC;
}



void
XPathProcessorImpl::initializeFunctionTable()
{
	s_functions[NODETYPE_PI_STRING] = XPathExpression::eNODETYPE_PI;
	s_functions[NODETYPE_COMMENT_STRING] = XPathExpression::eNODETYPE_COMMENT;
	s_functions[NODETYPE_TEXT_STRING] = XPathExpression::eNODETYPE_TEXT;
	s_functions[NODETYPE_NODE_STRING] = XPathExpression::eNODETYPE_NODE;
}



void
XPathProcessorImpl::initializeAxisNamesTable()
{
	s_axisNames[FROM_ANCESTORS_STRING] = XPathExpression::eFROM_ANCESTORS;
	s_axisNames[FROM_ANCESTORS_OR_SELF_STRING] = XPathExpression::eFROM_ANCESTORS_OR_SELF;
	s_axisNames[FROM_ATTRIBUTES_STRING] = XPathExpression::eFROM_ATTRIBUTES;
	s_axisNames[FROM_CHILDREN_STRING] = XPathExpression::eFROM_CHILDREN;
	s_axisNames[FROM_DESCENDANTS_STRING] = XPathExpression::eFROM_DESCENDANTS;
	s_axisNames[FROM_DESCENDANTS_OR_SELF_STRING] = XPathExpression::eFROM_DESCENDANTS_OR_SELF;
	s_axisNames[FROM_FOLLOWING_STRING] = XPathExpression::eFROM_FOLLOWING;
	s_axisNames[FROM_FOLLOWING_SIBLINGS_STRING] = XPathExpression::eFROM_FOLLOWING_SIBLINGS;
	s_axisNames[FROM_PARENT_STRING] = XPathExpression::eFROM_PARENT;
	s_axisNames[FROM_PRECEDING_STRING] = XPathExpression::eFROM_PRECEDING;
	s_axisNames[FROM_PRECEDING_SIBLINGS_STRING] = XPathExpression::eFROM_PRECEDING_SIBLINGS;
	s_axisNames[FROM_SELF_STRING] = XPathExpression::eFROM_SELF;
	s_axisNames[FROM_NAMESPACE_STRING] = XPathExpression::eFROM_NAMESPACE;
}



void
XPathProcessorImpl::initializeNodeTypesTable()
{
	s_nodeTypes[NODETYPE_COMMENT_STRING] = XPathExpression::eNODETYPE_COMMENT;
	s_nodeTypes[NODETYPE_TEXT_STRING] = XPathExpression::eNODETYPE_TEXT;
	s_nodeTypes[NODETYPE_PI_STRING] = XPathExpression::eNODETYPE_PI;
	s_nodeTypes[NODETYPE_NODE_STRING] = XPathExpression::eNODETYPE_NODE;
	s_nodeTypes[NODETYPE_ANYELEMENT_STRING] = XPathExpression::eNODETYPE_ANYELEMENT;
}



XPathProcessorImpl::StaticInitializer::StaticInitializer()
{
	XPathProcessorImpl::initializeKeywordsTable();
	XPathProcessorImpl::initializeFunctionTable();
	XPathProcessorImpl::initializeAxisNamesTable();
	XPathProcessorImpl::initializeNodeTypesTable();
}
