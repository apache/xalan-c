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
#if !defined(XPATHEXPRESSION_HEADER_GUARD_1357924680)
#define XPATHEXPRESSION_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



#include <xalanc/Include/XalanVector.hpp>



#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#else
#include <iosfwd>
#endif



#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/PrintWriter.hpp>



#include <xalanc/XPath/XToken.hpp>
#include <xalanc/XPath/XalanXPathException.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XALAN_USING_XERCES(MemoryManager)



class XALAN_XPATH_EXPORT XPathExpression
{
public:

    typedef XALAN_STD_QUALIFIER ostream         OstreamType;

    typedef XalanVector<int>                    OpCodeMapType;
    typedef XalanVector<XToken>                 TokenQueueType;

    typedef OpCodeMapType::value_type           OpCodeMapValueType;
    typedef OpCodeMapValueType                  OpCodeMapSizeType;

    typedef XalanVector<OpCodeMapValueType>     OpCodeMapValueVectorType;

    typedef XalanVector<double>                 NumberLiteralValueVectorType;

#define XALAN_XPATH_EXPRESSION_USE_ITERATORS

#if defined(XALAN_XPATH_EXPRESSION_USE_ITERATORS)
    typedef OpCodeMapType::const_iterator   OpCodeMapPositionType;
#else
    typedef OpCodeMapSizeType               OpCodeMapPositionType;
#endif
    typedef OpCodeMapType::difference_type  OpCodeMapDifferenceType;
    typedef TokenQueueType::value_type      TokenQueueValueType;
    typedef int                             TokenQueueSizeType;
    typedef TokenQueueSizeType              TokenQueuePositionType;

    /**
     * List of operations codes.
     *
     * Code for the descriptions of the operations codes:
     * [UPPER CASE] indicates a literal value,
     * [lower case] is a description of a value,
     *      ([length] always indicates the length of the operation,
     *       including the operations code and the length integer.)
     * {UPPER CASE} indicates the given production,
     * {description} is the description of a new production,
     *      (For instance, {boolean expression} means some expression 
     *       that should be resolved to a boolean.)
     *  * means that it occurs zero or more times,
     *  + means that it occurs one or more times,
     *  ? means that it is optional.
     *
     * returns: indicates what the production should return.
     */
    enum eOpCodes
    {
        /**
         * [ELEMWILDCARD]
         * Means ELEMWILDCARD ("*"), used instead 
         * of string index in some places.
         */
        eELEMWILDCARD = -3,

        /**
         * [EMPTY]
         * Empty slot to indicate NULL.
         */
        eEMPTY = -2,

        /**
         * [ENDOP]
         * Some operators may like to have a terminator.
         */
        eENDOP = -1,

        /**
         * [OP_XPATH]
         * [length]
         *  {expression}
         * 
         * returns: 
         *  XNodeSet
         *  XNumber
         *  XString
         *  XBoolean
         *  XRTree
         *  XObject
         */
        eOP_XPATH = 1,

        /**
         * [OP_OR]
         * [length]
         *  {boolean expression}
         *  {boolean expression}
         * 
         * returns: 
         *  XBoolean
         */
        eOP_OR = 2,

        /**
         * [OP_AND]
         * [length]
         *  {boolean expression}
         *  {boolean expression}
         * 
         * returns: 
         *  XBoolean
         */
        eOP_AND = 3,

        /**
         * [OP_NOTEQUALS]
         * [length]
         *  {expression}
         *  {expression}
         * 
         * returns: 
         *  XBoolean
         */
        eOP_NOTEQUALS = 4,

        /**
         * [OP_EQUALS]
         * [length]
         *  {expression}
         *  {expression}
         * 
         * returns: 
         *  XBoolean
         */
        eOP_EQUALS = 5,

        /**
         * [OP_LTE] (less-than-or-equals)
         * [length]
         *  {number expression}
         *  {number expression}
         * 
         * returns: 
         *  XBoolean
         */
        eOP_LTE = 6,

        /**
         * [OP_LT] (less-than)
         * [length]
         *  {number expression}
         *  {number expression}
         * 
         * returns: 
         *  XBoolean
         */
        eOP_LT = 7,

        /**
         * [OP_GTE] (greater-than-or-equals)
         * [length]
         *  {number expression}
         *  {number expression}
         * 
         * returns: 
         *  XBoolean
         */
        eOP_GTE = 8,

        /**
         * [OP_GT] (greater-than)
         * [length]
         *  {number expression}
         *  {number expression}
         * 
         * returns: 
         *  XBoolean
         */
        eOP_GT = 9,

