/*
 * Copyright 1999-2004 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#if !defined(FORMATTERTOXERCESDOM_HEADER_GUARD_1357924680)
#define FORMATTERTOXERCESDOM_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



#include <xalanc/Include/XalanVector.hpp>



// Base class header file.
#include <xalanc/PlatformSupport/FormatterListener.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/XercesParserLiaison/XercesWrapperTypes.hpp>



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
			DOMElementType*				currentElement,
            MemoryManagerType&          theManager XALAN_DEFAULT_MEMMGR);

	/**
	 * Construct a FormatterToXercesDOM instance.  it will add the DOM nodes 
	 * to the document.
	 *
	 * @param doc  document for nodes
	 * @param elem current element for nodes
	 */
	FormatterToXercesDOM(
			DOMDocument_Type*			doc,
			DOMElementType*				currentElement,
            MemoryManagerType&          theManager XALAN_DEFAULT_MEMMGR);

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


	// Data members...
	DOMDocument_Type*				m_doc;

	DOMDocumentFragmentType*		m_docFrag;

	DOMElementType*					m_currentElem;

	typedef XalanVector<DOMElementType*>			ElementStackType;

	ElementStackType				m_elemStack;

	XalanDOMString					m_buffer;

	XalanDOMString					m_textBuffer;

	static const XalanDOMString		s_emptyString;
};



XALAN_CPP_NAMESPACE_END



#endif	// FORMATTERTODOM_HEADER_GUARD_1357924680
