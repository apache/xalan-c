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
#include "XercesDocumentBridge.hpp"



#include <algorithm>
#include <cassert>



#include <dom/DOM_Node.hpp>
#include <dom/DOM_DOMException.hpp>
#include <dom/DocumentImpl.hpp>
#include <dom/NodeImpl.hpp>



#include <PlatformSupport/STLHelper.hpp>



#include <XalanDOM/XalanDocumentType.hpp>
#include <XalanDOM/XalanElement.hpp>



#include "XercesAttrBridge.hpp"
#include "XercesCommentBridge.hpp"
#include "XercesCDATASectionBridge.hpp"
#include "XercesDOM_NodeHack.hpp"
#include "XercesDOMException.hpp"
#include "XercesDOMImplementationBridge.hpp"
#include "XercesDocumentFragmentBridge.hpp"
#include "XercesDocumentTypeBridge.hpp"
#include "XercesElementBridge.hpp"
#include "XercesEntityBridge.hpp"
#include "XercesEntityReferenceBridge.hpp"
#include "XercesNodeListBridge.hpp"
#include "XercesNotationBridge.hpp"
#include "XercesProcessingInstructionBridge.hpp"
#include "XercesTextBridge.hpp"



XercesDocumentBridge::XercesDocumentBridge(const DOM_Document&	theXercesDocument) :
	XalanDocument(),
	m_xercesDocument(theXercesDocument),
	m_navigator(this),
	m_children(theXercesDocument.getChildNodes(),
			   m_navigator),
	m_cachedNodeLists(theXercesDocument,
					  m_navigator),
	m_nodeMap(),
	m_domImplementation(new XercesDOMImplementationBridge(theXercesDocument.getImplementation())),
	m_nodes(),
	m_doctype(0)
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::make_pair;
#endif

	// Put ourself into the node map, and don't assign an index...
	m_nodeMap.addAssociation(m_xercesDocument, this, false);

	// Try to build the doctype...
	DOM_DocumentType	theDoctype = theXercesDocument.getDoctype();

	if (theDoctype.isNull() == false)
	{
		m_doctype = new XercesDocumentTypeBridge(theDoctype, m_navigator);

		// Add it to the node map...
		m_nodeMap.addAssociation(theDoctype, m_doctype, false);

		m_nodes.insert(m_doctype);
	}
}



XercesDocumentBridge::~XercesDocumentBridge()
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::for_each;
#endif

	// m_bridgeMap contains all of the nodes that
	// are still alive...
	for_each(m_nodes.begin(),
			 m_nodes.end(),
			 DeleteFunctor<XalanNode>());
}



XalanNode*
XercesDocumentBridge::mapNode(const DOM_Node&	theXercesNode) const
{
	return mapNode(XercesDOM_NodeHack::getImpl(theXercesNode));
}



XalanNode*
XercesDocumentBridge::mapNode(NodeImpl*		theXercesNodeImpl) const
{
	XalanNode*	theXalanNode = 0;

	if (theXercesNodeImpl != 0)
	{
		theXalanNode = m_nodeMap.getNode(theXercesNodeImpl);

		if (theXalanNode == 0)
		{
			// Make sure the node belongs to our document...
			NodeImpl* const	theOwnerDocument =
				XercesDOM_NodeHack::getImpl(m_xercesDocument);
			assert(theOwnerDocument != 0);

			if (theXercesNodeImpl != theOwnerDocument &&
					theXercesNodeImpl->getOwnerDocument() != theOwnerDocument &&
				theXercesNodeImpl->getParentNode() != theOwnerDocument)
			{
				throw XalanDOMException(XalanDOMException::WRONG_DOCUMENT_ERR);
			}
			else
			{
				// OK, we haven't yet created a bridge not, so go ahead and
				// create one.
				theXalanNode = createBridgeNode(XercesDOM_NodeHack(theXercesNodeImpl));
				assert(theXalanNode != 0);
			}
		}
	}

	return theXalanNode;
}



DOM_Node
XercesDocumentBridge::mapNode(const XalanNode*	theXalanNode) const
{
	DOM_Node	theXercesNode;

	if (theXalanNode != 0)
	{
		theXercesNode = m_nodeMap.getNode(theXalanNode);

		if (theXercesNode.isNull() == true)
		{
			throw XalanDOMException(XalanDOMException::WRONG_DOCUMENT_ERR);
		}
	}

	return theXercesNode;
}



