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
// Class header file.
#include "XNodeSetNodeProxy.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XNodeSetNodeProxy::Proxy::Proxy(XalanNode*	theNode) :
	m_node(theNode)
{
}



XNodeSetNodeProxy::Proxy::~Proxy()
{
}



XalanNode*
XNodeSetNodeProxy::Proxy::item(size_type	index) const
{
	if (m_node == 0 || index > 0)
	{
		return 0;
	}
	else
	{
		return m_node;
	}
}



XNodeSetNodeProxy::Proxy::size_type
XNodeSetNodeProxy::Proxy::getLength() const
{
	return m_node == 0 ? 0 : 1;
}



XNodeSetNodeProxy::Proxy::size_type
XNodeSetNodeProxy::Proxy::indexOf(const XalanNode*	theNode) const
{
	if (m_node == 0 || theNode != m_node)
	{
		return npos;
	}
	else
	{
		return 0;
	}
}



XNodeSetNodeProxy::XNodeSetNodeProxy(XalanNode*		theNode) :
	XNodeSetBase(),
	m_proxy(theNode)
{
}



XNodeSetNodeProxy::XNodeSetNodeProxy(const XNodeSetNodeProxy&	source) :
	XNodeSetBase(source),
	m_proxy(source.m_proxy)
{
}



XNodeSetNodeProxy::~XNodeSetNodeProxy()
{
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XObject*
#else
XNodeSetNodeProxy*
#endif
XNodeSetNodeProxy::clone(void*	theAddress) const
{
	if (theAddress == 0)
	{
		return new XNodeSetNodeProxy(*this);
	}
	else
	{
		return new (theAddress) XNodeSetNodeProxy(*this);
	}
}



XNodeSetNodeProxy::eObjectType
XNodeSetNodeProxy::getRealType() const
{
	return eTypeNodeSetNodeProxy;
}



const NodeRefListBase&
XNodeSetNodeProxy::nodeset() const
{
	return m_proxy;
}



XalanNode*
XNodeSetNodeProxy::item(size_type	index) const
{
	return m_proxy.item(index);
}



XNodeSetNodeProxy::size_type
XNodeSetNodeProxy::getLength() const
{
	return m_proxy.getLength();
}



XALAN_CPP_NAMESPACE_END
