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
#include "XercesNamedNodeMapWrapper.hpp"



#include <algorithm>
#include <cassert>



#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/dom/DOMNode.hpp>



#include "XercesWrapperNavigator.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XercesNamedNodeMapWrapper::XercesNamedNodeMapWrapper(
            const DOMNamedNodeMapType*      theXercesNamedNodeMap,
            const XercesWrapperNavigator&   theNavigator) :
    m_xercesNamedNodeMap(theXercesNamedNodeMap),
    m_navigator(theNavigator)
{
    assert(m_xercesNamedNodeMap != 0);
}



XercesNamedNodeMapWrapper::~XercesNamedNodeMapWrapper()
{
}



XalanNode*
XercesNamedNodeMapWrapper::item(XalanSize_t     index) const
{
    return m_navigator.mapNode(m_xercesNamedNodeMap->item(index));
}



XalanNode*
XercesNamedNodeMapWrapper::getNamedItem(const XalanDOMString&   name) const
{
    return m_navigator.mapNode(m_xercesNamedNodeMap->getNamedItem(name.c_str()));
}



XalanSize_t
XercesNamedNodeMapWrapper::getLength() const
{
    return m_xercesNamedNodeMap->getLength();
}



XalanNode*
XercesNamedNodeMapWrapper::getNamedItemNS(
            const XalanDOMString&   namespaceURI,
            const XalanDOMString&   localName) const
{
    return m_navigator.mapNode(
            m_xercesNamedNodeMap->getNamedItemNS(
                namespaceURI.c_str(),
                localName.c_str()));
}



XALAN_CPP_NAMESPACE_END
