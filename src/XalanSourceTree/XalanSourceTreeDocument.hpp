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



#include <deque>
#include <map>



#include <XalanDOM/XalanDocument.hpp>



#include <Include/STLHelper.hpp>



#include <PlatformSupport/XalanArrayAllocator.hpp>
#include <PlatformSupport/XalanDOMStringPool.hpp>



#include <XalanSourceTree/XalanSourceTreeAttributeAllocator.hpp>
#include <XalanSourceTree/XalanSourceTreeAttributeNSAllocator.hpp>
#include <XalanSourceTree/XalanSourceTreeCommentAllocator.hpp>
#include <XalanSourceTree/XalanSourceTreeElementAllocator.hpp>
#include <XalanSourceTree/XalanSourceTreeElementNSAllocator.hpp>
#include <XalanSourceTree/XalanSourceTreeProcessingInstructionAllocator.hpp>
#include <XalanSourceTree/XalanSourceTreeTextAllocator.hpp>
#include <XalanSourceTree/XalanSourceTreeTextIWSAllocator.hpp>



class AttributeList;
class Attributes;
class PrefixResolver;
class XalanSourceTreeAttr;



class XALAN_XALANSOURCETREE_EXPORT XalanSourceTreeDocument : public XalanDocument
{
public:

	typedef XalanSourceTreeElement::AttributesCountType		AttributesCountType;
	typedef XalanArrayAllocator<XalanSourceTreeAttr*>		AttributesArrayAllocatorType;

#if defined (XALAN_NO_NAMESPACES)
	typedef map<
				const XalanDOMChar*,
				XalanSourceTreeElement*,
				less_null_terminated_arrays<XalanDOMChar> >		ElementByIDMapType;

	typedef map<
				XalanDOMString,
				XalanDOMString,
				less<XalanDOMString> >							UnparsedEntityURIMapType;

	typedef deque<XalanDOMString>								StringCollectionType;
#else
	typedef std::map<
				const XalanDOMChar*,
				XalanSourceTreeElement*,
				less_null_terminated_arrays<XalanDOMChar> >		ElementByIDMapType;

	typedef std::map<
				XalanDOMString,
				XalanDOMString>									UnparsedEntityURIMapType;

	typedef std::deque<XalanDOMString>							StringCollectionType;
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


	enum { eDefaultAttributeAllocatorBlockSize = 100,
		   eDefaultAttributeNSAllocatorBlockSize = 50,
		   eDefaultCommentAllocatorBlockSize = 10,
		   eDefaultElementAllocatorBlockSize = 100,
		   eDefaultElementNSAllocatorBlockSize = 100,
		   eDefaultPIAllocatorBlockSize = 10,
		   eDefaultTextAllocatorBlockSize = 100,
		   eDefaultTextIWSAllocatorBlockSize = 100,
		   eDefaultNamesStringPoolBlockSize = XalanDOMStringPool::eDefaultBlockSize,
		   eDefaultNamesStringPoolBucketCount = XalanDOMStringPool::eDefaultBucketCount,
		   eDefaultNamesStringPoolBucketSize = XalanDOMStringPool::eDefaultBucketSize,
		   eDefaultValuesStringPoolBlockSize = XalanDOMStringPool::eDefaultBlockSize,
		   eDefaultValuesStringPoolBucketCount = 997,
		   eDefaultValuesStringPoolBucketSize = XalanDOMStringPool::eDefaultBucketSize };


	typedef XalanSourceTreeAttributeAllocator::size_type	allocator_size_type;
	typedef XalanDOMStringPool::block_size_type				block_size_type;
	typedef XalanDOMStringPool::bucket_count_type			bucket_count_type;
	typedef XalanDOMStringPool::bucket_size_type			bucket_size_type;

