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
#if !defined(XPATHSUPPORTDEFAULT_HEADER_GUARD_1357924680)
#define XPATHSUPPORTDEFAULT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



#include <XPath/XPathSupport.hpp>
#include <DOMSupport/DOMServices.hpp>



class DOMSupport;



class XALAN_XPATH_EXPORT XPathSupportDefault : public XPathSupport, public DOMServices::WhitespaceSupport
{
public:

	XPathSupportDefault(DOMSupport&		theDOMSupport);

	virtual
	~XPathSupportDefault();

	
	// These interfaces are inherited from DOMServices::WhitespaceSupport...

	virtual bool
	isIgnorableWhitespace(const DOM_Text&	node) const;

	
	// These intefaces are inherited from XPathSupport...

	virtual DOMString
	getNamespaceForPrefix(
			const DOMString&	prefix, 
			const DOM_Element&	namespaceContext) const;

	virtual DOMString
	getNamespaceOfNode(const DOM_Node&	n) const;

	virtual DOMString
	getLocalNameOfNode(const DOM_Node&	n) const;

	virtual DOMString
	getNodeData(const DOM_Node&		n) const;

	virtual DOM_Node
	getParentOfNode(const DOM_Node&		node) const;

	virtual DOM_Element
	getElementByID(
			const DOMString&		id,
			const DOM_Document&		doc) const;

	virtual void
	setProcessNamespaces(bool	processNamespaces);

	virtual bool
	getProcessNamespaces() const;

	virtual DOMString
	getUnparsedEntityURI(
			const DOMString&		theName,
			const DOM_Document&		theDocument) const;


	// These interfaces are inherited from Resettable...

	virtual void
	reset();

private:

	// These are not implemented...
	XPathSupportDefault(const XPathSupportDefault&);

	XPathSupportDefault&
	operator=(const XPathSupportDefault&);

	bool
	operator==(const XPathSupportDefault&);

	// Data members...
	DOMSupport&		m_DOMSupport;
};



#endif	// XPATHSUPPORTDEFAULT_HEADER_GUARD_1357924680
