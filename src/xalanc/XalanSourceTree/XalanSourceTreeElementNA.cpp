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
#include "XalanSourceTreeElementNA.hpp"



#include <xalanc/XalanDOM/XalanDOMException.hpp>



XALAN_CPP_NAMESPACE_BEGIN



const XalanEmptyNamedNodeMap	XalanSourceTreeElementNA::s_emptyAttributes;



XalanSourceTreeElementNA::XalanSourceTreeElementNA(
			const XalanDOMString&		theTagName,
			XalanSourceTreeDocument*	theOwnerDocument,
			XalanNode*					theParentNode,
			XalanNode*					thePreviousSibling,
			XalanNode*					theNextSibling,
			IndexType					theIndex) :
	XalanSourceTreeElement(
		theTagName,
		theOwnerDocument,
		theParentNode,
		thePreviousSibling,
		theNextSibling,
		theIndex)
{
}



XalanSourceTreeElementNA::~XalanSourceTreeElementNA()
{
}



XalanSourceTreeElementNA::XalanSourceTreeElementNA(
			const XalanSourceTreeElementNA&		theSource,
			bool								deep) :
	XalanSourceTreeElement(theSource, deep)
{
}



const XalanNamedNodeMap*
XalanSourceTreeElementNA::getAttributes() const
{
	return &s_emptyAttributes;
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XalanSourceTreeElementNA*
#endif
XalanSourceTreeElementNA::cloneNode(bool	/* deep */) const
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	// Dummy return value...
	return 0;
}



const XalanDOMString&
XalanSourceTreeElementNA::getNamespaceURI() const
{
	return s_emptyString;
}



const XalanDOMString&
XalanSourceTreeElementNA::getPrefix() const
{
	return s_emptyString;
}



const XalanDOMString&
XalanSourceTreeElementNA::getLocalName() const
{
	return m_tagName;
}



const XalanDOMString&
XalanSourceTreeElementNA::getAttribute(const XalanDOMString&	/* name */) const
{
	return s_emptyString;
}



XalanAttr*
XalanSourceTreeElementNA::getAttributeNode(const XalanDOMString&	/* name */) const
{
	return 0;
}



const XalanDOMString&
XalanSourceTreeElementNA::getAttributeNS(
			const XalanDOMString&	/* namespaceURI */,
			const XalanDOMString&	/* localName */) const
{
	return s_emptyString;
}



XalanAttr*
XalanSourceTreeElementNA::getAttributeNodeNS(
			const XalanDOMString&	/* namespaceURI */,
			const XalanDOMString&	/* localName */) const
{
	return 0;
}



XALAN_CPP_NAMESPACE_END