        /**
         * [OP_PLUS]
         * [length]
         *  {number expression}
         *  {number expression}
         * 
         * returns: 
         *  XNumber
         */
        eOP_PLUS = 10,

        /**
         * [OP_MINUS]
         * [length]
         *  {number expression}
         *  {number expression}
         * 
         * returns: 
         *  XNumber
         */
        eOP_MINUS = 11,

        /**
         * [OP_MULT]
         * [length]
         *  {number expression}
         *  {number expression}
         * 
         * returns: 
         *  XNumber
         */
        eOP_MULT = 12,

        /**
         * [OP_DIV]
         * [length]
         *  {number expression}
         *  {number expression}
         * 
         * returns: 
         *  XNumber
         */
        eOP_DIV = 13,

        /**
         * [OP_MOD]
         * [length]
         *  {number expression}
         *  {number expression}
         * 
         * returns: 
         *  XNumber
         */
        eOP_MOD = 14,

        /**
         * [OP_NEG]
         * [length]
         *  {number expression}
         * 
         * returns: 
         *  XNumber
         */
        eOP_NEG = 15,

        /**
         * [OP_BOOL] (cast operation)
         * [length]
         *  {expression}
         * 
         * returns: 
         *  XBoolean
         */
        eOP_BOOL = 16,

        /**
         * [OP_UNION]
         * [length]
         *  {PathExpr}+
         * 
         * returns: 
         *  XNodeSet
         */
        eOP_UNION = 17,

        /**
         * [OP_LITERAL]
         * [3]
         * [index to token]
         * 
         * returns: 
         *  XString
         */
        eOP_LITERAL = 18,

        /**
         * [OP_VARIABLE]
         * [3]
         * [index to token]
         * 
         * returns: 
         *  XString
         */
        eOP_VARIABLE = 19,

        /**
         * [OP_GROUP]
         * [length]
         *  {expression}
         * 
         * returns: 
         *  XNodeSet
         *  XNumber
         *  XString
         *  XBoolean
         *  XRTree
         *  XObject
         */
        eOP_GROUP = 20,

        /**
         * [OP_NUMBERLIT] (Number literal.)
         * [3]
         * [index to token]
         * 
         * returns: 
         *  XString
         */
        eOP_NUMBERLIT = 21,

        /**
         * [OP_ARGUMENT] (Function argument.)
         * [length]
         *  {expression}
         * 
         * returns: 
         *  XNodeSet
         *  XNumber
         *  XString
         *  XBoolean
         *  XRTree
         *  XObject
         */
        eOP_ARGUMENT = 22,

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
        eOP_EXTFUNCTION = 23,

        /**
         * [OP_FUNCTION]
         * [length]
         * [FUNC_ID]
         * [arg count]
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
        eOP_FUNCTION = 24,

        /**
         * [OP_LOCATIONPATH]
         * [length]
         *   {FROM_stepType}
         * | {function}{predicate}*
         * [ENDOP]
         * 
         * (Note that element and attribute namespaces and 
         * names can be wildcarded '*'.)
         * 
         * returns: 
         *  XNodeSet
         */
        eOP_LOCATIONPATH = 25,

        /**
         * [OP_PREDICATE]
         * [length]
         *  {expression}
         * [ENDOP] (For safety)
         * 
         * returns: 
         *  XBoolean or XNumber
         */
        eOP_PREDICATE = 26,

        /**
         * [NODETYPE_COMMENT]
         * No size or arguments.
         * 
         * returns: 
         *  XBoolean
         */
        eNODETYPE_COMMENT = 27,
        
        /**
         * [NODETYPE_TEXT]
         * No size or arguments.
         * 
         * returns: 
         *  XBoolean
         */
        eNODETYPE_TEXT = 28,
        
        /**
         * [NODETYPE_PI]
         * [index to token]
         * 
         * returns: 
         *  XBoolean
         */
        eNODETYPE_PI = 29,
        
        /**
         * [NODETYPE_NODE]
         * No size or arguments.
         * 
         * returns: 
         *  XBoolean
         */
        eNODETYPE_NODE = 30,
        
        /**
         * [NODENAME]
         * [index to ns token or EMPTY]
         * [index to name token]
         * 
         * returns: 
         *  XBoolean
         */
        eNODENAME = 31,
        
        /**
         * [NODETYPE_ROOT]
         * No size or arguments.
         * 
         * returns: 
         *  XBoolean
         */
        eNODETYPE_ROOT = 32,
        
