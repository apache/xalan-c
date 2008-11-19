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
#include "XercesDocumentWrapper.hpp"



#include <algorithm>
#include <cassert>



#include <xercesc/dom/DOMAttr.hpp>
#include <xercesc/dom/DOMCDATASection.hpp>
#include <xercesc/dom/DOMComment.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMDocumentType.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMEntity.hpp>
#include <xercesc/dom/DOMEntityReference.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/dom/DOMNotation.hpp>
#include <xercesc/dom/DOMProcessingInstruction.hpp>
#include <xercesc/dom/DOMText.hpp>



#include <xalanc/Include/STLHelper.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include <xalanc/XalanDOM/XalanDocumentType.hpp>
#include <xalanc/XalanDOM/XalanElement.hpp>



#include "XercesAttrWrapper.hpp"
#include "XercesWrapperHelper.hpp"
#include "XercesCommentWrapper.hpp"
#include "XercesCDATASectionWrapper.hpp"
#include "XercesDOMWrapperException.hpp"
#include "XercesDOMImplementationWrapper.hpp"
#include "XercesDocumentTypeWrapper.hpp"
#include "XercesElementWrapper.hpp"
#include "XercesEntityWrapper.hpp"
#include "XercesEntityReferenceWrapper.hpp"
#include "XercesLiaisonXalanDOMStringPool.hpp"
#include "XercesNodeListWrapper.hpp"
#include "XercesNotationWrapper.hpp"
#include "XercesProcessingInstructionWrapper.hpp"
#include "XercesTextWrapper.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XercesDocumentWrapper::XercesDocumentWrapper(
            MemoryManager&              theManager,
            const DOMDocument_Type*     theXercesDocument,
            bool                        threadSafe,
            bool                        buildWrapper,
            bool                        buildMaps) :
    XalanDocument(),
    m_xercesDocument(theXercesDocument),
    m_documentElement(0),
    m_nodeMap(theManager),
    m_navigatorAllocator(theManager, 25),
    m_navigator(0),
    m_children(theXercesDocument->getChildNodes(),
               *m_navigator),
    m_nodes(theManager),
    m_doctype(0),
    m_mappingMode(threadSafe == true ? false : !buildWrapper),
    m_indexValid(false),
    m_buildMaps(m_mappingMode == true ? true : buildMaps),
    m_elementAllocator(theManager, 25),
    m_textAllocator(theManager, 25),
    m_attributeAllocator(theManager, 25),
    m_stringPool(theManager, threadSafe == true ? XercesLiaisonXalanDOMStringPool::create(theManager) : XalanDOMStringPool::create(theManager))
{
    assert(theXercesDocument != 0);

    m_navigator = m_navigatorAllocator.create(this);

    if (m_mappingMode == false)
    {
        // The document index is always 1...
        m_navigator->setIndex(1);

        // OK, let's build the nodes.  This makes things
        // thread-safe, so the document can be shared...
        buildWrapperNodes();
    }
    else
    {
        m_nodeMap.addAssociation(theXercesDocument, this);
    }
}



XercesDocumentWrapper*
XercesDocumentWrapper::create( 
            MemoryManager&              theManager,
            const DOMDocument_Type*     theXercesDocument,
            bool                        threadSafe,
            bool                        buildWrapper,
            bool                        buildMaps)
{
    typedef XercesDocumentWrapper   ThisType;

    XalanAllocationGuard    theGuard(theManager, theManager.allocate(sizeof(ThisType)));

    ThisType* const     theResult =
        new (theGuard.get()) ThisType(
                                theManager,
                                theXercesDocument,
                                threadSafe,
                                buildWrapper,
                                buildMaps);

    theGuard.release();

    return theResult;
}



XercesDocumentWrapper::~XercesDocumentWrapper()
{
    destroyWrapper();
}



