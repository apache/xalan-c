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
#if !defined(FORMATTERTOXML_UTF16_HEADER_GUARD_1357924680)
#define FORMATTERTOXML_UTF16_HEADER_GUARD_1357924680




// Base include file.  Must be first.
#include <xalanc/XMLSupport/XMLSupportDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



// Base class header file.
#include "FormatterToXMLBase.hpp"



XALAN_CPP_NAMESPACE_BEGIN



/**
 * FormatterToXML_UTF16 formats SAX-style events into XML.
 */
class XALAN_XMLSUPPORT_EXPORT FormatterToXML_UTF16 : public FormatterToXMLBase 
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
	startElement(
			const XMLCh* const	name,
			AttributeListType&	attrs);

    virtual void
	endElement(const XMLCh* const	name);

    virtual void
	charactersRaw(
			const XMLCh* const	chars,
			const unsigned int	length);

	virtual void
	entityReference(const XMLCh* const	name);

	virtual void
	comment(const XMLCh* const	data);

	virtual const XalanDOMString&
	getEncoding() const;

protected:

	virtual void
	writeXMLHeader();

	virtual void
	flushBuffer();

	virtual void
	writeDoctypeDecl(const XalanDOMChar*	name);

	virtual void
	writeProcessingInstruction(
			const XMLCh*	target,
			const XMLCh*	data);

	virtual void
	writeCharacters(
			const XMLCh*	chars,
			unsigned int	length);

	virtual void
	writeCDATA(
			const XMLCh*	chars,
			unsigned int	length);

	virtual void
	outputNewline();

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

	// Data members...

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
 	 * The string "&#13;".
 	 */
 	static const XalanDOMChar			    s_carriageReturnNCRString[];

	static const XalanDOMString::size_type	s_carriageReturnNCRStringLength;

	/**
 	 * The string "&#9;".
 	 */
 	static const XalanDOMChar				s_htabNCRString[];

	static const XalanDOMString::size_type	s_htabNCRStringLength;

	enum
	{
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


	XalanDOMChar				m_buffer[kBufferSize];

	XalanDOMChar*				m_bufferPosition;

	XalanDOMString::size_type	m_bufferRemaining;
};



XALAN_CPP_NAMESPACE_END



#endif	// FORMATTERTOXML_UTF16_HEADER_GUARD_1357924680
