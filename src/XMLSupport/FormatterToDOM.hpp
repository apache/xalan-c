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
 * This class takes SAX events (in addition to some extra events 
 * that SAX doesn't handle yet) and adds the result to a document 
 * or document fragment.
 */
class XALAN_XMLSUPPORT_EXPORT FormatterToDOM : public FormatterListener
{
public:

	/**
	 * FormatterToDOM instance constructor... it will add the DOM nodes 
	 * to the document fragment.
	 */
	FormatterToDOM(
			const DOM_Document&				doc,
			const DOM_DocumentFragment&		docFrag = DOM_DocumentFragment(),
			const DOM_Element&				currentElement = DOM_Element());

	FormatterToDOM(
			const DOM_Document&			doc,
			const DOM_Element&			elem);

	virtual
	~FormatterToDOM();

public:

	/**
	 * Receive an object for locating the origin of SAX document events.
	 *
	 * <p>SAX parsers are strongly encouraged (though not absolutely
	 * required) to supply a locator: if it does so, it must supply
	 * the locator to the application by invoking this method before
	 * invoking any of the other methods in the DocumentHandler
	 * interface.</p>
	 *
	 * <p>The locator allows the application to determine the end
	 * position of any document-related event, even if the parser is
	 * not reporting an error.  Typically, the application will
	 * use this information for reporting its own errors (such as
	 * character content that does not match an application's
	 * business rules).  The information returned by the locator
	 * is probably not sufficient for use with a search engine.</p>
	 *
	 * <p>Note that the locator will return correct information only
	 * during the invocation of the events in this interface.  The
	 * application should not attempt to use it at any other time.</p>
	 *
	 * @param locator An object that can return the location of
	 *				any SAX document event.
	 * @see org.xml.sax.Locator
	 */
	virtual void
	setDocumentLocator(Locator* const	locator);

	/**
	 * Receive notification of the beginning of a document.
	 *
	 * <p>The SAX parser will invoke this method only once, before any
	 * other methods in this interface or in DTDHandler (except for
	 * setDocumentLocator).</p>
	 *
	 * @exception org.xml.sax.SAXException Any SAX exception, possibly
	 *			wrapping another exception.
	 */
	 virtual void
	 startDocument();

	/**
	 * Receive notification of the end of a document.
	 *
	 * <p>The SAX parser will invoke this method only once, and it will
	 * be the last method invoked during the parse.  The parser shall
	 * not invoke this method until it has either abandoned parsing
	 * (because of an unrecoverable error) or reached the end of
	 * input.</p>
	 *
	 * @exception org.xml.sax.SAXException Any SAX exception, possibly
	 *			wrapping another exception.
	 */
	virtual void
	endDocument();

	/**
	 * Receive notification of the beginning of an element.
	 *
	 * <p>The Parser will invoke this method at the beginning of every
	 * element in the XML document; there will be a corresponding
	 * endElement() event for every startElement() event (even when the
	 * element is empty). All of the element's content will be
	 * reported, in order, before the corresponding endElement()
	 * event.</p>
	 *
	 * <p>If the element name has a namespace prefix, the prefix will
	 * still be attached.  Note that the attribute list provided will
	 * contain only attributes with explicit values (specified or
	 * defaulted): #IMPLIED attributes will be omitted.</p>
	 *
	 * @param name The element type name.
	 * @param atts The attributes attached to the element, if any.
	 * @exception org.xml.sax.SAXException Any SAX exception, possibly
	 *			wrapping another exception.
	 * @see #endElement
	 * @see org.xml.sax.AttributeList 
	 */
	virtual void
	startElement(
			const	XMLCh* const	name,
			AttributeList&			attrs);

	/**
	 * Receive notification of the end of an element.
	 *
	 * <p>The SAX parser will invoke this method at the end of every
	 * element in the XML document; there will be a corresponding
	 * startElement() event for every endElement() event (even when the
	 * element is empty).</p>
	 *
	 * <p>If the element name has a namespace prefix, the prefix will
	 * still be attached to the name.</p>
	 *
	 * @param name The element type name
	 * @exception org.xml.sax.SAXException Any SAX exception, possibly
	 *			wrapping another exception.
	 */
    virtual void
	endElement(const XMLCh* const	name);

	/**
	 * Receive notification of character data.
	 *
	 * <p>The Parser will call this method to report each chunk of
	 * character data.  SAX parsers may return all contiguous character
	 * data in a single chunk, or they may split it into several
	 * chunks; however, all of the characters in any single event
	 * must come from the same external entity, so that the Locator
	 * provides useful information.</p>
	 *
	 * <p>The application must not attempt to read from the array
	 * outside of the specified range.</p>
	 *
	 * <p>Note that some parsers will report whitespace using the
	 * ignorableWhitespace() method rather than this one (validating
	 * parsers must do so).</p>
	 *
	 * @param ch The characters from the XML document.
	 * @param start The start position in the array.
	 * @param length The number of characters to read from the array.
	 * @exception org.xml.sax.SAXException Any SAX exception, possibly
	 *			wrapping another exception.
	 * @see #ignorableWhitespace 
	 * @see org.xml.sax.Locator
	 */
    virtual void
	characters(
			const XMLCh* const	chars,
			const unsigned int	length);

  /**
   * If available, when the disable-output-escaping attribute is used, 
   * output raw text without escaping.
   */
    virtual void
	charactersRaw(
			const XMLCh* const	chars,
			const unsigned int	length);

	/**
	 * Receive notivication of a entityReference.
	 */
	virtual void
	entityReference(const XMLCh* const	name);

	/**
	 * Receive notification of ignorable whitespace in element content.
	 *
	 * <p>Validating Parsers must use this method to report each chunk
	 * of ignorable whitespace (see the W3C XML 1.0 recommendation,
	 * section 2.10): non-validating parsers may also use this method
	 * if they are capable of parsing and using content models.</p>
	 *
	 * <p>SAX parsers may return all contiguous whitespace in a single
	 * chunk, or they may split it into several chunks; however, all of
	 * the characters in any single event must come from the same
	 * external entity, so that the Locator provides useful
	 * information.</p>
	 *
	 * <p>The application must not attempt to read from the array
	 * outside of the specified range.</p>
	 *
	 * @param ch The characters from the XML document.
	 * @param start The start position in the array.
	 * @param length The number of characters to read from the array.
	 * @exception org.xml.sax.SAXException Any SAX exception, possibly
	 *			wrapping another exception.
	 * @see #characters
	 */
	virtual void
	ignorableWhitespace(
			const XMLCh* const	chars,
			const unsigned int	length);

	/**
	 * Receive notification of a processing instruction.
	 *
	 * <p>The Parser will invoke this method once for each processing
	 * instruction found: note that processing instructions may occur
	 * before or after the main document element.</p>
	 *
	 * <p>A SAX parser should never report an XML declaration (XML 1.0,
	 * section 2.8) or a text declaration (XML 1.0, section 4.3.1)
	 * using this method.</p>
	 *
	 * @param target The processing instruction target.
	 * @param data The processing instruction data, or null if
	 *		none was supplied.
	 * @exception org.xml.sax.SAXException Any SAX exception, possibly
	 *			wrapping another exception.
	 */
	virtual void
	processingInstruction(
			const XMLCh* const	target,
			const XMLCh* const	data);


    virtual void
	resetDocument();

	/**
	 * Called when a Comment is to be constructed.
	 * @param   data	The comment data.
	 * @exception org.xml.sax.SAXException Any SAX exception, possibly
	 *			wrapping another exception.
	 */
	virtual void
	comment(const XMLCh* const	data);

	/**
	 * Receive notification of cdata.
	 *
	 * <p>The Parser will call this method to report each chunk of
	 * character data.  SAX parsers may return all contiguous character
	 * data in a single chunk, or they may split it into several
	 * chunks; however, all of the characters in any single event
	 * must come from the same external entity, so that the Locator
	 * provides useful information.</p>
	 *
	 * <p>The application must not attempt to read from the array
	 * outside of the specified range.</p>
	 *
	 * <p>Note that some parsers will report whitespace using the
	 * ignorableWhitespace() method rather than this one (validating
	 * parsers must do so).</p>
	 *
	 * @param ch The characters from the XML document.
	 * @param start The start position in the array.
	 * @param length The number of characters to read from the array.
	 * @exception org.xml.sax.SAXException Any SAX exception, possibly
	 *			wrapping another exception.
	 * @see #ignorableWhitespace 
	 * @see org.xml.sax.Locator
	 */
	virtual void
	cdata(
			const XMLCh* const	ch,
			const unsigned int 	length);

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
