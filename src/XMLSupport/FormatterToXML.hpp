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


class Writer;



/**
 * FormatterToXML formats SAX-style events into XML.
 */
class XALAN_XMLSUPPORT_EXPORT FormatterToXML : public FormatterListener 
{
public:

#if defined(XALAN_NO_NAMESPACES)
#	define XALAN_STD
#else
#	define XALAN_STD std::
#endif
	typedef XALAN_STD map<DOMString, DOMString> DOMStringMapType;
	typedef XALAN_STD map<DOMString, int> DOMString2IntMapType;
	typedef std::vector<QName> QNameVectorType;
	typedef std::stack<bool>	BoolStackTYpe;
#undef XALAN_STD

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

	/**
	 * If true, XML header should be written to output.
	 */
	bool						m_shouldWriteXMLHeader;

	/**
	 * Number of spaces to indent, default is 2.
	 */
	int							m_indent;

  /**
   * Tells the XML version, for writing out to the XML decl.
   */
	DOMString m_version;

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
	 * @param cdataSectionElems list of the names of elements whose text node
	 *                          children should be output using CDATA sections
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
   * Normalize the characters, but don't escape.
	*
	* @param ch pointer to characters from the XML document
	* @param start starting offset
	* @return length of array
	* @param isCData true if characters are CDATA
   */
	void writeNormalizedChars(
			const XMLCh*  const ch,
			int start, int length,
			bool isCData);

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

protected:

	void FormatterToXML::copyBigCharIntoBuf(
			const XMLCh* const chars,		// Character string to process
			int&					i,				// Index into 'chars'
			int&					pos,			// Index in m_charBuf
			int					length,		// Length of 'chars' string
			XMLCh* buf = m_charBuf			// Buffer to write to
			);

	void writeBigChar(const XMLCh* const ch, int& i, int end);

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
	BoolStackTYpe	m_elemStack;
  
	bool				m_ispreserve;
	BoolStackTYpe	m_preserves;
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