XalanNode*
XercesDocumentWrapper::mapNode(const DOMNodeType*   theXercesNode) const
{
    XalanNode*  theXalanNode = 0;

    if (theXercesNode != 0)
    {
        theXalanNode = m_nodeMap.getNode(theXercesNode);

        if (theXalanNode == 0)
        {
            if (theXercesNode != m_xercesDocument &&
                    theXercesNode->getOwnerDocument() != m_xercesDocument &&
                theXercesNode->getParentNode() != static_cast<const DOMNodeType*>(m_xercesDocument))
            {
                throw XercesDOMWrapperException(XercesDOMWrapperException::WRONG_DOCUMENT_ERR);
            }
            else if (m_mappingMode == true)
            {
                // OK, we haven't yet created a wrapper, so go ahead and
                // create one.
                theXalanNode = createWrapperNode(theXercesNode, 0, true);
                assert(theXalanNode != 0);
            }
        }
    }

    return theXalanNode;
}



const DOMNodeType*
XercesDocumentWrapper::mapNode(XalanNode*   theXalanNode) const
{
    if (static_cast<const XalanNode*>(this) == theXalanNode)
    {
        return m_xercesDocument;
    }
    else if (theXalanNode == 0 ||
             static_cast<const XalanNode*>(this) != theXalanNode->getOwnerDocument())
    {
        throw XercesDOMWrapperException(XercesDOMWrapperException::WRONG_DOCUMENT_ERR);
    }
    else
    {
        switch(theXalanNode->getNodeType())
        {
        case XalanNode::ATTRIBUTE_NODE:
            return static_cast<const XercesAttrWrapper*>(theXalanNode)->getXercesNode();
            break;

        case XalanNode::CDATA_SECTION_NODE:
            return static_cast<const XercesCDATASectionWrapper*>(theXalanNode)->getXercesNode();
            break;

        case XalanNode::COMMENT_NODE:
            return static_cast<const XercesCommentWrapper*>(theXalanNode)->getXercesNode();
            break;

        case XalanNode::DOCUMENT_FRAGMENT_NODE:
            throw XercesDOMWrapperException(XercesDOMWrapperException::NOT_SUPPORTED_ERR);
            break;

        case XalanNode::ELEMENT_NODE:
            return static_cast<const XercesElementWrapper*>(theXalanNode)->getXercesNode();
            break;

        case XalanNode::ENTITY_NODE:
            return static_cast<const XercesEntityWrapper*>(theXalanNode)->getXercesNode();
            break;

        case XalanNode::ENTITY_REFERENCE_NODE:
            return static_cast<const XercesEntityReferenceWrapper*>(theXalanNode)->getXercesNode();
            break;

        case XalanNode::NOTATION_NODE:
            return static_cast<const XercesNotationWrapper*>(theXalanNode)->getXercesNode();
            break;

        case XalanNode::PROCESSING_INSTRUCTION_NODE:
            return static_cast<const XercesProcessingInstructionWrapper*>(theXalanNode)->getXercesNode();
            break;

        case XalanNode::TEXT_NODE:
            return static_cast<const XercesTextWrapper*>(theXalanNode)->getXercesNode();
            break;

        case XalanNode::DOCUMENT_TYPE_NODE:
            return static_cast<const XercesDocumentTypeWrapper*>(theXalanNode)->getXercesNode();
            break;

        default:
            assert(false);
            break;
        }

        return 0;
    }
}



XalanAttr*
XercesDocumentWrapper::mapNode(const DOMAttrType*   theXercesNode) const
{
    return static_cast<XalanAttr*>(mapNode(static_cast<const DOMNodeType*>(theXercesNode)));
}



XalanElement*
XercesDocumentWrapper::mapNode(const DOMElementType*    theXercesNode) const
{
    return static_cast<XercesElementWrapper*>(mapNode(static_cast<const DOMNodeType*>(theXercesNode)));
}



