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
#if !defined(FORMATTERTODEPRECATEDXERCESDOM_HEADER_GUARD_1357924680)
#define FORMATTERTODEPRECATEDXERCESDOM_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



#include <vector>



// Base class header file.
#include <xalanc/PlatformSupport/FormatterListener.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/XercesParserLiaison/Deprecated/XercesBridgeTypes.hpp>

// Required Xerces headers

#if XERCES_VERSION_MAJOR >= 2
#include <xercesc/dom/deprecated/DOM_Document.hpp>
#include <xercesc/dom/deprecated/DOM_DocumentFragment.hpp>
#include <xercesc/dom/deprecated/DOM_Element.hpp>
#else
#include <xercesc/dom/DOM_Document.hpp>
#include <xercesc/dom/DOM_DocumentFragment.hpp>
#include <xercesc/dom/DOM_Element.hpp>
#endif


XALAN_CPP_NAMESPACE_BEGIN



/**
 * This class takes SAX events (in addition to some extra events that SAX
 * doesn't handle yet) and adds the result to a document or document fragment.
 *
 * It is provided as assist developers transition away from the deprecated DOM.
 *
 * @deprecated The Xerces DOM bridge is deprecated.
 */
class XALAN_XERCESPARSERLIAISON_EXPORT FormatterToDeprecatedXercesDOM : public FormatterListener
{
public:

	/**
	 * Construct a FormatterToDeprecatedXercesDOM instance.  it will add the DOM nodes 
	 * to the document fragment.
	 *
	 * @deprecated This API is deprecated and is only provided as a transition tool
	 * @param doc            document for nodes
	 * @param docFrag        document fragment for nodes
	 * @param currentElement current element for nodes
	 */
	FormatterToDeprecatedXercesDOM(
			DOM_Document_Type			&doc,
			DOM_DocumentFragmentType	&docFrag,
			DOM_ElementType			    &currentElement);

	/**
	 * Construct a FormatterToDeprecatedXerces DOM instance.  it will add the DOM nodes 
	 * to the document.
	 *
	 * @deprecated This API is deprecated and is only provided as a transition tool
	 * @param doc  document for nodes
	 * @param elem current element for nodes
	 */
	FormatterToDeprecatedXercesDOM(
			DOM_Document_Type			&doc,
			DOM_ElementType 			&currentElement);

	/**
	 * Construct a FormatterToDeprecatedXercesDOM instance.  it will add the DOM nodes 
	 * to the document.
	 *
	 * @deprecated This API is deprecated and is only provided as a transition tool
	 * @param doc  document for nodes
	 */
	FormatterToDeprecatedXercesDOM(
 		    DOM_Document_Type			&doc);

	virtual
	~FormatterToDeprecatedXercesDOM();


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

	DOM_Document_Type
	getDocument() const
	{
		return m_doc;
	}

	void
	setDocument(DOM_Document_Type	&theDocument)
	{
		m_doc = theDocument;
	}

	DOM_DocumentFragmentType
	getDocumentFragment() const
	{
		return m_docFrag;
	}

	void
	setDocumentFragment(DOM_DocumentFragmentType	&theDocumentFragment)
	{
		m_docFrag = theDocumentFragment;
	}

	DOM_ElementType
	getCurrentElement() const
	{
		return m_currentElem;
	}

	void
	setCurrentElement(DOM_ElementType		&theElement)
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
	append(DOM_NodeType 	&newNode);

	/**
	 * Create the appropriate element, complete with attributes set.
	 *
	 * @param theElementName The name for the new element
	 * @param attrs The SAX AttributeList for the new attributes.
	 * @return A pointer to the new instance.
	 */
	DOM_ElementType
	createElement(
			const XalanDOMChar*		theElementName,
			AttributeListType&		attrs);

	void
	addAttributes(
			DOM_ElementType 		&theElement,
			AttributeListType&	attrs);


	// Data members...
	DOM_Document_Type				m_doc;

	DOM_DocumentFragmentType		m_docFrag;

	DOM_ElementType					m_currentElem;

#if defined(XALAN_NO_STD_NAMESPACE)
	typedef vector<DOM_ElementType>			ElementStackType;
#else
	typedef std::vector<DOM_ElementType>	ElementStackType;
#endif

	ElementStackType				m_elemStack;

	XalanDOMString					m_buffer;

	XalanDOMString					m_textBuffer;

	static const XalanDOMString		s_emptyString;
};



XALAN_CPP_NAMESPACE_END



#endif	// FORMATTERTODEPRECATEDXERCESDOM_HEADER_GUARD_1357924680
