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
#include "XalanSourceTreeCDATASection.hpp"



#include <XalanDOM/XalanDOMException.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>



#include "XalanSourceTreeComment.hpp"
#include "XalanSourceTreeDocument.hpp"
#include "XalanSourceTreeElement.hpp"
#include "XalanSourceTreeProcessingInstruction.hpp"
#include "XalanSourceTreeText.hpp"
#include "XalanSourceTreeHelper.hpp"



static const XalanDOMString		s_emptyString;



XalanSourceTreeCDATASection::XalanSourceTreeCDATASection(
			const XalanDOMString&		theData,
			XalanSourceTreeElement*		theParentElement,
			XalanNode*					thePreviousSibling,
			XalanNode*					theNextSibling,
			unsigned int				theIndex) :
	XalanCDATASection(),
	m_data(theData),
	m_parentElement(theParentElement),
	m_previousSibling(thePreviousSibling),
	m_nextSibling(theNextSibling),
	m_index(theIndex)
{
}



XalanSourceTreeCDATASection::~XalanSourceTreeCDATASection()
{
}



XalanSourceTreeCDATASection::XalanSourceTreeCDATASection(
			const XalanSourceTreeCDATASection&	theSource,
			bool							/* deep */) :
	XalanCDATASection(theSource),
	m_data(theSource.m_data),
	m_parentElement(0),
	m_previousSibling(0),
	m_nextSibling(0),
	m_index(0)
{
}



const XalanDOMString&
XalanSourceTreeCDATASection::getNodeName() const
{
	return s_emptyString;
}



const XalanDOMString&
XalanSourceTreeCDATASection::getNodeValue() const
{
	return m_data;
}



XalanSourceTreeCDATASection::NodeType
XalanSourceTreeCDATASection::getNodeType() const
{
	return CDATA_SECTION_NODE;
}



XalanNode*
XalanSourceTreeCDATASection::getParentNode() const
{
	return m_parentElement;
}



const XalanNodeList*
XalanSourceTreeCDATASection::getChildNodes() const
{
	return 0;
}



XalanNode*
XalanSourceTreeCDATASection::getFirstChild() const
{
	return 0;
}



XalanNode*
XalanSourceTreeCDATASection::getLastChild() const
{
	return 0;
}



XalanNode*
XalanSourceTreeCDATASection::getPreviousSibling() const
{
	return m_previousSibling;
}



XalanNode*
XalanSourceTreeCDATASection::getNextSibling() const
{
	return m_nextSibling;
}



const XalanNamedNodeMap*
XalanSourceTreeCDATASection::getAttributes() const
{
	return 0;
}



XalanDocument*
XalanSourceTreeCDATASection::getOwnerDocument() const
{
	assert(m_parentElement != 0);

	return m_parentElement->getDocument();
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XalanSourceTreeCDATASection*
#endif
XalanSourceTreeCDATASection::cloneNode(bool	deep) const
{
	return new XalanSourceTreeCDATASection(*this, deep);
}



XalanNode*
XalanSourceTreeCDATASection::insertBefore(
			XalanNode*	/* newChild */,
			XalanNode*	/* refChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeCDATASection::replaceChild(
			XalanNode*	/* newChild */,
			XalanNode*	/* oldChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeCDATASection::removeChild(XalanNode*	/* oldChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeCDATASection::appendChild(XalanNode*	/* newChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



bool
XalanSourceTreeCDATASection::hasChildNodes() const
{
	return false;
}



void
XalanSourceTreeCDATASection::setNodeValue(const XalanDOMString&		/* nodeValue */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XalanSourceTreeCDATASection::normalize()
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
XalanSourceTreeCDATASection::supports(
			const XalanDOMString&	/* feature */,
			const XalanDOMString&	/* version */) const
{
	return false;
}



const XalanDOMString&
XalanSourceTreeCDATASection::getNamespaceURI() const
{
	return s_emptyString;
}



const XalanDOMString&
XalanSourceTreeCDATASection::getPrefix() const
{
	return s_emptyString;
}



const XalanDOMString&
XalanSourceTreeCDATASection::getLocalName() const
{
	return s_emptyString;
}



void
XalanSourceTreeCDATASection::setPrefix(const XalanDOMString&	/* prefix */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
XalanSourceTreeCDATASection::isIndexed() const
{
	return true;
}



unsigned long
XalanSourceTreeCDATASection::getIndex() const
{
	return m_index;
}



const XalanDOMString&
XalanSourceTreeCDATASection::getData() const
{
	return m_data;
}



unsigned int
XalanSourceTreeCDATASection::getLength() const
{
	return length(m_data);
}



XalanDOMString
XalanSourceTreeCDATASection::substringData(
			unsigned int	offset,
			unsigned int	count) const
{
	return substring(m_data, offset, count);
}



void
XalanSourceTreeCDATASection::appendData(const XalanDOMString&	/* arg */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XalanSourceTreeCDATASection::insertData(
			unsigned int			/* offset */,
			const  XalanDOMString& 	/* arg */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XalanSourceTreeCDATASection::deleteData(
			unsigned int	/* offset */,
			unsigned int	/* count */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XalanSourceTreeCDATASection::replaceData(
			unsigned int			/* offset */,
			unsigned int			/* count */,
			const XalanDOMString&	/* arg */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



XalanText*
XalanSourceTreeCDATASection::splitText(unsigned int		/* offset */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
XalanSourceTreeCDATASection::isIgnorableWhitespace() const
{
	return false;
}



void
XalanSourceTreeCDATASection::setPreviousSibling(XalanSourceTreeComment*	thePreviousSibling)
{
	m_previousSibling = thePreviousSibling;
}



void
XalanSourceTreeCDATASection::setPreviousSibling(XalanSourceTreeCDATASection*		thePreviousSibling)
{
	m_previousSibling = thePreviousSibling;
}



void
XalanSourceTreeCDATASection::setPreviousSibling(XalanSourceTreeElement*	thePreviousSibling)
{
	m_previousSibling = thePreviousSibling;
}



void
XalanSourceTreeCDATASection::setPreviousSibling(XalanSourceTreeProcessingInstruction*	thePreviousSibling)
{
	m_previousSibling = thePreviousSibling;
}



void
XalanSourceTreeCDATASection::setPreviousSibling(XalanSourceTreeText*	thePreviousSibling)
{
	m_previousSibling = thePreviousSibling;
}



void
XalanSourceTreeCDATASection::appendSiblingNode(XalanSourceTreeComment*	theSibling)
{
	XalanSourceTreeHelper::appendSibling(this, m_nextSibling, theSibling);
}



void
XalanSourceTreeCDATASection::appendSiblingNode(XalanSourceTreeCDATASection*		theSibling)
{
	XalanSourceTreeHelper::appendSibling(this, m_nextSibling, theSibling);
}



void
XalanSourceTreeCDATASection::appendSiblingNode(XalanSourceTreeElement*	theSibling)
{
	XalanSourceTreeHelper::appendSibling(this, m_nextSibling, theSibling);
}



void
XalanSourceTreeCDATASection::appendSiblingNode(XalanSourceTreeProcessingInstruction*	theSibling)
{
	XalanSourceTreeHelper::appendSibling(this, m_nextSibling, theSibling);
}



void
XalanSourceTreeCDATASection::appendSiblingNode(XalanSourceTreeText*		theSibling)
{
	XalanSourceTreeHelper::appendSibling(this, m_nextSibling, theSibling);
}
