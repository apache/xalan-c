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



#include <Include/STLHelper.hpp>



#include <DOMSupport/DOMServices.hpp>



#include <XalanDOM/XalanDocumentType.hpp>
#include <XalanDOM/XalanElement.hpp>



#include "XercesAttrBridge.hpp"
#include "XercesBridgeHelper.hpp"
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
#include "XercesLiaisonXalanDOMStringPool.hpp"
#include "XercesNodeListBridge.hpp"
#include "XercesNotationBridge.hpp"
#include "XercesProcessingInstructionBridge.hpp"
#include "XercesTextBridge.hpp"



XercesDocumentBridge::XercesDocumentBridge(
			const DOM_Document&		theXercesDocument,
			unsigned long			theNumber,
			bool					threadSafe,
			bool					buildBridge) :
	XalanDocument(),
	m_number(theNumber),
	m_xercesDocument(theXercesDocument),
	m_documentElement(0),
	m_nodeMap(),
	m_domImplementation(new XercesDOMImplementationBridge(theXercesDocument.getImplementation())),
	m_navigators(1, XercesBridgeNavigator(this, threadSafe == true ? false : !buildBridge)),
	m_navigator(&m_navigators.front()),
	m_children(theXercesDocument.getChildNodes(),
			   *m_navigator),
	m_nodes(),
	m_doctype(0),
	m_mappingMode(threadSafe == true ? false : !buildBridge),
	m_indexValid(false),
	m_elementAllocator(25),
	m_textAllocator(25),
	m_attributeAllocator(25),
	m_stringPool(threadSafe == true ? new XercesLiaisonXalanDOMStringPool : new XalanDOMStringPool)
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::make_pair;
#endif

	if (m_mappingMode == false)
	{
		// The document index is always 1...
		m_navigator->setIndex(1);

		// OK, let's build the nodes.  This makes things
		// thread-safe, so the document can be shared...
		buildBridgeNodes();
	}
	else
	{
		m_nodeMap.addAssociation(m_xercesDocument, this);
	}
}



XercesDocumentBridge::~XercesDocumentBridge()
{
	destroyBridge();
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
			XercesDOM_NodeHack	theHack(theXercesNodeImpl);

			if (theHack != m_xercesDocument &&
					theHack.getOwnerDocument() != m_xercesDocument &&
				theHack.getParentNode() != m_xercesDocument)
			{
				throw XalanDOMException(XalanDOMException::WRONG_DOCUMENT_ERR);
			}
			else if (m_mappingMode == true)
			{
				// OK, we haven't yet created a bridge not, so go ahead and
				// create one.
				theXalanNode = createBridgeNode(theHack, 0, true);
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

#if defined(XALAN_OLD_STYLE_CASTS)
	return XercesDOM_AttrHack((AttrImpl*)theXercesNodeImpl);
#else
	return XercesDOM_AttrHack(reinterpret_cast<AttrImpl*>(theXercesNodeImpl));
#endif
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
#if defined(XALAN_OLD_STYLE_CASTS)
	return (XercesAttrBridge*)mapNode(XercesDOM_NodeHack::getImpl(theXercesNode));
#else
	return static_cast<XercesAttrBridge*>(mapNode(XercesDOM_NodeHack::getImpl(theXercesNode)));
#endif
}



XalanElement*
XercesDocumentBridge::mapNode(const DOM_Element& 	theXercesNode) const
{
#if defined(XALAN_OLD_STYLE_CASTS)
	return (XercesElementBridge*)mapNode(XercesDOM_NodeHack::getImpl(theXercesNode));
#else
	return static_cast<XercesElementBridge*>(mapNode(XercesDOM_NodeHack::getImpl(theXercesNode)));
#endif
}



void
XercesDocumentBridge::destroyBridge()
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::for_each;
#endif

	// Set this to null, since it will be deleted
	// by the next for_each...
	m_doctype = 0;

	// m_bridgeMap contains all of the nodes that
	// are still alive...
	for_each(m_nodes.begin(),
			 m_nodes.end(),
			 DeleteFunctor<XalanNode>());

	// Clear everything out, since we just delete everything...
	m_nodes.clear();

	// Clear out all of the navigators, except ours...
	m_navigators.erase(m_navigators.begin() + 1, m_navigators.end());

	m_navigator = &m_navigators.front();

	// Clear the node map...
	m_nodeMap.clear();

	m_indexValid = false;

	m_mappingMode = true;

	m_stringPool->clear();
}



