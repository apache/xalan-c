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
 */
#if !defined(XPATHPROCESSORIMPL_HEADER_GUARD_1357924680)
#define XPATHPROCESSORIMPL_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



#include <cstdlib>
#include <map>
#include <vector>



#include <XalanDOM/XalanDOMString.hpp>



// Base class header file...
#include <XPath/XPathProcessor.hpp>



#include <XPath/XPath.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XalanNode;



/**
 * The XPathProcessorImpl class responsibilities include tokenizing and
 * parsing the XPath expression, and acting as a general interface to XPaths.
 */
class XALAN_XPATH_EXPORT XPathProcessorImpl : public XPathProcessor
{
public:

#if defined(XALAN_NO_STD_NAMESPACE)
	typedef map<XalanDOMString,
				XalanDOMString,
				less<XalanDOMString> >	StringToStringMapType;

#if defined(XALAN_USE_DEQUE_FOR_VECTOR_BOOL)
	typedef deque<bool>					BoolVectorType;
#else
	typedef vector<bool>				BoolVectorType;
#endif
#else
	typedef std::map<XalanDOMString,
					 XalanDOMString>	StringToStringMapType;

#if defined(XALAN_USE_DEQUE_FOR_VECTOR_BOOL)
	typedef std::deque<bool>			BoolVectorType;
#else
	typedef std::vector<bool>			BoolVectorType;
#endif
#endif

	XPathProcessorImpl();

	virtual
	~XPathProcessorImpl();


	// These are inherited from XPathProcessor...

	virtual void
	initXPath(
			XPath&						pathObj,
			XPathConstructionContext&	constructionContext,
			const XalanDOMString&		expression,
			const PrefixResolver&		resolver,
			const LocatorType*			locator = 0);

	virtual void
	initMatchPattern(
			XPath&						pathObj,
			XPathConstructionContext&	constructionContext,
			const XalanDOMString&		expression,
			const PrefixResolver&		resolver,
			const LocatorType*			locator = 0);

private:

	/**
	 * Walk through the expression and build a token queue, and a map of the
	 * top-level elements.
	 *
	 * @param pat XSLT Expression.
	 */
	void
	tokenize(const XalanDOMString&	pat);
  
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

	void
	addToTokenQueue(const XalanDOMString&	s) const;

	void
	replaceTokenWithNamespaceToken() const;

	/**
	 * When a separator token is found, see if there's a element name or the
	 * like to map.
	 */
	int
	mapNSTokens(
			const XalanDOMString&	pat,
			int						startSubstring,
			int						posOfNSSep,
			int						posOfScan);

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
	tokenIs(const XalanDOMString&	s) const;

	/**
	 * Check if m_token==s. If m_token is null, this won't throw
	 * an exception, instead it just returns false (or true
	 * if s is also null).
	 */
	bool
	tokenIs(const XalanDOMChar*		s) const;

	/**
	 * Check if m_token==s. If m_token is null, this won't throw
	 * an exception, instead it just returns false (or true
	 * if s is also null).
	 */
	bool
	tokenIs(XalanDOMChar	c) const;

	/**
	 * Lookahead of the current token in order to 
	 * make a branching decision.
	 * @param s the string to compare it to.
	 * @param n number of tokens to lookahead.  Must be 
	 * greater than 1.
	 */
	bool
	lookahead(
			XalanDOMChar	c,
			int				n) const;

	/**
	 * Lookahead of the current token in order to 
	 * make a branching decision.
	 * @param s the string to compare it to.
	 * @param n number of tokens to lookahead.  Must be 
	 * greater than 1.
	 */
	bool
	lookahead(
			const XalanDOMChar*		s,
			int						n) const;

	/**
	 * Lookahead of the current token in order to 
	 * make a branching decision.
	 * @param s the string to compare it to.
	 * @param n number of tokens to lookahead.  Must be 
	 * greater than 1.
	 */
	bool
	lookahead(
			const XalanDOMString&	s,
			int						n) const;

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
	const XalanDOMString&
	getTokenRelative(int	theOffset) const;

	/**
	 * Retrieve the previous token from the command and
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
	consumeExpected(XalanDOMChar	expected);

	bool
	isCurrentLiteral() const;

	/**
	 * Determine if the token is an axis
	 *
	 * @param theToken The token to test
	 * @return true if the token is a valid axis, false if not.
	 */
	static bool
	isAxis(const XalanDOMString&	theToken);

	/**
	 * Determine if the token could be a node test
	 *
	 * @param theToken The token to test
	 * @return true if the token is a valid node test, false if not.
	 */
	static bool
	isNodeTest(const XalanDOMString&	theToken);

	/**
	 * Throw an exception using the provided message text.
	 */
	void
	error(
			const XalanDOMString&	msg,
			XalanNode*				sourceNode = 0) const;

	/**
	 * Throw an exception using the provided message text.
	 */
	void
	error(
			const char*		msg,
			XalanNode*		sourceNode = 0) const;

