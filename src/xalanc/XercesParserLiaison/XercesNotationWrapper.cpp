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
#include "XercesNotationWrapper.hpp"



#include <xercesc/dom/DOMNotation.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "XercesDOMWrapperException.hpp"
#include "XercesWrapperHelper.hpp"
#include "XercesWrapperNavigator.hpp"
#include "XercesDocumentWrapper.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XercesNotationWrapper::XercesNotationWrapper(
            const DOMNotationType*          theXercesDOMNotation,
            const XercesWrapperNavigator&   theNavigator) :
    XalanNotation(),
    m_xercesNode(theXercesDOMNotation),
    m_navigator(theNavigator)
{
    assert(theXercesDOMNotation != 0);
}



XercesNotationWrapper*
XercesNotationWrapper::create(
            MemoryManager&                  theManager,
            const DOMNotationType*          theXercesDOMNotation,
            const XercesWrapperNavigator&   theNavigator)

{
    typedef XercesNotationWrapper   ThisType;

    XalanAllocationGuard    theGuard(theManager, theManager.allocate(sizeof(ThisType)));

    ThisType* const     theResult =
        new (theGuard.get()) ThisType(
                                theXercesDOMNotation,
                                theNavigator);

    theGuard.release();

    return theResult;
}



XercesNotationWrapper::~XercesNotationWrapper()
{
}



const XalanDOMString&
XercesNotationWrapper::getNodeName() const
{
    return m_navigator.getPooledString(m_xercesNode->getNodeName());
}



const XalanDOMString&
XercesNotationWrapper::getNodeValue() const
{
    return m_navigator.getPooledString(m_xercesNode->getNodeValue());
}



XercesNotationWrapper::NodeType
XercesNotationWrapper::getNodeType() const
{
    return NOTATION_NODE;
}



XalanNode*
XercesNotationWrapper::getParentNode() const
{
    return m_navigator.getParentNode(m_xercesNode);
}



const XalanNodeList*
XercesNotationWrapper::getChildNodes() const
{
    return 0;
}



XalanNode*
XercesNotationWrapper::getFirstChild() const
{
    return 0;
}



XalanNode*
XercesNotationWrapper::getLastChild() const
{
    return 0;
}



XalanNode*
XercesNotationWrapper::getPreviousSibling() const
{
    return m_navigator.getPreviousSibling(m_xercesNode);
}



XalanNode*
XercesNotationWrapper::getNextSibling() const
{
    return m_navigator.getNextSibling(m_xercesNode);
}



const XalanNamedNodeMap*
XercesNotationWrapper::getAttributes() const
{
    return 0;
}



XalanDocument*
XercesNotationWrapper::getOwnerDocument() const
{
    return m_navigator.getOwnerDocument();
}



const XalanDOMString&
XercesNotationWrapper::getNamespaceURI() const
{
    return m_navigator.getPooledString(m_xercesNode->getNamespaceURI());
}



const XalanDOMString&
XercesNotationWrapper::getPrefix() const
{
    return m_navigator.getPooledString(m_xercesNode->getPrefix());
}



const XalanDOMString&
XercesNotationWrapper::getLocalName() const
{
    return m_navigator.getPooledString(m_xercesNode->getLocalName());
}



bool
XercesNotationWrapper::isIndexed() const
{
    return m_navigator.getOwnerDocument()->isIndexed();
}



XercesNotationWrapper::IndexType
XercesNotationWrapper::getIndex() const
{
    return m_navigator.getIndex();
}



const XalanDOMString&
XercesNotationWrapper::getPublicId() const
{
    return m_navigator.getPooledString(m_xercesNode->getPublicId());
}



const XalanDOMString&
XercesNotationWrapper::getSystemId() const
{
    return m_navigator.getPooledString(m_xercesNode->getSystemId());
}



XALAN_CPP_NAMESPACE_END
