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
#if !defined(XPATHPROCESSORIMPL_HEADER_GUARD_1357924680)
#define XPATHPROCESSORIMPL_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



#include <cstdlib>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/Include/XalanVector.hpp>
#include <xalanc/Include/XalanMap.hpp>



#include "xalanc/PlatformSupport/XalanMessageLoader.hpp"



// Base class header file...
#include <xalanc/XPath/XPathProcessor.hpp>



#include <xalanc/XPath/XPath.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XalanNode;



/**
 * The XPathProcessorImpl class responsibilities include tokenizing and
 * parsing the XPath expression, and acting as a general interface to XPaths.
 */
class XALAN_XPATH_EXPORT XPathProcessorImpl : public XPathProcessor
{
public:

    typedef XalanMap<XalanDOMString,
                const XalanDOMString*>  StringToStringMapType;

    typedef XalanVector<bool>               BoolVectorType;

    XPathProcessorImpl(MemoryManagerType& theManager XALAN_DEFAULT_MEMMGR);

    virtual
    ~XPathProcessorImpl();


    static XPathProcessorImpl*
    create(MemoryManagerType& theManager);
    // These are inherited from XPathProcessor...

    virtual void
    initXPath(
            XPath&                      pathObj,
            XPathConstructionContext&   constructionContext,
            const XalanDOMString&       expression,
            const PrefixResolver&       resolver,
            const LocatorType*          locator = 0,
            bool                        allowVariableReferences = true,
            bool                        allowKeyFunction = true);

    virtual void
    initMatchPattern(
            XPath&                      pathObj,
            XPathConstructionContext&   constructionContext,
            const XalanDOMString&       expression,
            const PrefixResolver&       resolver,
            const LocatorType*          locator = 0,
            bool                        allowVariableReferences = true,
            bool                        allowKeyFunction = true);

private:

    /**
     * Walk through the expression and build a token queue, and a map of the
     * top-level elements.
     *
     * @param pat XSLT Expression.
     */
    void
    tokenize(const XalanDOMString&  pat);
  
    void
    addToTokenQueue(const XalanDOMString&   s) const;

    void
    replaceTokenWithNamespaceToken() const;

    /**
     * When a separator token is found, see if there's a element name or the
     * like to map.
     */
    int
    mapNSTokens(
            const XalanDOMString&   pat,
            int                     startSubstring,
            int                     posOfNSSep,
            int                     posOfScan);

    /**
     * Check if m_token==s. If m_token is null, this won't throw
     * an exception, instead it just returns false (or true
     * if s is also null).
     */
    bool
    tokenIs(const XalanDOMString&   s) const;

    /**
     * Check if m_token==s. If m_token is null, this won't throw
     * an exception, instead it just returns false (or true
     * if s is also null).
     */
    bool
    tokenIs(const XalanDOMChar*     s) const;

    /**
     * Check if m_token==s. If m_token is null, this won't throw
     * an exception, instead it just returns false (or true
     * if s is also null).
     */
    bool
    tokenIs(XalanDOMChar    c) const;

    /**
     * Lookahead of the current token in order to 
     * make a branching decision.
     * @param s the string to compare it to.
     * @param n number of tokens to lookahead.  Must be 
     * greater than 1.
     */
    bool
    lookahead(
            XalanDOMChar    c,
            int             n) const;

    /**
     * Lookahead of the current token in order to 
     * make a branching decision.
     * @param s the string to compare it to.
     * @param n number of tokens to lookahead.  Must be 
     * greater than 1.
     */
    bool
    lookahead(
            const XalanDOMChar*     s,
            int                     n) const;

