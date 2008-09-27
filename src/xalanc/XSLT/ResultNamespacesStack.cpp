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

// Class header file...
#include "ResultNamespacesStack.hpp"



XALAN_CPP_NAMESPACE_BEGIN



ResultNamespacesStack::ResultNamespacesStack(MemoryManager& theManager) :
    m_resultNamespaces(theManager, 1),
    m_stackBegin(m_resultNamespaces.begin()),
    m_stackPosition(m_stackBegin),
    m_createNewContextStack(theManager)
{
    // m_resultNamespaces is initialized to a size of
    // 1, so we always have a dummy entry at the
    // beginning.  This makes the implementation
    // much simpler.
}



ResultNamespacesStack::~ResultNamespacesStack()
{
}



void
ResultNamespacesStack::addDeclaration(
            const XalanDOMString&       thePrefix,
            const XalanDOMChar*         theNamespaceURI,
            XalanDOMString::size_type   theLength)
{
    assert(theNamespaceURI != 0);
    assert(m_createNewContextStack.empty() == false);

    // Check to see if we need to create a new context and do so if necessary...
    if (m_createNewContextStack.back() == true)
    {
        ++m_stackPosition;

        if (m_stackPosition == m_resultNamespaces.end())
        {
            m_resultNamespaces.resize(m_resultNamespaces.size() + 1);

            m_stackPosition = m_resultNamespaces.end() - 1;
            m_stackBegin = m_resultNamespaces.begin();
        }

        m_createNewContextStack.back() = false;
    }

    NamespaceVectorType&    theCurrentNamespaces = *m_stackPosition;

    // Add a new namespace at the end of the current namespaces.
    theCurrentNamespaces.resize(theCurrentNamespaces.size() + 1);

    NameSpace&  theNewNamespace = theCurrentNamespaces.back();

    theNewNamespace.setPrefix(thePrefix);

    theNewNamespace.setURI(theNamespaceURI, theLength);
}



void
ResultNamespacesStack::pushContext()
{
    if (m_createNewContextStack.empty() == true)
    {
        m_createNewContextStack.reserve(eDefaultCreateNewContextStackSize);
    }

    m_createNewContextStack.push_back(true);
}



void
ResultNamespacesStack::popContext()
{
    assert(m_createNewContextStack.empty() == false);

    if (m_createNewContextStack.back() == false)
    {
        assert(m_resultNamespaces.empty() == false &&
               m_stackPosition != m_resultNamespaces.begin());

        (*m_stackPosition).clear();

        --m_stackPosition;
    }

    m_createNewContextStack.pop_back();
}



const XalanDOMString*
ResultNamespacesStack::getNamespaceForPrefix(const XalanDOMString&  thePrefix) const
{
    if (m_stackPosition == m_stackBegin)
    {
        return 0;
    }
    else
    {
        return XalanQName::getNamespaceForPrefix(
                m_stackBegin,
                m_stackPosition + 1,
                thePrefix);
    }
}



const XalanDOMString*
ResultNamespacesStack::getPrefixForNamespace(const XalanDOMString&  theNamespaceURI) const
{
    if (m_stackPosition == m_stackBegin)
    {
        return 0;
    }
    else
    {
        return XalanQName::getPrefixForNamespace(
                    m_stackBegin,
                    m_stackPosition + 1,
                    theNamespaceURI);
    }
}



bool
ResultNamespacesStack::prefixIsPresentLocal(const XalanDOMString&   thePrefix)
{
    bool    fResult = false;

    // Check to see if we need to create a new context.  If so, there are
    // no prefixes mapped at this level, so return false...
    if (m_createNewContextStack.back() == false)
    {
        const NamespaceVectorType&  theNamespaces =
            *m_stackPosition;

        NamespaceVectorType::const_iterator         i = theNamespaces.begin();
        const NamespaceVectorType::const_iterator   theEnd = theNamespaces.end();

        while(i != theEnd && fResult == false)
        {
            const NameSpace&        ns = (*i);

            const XalanDOMString&   thisPrefix = ns.getPrefix();

            if(thePrefix == thisPrefix)
            {
                fResult = true;
            }
            else
            {
                ++i;
            }
        }
    }

    return fResult;
}



void
ResultNamespacesStack::clear()
{
    // Since we always keep one dummy entry at the beginning,
    // swap with an OutputContextStackType instance of size 1.
    NamespacesStackType temp(XalanMemMgrs::getDummyMemMgr(), 1);
    temp.swap(m_resultNamespaces);

    m_stackBegin = m_resultNamespaces.begin();

    m_stackPosition = m_stackBegin;

    m_createNewContextStack.clear();
}



XALAN_CPP_NAMESPACE_END