        /**
         * [NODETYPE_ANY]
         * No size or arguments.
         * 
         * returns: 
         *  XBoolean
         */
        eNODETYPE_ANYELEMENT = 33,

        /**
         * [FROM_stepType]
         * [length, including predicates]
         * [length of just the step, without the predicates]
         * {node test}
         * {predicates}?
         * 
         * returns: 
         *  XBoolean
         */
        eFROM_ANCESTORS = 34,
        eFROM_ANCESTORS_OR_SELF = 35,
        eFROM_ATTRIBUTES = 36,
        eFROM_CHILDREN = 37,
        eFROM_DESCENDANTS = 38,
        eFROM_DESCENDANTS_OR_SELF = 39,
        eFROM_FOLLOWING = 40,
        eFROM_FOLLOWING_SIBLINGS = 41,
        eFROM_PARENT = 42,
        eFROM_PRECEDING = 43,
        eFROM_PRECEDING_SIBLINGS = 44,
        eFROM_SELF = 45,
        eFROM_NAMESPACE = 46,
        eFROM_ROOT = 47,

        /**
         * [OP_UNION]
         * [length]
         *  {PathExpr}+
         * 
         * returns: 
         *  XNodeSet
         */
        eOP_MATCHPATTERN = 48,

        /**
         * [OP_UNION]
         * [length]
         *  {PathExpr}+
         * 
         * returns: 
         *  XNodeSet
         */
        eOP_LOCATIONPATHPATTERN = 49,

        // For match patterns
        eMATCH_ATTRIBUTE = 50,
        eMATCH_ANY_ANCESTOR = 51,
        eMATCH_IMMEDIATE_ANCESTOR = 52,
        eMATCH_ANY_ANCESTOR_WITH_PREDICATE = 53,
        eMATCH_ANY_ANCESTOR_WITH_FUNCTION_CALL = 54,

        /**
         * [OP_PREDICATE_WITH_POSITION]
         * [length]
         *  {expression}
         * [ENDOP] (For safety)
         * 
         * returns: 
         *  XBoolean or XNumber
         */
        eOP_PREDICATE_WITH_POSITION = 55,
  
        /**
         * These are values for intrinsic functions which
         * have been compiled directly into the op map.
         */
        eOP_FUNCTION_POSITION = 56,
        eOP_FUNCTION_LAST = 57,
        eOP_FUNCTION_COUNT = 58,
        eOP_FUNCTION_NOT = 59,
        eOP_FUNCTION_TRUE = 60,
        eOP_FUNCTION_FALSE = 61,
        eOP_FUNCTION_BOOLEAN = 62,
        eOP_FUNCTION_NAME_0 = 63,
        eOP_FUNCTION_NAME_1 = 64,
        eOP_FUNCTION_LOCALNAME_0 = 65,
        eOP_FUNCTION_LOCALNAME_1 = 66,
        eOP_FUNCTION_FLOOR = 67,
        eOP_FUNCTION_CEILING = 68,
        eOP_FUNCTION_ROUND = 69,
        eOP_FUNCTION_NUMBER_0 = 70,
        eOP_FUNCTION_NUMBER_1 = 71,
        eOP_FUNCTION_STRING_0 = 72,
        eOP_FUNCTION_STRING_1 = 73,
        eOP_FUNCTION_STRINGLENGTH_0 = 74,
        eOP_FUNCTION_STRINGLENGTH_1 = 75,
        eOP_FUNCTION_NAMESPACEURI_0 = 76,
        eOP_FUNCTION_NAMESPACEURI_1 = 77,
        eOP_FUNCTION_SUM = 78,
        eOP_FUNCTION_CONCAT = 79,

        // Always add _before_ this one and update
        // s_opCodeLengthArray.
        eOpCodeNextAvailable
    };  // enum eOpCodes

    /**
     * Exception class thrown when an invalid XPath expression is encountered
     */
    class XALAN_XPATH_EXPORT XPathExpressionException : public XalanXPathException
    {
    public:

        /**
         * Construct an XPathExpressionException object.
         * 
         * @param theMessage string error message
         */
        XPathExpressionException(const XalanDOMString&  theMessage,
                                    MemoryManagerType& theManager);

        virtual~
        XPathExpressionException();
    };

    /**
     * Exception class thrown when an invalid XPath operation code is encountered
     */
    class XALAN_XPATH_EXPORT InvalidOpCodeException : public XPathExpressionException
    {
    public:

