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
// Class header file.
#include "XNodeSetNodeProxy.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XNodeSetNodeProxy::Proxy::Proxy(XalanNode*  theNode) :
    m_node(theNode)
{
}



XNodeSetNodeProxy::Proxy::~Proxy()
{
}



XalanNode*
XNodeSetNodeProxy::Proxy::item(size_type    index) const
{
    if (m_node == 0 || index > 0)
    {
        return 0;
    }
    else
    {
        return m_node;
    }
}



XNodeSetNodeProxy::Proxy::size_type
XNodeSetNodeProxy::Proxy::getLength() const
{
    return m_node == 0 ? 0 : 1;
}



XNodeSetNodeProxy::Proxy::size_type
XNodeSetNodeProxy::Proxy::indexOf(const XalanNode*  theNode) const
{
    if (m_node == 0 || theNode != m_node)
    {
        return npos;
    }
    else
    {
        return 0;
    }
}



XNodeSetNodeProxy::XNodeSetNodeProxy(
            MemoryManager&  theMemoryManager,
            XalanNode*      theNode) :
    XNodeSetBase(theMemoryManager),
    m_proxy(theNode)
{
}



XNodeSetNodeProxy::XNodeSetNodeProxy(
            const XNodeSetNodeProxy&    source,
            MemoryManager&              theMemoryManager) :
    XNodeSetBase(source, theMemoryManager),
    m_proxy(source.m_proxy)
{
}



XNodeSetNodeProxy::~XNodeSetNodeProxy()
{
}




XNodeSetNodeProxy::eObjectType
XNodeSetNodeProxy::getRealType() const
{
    return eTypeNodeSetNodeProxy;
}



const NodeRefListBase&
XNodeSetNodeProxy::nodeset() const
{
    return m_proxy;
}



XalanNode*
XNodeSetNodeProxy::item(size_type   index) const
{
    return m_proxy.item(index);
}



XNodeSetNodeProxy::size_type
XNodeSetNodeProxy::getLength() const
{
    return m_proxy.getLength();
}



XALAN_CPP_NAMESPACE_END
