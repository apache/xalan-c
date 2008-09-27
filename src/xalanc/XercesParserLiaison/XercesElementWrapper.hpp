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
#if !defined(XERCESELEMENTWRAPPER_HEADER_GUARD_1357924680)
#define XERCESELEMENTWRAPPER_HEADER_GUARD_1357924680



#include <xalanc/XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



#include <xalanc/XalanDOM/XalanElement.hpp>
#include <xalanc/XalanDOM/XalanNodeList.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



#include <xalanc/XercesParserLiaison/XercesNamedNodeMapWrapper.hpp>
#include <xalanc/XercesParserLiaison/XercesWrapperTypes.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XercesWrapperNavigator;



class XALAN_XERCESPARSERLIAISON_EXPORT XercesElementWrapper : public XalanElement, private XalanNodeList
{
public:

    XercesElementWrapper(
            const DOMElementType*           theXercesElement,
            const XercesWrapperNavigator&   theNavigator);

    virtual
    ~XercesElementWrapper();


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
    getTagName() const;

    /**
     * Get the Xerces node this instance represent.
     *
     * @return The Xerces node
     */
    const DOMElementType*
    getXercesNode() const
    {
        return m_xercesNode;
    }

    //@}

private:

    // Implementation of XalanNodeList for children.
    virtual XalanNode*
    item(XalanSize_t    index) const;

    virtual XalanSize_t
    getLength() const;


    // Not implemented...
    XercesElementWrapper(const XercesElementWrapper&    theSource);

    XalanNode&
    operator=(const XalanNode&  theSource);

    bool
    operator==(const XercesElementWrapper&  theRHS) const;

    // Data members...
    const DOMElementType* const     m_xercesNode;

    const XercesWrapperNavigator&   m_navigator;

    XercesNamedNodeMapWrapper       m_attributes;
};



XALAN_CPP_NAMESPACE_END



#endif  // !defined(XERCESELEMENTWRAPPER_HEADER_GUARD_1357924680)
