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


#include "XercesDocumentBridge.hpp"



#include <algorithm>
#include <cassert>



#include <xalanc/Include/STLHelper.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include <xalanc/XalanDOM/XalanDocumentType.hpp>
#include <xalanc/XalanDOM/XalanElement.hpp>



#include <xalanc/XercesParserLiaison/Deprecated/XercesAttrBridge.hpp>
#include <xalanc/XercesParserLiaison/Deprecated/XercesBridgeHelper.hpp>
#include <xalanc/XercesParserLiaison/Deprecated/XercesCommentBridge.hpp>
#include <xalanc/XercesParserLiaison/Deprecated/XercesCDATASectionBridge.hpp>
#include <xalanc/XercesParserLiaison/Deprecated/XercesDOM_NodeHack.hpp>
#include <xalanc/XercesParserLiaison/XercesDOMException.hpp>
#include <xalanc/XercesParserLiaison/Deprecated/XercesDOMImplementationBridge.hpp>
#include <xalanc/XercesParserLiaison/Deprecated/XercesDocumentFragmentBridge.hpp>
#include <xalanc/XercesParserLiaison/Deprecated/XercesDocumentTypeBridge.hpp>
#include <xalanc/XercesParserLiaison/Deprecated/XercesElementBridge.hpp>
#include <xalanc/XercesParserLiaison/Deprecated/XercesEntityBridge.hpp>
#include <xalanc/XercesParserLiaison/Deprecated/XercesEntityReferenceBridge.hpp>
#include <xalanc/XercesParserLiaison/XercesLiaisonXalanDOMStringPool.hpp>
#include <xalanc/XercesParserLiaison/Deprecated/XercesNodeListBridge.hpp>
#include <xalanc/XercesParserLiaison/Deprecated/XercesNotationBridge.hpp>
#include <xalanc/XercesParserLiaison/Deprecated/XercesProcessingInstructionBridge.hpp>
#include <xalanc/XercesParserLiaison/Deprecated/XercesTextBridge.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XercesDocumentBridge::XercesDocumentBridge(
            const DOM_Document_Type&    theXercesDocument,
            bool                        threadSafe,
            bool                        buildBridge) :
    XalanDocument(),
    m_xercesDocument(theXercesDocument),
    m_documentElement(0),
    m_nodeMap(),
    m_domImplementation(new XercesDOMImplementationBridge(theXercesDocument.getImplementation())),
    m_navigators(1, XercesBridgeNavigator(this, threadSafe == true ? false : !buildBridge)),
    m_navigator(&m_navigators.front()),
    m_children(theXercesDocument.getChildNodes(),
               *m_navigator),
    m_nodes(),
    m_doctype(0),
    m_mappingMode(threadSafe == true ? false : !buildBridge),
    m_indexValid(false),
    m_elementAllocator(25),
    m_textAllocator(25),
    m_attributeAllocator(25),
    m_stringPool(threadSafe == true ? new XercesLiaisonXalanDOMStringPool : new XalanDOMStringPool)
{
    if (m_mappingMode == false)
    {
        // The document index is always 1...
        m_navigator->setIndex(1);

        // OK, let's build the nodes.  This makes things
        // thread-safe, so the document can be shared...
        buildBridgeNodes();
    }
    else
    {
        m_nodeMap.addAssociation(m_xercesDocument, this);
    }
}



XercesDocumentBridge::~XercesDocumentBridge()
{
    destroyBridge();
}



XalanNode*
XercesDocumentBridge::mapNode(const DOM_NodeType&   theXercesNode) const
{
    return mapNode(XercesDOM_NodeHack::getImpl(theXercesNode));
}



XalanNode*
XercesDocumentBridge::mapNode(NodeImplType*     theXercesNodeImpl) const
{
    XalanNode*  theXalanNode = 0;

    if (theXercesNodeImpl != 0)
    {
        theXalanNode = m_nodeMap.getNode(theXercesNodeImpl);

        if (theXalanNode == 0)
        {
            XercesDOM_NodeHack  theHack(theXercesNodeImpl);

            if (theHack != m_xercesDocument &&
                    theHack.getOwnerDocument() != m_xercesDocument &&
                theHack.getParentNode() != m_xercesDocument)
            {
                throw XalanDOMException(XalanDOMException::WRONG_DOCUMENT_ERR);
            }
            else if (m_mappingMode == true)
            {
                // OK, we haven't yet created a bridge not, so go ahead and
                // create one.
                theXalanNode = createBridgeNode(theHack, 0, true);
                assert(theXalanNode != 0);
            }
        }
    }

    return theXalanNode;
}



DOM_NodeType
XercesDocumentBridge::mapNode(const XalanNode*  theXalanNode) const
{
    DOM_NodeType    theXercesNode;

    if (theXalanNode != 0)
    {
        theXercesNode = m_nodeMap.getNode(theXalanNode);

        if (theXercesNode.isNull() == true)
        {
            throw XalanDOMException(XalanDOMException::WRONG_DOCUMENT_ERR);
        }
    }

    return theXercesNode;
}



