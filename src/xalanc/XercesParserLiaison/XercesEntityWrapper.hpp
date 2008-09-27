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
#if !defined(XERCESENTITYWRAPPER_HEADER_GUARD_1357924680)
#define XERCESENTITYWRAPPER_HEADER_GUARD_1357924680



#include <xalanc/XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>


#include <xalanc/Include/XalanMemMgrAutoPtr.hpp>


#include <xalanc/XalanDOM/XalanEntity.hpp>



#include <xalanc/XercesParserLiaison/XercesNodeListWrapper.hpp>
#include <xalanc/XercesParserLiaison/XercesWrapperTypes.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XercesWrapperNavigator;



class XALAN_XERCESPARSERLIAISON_EXPORT XercesEntityWrapper : public XalanEntity
{
public:

    XercesEntityWrapper(
            const DOMEntityType*            theXercesDOMEntity,
            const XercesWrapperNavigator&   theNavigator);

    static XercesEntityWrapper*
    create( MemoryManager& theManager,
            const DOMEntityType*            theXercesDOMEntity,
            const XercesWrapperNavigator&   theNavigator);

    virtual
    ~XercesEntityWrapper();


    // These interfaces are inherited from XalanEntity...

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
    getPublicId() const;

    virtual const XalanDOMString&
    getSystemId() const;

    virtual const XalanDOMString&
    getNotationName() const;

    const DOMEntityType*
    getXercesNode() const
    {
        return m_xercesNode;
    }

private:

    // Not implemented...
    XercesEntityWrapper(const XercesEntityWrapper&  theSource);

    XercesEntityWrapper&
    operator=(const XercesEntityWrapper&        theSource);

    bool
    operator==(const XercesEntityWrapper&       theRHS) const;

    // Data members...
    const DOMEntityType* const      m_xercesNode;

    const XercesWrapperNavigator&   m_navigator;

    XercesNodeListWrapper           m_children;
};



XALAN_CPP_NAMESPACE_END



#endif  // !defined(XERCESENTITYWRAPPER_HEADER_GUARD_1357924680)
