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
#include "XalanSourceTreeDocumentFragment.hpp"



#include <xalanc/XalanDOM/XalanDOMException.hpp>



#include <xalanc/PlatformSupport/AttributeListImpl.hpp>
#include <xalanc/PlatformSupport/XalanUnicode.hpp>



#include "XalanSourceTreeDocument.hpp"
#include "XalanSourceTreeElement.hpp"
#include "XalanSourceTreeHelper.hpp"



XALAN_CPP_NAMESPACE_BEGIN



static const XalanDOMString		s_emptyString;



XalanSourceTreeDocumentFragment::XalanSourceTreeDocumentFragment(XalanSourceTreeDocument&	theOwnerDocument) :
	XalanDocumentFragment(),
	m_ownerDocument(&theOwnerDocument),
	m_firstChild(0)
{
}



XalanSourceTreeDocumentFragment::XalanSourceTreeDocumentFragment(
			const XalanSourceTreeDocumentFragment&	theSource,
			bool									deep) :
	m_ownerDocument(theSource.m_ownerDocument),
	m_firstChild(theSource.m_firstChild == 0 ? 0 : theSource.m_firstChild->cloneNode(deep))
{
}



XalanSourceTreeDocumentFragment::~XalanSourceTreeDocumentFragment()
{
}



const XalanDOMString&
XalanSourceTreeDocumentFragment::getNodeName() const
{
	return s_emptyString;
}



const XalanDOMString&
XalanSourceTreeDocumentFragment::getNodeValue() const
{
	return s_emptyString;
}



XalanSourceTreeDocumentFragment::NodeType
XalanSourceTreeDocumentFragment::getNodeType() const
{
	return DOCUMENT_FRAGMENT_NODE;
}



XalanNode*
XalanSourceTreeDocumentFragment::getParentNode() const
{
	return 0;
}



const XalanNodeList*
XalanSourceTreeDocumentFragment::getChildNodes() const
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeDocumentFragment::getFirstChild() const
{
	return m_firstChild;
}



XalanNode*
XalanSourceTreeDocumentFragment::getLastChild() const
{
	return XalanSourceTreeHelper::getLastSibling(m_firstChild);
}



XalanNode*
XalanSourceTreeDocumentFragment::getPreviousSibling() const
{
	return 0;
}



XalanNode*
XalanSourceTreeDocumentFragment::getNextSibling() const
{
	return 0;
}



const XalanNamedNodeMap*
XalanSourceTreeDocumentFragment::getAttributes() const
{
	return 0;
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanDocument*
#else
XalanSourceTreeDocument*
#endif
XalanSourceTreeDocumentFragment::getOwnerDocument() const
{
	return m_ownerDocument;
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XalanSourceTreeDocumentFragment*
#endif
XalanSourceTreeDocumentFragment::cloneNode(bool		/* deep */) const
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeDocumentFragment::insertBefore(
			XalanNode*	/* newChild */,
			XalanNode*	/* refChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeDocumentFragment::replaceChild(
			XalanNode*	/* newChild */,
			XalanNode*	/* oldChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeDocumentFragment::removeChild(XalanNode*		/* oldChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeDocumentFragment::appendChild(XalanNode*		/* newChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



bool
XalanSourceTreeDocumentFragment::hasChildNodes() const
{
	return m_firstChild != 0 ? true : false;
}



void
XalanSourceTreeDocumentFragment::setNodeValue(const XalanDOMString&		/* nodeValue */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XalanSourceTreeDocumentFragment::normalize()
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
XalanSourceTreeDocumentFragment::isSupported(
			const XalanDOMString&	/* feature */,
			const XalanDOMString&	/* version */) const
{
	return false;
}



const XalanDOMString&
XalanSourceTreeDocumentFragment::getNamespaceURI() const
{
	return s_emptyString;
}



const XalanDOMString&
XalanSourceTreeDocumentFragment::getPrefix() const
{
	return s_emptyString;
}



const XalanDOMString&
XalanSourceTreeDocumentFragment::getLocalName() const
{
	return s_emptyString;
}



void
XalanSourceTreeDocumentFragment::setPrefix(const XalanDOMString&	/* prefix */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
XalanSourceTreeDocumentFragment::isIndexed() const
{
	return false;
}



XalanSourceTreeDocumentFragment::IndexType
XalanSourceTreeDocumentFragment::getIndex() const
{
	return 0;
}



void
XalanSourceTreeDocumentFragment::appendChildNode(XalanSourceTreeComment*	theChild)
{
	XalanSourceTreeHelper::appendSiblingToChild(this, m_firstChild, theChild);
}



void
XalanSourceTreeDocumentFragment::appendChildNode(XalanSourceTreeElement*	theChild)
{
	XalanSourceTreeHelper::appendSiblingToChild(this, m_firstChild, theChild);
}



void
XalanSourceTreeDocumentFragment::appendChildNode(XalanSourceTreeProcessingInstruction*	theChild)
{
	XalanSourceTreeHelper::appendSiblingToChild(this, m_firstChild, theChild);
}



void
XalanSourceTreeDocumentFragment::appendChildNode(XalanSourceTreeText*	theChild)
{
	XalanSourceTreeHelper::appendSiblingToChild(this, m_firstChild, theChild);
}



void
XalanSourceTreeDocumentFragment::clearChildren()
{
	m_firstChild = 0;
}



XALAN_CPP_NAMESPACE_END