DOM_AttrType
XercesDocumentBridge::mapNode(const XalanAttr*  theXalanNode) const
{
    NodeImplType* const     theXercesNodeImpl =
        mapNodeToImpl(theXalanNode);

    return XercesDOM_AttrHack(reinterpret_cast<AttrImplType*>(theXercesNodeImpl));
}



NodeImplType*
XercesDocumentBridge::mapNodeToImpl(const XalanNode*    theXalanNode) const
{
    NodeImplType*   theXercesNodeImpl = 0;

    if (theXalanNode != 0)
    {
        theXercesNodeImpl = m_nodeMap.getNodeImpl(theXalanNode);

        if (theXercesNodeImpl == 0)
        {
            throw XalanDOMException(XalanDOMException::WRONG_DOCUMENT_ERR);
        }
    }

    return theXercesNodeImpl;
}



XalanAttr*
XercesDocumentBridge::mapNode(const DOM_AttrType&   theXercesNode) const
{
    return static_cast<XercesAttrBridge*>(mapNode(XercesDOM_NodeHack::getImpl(theXercesNode)));
}



XalanElement*
XercesDocumentBridge::mapNode(const DOM_ElementType&    theXercesNode) const
{
    return static_cast<XercesElementBridge*>(mapNode(XercesDOM_NodeHack::getImpl(theXercesNode)));
}



void
XercesDocumentBridge::destroyBridge()
{
    XALAN_USING_STD(for_each)

    // Set this to null, since it will be deleted
    // by the next for_each...
    m_doctype = 0;

    // m_bridgeMap contains all of the nodes that
    // are still alive...
    for_each(
            m_nodes.begin(),
            m_nodes.end(),
            DeleteFunctor<XalanNode>());

    // Clear everything out, since we just delete everything...
    m_nodes.clear();

    // Clear out all of the navigators, except ours...
    m_navigators.erase(m_navigators.begin() + 1, m_navigators.end());

    m_navigator = &m_navigators.front();

    // Clear the node map...
    m_nodeMap.clear();

    m_indexValid = false;

    m_mappingMode = true;

    m_stringPool->clear();
}



void
XercesDocumentBridge::rebuildBridge()
{
    destroyBridge();

    buildBridgeNodes();
}



XercesBridgeNavigator&
XercesDocumentBridge::pushNavigator(bool    mappingMode) const
{
    XercesDocumentBridge* const     This =
        const_cast<XercesDocumentBridge*>(this);

    This->m_navigators.push_back(XercesBridgeNavigator(This, mappingMode));

    return This->m_navigators.back();
}



XercesDocumentTypeBridge*
XercesDocumentBridge::createBridgeNode(
            const DOM_DocumentType_Type&    theDoctype,
            IndexType                       theIndex,
            bool                            mapNode) const
{
    // This is a special case, since there is only one
    // doctype node allowed...
    assert(m_doctype == 0);
    assert(m_xercesDocument.getDoctype().isNull() == false &&
           m_xercesDocument.getDoctype() == theDoctype);

    // Create a navigator...
    XercesBridgeNavigator&  theNavigator = pushNavigator(mapNode);

    theNavigator.setIndex(theIndex);

    // Update the member variable for the new instance.
    m_doctype = new XercesDocumentTypeBridge(theDoctype, theNavigator);

    if (mapNode == true)
    {
        m_nodeMap.addAssociation(theDoctype, m_doctype);
    }

    m_nodes.push_back(m_doctype);

    return m_doctype;
}



XercesElementBridge*
XercesDocumentBridge::createBridgeNode(
            const DOM_ElementType&  theXercesNode,
            IndexType               theIndex,
            bool                    mapNode) const
{
    // Create a navigator...
    XercesBridgeNavigator&  theNavigator = pushNavigator(mapNode);

    theNavigator.setIndex(theIndex);

    XercesElementBridge* const  theBridge =
        m_elementAllocator.create(theXercesNode, theNavigator);

    if (m_doctype != 0 || mapNode == true)
    {
        // Add it to the node map, since we my need it for getting
        // elements by ID, or we're in mapping mode.
        m_nodeMap.addAssociation(theXercesNode, theBridge);
    }

    return theBridge;
}



XercesDocumentFragmentBridge*
XercesDocumentBridge::createBridgeNode(
            const DOM_DocumentFragmentType&     theXercesNode,
            IndexType                           theIndex,
            bool                                mapNode) const
{
    // Create a navigator...
    XercesBridgeNavigator&  theNavigator = pushNavigator(mapNode);

    theNavigator.setIndex(theIndex);

    XercesDocumentFragmentBridge* const     theBridge =
        new XercesDocumentFragmentBridge(theXercesNode,
                                         theNavigator);

    m_nodes.push_back(theBridge);

    if (mapNode == true)
    {
        // Add it to the node map...
        m_nodeMap.addAssociation(theXercesNode, theBridge);
    }

    return theBridge;
}



XercesTextBridge*
XercesDocumentBridge::createBridgeNode(
            const DOM_TextType&     theXercesNode,
            IndexType               theIndex,
            bool                    mapNode) const
{
    // Create a navigator...
    XercesBridgeNavigator&  theNavigator = pushNavigator(mapNode);

    theNavigator.setIndex(theIndex);

    XercesTextBridge* const     theBridge =
        m_textAllocator.create(theXercesNode, theNavigator);

    if (mapNode == true)
    {
        // Add it to the node map...
        m_nodeMap.addAssociation(theXercesNode, theBridge);
    }

    return theBridge;
}