void
XercesDocumentBridge::rebuildBridge()
{
	destroyBridge();

	buildBridgeNodes();
}



XercesBridgeNavigator&
XercesDocumentBridge::pushNavigator(bool	mappingMode) const
{
	XercesDocumentBridge* const		This =
#if defined(XALAN_OLD_STYLE_CASTS)
		(XercesDocumentBridge*)this;
#else
		const_cast<XercesDocumentBridge*>(this);
#endif

	This->m_navigators.push_back(XercesBridgeNavigator(This, mappingMode));

	return This->m_navigators.back();
}



XercesDocumentTypeBridge*
XercesDocumentBridge::createBridgeNode(
			const DOM_DocumentType&		theDoctype,
			unsigned long				theIndex,
			bool						mapNode) const
{
	// This is a special case, since there is only one
	// doctype node allowed...
	assert(m_doctype == 0);
	assert(m_xercesDocument.getDoctype().isNull() == false &&
		   m_xercesDocument.getDoctype() == theDoctype);

	// Create a navigator...
	XercesBridgeNavigator&	theNavigator = pushNavigator(mapNode);

	theNavigator.setIndex(theIndex);

#if defined(XALAN_NO_MUTABLE)
	XercesDocumentBridge* const		This =
		(XercesDocumentBridge*)this;
#else
	const XercesDocumentBridge* const	This =
		this;
#endif

	// Update the member variable for the new instance.
	This->m_doctype = new XercesDocumentTypeBridge(theDoctype, theNavigator);

	if (mapNode == true)
	{
		This->m_nodeMap.addAssociation(theDoctype, m_doctype);
	}

	This->m_nodes.push_back(m_doctype);

	return m_doctype;
}



XercesElementBridge*
XercesDocumentBridge::createBridgeNode(
			const DOM_Element& 	theXercesNode,
			unsigned long		theIndex,
			bool				mapNode) const
{
	// Create a navigator...
	XercesBridgeNavigator&	theNavigator = pushNavigator(mapNode);

	theNavigator.setIndex(theIndex);

#if defined(XALAN_NO_MUTABLE)
	XercesDocumentBridge* const		This =
		(XercesDocumentBridge*)this;
#else
	const XercesDocumentBridge* const	This =
		this;
#endif

	XercesElementBridge* const	theBridge =
		This->m_elementAllocator.create(theXercesNode,
										theNavigator);

	if (m_doctype != 0 || mapNode == true)
	{
		// Add it to the node map, since we my need it for getting
		// elements by ID, or we're in mapping mode.
		This->m_nodeMap.addAssociation(theXercesNode, theBridge);
	}

	return theBridge;
}



XercesDocumentFragmentBridge*
XercesDocumentBridge::createBridgeNode(
			const DOM_DocumentFragment&		theXercesNode,
			unsigned long					theIndex,
			bool							mapNode) const
{
	// Create a navigator...
	XercesBridgeNavigator&	theNavigator = pushNavigator(mapNode);

	theNavigator.setIndex(theIndex);

	XercesDocumentFragmentBridge* const		theBridge =
		new XercesDocumentFragmentBridge(theXercesNode,
										 theNavigator);

#if defined(XALAN_NO_MUTABLE)
	XercesDocumentBridge* const		This =
		(XercesDocumentBridge*)this;
#else
	const XercesDocumentBridge* const	This =
		this;
#endif

	This->m_nodes.push_back(theBridge);

	if (mapNode == true)
	{
		// Add it to the node map...
		This->m_nodeMap.addAssociation(theXercesNode, theBridge);
	}

	return theBridge;
}



XercesTextBridge*
XercesDocumentBridge::createBridgeNode(
			const DOM_Text&		theXercesNode,
			unsigned long		theIndex,
			bool				mapNode) const
{
	// Create a navigator...
	XercesBridgeNavigator&	theNavigator = pushNavigator(mapNode);

	theNavigator.setIndex(theIndex);

#if defined(XALAN_NO_MUTABLE)
	XercesDocumentBridge* const		This =
		(XercesDocumentBridge*)this;
#else
	const XercesDocumentBridge* const	This =
		this;
#endif

	XercesTextBridge* const		theBridge =
		This->m_textAllocator.create(theXercesNode,
									 theNavigator);

	if (mapNode == true)
	{
		// Add it to the node map...
		This->m_nodeMap.addAssociation(theXercesNode, theBridge);
	}

	return theBridge;
}



