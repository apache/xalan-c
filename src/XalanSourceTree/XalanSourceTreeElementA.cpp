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
#include "XalanSourceTreeElementA.hpp"



#include <XalanDOM/XalanDOMException.hpp>



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
			unsigned int				theIndex) :
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
XalanSourceTreeElementA::cloneNode(bool	deep) const
{
	return clone(deep);
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
