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
#include "XercesDocumentWrapper.hpp"



#include <algorithm>
#include <cassert>



#include <xercesc/dom/DOMAttr.hpp>
#include <xercesc/dom/DOMCDATASection.hpp>
#include <xercesc/dom/DOMComment.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMDocumentType.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMEntity.hpp>
#include <xercesc/dom/DOMEntityReference.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/dom/DOMNotation.hpp>
#include <xercesc/dom/DOMProcessingInstruction.hpp>
#include <xercesc/dom/DOMText.hpp>



#include <xalanc/Include/STLHelper.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include <xalanc/XalanDOM/XalanDocumentType.hpp>
#include <xalanc/XalanDOM/XalanElement.hpp>



#include "XercesAttrWrapper.hpp"
#include "XercesWrapperHelper.hpp"
#include "XercesCommentWrapper.hpp"
#include "XercesCDATASectionWrapper.hpp"
#include "XercesDOMWrapperException.hpp"
#include "XercesDOMImplementationWrapper.hpp"
#include "XercesDocumentTypeWrapper.hpp"
#include "XercesElementWrapper.hpp"
#include "XercesEntityWrapper.hpp"
#include "XercesEntityReferenceWrapper.hpp"
#include "XercesLiaisonXalanDOMStringPool.hpp"
#include "XercesNodeListWrapper.hpp"
#include "XercesNotationWrapper.hpp"
#include "XercesProcessingInstructionWrapper.hpp"
#include "XercesTextWrapper.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XercesDocumentWrapper::XercesDocumentWrapper(
			const DOMDocument_Type*		theXercesDocument,
			bool						threadSafe,
			bool						buildWrapper,
 			bool						buildMaps) :
	XalanDocument(),
	m_xercesDocument(theXercesDocument),
	m_documentElement(0),
	m_nodeMap(),
	m_domImplementation(new XercesDOMImplementationWrapper(theXercesDocument->getImplementation())),
	m_navigators(1, XercesWrapperNavigator(this)),
	m_navigator(&m_navigators.front()),
	m_children(theXercesDocument->getChildNodes(),
			   *m_navigator),
	m_nodes(),
	m_doctype(0),
	m_mappingMode(threadSafe == true ? false : !buildWrapper),
	m_indexValid(false),
	m_buildMaps(m_mappingMode == true ? true : buildMaps),
	m_elementAllocator(25),
	m_textAllocator(25),
	m_attributeAllocator(25),
	m_stringPool(threadSafe == true ? new XercesLiaisonXalanDOMStringPool : new XalanDOMStringPool)
{
	assert(theXercesDocument != 0);

	if (m_mappingMode == false)
	{
		// The document index is always 1...
		m_navigator->setIndex(1);

		// OK, let's build the nodes.  This makes things
		// thread-safe, so the document can be shared...
		buildWrapperNodes();
	}
	else
	{
		m_nodeMap.addAssociation(theXercesDocument, this);
	}
}



XercesDocumentWrapper::~XercesDocumentWrapper()
{
	destroyWrapper();
}



XalanNode*
XercesDocumentWrapper::mapNode(const DOMNodeType*	theXercesNode) const
{
	XalanNode*	theXalanNode = 0;

	if (theXercesNode != 0)
	{
		theXalanNode = m_nodeMap.getNode(theXercesNode);

		if (theXalanNode == 0)
		{
			if (theXercesNode != m_xercesDocument &&
					theXercesNode->getOwnerDocument() != m_xercesDocument &&
#if defined(XALAN_OLD_STYLE_CASTS)
				theXercesNode->getParentNode() != (const DOMNodeType*)m_xercesDocument)
#else
				theXercesNode->getParentNode() != static_cast<const DOMNodeType*>(m_xercesDocument))
#endif
			{
				throw XercesDOMWrapperException(XercesDOMWrapperException::WRONG_DOCUMENT_ERR);
			}
			else if (m_mappingMode == true)
			{
				// OK, we haven't yet created a wrapper, so go ahead and
				// create one.
				theXalanNode = createWrapperNode(theXercesNode, 0, true);
				assert(theXalanNode != 0);
			}
		}
	}

	return theXalanNode;
}



const DOMNodeType*
XercesDocumentWrapper::mapNode(XalanNode*	theXalanNode) const
{
#if defined(XALAN_OLD_STYLE_CASTS)
	if ((const XalanNode*)this == theXalanNode)
#else
	if (static_cast<const XalanNode*>(this) == theXalanNode)
#endif
	{
		return m_xercesDocument;
	}
	else if (theXalanNode == 0 ||
#if defined(XALAN_OLD_STYLE_CASTS)
		(const XalanDocument*)this != theXalanNode->getOwnerDocument())
#else
		static_cast<const XalanDocument*>(this) != theXalanNode->getOwnerDocument())