XercesCommentBridge*
XercesDocumentBridge::createBridgeNode(
			const DOM_Comment&	theXercesNode,
			unsigned long		theIndex,
			bool				mapNode) const
{
	// Create a navigator...
	XercesBridgeNavigator&	theNavigator = pushNavigator(mapNode);

	theNavigator.setIndex(theIndex);

	XercesCommentBridge* const	theBridge =
		new XercesCommentBridge(theXercesNode,
								theNavigator);

#if defined(XALAN_NO_MUTABLE)
	XercesDocumentBridge* const		This =
		(XercesDocumentBridge*)this;
#else
	const XercesDocumentBridge* const	This =
		this;
#endif

	This->m_nodes.push_back(theBridge);

	if (mapNode == true)
	{
		// Add it to the node map...
		This->m_nodeMap.addAssociation(theXercesNode, theBridge);
	}

	return theBridge;
}



XercesCDATASectionBridge*
XercesDocumentBridge::createBridgeNode(
			const DOM_CDATASection&		theXercesNode,
			unsigned long				theIndex,
			bool						mapNode) const
{
	// Create a navigator...
	XercesBridgeNavigator&	theNavigator = pushNavigator(mapNode);

	theNavigator.setIndex(theIndex);

	XercesCDATASectionBridge* const		theBridge =
		new XercesCDATASectionBridge(theXercesNode,
									 theNavigator);

#if defined(XALAN_NO_MUTABLE)
	XercesDocumentBridge* const		This =
		(XercesDocumentBridge*)this;
#else
	const XercesDocumentBridge* const	This =
		this;
#endif

	This->m_nodes.push_back(theBridge);

	if (mapNode == true)
	{
		// Add it to the node map...
		This->m_nodeMap.addAssociation(theXercesNode, theBridge);
	}

	return theBridge;
}



XercesProcessingInstructionBridge*
XercesDocumentBridge::createBridgeNode(
			const DOM_ProcessingInstruction&	theXercesNode,
			unsigned long						theIndex,
			bool								mapNode) const
{
	// Create a navigator...
	XercesBridgeNavigator&	theNavigator = pushNavigator(mapNode);

	theNavigator.setIndex(theIndex);

	XercesProcessingInstructionBridge* const	theBridge =
		new XercesProcessingInstructionBridge(theXercesNode,
											  theNavigator);

#if defined(XALAN_NO_MUTABLE)
	XercesDocumentBridge* const		This =
		(XercesDocumentBridge*)this;
#else
	const XercesDocumentBridge* const	This =
		this;
#endif

	This->m_nodes.push_back(theBridge);

	if (mapNode == true)
	{
		// Add it to the node map...
		This->m_nodeMap.addAssociation(theXercesNode, theBridge);
	}

	return theBridge;
}



XercesAttrBridge*
XercesDocumentBridge::createBridgeNode(
			const DOM_Attr&		theXercesNode,
			unsigned long		theIndex,
			bool				mapNode) const
{
	// Create a navigator...
	XercesBridgeNavigator&	theNavigator = pushNavigator(mapNode);

	theNavigator.setIndex(theIndex);

#if defined(XALAN_NO_MUTABLE)
	XercesDocumentBridge* const		This =
		(XercesDocumentBridge*)this;
#else
	const XercesDocumentBridge* const	This =
		this;
#endif

	XercesAttrBridge* const		theBridge =
		This->m_attributeAllocator.create(theXercesNode,
										  theNavigator);

	// Add it to the node map -- attributes ALWAYS go in the map
	// for now...
	This->m_nodeMap.addAssociation(theXercesNode, theBridge);

	return theBridge;
}



