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
#if !defined(FORMATTERTOXML_HEADER_GUARD_1357924680)
#define FORMATTERTOXML_HEADER_GUARD_1357924680




// Base include file.  Must be first.
#include <XMLSupport/XMLSupportDefinitions.hpp>



#include <stack>
#include <vector>
#include <map>



#include <dom/DOMString.hpp>

#include <sax/AttributeList.hpp>

// Base class header file.
#include <XMLSupport/FormatterListener.hpp>

#include <XPath/QName.hpp>

typedef std::vector<QName> QNameVectorType;


class Writer;



/**
 * FormatterToXML formats SAX-style events into XML.
 */
class XALAN_XMLSUPPORT_EXPORT FormatterToXML : public FormatterListener 
{
public:

	typedef std::map<DOMString, DOMString> DOMStringMapType;
	typedef std::map<DOMString, int> DOMString2IntMapType;

	/**
	 * These are characters that will be escaped in the output.
	 */
	DOMString					m_attrSpecialChars;

	static const DOMString DEFAULT_MIME_ENCODING;
	  
	/**
	 * If true, cdata sections are simply stripped of their 
	 * CDATA brackets, without escaping.
	 */
	bool						m_stripCData;

	/**
	 * If true, characters in cdata sections are 
	 * escaped, instead of being writted out as 
	 * cdata sections.
	 */
	bool						m_escapeCData;

	bool						m_shouldWriteXMLHeader;

	/**
	 *
	 * Number of spaces to indent.  The default is 2
	 */

	int							m_indent;

  /**
   * Tells the XML version, for writing out to the XML decl.
   */
	DOMString m_version;

	/**
	 * Constructor for customized encoding and doctype.
	 * @param writer        The character output stream to use.
	 * @param encoding      Java character encoding in use by <VAR>writer</VAR>.
	 * @param doctype       String to be printed at the top of the document.
	 * @param indent        Number of spaces to indent at each nesting level.
	 */
	FormatterToXML(
			Writer&				writer,
			const DOMString& version,
			bool doIndent, 
			int indent,
			const DOMString& encoding, 
			const DOMString& mediaType,
			const DOMString& doctypeSystem,
			const DOMString& doctypePublic,
			bool xmlDecl,
			const DOMString& standalone, 
		  const QNameVectorType* const cdataSectionElems);

	virtual
	~FormatterToXML();

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
   * Normalize the characters, but don't escape.
   */
	void writeNormalizedChars(
			const XMLCh*  const ch,
			int start, int length,
			bool isCData);

	void FormatterToXML::copyBigCharIntoBuf(
			const XMLCh* const chars,		// Character string to process
			int&					i,				// Index into 'chars'
			int&					pos,			// Index in m_charBuf
			int					length,		// Length of 'chars' string
			XMLCh* buf = m_charBuf			// Buffer to write to
			);

	void writeBigChar(const XMLCh* const ch, int& i, int end);

	/**
	 * Receive notification of a entityReference.
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

protected:

	void
	writeParentTagEnd();
  
	void
	openElementForChildren();

	bool
	childNodesWereAdded();

	/**
	 * Process an attribute.
	 * @param   name   The name of the attribute.
	 * @param   value   The value of the attribute.
	 */
	void
	processAttribute(
			const DOMString&	name,
			const DOMString&	value);

	/**
	 * Returns the specified <var>string</var> after substituting <VAR>specials</VAR>,
	 * and UTF-16 surrogates for chracter references <CODE>&amp;#xnn</CODE>.
	 *
	 * @param   string      String to convert to XML format.
	 * @param   specials    Chracters, should be represeted in chracter referenfces.
	 * @param   encoding    CURRENTLY NOT IMPLEMENTED.
	 * @return              XML-formatted string.
	 * @see #backReference
	 */
	static DOMString
	prepAttrString(
			const DOMString&	string,
			const DOMString&	specials,
			const DOMString&	encoding);
	
	virtual bool
	shouldIndent() const;
  
	/**
	 * Prints <var>n</var> spaces.
	 * @param pw        The character output stream to use.
	 * @param n         Number of spaces to print.
	 * @exception IOException   Thrown if <var>pw</var> is invalid.
	 */
	static void
	printSpace(
			Writer&		pw,
			int			n);

