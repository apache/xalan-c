/*
 * Copyright 1999-2004 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "XalanSourceTreeElement.hpp"



#include <xalanc/XalanDOM/XalanDOMException.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



#include "XalanSourceTreeAttr.hpp"
#include "XalanSourceTreeComment.hpp"
#include "XalanSourceTreeDocument.hpp"
#include "XalanSourceTreeDocumentFragment.hpp"
#include "XalanSourceTreeProcessingInstruction.hpp"
#include "XalanSourceTreeText.hpp"
#include "XalanSourceTreeHelper.hpp"



XALAN_CPP_NAMESPACE_BEGIN



const XalanDOMString		XalanSourceTreeElement::s_emptyString;



XalanSourceTreeElement::XalanSourceTreeElement(
			const XalanDOMString&		theTagName,
			XalanSourceTreeDocument*	theOwnerDocument,
			XalanNode*					theParentNode,
			XalanNode*					thePreviousSibling,
			XalanNode*					theNextSibling,
			IndexType					theIndex) :
	XalanElement(),
	m_tagName(theTagName),
	m_ownerDocument(theOwnerDocument),
	m_parentNode(theParentNode),
	m_previousSibling(thePreviousSibling),
	m_nextSibling(theNextSibling),
	m_firstChild(0),
	m_index(theIndex)
{
}



XalanSourceTreeElement::~XalanSourceTreeElement()
{
}



XalanSourceTreeElement::XalanSourceTreeElement(
			const XalanSourceTreeElement&	theSource,
			bool								deep) :
	XalanElement(theSource),
	m_tagName(theSource.m_tagName),
	m_ownerDocument(theSource.m_ownerDocument),
	m_parentNode(0),
	m_previousSibling(0),
	m_nextSibling(0),
	m_firstChild(theSource.m_firstChild == 0 ? 0 : theSource.m_firstChild->cloneNode(deep)),
	m_index(0)
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



XalanDocument*
XalanSourceTreeElement::getOwnerDocument() const
{
	return m_ownerDocument;
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



XalanSourceTreeElement::IndexType
XalanSourceTreeElement::getIndex() const
{
	return m_index;
}



const XalanDOMString&
XalanSourceTreeElement::getTagName() const
{
	return m_tagName;
}



XalanNodeList*
XalanSourceTreeElement::getElementsByTagName(const XalanDOMString&	/* name */) const
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	// Dummy return value...
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
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	// Dummy return value...
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



XALAN_CPP_NAMESPACE_END
