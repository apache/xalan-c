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
#include "XalanSourceTreeElementA.hpp"



#include <xalanc/XalanDOM/XalanDOMException.hpp>



#include "XalanSourceTreeAttr.hpp"



XALAN_CPP_NAMESPACE_BEGIN



static const XalanDOMString		s_emptyString;



XalanSourceTreeElementA::XalanSourceTreeElementA(
			const XalanDOMString&		theTagName,
			XalanSourceTreeDocument*	theOwnerDocument,
			XalanSourceTreeAttr**		theAttributes,
			AttributesCountType			theAttributeCount,
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
		theIndex),
	m_attributes(theAttributes),
	m_attributeCount(theAttributeCount)
{
}



XalanSourceTreeElementA::~XalanSourceTreeElementA()
{
}



XalanSourceTreeElementA::XalanSourceTreeElementA(
			const XalanSourceTreeElementA&	theSource,
			bool							deep) :
	XalanSourceTreeElement(theSource, deep),
	m_attributes(theSource.m_attributes),
	m_attributeCount(theSource.m_attributeCount)
{
}



const XalanNamedNodeMap*
XalanSourceTreeElementA::getAttributes() const
{
	return this;
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XalanSourceTreeElementA*
#endif
XalanSourceTreeElementA::cloneNode(bool		/* deep */) const
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	// Dummy return value...
	return 0;
}



const XalanDOMString&
XalanSourceTreeElementA::getNamespaceURI() const
{
	return s_emptyString;
}



const XalanDOMString&
XalanSourceTreeElementA::getPrefix() const
{
	return s_emptyString;
}



const XalanDOMString&
XalanSourceTreeElementA::getLocalName() const
{
	return m_tagName;
}



const XalanDOMString&
XalanSourceTreeElementA::getAttribute(const XalanDOMString&		name) const
{
	XalanAttr* const	theAttr = getAttributeNode(name);

	return theAttr == 0 ? s_emptyString : theAttr->getValue();
}



XalanAttr*
XalanSourceTreeElementA::getAttributeNode(const XalanDOMString&		name) const
{
	for(unsigned int i = 0; i < m_attributeCount; ++i)
	{
		assert(m_attributes[i] != 0);

		if (m_attributes[i]->getNodeName() == name)
		{
			return m_attributes[i];
		}
	}

	return 0;
}



const XalanDOMString&
XalanSourceTreeElementA::getAttributeNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	localName) const
{
	XalanAttr* const	theAttr = getAttributeNodeNS(namespaceURI, localName);

	return theAttr == 0 ? s_emptyString : theAttr->getValue();
}



XalanAttr*
XalanSourceTreeElementA::getAttributeNodeNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	localName) const
{
	for(unsigned int i = 0; i < m_attributeCount; ++i)
	{
		assert(m_attributes[i] != 0);

		if (m_attributes[i]->getLocalName() == localName &&
			m_attributes[i]->getNamespaceURI() ==  namespaceURI)
		{
			return m_attributes[i];
		}
	}

	return 0;
}



XalanNode*
XalanSourceTreeElementA::setNamedItem(XalanNode* 	/* arg */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeElementA::item(unsigned int	index) const
{
	return index < m_attributeCount ? m_attributes[index] : 0;
}



XalanNode*
XalanSourceTreeElementA::getNamedItem(const XalanDOMString& 	name) const
{
	return getAttributeNode(name);
}



unsigned int
XalanSourceTreeElementA::getLength() const
{
	return m_attributeCount;
}



XalanNode*
XalanSourceTreeElementA::removeNamedItem(const XalanDOMString&	/* name */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeElementA::getNamedItemNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	localName) const
{
	return getAttributeNodeNS(namespaceURI, localName);
}



XalanNode*
XalanSourceTreeElementA::setNamedItemNS(XalanNode*	/* arg */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeElementA::removeNamedItemNS(
			const XalanDOMString&	/* namespaceURI */,
			const XalanDOMString&	/* localName */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XALAN_CPP_NAMESPACE_END
