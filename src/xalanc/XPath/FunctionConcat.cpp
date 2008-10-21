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
#include "FunctionConcat.hpp"



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include "XObjectFactory.hpp"



XALAN_CPP_NAMESPACE_BEGIN



FunctionConcat::FunctionConcat()
{
}



FunctionConcat::~FunctionConcat()
{
}



XObjectPtr
FunctionConcat::execute(
            XPathExecutionContext&  executionContext,
            XalanNode*              /* context */,          
            const XObjectPtr        arg1,
            const XObjectPtr        arg2,
            const Locator* const    /* locator */) const
{
    assert(arg1.null() == false && arg2.null() == false);   

    GetCachedString     theResult(executionContext);

    XalanDOMString&     theString = theResult.get();

    arg1->str(executionContext, theString);
    arg2->str(executionContext, theString);

    return executionContext.getXObjectFactory().createString(theResult);
}



XObjectPtr
FunctionConcat::execute(
            XPathExecutionContext&  executionContext,
            XalanNode*              /* context */,          
            const XObjectPtr        arg1,
            const XObjectPtr        arg2,
            const XObjectPtr        arg3,
            const Locator* const    /* locator */) const
{
    assert(arg1.null() == false && arg2.null() == false && arg3.null() == false);   

    GetCachedString     theResult(executionContext);

    XalanDOMString&     theString = theResult.get();

    arg1->str(executionContext, theString);
    arg2->str(executionContext, theString);
    arg3->str(executionContext, theString);

    return executionContext.getXObjectFactory().createString(theResult);
}



XObjectPtr
FunctionConcat::execute(
            XPathExecutionContext&          executionContext,
            XalanNode*                      /* context */,
            const XObjectArgVectorType&     args,
            const Locator*                  /* locator */) const
{
    const XObjectArgVectorType::const_iterator  theEnd = args.end();

    GetCachedString     theResult(executionContext);

    XalanDOMString&     theString = theResult.get();

    XObjectArgVectorType::const_iterator    i = args.begin();

    for(; i != theEnd; ++i)
    {
        (*i)->str(executionContext, theString);
    }

    return executionContext.getXObjectFactory().createString(theResult);
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
FunctionConcat*
#endif
FunctionConcat::clone(MemoryManager&    theManager) const
{
    return XalanCopyConstruct(theManager, *this);
}



const XalanDOMString&
FunctionConcat::getError(XalanDOMString&    theResult) const
{
    return XalanMessageLoader::getMessage(
                theResult,
                XalanMessages::FunctionTakes2ArgsAtLeast_1Param,
                "concat()");
}



XALAN_CPP_NAMESPACE_END
