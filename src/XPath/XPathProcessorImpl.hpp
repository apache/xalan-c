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
#if !defined(XPATHPROCESSORIMPL_HEADER_GUARD_1357924680)
#define XPATHPROCESSORIMPL_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



#include <set>
#include <string>
#include <vector>



#include <dom/DOMString.hpp>
#include <dom/DOM_Node.hpp>



// Base class header file...
#include <XPath/XPathProcessor.hpp>



#include <XPath/XPath.hpp>



/**
 * The XPathProcessorImpl class responsibilities include tokenizing and
 * parsing the XPath expression, and acting as a general interface to XPaths.
 */
class XALAN_XPATH_EXPORT XPathProcessorImpl : public XPathProcessor
{
public:

#if defined(XALAN_NO_NAMESPACES)
#	define XALAN_STD
#else
#	define XALAN_STD std::
#endif
	typedef XALAN_STD map<DOMString, int>						KeywordsMapType;
	typedef XALAN_STD map<DOMString, XPathExpression::eOpCodes>	FunctionNameMapType;
	typedef XALAN_STD map<DOMString, XPathExpression::eOpCodes>	AxisNamesMapType;
	typedef XALAN_STD map<DOMString, XPathExpression::eOpCodes>	NodeTypesMapType;
	typedef XALAN_STD vector<DOMString> DOMStringVectorType;
#undef XALAN_STD
	

	explicit
	XPathProcessorImpl();

	virtual
	~XPathProcessorImpl();


	// These are inherited from XPathProcessor...

	virtual void
	initXPath(
			XPath&					pathObj,
			const DOMString&		expression,
			const PrefixResolver&	prefixResolver,
			XObjectFactory&			xobjectFactory,
			const XPathEnvSupport&	envSupport);

	virtual void
	initMatchPattern(
			XPath&					pathObj,
			const DOMString&		expression,
			const PrefixResolver&	prefixResolver,
			XObjectFactory&			xobjectFactory,
			const XPathEnvSupport&	envSupport);

private:

	/**
	 * Walk through the expression and build a token queue, and a map of the
	 * top-level elements.
	 *
	 * @param pat XSLT Expression.
	 * @param targetStrings Vector to hold Strings, may be null.
	 */
	void
	tokenize(
			const DOMString&			pat,
			DOMStringVectorType*		targetStrings = 0);
  
	/**
	 * Record the current position on the token queue as long as this is a
	 * top-level element.  Must be called before the next token is added to the
	 * m_tokenQueue.
	 */
	bool
	mapPatternElemPos(
			int		nesting,
			bool	isStart,
			bool	isAttrName) const;

	/**
	 * Record the correct token string in the passed vector.
	 */
	void
	recordTokenString(DOMStringVectorType&	targetStrings);

	void
	addToTokenQueue(const DOMString&	s) const;

	/**
	 * When a separator token is found, see if there's a element name or the
	 * like to map.
	 */
	int
	mapNSTokens(
			const DOMString&	pat,
			int					startSubstring,
			int					posOfNSSep,
			int					posOfScan) const;

	/**
	 * Given a map pos, return the corresponding token queue pos.
	 */
	int
	getTokenQueuePosFromMap(int		i) const;

	/**
	 * This will return the index above the passed index that is the target
	 * element, i.e. it holds a value >= TARGETEXTRA. If there is no next
	 * target, it will return -1. Pass -1 in to start testing from zero.
	 */
	int
	getNextTargetIndexInMap(int		i) const;
  
	/**
	 * This will return the normalized index into the pattern 
	 * map above the passed index, or -1 if it is the last pattern.
	 */
	int
	getNextIndexInMap(int	i) const;
  
	/**
	 * This will return the index above the passed index that 
	 * is the start if the next subpattern, or -1 if there is none.
	 * If there is no next target, it will return -1.
	 * Pass -1 in to start testing from zero.
	 */
	int
	getNextSubpatternStartIndexInMap(int	i) const;

