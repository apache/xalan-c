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



#if defined(XALAN_INLINE_INITIALIZATION)
#include <PlatformSupport/DOMStringHelper.hpp>
#endif



class XalanNode;
class XalanElement;
class XalanText;



class XALAN_DOMSUPPORT_EXPORT DOMServices
{
public:

#if defined(XALAN_INLINE_INITIALIZATION)

	static const XalanDOMString		s_XMLString(XALAN_STATIC_UCODE_STRING("xml"));
	static const XalanDOMString		s_XMLNamespaceURI(XALAN_STATIC_UCODE_STRING("http://www.w3.org/XML/1998/namespace"));
	static const XalanDOMString		s_XMLNamespace(XALAN_STATIC_UCODE_STRING("xmlns"));
	static const XalanDOMString		s_XMLNamespaceWithSeparator(XALAN_STATIC_UCODE_STRING("xmlns:"));

#else

	static const XalanDOMString		s_XMLString;
	static const XalanDOMString		s_XMLNamespaceURI;
	static const XalanDOMString		s_XMLNamespace;
	static const XalanDOMString		s_XMLNamespaceWithSeparator;

#endif

	static const unsigned int		s_XMLStringLength;
	static const unsigned int		s_XMLNamespaceURILength;
	static const unsigned int		s_XMLNamespaceLength;
	static const unsigned int		s_XMLNamespaceWithSeparatorLength;

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
	 * Retrieves data for node
	 * 
	 * @param node        DOM node whose data is to be returned
	 * @return a string representation of the node's data
	 */
	static XalanDOMString
	getNodeData(const XalanNode&	node);

	/**
	 * Retrieve the name of the node, taking into
	 * account the differences between the DOM and
	 * XSLT data models.
	 * 
	 * @param node	DOM node whose name is returned
	 * @return name of the node
	 */
	static XalanDOMString
	getNameOfNode(const XalanNode&	n);

	/**
	 * Retrieve local name of node, taking into
	 * account the differences between the DOM and
	 * XSLT data models.
	 * 
	 * @param node	DOM node whose name is returned
	 * @return name of node without namespace
	 */
	static XalanDOMString
	getLocalNameOfNode(const XalanNode&		n);

	/**
	 * Retrieve the parent of a node. This function has to be implemented,
	 * because the DOM WG decided that attributes don't have parents.
	 * 
	 * @param node child node
	 * @return parent node
	 */
	static XalanNode*
	getParentOfNode(const XalanNode&	node);

	/**
	 * Retrieve the URI corresponding to a namespace prefix
	 * 
	 * @param prefix           prefix for a namespace
	 * @param namespaceContext DOM element representing the context for namespace
	 * @return URI corresponding to namespace
	 */
	static XalanDOMString
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

};	// class DOMServices



#endif	// DOMSERVICES_HEADER_GUARD_1357924680
