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
#if !defined(DOMSERVICES_HEADER_GUARD_1357924680)
#define DOMSERVICES_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <DOMSupport/DOMSupportDefinitions.hpp>



#include <XalanDOM/XalanDOMString.hpp>
#include <XalanDOM/XalanAttr.hpp>
#include <XalanDOM/XalanComment.hpp>
#include <XalanDOM/XalanDocument.hpp>
#include <XalanDOM/XalanElement.hpp>
#include <XalanDOM/XalanProcessingInstruction.hpp>
#include <XalanDOM/XalanText.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>



class XalanDocument;
class XalanDocumentFragment;



class XALAN_DOMSUPPORT_EXPORT DOMServices
{
public:

	static const XalanDOMString&	s_XMLString;
	static const XalanDOMString&	s_XMLStringWithSeparator;
	static const XalanDOMString&	s_XMLNamespaceURI;
	static const XalanDOMString&	s_XMLNamespace;
	static const XalanDOMString&	s_XMLNamespaceWithSeparator;
	static const XalanDOMString&	s_XMLNamespaceSeparatorString;

	static const unsigned int&		s_XMLStringLength;
	static const unsigned int&		s_XMLStringWithSeparatorLength;
	static const unsigned int&		s_XMLNamespaceURILength;
	static const unsigned int&		s_XMLNamespaceLength;
	static const unsigned int&		s_XMLNamespaceWithSeparatorLength;
	static const unsigned int&		s_XMLNamespaceSeparatorStringLength;

	// A dummy string to return when we need an emtpy string...
	static const XalanDOMString		s_emptyString;


	class XALAN_DOMSUPPORT_EXPORT WhitespaceSupport
	{
	public:

		WhitespaceSupport();

		virtual
		~WhitespaceSupport();

		/**
		 * Tell if the node is ignorable whitespace. This should be in the DOM.
		 * Return false if the parser doesn't handle this.
		 * 
		 * @param node	text node queried
		 * @return true if white space can be ignored
		 */
		virtual bool
		isIgnorableWhitespace(const XalanText&	node) const = 0;
	};

	// A default implementation using the values from the XML spec.
	class XALAN_DOMSUPPORT_EXPORT WhitespaceSupportDefault : public WhitespaceSupport
	{
	public:

		WhitespaceSupportDefault();

		virtual
		~WhitespaceSupportDefault();

		/**
		 * Tell if the node is ignorable whitespace. This should be in the DOM.
		 * Return false if the parser doesn't handle this.
		 * 
		 * @param node	text node queried
		 * @return true if white space can be ignored
		 */
		virtual bool
		isIgnorableWhitespace(const XalanText&	node) const;
	};

	/**
	 * Initialize static data.  Must be called before any
	 * other functions are called.
	 */
	static void
	initialize();

	/**
	 * Destroy static data.  After thus function is called,
	 * no other functions can be called.
	 */
	static void
	terminate();

	/**
	 * Retrieves data for node
	 * 
	 * @param node DOM node whose data is to be returned
	 * @return a string representation of the node's data
	 */
	static XalanDOMString
	getNodeData(const XalanNode&	node);

	/**
	 * Retrieves data for node
	 * 
	 * @param node DOM node whose data is to be returned
	 * @param data a string to which the node's data will be appended
	 */
	static void
	getNodeData(
			const XalanNode&	node,
			XalanDOMString&		data);

	/**
	 * Retrieves data for node
	 * 
	 * @param attribute DOM node whose data is to be returned
	 * @return a string representation of the node's data
	 */
	static XalanDOMString
	getNodeData(const XalanAttr&	attribute)
	{
		return attribute.getNodeValue();
	}

	/**
	 * Retrieves data for node
	 * 
	 * @param attribute DOM node whose data is to be returned
	 * @param data a string to which the node's data will be appended
	 */
	static void
	getNodeData(
			const XalanAttr&	attribute,
			XalanDOMString&		data)
	{
		append(data, attribute.getNodeValue());
	}

	/**
	 * Retrieves data for node
	 * 
	 * @param attribute DOM node whose data is to be returned
	 * @return a string representation of the node's data
	 */
	static XalanDOMString
	getNodeData(const XalanComment&		comment)
	{
		return comment.getData();
	}

	/**
	 * Retrieves data for node
	 * 
	 * @param attribute DOM node whose data is to be returned
	 * @param data a string to which the node's data will be appended
	 */
	static void
	getNodeData(
			const XalanComment&		comment,
			XalanDOMString&			data)
	{
		append(data, comment.getData());
	}

	/**
	 * Retrieves data for node
	 * 
	 * @param document DOM node whose data is to be returned
	 * @return a string representation of the node's data
	 */
	static XalanDOMString
	getNodeData(const XalanDocument&	document);

	/**
	 * Retrieves data for node
	 * 
	 * @param document DOM node whose data is to be returned
	 * @param data a string to which the node's data will be appended
	 */
	static void
	getNodeData(
			const XalanDocument&	document,
			XalanDOMString&			data);

	/**
	 * Retrieves data for node
	 * 
	 * @param documentFragment DOM node whose data is to be returned
	 * @return a string representation of the node's data
	 */
	static XalanDOMString
	getNodeData(const XalanDocumentFragment&	documentFragment);

	/**
	 * Retrieves data for node
	 * 
	 * @param documentFragment DOM node whose data is to be returned
	 * @param data a string to which the node's data will be appended
	 */
	static void
	getNodeData(
			const XalanDocumentFragment&	documentFragment,
			XalanDOMString&					data);