DOM_Attr
XercesDocumentBridge::mapNode(const XalanAttr* 	theXalanNode) const
{
	NodeImpl* const		theXercesNodeImpl =
		mapNodeToImpl(theXalanNode);

	return XercesDOM_AttrHack(reinterpret_cast<AttrImpl*>(theXercesNodeImpl));
}



NodeImpl*
XercesDocumentBridge::mapNodeToImpl(const XalanNode* 	theXalanNode) const
{
	NodeImpl*	theXercesNodeImpl = 0;

	if (theXalanNode != 0)
	{
		theXercesNodeImpl = m_nodeMap.getNodeImpl(theXalanNode);

		if (theXercesNodeImpl == 0)
		{
			throw XalanDOMException(XalanDOMException::WRONG_DOCUMENT_ERR);
		}
	}

	return theXercesNodeImpl;
}



XalanAttr*
XercesDocumentBridge::mapNode(const DOM_Attr& 	theXercesNode) const
{
	return static_cast<XercesAttrBridge*>(mapNode(XercesDOM_NodeHack::getImpl(theXercesNode)));
}



XalanElement*
XercesDocumentBridge::mapNode(const DOM_Element& 	theXercesNode) const
{
	return static_cast<XercesElementBridge*>(mapNode(XercesDOM_NodeHack::getImpl(theXercesNode)));
}



XercesElementBridge*
XercesDocumentBridge::createBridgeNode(const DOM_Element& 	theXercesNode) const
{

	XercesElementBridge* const	theBridge =
		new XercesElementBridge(theXercesNode,
								m_navigator);

	m_nodes.insert(theBridge);

	// Add it to the node map...
	m_nodeMap.addAssociation(theXercesNode, theBridge, false);

	return theBridge;
}



XercesDocumentFragmentBridge*
XercesDocumentBridge::createBridgeNode(const DOM_DocumentFragment&	theXercesNode) const
{
	XercesDocumentFragmentBridge* const		theBridge =
		new XercesDocumentFragmentBridge(theXercesNode,
										 m_navigator);

	m_nodes.insert(theBridge);

	// Add it to the node map...
	m_nodeMap.addAssociation(theXercesNode, theBridge, false);

	return theBridge;
}



XercesTextBridge*
XercesDocumentBridge::createBridgeNode(const DOM_Text&	theXercesNode) const
{
	XercesTextBridge* const		theBridge =
		new XercesTextBridge(theXercesNode,
								m_navigator);

	m_nodes.insert(theBridge);

	// Add it to the node map...
	m_nodeMap.addAssociation(theXercesNode, theBridge, false);

	return theBridge;
}



XercesCommentBridge*
XercesDocumentBridge::createBridgeNode(const DOM_Comment&	theXercesNode) const
{
	XercesCommentBridge* const	theBridge =
		new XercesCommentBridge(theXercesNode,
								m_navigator);

	m_nodes.insert(theBridge);

	// Add it to the node map...
	m_nodeMap.addAssociation(theXercesNode, theBridge, false);

	return theBridge;
}



XercesCDATASectionBridge*
XercesDocumentBridge::createBridgeNode(const DOM_CDATASection&	theXercesNode) const
{
	XercesCDATASectionBridge* const		theBridge =
		new XercesCDATASectionBridge(theXercesNode,
									 m_navigator);

	m_nodes.insert(theBridge);

	// Add it to the node map...
	m_nodeMap.addAssociation(theXercesNode, theBridge, false);

	return theBridge;
}



XercesProcessingInstructionBridge*
XercesDocumentBridge::createBridgeNode(const DOM_ProcessingInstruction&		theXercesNode) const
{
	XercesProcessingInstructionBridge* const	theBridge =
		new XercesProcessingInstructionBridge(theXercesNode,
											  m_navigator);

	m_nodes.insert(theBridge);

	// Add it to the node map...
	m_nodeMap.addAssociation(theXercesNode, theBridge, false);

	return theBridge;
}



