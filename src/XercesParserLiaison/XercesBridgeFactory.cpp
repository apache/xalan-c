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
#include "XercesBridgeFactory.hpp"



#include <cassert>



#include <dom/DOM_Node.hpp>
#include <dom/DOM_Document.hpp>
#include <dom/DOM_Entity.hpp>



#include "XercesAttrBridge.hpp"
#include "XercesCDATASectionBridge.hpp"
#include "XercesCommentBridge.hpp"
#include "XercesDocumentBridge.hpp"
#include "XercesElementBridge.hpp"
#include "XercesEntityBridge.hpp"
#include "XercesEntityReferenceBridge.hpp"
#include "XercesNotationBridge.hpp"
#include "XercesProcessingInstructionBridge.hpp"
#include "XercesTextBridge.hpp"
#include "XercesToXalanNodeMap.hpp"



XercesBridgeFactory::XercesBridgeFactory(
			const DOM_Document&			theXercesDocument,
			XercesDocumentBridge*		theOwnerDocument,
			XercesToXalanNodeMap*		theNodeMap) :
	m_xercesDocument(theXercesDocument),
	m_ownerDocument(theOwnerDocument),
	m_nodeMap(theNodeMap)
{
	assert(theXercesDocument.isNull() == false);
	assert(m_ownerDocument != 0);
}



XercesBridgeFactory::~XercesBridgeFactory()
{
}



XercesElementBridge*
XercesBridgeFactory::buildBridgeStructure() const
{
	XalanNode* const	theRootElement =
		buildBridgeStructure(m_xercesDocument.getDocumentElement());

	assert(theRootElement == 0 || theRootElement->getNodeType() == XalanNode::ELEMENT_NODE);

	return static_cast<XercesElementBridge*>(theRootElement);
}



