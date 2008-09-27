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
#if !defined(XALANSOURCETREETEXT_HEADER_GUARD_1357924680)
#define XALANSOURCETREETEXT_HEADER_GUARD_1357924680



#include <xalanc/XalanSourceTree/XalanSourceTreeDefinitions.hpp>



#include <xalanc/XalanDOM/XalanText.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XalanSourceTreeComment;
class XalanSourceTreeDocumentFragment;
class XalanSourceTreeElement;
class XalanSourceTreeProcessingInstruction;



class XALAN_XALANSOURCETREE_EXPORT XalanSourceTreeText : public XalanText
{
public:

    /**
     * Perform static initialization.  See class XalanSourceTreeInit.
     */
    static void
    initialize(MemoryManager& theManager);

    /**
     * Perform static shut down.  See class XalanSourceTreeInit.
     */
    static void
    terminate();

    /**
     * Constructor.
     *
     * @param theData The text data of the node
     * @param isWhitespace true if the text data is only whitespace
     * @param theParentNode The parent node, if any.
     * @param thePreviousSibling The previous sibling, if any.
     * @param theNextSibling The next sibling, if any.
     * @param theIndex The document-order index of the node.
     */
    XalanSourceTreeText(
            const XalanDOMString&   theData,
            XalanNode*              theParentNode = 0,
            XalanNode*              thePreviousSibling = 0,
            XalanNode*              theNextSibling = 0,
            IndexType               theIndex = 0);

    virtual
    ~XalanSourceTreeText();


    // These interfaces are inherited from XalanText...

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

    virtual const XalanDOMString&
    getData() const;

    virtual bool
    isWhitespace() const;


    // public interfaces not inherited from XalanText...

    void
    setParent(XalanSourceTreeElement*   theParent);

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
    setIndex(IndexType  theIndex)
    {
        m_index = theIndex;
    }

private:

    // Not implemented...
    XalanSourceTreeText(const XalanSourceTreeText&);

    XalanSourceTreeText&
    operator=(const XalanSourceTreeText&    theSource);

    bool
    operator==(const XalanSourceTreeText&   theRHS) const;


    // Data members...
    const XalanDOMString&           m_data;

    XalanNode*                      m_parentNode;

    XalanNode*                      m_previousSibling;

    XalanNode*                      m_nextSibling;

    IndexType                       m_index;

    static const XalanDOMString&    s_nameString;
};



XALAN_CPP_NAMESPACE_END



#endif  // !defined(XALANSOURCETREETEXT_HEADER_GUARD_1357924680)
