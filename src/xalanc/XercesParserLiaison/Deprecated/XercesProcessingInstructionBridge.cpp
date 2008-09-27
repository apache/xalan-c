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


#include "XercesProcessingInstructionBridge.hpp"



#include <xalanc/DOMSupport/DOMServices.hpp>



#include <xalanc/XercesParserLiaison/Deprecated/XercesBridgeHelper.hpp>
#include <xalanc/XercesParserLiaison/XercesDOMException.hpp>
#include <xalanc/XercesParserLiaison/Deprecated/XercesDocumentBridge.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XercesProcessingInstructionBridge::XercesProcessingInstructionBridge(
            const DOM_ProcessingInstructionType&    theXercesDOMProcessingInstruction,
            const XercesBridgeNavigator&            theNavigator) :
    XalanProcessingInstruction(),
    m_xercesNode(theXercesDOMProcessingInstruction),
    m_navigator(theNavigator)
{
}



XercesProcessingInstructionBridge::~XercesProcessingInstructionBridge()
{
}



const XalanDOMString&
XercesProcessingInstructionBridge::getNodeName() const
{
    return m_navigator.getPooledString(m_xercesNode.getNodeName());
}



const XalanDOMString&
XercesProcessingInstructionBridge::getNodeValue() const
{
    return m_navigator.getPooledString(m_xercesNode.getNodeValue());
}



XercesProcessingInstructionBridge::NodeType
XercesProcessingInstructionBridge::getNodeType() const
{
    return PROCESSING_INSTRUCTION_NODE;
}



XalanNode*
XercesProcessingInstructionBridge::getParentNode() const
{
    return m_navigator.getParentNode(m_xercesNode);
}



const XalanNodeList*
XercesProcessingInstructionBridge::getChildNodes() const
{
    return 0;
}



XalanNode*
XercesProcessingInstructionBridge::getFirstChild() const
{
    return 0;
}



XalanNode*
XercesProcessingInstructionBridge::getLastChild() const
{
    return 0;
}



XalanNode*
XercesProcessingInstructionBridge::getPreviousSibling() const
{
    return m_navigator.getPreviousSibling(m_xercesNode);
}



XalanNode*
XercesProcessingInstructionBridge::getNextSibling() const
{
    return m_navigator.getNextSibling(m_xercesNode);
}



const XalanNamedNodeMap*
XercesProcessingInstructionBridge::getAttributes() const
{
    return 0;
}



XalanDocument*
XercesProcessingInstructionBridge::getOwnerDocument() const
{
    return m_navigator.getOwnerDocument();
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XercesProcessingInstructionBridge*
#endif
XercesProcessingInstructionBridge::cloneNode(bool   deep) const
{
#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
    return m_navigator.cloneNode(this, m_xercesNode, deep);
#else
    XercesProcessingInstructionBridge* const    theBridge =
        static_cast<XercesProcessingInstructionBridge*>(m_navigator.cloneNode(this, m_xercesNode, deep));

    return theBridge;
#endif
}



XalanNode*
XercesProcessingInstructionBridge::insertBefore(
            XalanNode*  /* newChild */,
            XalanNode*  /* refChild */)
{
    throw XercesDOMException(XercesDOMException::HIERARCHY_REQUEST_ERR);

    return 0;
}



XalanNode*
XercesProcessingInstructionBridge::replaceChild(
            XalanNode*  /* newChild */,
            XalanNode*  /* oldChild */)
{
    throw XercesDOMException(XercesDOMException::HIERARCHY_REQUEST_ERR);

    return 0;
}



XalanNode*
XercesProcessingInstructionBridge::removeChild(XalanNode*   /* oldChild */)
{
    throw XercesDOMException(XercesDOMException::HIERARCHY_REQUEST_ERR);

    return 0;
}



XalanNode*
XercesProcessingInstructionBridge::appendChild(XalanNode*   /* newChild */)
{
    throw XercesDOMException(XercesDOMException::HIERARCHY_REQUEST_ERR);

    return 0;
}



bool
XercesProcessingInstructionBridge::hasChildNodes() const
{
    return false;
}



void
XercesProcessingInstructionBridge::setNodeValue(const XalanDOMString&   nodeValue)
{
    XercesBridgeHelper::setNodeValue(m_xercesNode, nodeValue);
}



void
XercesProcessingInstructionBridge::normalize()
{
    XercesBridgeHelper::normalize(m_xercesNode);
}


bool
XercesProcessingInstructionBridge::isSupported(
            const XalanDOMString&   feature,
            const XalanDOMString&   version) const
{
    return m_xercesNode.isSupported(
                XercesBridgeHelper::XalanDOMStringToXercesDOMString(feature),
                XercesBridgeHelper::XalanDOMStringToXercesDOMString(version));
}



const XalanDOMString&
XercesProcessingInstructionBridge::getNamespaceURI() const
{
    return m_navigator.getPooledString(m_xercesNode.getNamespaceURI());
}



const XalanDOMString&
XercesProcessingInstructionBridge::getPrefix() const
{
    return m_navigator.getPooledString(m_xercesNode.getPrefix());
}



const XalanDOMString&
XercesProcessingInstructionBridge::getLocalName() const
{
    return m_navigator.getPooledString(m_xercesNode.getLocalName());
}



void
XercesProcessingInstructionBridge::setPrefix(const XalanDOMString&  prefix)
{
    XercesBridgeHelper::setPrefix(m_xercesNode, prefix);
}



bool
XercesProcessingInstructionBridge::isIndexed() const
{
    return m_navigator.getOwnerDocument()->isIndexed();
}



XercesProcessingInstructionBridge::IndexType
XercesProcessingInstructionBridge::getIndex() const
{
    return m_navigator.getIndex();
}



const XalanDOMString&
XercesProcessingInstructionBridge::getTarget() const
{
    return m_navigator.getPooledString(m_xercesNode.getTarget());
}



const XalanDOMString&
XercesProcessingInstructionBridge::getData() const
{
    return m_navigator.getPooledString(m_xercesNode.getData());
}



void
XercesProcessingInstructionBridge::setData(const XalanDOMString&    data)
{
    XercesBridgeHelper::setNodeValue(m_xercesNode, data);
}



XALAN_CPP_NAMESPACE_END


#endif //XALAN_BUILD_DEPRECATED_DOM_BRIDGE 


