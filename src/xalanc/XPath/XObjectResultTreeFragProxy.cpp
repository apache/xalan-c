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
#include "XObjectResultTreeFragProxy.hpp"



#include <cassert>



#include <xalanc/XalanDOM/XalanDOMException.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XObjectResultTreeFragProxy::XObjectResultTreeFragProxy(const XObject&	theValue) :
	XObjectResultTreeFragProxyBase(),
	m_proxy(theValue)
{
}



XObjectResultTreeFragProxy::~XObjectResultTreeFragProxy()
{
}



#define XALAN_NO_MUTABLE
XalanNode*
XObjectResultTreeFragProxy::getFirstChild() const
{
#if defined(XALAN_NO_MUTABLE)
	return (XalanNode*)&m_proxy;
#else
	return &m_proxy;
#endif
}



XalanNode*
XObjectResultTreeFragProxy::getLastChild() const
{
#if defined(XALAN_NO_MUTABLE)
	return (XalanNode*)&m_proxy;
#else
	return &m_proxy;
#endif
}
#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XObjectResultTreeFragProxy*
#endif
XObjectResultTreeFragProxy::cloneNode(bool	/* deep */) const
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	return 0;
}



bool
XObjectResultTreeFragProxy::hasChildNodes() const
{
	return true;
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanDocumentFragment*
#else
XObjectResultTreeFragProxy*
#endif
XObjectResultTreeFragProxy::clone(bool	/* deep */) const
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	return 0;
}



XALAN_CPP_NAMESPACE_END
