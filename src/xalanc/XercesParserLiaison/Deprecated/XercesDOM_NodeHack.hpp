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

#if !defined(XERCESDOM_NODEHACK_HEADER_GUARD_1357924680)
#define XERCESDOM_NODEHACK_HEADER_GUARD_1357924680



#include <xalanc/XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



#if XERCES_VERSION_MAJOR >= 2
#include <xercesc/dom/deprecated/DOM_Node.hpp>
#include <xercesc/dom/deprecated/DOM_Attr.hpp>
#include <xercesc/dom/deprecated/DOM_Element.hpp>
#include <xercesc/dom/deprecated/DOM_Text.hpp>
#else
#include <xercesc/dom/DOM_Node.hpp>
#include <xercesc/dom/DOM_Attr.hpp>
#include <xercesc/dom/DOM_Element.hpp>
#include <xercesc/dom/DOM_Text.hpp>
#endif



#include <xalanc/XercesParserLiaison/Deprecated/XercesBridgeTypes.hpp>



XALAN_CPP_NAMESPACE_BEGIN


/**
 * This class is deprecated.
 *
 * An evil class to hack the Xerces smart-pointer class.  I'm
 * only doing this because I have to...
 * 
 * @deprecated This class is part of the deprecated Xerces DOM bridge.
 */
class XALAN_XERCESPARSERLIAISON_EXPORT XercesDOM_NodeHack : public XERCES_CPP_NAMESPACE_QUALIFIER DOM_Node
{
public:

	typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_Node		ParentType;

	XercesDOM_NodeHack(NodeImplType*	theImpl = 0);

	~XercesDOM_NodeHack();

	NodeImplType*
	getImpl() const
	{
		return fImpl;
	}

	static NodeImplType*
	getImpl(const ParentType&		theNode)
	{
#if defined(XALAN_OLD_STYLE_CASTS)
		return ((const XercesDOM_NodeHack&)theNode).getImpl();
#else
		return static_cast<const XercesDOM_NodeHack&>(theNode).getImpl();
#endif
	}
};



class XALAN_XERCESPARSERLIAISON_EXPORT XercesDOM_AttrHack : public XERCES_CPP_NAMESPACE_QUALIFIER DOM_Attr
{
public:

	typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_Attr		ParentType;

	XercesDOM_AttrHack(AttrImplType*	theImpl = 0);

	XercesDOM_AttrHack(const ParentType&	theSource);

	~XercesDOM_AttrHack();
};



class XALAN_XERCESPARSERLIAISON_EXPORT XercesDOM_ElementHack : public XERCES_CPP_NAMESPACE_QUALIFIER DOM_Element
{
public:

	typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_Element	ParentType;

	XercesDOM_ElementHack(ElementImplType*	theImpl = 0);

	XercesDOM_ElementHack(const ParentType&		theSource);

	~XercesDOM_ElementHack();


	const DOMStringType
	getNodeNameImpl() const;

	const DOMStringType
	getNodeValueImpl() const;

	const DOMStringType
	getNamespaceURIImpl() const;

	const DOMStringType
	getPrefixImpl() const;

	const DOMStringType
	getLocalNameImpl() const;

	const DOMStringType
	getTagNameImpl() const;

	const DOMStringType
	getAttributeImpl(const DOMStringType&	name) const;

	const DOMStringType
	getAttributeNSImpl(
			const DOMStringType&	namespaceURI,
			const DOMStringType&	localName) const;

	ElementImplType*
	getImpl() const
	{
#if defined(XALAN_OLD_STYLE_CASTS)
		return (ElementImplType*)fImpl;
#else
		return reinterpret_cast<ElementImplType*>(fImpl);
#endif
	}

	static ElementImplType*
	getImpl(const ParentType&	theNode)
	{
#if defined(XALAN_OLD_STYLE_CASTS)
		return ((const XercesDOM_ElementHack&)theNode).getImpl();
#else
		return static_cast<const XercesDOM_ElementHack&>(theNode).getImpl();
#endif
	}
};



class XALAN_XERCESPARSERLIAISON_EXPORT XercesDOM_TextHack : public XERCES_CPP_NAMESPACE_QUALIFIER DOM_Text
{
public:

	typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_Text		ParentType;

	XercesDOM_TextHack(TextImplType*	theImpl = 0);

	XercesDOM_TextHack(const ParentType&	theSource);

	~XercesDOM_TextHack();


	const DOMStringType
	getNodeNameImpl() const;

	const DOMStringType
	getNodeValueImpl() const;

	const DOMStringType
	getNamespaceURIImpl() const;

	const DOMStringType
	getPrefixImpl() const;

	const DOMStringType
	getLocalNameImpl() const;

	const DOMStringType
	getDataImpl() const;

	TextImplType*
	getImpl() const
	{
#if defined(XALAN_OLD_STYLE_CASTS)
		return (TextImplType*)fImpl;
#else
		return reinterpret_cast<TextImplType*>(fImpl);
#endif
	}

	static TextImplType*
	getImpl(const ParentType&	theNode)
	{
#if defined(XALAN_OLD_STYLE_CASTS)
		return ((const XercesDOM_TextHack&)theNode).getImpl();
#else
		return static_cast<const XercesDOM_TextHack&>(theNode).getImpl();
#endif
	}
};



XALAN_CPP_NAMESPACE_END



#endif	// !defined(XERCESDOM_NODEHACK_HEADER_GUARD_1357924680)