XercesAttrBridge*
XercesDocumentBridge::createBridgeNode(const DOM_Attr&	theXercesNode) const
{
	XercesAttrBridge* const		theBridge =
		new XercesAttrBridge(theXercesNode,
								m_navigator);

	m_nodes.insert(theBridge);

	// Add it to the node map...
	m_nodeMap.addAssociation(theXercesNode, theBridge, false);

	return theBridge;
}



XercesEntityBridge*
XercesDocumentBridge::createBridgeNode(const DOM_Entity&	theXercesNode) const
{
	XercesEntityBridge* const	theBridge =
		new XercesEntityBridge(theXercesNode,
								m_navigator);

	m_nodes.insert(theBridge);

	// Add it to the node map...
	m_nodeMap.addAssociation(theXercesNode, theBridge, false);

	return theBridge;
}



XercesEntityReferenceBridge*
XercesDocumentBridge::createBridgeNode(const DOM_EntityReference& 	theXercesNode) const
{
	XercesEntityReferenceBridge* const	theBridge =
		new XercesEntityReferenceBridge(theXercesNode,
										m_navigator);

	m_nodes.insert(theBridge);

	// Add it to the node map...
	m_nodeMap.addAssociation(theXercesNode, theBridge, false);

	return theBridge;
}



XercesNotationBridge*
XercesDocumentBridge::createBridgeNode(const DOM_Notation&	theXercesNode) const
{
	XercesNotationBridge* const		theBridge =
		new XercesNotationBridge(theXercesNode,
								 m_navigator);

	m_nodes.insert(theBridge);

	// Add it to the node map...
	m_nodeMap.addAssociation(theXercesNode, theBridge, false);

	return theBridge;
}



void
XercesDocumentBridge::destroyNode(XalanNode*	theNode)
{
	const NodeSetType::iterator 	i =
		m_nodes.find(theNode);

	if (i == m_nodes.end())
	{
		// Doesn't belong to this doc, so throw...
		throw XercesDOMException(XercesDOMException::WRONG_DOCUMENT_ERR);
	}
	else
	{
		// Delete the node...
		delete *i;

		// Erase it from the map...
		m_nodes.erase(i);
	}
}



XalanNode*
XercesDocumentBridge::internalCloneNode(
			const XalanNode*	theXalanNode,
			const DOM_Node&		theXercesNode,
			bool				deep)
{
	XalanNode*	theNewNode = 0;

	if (theXalanNode->getOwnerDocument() != this)
	{
		throw XercesDOMException(XercesDOMException::WRONG_DOCUMENT_ERR);
	}

	try
	{
		const DOM_Node	theNewXercesNode = 
			theXercesNode.cloneNode(deep);

		theNewNode = createBridgeNode(theNewXercesNode);
	}
	catch(const DOM_DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}

	return theNewNode;
}



// The rest of these are the standard DOM interfaces...

XalanDOMString
XercesDocumentBridge::getNodeName() const
{
	return m_xercesDocument.getNodeName();
}



XalanDOMString
XercesDocumentBridge::getNodeValue() const
{
	return m_xercesDocument.getNodeValue();
}



XercesDocumentBridge::NodeType
XercesDocumentBridge::getNodeType() const
{
	return DOCUMENT_NODE;
}



XalanNode*
XercesDocumentBridge::getParentNode() const
{
	return 0;
}



const XalanNodeList*
XercesDocumentBridge::getChildNodes() const
{
	return &m_children;
}



XalanNode*
XercesDocumentBridge::getFirstChild() const
{
	return m_navigator.getFirstChild(m_xercesDocument);
}



XalanNode*
XercesDocumentBridge::getLastChild() const
{
	return m_navigator.getLastChild(m_xercesDocument);
}



XalanNode*
XercesDocumentBridge::getPreviousSibling() const
{
	return 0;
}



XalanNode*
XercesDocumentBridge::getNextSibling() const
{
	return 0;
}



const XalanNamedNodeMap*
XercesDocumentBridge::getAttributes() const
{
	return 0;
}



