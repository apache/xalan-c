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
#if !defined(XPATHEXPRESSION_HEADER_GUARD_1357924680)
#define XPATHEXPRESSION_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



#include <map>
#include <set>
#include <vector>



#include <dom/DOMString.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/PrintWriter.hpp>



#include <XPath/XObject.hpp>
#include <XPath/XPathException.hpp>



class XObject;



class XALAN_XPATH_EXPORT XPathExpression
{
public:

	// List of operations codes.
	//
	// Code for the descriptions of the operations codes:
	// [UPPER CASE] indicates a literal value,
	// [lower case] is a description of a value,
	//		([length] always indicates the length of the operation,
	//		 including the operations code and the length integer.)
	// {UPPER CASE} indicates the given production,
	// {description} is the description of a new production,
	//		(For instance, {boolean expression} means some expression 
	//		 that should be resolved to a boolean.)
	//	* means that it occurs zero or more times,
	//	+ means that it occurs one or more times,
	//	? means that it is optional.
	//
	// returns: indicates what the production should return.

	enum eOpCodes
	{
		/**
		 * [ENDOP]
		 * Some operators may like to have a terminator.
		 */
		eENDOP = -1,

		/**
		 * [EMPTY]
		 * Empty slot to indicate NULL.
		 */
		eEMPTY = -2,

		/**
		 * [ELEMWILDCARD]
		 * Means ELEMWILDCARD ("*"), used instead 
		 * of string index in some places.
		 */
		eELEMWILDCARD = -3,

		/**
		 * [OP_XPATH]
		 * [length]
		 *	{expression}
		 * 
		 * returns: 
		 *	XNodeSet
		 *	XNumber
		 *	XString
		 *	XBoolean
		 *	XRTree
		 *	XObject
		 */
		eOP_XPATH = 1,

		/**
		 * [OP_OR]
		 * [length]
		 *	{boolean expression}
		 *	{boolean expression}
		 * 
		 * returns: 
		 *	XBoolean
		 */
		eOP_OR = 2,

		/**
		 * [OP_AND]
		 * [length]
		 *	{boolean expression}
		 *	{boolean expression}
		 * 
		 * returns: 
		 *	XBoolean
		 */
		eOP_AND = 3,

		/**
		 * [OP_NOTEQUALS]
		 * [length]
		 *	{expression}
		 *	{expression}
		 * 
		 * returns: 
		 *	XBoolean
		 */
		eOP_NOTEQUALS = 4,

		/**
		 * [OP_EQUALS]
		 * [length]
		 *	{expression}
		 *	{expression}
		 * 
		 * returns: 
		 *	XBoolean
		 */
		eOP_EQUALS = 5,

		/**
		 * [OP_LTE] (less-than-or-equals)
		 * [length]
		 *	{number expression}
		 *	{number expression}
		 * 
		 * returns: 
		 *	XBoolean
		 */
		eOP_LTE = 6,

		/**
		 * [OP_LT] (less-than)
		 * [length]
		 *	{number expression}
		 *	{number expression}
		 * 
		 * returns: 
		 *	XBoolean
		 */
		eOP_LT = 7,

		/**
		 * [OP_GTE] (greater-than-or-equals)
		 * [length]
		 *	{number expression}
		 *	{number expression}
		 * 
		 * returns: 
		 *	XBoolean
		 */
		eOP_GTE = 8,

		/**
		 * [OP_GT] (greater-than)
		 * [length]
		 *	{number expression}
		 *	{number expression}
		 * 
		 * returns: 
		 *	XBoolean
		 */
		eOP_GT = 9,

		/**
		 * [OP_PLUS]
		 * [length]
		 *	{number expression}
		 *	{number expression}
		 * 
		 * returns: 
		 *	XNumber
		 */
		eOP_PLUS = 10,

		/**
		 * [OP_MINUS]
		 * [length]
		 *	{number expression}
		 *	{number expression}
		 * 
		 * returns: 
		 *	XNumber
		 */
		eOP_MINUS = 11,

		/**
		 * [OP_MULT]
		 * [length]
		 *	{number expression}
		 *	{number expression}
		 * 
		 * returns: 
		 *	XNumber
		 */
		eOP_MULT = 12,

		/**
		 * [OP_DIV]
		 * [length]
		 *	{number expression}
		 *	{number expression}
		 * 
		 * returns: 
		 *	XNumber
		 */
		eOP_DIV = 13,

		/**
		 * [OP_MOD]
		 * [length]
		 *	{number expression}
		 *	{number expression}
		 * 
		 * returns: 
		 *	XNumber
		 */
		eOP_MOD = 14,

