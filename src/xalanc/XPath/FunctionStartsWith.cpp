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
#include "FunctionStartsWith.hpp"



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include "XObjectFactory.hpp"



XALAN_CPP_NAMESPACE_BEGIN



FunctionStartsWith::FunctionStartsWith()
{
}



FunctionStartsWith::~FunctionStartsWith()
{
}



XObjectPtr
FunctionStartsWith::execute(
            XPathExecutionContext&  executionContext,
            XalanNode*              /* context */,
            const XObjectPtr        arg1,
            const XObjectPtr        arg2,
            const Locator* const    /* locator */) const
{
    assert(arg1.null() == false && arg2.null() == false);   

    const bool  fStartsWith =
        startsWith(
            arg1->str(executionContext), 
            arg2->str(executionContext));

    return executionContext.getXObjectFactory().createBoolean(fStartsWith);
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
FunctionStartsWith*
#endif
FunctionStartsWith::clone(MemoryManager& theManager) const
{
    return XalanCopyConstruct(theManager, *this);
}



const XalanDOMString&
FunctionStartsWith::getError(XalanDOMString& theResult) const
{
    return XalanMessageLoader::getMessage(
                theResult,
                XalanMessages::FunctionTakesTwoArguments_1Param,
                "starts-with()");
}



XALAN_CPP_NAMESPACE_END
