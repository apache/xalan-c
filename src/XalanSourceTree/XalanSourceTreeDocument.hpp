/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2000 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *	  notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *	  notice, this list of conditions and the following disclaimer in
 *	  the documentation and/or other materials provided with the
 *	  distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *	  if any, must include the following acknowledgment:  
 *		 "This product includes software developed by the
 *		  Apache Software Foundation (http://www.apache.org/)."
 *	  Alternately, this acknowledgment may appear in the software itself,
 *	  if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *	  not be used to endorse or promote products derived from this
 *	  software without prior written permission. For written 
 *	  permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *	  nor may "Apache" appear in their name, without prior written
 *	  permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.	IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
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
#if !defined(XALANSOURCETREEDOCUMENT_HEADER_GUARD_1357924680)
#define XALANSOURCETREEDOCUMENT_HEADER_GUARD_1357924680



#include <XalanSourceTree/XalanSourceTreeDefinitions.hpp>



#include <map>



#include <XalanDOM/XalanDocument.hpp>
#include <XalanDOM/XalanNodeListSurrogate.hpp>



#include <PlatformSupport/STLHelper.hpp>
#include <PlatformSupport/XalanDOMStringPool.hpp>



#include <XalanSourceTree/XalanSourceTreeAttributeAllocator.hpp>
#include <XalanSourceTree/XalanSourceTreeAttributeNSAllocator.hpp>
#include <XalanSourceTree/XalanSourceTreeAttributesVector.hpp>
#include <XalanSourceTree/XalanSourceTreeCommentAllocator.hpp>
#include <XalanSourceTree/XalanSourceTreeElementAllocator.hpp>
#include <XalanSourceTree/XalanSourceTreeElementNSAllocator.hpp>
#include <XalanSourceTree/XalanSourceTreeProcessingInstructionAllocator.hpp>
#include <XalanSourceTree/XalanSourceTreeTextAllocator.hpp>
#include <XalanSourceTree/XalanSourceTreeTextIWSAllocator.hpp>



class AttributeList;
class Attributes;



class XALAN_XALANSOURCETREE_EXPORT XalanSourceTreeDocument : public XalanDocument
{
public:

#if defined (XALAN_NO_NAMESPACES)
	typedef map<
				const XalanDOMChar*,
				XalanSourceTreeElement*,
				less_null_terminated_arrays<XalanDOMChar> >		ElementByIDMapType;

	typedef map<
				XalanDOMString,
				XalanDOMString,
				less<XalanDOMString> >							UnparsedEntityURIMapType;
#else
	typedef std::map<
				const XalanDOMChar*,
				XalanSourceTreeElement*,
				less_null_terminated_arrays<XalanDOMChar> >		ElementByIDMapType;

	typedef std::map<
				XalanDOMString,
				XalanDOMString>									UnparsedEntityURIMapType;
#endif

	/**
	 * Perform static initialization.  See class XalanSourceTreeInit.
	 */
	static void
	initialize();

	/**
	 * Perform static shut down.  See class XalanSourceTreeInit.
	 */
	static void
	terminate();


	/**
	 *
	 * Constructor for XalanSourceTreeDocument.
	 *
	 * @param fDynamic If true, the document will allow dynamic building through _some_ of the standard DOM APIs
	 *
	 */
	XalanSourceTreeDocument(bool	fDynamic = false);

	virtual
	~XalanSourceTreeDocument();

	// These interfaces are inherited from XalanNode...

	virtual const XalanDOMString&
	getNodeName() const;

	virtual const XalanDOMString&
	getNodeValue() const;

	virtual NodeType
	getNodeType() const;

	virtual XalanNode*
	getParentNode() const;

	virtual const XalanNodeList*
	getChildNodes() const;

	virtual XalanNode*
	getFirstChild() const;

	virtual XalanNode*
	getLastChild() const;

	virtual XalanNode*
	getPreviousSibling() const;

	virtual XalanNode*
	getNextSibling() const;

	virtual const XalanNamedNodeMap*
	getAttributes() const;

	virtual XalanDocument*
	getOwnerDocument() const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual XalanNode*
#else
	virtual XalanSourceTreeDocument*
#endif
	cloneNode(bool deep) const;

	virtual XalanNode*
	insertBefore(
			XalanNode*	newChild,
			XalanNode*	refChild);

	virtual XalanNode*
	replaceChild(
			XalanNode*	newChild,
			XalanNode*	oldChild);

	virtual XalanNode*
	removeChild(XalanNode*	oldChild);

	virtual XalanNode*
	appendChild(XalanNode*	newChild);

	virtual bool
	hasChildNodes() const;

	virtual void
	setNodeValue(const XalanDOMString&		nodeValue);

	virtual void
	normalize();

	virtual bool
	supports(
			const XalanDOMString&	feature,
			const XalanDOMString&	version) const;

	virtual const XalanDOMString&
	getNamespaceURI() const;

	virtual const XalanDOMString&
	getPrefix() const;

	virtual const XalanDOMString&
	getLocalName() const;

	virtual void
	setPrefix(const XalanDOMString& prefix);

	virtual bool
	isIndexed() const;

	virtual unsigned long
	getIndex() const;

	virtual XalanElement*
	createElement(const XalanDOMString& tagName);

	virtual XalanDocumentFragment*
	createDocumentFragment();

	virtual XalanText*
	createTextNode(const XalanDOMString&	data);

	virtual XalanComment*
	createComment(const XalanDOMString& data);

	virtual XalanCDATASection*
	createCDATASection(const XalanDOMString&	data);

	virtual XalanProcessingInstruction*
	createProcessingInstruction(
			const XalanDOMString&	target,
			const XalanDOMString&	data);

	virtual XalanAttr*
	createAttribute(const XalanDOMString&	name);

	virtual XalanEntityReference*
	createEntityReference(const XalanDOMString& name);

	virtual XalanDocumentType*
	getDoctype() const;

	virtual XalanDOMImplementation*
	getImplementation() const;

	virtual XalanElement*
	getDocumentElement() const;

	virtual XalanNodeList*
	getElementsByTagName(const XalanDOMString&		tagname) const;

	virtual XalanNode*
	importNode(
			XalanNode*	importedNode,
			bool		deep);

	virtual XalanElement*
	createElementNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	qualifiedName);

	virtual XalanAttr*
	createAttributeNS(
			const XalanDOMString& namespaceURI,
			const XalanDOMString& qualifiedName);

	virtual XalanNodeList*
	getElementsByTagNameNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	localName) const;

	virtual XalanElement*
	getElementById(const XalanDOMString&	elementId) const;


	// Interfaces not inherited from XalanDocument...

	XalanSourceTreeElement*
	createElementNode(
			const XalanDOMChar*			name,
			const AttributeList&		attrs,
			XalanSourceTreeElement*		theParentElement = 0,
			XalanNode*					thePreviousSibling = 0,
			XalanNode*					theNextSibling = 0);

	XalanSourceTreeElement*
	createElementNode(
			const XalanDOMChar*			uri,
			const XalanDOMChar*			localname,
			const XalanDOMChar*			qname,
			const Attributes&			attrs,
			XalanSourceTreeElement*		theParentElement = 0,
			XalanNode*					thePreviousSibling = 0,
			XalanNode*					theNextSibling = 0);

	XalanSourceTreeElement*
	createElementNode(
			const XalanDOMChar*			name,
			const Attributes&			attrs,
			XalanSourceTreeElement*		theParentElement = 0,
			XalanNode*					thePreviousSibling = 0,
			XalanNode*					theNextSibling = 0);

	XalanSourceTreeComment*
	createCommentNode(
			const XalanDOMChar*			data,
			unsigned int				length,
			XalanSourceTreeElement*		theParentElement = 0,
			XalanNode*					thePreviousSibling = 0,
			XalanNode*					theNextSibling = 0);

	XalanSourceTreeProcessingInstruction*
	createProcessingInstructionNode(
			const XalanDOMChar*			target,
			const XalanDOMChar*			data,
			XalanSourceTreeElement*		theParentElement = 0,
			XalanNode*					thePreviousSibling = 0,
			XalanNode*					theNextSibling = 0);

	XalanSourceTreeText*
	createTextNode(
			const XalanDOMChar*			chars,
			unsigned int				length,
			XalanSourceTreeElement*		theParentElement = 0,
			XalanNode*					thePreviousSibling = 0,
			XalanNode*					theNextSibling = 0);

	XalanSourceTreeText*
	createTextIWSNode(
			const XalanDOMChar*			chars,
			unsigned int				length,
			XalanSourceTreeElement*		theParentElement = 0,
			XalanNode*					thePreviousSibling = 0,
			XalanNode*					theNextSibling = 0);

	void
	unparsedEntityDeclaration(
			const XMLCh*	name,
			const XMLCh*	publicId,
			const XMLCh*	systemId,
			const XMLCh*	notationName);

	const XalanDOMString&
	getUnparsedEntityURI(const XalanDOMString&	theName) const;

	// Child node setters...
	void
	appendChildNode(XalanSourceTreeComment*		theChild);

	void
	appendChildNode(XalanSourceTreeElement*		theChild);

	void
	appendChildNode(XalanSourceTreeProcessingInstruction*	theChild);

