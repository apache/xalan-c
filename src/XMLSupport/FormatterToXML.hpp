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



#include <XalanDOM/XalanDOMString.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/XalanUnicode.hpp>



// Base class header file.
#include <XMLSupport/FormatterListener.hpp>



class AttributeList;
class Writer;



/**
 * FormatterToXML formats SAX-style events into XML.
 */
class XALAN_XMLSUPPORT_EXPORT FormatterToXML : public FormatterListener 
{
public:

	enum eDummy
	{
		eDefaultIndentAmount = 4
	};

	/**
	 * Perform static initialization.  See class XMLSupportInit.
	 */
	static void
	initialize();

	/**
	 * Perform static shut down.  See class XMLSupportInit.
	 */
	static void
	terminate();

	/**
	 * Constructor for customized encoding and doctype.
	 *
	 * @param writer            character output stream to use
	 * @param version           version of the output method
	 * @param doIndent          true if output is to be indented
	 * @param indent            number of spaces to indent at each nesting level
	 * @param encoding          character encoding in use by <VAR>writer</VAR>
	 * @param mediaType         media type (MIME content type) of the data
	 * @param doctypeSystem     system identifier to be used in the document
	 *                          type declaration
	 * @param doctypePublic     public identifier to be used in the document
	 *                          type declaration
	 * @param xmlDecl           true if the XSLT processor should output an XML
	 *                          declaration
	 * @param standalone        true if the XSLT processor should output a
	 *                          standalone document declaration
	 *
	 * @param format			should be used only by derived classes.
	 */
	FormatterToXML(
			Writer&					writer,
			const XalanDOMString&	version = XalanDOMString(),
			bool					doIndent = false,
			int						indent = eDefaultIndentAmount,
			const XalanDOMString&	encoding = XalanDOMString(),
			const XalanDOMString&	mediaType = XalanDOMString(),
			const XalanDOMString&	doctypeSystem = XalanDOMString(),
			const XalanDOMString&	doctypePublic = XalanDOMString(),
			bool					xmlDecl = true,
			const XalanDOMString&	standalone = XalanDOMString(),
			eFormat					format = OUTPUT_METHOD_XML);

	virtual
	~FormatterToXML();


	// These methods are inherited from FormatterListener ...

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
	charactersRaw(
			const XMLCh* const	chars,
			const unsigned int	length);

	virtual void
	entityReference(const XMLCh* const	name);

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

	virtual void
	comment(const XMLCh* const	data);

	virtual void
	cdata(
			const XMLCh* const	ch,
			const unsigned int 	length);

#if 0
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

	static const XalanDOMString
	convertMime2JavaEncoding(const XalanDOMString&	mimeCharsetName);

	static const XalanDOMString
	convertJava2MimeEncoding(const XalanDOMString&	encoding);

#endif

	const Writer&
	getWriter() const
	{
		return m_writer;
	}

	Writer&
	getWriter()
	{
		return m_writer;
	}

	const XalanDOMString&
	getDoctypeSystem() const
	{
		return m_doctypeSystem;
	}

	const XalanDOMString&
	getDoctypePublic() const
	{
		return m_doctypePublic;
	}

	const XalanDOMString&
	getEncoding() const
	{
		return m_encoding;
	}

	const XalanDOMString&
	getMediaType() const
	{
		return m_mediaType;
	}

	const XalanDOMString&
	getVersion() const
	{
		return m_version;
	}

	const XalanDOMString&
	getStandalone() const
	{
		return m_standalone;
	}

	bool
	getShouldWriteXMLHeader() const
	{
		return m_shouldWriteXMLHeader;
	}

	void
	setShouldWriteXMLHeader(bool	b)
	{
		m_shouldWriteXMLHeader = b;
	}

	bool
	getStripCData() const
	{
		return m_stripCData;
	}

	void
	setStripCData(bool	b)
	{
		m_stripCData = b;
	}

	bool
	getEscapeCData() const
	{
		return m_escapeCData;
	}

	void
	setEscapeCData(bool	b)
	{
		m_escapeCData = b;
	}

	bool
	getDoIndent() const
	{
		return m_doIndent;
	}

	int
	getIndent() const
	{
		return m_indent;
	}

protected:

	/** 
	 * The writer where the XML will be written.
	 */
	Writer&					m_writer;

	/**
	 * Output a line break.
	 */
	void
	outputLineSep();

	/**
	 * Append a wide character to the buffer.
	 *
	 * @ch the character to append.
	 */
	void
	accum(XalanDOMChar	ch);

	/**
	 * Append a mull-termiated array of wide character to
	 * the buffer.
	 *
	 * @chars the array to append
	 */
	void
	accum(const XalanDOMChar*	chars);

	/**
	 * Append an array of wide character to the buffer.
	 *
	 * @chars the array to append
	 * @start the offset into the array to start from
	 * @length the number of characters to append
	 */
	void
	accum(
			const XalanDOMChar	chars[],
			unsigned int		start,
			unsigned int		length);

	/**
	 * Append a string to the buffer.
	 *
	 * @param str the string to append
	 */
	void
	accum(const XalanDOMString&		str);