#endif
	{
		throw XercesDOMWrapperException(XercesDOMWrapperException::WRONG_DOCUMENT_ERR);
	}
	else
	{
		switch(theXalanNode->getNodeType())
		{
		case XalanNode::ATTRIBUTE_NODE:
#if defined(XALAN_OLD_STYLE_CASTS)
			return ((const XercesAttrWrapper*)theXalanNode)->getXercesNode();
#else
			return static_cast<const XercesAttrWrapper*>(theXalanNode)->getXercesNode();
#endif
			break;

		case XalanNode::CDATA_SECTION_NODE:
#if defined(XALAN_OLD_STYLE_CASTS)
			return ((const XercesCDATASectionWrapper*)theXalanNode)->getXercesNode();
#else
			return static_cast<const XercesCDATASectionWrapper*>(theXalanNode)->getXercesNode();
#endif
			break;

		case XalanNode::COMMENT_NODE:
#if defined(XALAN_OLD_STYLE_CASTS)
			return ((const XercesCommentWrapper*)theXalanNode)->getXercesNode();
#else
			return static_cast<const XercesCommentWrapper*>(theXalanNode)->getXercesNode();
#endif
			break;

		case XalanNode::DOCUMENT_FRAGMENT_NODE:
			throw XercesDOMWrapperException(XercesDOMWrapperException::NOT_SUPPORTED_ERR);
			break;

		case XalanNode::ELEMENT_NODE:
#if defined(XALAN_OLD_STYLE_CASTS)
			return ((const XercesElementWrapper*)theXalanNode)->getXercesNode();
#else
			return static_cast<const XercesElementWrapper*>(theXalanNode)->getXercesNode();
#endif
			break;

		case XalanNode::ENTITY_NODE:
#if defined(XALAN_OLD_STYLE_CASTS)
			return ((const XercesEntityWrapper*)theXalanNode)->getXercesNode();
#else
			return static_cast<const XercesEntityWrapper*>(theXalanNode)->getXercesNode();
#endif
			break;

		case XalanNode::ENTITY_REFERENCE_NODE:
#if defined(XALAN_OLD_STYLE_CASTS)
			return ((const XercesEntityReferenceWrapper*)theXalanNode)->getXercesNode();
#else
			return static_cast<const XercesEntityReferenceWrapper*>(theXalanNode)->getXercesNode();
#endif
			break;

		case XalanNode::NOTATION_NODE:
#if defined(XALAN_OLD_STYLE_CASTS)
			return ((const XercesNotationWrapper*)theXalanNode)->getXercesNode();
#else
			return static_cast<const XercesNotationWrapper*>(theXalanNode)->getXercesNode();
#endif
			break;

		case XalanNode::PROCESSING_INSTRUCTION_NODE:
#if defined(XALAN_OLD_STYLE_CASTS)
			return ((const XercesProcessingInstructionWrapper*)theXalanNode)->getXercesNode();
#else
			return static_cast<const XercesProcessingInstructionWrapper*>(theXalanNode)->getXercesNode();
#endif
			break;

		case XalanNode::TEXT_NODE:
#if defined(XALAN_OLD_STYLE_CASTS)
			return ((const XercesTextWrapper*)theXalanNode)->getXercesNode();
#else
			return static_cast<const XercesTextWrapper*>(theXalanNode)->getXercesNode();
#endif
			break;

		case XalanNode::DOCUMENT_TYPE_NODE:
#if defined(XALAN_OLD_STYLE_CASTS)
			return ((const XercesDocumentTypeWrapper*)theXalanNode)->getXercesNode();
#else
			return static_cast<const XercesDocumentTypeWrapper*>(theXalanNode)->getXercesNode();
#endif
			break;

		default:
			assert(false);
			break;
		}

		return 0;
	}
}



XalanAttr*
XercesDocumentWrapper::mapNode(const DOMAttrType* 	theXercesNode) const
{
#if defined(XALAN_OLD_STYLE_CASTS)
	return (XalanAttr*)mapNode((const DOMNodeType*)theXercesNode);
#else
	return static_cast<XalanAttr*>(mapNode(static_cast<const DOMNodeType*>(theXercesNode)));
#endif
}



XalanElement*
XercesDocumentWrapper::mapNode(const DOMElementType* 	theXercesNode) const
{
#if defined(XALAN_OLD_STYLE_CASTS)
	return (XercesElementWrapper*)mapNode((const DOMNodeType*)theXercesNode);
#else
	return static_cast<XercesElementWrapper*>(mapNode(static_cast<const DOMNodeType*>(theXercesNode)));
#endif
}



