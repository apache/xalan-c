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



#include <dom/DOM_Node.hpp>
#include <dom/DOMString.hpp>



#if defined(XALAN_INLINE_INITIALIZATION)
#include <PlatformSupport/DOMStringHelper.hpp>
#endif



class DOM_Element;
class DOM_Text;



class XALAN_DOMSUPPORT_EXPORT DOMServices
{
public:

#if defined(XALAN_INLINE_INITIALIZATION)

	static const DOMString	s_XMLNamespaceURI(XALAN_STATIC_UCODE_STRING("http://www.w3.org/XML/1998/namespace"));

#else

	static const DOMString	s_XMLNamespaceURI;

#endif

	class XALAN_DOMSUPPORT_EXPORT WhitespaceSupport
	{
	public:

		WhitespaceSupport();

		virtual
		~WhitespaceSupport();

		/**
		 * Tell if the node is ignorable whitespace.
		 * This should be in the DOM.  Return false if the 
		 * parser doesn't handle this.
		 * 
		 * @param node	text node queried
		 * @return true if white space can be ignored
		 */
		virtual bool
		isIgnorableWhitespace(const DOM_Text&	node) const = 0;
	};

	/**
	 * Retrieves data for node
	 * 
	 * @param node        DOM node whose data is to be returned
	 * @param theResolver prefix resolver to use
	 * @return a string representation of the node's data
	 */
	static DOMString
	getNodeData(
			const DOM_Node&				node,
			const WhitespaceSupport&	theResolver);

	/**
	 * Retrieve local name of node
	 * 
	 * @param node	DOM node whose name is returned
	 * @return name of node without namespace
	 */
	static DOMString
	getLocalNameOfNode(const DOM_Node&		n);

	/**
	 * Retrieve the parent of a node. This function has to be implemented,
	 * because the DOM WG decided that attributes don't have parents.
	 * 
	 * @param node child node
	 * @return parent node
	 */
	static DOM_Node
	getParentOfNode(const DOM_Node&		node);

	/**
	 * Retrieve the URI corresponding to a namespace prefix
	 * 
	 * @param prefix           prefix for a namespace
	 * @param namespaceContext DOM element representing the context for namespace
	 * @return URI corresponding to namespace
	 */
	static DOMString
	getNamespaceForPrefix(
			const DOMString&	prefix,
			const DOM_Element&	namespaceContext);

};	// class DOMServices



#endif	// DOMSERVICES_HEADER_GUARD_1357924680
