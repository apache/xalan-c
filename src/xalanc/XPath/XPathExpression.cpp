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
// Class header file.
#include "XPathExpression.hpp"



#include <algorithm>
#include <cstdio>



#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#else
#include <ostream>
#endif



#include <xalanc/PlatformSupport/DoubleSupport.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include "XObjectTypeCallback.hpp"



XALAN_CPP_NAMESPACE_BEGIN



#if defined(XALAN_INLINE_INITIALIZATION) && !defined(XALAN_INLINE_INITIALIZATION_IS_DEFINITION_BUG)
const XPathExpression::TokenQueueSizeType   XPathExpression::s_opCodeMapLengthIndex;
#endif



// This is an array which indicates which of the value defined by
// XPathExpression::eOpCodes are node tests.
static const bool	theNodeTestOpCodesArray[] =
{
	false, false, true,  false, false, false, false, false, false, false,
	false, false, false, false, false, false, false, false, false, false,
	false, false, false, false, false, false, false, false, false, false,
	false, false, false, false, true,  true,  true,  true,  true,  true,
	true,  true,  true,  true,  true,  true,  true,  false, false, false, 
	true,  true,  true
};

static const unsigned int	theNodeTestOpCodesArraySize =
		sizeof(theNodeTestOpCodesArray) / sizeof(theNodeTestOpCodesArray[0]);


// This is an array for all of the opcode defined by XPathExpression::eOpCodes.
// It is offset by 3, because there opcode values are -3, -2, and -1.  So,
// when getting the length of an opcode, we add three to get the correct index.
static const int	theOpCodeLengthArray[] =
{
	1,
	1,
	1,

	0,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,

	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,

	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 3,
	XPathExpression::s_opCodeMapLengthIndex + 3,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	1,
	1,

	1,
	1,
	1,
	1,
	1,
	XPathExpression::s_opCodeMapLengthIndex + 2,
	XPathExpression::s_opCodeMapLengthIndex + 2,
	XPathExpression::s_opCodeMapLengthIndex + 2,
	XPathExpression::s_opCodeMapLengthIndex + 2,
	XPathExpression::s_opCodeMapLengthIndex + 2,

	XPathExpression::s_opCodeMapLengthIndex + 2,
	XPathExpression::s_opCodeMapLengthIndex + 2,
	XPathExpression::s_opCodeMapLengthIndex + 2,
	XPathExpression::s_opCodeMapLengthIndex + 2,
	XPathExpression::s_opCodeMapLengthIndex + 2,
	XPathExpression::s_opCodeMapLengthIndex + 2,
	XPathExpression::s_opCodeMapLengthIndex + 2,
	XPathExpression::s_opCodeMapLengthIndex + 2,
	XPathExpression::s_opCodeMapLengthIndex + 2,
	XPathExpression::s_opCodeMapLengthIndex + 1,

	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 2,
	XPathExpression::s_opCodeMapLengthIndex + 2,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1,
	XPathExpression::s_opCodeMapLengthIndex + 1
};

static const int	theOpCodeLengthArraySize =
		sizeof(theOpCodeLengthArray) / sizeof(theOpCodeLengthArray[0]);



inline int
getOpCodeLength(int		theOpCode)
{
	assert(theOpCodeLengthArraySize == XPathExpression::eOpCodeNextAvailable + 3);

	// Add 3 to get the correct index into the array...
	theOpCode += 3;

	if (theOpCode < 0 || theOpCode >= theOpCodeLengthArraySize)
	{
		return 0;
	}
	else
	{
		return theOpCodeLengthArray[theOpCode];
	}
}



XPathExpression::XPathExpressionException::XPathExpressionException(const XalanDOMString&	theMessage) :
	XalanXPathException(theMessage)
{
}



XPathExpression::XPathExpressionException::~XPathExpressionException()
{
}



XPathExpression::InvalidOpCodeException::InvalidOpCodeException(OpCodeMapValueType	theOpCode) :
	XPathExpressionException(FormatErrorMessage(theOpCode))
{
}



