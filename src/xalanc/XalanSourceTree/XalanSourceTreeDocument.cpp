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
#include "XalanSourceTreeDocument.hpp"



#include <xercesc/sax/AttributeList.hpp>
#include <xercesc/sax2/Attributes.hpp>



#include <xalanc/XalanDOM/XalanDOMException.hpp>



#include <xalanc/PlatformSupport/PrefixResolver.hpp>
#include <xalanc/PlatformSupport/XalanUnicode.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "XalanSourceTreeHelper.hpp"



XALAN_CPP_NAMESPACE_BEGIN



static const XalanDOMString		s_emptyString;



XalanSourceTreeDocument::XalanSourceTreeDocument(
			bool				fPoolAllText,
			block_size_type		theNamesStringPoolBlockSize,
			bucket_count_type	theNamesStringPoolBucketCount,
			bucket_size_type	theNamesStringPoolBucketSize,
			block_size_type		theValuesStringPoolBlockSize,
			bucket_count_type	theValuesStringPoolBucketCount,
			bucket_size_type	theValuesStringPoolBucketSize) :
	XalanDocument(),
	m_firstChild(0),
	m_documentElement(0),
	m_attributeAllocator(eDefaultAttributeAllocatorBlockSize),
	m_attributeNSAllocator(eDefaultAttributeNSAllocatorBlockSize),
	m_commentAllocator(eDefaultCommentAllocatorBlockSize),
	m_elementAAllocator(eDefaultElementAllocatorBlockSize),
	m_elementANSAllocator(eDefaultElementNSAllocatorBlockSize),
	m_elementNAAllocator(eDefaultElementAllocatorBlockSize),
	m_elementNANSAllocator(eDefaultElementNSAllocatorBlockSize),
	m_piAllocator(eDefaultPIAllocatorBlockSize),
	m_textAllocator(eDefaultTextAllocatorBlockSize),
	m_textIWSAllocator(eDefaultTextIWSAllocatorBlockSize),
	m_namesStringPool(theNamesStringPoolBlockSize, theNamesStringPoolBucketCount, theNamesStringPoolBucketSize),
	m_valuesStringPool(theValuesStringPoolBlockSize, theValuesStringPoolBucketCount, theValuesStringPoolBucketSize),
	m_attributesVector(),
	m_nextIndexValue(2),
	m_poolAllText(fPoolAllText),
	m_elementsByID(),
	m_unparsedEntityURIs(),
	m_nonPooledStrings(),
	m_stringBuffer()
{
}



XalanSourceTreeDocument::XalanSourceTreeDocument(
			allocator_size_type		theAttributeBlockSize,
			allocator_size_type		theAttributeNSBlockSize,
			allocator_size_type		theCommentBlockSize,
			allocator_size_type		theElementBlockSize,
			allocator_size_type		theElementNSBlockSize,
			allocator_size_type		thePIBlockSize,
			allocator_size_type		theTextBlockSize,
			allocator_size_type		theTextIWSBlockSize,
			bool					fPoolAllText) :
	XalanDocument(),
	m_firstChild(0),
	m_documentElement(0),
	m_attributeAllocator(theAttributeBlockSize),
	m_attributeNSAllocator(theAttributeNSBlockSize),
	m_commentAllocator(theCommentBlockSize),
	m_elementAAllocator(theElementBlockSize),
	m_elementANSAllocator(theElementNSBlockSize),
	m_elementNAAllocator(theElementBlockSize),
	m_elementNANSAllocator(theElementNSBlockSize),
	m_piAllocator(thePIBlockSize),
	m_textAllocator(theTextBlockSize),
	m_textIWSAllocator(theTextIWSBlockSize),
	m_namesStringPool(eDefaultNamesStringPoolBlockSize, eDefaultNamesStringPoolBucketCount, eDefaultNamesStringPoolBucketSize),
	m_valuesStringPool(eDefaultValuesStringPoolBlockSize, eDefaultValuesStringPoolBucketCount, eDefaultValuesStringPoolBucketSize),
	m_attributesVector(),
	m_nextIndexValue(2),
	m_poolAllText(fPoolAllText),
	m_elementsByID(),
	m_unparsedEntityURIs(),
	m_nonPooledStrings(),
	m_stringBuffer()
{
}



XalanSourceTreeDocument::~XalanSourceTreeDocument()
{
}



const XalanDOMString&
XalanSourceTreeDocument::getNodeName() const
{
	return s_nameString;
}



const XalanDOMString&
XalanSourceTreeDocument::getNodeValue() const
{
	return s_emptyString;
}



XalanSourceTreeDocument::NodeType
XalanSourceTreeDocument::getNodeType() const
{
	return DOCUMENT_NODE;
}



XalanNode*
XalanSourceTreeDocument::getParentNode() const
{
	return 0;
}



