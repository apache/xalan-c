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
#if !defined(XPATHSUPPORT_HEADER_GUARD_1357924680)
#define XPATHSUPPORT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



#include <dom/DOMString.hpp>
#include <dom/DOM_Node.hpp>
#include <dom/DOM_Document.hpp>
#include <dom/DOM_Element.hpp>



#include <PlatformSupport/Resettable.hpp>



class DocumentHandler;
class XMLURL;



class XALAN_XPATH_EXPORT XPathSupport : public Resettable
{
public:

	XPathSupport();

	virtual
	~XPathSupport();

	// These interfaces are inherited from Resettable...

	/**
	 * Reset the instance.
	 */
	virtual void
	reset() = 0;

	// These interfaces are inherited from DOMServices::WhitespaceSupport...

	/**
	 * Tell if the node is ignorable whitespace.
	 * This should be in the DOM.  Return false if the 
	 * parser doesn't handle this.
	 */
	virtual bool
	isIgnorableWhitespace(const DOM_Text&	node) const = 0;


	// These interfaces are new to XPathSupport...

	/**
	 * Given a namespace, get the corresponding prefix.
	 */
	virtual DOMString
	getNamespaceForPrefix(
			const DOMString&	prefix, 
			const DOM_Element&	namespaceContext) const = 0;

	/**
	 * Returns the namespace of the given node.
	 */
	virtual DOMString
	getNamespaceOfNode(const DOM_Node&	n) const = 0;

	/**
	 * Returns the local name of the given node.
	 */
	virtual DOMString
	getLocalNameOfNode(const DOM_Node&	n) const = 0;

	/**
	 * Get node data recursively.
	 * (Note whitespace issues.)
	 */
	virtual DOMString
	getNodeData(const DOM_Node&		n) const = 0;

	/**
	 * This function has to be implemented, 
	 * because the DOM WG decided that attributes don't 
	 * have parents.
	 */
	virtual DOM_Node
	getParentOfNode(const DOM_Node&		n) const = 0;

	/**
	 * Get an element from an ID.
	 */
	virtual DOM_Element
	getElementByID(
			const DOMString&		id,
			const DOM_Document&		doc) const = 0;

	/**
	 * Set whether or not the liaison attempts to expand namespaces.	Used 
	 * for optimization.
	 */
	virtual void
	setProcessNamespaces(bool	processNamespaces) = 0;

	/**
	 * Tells if namespaces should be supported.  For optimization purposes.
	 */
	virtual bool
	getProcessNamespaces() const = 0;

	/**
	 * The getUnparsedEntityURI function returns the URI of the unparsed
	 * entity with the specified name in the same document as the context
	 * node (see [3.3 Unparsed Entities]). It returns the empty string if
	 * there is no such entity.
	 *
	 * Since it states in the DOM draft: "An XML processor may choose to
	 * completely expand entities before the structure model is passed
	 * to the DOM; in this case, there will be no EntityReferences in the DOM tree."
	 * So I'm not sure how well this is going to work.
	 */
	virtual DOMString
	getUnparsedEntityURI(
			const DOMString&		theName,
			const DOM_Document&		theDocument) const = 0;

private:

	// These are not implemented...
	XPathSupport(const XPathSupport&);

	XPathSupport&
	operator=(const XPathSupport&);

	bool
	operator==(const XPathSupport&);
};



#endif	// XPATHSUPPORT_HEADER_GUARD_1357924680
