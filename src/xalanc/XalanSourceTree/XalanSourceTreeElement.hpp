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
#if !defined(XALANSOURCETREEELEMENT_HEADER_GUARD_1357924680)
#define XALANSOURCETREEELEMENT_HEADER_GUARD_1357924680



#include <xalanc/XalanSourceTree/XalanSourceTreeDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>
#include <xalanc/XalanDOM/XalanElement.hpp>
#include <xalanc/XalanDOM/XalanNamedNodeMap.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XalanSourceTreeAttr;
class XalanSourceTreeComment;
class XalanSourceTreeDocument;
class XalanSourceTreeDocumentFragment;
class XalanSourceTreeProcessingInstruction;
class XalanSourceTreeText;



class XALAN_XALANSOURCETREE_EXPORT XalanSourceTreeElement : public XalanElement
{
public:

    /**
     * Constructor.
     *
     * @param theTagName The tag name of the element
     * @param theOwnerDocument The document that owns the instance
     * @param theAttributes An array of pointers to the attribute instances for the element
     * @param theAttributeCount The number of attributes.
     * @param theParentNode The parent node, if any.
     * @param thePreviousSibling The previous sibling, if any.
     * @param theNextSibling The next sibling, if any.
     * @param theIndex The document-order index of the node.
     */
    XalanSourceTreeElement(
            MemoryManager&              theManager,
            const XalanDOMString&       theTagName,
            XalanSourceTreeDocument*    theOwnerDocument,
            XalanNode*                  theParentNode = 0,
            XalanNode*                  thePreviousSibling = 0,
            XalanNode*                  theNextSibling = 0,
            IndexType                   theIndex = 0);

    virtual
    ~XalanSourceTreeElement();

    MemoryManager& 
    getMemoryManager()
    {
        return m_memoryManager;
    }

    // These interfaces are inherited from XalanElement...

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
    getAttributes() const = 0;

    virtual XalanDocument*
    getOwnerDocument() const;

    virtual const XalanDOMString&
    getNamespaceURI() const = 0;

    virtual const XalanDOMString&
    getPrefix() const = 0;

    virtual const XalanDOMString&
    getLocalName() const = 0;

    virtual bool
    isIndexed() const;

    virtual IndexType
    getIndex() const;

    virtual const XalanDOMString&
    getTagName() const;

    // public interfaces not inherited from XalanElement...

    XalanSourceTreeDocument*
    getDocument() const
    {
        return m_ownerDocument;
    }

    void
    setParent(XalanSourceTreeElement*   theParent)
    {
        m_parentNode = theParent;
    }

    void
    setParent(XalanSourceTreeDocumentFragment*  theParent);

    void
    setPreviousSibling(XalanSourceTreeComment*  thePreviousSibling);

    void
    setPreviousSibling(XalanSourceTreeElement*  thePreviousSibling);

    void
    setPreviousSibling(XalanSourceTreeProcessingInstruction*    thePreviousSibling);

    void
    setPreviousSibling(XalanSourceTreeText*     thePreviousSibling);

    void
    appendSiblingNode(XalanSourceTreeComment*   theSibling);

    void
    appendSiblingNode(XalanSourceTreeElement*   theSibling);

    void
    appendSiblingNode(XalanSourceTreeProcessingInstruction*     theSibling);

    void
    appendSiblingNode(XalanSourceTreeText*  theSibling);

    void
    appendChildNode(XalanSourceTreeComment*     theChild);

    void
    appendChildNode(XalanSourceTreeElement*     theChild);

    void
    appendChildNode(XalanSourceTreeProcessingInstruction*   theChild);

    void
    appendChildNode(XalanSourceTreeText*    theChild);

    void
    setIndex(IndexType  theIndex)
    {
        m_index = theIndex;
    }

    /**
      * Removes all of the children.  Since the owner document controls the
      * lifetime of all nodes in the document, this just sets the first child
      * to 0.
      */
    void
    clearChildren()
    {
        m_firstChild = 0;
    }

protected:

    /*
    XalanSourceTreeElement(
            MemoryManager&              theManager,
            const XalanSourceTreeElement&   theSource,
            bool                            deep = false);
    */
    static const XalanDOMString     s_emptyString;

    // Data members...
    const XalanDOMString&       m_tagName;

private:

    // Not implemented...
    XalanSourceTreeElement(const XalanSourceTreeElement&    theSource);

    XalanSourceTreeElement&
    operator=(const XalanSourceTreeElement&     theSource);

    bool
    operator==(const XalanSourceTreeElement&    theRHS) const;


    // Data members...
    MemoryManager&          m_memoryManager;

    XalanSourceTreeDocument*    m_ownerDocument;

    XalanNode*                  m_parentNode;

    XalanNode*                  m_previousSibling;

    XalanNode*                  m_nextSibling;

    XalanNode*                  m_firstChild;

    IndexType                   m_index;
};



XALAN_CPP_NAMESPACE_END



#endif  // !defined(XALANSOURCETREEELEMENT_HEADER_GUARD_1357924680)
