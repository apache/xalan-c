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

	/**
	 * Construct a result tree fragment object from a DOM document.
	 * 
	 * @param theFactory document used to construct result tree fragment
	 * @param theSupport XPath support class instance
	 */
	ResultTreeFrag(
			const DOM_Document&		theFactory,
			XPathSupport&			theSupport);

	/**
	 * Construct a result tree fragment object from another.
	 * 
	 * @param theSource source to copy
	 * @param deepClone true if subobjects should be copied, default true
	 */
	ResultTreeFrag(const ResultTreeFrag&	theSource,
				   bool						deepClone = true);

	virtual
	~ResultTreeFrag();

	// These interfaces are inherited from Cloneable...
#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Cloneable*
#else
	virtual ResultTreeFrag*
#endif
	clone(bool	deep) const;

	// These interfaces are inherited from UnimplementedDocumentFragment...

	// These interfaces are inherited from NodeListImpl...

	virtual NodeImpl*
	item(unsigned int	index);

	virtual unsigned int
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

	virtual const NodeRefListBase&
	getChildNodesAsNodeRefList() const;

	virtual DOM_Node
	item(unsigned int	index) const;

	virtual unsigned int
	getLength() const;

	virtual DOM_Node
	getFirstChild() const;

	virtual DOM_Node
	getLastChild() const;

	virtual DOM_Document
	getOwnerDocument() const;

	virtual DOM_Node
	insertBefore(
			const DOM_Node&		newChild, 
			const DOM_Node&		refChild);

	virtual DOM_Node
	replaceChild(
			const DOM_Node&		newChild,
			const DOM_Node&		oldChild);

	virtual DOM_Node
	removeChild(const DOM_Node&		oldChild);
  
	virtual DOM_Node
	appendChild(const DOM_Node&		newChild);

	virtual bool
	hasChildNodes() const;

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