	/**
	 * Append a vector of wide characters to the buffer.
	 *
	 * @param theVector the vector to append
	 */
	void
	accum(const XalanDOMCharVectorType&		theVector);

	/**
	 * Escape and accum a character.
	 */
	void
	accumDefaultEscape(
			XalanDOMChar		ch,
			unsigned int		i,
			const XalanDOMChar	chars[],
			unsigned int		len,
			bool				escLF);

	/**
	 * Handle one of the default entities, return false if it 
	 * is not a default entity.
	*/
	bool
	accumDefaultEntity(
			XalanDOMChar		ch,
			unsigned int		i,
			const XalanDOMChar	chars[],
			unsigned int		len,
			bool				escLF);

	/**
	 * Set the attribute characters what will require special mapping.
	 */
	void
	initAttrCharsMap();

	/**
	 * Set the output characters what will require special mapping.
	 */
	void
	initCharsMap();

	/**
	 * Flush the char buffer.
	 */
	void
	flushChars();

	void
	flush();

	void
	flushWriter();

	void
	openElementForChildren();

	bool
	childNodesWereAdded();

	bool
	shouldIndent() const
	{
		return m_doIndent && (!m_ispreserve && !m_isprevtext);
	}

	/**
	 * Check to see if a parent's ">" has been written, and, if 
	 * it has not, write it.
	 */
	void
	writeParentTagEnd();

	/**
	 * Prints a newline character and <var>n</var> spaces.
	 * @param n         Number of spaces to print.
	 */
	void
	indent(int	n);

	/**
	 * Write normalized characters to the writer.
	 * @param ch the string to write.
	 * @param start the start offset into the string.
	 * @param length the length of the string.
	 * @param isCData true if writing CDATA.
	 */
	virtual void
	writeNormalizedChars(
			const XalanDOMChar	ch[],
			unsigned int		start,
			unsigned int		length,
			bool				isCData);

	/**
	 * Write a number into the buffer as an entity
	 * reference.
	 * @param theNumber the number to write.
	 */
	void
	writeNumberedEntityReference(unsigned long	theNumber);

	/**
	 * Write an attribute string.
	 * @param string The string to write.
	 * @param encoding The current encoding.
	 */
	virtual void
	writeAttrString(
			const XalanDOMChar*		string,
			const XalanDOMString&	encoding);

	/**
	 * Throw an exception when an invalid
	 * surrogate is encountered.
	 * @param ch The first character in the surrogate
	 */
	static void
	throwInvalidUTF16SurrogateException(XalanDOMChar	ch);

	/**
	 * Throw an exception when an invalid
	 * surrogate is encountered.
	 * @param ch The first character in the surrogate
	 * @param next The next character in the surrogate
	 */
	static void
	throwInvalidUTF16SurrogateException(
			XalanDOMChar	ch,
			unsigned int	next);

	enum eDummyTwo { SPECIALSSIZE = 256};

	/**
	 * The maximum character size before we have to resort 
	 * to escaping.
	 */
	XalanDOMChar			m_maxCharacter;

	XalanDOMChar			m_attrCharsMap[SPECIALSSIZE];

	XalanDOMChar			m_charsMap[SPECIALSSIZE];

	/**
	 * If true, XML header should be written to output.
	 */
	bool		m_shouldWriteXMLHeader;

	/**
	 * State flag to tell if preservation of whitespace 
	 * is important.
	 */
	bool		m_ispreserve;

	/**
	 * Flag to tell if indenting (pretty-printing) is on.
	 */
	bool		m_doIndent;

	/**
	 * Flag to signal that a newline should be added.
	 */
	bool		m_startNewLine;
  
	/**
	 * Flag to tell that we need to add the doctype decl, 
	 * which we can't do until the first element is 
	 * encountered.
	 */
	bool		m_needToOutputDocTypeDecl;
  
	/**
	 * State flag that tells if the previous node processed 
	 * was text, so we can tell if we should preserve whitespace.
	 */
	bool		m_isprevtext;

	/**
	 * If true, cdata sections are simply stripped of their 
	 * CDATA brackets, without escaping.
	 */
	bool		m_stripCData;

	/**
	 * Tell if the next text should be raw.
	 */
	bool		m_nextIsRaw;

	/**
	 * Tells if we're in CData section.
	 */
	bool		m_inCData;

	/**
	 * Flag to quickly tell if the encoding is UTF8.
	 */
	bool		m_isUTF8;

	/**
	 * The System ID for the doc type.
	 */
	const XalanDOMString	m_doctypeSystem;

	/**
	 * The public ID for the doc type.
	 */
	const XalanDOMString	m_doctypePublic;

	/**
	 * The character encoding.
	 */
	XalanDOMString	m_encoding;

	/**
	 * Flag to keep track of the indent amount.
	 */
	int		m_currentIndent;
  
	/**
	 * Amount to indent.
	 */
	int		m_indent;

	/**
	 * The text "xslt-next-is-raw".
	 */
	static const XalanDOMCharVectorType&	s_xsltNextIsRawString;