    /**
     * Lookahead of the current token in order to 
     * make a branching decision.
     * @param s the string to compare it to.
     * @param n number of tokens to lookahead.  Must be 
     * greater than 1.
     */
    bool
    lookahead(
            const XalanDOMString&   s,
            int                     n) const;

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
            char    c,
            int     n) const;

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
    lookbehindHasToken(int  n) const;

    /**
     * Retrieve the next token from the command and
     * store it in m_token string.
     */
    bool
    nextToken();

    /**
     * Retrieve the next token from the command and
     * store it in m_token string.
     */
    const XalanDOMString&
    getTokenRelative(int    theOffset) const;

    /**
     * Retrieve the previous token from the command and
     * store it in m_token string.
     */
    void
    prevToken();

    /**
     * Consume an expected token, throwing an exception if it 
     * isn't there.
     */
    void
    consumeExpected(XalanDOMChar    expected);

    bool
    isCurrentLiteral() const;

    /**
     * Determine if the token is an axis
     *
     * @param theToken The token to test
     * @return true if the token is a valid axis, false if not.
     */
    static bool
    isAxis(const XalanDOMString&    theToken);

    /**
     * Determine if the token could be a node test
     *
     * @param theToken The token to test
     * @return true if the token is a valid node test, false if not.
     */
    static bool
    isNodeTest(const XalanDOMString&    theToken);

    /**
     * Throw an exception using the provided message text.
     */
    void
    error(const XalanDOMString&     msg) const;

    /**
     * Throw an exception using the provided message text.
     */
    void
    error(XalanMessages::Codes  theCode) const;

    void
    error(
            XalanMessages::Codes    theCode,
            const XalanDOMString&   theToken) const;

    void
    error(
            XalanMessages::Codes    theCode,
            const XalanDOMChar*     theToken) const;

    void
    error(
            XalanMessages::Codes    theCode,
            XalanDOMChar            theToken1,
            const XalanDOMString&   theToken2) const;

    /**
     * Given a string, return the corresponding token.
     */
    static XPathExpression::eOpCodes
    getFunctionToken(const XalanDOMString&  key)
    {
        return searchTable(s_functionTable, s_functionTableSize, key).m_opCode;
    }

    /**
     * Given a string, return the corresponding token.
     */
    static XPathExpression::eOpCodes
    getNodeTypeToken(const XalanDOMString&  key)
    {
        return searchTable(s_nodeTypeTable, s_nodeTypeTableSize, key).m_opCode;
    }

    /**
     * Given a string, return the corresponding token.
     */
    static XPathExpression::eOpCodes
    getAxisToken(const XalanDOMString&  key)
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
     OrExpr  ::=    AndExpr   
     | OrExpr 'or' AndExpr   
     * --------------------------------------------------------------------------------
     */
    void
    OrExpr();

    /**
     * 
     * --------------------------------------------------------------------------------
     AndExpr    ::=  EqualityExpr   
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
    EqualityExpr(int    opCodePos = -1);

    /**
     * XXXX.
     * @returns an Object which is either a String, a Number, a Boolean, or a vector 
     * of nodes.
     * --------------------------------------------------------------------------------
     RelationalExpr  ::=    AdditiveExpr   
     | RelationalExpr '<' AdditiveExpr   
     | RelationalExpr '>' AdditiveExpr   
     | RelationalExpr '<=' AdditiveExpr 
     | RelationalExpr '>=' AdditiveExpr 
     * --------------------------------------------------------------------------------
     */
    int
    RelationalExpr(int  opCodePos = -1);

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
    AdditiveExpr(int    opCodePos = -1);

    /**
     * XXXX.
     * @returns an Object which is either a String, a Number, a Boolean, or a vector 
     * of nodes.
     * --------------------------------------------------------------------------------
     MultiplicativeExpr  ::=    UnaryExpr   
     | MultiplicativeExpr MultiplyOperator UnaryExpr     
     | MultiplicativeExpr 'div' UnaryExpr   
     | MultiplicativeExpr 'mod' UnaryExpr   
     | MultiplicativeExpr 'quo' UnaryExpr   
     * --------------------------------------------------------------------------------
     */
    int
    MultiplicativeExpr(int  opCodePos = -1);

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
     UnionExpr  ::=    PathExpr   
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
     FilterExpr  ::=    PrimaryExpr   
     | FilterExpr Predicate 
     * --------------------------------------------------------------------------------
     * @exception XSLProcessorException thrown if the active ProblemListener and XMLParserLiaison decide 
     * the error condition is severe enough to halt processing.
     */
    void
    FilterExpr();
  
    /**
     * --------------------------------------------------------------------------------
     PrimaryExpr    ::=  VariableReference   
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
     Argument    ::=      Expr     
     * --------------------------------------------------------------------------------
     */
    void
    Argument();
  
    /**
     * --------------------------------------------------------------------------------
     FunctionCall    ::=      FunctionName '(' ( Argument ( ',' Argument)*)? ')'    
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

    void
    FunctionNot();

    void
    FunctionTrue();

    void
    FunctionFalse();

    void
    FunctionBoolean();

    void
    FunctionName(int    opPos);

    void
    FunctionLocalName(int   opPos);

    void
    FunctionNumber(int  opPos);

    void
    FunctionFloor();

    void
    FunctionCeiling();

    void
    FunctionRound();

    void
    FunctionString(int  opPos);

    void
    FunctionStringLength(int    opPos);

    void
    FunctionSum();

    void
    FunctionNamespaceURI(int    opPos);

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
     Step    ::=      Basis Predicate*  
     | AbbreviatedStep
     */
    void
    Step();
  
    /**
     * --------------------------------------------------------------------------------
     Basis  ::=    AxisName '::' NodeTest   
     | AbbreviatedBasis  
     */
    void
    Basis();
  
    /**
     * --------------------------------------------------------------------------------
     Basis  ::=    AxisName '::' NodeTest   
     | AbbreviatedBasis  
     */
    XPathExpression::eOpCodes
    AxisName();
  
    /**
     * --------------------------------------------------------------------------------
     NodeTest    ::=      WildcardName   
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
     * QName ::=    (Prefix ':')? LocalPart 
     * Prefix ::=  NCName 
     * LocalPart ::=    NCName 
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
     Literal    ::=  '"' [^"]* '"'   
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
     Pattern    ::=  LocationPathPattern   
     | Pattern '|' LocationPathPattern   
     * --------------------------------------------------------------------------------
     */
    void
    Pattern();

    /**
     * 
     * --------------------------------------------------------------------------------
     LocationPathPattern    ::=  '/' RelativePathPattern?   
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
     RelativePathPattern    ::=  StepPattern   
     | RelativePathPattern '/' StepPattern   
     | RelativePathPattern '//' StepPattern 
     * --------------------------------------------------------------------------------
     */
    void
    RelativePathPattern();

    /**
     * --------------------------------------------------------------------------------
     StepPattern    ::=  AbbreviatedNodeTestStep  
     * --------------------------------------------------------------------------------
     */
    void
    StepPattern();

    /**
     * --------------------------------------------------------------------------------
     AbbreviatedNodeTestStep      ::=    '@'? NodeTest Predicate*   
     * --------------------------------------------------------------------------------
     */
    void
    AbbreviatedNodeTestStep();

    static bool
    isValidFunction(const XalanDOMString&   key);