void
XercesDocumentWrapper::destroyWrapper()
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
            DeleteFunctor<XalanNode>(m_nodes.getMemoryManager()));

    // Clear everything out, since we just delete everything...
    m_nodes.clear();

    // Clear out all of the navigators, except ours...
    m_navigatorAllocator.reset();

    m_navigator = m_navigatorAllocator.create(this);

    // Clear the node map...
    m_nodeMap.clear();

    m_indexValid = false;

    m_mappingMode = true;

    m_stringPool->clear();
}



void
XercesDocumentWrapper::rebuildWrapper()
{
    destroyWrapper();

    buildWrapperNodes();
}



XercesWrapperNavigator&
XercesDocumentWrapper::createNavigator() const
{
    return *m_navigatorAllocator.create(const_cast<XercesDocumentWrapper*>(this));
}



XercesDocumentTypeWrapper*
XercesDocumentWrapper::createWrapperNode(
            const DOMDocumentType_Type*     theDoctype,
            IndexType                       theIndex,
            bool                            mapNode,
            XercesWrapperNavigator**        theWrapperNodeNavigator) const
{
    // This is a special case, since there is only one
    // doctype node allowed...
    assert(m_doctype == 0);
    assert(m_xercesDocument->getDoctype() != 0 &&
           m_xercesDocument->getDoctype() == theDoctype);

    // Create a navigator...
    XercesWrapperNavigator& theNavigator = createNavigator();

    theNavigator.setIndex(theIndex);

    // Update the member variable for the new instance.
    m_doctype =  XercesDocumentTypeWrapper::create(getMemoryManager(), theDoctype, theNavigator);

    if (mapNode == true)
    {
        m_nodeMap.addAssociation(theDoctype, m_doctype);
    }

    m_nodes.push_back(m_doctype);

    if (theWrapperNodeNavigator != 0)
    {
        *theWrapperNodeNavigator = &theNavigator;
    }

    return m_doctype;
}



XercesElementWrapper*
XercesDocumentWrapper::createWrapperNode(
            const DOMElementType*       theXercesNode,
            IndexType                   theIndex,
            bool                        mapNode,
            XercesWrapperNavigator**    theWrapperNodeNavigator) const
{
    // Create a navigator...
    XercesWrapperNavigator& theNavigator = createNavigator();

    theNavigator.setIndex(theIndex);

    XercesElementWrapper* const theWrapper =
        m_elementAllocator.create(theXercesNode, theNavigator);

    if (m_doctype != 0 || mapNode == true)
    {
        // Add it to the node map, since we my need it for getting
        // elements by ID, or we're in mapping mode.
        m_nodeMap.addAssociation(theXercesNode, theWrapper);
    }

    if (theWrapperNodeNavigator != 0)
    {
        *theWrapperNodeNavigator = &theNavigator;
    }

    return theWrapper;
}



XercesTextWrapper*
XercesDocumentWrapper::createWrapperNode(
            const DOMTextType*          theXercesNode,
            IndexType                   theIndex,
            bool                        mapNode,
            XercesWrapperNavigator**    theWrapperNodeNavigator) const
{
    // Create a navigator...
    XercesWrapperNavigator& theNavigator = createNavigator();

    theNavigator.setIndex(theIndex);

    XercesTextWrapper* const        theWrapper =
        m_textAllocator.create(theXercesNode, theNavigator);

    if (mapNode == true)
    {
        // Add it to the node map...
        m_nodeMap.addAssociation(theXercesNode, theWrapper);
    }

    if (theWrapperNodeNavigator != 0)
    {
        *theWrapperNodeNavigator = &theNavigator;
    }

    return theWrapper;
}



XercesCommentWrapper*
XercesDocumentWrapper::createWrapperNode(
            const DOMCommentType*       theXercesNode,
            IndexType                   theIndex,
            bool                        mapNode,
            XercesWrapperNavigator**    theWrapperNodeNavigator) const
{
    // Create a navigator...
    XercesWrapperNavigator& theNavigator = createNavigator();

    theNavigator.setIndex(theIndex);

    XercesCommentWrapper* const theWrapper =
        XercesCommentWrapper::create(
                                getMemoryManager(),
                                theXercesNode,
                                theNavigator);

    m_nodes.push_back(theWrapper);

    if (mapNode == true)
    {
        // Add it to the node map...
        m_nodeMap.addAssociation(theXercesNode, theWrapper);
    }

    if (theWrapperNodeNavigator != 0)
    {
        *theWrapperNodeNavigator = &theNavigator;
    }

    return theWrapper;
}



