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
#include "DOMServices.hpp"



#include <vector>



#include <XalanDOM/XalanAttr.hpp>
#include <XalanDOM/XalanDocument.hpp>
#include <XalanDOM/XalanElement.hpp>
#include <XalanDOM/XalanNamedNodeMap.hpp>
#include <XalanDOM/XalanNodeList.hpp>
#include <XalanDOM/XalanText.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>
#include "DOMSupportException.hpp"



#if defined(XALAN_INLINE_INITIALIZATION)

const XalanDOMString	DOMServices::s_XMLString;
const XalanDOMString	DOMServices::s_XMLNamespaceURI;
const XalanDOMString	DOMServices::s_XMLNamespace;
const XalanDOMString	DOMServices::s_XMLNamespaceWithSeparator;

#else

const XalanDOMString	DOMServices::s_XMLString(XALAN_STATIC_UCODE_STRING("xml"));
const XalanDOMString	DOMServices::s_XMLNamespaceURI(XALAN_STATIC_UCODE_STRING("http://www.w3.org/XML/1998/namespace"));
const XalanDOMString	DOMServices::s_XMLNamespace(XALAN_STATIC_UCODE_STRING("xmlns"));
const XalanDOMString	DOMServices::s_XMLNamespaceWithSeparator(XALAN_STATIC_UCODE_STRING("xmlns:"));

#endif

const unsigned int		DOMServices::s_XMLStringLength = length(s_XMLString);
const unsigned int		DOMServices::s_XMLNamespaceURILength = length(s_XMLNamespaceURI);
const unsigned int		DOMServices::s_XMLNamespaceLength = length(s_XMLNamespace);
const unsigned int		DOMServices::s_XMLNamespaceWithSeparatorLength = length(s_XMLNamespaceWithSeparator);



DOMServices::WhitespaceSupport::WhitespaceSupport()
{
}



DOMServices::WhitespaceSupport::~WhitespaceSupport()
{
}



DOMServices::WhitespaceSupportDefault::WhitespaceSupportDefault()
{
}



DOMServices::WhitespaceSupportDefault::~WhitespaceSupportDefault()
{
}



bool
DOMServices::WhitespaceSupportDefault::isIgnorableWhitespace(const XalanText&	node) const
{
	const XalanDOMString	theData(node.getData());

	const unsigned int	theLength = length(theData);

	unsigned int i = 0;

	for (; i < theLength; i++)
	{
		const XalanDOMChar	theChar = charAt(theData, i);

		if (!(theChar == 0x20 || theChar == 0xD || theChar == 0xA || theChar == 0x9))
		{
			break;
		}
	}

	return i == theLength ? true : false;
}



XalanDOMString
DOMServices::getNodeData(const XalanNode&	node)
{
	XalanDOMString	data;

	switch(node.getNodeType())
	{
	case XalanNode::DOCUMENT_FRAGMENT_NODE:
		{
			const XalanNodeList* const	mnl = node.getChildNodes();
			assert(mnl != 0);

			const unsigned int	n = mnl->getLength();

			for(unsigned int i = 0; i < n; ++i)
			{
				assert(mnl->item(i) != 0);

				const XalanDOMString 	nodeData =
					getNodeData(*mnl->item(i));

				if(0 < length(nodeData))
				{
					data += nodeData;
				}
			}
		}
		break;

	case XalanNode::DOCUMENT_NODE:
	case XalanNode::ELEMENT_NODE:
		{
			const XalanNode*	child = node.getFirstChild();

			while(child != 0)
			{
				const XalanDOMString 	nodeData =
					getNodeData(*child);

				if(0 < length(nodeData))
				{
					data += nodeData;
				}

				child = child->getNextSibling();
			}
		}
	  break;

	case XalanNode::TEXT_NODE:
	case XalanNode::CDATA_SECTION_NODE:
		{
			const XalanText&	theTextNode =
#if defined(XALAN_OLD_STYLE_CASTS)
				(const XalanText&)node;
#else
				static_cast<const XalanText&>(node);
#endif

			// This is commented out because it turns out that it's incorrect...
//			if(theResolver.isIgnorableWhitespace(theTextNode) == false)
//			{
				data = theTextNode.getData();
//			}
		}
		break;

	case XalanNode::ATTRIBUTE_NODE:
		data = node.getNodeValue();
		break;

	default:
		// ignore
		break;
	}

	return data;
}



XalanDOMString
DOMServices::getNameOfNode(const XalanNode&		n)
{
	XalanDOMString	theResult;

	const XalanNode::NodeType	theNodeType =
				n.getNodeType();

	if (theNodeType == XalanNode::ATTRIBUTE_NODE)
	{
		const XalanAttr&	theAttributeNode =
#if defined(XALAN_OLD_STYLE_CASTS)
				(const XalanAttr&)(n);
#else
				static_cast<const XalanAttr&>(n);
#endif

		theResult = theAttributeNode.getName();

		if (startsWith(theResult, DOMServices::s_XMLNamespaceWithSeparator) == true)
		{
			// Uh oh, it's a namespace node, represented as an attribute in
			// the DOM.  XSLT says we have to strip off the xmlns: part...
			theResult = substring(theResult, length(DOMServices::s_XMLNamespaceWithSeparator));
		}

	}
	else if (theNodeType == XalanNode::ELEMENT_NODE ||
			 theNodeType == XalanNode::PROCESSING_INSTRUCTION_NODE)
	{
		theResult = n.getNodeName();
	}

	return theResult;
}



