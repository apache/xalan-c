/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the  "License");
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
#include "XPath.hpp"



#include <cassert>
#include <cfloat>



#include <xalanc/Include/STLHelper.hpp>



#include <xalanc/XalanDOM/XalanNamedNodeMap.hpp>



#include <xalanc/PlatformSupport/DoubleSupport.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>
#include <xalanc/PlatformSupport/PrefixResolver.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "FormatterStringLengthCounter.hpp"
#include "MutableNodeRefList.hpp"
#include "XalanQNameByReference.hpp"
#include "XObject.hpp"
#include "XObjectFactory.hpp"
#include "XPathConstructionContext.hpp"
#include "XPathEnvSupport.hpp"



XALAN_CPP_NAMESPACE_BEGIN




const XalanDOMString    XPath::s_emptyString(XalanMemMgrs::getDummyMemMgr());



XPath::XPath(
            MemoryManager&  theManager,
            const Locator*  theLocator) :
    m_expression(theManager),
    m_locator(theLocator),
    m_inStylesheet(false)
{
}



XPath*
XPath::create(
            MemoryManager&  theManager,
            const Locator*  theLocator)
{
    typedef XPath   ThisType;

    XalanAllocationGuard    theGuard(theManager, theManager.allocate(sizeof(ThisType)));

    ThisType* const     theResult =
        new (theGuard.get()) ThisType(
                                theManager,
                                theLocator);

    theGuard.release();

    return theResult;
}



XPath::~XPath()
{
}



void
XPath::unknownOpCodeError(
            XalanNode*              context,
            XPathExecutionContext&  executionContext,
            OpCodeMapPositionType   opPos) const
{
    const GetCachedString   theGuard1(executionContext);

    XalanDOMString&     theOpCode = theGuard1.get();

    NumberToDOMString(
        m_expression.getOpCodeMapValue(opPos),
        theOpCode);

    const GetCachedString   theGuard2(executionContext);

    executionContext.problem(
        XPathExecutionContext::eXPath,
        XPathExecutionContext::eError,
        XalanMessageLoader::getMessage(
            theGuard2.get(),
            XalanMessages::InvalidOpcodeWasDetected_1Param,
            theOpCode),
        m_locator,
        context);
}



void
XPath::notNodeSetError(
            XalanNode*              context,
            XPathExecutionContext&  executionContext) const
{
    GetCachedString     theGuard(executionContext);

    XalanDOMString&     theBuffer = theGuard.get();

    executionContext.problem(
        XPathExecutionContext::eXPath,
        XPathExecutionContext::eError,
        XalanMessageLoader::getMessage(
            theBuffer,
            XalanMessages::ExpressionDoesNotEvaluateToNodeSet),
        m_locator,
        context);
}



const XObjectPtr
XPath::execute(
            XalanNode*              context,
            const PrefixResolver&   prefixResolver,
            XPathExecutionContext&  executionContext) const
{
    // Push and pop the PrefixResolver...
    const PrefixResolverSetAndRestore   theResolverSetAndRestore(
                                    executionContext,
                                    &prefixResolver);

    // Push and pop the current node...
    const CurrentNodePushAndPop     theNodePushAndPop(
                                    executionContext,
                                    context);

    return executeMore(
            context,
            getInitialOpCodePosition(),
            executionContext);
}



void
XPath::execute(
            XalanNode*              context,
            const PrefixResolver&   prefixResolver,
            XPathExecutionContext&  executionContext,
            bool&                   result) const
{
    assert(context != 0);

    // Push and pop the PrefixResolver...
    const PrefixResolverSetAndRestore   theResolverSetAndRestore(
                                    executionContext,
                                    &prefixResolver);

    // Push and pop the current node...
    const CurrentNodePushAndPop theNodePushAndPop(
                                    executionContext,
                                    context);

    executeMore(
            context,
            getInitialOpCodePosition(),
            executionContext,
            result);
}



void
XPath::execute(
            XalanNode*              context,
            const PrefixResolver&   prefixResolver,
            XPathExecutionContext&  executionContext,
            double&                 result) const
{
    assert(context != 0);

    // Push and pop the PrefixResolver...
    const PrefixResolverSetAndRestore   theResolverSetAndRestore(
                                    executionContext,
                                    &prefixResolver);

    // Push and pop the current node...
    const CurrentNodePushAndPop theNodePushAndPop(
                                    executionContext,
                                    context);

    executeMore(
            context,
            getInitialOpCodePosition(),
            executionContext,
            result);
}



void
XPath::execute(
            XalanNode*              context,
            const PrefixResolver&   prefixResolver,
            XPathExecutionContext&  executionContext,
            XalanDOMString&         result) const
{
    assert(context != 0);

    // Push and pop the PrefixResolver...
    const PrefixResolverSetAndRestore   theResolverSetAndRestore(
                                    executionContext,
                                    &prefixResolver);

    // Push and pop the current node...
    const CurrentNodePushAndPop theNodePushAndPop(
                                    executionContext,
                                    context);

    executeMore(
            context,
            getInitialOpCodePosition(),
            executionContext,
            result);
}



void
XPath::execute(
            XalanNode*              context,
            const PrefixResolver&   prefixResolver,
            XPathExecutionContext&  executionContext,
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function) const
{
    assert(context != 0);

    // Push and pop the PrefixResolver...
    const PrefixResolverSetAndRestore   theResolverSetAndRestore(
                                    executionContext,
                                    &prefixResolver);

    // Push and pop the current node...
    const CurrentNodePushAndPop theNodePushAndPop(
                                    executionContext,
                                    context);

    executeMore(
            context,
            getInitialOpCodePosition(),
            executionContext,
            formatterListener,
            function);
}



const XObjectPtr
XPath::execute(
            XalanNode*              context,
            const PrefixResolver&   prefixResolver,
            XPathExecutionContext&  executionContext,
            MutableNodeRefList&     result) const
{
    assert(context != 0);
    assert(result.empty() == true);

    // Push and pop the PrefixResolver...
    const PrefixResolverSetAndRestore   theResolverSetAndRestore(
                                            executionContext,
                                            &prefixResolver);

    // Push and pop the current node...
    const CurrentNodePushAndPop     theNodePushAndPop(
                                        executionContext,
                                        context);

    return executeMore(
            context,
            getInitialOpCodePosition(),
            executionContext,
            result);
}



