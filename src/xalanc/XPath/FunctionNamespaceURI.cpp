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
#include "FunctionNamespaceURI.hpp"



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "XObjectFactory.hpp"



XALAN_CPP_NAMESPACE_BEGIN



FunctionNamespaceURI::FunctionNamespaceURI()
{
}



FunctionNamespaceURI::~FunctionNamespaceURI()
{
}



static const XalanDOMString     theEmptyString(XalanMemMgrs::getDummyMemMgr());



XObjectPtr
FunctionNamespaceURI::execute(
            XPathExecutionContext&  executionContext,
            XalanNode*              context,
            const Locator*          locator) const
{
    if (context == 0)
    {
        const GetCachedString   theGuard(executionContext);

        XalanDOMString& theResult = theGuard.get();

        executionContext.problem(
            XPathExecutionContext::eXPath,
            XPathExecutionContext::eError,
            XalanMessageLoader::getMessage(
                theResult,
                XalanMessages::FunctionRequiresNonNullContextNode_1Param,
                "namespace-uri()"),
            locator,
            context);

        // Dummy return value...
        return XObjectPtr(0);
    }
    else
    {
        return executionContext.getXObjectFactory().createStringReference(DOMServices::getNamespaceOfNode(*context));
    }
}



XObjectPtr
FunctionNamespaceURI::execute(
            XPathExecutionContext&  executionContext,
            XalanNode*              /* context */,
            const XObjectPtr        arg1,
            const Locator*          locator) const
{
    assert(arg1.null() == false);   
    
    const NodeRefListBase&  theList = arg1->nodeset();

    if (theList.getLength() == 0)
    {
        return executionContext.getXObjectFactory().createStringReference(theEmptyString);
    }
    else
    {
        assert(theList.item(0) != 0);

        return execute(executionContext, theList.item(0), locator);
    }
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
FunctionNamespaceURI*
#endif
FunctionNamespaceURI::clone(MemoryManager&  theManager) const
{
    return XalanCopyConstruct(theManager, *this);
}



const XalanDOMString&
FunctionNamespaceURI::getError(XalanDOMString& theResult) const
{
    return XalanMessageLoader::getMessage(
                theResult,
                XalanMessages::FunctionTakesZeroOrOneArg_1Param,
                "namespace-uri()");
}



XALAN_CPP_NAMESPACE_END