		/**
		 * [OP_QUO]
		 * [length]
		 *	{number expression}
		 *	{number expression}
		 * 
		 * returns: 
		 *	XNumber
		 */
		eOP_QUO = 15,

		/**
		 * [OP_NEG]
		 * [length]
		 *	{number expression}
		 * 
		 * returns: 
		 *	XNumber
		 */
		eOP_NEG = 16,

		/**
		 * [OP_STRING] (cast operation)
		 * [length]
		 *	{expression}
		 * 
		 * returns: 
		 *	XString
		 */
		eOP_STRING = 17,

		/**
		 * [OP_BOOL] (cast operation)
		 * [length]
		 *	{expression}
		 * 
		 * returns: 
		 *	XBoolean
		 */
		eOP_BOOL = 18,

		/**
		 * [OP_NUMBER] (cast operation)
		 * [length]
		 *	{expression}
		 * 
		 * returns: 
		 *	XBoolean
		 */
		eOP_NUMBER = 19,

		/**
		 * [OP_UNION]
		 * [length]
		 *	{PathExpr}+
		 * 
		 * returns: 
		 *	XNodeSet
		 */
		eOP_UNION = 20,

		/**
		 * [OP_LITERAL]
		 * [3]
		 * [index to token]
		 * 
		 * returns: 
		 *	XString
		 */
		eOP_LITERAL = 21,

		/**
		 * [OP_VARIABLE]
		 * [3]
		 * [index to token]
		 * 
		 * returns: 
		 *	XString
		 */
		eOP_VARIABLE = 22,

		/**
		 * [OP_GROUP]
		 * [length]
		 *	{expression}
		 * 
		 * returns: 
		 *	XNodeSet
		 *	XNumber
		 *	XString
		 *	XBoolean
		 *	XRTree
		 *	XObject
		 */
		eOP_GROUP = 23,

		/**
		 * [OP_NUMBERLIT] (Number literal.)
		 * [3]
		 * [index to token]
		 * 
		 * returns: 
		 *	XString
		 */
		eOP_NUMBERLIT = 24,

		/**
		 * [OP_ARGUMENT] (Function argument.)
		 * [length]
		 *	{expression}
		 * 
		 * returns: 
		 *	XNodeSet
		 *	XNumber
		 *	XString
		 *	XBoolean
		 *	XRTree
		 *	XObject
		 */
		eOP_ARGUMENT = 25,

		/**
		 * [OP_EXTFUNCTION] (Extension function.)
		 * [length]
		 * [index to namespace token]
		 * [index to function name token]
		 *  {OP_ARGUMENT}*
		 * 
		 * returns: 
		 *  XNodeSet
		 *  XNumber
		 *  XString
		 *  XBoolean
		 *  XRTree
		 *  XObject
		 */
		eOP_EXTFUNCTION = 26,

		/**
		 * [OP_FUNCTION]
		 * [length]
		 * [FUNC_name]
		 *  {OP_ARGUMENT}*
		 * [ENDOP]
		 * 
		 * returns: 
		 *  XNodeSet
		 *  XNumber
		 *  XString
		 *  XBoolean
		 *  XRTree
		 *  XObject
		 */
		eOP_FUNCTION = 27,

		/**
		 * [OP_LOCATIONPATH]
		 * [length]
		 *	 {FROM_stepType}
		 * | {function}{predicate}*
		 * [ENDOP]
		 * 
		 * (Note that element and attribute namespaces and 
		 * names can be wildcarded '*'.)
		 * 
		 * returns: 
		 *	XNodeSet
		 */
		eOP_LOCATIONPATH = 28,

		/**
		 * [OP_PREDICATE]
		 * [length]
		 *	{expression}
		 * [ENDOP] (For safety)
		 * 
		 * returns: 
		 *	XBoolean or XNumber
		 */
		eOP_PREDICATE = 29,
  
		/**
		 * [NODETYPE_COMMENT]
		 * No size or arguments.
		 * 
		 * returns: 
		 *	XBoolean
		 */
		eNODETYPE_COMMENT = 1030,
		
		/**
		 * [NODETYPE_TEXT]
		 * No size or arguments.
		 * 
		 * returns: 
		 *	XBoolean
		 */
		eNODETYPE_TEXT = 1031,
		
		/**
		 * [NODETYPE_PI]
		 * [index to token]
		 * 
		 * returns: 
		 *	XBoolean
		 */
		eNODETYPE_PI = 1032,
		
