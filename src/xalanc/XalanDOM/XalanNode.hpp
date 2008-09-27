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
#if !defined(XALANNODE_HEADER_GUARD_1357924680)
#define XALANNODE_HEADER_GUARD_1357924680



#include <xalanc/XalanDOM/XalanDOMDefinitions.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XalanDOMString;
class XalanDocument;
class XalanNamedNodeMap;
class XalanNodeList;



class XALAN_DOM_EXPORT XalanNode
{
public:

    XalanNode();

    virtual
    ~XalanNode();

    enum NodeType
    {
        UNKNOWN_NODE                = 0,
        ELEMENT_NODE                = 1,
        ATTRIBUTE_NODE              = 2,
        TEXT_NODE                   = 3,
        CDATA_SECTION_NODE          = 4,
        ENTITY_REFERENCE_NODE       = 5,
        ENTITY_NODE                 = 6,
        PROCESSING_INSTRUCTION_NODE = 7,
        COMMENT_NODE                = 8,
        DOCUMENT_NODE               = 9,
        DOCUMENT_TYPE_NODE          = 10,
        DOCUMENT_FRAGMENT_NODE      = 11,
        NOTATION_NODE               = 12
    };

    typedef unsigned long   IndexType;

    /**
     * Gets the name of this node, depending on its type.
     */
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
     * If there is no such node, this returns a null pointer.
     */
    virtual XalanNode*
    getParentNode() const = 0;

    /**
     * Gets a XalanNodeList that contains all children of this node.
     *
     * If there are no children, this is a XalanNodeList containing no nodes.
     */
    virtual const XalanNodeList*
    getChildNodes() const = 0;

    /**
     * Gets the first child of this node.
     *
     * If there is no such node, this returns a null pointer.
     */
    virtual XalanNode*
    getFirstChild() const = 0;

    /**
     * Gets the last child of this node.
     *
     * If there is no such node, this returns a null pointer.
     */
    virtual XalanNode*
    getLastChild() const = 0;

    /**
     * Gets the node immediately preceding this node.
     *
     * If there is no such node, this returns a null pointer.
     */
    virtual XalanNode*
    getPreviousSibling() const = 0;

    /**
     * Gets the node immediately following this node.
     *
     * If there is no such node, this returns a null pointer.
     */
    virtual XalanNode*
    getNextSibling() const = 0;

    /**
     * Gets a XalanNamedNodeMap containing the attributes of this node (if it
     * is an XalanElement) or a null pointer otherwise.
     */
    virtual const XalanNamedNodeMap*
    getAttributes() const = 0;

    /**
     * Gets the XalanDocument object associated with this node.  This may
     * be a null pointer.
     */
    virtual XalanDocument*
    getOwnerDocument() const = 0;

    /**
     * Get the namespace URI of this node.
     */
    virtual const XalanDOMString&
    getNamespaceURI() const = 0;

    /**
     * Get the namespace prefix of this node.
     */
    virtual const XalanDOMString&
    getPrefix() const = 0;

    /**
     * Returns the local part of the qualified name of this node.
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

protected:

    XalanNode(const XalanNode&  theSource);

    XalanNode&
    operator=(const XalanNode&  theSource);

    bool
    operator==(const XalanNode&     theRHS) const;

private:

#if !defined(NDEBUG)
    static size_t   s_instanceCount;
#endif
};



XALAN_CPP_NAMESPACE_END



#endif  // !defined(XALANNODE_HEADER_GUARD_1357924680)
