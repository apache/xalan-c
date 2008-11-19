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
#if !defined(XERCESDOCUMENTWRAPPER_HEADER_GUARD_1357924680)
#define XERCESDOCUMENTWRAPPER_HEADER_GUARD_1357924680



#include <xalanc/XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



#include <xalanc/Include/XalanVector.hpp>



#include <xalanc/XalanDOM/XalanDocument.hpp>



#if defined(XALAN_AUTO_PTR_REQUIRES_DEFINITION)
#include <xalanc/PlatformSupport/XalanDOMStringPool.hpp>
#endif



#include <xalanc/Include/XalanMemMgrAutoPtr.hpp>



#include <xalanc/XercesParserLiaison/XercesDOMWalker.hpp>
#include <xalanc/XercesParserLiaison/XercesWrapperNavigator.hpp>


#include <xalanc/XercesParserLiaison/XercesWrapperToXalanNodeMap.hpp>
#include <xalanc/XercesParserLiaison/XercesNodeListWrapper.hpp>
#include <xalanc/XercesParserLiaison/XercesAttrWrapperAllocator.hpp>
#include <xalanc/XercesParserLiaison/XercesElementWrapperAllocator.hpp>
#include <xalanc/XercesParserLiaison/XercesTextWrapperAllocator.hpp>
#include <xalanc/XercesParserLiaison/XercesWrapperNavigatorAllocator.hpp>


#include <xalanc/XercesParserLiaison/XercesWrapperTypes.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XalanDOMStringPool;
class XercesCommentWrapper;
class XercesCDATASectionWrapper;
class XercesDocumentFragmentWrapper;
class XercesDocumentTypeWrapper;
class XercesEntityWrapper;
class XercesEntityReferenceWrapper;
class XercesProcessingInstructionWrapper;
class XercesNotationWrapper;



class XALAN_XERCESPARSERLIAISON_EXPORT XercesDocumentWrapper : public XalanDocument
{
public:

    friend class XercesWrapperNavigator;

    /**
     *
     * Constructor for XercesDocumentWrapper.
     *
     * If the document will be shared amongst multiple threads of execution,
     * the parameter buildWrapper must be true.  Otherwise, the bridge
     * nodes will be built on demand, a process which is not synchronized.
     * This could cause serious problems if multiple threads tried to visit
     * an unbuilt node at the same time.
     *
     * @param theXercesDocument The Xerces document to bridge
     * @param threadSafe If true, the tree can safely be shared amongst multiple threads.  (Also implies buildWrapper == true)
     * @param buildWrapper If true, all of the bridge nodes will be built during construction.
     * @param buildMaps If true, a map of Xerces to Xalan nodes will be built, even if the bridge is built
     *
     */
    XercesDocumentWrapper(
            MemoryManager&          theManager,
            const DOMDocument_Type*     theXercesDocument,
            bool                        threadSafe = true,
            bool                        buildWrapper = true,
            bool                        buildMaps = false);

    static XercesDocumentWrapper*
    create( 
            MemoryManager&          theManager,
            const DOMDocument_Type*     theXercesDocument,
            bool                        threadSafe,
            bool                        buildWrapper,
            bool                        buildMaps);

    MemoryManager&
    getMemoryManager() const
    {
        return m_nodeMap.getMemoryManager();
    }

    virtual
    ~XercesDocumentWrapper();

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

    virtual const XalanDOMString&
    getNamespaceURI() const;

    virtual const XalanDOMString&
    getPrefix() const;

    virtual const XalanDOMString&
    getLocalName() const;

    virtual bool
    isIndexed() const;

    virtual IndexType
    getIndex() const;

    virtual XalanElement*
    getDocumentElement() const;

    virtual XalanElement*
    getElementById(const XalanDOMString&    elementId) const;

    // These are some special interfaces to manage relationships between
    // our nodes and Xerces nodes.

    /**
     * Destroy the entire bridge structure that connects
     * the Xerces document to this XercesDocumentWrapper
     * instance.  This will invalidate any pointers to
     * any nodes in the document (except, of course, the
     * document itself).
     */
    void
    destroyWrapper();

