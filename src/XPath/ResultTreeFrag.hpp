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
#if !defined(RESULTTREEFRAG_HEADER_GUARD_1357924680)
#define RESULTTREEFRAG_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



#include <dom/DOM_Document.hpp>



#include <XPath/MutableNodeRefList.hpp>
#include <XPath/NodeListImplSurrogate.hpp>
#include <XPath/ResultTreeFragBase.hpp>



class XPathSupport;



/**
 * The holder of result tree fragments.
 */
class XALAN_XPATH_EXPORT ResultTreeFrag : public ResultTreeFragBase
{
public:

	ResultTreeFrag(
			const DOM_Document&		theFactory,
			XPathSupport&			theSupport);

	ResultTreeFrag(const ResultTreeFrag&	theSource,
				   bool						deepClone = true);

	virtual
	~ResultTreeFrag();

	// these interfaces are inherited from Cloneable...
#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Cloneable*
#else
	virtual ResultTreeFrag*
#endif
	clone(bool	deep) const;

	// These interfaces are inherited from UnimplementedDocumentFragment...

	// These interfaces are inherited from NodeListImpl...

	virtual NodeImpl*
	item(unsigned long	index);

	virtual int
	getLength();

	// These interfaces are inherited from NodeImpl...

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual NodeImpl*
#else
	virtual UnimplementedDocumentFragment*
#endif
	cloneNode(bool deep);

	virtual NodeImpl*
	appendChild(NodeImpl*	newChild);

	virtual void
	changed();

	virtual NamedNodeMapImpl*
	getAttributes();

	virtual NodeListImpl*
	getChildNodes();

	virtual NodeImpl*
	getFirstChild();

	virtual NodeImpl*
	getLastChild();

	virtual DocumentImpl*
	getOwnerDocument();

	virtual bool
	hasChildNodes();

	virtual NodeImpl*
	insertBefore(
			NodeImpl*	newChild,
			NodeImpl*	refChild);

	virtual NodeImpl*
	removeChild(NodeImpl*	oldChild);

	virtual NodeImpl*
	replaceChild(
			NodeImpl*	newChild,
			NodeImpl*	oldChild);

	virtual void
	setReadOnly(
			bool	readOnly,
			bool	deep);

	virtual DOMString
	toString();

	// These interfaces are inherited from ResultTreeFragBase
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
	getChildNodesAsNodeRefList() const;

	virtual DOM_Node
	item(unsigned long	index) const;

	virtual int
	getLength() const;

	/**
	 * The first child of this node. If there is no such node, this returns 
	 * <code>null</code>.
	 */
	virtual DOM_Node
	getFirstChild() const;

	/**
	 * The last child of this node. If there is no such node, this returns 
	 * <code>null</code>.
	 */
	virtual DOM_Node
	getLastChild() const;

	/**
	 * The <code>Document</code> object associated with this node. This is also 
	 * the <code>Document</code> object used to create new nodes. When this 
	 * node is a <code>Document</code> this is <code>null</code>.
	 */
	virtual DOM_Document
	getOwnerDocument() const;

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
			const DOM_Node&		refChild);

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
			const DOM_Node&		oldChild);

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
	removeChild(const DOM_Node&		oldChild);
  
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
	appendChild(const DOM_Node&		newChild);

	/**
	 *  This is a convenience method to allow easy determination of whether a 
	 * node has any children.
	 * @return  <code>true</code> if the node has any children, 
	 *   <code>false</code> if the node has no children.
	 */
	virtual bool
	hasChildNodes() const;

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
	cloneNode(bool	deep) const;

	ResultTreeFrag&
	operator=(const ResultTreeFrag&		theRHS)
	{
		if (&theRHS != this)
		{
			ResultTreeFragBase::operator==(theRHS);

			m_document = theRHS.m_document;
			m_children = m_children;
		}

		return *this;
	}

private:

	// Not defined
	bool
	operator==(const ResultTreeFrag&	theRHS) const;


	DOM_Document			m_document;
	MutableNodeRefList		m_children;

	NodeListImplSurrogate	m_surrogate;
};



#endif	// RESULTTREEFRAGBASE_HEADER_GUARD_1357924680
