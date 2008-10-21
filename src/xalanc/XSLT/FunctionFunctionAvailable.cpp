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
#include "FunctionFunctionAvailable.hpp"



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/XPath/XObjectFactory.hpp>



XALAN_CPP_NAMESPACE_BEGIN



FunctionFunctionAvailable::FunctionFunctionAvailable()
{
}



FunctionFunctionAvailable::~FunctionFunctionAvailable()
{
}



XObjectPtr
FunctionFunctionAvailable::execute(
            XPathExecutionContext&  executionContext,
            XalanNode*              /* context */,
            const XObjectPtr        arg,
            const Locator*          locator) const
{
    assert(arg.null() == false);

    return executionContext.getXObjectFactory().createBoolean(
                executionContext.functionAvailable(
                    arg->str(executionContext),
                    locator));
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
FunctionFunctionAvailable*
#endif
FunctionFunctionAvailable::clone(MemoryManager&     theManager) const
{
    return XalanCopyConstruct(theManager, *this);
}



const XalanDOMString&
FunctionFunctionAvailable::getError(XalanDOMString&     theResult) const
{
    return XalanMessageLoader::getMessage(
                theResult,
                XalanMessages::FunctionRequiresNonNullContextNode_1Param,
                "function-available()");
}



XALAN_CPP_NAMESPACE_END
