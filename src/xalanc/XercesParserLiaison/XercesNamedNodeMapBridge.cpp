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
#include "XercesNamedNodeMapBridge.hpp"



#include <algorithm>
#include <cassert>



#if XERCES_VERSION_MAJOR >= 2
#include <xercesc/dom/deprecated/DOM_Node.hpp>
#else
#include <xercesc/dom/DOM_Node.hpp>
#endif



#include "XercesDocumentBridge.hpp"
#include "XercesDOMException.hpp"
#include "XercesElementBridge.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XercesNamedNodeMapBridge::XercesNamedNodeMapBridge(
			const DOM_NamedNodeMapType&		theXercesNamedNodeMap,
			const XercesBridgeNavigator&	theNavigator) :
	m_xercesNamedNodeMap(theXercesNamedNodeMap),
	m_navigator(theNavigator)
{
	assert(m_xercesNamedNodeMap != (const XERCES_CPP_NAMESPACE_QUALIFIER DOM_NullPtr*)0);
}



XercesNamedNodeMapBridge::~XercesNamedNodeMapBridge()
{
}



XalanNode*
XercesNamedNodeMapBridge::setNamedItem(XalanNode* 	arg)
{
	assert(arg != 0);

	XalanNode*	theXalanNode = 0;

	// Find the DOM_Node that corresponds to the bridge node...
	const DOM_NodeType	theNode = m_navigator.mapNode(arg);

	if (theNode.isNull() == true)
	{
		// Doesn't belong to this doc, so throw...
		throw XercesDOMException(XercesDOMException::WRONG_DOCUMENT_ERR);
	}

	try
	{
		const DOM_NodeType	theOldXercesNode =
			m_xercesNamedNodeMap.setNamedItem(theNode);

		if (theNode.isNull() == false)
		{
			// A node was replaced, so map the replacement...
			theXalanNode = m_navigator.mapNode(theOldXercesNode);
			assert(theXalanNode != 0);
		}
	}
	catch(const DOM_DOMExceptionType&	theException)
	{
		throw XercesDOMException(theException);
	}

	return theXalanNode;
}



XalanNode*
XercesNamedNodeMapBridge::item(unsigned int	index) const
{
	return m_navigator.mapNode(m_xercesNamedNodeMap.item(index));
}



XalanNode*
XercesNamedNodeMapBridge::getNamedItem(const XalanDOMString& 	name) const
{
	return m_navigator.mapNode(m_xercesNamedNodeMap.getNamedItem(c_wstr(name)));
}



unsigned int
XercesNamedNodeMapBridge::getLength() const
{
	return m_xercesNamedNodeMap.getLength();
}



XalanNode*
XercesNamedNodeMapBridge::removeNamedItem(const XalanDOMString&		name)
{
	XalanNode*	theXalanNode = 0;

	try
	{
		const DOM_NodeType	theXercesNode =
			m_xercesNamedNodeMap.removeNamedItem(c_wstr(name));

		// A node was removed, so get it corresponding XalanNode...
		theXalanNode = m_navigator.mapNode(theXercesNode);
		assert(theXalanNode != 0);
	}
	catch(const DOM_DOMExceptionType&	theException)
	{
		throw XercesDOMException(theException);
	}

	return theXalanNode;
}



XalanNode*
XercesNamedNodeMapBridge::getNamedItemNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	localName) const
{
#if defined(XALAN_NO_MUTABLE)
	return m_navigator.mapNode(((XercesNamedNodeMapBridge*)this)->m_xercesNamedNodeMap.getNamedItemNS(c_wstr(namespaceURI), c_wstr(localName)));
#else
	return m_navigator.mapNode(m_xercesNamedNodeMap.getNamedItemNS(c_wstr(namespaceURI), c_wstr(localName)));
#endif
}



XalanNode*
XercesNamedNodeMapBridge::setNamedItemNS(XalanNode*		arg)
{
	assert(arg != 0);

	XalanNode*	theXalanNode = 0;

	// Find the DOM_Node that corresponds to the bridge node...
	const DOM_NodeType	theNode = m_navigator.mapNode(arg);

	if (theNode.isNull() == true)
	{
		// Doesn't belong to this doc, so throw...
		throw XercesDOMException(XercesDOMException::WRONG_DOCUMENT_ERR);
	}

	try
	{
		const DOM_NodeType	theOldXercesNode =
			m_xercesNamedNodeMap.setNamedItemNS(theNode);

		if (theNode.isNull() == false)
		{
			// A node was replaced, so map the replacement...
			theXalanNode = m_navigator.mapNode(theOldXercesNode);
			assert(theXalanNode != 0);
		}
	}
	catch(const DOM_DOMExceptionType&	theException)
	{
		throw XercesDOMException(theException);
	}

	return theXalanNode;
}



XalanNode*
XercesNamedNodeMapBridge::removeNamedItemNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	localName)
{
	XalanNode*	theXalanNode = 0;

	try
	{
		const DOM_NodeType	theXercesNode =
			m_xercesNamedNodeMap.removeNamedItemNS(c_wstr(namespaceURI), c_wstr(localName));

		// A node was removed, so get it corresponding XalanNode...
		theXalanNode = m_navigator.mapNode(theXercesNode);
		assert(theXalanNode != 0);
	}
	catch(const DOM_DOMExceptionType&	theException)
	{
		throw XercesDOMException(theException);
	}

	return theXalanNode;
}



XALAN_CPP_NAMESPACE_END
