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
#if !defined(XALANSOURCETREEATTR_HEADER_GUARD_1357924680)
#define XALANSOURCETREEATTR_HEADER_GUARD_1357924680



#include <xalanc/XalanSourceTree/XalanSourceTreeDefinitions.hpp>



#include <xalanc/XalanDOM/XalanAttr.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XalanSourceTreeDocument;
class XalanSourceTreeElement;



/*
 * <meta name="usage" content="experimental"/>
 *
 * Base class for the Xalan source tree Attr interface.
 *
 * This class is experimental and subject to change!!
 */

class XALAN_XALANSOURCETREE_EXPORT XalanSourceTreeAttr : public XalanAttr
{
public:

    /**
     * Constructor.
     *
     * @param theName The name of the attribute
     * @param theValue The value of the attribute
     * @param theOwnerDocument The document that owns the instance
     * @param theOwnerElement The element that owns the instance
     * @param theIndex The document-order index of the node.
     */
    XalanSourceTreeAttr(
            const XalanDOMString&       theName,
            const XalanDOMString&       theValue,
            XalanSourceTreeElement*     theOwnerElement = 0,
            IndexType                   theIndex = 0);

    virtual
    ~XalanSourceTreeAttr();


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

    // These interfaces are inherited from XalanAttr...
    virtual const XalanDOMString&
    getName() const;

    virtual bool
    getSpecified() const;

    virtual const XalanDOMString&
    getValue() const;

    virtual XalanElement*
    getOwnerElement() const;

    // public interfaces not inherited from XalanAttr...

    void
    setOwnerElement(XalanSourceTreeElement*     theElement)
    {
        m_ownerElement = theElement;
    }

    void
    setIndex(IndexType  theIndex)
    {
        m_index = theIndex;
    }

private:

    // Not defined...
    XalanSourceTreeAttr(const XalanSourceTreeAttr&      theSource);

    XalanSourceTreeAttr&
    operator=(const XalanSourceTreeAttr&    theSource);

    bool
    operator==(const XalanSourceTreeAttr&   theRHS) const;


    // Data members...
    const XalanDOMString&       m_name;

    const XalanDOMString&       m_value;

    XalanSourceTreeElement*     m_ownerElement;

    IndexType                   m_index;
};



XALAN_CPP_NAMESPACE_END



#endif  // !defined(XALANSOURCETREEATTR_HEADER_GUARD_1357924680)