XercesCommentBridge*
XercesDocumentBridge::createBridgeNode(
            const DOM_CommentType&  theXercesNode,
            IndexType               theIndex,
            bool                    mapNode) const
{
    // Create a navigator...
    XercesBridgeNavigator&  theNavigator = pushNavigator(mapNode);

    theNavigator.setIndex(theIndex);

    XercesCommentBridge* const  theBridge =
        new XercesCommentBridge(theXercesNode,
                                theNavigator);

    m_nodes.push_back(theBridge);

    if (mapNode == true)
    {
        // Add it to the node map...
        m_nodeMap.addAssociation(theXercesNode, theBridge);
    }

    return theBridge;
}



XercesCDATASectionBridge*
XercesDocumentBridge::createBridgeNode(
            const DOM_CDATASectionType&     theXercesNode,
            IndexType                       theIndex,
            bool                            mapNode) const
{
    // Create a navigator...
    XercesBridgeNavigator&  theNavigator = pushNavigator(mapNode);

    theNavigator.setIndex(theIndex);

    XercesCDATASectionBridge* const     theBridge =
        new XercesCDATASectionBridge(theXercesNode,
                                     theNavigator);

    m_nodes.push_back(theBridge);

    if (mapNode == true)
    {
        // Add it to the node map...
        m_nodeMap.addAssociation(theXercesNode, theBridge);
    }

    return theBridge;
}



XercesProcessingInstructionBridge*
XercesDocumentBridge::createBridgeNode(
            const DOM_ProcessingInstructionType&    theXercesNode,
            IndexType                               theIndex,
            bool                                    mapNode) const
{
    // Create a navigator...
    XercesBridgeNavigator&  theNavigator = pushNavigator(mapNode);

    theNavigator.setIndex(theIndex);

    XercesProcessingInstructionBridge* const    theBridge =
        new XercesProcessingInstructionBridge(theXercesNode,
                                              theNavigator);

    m_nodes.push_back(theBridge);

    if (mapNode == true)
    {
        // Add it to the node map...
        m_nodeMap.addAssociation(theXercesNode, theBridge);
    }

    return theBridge;
}



XercesAttrBridge*
XercesDocumentBridge::createBridgeNode(
            const DOM_AttrType&     theXercesNode,
            IndexType               theIndex,
            bool                    mapNode) const
{
    // Create a navigator...
    XercesBridgeNavigator&  theNavigator = pushNavigator(mapNode);

    theNavigator.setIndex(theIndex);

    XercesAttrBridge* const     theBridge =
        m_attributeAllocator.create(theXercesNode, theNavigator);

    // Add it to the node map -- attributes ALWAYS go in the map
    // for now...
    m_nodeMap.addAssociation(theXercesNode, theBridge);

    return theBridge;
}



XercesEntityBridge*
XercesDocumentBridge::createBridgeNode(
            const DOM_EntityType&   theXercesNode,
            IndexType               theIndex,
            bool                    mapNode) const
{
    // Create a navigator...
    XercesBridgeNavigator&  theNavigator = pushNavigator(mapNode);

    theNavigator.setIndex(theIndex);

    XercesEntityBridge* const   theBridge =
        new XercesEntityBridge(theXercesNode,
                               theNavigator);

    m_nodes.push_back(theBridge);

    // Add it to the node map -- entities ALWAYS go in the map
    // for now...
    m_nodeMap.addAssociation(theXercesNode, theBridge);

    return theBridge;
}



XercesEntityReferenceBridge*
XercesDocumentBridge::createBridgeNode(
            const DOM_EntityReferenceType&  theXercesNode,
            IndexType                       theIndex,
            bool                            mapNode) const
{
    // Create a navigator...
    XercesBridgeNavigator&  theNavigator = pushNavigator(mapNode);

    theNavigator.setIndex(theIndex);

    XercesEntityReferenceBridge* const  theBridge =
        new XercesEntityReferenceBridge(theXercesNode,
                                        theNavigator);

    m_nodes.push_back(theBridge);

    if (mapNode == true)
    {
        // Add it to the node map...
        m_nodeMap.addAssociation(theXercesNode, theBridge);
    }

    return theBridge;
}



XercesNotationBridge*
XercesDocumentBridge::createBridgeNode(
            const DOM_NotationType&     theXercesNode,
            IndexType                   theIndex,
            bool                        mapNode) const
{
    // Create a navigator...
    XercesBridgeNavigator&  theNavigator = pushNavigator(mapNode);

    theNavigator.setIndex(theIndex);

    XercesNotationBridge* const     theBridge =
        new XercesNotationBridge(theXercesNode,
                                 theNavigator);

    m_nodes.push_back(theBridge);

    // Add it to the node map -- notations ALWAYS go in the map
    // for now...
    m_nodeMap.addAssociation(theXercesNode, theBridge);

    return theBridge;
}



