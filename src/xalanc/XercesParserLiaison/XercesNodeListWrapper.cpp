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
#include "XercesNodeListWrapper.hpp"



#include <cassert>



#include <xercesc/dom/DOMNodeList.hpp>



#include "XercesWrapperNavigator.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XercesNodeListWrapper::XercesNodeListWrapper(
            const DOMNodeListType*          theXercesNodeList,
            const XercesWrapperNavigator&   theNavigator) :
    XalanNodeList(),
    m_xercesNodeList(theXercesNodeList),
    m_navigator(theNavigator)
{
    assert(theXercesNodeList != 0);
}



XercesNodeListWrapper::~XercesNodeListWrapper()
{
}



XercesNodeListWrapper::XercesNodeListWrapper(const XercesNodeListWrapper&   theSource) :
    XalanNodeList(theSource),
    m_xercesNodeList(theSource.m_xercesNodeList),
    m_navigator(theSource.m_navigator)
{
}



XalanNode*
XercesNodeListWrapper::item(XalanSize_t     index) const
{
    assert(m_xercesNodeList != 0);

    return m_navigator.mapNode(m_xercesNodeList->item(index));
}



XalanSize_t
XercesNodeListWrapper::getLength() const
{
    assert(m_xercesNodeList != 0);

    return m_xercesNodeList->getLength();
}



XALAN_CPP_NAMESPACE_END