const XalanNodeList*
XalanSourceTreeDocument::getChildNodes() const
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeDocument::getFirstChild() const
{
	return m_firstChild;
}



XalanNode*
XalanSourceTreeDocument::getLastChild() const
{
	return m_documentElement;
}



XalanNode*
XalanSourceTreeDocument::getPreviousSibling() const
{
	return 0;
}



XalanNode*
XalanSourceTreeDocument::getNextSibling() const
{
	return 0;
}



const XalanNamedNodeMap*
XalanSourceTreeDocument::getAttributes() const
{
	return 0;
}



XalanDocument*
XalanSourceTreeDocument::getOwnerDocument() const
{
	return 0;
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XalanSourceTreeDocument*
#endif
XalanSourceTreeDocument::cloneNode(bool		/* deep */) const
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeDocument::insertBefore(
			XalanNode*	/* newChild */,
			XalanNode*	/* refChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeDocument::replaceChild(
			XalanNode*	/* newChild */,
			XalanNode*	/* oldChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeDocument::removeChild(XalanNode*	 /* oldChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XalanSourceTreeDocument::appendChild(XalanNode*		/* newChild */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



bool
XalanSourceTreeDocument::hasChildNodes() const
{
	return m_documentElement != 0 ? true : false;
}



void
XalanSourceTreeDocument::setNodeValue(const XalanDOMString&		/* nodeValue */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XalanSourceTreeDocument::normalize()
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}


bool
XalanSourceTreeDocument::isSupported(
			const XalanDOMString&	/* feature */,
			const XalanDOMString&	/* version */) const
{
	return false;
}



const XalanDOMString&
XalanSourceTreeDocument::getNamespaceURI() const
{
	return s_emptyString;
}



const XalanDOMString&
XalanSourceTreeDocument::getPrefix() const
{
	return s_emptyString;
}



const XalanDOMString&
XalanSourceTreeDocument::getLocalName() const
{
	return s_nameString;
}



void
XalanSourceTreeDocument::setPrefix(const XalanDOMString&	/* prefix */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
XalanSourceTreeDocument::isIndexed() const
{
	return true;
}



XalanSourceTreeDocument::IndexType
XalanSourceTreeDocument::getIndex() const
{
	return 1;
}



XalanElement*
XalanSourceTreeDocument::createElement(const XalanDOMString&	/* tagName */)
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	// Dummy return value...
	return 0;
}



XalanDocumentFragment*
XalanSourceTreeDocument::createDocumentFragment()
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	// Dummy return value...
	return 0;
}



XalanText*
XalanSourceTreeDocument::createTextNode(const XalanDOMString&	/* data */)
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	// Dummy return value...
	return 0;
}



XalanComment*
XalanSourceTreeDocument::createComment(const XalanDOMString&	/* data */)
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	// Dummy return value...
	return 0;
}



XalanCDATASection*
XalanSourceTreeDocument::createCDATASection(const XalanDOMString&	/* data */)
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	// Dummy return value...
	return 0;
}



XalanProcessingInstruction*
XalanSourceTreeDocument::createProcessingInstruction(
			const XalanDOMString&	/* target */,
			const XalanDOMString&	/* data */)
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	// Dummy return value...
	return 0;
}



XalanAttr*
XalanSourceTreeDocument::createAttribute(const XalanDOMString&	/* name */)
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	// Dummy return value...
	return 0;
}



XalanEntityReference*
XalanSourceTreeDocument::createEntityReference(const XalanDOMString&	/* name */)
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	// Dummy return value...
	return 0;
}



XalanDocumentType*
XalanSourceTreeDocument::getDoctype() const
{
	return 0;
}




XalanDOMImplementation*
XalanSourceTreeDocument::getImplementation() const
{
	return 0;
}



XalanElement*
XalanSourceTreeDocument::getDocumentElement() const
{
	return m_documentElement;
}



XalanNodeList*
XalanSourceTreeDocument::getElementsByTagName(const XalanDOMString&		/* tagname */) const
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	return 0;
}



XalanNode*
XalanSourceTreeDocument::importNode(
			XalanNode*	/* importedNode */,
			bool		/* deep */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	return 0;
}



XalanElement*
XalanSourceTreeDocument::createElementNS(
			const XalanDOMString&	/* namespaceURI */,
			const XalanDOMString&	/* qualifiedName */)
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	return 0;
}



XalanAttr*
XalanSourceTreeDocument::createAttributeNS(
			const XalanDOMString&	/* namespaceURI */,
			const XalanDOMString&	/* qualifiedName */)
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	return 0;
}



XalanNodeList*
XalanSourceTreeDocument::getElementsByTagNameNS(
			const XalanDOMString&	/* namespaceURI */,
			const XalanDOMString&	/* localName */) const
{
	throw XalanDOMException(XalanDOMException::NOT_SUPPORTED_ERR);

	// Dummy return value...
	return 0;
}



