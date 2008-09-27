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
#if !defined(XERCESATTRWRAPPER_HEADER_GUARD_1357924680)
#define XERCESATTRWRAPPER_HEADER_GUARD_1357924680



#include <xalanc/XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



#include <xercesc/dom/DOMAttr.hpp>



#include <xalanc/XalanDOM/XalanAttr.hpp>



#include <xalanc/XercesParserLiaison/XercesNodeListWrapper.hpp>
#include <xalanc/XercesParserLiaison/XercesWrapperTypes.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XercesWrapperNavigator;
class XalanElement;



class XALAN_XERCESPARSERLIAISON_EXPORT XercesAttrWrapper : public XalanAttr
{
public:

    XercesAttrWrapper(
            const DOMAttrType*              theXercesAttr,
            const XercesWrapperNavigator&   theNavigator);

    virtual
    ~XercesAttrWrapper();


    // These interfaces are inherited from XalanAttr...

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
    getName() const;

    virtual bool
    getSpecified() const;

    virtual const XalanDOMString&
    getValue() const;

    virtual XalanElement*
    getOwnerElement() const;

    /**
     * Get the Xerces node this instance represent.
     *
     * @return The Xerces node
     */
    const DOMAttrType*
    getXercesNode() const
    {
        return m_xercesNode;
    }

private:

    // Not implemented...
    XercesAttrWrapper(const XercesAttrWrapper&  theSource);

    XercesAttrWrapper&
    operator=(const XercesAttrWrapper&  theSource);

    bool
    operator==(const XercesAttrWrapper&     theRHS) const;

    // Data members...
    const DOMAttrType* const        m_xercesNode;

    XercesNodeListWrapper           m_children;

    const XercesWrapperNavigator&   m_navigator;
};



XALAN_CPP_NAMESPACE_END



#endif  // !defined(XERCESATTRBRIDGE_HEADER_GUARD_1357924680)
