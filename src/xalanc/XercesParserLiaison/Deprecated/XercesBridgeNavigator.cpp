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


#include "XercesBridgeNavigator.hpp"



#include <xalanc/XalanDOM/XalanNode.hpp>



#include <xalanc/XercesParserLiaison/Deprecated/XercesAttrBridge.hpp>
#include <xalanc/XercesParserLiaison/Deprecated/XercesDocumentBridge.hpp>
#include <xalanc/XercesParserLiaison/Deprecated/XercesTextBridge.hpp>
#include <xalanc/XercesParserLiaison/XercesDOMException.hpp>



XALAN_CPP_NAMESPACE_BEGIN



// This is used to distinguish between null nodes, which are valid, and
// an uninitialized cached node address.
static XalanNode* const     invalidNodeAddress = reinterpret_cast<XalanNode*>(1);



XercesBridgeNavigator::XercesBridgeNavigator(
            XercesDocumentBridge*   theOwnerDocument,
            bool                    mappingMode) :
    m_ownerDocument(theOwnerDocument),
    m_parentNode(mappingMode == true ? invalidNodeAddress : 0),
    m_previousSibling(mappingMode == true ? invalidNodeAddress : 0),
    m_nextSibling(mappingMode == true ? invalidNodeAddress : 0),
    m_firstChild(mappingMode == true ? invalidNodeAddress : 0),
    m_lastChild(mappingMode == true ? invalidNodeAddress : 0),
    m_index(0)
{
}



XercesBridgeNavigator::XercesBridgeNavigator(const XercesBridgeNavigator&   theSource) :
    m_ownerDocument(theSource.m_ownerDocument),
    m_parentNode(theSource.m_parentNode),
    m_previousSibling(theSource.m_previousSibling),
    m_nextSibling(theSource.m_nextSibling),
    m_firstChild(theSource.m_firstChild),
    m_lastChild(theSource.m_lastChild),
    m_index(theSource.m_index)
{
}



XercesBridgeNavigator::~XercesBridgeNavigator()
{
}



XalanNode*
XercesBridgeNavigator::mapNode(const DOM_NodeType&  theXercesNode) const
{
    return m_ownerDocument->mapNode(theXercesNode);
}



XalanAttr*
XercesBridgeNavigator::mapNode(const DOM_AttrType&  theXercesNode) const
{
    return m_ownerDocument->mapNode(theXercesNode);
}



DOM_NodeType
XercesBridgeNavigator::mapNode(const XalanNode*     theXalanNode) const
{
    return m_ownerDocument->mapNode(theXalanNode);
}



DOM_AttrType
XercesBridgeNavigator::mapNode(const XalanAttr*     theXalanNode) const
{
    return m_ownerDocument->mapNode(theXalanNode);
}



XalanNode*
XercesBridgeNavigator::getParentNode(const DOM_NodeType&    theXercesNode) const
{
    if (m_parentNode == invalidNodeAddress)
    {
        m_parentNode = m_ownerDocument->mapNode(theXercesNode.getParentNode());
    }

    return m_parentNode;
}



XalanNode*
XercesBridgeNavigator::getPreviousSibling(const DOM_NodeType&   theXercesNode) const
{
    if (m_previousSibling == invalidNodeAddress)
    {
        m_previousSibling = m_ownerDocument->mapNode(theXercesNode.getPreviousSibling());
    }

    return m_previousSibling;
}



XalanNode*
XercesBridgeNavigator::getNextSibling(const DOM_NodeType&   theXercesNode) const
{
    if (m_nextSibling == invalidNodeAddress)
    {
        m_nextSibling = m_ownerDocument->mapNode(theXercesNode.getNextSibling());
    }

    return m_nextSibling;
}



XalanNode*
XercesBridgeNavigator::getFirstChild(const DOM_NodeType&    theXercesNode) const
{
    if (m_firstChild == invalidNodeAddress)
    {
        m_firstChild = m_ownerDocument->mapNode(theXercesNode.getFirstChild());
    }

    return m_firstChild;
}



XalanNode*
XercesBridgeNavigator::getLastChild(const DOM_NodeType&     theXercesNode) const
{
    if (m_lastChild == invalidNodeAddress)
    {
        m_lastChild = m_ownerDocument->mapNode(theXercesNode.getLastChild());
    }

    return m_lastChild;
}



