/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2001 The Apache Software Foundation.  All rights 
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
// Class header file.
#include "XObjectResultTreeFragProxyBase.hpp"



#include <XalanDOM/XalanDOMException.hpp>



#include "NodeRefListBase.hpp"
#include "XNodeSetBase.hpp"



const XalanDOMString	XObjectResultTreeFragProxyBase::s_emptyString;



XObjectResultTreeFragProxyBase::XObjectResultTreeFragProxyBase() :
	ResultTreeFragBase(),
	XalanNodeList()
{
}



XObjectResultTreeFragProxyBase::XObjectResultTreeFragProxyBase(const XObjectResultTreeFragProxyBase&	/* theRHS */)
{
}



XObjectResultTreeFragProxyBase::~XObjectResultTreeFragProxyBase()
{
}



void
XObjectResultTreeFragProxyBase::clear()
{
	assert(false);
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
	return this;
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
XObjectResultTreeFragProxyBase::supports(
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



unsigned long
XObjectResultTreeFragProxyBase::getIndex() const
{
	return 0;
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
ResultTreeFragBase*
#else
XObjectResultTreeFragProxyBase*
#endif
XObjectResultTreeFragProxyBase::clone(bool	/* deep */) const
{
	assert(false);

	return 0;
}