XercesEntityBridge*
XercesDocumentBridge::createBridgeNode(
			const DOM_Entity&	theXercesNode,
			unsigned long		theIndex,
			bool				mapNode) const
{
	// Create a navigator...
	XercesBridgeNavigator&	theNavigator = pushNavigator(mapNode);

	theNavigator.setIndex(theIndex);

	XercesEntityBridge* const	theBridge =
		new XercesEntityBridge(theXercesNode,
							   theNavigator);

#if defined(XALAN_NO_MUTABLE)
	XercesDocumentBridge* const		This =
		(XercesDocumentBridge*)this;
#else
	const XercesDocumentBridge* const	This =
		this;
#endif

	This->m_nodes.push_back(theBridge);

	// Add it to the node map -- entities ALWAYS go in the map
	// for now...
	This->m_nodeMap.addAssociation(theXercesNode, theBridge);

	return theBridge;
}



XercesEntityReferenceBridge*
XercesDocumentBridge::createBridgeNode(
			const DOM_EntityReference& 	theXercesNode,
			unsigned long				theIndex,
			bool						mapNode) const
{
	// Create a navigator...
	XercesBridgeNavigator&	theNavigator = pushNavigator(mapNode);

	theNavigator.setIndex(theIndex);

	XercesEntityReferenceBridge* const	theBridge =
		new XercesEntityReferenceBridge(theXercesNode,
										theNavigator);

#if defined(XALAN_NO_MUTABLE)
	XercesDocumentBridge* const		This =
		(XercesDocumentBridge*)this;
#else
	const XercesDocumentBridge* const	This =
		this;
#endif

	This->m_nodes.push_back(theBridge);

	if (mapNode == true)
	{
		// Add it to the node map...
		This->m_nodeMap.addAssociation(theXercesNode, theBridge);
	}

	return theBridge;
}



XercesNotationBridge*
XercesDocumentBridge::createBridgeNode(
			const DOM_Notation&		theXercesNode,
			unsigned long			theIndex,
			bool					mapNode) const
{
	// Create a navigator...
	XercesBridgeNavigator&	theNavigator = pushNavigator(mapNode);

	theNavigator.setIndex(theIndex);

	XercesNotationBridge* const		theBridge =
		new XercesNotationBridge(theXercesNode,
								 theNavigator);

#if defined(XALAN_NO_MUTABLE)
	XercesDocumentBridge* const		This =
		(XercesDocumentBridge*)this;
#else
	const XercesDocumentBridge* const	This =
		this;
#endif

	This->m_nodes.push_back(theBridge);

	// Add it to the node map -- notations ALWAYS go in the map
	// for now...
	This->m_nodeMap.addAssociation(theXercesNode, theBridge);

	return theBridge;
}



