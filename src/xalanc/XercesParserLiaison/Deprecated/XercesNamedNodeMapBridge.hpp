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
#if !defined(XERCESNAMEDNODEMAPBRIDGE_HEADER_GUARD_1357924680)
#define XERCESNAMEDNODEMAPBRIDGE_HEADER_GUARD_1357924680



#include <xalanc/XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



#if XERCES_VERSION_MAJOR >= 2
#include <xercesc/dom/deprecated/DOM_NamedNodeMap.hpp>
#else
#include <xercesc/dom/DOM_NamedNodeMap.hpp>
#endif



#include <xalanc/XalanDOM/XalanNamedNodeMap.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XercesBridgeNavigator;
class XercesNodeBridge;


/**
 * This class is deprecated.
 *
 * @deprecated This class is part of the deprecated Xerces DOM bridge.
 */
class XALAN_XERCESPARSERLIAISON_EXPORT XercesNamedNodeMapBridge : public XalanNamedNodeMap
{
public:

    typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_NamedNodeMap     DOM_NamedNodeMapType;

    XercesNamedNodeMapBridge(
            const DOM_NamedNodeMapType&     theXercesNamedNodeMap,
            const XercesBridgeNavigator&    theNavigator);

    virtual
    ~XercesNamedNodeMapBridge();

    /** @name Set functions. */
    //@{

    /**
     * Adds a node using its <code>nodeName</code> attribute. 
     *
     * <br>As the <code>nodeName</code> attribute is used to derive the name 
     * which the node must be stored under, multiple nodes of certain types 
     * (those that have a "special" string value) cannot be stored as the names 
     * would clash. This is seen as preferable to allowing nodes to be aliased.
     * @param arg A node to store in a named node map. The node will later be 
     *   accessible using the value of the <code>nodeName</code> attribute of 
     *   the node. If a node with that name is already present in the map, it 
     *   is replaced by the new one.
     * @return If the new <code>Node</code> replaces an existing node the
     *   replaced <code>Node</code> is returned, 
     *   otherwise <code>null</code> is returned.
     * @exception DOMException
     *   WRONG_DOCUMENT_ERR: Raised if <code>arg</code> was created from a 
     *   different document than the one that created the 
     *   <code>NamedNodeMap</code>.
     *   <br>NO_MODIFICATION_ALLOWED_ERR: Raised if this 
     *   <code>NamedNodeMap</code> is readonly.
     *   <br>INUSE_ATTRIBUTE_ERR: Raised if <code>arg</code> is an 
     *   <code>Attr</code> that is already an attribute of another 
     *   <code>Element</code> object. The DOM user must explicitly clone 
     *   <code>Attr</code> nodes to re-use them in other elements.
     */
    virtual XalanNode*
    setNamedItem(XalanNode*     arg);

    /**
     * Returns the <code>index</code>th item in the map. 
     *
     * If <code>index</code> 
     * is greater than or equal to the number of nodes in the map, this returns 
     * <code>null</code>.
     * @param index Index into the map.
     * @return The node at the <code>index</code>th position in the 
     *   <code>NamedNodeMap</code>, or <code>null</code> if that is not a valid 
     *   index.
     */
    virtual XalanNode*
    item(unsigned int   index) const;

    //@}
    /** @name Get functions. */
    //@{

    /**
     * Retrieves a node specified by name.
     *
     * @param name The <code>nodeName</code> of a node to retrieve.
     * @return A <code>DOM_Node</code> (of any type) with the specified <code>nodeName</code>, or 
     *   <code>null</code> if it does not identify any node in 
     *   the map. 
     */
    virtual XalanNode*
    getNamedItem(const XalanDOMString&  name) const;

    /**
     * The number of nodes in the map. 
     *
     * The range of valid child node indices is 
     * 0 to <code>length-1</code> inclusive. 
     */
    virtual unsigned int
    getLength() const;

    //@}
    /** @name Functions to change the node collection. */
    //@{