XalanNode*
XercesBridgeFactory::createBridgeNode(const DOM_Node&	theXercesNode) const
{
	XalanNode*				theNewNode = 0;

	switch(theXercesNode.getNodeType())
	{
	case DOM_Node::ATTRIBUTE_NODE:
		{
			const DOM_Attr&		theAttrNode =
						static_cast<const DOM_Attr&>(theXercesNode);

			XercesAttrBridge* const		theBridgeNode =
				m_ownerDocument->createBridgeNode(theAttrNode,
												  0,
												  thePreviousSibling);
			assert(theBridgeNode != 0);

			theNavigator = theBridgeNode->getNavigator();
			assert(theNavigator != 0);

			theNewNode = theBridgeNode;
		}
		break;

	case DOM_Node::DOCUMENT_FRAGMENT_NODE:
		assert(false);
		break;

	case DOM_Node::CDATA_SECTION_NODE:
		{
			const DOM_CDATASection&		theCDATASectionNode =
						static_cast<const DOM_CDATASection&>(theXercesNode);

			XercesCDATASectionBridge* const		theBridgeNode =
				m_ownerDocument->createBridgeNode(theCDATASectionNode,
												  theParentNode,
												  thePreviousSibling);
			assert(theBridgeNode != 0);

			theNavigator = theBridgeNode->getNavigator();
			assert(theNavigator != 0);

			theNewNode = theBridgeNode;
		}
		break;

	case DOM_Node::COMMENT_NODE:
		{
			const DOM_Comment&	theCommentNode =
						static_cast<const DOM_Comment&>(theXercesNode);

			XercesCommentBridge* const		theBridgeNode =
				m_ownerDocument->createBridgeNode(theCommentNode,
												  theParentNode,
												  thePreviousSibling);
			assert(theBridgeNode != 0);

			theNavigator = theBridgeNode->getNavigator();
			assert(theNavigator != 0);

			theNewNode = theBridgeNode;
		}
		break;

	case DOM_Node::ELEMENT_NODE:
		{
			const DOM_Element&	theElementNode =
						static_cast<const DOM_Element&>(theXercesNode);

			XercesElementBridge* const	theBridgeNode =
				m_ownerDocument->createBridgeNode(theElementNode,
												  theParentNode,
												  thePreviousSibling);
			assert(theBridgeNode != 0);

			// Now, let's create a bridge node for each attribute...
			const DOM_NamedNodeMap		theXercesNamedNodeMap = theXercesNode.getAttributes();

			const unsigned int			theLength = theXercesNamedNodeMap.getLength();

			for (unsigned int i = 0; i < theLength; i++)
			{
				createBridgeNode(const_cast<DOM_NamedNodeMap&>(theXercesNamedNodeMap).item(i),
								 0,
								 0);
			}

			theNavigator = theBridgeNode->getNavigator();
			assert(theNavigator != 0);

			theNewNode = theBridgeNode;
		}
		break;

	case DOM_Node::ENTITY_NODE:
		{
			const DOM_Entity&	theEntityNode =
						static_cast<const DOM_Entity&>(theXercesNode);

			XercesEntityBridge* const	theBridgeNode =
				m_ownerDocument->createBridgeNode(theEntityNode,
												  theParentNode,
												  thePreviousSibling);
			assert(theBridgeNode != 0);

			theNavigator = theBridgeNode->getNavigator();
			assert(theNavigator != 0);

			theNewNode = theBridgeNode;
		}
		break;

	case DOM_Node::ENTITY_REFERENCE_NODE:
		{
			const DOM_EntityReference&	theEntityReferenceNode =
						static_cast<const DOM_EntityReference&>(theXercesNode);

			XercesEntityReferenceBridge* const	theBridgeNode =
				m_ownerDocument->createBridgeNode(theEntityReferenceNode,
												  theParentNode,
												  thePreviousSibling);
			assert(theBridgeNode != 0);

			theNavigator = theBridgeNode->getNavigator();
			assert(theNavigator != 0);

			theNewNode = theBridgeNode;
		}
		break;

	case DOM_Node::NOTATION_NODE:
		{
			const DOM_Notation&		theNotationNode =
						static_cast<const DOM_Notation&>(theXercesNode);

			XercesNotationBridge* const		theBridgeNode =
				m_ownerDocument->createBridgeNode(theNotationNode,
												  theParentNode,
												  thePreviousSibling);
			assert(theBridgeNode != 0);

			theNavigator = theBridgeNode->getNavigator();
			assert(theNavigator != 0);

			theNewNode = theBridgeNode;
		}
		break;

	case DOM_Node::PROCESSING_INSTRUCTION_NODE:
		{
			const DOM_ProcessingInstruction&	thePINode =
						static_cast<const DOM_ProcessingInstruction&>(theXercesNode);

			XercesProcessingInstructionBridge* const	theBridgeNode =
				m_ownerDocument->createBridgeNode(thePINode,
												  theParentNode,
												  thePreviousSibling);
			assert(theBridgeNode != 0);

			theNavigator = theBridgeNode->getNavigator();
			assert(theNavigator != 0);

			theNewNode = theBridgeNode;
		}
		break;

	case DOM_Node::TEXT_NODE:
		{
			const DOM_Text&		theTextNode =
						static_cast<const DOM_Text&>(theXercesNode);

			XercesTextBridge* const		theBridgeNode =
				m_ownerDocument->createBridgeNode(theTextNode,
												  theParentNode,
												  thePreviousSibling);
			assert(theBridgeNode != 0);

			theNavigator = theBridgeNode->getNavigator();
			assert(theNavigator != 0);

			theNewNode = theBridgeNode;
		}
		break;

	default:
		assert(false);
		break;
	}

	if (theNavigator != 0)
	{
		assert(theNewNode != 0);

		XalanNode* const	theFirstChild =
			buildChildStructure(theXercesNode,
								theNewNode);

		theNavigator->setFirstChild(theFirstChild);

		theNavigator->setLastChild(findLastSibling(theFirstChild));

		XalanNode* const	theNextSibling =
			buildSiblingStructure(theXercesNode,
								  theParentNode,
								  theNewNode);

		theNavigator->setNextSibling(theNextSibling);

		if (m_nodeMap != 0)
		{
			m_nodeMap->addAssociation(theXercesNode, theNewNode, true);
		}
	}

	return theNewNode;
}



XalanNode*
XercesBridgeFactory::buildBridgeStructure(
				const DOM_Node&			theXercesNode,
				XalanNode*				theParentNode,
				XalanNode*				thePreviousSibling) const
{
	XalanNode*	theResult = 0;

	if (theXercesNode.isNull() == false)
	{
		theResult = createBridgeNode(theXercesNode,
									 theParentNode,
									 thePreviousSibling);
	}

	return theResult;
}



XalanNode*
XercesBridgeFactory::findLastSibling(XalanNode*	theNode)
{
	XalanNode*	theCurrentNode = theNode;
	XalanNode*	thePreviousNode = theNode;

	while(theCurrentNode != 0)
	{
		thePreviousNode = theCurrentNode;
		theCurrentNode = theCurrentNode->getNextSibling();
	}

	return thePreviousNode;
}



XalanNode*
XercesBridgeFactory::buildChildStructure(
				const DOM_Node&			theXercesNode,
				XalanNode*				theParentNode) const
{
	return buildBridgeStructure(theXercesNode.getFirstChild(),
								theParentNode,
								0);

}



XalanNode*
XercesBridgeFactory::buildSiblingStructure(
				const DOM_Node&			theXercesNode,
				XalanNode*				theParentNode,
				XalanNode*				thePreviousSibling) const
{
	return buildBridgeStructure(theXercesNode.getNextSibling(),
								theParentNode,
								thePreviousSibling);

}