XercesCDATASectionWrapper*
XercesDocumentWrapper::createWrapperNode(
            const DOMCDATASectionType*  theXercesNode,
            IndexType                   theIndex,
            bool                        mapNode,
            XercesWrapperNavigator**    theWrapperNodeNavigator) const
{
    // Create a navigator...
    XercesWrapperNavigator& theNavigator = createNavigator();

    theNavigator.setIndex(theIndex);

    XercesCDATASectionWrapper* const        theWrapper =
        XercesCDATASectionWrapper::create(
                                     getMemoryManager(),
                                     theXercesNode,
                                     theNavigator);

    m_nodes.push_back(theWrapper);

    if (mapNode == true)
    {
        // Add it to the node map...
        m_nodeMap.addAssociation(theXercesNode, theWrapper);
    }

    if (theWrapperNodeNavigator != 0)
    {
        *theWrapperNodeNavigator = &theNavigator;
    }

    return theWrapper;
}



XercesProcessingInstructionWrapper*
XercesDocumentWrapper::createWrapperNode(
            const DOMProcessingInstructionType*     theXercesNode,
            IndexType                               theIndex,
            bool                                    mapNode,
            XercesWrapperNavigator**                theWrapperNodeNavigator) const
{
    // Create a navigator...
    XercesWrapperNavigator& theNavigator = createNavigator();

    theNavigator.setIndex(theIndex);

    XercesProcessingInstructionWrapper* const   theWrapper =
        XercesProcessingInstructionWrapper::create(
                                              getMemoryManager(),
                                              theXercesNode,
                                              theNavigator);

    m_nodes.push_back(theWrapper);

    if (mapNode == true)
    {
        // Add it to the node map...
        m_nodeMap.addAssociation(theXercesNode, theWrapper);
    }

    if (theWrapperNodeNavigator != 0)
    {
        *theWrapperNodeNavigator = &theNavigator;
    }

    return theWrapper;
}



XercesAttrWrapper*
XercesDocumentWrapper::createWrapperNode(
            const DOMAttrType*          theXercesNode,
            IndexType                   theIndex,
            bool                        /* mapNode */,
            XercesWrapperNavigator**    theWrapperNodeNavigator) const
{
    // Create a navigator...
    XercesWrapperNavigator& theNavigator = createNavigator();

    theNavigator.setIndex(theIndex);

    XercesAttrWrapper* const        theWrapper =
        m_attributeAllocator.create(theXercesNode, theNavigator);

    // Add it to the node map -- attributes ALWAYS go in the map
    // for now...
    m_nodeMap.addAssociation(theXercesNode, theWrapper);

    if (theWrapperNodeNavigator != 0)
    {
        *theWrapperNodeNavigator = &theNavigator;
    }

    return theWrapper;
}



XercesEntityWrapper*
XercesDocumentWrapper::createWrapperNode(
            const DOMEntityType*        theXercesNode,
            IndexType                   theIndex,
            bool                        /* mapNode */,
            XercesWrapperNavigator**    theWrapperNodeNavigator) const
{
    // Create a navigator...
    XercesWrapperNavigator& theNavigator = createNavigator();

    theNavigator.setIndex(theIndex);

    XercesEntityWrapper* const  theWrapper =
        XercesEntityWrapper::create(
                               getMemoryManager(),
                               theXercesNode,
                               theNavigator);

    m_nodes.push_back(theWrapper);

    // Add it to the node map -- entities ALWAYS go in the map
    // for now...
    m_nodeMap.addAssociation(theXercesNode, theWrapper);

    if (theWrapperNodeNavigator != 0)
    {
        *theWrapperNodeNavigator = &theNavigator;
    }

    return theWrapper;
}