        /**
         * Construct an InvalidOpCodeException object.
         * 
         * @param theOpCode operation code that caused the exception
         */
        InvalidOpCodeException(OpCodeMapValueType   theOpCode,
                                XalanDOMString&     theResult);

        virtual~
        InvalidOpCodeException();

    private:

        static XalanDOMString&
        FormatErrorMessage(OpCodeMapValueType   theOpCode,
                            XalanDOMString&     theResult);
    };

    /**
     * Exception class thrown when an invalid number of XPath arguments is
     * encountered
     */
    class XALAN_XPATH_EXPORT InvalidArgumentCountException : public XPathExpressionException
    {
    public:

        /**
         * Construct an InvalidArgumentCountException object.
         * 
         * @param theOpCode operation code that caused the exception
         * @param theExpectedCount the correct number of arguments for "opcode"
         * @param theSuppliedCount the number of arguments supplied
         */
        InvalidArgumentCountException(
            OpCodeMapValueType  theOpCode,
            OpCodeMapValueType  theExpectedCount,
            OpCodeMapValueType  theSuppliedCount,
            XalanDOMString&     theResult);

        virtual~
        InvalidArgumentCountException();

    private:

        static XalanDOMString&
        FormatErrorMessage(
            OpCodeMapValueType  theOpCode,
            OpCodeMapValueType  theExpectedCount,
            OpCodeMapValueType  theSuppliedCount,
            XalanDOMString&     theResult);
    };

    /**
     * Exception class thrown when an invalid XPath argument is encountered
     */
    class XALAN_XPATH_EXPORT InvalidArgumentException : public XPathExpressionException
    {
    public:

        /**
         * Construct an InvalidArgumentException object.
         * 
         * @param theOpCode operation code that caused the exception
         * @param theValue invalid argument value
         */
        InvalidArgumentException(
            OpCodeMapValueType  theOpCode,
            OpCodeMapValueType  theValue,
            XalanDOMString&     theResult);

        virtual~
        InvalidArgumentException();

    private:

        static XalanDOMString&
        FormatErrorMessage(
                OpCodeMapValueType  theOpCode,
                OpCodeMapValueType  theValue,
                XalanDOMString&     theResult);
    };


    /**
     * The length is always the opcode position + 1. Length is always expressed
     * as the opcode+length bytes, so it is always 2 or greater.  This is the
     * offset from the op code where the length is stored.  It will always
     * remain one.
     */
#if defined(XALAN_INLINE_INITIALIZATION)
    static const TokenQueueSizeType     s_opCodeMapLengthIndex = 1;
#else
    enum eDummy
    {
        s_opCodeMapLengthIndex = 1
    };
#endif

    explicit
    XPathExpression(MemoryManagerType& theManager);

    ~XPathExpression();

    MemoryManagerType&
    getMemoryManager()
    {
        return m_opMap.getMemoryManager();
    }
    /**
     * Reset the expression.
     */
    void
    reset();

    /**
     * Shrink internal tables.
     */
    void
    shrink();

    /**
     * Retrieve number of elements in the operations code map.
     * 
     * @return size of operations code map
     */
    OpCodeMapSizeType
    opCodeMapSize() const
    {
        return OpCodeMapSizeType(m_opMap.size());
    }

    /**
     * Retrieve length of the operations code map stored in the map. The length
     * of the entire map is stored after the first op code.  That offset is
     * determined by this const static member.  Note that as expressions are
     * defined recursively, this is really just the length of the first
     * expression in the map, which is the top of the parse tree. Any
     * subexpression will also have a length entry at the same offset from the
     * beginning of the subexpression.
     * 
     * @return length of operations code map
     */
    OpCodeMapValueType
    opCodeMapLength() const
    {
        const OpCodeMapSizeType     theSize = opCodeMapSize();

        if (theSize > s_opCodeMapLengthIndex)
        {
            assert(theSize == OpCodeMapSizeType(m_opMap[s_opCodeMapLengthIndex]));

            return m_opMap[s_opCodeMapLengthIndex];
        }
        else
        {
            assert(theSize == OpCodeMapValueType(theSize));

            return OpCodeMapValueType(theSize);
        }
    }

    OpCodeMapPositionType
    getInitialOpCodePosition() const
    {
#if defined(XALAN_XPATH_EXPRESSION_USE_ITERATORS)
        return m_opMap.begin();
#else
        return 0;
#endif
    }