void
XercesDocumentWrapper::destroyWrapper()
{
	XALAN_USING_STD(for_each)

	// Set this to null, since it will be deleted
	// by the next for_each...
	m_doctype = 0;

	// m_bridgeMap contains all of the nodes that
	// are still alive...
	for_each(
			m_nodes.begin(),
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
XercesDocumentWrapper::rebuildWrapper()
{
	destroyWrapper();

	buildWrapperNodes();
}



XercesWrapperNavigator&
XercesDocumentWrapper::pushNavigator() const
{
	XercesDocumentWrapper* const		This =
#if defined(XALAN_OLD_STYLE_CASTS)
		(XercesDocumentWrapper*)this;
#else
		const_cast<XercesDocumentWrapper*>(this);
#endif

	This->m_navigators.push_back(XercesWrapperNavigator(This));

	return This->m_navigators.back();
}



XercesDocumentTypeWrapper*
XercesDocumentWrapper::createWrapperNode(
			const DOMDocumentType_Type*		theDoctype,
			IndexType						theIndex,
			bool							mapNode) const
{
	// This is a special case, since there is only one
	// doctype node allowed...
	assert(m_doctype == 0);
	assert(m_xercesDocument->getDoctype() != 0 &&
		   m_xercesDocument->getDoctype() == theDoctype);

	// Create a navigator...
	XercesWrapperNavigator&	theNavigator = pushNavigator();

	theNavigator.setIndex(theIndex);

#if defined(XALAN_NO_MUTABLE)
	XercesDocumentWrapper* const		This =
		(XercesDocumentWrapper*)this;
#else
	const XercesDocumentWrapper* const	This =
		this;
#endif

	// Update the member variable for the new instance.
	This->m_doctype = new XercesDocumentTypeWrapper(theDoctype, theNavigator);

	if (mapNode == true)
	{
		This->m_nodeMap.addAssociation(theDoctype, m_doctype);
	}

	This->m_nodes.push_back(m_doctype);

	return m_doctype;
}



XercesElementWrapper*
XercesDocumentWrapper::createWrapperNode(
			const DOMElementType* 	theXercesNode,
			IndexType				theIndex,
			bool					mapNode) const
{
	// Create a navigator...
	XercesWrapperNavigator&	theNavigator = pushNavigator();

	theNavigator.setIndex(theIndex);

#if defined(XALAN_NO_MUTABLE)
	XercesDocumentWrapper* const		This =
		(XercesDocumentWrapper*)this;
#else
	const XercesDocumentWrapper* const	This =
		this;
#endif

	XercesElementWrapper* const	theWrapper =
		This->m_elementAllocator.create(theXercesNode,
										theNavigator);

	if (m_doctype != 0 || mapNode == true)
	{
		// Add it to the node map, since we my need it for getting
		// elements by ID, or we're in mapping mode.
		This->m_nodeMap.addAssociation(theXercesNode, theWrapper);
	}

	return theWrapper;
}



XercesTextWrapper*
XercesDocumentWrapper::createWrapperNode(
			const DOMTextType*	theXercesNode,
			IndexType			theIndex,
			bool				mapNode) const
{
	// Create a navigator...
	XercesWrapperNavigator&	theNavigator = pushNavigator();

	theNavigator.setIndex(theIndex);

#if defined(XALAN_NO_MUTABLE)
	XercesDocumentWrapper* const		This =
		(XercesDocumentWrapper*)this;
#else
	const XercesDocumentWrapper* const	This =
		this;
#endif

	XercesTextWrapper* const		theWrapper =
		This->m_textAllocator.create(theXercesNode,
									 theNavigator);

	if (mapNode == true)
	{
		// Add it to the node map...
		This->m_nodeMap.addAssociation(theXercesNode, theWrapper);
	}

	return theWrapper;
}



XercesCommentWrapper*
XercesDocumentWrapper::createWrapperNode(
			const DOMCommentType*	theXercesNode,
			IndexType				theIndex,
			bool					mapNode) const
{
	// Create a navigator...
	XercesWrapperNavigator&	theNavigator = pushNavigator();

	theNavigator.setIndex(theIndex);

	XercesCommentWrapper* const	theWrapper =
		new XercesCommentWrapper(theXercesNode,
								theNavigator);

#if defined(XALAN_NO_MUTABLE)
	XercesDocumentWrapper* const		This =
		(XercesDocumentWrapper*)this;
#else
	const XercesDocumentWrapper* const	This =
		this;
#endif

	This->m_nodes.push_back(theWrapper);

	if (mapNode == true)
	{
		// Add it to the node map...
		This->m_nodeMap.addAssociation(theXercesNode, theWrapper);
	}

	return theWrapper;
}



XercesCDATASectionWrapper*
XercesDocumentWrapper::createWrapperNode(
			const DOMCDATASectionType*	theXercesNode,
			IndexType					theIndex,
			bool						mapNode) const
{
	// Create a navigator...
	XercesWrapperNavigator&	theNavigator = pushNavigator();

	theNavigator.setIndex(theIndex);

	XercesCDATASectionWrapper* const		theWrapper =
		new XercesCDATASectionWrapper(theXercesNode,
									 theNavigator);

#if defined(XALAN_NO_MUTABLE)
	XercesDocumentWrapper* const		This =
		(XercesDocumentWrapper*)this;
#else
	const XercesDocumentWrapper* const	This =
		this;
#endif

	This->m_nodes.push_back(theWrapper);

	if (mapNode == true)
	{
		// Add it to the node map...
		This->m_nodeMap.addAssociation(theXercesNode, theWrapper);
	}

	return theWrapper;
}



XercesProcessingInstructionWrapper*
XercesDocumentWrapper::createWrapperNode(
			const DOMProcessingInstructionType*		theXercesNode,
			IndexType								theIndex,
			bool									mapNode) const
{
	// Create a navigator...
	XercesWrapperNavigator&	theNavigator = pushNavigator();

	theNavigator.setIndex(theIndex);

	XercesProcessingInstructionWrapper* const	theWrapper =
		new XercesProcessingInstructionWrapper(theXercesNode,
											  theNavigator);

#if defined(XALAN_NO_MUTABLE)
	XercesDocumentWrapper* const		This =
		(XercesDocumentWrapper*)this;
#else
	const XercesDocumentWrapper* const	This =
		this;
#endif

	This->m_nodes.push_back(theWrapper);

	if (mapNode == true)
	{
		// Add it to the node map...
		This->m_nodeMap.addAssociation(theXercesNode, theWrapper);
	}

	return theWrapper;
}



XercesAttrWrapper*
XercesDocumentWrapper::createWrapperNode(
			const DOMAttrType*	theXercesNode,
			IndexType			theIndex,
			bool				/* mapNode */) const
{
	// Create a navigator...
	XercesWrapperNavigator&	theNavigator = pushNavigator();

	theNavigator.setIndex(theIndex);

#if defined(XALAN_NO_MUTABLE)
	XercesDocumentWrapper* const		This =
		(XercesDocumentWrapper*)this;
#else
	const XercesDocumentWrapper* const	This =
		this;
#endif

	XercesAttrWrapper* const		theWrapper =
		This->m_attributeAllocator.create(theXercesNode,
										  theNavigator);

	// Add it to the node map -- attributes ALWAYS go in the map
	// for now...
	This->m_nodeMap.addAssociation(theXercesNode, theWrapper);

	return theWrapper;
}



XercesEntityWrapper*
XercesDocumentWrapper::createWrapperNode(
			const DOMEntityType*	theXercesNode,
			IndexType				theIndex,
			bool					/* mapNode */) const
{
	// Create a navigator...
	XercesWrapperNavigator&	theNavigator = pushNavigator();

	theNavigator.setIndex(theIndex);

	XercesEntityWrapper* const	theWrapper =
		new XercesEntityWrapper(theXercesNode,
							   theNavigator);

#if defined(XALAN_NO_MUTABLE)
	XercesDocumentWrapper* const		This =
		(XercesDocumentWrapper*)this;
#else
	const XercesDocumentWrapper* const	This =
		this;
#endif

	This->m_nodes.push_back(theWrapper);

	// Add it to the node map -- entities ALWAYS go in the map
	// for now...
	This->m_nodeMap.addAssociation(theXercesNode, theWrapper);

	return theWrapper;
}



XercesEntityReferenceWrapper*
XercesDocumentWrapper::createWrapperNode(
			const DOMEntityReferenceType* 	theXercesNode,
			IndexType						theIndex,
			bool							mapNode) const
{
	// Create a navigator...
	XercesWrapperNavigator&	theNavigator = pushNavigator();

	theNavigator.setIndex(theIndex);

	XercesEntityReferenceWrapper* const	theWrapper =
		new XercesEntityReferenceWrapper(theXercesNode,
										theNavigator);

#if defined(XALAN_NO_MUTABLE)
	XercesDocumentWrapper* const		This =
		(XercesDocumentWrapper*)this;
#else
	const XercesDocumentWrapper* const	This =
		this;
#endif

	This->m_nodes.push_back(theWrapper);

	if (mapNode == true)
	{
		// Add it to the node map...
		This->m_nodeMap.addAssociation(theXercesNode, theWrapper);
	}

	return theWrapper;
}



XercesNotationWrapper*
XercesDocumentWrapper::createWrapperNode(
			const DOMNotationType*	theXercesNode,
			IndexType				theIndex,
			bool					/* mapNode */) const
{
	// Create a navigator...
	XercesWrapperNavigator&	theNavigator = pushNavigator();

	theNavigator.setIndex(theIndex);

	XercesNotationWrapper* const		theWrapper =
		new XercesNotationWrapper(theXercesNode,
								 theNavigator);

#if defined(XALAN_NO_MUTABLE)
	XercesDocumentWrapper* const		This =
		(XercesDocumentWrapper*)this;
#else
	const XercesDocumentWrapper* const	This =
		this;
#endif

	This->m_nodes.push_back(theWrapper);

	// Add it to the node map -- notations ALWAYS go in the map
	// for now...
	This->m_nodeMap.addAssociation(theXercesNode, theWrapper);

	return theWrapper;
}



XalanNode*
XercesDocumentWrapper::createWrapperNode(
			const DOMNodeType*	theXercesNode,
			IndexType			theIndex,
			bool				mapNode) const
{
	assert(theXercesNode != 0);

	XalanNode*				theNewNode = 0;

	switch(theXercesNode->getNodeType())
	{
	case DOMNodeType::ATTRIBUTE_NODE:
		{
			const DOMAttrType*		theAttrNode =
#if defined(XALAN_OLD_STYLE_CASTS)
						(const DOMAttrType*)theXercesNode;
#else
						static_cast<const DOMAttrType*>(theXercesNode);
#endif

			theNewNode = createWrapperNode(theAttrNode, theIndex, mapNode);
			assert(theNewNode != 0);
		}
		break;

	case DOMNodeType::CDATA_SECTION_NODE:
		{
			const DOMCDATASectionType*	theCDATASectionNode =
#if defined(XALAN_OLD_STYLE_CASTS)
						(const DOMCDATASectionType*)theXercesNode;
#else
						static_cast<const DOMCDATASectionType*>(theXercesNode);
#endif

			theNewNode = createWrapperNode(theCDATASectionNode, theIndex, mapNode);
			assert(theNewNode != 0);

		}
		break;

	case DOMNodeType::COMMENT_NODE:
		{
			const DOMCommentType*	theCommentNode =
#if defined(XALAN_OLD_STYLE_CASTS)
						(const DOMCommentType*)theXercesNode;
#else
						static_cast<const DOMCommentType*>(theXercesNode);
#endif

			theNewNode = createWrapperNode(theCommentNode, theIndex, mapNode);
			assert(theNewNode != 0);
		}
		break;

	case DOMNodeType::DOCUMENT_FRAGMENT_NODE:
		throw XercesDOMWrapperException(XercesDOMWrapperException::NOT_SUPPORTED_ERR);
		break;

	case DOMNodeType::ELEMENT_NODE:
		{
			const DOMElementType*	theElementNode =
#if defined(XALAN_OLD_STYLE_CASTS)
						(const DOMElementType*)theXercesNode;
#else
						static_cast<const DOMElementType*>(theXercesNode);
#endif

			theNewNode = createWrapperNode(theElementNode, theIndex, mapNode);
			assert(theNewNode != 0);
		}
		break;

	case DOMNodeType::ENTITY_NODE:
		{
			const DOMEntityType*	theEntityNode =
#if defined(XALAN_OLD_STYLE_CASTS)
						(const DOMEntityType*)theXercesNode;
#else
						static_cast<const DOMEntityType*>(theXercesNode);
#endif

			theNewNode = createWrapperNode(theEntityNode, theIndex, mapNode);
			assert(theNewNode != 0);
		}
		break;

	case DOMNodeType::ENTITY_REFERENCE_NODE:
		{
			const DOMEntityReferenceType*	theEntityReferenceNode =
#if defined(XALAN_OLD_STYLE_CASTS)
						(const DOMEntityReferenceType*)theXercesNode;
#else
						static_cast<const DOMEntityReferenceType*>(theXercesNode);
#endif

			theNewNode = createWrapperNode(theEntityReferenceNode, theIndex, mapNode);
			assert(theNewNode != 0);
		}
		break;

	case DOMNodeType::NOTATION_NODE:
		{
			const DOMNotationType*	theNotationNode =
#if defined(XALAN_OLD_STYLE_CASTS)
						(const DOMNotationType*)theXercesNode;
#else
						static_cast<const DOMNotationType*>(theXercesNode);
#endif

			theNewNode = createWrapperNode(theNotationNode, theIndex, mapNode);
			assert(theNewNode != 0);
		}
		break;

	case DOMNodeType::PROCESSING_INSTRUCTION_NODE:
		{
			const DOMProcessingInstructionType*		thePINode =
#if defined(XALAN_OLD_STYLE_CASTS)
						(const DOMProcessingInstructionType*)theXercesNode;
#else
						static_cast<const DOMProcessingInstructionType*>(theXercesNode);
#endif

			theNewNode = createWrapperNode(thePINode, theIndex, mapNode);
			assert(theNewNode != 0);
		}
		break;

	case DOMNodeType::TEXT_NODE:
		{
			const DOMTextType*	theTextNode =
#if defined(XALAN_OLD_STYLE_CASTS)
						(const DOMTextType*)theXercesNode;
#else
						static_cast<const DOMTextType*>(theXercesNode);
#endif

			theNewNode = createWrapperNode(theTextNode, theIndex, mapNode);
			assert(theNewNode != 0);
		}
		break;

	case DOMNodeType::DOCUMENT_TYPE_NODE:
		{
			const DOMDocumentType_Type*		theDoctypeNode =
#if defined(XALAN_OLD_STYLE_CASTS)
						(const DOMDocumentType_Type*)theXercesNode;
#else
						static_cast<const DOMDocumentType_Type*>(theXercesNode);
#endif

			theNewNode = createWrapperNode(theDoctypeNode, theIndex, mapNode);

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
XercesDocumentWrapper::destroyNode(XalanNode*	theNode)
{
	XALAN_USING_STD(find)

	const NodeVectorType::iterator 	i =
		find(m_nodes.begin(), m_nodes.end(), theNode);

	if (i == m_nodes.end())
	{
		// Doesn't belong to this doc, so throw...
		throw XercesDOMWrapperException(XercesDOMWrapperException::WRONG_DOCUMENT_ERR);
	}
	else
	{
		// Delete the node...
		delete *i;

		// Erase it from the map...
		m_nodes.erase(i);
	}
}



const XalanDOMString&
XercesDocumentWrapper::getNodeName() const
{
	assert(m_navigator != 0);

	return m_navigator->getPooledString(m_xercesDocument->getNodeName());
}



const XalanDOMString&
XercesDocumentWrapper::getNodeValue() const
{
	assert(m_navigator != 0);

	return m_navigator->getPooledString(m_xercesDocument->getNodeValue());
}



XercesDocumentWrapper::NodeType
XercesDocumentWrapper::getNodeType() const
{
	return DOCUMENT_NODE;
}



XalanNode*
XercesDocumentWrapper::getParentNode() const
{
	return 0;
}



const XalanNodeList*
XercesDocumentWrapper::getChildNodes() const
{
	return &m_children;
}



XalanNode*
XercesDocumentWrapper::getFirstChild() const
{
	assert(m_navigator != 0);

	return m_navigator->getFirstChild(m_xercesDocument);
}



XalanNode*
XercesDocumentWrapper::getLastChild() const
{
	assert(m_navigator != 0);

	return m_navigator->getLastChild(m_xercesDocument);
}



XalanNode*
XercesDocumentWrapper::getPreviousSibling() const
{
	return 0;
}



XalanNode*
XercesDocumentWrapper::getNextSibling() const
{
	return 0;
}



const XalanNamedNodeMap*
XercesDocumentWrapper::getAttributes() const
{
	return 0;
}



XalanDocument*
XercesDocumentWrapper::getOwnerDocument() const
{
	return 0;
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XalanNode*
#else
XercesDocumentWrapper*
#endif
XercesDocumentWrapper::cloneNode(bool	/* deep */) const
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::NOT_SUPPORTED_ERR);

	return 0;
}



XalanNode*
XercesDocumentWrapper::insertBefore(
			XalanNode*	/* newChild */,
			XalanNode*	/* refChild */)
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::NOT_SUPPORTED_ERR);

	return 0;
}



XalanNode*
XercesDocumentWrapper::replaceChild(
			XalanNode*	/* newChild */,
			XalanNode*	/* oldChild */)
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::NO_MODIFICATION_ALLOWED_ERR);

	return 0;
}



