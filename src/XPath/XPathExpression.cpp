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
// Class header file.
#include "XPathExpression.hpp"



#include <cstdio>
#include <strstream>



const XPathExpression::OpCodeLengthMapType	XPathExpression::s_opCodeLengths =
				XPathExpression::IntializeOpCodeLengthMap();



const XPathExpression::NodeTestSetType		XPathExpression::s_NodeTestOpCodes =
				XPathExpression::InitializeNodeTestSet();


XPathExpression::XPathExpressionException::XPathExpressionException(const XalanDOMString&	theMessage) :
	XPathException(theMessage)
{
}



XPathExpression::XPathExpressionException::~XPathExpressionException()
{
}



XPathExpression::InvalidOpCodeException::InvalidOpCodeException(int		theOpCode) :
	XPathExpressionException(FormatErrorMessage(theOpCode))
{
}



XPathExpression::InvalidOpCodeException::~InvalidOpCodeException()
{
}



XalanDOMString
XPathExpression::InvalidOpCodeException::FormatErrorMessage(int		theOpCode)
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::ostrstream;
#endif

	ostrstream	theFormatter;

	theFormatter << "Invalid op code "
				 << theOpCode
				 << " was detected."
				 << '\0';

	theFormatter.freeze(false);

	return theFormatter.str();
}



XPathExpression::InvalidArgumentCountException::InvalidArgumentCountException(
			int		theOpCode,
			int		theExpectedCount,
			int		theSuppliedCount) :
	XPathExpressionException(FormatErrorMessage(theOpCode, theExpectedCount, theSuppliedCount))
{
}



XPathExpression::InvalidArgumentCountException::~InvalidArgumentCountException()
{
}



XalanDOMString
XPathExpression::InvalidArgumentCountException::FormatErrorMessage(
			int		theOpCode,
			int		theExpectedCount,
			int		theSuppliedCount)
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::ostrstream;
#endif

	ostrstream	theFormatter;

	theFormatter << "An invalid number of arguments for op code "
				 << theOpCode
				 << " was detected.  "
				 << "The required number of arguments is "
				 << theExpectedCount
				 << ", but "
				 << theSuppliedCount
				 << " arguments(s) were supplied."
				 << '\0';

	theFormatter.freeze(false);

	return theFormatter.str();
}



XPathExpression::InvalidArgumentException::InvalidArgumentException(
			int	theOpCode,
			int	theValue) :
	XPathExpressionException(FormatErrorMessage(theOpCode, theValue))
{
}



XPathExpression::InvalidArgumentException::~InvalidArgumentException()
{
}



XalanDOMString
XPathExpression::InvalidArgumentException::FormatErrorMessage(
				int		theOpCode,
				int		theValue)
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::ostrstream;
#endif

	ostrstream	theFormatter;

	theFormatter << "An invalid argument of "
				 << theValue
				 << " was supplied for op code "
				 << theOpCode
				 << "."
				 << '\0';

	theFormatter.freeze(false);

	return theFormatter.str();
}



XPathExpression::XPathExpression() :
	m_opMap(),
	m_lastOpCodeIndex(0),
	m_tokenQueue(),
	m_currentPosition(0),
	m_patternMap(100),
	m_currentPattern()
{
	m_opMap.reserve(100);
	m_tokenQueue.reserve(100);
}



XPathExpression::~XPathExpression()
{
}



void
XPathExpression::reset()
{
	m_opMap.clear();
	m_tokenQueue.clear();
	m_patternMap.clear();
}



void
XPathExpression::shrink()
{
	OpCodeMapType(m_opMap).swap(m_opMap);
	TokenQueueType(m_tokenQueue).swap(m_tokenQueue);
	PatternMapType(m_patternMap).swap(m_patternMap);
}



XPathExpression::OpCodeMapValueType
XPathExpression::getOpCodeLength(OpCodeMapSizeType	opPos) const
{
	OpCodeMapValueType	theResult = 0;

	if (opPos >= opCodeMapSize())
	{
		throw InvalidOpCodeException(-1);
	}
	else
	{
		// Is there a valid opcode?
		OpCodeLengthMapType::const_iterator		i =
					s_opCodeLengths.find(m_opMap[opPos]);

		if (i == s_opCodeLengths.end())
		{
			throw InvalidOpCodeException(-1);
		}
		else
		{
			// Does the Op code have a length > 1?
			if ((*i).second > 1)
			{
				// Yes, so get the length.
				theResult = m_opMap[opPos + s__opCodeMapLengthIndex];
			}
		}
	}

	return theResult;
}