    bool
    isValidOpCodePosition(OpCodeMapPositionType     opPos) const
    {
        const OpCodeMapDifferenceType  theDifference =
            OpCodeMapDifferenceType(opPos - getInitialOpCodePosition());

        return theDifference >= 0 && 
               theDifference < opCodeMapSize();
    }

#if defined(XALAN_XPATH_EXPRESSION_USE_ITERATORS)
    bool
    isValidOpCodePosition(OpCodeMapSizeType     theIndex) const
    {
        return theIndex >= 0 && theIndex < opCodeMapSize();
    }

    /**
     * Retrieve the value of an operation code at a specified index in the
     * op code map.
     * 
     * @param theIndex The index in list
     * @return value of operation code
     */
    OpCodeMapValueType
    getOpCodeMapValue(OpCodeMapSizeType     theIndex) const
    {
        assert(theIndex < opCodeMapLength());

        return m_opMap[theIndex];
    }
#endif

    /**
     * Retrieve the value of an operation code at a specified position in the
     * list.
     * 
     * @param opPos position in list
     * @return value of operation code
     */
    OpCodeMapValueType
    getOpCodeMapValue(OpCodeMapPositionType     opPos) const
    {
        assert(opPos < getInitialOpCodePosition() + opCodeMapLength());

#if defined(XALAN_XPATH_EXPRESSION_USE_ITERATORS)
        return *opPos;
#else

        return m_opMap[opPos];
#endif
    }

    /**
     * Set the value of an operation code at a specified index in the
     * OpCode map.
     * 
     * @param theOpCodeMapIndex The index in the OpCode map
     * @param theValue value of operation code
     */
    void
    setOpCodeMapValue(
            OpCodeMapSizeType           theOpCodeMapIndex,
            const OpCodeMapValueType&   theValue)
    {
        assert(theOpCodeMapIndex < opCodeMapLength());

        m_opMap[theOpCodeMapIndex] = theValue;
    }

    OpCodeMapValueType
    getOpCodeArgumentLength(OpCodeMapPositionType   opPos) const
    {
        return getOpCodeMapValue(opPos + XPathExpression::s_opCodeMapLengthIndex + 1) - 3;
    }

    /**
     * Retrieve the length of an operation code at a specified position in the
     * op map.
     * 
     * @param opPos position in the op map
     * @return length of operation code
     */
    OpCodeMapValueType
    getOpCodeLengthFromOpMap(OpCodeMapPositionType  opPos,
                             MemoryManagerType&     theManager) const;

#if defined(XALAN_XPATH_EXPRESSION_USE_ITERATORS)
    /**
     * Retrieve the length of an operation code at a specified index in the
     * op map.
     * 
     * @param theIndex The index in the op map
     * @return length of operation code
     */
    OpCodeMapValueType
    getOpCodeLengthFromOpMap(OpCodeMapSizeType      theIndex,
                                MemoryManagerType& theManager) const;
#endif

#if defined(XALAN_XPATH_EXPRESSION_USE_ITERATORS)
    /**
     * Retrieve the position of the next operation code at a specified position
     * in the list.
     * 
     * @param opPos position in list
     * @return position of next operation code
     */
    OpCodeMapPositionType
    getNextOpCodePosition(OpCodeMapPositionType     opPos) const
    {
        assert(opPos < getInitialOpCodePosition() + opCodeMapLength());

        return opPos + *(opPos + s_opCodeMapLengthIndex);
    }
#endif

    /**
     * Retrieve the position of the next operation code at a specified index
     * in the list.
     * 
     * @param theIndex theIndex in list
     * @return position of next operation code
     */
    OpCodeMapSizeType
#if defined(XALAN_XPATH_EXPRESSION_USE_ITERATORS)
    getNextOpCodePosition(OpCodeMapSizeType     theIndex) const
#else
    getNextOpCodePosition(OpCodeMapPositionType     theIndex) const
#endif
    {
        assert(theIndex < opCodeMapLength());

        assert(theIndex + m_opMap[theIndex + s_opCodeMapLengthIndex] ==
               OpCodeMapSizeType(theIndex + m_opMap[theIndex + s_opCodeMapLengthIndex]));

        return OpCodeMapSizeType(theIndex + m_opMap[theIndex + s_opCodeMapLengthIndex]);
    }

    /**
     * Set the arguments for an operation code at a specified index in the
     * list.
     * 
     * @param opPos position in list
     * @param theOpCode operation code
     * @param theIndex  index in list
     * @param theArgs   vector or arguments to supply
     */
    void
    setOpCodeArgs(
            eOpCodes                            theOpCode,
            OpCodeMapSizeType                   theIndex,
            const OpCodeMapValueVectorType&     theArgs);

