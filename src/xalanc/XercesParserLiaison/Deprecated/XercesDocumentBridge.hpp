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
#if !defined(XERCESDOCUMENTBRIDGE_HEADER_GUARD_1357924680)
#define XERCESDOCUMENTBRIDGE_HEADER_GUARD_1357924680



#include <xalanc/XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



#include <xalanc/Include/XalanVector.hpp>
#include <xalanc/Include/XalanDeque.hpp>



#if XERCES_VERSION_MAJOR >= 2
#include <xercesc/dom/deprecated/DOM_Document.hpp>
#else
#include <xercesc/dom/DOM_Document.hpp>
#endif



#include <xalanc/XalanDOM/XalanDocument.hpp>



#if defined(XALAN_AUTO_PTR_REQUIRES_DEFINITION)
#include <xalanc/XalanDOM/XalanDOMImplementation.hpp>
#include <xalanc/PlatformSupport/XalanDOMStringPool.hpp>
#endif



#include <xalanc/Include/XalanAutoPtr.hpp>



#include <xalanc/DOMSupport/TreeWalker.hpp>



#include <xalanc/XercesParserLiaison/Deprecated/XercesToXalanNodeMap.hpp>
#include <xalanc/XercesParserLiaison/Deprecated/XercesBridgeNavigator.hpp>
#include <xalanc/XercesParserLiaison/Deprecated/XercesNodeListBridge.hpp>
#include <xalanc/XercesParserLiaison/Deprecated/XercesTreeWalker.hpp>
#include <xalanc/XercesParserLiaison/Deprecated/XercesElementBridgeAllocator.hpp>
#include <xalanc/XercesParserLiaison/Deprecated/XercesTextBridgeAllocator.hpp>
#include <xalanc/XercesParserLiaison/Deprecated/XercesAttributeBridgeAllocator.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XalanDOMStringPool;
class XercesAttrBridge;
class XercesCommentBridge;
class XercesCDATASectionBridge;
class XercesDocumentFragmentBridge;
class XercesDocumentTypeBridge;
class XercesElementBridge;
class XercesEntityBridge;
class XercesEntityReferenceBridge;
class XercesProcessingInstructionBridge;
class XercesNotationBridge;
class XercesTextBridge;


/**
 * This class is deprecated.
 *
 * @deprecated This class is part of the deprecated Xerces DOM bridge.
 */
class XALAN_XERCESPARSERLIAISON_EXPORT XercesDocumentBridge : public XalanDocument
{
public:

    friend class XercesBridgeNavigator;

    /**
     *
     * Constructor for XercesDocumentBridge.
     *
     * If the document will be shared amongst multiple threads of execution,
     * the parameter buildBridge must be true.  Otherwise, the bridge
     * nodes will be built on demand, a process which is not synchronized.
     * This could cause serious problems if multiple threads tried to visit
     * an unbuilt node at the same time.
     *
     * @param theXercesDocument The Xerces document to bridge
     * @param threadSafe If true, the tree can safely be shared amongst multiple threads.  (Also implies buildBridge == true)
     * @param buildBridge If true, all of the bridge nodes will be built during construction.
     *
     */
    XercesDocumentBridge(
            const DOM_Document_Type&    theXercesDocument,
            bool                        threadSafe = true,
            bool                        buildBridge = true);

    virtual
    ~XercesDocumentBridge();

    // These interfaces are inherited from XalanNode...

    virtual const XalanDOMString&
    getNodeName() const;

    virtual const XalanDOMString&
    getNodeValue() const;

    virtual NodeType
    getNodeType() const;

    virtual XalanNode*
    getParentNode() const;

    virtual const XalanNodeList*
    getChildNodes() const;

    virtual XalanNode*
    getFirstChild() const;

    virtual XalanNode*
    getLastChild() const;

    virtual XalanNode*
    getPreviousSibling() const;

    virtual XalanNode*
    getNextSibling() const;

    virtual const XalanNamedNodeMap*
    getAttributes() const;

    virtual XalanDocument*
    getOwnerDocument() const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
    virtual XalanNode*
#else
    virtual XercesDocumentBridge*
#endif
    cloneNode(bool deep) const;

    virtual XalanNode*
    insertBefore(
            XalanNode*  newChild,
            XalanNode*  refChild);

    virtual XalanNode*
    replaceChild(
            XalanNode*  newChild,
            XalanNode*  oldChild);

    virtual XalanNode*
    removeChild(XalanNode*  oldChild);

    virtual XalanNode*
    appendChild(XalanNode*  newChild);

    virtual bool
    hasChildNodes() const;

    virtual void
    setNodeValue(const XalanDOMString&      nodeValue);

    virtual void
    normalize();

    virtual bool
    isSupported(
            const XalanDOMString&   feature,
            const XalanDOMString&   version) const;

    virtual const XalanDOMString&
    getNamespaceURI() const;

    virtual const XalanDOMString&
    getPrefix() const;