XalanElement*
XalanSourceTreeDocument::getElementById(const XalanDOMString&	elementId) const
{
	const ElementByIDMapType::const_iterator	i =
		m_elementsByID.find(c_wstr(elementId));

	if (i == m_elementsByID.end())
	{
		return 0;
	}
	else
	{
		return (*i).second;
	}
}



static bool
hasXMLNamespaceAttribute(const AttributeListType&	attrs)
{
	const unsigned int	theLength = attrs.getLength();

	for (unsigned int i = 0; i < theLength; ++i)
	{
		const XalanDOMChar* const	theName =
			attrs.getName(i);
		assert(theName != 0);

		if(equals(theName, DOMServices::s_XMLNamespacePrefix) == true)
		{
			return true;
		}
	}

	return false;
}



static bool
hasXMLNamespaceAttribute(const AttributesType&	attrs)
{
	const unsigned int	theLength = attrs.getLength();

	for (unsigned int i = 0; i < theLength; ++i)
	{
		const XalanDOMChar* const	theName =
			attrs.getQName(i);
		assert(theName != 0);

		if(equals(theName, DOMServices::s_XMLNamespacePrefix) == true)
		{
			return true;
		}
	}

	return false;
}



size_t
XalanSourceTreeDocument::createAttributes(
			XalanSourceTreeAttr**		theAttributeVector,
			const AttributeListType&	attrs,
			size_t						theStartIndex,
			XalanSourceTreeElement*		theOwnerElement,
			bool						fCreateNamespaces,
			const PrefixResolver*		thePrefixResolver)
{
	const unsigned int	theSAXAttributeCount = attrs.getLength();

	// Now, create the attributes...
	for(unsigned int i = 0; i < theSAXAttributeCount; ++i)
	{
		const XalanDOMChar* const	theName =
			attrs.getName(i);
		assert(theName != 0);

		const bool	isNamespaceNode = startsWith(theName, DOMServices::s_XMLNamespaceWithSeparator) == true ||
						theName == DOMServices::s_XMLNamespace;

		if ((isNamespaceNode == true && fCreateNamespaces == true) ||
			(isNamespaceNode == false && fCreateNamespaces == false))
		{
			const XalanDOMChar* const	theValue =
				attrs.getValue(i);
			assert(theValue != 0);

			if (thePrefixResolver == 0)
			{
				theAttributeVector[theStartIndex] =
					m_attributeAllocator.create(
						m_namesStringPool.get(theName),
						m_valuesStringPool.get(theValue),
						theOwnerElement,
						m_nextIndexValue++);

				assert(theAttributeVector[theStartIndex] != 0);
			}
			else
			{
                const XalanDOMChar*    theLocalName = 0;

				const XalanDOMString* const		theNamespace =
					getNamespaceForPrefix(
							theName,
							*thePrefixResolver,
							m_stringBuffer,
							false,
                            &theLocalName);

				if (theNamespace == 0 || length(*theNamespace) == 0)
				{
					// the prefix was returned by getNamespaceForPrefix()...
					assert(length(m_stringBuffer) == 0);

					theAttributeVector[theStartIndex] =
						m_attributeAllocator.create(
							m_namesStringPool.get(theName),
							m_valuesStringPool.get(theValue),
							theOwnerElement,
							m_nextIndexValue++);

					assert(theAttributeVector[theStartIndex] != 0);
				}
				else
				{
                    assert(theLocalName != 0);

					theAttributeVector[theStartIndex] =
						m_attributeNSAllocator.create(
								m_namesStringPool.get(theName),
								m_namesStringPool.get(theLocalName),
								m_namesStringPool.get(*theNamespace),
								// This is the prefix...
								m_namesStringPool.get(m_stringBuffer),
								m_valuesStringPool.get(theValue),
								theOwnerElement,
								m_nextIndexValue++);
				}
			}

			++theStartIndex;
		}
	}

	return theStartIndex;
}