    /**
     * Add an operation code to the list.
     * 
     * @param theOpCode operation code
     * @return the position of the op code
     */
    OpCodeMapSizeType
    appendOpCode(eOpCodes   theOpCode);

    /**
     * Add an operation code with supplied arguments to the list.
     * 
     * @param theOpCode operation code
     * @param theArgs   vector or arguments to supply
     */
    OpCodeMapSizeType
    appendOpCode(
            eOpCodes                            theOpCode,
            const OpCodeMapValueVectorType&     theArgs)
    {
        const OpCodeMapSizeType     thePosition = appendOpCode(theOpCode);

        setOpCodeArgs(theOpCode,
                      thePosition,
                      theArgs);

        return thePosition;
    }

    /**
     * Replace an operation code with supplied code.
     * 
     * @param theIndex  The index of the old operation code
     * @param theOldOpCode The old operation code
     * @param theNewOpCode The new operation code
     */
    void
    replaceOpCode(
            OpCodeMapSizeType   theIndex,
            eOpCodes            theOldOpCode,
            eOpCodes            theNewOpCode);

    /**
     * Insert an operation code at a specified index in the list.
     * 
     * @param theOpCode operation code
     * @param theIndex  index in list
     */
    OpCodeMapValueType
    insertOpCode(
            eOpCodes            theOpCode,
            OpCodeMapSizeType   theIndex);

    /**
     * Update the length of an operation code at a specified index in the list.
     * This presumes that the other opcodes have been appended to the
     * expression, and that the specified op code's length needs to be set.
     * The size includes the normal length of the opcode, plus the length of
     * its subexpressions.
     * 
     * @param theIndex  index in list
     */
    void
    updateOpCodeLength(OpCodeMapSizeType    theIndex)
    {
        assert(theIndex < opCodeMapSize());

        updateOpCodeLength(m_opMap[theIndex], theIndex);
    }

    /**
     * Update the length of an operation code that has moved to a new index in
     * the list.
     * 
     * @param theOpCode        operation code
     * @param theOriginalIndex original index in list
     * @param theNewIndex      new index in list
     */
    void
    updateShiftedOpCodeLength(
            OpCodeMapValueType  theOpCode,
            OpCodeMapSizeType   theOriginalIndex,
            OpCodeMapSizeType   theNewIndex);

    /**
     * Update the length of an operation code at a specified index in the list.
     * This presumes that the other opcodes have been appended to the
     * expression, and that the specified op code's length needs to be set.
     * The size includes the normal length of the opcode, plus the length of
     * its subexpressions.
     * 
     * @param theOpCode operation code at specified index
     * @param theIndex  index in list
     */
    void
    updateOpCodeLength(
            OpCodeMapValueType  theOpCode,
            OpCodeMapSizeType   theIndex);

    /**
     * Whether the operation code is one of the node test types, for example,
     * "ancestor::" or "child::"
     *
     * @param theOpCode operation code
     * @return true if code represents a node test
     */
    static bool
    isNodeTestOpCode(OpCodeMapValueType     theOpCode);

    /**
     * Update the length of an operation code after a node test code.
     * 
     * @param theIndex  index in list
     */
    void
    updateOpCodeLengthAfterNodeTest(OpCodeMapSizeType   theIndex);

    /**
     * Whether there are any more tokens in the token queue.
     *
     * @return true if there are more tokens
     */
    bool
    hasMoreTokens() const
    {
        return tokenQueueSize() > m_currentPosition ? true : false;
    }

    /**
     * Retrieve number of elements in the token queue.
     * 
     * @return size of token queue
     */
    TokenQueueSizeType
    tokenQueueSize() const
    {
        return TokenQueueSizeType(m_tokenQueue.size());
    }

    bool
    isValidTokenQueuePosition(TokenQueueSizeType    thePosition) const
    {
        return thePosition < tokenQueueSize();
    }

    /**
     * Retrieve the current position in the token queue.
     *
     * @return position in queue
     */
    TokenQueueSizeType
    getTokenPosition() const
    {
        return m_currentPosition;
    }

    /**
     * Set the current position in the token queue to zero.
     */
    void
    resetTokenPosition()
    {
        m_currentPosition = 0;
    }

    /**
     * Retrieve a token at the specified position in the token queue.
     * 
     * @param thePosition position in queue
     * @return pointer to XObject token
     */
    const XToken*
    getToken(TokenQueuePositionType     thePosition) const
    {
        assert(thePosition < tokenQueueSize());

        return &m_tokenQueue[thePosition];
    }