XPathExpression::InvalidOpCodeException::~InvalidOpCodeException()
{
}



XalanDOMString
XPathExpression::InvalidOpCodeException::FormatErrorMessage(OpCodeMapValueType  theOpCode)
{
	XalanDOMString	theOpcode; 

	LongToDOMString(theOpCode, theOpcode);

	return XalanMessageLoader::getMessage(XalanMessages::InvalidOpcodeWasDetected_1Param, theOpcode);
}



XPathExpression::InvalidArgumentCountException::InvalidArgumentCountException(
			OpCodeMapValueType	theOpCode,
			OpCodeMapValueType	theExpectedCount,
			OpCodeMapValueType	theSuppliedCount) :
	XPathExpressionException(FormatErrorMessage(theOpCode, theExpectedCount, theSuppliedCount))
{
}



XPathExpression::InvalidArgumentCountException::~InvalidArgumentCountException()
{
}



XalanDOMString
XPathExpression::InvalidArgumentCountException::FormatErrorMessage(
			OpCodeMapValueType	theOpCode,
			OpCodeMapValueType	theExpectedCount,
			OpCodeMapValueType	theSuppliedCount)
{
	XalanDOMString	theResult;

	LongToDOMString(theOpCode, theResult);

	XalanDOMString	theResult1;
	LongToDOMString(theExpectedCount, theResult1);

	XalanDOMString	theResult2;
	LongToDOMString(theSuppliedCount, theResult2);


	return  XalanMessageLoader::getMessage(XalanMessages::InvalidNumberOfArgsWasDetected_3Param, theResult ,theResult1 , theResult2);
}



XPathExpression::InvalidArgumentException::InvalidArgumentException(
			OpCodeMapValueType	theOpCode,
			OpCodeMapValueType	theValue) :
	XPathExpressionException(FormatErrorMessage(theOpCode, theValue))
{
}



XPathExpression::InvalidArgumentException::~InvalidArgumentException()
{
}



XalanDOMString
XPathExpression::InvalidArgumentException::FormatErrorMessage(
				OpCodeMapValueType	theOpCode,
				OpCodeMapValueType	theValue)
{
	XalanDOMString	theResult; 

	LongToDOMString(theValue, theResult);

	
	XalanDOMString	theResult1;
	UnsignedLongToDOMString(theOpCode, theResult1);

	return XalanMessageLoader::getMessage(XalanMessages::InvalidNumberOfArgsWasSupplied_2Param, theResult, theResult1) ;
}



static const XalanDOMString		s_emptyString;



XPathExpression::XPathExpression() :
	m_opMap(),
	m_lastOpCodeIndex(0),
	m_tokenQueue(),
	m_currentPosition(0),
	m_currentPattern(&s_emptyString)
{
	m_opMap.reserve(eDefaultOpMapSize);
	m_tokenQueue.reserve(eDefaultTokenQueueSize);
}



XPathExpression::~XPathExpression()
{
	reset();
}



void
XPathExpression::reset()
{
	XALAN_USING_STD(fill);
	XALAN_USING_STD(for_each);

	m_opMap.clear();
	m_tokenQueue.clear();
}



void
XPathExpression::shrink()
{
	if (m_opMap.capacity() > m_opMap.size())
	{
		OpCodeMapType(m_opMap).swap(m_opMap);
	}
}



#if defined(XALAN_XPATH_EXPRESSION_USE_ITERATORS)

XPathExpression::OpCodeMapValueType
XPathExpression::getOpCodeLengthFromOpMap(OpCodeMapPositionType     opPos) const
{
    assert(opPos - getInitialOpCodePosition() >= 0 &&
           opPos - getInitialOpCodePosition() < opCodeMapSize());

    OpCodeMapValueType	theResult = 0;

	// Is there a valid opcode?
	const int	theOpCodeLength = getOpCodeLength(*opPos);

	if (theOpCodeLength == 0)
	{
		throw InvalidOpCodeException(-1);
	}
	else
	{
		// Does the Op code have a length > 1?
		if (theOpCodeLength > 1)
		{
			// Yes, so get the length.
			theResult = *(opPos + s_opCodeMapLengthIndex);
		}
	}

	return theResult;
}