XalanNode*
XercesDocumentWrapper::removeChild(XalanNode*	/* oldChild */)
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::NO_MODIFICATION_ALLOWED_ERR);

	return 0;
}



XalanNode*
XercesDocumentWrapper::appendChild(XalanNode*	/* newChild */)
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::NO_MODIFICATION_ALLOWED_ERR);

	return 0;
}



bool
XercesDocumentWrapper::hasChildNodes() const
{
	return m_xercesDocument->hasChildNodes();
}



void
XercesDocumentWrapper::setNodeValue(const XalanDOMString&	/* nodeValue */)
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::NO_MODIFICATION_ALLOWED_ERR);
}



void
XercesDocumentWrapper::normalize()
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::NO_MODIFICATION_ALLOWED_ERR);
}


bool
XercesDocumentWrapper::isSupported(
			const XalanDOMString&	feature,
			const XalanDOMString&	version) const
{
	return XercesWrapperHelper::isSupported(m_xercesDocument, feature, version);
}



const XalanDOMString&
XercesDocumentWrapper::getNamespaceURI() const
{
	assert(m_navigator != 0);

	return m_navigator->getPooledString(m_xercesDocument->getNamespaceURI());
}



const XalanDOMString&
XercesDocumentWrapper::getPrefix() const
{
	assert(m_navigator != 0);

	return m_navigator->getPooledString(m_xercesDocument->getPrefix());
}



