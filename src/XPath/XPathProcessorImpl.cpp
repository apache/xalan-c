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
#include "XPathParserException.hpp"
#include "XPathSupport.hpp"



namespace
{

const char* const	FROM_ANCESTORS_STRING = "ancestor";
const char* const	FROM_ANCESTORS_OR_SELF_STRING = "ancestor-or-self";
const char* const	FROM_ATTRIBUTES_STRING = "attribute";
const char* const	FROM_CHILDREN_STRING = "child";
const char* const	FROM_DESCENDANTS_STRING = "descendant";
const char* const	FROM_DESCENDANTS_OR_SELF_STRING = "descendant-or-self";
const char* const	FROM_FOLLOWING_STRING = "following";
const char* const	FROM_FOLLOWING_SIBLINGS_STRING = "following-sibling";
const char* const	FROM_PARENT_STRING = "parent";
const char* const	FROM_PRECEDING_STRING = "preceding";
const char* const	FROM_PRECEDING_SIBLINGS_STRING = "preceding-sibling";
const char* const	FROM_SELF_STRING = "self";
const char* const	FROM_SELF_ABBREVIATED_STRING = ".";
const char* const	FROM_NAMESPACE_STRING = "namespace";

// This shouldn't really be here, since it duplicates a string that is part
// of the information that is maintained by the class XPathFunctionTable,
// but this is a reasonable optimization.
const char* const	FUNC_ID_STRING = "id";


// These shouldn't really be here, since they are not part of the XPath standard,
// but rather a part ofthe XSLT standard.
const char* const	FUNC_KEY_STRING = "key";
const char* const	FUNC_DOC_STRING = "doc";
const char* const	FUNC_DOCUMENT_STRING = "document";


const char* const	NODETYPE_COMMENT_STRING = "comment";
const char* const	NODETYPE_TEXT_STRING = "text";
const char* const	NODETYPE_PI_STRING = "processing-instruction";
const char* const	NODETYPE_NODE_STRING = "node";
const char* const	NODETYPE_ANYELEMENT_STRING = "*";


#if 0  
const string	XPathProcessorImpl::FROM_SELF_ABBREVIATED_STRING = ".";
const string	XPathProcessorImpl::FROM_ATTRIBUTE_STRING = "@";
const string	XPathProcessorImpl::FROM_DOC_STRING = "document";
const string	XPathProcessorImpl::FROM_DOCREF_STRING = "document";
const string	XPathProcessorImpl::FROM_ID_STRING = "id";
const string	XPathProcessorImpl::FROM_IDREF_STRING = "idref";
const string	XPathProcessorImpl::FUNC_CURRENT_STRING = "current";
const string	XPathProcessorImpl::FUNC_LAST_STRING = "last";
const string	XPathProcessorImpl::FUNC_POSITION_STRING = "position";
const string	XPathProcessorImpl::FUNC_COUNT_STRING = "count";
const string	XPathProcessorImpl::FUNC_ID_STRING = "id";
const string	XPathProcessorImpl::FUNC_IDREF_STRING = "idref";
const string	XPathProcessorImpl::FUNC_KEY_STRING = "key";
const string	XPathProcessorImpl::FUNC_KEYREF_STRING = "keyref";
const string	XPathProcessorImpl::FUNC_DOCUMENT_STRING = "document";
const string	XPathProcessorImpl::FUNC_DOCREF_STRING = "docref";
const string	XPathProcessorImpl::FUNC_LOCAL_PART_STRING = "local-name";
const string	XPathProcessorImpl::FUNC_NAMESPACE_STRING = "namespace-uri";
const string	XPathProcessorImpl::FUNC_NAME_STRING = "name";
const string	XPathProcessorImpl::FUNC_GENERATE_ID_STRING = "generate-id";
const string	XPathProcessorImpl::FUNC_NOT_STRING = "not";
const string	XPathProcessorImpl::FUNC_TRUE_STRING = "true";
const string	XPathProcessorImpl::FUNC_FALSE_STRING = "false";
const string	XPathProcessorImpl::FUNC_BOOLEAN_STRING = "boolean";
const string	XPathProcessorImpl::FUNC_LANG_STRING = "lang";
const string	XPathProcessorImpl::FUNC_NUMBER_STRING = "number";
const string	XPathProcessorImpl::FUNC_FLOOR_STRING = "floor";
const string	XPathProcessorImpl::FUNC_CEILING_STRING = "ceiling";
const string	XPathProcessorImpl::FUNC_ROUND_STRING = "round";
const string	XPathProcessorImpl::FUNC_SUM_STRING = "sum";
const string	XPathProcessorImpl::FUNC_STRING_STRING = "string";
const string	XPathProcessorImpl::FUNC_STARTS_WITH_STRING = "starts-with";
const string	XPathProcessorImpl::FUNC_CONTAINS_STRING = "contains";
const string	XPathProcessorImpl::FUNC_SUBSTRING_BEFORE_STRING = "substring-before";
const string	XPathProcessorImpl::FUNC_SUBSTRING_AFTER_STRING = "substring-after";
const string	XPathProcessorImpl::FUNC_NORMALIZE_STRING = "normalize";
const string	XPathProcessorImpl::FUNC_TRANSLATE_STRING = "translate";
const string	XPathProcessorImpl::FUNC_CONCAT_STRING = "concat";
const string	XPathProcessorImpl::FUNC_FORMAT_NUMBER_STRING = "format-number";
const string	XPathProcessorImpl::FUNC_SYSTEM_PROPERTY_STRING = "system-property";
const string	XPathProcessorImpl::FUNC_EXT_FUNCTION_AVAILABLE_STRING = "extension-function-available";
const string	XPathProcessorImpl::FUNC_EXT_ELEM_AVAILABLE_STRING = "extension-element-available";
const string	XPathProcessorImpl::FUNC_SUBSTRING_STRING = "substring";
const string	XPathProcessorImpl::FUNC_STRING_LENGTH_STRING = "string-length";
const string	XPathProcessorImpl::FUNC_DOCLOCATION_STRING = "document-location";
#endif


};



XPathProcessorImpl::KeywordsMapType 			XPathProcessorImpl::s_keywords;
XPathProcessorImpl::FunctionNameMapType 		XPathProcessorImpl::s_functions;
XPathProcessorImpl::AxisNamesMapType			XPathProcessorImpl::s_axisNames;
XPathProcessorImpl::NodeTypesMapType			XPathProcessorImpl::s_nodeTypes;



XPathProcessorImpl::XPathProcessorImpl(
			XPathEnvSupport&	theXPathEnvSupport,
			XPathSupport&		theXPathSupport) :
	m_envSupport(theXPathEnvSupport),
	m_support(theXPathSupport),
	m_xpath(0),
	m_token(),
	m_tokenChar(0),
	m_expression(0)
{
	// $$$ ToDo: This is not thread-safe!!!
	static StaticInitializer	theInitializer;
}



XPathProcessorImpl::~XPathProcessorImpl()
{
	m_expression = 0;
	m_xpath = 0;
}



void
XPathProcessorImpl::initXPath(
			XPath&					pathObj,
			const DOMString&		expression,
			const PrefixResolver&	resolver)
{
	m_xpath = &pathObj;

	m_expression = &m_xpath->getExpression();

	m_xpath->setPrefixResolver(resolver);

	tokenize(expression);

	m_expression->appendOpCode(XPathExpression::eOP_XPATH);

	nextToken();

	Expr();

	m_xpath = 0;
	m_expression = 0;
}



void
XPathProcessorImpl::initMatchPattern(
			XPath&					pathObj,
			const DOMString&		expression,
			const PrefixResolver&	resolver)
{
	m_xpath = &pathObj;

	m_expression = &m_xpath->getExpression();

	m_expression->reset();

	m_xpath->setPrefixResolver(resolver);

	tokenize(expression);

	m_expression->appendOpCode(XPathExpression::eOP_MATCHPATTERN);

	nextToken();

	Pattern();

	// Terminate for safety.
	m_expression->appendOpCode(XPathExpression::eENDOP);

	m_expression->shrink();

	m_xpath = 0;
	m_expression = 0;
}



void
XPathProcessorImpl::tokenize(
			const DOMString&			pat,
			std::vector<DOMString>* 	targetStrings)
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
		XMLCh	c = charAt(pat, i);

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
XPathProcessorImpl::recordTokenString(std::vector<DOMString>&	targetStrings)
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
XPathProcessorImpl::addToTokenQueue(const DOMString&	s) const
{
	assert(m_xpath != 0);
	assert(m_expression != 0);

	m_expression->pushToken(m_xpath->getXObjectFactory().createString(s));
}