void
XPathExpression::setOpCodeArgs(
			eOpCodes							theOpCode,
			OpCodeMapSizeType					theIndex,
			const OpCodeMapValueVectorType&		theArgs)
{
	// There must be at least enough space to hold the OpCode
	// and the length indicator.
	assert(opCodeMapSize() > theIndex + 1);

	OpCodeLengthMapType::const_iterator	i =
			s_opCodeLengths.find(theOpCode);

	// Make sure it's a valid op code and that it
	// matches the op code at supplied index.
	if (i == s_opCodeLengths.end() ||
		m_opMap[theIndex] != theOpCode)
	{
		throw InvalidOpCodeException(theOpCode);
	}
	else
	{
		assert((*i).second > 0);

		const OpCodeMapSizeType		theArgCount =
				(*i).second - 1 - s__opCodeMapLengthIndex;

		if (theArgCount != theArgs.size())
		{
			throw InvalidArgumentCountException(theOpCode,
												(*i).second,
												theArgCount);
		}
		else
		{
			assert(opCodeMapSize() > theIndex + theArgCount);

			for (OpCodeMapSizeType	i = 0; i < theArgCount; ++i)
			{
				if (theArgs[i] < 0)
				{
					throw InvalidArgumentException(theOpCode,
												   theArgs[i]);
				}
				else
				{
					m_opMap[i + theIndex + s__opCodeMapLengthIndex + 1] =
						theArgs[i];
				}
			}
		}
	}
}



void
XPathExpression::appendOpCode(eOpCodes	theOpCode)
{
	OpCodeLengthMapType::const_iterator	i =
			s_opCodeLengths.find(theOpCode);

	if (i == s_opCodeLengths.end())
	{
		throw InvalidOpCodeException(theOpCode);
	}
	else
	{
		assert((*i).second > 0);

		// Set the current index before pushing, by
		// getting the size, which will be correct
		// after the push.
		m_lastOpCodeIndex = opCodeMapSize();

		m_opMap.push_back(theOpCode);

		assert(s__opCodeMapLengthIndex == 1);

		const OpCodeMapSizeType	theSize =
					(*i).second;

		if (theSize > 1)
		{
			m_opMap.push_back(theSize);

			for (OpCodeMapSizeType	i = 2;
						i < theSize;
								i++)
			{
				m_opMap.push_back(eENDOP);
			}
		}

		if (m_lastOpCodeIndex != 0)
		{
			m_opMap[s__opCodeMapLengthIndex] += theSize;
		}
	}

	assert(opCodeMapSize() ==
			static_cast<OpCodeMapSizeType>(opCodeMapLength()));
}



XPathExpression::OpCodeMapValueType
XPathExpression::insertOpCode(
			eOpCodes			theOpCode,
			OpCodeMapSizeType	theIndex)
{
	OpCodeLengthMapType::const_iterator	i =
				s_opCodeLengths.find(theOpCode);

	if (i == s_opCodeLengths.end())
	{
		throw InvalidOpCodeException(theOpCode);
	}
	else
	{
		assert((*i).second > 0);

		// Insert -1 for each element of the opcode.
		m_opMap.insert(&m_opMap[theIndex], (*i).second, -1);

		// Set the member that contains the last insert/appended
		// index.
		m_lastOpCodeIndex = theIndex;

		// Assign the opcode.
		m_opMap[theIndex] = theOpCode;

		// Update the entire expression length.
		m_opMap[s__opCodeMapLengthIndex] += (*i).second;
	}

	return (*i).second;
}