	/**
	 *
	 * Constructor for XalanSourceTreeDocument.
	 *
	 * @param fPoolAllText If false, text node data that is not whitespace will not be pooled.
	 * @param theNamesStringPoolBlockSize The block size for allocating strings in the name pool
	 * @param theNamesStringPoolBucketCount The number of buckets for allocating strings in the name pool
	 * @param theNamesStringPoolBucketSize The bucket size for allocating strings in the name pool
	 * @param theValuesStringPoolBlockSize The block size for allocating strings in the values pool
	 * @param theValuesStringPoolBucketCount The number of buckets for allocating strings in the values pool
	 * @param theValuesStringPoolBucketSize The bucket size for allocating strings in the values pool
	 *
	 */
	XalanSourceTreeDocument(
			bool				fPoolAllText = true,
			block_size_type		theNamesStringPoolBlockSize = eDefaultNamesStringPoolBlockSize,
			bucket_count_type	theNamesStringPoolBucketCount = eDefaultNamesStringPoolBucketCount,
			bucket_size_type	theNamesStringPoolBucketSize = eDefaultNamesStringPoolBucketSize,
			block_size_type		theValuesStringPoolBlockSize = eDefaultValuesStringPoolBlockSize,
			bucket_count_type	theValuesStringPoolBucketCount = eDefaultValuesStringPoolBucketCount,
			bucket_size_type	theValuesStringPoolBucketSize = eDefaultValuesStringPoolBucketSize);

	/**
	 *
	 * Constructor for XalanSourceTreeDocument.
	 *
	 * @param theAttributeBlockSize The block size for allocating attribute nodes
	 * @param theAttributeNSBlockSize The block size for allocating attribute NS nodes
	 * @param theCommentBlockSize The block size for allocating comment nodes
	 * @param theElementBlockSize The block size for allocating element nodes
	 * @param theElementNSBlockSize The block size for allocating element nodes
	 * @param theTextBlockSize The block size for allocating text nodes,
	 * @param theTextIWSBlockSize The block size for allocating text IWS nodes,
	 * @param fPoolAllText If false, text node data that is not whitespace will not be pooled.
	 *
	 */
	XalanSourceTreeDocument(
			allocator_size_type		theAttributeBlockSize,
			allocator_size_type		theAttributeNSBlockSize,
			allocator_size_type		theCommentBlockSize,
			allocator_size_type		theElementBlockSize,
			allocator_size_type		theElementNSBlockSize,
			allocator_size_type		thePIBlockSize,
			allocator_size_type		theTextBlockSize,
			allocator_size_type		theTextIWSBlockSize,
			bool					fPoolAllText = true);

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
	isSupported(
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
			const XalanDOMChar*		name,
			const AttributeList&	attrs,
			XalanNode*				theParentNode = 0,
			XalanNode*				thePreviousSibling = 0,
			XalanNode*				theNextSibling = 0,
			bool					fAddXMLNamespaceAttribute = false);

	XalanSourceTreeElement*
	createElementNode(
			const XalanDOMChar*			uri,
			const XalanDOMChar*			localname,
			const XalanDOMChar*			qname,
			const Attributes&			attrs,
			XalanNode*					theParentNode = 0,
			XalanNode*					thePreviousSibling = 0,
			XalanNode*					theNextSibling = 0,
			bool						fAddXMLNamespaceAttribute = false);

	XalanSourceTreeElement*
	createElementNode(
			const XalanDOMChar*			tagName,
			const AttributeList&		attrs,
			const PrefixResolver&		thePrefixResolver,
			XalanNode*					theParentNode = 0,
			XalanNode*					thePreviousSibling = 0,
			XalanNode*					theNextSibling = 0,
			bool						fAddXMLNamespaceAttribute = false);

	XalanSourceTreeElement*
	createElementNode(
			const XalanDOMChar*			name,
			const Attributes&			attrs,
			XalanNode*					theParentNode = 0,
			XalanNode*					thePreviousSibling = 0,
			XalanNode*					theNextSibling = 0,
			bool						fAddXMLNamespaceAttribute = false);

	XalanSourceTreeComment*
	createCommentNode(
			const XalanDOMChar*			data,
			XalanDOMString::size_type	length,
			XalanNode*					theParentNode = 0,
			XalanNode*					thePreviousSibling = 0,
			XalanNode*					theNextSibling = 0);

