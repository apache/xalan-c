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
#if !defined(XALANELEMENT_HEADER_GUARD_1357924680)
#define XALANELEMENT_HEADER_GUARD_1357924680



#include <xalanc/XalanDOM/XalanDOMDefinitions.hpp>
#include <xalanc/XalanDOM/XalanNode.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XalanAttr;



/*
 * <meta name="usage" content="experimental"/>
 *
 * Base class for the DOM Element interface.
 *
 * This class is experimental and subject to change!!
 */

class XALAN_DOM_EXPORT XalanElement : public XalanNode
{
public:

    XalanElement();

    virtual
    ~XalanElement();

    // These interfaces are inherited from XalanNode
    virtual const XalanDOMString&
    getNodeName() const = 0;

    /**
     * Gets the value of this node, depending on its type.
     */
    virtual const XalanDOMString&
    getNodeValue() const = 0;

    /**
     * An enum value representing the type of the underlying object.
     */
    virtual NodeType
    getNodeType() const = 0;

    /**
     * Gets the parent of this node.
     *
     * All nodes, except <code>Document</code>,
     * <code>DocumentFragment</code>, and <code>Attr</code> may have a parent.
     * However, if a node has just been created and not yet added to the tree,
     * or if it has been removed from the tree, a <code>null</code> Node
     * is returned.
     */
    virtual XalanNode*
    getParentNode() const = 0;

    /**
     * Gets a <code>NodeList</code> that contains all children of this node.
     *
     * If there are no children, this is a <code>NodeList</code> containing no nodes.
     */
    virtual const XalanNodeList*
    getChildNodes() const = 0;

    /**
     * Gets the first child of this node.
     *
     * If there is no such node, this returns <code>null</code>.
     */
    virtual XalanNode*
    getFirstChild() const = 0;

    /**
     * Gets the last child of this node.
     *
     * If there is no such node, this returns <code>null</code>.
     */
    virtual XalanNode*
    getLastChild() const = 0;

    /**
     * Gets the node immediately preceding this node.
     *
     * If there is no such node, this returns <code>null</code>.
     */
    virtual XalanNode*
    getPreviousSibling() const = 0;

    /**
     * Gets the node immediately following this node.
     *
     * If there is no such node, this returns <code>null</code>.
     */
    virtual XalanNode*
    getNextSibling() const = 0;

    /**
     * Gets a <code>NamedNodeMap</code> containing the attributes of this node (if it
     * is an <code>Element</code>) or <code>null</code> otherwise.
     */
    virtual const XalanNamedNodeMap*
    getAttributes() const = 0;

    /**
     * Gets the <code>Document</code> object associated with this node.
     *
     * This is also
     * the <code>Document</code> object used to create new nodes. When this
     * node is a <code>Document</code> or a <code>DocumentType</code>
     * which is not used with any <code>Document</code> yet, this is
     * <code>null</code>.
     */
    virtual XalanDocument*
    getOwnerDocument() const = 0;

    /**
     * Get the <em>namespace URI</em> of
     * this node, or <code>null</code> if it is unspecified.
     * <p>
     * This is not a computed value that is the result of a namespace lookup
     * based on an examination of the namespace declarations in scope. It is
     * merely the namespace URI given at creation time.
     * <p>
     * For nodes of any type other than <CODE>ELEMENT_NODE</CODE> and 
     * <CODE>ATTRIBUTE_NODE</CODE> and nodes created with a DOM Level 1 method, 
     * such as <CODE>createElement</CODE> from the <CODE>Document</CODE>
     * interface, this is always <CODE>null</CODE>.
     */
    virtual const XalanDOMString&
    getNamespaceURI() const = 0;

    /**
     * Get the <em>namespace prefix</em>
     * of this node, or <code>null</code> if it is unspecified.
     */
    virtual const XalanDOMString&
    getPrefix() const = 0;

    /**
     * Returns the local part of the <em>qualified name</em> of this node.
     * <p>
     * For nodes created with a DOM Level 1 method, such as
     * <code>createElement</code> from the <code>Document</code> interface,
     * it is null.
     */
    virtual const XalanDOMString&
    getLocalName() const = 0;

    /**
     * Determine if the document is node-order indexed.
     *
     * @return true if the document is indexed, otherwise false.
     */
    virtual bool
    isIndexed() const = 0;

    /**
     * Get the node's index.  Valid only if the owner document
     * reports that the document is node-order indexed.
     *
     * @return The index value, or 0 if the node is not indexed.
     */
    virtual IndexType
    getIndex() const = 0;

    // These interfaces are new to XalanElement...

    /**
     * The name of the element. 
     *
     * For example, in: &lt;elementExample 
     * id="demo"&gt;  ... &lt;/elementExample&gt; , <code>tagName</code> has 
     * the value <code>"elementExample"</code>. Note that this is 
     * case-preserving in XML, as are all of the operations of the DOM. 
     */
    virtual const XalanDOMString&
    getTagName() const = 0;

protected:

    XalanElement(const XalanElement&    theSource);

    XalanElement&
    operator=(const XalanElement&   theSource);

    bool
    operator==(const XalanElement&  theRHS) const;

private:
};



XALAN_CPP_NAMESPACE_END



#endif  // !defined(XALANELEMENT_HEADER_GUARD_1357924680)