XalanNode*
XercesDocumentBridge::createBridgeNode(
            const DOM_NodeType&     theXercesNode,
            IndexType               theIndex,
            bool                    mapNode) const
{
    XalanNode*              theNewNode = 0;

    switch(theXercesNode.getNodeType())
    {
    case DOM_NodeType::ATTRIBUTE_NODE:
        {
            const DOM_AttrType&     theAttrNode =
                        static_cast<const DOM_AttrType&>(theXercesNode);

            theNewNode = createBridgeNode(theAttrNode, theIndex, mapNode);
            assert(theNewNode != 0);
        }
        break;

    case DOM_NodeType::CDATA_SECTION_NODE:
        {
            const DOM_CDATASectionType&     theCDATASectionNode =
                        static_cast<const DOM_CDATASectionType&>(theXercesNode);

            theNewNode = createBridgeNode(theCDATASectionNode, theIndex, mapNode);
            assert(theNewNode != 0);

        }
        break;

    case DOM_NodeType::COMMENT_NODE:
        {
            const DOM_CommentType&  theCommentNode =
                        static_cast<const DOM_CommentType&>(theXercesNode);

            theNewNode = createBridgeNode(theCommentNode, theIndex, mapNode);
            assert(theNewNode != 0);
        }
        break;

    case DOM_NodeType::DOCUMENT_FRAGMENT_NODE:
        {
            const DOM_DocumentFragmentType&     theDocumentFragmentNode =
                        static_cast<const DOM_DocumentFragmentType&>(theXercesNode);

            theNewNode = createBridgeNode(theDocumentFragmentNode, theIndex, mapNode);
            assert(theNewNode != 0);

        }
        break;

    case DOM_NodeType::ELEMENT_NODE:
        {
            const DOM_ElementType&  theElementNode =
                        static_cast<const DOM_ElementType&>(theXercesNode);

            theNewNode = createBridgeNode(theElementNode, theIndex, mapNode);
            assert(theNewNode != 0);
        }
        break;

    case DOM_NodeType::ENTITY_NODE:
        {
            const DOM_EntityType&   theEntityNode =
                        static_cast<const DOM_EntityType&>(theXercesNode);

            theNewNode = createBridgeNode(theEntityNode, theIndex, mapNode);
            assert(theNewNode != 0);
        }
        break;

    case DOM_NodeType::ENTITY_REFERENCE_NODE:
        {
            const DOM_EntityReferenceType&  theEntityReferenceNode =
                        static_cast<const DOM_EntityReferenceType&>(theXercesNode);

            theNewNode = createBridgeNode(theEntityReferenceNode, theIndex, mapNode);
            assert(theNewNode != 0);
        }
        break;

    case DOM_NodeType::NOTATION_NODE:
        {
            const DOM_NotationType&     theNotationNode =
                        static_cast<const DOM_NotationType&>(theXercesNode);

            theNewNode = createBridgeNode(theNotationNode, theIndex, mapNode);
            assert(theNewNode != 0);
        }
        break;

    case DOM_NodeType::PROCESSING_INSTRUCTION_NODE:
        {
            const DOM_ProcessingInstructionType&    thePINode =
                        static_cast<const DOM_ProcessingInstructionType&>(theXercesNode);

            theNewNode = createBridgeNode(thePINode, theIndex, mapNode);
            assert(theNewNode != 0);
        }
        break;

    case DOM_NodeType::TEXT_NODE:
        {
            const DOM_TextType&     theTextNode =
                        static_cast<const DOM_TextType&>(theXercesNode);

            theNewNode = createBridgeNode(theTextNode, theIndex, mapNode);
            assert(theNewNode != 0);
        }
        break;

    case DOM_NodeType::DOCUMENT_TYPE_NODE:
        {
            const DOM_DocumentType_Type&    theDoctypeNode =
                        static_cast<const DOM_DocumentType_Type&>(theXercesNode);

            theNewNode = createBridgeNode(theDoctypeNode, theIndex, mapNode);

            assert(theNewNode != 0);
        }
        break;

    default:
        assert(false);
        break;
    }

    return theNewNode;
}



void
XercesDocumentBridge::destroyNode(XalanNode*    theNode)
{
    XALAN_USING_STD(find)

    const NodeVectorType::iterator  i =
        find(m_nodes.begin(), m_nodes.end(), theNode);

    if (i == m_nodes.end())
    {
        // Doesn't belong to this doc, so throw...
        throw XercesDOMException(XercesDOMException::WRONG_DOCUMENT_ERR);
    }
    else
    {
        // Delete the node...
        delete *i;

        // Erase it from the map...
        m_nodes.erase(i);
    }
}



XalanNode*
XercesDocumentBridge::internalCloneNode(
            const XalanNode*        theXalanNode,
            const DOM_NodeType&     theXercesNode,
            bool                    deep)
{
    XalanNode*  theNewNode = 0;

    if (theXalanNode->getOwnerDocument() != this)
    {
        throw XercesDOMException(XercesDOMException::WRONG_DOCUMENT_ERR);
    }

    try
    {
        const DOM_NodeType  theNewXercesNode = 
            theXercesNode.cloneNode(deep);

        theNewNode = createBridgeNode(theNewXercesNode, 0, true);
    }
    catch(const DOM_DOMExceptionType&   theException)
    {
        throw XercesDOMException(theException);
    }

    return theNewNode;
}