const XalanDOMString&
XercesDocumentWrapper::getLocalName() const
{
	assert(m_navigator != 0);

	return m_navigator->getPooledString(m_xercesDocument->getLocalName());
}



void
XercesDocumentWrapper::setPrefix(const XalanDOMString&	/* prefix */)
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::NO_MODIFICATION_ALLOWED_ERR);
}



bool
XercesDocumentWrapper::isIndexed() const
{
	return m_indexValid;
}



XercesDocumentWrapper::IndexType
XercesDocumentWrapper::getIndex() const
{
	assert(m_navigator != 0);
	assert(m_navigator->getIndex() == 1);

	return m_navigator->getIndex();
}



XalanElement*
XercesDocumentWrapper::createElement(const XalanDOMString&	/* tagName */)
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::NOT_SUPPORTED_ERR);

	return 0;
}



XalanDocumentFragment*
XercesDocumentWrapper::createDocumentFragment()
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::NOT_SUPPORTED_ERR);

	return 0;
}



XalanText*
XercesDocumentWrapper::createTextNode(const XalanDOMString&	/* data */)
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::NOT_SUPPORTED_ERR);

	return 0;
}



XalanComment*
XercesDocumentWrapper::createComment(const XalanDOMString&	/* data */)
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::NOT_SUPPORTED_ERR);

	return 0;
}