	/**
	 * This will return the next index from the passed index,
	 * or -1 if it the passed index is the last index of the 
	 * subpattern.
	 */
	int
	getNextPatternPos(int	i) const;

	/**
	 * This will return the previous index from the passed index,
	 * or -1 if it the passed index is the first index of the 
	 * subpattern.
	 */
	int
	getPrevMapIndex(int		i) const;

	/**
	 * Check if m_token==s. If m_token is null, this won't throw
	 * an exception, instead it just returns false (or true
	 * if s is also null).
	 */
	bool
	tokenIs(const char*		s) const;

	/**
	 * Check if m_token==s. If m_token is null, this won't throw
	 * an exception, instead it just returns false (or true
	 * if s is also null).
	 */
	bool
	tokenIs(char	c) const;

	/**
	 * Lookahead of the current token in order to 
	 * make a branching decision.
	 * @param s the string to compare it to.
	 * @param n number of tokens to lookahead.  Must be 
	 * greater than 1.
	 */
	bool
	lookahead(
			char	c,
			int		n) const;

	/**
	 * Lookahead of the current token in order to 
	 * make a branching decision.
	 * @param s the string to compare it to.
	 * @param n number of tokens to lookahead.  Must be 
	 * greater than 1.
	 */
	bool
	lookahead(
			const char*		s,
			int				n) const;

	/**
	 * Lookbehind the first character of the current token in order to 
	 * make a branching decision.
	 * @param c the character to compare it to.
	 * @param n number of tokens to lookbehind.  Must be 
	 * greater than 1.  Note that the lookbehind terminates 
	 * at either the beginning of the string or on a '|' 
	 * character.  Because of this, this method should only
	 * be used for pattern matching.
	 */
	bool
	lookbehind( 
			char	c,
			int		n) const;
 
	/**
	 * look behind the current token in order to 
	 * see if there is a useable token.
	 * @param n number of tokens to lookahead.  Must be 
	 * greater than 1.  Note that the lookbehind terminates 
	 * at either the beginning of the string or on a '|' 
	 * character.  Because of this, this method should only
	 * be used for pattern matching.
	 * @return true if lookbehind has a token, false otherwise.
	 */
	bool
	lookbehindHasToken(int	n) const;

	/**
	 * Retrieve the next token from the command and
	 * store it in m_token string.
	 */
	void
	nextToken();

	/**
	 * Retrieve the next token from the command and
	 * store it in m_token string.
	 */
	DOMString
	getTokenRelative(int	theOffset) const;

	/**
	 * Retrieve the next token from the command and
	 * store it in m_token string.
	 */
	void
	prevToken();

	/**
	 * Reset token queue mark and m_token to a 
	 * given position.
	 */
	void
	resetTokenMark(int	mark);
  
	/**
	 * Consume an expected token, throwing an exception if it 
	 * isn't there.
	 */
	void
	consumeExpected(const char*		expected);

	/**
	 * Consume an expected token, throwing an exception if it 
	 * isn't there.
	 */
	void
	consumeExpected(char	expected);

	/**
	 * Warn the user of an problem.
	 */
	void
	warn(
			const DOMString&	msg,
			const DOM_Node&		sourceNode = DOM_Node()) const;

	/**
	 * Tell the user of an error, and probably throw an 
	 * exception.
	 */
	void
	error(
			const DOMString&	msg,
			const DOM_Node&		sourceNode = DOM_Node()) const;

#if 0
  
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


	/**
	 * Given a string, return the corresponding token.
	 */
	int
	getKeywordToken(const DOMString&	key) const;

	/**
	 * Given a string, return the corresponding token.
	 */
	int
	getFunctionToken(const DOMString&	key) const;

	/**
	 * 
	 * --------------------------------------------------------------------------------
	Expr  ::=  OrExpr   
	 * --------------------------------------------------------------------------------
	 */
	void
	Expr();


