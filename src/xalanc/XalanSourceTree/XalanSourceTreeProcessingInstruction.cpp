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
#include "XalanSourceTreeProcessingInstruction.hpp"



#include <xalanc/XalanDOM/XalanDOMException.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



#include "XalanSourceTreeComment.hpp"
#include "XalanSourceTreeDocument.hpp"
#include "XalanSourceTreeDocumentFragment.hpp"
#include "XalanSourceTreeElement.hpp"
#include "XalanSourceTreeText.hpp"
#include "XalanSourceTreeHelper.hpp"



XALAN_CPP_NAMESPACE_BEGIN



static const XalanDOMString     s_emptyString(XalanMemMgrs::getDummyMemMgr());



XalanSourceTreeProcessingInstruction::XalanSourceTreeProcessingInstruction(
            const XalanDOMString&       theTarget,
            const XalanDOMString&       theData,
            XalanSourceTreeDocument*    theOwnerDocument,
            XalanNode*                  theParentNode,
            XalanNode*                  thePreviousSibling,
            XalanNode*                  theNextSibling,
            IndexType                   theIndex) :
    XalanProcessingInstruction(),
    m_target(theTarget),
    m_data(theData),
    m_ownerDocument(theOwnerDocument),
    m_parentNode(theParentNode),
    m_previousSibling(thePreviousSibling),
    m_nextSibling(theNextSibling),
    m_index(theIndex)
{
}



XalanSourceTreeProcessingInstruction::~XalanSourceTreeProcessingInstruction()
{
}



const XalanDOMString&
XalanSourceTreeProcessingInstruction::getNodeName() const
{
    return m_target;
}



const XalanDOMString&
XalanSourceTreeProcessingInstruction::getNodeValue() const
{
    return m_data;
}



XalanSourceTreeProcessingInstruction::NodeType
XalanSourceTreeProcessingInstruction::getNodeType() const
{
    return PROCESSING_INSTRUCTION_NODE;
}



XalanNode*
XalanSourceTreeProcessingInstruction::getParentNode() const
{
    if (m_parentNode != 0)
    {
        return m_parentNode;
    }
    else
    {
        return m_ownerDocument;
    }
}



const XalanNodeList*
XalanSourceTreeProcessingInstruction::getChildNodes() const
{
    throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

    // Dummy return value...
    return 0;
}



XalanNode*
XalanSourceTreeProcessingInstruction::getFirstChild() const
{
    return 0;
}



XalanNode*
XalanSourceTreeProcessingInstruction::getLastChild() const
{
    return 0;
}



XalanNode*
XalanSourceTreeProcessingInstruction::getPreviousSibling() const
{
    return m_previousSibling;
}



XalanNode*
XalanSourceTreeProcessingInstruction::getNextSibling() const
{
    return m_nextSibling;
}



const XalanNamedNodeMap*
XalanSourceTreeProcessingInstruction::getAttributes() const
{
    return 0;
}



XalanDocument*
XalanSourceTreeProcessingInstruction::getOwnerDocument() const
{
    assert(m_ownerDocument != 0);

    return m_ownerDocument;
}



const XalanDOMString&
XalanSourceTreeProcessingInstruction::getNamespaceURI() const
{
    return s_emptyString;
}



const XalanDOMString&
XalanSourceTreeProcessingInstruction::getPrefix() const
{
    return s_emptyString;
}



const XalanDOMString&
XalanSourceTreeProcessingInstruction::getLocalName() const
{
    return s_emptyString;
}



bool
XalanSourceTreeProcessingInstruction::isIndexed() const
{
    return true;
}



XalanSourceTreeProcessingInstruction::IndexType
XalanSourceTreeProcessingInstruction::getIndex() const
{
    return m_index;
}



const XalanDOMString&
XalanSourceTreeProcessingInstruction::getTarget() const
{
    return m_target;
}



const XalanDOMString&
XalanSourceTreeProcessingInstruction::getData() const
{
    return m_data;
}



void
XalanSourceTreeProcessingInstruction::setParent(XalanSourceTreeElement* theParent)
{
    m_parentNode = theParent;
}



void
XalanSourceTreeProcessingInstruction::setParent(XalanSourceTreeDocumentFragment*    theParent)
{
    m_parentNode = theParent;
}



void
XalanSourceTreeProcessingInstruction::setPreviousSibling(XalanSourceTreeComment*    thePreviousSibling)
{
    m_previousSibling = thePreviousSibling;
}



void
XalanSourceTreeProcessingInstruction::setPreviousSibling(XalanSourceTreeElement*    thePreviousSibling)
{
    m_previousSibling = thePreviousSibling;
}



void
XalanSourceTreeProcessingInstruction::setPreviousSibling(XalanSourceTreeProcessingInstruction*  thePreviousSibling)
{
    m_previousSibling = thePreviousSibling;
}



void
XalanSourceTreeProcessingInstruction::setPreviousSibling(XalanSourceTreeText*   thePreviousSibling)
{
    m_previousSibling = thePreviousSibling;
}



void
XalanSourceTreeProcessingInstruction::appendSiblingNode(XalanSourceTreeComment*     theSibling)
{
    XalanSourceTreeHelper::appendSibling(this, m_nextSibling, theSibling);
}



void
XalanSourceTreeProcessingInstruction::appendSiblingNode(XalanSourceTreeElement*     theSibling)
{
    XalanSourceTreeHelper::appendSibling(this, m_nextSibling, theSibling);
}



void
XalanSourceTreeProcessingInstruction::appendSiblingNode(XalanSourceTreeProcessingInstruction*   theSibling)
{
    XalanSourceTreeHelper::appendSibling(this, m_nextSibling, theSibling);
}



void
XalanSourceTreeProcessingInstruction::appendSiblingNode(XalanSourceTreeText*    theSibling)
{
    XalanSourceTreeHelper::appendSibling(this, m_nextSibling, theSibling);
}



XALAN_CPP_NAMESPACE_END
