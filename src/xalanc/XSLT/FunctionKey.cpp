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
#include "FunctionKey.hpp"



#include <xalanc/XalanDOM/XalanNode.hpp>
#include <xalanc/XalanDOM/XalanDocument.hpp>
#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include <xalanc/XPath/MutableNodeRefList.hpp>
#include <xalanc/XPath/NodeRefListBase.hpp>
#include <xalanc/XPath/XalanQNameByReference.hpp>
#include <xalanc/XPath/XObjectFactory.hpp>



XALAN_CPP_NAMESPACE_BEGIN



FunctionKey::FunctionKey()
{
}



FunctionKey::~FunctionKey()
{
}



inline void
getNodeSet(
            XPathExecutionContext&  executionContext,
            XalanNode*              context,
            const XalanDOMString&   keyname,
            const XalanDOMString&   ref,
            const Locator*          locator,
            MutableNodeRefList&     theNodeRefList)
{
    assert(context != 0);

    if (indexOf(keyname, XalanUnicode::charColon) < keyname.length())
    {
        executionContext.getNodeSetByKey(
                context,
                keyname,
                ref,
                locator,
                theNodeRefList);
    }
    else
    {
        const XalanQNameByReference     theQName(keyname);

        executionContext.getNodeSetByKey(
                context,
                theQName,
                ref,
                locator,
                theNodeRefList);
    }
}



XObjectPtr
FunctionKey::execute(
            XPathExecutionContext&  executionContext,
            XalanNode*              context,
            const XObjectPtr        arg1,
            const XObjectPtr        arg2,
            const Locator*          locator) const
{
    assert(arg1.null() == false && arg2.null() == false);

    if (context == 0)
    {
        const GetCachedString   theGuard(executionContext);

        executionContext.problem(
            XPathExecutionContext::eXPath,
            XPathExecutionContext::eError,
            XalanMessageLoader::getMessage(
                theGuard.get(),
                XalanMessages::FunctionRequiresNonNullContextNode_1Param,
                "key()"),
            locator,
            context);

        return XObjectPtr();
    }
    else
    {
        assert(executionContext.getPrefixResolver() != 0);

        const XalanDOMString&   keyname = arg1->str(executionContext);

        assert(arg2.null() == false);

        // This list will hold the nodes...
        GetCachedNodeList   theNodeRefList(executionContext);

        if (arg2->getType() != XObject::eTypeNodeSet)
        {
            getNodeSet(
                executionContext,
                context,
                keyname,
                arg2->str(executionContext),
                locator,
                *theNodeRefList.get());
        }
        else
        {
            const NodeRefListBase&  theNodeSet = arg2->nodeset();

            const NodeRefListBase::size_type    nRefs = theNodeSet.getLength();

            if (nRefs == 1)
            {
                getNodeSet(
                    executionContext,
                    context,
                    keyname,
                    arg2->str(executionContext),
                    locator,
                    *theNodeRefList.get());
            }
            else if (nRefs > 1)
            {
                const GetCachedString   theResult(executionContext);

                XalanDOMString&     ref = theResult.get();

                for(NodeRefListBase::size_type i = 0; i < nRefs; i++)
                {
                    assert(theNodeSet.item(i) != 0);

                    DOMServices::getNodeData(*theNodeSet.item(i), executionContext, ref);

                    if (0 != ref.length())
                    {
                        getNodeSet(
                            executionContext,
                            context,
                            keyname,
                            ref,
                            locator,
                            *theNodeRefList.get());
                    }

                    ref.clear();
                }
            }
        }

        return executionContext.getXObjectFactory().createNodeSet(theNodeRefList);
    }
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
FunctionKey*
#endif
FunctionKey::clone(MemoryManager&   theManager) const
{
    return XalanCopyConstruct(theManager, *this);
}



const XalanDOMString&
FunctionKey::getError(XalanDOMString&   theResult) const
{
    return XalanMessageLoader::getMessage(
                theResult,
                XalanMessages::FunctionTakesTwoArguments_1Param,
                "key()");
}



XALAN_CPP_NAMESPACE_END