XalanDOMString
DOMServices::getLocalNameOfNode(const XalanNode&	n)
{
	const XalanDOMString	qname = n.getNodeName();

	const unsigned int		index = indexOf(qname, ':');

	return index == length(qname) ? qname : substring(qname, index + 1);
}



/**
 * Support for getParentOfNode.
 */
static XalanNode*
locateAttrParent(
			const XalanElement&		elem,
			const XalanNode&		attr)
{

    XalanNode*	parent = 0;

	const XalanNamedNodeMap* const	attrs = elem.getAttributes();
	assert(attrs != 0);

    if(attrs != 0)
	{
		const unsigned int	nAttrs = attrs->getLength();

		for(unsigned int i = 0; i < nAttrs; i++)
		{
			if(attrs->item(i) == &attr)
			{
#if defined(XALAN_OLD_STYLE_CASTS)
				parent = (XalanElement*)&elem;
#else
				parent = const_cast<XalanElement*>(&elem);
#endif
				
				break;
			}
		}
    }

	if(parent == 0)
    {
		bool				fFound = false;

		const XalanNode*	child = elem.getFirstChild();

		while(child != 0 && fFound == false)
		{
			if(child->getNodeType() == XalanNode::ELEMENT_NODE)
			{
#if defined(XALAN_OLD_STYLE_CASTS)
				parent = locateAttrParent((const XalanElement&)*child, attr);
#else
				parent = locateAttrParent(static_cast<const XalanElement&>(*child), attr);
#endif

				if(parent != 0)
				{
					fFound = true;
				}
			}

			if (fFound == false)
			{
				child = child->getNextSibling();
			}
		}
    }

	return parent;
}




XalanNode*
DOMServices::getParentOfNode(const XalanNode&	node)
{
	XalanNode*					parent = 0;

	const XalanNode::NodeType	nodeType = node.getNodeType();

	if(XalanNode::ATTRIBUTE_NODE == nodeType)
	{
		XalanDocument* const	doc = node.getOwnerDocument();

		if(doc == 0)
		{
			throw DOMSupportException("Attribute child does not have an owner document!");
		}
		else
		{
			XalanElement*	const	rootElem = doc->getDocumentElement();
			assert(rootElem != 0);

			parent = locateAttrParent(*rootElem, node);
		}
	}
	else
	{
		parent = node.getParentNode();

		if(nodeType != XalanNode::DOCUMENT_NODE && parent == 0)
		{
			throw DOMSupportException("Child does not have parent!");
		}
	}

	return parent;
}



XalanDOMString
DOMServices::getNamespaceForPrefix(
			const XalanDOMString&	prefix,
			const XalanElement&		namespaceContext)
{
	XalanDOMString theNamespace;

	if(equals(prefix, s_XMLString) == true)
	{
		theNamespace = s_XMLNamespaceURI;
	}
	else
	{
		XalanNode::NodeType		type;
		const XalanNode*		parent = &namespaceContext;
    
		while (parent != 0 && length(theNamespace) == 0
			&& ((type = parent->getNodeType()) == XalanNode::ELEMENT_NODE
				|| type == XalanNode::ENTITY_REFERENCE_NODE)) 
		{
			if (type == XalanNode::ELEMENT_NODE) 
			{
				const XalanNamedNodeMap* const	nnm = parent->getAttributes();
				assert(nnm != 0);

				const unsigned int				theLength = nnm->getLength();

				for (unsigned int i = 0;  i < theLength;  i ++) 
				{
					const XalanNode* const	attr = nnm->item(i);
					assert(attr != 0);

					const XalanDOMString		aname = attr->getNodeName();

					const unsigned int			len = length(aname);

					const bool isPrefix = len <= s_XMLNamespaceWithSeparatorLength ? false :
							equals(substring(aname,
											 0,
											 s_XMLNamespaceWithSeparatorLength),
								   s_XMLNamespaceWithSeparator);

					if (equals(aname, s_XMLNamespace) || isPrefix) 
					{
						const unsigned int	index = indexOf(aname,
															':');
              
						const XalanDOMString	p =
							isPrefix ? substring(aname,
												 index + 1,
												 len) : XalanDOMString();

						if (equals(p, prefix) == true)
						{
							theNamespace = attr->getNodeValue();

							break;
						}
					}
				}
			}

			parent = getParentOfNode(*parent);
		}
	}

	return theNamespace;
}






