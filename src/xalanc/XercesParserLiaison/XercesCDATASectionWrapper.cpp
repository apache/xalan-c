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
#include "XercesCDATASectionWrapper.hpp"



#include <cassert>



#include <xercesc/dom/DOMCDATASection.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "XercesWrapperHelper.hpp"
#include "XercesWrapperNavigator.hpp"
#include "XercesDOMWrapperException.hpp"
#include "XercesDocumentWrapper.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XercesCDATASectionWrapper::XercesCDATASectionWrapper(
			const DOMCDATASectionType*		theXercesCDATASection,
			const XercesWrapperNavigator&	theNavigator) :
	XalanCDATASection(),
	m_xercesNode(theXercesCDATASection),
	m_navigator(theNavigator)
{
	assert(theXercesCDATASection != 0);
}



XercesCDATASectionWrapper::~XercesCDATASectionWrapper()
{
}



const XalanDOMString&
XercesCDATASectionWrapper::getNodeName() const
{
	return m_navigator.getPooledString(m_xercesNode->getNodeName());
}



const XalanDOMString&
XercesCDATASectionWrapper::getNodeValue() const
{
	return m_navigator.getPooledString(m_xercesNode->getNodeValue());
}



XercesCDATASectionWrapper::NodeType
XercesCDATASectionWrapper::getNodeType() const
{
	return CDATA_SECTION_NODE;
}



XalanNode*
XercesCDATASectionWrapper::getParentNode() const
{
	return m_navigator.getParentNode(m_xercesNode);
}



const XalanNodeList*
XercesCDATASectionWrapper::getChildNodes() const
{
	return 0;
}



XalanNode*
XercesCDATASectionWrapper::getFirstChild() const
{
	return 0;
}



XalanNode*
XercesCDATASectionWrapper::getLastChild() const
{
	return 0;
}



XalanNode*
XercesCDATASectionWrapper::getPreviousSibling() const
{
	return m_navigator.getPreviousSibling(m_xercesNode);
}



XalanNode*
XercesCDATASectionWrapper::getNextSibling() const
{
	return m_navigator.getNextSibling(m_xercesNode);
}



const XalanNamedNodeMap*
XercesCDATASectionWrapper::getAttributes() const
{
	return 0;
}



XalanDocument*
XercesCDATASectionWrapper::getOwnerDocument() const
{
	return m_navigator.getOwnerDocument();
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XercesCDATASectionWrapper*
#endif
XercesCDATASectionWrapper::cloneNode(bool	/* deep */) const
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::NOT_SUPPORTED_ERR);

	return 0;
}



XalanNode*
XercesCDATASectionWrapper::insertBefore(
			XalanNode*	/* newChild */,
			XalanNode*	/* refChild */)
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::HIERARCHY_REQUEST_ERR);

	return 0;
}



XalanNode*
XercesCDATASectionWrapper::replaceChild(
			XalanNode*	/* newChild */,
			XalanNode*	/* oldChild */)
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::HIERARCHY_REQUEST_ERR);

	return 0;
}



XalanNode*
XercesCDATASectionWrapper::removeChild(XalanNode*	/* oldChild */)
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::HIERARCHY_REQUEST_ERR);

	return 0;
}



XalanNode*
XercesCDATASectionWrapper::appendChild(XalanNode*	 /* newChild */)
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::HIERARCHY_REQUEST_ERR);

	return 0;
}



bool
XercesCDATASectionWrapper::hasChildNodes() const
{
	return false;
}



void
XercesCDATASectionWrapper::setNodeValue(const XalanDOMString&	/* nodeValue */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XercesCDATASectionWrapper::normalize()
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
XercesCDATASectionWrapper::isSupported(
			const XalanDOMString&	feature,
			const XalanDOMString&	version) const
{
	return XercesWrapperHelper::isSupported(m_xercesNode, feature, version);
}



const XalanDOMString&
XercesCDATASectionWrapper::getNamespaceURI() const
{
	return m_navigator.getPooledString(m_xercesNode->getNamespaceURI());
}



const XalanDOMString&
XercesCDATASectionWrapper::getPrefix() const
{
	return m_navigator.getPooledString(m_xercesNode->getPrefix());
}



const XalanDOMString&
XercesCDATASectionWrapper::getLocalName() const
{
	return m_navigator.getPooledString(m_xercesNode->getLocalName());
}



void
XercesCDATASectionWrapper::setPrefix(const XalanDOMString&	/* prefix */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
XercesCDATASectionWrapper::isIndexed() const
{
	return m_navigator.getOwnerDocument()->isIndexed();
}



XercesCDATASectionWrapper::IndexType
XercesCDATASectionWrapper::getIndex() const
{
	return m_navigator.getIndex();
}



const XalanDOMString&
XercesCDATASectionWrapper::getData() const
{
	return m_navigator.getPooledString(m_xercesNode->getData());
}



unsigned int
XercesCDATASectionWrapper::getLength() const
{
	return m_xercesNode->getLength();
}



XalanDOMString
XercesCDATASectionWrapper::substringData(
			unsigned int	offset, 
			unsigned int	count) const
{
	return XercesWrapperHelper::substringData(m_xercesNode, offset, count);
}



void
XercesCDATASectionWrapper::appendData(const XalanDOMString&		/* arg */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XercesCDATASectionWrapper::insertData(
			unsigned int			/* offset */,
			const  XalanDOMString& 	/* arg */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XercesCDATASectionWrapper::deleteData(
			unsigned int	/* offset */,
			unsigned int	/* count */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XercesCDATASectionWrapper::replaceData(
			unsigned int			/* offset */,
			unsigned int			/* count */,
			const XalanDOMString&	/* arg */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



XalanText*
XercesCDATASectionWrapper::splitText(unsigned int	/* offset */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	return 0;
}



bool
XercesCDATASectionWrapper::isIgnorableWhitespace() const
{
	return isXMLWhitespace(getData());
}



XALAN_CPP_NAMESPACE_END