	/**
	 * The string "formatter-to-dom".
	 */
	static const XalanDOMCharVectorType&	s_formatterToDOMString;

#if defined(XALAN_NO_NAMESPACES)
	typedef vector<bool>				BoolStackType;
	typedef vector<XalanDOMChar>		DOMCharBufferType;
	typedef vector<char>				ByteBufferType;
#else
	typedef std::vector<bool>			BoolStackType;
	typedef std::vector<XalanDOMChar>	DOMCharBufferType;
	typedef std::vector<char>			ByteBufferType;
#endif

	/**
	 * Stack to keep track of whether or not we need to 
	 * preserve whitespace.
	 */
	BoolStackType	m_preserves;

private:

	// These are not implemented.
	FormatterToXML(const FormatterToXML&);

	FormatterToXML&
	operator=(const FormatterToXML&);

	bool
	operator==(const FormatterToXML&) const;

	/**
	 * Initialize the static codings table.
	 */
	static void
	initEncodings();

	/**
	 * Output the doc type declaration.
	 *
	 * @param name the name of the doctype.
	 */
	void
	outputDocTypeDecl(const XalanDOMString&		name);

	/**
	 * Process an attribute.
	 * @param name The name of the attribute.
	 * @param value The value of the attribute.
	 */
	void
	processAttribute(
			const XalanDOMChar*		name,
			const XalanDOMChar*		value);

	/**
	 * Prints <var>n</var> spaces.
	 * @param n         Number of spaces to print.
	 */
	void
	printSpace(int n);

	/**
	 * Normalize the data in a PI, to replace any
	 * "?>" pairs with "? >"
	 * @param theData the data to normalize.
	 */
	void
	accumNormalizedPIData(
			const XalanDOMChar*		theData,
			unsigned int			theLength);


	// Data members...
	/**
	 * True if an encoding is only has only values from 0 - 255
	 */
	bool		m_bytesEqualChars;

	bool		m_shouldFlush;

	/**
	 * Add space before '/>' for XHTML.
	 */
	bool		m_spaceBeforeClose;

	/**
	 * If true, characters in cdata sections are 
	 * escaped, instead of being writted out as 
	 * cdata sections.
	 */
	bool		m_escapeCData;

	/**
	 * Tells if we're in an EntityRef event.
	 */
	bool		m_inEntityRef;

	/**
	 * Assume java encoding names are the same as the ISO encoding names if this is true.
	 */
	static bool		s_javaEncodingIsISO;

	/**
	 * Tells the XML version, for writing out to the XML decl.
	 */
	const XalanDOMString	m_version;

	/**
	 * Text for standalone part of header.
	 */
	const XalanDOMString	m_standalone;

	/**
	 * The media type.  Not used right now.
	 */
	const XalanDOMString	m_mediaType;

	/**
	 * These are characters that will be escaped in the output.
	 */
	const XalanDOMString	m_attrSpecialChars;

	/**
	 * The default MIME encoding.
	 */
	static const XalanDOMCharVectorType&	s_defaultMIMEEncoding;

	/**
	 * The string "<!DOCTYPE ".
	 */
	static const XalanDOMCharVectorType&	s_doctypeHeaderStartString;

	/**
	 * The string " PUBLIC \"".
	 */
	static const XalanDOMCharVectorType&	s_doctypeHeaderPublicString;

	/**
	 * The string " SYSTEM \"".
	 */
	static const XalanDOMCharVectorType&	s_doctypeHeaderSystemString;

	/**
	 * The string "<?xml version=\"".
	 */
	static const XalanDOMCharVectorType&	s_xmlHeaderStartString;

	/**
	 * The string "<?xml version=\"".
	 */
	static const XalanDOMCharVectorType&	s_xmlHeaderEncodingString;

	/**
	 * The string "\" standalone=\"".
	 */
	static const XalanDOMCharVectorType&	s_xmlHeaderStandaloneString;

	/**
	 * The string "\"?>".
	 */
	static const XalanDOMCharVectorType&	s_xmlHeaderEndString;

	/**
	 * The string "1.0".
	 */
	static const XalanDOMCharVectorType&	s_defaultVersionString;

	/**
	 * The string "WINDOWS-1250".
	 */
	static const XalanDOMCharVectorType&	s_windows1250EncodingString;

	/**
	 * The string "US-ASCII".
	 */
	static const XalanDOMCharVectorType&	s_usASCIIEncodingString;

	/**
	 * The string "ASCII".
	 */
	static const XalanDOMCharVectorType&	s_asciiEncodingString;

	/**
	 * The string "UTF-8".
	 */
	static const XalanDOMString&	s_utf8EncodingString;


	DOMCharBufferType				m_charBuf;

	DOMCharBufferType::size_type	m_pos;

	static const DOMCharBufferType::size_type	s_maxBufferSize;

	/**
	 * Current level of indent.
	 */
	int		m_level;

	/**
	 * A stack of Boolean objects that tell if the given element 
	 * has children.
	 */
	BoolStackType	m_elemStack;
};



#endif	// FORMATTERTOXML_HEADER_GUARD_1357924680
