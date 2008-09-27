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


#include "XercesNamedNodeMapBridge.hpp"



#include <algorithm>
#include <cassert>



#if XERCES_VERSION_MAJOR >= 2
#include <xercesc/dom/deprecated/DOM_Node.hpp>
#else
#include <xercesc/dom/DOM_Node.hpp>
#endif



#include <xalanc/XercesParserLiaison/Deprecated/XercesDocumentBridge.hpp>
#include <xalanc/XercesParserLiaison/XercesDOMException.hpp>
#include <xalanc/XercesParserLiaison/Deprecated/XercesElementBridge.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XercesNamedNodeMapBridge::XercesNamedNodeMapBridge(
            const DOM_NamedNodeMapType&     theXercesNamedNodeMap,
            const XercesBridgeNavigator&    theNavigator) :
    m_xercesNamedNodeMap(theXercesNamedNodeMap),
    m_navigator(theNavigator)
{
    assert(m_xercesNamedNodeMap != (const XERCES_CPP_NAMESPACE_QUALIFIER DOM_NullPtr*)0);
}



XercesNamedNodeMapBridge::~XercesNamedNodeMapBridge()
{
}



XalanNode*
XercesNamedNodeMapBridge::setNamedItem(XalanNode*   arg)
{
    assert(arg != 0);

    XalanNode*  theXalanNode = 0;

    // Find the DOM_Node that corresponds to the bridge node...
    const DOM_NodeType  theNode = m_navigator.mapNode(arg);

    if (theNode.isNull() == true)
    {
        // Doesn't belong to this doc, so throw...
        throw XercesDOMException(XercesDOMException::WRONG_DOCUMENT_ERR);
    }

    try
    {
        const DOM_NodeType  theOldXercesNode =
            m_xercesNamedNodeMap.setNamedItem(theNode);

        if (theNode.isNull() == false)
        {
            // A node was replaced, so map the replacement...
            theXalanNode = m_navigator.mapNode(theOldXercesNode);
            assert(theXalanNode != 0);
        }
    }
    catch(const DOM_DOMExceptionType&   theException)
    {
        throw XercesDOMException(theException);
    }

    return theXalanNode;
}



XalanNode*
XercesNamedNodeMapBridge::item(unsigned int index) const
{
    return m_navigator.mapNode(m_xercesNamedNodeMap.item(index));
}



XalanNode*
XercesNamedNodeMapBridge::getNamedItem(const XalanDOMString&    name) const
{
    return m_navigator.mapNode(m_xercesNamedNodeMap.getNamedItem(name.c_str()));
}



unsigned int
XercesNamedNodeMapBridge::getLength() const
{
    return m_xercesNamedNodeMap.getLength();
}



XalanNode*
XercesNamedNodeMapBridge::removeNamedItem(const XalanDOMString&     name)
{
    XalanNode*  theXalanNode = 0;

    try
    {
        const DOM_NodeType  theXercesNode =
            m_xercesNamedNodeMap.removeNamedItem(name.c_str());

        // A node was removed, so get it corresponding XalanNode...
        theXalanNode = m_navigator.mapNode(theXercesNode);
        assert(theXalanNode != 0);
    }
    catch(const DOM_DOMExceptionType&   theException)
    {
        throw XercesDOMException(theException);
    }

    return theXalanNode;
}



XalanNode*
XercesNamedNodeMapBridge::getNamedItemNS(
            const XalanDOMString&   namespaceURI,
            const XalanDOMString&   localName) const
{
    return m_navigator.mapNode(m_xercesNamedNodeMap.getNamedItemNS(namespaceURI.c_str(), localName.c_str()));
}



XalanNode*
XercesNamedNodeMapBridge::setNamedItemNS(XalanNode*     arg)
{
    assert(arg != 0);

    XalanNode*  theXalanNode = 0;

    // Find the DOM_Node that corresponds to the bridge node...
    const DOM_NodeType  theNode = m_navigator.mapNode(arg);

    if (theNode.isNull() == true)
    {
        // Doesn't belong to this doc, so throw...
        throw XercesDOMException(XercesDOMException::WRONG_DOCUMENT_ERR);
    }

    try
    {
        const DOM_NodeType  theOldXercesNode =
            m_xercesNamedNodeMap.setNamedItemNS(theNode);

        if (theNode.isNull() == false)
        {
            // A node was replaced, so map the replacement...
            theXalanNode = m_navigator.mapNode(theOldXercesNode);
            assert(theXalanNode != 0);
        }
    }
    catch(const DOM_DOMExceptionType&   theException)
    {
        throw XercesDOMException(theException);
    }

    return theXalanNode;
}



XalanNode*
XercesNamedNodeMapBridge::removeNamedItemNS(
            const XalanDOMString&   namespaceURI,
            const XalanDOMString&   localName)
{
    XalanNode*  theXalanNode = 0;

    try
    {
        const DOM_NodeType  theXercesNode =
            m_xercesNamedNodeMap.removeNamedItemNS(namespaceURI.c_str(), localName.c_str());

        // A node was removed, so get it corresponding XalanNode...
        theXalanNode = m_navigator.mapNode(theXercesNode);
        assert(theXalanNode != 0);
    }
    catch(const DOM_DOMExceptionType&   theException)
    {
        throw XercesDOMException(theException);
    }

    return theXalanNode;
}



XALAN_CPP_NAMESPACE_END


#endif //XALAN_BUILD_DEPRECATED_DOM_BRIDGE 