XalanCDATASection*
XercesDocumentWrapper::createCDATASection(const XalanDOMString&		/* data */)
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::NOT_SUPPORTED_ERR);

	return 0;
}



XalanProcessingInstruction*
XercesDocumentWrapper::createProcessingInstruction(
			const XalanDOMString&	/* target */,
			const XalanDOMString&	/* data */)
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::NOT_SUPPORTED_ERR);

	return 0;
}



XalanAttr*
XercesDocumentWrapper::createAttribute(const XalanDOMString&	/* name */)
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::NOT_SUPPORTED_ERR);

	return 0;
}



XalanEntityReference*
XercesDocumentWrapper::createEntityReference(const XalanDOMString&	/* name */)
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::NOT_SUPPORTED_ERR);

	return 0;
}



XalanDocumentType*
XercesDocumentWrapper::getDoctype() const
{
	return m_doctype;
}




XalanDOMImplementation*
XercesDocumentWrapper::getImplementation() const
{
	return m_domImplementation.get();
}



XalanElement*
XercesDocumentWrapper::getDocumentElement() const
{
	// If we're working in mapping mode, m_documentElement will
	// be null, so we'll have to map the node...
	return m_documentElement != 0 ? m_documentElement : mapNode(m_xercesDocument->getDocumentElement());
}



