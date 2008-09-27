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
#include "XNodeSetResultTreeFragProxy.hpp"



#include <xalanc/XalanDOM/XalanDOMException.hpp>



#include "NodeRefListBase.hpp"
#include "XNodeSetBase.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XNodeSetResultTreeFragProxy::XNodeSetResultTreeFragProxy(const XNodeSetBase&    theValue) :
    XObjectResultTreeFragProxyBase(),
    m_value(theValue)
{
}



XNodeSetResultTreeFragProxy::~XNodeSetResultTreeFragProxy()
{
}



XalanNode*
XNodeSetResultTreeFragProxy::getFirstChild() const
{
    return m_value.getLength() == 0 ? 0 : m_value.item(0);
}



XalanNode*
XNodeSetResultTreeFragProxy::getLastChild() const
{
    const XNodeSetBase::size_type   theLength = m_value.getLength();
    
    return theLength == 0 ? 0 : m_value.item(theLength - 1);
}



XALAN_CPP_NAMESPACE_END
