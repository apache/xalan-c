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

#include "ExtensionFunctionHandler.hpp"



#include <cassert>



#include <xalanc/PlatformSupport/StringTokenizer.hpp>
#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



XALAN_CPP_NAMESPACE_BEGIN



const XalanDOMChar  ExtensionFunctionHandler::s_tokenDelimiterCharacters[] =
{
        XalanUnicode::charSpace,
        XalanUnicode::charHTab,
        XalanUnicode::charLF,
        XalanUnicode::charCR,
        0
};



ExtensionFunctionHandler::ExtensionFunctionHandler(const XalanDOMString&    namespaceUri,
                                                   MemoryManager& theManager) :
    m_namespaceUri(namespaceUri,theManager),
    m_scriptLang(theManager),
    m_scriptSrc(theManager),
    m_scriptSrcURL(theManager),
    m_javaObject(0),
    m_functions(theManager),
    m_componentStarted(false)
{
}



ExtensionFunctionHandler::~ExtensionFunctionHandler()
{
}



ExtensionFunctionHandler::ExtensionFunctionHandler (
            MemoryManager&      theManager,
            const XalanDOMString&   namespaceUri,
            const XalanDOMString&   funcNames,
            const XalanDOMString&   lang,
            const XalanDOMString&   srcURL,
            const XalanDOMString&   src) :
    m_namespaceUri(namespaceUri, theManager),
    m_scriptLang(lang, theManager),
    m_scriptSrc(src, theManager),
    m_scriptSrcURL(srcURL, theManager),
    m_javaObject(0),
    m_functions(theManager),
    m_componentStarted(false)
{
    setFunctions (funcNames);
}



void
ExtensionFunctionHandler::setFunctions(const XalanDOMString&    funcNames) 
{
    if (funcNames.empty()) 
    {
        return;
    }

    StringTokenizer     st(funcNames, s_tokenDelimiterCharacters, false);

    XalanDOMString theBuffer(getMemoryManager());

    while (st.hasMoreTokens() == true)
    {
        st.nextToken(theBuffer);

        m_functions.insert(theBuffer);
    }
}


void
ExtensionFunctionHandler::setScript(
            const XalanDOMString&   lang,
            const XalanDOMString&   srcURL,
            const XalanDOMString&   scriptSrc)
{
    m_scriptLang = lang;
    m_scriptSrcURL = srcURL;
    m_scriptSrc = scriptSrc;
}



bool
ExtensionFunctionHandler::isFunctionAvailable (const XalanDOMString&    function) const
{
    return m_functions.find(function) != m_functions.end();
}



XObjectPtr
ExtensionFunctionHandler::callFunction(
            const XalanDOMString&   /* funcName */,
            const ArgVectorType&    /* args */)
{
    assert(0);  // @@ TODO: Not implemented

    return XObjectPtr();
}



void
ExtensionFunctionHandler::startupComponent()
{
}



XALAN_CPP_NAMESPACE_END
