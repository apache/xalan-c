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
#if !defined(FORMATTERTOXML_UTF16_HEADER_GUARD_1357924680)
#define FORMATTERTOXML_UTF16_HEADER_GUARD_1357924680




// Base include file.  Must be first.
#include <xalanc/XMLSupport/XMLSupportDefinitions.hpp>



#include <vector>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



// Base class header file.
#include <xalanc/PlatformSupport/FormatterListener.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class Writer;



/**
 * FormatterToXML_UTF16 formats SAX-style events into XML.
 */
class XALAN_XMLSUPPORT_EXPORT FormatterToXML_UTF16 : public FormatterListener 
{
public:

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
	 * Constructor
	 *
	 * @param writer            the writer.
	 * @param version           the string to write for the XML version number.
	 * @param mediaType         media type (MIME content type) of the data
	 * @param doctypeSystem     system identifier to be used in the document
	 *                          type declaration
	 * @param doctypePublic     public identifier to be used in the document
	 *                          type declaration
	 * @param xmlDecl           true if the XSLT processor should output an XML
	 *                          declaration
	 * @param standalone        The string the XSLT processor should output for
	 *                          the standalone document declaration
	 */
	FormatterToXML_UTF16(
			Writer&					writer,
			const XalanDOMString&	version = XalanDOMString(),
			const XalanDOMString&	mediaType = XalanDOMString(),
			const XalanDOMString&	doctypeSystem = XalanDOMString(),
			const XalanDOMString&	doctypePublic = XalanDOMString(),
			bool					xmlDecl = true,
			const XalanDOMString&	standalone = XalanDOMString());

	virtual
	~FormatterToXML_UTF16();


	// These methods are inherited from FormatterListener ...

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

	virtual Writer*
	getWriter() const;

	virtual const XalanDOMString&
	getDoctypeSystem() const;

	virtual const XalanDOMString&
	getDoctypePublic() const;

	virtual const XalanDOMString&
	getEncoding() const;

	virtual const XalanDOMString&
	getMediaType() const;

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

#if defined(XALAN_NO_STD_NAMESPACE)
#if defined(XALAN_USE_DEQUE_FOR_VECTOR_BOOL)
	typedef deque<bool>			BoolStackType;
#else
	typedef vector<bool>		BoolStackType;
#endif
#else
#if defined(XALAN_USE_DEQUE_FOR_VECTOR_BOOL)
	typedef std::deque<bool>	BoolStackType;
#else
	typedef std::vector<bool>	BoolStackType;
#endif
#endif

protected:

	/** 
	 * The writer.
	 */
	Writer*					m_writer;

	/**
	 * Output a line break.
	 */
	void
	outputLineSep();

	/**
	 * Escape and write a character.
	 */
	void
	writeDefaultEscape(XalanDOMChar		ch);

	/**
	 * Escape and write a character in an attribute.
	 */
	void
	writeDefaultAttributeEscape(XalanDOMChar	ch);

	/**
	 * Handle one of the default entities, return false if it 
	 * is not a default entity.
	 */
	bool
	writeDefaultEntity(XalanDOMChar		ch);

	/**
	 * Handle one of the default entities, return false if it 
	 * is not a default entity.
	 */
	bool
	writeDefaultAttributeEntity(XalanDOMChar	ch);

	/**
	 * Write the data for a comment
	 * @param data The comment's data.
	 */
	void
	writeCommentData(const XalanDOMChar*	data);

	void
	flushWriter();

	void
	openElementForChildren();

	bool
	childNodesWereAdded();

	/**
	 * Check to see if a parent's ">" has been written, and, if 
	 * it has not, write it.
	 */
	void
	writeParentTagEnd();

	/**
	 * Write a normalized character to the stream.
	 * @param ch the character to write
	 */
	void
	writeNormalizedChar(XalanDOMChar	ch);