XalanSourceTreeElement*
XalanSourceTreeDocument::createElementNode(
			const XalanDOMChar*			name,
			const AttributeListType&	attrs,
			XalanNode*					theParentNode,
			XalanNode*					thePreviousSibling,
			XalanNode*					theNextSibling,
			bool						fAddXMLNamespaceAttribute)
{
	// We might have typedef'ed this to something smaller than unsigned int.
	AttributesCountType		theAttributeCount = AttributesCountType(attrs.getLength());

	// assert that we didn't lose anything...
	assert(theAttributeCount == attrs.getLength());

	if (fAddXMLNamespaceAttribute == true)
	{
		if (hasXMLNamespaceAttribute(attrs) == true)
		{
			fAddXMLNamespaceAttribute = false;
		}
		else
		{
			++theAttributeCount;
		}
	}

	XalanSourceTreeAttr**		theAttributeVector = 0;
	XalanSourceTreeElement*		theNewElement = 0;

	if (theAttributeCount == 0)
	{
		theNewElement = m_elementNAAllocator.create(
				m_namesStringPool.get(name),
				this,
				theParentNode,
				thePreviousSibling,
				theNextSibling,
				m_nextIndexValue++);
	}
	else
	{
		theAttributeVector = m_attributesVector.allocate(theAttributeCount);

		theNewElement = m_elementAAllocator.create(
				m_namesStringPool.get(name),
				this,
				theAttributeVector,
				theAttributeCount,
				theParentNode,
				thePreviousSibling,
				theNextSibling,
				m_nextIndexValue++);
	}
	assert(theNewElement != 0);

	size_t	theIndex = 0;

	if (fAddXMLNamespaceAttribute == true)
	{
		// The constructor parameters for AttrNS are:
		//
		// name
		// local name
		// namespace URI
		// prefix
		// value
		// owner element
		// index
		//
		theAttributeVector[theIndex] =
				m_attributeNSAllocator.create(
						m_namesStringPool.get(DOMServices::s_XMLNamespacePrefix),
						m_namesStringPool.get(DOMServices::s_XMLString),
						m_namesStringPool.get(DOMServices::s_XMLNamespacePrefixURI),
						m_namesStringPool.get(DOMServices::s_XMLNamespace),
						m_valuesStringPool.get(DOMServices::s_XMLNamespaceURI),
						theNewElement,
						m_nextIndexValue++);

		assert(theAttributeVector[theIndex] != 0);

		++theIndex;
	}

	// Create the namespace "nodes" first...
	theIndex = createAttributes(
				theAttributeVector,
				attrs,
				theIndex,
				theNewElement,
				true);

	// Now, create the attribute "nodes"...
	theIndex = createAttributes(
				theAttributeVector,
				attrs,
				theIndex,
				theNewElement,
				false);

	return theNewElement;
}



XalanSourceTreeElement*
XalanSourceTreeDocument::createElementNode(
			const XalanDOMChar*			tagName,
			const AttributeListType&	attrs,
			const PrefixResolver&		thePrefixResolver,
			XalanNode*					theParentNode,
			XalanNode*					thePreviousSibling,
			XalanNode*					theNextSibling,
			bool						fAddXMLNamespaceAttribute)
{
	// We might have typedef'ed this to something smaller than unsigned int.
	AttributesCountType		theAttributeCount = AttributesCountType(attrs.getLength());

	// assert that we didn't lose anything...
	assert(theAttributeCount == attrs.getLength());

	if (fAddXMLNamespaceAttribute == true)
	{
		if (hasXMLNamespaceAttribute(attrs) == true)
		{
			fAddXMLNamespaceAttribute = false;
		}
		else
		{
			++theAttributeCount;
		}
	}

	XalanSourceTreeAttr** const		theAttributeVector =
		theAttributeCount == 0 ? 0 : m_attributesVector.allocate(theAttributeCount);

	XalanSourceTreeElement* const	theNewElement =
		createElementNode(
			tagName,
			theAttributeVector,
			theAttributeCount,
			theParentNode,
			thePreviousSibling,
			theNextSibling,
			thePrefixResolver);

	assert(theNewElement != 0);

	size_t	theIndex = 0;

	if (fAddXMLNamespaceAttribute == true)
	{
		// The constructor parameters for AttrNS are:
		//
		// name
		// local name
		// namespace URI
		// prefix
		// value
		// owner element
		// index
		//
		theAttributeVector[theIndex] =
				m_attributeNSAllocator.create(
						m_namesStringPool.get(DOMServices::s_XMLNamespacePrefix),
						m_namesStringPool.get(DOMServices::s_XMLString),
						m_namesStringPool.get(DOMServices::s_XMLNamespacePrefixURI),
						m_namesStringPool.get(DOMServices::s_XMLNamespace),
						m_valuesStringPool.get(DOMServices::s_XMLNamespaceURI),
						theNewElement,
						m_nextIndexValue++);

		assert(theAttributeVector[theIndex] != 0);

		++theIndex;
	}

	// Create the namespace "nodes" first...
	theIndex = createAttributes(
				theAttributeVector,
				attrs,
				theIndex,
				theNewElement,
				true,
                &thePrefixResolver);

	// Now, create the attribute "nodes"...
	theIndex = createAttributes(
				theAttributeVector,
				attrs,
				theIndex,
				theNewElement,
				false,
                &thePrefixResolver);

	return theNewElement;
}



inline const XalanDOMString&
getElementNodePrefix(
			const XalanDOMChar*			qname,
			XalanDOMStringPool*			theStringPool,
			XalanDOMString::size_type	theLength,			
			XalanDOMString::size_type	theColonIndex)
{
	if(theColonIndex == theLength)
	{
		return theStringPool->get(XalanDOMString()); 
	}
	else
	{
		return theStringPool->get(qname, theColonIndex);
	}
}