	/**
	 * Retrieves data for node
	 * 
	 * @param element DOM node whose data is to be returned
	 * @return a string representation of the node's data
	 */
	static XalanDOMString
	getNodeData(const XalanElement&		element);

	/**
	 * Retrieves data for node
	 * 
	 * @param element DOM node whose data is to be returned
	 * @param data a string to which the node's data will be appended
	 */
	static void
	getNodeData(
			const XalanElement&		element,
			XalanDOMString&			data);

	/**
	 * Retrieves data for node
	 * 
	 * @param pi DOM node whose data is to be returned
	 * @return a string representation of the node's data
	 */
	static XalanDOMString
	getNodeData(const XalanProcessingInstruction&	pi)
	{
		return pi.getData();
	}

	/**
	 * Retrieves data for node
	 * 
	 * @param pi DOM node whose data is to be returned
	 * @param data a string to which the node's data will be appended
	 */
	static void
	getNodeData(
			const XalanProcessingInstruction&	pi,
			XalanDOMString&						data)
	{
		append(data, pi.getData());
	}

	/**
	 * Retrieves data for node
	 * 
	 * @param node DOM node whose data is to be returned
	 * @return a string representation of the node's data
	 */
	static XalanDOMString
	getNodeData(const XalanText&	text)
	{
		return text.getData();
	}

	/**
	 * Retrieves data for node
	 * 
	 * @param node DOM node whose data is to be returned
	 * @param data a string to which the node's data will be appended
	 */
	static void
	getNodeData(
			const XalanText&	text,
			XalanDOMString&		data)
	{
		append(data, text.getData());
	}

	/**
	 * Retrieve the name of the node, taking into
	 * account the differences between the DOM and
	 * XSLT data models.
	 * 
	 * @param node	DOM node whose name is returned
	 * @return name of the node
	 */
	static const XalanDOMString&
	getNameOfNode(const XalanNode&	n);

	/**
	 * Retrieve local name of node, taking into
	 * account the differences between the DOM and
	 * XSLT data models.
	 * 
	 * @param node	DOM node whose name is returned
	 * @return name of node without namespace
	 */
	static const XalanDOMString&
	getLocalNameOfNode(const XalanNode&		n)
	{
		const XalanDOMString&	theLocalName = n.getLocalName();

		if (length(theLocalName) != 0)
		{
			return theLocalName;
		}
		else
		{
			assert(length(n.getNodeName()) != 0);

			return n.getNodeName();
		}
	}

	/**
	 * Retrieve the namespace of the node, taking into
	 * account the differences between the DOM and
	 * XSLT data models.
	 * 
	 * @param node	DOM node whose namespace is returned
	 * @return namespace of the node
	 */
	static const XalanDOMString&
	getNamespaceOfNode(const XalanNode&	n);

	/**
	 * Retrieve the parent of a node. This function has to be implemented,
	 * because the DOM WG decided that attributes don't have parents.
	 * 
	 * @param node child node
	 * @return parent node
	 */
	static XalanNode*
	getParentOfNode(const XalanNode&	node)
	{
		if(node.getNodeType() == XalanNode::ATTRIBUTE_NODE)
		{
#if defined(XALAN_OLD_STYLE_CASTS)
			return findOwnerElement((const XalanAttr&)node);
#else
			return findOwnerElement(static_cast<const XalanAttr&>(node));
#endif
		}
		else
		{
			return node.getParentNode();
		}
	}

	/**
	 * Retrieve the URI corresponding to a namespace prefix
	 * 
	 * @param prefix           prefix for a namespace
	 * @param namespaceContext DOM element representing the context for namespace
	 * @return URI corresponding to namespace
	 */
	static const XalanDOMString&
	getNamespaceForPrefix(
			const XalanDOMString&	prefix,
			const XalanElement&		namespaceContext);

	/**
	 * Determine if a node is after another node, in document order.
	 *
	 * @param node1 The first node
	 * @param node2 The second node
	 * @return true if node1 one is after node2, or false if it is not.
	 */
	static bool
	isNodeAfter(
			const XalanNode&	node1,
			const XalanNode&	node2);

	/**
	 * Determine if a node is after another node in the sibling list.
	 *
	 * @param parent The parent of the nodes.
	 * @param node1 The first node
	 * @param node2 The second node
	 * @return true if node1 one is after node2, or false if it is not.
	 */
	static bool
	isNodeAfterSibling(
			const XalanNode&	parent,
			const XalanNode&	child1,
			const XalanNode&	child2);

private:

	/**
	 * If necessary, do a brute-force search for an owner element.  This is
	 * necessary when a given DOM implementation returns 0 for
	 * XalanAttr::getOwnerElement()
	 *
	 * @param attr The XalanAttr instance for which to find the owner element
	 * @return A pointer to the element node that owns the attribute
	 */
	static XalanNode*
	findOwnerElement(const XalanAttr&	attr)
	{
		XalanNode* const	theOwnerElement = attr.getOwnerElement();

		if (theOwnerElement != 0)
		{
			return theOwnerElement;
		}
		else
		{
			return findOwnerElement(attr, *attr.getOwnerDocument()->getDocumentElement());
		}
	}

	/**
	 * If necessary, do a brute-force search for an owner element.  This is
	 * necessary when a given DOM implementation returns 0 for
	 * XalanAttr::getOwnerElement()
	 *
	 * @param attr The XalanAttr instance for which to find the owner element
	 * @param element The document element
	 * @return A pointer to the element node that owns the attribute
	 */
	static XalanNode*
	findOwnerElement(
			const XalanNode&	attr,
			XalanNode&			element);

};



#endif	// DOMSERVICES_HEADER_GUARD_1357924680
