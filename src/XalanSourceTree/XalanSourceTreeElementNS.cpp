/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights 
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
#include "XalanSourceTreeElementNS.hpp"



#include <XalanDOM/XalanDOMException.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XalanSourceTreeElementNS::XalanSourceTreeElementNS(
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
			unsigned int				theIndex) :
	XalanSourceTreeElement(
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



XalanSourceTreeElementNS::~XalanSourceTreeElementNS()
{
}



XalanSourceTreeElementNS::XalanSourceTreeElementNS(
			const XalanSourceTreeElementNS&		theSource,
			bool								deep) :
	XalanSourceTreeElement(theSource, deep),
	m_localName(theSource.m_localName),
	m_prefix(theSource.m_prefix),
	m_namespaceURI(theSource.m_namespaceURI)
{
}



XalanSourceTreeElementNS::XalanSourceTreeElementNS(
			const XalanSourceTreeElement&	theSource,
			bool							deep) :
	XalanSourceTreeElement(theSource, deep),
	m_localName(theSource.getLocalName()),
	m_prefix(theSource.getPrefix()),
	m_namespaceURI(theSource.getNamespaceURI())
{
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XalanSourceTreeElementNS*
#endif
XalanSourceTreeElementNS::cloneNode(bool	deep) const
{
	return new XalanSourceTreeElementNS(*this, deep);
}



const XalanDOMString&
XalanSourceTreeElementNS::getNamespaceURI() const
{
	return m_namespaceURI;
}



const XalanDOMString&
XalanSourceTreeElementNS::getPrefix() const
{
	return m_prefix;
}



const XalanDOMString&
XalanSourceTreeElementNS::getLocalName() const
{
	return m_localName;
}



XALAN_CPP_NAMESPACE_END
