/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
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
#if !defined(RESULTTREEFRAGBASE_HEADER_GUARD_1357924680)
#define RESULTTREEFRAGBASE_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



#include <dom/DOM_Node.hpp>
#include <dom/DOM_NamedNodeMap.hpp>
#include <dom/DOMString.hpp>



#include <PlatformSupport/Cloneable.hpp>
#include <DOMSupport/UnimplementedDocumentFragment.hpp>



class DOM_Document;
class NodeRefListBase;
class XMLParserLiaison;



/**
 * The holder of result tree fragments.
 */
class XALAN_XPATH_EXPORT ResultTreeFragBase : public Cloneable, public UnimplementedDocumentFragment
{
public:

	ResultTreeFragBase(const DOM_Document&	theOwnerDocument);

	ResultTreeFragBase(const ResultTreeFragBase&);

	virtual
	~ResultTreeFragBase();

	// these interfaces are inherited from Cloneable...
#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Cloneable*
#else
	virtual ResultTreeFragBase*
#endif
	clone(bool	deep) const = 0;

	// These interfaces are inherited from UnimplementedDocumentFragment...

	// These interfaces are inherited from NodeListImpl...

	virtual NodeImpl*
	item(unsigned long	index) = 0;

	virtual int
	getLength() = 0;

	// These interfaces are inherited from NodeImpl...

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual NodeImpl*
#else
	virtual ResultTreeFragBase*
#endif
	cloneNode(bool deep) = 0;

	virtual NodeImpl*
	appendChild(NodeImpl*	newChild) = 0;

	virtual void
	changed() = 0;

	virtual NamedNodeMapImpl*
	getAttributes() = 0;

	virtual NodeListImpl*
	getChildNodes() = 0;

	virtual NodeImpl*
	getFirstChild() = 0;

	virtual NodeImpl*
	getLastChild() = 0;

	// Deriving classes do not need to override these next three
	// interfaces, since ResultTreeFragments do not have these
	// properties.

	virtual DOMString
	getNodeName();

	virtual short
	getNodeType();

	virtual DOMString
	getNodeValue();

	// Deriving classes must implement these interfaces ...

	virtual DocumentImpl*
	getOwnerDocument() = 0;

	// Deriving classes do not need to override these next three
	// interfaces, since ResultTreeFragments do not have these
	// properties.

	virtual NodeImpl* 
	getParentNode();

	virtual NodeImpl*
	getNextSibling();

	virtual NodeImpl* 
	getPreviousSibling();

	// Deriving classes must implement these interfaces ...

	virtual bool
	hasChildNodes() = 0;

	virtual NodeImpl*
	insertBefore(
			NodeImpl*	newChild,
			NodeImpl*	refChild) = 0;

	virtual NodeImpl*
	removeChild(NodeImpl *oldChild) = 0;

	virtual NodeImpl*
	replaceChild(
			NodeImpl*	newChild,
			NodeImpl*	oldChild) = 0;

	// Deriving classes do not need to override this, since
	// ResultTreeFragments do not have this properties.

	virtual void
	setNodeValue(const DOMString&	value);

	// Deriving classes must implement these interfaces ...

	virtual void
	setReadOnly(
			bool	readOnly,
			bool	deep) = 0;

	virtual DOMString
	toString() = 0;

	// These interfaces are new to ResultTreeFragBase

	// Many of these interfaces duplicate ones that are inherited from the Xerces DOM
	// implementation classes.  In general, they take and/or return instances of the
	// Xerces smart-pointer classes.  Other are const, which many of the Xerces functions
	// are not.

	/**
	 * A <code>NodeList</code> that contains all children of this node. If there 
	 * are no children, this is a <code>NodeList</code> containing no nodes. 
	 * The content of the returned <code>NodeList</code> is "live" in the sense 
	 * that, for instance, changes to the children of the node object that 
	 * it was created from are immediately reflected in the nodes returned by 
	 * the <code>NodeList</code> accessors; it is not a static snapshot of the 
	 * content of the node. This is true for every <code>NodeList</code>, 
	 * including the ones returned by the <code>getElementsByTagName</code> 
	 * method.
	 */
	virtual const NodeRefListBase&
	getChildNodesAsNodeRefList() const = 0;

	virtual DOM_Node
	item(unsigned long	index) const = 0;

	virtual int
	getLength() const = 0;

	/**
	 * The first child of this node. If there is no such node, this returns 
	 * <code>null</code>.
	 */
	virtual DOM_Node
	getFirstChild() const = 0;