bool
DOMServices::isNodeAfter(
			const XalanNode&	node1,
			const XalanNode&	node2)
{
	bool		isNodeAfter = false;

	// Doesn't matter in this case ...
	if (XalanNode::DOCUMENT_NODE == node1.getNodeType()
			&& XalanNode::DOCUMENT_NODE == node2.getNodeType()) return false;
	
	const XalanNode*	parent1 = getParentOfNode(node1);
// @@ Could be document node
//	assert(parent1 != 0);

	const XalanNode*	parent2 = getParentOfNode(node2);
// @@ Could be document node
//	assert(parent2 != 0);

	// Optimize for most common case
	if(parent1 == parent2) // then we know they are siblings
	{
		isNodeAfter = isNodeAfterSibling(*parent1,
										 node1,
										 node2);
	}
	else
	{
		// General strategy: Figure out the lengths of the two 
		// ancestor chains, and walk up them looking for the 
		// first common ancestor, at which point we can do a 
		// sibling compare.  Edge condition where one is the 
		// ancestor of the other.
	  
		// Count parents, so we can see if one of the chains 
		// needs to be equalized.
		int nParents1 = 2;
		int nParents2 = 2; // count node & parent obtained above

		while(parent1 != 0)
		{
			nParents1++;
			parent1 = getParentOfNode(*parent1);
		}

		while(parent2 != 0)
		{
			nParents2++;
			parent2 = getParentOfNode(*parent2);
		}

		// adjustable starting points
		const XalanNode*	startNode1 = &node1;
		const XalanNode*	startNode2 = &node2;

		// Do I have to adjust the start point in one of 
		// the ancesor chains?
		if(nParents1 < nParents2)
		{
			// adjust startNode2
			const int	adjust = nParents2 - nParents1;

			for(int i = 0; i < adjust; i++)
			{
				startNode2 = getParentOfNode(*startNode2);
			}
		}
		else if(nParents1 > nParents2)
		{
			// adjust startNode1
			const int	adjust = nParents1 - nParents2;

			for(int i = 0; i < adjust; i++)
			{
				startNode1 = getParentOfNode(*startNode1);
			}
		}

		// so we can "back up"
		const XalanNode*	prevChild1 = 0;
		const XalanNode*	prevChild2 = 0;
	  
		// Loop up the ancestor chain looking for common parent.
		while(0 != startNode1)
		{
			if(startNode1 == startNode2) // common parent?
			{
				if(0 == prevChild1) // first time in loop?
				{
					// Edge condition: one is the ancestor of the other.
					isNodeAfter = (nParents1 < nParents2) ? true : false;

					break; // from while loop
				}
				else
				{
					isNodeAfter = isNodeAfterSibling(*startNode1,
													 *prevChild1,
													 *prevChild2);

					break; // from while loop
				}
			}

			prevChild1 = startNode1;
// @@ Could be document node
//			assert(prevChild1 != 0);

			startNode1 = getParentOfNode(*startNode1);
//			assert(startNode1 != 0);

			prevChild2 = startNode2;
//			assert(prevChild2 != 0);

			startNode2 = getParentOfNode(*startNode2);
//			assert(startNode2 != 0);
		}
	}

	return isNodeAfter;
}



bool
DOMServices::isNodeAfterSibling(
			const XalanNode&	parent,
			const XalanNode&	child1,
			const XalanNode&	child2)
{
	bool	isNodeAfterSibling = false;

	const XalanNode::NodeType	child1type = child1.getNodeType();
	const XalanNode::NodeType	child2type = child2.getNodeType();

	if(XalanNode::ATTRIBUTE_NODE != child1type &&
	   XalanNode::ATTRIBUTE_NODE == child2type)
	{
		// always sort attributes before non-attributes.
		isNodeAfterSibling = false;
	}
	else if(XalanNode::ATTRIBUTE_NODE == child1type &&
			XalanNode::ATTRIBUTE_NODE != child2type)
	{
		// always sort attributes before non-attributes.
		isNodeAfterSibling = true;
	}
	else if(XalanNode::ATTRIBUTE_NODE == child1type)
	{
		const XalanNamedNodeMap*	children = parent.getAttributes();
	  
		const unsigned int	nNodes = children->getLength();

		bool				found1 = false;
		bool				found2 = false;

		for(unsigned int i = 0; i < nNodes; i++)
		{
			const XalanNode*	child = children->item(i);

			if(&child1 == child)
			{
				if(found2 == true)
				{
					isNodeAfterSibling = false;
					break;
				}
		  
				found1 = true;
			}
			else if(&child2 == child)
			{
				if(found1 == true)
				{
					isNodeAfterSibling = true;
					break;
				}
		  
				found2 = true;
			}
		}
	}
	else
	{
		const XalanNode*	child = parent.getFirstChild();

		bool			found1 = false;
		bool			found2 = false;

		while(child != 0)
		{
			if(&child1 == child)
			{
				if(found2 == true)
				{
					isNodeAfterSibling = false;
					break;
				}

				found1 = true;
			}
			else if(&child2 == child)
			{
				if(found1 == true)
				{
					isNodeAfterSibling = true;
					break;
				}

				found2 = true;
			}

			child = child->getNextSibling();
		}

		assert(found1 != found2);
	}

	return isNodeAfterSibling;
}
