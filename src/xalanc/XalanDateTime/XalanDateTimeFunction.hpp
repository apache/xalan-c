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

/**
 * This header file defines the C++ class that implement the default behavior of
 * a EXSLT Dates and Times library function.
 *
 * These functions inherits the behavior from the Function class located at 
 * 'xalanc/XPath/'. The execute(..., XObjectArgVectorType & args, ...) method
 * is overriden, calling any other execute(...) method, according the number of
 * available arguments into the 'args' parameter received.
 * 
 * There are Dates and Times EXSLT functions that receive one, two or no
 * arguments. If any other function with more than two arguments have to be
 * added, then the method receiving the XObjectArgVectorType argument have to be
 * overriden.
 */

#ifndef _XALAN_DATETIME_FUNCTION_H
#define _XALAN_DATETIME_FUNCTION_H

#include "XalanDateTimeDefinitons.hpp"

#include <xalanc/XPath/Function.hpp>

#include <xalanc/XPath/XObjectFactory.hpp>

XALAN_CPP_NAMESPACE_BEGIN

/**
 * This is the base class for Xalan Dates and Times EXSLT functions
 *
 * This base function inherits the behavior from the Function class located at 
 * 'xalanc/XPath/'. The execute(..., XObjectArgVectorType & args, ...) method
 * is overriden, calling any other execute(...) method of the subclass,
 * according the number of available arguments into the 'args' parameter
 * received. Each subclass have to override the correct execute(...) method,
 * according the number of arguments it expects to receive.
 */
class XalanDateTimeFunction
    : public Function
{

public:

    XalanDateTimeFunction();

    virtual
    ~XalanDateTimeFunction();

    #if !defined(XALAN_NO_USING_DECLARATION)
        using Function::execute;
    #endif

    virtual XObjectPtr
    execute(
        XPathExecutionContext &         executionContext,
        XalanNode *                     context,
        const XObjectArgVectorType &    args,
        const LocatorType *             locator) const;

    virtual XObjectPtr
    execute(
        XPathExecutionContext &     executionContext,
        XalanNode *                 context,
        const LocatorType *         locator) const;

    virtual XObjectPtr
    execute(
        XPathExecutionContext &     executionContext,
        XalanNode *                 context,
        const XObjectPtr            arg,
        const LocatorType *         locator) const;

    virtual XObjectPtr
    execute(
        XPathExecutionContext &     executionContext,
        XalanNode *                 context,
        const XObjectPtr            arg1,
        const XObjectPtr            arg2,
        const LocatorType *         locator) const;

    #if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
        virtual Function*
    #else
        virtual XalanDateTimeFunction*
    #endif
        clone(MemoryManagerType &   theManager) const;

protected:

    /**
     * This method propagate a XPath exception with a default message 
     *
     * The default message propagated alerts that the function was called
     * with a wrong number of parameters.
     *
     * @param context The XPath context.
     *
     * @param locator The locator type.
     *
     * @param executionContext The XPath execution context.
     *
     * @exception An exception with the default message is always thrown into
     *     the XPath execution context.
     */
    void throwDefaultError(
        XalanNode *                 context,
        const LocatorType *         locator,
        XPathExecutionContext &     executionContext) const;

    virtual const XalanDOMString & getError(
        XalanDOMString &            theResult) const;

};

XALAN_CPP_NAMESPACE_END

#endif