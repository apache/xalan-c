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



#include <XalanDOM/XalanDocument.hpp>



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
	 * @param theOwnerDocument The document used to construct result tree fragment
	 * @param theSupport The XPathSupport instance
	 */
	ResultTreeFrag(
			XalanDocument&	theOwnerDocument,
			XPathSupport&	theSupport);

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


	// These interfaces are inherited from XalanDocumentFragment...
	virtual XalanDOMString
	getNodeName() const;

	virtual XalanDOMString
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
	supports(
			const XalanDOMString&	feature,
			const XalanDOMString&	version) const;

	virtual XalanDOMString
	getNamespaceURI() const;

	virtual XalanDOMString
	getPrefix() const;

	virtual XalanDOMString
	getLocalName() const;

	virtual void
	setPrefix(const XalanDOMString&		prefix);


#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual ResultTreeFragBase*
#else
	virtual ResultTreeFrag*
#endif
	clone(bool	deep) const;

private:

	// Not defined
	bool
	operator==(const ResultTreeFrag&	theRHS) const;


	XalanDocument*			m_document;
	MutableNodeRefList		m_children;

	NodeListImplSurrogate	m_surrogate;
};



#endif	// RESULTTREEFRAGBASE_HEADER_GUARD_1357924680
