/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2004 The Apache Software Foundation.  All rights 
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
#if !defined(XERCESWRAPPERNAVIGATOR_HEADER_GUARD_1357924680)
#define XERCESWRAPPERNAVIGATOR_HEADER_GUARD_1357924680



#include <xalanc/XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



#include <cassert>



#include <xercesc/util/XercesDefs.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>
#include <xalanc/XalanDOM/XalanNode.hpp>



#include <xalanc/XercesParserLiaison/XercesWrapperTypes.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XercesDocumentWrapper;
class XalanAttr;
class XalanElement;
class XalanText;



class XALAN_XERCESPARSERLIAISON_EXPORT XercesWrapperNavigator
{
public:

	typedef XalanNode::IndexType	IndexType;

	explicit
	XercesWrapperNavigator(XercesDocumentWrapper*	theOwnerDocument = 0);

	XercesWrapperNavigator(const XercesWrapperNavigator&	theSource);

	virtual
	~XercesWrapperNavigator();


	XercesDocumentWrapper*
	getOwnerDocument() const
	{
		return m_ownerDocument;
	}

	void
	setOwnerDocument(XercesDocumentWrapper*		theDocument)
	{
		m_ownerDocument = theDocument;
	}

	XalanNode*
	mapNode(const DOMNodeType*	theXercesNode) const;

	XalanAttr*
	mapNode(const DOMAttrType*	theXercesNode) const;

	const DOMNodeType*
	mapNode(XalanNode*	theXalanNode) const;

	IndexType
	getIndex() const
	{
		return m_index;
	}

	void
	setIndex(IndexType	theIndex)
	{
		m_index = theIndex;
	}

	XalanNode*
	getParentNode(const DOMNodeType*	theXercesNode) const;

	XalanNode*
	getParentNode() const
	{
		return m_parentNode;
	}

	void
	setParentNode(XalanNode*	theParent)
	{
		m_parentNode = theParent;
	}

	XalanNode*
	getPreviousSibling(const DOMNodeType*	theXercesNode) const;

	XalanNode*
	getPreviousSibling() const
	{
		return m_previousSibling;
	}

	void
	setPreviousSibling(XalanNode*	thePreviousSibling)
	{
		m_previousSibling = thePreviousSibling;
	}

	XalanNode*
	getNextSibling(const DOMNodeType*	theXercesNode) const;

	XalanNode*
	getNextSibling() const
	{
		return m_nextSibling;
	}

	void
	setNextSibling(XalanNode*	theNextSibling)
	{
		m_nextSibling = theNextSibling;
	}

	XalanNode*
	getFirstChild(const DOMNodeType*	theXercesNode) const;

	XalanNode*
	getFirstChild() const
	{
		return m_firstChild;
	}

	void
	setFirstChild(XalanNode*	theFirstChild)
	{
		m_firstChild = theFirstChild;
	}

	XalanNode*
	getLastChild(const DOMNodeType*		theXercesNode) const;

	XalanNode*
	getLastChild() const
	{
		return m_lastChild;
	}

	void
	setLastChild(XalanNode*	theLastChild)
	{
		m_lastChild = theLastChild;
	}

	XalanElement*
	getOwnerElement(const DOMAttrType*	theXercesAttr) const;

	/**
	 * Get a pooled string.  If the string is not in the pool,
	 * add it.
	 *
	 * @param theString The string to pool.
	 * @return A const reference to the pooled string.
	 */
	const XalanDOMString&
	getPooledString(const XMLCh*	theString) const;

private:

	// Not implemented...
	bool
	operator==(const XercesWrapperNavigator&	theRHS) const;

	// Data members...
	XercesDocumentWrapper*	m_ownerDocument;

	XalanNode*				m_parentNode;

	XalanNode*				m_previousSibling;

	XalanNode*				m_nextSibling;

	XalanNode*				m_firstChild;

	XalanNode*				m_lastChild;

	IndexType				m_index;

	static const XalanDOMString		s_emptyString;
};



XALAN_CPP_NAMESPACE_END



#endif	// !defined(XERCESWRAPPERNAVIGATOR_HEADER_GUARD_1357924680)
