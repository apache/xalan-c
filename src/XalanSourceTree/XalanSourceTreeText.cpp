/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2000 The Apache Software Foundation.  All rights 
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
#include "XalanSourceTreeText.hpp"



#include <XalanDOM/XalanDOMException.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>



#include "XalanSourceTreeComment.hpp"
#include "XalanSourceTreeDocumentFragment.hpp"
#include "XalanSourceTreeElement.hpp"
#include "XalanSourceTreeProcessingInstruction.hpp"
#include "XalanSourceTreeHelper.hpp"



static const XalanDOMString		s_emptyString;



XalanSourceTreeText::XalanSourceTreeText(
			const XalanDOMString&	theData,
			XalanNode*				theParentNode,
			XalanNode*				thePreviousSibling,
			XalanNode*				theNextSibling,
			unsigned int			theIndex) :
	XalanText(),
	m_data(theData),
	m_parentNode(theParentNode),
	m_previousSibling(thePreviousSibling),
	m_nextSibling(theNextSibling),
	m_index(theIndex)
{
}



XalanSourceTreeText::~XalanSourceTreeText()
{
}



XalanSourceTreeText::XalanSourceTreeText(
			const XalanSourceTreeText&	theSource,
			bool						/* deep */) :
	XalanText(theSource),
	m_data(theSource.m_data),
	m_parentNode(0),
	m_previousSibling(0),
	m_nextSibling(0),
	m_index(0)
{
}



const XalanDOMString&
XalanSourceTreeText::getNodeName() const
{
	return s_nameString;
}



const XalanDOMString&
XalanSourceTreeText::getNodeValue() const
{
	return m_data;
}



XalanSourceTreeText::NodeType
XalanSourceTreeText::getNodeType() const
{
	return TEXT_NODE;
}



XalanNode*
XalanSourceTreeText::getParentNode() const
{
	return m_parentNode;
}



const XalanNodeList*
XalanSourceTreeText::getChildNodes() const
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeText::getFirstChild() const
{
	return 0;
}



XalanNode*
XalanSourceTreeText::getLastChild() const
{
	return 0;
}



XalanNode*
XalanSourceTreeText::getPreviousSibling() const
{
	return m_previousSibling;
}



XalanNode*
XalanSourceTreeText::getNextSibling() const
{
	return m_nextSibling;
}



const XalanNamedNodeMap*
XalanSourceTreeText::getAttributes() const
{
	return 0;
}



XalanDocument*
XalanSourceTreeText::getOwnerDocument() const
{
	assert(m_parentNode != 0);

	return m_parentNode->getOwnerDocument();
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XalanSourceTreeText*
#endif
XalanSourceTreeText::cloneNode(bool	deep) const
{
	return new XalanSourceTreeText(*this, deep);
}



XalanNode*
XalanSourceTreeText::insertBefore(
			XalanNode*	/* newChild */,
			XalanNode*	/* refChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeText::replaceChild(
			XalanNode*	/* newChild */,
			XalanNode*	/* oldChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeText::removeChild(XalanNode*	/* oldChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeText::appendChild(XalanNode*	/* newChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



bool
XalanSourceTreeText::hasChildNodes() const
{
	return false;
}



void
XalanSourceTreeText::setNodeValue(const XalanDOMString&		/* nodeValue */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XalanSourceTreeText::normalize()
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
XalanSourceTreeText::supports(
			const XalanDOMString&	/* feature */,
			const XalanDOMString&	/* version */) const
{
	return false;
}



const XalanDOMString&
XalanSourceTreeText::getNamespaceURI() const
{
	return s_emptyString;
}



const XalanDOMString&
XalanSourceTreeText::getPrefix() const
{
	return s_emptyString;
}



const XalanDOMString&
XalanSourceTreeText::getLocalName() const
{
	return s_emptyString;
}



void
XalanSourceTreeText::setPrefix(const XalanDOMString&	/* prefix */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
XalanSourceTreeText::isIndexed() const
{
	return true;
}



unsigned long
XalanSourceTreeText::getIndex() const
{
	return m_index;
}



const XalanDOMString&
XalanSourceTreeText::getData() const
{
	return m_data;
}



unsigned int
XalanSourceTreeText::getLength() const
{
	assert(unsigned(length(m_data)) == length(m_data));

	return unsigned(length(m_data));
}



XalanDOMString
XalanSourceTreeText::substringData(
			unsigned int	offset,
			unsigned int	count) const
{
	return substring(m_data, offset, count);
}



void
XalanSourceTreeText::appendData(const XalanDOMString&	/* arg */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XalanSourceTreeText::insertData(
			unsigned int			/* offset */,
			const  XalanDOMString& 	/* arg */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XalanSourceTreeText::deleteData(
			unsigned int	/* offset */,
			unsigned int	/* count */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XalanSourceTreeText::replaceData(
			unsigned int			/* offset */,
			unsigned int			/* count */,
			const XalanDOMString&	/* arg */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



XalanText*
XalanSourceTreeText::splitText(unsigned int		/* offset */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
	return 0;
}



bool
XalanSourceTreeText::isIgnorableWhitespace() const
{
	return false;
}



void
XalanSourceTreeText::setParent(XalanSourceTreeElement*	theParent)
{
	m_parentNode = theParent;
}



void
XalanSourceTreeText::setParent(XalanSourceTreeDocumentFragment*	theParent)
{
	m_parentNode = theParent;
}



void
XalanSourceTreeText::setPreviousSibling(XalanSourceTreeComment*		thePreviousSibling)
{
	m_previousSibling = thePreviousSibling;
}



void
XalanSourceTreeText::setPreviousSibling(XalanSourceTreeElement*		thePreviousSibling)
{
	m_previousSibling = thePreviousSibling;
}



void
XalanSourceTreeText::setPreviousSibling(XalanSourceTreeProcessingInstruction*	thePreviousSibling)
{
	m_previousSibling = thePreviousSibling;
}



void
XalanSourceTreeText::setPreviousSibling(XalanSourceTreeText*	thePreviousSibling)
{
	m_previousSibling = thePreviousSibling;
}



void
XalanSourceTreeText::appendSiblingNode(XalanSourceTreeComment*	theSibling)
{
	XalanSourceTreeHelper::appendSibling(this, m_nextSibling, theSibling);
}



void
XalanSourceTreeText::appendSiblingNode(XalanSourceTreeElement*	theSibling)
{
	XalanSourceTreeHelper::appendSibling(this, m_nextSibling, theSibling);
}



void
XalanSourceTreeText::appendSiblingNode(XalanSourceTreeProcessingInstruction*	theSibling)
{
	XalanSourceTreeHelper::appendSibling(this, m_nextSibling, theSibling);
}



void
XalanSourceTreeText::appendSiblingNode(XalanSourceTreeText*		theSibling)
{
	XalanSourceTreeHelper::appendSibling(this, m_nextSibling, theSibling);
}



static XalanDOMString	s_nameString;



const XalanDOMString&	XalanSourceTreeText::s_nameString = ::s_nameString;



void
XalanSourceTreeText::initialize()
{
	::s_nameString = XALAN_STATIC_UCODE_STRING("#text");
}



void
XalanSourceTreeText::terminate()
{
	releaseMemory(::s_nameString);
}