XercesEntityReferenceWrapper*
XercesDocumentWrapper::createWrapperNode(
            const DOMEntityReferenceType*   theXercesNode,
            IndexType                       theIndex,
            bool                            mapNode,
            XercesWrapperNavigator**        theWrapperNodeNavigator) const
{
    // Create a navigator...
    XercesWrapperNavigator& theNavigator = createNavigator();

    theNavigator.setIndex(theIndex);

    XercesEntityReferenceWrapper* const theWrapper =
        XercesEntityReferenceWrapper::create(
                                        getMemoryManager(),
                                        theXercesNode,
                                        theNavigator);

    m_nodes.push_back(theWrapper);

    if (mapNode == true)
    {
        // Add it to the node map...
        m_nodeMap.addAssociation(theXercesNode, theWrapper);
    }

    if (theWrapperNodeNavigator != 0)
    {
        *theWrapperNodeNavigator = &theNavigator;
    }

    return theWrapper;
}



XercesNotationWrapper*
XercesDocumentWrapper::createWrapperNode(
            const DOMNotationType*      theXercesNode,
            IndexType                   theIndex,
            bool                        /* mapNode */,
            XercesWrapperNavigator**    theWrapperNodeNavigator) const
{
    // Create a navigator...
    XercesWrapperNavigator& theNavigator = createNavigator();

    theNavigator.setIndex(theIndex);

    XercesNotationWrapper* const        theWrapper =
        XercesNotationWrapper::create(
                                 getMemoryManager(),
                                 theXercesNode,
                                 theNavigator);

    m_nodes.push_back(theWrapper);

    // Add it to the node map -- notations ALWAYS go in the map
    // for now...
    m_nodeMap.addAssociation(theXercesNode, theWrapper);

    if (theWrapperNodeNavigator != 0)
    {
        *theWrapperNodeNavigator = &theNavigator;
    }

    return theWrapper;
}