XalanNodeList*
XercesDocumentWrapper::getElementsByTagName(const XalanDOMString&	/* tagname */) const
{
	// Not supported...
	throw XercesDOMWrapperException(XercesDOMWrapperException::NOT_SUPPORTED_ERR);

	// Dummy return value...
	return 0;
}



XalanNode*
XercesDocumentWrapper::importNode(
			XalanNode*	/* importedNode */,
			bool		/* deep */)
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::NO_MODIFICATION_ALLOWED_ERR);

	// Dummy return value...
	return 0;
}



XalanElement*
XercesDocumentWrapper::createElementNS(
			const XalanDOMString&	/* namespaceURI */,
			const XalanDOMString&	/* qualifiedName */)
{
	// Not supported...
	throw XercesDOMWrapperException(XercesDOMWrapperException::NOT_SUPPORTED_ERR);

	// Dummy return value...
	return 0;
}



XalanAttr*
XercesDocumentWrapper::createAttributeNS(
			const XalanDOMString&	/* namespaceURI */,
			const XalanDOMString&	/* qualifiedName */)
{
	// Not supported...
	throw XercesDOMWrapperException(XercesDOMWrapperException::NOT_SUPPORTED_ERR);

	// Dummy return value...
	return 0;
}



XalanNodeList*
XercesDocumentWrapper::getElementsByTagNameNS(
			const XalanDOMString&	/* namespaceURI */,
			const XalanDOMString&	/* localName */) const
{
	throw XercesDOMWrapperException(XercesDOMWrapperException::NOT_SUPPORTED_ERR);

	// Dummy return value...
	return 0;
}



XalanElement*
XercesDocumentWrapper::getElementById(const XalanDOMString&		elementId) const
{
	const DOMNodeType* const	theXercesNode = m_xercesDocument->getElementById(c_wstr(elementId));

	if (theXercesNode == 0)
	{
		return 0;
	}
	else
	{
#if defined(XALAN_OLD_STYLE_CASTS)
		return (XalanElement*)mapNode(theXercesNode);
#else
		return static_cast<XalanElement*>(mapNode(theXercesNode));
#endif
	}
}