	XalanSourceTreeProcessingInstruction*
	createProcessingInstructionNode(
			const XalanDOMChar*		target,
			const XalanDOMChar*		data,
			XalanNode*				theParentNode = 0,
			XalanNode*				thePreviousSibling = 0,
			XalanNode*				theNextSibling = 0);

	XalanSourceTreeText*
	createTextNode(
			const XalanDOMChar*			chars,
			XalanDOMString::size_type	length,
			XalanNode*					theParentNode = 0,
			XalanNode*					thePreviousSibling = 0,
			XalanNode*					theNextSibling = 0);

	XalanSourceTreeText*
	createTextIWSNode(
			const XalanDOMChar*			chars,
			XalanDOMString::size_type	length,
			XalanNode*					theParentNode = 0,
			XalanNode*					thePreviousSibling = 0,
			XalanNode*					theNextSibling = 0);

	void
	unparsedEntityDeclaration(
			const XalanDOMChar*		name,
			const XalanDOMChar*		publicId,
			const XalanDOMChar*		systemId,
			const XalanDOMChar*		notationName);

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
			XalanSourceTreeElement*		theOwnerElement,
			const PrefixResolver&		thePrefixResolver);

	XalanSourceTreeAttr*
	createAttribute(
			const XalanDOMChar*			theName,
			const XalanDOMChar*			theValue,
			XalanSourceTreeElement*		theOwnerElement);

	XalanSourceTreeElement*
	createElementNode(
			const XalanDOMChar*		theTagName,
			XalanSourceTreeAttr**	theAttributeVector,
			AttributesCountType		theAttributeCount,
			XalanNode*				theParentNode,
			XalanNode*				thePreviousSibling,
			XalanNode*				theNextSibling,
			const PrefixResolver&	thePrefixResolver);

	void
	createAttributes(
			const Attributes&			theAttributes,
			XalanSourceTreeAttr**		theAttributeVector,
			XalanSourceTreeElement*		theOwnerElement,
			bool						fAddXMLNamespaceAttribute);

	const XalanDOMString&
	getTextNodeString(
			const XalanDOMChar*			chars,
			XalanDOMString::size_type	length);

	const XalanDOMString*
	getNamespaceForPrefix(
			const XalanDOMChar*		theName,
			const PrefixResolver&	thePrefixResolver,
			XalanDOMString&			thePrefix,
			bool					fUseDefault);

	// Not implemented...
	XalanSourceTreeDocument(const XalanSourceTreeDocument&	theSource);

	XalanSourceTreeDocument&
	operator=(const XalanSourceTreeDocument&	theRHS);

	bool
	operator==(const XalanSourceTreeDocument&	theRHS) const;


	// Data members...
	XalanNode*										m_firstChild;

	XalanSourceTreeElement*							m_documentElement;

	XalanSourceTreeAttributeAllocator				m_attributeAllocator;

	XalanSourceTreeAttributeNSAllocator				m_attributeNSAllocator;

	XalanSourceTreeCommentAllocator					m_commentAllocator;

	XalanSourceTreeElementAllocator					m_elementAllocator;

	XalanSourceTreeElementNSAllocator				m_elementNSAllocator;

	XalanSourceTreeProcessingInstructionAllocator	m_piAllocator;

	XalanSourceTreeTextAllocator					m_textAllocator;

	XalanSourceTreeTextIWSAllocator					m_textIWSAllocator;

	XalanDOMStringPool								m_namesStringPool;

	XalanDOMStringPool								m_valuesStringPool;

	AttributesArrayAllocatorType					m_attributesVector;

	unsigned int									m_nextIndexValue;

	const bool										m_poolAllText;

	ElementByIDMapType								m_elementsByID;

	UnparsedEntityURIMapType						m_unparsedEntityURIs;

	StringCollectionType							m_nonPooledStrings;

	XalanDOMString									m_stringBuffer;

	static const XalanDOMString&					s_nameString;
};



#endif	// !defined(XALANSOURCETREEDOCUMENT_HEADER_GUARD_1357924680)
