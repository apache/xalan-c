/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */
#if !defined(XALANSOURCETREENAMEDNODEMAP_HEADER_GUARD_1357924680)
#define XALANSOURCETREENAMEDNODEMAP_HEADER_GUARD_1357924680



#include <XalanSourceTree/XalanSourceTreeDefinitions.hpp>



#include <XalanDOM/XalanNamedNodeMap.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XalanSourceTreeElement;



/*
 * <meta name="usage" content="experimental"/>
 *
 * Base class for the source tree NamedNodeMap interface.
 *
 * This class is experimental and subject to change!!
 */

class XALAN_XALANSOURCETREE_EXPORT XalanSourceTreeNamedNodeMap : public XalanNamedNodeMap
{
public:

	XalanSourceTreeNamedNodeMap(const XalanSourceTreeElement*	theElement);

	XalanSourceTreeNamedNodeMap(const XalanSourceTreeNamedNodeMap&	theSource);

	virtual
	~XalanSourceTreeNamedNodeMap();

	XalanSourceTreeNamedNodeMap&
	operator=(const XalanSourceTreeNamedNodeMap&		theSource);

	bool
	operator==(const XalanSourceTreeNamedNodeMap& 	theRHS) const;


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
	 *	 accessible using the value of the <code>nodeName</code> attribute of 
	 *	 the node. If a node with that name is already present in the map, it 
	 *	 is replaced by the new one.
	 * @return If the new <code>Node</code> replaces an existing node the
	 *	 replaced <code>Node</code> is returned, 
	 *	 otherwise <code>null</code> is returned.
	 * @exception DOMException
	 *	 WRONG_DOCUMENT_ERR: Raised if <code>arg</code> was created from a 
	 *	 different document than the one that created the 
	 *	 <code>NamedNodeMap</code>.
	 *	 <br>NO_MODIFICATION_ALLOWED_ERR: Raised if this 
	 *	 <code>NamedNodeMap</code> is readonly.
	 *	 <br>INUSE_ATTRIBUTE_ERR: Raised if <code>arg</code> is an 
	 *	 <code>Attr</code> that is already an attribute of another 
	 *	 <code>Element</code> object. The DOM user must explicitly clone 
	 *	 <code>Attr</code> nodes to re-use them in other elements.
	 */
	virtual XalanNode*
	setNamedItem(XalanNode* 	arg);

	/**
	 * Returns the <code>index</code>th item in the map. 
	 *
	 * If <code>index</code> 
	 * is greater than or equal to the number of nodes in the map, this returns 
	 * <code>null</code>.
	 * @param index Index into the map.
	 * @return The node at the <code>index</code>th position in the 
	 *	 <code>NamedNodeMap</code>, or <code>null</code> if that is not a valid 
	 *	 index.
	 */
	virtual XalanNode*
	item(unsigned int	index) const;

	//@}
	/** @name Get functions. */
	//@{

	/**
	 * Retrieves a node specified by name.
	 *
	 * @param name The <code>nodeName</code> of a node to retrieve.
	 * @return A <code>Node</code> (of any type) with the specified <code>nodeName</code>, or 
	 *	 <code>null</code> if it does not identify any node in 
	 *	 the map. 
	 */
	virtual XalanNode*
	getNamedItem(const XalanDOMString& 	name) const;

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
	*	with such a name exists.
	* @exception DOMException
	*	NOT_FOUND_ERR: Raised if there is no node named <code>name</code> in 
	*	the map.
	* <br>
	*	NO_MODIFICATION_ALLOWED_ERR: Raised if this <code>NamedNodeMap</code>
	*	is readonly.
	*/
	virtual XalanNode*
	removeNamedItem(const XalanDOMString&	name);

	//@}
	/** @name Functions introduced in DOM Level 2. */
	//@{

	/**
	 * Retrieves a node specified by local name and namespace URI.
	 *
	 * @param namespaceURI The <em>namespace URI</em> of
	 *	  the node to retrieve.
	 * @param localName The <em>local name</em> of the node to retrieve.
	 * @return A <code>Node</code> (of any type) with the specified
	 *	  local name and namespace URI, or <code>null</code> if they do not
	 *	  identify any node in the map.
	 */
	virtual XalanNode*
	getNamedItemNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	localName) const;

	/**
	 * Adds a node using its <CODE>namespaceURI</CODE> and <CODE>localName</CODE>.
	 * @param arg A node to store in a named node map. The node will later be 
	 *		 accessible using the value of the <CODE>namespaceURI</CODE> and 
	 *		 <CODE>localName</CODE> attribute of the node. If a node with those 
	 *		 namespace URI and local name is already present in the map, it is 
	 *		 replaced by the new one.
	 * @return If the new <code>Node</code> replaces an existing node the
	 *	 replaced <code>Node</code> is returned, 
	 *	 otherwise <code>null</code> is returned.
	 * @exception DOMException
	 *	 WRONG_DOCUMENT_ERR: Raised if <code>arg</code> was created from a 
	 *	 different document than the one that created the 
	 *	 <code>NamedNodeMap</code>.
	 *	 <br>NO_MODIFICATION_ALLOWED_ERR: Raised if this 
	 *	 <code>NamedNodeMap</code> is readonly.
	 *	 <br>INUSE_ATTRIBUTE_ERR: Raised if <code>arg</code> is an 
	 *	 <code>Attr</code> that is already an attribute of another 
	 *	 <code>Element</code> object. The DOM user must explicitly clone 
	 *	 <code>Attr</code> nodes to re-use them in other elements.
	 */
	virtual XalanNode*
	setNamedItemNS(XalanNode*	arg);

	/**
	 * Removes a node specified by local name and namespace URI.
	 *
	 * @param namespaceURI The <em>namespace URI</em> of
	 *	  the node to remove.
	 * @param localName The <em>local name</em> of the
	 *	  node to remove. When this <code>NamedNodeMap</code> contains the
	 *	  attributes attached to an element, as returned by the attributes
	 *	  attribute of the <code>Node</code> interface, if the removed
	 *	  attribute is known to have a default value, an attribute
	 *	  immediately appears containing the default value
	 *	  as well as the corresponding namespace URI, local name, and prefix.
	 * @return The node removed from the map if a node with such a local name
	 *	  and namespace URI exists.
	 * @exception DOMException
	 *	 NOT_FOUND_ERR: Raised if there is no node named <code>name</code> in 
	 *	 the map.
	 * <br>
	 *	 NO_MODIFICATION_ALLOWED_ERR: Raised if this <code>NamedNodeMap</code>
	 *	 is readonly.
	 */
	virtual XalanNode*
	removeNamedItemNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	localName);

	//@}

private:

	const XalanSourceTreeElement*	m_element;
};



XALAN_CPP_NAMESPACE_END



#endif	// !defined(XALANSOURCETREENAMEDNODEMAP_HEADER_GUARD_1357924680)