XalanNode*
XercesDocumentBridge::createBridgeNode(
			const DOM_Node&		theXercesNode,
			unsigned long		theIndex,
			bool				mapNode) const
{
	XalanNode*				theNewNode = 0;

	switch(theXercesNode.getNodeType())
	{
	case DOM_Node::ATTRIBUTE_NODE:
		{
			const DOM_Attr&		theAttrNode =
#if defined(XALAN_OLD_STYLE_CASTS)
						(const DOM_Attr&)theXercesNode;
#else
						static_cast<const DOM_Attr&>(theXercesNode);
#endif

			theNewNode = createBridgeNode(theAttrNode, theIndex, mapNode);
			assert(theNewNode != 0);
		}
		break;

	case DOM_Node::CDATA_SECTION_NODE:
		{
			const DOM_CDATASection&		theCDATASectionNode =
#if defined(XALAN_OLD_STYLE_CASTS)
						(const DOM_CDATASection&)theXercesNode;
#else
						static_cast<const DOM_CDATASection&>(theXercesNode);
#endif

			theNewNode = createBridgeNode(theCDATASectionNode, theIndex, mapNode);
			assert(theNewNode != 0);

		}
		break;

	case DOM_Node::COMMENT_NODE:
		{
			const DOM_Comment&	theCommentNode =
#if defined(XALAN_OLD_STYLE_CASTS)
						(const DOM_Comment&)theXercesNode;
#else
						static_cast<const DOM_Comment&>(theXercesNode);
#endif

			theNewNode = createBridgeNode(theCommentNode, theIndex, mapNode);
			assert(theNewNode != 0);
		}
		break;

	case DOM_Node::DOCUMENT_FRAGMENT_NODE:
		{
			const DOM_DocumentFragment&		theDocumentFragmentNode =
#if defined(XALAN_OLD_STYLE_CASTS)
						(const DOM_DocumentFragment&)theXercesNode;
#else
						static_cast<const DOM_DocumentFragment&>(theXercesNode);
#endif

			theNewNode = createBridgeNode(theDocumentFragmentNode, theIndex, mapNode);
			assert(theNewNode != 0);

		}
		break;

	case DOM_Node::ELEMENT_NODE:
		{
			const DOM_Element&	theElementNode =
#if defined(XALAN_OLD_STYLE_CASTS)
						(const DOM_Element&)theXercesNode;
#else
						static_cast<const DOM_Element&>(theXercesNode);
#endif

			theNewNode = createBridgeNode(theElementNode, theIndex, mapNode);
			assert(theNewNode != 0);
		}
		break;

	case DOM_Node::ENTITY_NODE:
		{
			const DOM_Entity&	theEntityNode =
#if defined(XALAN_OLD_STYLE_CASTS)
						(const DOM_Entity&)theXercesNode;
#else
						static_cast<const DOM_Entity&>(theXercesNode);
#endif

			theNewNode = createBridgeNode(theEntityNode, theIndex, mapNode);
			assert(theNewNode != 0);
		}
		break;

	case DOM_Node::ENTITY_REFERENCE_NODE:
		{
			const DOM_EntityReference&	theEntityReferenceNode =
#if defined(XALAN_OLD_STYLE_CASTS)
						(const DOM_EntityReference&)theXercesNode;
#else
						static_cast<const DOM_EntityReference&>(theXercesNode);
#endif

			theNewNode = createBridgeNode(theEntityReferenceNode, theIndex, mapNode);
			assert(theNewNode != 0);
		}
		break;

	case DOM_Node::NOTATION_NODE:
		{
			const DOM_Notation&		theNotationNode =
#if defined(XALAN_OLD_STYLE_CASTS)
						(const DOM_Notation&)theXercesNode;
#else
						static_cast<const DOM_Notation&>(theXercesNode);
#endif

			theNewNode = createBridgeNode(theNotationNode, theIndex, mapNode);
			assert(theNewNode != 0);
		}
		break;

	case DOM_Node::PROCESSING_INSTRUCTION_NODE:
		{
			const DOM_ProcessingInstruction&	thePINode =
#if defined(XALAN_OLD_STYLE_CASTS)
						(const DOM_ProcessingInstruction&)theXercesNode;
#else
						static_cast<const DOM_ProcessingInstruction&>(theXercesNode);
#endif

			theNewNode = createBridgeNode(thePINode, theIndex, mapNode);
			assert(theNewNode != 0);
		}
		break;

	case DOM_Node::TEXT_NODE:
		{
			const DOM_Text&		theTextNode =
#if defined(XALAN_OLD_STYLE_CASTS)
						(const DOM_Text&)theXercesNode;
#else
						static_cast<const DOM_Text&>(theXercesNode);
#endif

			theNewNode = createBridgeNode(theTextNode, theIndex, mapNode);
			assert(theNewNode != 0);
		}
		break;

	case DOM_Node::DOCUMENT_TYPE_NODE:
		{
			const DOM_DocumentType&		theDoctypeNode =
#if defined(XALAN_OLD_STYLE_CASTS)
						(const DOM_DocumentType&)theXercesNode;
#else
						static_cast<const DOM_DocumentType&>(theXercesNode);
#endif

			theNewNode = createBridgeNode(theDoctypeNode, theIndex, mapNode);

			assert(theNewNode != 0);
		}
		break;

	default:
		assert(false);
		break;
	}

	return theNewNode;
}



void
XercesDocumentBridge::destroyNode(XalanNode*	theNode)
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::find;
#endif

	const NodeVectorType::iterator 	i =
		find(m_nodes.begin(), m_nodes.end(), theNode);

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

		theNewNode = createBridgeNode(theNewXercesNode, 0, true);
	}
	catch(const DOM_DOMException&	theException)
	{
		throw XercesDOMException(theException);
	}

	return theNewNode;
}



const XalanDOMString&
XercesDocumentBridge::getNodeName() const
{
	return getPooledString(m_xercesDocument.getNodeName().rawBuffer());
}



