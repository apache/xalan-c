/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 1999-2000 The Apache Software Foundation.	All rights
 * reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright
 *	  notice, this list of conditions and the following disclaimer. 
 * 
 * 2. Redistributions in binary form must reproduce the above copyright
 *	  notice, this list of conditions and the following disclaimer in
 *	  the documentation and/or other materials provided with the
 *	  distribution.
 * 
 * 3. The end-user documentation included with the redistribution,
 *	  if any, must include the following acknowledgment:  
 *		 "This product includes software developed by the
 *		  Apache Software Foundation (http://www.apache.org/)."
 *	  Alternately, this acknowledgment may appear in the software itself,
 *	  if and wherever such third-party acknowledgments normally appear.
 * 
 * 4. The names "Xerces" and "Apache Software Foundation" must
 *	  not be used to endorse or promote products derived from this
 *	  software without prior written permission. For written 
 *	  permission, please contact apache\@apache.org.
 * 
 * 5. Products derived from this software may not be called "Apache",
 *	  nor may "Apache" appear in their name, without prior written
 *	  permission of the Apache Software Foundation.
 * 
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.	IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
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
 * individuals on behalf of the Apache Software Foundation, and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

#if !defined(XALANSOURCETREECONTENTHANDLER_HEADER_GUARD_1357924680)
#define XALANSOURCETREECONTENTHANDLER_HEADER_GUARD_1357924680



#include <XalanSourceTree/XalanSourceTreeDefinitions.hpp>



#include <vector>



#include <xercesc/sax/DocumentHandler.hpp>
#include <xercesc/sax/DTDHandler.hpp>
#include <xercesc/sax2/ContentHandler.hpp>
#include <xercesc/sax2/LexicalHandler.hpp>



#include <XalanDOM/XalanDOMString.hpp>



class XalanNode;
class XalanSourceTreeDocument;
class XalanSourceTreeElement;



class XALAN_XALANSOURCETREE_EXPORT XalanSourceTreeContentHandler : public ContentHandler, public DTDHandler, public LexicalHandler
{
public:

#if defined(XALAN_NO_NAMESPACES)
	typedef vector<XalanSourceTreeElement*> 		ElementStackType;
	typedef vector<XalanNode*> 						LastChildStackType;
#else
	typedef std::vector<XalanSourceTreeElement*>	ElementStackType;
	typedef std::vector<XalanNode*> 				LastChildStackType;
#endif

	enum { eDefaultStackSize = 50, eDefaultTextBufferSize = 100 };


	// Constructor
	explicit
	XalanSourceTreeContentHandler(
			XalanSourceTreeDocument*	theDocument = 0,
			bool						fAccumulateText = true);

	virtual
	~XalanSourceTreeContentHandler();


	// Inherited from ContentHandler...
	virtual void
	characters(
			const XMLCh* const	chars,
			const unsigned int	length);

	virtual void
	endDocument();

	virtual void
	endElement(
			const XMLCh* const	uri, 
			const XMLCh* const	localname, 
			const XMLCh* const	qname);

	virtual void
	ignorableWhitespace(
			const XMLCh* const	chars,
			const unsigned int	length);

	virtual void
	processingInstruction(
		const XMLCh* const	target,
		const XMLCh* const	data);

	virtual void
	setDocumentLocator(const Locator* const 	locator);

	virtual void
	startDocument();

	virtual void
	startElement(
			const XMLCh* const	uri,
			const XMLCh* const	localname,
			const XMLCh* const	qname,
			const Attributes&	attrs);

	virtual void
	startPrefixMapping(
		const XMLCh* const	prefix,
		const XMLCh* const	uri);

	virtual void
	endPrefixMapping(const XMLCh* const 	prefix);


	virtual void
	skippedEntity(const XMLCh* const	name);


	// Inherited from DTDHandler...

	virtual void
	notationDecl(
			const XMLCh* const	  name,
			const XMLCh* const	  publicId,
			const XMLCh* const	  systemId);

	virtual void
	unparsedEntityDecl(
			const XMLCh* const	  name,
			const XMLCh* const	  publicId,
			const XMLCh* const	  systemId,
			const XMLCh* const	  notationName);

	virtual void
	resetDocType();


	// Inherited from LexicalHandler...

	virtual void
	comment(
			const XMLCh* const	chars,
			const unsigned int	length);

	virtual void
	endCDATA();

	virtual void
	endDTD();

	virtual void
	endEntity(const XMLCh* const	name);

	virtual void
	startCDATA();

	virtual void
	startDTD(
			const XMLCh* const	name,
			const XMLCh* const	publicId,
			const XMLCh* const	systemId);

	virtual void
	startEntity(const XMLCh* const	name);

	
	// New to XalanSourceTreeContentHandler...

	XalanSourceTreeDocument*
	getDocument() const
	{
		return m_document;
	}

	void
	setDocument(XalanSourceTreeDocument*	theDocument);

private:

	// Not implemented...
	XalanSourceTreeContentHandler(const XalanSourceTreeContentHandler&);

	XalanSourceTreeContentHandler&
	operator=(const XalanSourceTreeContentHandler&);

	bool
	operator==(const XalanSourceTreeContentHandler&) const;

	// Helper functions...
	XalanSourceTreeElement*
	createElement(
			const XMLCh* const			uri,
			const XMLCh* const			localname,
			const XMLCh* const			qname,
			const Attributes&			attrs,
			XalanSourceTreeElement* 	theOwnerElement);

	void
	processAccumulatedText();

	void
	doCharacters(
			const XMLCh*	chars,
			unsigned int	length);


	// Data members...

	// The current document we're building...
	XalanSourceTreeDocument*	m_document;

	// The current element...
	XalanSourceTreeElement* 	m_currentElement;

	// Stack of elements...
	ElementStackType			m_elementStack;

	// The last child appended to the current element.  This is
	// an important optimization, because XalanSourceTreeElement
	// does not have a pointer to its last child.  Without this,
	// appending a child becomes a linear search.
	XalanNode* 					m_lastChild;

	// Stack of last children appended.  There is a ono-to-one
	// correspondance to the entries in m_elementStack.
	LastChildStackType			m_lastChildStack;

	// If true, the handler will accumulate text from calls to
	// characters() until another event triggers the creation
	// of the node.
	const bool					m_accumulateText;

	// A buffer to hold accumulated text.
	XalanDOMString				m_textBuffer;

	// A flag to determine if the DTD is being processed.
	bool						m_inDTD;
};



#endif	// #if !defined(XALANSOURCETREECONTENTHANDLER_HEADER_GUARD_1357924680)
