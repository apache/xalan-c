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
#include "FunctionLang.hpp"



#include "xalanc/Include/XalanMemoryManagement.hpp"



#include "xalanc/XalanDOM/XalanNamedNodeMap.hpp"



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "XObjectFactory.hpp"



XALAN_CPP_NAMESPACE_BEGIN



static const XalanDOMString     s_emptyString(XalanMemMgrs::getDummyMemMgr());



FunctionLang::FunctionLang()
{
}



FunctionLang::~FunctionLang()
{
}



XObjectPtr
FunctionLang::execute(
            XPathExecutionContext&  executionContext,
            XalanNode*              context,
            const XObjectPtr        arg1,
            const Locator* const    /* locator */) const
{
    assert(arg1.null() == false);   

    const XalanNode*        parent = context;

    bool                    fMatch = false;

    const XalanDOMString&   lang = arg1->str(executionContext);

    while(0 != parent)
    {
        if (XalanNode::ELEMENT_NODE == parent->getNodeType())
        {
            const XalanElement* const   theElementNode =
                static_cast<const XalanElement*>(parent);

            const XalanNamedNodeMap* const  theAttributes =
                theElementNode->getAttributes();
            assert(theAttributes != 0);

            const XalanNode* const  theAttribute =
                theAttributes->getNamedItemNS(
                    DOMServices::s_XMLNamespaceURI,
                    s_attributeName);

            const XalanDOMString&   langVal = theAttribute == 0 ?
                        s_emptyString : theAttribute->getNodeValue();

            if (langVal.empty() == false)
            {
                const GetCachedString   theGuard1(executionContext);
                const GetCachedString   theGuard2(executionContext);

                if (startsWith(toLowerCaseASCII(langVal, theGuard1.get()), toLowerCaseASCII(lang, theGuard2.get())))
                {
                    const XalanDOMString::size_type     valLen = lang.length();

                    if (langVal.length() == valLen ||
                        langVal[valLen] == XalanUnicode::charHyphenMinus)
                    {
                        fMatch = true;

                        break;
                    }
                }
            }
        }

        parent = DOMServices::getParentOfNode(*parent);
    }

    return executionContext.getXObjectFactory().createBoolean(fMatch);
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
FunctionLang*
#endif
FunctionLang::clone(MemoryManager&  theManager) const
{
    return XalanCopyConstruct(theManager, *this);
}



const XalanDOMString&
FunctionLang::getError(XalanDOMString&  theResult) const
{
    return XalanMessageLoader::getMessage(
                theResult,
                XalanMessages::FunctionAcceptsOneArgument_1Param,
                "lang()");
}



static XalanDOMChar     s_langString[] =
{
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_g,
    0
};



static XalanDOMString   s_localString(XalanMemMgrs::getDummyMemMgr());



const XalanDOMString&   FunctionLang::s_attributeName = s_localString;



void
FunctionLang::initialize(MemoryManager&     theManager)
{
    s_localString.reset(theManager, s_langString);
}



void
FunctionLang::terminate()
{
    releaseMemory(s_localString, XalanMemMgrs::getDummyMemMgr());
}



XALAN_CPP_NAMESPACE_END
