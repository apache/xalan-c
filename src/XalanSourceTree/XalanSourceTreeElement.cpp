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
#include "XalanSourceTreeElement.hpp"



#include <XalanDOM/XalanDOMException.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>



#include "XalanSourceTreeAttr.hpp"
#include "XalanSourceTreeComment.hpp"
#include "XalanSourceTreeDocument.hpp"
#include "XalanSourceTreeDocumentFragment.hpp"
#include "XalanSourceTreeProcessingInstruction.hpp"
#include "XalanSourceTreeText.hpp"
#include "XalanSourceTreeHelper.hpp"



static const XalanDOMString		s_emptyString;



XalanSourceTreeElement::XalanSourceTreeElement(
			const XalanDOMString&		theTagName,
			XalanSourceTreeDocument*	theOwnerDocument,
			XalanSourceTreeAttr**		theAttributes,
			AttributesCountType			theAttributeCount,
			XalanNode*					theParentNode,
			XalanNode*					thePreviousSibling,
			XalanNode*					theNextSibling,
			unsigned int				theIndex) :
	XalanElement(),
	m_tagName(theTagName),
	m_ownerDocument(theOwnerDocument),
	m_parentNode(theParentNode),
	m_previousSibling(thePreviousSibling),
	m_nextSibling(theNextSibling),
	m_firstChild(0),
	m_index(theIndex),
	m_attributes(theAttributes),
	m_attributeCount(theAttributeCount)
{
}



XalanSourceTreeElement::~XalanSourceTreeElement()
{
}



XalanSourceTreeElement::XalanSourceTreeElement(
			const XalanSourceTreeElement&	theSource,
			bool							deep) :
	XalanElement(theSource),
	m_tagName(theSource.m_tagName),
	m_ownerDocument(theSource.m_ownerDocument),
	m_parentNode(0),
	m_previousSibling(0),
	m_nextSibling(0),
	m_firstChild(theSource.m_firstChild == 0 ? 0 : theSource.m_firstChild->cloneNode(deep)),
	m_index(0),
	m_attributes(theSource.m_attributes),
	m_attributeCount(theSource.m_attributeCount)
{
}



const XalanDOMString&
XalanSourceTreeElement::getNodeName() const
{
	return m_tagName;
}



const XalanDOMString&
XalanSourceTreeElement::getNodeValue() const
{
	return s_emptyString;
}



XalanSourceTreeElement::NodeType
XalanSourceTreeElement::getNodeType() const
{
	return ELEMENT_NODE;
}



XalanNode*
XalanSourceTreeElement::getParentNode() const
{
	if (m_parentNode != 0)
	{
		return m_parentNode;
	}
	else
	{
		return m_ownerDocument;
	}
}



const XalanNodeList*
XalanSourceTreeElement::getChildNodes() const
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeElement::getFirstChild() const
{
	return m_firstChild;
}



XalanNode*
XalanSourceTreeElement::getLastChild() const
{
	return XalanSourceTreeHelper::getLastSibling(m_firstChild);
}



XalanNode*
XalanSourceTreeElement::getPreviousSibling() const
{
	return m_previousSibling;
}



XalanNode*
XalanSourceTreeElement::getNextSibling() const
{
	return m_nextSibling;
}



const XalanNamedNodeMap*
XalanSourceTreeElement::getAttributes() const
{
	return this;
}



