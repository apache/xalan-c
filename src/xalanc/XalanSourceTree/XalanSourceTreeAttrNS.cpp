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
#include "XalanSourceTreeAttrNS.hpp"



#include <xalanc/XalanDOM/XalanDOMException.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XalanSourceTreeAttrNS::XalanSourceTreeAttrNS(
			const XalanDOMString&		theName,
			const XalanDOMString&		theLocalName,
			const XalanDOMString&		theNamespaceURI,
			const XalanDOMString&		thePrefix,
			const XalanDOMString&		theValue,
			XalanSourceTreeElement*		theOwnerElement,
			IndexType					theIndex) :
	XalanSourceTreeAttr(
		theName,
		theValue,
		theOwnerElement,
		theIndex),
	m_localName(theLocalName),
	m_prefix(thePrefix),
	m_namespaceURI(theNamespaceURI)
{
}



XalanSourceTreeAttrNS::~XalanSourceTreeAttrNS()
{
}



XalanSourceTreeAttrNS::XalanSourceTreeAttrNS(
			const XalanSourceTreeAttrNS&	theSource,
			bool							deep) :
	XalanSourceTreeAttr(theSource, deep),
	m_localName(theSource.m_localName),
	m_prefix(theSource.m_prefix),
	m_namespaceURI(theSource.m_namespaceURI)
{
}



XalanSourceTreeAttrNS::XalanSourceTreeAttrNS(
			const XalanSourceTreeAttr&	theSource,
			bool						deep) :
	XalanSourceTreeAttr(theSource, deep),
	m_localName(theSource.getLocalName()),
	m_prefix(theSource.getPrefix()),
	m_namespaceURI(theSource.getNamespaceURI())
{
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XalanSourceTreeAttrNS*
#endif
XalanSourceTreeAttrNS::cloneNode(bool	/* deep */) const
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	// Dummy return value...
	return 0;
}



const XalanDOMString&
XalanSourceTreeAttrNS::getNamespaceURI() const
{
	return m_namespaceURI;
}



const XalanDOMString&
XalanSourceTreeAttrNS::getPrefix() const
{
	return m_prefix;
}



const XalanDOMString&
XalanSourceTreeAttrNS::getLocalName() const
{
	return m_localName;
}



XALAN_CPP_NAMESPACE_END
