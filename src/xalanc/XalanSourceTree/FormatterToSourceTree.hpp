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
#if !defined(FORMATTERTOSOURCETREE_HEADER_GUARD_1357924680)
#define FORMATTERTOSOURCETREE_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XalanSourceTree/XalanSourceTreeDefinitions.hpp>



#include <xalanc/Include/XalanVector.hpp>



// Base class header file.
#include <xalanc/PlatformSupport/FormatterListener.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class PrefixResolver;
class XalanNode;
class XalanSourceTreeDocument;
class XalanSourceTreeDocumentFragment;
class XalanSourceTreeElement;



/**
 * This class takes SAX events (in addition to some extra events that SAX
 * doesn't handle yet) and adds the result to a document or document fragment.
 */
class XALAN_XALANSOURCETREE_EXPORT FormatterToSourceTree : public FormatterListener
{
public:

	typedef XalanVector<XalanSourceTreeElement*>			ElementStackType;
	typedef XalanVector<XalanNode*> 						LastChildStackType;

	enum { eDefaultStackSize = 50, eDefaultTextBufferSize = 100 };

	/**
	 * Perform static initialization.  See class XalanSourceTreeInit.
	 */
	static void
	initialize(MemoryManagerType& theManager);

	/**
	 * Perform static shut down.  See class XalanSourceTreeInit.
	 */
	static void
	terminate();


	/**
	 * Construct a FormatterToSourceTree instance.  it will add the nodes 
	 * to the document.
	 *
	 * @param theDocument The document for nodes
	 */
	explicit
	FormatterToSourceTree(  MemoryManagerType&          theManager XALAN_DEFAULT_CONSTRACTOR_MEMORY_MGR,
                            XalanSourceTreeDocument*	theDocument = 0);

	/**
	 * Construct a FormatterToSourceTree instance.  it will add the nodes 
	 * to the document fragment.
	 *
	 * @param theDocument The document for nodes
	 * @param theDocumentFragment The document fragment for nodes
	 */
	FormatterToSourceTree(
			XalanSourceTreeDocument*			theDocument,
			XalanSourceTreeDocumentFragment*	theDocumentFragment,
            MemoryManagerType&                  theManager XALAN_DEFAULT_MEMMGR);

	virtual
	~FormatterToSourceTree();


	XalanSourceTreeDocument*
	getDocument() const
	{
		return m_document;
	}

	void
	setDocument(XalanSourceTreeDocument*	theDocument)
	{
		m_document = theDocument;
	}

	XalanSourceTreeDocumentFragment*
	getDocumentFragment() const
	{
		return m_documentFragment;
	}

	void
	setDocumentFragment(XalanSourceTreeDocumentFragment*	theDocumentFragment)
	{
		m_documentFragment = theDocumentFragment;
	}

	XalanSourceTreeElement*
	getCurrentElement() const
	{
		return m_currentElement;
	}

	void
	setCurrentElement(XalanSourceTreeElement*	theElement)
	{
		m_currentElement = theElement;
	}

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
				const XMLCh* const	name,
				AttributeListType&	attrs);

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

	// Some utility functions...
	void
	processAccumulatedText();

	XalanSourceTreeElement*
	createElementNode(
			const XalanDOMChar*			name,
			AttributeListType&			attrs,
			XalanSourceTreeElement*		theParentElement);

	void
	doCharacters(
			const XalanDOMChar*			chars,
			XalanDOMString::size_type	length);

	void
	doProcessingInstruction(
			const XalanDOMChar*		target,
			const XalanDOMChar*		data);


	// Data members...
	XalanSourceTreeDocument*			m_document;

	XalanSourceTreeDocumentFragment*	m_documentFragment;

	XalanSourceTreeElement*				m_currentElement;

	ElementStackType					m_elementStack;

	// The last child appended to the current element.  This is
	// an important optimization, because XalanSourceTreeElement
	// does not have a pointer to it's last child.  Without this,
	// appending a child becomes a linear search.
	XalanNode* 							m_lastChild;

	// Stack of last children appended.  There is a one-to-one
	// correspondance to the entries in m_elementStack.
	LastChildStackType					m_lastChildStack;

	XalanDOMString						m_textBuffer;
};



XALAN_CPP_NAMESPACE_END



#endif	// FORMATTERTOSOURCETREE_HEADER_GUARD_1357924680
