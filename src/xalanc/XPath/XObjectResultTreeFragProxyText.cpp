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
#include "XObjectResultTreeFragProxyText.hpp"



#include <xalanc/XalanDOM/XalanDOMException.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



#include "XObject.hpp"



XALAN_CPP_NAMESPACE_BEGIN



static const XalanDOMString		s_emptyString;



XObjectResultTreeFragProxyText::XObjectResultTreeFragProxyText(const XObject&	theXObject) :
	XalanText(),
	m_value(theXObject)
{
}



XObjectResultTreeFragProxyText::~XObjectResultTreeFragProxyText()
{
}



const XalanDOMString&
XObjectResultTreeFragProxyText::getNodeName() const
{
	return s_nameString;
}



const XalanDOMString&
XObjectResultTreeFragProxyText::getNodeValue() const
{
	return m_value.str();
}



XObjectResultTreeFragProxyText::NodeType
XObjectResultTreeFragProxyText::getNodeType() const
{
	return TEXT_NODE;
}



XalanNode*
XObjectResultTreeFragProxyText::getParentNode() const
{
	return 0;
}



const XalanNodeList*
XObjectResultTreeFragProxyText::getChildNodes() const
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XObjectResultTreeFragProxyText::getFirstChild() const
{
	return 0;
}



XalanNode*
XObjectResultTreeFragProxyText::getLastChild() const
{
	return 0;
}



XalanNode*
XObjectResultTreeFragProxyText::getPreviousSibling() const
{
	return 0;
}



XalanNode*
XObjectResultTreeFragProxyText::getNextSibling() const
{
	return 0;
}



const XalanNamedNodeMap*
XObjectResultTreeFragProxyText::getAttributes() const
{
	return 0;
}



XalanDocument*
XObjectResultTreeFragProxyText::getOwnerDocument() const
{
	return 0;
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XObjectResultTreeFragProxyText*
#endif
XObjectResultTreeFragProxyText::cloneNode(bool	/* deep */) const
{
	assert(false);

	return 0;
}



XalanNode*
XObjectResultTreeFragProxyText::insertBefore(
			XalanNode*	/* newChild */,
			XalanNode*	/* refChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XObjectResultTreeFragProxyText::replaceChild(
			XalanNode*	/* newChild */,
			XalanNode*	/* oldChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XObjectResultTreeFragProxyText::removeChild(XalanNode*	/* oldChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XObjectResultTreeFragProxyText::appendChild(XalanNode*	/* newChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



bool
XObjectResultTreeFragProxyText::hasChildNodes() const
{
	return false;
}



void
XObjectResultTreeFragProxyText::setNodeValue(const XalanDOMString&		/* nodeValue */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XObjectResultTreeFragProxyText::normalize()
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
XObjectResultTreeFragProxyText::isSupported(
			const XalanDOMString&	/* feature */,
			const XalanDOMString&	/* version */) const
{
	return false;
}



const XalanDOMString&
XObjectResultTreeFragProxyText::getNamespaceURI() const
{
	return s_emptyString;
}



const XalanDOMString&
XObjectResultTreeFragProxyText::getPrefix() const
{
	return s_emptyString;
}



const XalanDOMString&
XObjectResultTreeFragProxyText::getLocalName() const
{
	return s_emptyString;
}



void
XObjectResultTreeFragProxyText::setPrefix(const XalanDOMString&	/* prefix */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
XObjectResultTreeFragProxyText::isIndexed() const
{
	return false;
}



XObjectResultTreeFragProxyText::IndexType
XObjectResultTreeFragProxyText::getIndex() const
{
	return 0;
}



const XalanDOMString&
XObjectResultTreeFragProxyText::getData() const
{
	return m_value.str();
}



unsigned int
XObjectResultTreeFragProxyText::getLength() const
{
	assert(unsigned(length(m_value.str())) == length(m_value.str()));

	return unsigned(length(m_value.str()));
}



XalanDOMString
XObjectResultTreeFragProxyText::substringData(
			unsigned int	offset,
			unsigned int	count) const
{
	return m_value.str().substr(offset, count);
}



void
XObjectResultTreeFragProxyText::appendData(const XalanDOMString&	/* arg */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XObjectResultTreeFragProxyText::insertData(
			unsigned int			/* offset */,
			const  XalanDOMString& 	/* arg */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XObjectResultTreeFragProxyText::deleteData(
			unsigned int	/* offset */,
			unsigned int	/* count */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XObjectResultTreeFragProxyText::replaceData(
			unsigned int			/* offset */,
			unsigned int			/* count */,
			const XalanDOMString&	/* arg */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



XalanText*
XObjectResultTreeFragProxyText::splitText(unsigned int	/* offset */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	return 0;
}



bool
XObjectResultTreeFragProxyText::isIgnorableWhitespace() const
{
	return isXMLWhitespace(m_value.str());
}



XALAN_CPP_NAMESPACE_END



static XALAN_CPP_NAMESPACE_QUALIFIER XalanDOMString		s_nameString;



XALAN_CPP_NAMESPACE_BEGIN



const XalanDOMString&	XObjectResultTreeFragProxyText::s_nameString = ::s_nameString;



void
XObjectResultTreeFragProxyText::initialize()
{
	::s_nameString = XALAN_STATIC_UCODE_STRING("#text");
}



void
XObjectResultTreeFragProxyText::terminate()
{
	releaseMemory(::s_nameString);
}



XALAN_CPP_NAMESPACE_END