void
XPathExpression::updateShiftedOpCodeLength(
			OpCodeMapValueType	theOpCode,
			OpCodeMapSizeType	theOriginalIndex,
			OpCodeMapSizeType	theNewIndex)
{
	// There must be some other expressions in
	// the buffer...
	assert(opCodeMapSize() > theNewIndex + 1);

	assert(theNewIndex > theOriginalIndex);

	OpCodeLengthMapType::const_iterator	i =
			s_opCodeLengths.find(theOpCode);

	// Make sure it's a valid op code and that it
	// matches the op code at supplied index.
	if (i == s_opCodeLengths.end() ||
		m_opMap[theNewIndex] != theOpCode)
	{
		throw InvalidOpCodeException(theOpCode);
	}
	else
	{
		assert((*i).second > 0);

		// Determine where the length position of the op code
		// is.
		const	OpCodeMapSizeType	theLengthIndex =
						theNewIndex + s__opCodeMapLengthIndex;

		// Too long, then throw an exception.
		if (theLengthIndex >= opCodeMapSize())
		{
			throw InvalidOpCodeException(theOpCode);
		}
		else
		{
			// Look ahead to where the new operand is.
			const OpCodeMapSizeType		theNextOpCodePosition =
						getNextOpCodePosition(theNewIndex);

			// If it's there, then update our size by
			// adding that size in.
			if (theNextOpCodePosition < opCodeMapSize())
			{
				m_opMap[theLengthIndex] += getOpCodeLength(theNextOpCodePosition);
			}
		}
	}
}



void
XPathExpression::updateOpCodeLength(
			OpCodeMapValueType	theOpCode,
			OpCodeMapSizeType	theIndex)
{
	// There must be some other expressions in
	// the buffer...
	assert(opCodeMapSize() > theIndex + 1);

	OpCodeLengthMapType::const_iterator	i =
				s_opCodeLengths.find(theOpCode);

	// Make sure it's a valid op code and that it
	// matches the op code at supplied index.
	if (i == s_opCodeLengths.end() ||
		m_opMap[theIndex] != theOpCode)
	{
		throw InvalidOpCodeException(theOpCode);
	}
	else
	{
		assert((*i).second > 0);

		// This presumes that the other opcodes
		// have been appended to the expression,
		// and that the specified op code's length
		// needs to be set.  The size includes the
		// normal length of the opcode, plus the
		// length of its subexpressions.
		m_opMap[theIndex + s__opCodeMapLengthIndex] =
							opCodeMapLength() - theIndex;
	}
}



