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
#if !defined(RESULTTREEFRAG_HEADER_GUARD_1357924680)
#define RESULTTREEFRAG_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XSLT/XSLTDefinitions.hpp>



#include <XalanDOM/XalanNodeList.hpp>



#include <XPath/ResultTreeFragBase.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XalanSourceTreeDocumentFragment;



/**
 * The holder of result tree fragments.
 */
class XALAN_XSLT_EXPORT ResultTreeFrag : public ResultTreeFragBase, private XalanNodeList
{
public:

	/**
	 * Construct a result tree fragment object from a DOM document.
	 */
	explicit
	ResultTreeFrag(XalanSourceTreeDocumentFragment*	theDocumentFragment = 0);

	/**
	 * Construct a result tree fragment object from another.
	 * 
	 * @param theSource source to copy
	 * @param deepClone true if subobjects should be copied, default is false
	 */
	ResultTreeFrag(
			const ResultTreeFrag&	theSource,
			bool					deepClone = false);

	virtual
	~ResultTreeFrag();


	// These interfaces are new...

	void
	clear();


	// These interfaces are inherited from ResultTreeFragBase...

	virtual XalanNode*
	getNodesetRoot() const;


	// These interfaces are inherited from XalanDocumentFragment...

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

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual XalanNode*
#else
	virtual ResultTreeFrag*
#endif
	cloneNode(bool deep) const;

	virtual XalanNode*
	insertBefore(
			XalanNode*	newChild,
			XalanNode*	refChild);

	virtual XalanNode*
	replaceChild(
			XalanNode*	newChild,
			XalanNode*	oldChild);

	virtual XalanNode*
	removeChild(XalanNode*	oldChild);

	virtual XalanNode*
	appendChild(XalanNode*	newChild);

	virtual bool
	hasChildNodes() const;

	virtual void
	setNodeValue(const XalanDOMString&	nodeValue);

	virtual void
	normalize();

	virtual bool
	isSupported(
			const XalanDOMString&	feature,
			const XalanDOMString&	version) const;

	virtual const XalanDOMString&
	getNamespaceURI() const;

	virtual const XalanDOMString&
	getPrefix() const;

	virtual const XalanDOMString&
	getLocalName() const;

	virtual void
	setPrefix(const XalanDOMString&		prefix);

	virtual bool
	isIndexed() const;

	virtual IndexType
	getIndex() const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual ResultTreeFragBase*
#else
	virtual ResultTreeFrag*
#endif
	clone(bool	deep) const;

	XalanSourceTreeDocumentFragment*
	getDocumentFragment() const
	{
		return m_documentFragment;
	}

	void
	setDocumentFragment(XalanSourceTreeDocumentFragment*	theDocumentFragment);

private:

	// These methods are inherited from XalanNodeList...

	virtual XalanNode*
	item(unsigned int	index) const;

	virtual unsigned int
	getLength() const;


	// Not defined
	ResultTreeFrag&
	operator=(const ResultTreeFrag&		theRHS);

	bool
	operator==(const ResultTreeFrag&	theRHS) const;

	// Data members...
	XalanSourceTreeDocumentFragment*	m_documentFragment;

	// These are for caching results...
	mutable unsigned int				m_length;

	mutable unsigned int				m_lastIndex;

	mutable XalanNode*					m_lastNode;

	static const XalanDOMString		s_emptyString;
};



XALAN_CPP_NAMESPACE_END



#endif	// RESULTTREEFRAG_HEADER_GUARD_1357924680
