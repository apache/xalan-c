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
#include "XalanSourceTreeAttr.hpp"



#include <cassert>



#include <xalanc/XalanDOM/XalanDOMException.hpp>



#include "XalanSourceTreeElement.hpp"



XALAN_CPP_NAMESPACE_BEGIN



static const XalanDOMString		s_emptyString;



XalanSourceTreeAttr::XalanSourceTreeAttr(
			const XalanDOMString&		theName,
			const XalanDOMString&		theValue,
			XalanSourceTreeElement*		theOwnerElement,
			IndexType					theIndex) :
	XalanAttr(),
	m_name(theName),
	m_value(theValue),
	m_ownerElement(theOwnerElement),
	m_index(theIndex)
{
}



XalanSourceTreeAttr::~XalanSourceTreeAttr()
{
}



XalanSourceTreeAttr::XalanSourceTreeAttr(
			const XalanSourceTreeAttr&		theSource,
			bool							/* deep */) :
	XalanAttr(theSource),
	m_name(theSource.m_name),
	m_value(theSource.m_value),
	m_ownerElement(0),
	m_index(0)
{
	// Deep copy is the same as shallow copy -- The owning element
	// must deal with the hookup of the siblings.
}



const XalanDOMString&
XalanSourceTreeAttr::getNodeName() const
{
	return m_name;
}



const XalanDOMString&
XalanSourceTreeAttr::getNodeValue() const
{
	return m_value;
}



XalanSourceTreeAttr::NodeType
XalanSourceTreeAttr::getNodeType() const
{
	return ATTRIBUTE_NODE;
}



XalanNode*
XalanSourceTreeAttr::getParentNode() const
{
	return 0;
}



const XalanNodeList*
XalanSourceTreeAttr::getChildNodes() const
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeAttr::getFirstChild() const
{
	return 0;
}



XalanNode*
XalanSourceTreeAttr::getLastChild() const
{
	return 0;
}



XalanNode*
XalanSourceTreeAttr::getPreviousSibling() const
{
	return 0;
}



XalanNode*
XalanSourceTreeAttr::getNextSibling() const
{
	return 0;
}



const XalanNamedNodeMap*
XalanSourceTreeAttr::getAttributes() const
{
	return 0;
}



XalanDocument*
XalanSourceTreeAttr::getOwnerDocument() const
{
	assert(m_ownerElement != 0);

	return m_ownerElement->getOwnerDocument();
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XalanSourceTreeAttr*
#endif
XalanSourceTreeAttr::cloneNode(bool		/* deep */) const
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeAttr::insertBefore(
			XalanNode*	/* newChild */,
			XalanNode*	/* refChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeAttr::replaceChild(
			XalanNode*	/* newChild */,
			XalanNode*	/* oldChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeAttr::removeChild(XalanNode*		/* oldChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeAttr::appendChild(XalanNode*		/* newChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



bool
XalanSourceTreeAttr::hasChildNodes() const
{
	return false;
}



void
XalanSourceTreeAttr::setNodeValue(const XalanDOMString&		/* nodeValue */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XalanSourceTreeAttr::normalize()
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
XalanSourceTreeAttr::isSupported(
			const XalanDOMString&	/* feature */,
			const XalanDOMString&	/* version */) const
{
	return false;
}



const XalanDOMString&
XalanSourceTreeAttr::getNamespaceURI() const
{
	return s_emptyString;
}



const XalanDOMString&
XalanSourceTreeAttr::getPrefix() const
{
	return s_emptyString;
}



const XalanDOMString&
XalanSourceTreeAttr::getLocalName() const
{
	return s_emptyString;
}



void
XalanSourceTreeAttr::setPrefix(const XalanDOMString&	/* prefix */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
XalanSourceTreeAttr::isIndexed() const
{
	return true;
}



XalanSourceTreeAttr::IndexType
XalanSourceTreeAttr::getIndex() const
{
	return m_index;
}



const XalanDOMString&
XalanSourceTreeAttr::getName() const
{
	return m_name;
}



bool
XalanSourceTreeAttr::getSpecified() const
{
	return false;
}



const XalanDOMString&
XalanSourceTreeAttr::getValue() const
{
	return m_value;
}



void
XalanSourceTreeAttr::setValue(const XalanDOMString&		/* value */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



XalanElement*
XalanSourceTreeAttr::getOwnerElement() const
{
	return m_ownerElement;
}



XALAN_CPP_NAMESPACE_END