XalanDocument*
XalanSourceTreeElement::getOwnerDocument() const
{
	return m_ownerDocument;
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XalanSourceTreeElement*
#endif
XalanSourceTreeElement::cloneNode(bool	deep) const
{
	return clone(deep);
}



XalanNode*
XalanSourceTreeElement::insertBefore(
			XalanNode*	/* newChild */,
			XalanNode*	/* refChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeElement::replaceChild(
			XalanNode*	/* newChild */,
			XalanNode*	/* oldChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeElement::removeChild(XalanNode*	/* oldChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeElement::appendChild(XalanNode*	/* newChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



bool
XalanSourceTreeElement::hasChildNodes() const
{
	return m_firstChild != 0 ? true : false;
}



void
XalanSourceTreeElement::setNodeValue(const XalanDOMString&		/* nodeValue */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XalanSourceTreeElement::normalize()
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
XalanSourceTreeElement::isSupported(
			const XalanDOMString&	/* feature */,
			const XalanDOMString&	/* version */) const
{
	return false;
}



const XalanDOMString&
XalanSourceTreeElement::getNamespaceURI() const
{
	return s_emptyString;
}



const XalanDOMString&
XalanSourceTreeElement::getPrefix() const
{
	return s_emptyString;
}



const XalanDOMString&
XalanSourceTreeElement::getLocalName() const
{
	return s_emptyString;
}



void
XalanSourceTreeElement::setPrefix(const XalanDOMString&	/* prefix */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
XalanSourceTreeElement::isIndexed() const
{
	return true;
}



unsigned long
XalanSourceTreeElement::getIndex() const
{
	return m_index;
}



const XalanDOMString&
XalanSourceTreeElement::getTagName() const
{
	return m_tagName;
}



const XalanDOMString&
XalanSourceTreeElement::getAttribute(const XalanDOMString&		name) const
{
	XalanAttr* const	theAttr = getAttributeNode(name);

	return theAttr == 0 ? s_emptyString : theAttr->getValue();
}



XalanAttr*
XalanSourceTreeElement::getAttributeNode(const XalanDOMString&		name) const
{
	for(unsigned int i = 0; i < m_attributeCount; ++i)
	{
		assert(m_attributes[i] != 0);

		if (equals(m_attributes[i]->getNodeName(), name) == true)
		{
			return m_attributes[i];
		}
	}

	return 0;
}



XalanNodeList*
XalanSourceTreeElement::getElementsByTagName(const XalanDOMString&	/* name */) const
{
	return 0;
}



void
XalanSourceTreeElement::setAttribute(
			const XalanDOMString&	/* name */,
			const XalanDOMString&	/* value */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



XalanAttr*
XalanSourceTreeElement::setAttributeNode(XalanAttr*		/* newAttr */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanAttr*
XalanSourceTreeElement::removeAttributeNode(XalanAttr*	/* oldAttr */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



void
XalanSourceTreeElement::removeAttribute(const XalanDOMString&	/* name */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



const XalanDOMString&
XalanSourceTreeElement::getAttributeNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	localName) const
{
	XalanAttr* const	theAttr = getAttributeNodeNS(namespaceURI, localName);

	return theAttr == 0 ? s_emptyString : theAttr->getValue();
}



void
XalanSourceTreeElement::setAttributeNS(
			const XalanDOMString&	/* namespaceURI */,
			const XalanDOMString&	/* qualifiedName */,
			const XalanDOMString&	/* value */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XalanSourceTreeElement::removeAttributeNS(
			const XalanDOMString&	/* namespaceURI */,
			const XalanDOMString&	/* localName */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



XalanAttr*
XalanSourceTreeElement::getAttributeNodeNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	localName) const
{
	for(unsigned int i = 0; i < m_attributeCount; ++i)
	{
		assert(m_attributes[i] != 0);

		if (equals(m_attributes[i]->getLocalName(), localName) == true &&
			equals(m_attributes[i]->getNamespaceURI(), namespaceURI) == true)
		{
			return m_attributes[i];
		}
	}

	return 0;
}



XalanAttr*
XalanSourceTreeElement::setAttributeNodeNS(XalanAttr*	/* newAttr */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNodeList*
XalanSourceTreeElement::getElementsByTagNameNS(
			const XalanDOMString&	/* namespaceURI */,
			const XalanDOMString&	/* localName */) const
{
	return 0;
}



void
XalanSourceTreeElement::setParent(XalanSourceTreeDocumentFragment*	theParent)
{
	m_parentNode = theParent;
}



void
XalanSourceTreeElement::setPreviousSibling(XalanSourceTreeComment*	thePreviousSibling)
{
	m_previousSibling = thePreviousSibling;
}



void
XalanSourceTreeElement::setPreviousSibling(XalanSourceTreeElement*	thePreviousSibling)
{
	m_previousSibling = thePreviousSibling;
}



void
XalanSourceTreeElement::setPreviousSibling(XalanSourceTreeProcessingInstruction*	thePreviousSibling)
{
	m_previousSibling = thePreviousSibling;
}



void
XalanSourceTreeElement::setPreviousSibling(XalanSourceTreeText*		thePreviousSibling)
{
	m_previousSibling = thePreviousSibling;
}



void
XalanSourceTreeElement::appendSiblingNode(XalanSourceTreeComment*	theSibling)
{
	XalanSourceTreeHelper::appendSibling(this, m_nextSibling, theSibling);
}



void
XalanSourceTreeElement::appendSiblingNode(XalanSourceTreeElement*	theSibling)
{
	XalanSourceTreeHelper::appendSibling(this, m_nextSibling, theSibling);
}



void
XalanSourceTreeElement::appendSiblingNode(XalanSourceTreeProcessingInstruction*		theSibling)
{
	XalanSourceTreeHelper::appendSibling(this, m_nextSibling, theSibling);
}



void
XalanSourceTreeElement::appendSiblingNode(XalanSourceTreeText*	theSibling)
{
	XalanSourceTreeHelper::appendSibling(this, m_nextSibling, theSibling);
}



void
XalanSourceTreeElement::appendChildNode(XalanSourceTreeComment*		theChild)
{
	XalanSourceTreeHelper::appendSiblingToChild(this, m_firstChild, theChild);
}



void
XalanSourceTreeElement::appendChildNode(XalanSourceTreeElement*		theChild)
{
	XalanSourceTreeHelper::appendSiblingToChild(this, m_firstChild, theChild);
}



void
XalanSourceTreeElement::appendChildNode(XalanSourceTreeProcessingInstruction*	theChild)
{
	XalanSourceTreeHelper::appendSiblingToChild(this, m_firstChild, theChild);
}



void
XalanSourceTreeElement::appendChildNode(XalanSourceTreeText*	theChild)
{
	XalanSourceTreeHelper::appendSiblingToChild(this, m_firstChild, theChild);
}



XalanNode*
XalanSourceTreeElement::setNamedItem(XalanNode* 	/* arg */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeElement::item(unsigned int	index) const
{
	return index < m_attributeCount ? m_attributes[index] : 0;
}



XalanNode*
XalanSourceTreeElement::getNamedItem(const XalanDOMString& 	name) const
{
	return getAttributeNode(name);
}



unsigned int
XalanSourceTreeElement::getLength() const
{
	return m_attributeCount;
}



XalanNode*
XalanSourceTreeElement::removeNamedItem(const XalanDOMString&	/* name */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeElement::getNamedItemNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	localName) const
{
	return getAttributeNodeNS(namespaceURI, localName);
}



XalanNode*
XalanSourceTreeElement::setNamedItemNS(XalanNode*	/* arg */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeElement::removeNamedItemNS(
			const XalanDOMString&	/* namespaceURI */,
			const XalanDOMString&	/* localName */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}
