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
#if !defined(FORMATTERTODOM_HEADER_GUARD_1357924680)
#define FORMATTERTODOM_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XMLSupport/XMLSupportDefinitions.hpp>



#include <stack>



#include <dom/DOM_Node.hpp>
#include <dom/DOM_Element.hpp>
#include <dom/DOM_Document.hpp>



// Base class header file.
#include <XMLSupport/FormatterListener.hpp>



/**
 * This class takes SAX events (in addition to some extra events that SAX
 * doesn't handle yet) and adds the result to a document or document fragment.
 */
class XALAN_XMLSUPPORT_EXPORT FormatterToDOM : public FormatterListener
{
public:

	/**
	 * Construct a FormatterToDOM instance.  it will add the DOM nodes 
	 * to the document fragment.
	 *
	 * @param doc            document for nodes
	 * @param docFrag        document fragment for nodes, default none
	 * @param currentElement current element for nodes, default none
	 */
	FormatterToDOM(
			const DOM_Document&				doc,
			const DOM_DocumentFragment&		docFrag = DOM_DocumentFragment(),
			const DOM_Element&				currentElement = DOM_Element());

	/**
	 * Construct a FormatterToDOM instance.  it will add the DOM nodes 
	 * to the document.
	 *
	 * @param doc  document for nodes
	 * @param elem current element for nodes
	 */
	FormatterToDOM(
			const DOM_Document&			doc,
			const DOM_Element&			elem);

	virtual
	~FormatterToDOM();


	// These methods are inherited from DocumentHandler ...

	virtual void
	charactersRaw(
			const XMLCh* const	chars,
			const unsigned int	length);

	virtual void
	comment(const XMLCh* const	data);

	virtual void
	cdata(
			const XMLCh* const	ch,
			const unsigned int 	length);

	virtual void
	entityReference(const XMLCh* const	name);

	virtual void
	setDocumentLocator(const Locator* const		locator);

	virtual void
	startDocument();

	virtual void
	endDocument();

	virtual void
	startElement(
				const	XMLCh* const	name,
				AttributeList&			attrs);

	virtual void
	endElement(const XMLCh* const	name);

	virtual void
	characters(
				const XMLCh* const	chars,
				const unsigned int	length);

	virtual void
	ignorableWhitespace(
				const XMLCh* const	chars,
				const unsigned int	length);

	virtual void
	processingInstruction(
			const XMLCh* const	target,
			const XMLCh* const	data);

	virtual void
	resetDocument();

private:

	/**
	 * Append a node to the current container.
	 */
	void
	append(const DOM_Node& 	newNode);


	// Data members...
	DOM_DocumentFragment		m_docFrag;

	DOM_Document				m_doc;
	DOM_Element					m_currentElem;

	std::stack<DOM_Element>		m_elemStack;
};



#endif	// FORMATTERTODOM_HEADER_GUARD_1357924680