    /**
     * Retrieve the next token in the token queue.
     * 
     * @return pointer to XObject token
     */
    const XToken*
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

    /**
     * Retrieve the previous token in the token queue.
     * 
     * @return pointer to XObject token
     */
    const XToken*
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

    enum eRelativeDirection
    {
        eRelativeBackward,
        eRelativeForward
    };

    /**
     * Retrieve a token at the specified offset relative to the current
     * position in the token queue.
     * 
     * @param theOffset offset from current position
     * @param theDirection the direction in which to move
     * @return pointer to XObject token
     */
    const XToken*
    getRelativeToken(
        TokenQueuePositionType  theOffset,
        eRelativeDirection      theDirection) const
    {
        const TokenQueuePositionType    thePosition =
            calculateRelativePosition(theOffset, theDirection);

        if (thePosition == tokenQueueSize())
        {
            return 0;
        }
        else
        {
            return getToken(thePosition);
        }
    }

    /**
     * Push a token onto the token queue.
     * 
     * @param theToken the string value to push
     */
    void
    pushToken(const XalanDOMString&     theToken)
    {
        m_tokenQueue.push_back(XToken(theToken, getMemoryManager()));
    }

    /**
     * Push a token onto the token queue.
     * 
     * @param theNumber the number value to push
     * @param theString the string value to push
     */
    void
    pushToken(
            double                  theNumber,
            const XalanDOMString&   theString)
    {
        m_tokenQueue.push_back(XToken(theNumber, theString));
    }

    /**
     * Insert a token onto the token queue at the
     * current position.
     *
     * @param theToken the string value to push
     */
    void
    insertToken(const XalanDOMString&   theToken)
    {
        m_tokenQueue.insert(m_tokenQueue.begin() + (m_currentPosition - 1), XToken(theToken, getMemoryManager()));
    }

    /**
     * Insert a token onto the token queue at the
     * current position.
     *
     * @param theNumber the number value to push
     * @param theString the string value to push
     */
    void
    insertToken(
            double                  theNumber,
            const XalanDOMString&   theString)
    {
        m_tokenQueue.insert(m_tokenQueue.begin() + (m_currentPosition - 1), XToken(theNumber, theString));
    }

    /**
     * Replace a token in the token queue.
     * 
     * @param theOffset the offset at which to replace the token.
     * @param theString The string data for the token.  The instance will keep a pointer to this string, so it must be persistent.
     */
    void
    replaceRelativeToken(
            TokenQueuePositionType  theOffset,
            eRelativeDirection      theDirection,
            const XalanDOMString&   theString)
    {
        const TokenQueuePositionType    thePosition =
            calculateRelativePosition(theOffset, theDirection);
        assert(thePosition < tokenQueueSize());

        m_tokenQueue[thePosition].set(theString, getMemoryManager());
    }

    /**
     * Diagnostic function to output the operation code map.
     * 
     * @param thePrintWriter   output device
     * @param theStartPosition starting position in map
     */
    void
    dumpOpCodeMap(
            PrintWriter&        thePrintWriter,
            OpCodeMapSizeType   theStartPosition = 0) const;

    /**
     * Diagnostic function to output the operation code map.
     * 
     * @param theStream output stream
     * @param theStartPosition starting position in map
     */
    void
    dumpOpCodeMap(
            OstreamType&        theStream,
            OpCodeMapSizeType   theStartPosition = 0) const;

    /**
     * Diagnostic function to output the token queue.
     * 
     * @param thePrintWriter   output device
     * @param theStartPosition starting position in token queue
     */
    void
    dumpTokenQueue(
            PrintWriter&        thePrintWriter,
            TokenQueueSizeType  theStartPosition = 0) const;

    /**
     * Diagnostic function to output the token queue.
     * 
     * @param thePrintWriter   output device
     * @param theStartPosition starting position in token queue
     */
    void
    dumpTokenQueue(
            OstreamType&        theStream,
            TokenQueueSizeType  theStartPosition = 0) const;

    /**
     * Diagnostic function to output the remaining tokens in the token queue.
     * 
     * @param thePrintWriter   output device
     */
    void
    dumpRemainingTokenQueue(PrintWriter&    thePrintWriter) const;

    /**
     * Diagnostic function to output the remaining tokens in the token queue.
     * 
     * @param theStream The output stream
     * @param theMemoryManager The MemoryManager instance.
     */
    void
    dumpRemainingTokenQueue(
            OstreamType&    theStream,
            MemoryManager&  theMemoryManager) const;

