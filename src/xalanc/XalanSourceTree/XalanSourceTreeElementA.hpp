/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2003 The Apache Software Foundation.  All rights 
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
#if !defined(XALANSOURCETREEELEMENTA_HEADER_GUARD_1357924680)
#define XALANSOURCETREEELEMENTA_HEADER_GUARD_1357924680



#include <xalanc/XalanSourceTree/XalanSourceTreeDefinitions.hpp>



#include <xalanc/XalanDOM/XalanNamedNodeMap.hpp>



#include <xalanc/XalanSourceTree/XalanSourceTreeElement.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XALANSOURCETREE_EXPORT XalanSourceTreeElementA : public XalanSourceTreeElement, private XalanNamedNodeMap
{
public:

	typedef unsigned long	AttributesCountType;

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
			const XalanDOMString&		theTagName,
			XalanSourceTreeDocument*	theOwnerDocument,
			XalanSourceTreeAttr**		theAttributes,
			AttributesCountType			theAttributeCount,
			XalanNode*					theParentNode = 0,
			XalanNode*					thePreviousSibling = 0,
			XalanNode*					theNextSibling = 0,
			IndexType					theIndex = 0);

	virtual
	~XalanSourceTreeElementA();


	virtual const XalanNamedNodeMap*
	getAttributes() const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual XalanNode*
#else
	virtual XalanSourceTreeElementA*
#endif
	cloneNode(bool deep) const;

	virtual const XalanDOMString&
	getNamespaceURI() const;

	virtual const XalanDOMString&
	getPrefix() const;

	virtual const XalanDOMString&
	getLocalName() const;

	virtual const XalanDOMString&
	getAttribute(const XalanDOMString&		name) const;

	virtual XalanAttr*
	getAttributeNode(const XalanDOMString&		name) const;

	virtual const XalanDOMString&
	getAttributeNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	localName) const;

	virtual XalanAttr*
	getAttributeNodeNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	localName) const;


	// public interfaces not inherited from XalanElement...

	AttributesCountType
	getAttributeCount() const
	{
		return m_attributeCount;
	}

	XalanSourceTreeAttr*
	getAttributeByIndex(AttributesCountType		index) const
	{
		return index < m_attributeCount ? m_attributes[index] : 0;
	}

	XalanSourceTreeElementA*
	clone(bool	deep) const
	{
		return new XalanSourceTreeElementA(*this, deep);
	}

protected:

	XalanSourceTreeElementA(
			const XalanSourceTreeElementA&	theSource,
			bool							deep = false);

private:

	// These are from XalanNamedNodeMap...

	virtual XalanNode*
	setNamedItem(XalanNode* 	arg);

	virtual XalanNode*
	item(unsigned int	index) const;

	virtual XalanNode*
	getNamedItem(const XalanDOMString& 	name) const;

	virtual unsigned int
	getLength() const;

	virtual XalanNode*
	removeNamedItem(const XalanDOMString&	name);

	virtual XalanNode*
	getNamedItemNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	localName) const;

	virtual XalanNode*
	setNamedItemNS(XalanNode*	arg);

	virtual XalanNode*
	removeNamedItemNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	localName);


	// Not implemented...
	XalanSourceTreeElementA&
	operator=(const XalanSourceTreeElementA&	theSource);

	bool
	operator==(const XalanSourceTreeElementA&	theRHS) const;


	// Data members...
	XalanSourceTreeAttr* const *	m_attributes;

	const AttributesCountType		m_attributeCount;
};



XALAN_CPP_NAMESPACE_END



#endif	// !defined(XALANSOURCETREEELEMENTA_HEADER_GUARD_1357924680)