bool
XPathExpression::isNodeTestOpCode(OpCodeMapValueType	theOpCode)
{
	const NodeTestSetType::const_iterator	i =
		s_NodeTestOpCodes.find(theOpCode);

	if (i != s_NodeTestOpCodes.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}



void
XPathExpression::updateOpCodeLengthAfterNodeTest(OpCodeMapSizeType	theIndex)
{
	// There must be some other expressions in
	// the buffer...
	assert(opCodeMapSize() > theIndex + 1);

	const OpCodeMapValueType	theOpCode =
						m_opMap[theIndex];

	OpCodeLengthMapType::const_iterator	i =
				s_opCodeLengths.find(theOpCode);

	// Make sure it's a valid op code and that it
	// is a node test op code.
	if (i == s_opCodeLengths.end() ||
		isNodeTestOpCode(theOpCode) == false)
	{
		throw InvalidOpCodeException(theOpCode);
	}
	else
	{
		assert((*i).second > 0);

		m_opMap[theIndex + s__opCodeMapLengthIndex + 1] =
							opCodeMapLength() - theIndex;
	}
}



void
XPathExpression::dumpOpCodeMap(
			PrintWriter&		thePrintWriter,
			OpCodeMapSizeType	theStartPosition) const
{
	for (OpCodeMapSizeType	i = theStartPosition;
				i < opCodeMapSize(); i++) 
	{
		thePrintWriter.print(" '");
		thePrintWriter.print(m_opMap[i]);
		thePrintWriter.print("'");
	}
}



void
XPathExpression::dumpTokenQueue(
			PrintWriter&		thePrintWriter,
			TokenQueueSizeType	theStartPosition) const
{
	for (TokenQueueSizeType	i = theStartPosition;
				i < tokenQueueSize(); i++) 
	{
		assert(getToken(i) != 0);

		thePrintWriter.print(" '");
		thePrintWriter.print(getToken(i)->str());
		thePrintWriter.print("'");
	}
}



void
XPathExpression::dumpRemainingTokenQueue(PrintWriter&	thePrintWriter) const
{
	thePrintWriter.print("Remaining tokens: (");

	dumpTokenQueue(thePrintWriter,
				   m_currentPosition);

	thePrintWriter.print(")");
}



void
XPathExpression::pushArgumentOnOpCodeMap(XObject*	theToken)
{
	assert(theToken != 0);
	assert(m_currentPosition != 0);

	const TokenQueueSizeType	thePosition = m_currentPosition - 1;

	assert(thePosition < tokenQueueSize());

	// Set the entry in the token queue to the XObject.
	m_tokenQueue[thePosition] = theToken;

	// Push the index onto the op map.
	m_opMap.push_back(thePosition);

	// Update the op map length.
	m_opMap[s__opCodeMapLengthIndex]++;
}



void
XPathExpression::pushCurrentTokenOnOpCodeMap()
{
	assert(m_currentPosition != 0);

	const TokenQueueSizeType	thePosition = m_currentPosition - 1;

	assert(thePosition < tokenQueueSize());

	// Push the index onto the op map.
	m_opMap.push_back(thePosition);

	// Update the op map length.
	m_opMap[s__opCodeMapLengthIndex]++;
}



XPathExpression::OpCodeLengthMapType
XPathExpression::IntializeOpCodeLengthMap()
{
	OpCodeLengthMapType		theMap;

	theMap[eENDOP] = 1;
	theMap[eEMPTY] = 1;
	theMap[eELEMWILDCARD] = 1;
	theMap[eOP_XPATH] = 1 + s__opCodeMapLengthIndex;
	theMap[eOP_OR] = 1 + s__opCodeMapLengthIndex;
	theMap[eOP_AND] = 1 + s__opCodeMapLengthIndex;
	theMap[eOP_NOTEQUALS] = 1 + s__opCodeMapLengthIndex;
	theMap[eOP_EQUALS] = 1 + s__opCodeMapLengthIndex;
	theMap[eOP_LTE] = 1 + s__opCodeMapLengthIndex;
	theMap[eOP_LT] = 1 + s__opCodeMapLengthIndex;
	theMap[eOP_GTE] = 1 + s__opCodeMapLengthIndex;
	theMap[eOP_GT] = 1 + s__opCodeMapLengthIndex;
	theMap[eOP_PLUS] = 1 + s__opCodeMapLengthIndex;
	theMap[eOP_MINUS] = 1 + s__opCodeMapLengthIndex;
	theMap[eOP_MULT] = 1 + s__opCodeMapLengthIndex;
	theMap[eOP_DIV] = 1 + s__opCodeMapLengthIndex;
	theMap[eOP_MOD] = 1 + s__opCodeMapLengthIndex;
	theMap[eOP_QUO] = 1 + s__opCodeMapLengthIndex;
	theMap[eOP_NEG] = 1 + s__opCodeMapLengthIndex;
	theMap[eOP_STRING] = 1 + s__opCodeMapLengthIndex;
	theMap[eOP_BOOL] = 1 + s__opCodeMapLengthIndex;
	theMap[eOP_NUMBER] = 1 + s__opCodeMapLengthIndex;
	theMap[eOP_UNION] = 1 + s__opCodeMapLengthIndex;
	theMap[eOP_LITERAL] = 1 + s__opCodeMapLengthIndex;
	theMap[eOP_VARIABLE] = 1 + s__opCodeMapLengthIndex;
	theMap[eOP_GROUP] = 1 + s__opCodeMapLengthIndex;
	theMap[eOP_NUMBERLIT] = 1 + s__opCodeMapLengthIndex;
	theMap[eOP_ARGUMENT] = 1 + s__opCodeMapLengthIndex;
	theMap[eOP_EXTFUNCTION] = 3 + s__opCodeMapLengthIndex;
	theMap[eOP_FUNCTION] = 2 + s__opCodeMapLengthIndex;
	theMap[eOP_LOCATIONPATH] = 1 + s__opCodeMapLengthIndex;
	theMap[eOP_PREDICATE] = 1 + s__opCodeMapLengthIndex;
	theMap[eNODETYPE_COMMENT] = 1;
	theMap[eNODETYPE_TEXT] = 1;
	theMap[eNODETYPE_PI] = 1;
	theMap[eNODETYPE_NODE] = 1;
	theMap[eNODENAME] = 1;
	theMap[eNODETYPE_ROOT] = 1;
	theMap[eNODETYPE_ANYELEMENT] = 1;
	theMap[eFROM_ANCESTORS] = 2 + s__opCodeMapLengthIndex;
	theMap[eFROM_ANCESTORS_OR_SELF] = 2 + s__opCodeMapLengthIndex;
	theMap[eFROM_ATTRIBUTES] = 2 + s__opCodeMapLengthIndex;
	theMap[eFROM_CHILDREN] = 2 + s__opCodeMapLengthIndex;
	theMap[eFROM_DESCENDANTS] = 2 + s__opCodeMapLengthIndex;
	theMap[eFROM_DESCENDANTS_OR_SELF] = 2 + s__opCodeMapLengthIndex;
	theMap[eFROM_FOLLOWING] = 2 + s__opCodeMapLengthIndex;
	theMap[eFROM_FOLLOWING_SIBLINGS] = 2 + s__opCodeMapLengthIndex;
	theMap[eFROM_PARENT] = 2 + s__opCodeMapLengthIndex;
	theMap[eFROM_PRECEDING] = 2 + s__opCodeMapLengthIndex;
	theMap[eFROM_PRECEDING_SIBLINGS] = 2 + s__opCodeMapLengthIndex;
	theMap[eFROM_SELF] = 2 + s__opCodeMapLengthIndex;
	theMap[eFROM_NAMESPACE] = 1 + s__opCodeMapLengthIndex;
//	theMap[eFROM_ATTRIBUTE] = -1;
//	theMap[eFROM_DOC] = -1;
//	theMap[eFROM_DOCREF] = -1;
//	theMap[eFROM_ID] = -1;
//	theMap[eFROM_IDREF] = -1;
	theMap[eFROM_ROOT] = 2 + s__opCodeMapLengthIndex;
	theMap[eOP_MATCHPATTERN] = 1 + s__opCodeMapLengthIndex;
	theMap[eOP_LOCATIONPATHPATTERN] = 1 + s__opCodeMapLengthIndex;
	theMap[eMATCH_ATTRIBUTE] = 1 + s__opCodeMapLengthIndex;
	theMap[eMATCH_ANY_ANCESTOR] = 1 + s__opCodeMapLengthIndex;
	theMap[eMATCH_IMMEDIATE_ANCESTOR] = 1 + s__opCodeMapLengthIndex;
	theMap[eMATCH_ANY_ANCESTOR_WITH_PREDICATE] = 2 + s__opCodeMapLengthIndex;

	return theMap;
}



XPathExpression::NodeTestSetType
XPathExpression::InitializeNodeTestSet()
{
	NodeTestSetType		theSet;

	theSet.insert(eMATCH_ANY_ANCESTOR);
	theSet.insert(eMATCH_IMMEDIATE_ANCESTOR);
	theSet.insert(eMATCH_ATTRIBUTE);
	theSet.insert(eFROM_ATTRIBUTES);
	theSet.insert(eFROM_DESCENDANTS_OR_SELF);
	theSet.insert(eFROM_CHILDREN);
	theSet.insert(eFROM_ANCESTORS);
	theSet.insert(eFROM_ANCESTORS_OR_SELF);
	theSet.insert(eFROM_ATTRIBUTES);
	theSet.insert(eFROM_CHILDREN);
	theSet.insert(eFROM_DESCENDANTS);
	theSet.insert(eFROM_DESCENDANTS_OR_SELF);
	theSet.insert(eFROM_FOLLOWING);
	theSet.insert(eFROM_FOLLOWING_SIBLINGS);
	theSet.insert(eFROM_PARENT);
	theSet.insert(eFROM_PRECEDING);
	theSet.insert(eFROM_PRECEDING_SIBLINGS);
	theSet.insert(eFROM_SELF);
	theSet.insert(eFROM_NAMESPACE);
	// theSet.insert(eFROM_ATTRIBUTE);
	// theSet.insert(eFROM_DOC);
	// theSet.insert(eFROM_DOCREF);
	// theSet.insert(eFROM_ID);
	// theSet.insert(eFROM_IDREF);
	theSet.insert(eFROM_ROOT);
	theSet.insert(eOP_OR);

	return theSet;
}
