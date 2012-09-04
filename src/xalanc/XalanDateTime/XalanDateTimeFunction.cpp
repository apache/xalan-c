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

#include "XalanDateTimeFunction.hpp"

#include <xalanc/Include/XalanVector.hpp>
#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/Include/XalanVersion.hpp>
#include <xalanc/XalanDOM/XalanDOMString.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>

XALAN_CPP_NAMESPACE_BEGIN

XalanDateTimeFunction::XalanDateTimeFunction()
    : Function()
{
}

XalanDateTimeFunction::~XalanDateTimeFunction()
{
}

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
    Function*
#else
    XalanDateTimeFunction*
#endif
XalanDateTimeFunction::clone(MemoryManagerType& theManager) const
{
    return XalanCopyConstruct(theManager, *this);
}

XObjectPtr XalanDateTimeFunction::execute(
            XPathExecutionContext &         executionContext,
            XalanNode *                     context,
            const XObjectArgVectorType &    args,
            const LocatorType *             locator) const
{
    const XObjectArgVectorType::size_type	theArgCount = args.size();

    if (theArgCount == 0)
    {
        return execute(executionContext, context, locator);
    }
    else if (theArgCount == 1)
    {
        return execute(executionContext, context, args[0], locator);
    }
    else if (theArgCount == 2)
    {
        return execute(executionContext, context, args[0], args[1], locator);
    }

    throwDefaultError(context, locator, executionContext);

    return XObjectPtr(0);
}

/*
 * Execute with No Arguments
 */

XObjectPtr
XalanDateTimeFunction::execute(
    XPathExecutionContext &		executionContext,
    XalanNode *					context,
    const LocatorType *			locator) const
{
    throwDefaultError(context, locator, executionContext);

    return XObjectPtr(0);
}

/*
 * Execute with One Argument
 */

XObjectPtr
XalanDateTimeFunction::execute(
    XPathExecutionContext &		executionContext,
    XalanNode *					context,
    const XObjectPtr			arg1,
    const LocatorType *			locator) const
{
    throwDefaultError(context, locator, executionContext);

    return XObjectPtr(0);
}


/*
 * Execute with Two Arguments
 */

XObjectPtr
XalanDateTimeFunction::execute(
    XPathExecutionContext &		executionContext,
    XalanNode *					context,
    const XObjectPtr			arg1,
    const XObjectPtr			arg2,
    const LocatorType *			locator) const
{
    throwDefaultError(context, locator, executionContext);

    return XObjectPtr(0);
}

void XalanDateTimeFunction::throwDefaultError(
        XalanNode *                 context,
        const LocatorType *         locator,
        XPathExecutionContext &     executionContext) const
{
    XalanDOMString theBuffer(executionContext.getMemoryManager());

    #if (_XALAN_VERSION == 11000)
        executionContext.error(getError(theBuffer), context, locator);
    #elif (_XALAN_VERSION >= 11100)
        executionContext.problem(
            XPathExecutionContext::eXPath,
            XPathExecutionContext::eError,
            getError(theBuffer),
            locator, context);
    #elif (_XALAN_VERSION < 11000)
        #error "XALAN Library Version must be 1_10 or newer"
    #endif
}

const XalanDOMString&
XalanDateTimeFunction::getError(XalanDOMString& theResult) const
{
    return XalanMessageLoader::getMessage(
        theResult,
        XalanMessages::FunctionTakesTwoOrThreeArguments_1Param,
        "TheFunctionName()");
}

XALAN_CPP_NAMESPACE_END