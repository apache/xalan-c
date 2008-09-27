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
#if !defined(XALANATTR_HEADER_GUARD_1357924680)
#define XALANATTR_HEADER_GUARD_1357924680



#include <xalanc/XalanDOM/XalanDOMDefinitions.hpp>
#include <xalanc/XalanDOM/XalanNode.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XalanElement;



/*
 * Base class for the DOM Attr interface.
 */

class XALAN_DOM_EXPORT XalanAttr : public XalanNode
{
public:

    XalanAttr();

    virtual
    ~XalanAttr();


    // These interfaces are inherited from XalanNode...
    virtual const XalanDOMString&
    getNodeName() const = 0;

    virtual const XalanDOMString&
    getNodeValue() const = 0;

    virtual NodeType
    getNodeType() const = 0;

    virtual XalanNode*
    getParentNode() const = 0;

    virtual const XalanNodeList*
    getChildNodes() const = 0;

    virtual XalanNode*
    getFirstChild() const = 0;

    virtual XalanNode*
    getLastChild() const = 0;

    virtual XalanNode*
    getPreviousSibling() const = 0;

    virtual XalanNode*
    getNextSibling() const = 0;

    virtual const XalanNamedNodeMap*
    getAttributes() const = 0;

    virtual XalanDocument*
    getOwnerDocument() const = 0;

    virtual const XalanDOMString&
    getNamespaceURI() const = 0;

    virtual const XalanDOMString&
    getPrefix() const = 0;

    virtual const XalanDOMString&
    getLocalName() const = 0;

    virtual bool
    isIndexed() const = 0;

    virtual IndexType
    getIndex() const = 0;

    // These interfaces are new to XalanAttr...

   /**
    * Returns the name of this attribute. 
    */
    virtual const XalanDOMString&
    getName() const = 0;

    /**
     * Returns the value of the attribute.
     *
     * The value of the attribute is returned as a string. 
     */
    virtual const XalanDOMString&
    getValue() const = 0;

    /**
     * The XalanElement node this attribute is attached to.
     * If there is no such node, this returns a null pointer.
     */
    virtual XalanElement*
    getOwnerElement() const = 0;

protected:

    XalanAttr(const XalanAttr&  theSource);

    XalanAttr&
    operator=(const XalanAttr&  theSource);

    bool
    operator==(const XalanAttr&     theRHS) const;

private:
};



XALAN_CPP_NAMESPACE_END



#endif  // !defined(XALANATTR_HEADER_GUARD_1357924680)
