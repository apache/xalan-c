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
#include "XalanSourceTreeElementANS.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XalanSourceTreeElementANS::XalanSourceTreeElementANS(
			const XalanDOMString&		theTagName,
			const XalanDOMString&		theLocalName,
			const XalanDOMString&		theNamespaceURI,
			const XalanDOMString&		thePrefix,
			XalanSourceTreeDocument*	theOwnerDocument,
			XalanSourceTreeAttr**		theAttributes,
			AttributesCountType			theAttributeCount,
			XalanNode*					theParentNode,
			XalanNode*					thePreviousSibling,
			XalanNode*					theNextSibling,
			IndexType					theIndex) :
	XalanSourceTreeElementA(
		theTagName,
		theOwnerDocument,
		theAttributes,
		theAttributeCount,
		theParentNode,
		thePreviousSibling,
		theNextSibling,
		theIndex),
	m_localName(theLocalName),
	m_prefix(thePrefix),
	m_namespaceURI(theNamespaceURI)
{
}



XalanSourceTreeElementANS::~XalanSourceTreeElementANS()
{
}



XalanSourceTreeElementANS::XalanSourceTreeElementANS(
			const XalanSourceTreeElementANS&	theSource,
			bool								deep) :
	XalanSourceTreeElementA(theSource, deep),
	m_localName(theSource.m_localName),
	m_prefix(theSource.m_prefix),
	m_namespaceURI(theSource.m_namespaceURI)
{
}



XalanSourceTreeElementANS::XalanSourceTreeElementANS(
			const XalanSourceTreeElementA&	theSource,
			bool							deep) :
	XalanSourceTreeElementA(theSource, deep),
	m_localName(theSource.getLocalName()),
	m_prefix(theSource.getPrefix()),
	m_namespaceURI(theSource.getNamespaceURI())
{
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XalanSourceTreeElementANS*
#endif
XalanSourceTreeElementANS::cloneNode(bool	/* deep */) const
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	// Dummy return value...
	return 0;
}



const XalanDOMString&
XalanSourceTreeElementANS::getNamespaceURI() const
{
	return m_namespaceURI;
}



const XalanDOMString&
XalanSourceTreeElementANS::getPrefix() const
{
	return m_prefix;
}



const XalanDOMString&
XalanSourceTreeElementANS::getLocalName() const
{
	return m_localName;
}



XALAN_CPP_NAMESPACE_END
