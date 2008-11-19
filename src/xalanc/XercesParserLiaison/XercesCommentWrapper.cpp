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
#include "XercesCommentWrapper.hpp"



#include <xercesc/dom/DOMComment.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "XercesWrapperHelper.hpp"
#include "XercesWrapperNavigator.hpp"
#include "XercesDOMWrapperException.hpp"
#include "XercesDocumentWrapper.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XercesCommentWrapper::XercesCommentWrapper(
            const DOMCommentType*           theXercesComment,
            const XercesWrapperNavigator&   theNavigator) :
    XalanComment(),
    m_xercesNode(theXercesComment),
    m_navigator(theNavigator)
{
    assert(theXercesComment != 0);
}



XercesCommentWrapper*
XercesCommentWrapper::create(
            MemoryManager&                  theManager,
            const DOMCommentType*          theXercesComment,
            const XercesWrapperNavigator&  theNavigator)

{
    typedef XercesCommentWrapper    ThisType;

    XalanAllocationGuard    theGuard(theManager, theManager.allocate(sizeof(ThisType)));

    ThisType* const     theResult =
        new (theGuard.get()) ThisType(
                                theXercesComment,
                                theNavigator);

    theGuard.release();

    return theResult;
}



XercesCommentWrapper::~XercesCommentWrapper()
{
}



const XalanDOMString&
XercesCommentWrapper::getNodeName() const
{
    return m_navigator.getPooledString(m_xercesNode->getNodeName());
}



const XalanDOMString&
XercesCommentWrapper::getNodeValue() const
{
    return m_navigator.getPooledString(m_xercesNode->getNodeValue());
}



XercesCommentWrapper::NodeType
XercesCommentWrapper::getNodeType() const
{
    return COMMENT_NODE;
}



XalanNode*
XercesCommentWrapper::getParentNode() const
{
    return m_navigator.getParentNode(m_xercesNode);
}



const XalanNodeList*
XercesCommentWrapper::getChildNodes() const
{
    return 0;
}



XalanNode*
XercesCommentWrapper::getFirstChild() const
{
    return 0;
}



XalanNode*
XercesCommentWrapper::getLastChild() const
{
    return 0;
}



XalanNode*
XercesCommentWrapper::getPreviousSibling() const
{
    return m_navigator.getPreviousSibling(m_xercesNode);
}



XalanNode*
XercesCommentWrapper::getNextSibling() const
{
    return m_navigator.getNextSibling(m_xercesNode);
}



const XalanNamedNodeMap*
XercesCommentWrapper::getAttributes() const
{
    return 0;
}



XalanDocument*
XercesCommentWrapper::getOwnerDocument() const
{
    return m_navigator.getOwnerDocument();
}



const XalanDOMString&
XercesCommentWrapper::getNamespaceURI() const
{
    return m_navigator.getPooledString(m_xercesNode->getNamespaceURI());
}



const XalanDOMString&
XercesCommentWrapper::getPrefix() const
{
    return m_navigator.getPooledString(m_xercesNode->getPrefix());
}



const XalanDOMString&
XercesCommentWrapper::getLocalName() const
{
    return m_navigator.getPooledString(m_xercesNode->getLocalName());
}



bool
XercesCommentWrapper::isIndexed() const
{
    return m_navigator.getOwnerDocument()->isIndexed();
}



XercesCommentWrapper::IndexType
XercesCommentWrapper::getIndex() const
{
    return m_navigator.getIndex();
}



const XalanDOMString&
XercesCommentWrapper::getData() const
{
    return m_navigator.getPooledString(m_xercesNode->getData());
}



XALAN_CPP_NAMESPACE_END