XalanDocument*
XercesDocumentBridge::getOwnerDocument() const
{
	return 0;
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XercesDocumentBridge*
#endif
XercesDocumentBridge::cloneNode(bool	deep) const
{
	XercesDocumentBridge*	theBridge = 0;

	try
	{
		const DOM_Node	theNewDocument = m_xercesDocument.cloneNode(deep);

		theBridge =
			new XercesDocumentBridge(static_cast<const DOM_Document&>(theNewDocument));
	}
	catch(const DOM_DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}

	return theBridge;
}



XalanNode*
XercesDocumentBridge::insertBefore(
			XalanNode*	newChild,
			XalanNode*	refChild)
{
	return m_navigator.insertBefore(m_xercesDocument, newChild, refChild);
}



XalanNode*
XercesDocumentBridge::replaceChild(
			XalanNode*	newChild,
			XalanNode*	oldChild)
{
	return m_navigator.replaceChild(m_xercesDocument, newChild, oldChild);
}



XalanNode*
XercesDocumentBridge::removeChild(XalanNode*	 oldChild)
{
	return m_navigator.removeChild(m_xercesDocument, oldChild);
}



XalanNode*
XercesDocumentBridge::appendChild(XalanNode*	newChild)
{
	return m_navigator.appendChild(m_xercesDocument, newChild);
}



bool
XercesDocumentBridge::hasChildNodes() const
{
	return m_xercesDocument.hasChildNodes();
}



void
XercesDocumentBridge::setNodeValue(const XalanDOMString&	nodeValue)
{
	try
	{
		m_xercesDocument.setNodeValue(nodeValue);
	}
	catch(const DOM_DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}
}



void
XercesDocumentBridge::normalize()
{
	try
	{
		m_xercesDocument.normalize();
	}
	catch(const DOM_DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}
}


bool
XercesDocumentBridge::supports(
			const XalanDOMString&	feature,
			const XalanDOMString&	version) const
{
	return m_xercesDocument.supports(feature, version);
}



XalanDOMString
XercesDocumentBridge::getNamespaceURI() const
{
	return m_xercesDocument.getNamespaceURI();
}



XalanDOMString
XercesDocumentBridge::getPrefix() const
{
	return m_xercesDocument.getPrefix();
}



XalanDOMString
XercesDocumentBridge::getLocalName() const
{
	return m_xercesDocument.getLocalName();
}



void
XercesDocumentBridge::setPrefix(const XalanDOMString&	prefix)
{
	try
	{
		m_xercesDocument.setPrefix(prefix);
	}
	catch(const DOM_DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}
}



XalanElement*
XercesDocumentBridge::createElement(const XalanDOMString&	tagName)
{
	XalanElement*	theBridgeNode = 0;

	try
	{
		const DOM_Element	theXercesNode =
			m_xercesDocument.createElement(tagName);
		assert(theXercesNode.isNull() == false);

		theBridgeNode = createBridgeNode(theXercesNode);
		assert(mapNode(theXercesNode) == theBridgeNode);
	}
	catch(const DOM_DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}

	return theBridgeNode;
}



XalanDocumentFragment*
XercesDocumentBridge::createDocumentFragment()
{
	XalanDocumentFragment*	theBridgeNode = 0;

	try
	{
		const DOM_DocumentFragment	theXercesNode =
			m_xercesDocument.createDocumentFragment();
		assert(theXercesNode.isNull() == false);

		theBridgeNode = createBridgeNode(theXercesNode);
		assert(mapNode(theXercesNode) == theBridgeNode);
	}
	catch(const DOM_DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}

	return theBridgeNode;
}



XalanText*
XercesDocumentBridge::createTextNode(const XalanDOMString&	data)
{
	XalanText*	theBridgeNode = 0;

	try
	{
		const DOM_Text	theXercesNode =
			m_xercesDocument.createTextNode(data);
		assert(theXercesNode.isNull() == false);

		theBridgeNode = createBridgeNode(theXercesNode);
		assert(mapNode(theXercesNode) == theBridgeNode);
	}
	catch(const DOM_DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}

	return theBridgeNode;
}



XalanComment*
XercesDocumentBridge::createComment(const XalanDOMString&	data)
{
	XalanComment*	theBridgeNode = 0;

	try
	{
		const DOM_Comment	theXercesNode =
			m_xercesDocument.createComment(data);
		assert(theXercesNode.isNull() == false);

		theBridgeNode = createBridgeNode(theXercesNode);
		assert(mapNode(theXercesNode) == theBridgeNode);
	}
	catch(const DOM_DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}

	return theBridgeNode;
}



XalanCDATASection*
XercesDocumentBridge::createCDATASection(const XalanDOMString&	data)
{
	XalanCDATASection*	theBridgeNode = 0;

	try
	{
		const DOM_CDATASection	theXercesNode =
			m_xercesDocument.createCDATASection(data);
		assert(theXercesNode.isNull() == false);

		theBridgeNode = createBridgeNode(theXercesNode);
		assert(mapNode(theXercesNode) == theBridgeNode);
	}
	catch(const DOM_DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}

	return theBridgeNode;
}



XalanProcessingInstruction*
XercesDocumentBridge::createProcessingInstruction(
			const XalanDOMString&	target,
			const XalanDOMString&	data)
{
	XalanProcessingInstruction*	theBridgeNode = 0;

	try
	{
		const DOM_ProcessingInstruction	theXercesNode =
			m_xercesDocument.createProcessingInstruction(target, data);
		assert(theXercesNode.isNull() == false);

		theBridgeNode = createBridgeNode(theXercesNode);
		assert(mapNode(theXercesNode) == theBridgeNode);
	}
	catch(const DOM_DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}

	return theBridgeNode;
}



XalanAttr*
XercesDocumentBridge::createAttribute(const XalanDOMString&		name)
{
	XalanAttr*	theBridgeNode = 0;

	try
	{
		const DOM_Attr	theXercesNode =
			m_xercesDocument.createAttribute(name);
		assert(theXercesNode.isNull() == false);

		theBridgeNode = createBridgeNode(theXercesNode);
		assert(mapNode(theXercesNode) == theBridgeNode);
	}
	catch(const DOM_DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}

	return theBridgeNode;
}



XalanEntityReference*
XercesDocumentBridge::createEntityReference(const XalanDOMString&	name)
{
	XalanEntityReference*	theBridgeNode = 0;

	try
	{
		const DOM_EntityReference	theXercesNode =
			m_xercesDocument.createEntityReference(name);
		assert(theXercesNode.isNull() == false);

		theBridgeNode = createBridgeNode(theXercesNode);
		assert(mapNode(theXercesNode) == theBridgeNode);
	}
	catch(const DOM_DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}

	return theBridgeNode;
}



XalanDocumentType*
XercesDocumentBridge::getDoctype() const
{
	return m_doctype;
}




XalanDOMImplementation*
XercesDocumentBridge::getImplementation() const
{
	return m_domImplementation.get();
}



XalanElement*
XercesDocumentBridge::getDocumentElement() const
{
	return mapNode(m_xercesDocument.getDocumentElement());
}



XalanNodeList*
XercesDocumentBridge::getElementsByTagName(const XalanDOMString&	tagname) const
{
	return m_cachedNodeLists.getElementsByTagName(tagname);
}



XalanNode*
XercesDocumentBridge::importNode(
			XalanNode*	importedNode,
			bool		deep)
{
	// $$$ToDo: Fix this....
	throw XercesDOMException(XercesDOMException::NO_MODIFICATION_ALLOWED_ERR);

	return 0;
}



XalanElement*
XercesDocumentBridge::createElementNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	qualifiedName)
{
	XalanElement*	theBridgeNode = 0;

	try
	{
		const DOM_Element	theXercesNode =
			m_xercesDocument.createElementNS(namespaceURI, qualifiedName);
		assert(theXercesNode.isNull() == false);

		theBridgeNode = createBridgeNode(theXercesNode);
		assert(mapNode(theXercesNode) == theBridgeNode);
	}
	catch(const DOM_DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}

	return theBridgeNode;
}



