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
#if !defined(XERCESBRIDGENAVIGATOR_HEADER_GUARD_1357924680)
#define XERCESBRIDGENAVIGATOR_HEADER_GUARD_1357924680



#include <xalanc/XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



#include <cassert>



#include <xalanc/XalanDOM/XalanDOMString.hpp>
#include <xalanc/XalanDOM/XalanNode.hpp>



#include <xalanc/XercesParserLiaison/Deprecated/XercesBridgeTypes.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XercesDocumentBridge;
class XalanAttr;
class XalanElement;
class XalanText;


/**
 * This class is deprecated.
 *
 * @deprecated This class is part of the deprecated Xerces DOM bridge.
 */
class XALAN_XERCESPARSERLIAISON_EXPORT XercesBridgeNavigator
{
public:

    typedef XalanNode::IndexType    IndexType;

    explicit
    XercesBridgeNavigator(
            XercesDocumentBridge*   theOwnerDocument = 0,
            bool                    mappingMode = true);

    XercesBridgeNavigator(const XercesBridgeNavigator&  theSource);

    virtual
    ~XercesBridgeNavigator();


    XercesDocumentBridge*
    getOwnerDocument() const
    {
        return m_ownerDocument;
    }

    void
    setOwnerDocument(XercesDocumentBridge*  theDocument)
    {
        m_ownerDocument = theDocument;
    }

    XalanNode*
    mapNode(const DOM_NodeType&     theXercesNode) const;

    XalanAttr*
    mapNode(const DOM_AttrType&     theXercesNode) const;

    DOM_NodeType
    mapNode(const XalanNode*    theXalanNode) const;

    DOM_AttrType
    mapNode(const XalanAttr*    theXercesNode) const;

    IndexType
    getIndex() const
    {
        return m_index;
    }

    void
    setIndex(IndexType  theIndex)
    {
        m_index = theIndex;
    }

    XalanNode*
    getParentNode(const DOM_NodeType&   theXercesNode) const;

    XalanNode*
    getParentNode() const
    {
        return m_parentNode;
    }

    void
    setParentNode(XalanNode*    theParent)
    {
        m_parentNode = theParent;
    }

    XalanNode*
    getPreviousSibling(const DOM_NodeType&  theXercesNode) const;

    XalanNode*
    getPreviousSibling() const
    {
        return m_previousSibling;
    }

    void
    setPreviousSibling(XalanNode*   thePreviousSibling)
    {
        m_previousSibling = thePreviousSibling;
    }

    XalanNode*
    getNextSibling(const DOM_NodeType&  theXercesNode) const;

    XalanNode*
    getNextSibling() const
    {
        return m_nextSibling;
    }

    void
    setNextSibling(XalanNode*   theNextSibling)
    {
        m_nextSibling = theNextSibling;
    }

    XalanNode*
    getFirstChild(const DOM_NodeType&   theXercesNode) const;

    XalanNode*
    getFirstChild() const
    {
        return m_firstChild;
    }

    void
    setFirstChild(XalanNode*    theFirstChild)
    {
        m_firstChild = theFirstChild;
    }

    XalanNode*
    getLastChild(const DOM_NodeType&    theXercesNode) const;

    XalanNode*
    getLastChild() const
    {
        return m_lastChild;
    }

    void
    setLastChild(XalanNode* theLastChild)
    {
        m_lastChild = theLastChild;
    }

    XalanNode*
    insertBefore(
            DOM_NodeType&   theXercesParent,
            XalanNode*      newChild,
            XalanNode*      refChild) const;

    XalanNode*
    replaceChild(
            DOM_NodeType&   theXercesParent,
            XalanNode*      newChild,
            XalanNode*      oldChild) const;

    XalanNode*
    removeChild(
            DOM_NodeType&   theXercesParent,
            XalanNode*      oldChild) const;

    XalanNode*
    appendChild(
            DOM_NodeType&   theXercesParent,
            XalanNode*      newChild) const;

    XalanElement*
    getOwnerElement(const DOM_AttrType&     theXercesAttr) const;

    XalanNode*
    cloneNode(
            const XalanNode*        theXalanNode,
            const DOM_NodeType&     theXercesNode,
            bool                    deep) const;

    XalanText*
    splitText(
            DOM_TextType&   theXercesText,
            unsigned int    offset) const;

    /**
     * Get a pooled string.  If the string is not in the pool,
     * add it.
     *
     * @param theString The string to pool.
     * @return A const reference to the pooled string.
     */
    const XalanDOMString&
    getPooledString(const DOMStringType&    theString) const;

private:

    // Not implemented...
    bool
    operator==(const XercesBridgeNavigator& theRHS) const;

    // Data members...
    XercesDocumentBridge*   m_ownerDocument;

    mutable XalanNode*      m_parentNode;

    mutable XalanNode*      m_previousSibling;

    mutable XalanNode*      m_nextSibling;

    mutable XalanNode*      m_firstChild;

    mutable XalanNode*      m_lastChild;

    IndexType               m_index;
};



XALAN_CPP_NAMESPACE_END



#endif  // !defined(XERCESBRIDGENAVIGATOR_HEADER_GUARD_1357924680)