XalanSourceTreeElement*
XalanSourceTreeDocument::createElementNode(
			const XalanDOMChar*		uri,
			const XalanDOMChar*		localname,
			const XalanDOMChar*		qname,
			const AttributesType&	attrs,
			XalanNode*				theParentNode,
			XalanNode*				thePreviousSibling,
			XalanNode*				theNextSibling,
			bool					fAddXMLNamespaceAttribute)
{

	// We might have typedef'ed this to something smaller than unsigned int.
	AttributesCountType		theAttributeCount = AttributesCountType(attrs.getLength());

	// assert that we didn't lose anything...
	assert(theAttributeCount == attrs.getLength());

	if (fAddXMLNamespaceAttribute == true)
	{
		if (hasXMLNamespaceAttribute(attrs) == true)
		{
			fAddXMLNamespaceAttribute = false;
		}
		else
		{
			++theAttributeCount;
		}
	}

	XalanSourceTreeAttr**		theAttributeVector = 0;
	XalanSourceTreeElement*		theNewElement = 0;

	const XalanDOMString::size_type		theColonIndex = indexOf(qname, XalanUnicode::charColon);
	const XalanDOMString::size_type		theLength = length(qname);

	if (theAttributeCount == 0)
	{

		theNewElement = m_elementNANSAllocator.create(
				m_namesStringPool.get(qname),
				m_namesStringPool.get(localname),
				m_namesStringPool.get(uri),
				// This is the prefix...
				getElementNodePrefix(qname, &m_namesStringPool, theLength, theColonIndex),
				this,
				theParentNode,
				thePreviousSibling,
				theNextSibling,
				m_nextIndexValue++);
	}
	else
	{
		theAttributeVector = m_attributesVector.allocate(theAttributeCount);

		theNewElement = m_elementANSAllocator.create(
				m_namesStringPool.get(qname),
				m_namesStringPool.get(localname),
				m_namesStringPool.get(uri),
				// This is the prefix...
				getElementNodePrefix(qname, &m_namesStringPool, theLength, theColonIndex),
				this,
				theAttributeVector,
				theAttributeCount,
				theParentNode,
				thePreviousSibling,
				theNextSibling,
				m_nextIndexValue++);

		createAttributes(attrs, theAttributeVector, theNewElement, fAddXMLNamespaceAttribute);
	}

	return theNewElement;
}



XalanSourceTreeElement*
XalanSourceTreeDocument::createElementNode(
			const XalanDOMChar*		name,
			const AttributesType&	attrs,
			XalanNode*				theParentNode,
			XalanNode*				thePreviousSibling,
			XalanNode*				theNextSibling,
			bool					fAddXMLNamespaceAttribute)
{
	assert(indexOf(name, XalanUnicode::charColon) == length(name));

	// We might have typedef'ed this to something smaller than unsigned int.
	AttributesCountType		theAttributeCount = AttributesCountType(attrs.getLength());

	// assert that we didn't lose anything...
	assert(theAttributeCount == attrs.getLength());

	if (fAddXMLNamespaceAttribute == true)
	{
		if (hasXMLNamespaceAttribute(attrs) == true)
		{
			fAddXMLNamespaceAttribute = false;
		}
		else
		{
			++theAttributeCount;
		}
	}

	XalanSourceTreeAttr**		theAttributeVector = 0;
	XalanSourceTreeElement*		theNewElement = 0;

	if (theAttributeCount == 0)
	{
		theNewElement = m_elementNAAllocator.create(
				m_namesStringPool.get(name),
				this,
				theParentNode,
				thePreviousSibling,
				theNextSibling,
				m_nextIndexValue++);
	}
	else
	{
		theAttributeVector = m_attributesVector.allocate(theAttributeCount);

		theNewElement = m_elementAAllocator.create(
				m_namesStringPool.get(name),
				this,
				theAttributeVector,
				theAttributeCount,
				theParentNode,
				thePreviousSibling,
				theNextSibling,
				m_nextIndexValue++);

		createAttributes(attrs, theAttributeVector, theNewElement, fAddXMLNamespaceAttribute);
	}

	return theNewElement;
}



XalanSourceTreeComment*
XalanSourceTreeDocument::createCommentNode(
			const XalanDOMChar*			data,
			XalanDOMString::size_type	length,
			XalanNode*					theParentNode,
			XalanNode*					thePreviousSibling,
			XalanNode*					theNextSibling)
{
	return m_commentAllocator.create(
				m_valuesStringPool.get(data, length),
				this,
				theParentNode,
				thePreviousSibling,
				theNextSibling,
				m_nextIndexValue++);
}