	/**
	 * 
	 * --------------------------------------------------------------------------------
	 OrExpr  ::=	AndExpr   
	 | OrExpr 'or' AndExpr   
	 * --------------------------------------------------------------------------------
	 */
	void
	OrExpr();

	/**
	 * 
	 * --------------------------------------------------------------------------------
	 AndExpr	::=  EqualityExpr	
	 | AndExpr 'and' EqualityExpr   
	 * --------------------------------------------------------------------------------
	 */
	void
	AndExpr() ;
  
	/**
	 * XXXX.
	 * @returns an Object which is either a String, a Number, a Boolean, or a vector 
	 * of nodes.
	 * --------------------------------------------------------------------------------
	 EqualityExpr  ::=  RelationalExpr   
	 | EqualityExpr '=' RelationalExpr   
	 * --------------------------------------------------------------------------------
	 */
	int
	EqualityExpr(int	opCodePos = -1);

	/**
	 * XXXX.
	 * @returns an Object which is either a String, a Number, a Boolean, or a vector 
	 * of nodes.
	 * --------------------------------------------------------------------------------
	 RelationalExpr  ::=	AdditiveExpr   
	 | RelationalExpr '<' AdditiveExpr   
	 | RelationalExpr '>' AdditiveExpr   
	 | RelationalExpr '<=' AdditiveExpr	
	 | RelationalExpr '>=' AdditiveExpr	
	 * --------------------------------------------------------------------------------
	 */
	int
	RelationalExpr(int	opCodePos = -1);

	/**
	 * XXXX.
	 * @returns an Object which is either a String, a Number, a Boolean, or a vector 
	 * of nodes.
	 * --------------------------------------------------------------------------------
	 AdditiveExpr  ::=  MultiplicativeExpr   
	 | AdditiveExpr '+' MultiplicativeExpr   
	 | AdditiveExpr '-' MultiplicativeExpr   
	 * --------------------------------------------------------------------------------
	 */
	int
	AdditiveExpr(int	opCodePos = -1);

	/**
	 * XXXX.
	 * @returns an Object which is either a String, a Number, a Boolean, or a vector 
	 * of nodes.
	 * --------------------------------------------------------------------------------
	 MultiplicativeExpr  ::=	UnaryExpr	
	 | MultiplicativeExpr MultiplyOperator UnaryExpr	 
	 | MultiplicativeExpr 'div' UnaryExpr   
	 | MultiplicativeExpr 'mod' UnaryExpr   
	 | MultiplicativeExpr 'quo' UnaryExpr   
	 * --------------------------------------------------------------------------------
	 */
	int
	MultiplicativeExpr(int	opCodePos = -1);

	/**
	 * XXXX.
	 * @returns an Object which is either a String, a Number, a Boolean, or a vector 
	 * of nodes.
	 * --------------------------------------------------------------------------------
	 UnaryExpr  ::=  UnionExpr   
	 | '-' UnaryExpr	 
	 * --------------------------------------------------------------------------------
	 */
	void
	UnaryExpr();
  
	/**
	 * --------------------------------------------------------------------------------
	 StringExpr  ::=	Expr   
	 * --------------------------------------------------------------------------------
	 */
	void
	StringExpr();

	/**
	 * 
	 * --------------------------------------------------------------------------------
	 BooleanExpr  ::=	Expr   
	 * --------------------------------------------------------------------------------
	 */
	void
	BooleanExpr();

	/**
	 * 
	 * --------------------------------------------------------------------------------
	 NumberExpr  ::=	Expr   
	 * --------------------------------------------------------------------------------
	 */
	void
	NumberExpr();
  
