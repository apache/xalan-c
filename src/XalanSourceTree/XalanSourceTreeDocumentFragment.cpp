/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2001 The Apache Software Foundation.  All rights 
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



#include <XalanDOM/XalanDOMException.hpp>



#include <PlatformSupport/AttributeListImpl.hpp>
#include <PlatformSupport/XalanUnicode.hpp>



#include "XalanSourceTreeDocument.hpp"
#include "XalanSourceTreeElement.hpp"



static const XalanDOMChar		s_nameString[] = { XalanUnicode::charLetter_A, 0 };
static const AttributeListImpl	s_emptyAttributes;
static const XalanDOMString		s_emptyString;



XalanSourceTreeDocumentFragment::XalanSourceTreeDocumentFragment(XalanSourceTreeDocument&	theOwnerDocument) :
	XalanDocumentFragment(),
	m_hostElement(theOwnerDocument.createElementNode(s_nameString, s_emptyAttributes))
{
}



XalanSourceTreeDocumentFragment::XalanSourceTreeDocumentFragment(
			const XalanSourceTreeDocumentFragment&		theSource,
			bool										deep) :
	m_hostElement(theSource.m_hostElement->clone(deep))
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
	return m_hostElement->getChildNodes();
}



XalanNode*
XalanSourceTreeDocumentFragment::getFirstChild() const
{
	return m_hostElement->getFirstChild();
}



XalanNode*
XalanSourceTreeDocumentFragment::getLastChild() const
{
	return m_hostElement->getLastChild();
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



XalanDocument*
XalanSourceTreeDocumentFragment::getOwnerDocument() const
{
	return m_hostElement->getOwnerDocument();
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XalanSourceTreeDocumentFragment*
#endif
XalanSourceTreeDocumentFragment::cloneNode(bool deep) const
{
	return new XalanSourceTreeDocumentFragment(*this, deep);
}



XalanNode*
XalanSourceTreeDocumentFragment::insertBefore(
			XalanNode*	newChild,
			XalanNode*	refChild)
{
	return m_hostElement->insertBefore(newChild, refChild);
}



XalanNode*
XalanSourceTreeDocumentFragment::replaceChild(
			XalanNode*	newChild,
			XalanNode*	oldChild)
{
	return m_hostElement->replaceChild(newChild, oldChild);
}



XalanNode*
XalanSourceTreeDocumentFragment::removeChild(XalanNode*	oldChild)
{
	return m_hostElement->removeChild(oldChild);
}



XalanNode*
XalanSourceTreeDocumentFragment::appendChild(XalanNode*	newChild)
{
	return m_hostElement->appendChild(newChild);
}



bool
XalanSourceTreeDocumentFragment::hasChildNodes() const
{
	return m_hostElement->hasChildNodes();
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
XalanSourceTreeDocumentFragment::supports(
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



unsigned long
XalanSourceTreeDocumentFragment::getIndex() const
{
	return 0;
}



void
XalanSourceTreeDocumentFragment::appendChildNode(XalanSourceTreeComment*	theChild)
{
	m_hostElement->appendChildNode(theChild);
}



void
XalanSourceTreeDocumentFragment::appendChildNode(XalanSourceTreeElement*	theChild)
{
	m_hostElement->appendChildNode(theChild);
}



void
XalanSourceTreeDocumentFragment::appendChildNode(XalanSourceTreeProcessingInstruction*	theChild)
{
	m_hostElement->appendChildNode(theChild);
}



void
XalanSourceTreeDocumentFragment::appendChildNode(XalanSourceTreeText*	theChild)
{
	m_hostElement->appendChildNode(theChild);
}



void
XalanSourceTreeDocumentFragment::clearChildren()
{
	m_hostElement->clearChildren();
}
