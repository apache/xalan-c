/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2000 The Apache Software Foundation.  All rights 
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
#if !defined(FORMATTERTOSOURCETREE_HEADER_GUARD_1357924680)
#define FORMATTERTOSOURCETREE_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XalanSourceTree/XalanSourceTreeDefinitions.hpp>



#include <vector>



// Base class header file.
#include <XMLSupport/FormatterListener.hpp>



#include <XalanDOM/XalanDOMString.hpp>



class PrefixResolver;
class XalanDocument;
class XalanDocumentFragment;
class XalanElement;
class XalanNode;
class XalanSourceTreeDocument;
class XalanSourceTreeElement;



/**
 * This class takes SAX events (in addition to some extra events that SAX
 * doesn't handle yet) and adds the result to a document or document fragment.
 */
class XALAN_XALANSOURCETREE_EXPORT FormatterToSourceTree : public FormatterListener
{
public:

	/**
	 * Perform static initialization.  See class XalanSourceTreeInit.
	 */
	static void
	initialize();

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
	FormatterToSourceTree(XalanSourceTreeDocument*	theDocument = 0);

	/**
	 * Construct a FormatterToSourceTree instance.  it will add the nodes 
	 * to the document fragment.
	 *
	 * @param theDocument The document for nodes
	 * @param theDocumentFragment The document fragment for nodes
	 */
	FormatterToSourceTree(
			XalanSourceTreeDocument*	theDocument,
			XalanDocumentFragment*		theDocumentFragment);

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

	XalanDocumentFragment*
	getDocumentFragment() const
	{
		return m_documentFragment;
	}

	void
	setDocumentFragment(XalanDocumentFragment*	theDocumentFragment)
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

	const PrefixResolver*
	getPrefixResolver() const
	{
		return m_prefixResolver;
	}

	void
	setPrefixResolver(const PrefixResolver*		thePrefixResolver)
	{
		m_prefixResolver = thePrefixResolver;
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

	// Some utility functions...
	void
	processAccumulatedText();

	XalanSourceTreeElement*
	createElementNode(
			const XalanDOMChar*			name,
			AttributeList&				attrs,
			XalanSourceTreeElement*		theParentElement);

	void
	doCharacters(
			const XalanDOMChar*		chars,
			unsigned int			length);

	void
	doProcessingInstruction(
			const XalanDOMChar*		target,
			const XalanDOMChar*		data);


	// Data members...
	XalanSourceTreeDocument*						m_document;

	XalanDocumentFragment*							m_documentFragment;

	XalanSourceTreeElement*							m_currentElement;

#if defined(XALAN_NO_NAMESPACES)
	typedef vector<XalanSourceTreeElement*>			ElementStackType;
#else
	typedef std::vector<XalanSourceTreeElement*>	ElementStackType;
#endif

	ElementStackType								m_elementStack;

	XalanDOMString									m_textBuffer;

	const PrefixResolver*							m_prefixResolver;
};



#endif	// FORMATTERTOSOURCETREE_HEADER_GUARD_1357924680