#endif

XPathExpression::OpCodeMapValueType
#if defined(XALAN_XPATH_EXPRESSION_USE_ITERATORS)
XPathExpression::getOpCodeLengthFromOpMap(OpCodeMapSizeType     theIndex) const
#else
XPathExpression::getOpCodeLengthFromOpMap(OpCodeMapPositionType     theIndex) const
#endif
{
	OpCodeMapValueType	theResult = 0;

	if (theIndex >= opCodeMapSize())
	{
		throw InvalidOpCodeException(-1);
	}
	else
	{
		// Is there a valid opcode?
		const int	theOpCodeLength = getOpCodeLength(m_opMap[theIndex]);

		if (theOpCodeLength == 0)
		{
			throw InvalidOpCodeException(-1);
		}
		else
		{
			// Does the Op code have a length > 1?
			if (theOpCodeLength > 1)
			{
				// Yes, so get the length.
				theResult = m_opMap[theIndex + s_opCodeMapLengthIndex];
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

	const int	theOpCodeLength = getOpCodeLength(theOpCode);

	// Make sure it's a valid op code and that it
	// matches the op code at supplied index.
	if (theOpCodeLength == 0 ||
		m_opMap[theIndex] != theOpCode)
	{
		throw InvalidOpCodeException(theOpCode);
	}
	else
	{
		const OpCodeMapValueType    theArgCount =
				theOpCodeLength - 1 - s_opCodeMapLengthIndex;

        if (OpCodeMapValueVectorType::size_type(theArgCount) != theArgs.size())
		{
			throw InvalidArgumentCountException(theOpCode,
												theOpCodeLength,
												theArgCount);
		}
		else
		{
			assert(opCodeMapSize() > theIndex + theArgCount);

			for (OpCodeMapValueType	i = 0; i < theArgCount; ++i)
			{
				if (theArgs[i] < 0)
				{
					throw InvalidArgumentException(theOpCode,
												   theArgs[i]);
				}
				else
				{
					m_opMap[i + theIndex + s_opCodeMapLengthIndex + 1] =
						theArgs[i];
				}
			}
		}
	}
}



XPathExpression::OpCodeMapSizeType
XPathExpression::appendOpCode(eOpCodes	theOpCode)
{
	const int	theOpCodeLength = getOpCodeLength(theOpCode);

	if (theOpCodeLength == 0)
	{
		throw InvalidOpCodeException(theOpCode);
	}
	else
	{
		// Set the current index before pushing, by
		// getting the length, which will be correct
		// after the push.
		m_lastOpCodeIndex = opCodeMapSize();

		m_opMap.push_back(theOpCode);

		assert(s_opCodeMapLengthIndex == 1);

		if (theOpCodeLength > 1)
		{
			m_opMap.push_back(theOpCodeLength);

			for (int i = 2; i < theOpCodeLength; ++i)
			{
				m_opMap.push_back(eENDOP);
			}
		}

		if (m_lastOpCodeIndex != 0)
		{
			m_opMap[s_opCodeMapLengthIndex] += theOpCodeLength;
		}
	}

	return m_lastOpCodeIndex;

	assert(opCodeMapSize() == OpCodeMapSizeType(opCodeMapLength()));
}



void
XPathExpression::replaceOpCode(
			OpCodeMapSizeType	theIndex,
			eOpCodes			theOldOpCode,
			eOpCodes			theNewOpCode)
{
	if (theIndex >= opCodeMapSize() ||
		m_opMap[theIndex] != theOldOpCode ||
		getOpCodeLength(theOldOpCode) != getOpCodeLength(theNewOpCode))
	{
		throw InvalidOpCodeException(theNewOpCode);
	}
	else
	{
		m_opMap[theIndex] = theNewOpCode;
	}
}



XPathExpression::OpCodeMapValueType
XPathExpression::insertOpCode(
			eOpCodes			theOpCode,
			OpCodeMapSizeType	theIndex)
{
	const int	theOpCodeLength = getOpCodeLength(theOpCode);

	if (theOpCodeLength == 0)
	{
		throw InvalidOpCodeException(theOpCode);
	}
	else
	{
		// Insert -1 for each element of the opcode.
		m_opMap.insert(
			m_opMap.begin() + theIndex,
			OpCodeMapSizeType(theOpCodeLength),
			-1);

		// Set the member that contains the last insert/appended
		// index.
		m_lastOpCodeIndex = theIndex;

		// Assign the opcode.
		m_opMap[theIndex] = theOpCode;

		// Update the entire expression length.
		m_opMap[s_opCodeMapLengthIndex] += theOpCodeLength;
	}

	return theOpCodeLength;
}



void
XPathExpression::updateShiftedOpCodeLength(
			OpCodeMapValueType	theOpCode,
#if defined(NDEBUG)
			OpCodeMapSizeType	/* theOriginalIndex */,
#else
			OpCodeMapSizeType	theOriginalIndex,
#endif
			OpCodeMapSizeType	theNewIndex)
{
	// There must be some other expressions in
	// the buffer...
	assert(opCodeMapSize() > theNewIndex + 1);

	assert(theNewIndex > theOriginalIndex);

	const OpCodeMapValueType    theOpCodeLength = getOpCodeLength(theOpCode);

	// Make sure it's a valid op code and that it
	// matches the op code at supplied index.
	if (theOpCodeLength == 0 ||
		m_opMap[theNewIndex] != theOpCode)
	{
		throw InvalidOpCodeException(theOpCode);
	}
	else
	{
		// Determine where the length position of the op code
		// is.
		const	OpCodeMapSizeType	theLengthIndex =
						theNewIndex + s_opCodeMapLengthIndex;

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
				m_opMap[theLengthIndex] += getOpCodeLengthFromOpMap(theNextOpCodePosition);
			}
		}
	}
}



void
XPathExpression::updateOpCodeLength(
			OpCodeMapValueType	theOpCode,
			OpCodeMapValueType	theIndex)
{
	// There must be some other expressions in
	// the buffer...
	assert(opCodeMapSize() > theIndex + 1);

	const int	theOpCodeLength = getOpCodeLength(theOpCode);

	// Make sure it's a valid op code and that it
	// matches the op code at supplied index.
	if (theOpCodeLength == 0 ||
		m_opMap[theIndex] != theOpCode)
	{
		throw InvalidOpCodeException(theOpCode);
	}
	else
	{
		// This presumes that the other opcodes
		// have been appended to the expression,
		// and that the specified op code's length
		// needs to be set.  The size includes the
		// normal length of the opcode, plus the
		// length of its subexpressions.
		m_opMap[theIndex + s_opCodeMapLengthIndex] =
							opCodeMapLength() - theIndex;
	}
}



bool
XPathExpression::isNodeTestOpCode(OpCodeMapValueType	theOpCode)
{
	if (theOpCode < theNodeTestOpCodesArraySize)
	{
		return theNodeTestOpCodesArray[theOpCode];
	}
	else
	{
		return false;
	}
}



void
XPathExpression::updateOpCodeLengthAfterNodeTest(OpCodeMapSizeType  theIndex)
{
	// There must be some other expressions in
	// the buffer...
	assert(opCodeMapSize() > theIndex + 1);

	const OpCodeMapValueType	theOpCode =
						m_opMap[theIndex];

	const int	theOpCodeLength = getOpCodeLength(theOpCode);

	// Make sure it's a valid op code and that it
	// is a node test op code.
	if (theOpCodeLength == 0 ||
		isNodeTestOpCode(theOpCode) == false)
	{
		throw InvalidOpCodeException(theOpCode);
	}
	else
	{
		m_opMap[theIndex + s_opCodeMapLengthIndex + 1] =
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
XPathExpression::dumpOpCodeMap(
			OstreamType&		theStream,
			OpCodeMapSizeType	theStartPosition) const
{
	for (OpCodeMapSizeType	i = theStartPosition;
				i < opCodeMapSize(); i++) 
	{
		theStream << " '" << m_opMap[i] << "'";
	}
}



void
XPathExpression::dumpTokenQueue(
			OstreamType&		theStream,
			TokenQueueSizeType	theStartPosition) const
{
	for (TokenQueueSizeType	i = theStartPosition;
				i < tokenQueueSize(); i++) 
	{
		assert(getToken(i) != 0);

		theStream << " '" << getToken(i)->str() << "'";
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

		if (i > 0)
		{
			thePrintWriter.print(" '");
		}
		else
		{
			thePrintWriter.print("'");
		}

		thePrintWriter.print(getToken(i)->str());
		thePrintWriter.print("'");
	}
}



void
XPathExpression::dumpRemainingTokenQueue(PrintWriter&	thePrintWriter) const
{
	thePrintWriter.print(XalanMessageLoader::getMessage(XalanMessages::RemainingTokens));
	thePrintWriter.print("(");

	dumpTokenQueue(thePrintWriter,
				   m_currentPosition);

	thePrintWriter.print(")");
}



void
XPathExpression::dumpRemainingTokenQueue(OstreamType&	theStream) const
{
	theStream << XalanMessageLoader::getMessage(XalanMessages::RemainingTokens) << "(";


	dumpTokenQueue(theStream,
				   m_currentPosition);

	theStream << ")";
}



void
XPathExpression::pushArgumentOnOpCodeMap(const XToken&	theXToken)
{
	assert(m_currentPosition != 0);

	const int	thePosition = m_currentPosition - 1;

	assert(thePosition < tokenQueueSize());

	// Set the entry in the token queue to the XObject.
	m_tokenQueue[thePosition] = theXToken;

	// Push the index onto the op map.
	m_opMap.push_back(thePosition);

	// Update the op map length.
	++m_opMap[s_opCodeMapLengthIndex];
}



void
XPathExpression::pushArgumentOnOpCodeMap(const XalanDOMString&	theToken)
{
	assert(m_currentPosition != 0);

	const TokenQueueSizeType	thePosition = m_currentPosition - 1;

	assert(thePosition < tokenQueueSize());

	// Set the entry in the token queue to the XObject.
	m_tokenQueue[thePosition].set(theToken);

	// Push the index onto the op map.
	m_opMap.push_back(thePosition);

	// Update the op map length.
	++m_opMap[s_opCodeMapLengthIndex];
}



void
XPathExpression::pushArgumentOnOpCodeMap(
			double					theNumber,
			const XalanDOMString&	theString)
{
	assert(m_currentPosition != 0);

	const TokenQueueSizeType	thePosition = m_currentPosition - 1;

	assert(thePosition < tokenQueueSize());

	// Set the entry in the token queue to the XObject.
	m_tokenQueue[thePosition].set(theNumber, theString);

	// Push the index onto the op map.
	m_opMap.push_back(thePosition);

	// Update the op map length.
	++m_opMap[s_opCodeMapLengthIndex];
}



void
XPathExpression::pushNumberLiteralOnOpCodeMap(double	theNumber)
{
	// Get the new index for the literal...
	const OpCodeMapValueType	theIndex = OpCodeMapValueType(m_numberLiteralValues.size());

	assert(NumberLiteralValueVectorType::size_type(theIndex) == m_numberLiteralValues.size());

	// Push the index onto the op map.
	m_opMap.push_back(theIndex);

	// Update the op map length.
	++m_opMap[s_opCodeMapLengthIndex];

	m_numberLiteralValues.push_back(theNumber);
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
	++m_opMap[s_opCodeMapLengthIndex];
}



XALAN_CPP_NAMESPACE_END