int
XPathProcessorImpl::mapNSTokens(
			const DOMString&	pat,
			int 				startSubstring,
			int 				posOfNSSep,
			int 				posOfScan) const
{
	assert(m_xpath != 0);

	const DOMString 	prefix = substring(pat, startSubstring, posOfNSSep);

	const DOMString 	uName =
				m_xpath->getPrefixResolver().getNamespaceForPrefix(prefix);

	if(length(uName) > 0)
	{
		addToTokenQueue(uName);

		addToTokenQueue(":");

		const DOMString 	s = substring(pat, posOfNSSep + 1, posOfScan);
	  
		if(length(s) > 0)
		{
			addToTokenQueue(s);
		}
	}
	else
	{
		// error(DOMString("Could not locate namespace for prefix: ") + prefix);
		addToTokenQueue(prefix);

		addToTokenQueue(":");

		const DOMString 	s = substring(pat, posOfNSSep + 1, posOfScan);

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
XPathProcessorImpl::tokenIs(const char* 	s) const
{
	return equals(m_token, s);
}



bool
XPathProcessorImpl::tokenIs(char	c) const
{
	return m_tokenChar == static_cast<XMLCh>(c) ? true : false;
}



bool
XPathProcessorImpl::lookahead(
			char	c,
			int 	n) const
{
	const DOMString 	tok =
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
			const char* 	s,
			int 			n) const
{
	const DOMString 	tok =
		getTokenRelative(n - 1);

	return equals(tok, s);
}



bool
XPathProcessorImpl::lookbehind(
			char	c,
			int 	n) const
{
	const DOMString 	tok =
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
	const DOMString 	tok =
		getTokenRelative(-(n + 1));

	const XMLCh 		c0 = length(tok) == 0 ? '|' : charAt(tok, 0);

	return c0 == '|' ? false : true;
}



void
XPathProcessorImpl::nextToken()
{
	assert(m_expression != 0);

	const XObject* const	theNextToken =
			m_expression->getNextToken();

	m_token = theNextToken == 0 ? "" : theNextToken->str();

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

	m_token = thePreviousToken == 0 ? "" : thePreviousToken->str();

	if(length(m_token) > 0)
	{
		m_tokenChar = charAt(m_token, 0);
	}
	else
	{
		m_tokenChar = 0;
	}
}



DOMString
XPathProcessorImpl::getTokenRelative(int	theOffset) const
{
	assert(m_expression != 0);

	const XObject* const	theToken =
		m_expression->getRelativeToken(theOffset);

	return theToken == 0 ? DOMString() : theToken->str();
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
		error(DOMString("Expected ") +
			  DOMString(expected) +
			  DOMString(", but found: ") +
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
		DOMString	theMsg("Expected ");

		theMsg += expected;
		theMsg += ", but found: ";
		theMsg += m_token;

		error(theMsg);
	}
}



void
XPathProcessorImpl::warn(
			const DOMString&	msg,
			const DOM_Node& 	sourceNode) const
{
	assert(m_xpath != 0);

	const bool	shouldThrow =
		m_envSupport.problem(XPathEnvSupport::eXPATHParser, 
							 XPathEnvSupport::eWarning,
							 m_xpath->getPrefixResolver(), 
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
			const DOMString&	msg,
			const DOM_Node& sourceNode) const
{
	DOMString	emsg;

	if (m_expression == 0)
	{
		emsg = msg;
	}
	else
	{
		const DOMString&	theCurrentPattern =
				m_expression->getCurrentPattern();

		DOMStringPrintWriter	thePrintWriter;

		if (length(theCurrentPattern) != 0)
		{
			thePrintWriter.print("pattern = '");
			thePrintWriter.println(theCurrentPattern);
		}

		thePrintWriter.print(msg);

		m_expression->dumpRemainingTokenQueue(thePrintWriter);

		emsg = thePrintWriter.getString();
	}

	assert(m_xpath != 0);

	const bool	shouldThrow =
		m_envSupport.problem(XPathEnvSupport::eXPATHParser, 
							 XPathEnvSupport::eError,
							 m_xpath->getPrefixResolver(), 
							 sourceNode,
							 emsg,
							 0,
							 0);

	if(shouldThrow == true)
	{
	  throw XPathParserException(emsg);
	}
}
  


#if 0
  private static Hashtable m_keywords = new Hashtable();
  private static Hashtable m_axisnames = new Hashtable();
  static Hashtable m_functions = new Hashtable();
  private static Hashtable m_nodetypes = new Hashtable();

  private static final String FROM_ANCESTORS_STRING = "ancestor";
  private static final String FROM_ANCESTORS_OR_SELF_STRING = "ancestor-or-self";
  private static final String FROM_ATTRIBUTES_STRING = "attribute";
  private static final String FROM_CHILDREN_STRING = "child";
  private static final String FROM_DESCENDANTS_STRING = "descendant";
  private static final String FROM_DESCENDANTS_OR_SELF_STRING = "descendant-or-self";
  private static final String FROM_FOLLOWING_STRING = "following";
  private static final String FROM_FOLLOWING_SIBLINGS_STRING = "following-sibling";
  private static final String FROM_PARENT_STRING = "parent";
  private static final String FROM_PRECEDING_STRING = "preceding";
  private static final String FROM_PRECEDING_SIBLINGS_STRING = "preceding-sibling";
  private static final String FROM_SELF_STRING = "self";
  private static final String FROM_NAMESPACE_STRING = "namespace";
  
  private static final String FROM_SELF_ABBREVIATED_STRING = ".";
  private static final String NODETYPE_COMMENT_STRING = "comment";
  private static final String NODETYPE_TEXT_STRING = "text";
  private static final String NODETYPE_PI_STRING = "processing-instruction";
  private static final String NODETYPE_NODE_STRING = "node";
  private static final String FROM_ATTRIBUTE_STRING = "@";
  private static final String FROM_DOC_STRING = "document";
  private static final String FROM_DOCREF_STRING = "document";
  private static final String FROM_ID_STRING = "id";
  private static final String FROM_IDREF_STRING = "idref";
  private static final String NODETYPE_ANYELEMENT_STRING = "*";
  private static final String FUNC_CURRENT_STRING = "current";
  private static final String FUNC_LAST_STRING = "last";
  private static final String FUNC_POSITION_STRING = "position";
  private static final String FUNC_COUNT_STRING = "count";
  private static final String FUNC_ID_STRING = "id";
  private static final String FUNC_IDREF_STRING = "idref";
  private static final String FUNC_KEY_STRING = "key";
  private static final String FUNC_KEYREF_STRING = "keyref";
  private static final String FUNC_DOC_STRING = "doc";
  private static final String FUNC_DOCUMENT_STRING = "document";
  private static final String FUNC_DOCREF_STRING = "docref";
  private static final String FUNC_LOCAL_PART_STRING = "local-name";
  private static final String FUNC_NAMESPACE_STRING = "namespace-uri";
  private static final String FUNC_NAME_STRING = "name";
  private static final String FUNC_GENERATE_ID_STRING = "generate-id";
  private static final String FUNC_NOT_STRING = "not";
  private static final String FUNC_TRUE_STRING = "true";
  private static final String FUNC_FALSE_STRING = "false";
  private static final String FUNC_BOOLEAN_STRING = "boolean";
  private static final String FUNC_LANG_STRING = "lang";
  private static final String FUNC_NUMBER_STRING = "number";
  private static final String FUNC_FLOOR_STRING = "floor";
  private static final String FUNC_CEILING_STRING = "ceiling";
  private static final String FUNC_ROUND_STRING = "round";
  private static final String FUNC_SUM_STRING = "sum";
  private static final String FUNC_STRING_STRING = "string";
  private static final String FUNC_STARTS_WITH_STRING = "starts-with";
  private static final String FUNC_CONTAINS_STRING = "contains";
  private static final String FUNC_SUBSTRING_BEFORE_STRING = "substring-before";
  private static final String FUNC_SUBSTRING_AFTER_STRING = "substring-after";
  private static final String FUNC_NORMALIZE_STRING = "normalize";
  private static final String FUNC_TRANSLATE_STRING = "translate";
  private static final String FUNC_CONCAT_STRING = "concat";
  private static final String FUNC_FORMAT_NUMBER_STRING = "format-number";
  private static final String FUNC_SYSTEM_PROPERTY_STRING = "system-property";
  private static final String FUNC_EXT_FUNCTION_AVAILABLE_STRING = "extension-function-available";
  private static final String FUNC_EXT_ELEM_AVAILABLE_STRING = "extension-element-available";
  private static final String FUNC_SUBSTRING_STRING = "substring";
  private static final String FUNC_STRING_LENGTH_STRING = "string-length";

  // Proprietary, built in functions
  private static final String FUNC_DOCLOCATION_STRING = "document-location";

  static 
  {
	m_axisnames.put((FROM_ANCESTORS_STRING).intern(), new Integer(XPath.FROM_ANCESTORS));
	m_axisnames.put((FROM_ANCESTORS_OR_SELF_STRING).intern(), new Integer(XPath.FROM_ANCESTORS_OR_SELF));
	m_axisnames.put((FROM_ATTRIBUTES_STRING).intern(), new Integer(XPath.FROM_ATTRIBUTES));
	m_axisnames.put((FROM_CHILDREN_STRING).intern(), new Integer(XPath.FROM_CHILDREN));
	m_axisnames.put((FROM_DESCENDANTS_STRING).intern(), new Integer(XPath.FROM_DESCENDANTS));
	m_axisnames.put((FROM_DESCENDANTS_OR_SELF_STRING).intern(), new Integer(XPath.FROM_DESCENDANTS_OR_SELF));
	m_axisnames.put((FROM_FOLLOWING_STRING).intern(), new Integer(XPath.FROM_FOLLOWING));
	m_axisnames.put((FROM_FOLLOWING_SIBLINGS_STRING).intern(), new Integer(XPath.FROM_FOLLOWING_SIBLINGS));
	m_axisnames.put((FROM_PARENT_STRING).intern(), new Integer(XPath.FROM_PARENT));
	m_axisnames.put((FROM_PRECEDING_STRING).intern(), new Integer(XPath.FROM_PRECEDING));
	m_axisnames.put((FROM_PRECEDING_SIBLINGS_STRING).intern(), new Integer(XPath.FROM_PRECEDING_SIBLINGS));
	m_axisnames.put((FROM_SELF_STRING).intern(), new Integer(XPath.FROM_SELF));
	m_axisnames.put((FROM_NAMESPACE_STRING).intern(), new Integer(XPath.FROM_NAMESPACE));

	m_nodetypes.put((NODETYPE_COMMENT_STRING).intern(), new Integer(XPath.NODETYPE_COMMENT));
	m_nodetypes.put((NODETYPE_TEXT_STRING).intern(), new Integer(XPath.NODETYPE_TEXT));
	m_nodetypes.put((NODETYPE_PI_STRING).intern(), new Integer(XPath.NODETYPE_PI));
	m_nodetypes.put((NODETYPE_NODE_STRING).intern(), new Integer(XPath.NODETYPE_NODE));
	m_nodetypes.put((NODETYPE_ANYELEMENT_STRING).intern(), new Integer(XPath.NODETYPE_ANYELEMENT));
	
	m_keywords.put((FROM_SELF_ABBREVIATED_STRING).intern(), new Integer(XPath.FROM_SELF));
	// m_keywords.put((FROM_ATTRIBUTE_STRING).intern(), new Integer(XPath.FROM_ATTRIBUTE));
	// m_keywords.put((FROM_DOC_STRING).intern(), new Integer(XPath.FROM_DOC));
	// m_keywords.put((FROM_DOCREF_STRING).intern(), new Integer(XPath.FROM_DOCREF));
	// m_keywords.put((FROM_ID_STRING).intern(), new Integer(XPath.FROM_ID));
	// m_keywords.put((FROM_IDREF_STRING).intern(), new Integer(XPath.FROM_IDREF));

	m_keywords.put((FUNC_ID_STRING).intern(), new Integer(XPath.FUNC_ID));
	m_keywords.put((FUNC_KEY_STRING).intern(), new Integer(XPath.FUNC_KEY));
	m_keywords.put((FUNC_DOCUMENT_STRING).intern(), new Integer(XPath.FUNC_DOC));

	m_functions.put((FUNC_CURRENT_STRING).intern(), new Integer(XPath.FUNC_CURRENT));
	m_functions.put((FUNC_LAST_STRING).intern(), new Integer(XPath.FUNC_LAST));
	m_functions.put((FUNC_POSITION_STRING).intern(), new Integer(XPath.FUNC_POSITION));
	m_functions.put((FUNC_COUNT_STRING).intern(), new Integer(XPath.FUNC_COUNT));
	m_functions.put((FUNC_ID_STRING).intern(), new Integer(XPath.FUNC_ID));
	m_functions.put((FUNC_KEY_STRING).intern(), new Integer(XPath.FUNC_KEY));
	m_functions.put((FUNC_DOCUMENT_STRING).intern(), new Integer(XPath.FUNC_DOC));
	m_functions.put((FUNC_LOCAL_PART_STRING).intern(), new Integer(XPath.FUNC_LOCAL_PART));
	m_functions.put((FUNC_NAMESPACE_STRING).intern(), new Integer(XPath.FUNC_NAMESPACE));
	m_functions.put((FUNC_NAME_STRING).intern(), new Integer(XPath.FUNC_QNAME));
	m_functions.put((FUNC_GENERATE_ID_STRING).intern(), new Integer(XPath.FUNC_GENERATE_ID));
	m_functions.put((FUNC_NOT_STRING).intern(), new Integer(XPath.FUNC_NOT));
	m_functions.put((FUNC_TRUE_STRING).intern(), new Integer(XPath.FUNC_TRUE));
	m_functions.put((FUNC_FALSE_STRING).intern(), new Integer(XPath.FUNC_FALSE));
	m_functions.put((FUNC_BOOLEAN_STRING).intern(), new Integer(XPath.FUNC_BOOLEAN));
	m_functions.put((FUNC_LANG_STRING).intern(), new Integer(XPath.FUNC_LANG));
	m_functions.put((FUNC_NUMBER_STRING).intern(), new Integer(XPath.FUNC_NUMBER));
	m_functions.put((FUNC_FLOOR_STRING).intern(), new Integer(XPath.FUNC_FLOOR));
	m_functions.put((FUNC_CEILING_STRING).intern(), new Integer(XPath.FUNC_CEILING));
	m_functions.put((FUNC_ROUND_STRING).intern(), new Integer(XPath.FUNC_ROUND));
	m_functions.put((FUNC_SUM_STRING).intern(), new Integer(XPath.FUNC_SUM));
	m_functions.put((FUNC_STRING_STRING).intern(), new Integer(XPath.FUNC_STRING));
	m_functions.put((FUNC_STARTS_WITH_STRING).intern(), new Integer(XPath.FUNC_STARTS_WITH));
	m_functions.put((FUNC_CONTAINS_STRING).intern(), new Integer(XPath.FUNC_CONTAINS));
	m_functions.put((FUNC_SUBSTRING_BEFORE_STRING).intern(), new Integer(XPath.FUNC_SUBSTRING_BEFORE));
	m_functions.put((FUNC_SUBSTRING_AFTER_STRING).intern(), new Integer(XPath.FUNC_SUBSTRING_AFTER));
	m_functions.put((FUNC_NORMALIZE_STRING).intern(), new Integer(XPath.FUNC_NORMALIZE));
	m_functions.put((FUNC_TRANSLATE_STRING).intern(), new Integer(XPath.FUNC_TRANSLATE));
	m_functions.put((FUNC_CONCAT_STRING).intern(), new Integer(XPath.FUNC_CONCAT));
	m_functions.put((FUNC_FORMAT_NUMBER_STRING).intern(), new Integer(XPath.FUNC_FORMAT_NUMBER));
	m_functions.put((FUNC_SYSTEM_PROPERTY_STRING).intern(), new Integer(XPath.FUNC_SYSTEM_PROPERTY));
	m_functions.put((FUNC_EXT_FUNCTION_AVAILABLE_STRING).intern(), new Integer(XPath.FUNC_EXT_FUNCTION_AVAILABLE));
	m_functions.put((FUNC_EXT_ELEM_AVAILABLE_STRING).intern(), new Integer(XPath.FUNC_EXT_ELEM_AVAILABLE));
	m_functions.put((FUNC_SUBSTRING_STRING).intern(), new Integer(XPath.FUNC_SUBSTRING));
	m_functions.put((FUNC_STRING_LENGTH_STRING).intern(), new Integer(XPath.FUNC_STRING_LENGTH));

	// These aren't really functions.
	m_functions.put((NODETYPE_COMMENT_STRING).intern(), new Integer(XPath.NODETYPE_COMMENT));
	m_functions.put((NODETYPE_TEXT_STRING).intern(), new Integer(XPath.NODETYPE_TEXT));
	m_functions.put((NODETYPE_PI_STRING).intern(), new Integer(XPath.NODETYPE_PI));
	m_functions.put((NODETYPE_NODE_STRING).intern(), new Integer(XPath.NODETYPE_NODE));
		
	m_functions.put((FUNC_DOCLOCATION_STRING).intern(), new Integer(XPath.FUNC_DOCLOCATION));
  }
#endif


  
int
XPathProcessorImpl::getKeywordToken(const DOMString&	key) const
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
XPathProcessorImpl::getFunctionToken(const DOMString&	key) const
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

	if(tokenIs("or") == true)
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

	if(tokenIs("and") == true)
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
		else if(tokenIs("div") == true)
		{
			theOpCode = XPathExpression::eOP_DIV;
		}
		else if(tokenIs("mod") == true)
		{
			theOpCode = XPathExpression::eOP_MOD;
		}
		else if(tokenIs("quo") == true)
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
			warn(DOMString("Could not find function: ") +
				 m_token +
				 DOMString("()"));
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

	if(tokenIs(".") == true)
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
	else if(tokenIs("..") == true)
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
	if(lookahead("::", 1) == true)
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
		error(DOMString("illegal axis name: ") +
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
			error(DOMString("Unknown nodetype: ") +
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

	const int last = length(m_token) - 1;

	assert(last > 0);

	const XMLCh 	c0 = m_tokenChar;
	const XMLCh 	cX = charAt(m_token, last);

	if((c0 == '\"' && cX == '\"') ||
	   (c0 == '\'' && cX == '\''))
	{
		XObject* const	theArgument =
			m_xpath->getXObjectFactory().createString(substring(m_token, 1, last));

		m_expression->pushArgumentOnOpCodeMap(theArgument);

		nextToken();
	}
	else
	{
		error(DOMString("Pattern literal (") +
			  m_token +
			  DOMString(") needs to be quoted!"));
	}
}
  


void
XPathProcessorImpl::Number()
{
	assert(m_xpath != 0);
	assert(m_expression != 0);

	if(0 != length(m_token))
	{
		// Mutate the token to remove the quotes and have the XNumber object
		// already made.
		const double	num = DOMStringToDouble(m_token);

		XObject* const	theArgument =
			m_xpath->getXObjectFactory().createNumber(num);

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
XPathProcessorImpl::isValidFunction(const DOMString&	key) const
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



#if 0
  // ============= GRAMMAR FUNCTIONS =================
  private void ____DIAGNOSTIC_FUNCTIONS____(){}

  public static void diagnoseXPathString( String str )	  
  {    
	XPathSupport callbacks = new XPathSupportDefault();
	XPathProcessorImpl processor = new XPathProcessorImpl(callbacks);
	XPath xpath = new XPath(callbacks);
	processor.initXPath(xpath, str, null);
	processor.diagnoseXPath(xpath, 0, 0);
  }

  static int diagnoseXPathBinaryOperation(String op, XPath xpath, int opPos, int indent)
  {
	System.out.println(op+" {");
	opPos+=2;
	
	opPos = diagnoseXPath(xpath, opPos, indent+1);
	
	opPos = diagnoseXPath(xpath, opPos, indent+1);
	
	indent(indent);
	System.out.println("}");
	return opPos;
  }

  static int diagnoseXPathUnaryOperation(String op, XPath xpath, int opPos, int indent)
  {
	System.out.println(op+" {");
	opPos+=2;
	opPos = diagnoseXPath(xpath, opPos, indent+1);
	indent(indent);
	System.out.println("}");
	return opPos;
  }

  static int diagnoseXPathMultiOperation(String op, int multiOp, XPath xpath, int opPos, int indent)
  {
	System.out.println(op+" {");
	opPos+=2;
	while(xpath.m_opMap[opPos] == multiOp)
	{
	  indent(indent+1);
	  System.out.println("{");
	  opPos = diagnoseXPath(xpath, opPos, indent+2);
	  indent(indent+1);
	  System.out.println("}");
	}
	indent(indent);
	System.out.println("}");
	return opPos;
  }
  
  static int diagnoseToken(XPath xpath, int opPos)
  {
	System.out.print("{");
	System.out.print(xpath.m_tokenQueue[xpath.m_opMap[opPos]]);
	System.out.print("}");
	return opPos+1;
  }

  static int diagnoseXPathSimpleOperation(String op, XPath xpath, int opPos, int indent)
  {
	opPos+=2;
	System.out.print(op);
	opPos = diagnoseToken(xpath, opPos);
	System.out.println("");
	return opPos;
  }

  static int diagnoseXPathLocationStep(String op, XPath xpath, int opPos, int indent)
  {
	// int opLen = xpath.m_opMap[opPos+xpath.MAPINDEX_LENGTH];
	int stepLen = xpath.m_opMap[opPos+xpath.MAPINDEX_LENGTH+1];
	opPos+=3;
	System.out.print(op);
	if(stepLen > 3)
	{
	  opPos = diagnoseXPath(xpath, opPos, 1);
	}
	System.out.println("");
	return opPos;
  }

  static int diagnoseXPath(XPath xpath, int opPos, int indent)
  {
	indent(indent);
	switch(xpath.m_opMap[opPos])
	{
	case xpath.OP_XPATH:
	  opPos = diagnoseXPathUnaryOperation("OP_XPATH", xpath, opPos, indent);
	  break;
	case xpath.EMPTY:
	  System.out.println("{EMPTY}");
	  opPos++;
	  break;
	case xpath.OP_OR:
	  opPos = diagnoseXPathBinaryOperation("OP_OR", xpath, opPos, indent);
	  break;
	case xpath.OP_AND:
	  opPos = diagnoseXPathBinaryOperation("OP_AND", xpath, opPos, indent);
	  break;
	case xpath.OP_NOTEQUALS:
	  opPos = diagnoseXPathBinaryOperation("OP_NOTEQUALS", xpath, opPos, indent);
	  break;
	case xpath.OP_EQUALS:
	  opPos = diagnoseXPathBinaryOperation("OP_EQUALS", xpath, opPos, indent);
	  break;
	case xpath.OP_LTE:
	  opPos = diagnoseXPathBinaryOperation("OP_LTE", xpath, opPos, indent);
	  break;
	case xpath.OP_LT:
	  opPos = diagnoseXPathBinaryOperation("OP_LT", xpath, opPos, indent);
	  break;
	case xpath.OP_GTE:
	  opPos = diagnoseXPathBinaryOperation("OP_GTE", xpath, opPos, indent);
	  break;
	case xpath.OP_GT:
	  opPos = diagnoseXPathBinaryOperation("OP_GT", xpath, opPos, indent);
	  break;
	case xpath.OP_PLUS:
	  opPos = diagnoseXPathBinaryOperation("OP_PLUS", xpath, opPos, indent);
	  break;
	case xpath.OP_MINUS:
	  opPos = diagnoseXPathBinaryOperation("OP_MINUS", xpath, opPos, indent);
	  break;
	case xpath.OP_MULT:
	  opPos = diagnoseXPathBinaryOperation("OP_MULT", xpath, opPos, indent);
	  break;
	case xpath.OP_DIV:
	  opPos = diagnoseXPathBinaryOperation("OP_DIV", xpath, opPos, indent);
	  break;
	case xpath.OP_MOD:
	  opPos = diagnoseXPathBinaryOperation("OP_MOD", xpath, opPos, indent);
	  break;
	case xpath.OP_QUO:
	  opPos = diagnoseXPathBinaryOperation("OP_QUO", xpath, opPos, indent);
	  break;
	case xpath.OP_NEG:
	  opPos = diagnoseXPathUnaryOperation("OP_NEG", xpath, opPos, indent);
	  break;
	case xpath.OP_STRING:
	  opPos = diagnoseXPathUnaryOperation("OP_STRING", xpath, opPos, indent);
	  break;
	case xpath.OP_BOOL:
	  opPos = diagnoseXPathUnaryOperation("OP_BOOL", xpath, opPos, indent);
	  break;
	case xpath.OP_NUMBER:
	  opPos = diagnoseXPathUnaryOperation("OP_NUMBER", xpath, opPos, indent);
	  break;
	case xpath.OP_UNION:
	  opPos = diagnoseXPathMultiOperation("OP_UNION", xpath.OP_LOCATIONPATH, xpath, opPos, indent);
	  break;
	case xpath.OP_LITERAL:
	  opPos = diagnoseXPathSimpleOperation("OP_LITERAL", xpath, opPos, indent);
	  break;
	case xpath.OP_VARIABLE:
	  opPos = diagnoseXPathSimpleOperation("OP_VARIABLE", xpath, opPos, indent);
	  break;
	case xpath.OP_GROUP:
	  opPos = diagnoseXPathUnaryOperation("OP_GROUP", xpath, opPos, indent);
	  break;
	case xpath.OP_NUMBERLIT:
	  opPos = diagnoseXPathSimpleOperation("OP_NUMBERLIT", xpath, opPos, indent);
	  break;
	case xpath.OP_ARGUMENT:
	  opPos = diagnoseXPathUnaryOperation("OP_ARGUMENT", xpath, opPos, indent);
	  break;
	case xpath.OP_EXTFUNCTION:
	  {
		System.out.println("OP_EXTFUNCTION {");
		int endExtFunc = opPos+xpath.m_opMap[opPos+1]-1;
		opPos+=2;
		indent(indent+1);
		opPos = diagnoseToken(xpath, opPos);
		System.out.print(":");
		opPos = diagnoseToken(xpath, opPos);
		System.out.println("");
		while(opPos < endExtFunc)
		{
		  indent(indent+1);
		  System.out.println("{");
		  opPos = diagnoseXPath(xpath, opPos, indent+2);
		  indent(indent+1);
		  System.out.println("}");
		}
		indent(indent);
		System.out.println("}");
		if(xpath.m_opMap[opPos] != xpath.ENDOP)
		{
		  System.out.println("ERROR! Could not find ENDOP after OP_LOCATIONPATH");
		}
		opPos++;
	  }
	  break;
	case xpath.OP_FUNCTION:
	  {
		System.out.println("OP_FUNCTION {");
		int endFunc = opPos+xpath.m_opMap[opPos+1]-1;
		opPos+=2;
		indent(indent+1);
		int funcID = xpath.m_opMap[opPos];
		switch(funcID)
		{
		case xpath.FUNC_LAST: System.out.print("FUNC_LAST"); break;
		case xpath.FUNC_POSITION: System.out.print("FUNC_POSITION"); break;
		case xpath.FUNC_COUNT: System.out.print("FUNC_COUNT"); break;
		case xpath.FUNC_ID: System.out.print("FUNC_ID"); break;
		case xpath.FUNC_KEY: System.out.print("FUNC_KEY"); break;
		case xpath.FUNC_DOC: System.out.print("FUNC_DOC"); break;
		case xpath.FUNC_LOCAL_PART: System.out.print("FUNC_LOCAL_PART"); break;
		case xpath.FUNC_NAMESPACE: System.out.print("FUNC_NAMESPACE"); break;
		case xpath.FUNC_QNAME: System.out.print("FUNC_QNAME"); break;
		case xpath.FUNC_GENERATE_ID: System.out.print("FUNC_GENERATE_ID"); break;
		case xpath.FUNC_NOT: System.out.print("FUNC_NOT"); break;
		case xpath.FUNC_TRUE: System.out.print("FUNC_TRUE"); break;
		case xpath.FUNC_FALSE: System.out.print("FUNC_FALSE"); break;
		case xpath.FUNC_BOOLEAN: System.out.print("FUNC_BOOLEAN"); break;
		case xpath.FUNC_LANG: System.out.print("FUNC_LANG"); break;
		case xpath.FUNC_NUMBER: System.out.print("FUNC_NUMBER"); break;
		case xpath.FUNC_FLOOR: System.out.print("FUNC_FLOOR"); break;
		case xpath.FUNC_CEILING: System.out.print("FUNC_CEILING"); break;
		case xpath.FUNC_ROUND: System.out.print("FUNC_ROUND"); break;
		case xpath.FUNC_SUM: System.out.print("FUNC_SUM"); break;
		case xpath.FUNC_STRING: System.out.print("FUNC_STRING"); break;
		case xpath.FUNC_STARTS_WITH: System.out.print("FUNC_STARTS_WITH"); break;
		case xpath.FUNC_CONTAINS: System.out.print("FUNC_CONTAINS"); break;
		case xpath.FUNC_SUBSTRING_BEFORE: System.out.print("FUNC_SUBSTRING_BEFORE"); break;
		case xpath.FUNC_SUBSTRING_AFTER: System.out.print("FUNC_SUBSTRING_AFTER"); break;
		case xpath.FUNC_NORMALIZE: System.out.print("FUNC_NORMALIZE"); break;
		case xpath.FUNC_TRANSLATE: System.out.print("FUNC_TRANSLATE"); break;
		case xpath.FUNC_CONCAT: System.out.print("FUNC_CONCAT"); break;
		case xpath.FUNC_FORMAT_NUMBER: System.out.print("FUNC_FORMAT_NUMBER"); break;
		case xpath.FUNC_SYSTEM_PROPERTY: System.out.print("FUNC_SYSTEM_PROPERTY"); break;
		case xpath.FUNC_EXT_FUNCTION_AVAILABLE: System.out.print("FUNC_EXT_FUNCTION_AVAILABLE"); break;
		case xpath.FUNC_EXT_ELEM_AVAILABLE: System.out.print("FUNC_EXT_ELEM_AVAILABLE"); break;
		case xpath.FUNC_SUBSTRING: System.out.print("FUNC_SUBSTRING"); break;
		case xpath.FUNC_STRING_LENGTH: System.out.print("FUNC_STRING_LENGTH"); break;
		case xpath.FUNC_DOCLOCATION: System.out.print("FUNC_DOCLOCATION"); break;
		}
		opPos++;
		System.out.println("");
		while(opPos < endFunc)
		{
		  indent(indent+1);
		  System.out.println("{");
		  opPos = diagnoseXPath(xpath, opPos, indent+2);
		  indent(indent+1);
		  System.out.println("}");
		}
		indent(indent);
		System.out.println("}");
		if(xpath.m_opMap[opPos] != xpath.ENDOP)
		{
		  System.out.println("ERROR! Could not find ENDOP after OP_LOCATIONPATH");
		}
		opPos++;
	  }
	  break;
	case xpath.OP_LOCATIONPATH:
	  System.out.println("OP_LOCATIONPATH"+" {");
	  int endPath = opPos+xpath.m_opMap[opPos+1]-1;
	  opPos+=2;
	  while(opPos < endPath)
	  {
		opPos = diagnoseXPath(xpath, opPos, indent+1);
	  }
	  indent(indent);
	  System.out.println("}");
	  if(xpath.m_opMap[opPos] != xpath.ENDOP)
	  {
		System.out.println("ERROR! Could not find ENDOP after OP_LOCATIONPATH");
	  }
	  opPos++;
	  break;
	case xpath.OP_PREDICATE:
	  opPos = diagnoseXPathUnaryOperation("OP_PREDICATE", xpath, opPos, indent);
	  if(xpath.m_opMap[opPos] != xpath.ENDOP)
	  {
		System.out.println("ERROR! Could not find ENDOP after OP_LOCATIONPATH");
	  }
	  opPos++;
	  break;
	case xpath.FROM_ANCESTORS:
	  opPos = diagnoseXPathLocationStep("FROM_ANCESTORS", xpath, opPos, 1);
	  break;
	case xpath.FROM_ANCESTORS_OR_SELF:
	  opPos = diagnoseXPathLocationStep("FROM_ANCESTORS_OR_SELF", xpath, opPos, 1);
	  break;
	case xpath.FROM_ATTRIBUTES:
	  opPos = diagnoseXPathLocationStep("FROM_ATTRIBUTES", xpath, opPos, 1);
	  break;
	case xpath.FROM_CHILDREN:
	  opPos = diagnoseXPathLocationStep("FROM_CHILDREN", xpath, opPos, 1);
	  break;
	case xpath.FROM_DESCENDANTS:
	  opPos = diagnoseXPathLocationStep("FROM_DESCENDANTS", xpath, opPos, 1);
	  break;
	case xpath.FROM_DESCENDANTS_OR_SELF:
	  opPos = diagnoseXPathLocationStep("FROM_DESCENDANTS_OR_SELF", xpath, opPos, 1);
	  break;
	case xpath.FROM_FOLLOWING:
	  opPos = diagnoseXPathLocationStep("FROM_FOLLOWING", xpath, opPos, indent);
	  break;
	case xpath.FROM_FOLLOWING_SIBLINGS:
	  opPos = diagnoseXPathLocationStep("FROM_FOLLOWING_SIBLINGS", xpath, opPos, indent);
	  break;
	case xpath.FROM_PARENT:
	  opPos = diagnoseXPathLocationStep("FROM_PARENT", xpath, opPos, indent);
	  break;
	case xpath.FROM_PRECEDING:
	  opPos = diagnoseXPathLocationStep("FROM_PRECEDING", xpath, opPos, indent);
	  break;
	case xpath.FROM_PRECEDING_SIBLINGS:
	  opPos = diagnoseXPathLocationStep("FROM_PRECEDING_SIBLINGS", xpath, opPos, indent);
	  break;
	case xpath.FROM_SELF:
	  opPos = diagnoseXPathLocationStep("FROM_SELF", xpath, opPos, indent);
	  break;
	case xpath.FROM_NAMESPACE:
	  opPos = diagnoseXPathLocationStep("FROM_NAMESPACE", xpath, opPos, indent);
	  break;
	// case xpath.FROM_ATTRIBUTE:
	//	 opPos = diagnoseXPathLocationStep("FROM_ATTRIBUTE", xpath, opPos, indent);
	//	break;
	// case xpath.FROM_DOC:
	//	opPos = diagnoseXPathLocationStep("FROM_DOC", xpath, opPos, indent);
	//	break;
	// case xpath.FROM_DOCREF:
	//	opPos = diagnoseXPathLocationStep("FROM_DOCREF", xpath, opPos, indent);
	//	break;
	// case xpath.FROM_ID:
	//	opPos = diagnoseXPathLocationStep("FROM_ID", xpath, opPos, indent);
	//	break;
	// case xpath.FROM_IDREF:
	//	opPos = diagnoseXPathLocationStep("FROM_IDREF", xpath, opPos, indent);
	//	break;
	case xpath.FROM_ROOT:
	  opPos = diagnoseXPathLocationStep("FROM_ROOT", xpath, opPos, indent);
	  break;
	case xpath.NODETYPE_COMMENT:
	  System.out.println("{NODETYPE_COMMENT}");
	  opPos++;
	  break;
	case xpath.NODETYPE_TEXT:
	  System.out.println("{NODETYPE_TEXT}");
	  opPos++;
	  break;
	case xpath.NODETYPE_PI:
	  int piLen = xpath.m_opMap[opPos-1];
	  System.out.println("{NODETYPE_PI ");
	  opPos++;
	  if(piLen > 3)
	  {
		opPos = diagnoseToken(xpath, opPos);
	  }
	  break;
	case xpath.NODETYPE_NODE:
	  System.out.println("{NODETYPE_NODE}");
	  opPos++;
	  break;
	case xpath.NODETYPE_ROOT:
	  System.out.println("{NODETYPE_ROOT}");
	  opPos++;
	  break;
	case xpath.NODETYPE_ANYELEMENT:
	  System.out.println("{NODETYPE_ANYELEMENT}");
	  opPos++;
	  break;
	case xpath.NODENAME:
	  System.out.print("{NODENAME ");
	  opPos++;
	  if(xpath.m_opMap[opPos] < 0)
	  { 
		System.out.print("{EMPTY}");
		opPos++;
	  }
	  else
	  {
		opPos = diagnoseToken(xpath, opPos);
	  }
	  System.out.print(":");
	  opPos = diagnoseToken(xpath, opPos);
	  break;
	default:
	  System.out.println("ERROR! Unknown op code: "+xpath.m_opMap[opPos]);
	}
	return opPos;
  }
  
  static void indent(int amount)
  {
	int n = amount * 3;
	for(int i = 0;	i < n; i ++)
	{
	  System.out.print(" ");
	}
  }
  
  private static String m_opLabel	  = "[";
  private static String m_lenLabel	  = "[";
  private static String m_arglenLabel = "[";
  private static String m_noLabel	  = "[";
  private static String m_nTestLabel  = "[";
  private static String m_open = "[";
  private static String m_close = "]";

  public static void diagnoseXPathString2( String str )    
  {    
	XPathSupport callbacks = new XPathSupportDefault();
	XPathProcessorImpl processor = new XPathProcessorImpl(callbacks);
	XPath xpath = new XPath(callbacks);
	processor.initXPath(xpath, str, null);
	processor.diagnoseXPath2(xpath, 0, 0);
  }

  public static void diagnoseXPathString3( String str )    
  {    
	XPathSupport callbacks = new XPathSupportDefault();
	XPathProcessorImpl processor = new XPathProcessorImpl(callbacks);
	XPath xpath = new XPath(callbacks);
	processor.initXPath(xpath, str, null);
	int len = xpath.m_opMap[xpath.MAPINDEX_LENGTH];
	for(int i = 0; i < len; i++)
	{
	  System.out.println("["+xpath.m_opMap[i]+"]");
	}
  }

  private static void diagnoseNodeTest2(int opPos, String op)
  {
	System.out.print(m_nTestLabel+op+m_close);
  }

  private static void diagnoseOpNoLable2(int opPos, String op)
  {
	System.out.println(m_noLabel+op+m_close);
  }

  private static void diagnoseOpOnly2(int opPos, String op)
  {
	System.out.println(m_opLabel+op+m_close);
  }

  private static void diagnoseOp2(String op, XPath xpath, int opPos)
  {
	System.out.print(m_opLabel+op+m_close);
	int opLen = xpath.m_opMap[opPos+xpath.MAPINDEX_LENGTH];
	System.out.println(m_open+opLen+m_close);
  }

  private static void diagnoseOp2SameLine(String op, XPath xpath, int opPos)
  {
	System.out.print(m_opLabel+op+m_close);
	int opLen = xpath.m_opMap[opPos+xpath.MAPINDEX_LENGTH];
	System.out.print(m_open+opLen+m_close);
  }

  static int diagnoseXPathBinaryOperation2(String op, XPath xpath, int opPos, int indent)
  {
	diagnoseOp2(op, xpath, opPos);
	opPos+=2;
	
	opPos = diagnoseXPath2(xpath, opPos, indent+1);
	
	opPos = diagnoseXPath2(xpath, opPos, indent+1);
	
	return opPos;
  }

  static int diagnoseXPathUnaryOperation2(String op, XPath xpath, int opPos, int indent)
  {
	diagnoseOp2(op, xpath, opPos);
	opPos+=2;
	opPos = diagnoseXPath2(xpath, opPos, indent+1);
	return opPos;
  }

  static int diagnoseXPathMultiOperation2(String op, int multiOp, XPath xpath, int opPos, int indent)
  {
	diagnoseOp2(op, xpath, opPos);
	opPos+=2;
	while(xpath.m_opMap[opPos] == multiOp)
	{
	  opPos = diagnoseXPath2(xpath, opPos, indent+2);
	}
	return opPos;
  }
  
  static int diagnoseToken2(XPath xpath, int opPos)
  {
	int tokenPos = xpath.m_opMap[opPos];
	String token = (tokenPos >= 0) ? xpath.m_tokenQueue[tokenPos].toString() : 
									 (tokenPos == xpath.ELEMWILDCARD) ?
									 "*" : (tokenPos == xpath.EMPTY) ?
										   "EMPTY" : "UNKNOWN";
	System.out.println(m_noLabel+token+m_close);
	return opPos+1;
  }

  static int diagnoseToken2SameLine(XPath xpath, int opPos)
  {
	System.out.print(m_noLabel+xpath.m_tokenQueue[xpath.m_opMap[opPos]]+m_close);
	return opPos+1;
  }

  static int diagnoseXPathSimpleOperation2(String op, XPath xpath, int opPos, int indent)
  {
	diagnoseOp2SameLine(op, xpath, opPos);
	opPos+=2;
	opPos = diagnoseToken2(xpath, opPos);
	return opPos;
  }

  static int diagnoseXPathLocationStep2(String op, XPath xpath, int opPos, int indent)
  {
	int opLen = xpath.m_opMap[opPos+xpath.MAPINDEX_LENGTH];
	int stepLen = xpath.m_opMap[opPos+xpath.MAPINDEX_LENGTH+1];
	System.out.print(m_opLabel+op+m_close);
	System.out.print(m_open+opLen+m_close);
	System.out.print(m_open+stepLen+m_close);
	opPos+=3;
	if(stepLen > 3)
	{
	  opPos = diagnoseXPath2(xpath, opPos, 0);
	}
	return opPos;
  }
  
  static int diagnoseXPath2(XPath xpath, int opPos, int indent)
  {
	indent(indent);
	switch(xpath.m_opMap[opPos])
	{
	case xpath.OP_XPATH:
	  opPos = diagnoseXPathUnaryOperation2("OP_XPATH", xpath, opPos, indent);
	  break;
	case xpath.EMPTY:
	  diagnoseOpOnly2(opPos, "EMPTY");
	  opPos++;
	  break;
	case xpath.OP_OR:
	  opPos = diagnoseXPathBinaryOperation2("OP_OR", xpath, opPos, indent);
	  break;
	case xpath.OP_AND:
	  opPos = diagnoseXPathBinaryOperation2("OP_AND", xpath, opPos, indent);
	  break;
	case xpath.OP_NOTEQUALS:
	  opPos = diagnoseXPathBinaryOperation2("OP_NOTEQUALS", xpath, opPos, indent);
	  break;
	case xpath.OP_EQUALS:
	  opPos = diagnoseXPathBinaryOperation2("OP_EQUALS", xpath, opPos, indent);
	  break;
	case xpath.OP_LTE:
	  opPos = diagnoseXPathBinaryOperation2("OP_LTE", xpath, opPos, indent);
	  break;
	case xpath.OP_LT:
	  opPos = diagnoseXPathBinaryOperation2("OP_LT", xpath, opPos, indent);
	  break;
	case xpath.OP_GTE:
	  opPos = diagnoseXPathBinaryOperation2("OP_GTE", xpath, opPos, indent);
	  break;
	case xpath.OP_GT:
	  opPos = diagnoseXPathBinaryOperation2("OP_GT", xpath, opPos, indent);
	  break;
	case xpath.OP_PLUS:
	  opPos = diagnoseXPathBinaryOperation2("OP_PLUS", xpath, opPos, indent);
	  break;
	case xpath.OP_MINUS:
	  opPos = diagnoseXPathBinaryOperation2("OP_MINUS", xpath, opPos, indent);
	  break;
	case xpath.OP_MULT:
	  opPos = diagnoseXPathBinaryOperation2("OP_MULT", xpath, opPos, indent);
	  break;
	case xpath.OP_DIV:
	  opPos = diagnoseXPathBinaryOperation2("OP_DIV", xpath, opPos, indent);
	  break;
	case xpath.OP_MOD:
	  opPos = diagnoseXPathBinaryOperation2("OP_MOD", xpath, opPos, indent);
	  break;
	case xpath.OP_QUO:
	  opPos = diagnoseXPathBinaryOperation2("OP_QUO", xpath, opPos, indent);
	  break;
	case xpath.OP_NEG:
	  opPos = diagnoseXPathUnaryOperation2("OP_NEG", xpath, opPos, indent);
	  break;
	case xpath.OP_STRING:
	  opPos = diagnoseXPathUnaryOperation2("OP_STRING", xpath, opPos, indent);
	  break;
	case xpath.OP_BOOL:
	  opPos = diagnoseXPathUnaryOperation2("OP_BOOL", xpath, opPos, indent);
	  break;
	case xpath.OP_NUMBER:
	  opPos = diagnoseXPathUnaryOperation2("OP_NUMBER", xpath, opPos, indent);
	  break;
	case xpath.OP_UNION:
	  opPos = diagnoseXPathMultiOperation2("OP_UNION", xpath.OP_LOCATIONPATH, xpath, opPos, indent);
	  break;
	case xpath.OP_LITERAL:
	  opPos = diagnoseXPathSimpleOperation2("OP_LITERAL", xpath, opPos, indent);
	  break;
	case xpath.OP_VARIABLE:
	  opPos = diagnoseXPathSimpleOperation2("OP_VARIABLE", xpath, opPos, indent);
	  break;
	case xpath.OP_GROUP:
	  opPos = diagnoseXPathUnaryOperation2("OP_GROUP", xpath, opPos, indent);
	  break;
	case xpath.OP_NUMBERLIT:
	  opPos = diagnoseXPathSimpleOperation2("OP_NUMBERLIT", xpath, opPos, indent);
	  break;
	case xpath.OP_ARGUMENT:
	  opPos = diagnoseXPathUnaryOperation2("OP_ARGUMENT", xpath, opPos, indent);
	  break;
	case xpath.OP_EXTFUNCTION:
	  {
		diagnoseOp2SameLine("OP_EXTFUNCTION", xpath, opPos);
		int endExtFunc = opPos+xpath.m_opMap[opPos+1]-1;
		opPos+=2;
		opPos = diagnoseToken2SameLine(xpath, opPos);
		opPos = diagnoseToken2(xpath, opPos);
		while(opPos < endExtFunc)
		{
		  opPos = diagnoseXPath2(xpath, opPos, indent+2);
		}
		if(xpath.m_opMap[opPos] != xpath.ENDOP)
		{
		  System.out.println("ERROR! Could not find ENDOP after OP_LOCATIONPATH");
		}
		indent(indent+1);
		diagnoseOpOnly2(opPos, "ENDOP");
		opPos++;
	  }
	  break;
	case xpath.OP_FUNCTION:
	  {
		diagnoseOp2SameLine("OP_FUNCTION", xpath, opPos);
		int endFunc = opPos+xpath.m_opMap[opPos+1]-1;
		opPos+=2;
		int funcID = xpath.m_opMap[opPos];
		switch(funcID)
		{
		case xpath.FUNC_LAST: diagnoseOpNoLable2(opPos, "FUNC_LAST"); break;
		case xpath.FUNC_POSITION: diagnoseOpNoLable2(opPos, "FUNC_POSITION"); break;
		case xpath.FUNC_COUNT: diagnoseOpNoLable2(opPos, "FUNC_COUNT"); break;
		case xpath.FUNC_ID: diagnoseOpNoLable2(opPos, "FUNC_ID"); break;
		case xpath.FUNC_KEY: diagnoseOpNoLable2(opPos, "FUNC_KEY"); break;
		case xpath.FUNC_DOC: diagnoseOpNoLable2(opPos, "FUNC_DOC"); break;
		case xpath.FUNC_LOCAL_PART: diagnoseOpNoLable2(opPos, "FUNC_LOCAL_PART"); break;
		case xpath.FUNC_NAMESPACE: diagnoseOpNoLable2(opPos, "FUNC_NAMESPACE"); break;
		case xpath.FUNC_QNAME: diagnoseOpNoLable2(opPos, "FUNC_QNAME"); break;
		case xpath.FUNC_GENERATE_ID: diagnoseOpNoLable2(opPos, "FUNC_GENERATE_ID"); break;
		case xpath.FUNC_NOT: diagnoseOpNoLable2(opPos, "FUNC_NOT"); break;
		case xpath.FUNC_TRUE: diagnoseOpNoLable2(opPos, "FUNC_TRUE"); break;
		case xpath.FUNC_FALSE: diagnoseOpNoLable2(opPos, "FUNC_FALSE"); break;
		case xpath.FUNC_BOOLEAN: diagnoseOpNoLable2(opPos, "FUNC_BOOLEAN"); break;
		case xpath.FUNC_LANG: diagnoseOpNoLable2(opPos, "FUNC_LANG"); break;
		case xpath.FUNC_NUMBER: diagnoseOpNoLable2(opPos, "FUNC_NUMBER"); break;
		case xpath.FUNC_FLOOR: diagnoseOpNoLable2(opPos, "FUNC_FLOOR"); break;
		case xpath.FUNC_CEILING: diagnoseOpNoLable2(opPos, "FUNC_CEILING"); break;
		case xpath.FUNC_ROUND: diagnoseOpNoLable2(opPos, "FUNC_ROUND"); break;
		case xpath.FUNC_SUM: diagnoseOpNoLable2(opPos, "FUNC_SUM"); break;
		case xpath.FUNC_STRING: diagnoseOpNoLable2(opPos, "FUNC_STRING"); break;
		case xpath.FUNC_STARTS_WITH: diagnoseOpNoLable2(opPos, "FUNC_STARTS_WITH"); break;
		case xpath.FUNC_CONTAINS: diagnoseOpNoLable2(opPos, "FUNC_CONTAINS"); break;
		case xpath.FUNC_SUBSTRING_BEFORE: diagnoseOpNoLable2(opPos, "FUNC_SUBSTRING_BEFORE"); break;
		case xpath.FUNC_SUBSTRING_AFTER: diagnoseOpNoLable2(opPos, "FUNC_SUBSTRING_AFTER"); break;
		case xpath.FUNC_NORMALIZE: diagnoseOpNoLable2(opPos, "FUNC_NORMALIZE"); break;
		case xpath.FUNC_TRANSLATE: diagnoseOpNoLable2(opPos, "FUNC_TRANSLATE"); break;
		case xpath.FUNC_CONCAT: diagnoseOpNoLable2(opPos, "FUNC_CONCAT"); break;
		case xpath.FUNC_FORMAT_NUMBER: diagnoseOpNoLable2(opPos, "FUNC_FORMAT_NUMBER"); break;
		case xpath.FUNC_SYSTEM_PROPERTY: diagnoseOpNoLable2(opPos, "FUNC_SYSTEM_PROPERTY"); break;
		case xpath.FUNC_EXT_FUNCTION_AVAILABLE: diagnoseOpNoLable2(opPos, "FUNC_EXT_FUNCTION_AVAILABLE"); break;
		case xpath.FUNC_EXT_ELEM_AVAILABLE: diagnoseOpNoLable2(opPos, "FUNC_EXT_ELEM_AVAILABLE"); break;
		case xpath.FUNC_SUBSTRING: diagnoseOpNoLable2(opPos, "FUNC_SUBSTRING"); break;
		case xpath.FUNC_STRING_LENGTH: diagnoseOpNoLable2(opPos, "FUNC_STRING_LENGTH"); break;
		case xpath.FUNC_DOCLOCATION: diagnoseOpNoLable2(opPos, "FUNC_DOCLOCATION"); break;
		}
		opPos++;
		while(opPos < endFunc)
		{
		  // indent(indent+1);
		  opPos = diagnoseXPath2(xpath, opPos, indent+2);
		}
		indent(indent);
		if(xpath.m_opMap[opPos] != xpath.ENDOP)
		{
		  System.out.println("ERROR! Could not find ENDOP after OP_LOCATIONPATH");
		}
		indent(indent+1);
		diagnoseOpOnly2(opPos, "ENDOP");
		opPos++;
	  }
	  break;
	case xpath.OP_LOCATIONPATH:
	  diagnoseOp2("OP_LOCATIONPATH", xpath, opPos);
	  int endPath = opPos+xpath.m_opMap[opPos+1]-1;
	  opPos+=2;
	  while(opPos < endPath)
	  {
		opPos = diagnoseXPath2(xpath, opPos, indent+1);
	  }
	  if(xpath.m_opMap[opPos] != xpath.ENDOP)
	  {
		System.out.println("ERROR! Could not find ENDOP after OP_LOCATIONPATH");
	  }
	  indent(indent+1);
	  diagnoseOpOnly2(opPos, "ENDOP");
	  opPos++;
	  break;
	case xpath.OP_PREDICATE:
	  indent(1);
	  opPos = diagnoseXPathUnaryOperation2("OP_PREDICATE", xpath, opPos, indent+1);
	  if(xpath.m_opMap[opPos] != xpath.ENDOP)
	  {
		System.out.println("ERROR! Could not find ENDOP after OP_LOCATIONPATH");
	  }
	  indent(indent+2);
	  diagnoseOpOnly2(opPos, "ENDOP");
	  opPos++;
	  break;
	case xpath.FROM_ANCESTORS:
	  opPos = diagnoseXPathLocationStep2("FROM_ANCESTORS", xpath, opPos, 1);
	  break;
	case xpath.FROM_ANCESTORS_OR_SELF:
	  opPos = diagnoseXPathLocationStep2("FROM_ANCESTORS_OR_SELF", xpath, opPos, 1);
	  break;
	case xpath.FROM_ATTRIBUTES:
	  opPos = diagnoseXPathLocationStep2("FROM_ATTRIBUTES", xpath, opPos, 1);
	  break;
	case xpath.FROM_CHILDREN:
	  opPos = diagnoseXPathLocationStep2("FROM_CHILDREN", xpath, opPos, 1);
	  break;
	case xpath.FROM_DESCENDANTS:
	  opPos = diagnoseXPathLocationStep2("FROM_DESCENDANTS", xpath, opPos, 1);
	  break;
	case xpath.FROM_DESCENDANTS_OR_SELF:
	  opPos = diagnoseXPathLocationStep2("FROM_DESCENDANTS_OR_SELF", xpath, opPos, 1);
	  break;
	case xpath.FROM_FOLLOWING:
	  opPos = diagnoseXPathLocationStep2("FROM_FOLLOWING", xpath, opPos, indent);
	  break;
	case xpath.FROM_FOLLOWING_SIBLINGS:
	  opPos = diagnoseXPathLocationStep2("FROM_FOLLOWING_SIBLINGS", xpath, opPos, indent);
	  break;
	case xpath.FROM_PARENT:
	  opPos = diagnoseXPathLocationStep2("FROM_PARENT", xpath, opPos, indent);
	  break;
	case xpath.FROM_PRECEDING:
	  opPos = diagnoseXPathLocationStep2("FROM_PRECEDING", xpath, opPos, indent);
	  break;
	case xpath.FROM_PRECEDING_SIBLINGS:
	  opPos = diagnoseXPathLocationStep2("FROM_PRECEDING_SIBLINGS", xpath, opPos, indent);
	  break;
	case xpath.FROM_SELF:
	  opPos = diagnoseXPathLocationStep2("FROM_SELF", xpath, opPos, indent);
	  break;
	case xpath.FROM_NAMESPACE:
	  opPos = diagnoseXPathLocationStep2("FROM_NAMESPACE", xpath, opPos, indent);
	  break;
	// case xpath.FROM_ATTRIBUTE:
	//	 opPos = diagnoseXPathLocationStep("FROM_ATTRIBUTE", xpath, opPos, indent);
	//	break;
	// case xpath.FROM_DOC:
	//	opPos = diagnoseXPathLocationStep("FROM_DOC", xpath, opPos, indent);
	//	break;
	// case xpath.FROM_DOCREF:
	//	opPos = diagnoseXPathLocationStep("FROM_DOCREF", xpath, opPos, indent);
	//	break;
	// case xpath.FROM_ID:
	//	opPos = diagnoseXPathLocationStep("FROM_ID", xpath, opPos, indent);
	//	break;
	// case xpath.FROM_IDREF:
	//	opPos = diagnoseXPathLocationStep("FROM_IDREF", xpath, opPos, indent);
	//	break;
	case xpath.FROM_ROOT:
	  opPos = diagnoseXPathLocationStep2("FROM_ROOT", xpath, opPos, indent);
	  // opPos++;
	  break;
	case xpath.NODETYPE_COMMENT:
	  diagnoseNodeTest2(opPos, "NODETYPE_COMMENT");
	  System.out.println();
	  opPos++;
	  break;
	case xpath.NODETYPE_TEXT:
	  diagnoseNodeTest2(opPos, "NODETYPE_TEXT");
	  System.out.println();
	  opPos++;
	  break;
	case xpath.NODETYPE_PI:
	  int piLen = xpath.m_opMap[opPos-1];
	  diagnoseNodeTest2(opPos, "NODETYPE_PI");
	  opPos++;
	  if(piLen > 3)
	  {
		opPos = diagnoseToken(xpath, opPos);
	  }
	  break;
	case xpath.NODETYPE_NODE:
	  diagnoseNodeTest2(opPos, "NODETYPE_NODE");
	  System.out.println();
	  opPos++;
	  break;
	case xpath.NODETYPE_ROOT:
	  diagnoseNodeTest2(opPos, "NODETYPE_ROOT");
	  System.out.println();
	  opPos++;
	  break;
	case xpath.NODETYPE_ANYELEMENT:
	  diagnoseNodeTest2(opPos, "NODETYPE_ANYELEMENT");
	  System.out.println();
	  opPos++;
	  break;
	case xpath.NODENAME:
	  diagnoseNodeTest2(opPos, "NODENAME");
	  opPos++;
	  if(xpath.m_opMap[opPos] < 0)
	  { 
		System.out.print(m_noLabel+"EMPTY"+m_close);
		opPos++;
	  }
	  else
	  {
		opPos = diagnoseToken2(xpath, opPos);
	  }
	  opPos = diagnoseToken2(xpath, opPos);
	  break;
	default:
	  System.out.println("ERROR! Unknown op code: "+xpath.m_opMap[opPos]);
	}
	return opPos;
  }


#endif



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