	/**
	 * The context of the right hand side expressions is the context of the 
	 * left hand side expression. The results of the right hand side expressions 
	 * are node sets. The result of the left hand side UnionExpr is the union 
	 * of the results of the right hand side expressions.
	 * 
	 * --------------------------------------------------------------------------------
	 UnionExpr	::=    PathExpr   
	 | UnionExpr '|' PathExpr 	
	 * --------------------------------------------------------------------------------
	 */
	void
	UnionExpr();

	/**
	 * 
	 * --------------------------------------------------------------------------------
	 PathExpr  ::=  LocationPath	 
	 | FilterExpr   
	 | FilterExpr '/' RelativeLocationPath   
	 | FilterExpr '//' RelativeLocationPath	
	 * --------------------------------------------------------------------------------
	 * @exception XSLProcessorException thrown if the active ProblemListener and XMLParserLiaison decide 
	 * the error condition is severe enough to halt processing.
	 */
	void
	PathExpr();

	/**
	 * 
	 * --------------------------------------------------------------------------------
	 FilterExpr  ::=	PrimaryExpr   
	 | FilterExpr Predicate	
	 * --------------------------------------------------------------------------------
	 * @exception XSLProcessorException thrown if the active ProblemListener and XMLParserLiaison decide 
	 * the error condition is severe enough to halt processing.
	 */
	void
	FilterExpr();
  
	/**
	 * --------------------------------------------------------------------------------
	 PrimaryExpr	::=  VariableReference	 
	 | '(' Expr ')'	
	 | Literal   
	 | Number   
	 | FunctionCall	
	 * --------------------------------------------------------------------------------
	 */
	void
	PrimaryExpr();


	/**
	 * --------------------------------------------------------------------------------
	 Argument    ::=	  Expr	   
	 * --------------------------------------------------------------------------------
	 */
	void
	Argument();
  
	/**
	 * --------------------------------------------------------------------------------
	 FunctionCall    ::=	  FunctionName '(' ( Argument ( ',' Argument)*)? ')'	
	 * --------------------------------------------------------------------------------
	 */
	void
	FunctionCall();

	/**
	 * --------------------------------------------------------------------------------
	 LocationPath ::= RelativeLocationPath 
	 | AbsoluteLocationPath 
	 * --------------------------------------------------------------------------------
	 */
	void
	LocationPath();
  
	/**
	 * --------------------------------------------------------------------------------
	 RelativeLocationPath ::= Step 
	 | RelativeLocationPath '/' Step 
	 | AbbreviatedRelativeLocationPath 
	 * --------------------------------------------------------------------------------
	 */
	void
	RelativeLocationPath();
  
	/**
	 * --------------------------------------------------------------------------------
	 Step    ::=	  Basis Predicate*	
	 | AbbreviatedStep
	 */
	void
	Step();
  
	/**
	 * --------------------------------------------------------------------------------
	 Basis	::=    AxisName '::' NodeTest	
	 | AbbreviatedBasis  
	 */
	void
	Basis();
  
	/**
	 * --------------------------------------------------------------------------------
	 Basis	::=    AxisName '::' NodeTest	
	 | AbbreviatedBasis  
	 */
	void
	AxisName();
  
	/**
	 * --------------------------------------------------------------------------------
	 NodeTest    ::=	  WildcardName	 
	 | NodeType '(' ')'  
	 | 'processing-instruction' '(' Literal ')' 
	 */
	void
	NodeTest();

	/**
	 * --------------------------------------------------------------------------------
	 Predicate ::= '[' PredicateExpr ']' 
	 * --------------------------------------------------------------------------------
	 */
	void
	Predicate();

	/**
	 *--------------------------------------------------------------------------------
	 PredicateExpr ::= Expr 
	 *--------------------------------------------------------------------------------
	 */
	void
	PredicateExpr();
  
	/**
	 * QName ::=	(Prefix ':')? LocalPart 
	 * Prefix ::=  NCName 
	 * LocalPart ::=	NCName 
	 */
	void
	QName();