    /**
    * Removes a node specified by name.
    *
    * If the removed node is an 
    * <code>Attr</code> with a default value it is immediately replaced.
    * @param name The <code>nodeName</code> of a node to remove.
    * @return The node removed from the map or <code>null</code> if no node 
    *   with such a name exists.
    * @exception DOMException
    *   NOT_FOUND_ERR: Raised if there is no node named <code>name</code> in 
    *   the map.
    * <br>
    *   NO_MODIFICATION_ALLOWED_ERR: Raised if this <code>NamedNodeMap</code>
    *   is readonly.
    */
    virtual XalanNode*
    removeNamedItem(const XalanDOMString&   name);

    //@}
    /** @name Functions introduced in DOM Level 2. */
    //@{

    /**
     * Retrieves a node specified by local name and namespace URI.
     *
     * @param namespaceURI The <em>namespace URI</em> of
     *    the node to retrieve.
     * @param localName The <em>local name</em> of the node to retrieve.
     * @return A <code>DOM_Node</code> (of any type) with the specified
     *    local name and namespace URI, or <code>null</code> if they do not
     *    identify any node in the map.
     */
    virtual XalanNode*
    getNamedItemNS(
            const XalanDOMString&   namespaceURI,
            const XalanDOMString&   localName) const;

    /**
     * Adds a node using its <CODE>namespaceURI</CODE> and <CODE>localName</CODE>.
     * @param arg A node to store in a named node map. The node will later be 
     *       accessible using the value of the <CODE>namespaceURI</CODE> and 
     *       <CODE>localName</CODE> attribute of the node. If a node with those 
     *       namespace URI and local name is already present in the map, it is 
     *       replaced by the new one.
     * @return If the new <code>Node</code> replaces an existing node the
     *   replaced <code>Node</code> is returned, 
     *   otherwise <code>null</code> is returned.
     * @exception DOMException
     *   WRONG_DOCUMENT_ERR: Raised if <code>arg</code> was created from a 
     *   different document than the one that created the 
     *   <code>NamedNodeMap</code>.
     *   <br>NO_MODIFICATION_ALLOWED_ERR: Raised if this 
     *   <code>NamedNodeMap</code> is readonly.
     *   <br>INUSE_ATTRIBUTE_ERR: Raised if <code>arg</code> is an 
     *   <code>Attr</code> that is already an attribute of another 
     *   <code>Element</code> object. The DOM user must explicitly clone 
     *   <code>Attr</code> nodes to re-use them in other elements.
     */
    virtual XalanNode*
    setNamedItemNS(XalanNode*   arg);

    /**
     * Removes a node specified by local name and namespace URI.
     *
     * @param namespaceURI The <em>namespace URI</em> of
     *    the node to remove.
     * @param localName The <em>local name</em> of the
     *    node to remove. When this <code>DOM_NamedNodeMap</code> contains the
     *    attributes attached to an element, as returned by the attributes
     *    attribute of the <code>DOM_Node</code> interface, if the removed
     *    attribute is known to have a default value, an attribute
     *    immediately appears containing the default value
     *    as well as the corresponding namespace URI, local name, and prefix.
     * @return The node removed from the map if a node with such a local name
     *    and namespace URI exists.
     * @exception DOMException
     *   NOT_FOUND_ERR: Raised if there is no node named <code>name</code> in 
     *   the map.
     * <br>
     *   NO_MODIFICATION_ALLOWED_ERR: Raised if this <code>NamedNodeMap</code>
     *   is readonly.
     */
    virtual XalanNode*
    removeNamedItemNS(
            const XalanDOMString&   namespaceURI,
            const XalanDOMString&   localName);

    //@}

private:

    // Not implemented...
    XercesNamedNodeMapBridge(const XercesNamedNodeMapBridge&    theSource);

    XercesNamedNodeMapBridge&
    operator=(const XercesNamedNodeMapBridge&   theRHS);

    bool
    operator==(const XercesNamedNodeMapBridge&  theRHS) const;


    // $$$ ToDo:  Eventually, this will not need to be mutable.
    mutable DOM_NamedNodeMapType    m_xercesNamedNodeMap;

    const XercesBridgeNavigator&    m_navigator;
};



XALAN_CPP_NAMESPACE_END



#endif  // !defined(XERCESNAMEDNODEMAPBRIDGE_HEADER_GUARD_1357924680)
