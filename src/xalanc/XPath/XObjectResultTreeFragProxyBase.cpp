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
// Class header file.
#include "XObjectResultTreeFragProxyBase.hpp"



#include <xalanc/XalanDOM/XalanDOMException.hpp>



#include "NodeRefListBase.hpp"
#include "XNodeSetBase.hpp"



XALAN_CPP_NAMESPACE_BEGIN



const XalanDOMString	XObjectResultTreeFragProxyBase::s_emptyString;



XObjectResultTreeFragProxyBase::XObjectResultTreeFragProxyBase() :
	XalanDocumentFragment()
{
}



XObjectResultTreeFragProxyBase::XObjectResultTreeFragProxyBase(const XObjectResultTreeFragProxyBase&	/* theRHS */)
{
}



XObjectResultTreeFragProxyBase::~XObjectResultTreeFragProxyBase()
{
}



const XalanDOMString&
XObjectResultTreeFragProxyBase::getNodeName() const
{
	return s_emptyString;
}



const XalanDOMString&
XObjectResultTreeFragProxyBase::getNodeValue() const
{
	return s_emptyString;
}



XObjectResultTreeFragProxyBase::NodeType
XObjectResultTreeFragProxyBase::getNodeType() const
{
	return DOCUMENT_FRAGMENT_NODE;
}



XalanNode*
XObjectResultTreeFragProxyBase::getParentNode() const
{
	return 0;
}



const XalanNodeList*
XObjectResultTreeFragProxyBase::getChildNodes() const
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	return 0;
}


XalanNode*
XObjectResultTreeFragProxyBase::getPreviousSibling() const
{
	return 0;
}



XalanNode*
XObjectResultTreeFragProxyBase::getNextSibling() const
{
	return 0;
}



const XalanNamedNodeMap*
XObjectResultTreeFragProxyBase::getAttributes() const
{
	return 0;
}



XalanDocument*
XObjectResultTreeFragProxyBase::getOwnerDocument() const
{
	return 0;
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XObjectResultTreeFragProxyBase*
#endif
XObjectResultTreeFragProxyBase::cloneNode(bool	/* deep */) const
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	return 0;
}



XalanNode*
XObjectResultTreeFragProxyBase::insertBefore(
			XalanNode*	/* newChild */,
			XalanNode*	/* refChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XObjectResultTreeFragProxyBase::replaceChild(
			XalanNode*	/* newChild */,
			XalanNode*	/* oldChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XObjectResultTreeFragProxyBase::appendChild(XalanNode*	/* newChild */ )
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XObjectResultTreeFragProxyBase::removeChild(XalanNode*	/* oldChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



void
XObjectResultTreeFragProxyBase::setNodeValue(const XalanDOMString&	/* nodeValue */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XObjectResultTreeFragProxyBase::normalize()
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
XObjectResultTreeFragProxyBase::isSupported(
			const XalanDOMString&	/* feature */,
			const XalanDOMString&	/* version */) const
{
	return false;
}



const XalanDOMString&
XObjectResultTreeFragProxyBase::getNamespaceURI() const
{
	return s_emptyString;
}



const XalanDOMString&
XObjectResultTreeFragProxyBase::getPrefix() const
{
	return s_emptyString;
}



const XalanDOMString&
XObjectResultTreeFragProxyBase::getLocalName() const
{
	return s_emptyString;
}



void
XObjectResultTreeFragProxyBase::setPrefix(const XalanDOMString&		/* prefix */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
XObjectResultTreeFragProxyBase::isIndexed() const
{
	return false;
}



XObjectResultTreeFragProxyBase::IndexType
XObjectResultTreeFragProxyBase::getIndex() const
{
	return 0;
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanDocumentFragment*
#else
XObjectResultTreeFragProxyBase*
#endif
XObjectResultTreeFragProxyBase::clone(bool	/* deep */) const
{
	assert(false);

	return 0;
}



XALAN_CPP_NAMESPACE_END