XalanSourceTreeProcessingInstruction*
XalanSourceTreeDocument::createProcessingInstructionNode(
			const XalanDOMChar*		target,
			const XalanDOMChar*		data,
			XalanNode*				theParentNode,
			XalanNode*				thePreviousSibling,
			XalanNode*				theNextSibling)
{
	assert(target != 0);
	assert(data != 0);

	return m_piAllocator.create(
				m_namesStringPool.get(target),
				m_valuesStringPool.get(data),
				this,
				theParentNode,
				thePreviousSibling,
				theNextSibling,
				m_nextIndexValue++);
}



inline const XalanDOMString&
XalanSourceTreeDocument::getTextNodeString(
			const XalanDOMChar*			chars,
			XalanDOMString::size_type	length)
{
	if (m_poolAllText == true)
	{
		return m_valuesStringPool.get(chars, length);
	}
	else
	{
		const StringCollectionType::iterator	theIterator =
				m_nonPooledStrings.insert(m_nonPooledStrings.end(), XalanDOMString());

		XalanDOMString&		theString = *theIterator;

		assign(theString, chars, length);

		assert(length == theString.length());

		return theString;
	}
}



const XalanDOMString*
XalanSourceTreeDocument::getNamespaceForPrefix(
			const XalanDOMChar*		theName,
			const PrefixResolver&	thePrefixResolver,
			XalanDOMString&			thePrefix,
			bool					fUseDefault,
            const XalanDOMChar**    theLocalName)
{
	const XalanDOMString::size_type		theLength = length(theName);
	const XalanDOMString::size_type		theColonIndex = indexOf(theName, XalanUnicode::charColon);

	if (theColonIndex != theLength)
	{
		// Get the prefix from theName...
		assign(thePrefix, theName, theColonIndex);
		assert(length(thePrefix) != 0);

        if (theLocalName != 0)
        {
            *theLocalName = theName + theColonIndex + 1;
        }

		return thePrefixResolver.getNamespaceForPrefix(thePrefix);
	}
	else
	{
		clear(thePrefix);

        if (theLocalName != 0)
        {
            *theLocalName = theName;
        }

		if (fUseDefault == false)
		{
			return 0;
		}
		else
		{
			return thePrefixResolver.getNamespaceForPrefix(s_emptyString);
		}
	}
}



XalanSourceTreeText*
XalanSourceTreeDocument::createTextNode(
			const XalanDOMChar*			chars,
			XalanDOMString::size_type	length,
			XalanNode*					theParentNode,
			XalanNode*					thePreviousSibling,
			XalanNode*					theNextSibling)
{
	assert(chars != 0);

	if (isXMLWhitespace(chars, 0, length) == true)
	{
		const XalanDOMString&	theString = m_valuesStringPool.get(chars, length);

		return m_textIWSAllocator.create(
				theString,
				theParentNode,
				thePreviousSibling,
				theNextSibling,
				m_nextIndexValue++);
	}
	else
	{
		return m_textAllocator.create(
				getTextNodeString(chars, length),
				theParentNode,
				thePreviousSibling,
				theNextSibling,
				m_nextIndexValue++);
	}
}



XalanSourceTreeText*
XalanSourceTreeDocument::createTextIWSNode(
			const XalanDOMChar*			chars,
			XalanDOMString::size_type	length,
			XalanNode*					theParentNode,
			XalanNode*					thePreviousSibling,
			XalanNode*					theNextSibling)
{
	assert(chars != 0);

	return m_textIWSAllocator.create(
			m_valuesStringPool.get(chars, length),
			theParentNode,
			thePreviousSibling,
			theNextSibling,
			m_nextIndexValue++);
}



void
XalanSourceTreeDocument::unparsedEntityDeclaration(
			const XalanDOMChar*		name,
			const XalanDOMChar*		/* publicId */,
			const XalanDOMChar*		systemId,
			const XalanDOMChar*		/* notationName */)
{
	m_unparsedEntityURIs[XalanDOMString(name)] = XalanDOMString(systemId);
}



const XalanDOMString&
XalanSourceTreeDocument::getUnparsedEntityURI(const XalanDOMString&		theName) const
{
	const UnparsedEntityURIMapType::const_iterator	i =
		m_unparsedEntityURIs.find(theName);

	if (i != m_unparsedEntityURIs.end())
	{
		return (*i).second;
	}
	else
	{
		return s_emptyString;
	}
}