const XalanDOMString&
XercesDocumentBridge::getNodeName() const
{
    assert(m_navigator != 0);

    return m_navigator->getPooledString(m_xercesDocument.getNodeName());
}



const XalanDOMString&
XercesDocumentBridge::getNodeValue() const
{
    assert(m_navigator != 0);

    return m_navigator->getPooledString(m_xercesDocument.getNodeValue());
}



XercesDocumentBridge::NodeType
XercesDocumentBridge::getNodeType() const
{
    return DOCUMENT_NODE;
}



XalanNode*
XercesDocumentBridge::getParentNode() const
{
    return 0;
}



const XalanNodeList*
XercesDocumentBridge::getChildNodes() const
{
    return &m_children;
}



XalanNode*
XercesDocumentBridge::getFirstChild() const
{
    assert(m_navigator != 0);

    return m_navigator->getFirstChild(m_xercesDocument);
}



XalanNode*
XercesDocumentBridge::getLastChild() const
{
    assert(m_navigator != 0);

    return m_navigator->getLastChild(m_xercesDocument);
}



XalanNode*
XercesDocumentBridge::getPreviousSibling() const
{
    return 0;
}



XalanNode*
XercesDocumentBridge::getNextSibling() const
{
    return 0;
}



const XalanNamedNodeMap*
XercesDocumentBridge::getAttributes() const
{
    return 0;
}