XalanAttr*
XercesDocumentBridge::createAttributeNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	qualifiedName)
{
	XalanAttr*	theBridgeNode = 0;

	try
	{
		const DOM_Attr	theXercesNode =
			m_xercesDocument.createAttributeNS(namespaceURI, qualifiedName);
		assert(theXercesNode.isNull() == false);

		theBridgeNode = createBridgeNode(theXercesNode);
		assert(mapNode(theXercesNode) == theBridgeNode);
	}
	catch(const DOM_DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}

	return theBridgeNode;
}



XalanNodeList*
XercesDocumentBridge::getElementsByTagNameNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	localName) const
{
	return m_cachedNodeLists.getElementsByTagNameNS(namespaceURI, localName);
}



XalanElement*
XercesDocumentBridge::getElementById(const XalanDOMString&	elementId) const
{
	// $$$ ToDo: This is because DOM_Document::getElementById() is not
	// const...
#if defined(XERCES_NO_MUTABLE)
	const DOM_Node	theXercesNode((DOM_Document&)m_xercesDocument).getElementById(elementId));
#else
	const DOM_Node	theXercesNode(m_xercesDocument.getElementById(elementId));
#endif

	assert(theXercesNode.isNull() == true ||
		   (mapNode(theXercesNode) != 0 &&
		    mapNode(theXercesNode)->getNodeType() == XalanNode::ELEMENT_NODE));

	return theXercesNode.isNull() == true ? 0 :
				static_cast<XalanElement*>(mapNode(theXercesNode));
}