    /**
     * Push a value onto the operations code
     * map.
     *
     * @param theToken string value of the token to push
     */
    void
    pushValueOnOpCodeMap(const OpCodeMapType::value_type&   theValue)
    {
        // Push the index onto the op map.
        m_opMap.push_back(theValue);

        // Update the op map length.
        ++m_opMap[s_opCodeMapLengthIndex];
    }

    /**
     * Push a token onto the token queue and its index onto the operations code
     * map.
     *
     * @param theXToken the XToken to push
     */
    void
    pushArgumentOnOpCodeMap(const XToken&   theXToken);

    /**
     * Push a token onto the token queue and its index onto the operations code
     * map.
     *
     * @param theString The string data for the token.  The instance will keep a pointer to this string, so it must be persistent.
     */
    void
    pushArgumentOnOpCodeMap(const XalanDOMString&   theString);

    /**
     * Push a token onto the token queue and its index onto the operations code
     * map.
     *
     * @param theNumber The numeric data for the token.  This must be consistent with the lexical value in theString.
     * @param theString The string data for the token.  The instance will keep a pointer to this string, so it must be persistent.
     */
    void
    pushArgumentOnOpCodeMap(
            double                  theNumber,
            const XalanDOMString&   theString);

    /**
     * Push a number literal onto the vector of number literals and its index onto
     * the operations code map.
     *
     * @param theToken number value of the token to push
     */
    void
    pushNumberLiteralOnOpCodeMap(double     theNumber);

    /**
     * Get a number literal from the vector of number literals.
     *
     * @param theIndex The index of the desired value.
     */
    double
    getNumberLiteral(int    theIndex) const
    {
        assert(theIndex >= 0 &&
               NumberLiteralValueVectorType::size_type(theIndex) < m_numberLiteralValues.size());

        return m_numberLiteralValues[NumberLiteralValueVectorType::size_type(theIndex)];
    }

    /**
     * Push the current position in the token queue onto the operations code
     * map.
     */
    void
    pushCurrentTokenOnOpCodeMap();

    /**
     * Change the current pattern in the pattern map.
     *
     * @param thePattern match pattern to make current
     */
    void
    setCurrentPattern(const XalanDOMString&     thePattern)
    {
        m_currentPattern = &thePattern;
    }

    /**
     * Retrieve the current pattern in the pattern map.
     * 
     * @return string for current match pattern
     */
    const XalanDOMString&
    getCurrentPattern() const
    {
        assert(m_currentPattern != 0);

        return *m_currentPattern;
    }

private:

    /**
     * Calculate the relative token position given the offset
     * and direction.  Returns the size of the token queue
     * if the offset is not valid.
     *
     * @param theOffset offset from current position
     * @param theDirection the direction in which to move
     * @return thePosition
     */
    TokenQueuePositionType
    calculateRelativePosition(
        TokenQueuePositionType  theOffset,
        eRelativeDirection      theDirection) const
    {
        if (theDirection == eRelativeBackward &&
            theOffset <= m_currentPosition)
        {
            return m_currentPosition - theOffset;
        }
        else if (theDirection == eRelativeForward &&
                 m_currentPosition + theOffset < tokenQueueSize())
        {
            return m_currentPosition + theOffset;
        }
        else
        {
            return tokenQueueSize();
        }
    }

    /**
     * An operations map is used instead of a proper parse tree.  It contains
     * operations codes and indexes into the m_tokenQueue. We use an array
     * instead of a full parse tree in order to cut down on the number of
     * objects created.
     */
    OpCodeMapType           m_opMap;

    /**
     * The index of the last opcode that was appended or inserted.
     *
     */
    OpCodeMapSizeType       m_lastOpCodeIndex;

    /**
     * The queue of used tokens. The current token is the token at the end of
     * the m_tokenQueue. The idea is that the queue can be marked and a
     * sequence of tokens can be reused.
     */
    TokenQueueType          m_tokenQueue;

    /**
     *  The current position in the token queue.
     */
    TokenQueueSizeType      m_currentPosition;

    /**
     * The current pattern string, for diagnostics purposes.
     */
    const XalanDOMString*   m_currentPattern;

    // Default vector allocation sizes.
    enum
    {
        eDefaultOpMapSize = 100,
        eDefaultTokenQueueSize = 30
    };

    NumberLiteralValueVectorType    m_numberLiteralValues;
};



XALAN_CPP_NAMESPACE_END



#endif  // XPATHEXPRESSION_HEADER_GUARD_1357924680
