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



#include <dom/DOM_Attr.hpp>
#include <dom/DOM_Document.hpp>
#include <dom/DOM_Element.hpp>
#include <dom/DOM_NamedNodeMap.hpp>
#include <dom/DOM_Text.hpp>



#include <Include/DOMHelper.hpp>
#include <PlatformSupport/DOMStringHelper.hpp>
#include "DOMSupportException.hpp"



DOMServices::WhitespaceSupport::WhitespaceSupport()
{
}



DOMServices::WhitespaceSupport::~WhitespaceSupport()
{
}



XALAN_DOMSUPPORT_EXPORT_FUNCTION(DOMString)
DOMServices::getNodeData(
 			const DOM_Node&				node,
			const WhitespaceSupport&	theResolver)
{
	DOMString	data;

	switch(node.getNodeType())
	{
	case DOM_Node::DOCUMENT_FRAGMENT_NODE:
	case DOM_Node::DOCUMENT_NODE:
	case DOM_Node::ELEMENT_NODE:
		{
			DOM_NodeList	children = node.getChildNodes();

			const int 	nNodes = children.getLength();

			for(int i = 0; i < nNodes; i++)
			{
				const DOMString 	nodeData =
					getNodeData(children.item(i),
								theResolver);

				if(0 < length(nodeData))
				{
					data += nodeData;
				}
			}
		}
	  break;

	case DOM_Node::TEXT_NODE:
	case DOM_Node::CDATA_SECTION_NODE:
		{
			const DOM_Text&		theTextNode =
				static_cast<const DOM_Text&>(node);

			if(theResolver.isIgnorableWhitespace(theTextNode) == false)
			{
				// data = fixWhiteSpace(theTextNode.getData(), false, false, true);
				data = theTextNode.getData();
			}
		}
		break;

	case DOM_Node::ATTRIBUTE_NODE:
		data = node.getNodeValue();
		break;

	default:
		// ignore
		break;
	}

	return data;
}



XALAN_DOMSUPPORT_EXPORT_FUNCTION(DOMString)
DOMServices::getLocalNameOfNode(const DOM_Node&		n)
{
	const DOMString		qname = n.getNodeName();

	const int			index = indexOf(qname, ':');

	return (index < 0) ? qname : substring(qname, index + 1);
}



/**
 * Support for getParentOfNode.
 */
namespace {

DOM_Node
locateAttrParent(
			const DOM_Element&	elem,
			const DOM_Node&		attr)
{

    DOM_Node			parent;

	DOM_NamedNodeMap	attrs = elem.getAttributes();

    if(attrs != 0)
	{
		const int		nAttrs = attrs.getLength();
		
		for(int i = 0; i < nAttrs; i++)
		{
			if(attrs.item(i) == attr)
			{
				parent = elem;
				
				break;
			}
		}
    }

	if(parent == 0)
    {
		DOM_NodeList	children = elem.getChildNodes();

		const int		nChildren = children.getLength();

		for(int i = 0; i < nChildren; i++)
		{
			DOM_Node node = children.item(i);

			if(node.getNodeType() == DOM_Node::ELEMENT_NODE)
			{
				parent = locateAttrParent(static_cast<DOM_Element&>(node), attr);

				if(parent != 0)
					break;
			}
		}
    }

	return parent;
}

};	// namespace



XALAN_DOMSUPPORT_EXPORT_FUNCTION(DOM_Node)
DOMServices::getParentOfNode(const DOM_Node&	node)
{
	DOM_Node		parent;
	const short		nodeType = node.getNodeType();

	if(DOM_Node::ATTRIBUTE_NODE == nodeType)
	{
		const DOM_Document	doc = node.getOwnerDocument();

		if(doc == 0)
		{
			throw DOMSupportException("Attribute child does not have an owner document!");
		}
		else
		{
			const DOM_Element	rootElem = doc.getDocumentElement();
			assert(rootElem != 0);

			parent = locateAttrParent(rootElem, node);
		}
	}
	else
	{
		parent = node.getParentNode();
      
		if(nodeType != DOM_Node::DOCUMENT_NODE && parent == 0)
		{
			throw DOMSupportException("Child does not have parent!");
		}
	}

	return parent;
}



XALAN_DOMSUPPORT_EXPORT_FUNCTION(DOMString)
DOMServices::getNamespaceForPrefix(
			const DOMString&	prefix,
			const DOM_Element&	namespaceContext)
{
	DOMString theNamespace;

	if(equals(prefix, "xml") == true)
	{
		theNamespace = theXMLNamespaceURI;
	}
	else
	{
		short		type;
		DOM_Node	parent = namespaceContext;
    
		while (parent != 0 && length(theNamespace) == 0
			&& ((type = parent.getNodeType()) == DOM_Node::ELEMENT_NODE
				|| type == DOM_Node::ENTITY_REFERENCE_NODE)) 
		{
			if (type == DOM_Node::ELEMENT_NODE) 
			{
				DOM_NamedNodeMap nnm = parent.getAttributes();
				
				for (int i = 0;  i < nnm.getLength();  i ++) 
				{
					DOM_Node	attr = nnm.item(i);
					DOMString	aname = attr.getNodeName();

					const char* const	theXMLNS = "xmlns:";

					const bool isPrefix =
							equals(substring(aname,
											 0,
											 0 + strlen(theXMLNS)),
								   theXMLNS);

					if (equals(aname, "xmlns") || isPrefix) 
					{
						const int	index = indexOf(aname,
													':');
              
						const DOMString p =
							isPrefix ? substring(aname,
												 index + 1,
												 length(aname)) : DOMString();
              
						if (equals(p, prefix) == true)
						{
							theNamespace = attr.getNodeValue();

							break;
						}
					}
				}
			}

			parent = getParentOfNode(parent);
		}
	}

	return theNamespace;
}
