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
#include "XercesDocumentTypeWrapper.hpp"



#include <xercesc/dom/DOMDocumentType.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "XercesWrapperHelper.hpp"
#include "XercesWrapperNavigator.hpp"
#include "XercesDOMWrapperException.hpp"
#include "XercesDocumentWrapper.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XercesDocumentTypeWrapper::XercesDocumentTypeWrapper(
            const DOMDocumentType_Type*     theXercesDOMDocumentType,
            const XercesWrapperNavigator&   theNavigator) :
    XalanDocumentType(),
    m_xercesNode(theXercesDOMDocumentType),
    m_entities(m_xercesNode->getEntities(),
               theNavigator),
    m_notations(m_xercesNode->getNotations(),
                theNavigator),
    m_navigator(theNavigator)
{
    assert(theXercesDOMDocumentType != 0);
}



XercesDocumentTypeWrapper*
XercesDocumentTypeWrapper::create(
            MemoryManager&                  theManager,
            const DOMDocumentType_Type*     theXercesDOMDocumentType,
            const XercesWrapperNavigator&   theNavigator)
{
    typedef XercesDocumentTypeWrapper   ThisType;

    XalanAllocationGuard    theGuard(theManager, theManager.allocate(sizeof(ThisType)));

    ThisType* const     theResult =
        new (theGuard.get()) ThisType(
                                theXercesDOMDocumentType,
                                theNavigator);

    theGuard.release();

    return theResult;
}



XercesDocumentTypeWrapper::~XercesDocumentTypeWrapper()
{
}



const XalanDOMString&
XercesDocumentTypeWrapper::getNodeName() const
{
    return m_navigator.getPooledString(m_xercesNode->getNodeName());
}



const XalanDOMString&
XercesDocumentTypeWrapper::getNodeValue() const
{
    return m_navigator.getPooledString(m_xercesNode->getNodeValue());
}



XercesDocumentTypeWrapper::NodeType
XercesDocumentTypeWrapper::getNodeType() const
{
    return DOCUMENT_TYPE_NODE;
}



XalanNode*
XercesDocumentTypeWrapper::getParentNode() const
{
    return m_navigator.getParentNode(m_xercesNode);
}



const XalanNodeList*
XercesDocumentTypeWrapper::getChildNodes() const
{
    return 0;
}



XalanNode*
XercesDocumentTypeWrapper::getFirstChild() const
{
    return 0;
}



XalanNode*
XercesDocumentTypeWrapper::getLastChild() const
{
    return 0;
}



XalanNode*
XercesDocumentTypeWrapper::getPreviousSibling() const
{
    return m_navigator.getPreviousSibling(m_xercesNode);
}



XalanNode*
XercesDocumentTypeWrapper::getNextSibling() const
{
    return m_navigator.getNextSibling(m_xercesNode);
}



const XalanNamedNodeMap*
XercesDocumentTypeWrapper::getAttributes() const
{
    return 0;
}



XalanDocument*
XercesDocumentTypeWrapper::getOwnerDocument() const
{
    return m_navigator.getOwnerDocument();
}



const XalanDOMString&
XercesDocumentTypeWrapper::getNamespaceURI() const
{
    return m_navigator.getPooledString(m_xercesNode->getNamespaceURI());
}



const XalanDOMString&
XercesDocumentTypeWrapper::getPrefix() const
{
    return m_navigator.getPooledString(m_xercesNode->getPrefix());
}



const XalanDOMString&
XercesDocumentTypeWrapper::getLocalName() const
{
    return m_navigator.getPooledString(m_xercesNode->getLocalName());
}



bool
XercesDocumentTypeWrapper::isIndexed() const
{
    return m_navigator.getOwnerDocument()->isIndexed();
}



XercesDocumentTypeWrapper::IndexType
XercesDocumentTypeWrapper::getIndex() const
{
    return m_navigator.getIndex();
}



const XalanDOMString&
XercesDocumentTypeWrapper::getName() const
{
    return m_navigator.getPooledString(m_xercesNode->getName());
}



const XalanNamedNodeMap*
XercesDocumentTypeWrapper::getEntities() const
{
    return &m_entities;
}



const XalanNamedNodeMap*
XercesDocumentTypeWrapper::getNotations() const
{
    return &m_notations;
}



const XalanDOMString&
XercesDocumentTypeWrapper::getPublicId() const
{
    return m_navigator.getPooledString(m_xercesNode->getPublicId());
}



const XalanDOMString&
XercesDocumentTypeWrapper::getSystemId() const
{
    return m_navigator.getPooledString(m_xercesNode->getSystemId());
}



const XalanDOMString&
XercesDocumentTypeWrapper::getInternalSubset() const
{
    return m_navigator.getPooledString(m_xercesNode->getInternalSubset());
}



XALAN_CPP_NAMESPACE_END
