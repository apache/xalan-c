/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2003 The Apache Software Foundation.  All rights 
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
#include "XercesElementWrapper.hpp"



#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMNodeList.hpp>



#include <xalanc/XalanDOM/XalanAttr.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "XercesWrapperHelper.hpp"
#include "XercesWrapperNavigator.hpp"
#include "XercesDOMWrapperException.hpp"
#include "XercesDocumentWrapper.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XercesElementWrapper::XercesElementWrapper(
			const DOMElementType*			theXercesElement,
			const XercesWrapperNavigator&	theNavigator) :
	XalanElement(),
	m_xercesNode(theXercesElement),
	m_navigator(theNavigator),
	m_attributes(theXercesElement->getAttributes(),
				 theNavigator)
{
	assert(theXercesElement != 0);
}



XercesElementWrapper::~XercesElementWrapper()
{
}



const XalanDOMString&
XercesElementWrapper::getNodeName() const
{
	return m_navigator.getPooledString(m_xercesNode->getNodeName());
}



const XalanDOMString&
XercesElementWrapper::getNodeValue() const
{
	return m_navigator.getPooledString(m_xercesNode->getNodeValue());
}



XercesElementWrapper::NodeType
XercesElementWrapper::getNodeType() const
{
	return ELEMENT_NODE;
}



XalanNode*
XercesElementWrapper::getParentNode() const
{
	return m_navigator.getParentNode(m_xercesNode);
}



const XalanNodeList*
XercesElementWrapper::getChildNodes() const
{
	return this;
}



XalanNode*
XercesElementWrapper::getFirstChild() const
{
	return m_navigator.getFirstChild(m_xercesNode);
}



XalanNode*
XercesElementWrapper::getLastChild() const
{
	return m_navigator.getLastChild(m_xercesNode);
}



XalanNode*
XercesElementWrapper::getPreviousSibling() const
{
	return m_navigator.getPreviousSibling(m_xercesNode);
}



XalanNode*
XercesElementWrapper::getNextSibling() const
{
	return m_navigator.getNextSibling(m_xercesNode);
}



const XalanNamedNodeMap*
XercesElementWrapper::getAttributes() const
{
	return &m_attributes;
}



XalanDocument*
XercesElementWrapper::getOwnerDocument() const
{
	return m_navigator.getOwnerDocument();
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XercesElementWrapper*
#endif
XercesElementWrapper::cloneNode(bool	/* deep */) const
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::NOT_SUPPORTED_ERR);

	return 0;
}



XalanNode*
XercesElementWrapper::insertBefore(
			XalanNode*	/* newChild */,
			XalanNode*	/* refChild */)
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::HIERARCHY_REQUEST_ERR);

	return 0;
}



XalanNode*
XercesElementWrapper::replaceChild(
			XalanNode*	/* newChild */,
			XalanNode*	/* oldChild */)
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::HIERARCHY_REQUEST_ERR);

	return 0;
}



XalanNode*
XercesElementWrapper::removeChild(XalanNode*	/* oldChild */)
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::HIERARCHY_REQUEST_ERR);

	return 0;
}



XalanNode*
XercesElementWrapper::appendChild(XalanNode*	/* newChild */)
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::HIERARCHY_REQUEST_ERR);

	return 0;
}



bool
XercesElementWrapper::hasChildNodes() const
{
	return m_xercesNode->hasChildNodes();
}