XalanNode*
XercesDocumentWrapper::createWrapperNode(
            const DOMNodeType*          theXercesNode,
            IndexType                   theIndex,
            bool                        mapNode,
            XercesWrapperNavigator**    theWrapperNodeNavigator) const
{
    assert(theXercesNode != 0);

    XalanNode*              theNewNode = 0;

    switch(theXercesNode->getNodeType())
    {
    case DOMNodeType::ATTRIBUTE_NODE:
        {
            const DOMAttrType*      theAttrNode =
                        static_cast<const DOMAttrType*>(theXercesNode);

            theNewNode = createWrapperNode(theAttrNode, theIndex, mapNode, theWrapperNodeNavigator);
            assert(theNewNode != 0);
        }
        break;

    case DOMNodeType::CDATA_SECTION_NODE:
        {
            const DOMCDATASectionType*  theCDATASectionNode =
                        static_cast<const DOMCDATASectionType*>(theXercesNode);

            theNewNode = createWrapperNode(theCDATASectionNode, theIndex, mapNode, theWrapperNodeNavigator);
            assert(theNewNode != 0);

        }
        break;

    case DOMNodeType::COMMENT_NODE:
        {
            const DOMCommentType*   theCommentNode =
                        static_cast<const DOMCommentType*>(theXercesNode);

            theNewNode = createWrapperNode(theCommentNode, theIndex, mapNode, theWrapperNodeNavigator);
            assert(theNewNode != 0);
        }
        break;

    case DOMNodeType::DOCUMENT_FRAGMENT_NODE:
        throw XercesDOMWrapperException(XercesDOMWrapperException::NOT_SUPPORTED_ERR);
        break;

    case DOMNodeType::ELEMENT_NODE:
        {
            const DOMElementType*   theElementNode =
                        static_cast<const DOMElementType*>(theXercesNode);

            theNewNode = createWrapperNode(theElementNode, theIndex, mapNode, theWrapperNodeNavigator);
            assert(theNewNode != 0);
        }
        break;

    case DOMNodeType::ENTITY_NODE:
        {
            const DOMEntityType*    theEntityNode =
                        static_cast<const DOMEntityType*>(theXercesNode);

            theNewNode = createWrapperNode(theEntityNode, theIndex, mapNode, theWrapperNodeNavigator);
            assert(theNewNode != 0);
        }
        break;

    case DOMNodeType::ENTITY_REFERENCE_NODE:
        {
            const DOMEntityReferenceType*   theEntityReferenceNode =
                        static_cast<const DOMEntityReferenceType*>(theXercesNode);

            theNewNode = createWrapperNode(theEntityReferenceNode, theIndex, mapNode, theWrapperNodeNavigator);
            assert(theNewNode != 0);
        }
        break;

    case DOMNodeType::NOTATION_NODE:
        {
            const DOMNotationType*  theNotationNode =
                        static_cast<const DOMNotationType*>(theXercesNode);

            theNewNode = createWrapperNode(theNotationNode, theIndex, mapNode, theWrapperNodeNavigator);
            assert(theNewNode != 0);
        }
        break;

    case DOMNodeType::PROCESSING_INSTRUCTION_NODE:
        {
            const DOMProcessingInstructionType*     thePINode =
                        static_cast<const DOMProcessingInstructionType*>(theXercesNode);

            theNewNode = createWrapperNode(thePINode, theIndex, mapNode, theWrapperNodeNavigator);
            assert(theNewNode != 0);
        }
        break;

    case DOMNodeType::TEXT_NODE:
        {
            const DOMTextType*  theTextNode =
                        static_cast<const DOMTextType*>(theXercesNode);

            theNewNode = createWrapperNode(theTextNode, theIndex, mapNode, theWrapperNodeNavigator);
            assert(theNewNode != 0);
        }
        break;

    case DOMNodeType::DOCUMENT_TYPE_NODE:
        {
            const DOMDocumentType_Type*     theDoctypeNode =
                        static_cast<const DOMDocumentType_Type*>(theXercesNode);

            theNewNode = createWrapperNode(theDoctypeNode, theIndex, mapNode, theWrapperNodeNavigator);

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
XercesDocumentWrapper::destroyNode(XalanNode*   theNode)
{
    XALAN_USING_STD(find)

    const NodeVectorType::iterator  i =
        find(m_nodes.begin(), m_nodes.end(), theNode);

    if (i == m_nodes.end())
    {
        // Doesn't belong to this doc, so throw...
        throw XercesDOMWrapperException(XercesDOMWrapperException::WRONG_DOCUMENT_ERR);
    }
    else
    {
        assert(*i != 0);

        // Delete the node...
        XalanDestroy(
            m_nodes.getMemoryManager(),
            **i);

        // Erase it from the map...
        m_nodes.erase(i);
    }
}



const XalanDOMString&
XercesDocumentWrapper::getNodeName() const
{
    assert(m_navigator != 0);

    return m_navigator->getPooledString(m_xercesDocument->getNodeName());
}



const XalanDOMString&
XercesDocumentWrapper::getNodeValue() const
{
    assert(m_navigator != 0);

    return m_navigator->getPooledString(m_xercesDocument->getNodeValue());
}



XercesDocumentWrapper::NodeType
XercesDocumentWrapper::getNodeType() const
{
    return DOCUMENT_NODE;
}



XalanNode*
XercesDocumentWrapper::getParentNode() const
{
    return 0;
}



const XalanNodeList*
XercesDocumentWrapper::getChildNodes() const
{
    return &m_children;
}



XalanNode*
XercesDocumentWrapper::getFirstChild() const
{
    assert(m_navigator != 0);

    return m_navigator->getFirstChild(m_xercesDocument);
}



XalanNode*
XercesDocumentWrapper::getLastChild() const
{
    assert(m_navigator != 0);

    return m_navigator->getLastChild(m_xercesDocument);
}



XalanNode*
XercesDocumentWrapper::getPreviousSibling() const
{
    return 0;
}



XalanNode*
XercesDocumentWrapper::getNextSibling() const
{
    return 0;
}



const XalanNamedNodeMap*
XercesDocumentWrapper::getAttributes() const
{
    return 0;
}



XalanDocument*
XercesDocumentWrapper::getOwnerDocument() const
{
    return 0;
}



const XalanDOMString&
XercesDocumentWrapper::getNamespaceURI() const
{
    assert(m_navigator != 0);

    return m_navigator->getPooledString(m_xercesDocument->getNamespaceURI());
}



const XalanDOMString&
XercesDocumentWrapper::getPrefix() const
{
    assert(m_navigator != 0);

    return m_navigator->getPooledString(m_xercesDocument->getPrefix());
}



const XalanDOMString&
XercesDocumentWrapper::getLocalName() const
{
    assert(m_navigator != 0);

    return m_navigator->getPooledString(m_xercesDocument->getLocalName());
}



bool
XercesDocumentWrapper::isIndexed() const
{
    return m_indexValid;
}



XercesDocumentWrapper::IndexType
XercesDocumentWrapper::getIndex() const
{
    assert(m_navigator != 0);
    assert(m_navigator->getIndex() == 1);

    return m_navigator->getIndex();
}



XalanElement*
XercesDocumentWrapper::getDocumentElement() const
{
    // If we're working in mapping mode, m_documentElement will
    // be null, so we'll have to map the node...
    return m_documentElement != 0 ? m_documentElement : mapNode(m_xercesDocument->getDocumentElement());
}



XalanElement*
XercesDocumentWrapper::getElementById(const XalanDOMString&     elementId) const
{
    const DOMNodeType* const    theXercesNode =
        m_xercesDocument->getElementById(elementId.c_str());

    if (theXercesNode == 0)
    {
        return 0;
    }
    else
    {
        return static_cast<XalanElement*>(mapNode(theXercesNode));
    }
}



void
XercesDocumentWrapper::buildWrapperNodes()
{
    const DOMNodeType*  theStartChild = m_xercesDocument->getFirstChild();

    if (theStartChild != 0)
    {
        assert(m_navigator != 0);

        m_navigator->setIndex(1);
        m_navigator->setFirstChild(0);

        BuildWrapperTreeWalker  theTreeWalker(
                this,
                m_navigator,
                2,
                m_buildMaps);

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



XercesDocumentWrapper::BuildWrapperTreeWalker::BuildWrapperTreeWalker(
            XercesDocumentWrapper*              theDocument,
            XercesWrapperNavigator*             theDocumentNavigator,
            IndexType                           theStartIndex,
            bool                                theBuildMapsFlag) :
    m_document(theDocument),
    m_currentIndex(theStartIndex),
    m_parentNavigatorStack(theDocument->getMemoryManager()),
    m_siblingNavigatorStack(theDocument->getMemoryManager()),
    m_buildMaps(theBuildMapsFlag)
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



XercesDocumentWrapper::BuildWrapperTreeWalker::~BuildWrapperTreeWalker()
{
}



bool
XercesDocumentWrapper::BuildWrapperTreeWalker::startNode(const DOMNodeType*     node)
{
    XercesWrapperNavigator*     theWrapperNodeNavigator;

    XalanNode* const    theWrapperNode =
        m_document->createWrapperNode(
                node,
                m_currentIndex,
                m_buildMaps,
                &theWrapperNodeNavigator);
    assert(theWrapperNodeNavigator != 0);

    assert(m_parentNavigatorStack.empty() == false);
    assert(m_siblingNavigatorStack.empty() == false);

    // Get the two navigators...
    NavigatorStackEntryType&    theParentEntry = m_parentNavigatorStack.back();
    NavigatorStackEntryType&    theSiblingEntry = m_siblingNavigatorStack.back();

    theWrapperNodeNavigator->setParentNode(theParentEntry.m_node);

    // If the first child has not been set, then set it
    // now...
    if (theParentEntry.m_navigator->getFirstChild() == 0)
    {
        assert(theSiblingEntry.m_node == 0);

        theParentEntry.m_navigator->setFirstChild(theWrapperNode);
    }

    // Always set the last child...
    theParentEntry.m_navigator->setLastChild(theWrapperNode);

    theWrapperNodeNavigator->setPreviousSibling(theSiblingEntry.m_node);

    if (theSiblingEntry.m_navigator != 0)
    {
        theSiblingEntry.m_navigator->setNextSibling(theWrapperNode);
    }

    // Build an entry for the stacks...
    const NavigatorStackEntryType   theCurrentEntry(theWrapperNodeNavigator, theWrapperNode);

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

    if (node->getNodeType() == DOMNodeType::DOCUMENT_TYPE_NODE)
    {
        // Special case for doctype -- we have to build its entities...
        const DOMDocumentType_Type* const   theDoctype =
            static_cast<const DOMDocumentType_Type*>(node);

        const DOMNamedNodeMapType* const    theEntities =
            theDoctype->getEntities();

        const XalanSize_t   theLength =
            theEntities->getLength();

        for (XalanSize_t i = 0; i < theLength; ++i)
        {
            // Build it, but don't index it...
            m_document->createWrapperNode(theEntities->item(i), m_currentIndex++, true);
        }
    }
    else if (node->getNodeType() == DOMNodeType::ELEMENT_NODE)
    {
        // Special case for element nodes -- we have to build the attributes...
        const DOMElementType* const     theElement =
            static_cast<const DOMElementType*>(node);

        const DOMNamedNodeMapType* const    theAttributes =
            theElement->getAttributes();
        assert(theAttributes != 0);

        const XalanSize_t   theLength =
            theAttributes->getLength();

        XercesWrapperNavigator* thePreviousAttrNavigator = 0;
        XalanNode*              thePreviousAttr = 0;

        for (XalanSize_t i = 0; i < theLength; ++i)
        {
            // Get the attribute from the node map...
            const DOMNodeType* const    theAttr = theAttributes->item(i);
            assert(theAttr != 0);

            XercesWrapperNavigator*     theCurrentAttrNavigator;

            // Create a wrapper node.
            XalanNode* const    theCurrentAttr =
                m_document->createWrapperNode(
                    theAttr,
                    m_currentIndex,
                    m_buildMaps,
                    &theCurrentAttrNavigator);
            assert(theCurrentAttr != 0 && theCurrentAttrNavigator != 0);

            // Set the parent node...
            theCurrentAttrNavigator->setParentNode(theWrapperNode);

            if (thePreviousAttr != 0)
            {
                assert(thePreviousAttrNavigator != 0);

                // Link in the previous attribute...
                theCurrentAttrNavigator->setPreviousSibling(thePreviousAttr);

                thePreviousAttrNavigator->setNextSibling(theCurrentAttr);
            }

            // Update the pointers so they point to this attribute...
            thePreviousAttr = theCurrentAttr;
            thePreviousAttrNavigator = theCurrentAttrNavigator;

            // Finally, increment the index...
            ++m_currentIndex;
        }
    }

    return false;
}



bool
XercesDocumentWrapper::BuildWrapperTreeWalker::endNode(const DOMNodeType*   /* node */)
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

    return false;
}



const XalanDOMString&
XercesDocumentWrapper::getPooledString(const XalanDOMString&    theString) const
{
    return m_stringPool->get(theString);
}



const XalanDOMString&
XercesDocumentWrapper::getPooledString(
            const XalanDOMChar*         theString,
            XalanDOMString::size_type   theLength) const
{
    return m_stringPool->get(theString, theLength);
}



XALAN_CPP_NAMESPACE_END