    virtual const XalanDOMString&
    getLocalName() const;

    virtual void
    setPrefix(const XalanDOMString& prefix);

    virtual bool
    isIndexed() const;

    virtual IndexType
    getIndex() const;

    virtual XalanElement*
    createElement(const XalanDOMString& tagName);

    virtual XalanDocumentFragment*
    createDocumentFragment();

    virtual XalanText*
    createTextNode(const XalanDOMString&    data);

    virtual XalanComment*
    createComment(const XalanDOMString& data);

    virtual XalanCDATASection*
    createCDATASection(const XalanDOMString&    data);

    virtual XalanProcessingInstruction*
    createProcessingInstruction(
            const XalanDOMString&   target,
            const XalanDOMString&   data);

    virtual XalanAttr*
    createAttribute(const XalanDOMString&   name);

    virtual XalanEntityReference*
    createEntityReference(const XalanDOMString& name);

    virtual XalanDocumentType*
    getDoctype() const;

    virtual XalanDOMImplementation*
    getImplementation() const;

    virtual XalanElement*
    getDocumentElement() const;

    virtual XalanNodeList*
    getElementsByTagName(const XalanDOMString&      tagname) const;

    virtual XalanNode*
    importNode(
            XalanNode*  importedNode,
            bool        deep);

    virtual XalanElement*
    createElementNS(
            const XalanDOMString&   namespaceURI,
            const XalanDOMString&   qualifiedName);

    virtual XalanAttr*
    createAttributeNS(
            const XalanDOMString& namespaceURI,
            const XalanDOMString& qualifiedName);

    virtual XalanNodeList*
    getElementsByTagNameNS(
            const XalanDOMString&   namespaceURI,
            const XalanDOMString&   localName) const;

    virtual XalanElement*
    getElementById(const XalanDOMString&    elementId) const;

    // These are some special interfaces to manage relationships between
    // our nodes and Xerces nodes.

    /**
     * Destroy the entire bridge structure that connects
     * the Xerces document to this XercesDocumentBridge
     * instance.  This will invalidate any pointers to
     * any nodes in the document (except, of course, the
     * document itself).
     */
    void
    destroyBridge();

    /**
     * Rebuild the entire bridge structure that connects
     * the Xerces document to this XercesDocumentBridge
     * instance.  This destroys the bridge before
     * rebuilding.
     */
    void
    rebuildBridge();

    XalanNode*
    mapNode(const DOM_NodeType&     theXercesNode) const;

    XalanAttr*
    mapNode(const DOM_AttrType&     theXercesNode) const;

    XalanElement*
    mapNode(const DOM_ElementType&  theXercesNode) const;

    DOM_NodeType
    mapNode(const XalanNode*    theXalanNode) const;

    DOM_AttrType
    mapNode(const XalanAttr*    theXalanNode) const;

    NodeImplType*
    mapNodeToImpl(const XalanNode*  theXalanNode) const;

    /**
     *
     * Get the Xerces DOM_Document that this XercesDocument represents.
     *
     * @return the Xerces DOM_Document instance.
     *
     */
    DOM_Document_Type
    getXercesDocument() const
    {
        return m_xercesDocument;
    }

    /**
     * Build the entire bridge structure.  This should be done before any
     * processing begins, if the tree will be shared amongst multiple
     * threads.
     */
    void
    buildBridgeNodes();


    typedef XalanDeque<XercesBridgeNavigator>   NavigatorBridgeVectorType;
    typedef XalanDeque<XalanNode*>              NodeVectorType;


    // Helper class to walk the tree and build everything...
    class BuildBridgeTreeWalker : public XercesTreeWalker
    {
    public:

        typedef NavigatorBridgeVectorType   NavigatorBridgeVectorInnerType;

        BuildBridgeTreeWalker(
                XercesDocumentBridge*           theDocument,
                XercesBridgeNavigator*          theDocumentNavigator,
                NavigatorBridgeVectorInnerType& theNavigators,
                IndexType                       theStartIndex);

        virtual
        ~BuildBridgeTreeWalker();

        struct NavigatorStackEntryType
        {
            NavigatorStackEntryType(
                        XercesBridgeNavigator*  theNavigator = 0,
                        XalanNode*              theNode = 0) :
                m_navigator(theNavigator),
                m_node(theNode)
            {
            }

            XercesBridgeNavigator*  m_navigator;

            XalanNode*              m_node;
        };

    
        typedef XalanVector<NavigatorStackEntryType>    NavigatorStackType;

    protected:

        virtual void
        startNode(const DOM_NodeType&   node);

        virtual void
        endNode(const DOM_NodeType&     node);

    private:

        XercesDocumentBridge*           m_document;

        NavigatorBridgeVectorInnerType& m_navigators;

        IndexType                       m_currentIndex;

        NavigatorStackType              m_parentNavigatorStack;

        NavigatorStackType              m_siblingNavigatorStack;
    };