    /**
     * Rebuild the entire bridge structure that connects
     * the Xerces document to this XercesDocumentWrapper
     * instance.  This destroys the bridge before
     * rebuilding.
     */
    void
    rebuildWrapper();

    /**
     * Map a Xerces node to the corresponding wrapper node.
     * If the constructor for the instance was called with
     * the threadSafe or buildWrapper parameter equal to
     * true, this call will fail.
     *
     * @param theXercesNode The Xerces instance to map
     *
     * @return The pointer to the corresponding XalanNode instance, or 0 if the node could not be mapped.
     */
    XalanNode*
    mapNode(const DOMNodeType*  theXercesNode) const;

    /**
     * Map a Xerces node to the corresponding wrapper node.
     * If the constructor for the instance was called with
     * the threadSafe or buildWrapper parameter equal to
     * true, this call will fail.
     *
     * @param theXercesNode The Xerces instance to map
     *
     * @return The pointer to the corresponding XalanNode instance, or 0 if the node could not be mapped.
     */
    XalanAttr*
    mapNode(const DOMAttrType*  theXercesNode) const;

    /**
     * Map a Xerces node to the corresponding wrapper node.
     * If the constructor for the instance was called with
     * the threadSafe or buildWrapper parameter equal to
     * true, this call will fail.
     *
     * @param theXercesNode The Xerces instance to map
     *
     * @return The pointer to the corresponding XalanNode instance, or 0 if the node could not be mapped.
     */
    XalanElement*
    mapNode(const DOMElementType*   theXercesNode) const;

    /**
     * Map a XalanNode to the corresponding Xerces node.
     * If the node not owned by this document, the
     * function will throw XalanDOMException with the code
     * WRONG_DOCUMENT_ERR.
     *
     * @param theXalanNode The Xalan instance to map
     *
     * @return The pointer to the corresponding XalanNode instance, or 0 if the node could not be mapped.
     */
    const DOMNodeType*
    mapNode(XalanNode*  theXalanNode) const;

    /**
     *
     * Get the Xerces DOMDocument that this XercesDocument represents.
     *
     * @return the Xerces DOMDocument instance.
     *
     */
    const DOMDocument_Type*
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
    buildWrapperNodes();

    typedef XalanVector<XalanNode*>     NodeVectorType;

    // Helper class to walk the tree and build everything...
    class BuildWrapperTreeWalker : public XercesDOMWalker
    {
    public:

        typedef XercesDOMWalker             ParentType;

        BuildWrapperTreeWalker(
                XercesDocumentWrapper*              theDocument,
                XercesWrapperNavigator*             theDocumentNavigator,
                IndexType                           theStartIndex,
                bool                                theBuildMapsFlag);

        virtual
        ~BuildWrapperTreeWalker();

        struct NavigatorStackEntryType
        {
            NavigatorStackEntryType(
                        XercesWrapperNavigator* theNavigator = 0,
                        XalanNode*              theNode = 0) :
                m_navigator(theNavigator),
                m_node(theNode)
            {
            }

            XercesWrapperNavigator* m_navigator;

            XalanNode*              m_node;
        };

        typedef XalanVector<NavigatorStackEntryType>            NavigatorStackType;

    protected:

        virtual bool
        startNode(const DOMNodeType*    node);

        virtual bool
        endNode(const DOMNodeType*  node);

        using ParentType::startNode;
        using ParentType::endNode;

    private:

        XercesDocumentWrapper*              m_document;

        IndexType                           m_currentIndex;

        NavigatorStackType                  m_parentNavigatorStack;

        NavigatorStackType                  m_siblingNavigatorStack;

        const bool                          m_buildMaps;
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
            XalanDOMString::size_type   theLength = XalanDOMString::npos) const;

    bool
    getMappingMode() const
    {
        return m_mappingMode;
    }

private:

    // Destruction API...
    void
    destroyWrapperNode(XalanNode*   theNode);

    // Not implemented...
    XercesDocumentWrapper(const XercesDocumentWrapper&  theSource);

    XercesDocumentWrapper&
    operator=(const XercesDocumentWrapper&  theRHS);

    bool
    operator==(const XercesDocumentWrapper& theRHS) const;

    // Private delete function...
    void
    destroyNode(XalanNode*  theNode);

