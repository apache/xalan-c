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
#include "XalanSourceTreeDocument.hpp"



#include <sax/AttributeList.hpp>
#include <sax2/Attributes.hpp>



#include <XalanDOM/XalanDOMException.hpp>



#include <PlatformSupport/XalanUnicode.hpp>



#include "XalanSourceTreeHelper.hpp"



static const XalanDOMString		s_emptyString;



XalanSourceTreeDocument::XalanSourceTreeDocument(bool	fDynamic) :
	XalanDocument(),
	m_firstChild(0),
	m_documentElement(0),
	m_children(*this),
	m_attributeAllocator(50),
	m_attributeNSAllocator(50),
	m_commentAllocator(20),
	m_cdataSectionAllocator(20),
	m_cdataSectionIWSAllocator(20),
	m_elementAllocator(50),
	m_elementNSAllocator(50),
	m_piAllocator(25),
	m_textAllocator(50),
	m_textIWSAllocator(50),
	m_stringPool(),
	m_attributesVector(),
	m_nextIndexValue(2),
	m_dynamic(fDynamic),
	m_elementsByID(),
	m_unparsedEntityURIs()
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
	return &m_children;
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
XalanSourceTreeDocument::supports(
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



unsigned long
XalanSourceTreeDocument::getIndex() const
{
	return 1;
}



XalanElement*
XalanSourceTreeDocument::createElement(const XalanDOMString&	/* tagName */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanDocumentFragment*
XalanSourceTreeDocument::createDocumentFragment()
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanText*
XalanSourceTreeDocument::createTextNode(const XalanDOMString&	/* data */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanComment*
XalanSourceTreeDocument::createComment(const XalanDOMString&	/* data */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanCDATASection*
XalanSourceTreeDocument::createCDATASection(const XalanDOMString&	/* data */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanProcessingInstruction*
XalanSourceTreeDocument::createProcessingInstruction(
			const XalanDOMString&	/* target */,
			const XalanDOMString&	/* data */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanAttr*
XalanSourceTreeDocument::createAttribute(const XalanDOMString&	/* name */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanEntityReference*
XalanSourceTreeDocument::createEntityReference(const XalanDOMString&	/* name */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

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
XalanSourceTreeDocument::getElementsByTagName(const XalanDOMString&	/* tagname */) const
{
	// Not supported...
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
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	return 0;
}



XalanAttr*
XalanSourceTreeDocument::createAttributeNS(
			const XalanDOMString&	/* namespaceURI */,
			const XalanDOMString&	/* qualifiedName */)
{
	throw XalanDOMException(XalanDOMException::NO_MODIFICATION_ALLOWED_ERR);

	return 0;
}



XalanNodeList*
XalanSourceTreeDocument::getElementsByTagNameNS(
			const XalanDOMString&	/* namespaceURI */,
			const XalanDOMString&	/* localName */) const
{
	// Not supported
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



#if 0
void
XalanSourceTreeDocument::buildBridgeNodes()
{
	const DOM_Node	theStartChild = m_xercesDocument.getFirstChild();

	if (theStartChild.isNull() == false)
	{
		assert(m_navigators.back().getIndex() == 1);
		assert(m_navigators.size() == 1);

		BuildBridgeTreeWalker	theTreeWalker(
				this,
				&m_navigators.back(),
				m_navigators,
				2);

		theTreeWalker.traverse(theStartChild, m_xercesDocument);
	}

	// OK, now set m_documentElement...
	XalanNode*	theChild = m_navigator.getFirstChild();

	while(theChild != 0 && theChild->getNodeType() != XalanNode::ELEMENT_NODE)
	{
		theChild = theChild->getNextSibling();
	}

#if defined(XALAN_OLD_STYLE_CASTS)
	m_documentElement = (XalanElement*)theChild;
#else
	m_documentElement = static_cast<XalanElement*>(theChild);
#endif

	m_indexValid = true;

	m_mappingMode = false;
}



XalanSourceTreeDocument::BuildBridgeTreeWalker::BuildBridgeTreeWalker(
			XalanSourceTreeDocument*		theDocument,
			XercesBridgeNavigator*		theDocumentNavigator,
			NavigatorBridgeVectorType&	theNavigators,
			unsigned long				theStartIndex) :
	m_document(theDocument),
	m_navigators(theNavigators),
	m_currentIndex(theStartIndex),
	m_parentNavigatorStack(),
	m_siblingNavigatorStack()
{
	assert(theDocument != 0 && theDocumentNavigator != 0);

	// Reserve some space...
	m_parentNavigatorStack.reserve(100);
	m_parentNavigatorStack.reserve(100);

	// The document navigator is the last navigator on the stack...
	m_parentNavigatorStack.push_back(NavigatorStackEntryType(theDocumentNavigator, theDocument));

	// There is no previous sibling...
	m_siblingNavigatorStack.push_back(NavigatorStackEntryType(0, 0));
}



XalanSourceTreeDocument::BuildBridgeTreeWalker::~BuildBridgeTreeWalker()
{
}



void
XalanSourceTreeDocument::BuildBridgeTreeWalker::startNode(const DOM_Node&	node)
{
	XalanNode* const	theBridgeNode = m_document->createBridgeNode(node, m_currentIndex, false);

	XercesBridgeNavigator&	theCurrentNodeNavigator = m_navigators.back();

	assert(m_parentNavigatorStack.empty() == false);
	assert(m_siblingNavigatorStack.empty() == false);

	// Get the two navigators...
	NavigatorStackEntryType&	theParentEntry = m_parentNavigatorStack.back();
	NavigatorStackEntryType&	theSiblingEntry = m_siblingNavigatorStack.back();

	theCurrentNodeNavigator.setParentNode(theParentEntry.m_node);

	// If the first child has not been set, then set it
	// now...
	if (theParentEntry.m_navigator->getFirstChild() == 0)
	{
		assert(theSiblingEntry.m_node == 0);

		theParentEntry.m_navigator->setFirstChild(theBridgeNode);
	}

	// Always set the last child...
	theParentEntry.m_navigator->setLastChild(theBridgeNode);

	theCurrentNodeNavigator.setPreviousSibling(theSiblingEntry.m_node);

	if (theSiblingEntry.m_navigator != 0)
	{
		theSiblingEntry.m_navigator->setNextSibling(theBridgeNode);
	}

	// Build an entry for the stacks...
	const NavigatorStackEntryType	theCurrentEntry(&theCurrentNodeNavigator, theBridgeNode);

	// My child nodes will now be visited, so push the current
	// context on the parent stack...
	m_parentNavigatorStack.push_back(theCurrentEntry);

	// My siblings will also need to know about me as well...
	m_siblingNavigatorStack.push_back(theCurrentEntry);

	// This will serve to mark the sibling context for my first child,
	// since it has no previous sibling.  This will be popped off
	// when endNode() is called.
	m_siblingNavigatorStack.push_back(NavigatorStackEntryType(0, 0));

	// Finally, increment the index counter...
	++m_currentIndex;

	const short		theType = node.getNodeType();

	if (theType == DOM_Node::DOCUMENT_TYPE_NODE)
	{
		// Special case for doctype -- we have to build its entities...
		const DOM_DocumentType&		theDoctype =
#if defined(XALAN_OLD_STYLE_CASTS)
			(const DOM_DocumentType&)node;
#else
			static_cast<const DOM_DocumentType&>(node);
#endif

		const DOM_NamedNodeMap	theEntities =
			theDoctype.getEntities();

		const unsigned int	theLength =
			theEntities.getLength();

		for (unsigned int i = 0; i < theLength; ++i)
		{
			// Build it, but don't index it...
			m_document->createBridgeNode(theEntities.item(i), m_currentIndex++, true);
		}
	}
	else if (theType == DOM_Node::ELEMENT_NODE)
	{
	// Special case for element nodes -- we have to build the attributes...
		const DOM_Element&	theElement =
#if defined(XALAN_OLD_STYLE_CASTS)
			(const DOM_Element&)node;
#else
			static_cast<const DOM_Element&>(node);
#endif

		const DOM_NamedNodeMap	theAttributes =
			theElement.getAttributes();

		const unsigned int	theLength =
			theAttributes.getLength();

		XercesBridgeNavigator*	thePreviousAttrNavigator = 0;
		XalanNode*				thePreviousAttr = 0;

		for (unsigned int i = 0; i < theLength; ++i)
		{
			// Get the attribute from the node map...
			const DOM_Node	theAttr = theAttributes.item(i);
			assert(theAttr.isNull() == false);

			// Create a bridge node.
			XalanNode* const	theCurrentAttr =
				m_document->createBridgeNode(theAttr, m_currentIndex, false);
			assert(theCurrentAttr != 0);

			// Get the attribute node's navigator...
			XercesBridgeNavigator&	theCurrentAttrNavigator =
				m_navigators.back();

			// Set the parent node...
			theCurrentAttrNavigator.setParentNode(theBridgeNode);

			if (thePreviousAttr != 0)
			{
				assert(thePreviousAttrNavigator != 0);

				// Link in the previous attribute...
				theCurrentAttrNavigator.setPreviousSibling(thePreviousAttr);

				thePreviousAttrNavigator->setNextSibling(theCurrentAttr);
			}

			// Update the pointers so they point to this attribute...
			thePreviousAttr = theCurrentAttr;
			thePreviousAttrNavigator = &theCurrentAttrNavigator;

			// Finally, increment the index...
			++m_currentIndex;
		}
	}
}



void
XalanSourceTreeDocument::BuildBridgeTreeWalker::endNode(const DOM_Node&	/* node */)
{
	assert(m_parentNavigatorStack.empty() == false);
	assert(m_siblingNavigatorStack.empty() == false);

	// I have to pop my entry, since my children are finished...
	m_parentNavigatorStack.pop_back();

	// Pop any sibling navigators my child pushed...
	while(m_siblingNavigatorStack.back().m_navigator != 0)
	{
		assert(m_siblingNavigatorStack.back().m_node != 0);

		m_siblingNavigatorStack.pop_back();
	}

	// There must be a context marker...
	assert(m_siblingNavigatorStack.back().m_navigator == 0 &&
		   m_siblingNavigatorStack.back().m_node == 0);

	// Pop the context marker...
	m_siblingNavigatorStack.pop_back();
}
#endif



XalanSourceTreeElement*
XalanSourceTreeDocument::createElementNode(
			const XalanDOMChar*			name,
			const AttributeList&		attrs,
			XalanSourceTreeElement*		theParentElement,
			XalanNode*					thePreviousSibling,
			XalanNode*					theNextSibling)
{
	const unsigned int	theAttributeCount = attrs.getLength();

	XalanSourceTreeAttr** const		theAttributeVector =
		theAttributeCount == 0 ? 0 : m_attributesVector.allocate(theAttributeCount);

	XalanSourceTreeElement* const	theNewElement =
			m_elementAllocator.create(
				m_stringPool.get(name),
				this,
				theAttributeVector,
				theAttributeCount,
				theParentElement,
				thePreviousSibling,
				theNextSibling,
				m_nextIndexValue++);

	// Now, create the attributes...
	for(unsigned int i = 0; i < theAttributeCount; ++i)
	{
		const XalanDOMChar* const	theName =
			attrs.getName(i);
		assert(theName != 0);

		const XalanDOMChar* const	theValue =
			attrs.getValue(i);
		assert(theValue != 0);

		theAttributeVector[i] =
			m_attributeAllocator.create(
				m_stringPool.get(theName),
				m_stringPool.get(theValue),
				theNewElement,
				m_nextIndexValue++);
	}

	return theNewElement;
}



XalanSourceTreeElement*
XalanSourceTreeDocument::createElementNode(
			const XalanDOMChar*			uri,
			const XalanDOMChar*			localname,
			const XalanDOMChar*			qname,
			const Attributes&			attrs,
			XalanSourceTreeElement*		theParentElement,
			XalanNode*					thePreviousSibling,
			XalanNode*					theNextSibling)
{
	const unsigned int	theAttributeCount = attrs.getLength();

	XalanSourceTreeAttr** const		theAttributeVector =
		theAttributeCount == 0 ? 0 : m_attributesVector.allocate(theAttributeCount);

	const unsigned int	theColonIndex = indexOf(qname, XalanUnicode::charColon);
	const unsigned int	theLength = length(qname);

	XalanSourceTreeElement* const	theNewElement =
		m_elementNSAllocator.create(
				m_stringPool.get(qname),
				m_stringPool.get(localname),
				m_stringPool.get(uri),
				// This is the prefix...
				theColonIndex == theLength ? m_stringPool.get(XalanDOMString()) : m_stringPool.get(qname, theColonIndex),
				this,
				theAttributeVector,
				theAttributeCount,
				theParentElement,
				thePreviousSibling,
				theNextSibling,
				m_nextIndexValue++);

	if (theAttributeCount != 0)
	{
		createAttributes(attrs, theAttributeVector, theAttributeCount, theNewElement);
	}

	return theNewElement;
}



XalanSourceTreeElement*
XalanSourceTreeDocument::createElementNode(
			const XalanDOMChar*			name,
			const Attributes&			attrs,
			XalanSourceTreeElement*		theParentElement,
			XalanNode*					thePreviousSibling,
			XalanNode*					theNextSibling)
{
	const unsigned int	theAttributeCount = attrs.getLength();

	XalanSourceTreeAttr** const		theAttributeVector =
		theAttributeCount == 0 ? 0 : m_attributesVector.allocate(theAttributeCount);

	assert(indexOf(name, XalanUnicode::charColon) == length(name));

	XalanSourceTreeElement* const	theNewElement =
		m_elementAllocator.create(
				m_stringPool.get(name),
				this,
				theAttributeVector,
				theAttributeCount,
				theParentElement,
				thePreviousSibling,
				theNextSibling,
				m_nextIndexValue++);

	if (theAttributeCount != 0)
	{
		createAttributes(attrs, theAttributeVector, theAttributeCount, theNewElement);
	}

	return theNewElement;
}



XalanSourceTreeCDATASection*
XalanSourceTreeDocument::createCDATASectionNode(
			const XalanDOMChar*			chars,
			unsigned int				length,
			XalanSourceTreeElement*		theParentElement,
			XalanNode*					thePreviousSibling,
			XalanNode*					theNextSibling)
{
	const XalanDOMString&	theString = m_stringPool.get(chars, length);

	if (isXMLWhitespace(theString) == true)
	{
		return m_cdataSectionIWSAllocator.create(
				theString,
				theParentElement,
				thePreviousSibling,
				theNextSibling,
				m_nextIndexValue++);
	}
	else
	{
		return m_cdataSectionAllocator.create(
				theString,
				theParentElement,
				thePreviousSibling,
				theNextSibling,
				m_nextIndexValue++);
	}
}



XalanSourceTreeComment*
XalanSourceTreeDocument::createCommentNode(
			const XalanDOMChar*			data,
			XalanSourceTreeElement*		theParentElement,
			XalanNode*					thePreviousSibling,
			XalanNode*					theNextSibling)
{
	return m_commentAllocator.create(
				m_stringPool.get(data),
				theParentElement,
				thePreviousSibling,
				theNextSibling,
				m_nextIndexValue++);
}



XalanSourceTreeProcessingInstruction*
XalanSourceTreeDocument::createProcessingInstructionNode(
			const XalanDOMChar*			target,
			const XalanDOMChar*			data,
			XalanSourceTreeElement*		theParentElement,
			XalanNode*					thePreviousSibling,
			XalanNode*					theNextSibling)
{
	assert(target != 0);
	assert(data != 0);

	return m_piAllocator.create(
				m_stringPool.get(target),
				m_stringPool.get(data),
				this,
				theParentElement,
				thePreviousSibling,
				theNextSibling,
				m_nextIndexValue++);
}



XalanSourceTreeText*
XalanSourceTreeDocument::createTextNode(
			const XalanDOMChar*			chars,
			unsigned int				length,
			XalanSourceTreeElement*		theParentElement,
			XalanNode*					thePreviousSibling,
			XalanNode*					theNextSibling)
{
	assert(chars != 0);

	const XalanDOMString&	theString = m_stringPool.get(chars, length);

	if (isXMLWhitespace(theString) == true)
	{
		return m_textIWSAllocator.create(
				theString,
				theParentElement,
				thePreviousSibling,
				theNextSibling,
				m_nextIndexValue++);
	}
	else
	{
		return m_textAllocator.create(
				theString,
				theParentElement,
				thePreviousSibling,
				theNextSibling,
				m_nextIndexValue++);
	}
}



XalanSourceTreeText*
XalanSourceTreeDocument::createTextIWSNode(
			const XalanDOMChar*			chars,
			unsigned int				length,
			XalanSourceTreeElement*		theParentElement,
			XalanNode*					thePreviousSibling,
			XalanNode*					theNextSibling)
{
	assert(chars != 0);

	return m_textIWSAllocator.create(
			m_stringPool.get(chars, length),
			theParentElement,
			thePreviousSibling,
			theNextSibling,
			m_nextIndexValue++);
}



void
XalanSourceTreeDocument::unparsedEntityDecl(
			const XMLCh*	name,
			const XMLCh*	publicId,
			const XMLCh*	systemId,
			const XMLCh*	notationName)
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



#if 0
// Commented out for now, since we need a way to get namespace URIs.  (Like Stylesheet does).
XalanSourceTreeAttr*
XalanSourceTreeDocument::createAttribute(
			const XalanDOMChar*			theName,
			const XalanDOMChar*			theValue,
			XalanSourceTreeElement*		theOwnerElement)
{
	const unsigned int	theLength = length(theName);
	const unsigned int	theColonIndex = indexOf(theName, XalanUnicode::charColon);

	if (theColonIndex == theLength)
	{
		return m_attributeAllocator.create(
				m_stringPool.get(theName),
				m_stringPool.get(theValue),
				theOwnerElement,
				m_nextIndexValue++);
	}
	else
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
		return m_attributeNSAllocator.create(
				m_stringPool.get(theName, theLength),
				m_stringPool.get(theName, theColonIndex),
				m_stringPool.get(XalanDOMString()),
				m_stringPool.get(theName + theColonIndex, theLength - theColonIndex),
				m_stringPool.get(theValue),
				theOwnerElement,
				m_nextIndexValue++);
	}

	return 0;
}



// Commented out for now, since we need a way to get namespace URIs.  (Like Stylesheet does).
XalanSourceTreeElement*
XalanSourceTreeDocument::createElement(
			const XalanDOMChar*			theTagName,
			XalanSourceTreeAttr**		theAttributeVector,
			unsigned int				theAttributeCount,
			XalanSourceTreeElement*		theParentElement,
			XalanNode*					thePreviousSibling,
			XalanNode*					theNextSibling)
{
	const unsigned int	theLength = length(theTagName);
	const unsigned int	theColonIndex = indexOf(theTagName, XalanUnicode::charColon);

	if (theColonIndex == theLength)
	{
		return m_elementAllocator.create(
				m_stringPool.get(theTagName),
				this,
				theAttributeVector,
				theAttributeCount,
				theParentElement,
				thePreviousSibling,
				theNextSibling,
				m_nextIndexValue++);
	}
	else
	{
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
		return m_elementNSAllocator.create(
				m_stringPool.get(theTagName, theLength),
				m_stringPool.get(theTagName + theColonIndex + 1, theColonIndex),
				// How do we get the namespace for the prefix?
				m_stringPool.get(XalanDOMString()),
				m_stringPool.get(theTagName,),
				this,
				theAttributeVector,
				theAttributeCount,
				theParentElement,
				thePreviousSibling,
				theNextSibling,
				m_nextIndexValue++);
	}

	return 0;
}
#endif



void
XalanSourceTreeDocument::createAttributes(
			const Attributes&			theAttributes,
			XalanSourceTreeAttr**		theAttributeVector,
			unsigned int				theAttributeCount,
			XalanSourceTreeElement*		theOwnerElement)
{
	// Now, create the attributes...
	for(unsigned int i = 0; i < theAttributeCount; ++i)
	{
		const XalanDOMChar* const	theQName =
			theAttributes.getQName(i);
		assert(theQName != 0);

		const XalanDOMChar* const	theValue =
			theAttributes.getValue(i);
		assert(theValue != 0);

		const XalanDOMChar* const	theURI =
			theAttributes.getURI(i);

#if 1
		if (equals(theURI, XALAN_STATIC_UCODE_STRING("EMPTY")) == true)
#else
		if (theURI == 0 || length(theURI) == 0)
#endif
		{
			theAttributeVector[i] =
				m_attributeAllocator.create(
					m_stringPool.get(theQName),
					m_stringPool.get(theValue),
					theOwnerElement,
					m_nextIndexValue++);
		}
		else
		{
			const XalanDOMChar* const	theLocalName =
				theAttributes.getLocalName(i);
			assert(theLocalName != 0);

			const unsigned int	theColonIndex = indexOf(theQName, XalanUnicode::charColon);
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
			theAttributeVector[i] =
				m_attributeNSAllocator.create(
						m_stringPool.get(theQName),
						m_stringPool.get(theLocalName),
						m_stringPool.get(theURI),
						// This is the prefix...
						m_stringPool.get(theQName, theColonIndex),
						m_stringPool.get(theValue),
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
			m_elementsByID[c_wstr(theAttributeVector[i]->getValue())] = theOwnerElement;
		}
	}
}



void
XalanSourceTreeDocument::setDocumentElement(XalanSourceTreeElement*		theElement)
{
	if (m_documentElement != 0)
	{
		throw XalanDOMException(XalanDOMException::HIERARCHY_REQUEST_ERR);
	}
	else
	{
		m_documentElement = theElement;

		XalanSourceTreeHelper::appendSibling(this, m_firstChild, theElement);
	}
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



static XalanDOMString	s_nameString;



const XalanDOMString&	XalanSourceTreeDocument::s_nameString = ::s_nameString;



void
XalanSourceTreeDocument::initialize()
{
	::s_nameString = XALAN_STATIC_UCODE_STRING("#document");
}



void
XalanSourceTreeDocument::terminate()
{
	clear(::s_nameString);
}
