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


#if defined(XALAN_BUILD_DEPRECATED_DOM_BRIDGE)


#include "XercesToXalanNodeMap.hpp"



#include <algorithm>
#include <cassert>
#include <functional>



#if XERCES_VERSION_MAJOR >= 2
#include <xercesc/dom/deprecated/DOM_Document.hpp>
#else
#include <xercesc/dom/DOM_Document.hpp>
#endif



#include <xalanc/XalanDOM/XalanDOMException.hpp>
#include <xalanc/XalanDOM/XalanNode.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XercesToXalanNodeMap::XercesToXalanNodeMap() :
    m_xalanMap(),
    m_xercesMap()
{
}



XercesToXalanNodeMap::~XercesToXalanNodeMap()
{
}



void
XercesToXalanNodeMap::addAssociation(
            const DOM_NodeType&     theXercesNode,
            XalanNode*              theXalanNode)
{
    NodeImplType* const     theImpl = XercesDOM_NodeHack::getImpl(theXercesNode);

    m_xercesMap.insert(XercesNodeMapType::value_type(theImpl, theXalanNode));
}



void
XercesToXalanNodeMap::clear()
{
    m_xalanMap.clear();
    m_xercesMap.clear();
}



XercesToXalanNodeMap::NodeImplType*
XercesToXalanNodeMap::getNodeImpl(const XalanNode*  theXalanNode) const
{
    XALAN_USING_STD(find_if)

    const XercesNodeMapType::const_iterator     i =
        find_if(
            m_xercesMap.begin(),
            m_xercesMap.end(),
            NameMapEqualsFunctor(theXalanNode));

    if (i != m_xercesMap.end())
    {
        return (*i).first;
    }
    else
    {
        return 0;
    }
}



XALAN_CPP_NAMESPACE_END


#endif //XALAN_BUILD_DEPRECATED_DOM_BRIDGE 