XalanNode*
XercesDocumentBridge::createBridgeNode(const DOM_Node&	theXercesNode) const
{
	XalanNode*				theNewNode = 0;

	switch(theXercesNode.getNodeType())
	{
	case DOM_Node::ATTRIBUTE_NODE:
		{
			const DOM_Attr&		theAttrNode =
						static_cast<const DOM_Attr&>(theXercesNode);

			theNewNode = createBridgeNode(theAttrNode);
			assert(theNewNode != 0);
		}
		break;

	case DOM_Node::CDATA_SECTION_NODE:
		{
			const DOM_CDATASection&		theCDATASectionNode =
						static_cast<const DOM_CDATASection&>(theXercesNode);

			theNewNode = createBridgeNode(theCDATASectionNode);
			assert(theNewNode != 0);

		}
		break;

	case DOM_Node::COMMENT_NODE:
		{
			const DOM_Comment&	theCommentNode =
						static_cast<const DOM_Comment&>(theXercesNode);

			theNewNode = createBridgeNode(theCommentNode);
			assert(theNewNode != 0);
		}
		break;

	case DOM_Node::DOCUMENT_FRAGMENT_NODE:
		{
			const DOM_DocumentFragment&		theDocumentFragmentNode =
						static_cast<const DOM_DocumentFragment&>(theXercesNode);

			theNewNode = createBridgeNode(theDocumentFragmentNode);
			assert(theNewNode != 0);

		}
		break;

	case DOM_Node::ELEMENT_NODE:
		{
			const DOM_Element&	theElementNode =
						static_cast<const DOM_Element&>(theXercesNode);

			theNewNode = createBridgeNode(theElementNode);
			assert(theNewNode != 0);
		}
		break;

	case DOM_Node::ENTITY_NODE:
		{
			const DOM_Entity&	theEntityNode =
						static_cast<const DOM_Entity&>(theXercesNode);

			theNewNode = createBridgeNode(theEntityNode);
			assert(theNewNode != 0);
		}
		break;

	case DOM_Node::ENTITY_REFERENCE_NODE:
		{
			const DOM_EntityReference&	theEntityReferenceNode =
						static_cast<const DOM_EntityReference&>(theXercesNode);

			theNewNode = createBridgeNode(theEntityReferenceNode);
			assert(theNewNode != 0);
		}
		break;

	case DOM_Node::NOTATION_NODE:
		{
			const DOM_Notation&		theNotationNode =
						static_cast<const DOM_Notation&>(theXercesNode);

			theNewNode = createBridgeNode(theNotationNode);
			assert(theNewNode != 0);
		}
		break;

	case DOM_Node::PROCESSING_INSTRUCTION_NODE:
		{
			const DOM_ProcessingInstruction&	thePINode =
						static_cast<const DOM_ProcessingInstruction&>(theXercesNode);

			theNewNode = createBridgeNode(thePINode);
			assert(theNewNode != 0);
		}
		break;

	case DOM_Node::TEXT_NODE:
		{
			const DOM_Text&		theTextNode =
						static_cast<const DOM_Text&>(theXercesNode);

			theNewNode = createBridgeNode(theTextNode);
			assert(theNewNode != 0);
		}
		break;

	default:
		assert(false);
		break;
	}

	return theNewNode;
}
