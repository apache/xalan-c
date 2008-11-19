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
#include "XercesCDATASectionWrapper.hpp"



#include <cassert>



#include <xercesc/dom/DOMCDATASection.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "XercesWrapperHelper.hpp"
#include "XercesWrapperNavigator.hpp"
#include "XercesDOMWrapperException.hpp"
#include "XercesDocumentWrapper.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XercesCDATASectionWrapper::XercesCDATASectionWrapper(
            const DOMCDATASectionType*      theXercesCDATASection,
            const XercesWrapperNavigator&   theNavigator) :
    XalanCDATASection(),
    m_xercesNode(theXercesCDATASection),
    m_navigator(theNavigator)
{
    assert(theXercesCDATASection != 0);
}


    
XercesCDATASectionWrapper*
XercesCDATASectionWrapper::create(
            MemoryManager&                  theManager,
            const DOMCDATASectionType*      theXercesCDATASection,
            const XercesWrapperNavigator&   theNavigator)

{
    typedef XercesCDATASectionWrapper   ThisType;

    XalanAllocationGuard    theGuard(theManager, theManager.allocate(sizeof(ThisType)));

    ThisType* const     theResult =
        new (theGuard.get()) ThisType(
                                theXercesCDATASection,
                                theNavigator);

    theGuard.release();

    return theResult;
}



XercesCDATASectionWrapper::~XercesCDATASectionWrapper()
{
}



const XalanDOMString&
XercesCDATASectionWrapper::getNodeName() const
{
    return m_navigator.getPooledString(m_xercesNode->getNodeName());
}



const XalanDOMString&
XercesCDATASectionWrapper::getNodeValue() const
{
    return m_navigator.getPooledString(m_xercesNode->getNodeValue());
}



XercesCDATASectionWrapper::NodeType
XercesCDATASectionWrapper::getNodeType() const
{
    return CDATA_SECTION_NODE;
}



XalanNode*
XercesCDATASectionWrapper::getParentNode() const
{
    return m_navigator.getParentNode(m_xercesNode);
}



const XalanNodeList*
XercesCDATASectionWrapper::getChildNodes() const
{
    return 0;
}



XalanNode*
XercesCDATASectionWrapper::getFirstChild() const
{
    return 0;
}



XalanNode*
XercesCDATASectionWrapper::getLastChild() const
{
    return 0;
}



XalanNode*
XercesCDATASectionWrapper::getPreviousSibling() const
{
    return m_navigator.getPreviousSibling(m_xercesNode);
}



XalanNode*
XercesCDATASectionWrapper::getNextSibling() const
{
    return m_navigator.getNextSibling(m_xercesNode);
}



const XalanNamedNodeMap*
XercesCDATASectionWrapper::getAttributes() const
{
    return 0;
}



XalanDocument*
XercesCDATASectionWrapper::getOwnerDocument() const
{
    return m_navigator.getOwnerDocument();
}



const XalanDOMString&
XercesCDATASectionWrapper::getNamespaceURI() const
{
    return m_navigator.getPooledString(m_xercesNode->getNamespaceURI());
}



const XalanDOMString&
XercesCDATASectionWrapper::getPrefix() const
{
    return m_navigator.getPooledString(m_xercesNode->getPrefix());
}



const XalanDOMString&
XercesCDATASectionWrapper::getLocalName() const
{
    return m_navigator.getPooledString(m_xercesNode->getLocalName());
}



bool
XercesCDATASectionWrapper::isIndexed() const
{
    return m_navigator.getOwnerDocument()->isIndexed();
}



XercesCDATASectionWrapper::IndexType
XercesCDATASectionWrapper::getIndex() const
{
    return m_navigator.getIndex();
}



const XalanDOMString&
XercesCDATASectionWrapper::getData() const
{
    return m_navigator.getPooledString(m_xercesNode->getData());
}



bool
XercesCDATASectionWrapper::isWhitespace() const
{
    return isXMLWhitespace(getData());
}



XALAN_CPP_NAMESPACE_END