    // More internal implementation stuff...

    // Factory methods for our implementation nodes...
    XalanNode*
    createWrapperNode(
            const DOMNodeType*          theXercesNode,
            IndexType                   theIndex,
            bool                        mapNode,
            XercesWrapperNavigator**    theWrapperNodeNavigator = 0) const;

    XercesDocumentTypeWrapper*
    createWrapperNode(
            const DOMDocumentType_Type*     theDoctype,
            IndexType                       theIndex,
            bool                            mapNode,
            XercesWrapperNavigator**        theWrapperNodeNavigator = 0) const;

    XercesElementWrapper*
    createWrapperNode(
            const DOMElementType*       theXercesNode,
            IndexType                   theIndex,
            bool                        mapNode,
            XercesWrapperNavigator**    theWrapperNodeNavigator = 0) const;

    XercesTextWrapper*
    createWrapperNode(
            const DOMTextType*          theXercesNode,
            IndexType                   theIndex,
            bool                        mapNode,
            XercesWrapperNavigator**    theWrapperNodeNavigator = 0) const;

    XercesCommentWrapper*
    createWrapperNode(
            const DOMCommentType*       theXercesNode,
            IndexType                   theIndex,
            bool                        mapNode,
            XercesWrapperNavigator**    theWrapperNodeNavigator = 0) const;

    XercesCDATASectionWrapper*
    createWrapperNode(
            const DOMCDATASectionType*  theXercesNode,
            IndexType                   theIndex,
            bool                        mapNode,
            XercesWrapperNavigator**    theWrapperNodeNavigator = 0) const;

    XercesProcessingInstructionWrapper*
    createWrapperNode(
            const DOMProcessingInstructionType*     theXercesNode,
            IndexType                               theIndex,
            bool                                    mapNode,
            XercesWrapperNavigator**                theWrapperNodeNavigator = 0) const;

    XercesAttrWrapper*
    createWrapperNode(
            const DOMAttrType*          theXercesNode,
            IndexType                   theIndex,
            bool                        mapNode,
            XercesWrapperNavigator**    theWrapperNodeNavigator = 0) const;

    XercesEntityWrapper*
    createWrapperNode(
            const DOMEntityType*        theXercesNode,
            IndexType                   theIndex,
            bool                        mapNode,
            XercesWrapperNavigator**    theWrapperNodeNavigator = 0) const;

    XercesEntityReferenceWrapper*
    createWrapperNode(
            const DOMEntityReferenceType*   theXercesNode,
            IndexType                       theIndex,
            bool                            mapNode,
            XercesWrapperNavigator**        theWrapperNodeNavigator = 0) const;

    XercesNotationWrapper*
    createWrapperNode(
            const DOMNotationType*      theXercesNode,
            IndexType                   theIndex,
            bool                        mapNode,
            XercesWrapperNavigator**    theWrapperNodeNavigator = 0) const;

    XercesWrapperNavigator&
    createNavigator() const;

    // This is a private helper class for building the tree...
    friend class BuildWrapperTreeWalker;

    const DOMDocument_Type* const                   m_xercesDocument;

    XalanElement*                                   m_documentElement;

    mutable XercesWrapperToXalanNodeMap             m_nodeMap;

    mutable XercesWrapperNavigatorAllocator         m_navigatorAllocator;

    // Our navigator will be the first entry in m_navigators,
    // but we'll cache this so access is faster...
    XercesWrapperNavigator*                         m_navigator;

    XercesNodeListWrapper                           m_children;

    mutable NodeVectorType                          m_nodes;

    mutable XercesDocumentTypeWrapper*              m_doctype;

    bool                                            m_mappingMode;

    bool                                            m_indexValid;

    const bool                                      m_buildMaps;

    mutable XercesElementWrapperAllocator           m_elementAllocator;

    mutable XercesTextWrapperAllocator              m_textAllocator;

    mutable XercesAttrWrapperAllocator              m_attributeAllocator;

    const XalanMemMgrAutoPtr<XalanDOMStringPool>    m_stringPool;
};



XALAN_CPP_NAMESPACE_END



#endif  // !defined(XERCESDOCUMENTWRAPPER_HEADER_GUARD_1357924680)
