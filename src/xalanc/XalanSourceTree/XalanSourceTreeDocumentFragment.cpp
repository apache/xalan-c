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