void
XercesDocumentWrapper::buildWrapperNodes()
{
	const DOMNodeType*	theStartChild = m_xercesDocument->getFirstChild();

	if (theStartChild != 0)
	{
		assert(m_navigator != 0);
		assert(m_navigators.size() == 1);

		m_navigator->setIndex(1);
		m_navigator->setFirstChild(0);

		BuildWrapperTreeWalker	theTreeWalker(
				this,
				&m_navigators.back(),
				m_navigators,
				2,
				m_buildMaps);

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



XercesDocumentWrapper::BuildWrapperTreeWalker::BuildWrapperTreeWalker(
			XercesDocumentWrapper*			theDocument,
			XercesWrapperNavigator*			theDocumentNavigator,
			WrapperNavigatorVectorType&		theNavigators,
			IndexType						theStartIndex,
			bool							theBuildMapsFlag) :
	m_document(theDocument),
	m_navigators(theNavigators),
	m_currentIndex(theStartIndex),
	m_parentNavigatorStack(),
	m_siblingNavigatorStack(),
	m_buildMaps(theBuildMapsFlag)
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



XercesDocumentWrapper::BuildWrapperTreeWalker::~BuildWrapperTreeWalker()
{
}



bool
XercesDocumentWrapper::BuildWrapperTreeWalker::startNode(const DOMNodeType*		node)
{
	XalanNode* const	theWrapperNode =
		m_document->createWrapperNode(node, m_currentIndex, m_buildMaps);

	XercesWrapperNavigator&	theCurrentNodeNavigator = m_navigators.back();

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

		theParentEntry.m_navigator->setFirstChild(theWrapperNode);
	}

	// Always set the last child...
	theParentEntry.m_navigator->setLastChild(theWrapperNode);

	theCurrentNodeNavigator.setPreviousSibling(theSiblingEntry.m_node);

	if (theSiblingEntry.m_navigator != 0)
	{
		theSiblingEntry.m_navigator->setNextSibling(theWrapperNode);
	}

	// Build an entry for the stacks...
	const NavigatorStackEntryType	theCurrentEntry(&theCurrentNodeNavigator, theWrapperNode);

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

	const short		theType = node->getNodeType();

	if (theType == DOMNodeType::DOCUMENT_TYPE_NODE)
	{
		// Special case for doctype -- we have to build its entities...
		const DOMDocumentType_Type* const	theDoctype =
#if defined(XALAN_OLD_STYLE_CASTS)
			(const DOMDocumentType_Type*)node;
#else
			static_cast<const DOMDocumentType_Type*>(node);
#endif

		const DOMNamedNodeMapType* const	theEntities =
			theDoctype->getEntities();

		const unsigned int	theLength =
			theEntities->getLength();

		for (unsigned int i = 0; i < theLength; ++i)
		{
			// Build it, but don't index it...
			m_document->createWrapperNode(theEntities->item(i), m_currentIndex++, true);
		}
	}
	else if (theType == DOMNodeType::ELEMENT_NODE)
	{
	// Special case for element nodes -- we have to build the attributes...
		const DOMElementType* const		theElement =
#if defined(XALAN_OLD_STYLE_CASTS)
			(const DOMElementType*)node;
#else
			static_cast<const DOMElementType*>(node);
#endif

		const DOMNamedNodeMapType* const	theAttributes =
			theElement->getAttributes();
		assert(theAttributes != 0);

		const unsigned int	theLength =
			theAttributes->getLength();

		XercesWrapperNavigator*	thePreviousAttrNavigator = 0;
		XalanNode*				thePreviousAttr = 0;

		for (unsigned int i = 0; i < theLength; ++i)
		{
			// Get the attribute from the node map...
			const DOMNodeType* const	theAttr = theAttributes->item(i);
			assert(theAttr != 0);

			// Create a wrapper node.
			XalanNode* const	theCurrentAttr =
				m_document->createWrapperNode(theAttr, m_currentIndex, m_buildMaps);
			assert(theCurrentAttr != 0);

			// Get the attribute node's navigator...
			XercesWrapperNavigator&	theCurrentAttrNavigator =
				m_navigators.back();

			// Set the parent node...
			theCurrentAttrNavigator.setParentNode(theWrapperNode);

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

	return false;
}



bool
XercesDocumentWrapper::BuildWrapperTreeWalker::endNode(const DOMNodeType*	/* node */)
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

	return false;
}



const XalanDOMString&
XercesDocumentWrapper::getPooledString(const XalanDOMString&	theString) const
{
	return m_stringPool->get(theString);
}



const XalanDOMString&
XercesDocumentWrapper::getPooledString(
			const XalanDOMChar*			theString,
			XalanDOMString::size_type	theLength) const
{
	return m_stringPool->get(theString, theLength);
}



XALAN_CPP_NAMESPACE_END
