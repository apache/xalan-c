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
#include "XercesEntityWrapper.hpp"



#include <xercesc/dom/DOMEntity.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "XercesDOMWrapperException.hpp"
#include "XercesWrapperHelper.hpp"
#include "XercesWrapperNavigator.hpp"
#include "XercesDocumentWrapper.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XercesEntityWrapper::XercesEntityWrapper(
            const DOMEntityType*            theXercesDOMEntity,
            const XercesWrapperNavigator&   theNavigator) :
    XalanEntity(),
    m_xercesNode(theXercesDOMEntity),
    m_navigator(theNavigator),
    m_children(m_xercesNode->getChildNodes(),
               theNavigator)
{
    assert(theXercesDOMEntity != 0);
}


    
XercesEntityWrapper*
XercesEntityWrapper::create(
            MemoryManager&                  theManager,
            const DOMEntityType*            theXercesDOMEntity,
            const XercesWrapperNavigator&   theNavigator)

{
    typedef XercesEntityWrapper     ThisType;

    XalanAllocationGuard    theGuard(theManager, theManager.allocate(sizeof(ThisType)));

    ThisType* const     theResult =
        new (theGuard.get()) ThisType(
                                theXercesDOMEntity,
                                theNavigator);

    theGuard.release();

    return theResult;
}



XercesEntityWrapper::~XercesEntityWrapper()
{
}



const XalanDOMString&
XercesEntityWrapper::getNodeName() const
{
    return m_navigator.getPooledString(m_xercesNode->getNodeName());
}



const XalanDOMString&
XercesEntityWrapper::getNodeValue() const
{
    return m_navigator.getPooledString(m_xercesNode->getNodeValue());
}



XercesEntityWrapper::NodeType
XercesEntityWrapper::getNodeType() const
{
    return ENTITY_NODE;
}



XalanNode*
XercesEntityWrapper::getParentNode() const
{
    return m_navigator.getParentNode(m_xercesNode);
}



const XalanNodeList*
XercesEntityWrapper::getChildNodes() const
{
    return &m_children;
}



XalanNode*
XercesEntityWrapper::getFirstChild() const
{
    return m_navigator.getFirstChild(m_xercesNode);
}



XalanNode*
XercesEntityWrapper::getLastChild() const
{
    return m_navigator.getLastChild(m_xercesNode);
}



XalanNode*
XercesEntityWrapper::getPreviousSibling() const
{
    return m_navigator.getPreviousSibling(m_xercesNode);
}



XalanNode*
XercesEntityWrapper::getNextSibling() const
{
    return m_navigator.getNextSibling(m_xercesNode);
}



const XalanNamedNodeMap*
XercesEntityWrapper::getAttributes() const
{
    return 0;
}



XalanDocument*
XercesEntityWrapper::getOwnerDocument() const
{
    return m_navigator.getOwnerDocument();
}



const XalanDOMString&
XercesEntityWrapper::getNamespaceURI() const
{
    return m_navigator.getPooledString(m_xercesNode->getNamespaceURI());
}



const XalanDOMString&
XercesEntityWrapper::getPrefix() const
{
    return m_navigator.getPooledString(m_xercesNode->getPrefix());
}



const XalanDOMString&
XercesEntityWrapper::getLocalName() const
{
    return m_navigator.getPooledString(m_xercesNode->getLocalName());
}



bool
XercesEntityWrapper::isIndexed() const
{
    return m_navigator.getOwnerDocument()->isIndexed();
}



XercesEntityWrapper::IndexType
XercesEntityWrapper::getIndex() const
{
    return m_navigator.getIndex();
}



const XalanDOMString&
XercesEntityWrapper::getPublicId() const
{
    return m_navigator.getPooledString(m_xercesNode->getPublicId());
}



const XalanDOMString&
XercesEntityWrapper::getSystemId() const
{
    return m_navigator.getPooledString(m_xercesNode->getSystemId());
}



const XalanDOMString&
XercesEntityWrapper::getNotationName() const
{
    return m_navigator.getPooledString(m_xercesNode->getNotationName());
}



XALAN_CPP_NAMESPACE_END
