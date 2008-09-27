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
#include "XalanSimplePrefixResolver.hpp"



#include <xalanc/XalanDOM/XalanDOMString.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XalanSimplePrefixResolver::XalanSimplePrefixResolver(
            const XalanDOMString&   thePrefix,
            const XalanDOMString&   theNamespaceURI,
            const XalanDOMString&   theURI) :
    m_prefix(thePrefix),
    m_namespaceURI(theNamespaceURI),
    m_uri(theURI)
{
}



XalanSimplePrefixResolver::~XalanSimplePrefixResolver()
{
}



const XalanDOMString*
XalanSimplePrefixResolver::getNamespaceForPrefix(const XalanDOMString&  prefix) const
{
    if (prefix == m_prefix)
    {
        return &m_namespaceURI;
    }
    else
    {
        return 0;
    }
}



const XalanDOMString&
XalanSimplePrefixResolver::getURI() const
{
    return m_uri;
}



XALAN_CPP_NAMESPACE_END