		/**
		 * [NODETYPE_NODE]
		 * No size or arguments.
		 * 
		 * returns: 
		 *	XBoolean
		 */
		eNODETYPE_NODE = 1033,
		
		/**
		 * [NODENAME]
		 * [index to ns token or EMPTY]
		 * [index to name token]
		 * 
		 * returns: 
		 *	XBoolean
		 */
		eNODENAME = 34,
		
		/**
		 * [NODETYPE_ROOT]
		 * No size or arguments.
		 * 
		 * returns: 
		 *	XBoolean
		 */
		eNODETYPE_ROOT = 35,
		
		/**
		 * [NODETYPE_ANY]
		 * No size or arguments.
		 * 
		 * returns: 
		 *	XBoolean
		 */
		eNODETYPE_ANYELEMENT = 36,

		/**
		 * [FROM_stepType]
		 * [length, including predicates]
		 * [length of just the step, without the predicates]
		 * {node test}
		 * {predicates}?
		 * 
		 * returns: 
		 *	XBoolean
		 */
		eFROM_ANCESTORS = 37,
		eFROM_ANCESTORS_OR_SELF = 38,
		eFROM_ATTRIBUTES = 39,
		eFROM_CHILDREN = 40,
		eFROM_DESCENDANTS = 41,
		eFROM_DESCENDANTS_OR_SELF = 42,
		eFROM_FOLLOWING = 43,
		eFROM_FOLLOWING_SIBLINGS = 44,
		eFROM_PARENT = 45,
		eFROM_PRECEDING = 46,
		eFROM_PRECEDING_SIBLINGS = 47,
		eFROM_SELF = 48,
		eFROM_NAMESPACE = 49,
		// eFROM_ATTRIBUTE = 50,
		// eFROM_DOC = 51,
		// eFROM_DOCREF = 52,
		// eFROM_ID = 53,
		// eFROM_IDREF = 54,
		eFROM_ROOT = 55,

		/**
		 * [OP_UNION]
		 * [length]
		 *  {PathExpr}+
		 * 
		 * returns: 
		 *  XNodeSet
		 */
		eOP_MATCHPATTERN = 92,

		/**
		 * [OP_UNION]
		 * [length]
		 *  {PathExpr}+
		 * 
		 * returns: 
		 *  XNodeSet
		 */
		eOP_LOCATIONPATHPATTERN = 93,

		// For match patterns
		eMATCH_ATTRIBUTE = 94,
		eMATCH_ANY_ANCESTOR = 95,
		eMATCH_IMMEDIATE_ANCESTOR = 96,
		eMATCH_ANY_ANCESTOR_WITH_PREDICATE = 97,
		// Always add _before_ this one.

		// $$$ ToDo: Determine if these specific numbers mean
		// anything vis-a-vis the parser and xpath code.
		eOpCodeNextAvailable
	};	// enum eOpCodes

	/**
	 * The length is always the opcode position + 1.
	 * Length is always expressed as the opcode+length bytes, 
	 * so it is always 2 or greater.  This is the offset
	 * from the op code where the length is stored.  It will
	 * always remain one.
	 */
#if defined(XALAN_INLINE_INITIALIZATION)
	const int	s__opCodeMapLengthIndex = 1;
#else
	enum eDummy
	{
		s__opCodeMapLengthIndex = 1
	};
#endif

	class XALAN_XPATH_EXPORT XPathExpressionException : public XPathException
	{
	public:

		XPathExpressionException(const std::string&		theMessage);

		virtual~
		XPathExpressionException();
	};

	class XALAN_XPATH_EXPORT InvalidOpCodeException : public XPathExpressionException
	{
	public:

		InvalidOpCodeException(int	theOpCode);

		virtual~
		InvalidOpCodeException();

	private:

		static std::string
		FormatErrorMessage(int	theOpCode);
	};

	class XALAN_XPATH_EXPORT InvalidArgumentCountException : public XPathExpressionException
	{
	public:

		InvalidArgumentCountException(
			int		theOpCode,
			int		theExpectedCount,
			int		theSuppliedCount);

		virtual~
		InvalidArgumentCountException();

	private:

		static std::string
		FormatErrorMessage(
			int		theOpCode,
			int		theExpectedCount,
			int		theSuppliedCount);
	};

	class XALAN_XPATH_EXPORT InvalidArgumentException : public XPathExpressionException
	{
	public:

		InvalidArgumentException(
			int	theOpCode,
			int	theValue);

		virtual~
		InvalidArgumentException();

	private:

		static std::string
		FormatErrorMessage(
				int		theOpCode,
				int		theValue);
	};


	typedef std::vector<int>				OpCodeMapType;
	typedef std::vector<XObject*>			TokenQueueType;
	typedef std::vector<int>				PatternMapType;
	typedef std::map<int, int>				OpCodeLengthMapType;

	typedef OpCodeMapType::value_type		OpCodeMapValueType;
	typedef OpCodeMapType::size_type		OpCodeMapSizeType;
	typedef TokenQueueType::value_type		TokenQueueValueType;
	typedef TokenQueueType::size_type		TokenQueueSizeType;
	typedef PatternMapType::value_type		PatternMapValueType;
	typedef PatternMapType::size_type		PatternMapSizeType;

	typedef std::set<OpCodeMapValueType>	NodeTestSetType;

	explicit
	XPathExpression();

	~XPathExpression();

	void
	reset();

	void
	shrink();

	OpCodeMapSizeType
	opCodeMapSize() const
	{
		return m_opMap.size();
	}

	OpCodeMapValueType
	opCodeMapLength() const
	{
		// The length of the entire map is stored after
		// the first op code.  That offset is determined
		// by this const static member.  Note that as
		// expressions are defined recursively, this is
		// really just the length of the first expression
		// in the map, which is the top of the parse tree.
		// Any subexpression will also have a length entry
		// at the same offset from the beginning of the
		// subexpression.
		const OpCodeMapSizeType		theSize = opCodeMapSize();

		if (theSize > s__opCodeMapLengthIndex)
		{
			assert(theSize ==
				static_cast<OpCodeMapSizeType>(m_opMap[s__opCodeMapLengthIndex]));

			return m_opMap[s__opCodeMapLengthIndex];
		}
		else
		{
			return theSize;
		}
	}

	TokenQueueSizeType
	tokenQueueSize() const
	{
		return m_tokenQueue.size();
	}

	PatternMapSizeType
	patternMapSize() const
	{
		return m_patternMap.size();
	}

	OpCodeMapValueType
	getOpCodeMapValue(OpCodeMapSizeType		opPos) const
	{
		return m_opMap[opPos];
	}

	OpCodeMapValueType
	getOpCodeLength(OpCodeMapSizeType	opPos) const;

	OpCodeMapValueType
	getNextOpCodePosition(OpCodeMapSizeType		opPos) const
	{
		assert(opPos < opCodeMapSize());

		return opPos + m_opMap[opPos + s__opCodeMapLengthIndex];
	}

	void
	setOpCodeArgs(
			eOpCodes								theOpCode,
			OpCodeMapSizeType						theIndex,
			const std::vector<OpCodeMapValueType>&	theArgs);

	void
	appendOpCode(eOpCodes	theOpCode);

	void
	appendOpCode(eOpCodes									theOpCode,
				 const std::vector<OpCodeMapValueType>&		theArgs)
	{
		appendOpCode(theOpCode);

		setOpCodeArgs(theOpCode,
					  m_lastOpCodeIndex,
					  theArgs);
	}

	OpCodeMapValueType
	insertOpCode(
			eOpCodes			theOpCode,
			OpCodeMapSizeType	theIndex);

	void
	updateOpCodeLength(OpCodeMapSizeType	theIndex)
	{
		assert(theIndex < opCodeMapSize());

		// This presumes that the other opcodes
		// have been appended to the expression,
		// and that the specified op code's length
		// needs to be set.  The size includes the
		// normal length of the opcode, plus the
		// length of its subexpressions.
		updateOpCodeLength(m_opMap[theIndex], theIndex);
	}

	void
	updateShiftedOpCodeLength(
			OpCodeMapValueType	theOpCode,
			OpCodeMapSizeType	theOriginalIndex,
			OpCodeMapSizeType	theNewIndex);

	void
	updateOpCodeLength(
			OpCodeMapValueType	theOpCode,
			OpCodeMapSizeType	theIndex);

	static bool
	isNodeTestOpCode(OpCodeMapValueType		theOpCode);

	void
	updateOpCodeLengthAfterNodeTest(OpCodeMapSizeType	theIndex);

	bool
	hasMoreTokens() const
	{
		return tokenQueueSize() - m_currentPosition > 0 ? true : false;
	}

	TokenQueueSizeType
	getTokenPosition() const
	{
		return m_currentPosition;
	}

	void
	resetTokenPosition()
	{
		m_currentPosition = 0;
	}

