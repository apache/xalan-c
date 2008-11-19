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
#include "XercesEntityReferenceWrapper.hpp"



#include <xercesc/dom/DOMEntityReference.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "XercesWrapperHelper.hpp"
#include "XercesWrapperNavigator.hpp"
#include "XercesDocumentWrapper.hpp"
#include "XercesDOMWrapperException.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XercesEntityReferenceWrapper::XercesEntityReferenceWrapper(
            const DOMEntityReferenceType*   theXercesDOMEntityReference,
            const XercesWrapperNavigator&   theNavigator) :
    XalanEntityReference(),
    m_xercesNode(theXercesDOMEntityReference),
    m_navigator(theNavigator),
    m_children(theXercesDOMEntityReference->getChildNodes(),
               theNavigator)
{
    assert(theXercesDOMEntityReference != 0);
}



XercesEntityReferenceWrapper*
XercesEntityReferenceWrapper::create(
            MemoryManager&                  theManager,
            const DOMEntityReferenceType*   theXercesDOMEntityReference,
            const XercesWrapperNavigator&   theNavigator)

{
    typedef XercesEntityReferenceWrapper    ThisType;

    XalanAllocationGuard    theGuard(theManager, theManager.allocate(sizeof(ThisType)));

    ThisType* const     theResult =
        new (theGuard.get()) ThisType(
                                theXercesDOMEntityReference,
                                theNavigator);

    theGuard.release();

    return theResult;
}



XercesEntityReferenceWrapper::~XercesEntityReferenceWrapper()
{
}



const XalanDOMString&
XercesEntityReferenceWrapper::getNodeName() const
{
    return m_navigator.getPooledString(m_xercesNode->getNodeName());
}



const XalanDOMString&
XercesEntityReferenceWrapper::getNodeValue() const
{
    return m_navigator.getPooledString(m_xercesNode->getNodeValue());
}



XercesEntityReferenceWrapper::NodeType
XercesEntityReferenceWrapper::getNodeType() const
{
    return ENTITY_REFERENCE_NODE;
}



XalanNode*
XercesEntityReferenceWrapper::getParentNode() const
{
    return m_navigator.getParentNode(m_xercesNode);
}



const XalanNodeList*
XercesEntityReferenceWrapper::getChildNodes() const
{
    return &m_children;
}



XalanNode*
XercesEntityReferenceWrapper::getFirstChild() const
{
    return m_navigator.getFirstChild(m_xercesNode);
}



XalanNode*
XercesEntityReferenceWrapper::getLastChild() const
{
    return m_navigator.getLastChild(m_xercesNode);
}



XalanNode*
XercesEntityReferenceWrapper::getPreviousSibling() const
{
    return m_navigator.getPreviousSibling(m_xercesNode);
}



XalanNode*
XercesEntityReferenceWrapper::getNextSibling() const
{
    return m_navigator.getNextSibling(m_xercesNode);
}



const XalanNamedNodeMap*
XercesEntityReferenceWrapper::getAttributes() const
{
    return 0;
}



XalanDocument*
XercesEntityReferenceWrapper::getOwnerDocument() const
{
    return m_navigator.getOwnerDocument();
}



const XalanDOMString&
XercesEntityReferenceWrapper::getNamespaceURI() const
{
    return m_navigator.getPooledString(m_xercesNode->getNamespaceURI());
}



const XalanDOMString&
XercesEntityReferenceWrapper::getPrefix() const
{
    return m_navigator.getPooledString(m_xercesNode->getPrefix());
}



const XalanDOMString&
XercesEntityReferenceWrapper::getLocalName() const
{
    return m_navigator.getPooledString(m_xercesNode->getLocalName());
}



bool
XercesEntityReferenceWrapper::isIndexed() const
{
    return m_navigator.getOwnerDocument()->isIndexed();
}



XercesEntityReferenceWrapper::IndexType
XercesEntityReferenceWrapper::getIndex() const
{
    return m_navigator.getIndex();
}



XALAN_CPP_NAMESPACE_END