	/**
	 * Given a string, return the corresponding token.
	 */
	static XPathExpression::eOpCodes
	getFunctionToken(const XalanDOMString&	key)
	{
		return searchTable(s_functionTable, s_functionTableSize, key).m_opCode;
	}

	/**
	 * Given a string, return the corresponding token.
	 */
	static XPathExpression::eOpCodes
	getNodeTypeToken(const XalanDOMString&	key)
	{
		return searchTable(s_nodeTypeTable, s_nodeTypeTableSize, key).m_opCode;
	}

	/**
	 * Given a string, return the corresponding token.
	 */
	static XPathExpression::eOpCodes
	getAxisToken(const XalanDOMString&	key)
	{
		return searchTable(s_axisTable, s_axisTableSize, key).m_opCode;
	}

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

	void
	FunctionPosition();

	void
	FunctionLast();

	void
	FunctionCount();

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
	XPathExpression::eOpCodes
	AxisName();
  
	/**
	 * --------------------------------------------------------------------------------
	 NodeTest    ::=	  WildcardName	 
	 | NodeType '(' ')'  
	 | 'processing-instruction' '(' Literal ')' 
	 */
	int
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

	static bool
	isValidFunction(const XalanDOMString&	key);

private:

	int
	FunctionCallArguments();

	struct TableEntry
	{
		const XalanDOMChar*			m_string;

		XPathExpression::eOpCodes	m_opCode;
	};

#if defined(XALAN_STRICT_ANSI_HEADERS)
	typedef std::size_t				size_type;
#else
	typedef size_t					size_type;
#endif

	static const TableEntry&
	searchTable(
		const TableEntry		theTable[],
		size_type				theTableSize,
		const XalanDOMString&	theString);

	/**
	 * The current input token.
	 */
	XalanDOMString					m_token;
  
	/**
	 * The first char in m_token, the theory being that this 
	 * is an optimization because we won't have to do charAt(0) as
	 * often.
	 */
	XalanDOMChar					m_tokenChar;

	/**
	 * A pointer to the current XPath.
	 */
	XPath*							m_xpath;

	/**
	 * A pointer to the current XPathConstructionContext.
	 */
	XPathConstructionContext*		m_constructionContext;

	/**
	 * A pointer to the current XPath's expression.
	 */
	XPathExpression*				m_expression;

	/**
	 * A pointer to the current executionContext.
	 */
	const PrefixResolver*			m_prefixResolver;

	bool							m_requireLiterals;

	bool							m_isMatchPattern;

	const LocatorType*				m_locator;

	BoolVectorType					m_positionPredicateStack;

	StringToStringMapType			m_namespaces;

	enum eDummy
	{
		TARGETEXTRA = 10000
	};

	static const XalanDOMString		s_emptyString;

	static const XalanDOMChar		s_functionIDString[];

	// This shouldn't really be here, since it's not part of the XPath standard,
	// but rather a part ofthe XSLT standard.
	static const XalanDOMChar		s_functionKeyString[];

	static const XalanDOMChar		s_orString[];

	static const XalanDOMChar		s_andString[];

	static const XalanDOMChar		s_divString[];

	static const XalanDOMChar		s_modString[];

	static const XalanDOMChar		s_dotString[];

	static const XalanDOMChar		s_dotDotString[];

	static const XalanDOMChar		s_axisString[];

	static const XalanDOMChar		s_attributeString[];

	static const XalanDOMChar		s_childString[];

	static const XalanDOMChar		s_lastString[];

	static const XalanDOMChar		s_positionString[];

	static const XalanDOMChar		s_asteriskString[];

	static const XalanDOMChar		s_commentString[];

	static const XalanDOMChar		s_piString[];

	static const XalanDOMChar		s_nodeString[];

	static const XalanDOMChar		s_textString[];

	static const XalanDOMChar		s_ancestorString[];

	static const XalanDOMChar		s_ancestorOrSelfString[];

	static const XalanDOMChar		s_descendantString[];

	static const XalanDOMChar		s_descendantOrSelfString[];

	static const XalanDOMChar		s_followingString[];

	static const XalanDOMChar		s_followingSiblingString[];

	static const XalanDOMChar		s_parentString[];

	static const XalanDOMChar		s_precedingString[];

	static const XalanDOMChar		s_precedingSiblingString[];

	static const XalanDOMChar		s_selfString[];

	static const XalanDOMChar		s_namespaceString[];

	static const TableEntry			s_functionTable[];

	static const size_type			s_functionTableSize;

	static const TableEntry			s_nodeTypeTable[];

	static const size_type			s_nodeTypeTableSize;

	static const TableEntry			s_axisTable[];

	static const size_type			s_axisTableSize;

	static const TableEntry			s_dummyEntry;
};



XALAN_CPP_NAMESPACE_END



#endif	// XPATHPROCESSORIMPL_HEADER_GUARD_1357924680
