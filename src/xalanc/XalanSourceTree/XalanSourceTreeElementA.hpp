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
#if !defined(XALANSOURCETREEELEMENTA_HEADER_GUARD_1357924680)
#define XALANSOURCETREEELEMENTA_HEADER_GUARD_1357924680



#include <xalanc/XalanSourceTree/XalanSourceTreeDefinitions.hpp>



#include <xalanc/XalanDOM/XalanNamedNodeMap.hpp>



#include <xalanc/XalanSourceTree/XalanSourceTreeElement.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XALANSOURCETREE_EXPORT XalanSourceTreeElementA : public XalanSourceTreeElement, private XalanNamedNodeMap
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
    XalanSourceTreeElementA(
            MemoryManager&          theManager,
            const XalanDOMString&       theTagName,
            XalanSourceTreeDocument*    theOwnerDocument,
            XalanSourceTreeAttr**       theAttributes,
            XalanSize_t                 theAttributeCount,
            XalanNode*                  theParentNode = 0,
            XalanNode*                  thePreviousSibling = 0,
            XalanNode*                  theNextSibling = 0,
            IndexType                   theIndex = 0);

    virtual
    ~XalanSourceTreeElementA();


    virtual const XalanNamedNodeMap*
    getAttributes() const;

    virtual const XalanDOMString&
    getNamespaceURI() const;

    virtual const XalanDOMString&
    getPrefix() const;

    virtual const XalanDOMString&
    getLocalName() const;


    // public interfaces not inherited from XalanElement...

    XalanSize_t
    getAttributeCount() const
    {
        return m_attributeCount;
    }

    XalanSourceTreeAttr*
    getAttributeByIndex(XalanSize_t     index) const
    {
        return index < m_attributeCount ? m_attributes[index] : 0;
    }

private:

    // These are from XalanNamedNodeMap...

    virtual XalanNode*
    item(XalanSize_t    index) const;

    virtual XalanNode*
    getNamedItem(const XalanDOMString&  name) const;

    virtual XalanSize_t
    getLength() const;

    virtual XalanNode*
    getNamedItemNS(
            const XalanDOMString&   namespaceURI,
            const XalanDOMString&   localName) const;


    // Not implemented...
    XalanSourceTreeElementA(const XalanSourceTreeElementA&);

    XalanSourceTreeElementA&
    operator=(const XalanSourceTreeElementA&);

    bool
    operator==(const XalanSourceTreeElementA&) const;


    // Data members...
    XalanSourceTreeAttr* const *    m_attributes;

    const XalanSize_t               m_attributeCount;
};



XALAN_CPP_NAMESPACE_END



#endif  // !defined(XALANSOURCETREEELEMENTA_HEADER_GUARD_1357924680)