	/**
	 * Prints a newline character and <var>n</var> spaces.
	 * @param pw        The character output stream to use.
	 * @param n         Number of spaces to print.
	 * @exception IOException   Thrown if <var>pw</var> is invalid.
	 */
	void
	indent(
			Writer&		pw,
			int			n);
  /**
   * Convert a MIME charset name, also known as an XML encoding name, to a Java encoding name.
   * @param   mimeCharsetName Case insensitive MIME charset name: <code>UTF-8, US-ASCII, ISO-8859-1,
   *                          ISO-8859-2, ISO-8859-3, ISO-8859-4, ISO-8859-5, ISO-8859-6,
   *                          ISO-8859-7, ISO-8859-8, ISO-8859-9, ISO-2022-JP, Shift_JIS, 
   *                          EUC-JP, GB2312, Big5, EUC-KR, ISO-2022-KR, KOI8-R,
   *                          EBCDIC-CP-US, EBCDIC-CP-CA, EBCDIC-CP-NL, EBCDIC-CP-DK,
   *                          EBCDIC-CP-NO, EBCDIC-CP-FI, EBCDIC-CP-SE, EBCDIC-CP-IT,
   *                          EBCDIC-CP-ES, EBCDIC-CP-GB, EBCDIC-CP-FR, EBCDIC-CP-AR1,
   *                          EBCDIC-CP-HE, EBCDIC-CP-CH, EBCDIC-CP-ROECE, EBCDIC-CP-YU,
   *                          EBCDIC-CP-IS and EBCDIC-CP-AR2</code>.
   * @return                  Java encoding name, or <var>null</var> if <var>mimeCharsetName</var>
   *                          is unknown.
   * @see #reverse
   */

	static const DOMString convertMime2JavaEncoding(const DOMString& mimeCharsetName);
	static const DOMString convertJava2MimeEncoding(const DOMString& encoding);
	static void initEncodings();

protected:

	enum { MAXCHARBUF = 4096, MAXSAFECHARBUF = MAXCHARBUF - 256 };

	static XMLCh		m_charBuf[MAXCHARBUF];
	static XMLCh m_lineSep;
	bool m_needToOutputDocTypeDecl;
	Writer&				m_writer;
/**
 * The maximum character value before we have to resort 
 * to escaping.
 */
	unsigned int	m_maxCharacter;
  
	/**
	 * The character encoding.  Not currently used.
	 */
	DOMString		m_encoding;

	/**
	 * Tell if the next text should be raw.
	 */
	bool m_nextIsRaw;

  /**
   * A stack of Boolean objects that tell if the given element 
   * has children.
   */
	std::stack<bool>	m_elemStack;
  
	bool				m_ispreserve;
	std::stack<bool>	m_preserves;
	bool				m_isprevtext;
	bool				m_doIndent;
	int					m_currentIndent;
	int					m_level;
	DOMString			m_doctypeSystem;
	DOMString			m_doctypePublic;
	bool				m_startNewLine;
  /**
   * Assume java encoding names are the same as the ISO encoding names if this is true.
   */
	static bool m_javaEncodingIsISO;

	/**
	 * Flag to quickly tell if the encoding is UTF8.
	 */
	bool m_isUTF8;

  /**
   * Add space before '/>' for XHTML.
   */
  bool     m_spaceBeforeClose;

	static DOMStringMapType s_enchash;
	static DOMStringMapType s_revhash;

	static DOMString2IntMapType s_revsize;
  

	int copyEntityIntoBuf(
			const DOMString&	s,
			int					pos);

	int copyUTF16IntoBuf(
			const XMLCh* const chars,
			int&					i,				// Index into 'chars'
			int&					pos,			// Index in m_charBuf
			int					length		// Length of 'chars' string
		);
	
private:

	// These are not implemented.
	FormatterToXML(const FormatterToXML&);

	FormatterToXML&
	operator=(const FormatterToXML&);

	bool
	operator==(const FormatterToXML&) const;
};



#endif	// FORMATTERTOXML_HEADER_GUARD_1357924680