XalanSourceTreeAttr*
XalanSourceTreeDocument::createAttribute(
			const XalanDOMChar*			theName,
			const XalanDOMChar*			theValue,
			XalanSourceTreeElement*		theOwnerElement,
			const PrefixResolver&		thePrefixResolver)
{
	// Get the namespace for the theName.  Since attributes do not use
	// the default namespace, make sure we don't get it...
	const XalanDOMString* const		theNamespace =
		getNamespaceForPrefix(
				theName,
				thePrefixResolver,
				m_stringBuffer,
				false);

	assert(theNamespace == 0 && length(m_stringBuffer) == 0 ||
		   theNamespace != 0 && length(m_stringBuffer) != 0);

	if (theNamespace == 0 || length(*theNamespace) == 0)
	{
		return m_attributeAllocator.create(
				m_namesStringPool.get(theName),
				m_valuesStringPool.get(theValue),
				theOwnerElement,
				m_nextIndexValue++);
	}
	else
	{
		// There must be a prefix, so we don't have to check to see if
		// we got one...

		// The constructor parameters for AttrNS are:
		//
		// name
		// local name
		// namespace URI
		// prefix
		// value
		// owner element
		// index
		//
		return m_attributeNSAllocator.create(
				m_namesStringPool.get(theName),
				m_namesStringPool.get(theName + length(m_stringBuffer) + 1),
				m_namesStringPool.get(*theNamespace),
				// This is the prefix...
				m_namesStringPool.get(m_stringBuffer),
				m_valuesStringPool.get(theValue),
				theOwnerElement,
				m_nextIndexValue++);
	}
}



XalanSourceTreeElement*
XalanSourceTreeDocument::createElementNode(
			const XalanDOMChar*		theTagName,
			XalanSourceTreeAttr**	theAttributeVector,
			AttributesCountType		theAttributeCount,
			XalanNode*				theParentNode,
			XalanNode*				thePreviousSibling,
			XalanNode*				theNextSibling,
			const PrefixResolver&	thePrefixResolver)
{
	const XalanDOMString* const		theNamespace =
		getNamespaceForPrefix(
				theTagName,
				thePrefixResolver,
				m_stringBuffer,
				true);

	if (theNamespace == 0 || length(*theNamespace) == 0)
	{
		// the prefix was returned by getNamespaceForPrefix()...
		assert(length(m_stringBuffer) == 0);

		if (theAttributeCount == 0)
		{
			return m_elementNAAllocator.create(
					m_namesStringPool.get(theTagName),
					this,
					theParentNode,
					thePreviousSibling,
					theNextSibling,
					m_nextIndexValue++);
		}
		else
		{
			return m_elementAAllocator.create(
					m_namesStringPool.get(theTagName),
					this,
					theAttributeVector,
					theAttributeCount,
					theParentNode,
					thePreviousSibling,
					theNextSibling,
					m_nextIndexValue++);
		}
	}
	else
	{
		// We need figure out if there's a prefix on theTagName.  If not,
		// the local name is the same as the tag name.  Otherwise, we need
		// to remove the prefix and the ':' that separates them.  If
		// m_stringBuffer is of length 0, there's no prefix.
		const XalanDOMString::size_type		thePrefixLength = length(m_stringBuffer);

		const XalanDOMChar* const			theLocalName =
			thePrefixLength == 0 ? theTagName : theTagName + thePrefixLength + 1;

		// The constructor parameters for ElementNS are:
		//
		// tag name
		// local name
		// namespace URI
		// prefix
		// owner document
		// attributes
		// attribute count
		// parent element
		// previous sibling
		// next sibling
		// index
		//
		if (theAttributeCount == 0)
		{
			return m_elementNANSAllocator.create(
					m_namesStringPool.get(theTagName),
					m_namesStringPool.get(theLocalName),
					m_namesStringPool.get(*theNamespace),
					m_namesStringPool.get(m_stringBuffer),
					this,
					theParentNode,
					thePreviousSibling,
					theNextSibling,
					m_nextIndexValue++);
		}
		else
		{
			return m_elementANSAllocator.create(
					m_namesStringPool.get(theTagName),
					m_namesStringPool.get(theLocalName),
					m_namesStringPool.get(*theNamespace),
					m_namesStringPool.get(m_stringBuffer),
					this,
					theAttributeVector,
					theAttributeCount,
					theParentNode,
					thePreviousSibling,
					theNextSibling,
					m_nextIndexValue++);
		}
	}
}