private:

    int
    FunctionCallArguments();

    struct TableEntry
    {
        const XalanDOMChar*         m_string;

        XPathExpression::eOpCodes   m_opCode;
    };

#if defined(XALAN_STRICT_ANSI_HEADERS)
    typedef std::size_t             size_type;
#else
    typedef size_t                  size_type;
#endif

    static const TableEntry&
    searchTable(
        const TableEntry        theTable[],
        size_type               theTableSize,
        const XalanDOMString&   theString);

    /**
     * The current input token.
     */
    XalanDOMString                  m_token;
  
    /**
     * The first char in m_token, the theory being that this 
     * is an optimization because we won't have to do charAt(0) as
     * often.
     */
    XalanDOMChar                    m_tokenChar;

    /**
     * A pointer to the current XPath.
     */
    XPath*                          m_xpath;

    /**
     * A pointer to the current XPathConstructionContext.
     */
    XPathConstructionContext*       m_constructionContext;

    /**
     * A pointer to the current XPath's expression.
     */
    XPathExpression*                m_expression;

    /**
     * A pointer to the current executionContext.
     */
    const PrefixResolver*           m_prefixResolver;

    bool                            m_requireLiterals;

    bool                            m_isMatchPattern;

    const LocatorType*              m_locator;

    BoolVectorType                  m_positionPredicateStack;

    StringToStringMapType           m_namespaces;

    bool                            m_allowVariableReferences;

    bool                            m_allowKeyFunction;

    // Static stuff here...
    static const XalanDOMString     s_emptyString;

    static const XalanDOMChar       s_functionIDString[];

    // This shouldn't really be here, since it's not part of the XPath standard,
    // but rather a part ofthe XSLT standard.
    static const XalanDOMChar       s_functionKeyString[];

    static const XalanDOMChar       s_orString[];

    static const XalanDOMChar       s_andString[];

    static const XalanDOMChar       s_divString[];

    static const XalanDOMChar       s_modString[];

    static const XalanDOMChar       s_dotString[];

    static const XalanDOMChar       s_dotDotString[];

    static const XalanDOMChar       s_axisString[];

    static const XalanDOMChar       s_attributeString[];

    static const XalanDOMChar       s_childString[];

    static const XalanDOMChar       s_lastString[];

    static const XalanDOMChar       s_positionString[];

    static const XalanDOMChar       s_asteriskString[];

    static const XalanDOMChar       s_commentString[];

    static const XalanDOMChar       s_piString[];

    static const XalanDOMChar       s_nodeString[];

    static const XalanDOMChar       s_textString[];

    static const XalanDOMChar       s_ancestorString[];

    static const XalanDOMChar       s_ancestorOrSelfString[];

    static const XalanDOMChar       s_descendantString[];

    static const XalanDOMChar       s_descendantOrSelfString[];

    static const XalanDOMChar       s_followingString[];

    static const XalanDOMChar       s_followingSiblingString[];

    static const XalanDOMChar       s_parentString[];

    static const XalanDOMChar       s_precedingString[];

    static const XalanDOMChar       s_precedingSiblingString[];

    static const XalanDOMChar       s_selfString[];

    static const XalanDOMChar       s_namespaceString[];

    static const TableEntry         s_functionTable[];

    static const size_type          s_functionTableSize;

    static const TableEntry         s_nodeTypeTable[];

    static const size_type          s_nodeTypeTableSize;

    static const TableEntry         s_axisTable[];

    static const size_type          s_axisTableSize;

    static const TableEntry         s_dummyEntry;
};



XALAN_CPP_NAMESPACE_END



#endif  // XPATHPROCESSORIMPL_HEADER_GUARD_1357924680