	/**
	 * NCName ::=  (Letter | '_') (NCNameChar)*
	 * NCNameChar ::=  Letter | Digit | '.' | '-' | '_' | CombiningChar | Extender 
	 */
	void
	NCName();
  
	/**
	 * The value of the Literal is the sequence of characters inside 
	 * the " or ' characters>.
	 * --------------------------------------------------------------------------------
	 Literal	::=  '"' [^"]* '"'   
	 | "'" [^']* "'"	 
	 * --------------------------------------------------------------------------------
	 */
	void
	Literal();
  
	/**
	 * --------------------------------------------------------------------------------
	 * Number ::= [0-9]+('.'[0-9]+)? | '.'[0-9]+
	 * --------------------------------------------------------------------------------
	 */
	void
	Number();

	/**
	 * --------------------------------------------------------------------------------
	 Pattern	::=  LocationPathPattern   
	 | Pattern '|' LocationPathPattern   
	 * --------------------------------------------------------------------------------
	 */
	void
	Pattern();

	/**
	 * 
	 * --------------------------------------------------------------------------------
	 LocationPathPattern	::=  '/' RelativePathPattern?	
	 | IdKeyPattern (('/' | '//') RelativePathPattern)?	
	 | '//'? RelativePathPattern	 
	 * --------------------------------------------------------------------------------
	 */
	void
	LocationPathPattern();

	/**
	 * --------------------------------------------------------------------------------
	 IdKeyPattern  ::=  'id' '(' Literal ')'	 
	 | 'key' '(' Literal ',' Literal ')'	 
	 * (Also handle doc())
	 * --------------------------------------------------------------------------------
	 */
	void
	IdKeyPattern();

	/**
	 * --------------------------------------------------------------------------------
	 RelativePathPattern	::=  StepPattern   
	 | RelativePathPattern '/' StepPattern   
	 | RelativePathPattern '//' StepPattern	
	 * --------------------------------------------------------------------------------
	 */
	void
	RelativePathPattern();

	/**
	 * --------------------------------------------------------------------------------
	 StepPattern	::=  AbbreviatedNodeTestStep  
	 * --------------------------------------------------------------------------------
	 */
	void
	StepPattern();

	/**
	 * --------------------------------------------------------------------------------
	 AbbreviatedNodeTestStep	  ::=	 '@'? NodeTest Predicate*	
	 * --------------------------------------------------------------------------------
	 */
	void
	AbbreviatedNodeTestStep();

	bool
	isValidFunction(const DOMString&	key) const;

#if 0
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

private:

	static void
	initializeKeywordsTable();

	static void
	initializeFunctionTable();

	static void
	initializeAxisNamesTable();

	static void
	initializeNodeTypesTable();

	/**
	 * The current input token.
	 */
	DOMString						m_token;
  
	/**
	 * The first char in m_token, the theory being that this 
	 * is an optimization because we won't have to do charAt(0) as
	 * often.
	 */
	XMLCh							m_tokenChar;

	/**
	 * A pointer to the current XPath.
	 */
	XPath*							m_xpath;

	/**
	 * A pointer to the current XPath's expression.
	 */
	XPathExpression*				m_expression;

	/**
	 * A pointer to the current executionContext.
	 */
	const PrefixResolver*			m_prefixResolver;

	XObjectFactory*					m_xobjectFactory;

	const XPathEnvSupport*			m_envSupport;

	enum eDummy
	{
		TARGETEXTRA = 10000
	};

	/**
	 * Map of keyword names to token values.
	 */
	static KeywordsMapType			s_keywords;

	static FunctionNameMapType		s_functions;

	static AxisNamesMapType			s_axisNames;

	static NodeTypesMapType			s_nodeTypes;

	class StaticInitializer
	{
	public:

		StaticInitializer();

		~StaticInitializer()
		{
		}
	};

	friend class StaticInitializer;
};



#endif	// XPATHPROCESSORIMPL_HEADER_GUARD_1357924680
