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
#if !defined(FORMATTERTODOM_HEADER_GUARD_1357924680)
#define FORMATTERTODOM_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XMLSupport/XMLSupportDefinitions.hpp>



#include <xalanc/Include/XalanVector.hpp>



// Base class header file.
#include <xalanc/PlatformSupport/FormatterListener.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XalanDocument;
class XalanDocumentFragment;
class XalanElement;
class XalanNode;



/**
 * This class takes SAX events (in addition to some extra events that SAX
 * doesn't handle yet) and adds the result to a document or document fragment.
 *
 * This class is deprecated and will be removed!!!!
 */
class XALAN_XMLSUPPORT_EXPORT FormatterToDOM : public FormatterListener
{
public:

	typedef XalanVector<XalanElement*>		ElementStackType;

	/**
	 * Construct a FormatterToDOM instance.  it will add the DOM nodes 
	 * to the document fragment.
	 *
	 * @param doc            document for nodes
	 * @param docFrag        document fragment for nodes
	 * @param currentElement current element for nodes
	 */
	FormatterToDOM(
			XalanDocument*			doc,
			XalanDocumentFragment*	docFrag,
			XalanElement*			currentElement,
            MemoryManagerType&      theManager XALAN_DEFAULT_MEMMGR);

	/**
	 * Construct a FormatterToDOM instance.  it will add the DOM nodes 
	 * to the document.
	 *
	 * @param doc  document for nodes
	 * @param elem current element for nodes
	 */
	FormatterToDOM(
			XalanDocument*	        doc,
			XalanElement*	        elem,
            MemoryManagerType&      theManager XALAN_DEFAULT_MEMMGR);

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

	XalanDocument*
	getDocument() const
	{
		return m_doc;
	}

	void
	setDocument(XalanDocument*	theDocument)
	{
		m_doc = theDocument;
	}

	XalanDocumentFragment*
	getDocumentFragment() const
	{
		return m_docFrag;
	}

	void
	setDocumentFragment(XalanDocumentFragment*	theDocumentFragment)
	{
		m_docFrag = theDocumentFragment;
	}

	XalanElement*
	getCurrentElement() const
	{
		return m_currentElem;
	}

	void
	setCurrentElement(XalanElement*		theElement)
	{
		m_currentElem = theElement;
	}

private:

	/**
	 * Append a node to the current container.
	 */
	void
	append(XalanNode* 	newNode);

	/**
	 * Create the appropriate element, complete with attributes set.
	 *
	 * @param theElementName The name for the new element
	 * @param attrs The SAX AttributeList for the new attributes.
	 * @return A pointer to the new instance.
	 */
	XalanElement*
	createElement(
			const XalanDOMChar*		theElementName,
			AttributeListType&		attrs);

	void
	addAttributes(
			XalanElement*		theElement,
			AttributeListType&	attrs);


	// Data members...
	XalanDocument*					m_doc;

	XalanDocumentFragment*			m_docFrag;

	XalanElement*					m_currentElem;

	ElementStackType				m_elemStack;

	XalanDOMString					m_buffer1;

	XalanDOMString					m_buffer2;

	static const XalanDOMString		s_emptyString;
};



XALAN_CPP_NAMESPACE_END



#endif	// FORMATTERTODOM_HEADER_GUARD_1357924680