XalanNode*
XercesBridgeNavigator::insertBefore(
            DOM_NodeType&   theXercesParent,
            XalanNode*      newChild,
            XalanNode*      refChild) const
{
    assert(newChild != 0);

    // Get the corresponding Xerces nodes...
    const DOM_NodeType  theNewChild = m_ownerDocument->mapNode(newChild);
    const DOM_NodeType  theRefChild = m_ownerDocument->mapNode(refChild);

    try
    {
        const DOM_NodeType  theXercesResult =
            theXercesParent.insertBefore(theNewChild, theRefChild);
        assert(m_ownerDocument->mapNode(theXercesResult) == newChild);
    }
    catch(const DOM_DOMExceptionType&   theException)
    {
        throw XercesDOMException(theException);
    }

    return newChild;
}



XalanNode*
XercesBridgeNavigator:: replaceChild(
            DOM_NodeType&   theXercesParent,
            XalanNode*      newChild,
            XalanNode*      oldChild) const
{
    assert(newChild != 0);
    assert(oldChild != 0);

    // Get the corresponding Xerces nodes...
    const DOM_NodeType  theNewChild = m_ownerDocument->mapNode(newChild);
    const DOM_NodeType  theOldChild = m_ownerDocument->mapNode(oldChild);

    try
    {
        const DOM_NodeType  theXercesResult =
            theXercesParent.replaceChild(theNewChild, theOldChild);
        assert(m_ownerDocument->mapNode(theXercesResult) == oldChild);
    }
    catch(const DOM_DOMExceptionType&   theException)
    {
        throw XercesDOMException(theException);
    }

    return oldChild;
}



XalanNode*
XercesBridgeNavigator::removeChild(
            DOM_NodeType&   theXercesParent,
            XalanNode*      oldChild) const
{
    assert(oldChild != 0);

    // Get the corresponding Xerces nodes...
    const DOM_NodeType  theOldChild = m_ownerDocument->mapNode(oldChild);

    try
    {
        const DOM_NodeType  theXercesResult =
            theXercesParent.removeChild(theOldChild);
        assert(m_ownerDocument->mapNode(theXercesResult) == oldChild);
    }
    catch(const DOM_DOMExceptionType&   theException)
    {
        throw XercesDOMException(theException);
    }

    return oldChild;
}



XalanNode*
XercesBridgeNavigator::appendChild(
            DOM_NodeType&   theXercesParent,
            XalanNode*      newChild) const
{
    return insertBefore(theXercesParent, newChild, 0);
}




XalanElement*
XercesBridgeNavigator::getOwnerElement(const DOM_AttrType&  theXercesAttr) const
{
    if (m_parentNode != invalidNodeAddress)
    {
        assert(m_parentNode->getNodeType() == XalanNode::ELEMENT_NODE);

        return static_cast<XalanElement*>(m_parentNode);
    }
    else
    {
        return m_ownerDocument->mapNode(theXercesAttr.getOwnerElement());
    }
}



XalanNode*
XercesBridgeNavigator::cloneNode(
            const XalanNode*        theXalanNode,
            const DOM_NodeType&     theXercesNode,
            bool                    deep) const
{
    return m_ownerDocument->internalCloneNode(theXalanNode, theXercesNode, deep);
}



XalanText*
XercesBridgeNavigator::splitText(
            DOM_TextType&   theXercesText,
            unsigned int    offset) const
{
    XalanText*  theXalanText = 0;

    try
    {
        DOM_TextType    theNewXercesText = theXercesText.splitText(offset);
        assert(theXercesText.isNull() == false);

        theXalanText = m_ownerDocument->createBridgeNode(theNewXercesText, 0, true);
    }
    catch(const DOM_DOMExceptionType&   theException)
    {
        throw XercesDOMException(theException);
    }

    return theXalanText;
}



const XalanDOMString&
XercesBridgeNavigator:: getPooledString(const DOMStringType&    theString) const
{
    return m_ownerDocument->getPooledString(theString.rawBuffer(), theString.length());
}



XALAN_CPP_NAMESPACE_END


#endif //XALAN_BUILD_DEPRECATED_DOM_BRIDGE 