const XObjectPtr
XPath::executeMore(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext) const
{
    switch(m_expression.getOpCodeMapValue(opPos))
    {
    case XPathExpression::eOP_OR:
        return executionContext.getXObjectFactory().createBoolean(Or(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_AND:
        return executionContext.getXObjectFactory().createBoolean(And(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_NOTEQUALS:
        return executionContext.getXObjectFactory().createBoolean(notequals(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_EQUALS:
        return executionContext.getXObjectFactory().createBoolean(equals(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_LTE:
        return executionContext.getXObjectFactory().createBoolean(lte(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_LT:
        return executionContext.getXObjectFactory().createBoolean(lt(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_GTE:
        return executionContext.getXObjectFactory().createBoolean(gte(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_GT:
        return executionContext.getXObjectFactory().createBoolean(gt(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_PLUS:
        return executionContext.getXObjectFactory().createNumber(plus(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_MINUS:
        return executionContext.getXObjectFactory().createNumber(minus(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_MULT:
        return executionContext.getXObjectFactory().createNumber(mult(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_DIV:
        return executionContext.getXObjectFactory().createNumber(div(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_MOD:
        return executionContext.getXObjectFactory().createNumber(mod(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_NEG:
        return executionContext.getXObjectFactory().createNumber(neg(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_UNION:
        return Union(context, opPos, executionContext);
        break;

    case XPathExpression::eOP_LITERAL:
        return literal(opPos, executionContext);
        break;

    case XPathExpression::eOP_VARIABLE:
        return variable(opPos, executionContext);
        break;

    case XPathExpression::eOP_GROUP:
        return group(context, opPos, executionContext);
        break;

    case XPathExpression::eOP_NUMBERLIT:
        return numberlit(opPos, executionContext);
        break;

    case XPathExpression::eOP_EXTFUNCTION:
        return runExtFunction(context, opPos, executionContext);
        break;

    case XPathExpression::eOP_FUNCTION:
        return runFunction(context, opPos, executionContext);
        break;

    case XPathExpression::eOP_LOCATIONPATH:
        return locationPath(context, opPos, executionContext);
        break;

    case XPathExpression::eOP_FUNCTION_POSITION:
        return executionContext.getXObjectFactory().createNumber(functionPosition(context, executionContext));
        break;

    case XPathExpression::eOP_FUNCTION_LAST:
        return executionContext.getXObjectFactory().createNumber(functionLast(executionContext));
        break;

    case XPathExpression::eOP_FUNCTION_COUNT:
        return executionContext.getXObjectFactory().createNumber(functionCount(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_FUNCTION_NOT:
        return executionContext.getXObjectFactory().createBoolean(functionNot(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_FUNCTION_TRUE:
        return executionContext.getXObjectFactory().createBoolean(true);
        break;

    case XPathExpression::eOP_FUNCTION_FALSE:
        return executionContext.getXObjectFactory().createBoolean(false);
        break;

    case XPathExpression::eOP_FUNCTION_BOOLEAN:
        return executionContext.getXObjectFactory().createBoolean(functionBoolean(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_FUNCTION_NAME_0:
        return executionContext.getXObjectFactory().createStringReference(functionName(context));
        break;

    case XPathExpression::eOP_FUNCTION_NAME_1:
        return executionContext.getXObjectFactory().createStringReference(functionName(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_FUNCTION_LOCALNAME_0:
        return executionContext.getXObjectFactory().createStringReference(functionLocalName(context));
        break;

    case XPathExpression::eOP_FUNCTION_LOCALNAME_1:
        return executionContext.getXObjectFactory().createStringReference(functionLocalName(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_FUNCTION_FLOOR:
        return executionContext.getXObjectFactory().createNumber(functionFloor(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_FUNCTION_CEILING:
        return executionContext.getXObjectFactory().createNumber(functionCeiling(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_FUNCTION_ROUND:
        return executionContext.getXObjectFactory().createNumber(functionRound(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_FUNCTION_NUMBER_0:
        return executionContext.getXObjectFactory().createNumber(functionNumber(context, executionContext));
        break;

    case XPathExpression::eOP_FUNCTION_NUMBER_1:
        return executionContext.getXObjectFactory().createNumber(functionNumber(context, opPos, executionContext));
        break;

//  case XPathExpression::eOP_FUNCTION_STRING_0:
//      return functionString(context);
//      break;

//  case XPathExpression::eOP_FUNCTION_STRING_1:
//      return functionString(context, opPos, executionContext);
//      break;

    case XPathExpression::eOP_FUNCTION_STRINGLENGTH_0:
        return executionContext.getXObjectFactory().createNumber(functionStringLength(context, executionContext));
        break;

    case XPathExpression::eOP_FUNCTION_STRINGLENGTH_1:
        return executionContext.getXObjectFactory().createNumber(functionStringLength(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_FUNCTION_SUM:
        return executionContext.getXObjectFactory().createNumber(functionSum(context, opPos, executionContext));
        break;

    default:
        unknownOpCodeError(context, executionContext, opPos);
        break;
    }

    return XObjectPtr();
}



void
XPath::executeMore(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext,
            bool&                   result) const
{
    switch(m_expression.getOpCodeMapValue(opPos))
    {
    case XPathExpression::eOP_OR:
        result = Or(context, opPos, executionContext);
        break;

    case XPathExpression::eOP_AND:
        result = And(context, opPos, executionContext);
        break;

    case XPathExpression::eOP_NOTEQUALS:
        result = notequals(context, opPos, executionContext);
        break;

    case XPathExpression::eOP_EQUALS:
        result = equals(context, opPos, executionContext);
        break;

    case XPathExpression::eOP_LTE:
        result = lte(context, opPos, executionContext);
        break;

    case XPathExpression::eOP_LT:
        result = lt(context, opPos, executionContext);
        break;

    case XPathExpression::eOP_GTE:
        result = gte(context, opPos, executionContext);
        break;

    case XPathExpression::eOP_GT:
        result = gt(context, opPos, executionContext);
        break;

    case XPathExpression::eOP_PLUS:
        result = XObject::boolean(plus(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_MINUS:
        result = XObject::boolean(minus(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_MULT:
        result = XObject::boolean(mult(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_DIV:
        result = XObject::boolean(div(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_MOD:
        result = XObject::boolean(mod(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_NEG:
        result = XObject::boolean(neg(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_UNION:
        Union(context, opPos, executionContext, result);
        break;

    case XPathExpression::eOP_LITERAL:
        literal(opPos, result);
        break;

    case XPathExpression::eOP_VARIABLE:
        result = variable(opPos, executionContext)->boolean(executionContext);
        break;

    case XPathExpression::eOP_GROUP:
        group(context, opPos, executionContext, result);
        break;

    case XPathExpression::eOP_NUMBERLIT:
        numberlit(opPos, result);
        break;

    case XPathExpression::eOP_EXTFUNCTION:
        result = runExtFunction(context, opPos, executionContext)->boolean(executionContext);
        break;

    case XPathExpression::eOP_FUNCTION:
        result = runFunction(context, opPos, executionContext)->boolean(executionContext);
        break;

    case XPathExpression::eOP_LOCATIONPATH:
        locationPath(context, opPos, executionContext, result);
        break;

    case XPathExpression::eOP_FUNCTION_POSITION:
        result = XObject::boolean(functionPosition(context, executionContext));
        break;

    case XPathExpression::eOP_FUNCTION_LAST:
        result = XObject::boolean(functionLast(executionContext));
        break;

    case XPathExpression::eOP_FUNCTION_COUNT:
        result = XObject::boolean(functionCount(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_FUNCTION_NOT:
        result = functionNot(context, opPos, executionContext);
        break;

    case XPathExpression::eOP_FUNCTION_TRUE:
        result = true;
        break;

    case XPathExpression::eOP_FUNCTION_FALSE:
        result = false;
        break;

    case XPathExpression::eOP_FUNCTION_BOOLEAN:
        result = functionBoolean(context, opPos, executionContext);
        break;

    case XPathExpression::eOP_FUNCTION_NAME_0:
        result = XObject::boolean(functionName(context));
        break;

    case XPathExpression::eOP_FUNCTION_NAME_1:
        result = XObject::boolean(functionName(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_FUNCTION_LOCALNAME_0:
        result = XObject::boolean(functionLocalName(context));
        break;

    case XPathExpression::eOP_FUNCTION_LOCALNAME_1:
        result = XObject::boolean(functionLocalName(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_FUNCTION_FLOOR:
        result = XObject::boolean(functionFloor(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_FUNCTION_CEILING:
        result = XObject::boolean(functionCeiling(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_FUNCTION_ROUND:
        result = XObject::boolean(functionRound(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_FUNCTION_NUMBER_0:
        result = XObject::boolean(functionNumber(context, executionContext));
        break;

    case XPathExpression::eOP_FUNCTION_NUMBER_1:
        result = XObject::boolean(functionNumber(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_FUNCTION_STRINGLENGTH_0:
        result = XObject::boolean(functionStringLength(context, executionContext));
        break;

    case XPathExpression::eOP_FUNCTION_STRINGLENGTH_1:
        result = XObject::boolean(functionStringLength(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_FUNCTION_SUM:
        result = XObject::boolean(functionSum(context, opPos, executionContext));
        break;

    default:
        unknownOpCodeError(context, executionContext, opPos);
        break;
    }
}



void
XPath::executeMore(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext,
            double&                 result) const
{
    switch(m_expression.getOpCodeMapValue(opPos))
    {
    case XPathExpression::eOP_OR:
        result = XObject::number(Or(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_AND:
        result = XObject::number(And(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_NOTEQUALS:
        result = XObject::number(notequals(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_EQUALS:
        result = XObject::number(equals(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_LTE:
        result = XObject::number(lte(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_LT:
        result = XObject::number(lt(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_GTE:
        result = XObject::number(gte(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_GT:
        result = XObject::number(gt(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_PLUS:
        result = plus(context, opPos, executionContext);
        break;

    case XPathExpression::eOP_MINUS:
        result = minus(context, opPos, executionContext);
        break;

    case XPathExpression::eOP_MULT:
        result = mult(context, opPos, executionContext);
        break;

    case XPathExpression::eOP_DIV:
        result = div(context, opPos, executionContext);
        break;

    case XPathExpression::eOP_MOD:
        result = mod(context, opPos, executionContext);
        break;

    case XPathExpression::eOP_NEG:
        result = neg(context, opPos, executionContext);
        break;

    case XPathExpression::eOP_UNION:
        Union(context, opPos, executionContext, result);
        break;

    case XPathExpression::eOP_LITERAL:
        literal(opPos, result);
        break;

    case XPathExpression::eOP_VARIABLE:
        result = variable(opPos, executionContext)->num(executionContext);
        break;

    case XPathExpression::eOP_GROUP:
        group(context, opPos, executionContext, result);
        break;

    case XPathExpression::eOP_NUMBERLIT:
        result = numberlit(opPos);
        break;

    case XPathExpression::eOP_EXTFUNCTION:
        result = runExtFunction(context, opPos, executionContext)->num(executionContext);
        break;

    case XPathExpression::eOP_FUNCTION:
        result = runFunction(context, opPos, executionContext)->num(executionContext);
        break;

    case XPathExpression::eOP_LOCATIONPATH:
        locationPath(context, opPos, executionContext, result);
        break;

    case XPathExpression::eOP_FUNCTION_POSITION:
        result = functionPosition(context, executionContext);
        break;

    case XPathExpression::eOP_FUNCTION_LAST:
        result = functionLast(executionContext);
        break;

    case XPathExpression::eOP_FUNCTION_COUNT:
        result = functionCount(context, opPos, executionContext);
        break;

    case XPathExpression::eOP_FUNCTION_NOT:
        result = XObject::number(functionNot(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_FUNCTION_TRUE:
        result = XObject::number(true);
        break;

    case XPathExpression::eOP_FUNCTION_FALSE:
        result = XObject::number(false);
        break;

    case XPathExpression::eOP_FUNCTION_BOOLEAN:
        result = XObject::number(functionBoolean(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_FUNCTION_NAME_0:
        result = XObject::number(functionName(context),executionContext.getMemoryManager());
        break;

    case XPathExpression::eOP_FUNCTION_NAME_1:
        result = XObject::number(functionName(context, opPos, executionContext),executionContext.getMemoryManager());
        break;

    case XPathExpression::eOP_FUNCTION_LOCALNAME_0:
        result = XObject::number(functionLocalName(context),executionContext.getMemoryManager());
        break;

    case XPathExpression::eOP_FUNCTION_LOCALNAME_1:
        result = XObject::number(functionLocalName(context, opPos, executionContext),executionContext.getMemoryManager());
        break;

    case XPathExpression::eOP_FUNCTION_FLOOR:
        result = functionFloor(context, opPos, executionContext);
        break;

    case XPathExpression::eOP_FUNCTION_CEILING:
        result = functionCeiling(context, opPos, executionContext);
        break;

    case XPathExpression::eOP_FUNCTION_ROUND:
        result = functionRound(context, opPos, executionContext);
        break;

    case XPathExpression::eOP_FUNCTION_NUMBER_0:
        result = functionNumber(context, executionContext);
        break;

    case XPathExpression::eOP_FUNCTION_NUMBER_1:
        result = functionNumber(context, opPos, executionContext);
        break;

    case XPathExpression::eOP_FUNCTION_STRINGLENGTH_0:
        result = functionStringLength(context, executionContext);
        break;

    case XPathExpression::eOP_FUNCTION_STRINGLENGTH_1:
        result = functionStringLength(context, opPos, executionContext);
        break;

    case XPathExpression::eOP_FUNCTION_SUM:
        result = functionSum(context, opPos, executionContext);
        break;

    default:
        unknownOpCodeError(context, executionContext, opPos);
        break;
    }
}



void
XPath::executeMore(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext,
            XalanDOMString&         result) const
{
    switch(m_expression.getOpCodeMapValue(opPos))
    {
    case XPathExpression::eOP_OR:
        XObject::string(Or(context, opPos, executionContext), result);
        break;

    case XPathExpression::eOP_AND:
        XObject::string(And(context, opPos, executionContext), result);
        break;

    case XPathExpression::eOP_NOTEQUALS:
        XObject::string(notequals(context, opPos, executionContext), result);
        break;

    case XPathExpression::eOP_EQUALS:
        XObject::string(equals(context, opPos, executionContext), result);
        break;

    case XPathExpression::eOP_LTE:
        XObject::string(lte(context, opPos, executionContext), result);
        break;

    case XPathExpression::eOP_LT:
        XObject::string(lt(context, opPos, executionContext), result);
        break;

    case XPathExpression::eOP_GTE:
        XObject::string(gte(context, opPos, executionContext), result);
        break;

    case XPathExpression::eOP_GT:
        XObject::string(gt(context, opPos, executionContext), result);
        break;

    case XPathExpression::eOP_PLUS:
        XObject::string(plus(context, opPos, executionContext), result);
        break;

    case XPathExpression::eOP_MINUS:
        XObject::string(minus(context, opPos, executionContext), result);
        break;

    case XPathExpression::eOP_MULT:
        XObject::string(mult(context, opPos, executionContext), result);
        break;

    case XPathExpression::eOP_DIV:
        XObject::string(div(context, opPos, executionContext), result);
        break;

    case XPathExpression::eOP_MOD:
        XObject::string(mod(context, opPos, executionContext), result);
        break;

    case XPathExpression::eOP_NEG:
        XObject::string(neg(context, opPos, executionContext), result);
        break;

    case XPathExpression::eOP_UNION:
        Union(context, opPos, executionContext, result);
        break;

    case XPathExpression::eOP_LITERAL:
        literal(opPos, result);
        break;

    case XPathExpression::eOP_VARIABLE:
        variable(opPos, executionContext)->str(executionContext, result);
        break;

    case XPathExpression::eOP_GROUP:
        group(context, opPos, executionContext, result);
        break;

    case XPathExpression::eOP_NUMBERLIT:
        numberlit(opPos, result);
        break;

    case XPathExpression::eOP_EXTFUNCTION:
        runExtFunction(context, opPos, executionContext)->str(executionContext, result);
        break;

    case XPathExpression::eOP_FUNCTION:
        runFunction(context, opPos, executionContext)->str(executionContext, result);
        break;

    case XPathExpression::eOP_LOCATIONPATH:
        locationPath(context, opPos, executionContext, result);
        break;

    case XPathExpression::eOP_FUNCTION_POSITION:
        XObject::string(functionPosition(context, executionContext), result);
        break;

    case XPathExpression::eOP_FUNCTION_LAST:
        XObject::string(functionLast(executionContext), result);
        break;

    case XPathExpression::eOP_FUNCTION_COUNT:
        XObject::string(functionCount(context, opPos, executionContext), result);
        break;

    case XPathExpression::eOP_FUNCTION_NOT:
        XObject::string(functionNot(context, opPos, executionContext), result);
        break;

    case XPathExpression::eOP_FUNCTION_TRUE:
        XObject::string(true, result);
        break;

    case XPathExpression::eOP_FUNCTION_FALSE:
        XObject::string(false, result);
        break;

    case XPathExpression::eOP_FUNCTION_BOOLEAN:
        XObject::string(functionBoolean(context, opPos, executionContext), result);
        break;

    case XPathExpression::eOP_FUNCTION_NAME_0:
        result.append(functionName(context));
        break;

    case XPathExpression::eOP_FUNCTION_NAME_1:
        result.append(functionName(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_FUNCTION_LOCALNAME_0:
        result.append(functionLocalName(context));
        break;

    case XPathExpression::eOP_FUNCTION_LOCALNAME_1:
        result.append(functionLocalName(context, opPos, executionContext));
        break;

    case XPathExpression::eOP_FUNCTION_FLOOR:
        XObject::string(functionFloor(context, opPos, executionContext), result);
        break;

    case XPathExpression::eOP_FUNCTION_CEILING:
        XObject::string(functionCeiling(context, opPos, executionContext), result);
        break;

    case XPathExpression::eOP_FUNCTION_ROUND:
        XObject::string(functionRound(context, opPos, executionContext), result);
        break;

    case XPathExpression::eOP_FUNCTION_NUMBER_0:
        XObject::string(functionNumber(context, executionContext), result);
        break;

    case XPathExpression::eOP_FUNCTION_NUMBER_1:
        XObject::string(functionNumber(context, opPos, executionContext), result);
        break;

    case XPathExpression::eOP_FUNCTION_STRINGLENGTH_0:
        XObject::string(functionStringLength(context, executionContext), result);
        break;

    case XPathExpression::eOP_FUNCTION_STRINGLENGTH_1:
        XObject::string(functionStringLength(context, opPos, executionContext), result);
        break;

    case XPathExpression::eOP_FUNCTION_SUM:
        XObject::string(functionSum(context, opPos, executionContext), result);
        break;

    default:
        unknownOpCodeError(context, executionContext, opPos);
        break;
    }
}



inline void
stringToCharacters(
            const XalanDOMString&       str,
            FormatterListener&          formatterListener,
            XPath::MemberFunctionPtr    function)
{
    const XalanDOMString::size_type     theLength = str.length();

    if (theLength != 0)
    {
        assert(theLength == FormatterListener::size_type(theLength));

        (formatterListener.*function)(
            str.c_str(),
            FormatterListener::size_type(theLength));
    }
}



void
XPath::executeMore(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext,
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function) const
{
    switch(m_expression.getOpCodeMapValue(opPos))
    {
    case XPathExpression::eOP_OR:
        XObject::string(
                Or(context, opPos, executionContext),
                formatterListener,
                function);
        break;

    case XPathExpression::eOP_AND:
        XObject::string(
                And(context, opPos, executionContext),
                formatterListener,
                function);
        break;

    case XPathExpression::eOP_NOTEQUALS:
        XObject::string(
                notequals(context, opPos, executionContext),
                formatterListener,
                function);
        break;

    case XPathExpression::eOP_EQUALS:
        XObject::string(
                equals(context, opPos, executionContext),
                formatterListener,
                function);
        break;

    case XPathExpression::eOP_LTE:
        XObject::string(
                lte(context, opPos, executionContext),
                formatterListener,
                function);
        break;

    case XPathExpression::eOP_LT:
        XObject::string(
                lt(context, opPos, executionContext),
                formatterListener,
                function);
        break;

    case XPathExpression::eOP_GTE:
        XObject::string(
                gte(context, opPos, executionContext),
                formatterListener,
                function);
        break;

    case XPathExpression::eOP_GT:
        XObject::string(
                gt(context, opPos, executionContext),
                formatterListener,
                function);
        break;

    case XPathExpression::eOP_PLUS:
        plus(context, opPos, executionContext, formatterListener, function);
        break;

    case XPathExpression::eOP_MINUS:
        minus(context, opPos, executionContext, formatterListener, function);
        break;

    case XPathExpression::eOP_MULT:
        mult(context, opPos, executionContext, formatterListener, function);
        break;

    case XPathExpression::eOP_DIV:
        div(context, opPos, executionContext, formatterListener, function);
        break;

    case XPathExpression::eOP_MOD:
        mod(context, opPos, executionContext, formatterListener, function);
        break;

    case XPathExpression::eOP_NEG:
        neg(context, opPos, executionContext, formatterListener, function);
        break;

    case XPathExpression::eOP_UNION:
        Union(context, opPos, executionContext, formatterListener, function);
        break;

    case XPathExpression::eOP_LITERAL:
        literal(opPos, formatterListener, function);
        break;

    case XPathExpression::eOP_VARIABLE:
        variable(opPos, executionContext)->str(executionContext, formatterListener, function);
        break;

    case XPathExpression::eOP_GROUP:
        group(context, opPos, executionContext, formatterListener, function);
        break;

    case XPathExpression::eOP_NUMBERLIT:
        numberlit(opPos, formatterListener, function);
        break;

    case XPathExpression::eOP_EXTFUNCTION:
        runExtFunction(context, opPos, executionContext)->str(executionContext, formatterListener, function);
        break;

    case XPathExpression::eOP_FUNCTION:
        runFunction(context, opPos, executionContext)->str(executionContext, formatterListener, function);
        break;

    case XPathExpression::eOP_LOCATIONPATH:
        locationPath(context, opPos, executionContext, formatterListener, function);
        break;

    case XPathExpression::eOP_FUNCTION_POSITION:
        XObject::string(functionPosition(context, executionContext), formatterListener, function);
        break;

    case XPathExpression::eOP_FUNCTION_LAST:
        XObject::string(functionLast(executionContext), formatterListener, function);
        break;

    case XPathExpression::eOP_FUNCTION_COUNT:
        XObject::string(functionCount(context, opPos, executionContext), formatterListener, function);
        break;

    case XPathExpression::eOP_FUNCTION_NOT:
        XObject::string(functionNot(context, opPos, executionContext), formatterListener, function);
        break;

    case XPathExpression::eOP_FUNCTION_TRUE:
        XObject::string(true, formatterListener, function);
        break;

    case XPathExpression::eOP_FUNCTION_FALSE:
        XObject::string(false, formatterListener, function);
        break;

    case XPathExpression::eOP_FUNCTION_BOOLEAN:
        XObject::string(functionBoolean(context, opPos, executionContext), formatterListener, function);
        break;

    case XPathExpression::eOP_FUNCTION_NAME_0:
        stringToCharacters(functionName(context), formatterListener, function);
        break;

    case XPathExpression::eOP_FUNCTION_NAME_1:
        stringToCharacters(functionName(context, opPos, executionContext), formatterListener, function);
        break;

    case XPathExpression::eOP_FUNCTION_LOCALNAME_0:
        stringToCharacters(functionLocalName(context), formatterListener, function);
        break;

    case XPathExpression::eOP_FUNCTION_LOCALNAME_1:
        stringToCharacters(functionLocalName(context, opPos, executionContext), formatterListener, function);
        break;

    case XPathExpression::eOP_FUNCTION_FLOOR:
        XObject::string(functionFloor(context, opPos, executionContext), formatterListener, function);
        break;

    case XPathExpression::eOP_FUNCTION_CEILING:
        XObject::string(functionCeiling(context, opPos, executionContext), formatterListener, function);
        break;

    case XPathExpression::eOP_FUNCTION_ROUND:
        XObject::string(functionRound(context, opPos, executionContext), formatterListener, function);
        break;

    case XPathExpression::eOP_FUNCTION_NUMBER_0:
        XObject::string(functionNumber(context, executionContext), formatterListener, function);
        break;

    case XPathExpression::eOP_FUNCTION_NUMBER_1:
        XObject::string(functionNumber(context, opPos, executionContext), formatterListener, function);
        break;

    case XPathExpression::eOP_FUNCTION_STRINGLENGTH_0:
        XObject::string(functionStringLength(context, executionContext), formatterListener, function);
        break;

    case XPathExpression::eOP_FUNCTION_STRINGLENGTH_1:
        XObject::string(functionStringLength(context, opPos, executionContext), formatterListener, function);
        break;

    case XPathExpression::eOP_FUNCTION_SUM:
        XObject::string(functionSum(context, opPos, executionContext), formatterListener, function);
        break;

    default:
        unknownOpCodeError(context, executionContext, opPos);
        break;
    }
}



const XObjectPtr
XPath::executeMore(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext,
            MutableNodeRefList&     result) const
{
    XObjectPtr  theXObject;

    switch(m_expression.getOpCodeMapValue(opPos))
    {
    case XPathExpression::eOP_XPATH:
        theXObject = executeMore(context, opPos + 2, executionContext, result);
        break;

    case XPathExpression::eOP_OR:
    case XPathExpression::eOP_AND:
    case XPathExpression::eOP_NOTEQUALS:
    case XPathExpression::eOP_EQUALS:
    case XPathExpression::eOP_LTE:
    case XPathExpression::eOP_LT:
    case XPathExpression::eOP_GTE:
    case XPathExpression::eOP_GT:
    case XPathExpression::eOP_PLUS:
    case XPathExpression::eOP_MINUS:
    case XPathExpression::eOP_MULT:
    case XPathExpression::eOP_DIV:
    case XPathExpression::eOP_MOD:
    case XPathExpression::eOP_NEG:
    case XPathExpression::eOP_LITERAL:
    case XPathExpression::eOP_NUMBERLIT:
    case XPathExpression::eOP_FUNCTION_POSITION:
    case XPathExpression::eOP_FUNCTION_LAST:
    case XPathExpression::eOP_FUNCTION_COUNT:
    case XPathExpression::eOP_FUNCTION_NOT:
    case XPathExpression::eOP_FUNCTION_TRUE:
    case XPathExpression::eOP_FUNCTION_FALSE:
    case XPathExpression::eOP_FUNCTION_BOOLEAN:
    case XPathExpression::eOP_FUNCTION_NAME_0:
    case XPathExpression::eOP_FUNCTION_NAME_1:
    case XPathExpression::eOP_FUNCTION_LOCALNAME_0:
    case XPathExpression::eOP_FUNCTION_LOCALNAME_1:
    case XPathExpression::eOP_FUNCTION_FLOOR:
    case XPathExpression::eOP_FUNCTION_CEILING:
    case XPathExpression::eOP_FUNCTION_ROUND:
    case XPathExpression::eOP_FUNCTION_NUMBER_0:
    case XPathExpression::eOP_FUNCTION_NUMBER_1:
    case XPathExpression::eOP_FUNCTION_STRING_0:
    case XPathExpression::eOP_FUNCTION_STRING_1:
    case XPathExpression::eOP_FUNCTION_STRINGLENGTH_0:
    case XPathExpression::eOP_FUNCTION_STRINGLENGTH_1:
    case XPathExpression::eOP_FUNCTION_NAMESPACEURI_0:
    case XPathExpression::eOP_FUNCTION_NAMESPACEURI_1:
    case XPathExpression::eOP_FUNCTION_SUM:
        notNodeSetError(context, executionContext);
        break;

    case XPathExpression::eOP_UNION:
        Union(context, opPos, executionContext, result);
        break;

    case XPathExpression::eOP_VARIABLE:
        theXObject = variable(opPos, executionContext);
        break;

    case XPathExpression::eOP_GROUP:
        group(context, opPos, executionContext, result);
        break;

    case XPathExpression::eOP_EXTFUNCTION:
        theXObject = runExtFunction(context, opPos, executionContext);
        break;

    case XPathExpression::eOP_FUNCTION:
        theXObject = runFunction(context, opPos, executionContext);
        break;

    case XPathExpression::eOP_LOCATIONPATH:
        locationPath(context, opPos, executionContext, result);
        break;

    default:
        unknownOpCodeError(context, executionContext, opPos);
        break;
    }

    if (theXObject.null() == false && theXObject->getType() != XObject::eTypeNodeSet)
    {
        notNodeSetError(context, executionContext);
    }

    return theXObject;
}



inline void
XPath::doGetMatchScore(
            XalanNode*              context,
            XPathExecutionContext&  executionContext,
            eMatchScore&            score) const
{
    assert(context != 0);

    OpCodeMapPositionType   opPos =
        m_expression.getInitialOpCodePosition() + 2;

    while(m_expression.getOpCodeMapValue(opPos) == XPathExpression::eOP_LOCATIONPATHPATTERN)
    {
        const OpCodeMapPositionType     nextOpPos = m_expression.getNextOpCodePosition(opPos);

        score = locationPathPattern(executionContext, *context, opPos);

        if(score == eMatchScoreNone)
        {
            opPos = nextOpPos;
        }
        else
        {
            break;
        }
    }
}



XPath::eMatchScore
XPath::getMatchScore(
            XalanNode*              node,
            XPathExecutionContext&  executionContext) const
{
    eMatchScore     score = eMatchScoreNone;

    if(m_expression.getOpCodeMapValue(0) != XPathExpression::eOP_MATCHPATTERN)
    {
        GetCachedString     theGuard(executionContext);

        XalanDOMString&     theBuffer = theGuard.get();

        executionContext.problem(
            XPathExecutionContext::eXPath,
            XPathExecutionContext::eError,
            XalanMessageLoader::getMessage(
                theBuffer,
                XalanMessages::CannotEvaluateXPathExpressionAsMatchPattern),
            m_locator,
            node);
    }
    else
    {
        assert(node != 0);

        doGetMatchScore(node, executionContext, score);
    }

    return score;
}



XPath::eMatchScore
XPath::getMatchScore(
            XalanNode*              node,
            const PrefixResolver&   resolver,
            XPathExecutionContext&  executionContext) const
{
    const PrefixResolver* const     theCurrentResolver =
        executionContext.getPrefixResolver();

    if (theCurrentResolver == &resolver)
    {
        return getMatchScore(node, executionContext);
    }
    else
    {
        // Push and pop the PrefixResolver...
        const PrefixResolverSetAndRestore   theSetAndRestore(
                                                executionContext,
                                                theCurrentResolver,
                                                &resolver);

        return getMatchScore(node, executionContext);
    }
}



inline const XalanDOMString*
getStringFromTokenQueue(
            const XPathExpression&          expression,
            XPath::OpCodeMapPositionType    opPos)
{
    const XPath::OpCodeMapValueType     tokenPosition =
                expression.getOpCodeMapValue(opPos);

    if (tokenPosition == XPathExpression::eEMPTY ||
        tokenPosition == XPathExpression::eELEMWILDCARD)
    {
        return 0;
    }
    else
    {
        assert(tokenPosition != XPathExpression::eENDOP);

        const XToken* const     token =
                    expression.getToken(tokenPosition);
        assert(token != 0);

        return &token->str();
    }
}



void
XPath::getTargetData(TargetDataVectorType&  targetData) const
{
    OpCodeMapPositionType   opPos =
        m_expression.getInitialOpCodePosition() + 2;

    targetData.reserve(eDefaultTargetDataSize);

    while(m_expression.getOpCodeMapValue(opPos) == XPathExpression::eOP_LOCATIONPATHPATTERN)
    {
        const OpCodeMapPositionType     nextOpPos = m_expression.getNextOpCodePosition(opPos);

        opPos += 2;
     
        unsigned long   stepCount = 0;

        while(m_expression.getOpCodeMapValue(opPos) != XPathExpression::eENDOP)
        {
            ++stepCount;

            const OpCodeMapPositionType     nextStepPos =
                m_expression.getNextOpCodePosition(opPos);

            const OpCodeMapValueType    nextOp = m_expression.getOpCodeMapValue(nextStepPos);

            if(nextOp == XPathExpression::eENDOP)
            {
                eMatchScore                 score = eMatchScoreNone;

                const XalanDOMChar*         targetLocalName = 0;

                TargetData::eTargetType     targetType = TargetData::eOther;

                bool    fIsAttribute = false;

                const OpCodeMapValueType    stepType = m_expression.getOpCodeMapValue(opPos);

                opPos += 3;

                switch(stepType)
                {
                case XPathExpression::eOP_FUNCTION:
                    targetLocalName = PSEUDONAME_ANY;
                    score = eMatchScoreOther;
                    targetType = TargetData::eAny;
                    break;

                case XPathExpression::eFROM_ROOT:
                    targetLocalName = PSEUDONAME_ROOT;
                    score = eMatchScoreOther;
                    break;

                case XPathExpression::eMATCH_ATTRIBUTE:
                    fIsAttribute = true;
                    // fall through on purpose...

                case XPathExpression::eMATCH_ANY_ANCESTOR:
                case XPathExpression::eMATCH_IMMEDIATE_ANCESTOR:
                    {
                        const OpCodeMapValueType    tok = m_expression.getOpCodeMapValue(opPos);

                        switch(tok)
                        {
                        case XPathExpression::eNODETYPE_COMMENT:
                            targetLocalName = PSEUDONAME_COMMENT;
                            score = eMatchScoreNodeTest;
                            break;

                        case XPathExpression::eNODETYPE_TEXT:
                            targetLocalName = PSEUDONAME_TEXT;
                            score = eMatchScoreNodeTest;
                            break;

                        case XPathExpression::eNODETYPE_NODE:
                            targetLocalName = PSEUDONAME_NODE;
                            score = eMatchScoreNodeTest;
                            break;

                        case XPathExpression::eNODETYPE_ROOT:
                            targetLocalName = PSEUDONAME_ROOT;
                            score = eMatchScoreNodeTest;
                            break;

                        case XPathExpression::eNODETYPE_ANYELEMENT:
                            targetLocalName = PSEUDONAME_ANY;
                            score = eMatchScoreNodeTest;
                            targetType = TargetData::eElement;
                            break;

                        case XPathExpression::eNODETYPE_PI:
                            {
                                const OpCodeMapValueType    argLen =
                                    m_expression.getOpCodeArgumentLength(opPos - 3);

                                targetLocalName = PSEUDONAME_PI;

                                if (argLen == 1)
                                {
                                    score = eMatchScoreNodeTest;
                                }
                                else if (argLen == 2)
                                {
                                    score = eMatchScoreQName;
                                }
                            }
                            break;

                        case XPathExpression::eNODENAME:
                            {
                                const XalanDOMString* const     targetNamespace =
                                        getStringFromTokenQueue(m_expression, opPos + 1);

                                const XalanDOMString* const     targetLocal =
                                        getStringFromTokenQueue(m_expression, opPos + 2);

                                if (targetLocal == 0)
                                {
                                    targetLocalName = 0;
                                }
                                else
                                {
                                    targetLocalName = targetLocal->c_str();
                                }

                                targetType = fIsAttribute ?
                                    TargetData::eAttribute :
                                    TargetData::eElement;

                                if(targetLocalName != 0)
                                {
                                    if(targetLocalName == PSEUDONAME_ANY)
                                    {
                                        targetLocalName = PSEUDONAME_ANY;

                                        if (targetNamespace == 0 ||
                                            *targetNamespace == PSEUDONAME_ANY)
                                        {
                                            score = eMatchScoreNodeTest;
                                        }
                                        else
                                        {
                                            score = eMatchScoreNSWild;
                                        }
                                    }
                                    else
                                    {
                                        score = eMatchScoreQName;
                                    }
                                }
                                else
                                {
                                    targetLocalName = PSEUDONAME_ANY;

                                    if (targetNamespace == 0 ||
                                        *targetNamespace == PSEUDONAME_ANY)
                                    {
                                        score = eMatchScoreNodeTest;
                                    }
                                    else
                                    {
                                        score = eMatchScoreNSWild;
                                    }
                                }
                            }

                            break;

                        default:
                            targetLocalName = PSEUDONAME_ANY;
                            score = eMatchScoreNodeTest;
                            break;
                        }
                    }
                    break;
                }

                assert(targetLocalName != 0);

                // If there are multiple steps, or a predicate,
                // the priority is always eMatchScoreOther.
                if (stepCount > 1 ||
                    opPos + 3 < nextStepPos)
                {
                    score = eMatchScoreOther;
                }

                targetData.push_back(TargetData(targetLocalName, score, targetType));
            }

            opPos = nextStepPos;
        }

        opPos = nextOpPos;
    }
}



bool
XPath::Or(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext) const
{
    opPos += 2;

    bool    theResult;

    executeMore(context, opPos, executionContext, theResult);

    if(theResult == false)
    {
        opPos = m_expression.getNextOpCodePosition(opPos);

        executeMore(context, opPos, executionContext, theResult);
    }

    return theResult;
}



bool
XPath::And(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext) const
{
    opPos += 2; 

    bool    fResult;

    executeMore(context, opPos, executionContext, fResult);

    if (fResult == true)
    {
        opPos = m_expression.getNextOpCodePosition(opPos);

        executeMore(context, opPos, executionContext, fResult);
    }   

    return fResult;
}



bool
XPath::notequals(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext) const
{
    opPos += 2; 

    const XObjectPtr    expr1(executeMore(context, opPos, executionContext));
    assert(expr1.get() != 0);

    opPos = m_expression.getNextOpCodePosition(opPos);

    return expr1->notEquals(
            *executeMore(context, opPos, executionContext).get(),
            executionContext);
}



bool
XPath::equals(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext) const
{
    opPos += 2; 

    const XObjectPtr    expr1(executeMore(context, opPos, executionContext));
    assert(expr1.get() != 0);

    opPos = m_expression.getNextOpCodePosition(opPos);

    return expr1->equals(
            *executeMore(context, opPos, executionContext).get(),
            executionContext);
}



bool
XPath::lte(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext) const
{
    opPos += 2; 

    const XObjectPtr    expr1(executeMore(context, opPos, executionContext));
    assert(expr1.get() != 0);

    opPos = m_expression.getNextOpCodePosition(opPos);

    return expr1->lessThanOrEquals(
            *executeMore(context, opPos, executionContext).get(),
            executionContext);
}



bool
XPath::lt(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext) const
{
    opPos += 2;

    const XObjectPtr    expr1(executeMore(context, opPos, executionContext));
    assert(expr1.get() != 0);

    opPos = m_expression.getNextOpCodePosition(opPos);

    return expr1->lessThan(
            *executeMore(context, opPos, executionContext).get(),
            executionContext);
}



bool
XPath::gte(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext) const
{
    opPos += 2;

    const XObjectPtr    expr1(executeMore(context, opPos, executionContext));
    assert(expr1.get() != 0);

    opPos = m_expression.getNextOpCodePosition(opPos);

    return expr1->greaterThanOrEquals(
            *executeMore(context, opPos, executionContext).get(),
            executionContext);
}



bool
XPath::gt(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext) const
{
    opPos += 2;

    const XObjectPtr    expr1(executeMore(context, opPos, executionContext));
    assert(expr1.get() != 0);

    opPos = m_expression.getNextOpCodePosition(opPos);

    return expr1->greaterThan(
            *executeMore(context, opPos, executionContext).get(),
            executionContext);
}



double
XPath::getNumericOperand(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext) const
{
    if (m_expression.getOpCodeMapValue(opPos) == XPathExpression::eOP_NUMBERLIT)
    {
        assert(m_expression.tokenQueueSize() >
            m_expression.getOpCodeMapValue(opPos + 3));

        return m_expression.getNumberLiteral(m_expression.getOpCodeMapValue(opPos + 2));
    }
    else
    {
        double  theResult;

        executeMore(context, opPos, executionContext, theResult);

        return theResult;
    }
}



double
XPath::plus(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext) const
{
    opPos += 2;

    const double    expr1 = getNumericOperand(context, opPos, executionContext);

    opPos = m_expression.getNextOpCodePosition(opPos);

    const double    expr2 = getNumericOperand(context, opPos, executionContext);

    return DoubleSupport::add(expr1, expr2);
}



void
XPath::plus(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext,
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function) const
{
    const double    theResult = plus(context, opPos, executionContext);

    XObject::string(theResult, formatterListener, function);
}



double
XPath::minus(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext) const
{
    opPos += 2;

    const double    expr1 = getNumericOperand(context, opPos, executionContext);

    opPos = m_expression.getNextOpCodePosition(opPos);

    const double    expr2 = getNumericOperand(context, opPos, executionContext);

    return DoubleSupport::subtract(expr1, expr2);
}



void
XPath::minus(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext,
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function) const
{
    const double    theResult = minus(context, opPos, executionContext);

    XObject::string(theResult, formatterListener, function);
}



double
XPath::mult(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext) const
{
    opPos += 2;

    const double    expr1 = getNumericOperand(context, opPos, executionContext);

    opPos = m_expression.getNextOpCodePosition(opPos);

    const double    expr2 = getNumericOperand(context, opPos, executionContext);

    return DoubleSupport::multiply(expr1, expr2);
}



void
XPath::mult(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext,
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function) const
{
    const double    theResult = mult(context, opPos, executionContext);

    XObject::string(theResult, formatterListener, function);
}



double
XPath::div(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext) const
{
    opPos += 2;

    const double    expr1 = getNumericOperand(context, opPos, executionContext);

    opPos = m_expression.getNextOpCodePosition(opPos);

    const double    expr2 = getNumericOperand(context, opPos, executionContext);

    return DoubleSupport::divide(expr1, expr2);
}



void
XPath::div(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext,
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function) const
{
    const double    theResult = div(context, opPos, executionContext);

    XObject::string(theResult, formatterListener, function);
}



double
XPath::mod(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext) const
{
    opPos += 2; 

    const double    expr1 = getNumericOperand(context, opPos, executionContext);

    opPos = m_expression.getNextOpCodePosition(opPos);

    const double    expr2 = getNumericOperand(context, opPos, executionContext);

    return DoubleSupport::modulus(expr1, expr2);
}



void
XPath::mod(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext,
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function) const
{
    const double    theResult = mod(context, opPos, executionContext);

    XObject::string(theResult, formatterListener, function);
}



double
XPath::neg(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext) const
{
    return DoubleSupport::negative(getNumericOperand(context, opPos + 2, executionContext));
}



void
XPath::neg(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext,
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function) const
{
    const double    theResult = neg(context, opPos, executionContext);

    XObject::string(theResult, formatterListener, function);
}



const XObjectPtr
XPath::Union(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext) const
{
    typedef XPathExecutionContext::BorrowReturnMutableNodeRefList   BorrowReturnMutableNodeRefList;

    BorrowReturnMutableNodeRefList  resultNodeList(executionContext);

    Union(context, opPos, executionContext, *resultNodeList);

    return executionContext.getXObjectFactory().createNodeSet(resultNodeList);
}



void
XPath::Union(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext,
            bool&                   result) const
{
    typedef XPathExecutionContext::BorrowReturnMutableNodeRefList   BorrowReturnMutableNodeRefList;

    BorrowReturnMutableNodeRefList  resultNodeList(executionContext);

    Union(context, opPos, executionContext, *resultNodeList);

    result = XObject::boolean(*resultNodeList);
}



void
XPath::Union(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext,
            double&                 result) const
{
    typedef XPathExecutionContext::BorrowReturnMutableNodeRefList   BorrowReturnMutableNodeRefList;

    BorrowReturnMutableNodeRefList  resultNodeList(executionContext);

    Union(context, opPos, executionContext, *resultNodeList);

    result = XObject::number(executionContext, *resultNodeList);
}



void
XPath::Union(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext,
            XalanDOMString&         result) const
{
    typedef XPathExecutionContext::BorrowReturnMutableNodeRefList   BorrowReturnMutableNodeRefList;

    BorrowReturnMutableNodeRefList  resultNodeList(executionContext);

    Union(context, opPos, executionContext, *resultNodeList);

    XObject::string(*resultNodeList, executionContext, result);
}



void
XPath::Union(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext,
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function) const
{
    typedef XPathExecutionContext::BorrowReturnMutableNodeRefList   BorrowReturnMutableNodeRefList;

    BorrowReturnMutableNodeRefList  resultNodeList(executionContext);

    Union(context, opPos, executionContext, *resultNodeList);

    XObject::string(*resultNodeList, executionContext, formatterListener, function);
}



void
XPath::Union(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext,
            MutableNodeRefList&     result) const
{
    assert(result.empty() == true);

    opPos += 2;

    typedef XPathExecutionContext::BorrowReturnMutableNodeRefList   BorrowReturnMutableNodeRefList;

    BorrowReturnMutableNodeRefList  theGuard(executionContext);

    MutableNodeRefList&     tempNodeList = *theGuard;

    while(m_expression.getOpCodeMapValue(opPos) != XPathExpression::eENDOP)
    {
        const XObjectPtr    nodesetResult(executeMore(context, opPos, executionContext, tempNodeList));

        if (nodesetResult.null() == false)
        {
            result.addNodesInDocOrder(nodesetResult->nodeset(), executionContext);
        }
        else
        {
            result.addNodesInDocOrder(tempNodeList, executionContext);

            tempNodeList.clear();
        }

        opPos = m_expression.getNextOpCodePosition(opPos);
    }

    result.setDocumentOrder();
}



const XObjectPtr
XPath::literal(
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext) const
{
    assert(m_expression.isValidOpCodePosition(opPos + 2));
    assert(m_expression.isValidTokenQueuePosition(m_expression.getOpCodeMapValue(opPos + 2)));

    const XToken* const     theLiteral = m_expression.getToken(m_expression.getOpCodeMapValue(opPos + 2));
    assert(theLiteral != 0);

    if (m_inStylesheet == true)
    {
        return executionContext.getXObjectFactory().createString(*theLiteral);
    }
    else
    {
        return executionContext.getXObjectFactory().createString(theLiteral->str());
    }
}



void
XPath::literal(
            OpCodeMapPositionType   opPos,
            bool&                   theResult) const
{
    assert(m_expression.isValidOpCodePosition(opPos + 2));
    assert(m_expression.isValidTokenQueuePosition(m_expression.getOpCodeMapValue(opPos + 2)));

    const XToken* const     theLiteral = m_expression.getToken(m_expression.getOpCodeMapValue(opPos + 2));
    assert(theLiteral != 0);

    theResult = theLiteral->boolean();
}



void
XPath::literal(
            OpCodeMapPositionType   opPos,
            double&                 theResult) const
{
    assert(m_expression.isValidOpCodePosition(opPos + 2));
    assert(m_expression.isValidTokenQueuePosition(m_expression.getOpCodeMapValue(opPos + 2)));

    const XToken* const     theLiteral = m_expression.getToken(m_expression.getOpCodeMapValue(opPos + 2));
    assert(theLiteral != 0);

    theResult = theLiteral->num();
}



void
XPath::literal(
            OpCodeMapPositionType   opPos,
            XalanDOMString&         theString) const
{
    assert(m_expression.isValidOpCodePosition(opPos + 2));
    assert(m_expression.isValidTokenQueuePosition(m_expression.getOpCodeMapValue(opPos + 2)));

    const XToken* const     theLiteral = m_expression.getToken(m_expression.getOpCodeMapValue(opPos + 2));
    assert(theLiteral != 0);

    theString = theLiteral->str();
}



void
XPath::literal(
            OpCodeMapPositionType   opPos,
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function) const
{
    assert(m_expression.isValidOpCodePosition(opPos + 2));
    assert(m_expression.isValidTokenQueuePosition(m_expression.getOpCodeMapValue(opPos + 2)));

    const XToken* const     theLiteral = m_expression.getToken(m_expression.getOpCodeMapValue(opPos + 2));
    assert(theLiteral != 0);

    theLiteral->str(formatterListener, function);
}



const XObjectPtr
XPath::variable(
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext) const
{
    assert(m_expression.isValidOpCodePosition(opPos + 3));
    assert(m_expression.isValidTokenQueuePosition(m_expression.getOpCodeMapValue(opPos + 3)));

    const XToken* const     ns =
        m_expression.getToken(m_expression.getOpCodeMapValue(opPos + 2));
    assert(ns != 0);

    const XToken* const     varName =
        m_expression.getToken(m_expression.getOpCodeMapValue(opPos + 3));
    assert(varName != 0);

    return executionContext.getVariable(XalanQNameByReference(ns->str(), varName->str()), m_locator);
}



const XObjectPtr
XPath::numberlit(
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext) const
{
    assert(m_expression.isValidOpCodePosition(opPos + 3));
    assert(m_expression.isValidTokenQueuePosition(m_expression.getOpCodeMapValue(opPos + 3)));

    const XToken* const     theLiteral =
        m_expression.getToken(m_expression.getOpCodeMapValue(opPos + 3));
    assert(theLiteral != 0);

    if (m_inStylesheet == true)
    {
        return executionContext.getXObjectFactory().createNumber(*theLiteral);
    }
    else
    {
        return executionContext.getXObjectFactory().createNumber(theLiteral->num());
    }
}



double
XPath::numberlit(OpCodeMapPositionType  opPos) const
{
    assert(m_expression.isValidOpCodePosition(opPos + 3));
    assert(m_expression.isValidTokenQueuePosition(m_expression.getOpCodeMapValue(opPos + 3)));

    const XToken* const     theLiteral =
        m_expression.getToken(m_expression.getOpCodeMapValue(opPos + 3));
    assert(theLiteral != 0);

    return theLiteral->num();
}



void
XPath::numberlit(
            OpCodeMapPositionType   opPos,
            bool&                   theResult) const
{
    assert(m_expression.isValidOpCodePosition(opPos + 3));
    assert(m_expression.isValidTokenQueuePosition(m_expression.getOpCodeMapValue(opPos + 3)));

    const XToken* const     theLiteral =
        m_expression.getToken(m_expression.getOpCodeMapValue(opPos + 3));
    assert(theLiteral != 0);

    theResult = theLiteral->boolean();
}



void
XPath::numberlit(
            OpCodeMapPositionType   opPos,
            XalanDOMString&         theString) const
{
    assert(m_expression.isValidOpCodePosition(opPos + 3));
    assert(m_expression.isValidTokenQueuePosition(m_expression.getOpCodeMapValue(opPos + 3)));

    const XToken* const     theLiteral =
        m_expression.getToken(m_expression.getOpCodeMapValue(opPos + 3));
    assert(theLiteral != 0);

    theString = theLiteral->str();
}



void
XPath::numberlit(
            OpCodeMapPositionType   opPos,
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function) const
{
    assert(m_expression.isValidOpCodePosition(opPos + 3));
    assert(m_expression.isValidTokenQueuePosition(m_expression.getOpCodeMapValue(opPos + 3)));

    const XToken* const     theLiteral =
        m_expression.getToken(m_expression.getOpCodeMapValue(opPos + 3));
    assert(theLiteral != 0);

    theLiteral->str( formatterListener, function);
}



const XObjectPtr
XPath::locationPath(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext) const
{    
    assert(context != 0);

    typedef XPathExecutionContext::BorrowReturnMutableNodeRefList   BorrowReturnMutableNodeRefList;

    BorrowReturnMutableNodeRefList  mnl(executionContext);

    locationPath(context, opPos, executionContext, *mnl.get());

    return executionContext.getXObjectFactory().createNodeSet(mnl);
}



void
XPath::locationPath(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext,
            bool&                   theResult) const
{
    assert(context != 0);

    typedef XPathExecutionContext::BorrowReturnMutableNodeRefList   BorrowReturnMutableNodeRefList;

    BorrowReturnMutableNodeRefList  mnl(executionContext);

    locationPath(context, opPos, executionContext, *mnl.get());

    theResult = XObject::boolean(*mnl.get());
}



void
XPath::locationPath(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext,
            double&                 theResult) const
{
    assert(context != 0);

    typedef XPathExecutionContext::BorrowReturnMutableNodeRefList   BorrowReturnMutableNodeRefList;

    BorrowReturnMutableNodeRefList  mnl(executionContext);

    locationPath(context, opPos, executionContext, *mnl.get());

    theResult = XObject::number(executionContext, *mnl.get());
}



void
XPath::locationPath(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext,
            XalanDOMString&         theResult) const
{
    assert(context != 0);

    typedef XPathExecutionContext::BorrowReturnMutableNodeRefList   BorrowReturnMutableNodeRefList;

    BorrowReturnMutableNodeRefList  mnl(executionContext);

    locationPath(context, opPos, executionContext, *mnl.get());

    XObject::string(*mnl.get(), executionContext, theResult);
}



void
XPath::locationPath(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext,
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function) const
{
    assert(context != 0);

    typedef XPathExecutionContext::BorrowReturnMutableNodeRefList   BorrowReturnMutableNodeRefList;

    BorrowReturnMutableNodeRefList  mnl(executionContext);

    locationPath(context, opPos, executionContext, *mnl.get());

    XObject::string(*mnl, executionContext, formatterListener, function);
}



const XObjectPtr
XPath::runExtFunction(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext) const
{
    assert(context != 0);

    const OpCodeMapPositionType     endExtFunc =
        opPos + m_expression.getOpCodeMapValue(opPos + 1) - 1;

    opPos += 2;

    const XToken* const     ns = m_expression.getToken(m_expression.getOpCodeMapValue(opPos));
    assert(ns != 0);

    ++opPos;

    const XToken* const     funcName = m_expression.getToken(m_expression.getOpCodeMapValue(opPos));
    assert(funcName != 0);

    ++opPos;

    typedef XPathExecutionContext::XObjectArgVectorType     XObjectArgVectorType;

    XObjectArgVectorType    args(executionContext.getMemoryManager());

    while(opPos < endExtFunc)
    {
        const OpCodeMapPositionType     nextOpPos =
            m_expression.getNextOpCodePosition(opPos);

        args.push_back(executeMore(context, opPos, executionContext));

        opPos = nextOpPos;
    }

    return extfunction(context, opPos, ns->str(), funcName->str(), args, executionContext);
}



const XObjectPtr
XPath::runFunction(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext) const
{
    const OpCodeMapPositionType     endFunc =
        opPos + m_expression.getOpCodeMapValue(opPos + 1) - 1;

    opPos += 2;

    // This is index into the function table for the
    // function.
    const OpCodeMapValueType    funcID = m_expression.getOpCodeMapValue(opPos);

    opPos++;

    // Number of args is next.
    const OpCodeMapValueType    argCount = m_expression.getOpCodeMapValue(opPos);

    opPos++;

    if (argCount == 0)
    {
        assert(opPos == endFunc);

        return s_functions[funcID].execute(executionContext, context, m_locator);
    }
    else if (argCount == 1)
    {
        return s_functions[funcID].execute(
            executionContext,
            context,
            executeMore(context, opPos, executionContext),
            m_locator);
    }
    else if (argCount == 2)
    {
        const XObjectPtr    theArg1(executeMore(context, opPos, executionContext));

        opPos = m_expression.getNextOpCodePosition(opPos);

        assert(m_expression.getNextOpCodePosition(opPos) == endFunc);

        return s_functions[funcID].execute(
            executionContext,
            context,
            theArg1,
            executeMore(context, opPos, executionContext),
            m_locator);
    }
    else if (argCount == 3)
    {
        const XObjectPtr    theArg1(executeMore(context, opPos, executionContext));

        opPos = m_expression.getNextOpCodePosition(opPos);

        const XObjectPtr    theArg2(executeMore(context, opPos, executionContext));

        opPos = m_expression.getNextOpCodePosition(opPos);

        assert(m_expression.getNextOpCodePosition(opPos) == endFunc);

        return s_functions[funcID].execute(
            executionContext,
            context,
            theArg1,
            theArg2,
            executeMore(context, opPos, executionContext),
            m_locator);
    }
    else
    {
        typedef XPathExecutionContext::XObjectArgVectorType     XObjectArgVectorType;

        XObjectArgVectorType    args(executionContext.getMemoryManager());

        args.reserve(argCount);

        while(opPos < endFunc)
        {
            args.push_back(executeMore(context, opPos, executionContext));

            opPos = m_expression.getNextOpCodePosition(opPos);
        }

        return function(context, funcID, args, executionContext);
    }
}



double
XPath::functionCount(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext) const
{
    assert(context != 0);

    typedef XPathExecutionContext::BorrowReturnMutableNodeRefList   BorrowReturnMutableNodeRefList;

    BorrowReturnMutableNodeRefList  result(executionContext);

    const XObjectPtr    nodesetResult(executeMore(context, opPos + 2, executionContext, *result));

    const XPathExecutionContext::size_type  theResult =
        nodesetResult.null() == false ?
            nodesetResult->nodeset().getLength() :
            result->getLength();
    assert(static_cast<double>(theResult) == theResult);

    return static_cast<double>(theResult);
}



const XalanDOMString&
XPath::functionName(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext) const
{
    assert(context != 0);

    typedef XPathExecutionContext::BorrowReturnMutableNodeRefList   BorrowReturnMutableNodeRefList;

    BorrowReturnMutableNodeRefList  result(executionContext);

    const XObjectPtr    nodesetResult(executeMore(context, opPos + 2, executionContext, *result));

    const NodeRefListBase* const    theNodeList = nodesetResult.null() == false ?
            &nodesetResult->nodeset() : &*result;
    assert(theNodeList != 0);

    if (theNodeList->getLength() == 0)
    {
        return s_emptyString;
    }
    else
    {
        assert(theNodeList->item(0) != 0);

        return functionName(theNodeList->item(0));
    }
}



const XalanDOMString&
XPath::functionLocalName(XalanNode*     context) const
{
    assert(context != 0);

    const XalanDOMString*       theResult = &s_emptyString;

    const XalanNode::NodeType   theType = context->getNodeType();

    if(theType == XalanNode::ELEMENT_NODE ||
       theType == XalanNode::PROCESSING_INSTRUCTION_NODE)
    {
        theResult = &DOMServices::getLocalNameOfNode(*context);
    }
    else if (theType == XalanNode::ATTRIBUTE_NODE)
    {
        const XalanDOMString&   theLocalName = DOMServices::getLocalNameOfNode(*context);

        if (theLocalName != DOMServices::s_XMLNamespace)
        {
            theResult = &theLocalName;
        }
    }

    return *theResult;
}



const XalanDOMString&
XPath::functionLocalName(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext) const
{
    assert(context != 0);

    typedef XPathExecutionContext::BorrowReturnMutableNodeRefList   BorrowReturnMutableNodeRefList;

    BorrowReturnMutableNodeRefList  result(executionContext);

    const XObjectPtr    nodesetResult(executeMore(context, opPos + 2, executionContext, *result));

    const NodeRefListBase* const    theNodeList = nodesetResult.null() == false ?
            &nodesetResult->nodeset() : &*result;
    assert(theNodeList != 0);

    if (theNodeList->getLength() == 0)
    {
        return s_emptyString;
    }
    else
    {
        assert(theNodeList->item(0) != 0);

        return functionLocalName(theNodeList->item(0));
    }
}



double
XPath::functionStringLength(
            XalanNode*              context,
            XPathExecutionContext&  executionContext) const
{
    assert(context != 0);

    FormatterStringLengthCounter    theCounter;

    DOMServices::getNodeData(*context, executionContext, theCounter, &FormatterListener::characters);

    const FormatterListener::size_type  theResult = theCounter.getCount();
    assert(static_cast<double>(theResult) == theResult);

    return static_cast<double>(theResult);
}



double
XPath::functionStringLength(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext) const
{
    assert(context != 0);

    FormatterStringLengthCounter    theCounter;

    executeMore(context, opPos + 2, executionContext, theCounter, &FormatterListener::characters);

    const FormatterListener::size_type  theResult = theCounter.getCount();
    assert(static_cast<double>(theResult) == theResult);

    return static_cast<double>(theResult);
}



double
XPath::functionSum(
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            XPathExecutionContext&  executionContext) const
{
    assert(context != 0);

    double  sum = 0.0;

    typedef XPathExecutionContext::BorrowReturnMutableNodeRefList   BorrowReturnMutableNodeRefList;

    BorrowReturnMutableNodeRefList  result(executionContext);

    const XObjectPtr    nodesetResult(executeMore(context, opPos + 2, executionContext, *result));

    const NodeRefListBase* const    theNodeList = nodesetResult.null() == false ?
            &nodesetResult->nodeset() : &*result;
    assert(theNodeList != 0);

    const NodeRefListBase::size_type    theLength = theNodeList->getLength();

    if (theLength != 0)
    {
        assert(theNodeList->item(0) != 0);

        const GetCachedString   theData(executionContext);

        XalanDOMString&     theString = theData.get();

        for (NodeRefListBase::size_type i = 0; i < theLength; i++)
        {
            DOMServices::getNodeData(*theNodeList->item(i), executionContext, theString);

            sum = DoubleSupport::add(sum, DoubleSupport::toDouble(theString, executionContext.getMemoryManager()));

            theString.clear();
        }
    }

    return sum;
}



XPath::eMatchScore
XPath::locationPathPattern(
            XPathExecutionContext&  executionContext,
            XalanNode&              context, 
            OpCodeMapPositionType   opPos) const
{
    eMatchScore score = eMatchScoreNone;

    stepPattern(executionContext, &context, opPos + 2, score);

    return score;
}



void
XPath::step(
            XPathExecutionContext&  executionContext,
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            MutableNodeRefList&     queryResults) const
{
    const XPathExpression&  currentExpression = getExpression();

    const OpCodeMapValueType   stepType =
        currentExpression.getOpCodeMapValue(opPos);

    typedef XPathExecutionContext::BorrowReturnMutableNodeRefList   BorrowReturnMutableNodeRefList;

    BorrowReturnMutableNodeRefList  subQueryResults(executionContext);

    bool    continueStepRecursion = true;

    switch(stepType)
    {
    case XPathExpression::eOP_VARIABLE:
    case XPathExpression::eOP_EXTFUNCTION:
    case XPathExpression::eOP_FUNCTION:
    case XPathExpression::eOP_GROUP:
        opPos = findNodeSet(executionContext, context, opPos, stepType, *subQueryResults);
        break;

    case XPathExpression::eFROM_ROOT:
        opPos = findRoot(executionContext, context, opPos, stepType, *subQueryResults);
        break;

    case XPathExpression::eFROM_PARENT:
        opPos = findParent(executionContext, context, opPos, stepType, *subQueryResults);
        break;

    case XPathExpression::eFROM_SELF:
        opPos = findSelf(executionContext, context, opPos, stepType, *subQueryResults);
        break;

    case XPathExpression::eFROM_ANCESTORS:
        opPos = findAncestors(executionContext, context, opPos, stepType, *subQueryResults);
        break;

    case XPathExpression::eFROM_ANCESTORS_OR_SELF:
        opPos = findAncestorsOrSelf(executionContext, context, opPos, stepType, *subQueryResults);
        break;

    case XPathExpression::eMATCH_ATTRIBUTE:
        continueStepRecursion = false;
        // fall-through on purpose.

    case XPathExpression::eFROM_ATTRIBUTES:
        opPos = findAttributes(executionContext, context, opPos, stepType, *subQueryResults);
        break;

    case XPathExpression::eMATCH_ANY_ANCESTOR:
    case XPathExpression::eMATCH_IMMEDIATE_ANCESTOR:
    case XPathExpression::eMATCH_ANY_ANCESTOR_WITH_PREDICATE:
        continueStepRecursion = false;
        // fall-through on purpose.

    case XPathExpression::eFROM_CHILDREN:
        opPos = findChildren(executionContext, context, opPos, stepType, *subQueryResults);
        break;

    case XPathExpression::eFROM_DESCENDANTS:
    case XPathExpression::eFROM_DESCENDANTS_OR_SELF:
        opPos = findDescendants(executionContext, context, opPos, stepType, *subQueryResults);
        break;

    case XPathExpression::eFROM_FOLLOWING:
        opPos = findFollowing(executionContext, context, opPos, stepType, *subQueryResults);
        break;

    case XPathExpression::eFROM_FOLLOWING_SIBLINGS:
        opPos = findFollowingSiblings(executionContext, context, opPos, stepType, *subQueryResults);
        break;

    case XPathExpression::eFROM_PRECEDING:
        opPos = findPreceeding(executionContext, context, opPos, stepType, *subQueryResults);
        break;

    case XPathExpression::eFROM_PRECEDING_SIBLINGS:
        opPos = findPreceedingSiblings(executionContext, context, opPos, stepType, *subQueryResults);
        break;

    case XPathExpression::eFROM_NAMESPACE:
        opPos = findNamespace(executionContext, context, opPos,  stepType, *subQueryResults);
        break;

    default:
        opPos = findNodesOnUnknownAxis(executionContext, context, opPos, stepType, *subQueryResults);
        break;
    }

    OpCodeMapValueType  nextStepType = currentExpression.getOpCodeMapValue(opPos);

    // Push and pop the context node list...
    XPathExecutionContext::ContextNodeListPushAndPop    thePushAndPop(
                                        executionContext,
                                        *subQueryResults);

    if(XPathExpression::eOP_PREDICATE == nextStepType ||
       XPathExpression::eOP_PREDICATE_WITH_POSITION == nextStepType)
    {
        opPos =
            predicates(
                executionContext,
                opPos, 
                *subQueryResults);

        nextStepType = currentExpression.getOpCodeMapValue(opPos);
    }

    if(XPathExpression::eENDOP != nextStepType && continueStepRecursion == true)
    {
        const NodeRefListBase::size_type    nContexts = subQueryResults->getLength();

        if (nContexts > 0)
        {
            for(NodeRefListBase::size_type i = 0; i < nContexts; i++)
            {
                XalanNode* const    node = subQueryResults->item(i);
                assert(node != 0);

                BorrowReturnMutableNodeRefList  mnl(executionContext);

                step(executionContext, node, opPos, *mnl);

                if (mnl->empty() == false)
                {
                    if(queryResults.empty() == false)
                    {
                        queryResults.addNodesInDocOrder(*mnl, executionContext);

                        queryResults.setDocumentOrder();
                    }
                    else
                    {
                        assert(mnl->getDocumentOrder() == true);

                        queryResults.swap(*mnl);
                    }
                }
            }

            if (queryResults.empty() == true)
            {
                queryResults.setDocumentOrder();
            }
        }
    }
    else
    {
        if (subQueryResults->empty() == true)
        {
            queryResults.clear();

            queryResults.setDocumentOrder();
        }
        else if (subQueryResults->getReverseDocumentOrder() == true)
        {
            queryResults.swap(*subQueryResults);

            queryResults.reverse();
        }
        else
        {
            assert(subQueryResults->getDocumentOrder() == true);

            queryResults.swap(*subQueryResults);
        }
    }
}



XPath::eMatchScore
XPath::doStepPredicate(
            XPathExecutionContext&  executionContext,
            XalanNode*              context,
            OpCodeMapPositionType   opPos,
            OpCodeMapPositionType   startOpPos,
            eMatchScore             score) const
{
    const XPathExpression&  currentExpression = getExpression();

    OpCodeMapValueType  nextStepType = currentExpression.getOpCodeMapValue(opPos);

    if (XPathExpression::eOP_PREDICATE == nextStepType ||
        XPathExpression::eOP_PREDICATE_WITH_POSITION == nextStepType)
    {
        do
        {
            // This is a quick hack to look ahead and see if we have
            // number literal as the predicate, i.e. match="foo[1]".
            if (XPathExpression::eOP_PREDICATE_WITH_POSITION == nextStepType)
            {
                if (m_expression.getOpCodeMapValue(opPos + 2) == XPathExpression::eOP_NUMBERLIT)
                {
                    score = handleFoundIndexPositional(
                            executionContext,
                            context,
                            startOpPos);
                }
                else
                {
                    score = handleFoundIndex(
                            executionContext,
                            context,
                            startOpPos);
                }
            }
            else
            {
                const XObjectPtr    pred(predicate(context, opPos, executionContext));
                assert(pred.get() != 0);

                if(XObject::eTypeNumber == pred->getType())
                {
                    score = handleFoundIndex(executionContext, context, startOpPos);
                }
                else if(pred->boolean(executionContext) == false)
                {
                    score = eMatchScoreNone;

                    break;
                }
            }

            opPos = currentExpression.getNextOpCodePosition(opPos);
            nextStepType = currentExpression.getOpCodeMapValue(opPos);
        }
        while(XPathExpression::eOP_PREDICATE == nextStepType ||
              XPathExpression::eOP_PREDICATE_WITH_POSITION == nextStepType);
    }

    return score;
}



XalanNode*
XPath::stepPattern(
            XPathExecutionContext&  executionContext,
            XalanNode*              context, 
            OpCodeMapPositionType   opPos,
            eMatchScore&            scoreHolder) const
{
    const XPathExpression&  currentExpression = getExpression();

    const OpCodeMapPositionType     endStep = currentExpression.getNextOpCodePosition(opPos);
    OpCodeMapValueType              nextStepType = currentExpression.getOpCodeMapValue(endStep);

    bool    fDoPredicates = true;

    if(XPathExpression::eENDOP != nextStepType)
    {
        // Continue step via recursion...
        context = stepPattern(
                        executionContext,
                        context,
                        endStep,
                        scoreHolder);

        if(0 == context)
        {
            scoreHolder = eMatchScoreNone;

        }

        if (scoreHolder == eMatchScoreNone)
        {
            // !!!!!!!!!!!!! Big ugly return here !!!!!!!!!!!!!!!!!!!
            return 0;
        }

        scoreHolder = eMatchScoreOther;

        if (nextStepType != XPathExpression::eMATCH_ANY_ANCESTOR_WITH_FUNCTION_CALL)
        {
            context = DOMServices::getParentOfNode(*context);
        }

        if(0 == context)
        {
            // !!!!!!!!!!!!! Big ugly return here !!!!!!!!!!!!!!!!!!!
            return 0;
        }
    }

    assert(context != 0);

    OpCodeMapValueType  argLen = 0;

    eMatchScore         score = eMatchScoreNone;

    const OpCodeMapPositionType     startOpPos = opPos;
    const OpCodeMapValueType        stepType =
        currentExpression.getOpCodeMapValue(opPos);

    switch(stepType)
    {
    case XPathExpression::eMATCH_ANY_ANCESTOR_WITH_FUNCTION_CALL:
        score = scoreHolder;
        break;

    case XPathExpression::eOP_FUNCTION:
        {
            argLen = currentExpression.getOpCodeLengthFromOpMap(opPos,
                                                                executionContext.getMemoryManager());

            const XObjectPtr        obj(executeMore(context, opPos, executionContext));
            assert(obj.get() != 0);

            const NodeRefListBase&              nl = obj->nodeset();

            const NodeRefListBase::size_type    len = nl.getLength();

            if (nextStepType == XPathExpression::eMATCH_ANY_ANCESTOR_WITH_FUNCTION_CALL)
            {
                bool    fFound = false;

                while(context != 0 && fFound == false)
                {
                    for(NodeRefListBase::size_type i = 0; i < len; i++)
                    {
                        XalanNode* const    n = nl.item(i);

                        if(n == context)
                        {
                            score = eMatchScoreOther;

                            context = n;

                            fFound = true;

                            break;
                        }
                    }

                    context = DOMServices::getParentOfNode(*context);
                }
            }
            else
            {
                for(NodeRefListBase::size_type i = 0; i < len; i++)
                {
                    XalanNode* const    n = nl.item(i);

                    if(n == context)
                    {
                        score = eMatchScoreOther;

                        context = n;

                        break;
                    }
                }
            }
        }
        break;

    case XPathExpression::eFROM_ROOT:
        {
            argLen = currentExpression.getOpCodeArgumentLength(opPos);

            opPos += 3;

            const XalanNode::NodeType   nodeType = context->getNodeType();

            if (nodeType == XalanNode::DOCUMENT_NODE ||
                nodeType == XalanNode::DOCUMENT_FRAGMENT_NODE)
            {
                score = eMatchScoreOther;
            }
            else
            {
                const OpCodeMapPositionType     prevPos = currentExpression.getNextOpCodePosition(startOpPos);      
                const OpCodeMapValueType        prevStepType = currentExpression.getOpCodeMapValue(prevPos);

                if (eMatchScoreNone == score  && 
                    (prevStepType == XPathExpression::eMATCH_ANY_ANCESTOR ||
                     prevStepType == XPathExpression::eMATCH_ANY_ANCESTOR_WITH_PREDICATE))
                {
                    const NodeTester    theTester(
                                    *this,
                                    executionContext,
                                    opPos,
                                    argLen,
                                    stepType);

                    while(0 != context)
                    {
                        score =
                            theTester(*context, context->getNodeType());

                        if(eMatchScoreNone != score)
                            break;

                        context = DOMServices::getParentOfNode(*context);
                    }
                }
            }
        }
        break;

    case XPathExpression::eMATCH_ATTRIBUTE:
        {
            argLen = currentExpression.getOpCodeArgumentLength(opPos);

            opPos += 3;

            score = NodeTester(
                            *this,
                            executionContext,
                            opPos,
                            argLen,
                            XPathExpression::eFROM_ATTRIBUTES)(*context, context->getNodeType());
        }
        break;

    case XPathExpression::eMATCH_ANY_ANCESTOR:
    case XPathExpression::eMATCH_ANY_ANCESTOR_WITH_PREDICATE:
        {
            assert(fDoPredicates == true);
            fDoPredicates = false;

            argLen = currentExpression.getOpCodeArgumentLength(opPos);

            XalanNode::NodeType     nodeType = context->getNodeType();

            if(nodeType != XalanNode::ATTRIBUTE_NODE)
            {
                opPos += 3;

                const NodeTester    theTester(
                                    *this,
                                    executionContext,
                                    opPos,
                                    argLen,
                                    stepType);

                for(;;)
                {
                    score = theTester(*context, nodeType);

                    if (eMatchScoreNone != score)
                    {
                        score = 
                            doStepPredicate(
                                executionContext,
                                context, 
                                opPos + argLen,
                                startOpPos,
                                score);
                        if (eMatchScoreNone != score)
                        {
                            break;
                        }
                    }

                    context = DOMServices::getParentOfNode(*context);

                    if (context == 0)
                        break;

                    nodeType = context->getNodeType();
                }
            }
        }
        break;

    case XPathExpression::eMATCH_IMMEDIATE_ANCESTOR:
        {
            argLen = currentExpression.getOpCodeArgumentLength(opPos);

            const XalanNode::NodeType   nodeType = context->getNodeType();

            if(nodeType != XalanNode::ATTRIBUTE_NODE)
            {
                opPos += 3;

                score = NodeTester(
                                *this,
                                executionContext,
                                opPos,
                                argLen,
                                XPathExpression::eMATCH_IMMEDIATE_ANCESTOR)(*context, nodeType);
            }
        }
        break;

    default:
        {
            const GetCachedString   theGuard1(executionContext);
            const GetCachedString   theGuard2(executionContext);

            executionContext.problem(
                XPathExecutionContext::eXPath,
                XPathExecutionContext::eError,
                XalanMessageLoader::getMessage(
                    theGuard1.get(),
                    XalanMessages::UnknownMatchOpCode_1Param,
                    NumberToDOMString(
                        stepType,
                        theGuard2.get())),
                getLocator(),
                context);

            break;
        }
    }

    if (fDoPredicates == true && score != eMatchScoreNone)
    {
        score =
            doStepPredicate(
                executionContext,
                context, 
                opPos + argLen,
                startOpPos,
                score);
    }

    if (scoreHolder == eMatchScoreNone || 
        score == eMatchScoreNone)
    {
        scoreHolder = score;
    }

    return score == eMatchScoreNone ? 0 : context;
}



XPath::eMatchScore
XPath::handleFoundIndex(
            XPathExecutionContext&  executionContext,
            XalanNode*              localContext, 
            OpCodeMapPositionType   startOpPos) const
{
    // We have an index somewhere in our pattern.  So, we have 
    // to do a full search for our step, using the parent as 
    // localContext, then see if the current localContext is found in the 
    // node set.  Seems crazy, but, so far, it seems like the 
    // easiest way.
    XalanNode* const    parentContext =
                DOMServices::getParentOfNode(*localContext);

    if (parentContext == 0)
    {
        return eMatchScoreNone;
    }
    else
    {
        typedef XPathExecutionContext::BorrowReturnMutableNodeRefList   BorrowReturnMutableNodeRefList;

        BorrowReturnMutableNodeRefList  mnl(executionContext);

        step(executionContext, parentContext, startOpPos, *mnl);

        if (mnl->indexOf(localContext) == MutableNodeRefList::npos)
        {
            return eMatchScoreNone;
        }
        else
        {
            return eMatchScoreOther;
        }
    }
}



XPath::eMatchScore
XPath::handleFoundIndexPositional(
            XPathExecutionContext&  executionContext,
            XalanNode*              localContext,
            OpCodeMapPositionType   startOpPos) const
{
    XalanNode* const    parentContext =
                DOMServices::getParentOfNode(*localContext);

    if (parentContext == 0)
    {
        return eMatchScoreNone;
    }
    else
    {
        typedef XPathExecutionContext::BorrowReturnMutableNodeRefList   BorrowReturnMutableNodeRefList;

        BorrowReturnMutableNodeRefList  mnl(executionContext);

        step(executionContext, parentContext, startOpPos, *mnl);

        if (mnl->empty() == true)
        {
            return eMatchScoreNone;
        }
        else
        {
            assert(mnl->getLength() == 1 && mnl->item(0) == localContext);

            return eMatchScoreOther;
        }
    }
}



XPath::OpCodeMapPositionType
XPath::findNodeSet(
            XPathExecutionContext&  executionContext,
            XalanNode*              context, 
            OpCodeMapPositionType   opPos,
            OpCodeMapValueType      /* stepType */,
            MutableNodeRefList&     subQueryResults) const
{
    assert(subQueryResults.empty() == true);

    const XObjectPtr    nodesetResult(executeMore(context, opPos, executionContext, subQueryResults));

    if (nodesetResult.null() == true)
    {
        assert(subQueryResults.getDocumentOrder());
    }
    else
    {
        subQueryResults.addNodesInDocOrder(nodesetResult->nodeset(), executionContext);

        subQueryResults.setDocumentOrder();
    }

    return opPos + getExpression().getOpCodeLengthFromOpMap(opPos, executionContext.getMemoryManager());
}



XPath::OpCodeMapPositionType
XPath::findRoot(
            XPathExecutionContext&  /* executionContext */,
            XalanNode*              context, 
            OpCodeMapPositionType   opPos,
            OpCodeMapValueType      /* stepType */,
            MutableNodeRefList&     subQueryResults) const
{
    assert(context != 0);
    assert(subQueryResults.empty() == true);

    const XPathExpression&  currentExpression = getExpression();

    const OpCodeMapValueType    argLen =
        currentExpression.getOpCodeArgumentLength(opPos);

    XalanNode::NodeType   nodeType =
                context->getNodeType();

    XalanNode*  docContext =
        nodeType == XalanNode::DOCUMENT_NODE ?
            context :
            context->getOwnerDocument();

    // This is a special case for RTFs, as the "owner document" is
    // just a factory for the RTF.  Instead, we have to search for
    // the containing XalanDocumentFragment node.
    if (static_cast<const XalanDocument*>(docContext)->getDocumentElement() == 0)
    {
        docContext = context;

        for(;;)
        {
            if (nodeType == XalanNode::DOCUMENT_FRAGMENT_NODE)
            {
                break;
            }
            else
            {
                docContext =
                    DOMServices::getParentOfNode(*docContext);
                assert(docContext != 0);

                nodeType = docContext->getNodeType();

            }
        }
    }

    assert(docContext != 0);

    subQueryResults.addNode(docContext);

    subQueryResults.setDocumentOrder();

    return opPos + argLen + 3;
}



XPath::OpCodeMapPositionType
XPath::findParent(
            XPathExecutionContext&  executionContext,
            XalanNode*              context, 
            OpCodeMapPositionType   opPos,
            OpCodeMapValueType      stepType,
            MutableNodeRefList&     subQueryResults) const
{
    assert(subQueryResults.empty() == true);
    assert(context != 0);

    const XPathExpression&  currentExpression = getExpression();

    const OpCodeMapValueType    argLen =
            currentExpression.getOpCodeArgumentLength(opPos);

    XalanNode* const    theParent = DOMServices::getParentOfNode(*context);

    opPos += 3;

    if(0 != theParent)
    {
        if(argLen > 0)
        {
            const NodeTester    theTester(
                            *this,
                            executionContext,
                            opPos,
                            argLen,
                            stepType);

            const eMatchScore   score = theTester(*theParent, theParent->getNodeType());

            if(eMatchScoreNone != score)
            {
                subQueryResults.addNode(theParent);
            }
        }
        else
        {
            subQueryResults.addNode(theParent);
        }
    }

    subQueryResults.setDocumentOrder();

    return opPos + argLen;
}



XPath::OpCodeMapPositionType
XPath::findSelf(
            XPathExecutionContext&  executionContext,
            XalanNode*              context, 
            OpCodeMapPositionType   opPos,
            OpCodeMapValueType      stepType,
            MutableNodeRefList&     subQueryResults) const
{
    assert(subQueryResults.empty() == true);
    assert(context != 0);

    const XPathExpression&  currentExpression = getExpression();

    const OpCodeMapValueType    argLen =
        currentExpression.getOpCodeArgumentLength(opPos);

    opPos += 3;

    if(argLen == 0)
    {
        subQueryResults.addNode(context);
    }
    else
    {
        const NodeTester    theTester(
                        *this,
                        executionContext,
                        opPos,
                        argLen,
                        stepType);

        const eMatchScore   score =
            theTester(*context, context->getNodeType());

        if(eMatchScoreNone != score)
        {
            subQueryResults.addNode(context);
        }
    }

    subQueryResults.setDocumentOrder();

    return opPos + argLen;
}



XPath::OpCodeMapPositionType
XPath::findAncestors(
            XPathExecutionContext&  executionContext,
            XalanNode*              context, 
            OpCodeMapPositionType   opPos,
            OpCodeMapValueType      stepType,
            MutableNodeRefList&     subQueryResults) const
{
    assert(subQueryResults.empty() == true);
    assert(context != 0);

    context = DOMServices::getParentOfNode(*context);

    const XPathExpression&  currentExpression = getExpression();

    const OpCodeMapValueType    argLen =
            currentExpression.getOpCodeArgumentLength(opPos);

    opPos += 3;

    if (context != 0)
    {
        const NodeTester    theTester(
                        *this,
                        executionContext,
                        opPos,
                        argLen,
                        stepType);

        do
        {
            const eMatchScore   score =
                theTester(*context, context->getNodeType());

            if(eMatchScoreNone != score)
            {
                subQueryResults.addNode(context);
            }

            context = DOMServices::getParentOfNode(*context);
        } while(0 != context);
    }

    subQueryResults.setReverseDocumentOrder();

    return opPos + argLen;
}



XPath::OpCodeMapPositionType
XPath::findAncestorsOrSelf(
            XPathExecutionContext&  executionContext,
            XalanNode*              context, 
            OpCodeMapPositionType   opPos,
            OpCodeMapValueType      stepType,
            MutableNodeRefList&     subQueryResults) const
{
    assert(subQueryResults.empty() == true);
    assert(context != 0);

    const XPathExpression&  currentExpression = getExpression();

    const OpCodeMapValueType    argLen =
        currentExpression.getOpCodeArgumentLength(opPos);

    opPos += 3;

    const NodeTester    theTester(
                    *this,
                    executionContext,
                    opPos,
                    argLen,
                    stepType);

    do
    {
        const eMatchScore   score =
                theTester(*context, context->getNodeType());

        if(eMatchScoreNone != score)
        {
            subQueryResults.addNode(context);
        }

        context = DOMServices::getParentOfNode(*context);
    } while(0 != context);

    subQueryResults.setReverseDocumentOrder();

    return opPos + argLen;
}



XPath::OpCodeMapPositionType
XPath::findAttributes(
            XPathExecutionContext&  executionContext,
            XalanNode*              context, 
            OpCodeMapPositionType   opPos,
            OpCodeMapValueType      stepType,
            MutableNodeRefList&     subQueryResults) const
{
    assert(subQueryResults.empty() == true);
    assert(context != 0);

    const XPathExpression&  currentExpression = getExpression();

    const OpCodeMapValueType    argLen =
                currentExpression.getOpCodeArgumentLength(opPos);

    opPos += 3;

    if(context->getNodeType() == XalanNode::ELEMENT_NODE)
    {
        const XalanNamedNodeMap* const  attributeList = context->getAttributes();

        if(attributeList != 0) 
        {
            const XalanSize_t  nAttrs = attributeList->getLength();

            if (nAttrs != 0)
            {
                const NodeTester    theTester(
                                *this,
                                executionContext,
                                opPos,
                                argLen,
                                stepType);

                for (XalanSize_t j = 0; j < nAttrs; j++)
                {
                    XalanNode* const    theNode = attributeList->item(j);
                    assert(theNode != 0 && theNode->getNodeType() == XalanNode::ATTRIBUTE_NODE);

                    const eMatchScore   score =
                        theTester(*theNode, XalanNode::ATTRIBUTE_NODE);

                    if(eMatchScoreNone != score)
                    {
                        subQueryResults.addNode(theNode);
                    }
                }
            }
        }
    }

    subQueryResults.setDocumentOrder();

    return opPos + argLen;
}



XPath::OpCodeMapPositionType
XPath::findChildren(
            XPathExecutionContext&  executionContext,
            XalanNode*              context, 
            OpCodeMapPositionType   opPos,
            OpCodeMapValueType      stepType,
            MutableNodeRefList&     subQueryResults) const
{
    assert(subQueryResults.empty() == true);
    assert(context != 0);

    XalanNode*  child = context->getFirstChild();

    const XPathExpression&  currentExpression = getExpression();

    const OpCodeMapValueType    argLen =
        currentExpression.getOpCodeArgumentLength(opPos);

    opPos += 3;

    if (child != 0)
    {
        const NodeTester    theTester(
                        *this,
                        executionContext,
                        opPos,
                        argLen,
                        stepType);

        do
        {
            const eMatchScore   score =
                    theTester(*child, child->getNodeType());

            if(eMatchScoreNone != score)
            {
                subQueryResults.addNode(child);
            }

            child = child->getNextSibling();
        } while(0 != child);
    }

    subQueryResults.setDocumentOrder();

    return opPos + argLen;
}



XPath::OpCodeMapPositionType
XPath::findDescendants(
            XPathExecutionContext&  executionContext,
            XalanNode*              context, 
            OpCodeMapPositionType   opPos,
            OpCodeMapValueType      stepType,
            MutableNodeRefList&     subQueryResults) const
{
    assert(subQueryResults.empty() == true);
    assert(context != 0);

    const XPathExpression&  currentExpression = getExpression();

    const OpCodeMapValueType    argLen =
        currentExpression.getOpCodeArgumentLength(opPos);

    // Perform a pre-order traversal of descendents...
    XalanNode*  pos = context;

    opPos += 3;

    const NodeTester    theTester(
                        *this,
                        executionContext,
                        opPos,
                        argLen,
                        stepType);

    do
    {                   
        if(stepType == XPathExpression::eFROM_DESCENDANTS_OR_SELF ||
           context != pos)
        {
            const eMatchScore   score =
                theTester(*pos, pos->getNodeType());

            if(score != eMatchScoreNone)
            {
                subQueryResults.addNode(pos);
            }
        }

        XalanNode*  nextNode = pos->getFirstChild();

        while(0 == nextNode)
        {
            if(context == pos)
                break;

            nextNode = pos->getNextSibling();

            if(0 == nextNode)
            {
                pos = DOMServices::getParentOfNode(*pos);

                if(context == pos || pos == 0)
                {
                    nextNode = 0;
                    break;
                }
            }
        }

        pos = nextNode;
    } while(0 != pos);

    subQueryResults.setDocumentOrder();

    return opPos + argLen;
}



XPath::OpCodeMapPositionType
XPath::findFollowing(
            XPathExecutionContext&  executionContext,
            XalanNode*              context, 
            OpCodeMapPositionType   opPos,
            OpCodeMapValueType      stepType,
            MutableNodeRefList&     subQueryResults) const
{
    assert(subQueryResults.empty() == true);
    assert(context != 0);

    const XPathExpression&  currentExpression = getExpression();

    const OpCodeMapValueType    argLen =
        currentExpression.getOpCodeArgumentLength(opPos);

    opPos += 3;

    // What fun...
    XalanDocument* const    doc = context->getOwnerDocument();

    XalanNode*              pos = context;

    const NodeTester    theTester(
                    *this,
                    executionContext,
                    opPos,
                    argLen,
                    stepType);

    while(0 != pos)
    {
        XalanNode*  nextNode = 0;

        if(pos != context)
        {
            const eMatchScore   score =
                theTester(*pos, pos->getNodeType());

            if(eMatchScoreNone != score)
            {
                subQueryResults.addNodeInDocOrder(pos, executionContext);
            }

            nextNode = pos->getFirstChild();
        }
        else
        {
            nextNode = 0;
        }

        while(0 == nextNode)
        {
            // This requires some explanation.  pos could be an attribute
            // node, so getNextSibling() will always return 0.  In that
            // case, I want to continue the search with the first child of
            // the owner element, as if attribute nodes are children which
            // are always _before_ the first child element.  I don't have to
            // consider following attributes, since they _never_ match the
            // following axis.
            if (pos->getNodeType() == XalanNode::ATTRIBUTE_NODE)
            {
                assert(DOMServices::getParentOfNode(*pos) != 0);

                nextNode = DOMServices::getParentOfNode(*pos)->getFirstChild();
            }
            else
            {
                nextNode = pos->getNextSibling();
            }

            if(0 == nextNode)
            {
                pos = DOMServices::getParentOfNode(*pos);

                if(doc == pos || 0 == pos)
                {
                    nextNode = 0;

                    break;
                }
            }
        }

        pos = nextNode;
    }

    subQueryResults.setDocumentOrder();

    return opPos + argLen;
}



XPath::OpCodeMapPositionType
XPath::findFollowingSiblings(
            XPathExecutionContext&  executionContext,
            XalanNode*              context, 
            OpCodeMapPositionType   opPos,
            OpCodeMapValueType      stepType,
            MutableNodeRefList&     subQueryResults) const
{
    assert(subQueryResults.empty() == true);
    assert(context != 0);

    const XPathExpression&  currentExpression = getExpression();

    const OpCodeMapValueType    argLen =
        currentExpression.getOpCodeArgumentLength(opPos);

    XalanNode*  pos = context->getNextSibling();

    opPos += 3;

    if (pos != 0)
    {
        const NodeTester    theTester(
                        *this,
                        executionContext,
                        opPos,
                        argLen,
                        stepType);

        do
        {
            const eMatchScore   score =
                    theTester(*pos, pos->getNodeType());

            if(eMatchScoreNone != score)
            {
                subQueryResults.addNode(pos);
            }

            pos = pos->getNextSibling();
        } while(0 != pos);
    }

    subQueryResults.setDocumentOrder();

    return opPos + argLen;
}



/*
 * This is bit of a hack to figure out where we should start for
 * the preceeding axis.  It's mostly to support RTFs which have
 * been coerced to nodesets through the nodeset() extension function.
 */
static XalanNode*
findTopNode(
            XalanNode*              context,
            XalanNode::NodeType     theType)
{
    assert(context != 0);

    if (theType == XalanNode::DOCUMENT_NODE)
    {
        return context;
    }
    else
    {
        XalanDocument* const    theDocument = context->getOwnerDocument();
        assert(theDocument != 0);

        if (theDocument->getDocumentElement() != 0)
        {
            return theDocument;
        }
        else
        {
            XalanNode*  theNode = 0;

            while(context != 0 &&
                  context->getNodeType() != XalanNode::DOCUMENT_NODE)
            {
                theNode = context;

                context = DOMServices::getParentOfNode(*context);
            }

            assert(theNode != 0);

            return theNode;
        }
    }
}



XPath::OpCodeMapPositionType
XPath::findPreceeding(
            XPathExecutionContext&  executionContext,
            XalanNode*              context, 
            OpCodeMapPositionType   opPos,
            OpCodeMapValueType      stepType,
            MutableNodeRefList&     subQueryResults) const
{
    assert(subQueryResults.empty() == true);
    assert(context != 0);

    const XPathExpression&  currentExpression = getExpression();

    const OpCodeMapValueType    argLen =
        currentExpression.getOpCodeArgumentLength(opPos);

    opPos += 3;

    // Ugh.  Reverse document order, no parents, I guess.
    const XalanNode::NodeType   theType = context->getNodeType();

    XalanNode* const    topNode = findTopNode(context, theType);

    XalanNode*          pos = topNode;

    // If the context node is an attribute, we need to perform some
    // magic to stop the search at the appropriate point, which is when
    // we arrive back at the parent.
    const bool              contextIsAttribute =
            theType == XalanNode::ATTRIBUTE_NODE ? true : false;

    const XalanNode* const  theAttributeContextParent =
        contextIsAttribute == true ? DOMServices::getParentOfNode(*context) : 0;

    const NodeTester    theTester(
                        *this,
                        executionContext,
                        opPos,
                        argLen,
                        stepType);

    while(0 != pos)
    {
        if(context == pos)
        {
            break;
        }

        const eMatchScore   score =
                theTester(*pos, pos->getNodeType());

        if(eMatchScoreNone != score)
        {
            // Ugh. If I could think a little better tonight, I'm
            // sure there's a better way to check for the parent.
            bool        isParent = false;

            XalanNode*  parent = DOMServices::getParentOfNode(*context);

            while(0 != parent)
            {
                if(parent == pos)
                {
                    isParent = true;
                    break;
                }

                parent = DOMServices::getParentOfNode(*parent);
            }

            if(isParent == false)
            {
                subQueryResults.addNode(pos);
            }
        }

        XalanNode*  nextNode = 0;

        // Check to see if we're back at the attribute context node's
        // parent, in which case, we should stop.
        if (contextIsAttribute == true &&
            pos == theAttributeContextParent)
        {
            nextNode = context;
        }
        else
        {
            nextNode = pos->getFirstChild();
        }

        while(0 == nextNode)
        {
            nextNode = pos->getNextSibling();

            if(0 == nextNode)
            {
                pos = DOMServices::getParentOfNode(*pos);

                if(topNode == pos)
                {
                    nextNode = 0;
                    break;
                }
            }
        }

        pos = nextNode;
    }

    // Now, reverse the order of the nodes, since
    // preceeding is a reverse axis, and we searched
    // the document from the root to this node.
    subQueryResults.reverse();

    subQueryResults.setReverseDocumentOrder();

    return opPos + argLen;
}



XPath::OpCodeMapPositionType
XPath::findPreceedingSiblings(
            XPathExecutionContext&  executionContext,
            XalanNode*              context, 
            OpCodeMapPositionType   opPos,
            OpCodeMapValueType      stepType,
            MutableNodeRefList&     subQueryResults) const
{
    assert(subQueryResults.empty() == true);
    assert(context != 0);

    const XPathExpression&  currentExpression = getExpression();

    const OpCodeMapValueType    argLen =
        currentExpression.getOpCodeArgumentLength(opPos);

    opPos += 3;

#if 1
    XalanNode*  pos = context->getPreviousSibling();

    if (pos != 0)
    {
        const NodeTester    theTester(
                        *this,
                        executionContext,
                        opPos,
                        argLen,
                        stepType);

        do
        {
            const eMatchScore   score = 
                theTester(*pos, pos->getNodeType());

            if(eMatchScoreNone != score)
            {
                subQueryResults.addNode(pos);
            }

            pos = pos->getPreviousSibling();
        } while(0 != pos);
    }
#else
    // This is some experimental code which avoids using getPreviousSibling(), with the idea that we
    // could reduce our in-memory representation size by not keeping the previous sibling node.
    XalanNode*  pos = context->getParentNode();

    if (pos != 0)
    {
        pos = pos->getFirstChild();

        if (pos != context)
        {
            const NodeTester    theTester(
                            *this,
                            executionContext,
                            opPos,
                            argLen,
                            stepType);

            do
            {
                const eMatchScore   score = 
                    theTester(*pos, pos->getNodeType());

                if(eMatchScoreNone != score)
                {
                    subQueryResults.addNode(pos);
                }

                pos = pos->getNextSibling();
            } while(pos != context);
        }

        // Now, reverse the order of the nodes, since
        // preceeding-sibling is a reverse axis, and we searched
        // the document from the root to this node.
        subQueryResults.reverse();
    }
#endif

    subQueryResults.setReverseDocumentOrder();

    return opPos + argLen;
}



XPath::OpCodeMapPositionType
XPath::findNamespace(
            XPathExecutionContext&  executionContext,
            XalanNode*              context, 
            OpCodeMapPositionType   opPos,
            OpCodeMapValueType      stepType,
            MutableNodeRefList&     subQueryResults) const
{
    assert(subQueryResults.empty() == true);
    assert(context != 0);

    const XPathExpression&  currentExpression = getExpression();

    const OpCodeMapValueType    argLen =
        currentExpression.getOpCodeArgumentLength(opPos);

    opPos += 3;

    if(context->getNodeType() == XalanNode::ELEMENT_NODE)
    {
        // Look up the element chain until we hit the document, so that we
        // get all of the attribute/namespace nodes.
        const XalanNode* const  theOwnerDocument = context->getOwnerDocument();
        assert(theOwnerDocument != 0);

        const XalanNode*        theCurrentNode = context;

        const NodeTester    theTester(
                        *this,
                        executionContext,
                        opPos,
                        argLen,
                        stepType);

        NodeRefListBase::size_type  nNSFound = 0;
        bool                        defaultNSFound = false;

        do
        {
            const XalanNamedNodeMap* const  attributeList =
                theCurrentNode->getAttributes();

            if(attributeList != 0) 
            {
                XalanSize_t    nAttrs = attributeList->getLength();

                while (nAttrs > 0)
                {
                    --nAttrs;

                    XalanNode* const    attr = attributeList->item(nAttrs);
                    assert(attr != 0 && attr->getNodeType() == XalanNode::ATTRIBUTE_NODE);

                    const XalanDOMString&   theNodeName = attr->getNodeName();

                    // This is an optimization to keep non-namespace attributes out of
                    // the call to the NodeTester.
                    if (startsWith(theNodeName, DOMServices::s_XMLNamespaceWithSeparator) == true ||
                        theNodeName == DOMServices::s_XMLNamespace)
                    {
                        const eMatchScore   score =
                            theTester(*attr, XalanNode::ATTRIBUTE_NODE);

                        if(score != eMatchScoreNone)
                        {
                            const XalanDOMString&   theNodeValue = attr->getNodeValue();

                            bool    foundNSMatch = false;
 
                            // Need to check default NS slightly differently
                            if (theNodeName == DOMServices::s_XMLNamespace)
                            {
                                // If namespace is empty, don't add anything
                                // as under XPath an empty default is indicated
                                // by no node.
 
                                foundNSMatch = defaultNSFound ||
                                    (theNodeValue == DOMServices::s_emptyString);
                                defaultNSFound = true;
                            }
 
                            for (NodeRefListBase::size_type lstIndex = 0; 
                                 foundNSMatch == false && lstIndex < nNSFound;
                                 ++lstIndex)
                            {
                                if (subQueryResults.item(lstIndex)->getNodeName() == theNodeName)
                                {
                                    foundNSMatch = true;
                                }
                            }

                            if (foundNSMatch == false) 
                            {
                                subQueryResults.addNode(attr);
                                ++nNSFound;
                            }
                        }
                    }
                }
            }

            theCurrentNode = theCurrentNode->getParentNode();
        } while (theCurrentNode != theOwnerDocument && theCurrentNode != 0);
    }

    subQueryResults.reverse();

    subQueryResults.setDocumentOrder();

    return opPos + argLen;
}



XPath::OpCodeMapPositionType
XPath::findNodesOnUnknownAxis(
            XPathExecutionContext&  executionContext,
            XalanNode*              context, 
            OpCodeMapPositionType   opPos,
            OpCodeMapValueType      stepType,
            MutableNodeRefList&     /* subQueryResults */) const
{
    const XPathExpression&  currentExpression = getExpression();

    const OpCodeMapValueType    argLen =
        currentExpression.getOpCodeArgumentLength(opPos);

    const GetCachedString   theGuard1(executionContext);
    const GetCachedString   theGuard2(executionContext);

    executionContext.problem(
            XPathExecutionContext::eXPath,
            XPathExecutionContext::eError,
            XalanMessageLoader::getMessage(
                theGuard1.get(),
                XalanMessages::UnknownAxis_1Param,
                NumberToDOMString(
                    stepType,
                    theGuard2.get())),
            getLocator(),
            context);

    return opPos + argLen + 3;
}



XPath::OpCodeMapPositionType
XPath::predicates(
            XPathExecutionContext&  executionContext,
            OpCodeMapPositionType   opPos,
            MutableNodeRefList&     subQueryResults) const
{
    assert(&executionContext.getContextNodeList() == &subQueryResults);

    const XPathExpression&  currentExpression = getExpression();

    assert(currentExpression.getOpCodeMapValue(opPos) == XPathExpression::eOP_PREDICATE ||
           currentExpression.getOpCodeMapValue(opPos) == XPathExpression::eOP_PREDICATE_WITH_POSITION);

    NodeRefListBase::size_type  theLength = subQueryResults.getLength();

    for(;;)
    {
        // If we have no nodes left, then there's no point in executing any
        // predicates.  However, we will continue to loop, since we need to
        // update endPredicatePos.
        if (theLength > 0)
        {
            const OpCodeMapPositionType predOpPos = opPos + 2;

            // OK, this is a huge hack/optimization.  If the predicate is
            // simple a number, such as [2], we can just get the
            // numeric value from the expression, and not bother executing
            // the predicate.  Furthermore, we don't need to execute the
            // predicate for each node, since the evaluation is no dependent
            // on the context node.  All we really have to do is remove all
            // nodes from subQueryResults, _except_ for the node at that
            // position.  The only trick is that XPath indexes from 1, while
            // our node lists index from 0.
            if (m_expression.getOpCodeMapValue(predOpPos) == XPathExpression::eOP_NUMBERLIT)
            {
                assert(m_expression.tokenQueueSize() > m_expression.getOpCodeMapValue(predOpPos + 3));

                // Get the value of the number...
                const double    theIndex =
                    m_expression.getNumberLiteral(m_expression.getOpCodeMapValue(predOpPos + 2));

                // If the index is out of range, or not an integer, just clear subQueryResults...
                if (theIndex <= 0.0 ||
                    NodeRefListBase::size_type(theIndex) > theLength ||
                    double(NodeRefListBase::size_type(theIndex)) != theIndex)
                {
                    subQueryResults.clear();
                }
                else if (theLength > 1)
                {
                    // Save the matching node...
                    XalanNode* const    theNode =
                        subQueryResults.item(NodeRefListBase::size_type(theIndex) - 1);

                    // Clear the list...
                    subQueryResults.clear();

                    // Add the node back in...
                    subQueryResults.addNode(theNode);

                    subQueryResults.setDocumentOrder();
                }
                else
                {
                    // OK, if there's only 1 node in the list, then
                    // we don't need to bother modifying the list.
                    // Just assert that theIndex == 1.0...
                    assert(theIndex == 1.0);
                }
            }
            else
            {
                for(NodeRefListBase::size_type i = 0; i < theLength; ++i)
                {
                    XalanNode* const    theNode = subQueryResults.item(i);
                    assert(theNode != 0);

                    const XObjectPtr        pred(predicate(theNode, opPos, executionContext));
                    assert(pred.get() != 0);

                    // Remove any node that doesn't satisfy the predicate.
                    if((XObject::eTypeNumber == pred->getType() && i + 1 != pred->num(executionContext)) ||
                        pred->boolean(executionContext) == false)
                    {
                        // Set the node to 0.  After we're done,
                        // we'll clear it out.
                        subQueryResults.setNode(i, 0);
                    }
                }

                // Clear out any null entries...
                subQueryResults.clearNulls();
            }
        }

        opPos = currentExpression.getNextOpCodePosition(opPos);

        const OpCodeMapValueType    nextStepType = currentExpression.getOpCodeMapValue(opPos);

        if (nextStepType != XPathExpression::eOP_PREDICATE &&
            nextStepType != XPathExpression::eOP_PREDICATE_WITH_POSITION)
        {
            break;
        }
        else
        {
            theLength = subQueryResults.getLength();
        }
    }

    return opPos;
}



XPath::NodeTester::NodeTester(const NodeTester&     theSource) :
    m_executionContext(theSource.m_executionContext),
    m_targetNamespace(theSource.m_targetNamespace),
    m_targetLocalName(theSource.m_targetLocalName),
    m_testFunction(theSource.m_testFunction),
    m_testFunction2(theSource.m_testFunction2)
{
}



XPath::NodeTester::NodeTester(
            const XPath&            xpath,
            XPathExecutionContext&  executionContext,
            OpCodeMapPositionType   opPos,
            OpCodeMapValueType      argLen,
            OpCodeMapValueType      stepType) :
    m_executionContext(&executionContext),
    m_targetNamespace(0),
    m_targetLocalName(0),
    m_testFunction(0),
    m_testFunction2(&NodeTester::testDefault2)
{
    const XPathExpression&  theExpression = xpath.getExpression();

    switch(theExpression.getOpCodeMapValue(opPos))
    {
    case XPathExpression::eNODETYPE_COMMENT:
        m_testFunction = &NodeTester::testComment;
        break;

    case XPathExpression::eNODETYPE_TEXT:
        m_testFunction = &NodeTester::testText;
        break;

    case XPathExpression::eNODETYPE_PI:
        if (argLen == 1)
        {
            m_testFunction = &NodeTester::testPI;
        }
        else if(argLen == 2)
        {
            m_testFunction = &NodeTester::testPIName;

            m_targetLocalName = getStringFromTokenQueue(
                theExpression,
                opPos + 1);
        }
        else
        {
            GetCachedString     theGuard(executionContext);

            executionContext.problem(
                XPathExecutionContext::eXPath,
                XPathExecutionContext::eError,
                XalanMessageLoader::getMessage(
                    theGuard.get(),
                    XalanMessages::ArgLengthNodeTestIsIncorrect_1Param,
                    "processing-instruction()"),
                xpath.getLocator(),
                executionContext.getCurrentNode());
        }
        break;

    case XPathExpression::eNODETYPE_NODE:
        m_testFunction = &NodeTester::testNode;
        break;

    case XPathExpression::eNODETYPE_ROOT:
        m_testFunction = &NodeTester::testRoot;
        break;

    case XPathExpression::eNODENAME:
        {
            bool    isTotallyWild = false;

            m_targetNamespace = getStringFromTokenQueue(
                    theExpression,
                    opPos + 1);

            if (m_targetNamespace == 0 &&
                theExpression.getOpCodeMapValue(opPos + 2) == XPathExpression::eELEMWILDCARD)
            {
                isTotallyWild = true;
            }
            else
            {
                m_targetLocalName = getStringFromTokenQueue(
                    theExpression,
                    opPos + 2);
            }

            if(stepType == XPathExpression::eFROM_ATTRIBUTES)
            {
                if (isTotallyWild == true)
                {
                    m_testFunction = &NodeTester::testAttributeTotallyWild;
                }
                else if (m_targetNamespace == 0)
                {
                    assert(m_targetLocalName != 0);

                    m_testFunction = &NodeTester::testAttributeNCName;
                }
                else if (m_targetLocalName == 0)
                {
                    assert(m_targetNamespace != 0);

                    m_testFunction = &NodeTester::testAttributeNamespaceOnly;
                }
                else
                {
                    assert(m_targetNamespace != 0 && m_targetLocalName != 0);


                    m_testFunction = &NodeTester::testAttributeQName;
                }
            }
            else if (stepType == XPathExpression::eFROM_NAMESPACE)
            {
                if (isTotallyWild == true)
                {
                    m_testFunction = &NodeTester::testNamespaceTotallyWild;
                }
                else
                {
                    m_testFunction = &NodeTester::testNamespaceNCName;
                }
            }
            else
            {
                if (isTotallyWild == true)
                {
                    m_testFunction = &NodeTester::testElementTotallyWild;
                }
                else if (m_targetNamespace == 0)
                {
                    m_testFunction = &NodeTester::testElementNCName;
                }
                else if (m_targetLocalName == 0)
                {
                    assert(m_targetNamespace != 0);

                    m_testFunction = &NodeTester::testElementNamespaceOnly;
                }
                else
                {
                    assert(m_targetNamespace != 0 && m_targetLocalName != 0);

                    m_testFunction = &NodeTester::testElementQName;
                }
            }
        }
        break;

    default:
        m_testFunction = &NodeTester::testDefault;
        break;
    }

    assert(m_testFunction != 0);
}



XPath::NodeTester::NodeTester() :
    m_executionContext(0),
    m_targetNamespace(0),
    m_targetLocalName(0),
    m_testFunction(&NodeTester::testDefault),
    m_testFunction2(&NodeTester::testDefault2)
{
}



XPath::NodeTester::NodeTester(
            XPathConstructionContext&   theConstructionContext,
            const XalanDOMString&       theNameTest,
            const PrefixResolver&       thePrefixResolver,
            const Locator*              theLocator,
            eMatchScore*                theMatchScore) :
    m_executionContext(0),
    m_targetNamespace(0),
    m_targetLocalName(0),
    m_testFunction(&NodeTester::testDefault),
    m_testFunction2(&NodeTester::testDefault2)
{
    const eMatchScore   theScore =
        initialize(
            theConstructionContext,
            theNameTest,
            thePrefixResolver,
            theLocator);

    if (theMatchScore != 0)
    {
        *theMatchScore = theScore;
    }
}
    


XPath::NodeTester::NodeTester(
            const XalanDOMString&   theNamespaceURI,
            const XalanDOMString&   theLocalName,
            eMatchScore*            theMatchScore) :
    m_executionContext(0),
    m_targetNamespace(0),
    m_targetLocalName(0),
    m_testFunction(&NodeTester::testDefault),
    m_testFunction2(0)
{
    const eMatchScore   theScore =
        initialize(theNamespaceURI, theLocalName);

    if (theMatchScore != 0)
    {
        *theMatchScore = theScore;
    }
}



XPath::eMatchScore
XPath::NodeTester::initialize(
            XPathConstructionContext&   theConstructionContext,
            const XalanDOMString&       theNameTest,
            const PrefixResolver&       thePrefixResolver,
            const Locator*              theLocator)
{
    const XalanDOMString::size_type     theLength =
                theNameTest.length();

    if (theLength == 1 && theNameTest[0] == XPath::PSEUDONAME_ANY[0])
    {
        return initialize(s_emptyString, s_emptyString);
    }
    else
    {
        eMatchScore     theResult = eMatchScoreNone;

        const XalanDOMString::size_type     theIndex =
                indexOf(theNameTest, XalanUnicode::charColon);

        // If there's no ':', it's an NCName...
        if (theIndex == theLength)
        {
            if (XalanQName::isValidNCName(theNameTest) == false)
            {
                const XPathConstructionContext::GetCachedString     theGuard(theConstructionContext);

                theConstructionContext.problem(
                    XPathConstructionContext::eXPath,
                    XPathConstructionContext::eError,
                    XalanMessageLoader::getMessage(
                        theGuard.get(),
                        XalanMessages::IsNotValidQName_1Param,
                        theNameTest),
                        theLocator,
                        0);
            }
            else
            {
                theResult = initialize(
                    s_emptyString,
                    theConstructionContext.getPooledString(theNameTest));
            }
        }
        else
        {
            const XPathConstructionContext::GetCachedString     scratchGuard(theConstructionContext);

            XalanDOMString&     theScratchString = scratchGuard.get();

            theScratchString.assign(theNameTest, 0, theIndex);

            // Get the namespace URI for the prefix...
            const XalanDOMString* const     theNamespaceURI =
                thePrefixResolver.getNamespaceForPrefix(theScratchString);

            if (theNamespaceURI == 0)
            {
                theConstructionContext.problem(
                    XPathConstructionContext::eXPath,
                    XPathConstructionContext::eError,
                    XalanMessageLoader::getMessage(
                        theScratchString,
                        XalanMessages::PrefixIsNotDeclared_1Param),
                        theLocator,
                        0);
            }
            else
            {
                // OK, now we have a namespace URI...
                if (XalanQName::isValidNCName(theScratchString) == false)
                {
                    theConstructionContext.problem(
                        XPathConstructionContext::eXPath,
                        XPathConstructionContext::eError,
                        XalanMessageLoader::getMessage(
                            theScratchString,
                            XalanMessages::IsNotValidQName_1Param,
                            theNameTest),
                            theLocator,
                            0);
                }
                else if (theIndex == theLength - 2 &&
                         theNameTest[theIndex + 1] == XPath::PSEUDONAME_ANY[0])
                {
                    // It's of the form "NCName:*"
                    theResult = initialize(
                        theConstructionContext.getPooledString(*theNamespaceURI),
                        s_emptyString);
                }
                else
                {
                    theScratchString.assign(theNameTest, theIndex + 1, theLength - theIndex - 1);

                    if (XalanQName::isValidNCName(theScratchString) == false)
                    {
                        theConstructionContext.problem(
                            XPathConstructionContext::eXPath,
                            XPathConstructionContext::eError,
                            XalanMessageLoader::getMessage(
                                theScratchString,
                                XalanMessages::IsNotValidQName_1Param,
                                theNameTest),
                            theLocator,
                            0);
                    }
                    else
                    {
                        // It's of the form "NCName:NCName"
                        theResult = initialize(
                            theConstructionContext.getPooledString(*theNamespaceURI),
                            theConstructionContext.getPooledString(theScratchString));
                    }
                }
            }
        }

        return theResult;
    }
}



XPath::eMatchScore
XPath::NodeTester::initialize(
            const XalanDOMString&   theNamespaceURI,
            const XalanDOMString&   theLocalName)
{
    if (theNamespaceURI.empty() == false)
    {
        m_targetNamespace = &theNamespaceURI;

        if (theLocalName.empty() == true)
        {
            m_testFunction2 = &NodeTester::testElementNamespaceOnly2;

            return eMatchScoreNSWild;
        }
        else
        {
            m_testFunction2 = &NodeTester::testElementQName2;

            m_targetLocalName = &theLocalName;

            return eMatchScoreQName;
        }
    }
    else if (theLocalName.empty() == false)
    {
        m_testFunction2 = &NodeTester::testElementNCName2;

        m_targetLocalName = &theLocalName;

        return eMatchScoreQName;
    }
    else
    {
        m_testFunction2 = &NodeTester::testElementTotallyWild2;

        return eMatchScoreNodeTest;
    }
}



inline bool
isNamespaceDeclaration(const XalanNode&     theAttributeNode)
{
    assert(theAttributeNode.getNodeType() == XalanNode::ATTRIBUTE_NODE);

    return DOMServices::isNamespaceDeclaration(static_cast<const XalanAttr&>(theAttributeNode));
}



// MSVC 6 generates some really horrible code for some of these very simple functions when they're inlined...
#if defined(_MSC_VER) && _MSC_VER <= 1300 && !defined(__INTEL_COMPILER)
#pragma auto_inline(off)
#endif

XPath::eMatchScore
XPath::NodeTester::testComment(
            const XalanNode&        /* context */,
            XalanNode::NodeType     nodeType) const
{
    if (XalanNode::COMMENT_NODE == nodeType)
    {
        return eMatchScoreNodeTest;
    }
    else
    {
        return eMatchScoreNone;
    }
}



XPath::eMatchScore
XPath::NodeTester::testText(
            const XalanNode&        context,
            XalanNode::NodeType     nodeType) const
{
    if (XalanNode::TEXT_NODE == nodeType &&
        shouldStripSourceNode(static_cast<const XalanText&>(context)) == false)
    {
        return eMatchScoreNodeTest;
    }
    else
    {
        return eMatchScoreNone;
    }
}



XPath::eMatchScore
XPath::NodeTester::testPI(
            const XalanNode&        /* context */,
            XalanNode::NodeType     nodeType) const
{
    if (XalanNode::PROCESSING_INSTRUCTION_NODE == nodeType)
    {
        return eMatchScoreNodeTest;
    }
    else
    {
        return eMatchScoreNone;
    }
}



XPath::eMatchScore
XPath::NodeTester::testPIName(
            const XalanNode&        context,
            XalanNode::NodeType     nodeType) const
{
    assert(m_targetLocalName != 0);

    if (XalanNode::PROCESSING_INSTRUCTION_NODE == nodeType &&
        context.getNodeName() == *m_targetLocalName)
    {
        return eMatchScoreQName;
    }
    else
    {
        return eMatchScoreNone;
    }
}



XPath::eMatchScore
XPath::NodeTester::testNode(
            const XalanNode&        context,
            XalanNode::NodeType     nodeType) const
{
    if (nodeType != XalanNode::TEXT_NODE ||
        shouldStripSourceNode(static_cast<const XalanText&>(context)) == false)
    {
        return eMatchScoreNodeTest;
    }
    else
    {
        return eMatchScoreNone;
    }
}



XPath::eMatchScore
XPath::NodeTester::testRoot(
            const XalanNode&        /* context */,
            XalanNode::NodeType     nodeType) const
{
    if (XalanNode::DOCUMENT_NODE == nodeType ||
        XalanNode::DOCUMENT_FRAGMENT_NODE == nodeType)
    {
        return eMatchScoreOther;
    }
    else
    {
        return eMatchScoreNone;
    }
}



XPath::eMatchScore
XPath::NodeTester::testAttributeNCName(
            const XalanNode&        context,
            XalanNode::NodeType     nodeType) const
{
    assert(m_targetNamespace == 0 && m_targetLocalName != 0);

    if (XalanNode::ATTRIBUTE_NODE != nodeType ||
        isNamespaceDeclaration(context) == true ||
        matchLocalName(context) == false)
    {
        return eMatchScoreNone;
    }
    else
    {
        return eMatchScoreQName;
    }
}

inline bool
XPath::NodeTester::matchLocalNameAndNamespaceURI(const XalanNode&   context) const
{
    assert(m_targetNamespace != 0 && m_targetLocalName != 0);

    return DOMServices::getLocalNameOfNode(context) == *m_targetLocalName &&
           context.getNamespaceURI() == *m_targetNamespace;
}


XPath::eMatchScore
XPath::NodeTester::testAttributeQName(
            const XalanNode&        context,
            XalanNode::NodeType     nodeType) const
{
    assert(m_targetNamespace != 0 && m_targetLocalName != 0);

    if (XalanNode::ATTRIBUTE_NODE != nodeType ||
        isNamespaceDeclaration(context) == true ||
        matchLocalNameAndNamespaceURI(context) == false)
    {
        return eMatchScoreNone;
    }
    else
    {
        return eMatchScoreQName;
    }
}



XPath::eMatchScore
XPath::NodeTester::testAttributeNamespaceOnly(
            const XalanNode&        context,
            XalanNode::NodeType     nodeType) const
{
    assert(m_targetNamespace != 0 && m_targetLocalName == 0);

    if (XalanNode::ATTRIBUTE_NODE != nodeType ||
        isNamespaceDeclaration(context) == true ||
        matchNamespaceURI(context) == false)
    {
        return eMatchScoreNone;
    }
    else
    {
        return eMatchScoreNSWild;
    }
}



XPath::eMatchScore
XPath::NodeTester::testAttributeTotallyWild(
            const XalanNode&        context,
            XalanNode::NodeType     nodeType) const
{
    if (XalanNode::ATTRIBUTE_NODE != nodeType ||
        isNamespaceDeclaration(context) == true)
    {
        return eMatchScoreNone;
    }
    else
    {
        return eMatchScoreNodeTest;
    }
}



XPath::eMatchScore
XPath::NodeTester::testElementNCName(
            const XalanNode&        context,
            XalanNode::NodeType     nodeType) const
{
    assert(m_targetNamespace == 0 && m_targetLocalName != 0);

    if (XalanNode::ELEMENT_NODE != nodeType ||
        matchLocalName(context) == false)
    {
        return eMatchScoreNone;
    }
    else
    {
        return eMatchScoreQName;
    }
}



XPath::eMatchScore
XPath::NodeTester::testElementQName(
            const XalanNode&        context,
            XalanNode::NodeType     nodeType) const
{
    assert(m_targetNamespace != 0 && m_targetLocalName != 0);

    if (XalanNode::ELEMENT_NODE != nodeType ||
        matchLocalNameAndNamespaceURI(context) == false)
    {
        return eMatchScoreNone;
    }
    else
    {
        return eMatchScoreQName;
    }
}



XPath::eMatchScore
XPath::NodeTester::testElementNamespaceOnly(
            const XalanNode&        context,
            XalanNode::NodeType     nodeType) const
{
    assert(m_targetNamespace != 0 && m_targetLocalName == 0);

    if (XalanNode::ELEMENT_NODE != nodeType ||
        matchNamespaceURI(context) == false)
    {
        return eMatchScoreNone;
    }
    else
    {
        return eMatchScoreNSWild;
    }
}



XPath::eMatchScore
XPath::NodeTester::testElementTotallyWild(
            const XalanNode&        /* context */,
            XalanNode::NodeType     nodeType) const
{
    assert(m_targetNamespace == 0 && m_targetLocalName == 0);

    if (XalanNode::ELEMENT_NODE != nodeType)
    {
        return eMatchScoreNone;
    }
    else
    {
        return eMatchScoreNodeTest;
    }
}



XPath::eMatchScore
XPath::NodeTester::testElementNCName2(const XalanElement&   context) const
{
    assert(
        m_targetNamespace == 0 &&
        m_targetLocalName != 0);

    if (matchLocalName(context) == false)
    {
        return eMatchScoreNone;
    }
    else
    {
        return eMatchScoreQName;
    }
}



XPath::eMatchScore
XPath::NodeTester::testElementQName2(const XalanElement&    context) const
{
    assert(
        m_targetNamespace != 0 &&
        m_targetLocalName != 0);

    if (matchLocalNameAndNamespaceURI(context) == false)
    {
        return eMatchScoreNone;
    }
    else
    {
        return eMatchScoreQName;
    }
}



XPath::eMatchScore
XPath::NodeTester::testElementNamespaceOnly2(const XalanElement&    context) const
{
    assert(
        m_targetNamespace != 0 &&
        m_targetLocalName == 0);

    if (matchNamespaceURI(context) == false)
    {
        return eMatchScoreNone;
    }
    else
    {
        return eMatchScoreNSWild;
    }
}



XPath::eMatchScore
XPath::NodeTester::testElementTotallyWild2(const XalanElement&  /* context */) const
{
    assert(
        m_targetNamespace == 0 &&
        m_targetLocalName == 0);

    return eMatchScoreNodeTest;
}



XPath::eMatchScore
XPath::NodeTester::testNamespaceNCName(
            const XalanNode&        context,
            XalanNode::NodeType     nodeType) const
{
    assert(m_targetNamespace == 0 && m_targetLocalName != 0);

    if (XalanNode::ATTRIBUTE_NODE != nodeType ||
        isNamespaceDeclaration(context) == false ||
        matchNamespace(context) == false)
    {
        return eMatchScoreNone;
    }
    else
    {
        return eMatchScoreQName;
    }
}



XPath::eMatchScore
XPath::NodeTester::testNamespaceTotallyWild(
            const XalanNode&        context,
            XalanNode::NodeType     nodeType) const
{
    assert(m_targetNamespace == 0 && m_targetLocalName == 0);

    if (XalanNode::ATTRIBUTE_NODE != nodeType ||
        isNamespaceDeclaration(context) == false)
    {
        return eMatchScoreNone;
    }
    else
    {
        return eMatchScoreNodeTest;
    }
}



XPath::eMatchScore
XPath::NodeTester::testDefault(
            const XalanNode&        /* context */,
            XalanNode::NodeType     /* nodeType */) const
{
    return eMatchScoreNone;
}



XPath::eMatchScore
XPath::NodeTester::testDefault2(const XalanElement&     /* context */) const
{
    return eMatchScoreNone;
}



bool
XPath::NodeTester::matchLocalName(const XalanNode&  context) const
{
    assert(m_targetLocalName != 0);

    return context.getNamespaceURI().empty() == true &&
           DOMServices::getLocalNameOfNode(context) == *m_targetLocalName;
}



bool
XPath::NodeTester::matchNamespaceURI(const XalanNode&   context) const
{
    assert(m_targetNamespace != 0);

    return context.getNamespaceURI() == *m_targetNamespace;
}


bool
XPath::NodeTester::matchNamespace(const XalanNode&  context) const
{
    assert(m_targetLocalName != 0);

    return context.getLocalName() == *m_targetLocalName;
}



bool
XPath::NodeTester::shouldStripSourceNode(const XalanText&   context) const
{
    assert(m_executionContext != 0);

    return m_executionContext->shouldStripSourceNode(context);
}



const XalanDOMChar  XPath::PSEUDONAME_ANY[] =
{
    XalanUnicode::charAsterisk,
    0
};

const XalanDOMChar  XPath::PSEUDONAME_ROOT[] =
{
    XalanUnicode::charSolidus,
    0
};

const XalanDOMChar  XPath::PSEUDONAME_TEXT[] =
{
    XalanUnicode::charNumberSign,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_x,
    XalanUnicode::charLetter_t,
    0
};

const XalanDOMChar  XPath::PSEUDONAME_COMMENT[] =
{
    XalanUnicode::charNumberSign,
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_m,
    XalanUnicode::charLetter_m,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_t,
    0
};

const XalanDOMChar  XPath::PSEUDONAME_PI[] =
{
    XalanUnicode::charNumberSign,
    XalanUnicode::charLetter_p,
    XalanUnicode::charLetter_i,
    0
};

const XalanDOMChar  XPath::PSEUDONAME_OTHER[] =
{
    XalanUnicode::charAsterisk,
    0
};

const XalanDOMChar  XPath::PSEUDONAME_NODE[] =
{
    XalanUnicode::charNumberSign,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_d,
    XalanUnicode::charLetter_e,
    0
};



// Don't auto-create the table...

XPath::FunctionTableType        XPath::s_functions(false);



void
XPath::initialize(MemoryManager& theManager)
{
    s_functions.setMemoryManager(theManager);

    s_functions.CreateTable();
}



void
XPath::terminate()
{
    s_functions.DestroyTable();
}



XALAN_CPP_NAMESPACE_END
