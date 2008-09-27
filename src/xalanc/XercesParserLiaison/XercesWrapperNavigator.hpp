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
#if !defined(XERCESWRAPPERNAVIGATOR_HEADER_GUARD_1357924680)
#define XERCESWRAPPERNAVIGATOR_HEADER_GUARD_1357924680



#include <xalanc/XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



#include <cassert>



#include <xercesc/util/XercesDefs.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>
#include <xalanc/XalanDOM/XalanNode.hpp>



#include <xalanc/XercesParserLiaison/XercesWrapperTypes.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XercesDocumentWrapper;
class XalanAttr;
class XalanElement;
class XalanText;



class XALAN_XERCESPARSERLIAISON_EXPORT XercesWrapperNavigator
{
public:

    typedef XalanNode::IndexType    IndexType;

    explicit
    XercesWrapperNavigator(XercesDocumentWrapper*   theOwnerDocument = 0);

    XercesWrapperNavigator(const XercesWrapperNavigator&    theSource);

    virtual
    ~XercesWrapperNavigator();


    XercesDocumentWrapper*
    getOwnerDocument() const
    {
        return m_ownerDocument;
    }

    void
    setOwnerDocument(XercesDocumentWrapper*     theDocument)
    {
        m_ownerDocument = theDocument;
    }

    XalanNode*
    mapNode(const DOMNodeType*  theXercesNode) const;

    XalanAttr*
    mapNode(const DOMAttrType*  theXercesNode) const;

    const DOMNodeType*
    mapNode(XalanNode*  theXalanNode) const;

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
    getParentNode(const DOMNodeType*    theXercesNode) const;

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
    getPreviousSibling(const DOMNodeType*   theXercesNode) const;

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
    getNextSibling(const DOMNodeType*   theXercesNode) const;

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
    getFirstChild(const DOMNodeType*    theXercesNode) const;

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
    getLastChild(const DOMNodeType*     theXercesNode) const;

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

    XalanElement*
    getOwnerElement(const DOMAttrType*  theXercesAttr) const;

    /**
     * Get a pooled string.  If the string is not in the pool,
     * add it.
     *
     * @param theString The string to pool.
     * @return A const reference to the pooled string.
     */
    const XalanDOMString&
    getPooledString(const XMLCh*    theString) const;

private:

    // Not implemented...
    bool
    operator==(const XercesWrapperNavigator&    theRHS) const;

    // Data members...
    XercesDocumentWrapper*  m_ownerDocument;

    XalanNode*              m_parentNode;

    XalanNode*              m_previousSibling;

    XalanNode*              m_nextSibling;

    XalanNode*              m_firstChild;

    XalanNode*              m_lastChild;

    IndexType               m_index;

    static const XalanDOMString     s_emptyString;
};



XALAN_CPP_NAMESPACE_END



#endif  // !defined(XERCESWRAPPERNAVIGATOR_HEADER_GUARD_1357924680)