	/**
	 * The last child of this node. If there is no such node, this returns 
	 * <code>null</code>.
	 */
	virtual DOM_Node
	getLastChild() const = 0;

	/**
	 * The <code>Document</code> object associated with this node. This is also 
	 * the <code>Document</code> object used to create new nodes. When this 
	 * node is a <code>Document</code> this is <code>null</code>.
	 */
	virtual DOM_Document
	getOwnerDocument() const = 0;

	/**
	 * Inserts the node <code>newChild</code> before the existing child node 
	 * <code>refChild</code>. If <code>refChild</code> is <code>null</code>, 
	 * insert <code>newChild</code> at the end of the list of children.
	 * <br>If <code>newChild</code> is a <code>DocumentFragment</code> object, 
	 * all of its children are inserted, in the same order, before 
	 * <code>refChild</code>. If the <code>newChild</code> is already in the 
	 * tree, it is first removed.
	 * @param newChild The node to insert.
	 * @param refChild The reference node, i.e., the node before which the new 
	 *   node must be inserted.
	 * @return The node being inserted.
	 * @exception DOMException
	 *   HIERARCHY_REQUEST_ERR: Raised if this node is of a type that does not 
	 *   allow children of the type of the <code>newChild</code> node, or if 
	 *   the node to insert is one of this node's ancestors.
	 *   <br>WRONG_DOCUMENT_ERR: Raised if <code>newChild</code> was created 
	 *   from a different document than the one that created this node.
	 *   <br>NO_MODIFICATION_ALLOWED_ERR: Raised if this node is readonly.
	 *   <br>NOT_FOUND_ERR: Raised if <code>refChild</code> is not a child of 
	 *   this node.
	 */
	virtual DOM_Node
	insertBefore(
			const DOM_Node&		newChild, 
			const DOM_Node&		refChild) = 0;

	/**
	 * Replaces the child node <code>oldChild</code> with <code>newChild</code> 
	 * in the list of children, and returns the <code>oldChild</code> node. If 
	 * the <code>newChild</code> is already in the tree, it is first removed.
	 * @param newChild The new node to put in the child list.
	 * @param oldChild The node being replaced in the list.
	 * @return The node replaced.
	 * @exception DOMException
	 *   HIERARCHY_REQUEST_ERR: Raised if this node is of a type that does not 
	 *   allow children of the type of the <code>newChild</code> node, or it 
	 *   the node to put in is one of this node's ancestors.
	 *   <br>WRONG_DOCUMENT_ERR: Raised if <code>newChild</code> was created 
	 *   from a different document than the one that created this node.
	 *   <br>NO_MODIFICATION_ALLOWED_ERR: Raised if this node is readonly.
	 *   <br>NOT_FOUND_ERR: Raised if <code>oldChild</code> is not a child of 
	 *   this node.
	 */
	virtual DOM_Node
	replaceChild(
			const DOM_Node&		newChild,
			const DOM_Node&		oldChild) = 0;

	/**
	 * Removes the child node indicated by <code>oldChild</code> from the list 
	 * of children, and returns it.
	 * @param oldChild The node being removed.
	 * @return The node removed.
	 * @exception DOMException
	 *   NO_MODIFICATION_ALLOWED_ERR: Raised if this node is readonly.
	 *   <br>NOT_FOUND_ERR: Raised if <code>oldChild</code> is not a child of 
	 *   this node.
	 */
	virtual DOM_Node
	removeChild(const DOM_Node&		oldChild) = 0;
  
	/**
	 * Adds the node <code>newChild</code> to the end of the list of children of 
	 * this node. If the <code>newChild</code> is already in the tree, it is 
	 * first removed.
	 * @param newChild The node to add.If it is a  <code>DocumentFragment</code> 
	 *   object, the entire contents of the document fragment are moved into 
	 *   the child list of this node
	 * @return The node added.
	 * @exception DOMException
	 *   HIERARCHY_REQUEST_ERR: Raised if this node is of a type that does not 
	 *   allow children of the type of the <code>newChild</code> node, or if 
	 *   the node to append is one of this node's ancestors.
	 *   <br>WRONG_DOCUMENT_ERR: Raised if <code>newChild</code> was created 
	 *   from a different document than the one that created this node.
	 *   <br>NO_MODIFICATION_ALLOWED_ERR: Raised if this node is readonly.
	 */
	virtual DOM_Node
	appendChild(const DOM_Node&		newChild) = 0;

