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
#if !defined(RESULTTREEFRAGBASE_HEADER_GUARD_1357924680)
#define RESULTTREEFRAGBASE_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDocumentFragment.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class NodeRefListBase;
class XMLParserLiaison;
class XalanDocument;



/**
 * The holder of result tree fragments.
 */
class XALAN_XPATH_EXPORT ResultTreeFragBase : public XalanDocumentFragment
{
public:

	/**
	 * Construct a result tree fragment object.
	 */
	ResultTreeFragBase();

	/**
	 * Construct a result tree fragment object from another.
	 * 
	 * @param theSource source to copy
	 */
	ResultTreeFragBase(const ResultTreeFragBase&	theSource);

	virtual
	~ResultTreeFragBase();

	// These interfaces are new to ResultTreeFragBase...

	/**
	 * Get the root when this instance is cast to a Nodeset.
	 * This value might be this instance itself, or could
	 * be another object that is serving as a proxy for
	 * this instance.  Most likely the proxy would be a
	 * DocumentFragment.
	 *
	 * @return The root for this fragment.
	 */
	virtual XalanNode*
	getNodesetRoot() const = 0;

	// These interfaces are inherited from XalanDocumentFragment...
	virtual const XalanDOMString&
	getNodeName() const = 0;

	virtual const XalanDOMString&
	getNodeValue() const = 0;

	virtual NodeType
	getNodeType() const = 0;

	virtual XalanNode*
	getParentNode() const = 0;

	virtual const XalanNodeList*
	getChildNodes() const = 0;

	virtual XalanNode*
	getFirstChild() const = 0;

	virtual XalanNode*
	getLastChild() const = 0;

	virtual XalanNode*
	getPreviousSibling() const = 0;

	virtual XalanNode*
	getNextSibling() const = 0;

	virtual const XalanNamedNodeMap*
	getAttributes() const = 0;

	virtual XalanDocument*
	getOwnerDocument() const = 0;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual XalanNode*
#else
	virtual ResultTreeFragBase*
#endif
	cloneNode(bool deep) const = 0;

	virtual XalanNode*
	insertBefore(
			XalanNode*	newChild,
			XalanNode*	refChild) = 0;

	virtual XalanNode*
	replaceChild(
			XalanNode*	newChild,
			XalanNode*	oldChild) = 0;

	virtual XalanNode*
	removeChild(XalanNode*	oldChild) = 0;

	virtual XalanNode*
	appendChild(XalanNode*	newChild) = 0;

	virtual bool
	hasChildNodes() const = 0;

	virtual void
	setNodeValue(const XalanDOMString&	nodeValue) = 0;

	virtual void
	normalize() = 0;

	virtual bool
	isSupported(
			const XalanDOMString&	feature,
			const XalanDOMString&	version) const = 0;

	virtual const XalanDOMString&
	getNamespaceURI() const = 0;

	virtual const XalanDOMString&
	getPrefix() const = 0;

	virtual const XalanDOMString&
	getLocalName() const = 0;

	virtual void
	setPrefix(const XalanDOMString&		prefix) = 0;

	virtual bool
	isIndexed() const = 0;

	virtual IndexType
	getIndex() const = 0;

	virtual ResultTreeFragBase*
	clone(bool	deep) const = 0;

protected:

	ResultTreeFragBase&
	operator=(const ResultTreeFragBase&		theRHS)
	{
		XalanDocumentFragment::operator=(theRHS);

		return *this;
	}

	bool
	operator==(const ResultTreeFragBase&) const
	{
		return true;
	}
};



XALAN_CPP_NAMESPACE_END



#endif	// RESULTTREEFRAGBASE_HEADER_GUARD