size_t
XalanSourceTreeDocument::createAttributes(
			XalanSourceTreeAttr**		theAttributeVector,
			const AttributesType&		theAttributes,
			size_t						theStartIndex,
			XalanSourceTreeElement*		theOwnerElement,
			bool						fCreateNamespaces)
{
	const unsigned int	theSAXAttributeCount = theAttributes.getLength();

	// Now, create the attributes...
	for(unsigned int i = 0; i < theSAXAttributeCount; ++i)
	{
		const XalanDOMChar* const	theQName =
			theAttributes.getQName(i);
		assert(theQName != 0);

		const bool	isNamespaceNode = startsWith(theQName, DOMServices::s_XMLNamespaceWithSeparator) == true ||
						theQName == DOMServices::s_XMLNamespace;

		if ((isNamespaceNode == true && fCreateNamespaces == true) ||
			(isNamespaceNode == false && fCreateNamespaces == false))
		{
			const XalanDOMChar* const	theValue =
				theAttributes.getValue(i);
			assert(theValue != 0);

			const XalanDOMChar* const	theURI =
				theAttributes.getURI(i);
			assert(theURI != 0);

			if (length(theURI) == 0)
			{
				theAttributeVector[theStartIndex] =
					m_attributeAllocator.create(
						m_namesStringPool.get(theQName),
						m_valuesStringPool.get(theValue),
						theOwnerElement,
						m_nextIndexValue++);
			}
			else
			{
				const XalanDOMChar* const	theLocalName =
					theAttributes.getLocalName(i);
				assert(theLocalName != 0);

				const XalanDOMString::size_type		theColonIndex = indexOf(theQName, XalanUnicode::charColon);
				assert(theColonIndex != length(theQName));

				// The constructor parameters for AttrNS are:
				//
				// name
				// local name
				// namespace URI
				// prefix
				// value
				// owner element
				// index
				//
				theAttributeVector[theStartIndex] =
					m_attributeNSAllocator.create(
							m_namesStringPool.get(theQName),
							m_namesStringPool.get(theLocalName),
							m_namesStringPool.get(theURI),
							// This is the prefix...
							m_namesStringPool.get(theQName, theColonIndex),
							m_valuesStringPool.get(theValue),
							theOwnerElement,
							m_nextIndexValue++);
			}

			// OK, now let's see if we have an ID attribute...
			const XalanDOMChar*		theType =
				theAttributes.getType(i);

			// Look for an attribute that was declared as type ID in the DTD.
			// Note that we can just save a pointer to the attribute's XalanDOMString
			// data, since the attribute is guaranteed to exist for as long as the
			// document does.
			if (*theType == XalanUnicode::charLetter_I &&
				*++theType == XalanUnicode::charLetter_D &&
				*++theType == 0)
			{
				// The XPath says that if there are duplicate IDs, the first node is
				// always returned, so use insert(), rather than []
				m_elementsByID.insert(
					ElementByIDMapType::value_type(
						c_wstr(theAttributeVector[theStartIndex]->getValue()),
						theOwnerElement));
			}

			++theStartIndex;
		}
	}

	return theStartIndex;
}



void
XalanSourceTreeDocument::createAttributes(
			const AttributesType&		theAttributes,
			XalanSourceTreeAttr**		theAttributeVector,
			XalanSourceTreeElement*		theOwnerElement,
			bool						fAddXMLNamespaceAttribute)
{
	size_t	theIndex = 0;

	if (fAddXMLNamespaceAttribute == true)
	{
		// The constructor parameters for AttrNS are:
		//
		// name
		// local name
		// namespace URI
		// prefix
		// value
		// owner element
		// index
		//
		theAttributeVector[theIndex] =
				m_attributeNSAllocator.create(
						m_namesStringPool.get(DOMServices::s_XMLNamespacePrefix),
						m_namesStringPool.get(DOMServices::s_XMLString),
						m_namesStringPool.get(DOMServices::s_XMLNamespacePrefixURI),
						m_namesStringPool.get(DOMServices::s_XMLNamespace),
						m_valuesStringPool.get(DOMServices::s_XMLNamespaceURI),
						theOwnerElement,
						m_nextIndexValue++);

		++theIndex;
	}

	// Create the namespace "nodes" first...
	theIndex = createAttributes(
				theAttributeVector,
				theAttributes,
				theIndex,
				theOwnerElement,
				true);

	// Now, create the attribute "nodes"...
	theIndex = createAttributes(
				theAttributeVector,
				theAttributes,
				theIndex,
				theOwnerElement,
				false);
}



void
XalanSourceTreeDocument::appendChildNode(XalanSourceTreeComment*	theChild)
{
	XalanSourceTreeHelper::appendSibling(this, m_firstChild, theChild);
}



void
XalanSourceTreeDocument::appendChildNode(XalanSourceTreeElement*	theChild)
{
	if (m_documentElement != 0)
	{
		throw XalanDOMException(XalanDOMException::HIERARCHY_REQUEST_ERR);
	}
	else
	{
		m_documentElement = theChild;

		XalanSourceTreeHelper::appendSibling(this, m_firstChild, theChild);
	}
}



void
XalanSourceTreeDocument::appendChildNode(XalanSourceTreeProcessingInstruction*	theChild)
{
	XalanSourceTreeHelper::appendSibling(this, m_firstChild, theChild);
}



static XalanDOMString	s_staticNameString;



const XalanDOMString&	XalanSourceTreeDocument::s_nameString = s_staticNameString;



static const XalanDOMChar	s_document[] =
{
	XalanUnicode::charNumberSign,
	XalanUnicode::charLetter_d,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_t,
	0
};



void
XalanSourceTreeDocument::initialize()
{
	s_staticNameString = s_document;
}



void
XalanSourceTreeDocument::terminate()
{
	releaseMemory(s_staticNameString);
}



XALAN_CPP_NAMESPACE_END