    /**
     * Get a pooled string.  If the string is not in the pool,
     * add it.
     *
     * @param theString The string to pool.
     * @return A const reference to the pooled string.
     */
    const XalanDOMString&
    getPooledString(const XalanDOMString&   theString) const;

    /**
     * Get a pooled string.  If the string is not in the pool,
     * add it.
     *
     * @param theString The string to pool.
     * @param theLength The length of the string.  If XalanDOMString::npos, the string is assumed to be null-terminated.
     * @return A const reference to the pooled string.
     */
    const XalanDOMString&
    getPooledString(
            const XalanDOMChar*         theString,
            XalanDOMString::size_type   theLength /* = XalanDOMString::npos */) const;

private:

    XalanNode*
    mapNode(NodeImplType*   theXercesNodeImpl) const;

    // Destruction API...
    void
    destroyBridgeNode(XalanNode*    theNode);

    // Not implemented...
    XercesDocumentBridge(const XercesDocumentBridge&    theSource);

    XercesDocumentBridge&
    operator=(const XercesDocumentBridge&   theRHS);

    bool
    operator==(const XercesDocumentBridge&  theRHS) const;

    // Private delete function...
    void
    destroyNode(XalanNode*  theNode);

    // More internal implementation stuff...
    XalanNode*
    internalCloneNode(
            const XalanNode*        theXalanNode,
            const DOM_NodeType&     theXercesNode,
            bool                    deep);

    // Factory methods for our implementation nodes...
    XalanNode*
    createBridgeNode(
            const DOM_NodeType&     theXercesNode,
            IndexType               theIndex,
            bool                    mapNode) const;

    XercesDocumentTypeBridge*
    createBridgeNode(
            const DOM_DocumentType_Type&    theDoctype,
            IndexType                       theIndex,
            bool                            mapNode) const;

    XercesElementBridge*
    createBridgeNode(
            const DOM_ElementType&  theXercesNode,
            IndexType               theIndex,
            bool                    mapNode) const;

    XercesDocumentFragmentBridge*
    createBridgeNode(
            const DOM_DocumentFragmentType&     theXercesNode,
            IndexType                           theIndex,
            bool                                mapNode) const;

    XercesTextBridge*
    createBridgeNode(
            const DOM_TextType&     theXercesNode,
            IndexType               theIndex,
            bool                    mapNode) const;

    XercesCommentBridge*
    createBridgeNode(
            const DOM_CommentType&  theXercesNode,
            IndexType               theIndex,
            bool                    mapNode) const;

    XercesCDATASectionBridge*
    createBridgeNode(
            const DOM_CDATASectionType&     theXercesNode,
            IndexType                       theIndex,
            bool                            mapNode) const;

    XercesProcessingInstructionBridge*
    createBridgeNode(
            const DOM_ProcessingInstructionType&    theXercesNode,
            IndexType                               theIndex,
            bool                                    mapNode) const;

    XercesAttrBridge*
    createBridgeNode(
            const DOM_AttrType&     theXercesNode,
            IndexType               theIndex,
            bool                    mapNode) const;

    XercesEntityBridge*
    createBridgeNode(
            const DOM_EntityType&   theXercesNode,
            IndexType               theIndex,
            bool                    mapNode) const;

    XercesEntityReferenceBridge*
    createBridgeNode(
            const DOM_EntityReferenceType&  theXercesNode,
            IndexType                       theIndex,
            bool                            mapNode) const;

    XercesNotationBridge*
    createBridgeNode(
            const DOM_NotationType&     theXercesNode,
            IndexType                   theIndex,
            bool                        mapNode) const;

    XercesBridgeNavigator&
    pushNavigator(bool  mappingMode) const;

    // This is a private helper class for building the tree...
    friend class BuildBridgeTreeWalker;

    // $$$ ToDo: This is because DOM_Document::getElementById() is not
    // const...
    mutable DOM_Document_Type               m_xercesDocument;

    XalanElement*                           m_documentElement;

    mutable XercesToXalanNodeMap            m_nodeMap;

    XalanAutoPtr<XalanDOMImplementation>    m_domImplementation;

    mutable NavigatorBridgeVectorType       m_navigators;

    // Our navigator will be the first entry in m_navigators,
    // but we'll cache this so access is faster...
    XercesBridgeNavigator*                  m_navigator;

    XercesNodeListBridge                    m_children;

    mutable NodeVectorType                  m_nodes;

    mutable XercesDocumentTypeBridge*       m_doctype;

    bool                                    m_mappingMode;

    bool                                    m_indexValid;

    mutable XercesElementBridgeAllocator    m_elementAllocator;

    mutable XercesTextBridgeAllocator       m_textAllocator;

    mutable XercesAttributeBridgeAllocator  m_attributeAllocator;

    const XalanAutoPtr<XalanDOMStringPool>  m_stringPool;
};



XALAN_CPP_NAMESPACE_END



#endif  // !defined(XERCESDOCUMENTBRIDGE_HEADER_GUARD_1357924680)