private:

	// Helper functions...
	XalanSourceTreeAttr*
	createAttribute(
			const XalanDOMChar*			theName,
			const XalanDOMChar*			theValue,
			XalanSourceTreeElement*		theOwnerElement);

	XalanSourceTreeElement*
	createElement(
			const XalanDOMChar*			theTagName,
			XalanSourceTreeAttr**		theAttributeVector,
			unsigned int				theAttributeCount,
			XalanSourceTreeElement*		theParentElement,
			XalanNode*					thePreviousSibling,
			XalanNode*					theNextSibling);

	void
	createAttributes(
			const Attributes&			theAttributes,
			XalanSourceTreeAttr**		theAttributeVector,
			unsigned int				theAttributeCount,
			XalanSourceTreeElement*		theOwnerElement);

	// Not implemented...
	XalanSourceTreeDocument(const XalanSourceTreeDocument&	theSource);

	XalanSourceTreeDocument&
	operator=(const XalanSourceTreeDocument&	theRHS);

	bool
	operator==(const XalanSourceTreeDocument&	theRHS) const;


	// Data members...
	XalanNode*										m_firstChild;

	XalanSourceTreeElement*							m_documentElement;

	XalanNodeListSurrogate							m_children;

	XalanSourceTreeAttributeAllocator				m_attributeAllocator;

	XalanSourceTreeAttributeNSAllocator				m_attributeNSAllocator;

	XalanSourceTreeCommentAllocator					m_commentAllocator;

	XalanSourceTreeElementAllocator					m_elementAllocator;

	XalanSourceTreeElementNSAllocator				m_elementNSAllocator;

	XalanSourceTreeProcessingInstructionAllocator	m_piAllocator;

	XalanSourceTreeTextAllocator					m_textAllocator;

	XalanSourceTreeTextIWSAllocator					m_textIWSAllocator;

	XalanDOMStringPool								m_stringPool;

	XalanSourceTreeAttributesVector					m_attributesVector;

	unsigned int									m_nextIndexValue;

	const bool										m_dynamic;

	ElementByIDMapType								m_elementsByID;

	UnparsedEntityURIMapType						m_unparsedEntityURIs;

	static const XalanDOMString&					s_nameString;
};



#endif	// !defined(XALANSOURCETREEDOCUMENT_HEADER_GUARD_1357924680)