	void
	setTokenPosition(TokenQueueSizeType		thePosition)
	{
		const TokenQueueSizeType	theSize = tokenQueueSize();

		m_currentPosition = thePosition > theSize ? theSize : thePosition;
	}

	void
	setTokenPosition(int	thePosition)
	{
		setTokenPosition(thePosition > 0 ? static_cast<TokenQueueSizeType>(thePosition) : 0);
	}

	XObject*
	getToken(TokenQueueSizeType		thePosition) const
	{
		assert(thePosition < tokenQueueSize());

		return m_tokenQueue[thePosition];
	}

	XObject*
	getNextToken()
	{
		if (hasMoreTokens() == true)
		{
			return getToken(m_currentPosition++);
		}
		else
		{
			return 0;
		}
	}

	XObject*
	getPreviousToken()
	{
		if (m_currentPosition > 0)
		{
			return getToken(--m_currentPosition);
		}
		else
		{
			return 0;
		}
	}

	XObject*
	getRelativeToken(int	theOffset) const
	{
		const int	thePosition =
			static_cast<int>(m_currentPosition) + theOffset;

		if (thePosition < 0 ||
			thePosition >= static_cast<int>(tokenQueueSize()))
		{
			return 0;
		}
		else
		{
			return getToken(thePosition);
		}
	}

	void
	pushToken(XObject*	theToken)
	{
		assert(theToken != 0);

		m_tokenQueue.push_back(theToken);
	}

	void
	dumpOpCodeMap(PrintWriter&			thePrintWriter,
				  OpCodeMapSizeType		theStartPosition = 0) const;

	void
	dumpTokenQueue(PrintWriter&			thePrintWriter,
				   TokenQueueSizeType	theStartPosition = 0) const;

	void
	dumpRemainingTokenQueue(PrintWriter&	thePrintWriter) const;

	void
	pushArgumentOnOpCodeMap(XObject*	theToken);

	void
	pushCurrentTokenOnOpCodeMap();

	PatternMapValueType
	getPattern(int	thePatternPosition) const
	{
		assert(static_cast<int>(patternMapSize()) > thePatternPosition);

		return m_patternMap[thePatternPosition];
	}

	PatternMapValueType
	getPattern(PatternMapSizeType	thePatternPosition) const
	{
		assert(patternMapSize() > thePatternPosition);

		return m_patternMap[thePatternPosition];
	}

	void
	pushPattern(PatternMapValueType	thePattern)
	{
		m_patternMap.push_back(thePattern);
	}

	void
	adjustPattern(
			OpCodeMapSizeType	theIndex,
			PatternMapValueType	theAdjustment)
	{
		m_patternMap[theIndex] += theAdjustment;
	}

	void
	setCurrentPattern(const DOMString&	thePattern)
	{
		m_currentPattern = thePattern;
	}

	const DOMString&
	getCurrentPattern() const
	{
		return m_currentPattern;
	}

	/**
	 * An operations map is used instead of a proper parse tree.  It contains 
	 * operations codes and indexes into the m_tokenQueue.
	 * We use an array instead of a full parse tree in order to cut down 
	 * on the number of objects created.
	 */
	OpCodeMapType			m_opMap;

	/**
	 * This is the index of the last opcode that was appended or inserted.
	 *
	 */
	OpCodeMapSizeType		m_lastOpCodeIndex;

	/**
	 *  This is the queue of used tokens. The current token is the token at the 
	 * end of the m_tokenQueue. The idea is that the queue can be marked and a
	 * sequence of tokens can be reused.
	 */
	TokenQueueType			m_tokenQueue;

	/**
	 *  This is the current position in the token queue.
	 */
	TokenQueueSizeType		m_currentPosition;

	/**
	 * Ignore this, it is going away.
	 * This holds a map to the m_tokenQueue that tells where the top-level elements are.
	 * It is used for pattern matching so the m_tokenQueue can be walked backwards.
	 * Each element that is a 'target', (right-most top level element name) has 
	 * TARGETEXTRA added to it.
	 * 
	 */
	PatternMapType			m_patternMap;

	/**
	 * The current pattern string, for diagnostics purposes
	 */
	DOMString				m_currentPattern;

private:

	// A map of Op codes to op code lengths.
	const static OpCodeLengthMapType	s_opCodeLengths;

	static OpCodeLengthMapType
	IntializeOpCodeLengthMap();

	// A static set of Op codes that are node tests.
	const static NodeTestSetType		s_NodeTestOpCodes;

	static NodeTestSetType
	InitializeNodeTestSet();
};



#endif	// XPATHEXPRESSION_HEADER_GUARD_1357924680
