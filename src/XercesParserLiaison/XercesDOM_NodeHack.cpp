/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
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



#include "XercesDOM_NodeHack.hpp"



#if defined(XALAN_USE_XERCES_INTERNAL_CLASSES)
#include <dom/ElementImpl.hpp>
#include <dom/TextImpl.hpp>
#endif



XercesDOM_NodeHack::XercesDOM_NodeHack(NodeImpl*	theImpl) :
	DOM_Node(theImpl)
{
}



XercesDOM_NodeHack::~XercesDOM_NodeHack()
{
}



XercesDOM_AttrHack::XercesDOM_AttrHack(AttrImpl*	theImpl) :
	DOM_Attr(theImpl)
{
}



XercesDOM_AttrHack::XercesDOM_AttrHack(const DOM_Attr&	theSource) :
	DOM_Attr(theSource)
{
}



XercesDOM_AttrHack::~XercesDOM_AttrHack()
{
}



XercesDOM_ElementHack::XercesDOM_ElementHack(ElementImpl*	theImpl) :
	DOM_Element(theImpl)
{
}



XercesDOM_ElementHack::XercesDOM_ElementHack(const DOM_Element&		theSource) :
	DOM_Element(theSource)
{
}



XercesDOM_ElementHack::~XercesDOM_ElementHack()
{
}



const DOMString
XercesDOM_ElementHack::getNodeNameImpl() const
{
#if defined(XALAN_USE_XERCES_INTERNAL_CLASSES)
	return getImpl()->getNodeName();
#else
	return getNodeName();
#endif
}



const DOMString
XercesDOM_ElementHack::getNodeValueImpl() const
{
#if defined(XALAN_USE_XERCES_INTERNAL_CLASSES)
	return getImpl()->getNodeValue();
#else
	return getNodeValue();
#endif
}



const DOMString
XercesDOM_ElementHack::	getNamespaceURIImpl() const
{
#if defined(XALAN_USE_XERCES_INTERNAL_CLASSES)
	return getImpl()->getNamespaceURI();
#else
	return getNamespaceURI();
#endif
}



const DOMString
XercesDOM_ElementHack::getPrefixImpl() const
{
#if defined(XALAN_USE_XERCES_INTERNAL_CLASSES)
	return getImpl()->getPrefix();
#else
	return getPrefix();
#endif
}



const DOMString
XercesDOM_ElementHack::getLocalNameImpl() const
{
#if defined(XALAN_USE_XERCES_INTERNAL_CLASSES)
	return getImpl()->getLocalName();
#else
	return getLocalName();
#endif
}



const DOMString
XercesDOM_ElementHack::getTagNameImpl() const
{
#if defined(XALAN_USE_XERCES_INTERNAL_CLASSES)
	return getImpl()->getTagName();
#else
	return getTagName();
#endif
}



const DOMString
XercesDOM_ElementHack::getAttributeImpl(const DOMString&	name) const
{
#if defined(XALAN_USE_XERCES_INTERNAL_CLASSES)
	return getImpl()->getAttribute(name);
#else
	return getAttribute(name);
#endif
}



const DOMString
XercesDOM_ElementHack::getAttributeNSImpl(
			const DOMString&	namespaceURI,
			const DOMString&	localName) const
{
#if defined(XALAN_USE_XERCES_INTERNAL_CLASSES)
	return getImpl()->getAttributeNS(namespaceURI, localName);
#else
	return getAttributeNS(namespaceURI, localName);
#endif
}



XercesDOM_TextHack::XercesDOM_TextHack(TextImpl*	theImpl) :
	DOM_Text(theImpl)
{
}



XercesDOM_TextHack::XercesDOM_TextHack(const DOM_Text&	theSource) :
	DOM_Text(theSource)
{
}



XercesDOM_TextHack::~XercesDOM_TextHack()
{
}



const DOMString
XercesDOM_TextHack::getNodeNameImpl() const
{
#if defined(XALAN_USE_XERCES_INTERNAL_CLASSES)
	return getImpl()->getNodeName();
#else
	return getNodeName();
#endif
}



const DOMString
XercesDOM_TextHack::getNodeValueImpl() const
{
#if defined(XALAN_USE_XERCES_INTERNAL_CLASSES)
	return getImpl()->getNodeValue();
#else
	return getNodeValue();
#endif
}



const DOMString
XercesDOM_TextHack::getNamespaceURIImpl() const
{
#if defined(XALAN_USE_XERCES_INTERNAL_CLASSES)
	return getImpl()->getNamespaceURI();
#else
	return getNamespaceURI();
#endif
}



const DOMString
XercesDOM_TextHack::getPrefixImpl() const
{
#if defined(XALAN_USE_XERCES_INTERNAL_CLASSES)
	return getImpl()->getPrefix();
#else
	return getPrefix();
#endif
}



const DOMString
XercesDOM_TextHack::getLocalNameImpl() const
{
#if defined(XALAN_USE_XERCES_INTERNAL_CLASSES)
	return getImpl()->getLocalName();
#else
	return getLocalName();
#endif
}



const DOMString
XercesDOM_TextHack::getDataImpl() const
{
#if defined(XALAN_USE_XERCES_INTERNAL_CLASSES)
	return getImpl()->getData();
#else
	return getData();
#endif
}