XalanDocument*
XercesDocumentBridge::getOwnerDocument() const
{
    return 0;
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XercesDocumentBridge*
#endif
XercesDocumentBridge::cloneNode(bool    deep) const
{
    XercesDocumentBridge*   theBridge = 0;

    try
    {
        const DOM_NodeType  theNewDocument = m_xercesDocument.cloneNode(deep);

        theBridge =
            new XercesDocumentBridge(static_cast<const DOM_Document_Type&>(theNewDocument));
    }
    catch(const DOM_DOMExceptionType&   theException)
    {
        throw XercesDOMException(theException);
    }

    return theBridge;
}



XalanNode*
XercesDocumentBridge::insertBefore(
            XalanNode*  newChild,
            XalanNode*  refChild)
{
    assert(m_navigator != 0);

    return m_navigator->insertBefore(m_xercesDocument, newChild, refChild);
}



XalanNode*
XercesDocumentBridge::replaceChild(
            XalanNode*  newChild,
            XalanNode*  oldChild)
{
    assert(m_navigator != 0);

    return m_navigator->replaceChild(m_xercesDocument, newChild, oldChild);
}



XalanNode*
XercesDocumentBridge::removeChild(XalanNode*     oldChild)
{
    assert(m_navigator != 0);

    return m_navigator->removeChild(m_xercesDocument, oldChild);
}



XalanNode*
XercesDocumentBridge::appendChild(XalanNode*    newChild)
{
    assert(m_navigator != 0);

    return m_navigator->appendChild(m_xercesDocument, newChild);
}



bool
XercesDocumentBridge::hasChildNodes() const
{
    return m_xercesDocument.hasChildNodes();
}



void
XercesDocumentBridge::setNodeValue(const XalanDOMString&    nodeValue)
{
    XercesBridgeHelper::setNodeValue(m_xercesDocument, nodeValue);
}



void
XercesDocumentBridge::normalize()
{
    XercesBridgeHelper::normalize(m_xercesDocument);
}


bool
XercesDocumentBridge::isSupported(
            const XalanDOMString&   feature,
            const XalanDOMString&   version) const
{
    return m_xercesDocument.isSupported(
                XercesBridgeHelper::XalanDOMStringToXercesDOMString(feature),
                XercesBridgeHelper::XalanDOMStringToXercesDOMString(version));
}



const XalanDOMString&
XercesDocumentBridge::getNamespaceURI() const
{
    assert(m_navigator != 0);

    return m_navigator->getPooledString(m_xercesDocument.getNamespaceURI());
}



const XalanDOMString&
XercesDocumentBridge::getPrefix() const
{
    assert(m_navigator != 0);

    return m_navigator->getPooledString(m_xercesDocument.getPrefix());
}



const XalanDOMString&
XercesDocumentBridge::getLocalName() const
{
    assert(m_navigator != 0);

    return m_navigator->getPooledString(m_xercesDocument.getLocalName());
}



void
XercesDocumentBridge::setPrefix(const XalanDOMString&   prefix)
{
    XercesBridgeHelper::setPrefix(m_xercesDocument, prefix);
}



bool
XercesDocumentBridge::isIndexed() const
{
    return m_indexValid;
}



XercesDocumentBridge::IndexType
XercesDocumentBridge::getIndex() const
{
    assert(m_navigator != 0);
    assert(m_navigator->getIndex() == 1);

    return m_navigator->getIndex();
}



XalanElement*
XercesDocumentBridge::createElement(const XalanDOMString&   tagName)
{
    XalanElement*   theBridgeNode = 0;

    try
    {
        const DOM_ElementType   theXercesNode =
            m_xercesDocument.createElement(tagName.c_str());
        assert(theXercesNode.isNull() == false);

        theBridgeNode = createBridgeNode(theXercesNode, 0, true);
        assert(mapNode(theXercesNode) == theBridgeNode);
    }
    catch(const DOM_DOMExceptionType&   theException)
    {
        throw XercesDOMException(theException);
    }

    return theBridgeNode;
}



XalanDocumentFragment*
XercesDocumentBridge::createDocumentFragment()
{
    XalanDocumentFragment*  theBridgeNode = 0;

    try
    {
        const DOM_DocumentFragmentType  theXercesNode =
            m_xercesDocument.createDocumentFragment();
        assert(theXercesNode.isNull() == false);

        theBridgeNode = createBridgeNode(theXercesNode, 0, true);
        assert(mapNode(theXercesNode) == theBridgeNode);
    }
    catch(const DOM_DOMExceptionType&   theException)
    {
        throw XercesDOMException(theException);
    }

    return theBridgeNode;
}



XalanText*
XercesDocumentBridge::createTextNode(const XalanDOMString&  data)
{
    XalanText*  theBridgeNode = 0;

    try
    {
        const DOM_TextType  theXercesNode =
            m_xercesDocument.createTextNode(data.c_str());
        assert(theXercesNode.isNull() == false);

        theBridgeNode = createBridgeNode(theXercesNode, 0, true);
        assert(mapNode(theXercesNode) == theBridgeNode);
    }
    catch(const DOM_DOMExceptionType&   theException)
    {
        throw XercesDOMException(theException);
    }

    return theBridgeNode;
}



XalanComment*
XercesDocumentBridge::createComment(const XalanDOMString&   data)
{
    XalanComment*   theBridgeNode = 0;

    try
    {
        const DOM_CommentType   theXercesNode =
            m_xercesDocument.createComment(data.c_str());
        assert(theXercesNode.isNull() == false);

        theBridgeNode = createBridgeNode(theXercesNode, 0, true);
        assert(mapNode(theXercesNode) == theBridgeNode);
    }
    catch(const DOM_DOMExceptionType&   theException)
    {
        throw XercesDOMException(theException);
    }

    return theBridgeNode;
}



XalanCDATASection*
XercesDocumentBridge::createCDATASection(const XalanDOMString&  data)
{
    XalanCDATASection*  theBridgeNode = 0;

    try
    {
        const DOM_CDATASectionType  theXercesNode =
            m_xercesDocument.createCDATASection(data.c_str());
        assert(theXercesNode.isNull() == false);

        theBridgeNode = createBridgeNode(theXercesNode, 0, true);
        assert(mapNode(theXercesNode) == theBridgeNode);
    }
    catch(const DOM_DOMExceptionType&   theException)
    {
        throw XercesDOMException(theException);
    }

    return theBridgeNode;
}



XalanProcessingInstruction*
XercesDocumentBridge::createProcessingInstruction(
            const XalanDOMString&   target,
            const XalanDOMString&   data)
{
    XalanProcessingInstruction* theBridgeNode = 0;

    try
    {
        const DOM_ProcessingInstructionType     theXercesNode =
            m_xercesDocument.createProcessingInstruction(target.c_str(), data.c_str());
        assert(theXercesNode.isNull() == false);

        theBridgeNode = createBridgeNode(theXercesNode, 0, true);
        assert(mapNode(theXercesNode) == theBridgeNode);
    }
    catch(const DOM_DOMExceptionType&   theException)
    {
        throw XercesDOMException(theException);
    }

    return theBridgeNode;
}



XalanAttr*
XercesDocumentBridge::createAttribute(const XalanDOMString&     name)
{
    XalanAttr*  theBridgeNode = 0;

    try
    {
        const DOM_AttrType  theXercesNode =
            m_xercesDocument.createAttribute(name.c_str());
        assert(theXercesNode.isNull() == false);

        theBridgeNode = createBridgeNode(theXercesNode, 0, true);
        assert(mapNode(theXercesNode) == theBridgeNode);
    }
    catch(const DOM_DOMExceptionType&   theException)
    {
        throw XercesDOMException(theException);
    }

    return theBridgeNode;
}



XalanEntityReference*
XercesDocumentBridge::createEntityReference(const XalanDOMString&   name)
{
    XalanEntityReference*   theBridgeNode = 0;

    try
    {
        const DOM_EntityReferenceType   theXercesNode =
            m_xercesDocument.createEntityReference(name.c_str());
        assert(theXercesNode.isNull() == false);

        theBridgeNode = createBridgeNode(theXercesNode, 0, true);
        assert(mapNode(theXercesNode) == theBridgeNode);
    }
    catch(const DOM_DOMExceptionType&   theException)
    {
        throw XercesDOMException(theException);
    }

    return theBridgeNode;
}



XalanDocumentType*
XercesDocumentBridge::getDoctype() const
{
    return m_doctype;
}




XalanDOMImplementation*
XercesDocumentBridge::getImplementation() const
{
    return m_domImplementation.get();
}



XalanElement*
XercesDocumentBridge::getDocumentElement() const
{
    // If we're working in mapping mode, m_documentElement will
    // be null, so we'll have to map the node...
    return m_documentElement != 0 ? m_documentElement : mapNode(m_xercesDocument.getDocumentElement());
}



XalanNodeList*
XercesDocumentBridge::getElementsByTagName(const XalanDOMString&    /* tagname */) const
{
    // Not supported
    throw XercesDOMException(XercesDOMException::NOT_SUPPORTED_ERR);

    // Dummy return value...
    return 0;
}



XalanNode*
XercesDocumentBridge::importNode(
            XalanNode*  /* importedNode */,
            bool        /* deep */)
{
    throw XercesDOMException(XercesDOMException::NOT_SUPPORTED_ERR);

    // Dummy return value...
    return 0;
}



XalanElement*
XercesDocumentBridge::createElementNS(
            const XalanDOMString&   namespaceURI,
            const XalanDOMString&   qualifiedName)
{
    XalanElement*   theBridgeNode = 0;

    try
    {
        const DOM_ElementType   theXercesNode =
            m_xercesDocument.createElementNS(namespaceURI.c_str(), qualifiedName.c_str());
        assert(theXercesNode.isNull() == false);

        theBridgeNode = createBridgeNode(theXercesNode, 0, true);
        assert(mapNode(theXercesNode) == theBridgeNode);
    }
    catch(const DOM_DOMExceptionType&   theException)
    {
        throw XercesDOMException(theException);
    }

    return theBridgeNode;
}



XalanAttr*
XercesDocumentBridge::createAttributeNS(
            const XalanDOMString&   namespaceURI,
            const XalanDOMString&   qualifiedName)
{
    XalanAttr*  theBridgeNode = 0;

    try
    {
        const DOM_AttrType  theXercesNode =
            m_xercesDocument.createAttributeNS(namespaceURI.c_str(), qualifiedName.c_str());
        assert(theXercesNode.isNull() == false);

        theBridgeNode = createBridgeNode(theXercesNode, 0, true);
        assert(mapNode(theXercesNode) == theBridgeNode);
    }
    catch(const DOM_DOMExceptionType&   theException)
    {
        throw XercesDOMException(theException);
    }

    return theBridgeNode;
}



XalanNodeList*
XercesDocumentBridge::getElementsByTagNameNS(
            const XalanDOMString&   /* namespaceURI */,
            const XalanDOMString&   /* localName */) const
{
    throw XercesDOMException(XercesDOMException::NOT_SUPPORTED_ERR);

    // Dummy return value...
    return 0;
}



XalanElement*
XercesDocumentBridge::getElementById(const XalanDOMString&  elementId) const
{
    // $$$ ToDo: This is because DOM_Document::getElementById() is not
    // const...
    const DOM_NodeType  theXercesNode(m_xercesDocument.getElementById(elementId.c_str()));

    return theXercesNode.isNull() == true ? 0 :
                static_cast<XalanElement*>(mapNode(theXercesNode));
}



void
XercesDocumentBridge::buildBridgeNodes()
{
    const DOM_NodeType  theStartChild = m_xercesDocument.getFirstChild();

    if (theStartChild.isNull() == false)
    {
        assert(m_navigator != 0);
        assert(m_navigators.size() == 1);

        m_navigator->setIndex(1);
        m_navigator->setFirstChild(0);

        BuildBridgeTreeWalker   theTreeWalker(
                this,
                &m_navigators.back(),
                m_navigators,
                2);

        theTreeWalker.traverse(theStartChild, m_xercesDocument);
    }

    // OK, now set m_documentElement...
    XalanNode*  theChild = m_navigator->getFirstChild();

    while(theChild != 0 && theChild->getNodeType() != XalanNode::ELEMENT_NODE)
    {
        theChild = theChild->getNextSibling();
    }

    m_documentElement = static_cast<XalanElement*>(theChild);

    m_indexValid = true;

    m_mappingMode = false;
}



XercesDocumentBridge::BuildBridgeTreeWalker::BuildBridgeTreeWalker(
            XercesDocumentBridge*       theDocument,
            XercesBridgeNavigator*      theDocumentNavigator,
            NavigatorBridgeVectorType&  theNavigators,
            IndexType                   theStartIndex) :
    m_document(theDocument),
    m_navigators(theNavigators),
    m_currentIndex(theStartIndex),
    m_parentNavigatorStack(),
    m_siblingNavigatorStack()
{
    assert(theDocument != 0 && theDocumentNavigator != 0);

    // Reserve some space...
    m_parentNavigatorStack.reserve(100);
    m_parentNavigatorStack.reserve(100);

    // The document navigator is the last navigator on the stack...
    m_parentNavigatorStack.push_back(NavigatorStackEntryType(theDocumentNavigator, theDocument));

    // There is no previous sibling...
    m_siblingNavigatorStack.push_back(NavigatorStackEntryType(0, 0));
}



XercesDocumentBridge::BuildBridgeTreeWalker::~BuildBridgeTreeWalker()
{
}



void
XercesDocumentBridge::BuildBridgeTreeWalker::startNode(const DOM_NodeType&  node)
{
    XalanNode* const    theBridgeNode = m_document->createBridgeNode(node, m_currentIndex, false);

    XercesBridgeNavigator&  theCurrentNodeNavigator = m_navigators.back();

    assert(m_parentNavigatorStack.empty() == false);
    assert(m_siblingNavigatorStack.empty() == false);

    // Get the two navigators...
    NavigatorStackEntryType&    theParentEntry = m_parentNavigatorStack.back();
    NavigatorStackEntryType&    theSiblingEntry = m_siblingNavigatorStack.back();

    theCurrentNodeNavigator.setParentNode(theParentEntry.m_node);

    // If the first child has not been set, then set it
    // now...
    if (theParentEntry.m_navigator->getFirstChild() == 0)
    {
        assert(theSiblingEntry.m_node == 0);

        theParentEntry.m_navigator->setFirstChild(theBridgeNode);
    }

    // Always set the last child...
    theParentEntry.m_navigator->setLastChild(theBridgeNode);

    theCurrentNodeNavigator.setPreviousSibling(theSiblingEntry.m_node);

    if (theSiblingEntry.m_navigator != 0)
    {
        theSiblingEntry.m_navigator->setNextSibling(theBridgeNode);
    }

    // Build an entry for the stacks...
    const NavigatorStackEntryType   theCurrentEntry(&theCurrentNodeNavigator, theBridgeNode);

    // My child nodes will now be visited, so push the current
    // context on the parent stack...
    m_parentNavigatorStack.push_back(theCurrentEntry);

    // My siblings will also need to know about me as well...
    m_siblingNavigatorStack.push_back(theCurrentEntry);

    // This will serve to mark the sibling context for my first child,
    // since it has no previous sibling.  This will be popped off
    // when endNode() is called.
    m_siblingNavigatorStack.push_back(NavigatorStackEntryType(0, 0));

    // Finally, increment the index counter...
    ++m_currentIndex;

    const short     theType = node.getNodeType();

    if (theType == DOM_NodeType::DOCUMENT_TYPE_NODE)
    {
        // Special case for doctype -- we have to build its entities...
        const DOM_DocumentType_Type&    theDoctype =
            static_cast<const DOM_DocumentType_Type&>(node);

        const DOM_NamedNodeMapType  theEntities =
            theDoctype.getEntities();

        const unsigned int  theLength =
            theEntities.getLength();

        for (unsigned int i = 0; i < theLength; ++i)
        {
            // Build it, but don't index it...
            m_document->createBridgeNode(theEntities.item(i), m_currentIndex++, true);
        }
    }
    else if (theType == DOM_NodeType::ELEMENT_NODE)
    {
        // Special case for element nodes -- we have to build the attributes...
        const DOM_ElementType&  theElement =
            static_cast<const DOM_ElementType&>(node);

        const DOM_NamedNodeMapType  theAttributes =
            theElement.getAttributes();

        const unsigned int  theLength =
            theAttributes.getLength();

        XercesBridgeNavigator*  thePreviousAttrNavigator = 0;
        XalanNode*              thePreviousAttr = 0;

        for (unsigned int i = 0; i < theLength; ++i)
        {
            // Get the attribute from the node map...
            const DOM_NodeType  theAttr = theAttributes.item(i);
            assert(theAttr.isNull() == false);

            // Create a bridge node.
            XalanNode* const    theCurrentAttr =
                m_document->createBridgeNode(theAttr, m_currentIndex, false);
            assert(theCurrentAttr != 0);

            // Get the attribute node's navigator...
            XercesBridgeNavigator&  theCurrentAttrNavigator =
                m_navigators.back();

            // Set the parent node...
            theCurrentAttrNavigator.setParentNode(theBridgeNode);

            if (thePreviousAttr != 0)
            {
                assert(thePreviousAttrNavigator != 0);

                // Link in the previous attribute...
                theCurrentAttrNavigator.setPreviousSibling(thePreviousAttr);

                thePreviousAttrNavigator->setNextSibling(theCurrentAttr);
            }

            // Update the pointers so they point to this attribute...
            thePreviousAttr = theCurrentAttr;
            thePreviousAttrNavigator = &theCurrentAttrNavigator;

            // Finally, increment the index...
            ++m_currentIndex;
        }
    }
}



void
XercesDocumentBridge::BuildBridgeTreeWalker::endNode(const DOM_NodeType&    /* node */)
{
    assert(m_parentNavigatorStack.empty() == false);
    assert(m_siblingNavigatorStack.empty() == false);

    // I have to pop my entry, since my children are finished...
    m_parentNavigatorStack.pop_back();

    // Pop any sibling navigators my child pushed...
    while(m_siblingNavigatorStack.back().m_navigator != 0)
    {
        assert(m_siblingNavigatorStack.back().m_node != 0);

        m_siblingNavigatorStack.pop_back();
    }

    // There must be a context marker...
    assert(m_siblingNavigatorStack.back().m_navigator == 0 &&
           m_siblingNavigatorStack.back().m_node == 0);

    // Pop the context marker...
    m_siblingNavigatorStack.pop_back();
}



const XalanDOMString&
XercesDocumentBridge::getPooledString(const XalanDOMString&     theString) const
{
    return m_stringPool->get(theString);
}



const XalanDOMString&
XercesDocumentBridge::getPooledString(
            const XalanDOMChar*         theString,
            XalanDOMString::size_type   theLength) const
{
    return m_stringPool->get(theString, theLength);
}



XALAN_CPP_NAMESPACE_END


#endif //XALAN_BUILD_DEPRECATED_DOM_BRIDGE 