	/**
	 *  This is a convenience method to allow easy determination of whether a 
	 * node has any children.
	 * @return  <code>true</code> if the node has any children, 
	 *   <code>false</code> if the node has no children.
	 */
	virtual bool
	hasChildNodes() const = 0;

	/**
	 * Returns a duplicate of this node, i.e., serves as a generic copy 
	 * constructor for nodes. The duplicate node has no parent (
	 * <code>parentNode</code> returns <code>null</code>.).
	 * <br>Cloning an <code>Element</code> copies all attributes and their 
	 * values, including those generated by the  XML processor to represent 
	 * defaulted attributes, but this method does not copy any text it contains 
	 * unless it is a deep clone, since the text is contained in a child 
	 * <code>Text</code> node. Cloning any other type of node simply returns a 
	 * copy of this node. 
	 * @param deep If <code>true</code>, recursively clone the subtree under the 
	 *   specified node; if <code>false</code>, clone only the node itself (and 
	 *   its attributes, if it is an <code>Element</code>).  
	 * @return The duplicate node.
	 */
	virtual DOM_Node
	cloneNode(bool	deep) const = 0;

protected:

	ResultTreeFragBase&
	operator=(const ResultTreeFragBase&		theRHS)
	{
		Cloneable::operator=(theRHS);

		UnimplementedDocumentFragment::operator=(theRHS);

		return *this;
	}

	bool
	operator==(const ResultTreeFragBase&) const
	{
		return true;
	}
};



class XALAN_XPATH_EXPORT DOM_ResultTreeFragBase : public DOM_UnimplementedDocumentFragment
{
public:

	/**
	 * Constructors.
	 */
	DOM_ResultTreeFragBase(ResultTreeFragBase*	theFragment);

	DOM_ResultTreeFragBase();

	DOM_ResultTreeFragBase(const DOM_ResultTreeFragBase&	theNode);

	/**
	 * Destructor.
	 */
	~DOM_ResultTreeFragBase();

	/**
	 * Assignment operator.
	 *
	 * @param other The source to be assigned.
	 */
	DOM_ResultTreeFragBase&
	operator=(const DOM_ResultTreeFragBase&	other)
	{
		DOM_UnimplementedDocumentFragment::operator=(other);

		return *this;
	}

	/**
	 * Assignment operator.  This overloaded variant is provided for
	 *	 the sole purpose of setting a DOM_Node reference variable to
	 *	 zero.	Nulling out a reference variable in this way will decrement
	 *	 the reference count on the underlying Node object that the variable
	 *	 formerly referenced.	This effect is normally obtained when reference
	 *	 variable goes out of scope, but zeroing them can be useful for
	 *	 global instances, or for local instances that will remain in scope
	 *	 for an extended time,	when the storage belonging to the underlying
	 *	 node needs to be reclaimed.
	 *
	 * @param val.	Only a value of 0, or null, is allowed.
	 */
	DOM_ResultTreeFragBase&
	operator=(const DOM_NullPtr*	val)
	{
		DOM_UnimplementedDocumentFragment::operator=(val);

		return *this;
	}

	/**
	 * The equality operator.  This compares to references to nodes, and
	 * returns true if they both refer to the same underlying node.  It
	 * is exactly analogous to Java's operator ==  on object reference
	 * variables.  This operator can not be used to compare the values
	 * of two different nodes in the document tree.
	 *
	 * @param other The object reference with which <code>this</code> object is compared
	 * @returns True if both <code>DOM_Node</code>s refer to the same
	 *	actual node, or are both null; return false otherwise.
	 */
	bool
	operator==(const DOM_Node&	other) const
	{
		return DOM_UnimplementedDocumentFragment::operator==(other);
	}

	/**
	 *	Compare with a pointer.  Intended only to allow a convenient
	 *	  comparison with null.
	 *
	 */
	bool
	operator==(const DOM_NullPtr*	other) const
	{
		return DOM_UnimplementedDocumentFragment::operator==(other);
	}

	/**
	 * The inequality operator.  See operator ==.
	 *
	 */
	bool
	operator!=(const DOM_Node&	other) const
	{
		return DOM_UnimplementedDocumentFragment::operator!=(other);
	}

	/**
	 *	Compare with a pointer.  Intended only to allow a convenient
	 *	  comparison with null.
	 *
	 */
	bool
	operator!=(const DOM_NullPtr*	other) const
	{
		return DOM_UnimplementedDocumentFragment::operator!=(other);
	}
};



#endif	// RESULTTREEFRAGBASE_HEADER_GUARD