	/**
	 * Write characters for a CDATA section
	 *
	 * @param ch the string to write.
	 * @param length the length of the string.
	 */
	void
	writeCDATAChars(
			const XalanDOMChar			ch[],
			XalanDOMString::size_type	length);

	/**
	 * Write an attribute string.
	 *
	 * @param theString The string to write.
	 * @param theStringLength The length of the string.
	 */
	void
	writeAttrString(
			const XalanDOMChar*			theString,
			XalanDOMString::size_type	theStringLength);

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
			XalanDOMChar	next);

	/**
	 * If true, XML header should be written to output.
	 */
	bool		m_shouldWriteXMLHeader;

	/**
	 * Flag to tell that we need to add the doctype decl, 
	 * which we can't do until the first element is 
	 * encountered.
	 */
	bool		m_needToOutputDocTypeDecl;
  
	/**
	 * Tell if the next text should be raw.
	 */
	bool		m_nextIsRaw;

	/**
	 * The System ID for the doc type.
	 */
	const XalanDOMString	m_doctypeSystem;

	/**
	 * The public ID for the doc type.
	 */
	const XalanDOMString	m_doctypePublic;

private:

	// These are not implemented.
	FormatterToXML_UTF16(const FormatterToXML_UTF16&);

	FormatterToXML_UTF16&
	operator=(const FormatterToXML_UTF16&);

	bool
	operator==(const FormatterToXML_UTF16&) const;

	void
	write(
			const XalanDOMChar*			theChars,
			XalanDOMString::size_type	theLength);

	void
	writeName(const XalanDOMChar*	theChars);

	void
	write(const XalanDOMChar*	theChars)
	{
		write(theChars, XalanDOMString::length(theChars));
	}

	void
	write(XalanDOMChar	theChar)
	{
		write(&theChar, 1);
	}

	void
	write(const XalanDOMString&		theChars)
	{
		write(theChars.c_str(), theChars.length());
	}

	/**
	 * Output the doc type declaration.
	 *
	 * @param name the name of the doctype.
	 */
	void
	outputDocTypeDecl(const XalanDOMChar*	name);

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
	 * Normalize the data in a PI, to replace any
	 * "?>" pairs with "? >"
	 * @param theData the data to normalize.
	 */
	void
	writeNormalizedPIData(
			const XalanDOMChar*			theData,
			XalanDOMString::size_type	theLength);

	void
	flushBuffer();

	// Data members...
	/**
	 * Add space before '/>' for XHTML.
	 */
	bool		m_spaceBeforeClose;

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
	 * The string "UTF-16".
	 */
	static const XalanDOMString&	s_utf16String;

	/**
	 * The string "<!DOCTYPE ".
	 */
	static const XalanDOMChar				s_doctypeHeaderStartString[];

	static const XalanDOMString::size_type	s_doctypeHeaderStartStringLength;

	/**
	 * The string " PUBLIC \"".
	 */
	static const XalanDOMChar				s_doctypeHeaderPublicString[];

	static const XalanDOMString::size_type	s_doctypeHeaderPublicStringLength;

	/**
	 * The string " SYSTEM \"".
	 */
	static const XalanDOMChar				s_doctypeHeaderSystemString[];

	static const XalanDOMString::size_type	s_doctypeHeaderSystemStringLength;

	/**
	 * The string "<?xml version=\"".
	 */
	static const XalanDOMChar				s_xmlHeaderStartString[];

	static const XalanDOMString::size_type	s_xmlHeaderStartStringLength;

	/**
	 * The string "\" encoding=\"".
	 */
	static const XalanDOMChar				s_xmlHeaderEncodingString[];

	static const XalanDOMString::size_type	s_xmlHeaderEncodingStringLength;

	/**
	 * The string "\" standalone=\"".
	 */
	static const XalanDOMChar				s_xmlHeaderStandaloneString[];

	static const XalanDOMString::size_type	s_xmlHeaderStandaloneStringLength;

	/**
	 * The string "\"?>".
	 */
	static const XalanDOMChar				s_xmlHeaderEndString[];

	static const XalanDOMString::size_type	s_xmlHeaderEndStringLength;

	/**
	 * The string "1.0".
	 */
	static const XalanDOMChar				s_defaultVersionString[];

	static const XalanDOMString::size_type	s_defaultVersionStringLength;

	/**
 	 * The string "-//W3C//DTD XHTML".
 	 */
 	static const XalanDOMChar				s_xhtmlDocTypeString[];

	static const XalanDOMString::size_type	s_xhtmlDocTypeStringLength;

	/**
 	 * The string "<![CDATA[".
 	 */
 	static const XalanDOMChar				s_cdataOpenString[];

	static const XalanDOMString::size_type	s_cdataOpenStringLength;

	/**
 	 * The string "]]>".
 	 */
 	static const XalanDOMChar				s_cdataCloseString[];

	static const XalanDOMString::size_type	s_cdataCloseStringLength;

	/**
 	 * The string "&lt;".
 	 */
 	static const XalanDOMChar				s_lessThanEntityString[];

	static const XalanDOMString::size_type	s_lessThanEntityStringLength;

	/**
 	 * The string "&gt;".
 	 */
 	static const XalanDOMChar				s_greaterThanEntityString[];

	static const XalanDOMString::size_type	s_greaterThanEntityStringLength;

	/**
 	 * The string "&amp;".
 	 */
 	static const XalanDOMChar				s_ampersandEntityString[];

	static const XalanDOMString::size_type	s_ampersandEntityStringLength;

	/**
 	 * The string "&quot;".
 	 */
 	static const XalanDOMChar				s_quoteEntityString[];

	static const XalanDOMString::size_type	s_quoteEntityStringLength;

	/**
 	 * The string "&#10;".
 	 */
 	static const XalanDOMChar				s_linefeedNCRString[];

	static const XalanDOMString::size_type	s_linefeedNCRStringLength;

	/**
	 * A stack of Boolean objects that tell if the given element 
	 * has children.
	 */
	BoolStackType	m_elemStack;

	/**
	 * The string of characters that represents the newline
	 */
	const XalanDOMChar*		m_newlineString;

	/**
	 * The length of the the string of characters that represents the newline
	 */
	XalanDOMString::size_type	m_newlineStringLength;


	enum
	{
			kNotSpecial = 0,
			kContentSpecial = 1,	// A flag to indicate a value in s_specialChars applies to content
			kAttributeSpecial = 2,	// A flag to indicate a value in s_specialChars applies to attributes
			kBothSpecial = 3,		// A flag t0 indicate a value in s_specialChars applies to both content and attributes
			kSpecialsSize = 0x80,	// The size of s_specialChars
			kBufferSize = 512		// The size of the buffer
	};

	static bool
	isContentSpecial(XalanDOMChar	theChar)
	{
		if ((theChar < kSpecialsSize) && (s_specialChars[theChar] & kContentSpecial))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	static bool
	isAttributeSpecial(XalanDOMChar		theChar)
	{
		if ((theChar < kSpecialsSize) && (s_specialChars[theChar] & kAttributeSpecial))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	static bool
	isUTF16HighSurrogate(XalanDOMChar	theChar)
	{
		return 0xD800u <= theChar && theChar <= 0xDBFFu ? true : false;
	}

	static bool
	isUTF16LowSurrogate(XalanDOMChar	theChar)
	{
		return 0xDC00u <= theChar && theChar <= 0xDFFFu ? true : false;
	}

	XalanDOMChar				m_buffer[kBufferSize];

	XalanDOMChar*				m_bufferPosition;

	XalanDOMString::size_type					m_bufferRemaining;

	static const XalanDOMChar	s_specialChars[];
};



XALAN_CPP_NAMESPACE_END



#endif	// FORMATTERTOXML_UTF16_HEADER_GUARD_1357924680
