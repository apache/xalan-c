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
#include "FunctionUnparsedEntityURI.hpp"



#include <xalanc/XalanDOM/XalanNode.hpp>
#include <xalanc/XalanDOM/XalanDocument.hpp>



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/XPath/XObjectFactory.hpp>



XALAN_CPP_NAMESPACE_BEGIN



FunctionUnparsedEntityURI::FunctionUnparsedEntityURI()
{
}



FunctionUnparsedEntityURI::~FunctionUnparsedEntityURI()
{
}


XObjectPtr
FunctionUnparsedEntityURI::execute(
            XPathExecutionContext&  executionContext,
            XalanNode*              context,            
            const XObjectPtr        arg,
            const Locator*          locator) const
{
    assert(arg.null() == false);

    if (context == 0)
    {
        const GetCachedString   theGuard(executionContext);

        executionContext.problem(
            XPathExecutionContext::eXPath,
            XPathExecutionContext::eError,
            XalanMessageLoader::getMessage(
                theGuard.get(),
                XalanMessages::FunctionRequiresNonNullContextNode_1Param,
                "unparsed-entity-uri"),
            locator,
            context);

        return XObjectPtr();
    }
    else
    {
        const XalanDOMString&   name = arg->str(executionContext);

        XalanDocument* const    doc =
                XalanNode::DOCUMENT_NODE == context->getNodeType() ?
                    static_cast<XalanDocument*>(context) :
                    context->getOwnerDocument();
        assert(doc != 0);

        const XalanDOMString&   theURI =
            executionContext.getUnparsedEntityURI(name, *doc);

        return executionContext.getXObjectFactory().createStringReference(theURI);
    }
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
FunctionUnparsedEntityURI*
#endif
FunctionUnparsedEntityURI::clone(MemoryManager&     theManager) const
{
   return XalanCopyConstruct(theManager, *this);
}



const XalanDOMString&
FunctionUnparsedEntityURI::getError(XalanDOMString&     theResult) const
{
    return XalanMessageLoader::getMessage(
                theResult,
                XalanMessages::FunctionAcceptsOneArgument_1Param,
                "unparsed-entity-uri");
}



XALAN_CPP_NAMESPACE_END
