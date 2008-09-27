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
#include "XalanSourceTreeText.hpp"



#include <xalanc/XalanDOM/XalanDOMException.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



#include "XalanSourceTreeComment.hpp"
#include "XalanSourceTreeDocumentFragment.hpp"
#include "XalanSourceTreeElement.hpp"
#include "XalanSourceTreeProcessingInstruction.hpp"
#include "XalanSourceTreeHelper.hpp"



XALAN_CPP_NAMESPACE_BEGIN



static const XalanDOMString     s_emptyString(XalanMemMgrs::getDummyMemMgr());



XalanSourceTreeText::XalanSourceTreeText(
            const XalanDOMString&   theData,
            XalanNode*              theParentNode,
            XalanNode*              thePreviousSibling,
            XalanNode*              theNextSibling,
            IndexType               theIndex) :
    XalanText(),
    m_data(theData),
    m_parentNode(theParentNode),
    m_previousSibling(thePreviousSibling),
    m_nextSibling(theNextSibling),
    m_index(theIndex)
{
}



XalanSourceTreeText::~XalanSourceTreeText()
{
}



const XalanDOMString&
XalanSourceTreeText::getNodeName() const
{
    return s_nameString;
}



const XalanDOMString&
XalanSourceTreeText::getNodeValue() const
{
    return m_data;
}



XalanSourceTreeText::NodeType
XalanSourceTreeText::getNodeType() const
{
    return TEXT_NODE;
}



XalanNode*
XalanSourceTreeText::getParentNode() const
{
    return m_parentNode;
}



const XalanNodeList*
XalanSourceTreeText::getChildNodes() const
{
    throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

    // Dummy return value...
    return 0;
}



XalanNode*
XalanSourceTreeText::getFirstChild() const
{
    return 0;
}



XalanNode*
XalanSourceTreeText::getLastChild() const
{
    return 0;
}



XalanNode*
XalanSourceTreeText::getPreviousSibling() const
{
    return m_previousSibling;
}



XalanNode*
XalanSourceTreeText::getNextSibling() const
{
    return m_nextSibling;
}



const XalanNamedNodeMap*
XalanSourceTreeText::getAttributes() const
{
    return 0;
}



XalanDocument*
XalanSourceTreeText::getOwnerDocument() const
{
    assert(m_parentNode != 0);

    return m_parentNode->getOwnerDocument();
}



const XalanDOMString&
XalanSourceTreeText::getNamespaceURI() const
{
    return s_emptyString;
}



const XalanDOMString&
XalanSourceTreeText::getPrefix() const
{
    return s_emptyString;
}



const XalanDOMString&
XalanSourceTreeText::getLocalName() const
{
    return s_emptyString;
}



bool
XalanSourceTreeText::isIndexed() const
{
    return true;
}



XalanSourceTreeText::IndexType
XalanSourceTreeText::getIndex() const
{
    return m_index;
}



const XalanDOMString&
XalanSourceTreeText::getData() const
{
    return m_data;
}



bool
XalanSourceTreeText::isWhitespace() const
{
    return false;
}



void
XalanSourceTreeText::setParent(XalanSourceTreeElement*  theParent)
{
    m_parentNode = theParent;
}



void
XalanSourceTreeText::setParent(XalanSourceTreeDocumentFragment* theParent)
{
    m_parentNode = theParent;
}



void
XalanSourceTreeText::setPreviousSibling(XalanSourceTreeComment*     thePreviousSibling)
{
    m_previousSibling = thePreviousSibling;
}



void
XalanSourceTreeText::setPreviousSibling(XalanSourceTreeElement*     thePreviousSibling)
{
    m_previousSibling = thePreviousSibling;
}



void
XalanSourceTreeText::setPreviousSibling(XalanSourceTreeProcessingInstruction*   thePreviousSibling)
{
    m_previousSibling = thePreviousSibling;
}



void
XalanSourceTreeText::setPreviousSibling(XalanSourceTreeText*    thePreviousSibling)
{
    m_previousSibling = thePreviousSibling;
}



void
XalanSourceTreeText::appendSiblingNode(XalanSourceTreeComment*  theSibling)
{
    XalanSourceTreeHelper::appendSibling(this, m_nextSibling, theSibling);
}



void
XalanSourceTreeText::appendSiblingNode(XalanSourceTreeElement*  theSibling)
{
    XalanSourceTreeHelper::appendSibling(this, m_nextSibling, theSibling);
}



void
XalanSourceTreeText::appendSiblingNode(XalanSourceTreeProcessingInstruction*    theSibling)
{
    XalanSourceTreeHelper::appendSibling(this, m_nextSibling, theSibling);
}



void
XalanSourceTreeText::appendSiblingNode(XalanSourceTreeText*     theSibling)
{
    XalanSourceTreeHelper::appendSibling(this, m_nextSibling, theSibling);
}



static XalanDOMString   s_staticNameString(XalanMemMgrs::getDummyMemMgr());



const XalanDOMString&   XalanSourceTreeText::s_nameString = s_staticNameString;



const XalanDOMChar  s_text[] =
{
    XalanUnicode::charNumberSign,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_x,
    XalanUnicode::charLetter_t,
    0
};



void
XalanSourceTreeText::initialize(MemoryManager& theManager)
{
    XalanDOMString theBuffer(s_text, theManager);

    s_staticNameString.swap(theBuffer);
}



void
XalanSourceTreeText::terminate()
{
    releaseMemory(s_staticNameString, XalanMemMgrs::getDummyMemMgr());
}



XALAN_CPP_NAMESPACE_END