const XalanDOMString&
XercesDocumentBridge::getNodeValue() const
{
	return getPooledString(m_xercesDocument.getNodeValue().rawBuffer());
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
	assert(m_navigator != 0);

	return m_navigator->getFirstChild(m_xercesDocument);
}



XalanNode*
XercesDocumentBridge::getLastChild() const
{
	assert(m_navigator != 0);

	return m_navigator->getLastChild(m_xercesDocument);
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
#if defined(XALAN_OLD_STYLE_CASTS)
			new XercesDocumentBridge((const DOM_Document&)theNewDocument);
#else
			new XercesDocumentBridge(static_cast<const DOM_Document&>(theNewDocument));
#endif
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
	assert(m_navigator != 0);

	return m_navigator->insertBefore(m_xercesDocument, newChild, refChild);
}



XalanNode*
XercesDocumentBridge::replaceChild(
			XalanNode*	newChild,
			XalanNode*	oldChild)
{
	assert(m_navigator != 0);

	return m_navigator->replaceChild(m_xercesDocument, newChild, oldChild);
}



XalanNode*
XercesDocumentBridge::removeChild(XalanNode*	 oldChild)
{
	assert(m_navigator != 0);

	return m_navigator->removeChild(m_xercesDocument, oldChild);
}



XalanNode*
XercesDocumentBridge::appendChild(XalanNode*	newChild)
{
	assert(m_navigator != 0);

	return m_navigator->appendChild(m_xercesDocument, newChild);
}



bool
XercesDocumentBridge::hasChildNodes() const
{
	return m_xercesDocument.hasChildNodes();
}



void
XercesDocumentBridge::setNodeValue(const XalanDOMString&	nodeValue)
{
	XercesBridgeHelper::setNodeValue(m_xercesDocument, nodeValue);
}



void
XercesDocumentBridge::normalize()
{
	XercesBridgeHelper::normalize(m_xercesDocument);
}


bool
XercesDocumentBridge::supports(
			const XalanDOMString&	feature,
			const XalanDOMString&	version) const
{
	return m_xercesDocument.supports(
				XercesBridgeHelper::XalanDOMStringToXercesDOMString(feature),
				XercesBridgeHelper::XalanDOMStringToXercesDOMString(version));
}



const XalanDOMString&
XercesDocumentBridge::getNamespaceURI() const
{
	return getPooledString(m_xercesDocument.getNamespaceURI().rawBuffer());
}



const XalanDOMString&
XercesDocumentBridge::getPrefix() const
{
	return getPooledString(m_xercesDocument.getPrefix().rawBuffer());
}



const XalanDOMString&
XercesDocumentBridge::getLocalName() const
{
	return getPooledString(m_xercesDocument.getLocalName().rawBuffer());
}



void
XercesDocumentBridge::setPrefix(const XalanDOMString&	prefix)
{
	XercesBridgeHelper::setPrefix(m_xercesDocument, prefix);
}



bool
XercesDocumentBridge::isIndexed() const
{
	return m_indexValid;
}



unsigned long
XercesDocumentBridge::getIndex() const
{
	assert(m_navigator != 0);
	assert(m_navigator->getIndex() == 1);

	return m_navigator->getIndex();
}