void
XercesElementWrapper::setNodeValue(const XalanDOMString&	/* nodeValue */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XercesElementWrapper::normalize()
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
XercesElementWrapper::isSupported(
			const XalanDOMString&	feature,
			const XalanDOMString&	version) const
{
	return XercesWrapperHelper::isSupported(m_xercesNode, feature, version);
}



const XalanDOMString&
XercesElementWrapper::getNamespaceURI() const
{
	return m_navigator.getPooledString(m_xercesNode->getNamespaceURI());
}



const XalanDOMString&
XercesElementWrapper::getPrefix() const
{
	return m_navigator.getPooledString(m_xercesNode->getPrefix());
}



const XalanDOMString&
XercesElementWrapper::getLocalName() const
{
	return m_navigator.getPooledString(m_xercesNode->getLocalName());
}



void
XercesElementWrapper::setPrefix(const XalanDOMString&	/* prefix */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
XercesElementWrapper::isIndexed() const
{
	return m_navigator.getOwnerDocument()->isIndexed();
}



XercesElementWrapper::IndexType
XercesElementWrapper::getIndex() const
{
	return m_navigator.getIndex();
}



const XalanDOMString&
XercesElementWrapper::getTagName() const
{
	return m_navigator.getPooledString(m_xercesNode->getTagName());
}



const XalanDOMString&
XercesElementWrapper::getAttribute(const XalanDOMString&	name) const
{
	return m_navigator.getPooledString(m_xercesNode->getAttribute(c_wstr(name)));
}



XalanAttr*
XercesElementWrapper::getAttributeNode(const XalanDOMString&	name) const
{
#if defined(XALAN_OLD_STYLE_CASTS)
	return (XalanAttr*)m_attributes.getNamedItem(name);
#else
	return static_cast<XalanAttr*>(m_attributes.getNamedItem(name));
#endif
}



XalanNodeList*
XercesElementWrapper::getElementsByTagName(const XalanDOMString&	/* name */) const
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::NOT_SUPPORTED_ERR);

	return 0;
}



void
XercesElementWrapper::setAttribute(
			const XalanDOMString&	/* name */,
			const XalanDOMString&	/* value */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



XalanAttr*
XercesElementWrapper::setAttributeNode(XalanAttr*	/* newAttr */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	return 0;
}



XalanAttr*
XercesElementWrapper::removeAttributeNode(XalanAttr*	/* oldAttr */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	return 0;
}



void
XercesElementWrapper::removeAttribute(const XalanDOMString&		/* name */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



const XalanDOMString&
XercesElementWrapper::getAttributeNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	localName) const
{
	return m_navigator.getPooledString(m_xercesNode->getAttributeNS(c_wstr(namespaceURI), c_wstr(localName)));
}



void
XercesElementWrapper::setAttributeNS(
			const XalanDOMString&	/* namespaceURI */,
			const XalanDOMString&	/* qualifiedName */,
			const XalanDOMString&	/* value */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XercesElementWrapper::removeAttributeNS(
			const XalanDOMString&	/* namespaceURI */,
			const XalanDOMString&	/* localName */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



XalanAttr*
XercesElementWrapper::getAttributeNodeNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	localName) const
{
	DOMAttrType* const	theAttrNode =
		m_xercesNode->getAttributeNodeNS(c_wstr(namespaceURI), c_wstr(localName));

	if (theAttrNode == 0)
	{
		return 0;
	}
	else
	{
		return m_navigator.mapNode(theAttrNode);
	}
}



XalanAttr*
XercesElementWrapper::setAttributeNodeNS(XalanAttr*		/* newAttr */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	return 0;
}



XalanNodeList*
XercesElementWrapper::getElementsByTagNameNS(
			const XalanDOMString&	/* namespaceURI */,
			const XalanDOMString&	/* localName */) const
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::NOT_SUPPORTED_ERR);

	return 0;
}



XalanNode*
XercesElementWrapper::item(unsigned int		index) const
{
	assert(index < getLength());

	if (m_navigator.getOwnerDocument()->getMappingMode() == true)
	{
		assert(m_xercesNode->getChildNodes()->item(index));

		return m_navigator.mapNode(m_xercesNode->getChildNodes()->item(index));
	}
	else
	{
		XalanNode*	child = getFirstChild();
		assert(child != 0);

		for(unsigned int i = 0; i < index; ++i)
		{
			child = child->getNextSibling();
			assert(child != 0);
		}

		return child;
	}
}



unsigned int
XercesElementWrapper::getLength() const
{
	return m_xercesNode->getChildNodes()->getLength();
}



XALAN_CPP_NAMESPACE_END
