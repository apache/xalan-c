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