XalanElement*
XercesDocumentBridge::createElement(const XalanDOMString&	tagName)
{
	XalanElement*	theBridgeNode = 0;

	try
	{
		const DOM_Element	theXercesNode =
			m_xercesDocument.createElement(c_wstr(tagName));
		assert(theXercesNode.isNull() == false);

		theBridgeNode = createBridgeNode(theXercesNode, 0, true);
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

		theBridgeNode = createBridgeNode(theXercesNode, 0, true);
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
			m_xercesDocument.createTextNode(c_wstr(data));
		assert(theXercesNode.isNull() == false);

		theBridgeNode = createBridgeNode(theXercesNode, 0, true);
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
			m_xercesDocument.createComment(c_wstr(data));
		assert(theXercesNode.isNull() == false);

		theBridgeNode = createBridgeNode(theXercesNode, 0, true);
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
			m_xercesDocument.createCDATASection(c_wstr(data));
		assert(theXercesNode.isNull() == false);

		theBridgeNode = createBridgeNode(theXercesNode, 0, true);
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
			m_xercesDocument.createProcessingInstruction(c_wstr(target), c_wstr(data));
		assert(theXercesNode.isNull() == false);

		theBridgeNode = createBridgeNode(theXercesNode, 0, true);
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
			m_xercesDocument.createAttribute(c_wstr(name));
		assert(theXercesNode.isNull() == false);

		theBridgeNode = createBridgeNode(theXercesNode, 0, true);
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
			m_xercesDocument.createEntityReference(c_wstr(name));
		assert(theXercesNode.isNull() == false);

		theBridgeNode = createBridgeNode(theXercesNode, 0, true);
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
	// If we're working in mapping mode, m_documentElement will
	// be null, so we'll have to map the node...
	return m_documentElement != 0 ? m_documentElement : mapNode(m_xercesDocument.getDocumentElement());
}



XalanNodeList*
XercesDocumentBridge::getElementsByTagName(const XalanDOMString&	/* tagname */) const
{
	// Not supported
	return 0;
}



XalanNode*
XercesDocumentBridge::importNode(
			XalanNode*	importedNode,
			bool		deep)
{
	// $$$ToDo: Fix this....
	// The problem is that we must get the Xerces node that corresponds to the
	// importedNode parameter.  We could assume that it is indeed a node from
	// another XercesDocumentBridge, but I'm not sure that we should do that.
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
			m_xercesDocument.createElementNS(c_wstr(namespaceURI), c_wstr(qualifiedName));
		assert(theXercesNode.isNull() == false);

		theBridgeNode = createBridgeNode(theXercesNode, 0, true);
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
			m_xercesDocument.createAttributeNS(c_wstr(namespaceURI), c_wstr(qualifiedName));
		assert(theXercesNode.isNull() == false);

		theBridgeNode = createBridgeNode(theXercesNode, 0, true);
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
			const XalanDOMString&	/* namespaceURI */,
			const XalanDOMString&	/* localName */) const
{
	// Not supported
	return 0;
}



XalanElement*
XercesDocumentBridge::getElementById(const XalanDOMString&	elementId) const
{
	// $$$ ToDo: This is because DOM_Document::getElementById() is not
	// const...
#if defined(XALAN_NO_MUTABLE)
	const DOM_Node	theXercesNode(((DOM_Document&)m_xercesDocument).getElementById(c_wstr(elementId)));
#else
	const DOM_Node	theXercesNode(m_xercesDocument.getElementById(c_wstr(elementId)));
#endif

	return theXercesNode.isNull() == true ? 0 :
#if defined(XALAN_OLD_STYLE_CASTS)
				(XalanElement*)mapNode(theXercesNode);
#else
				static_cast<XalanElement*>(mapNode(theXercesNode));
#endif
}



unsigned long
XercesDocumentBridge::getNumber() const
{
	return m_number;
}



void
XercesDocumentBridge::buildBridgeNodes()
{
	const DOM_Node	theStartChild = m_xercesDocument.getFirstChild();

	if (theStartChild.isNull() == false)
	{
		assert(m_navigator != 0);
		assert(m_navigators.size() == 1);

		m_navigator->setIndex(1);
		m_navigator->setFirstChild(0);

		BuildBridgeTreeWalker	theTreeWalker(
				this,
				&m_navigators.back(),
				m_navigators,
				2);

		theTreeWalker.traverse(theStartChild, m_xercesDocument);
	}

	// OK, now set m_documentElement...
	XalanNode*	theChild = m_navigator->getFirstChild();

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



XercesDocumentBridge::BuildBridgeTreeWalker::BuildBridgeTreeWalker(
			XercesDocumentBridge*		theDocument,
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



XercesDocumentBridge::BuildBridgeTreeWalker::~BuildBridgeTreeWalker()
{
}



void
XercesDocumentBridge::BuildBridgeTreeWalker::startNode(const DOM_Node&	node)
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
XercesDocumentBridge::BuildBridgeTreeWalker::endNode(const DOM_Node&	/* node */)
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



const XalanDOMString&
XercesDocumentBridge::getPooledString(const XalanDOMString&		theString) const
{
	return m_stringPool->get(theString);
}



const XalanDOMString&
XercesDocumentBridge::getPooledString(const XalanDOMChar*		theString) const
{
	return m_stringPool->get(theString);
}
