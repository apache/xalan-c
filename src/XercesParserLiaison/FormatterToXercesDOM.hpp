/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2003 The Apache Software Foundation.  All rights 
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
#if !defined(FORMATTERTOXERCESDOM_HEADER_GUARD_1357924680)
#define FORMATTERTOXERCESDOM_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



#include <vector>



// Base class header file.
#include <PlatformSupport/FormatterListener.hpp>



#include <XalanDOM/XalanDOMString.hpp>



#include <XercesParserLiaison/XercesWrapperTypes.hpp>



XALAN_CPP_NAMESPACE_BEGIN



/**
 * This class takes SAX events (in addition to some extra events that SAX
 * doesn't handle yet) and adds the result to a document or document fragment.
 */
class XALAN_XERCESPARSERLIAISON_EXPORT FormatterToXercesDOM : public FormatterListener
{
public:

	/**
	 * Construct a FormatterToXercesDOM instance.  it will add the DOM nodes 
	 * to the document fragment.
	 *
	 * @param doc            document for nodes
	 * @param docFrag        document fragment for nodes
	 * @param currentElement current element for nodes
	 */
	FormatterToXercesDOM(
			DOMDocument_Type*			doc,
			DOMDocumentFragmentType*	docFrag,
			DOMElementType*				currentElement);

	/**
	 * Construct a FormatterToDOM instance.  it will add the DOM nodes 
	 * to the document.
	 *
	 * @param doc  document for nodes
	 * @param elem current element for nodes
	 */
	FormatterToXercesDOM(
			DOMDocument_Type*			doc,
			DOMElementType*				currentElement);

	virtual
	~FormatterToXercesDOM();


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
	setDocumentLocator(const LocatorType* const		locator);

	virtual void
	startDocument();

	virtual void
	endDocument();

	virtual void
	startElement(
				const	XMLCh* const	name,
				AttributeListType&		attrs);

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

	DOMDocument_Type*
	getDocument() const
	{
		return m_doc;
	}

	void
	setDocument(DOMDocument_Type*	theDocument)
	{
		m_doc = theDocument;
	}

	DOMDocumentFragmentType*
	getDocumentFragment() const
	{
		return m_docFrag;
	}

	void
	setDocumentFragment(DOMDocumentFragmentType*	theDocumentFragment)
	{
		m_docFrag = theDocumentFragment;
	}

	DOMElementType*
	getCurrentElement() const
	{
		return m_currentElem;
	}

	void
	setCurrentElement(DOMElementType*		theElement)
	{
		m_currentElem = theElement;
	}

private:

	/**
	 * Process any accumulated text and create a node for it.
	 */
	void
	processAccumulatedText();

	/**
	 * Append a node to the current container.
	 */
	void
	append(DOMNodeType* 	newNode);

	/**
	 * Create the appropriate element, complete with attributes set.
	 *
	 * @param theElementName The name for the new element
	 * @param attrs The SAX AttributeList for the new attributes.
	 * @return A pointer to the new instance.
	 */
	DOMElementType*
	createElement(
			const XalanDOMChar*		theElementName,
			AttributeListType&		attrs);

	void
	addAttributes(
			DOMElementType*		theElement,
			AttributeListType&	attrs);

	const XalanDOMString*
	getNamespaceForPrefix(
			const XalanDOMChar*		theName,
			const PrefixResolver&	thePrefixResolver,
			XalanDOMString&			thePrefix);


	// Data members...
	DOMDocument_Type*				m_doc;

	DOMDocumentFragmentType*		m_docFrag;

	DOMElementType*					m_currentElem;

#if defined(XALAN_NO_STD_NAMESPACE)
	typedef vector<DOMElementType*>			ElementStackType;
#else
	typedef std::vector<DOMElementType*>	ElementStackType;
#endif

	ElementStackType				m_elemStack;

	XalanDOMString					m_buffer;

	XalanDOMString					m_textBuffer;

	static const XalanDOMString		s_emptyString;
};



XALAN_CPP_NAMESPACE_END



#endif	// FORMATTERTODOM_HEADER_GUARD_1357924680
