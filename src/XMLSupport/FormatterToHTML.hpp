/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights 
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
#if !defined(FORMATTERTOHTML_HEADER_GUARD_1357924680)
#define FORMATTERTOHTML_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XMLSupport/XMLSupportDefinitions.hpp>



#include <vector>



// Base class header file.
#include <XMLSupport/FormatterToXML.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>



#include <XMLSupport/XalanHTMLElementsProperties.hpp>



/**
 * FormatterToHTML formats SAX-style events into HTML.
 */
class XALAN_XMLSUPPORT_EXPORT FormatterToHTML : public FormatterToXML 
{  
public:

#if defined(XALAN_NO_NAMESPACES)
	typedef vector<XalanHTMLElementsProperties::ElementProperties>	ElementPropertiesStackType;
#else
	typedef std::vector<XalanHTMLElementsProperties::ElementProperties>	ElementPropertiesStackType;
#endif

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

	enum eDummy
	{
		eDefaultIndentAmount = 0
	};

	/**
	 * Constructor for customized encoding and doctype.
	 * @param writer        The character output stream to use.
	 * @param encoding      The character encoding in use by writer.
	 * @param doctypeSystem The system ID for the DOCTYPE.
	 * @param doctypePublic The public ID for the DOCTYPE.
	 * @param doIndent      true if indenting should be enabled
	 * @param indent        Number of spaces to indent at each nesting level.
	 * @param escapeURLs    Whether or not to escape URLs according to the recommendation.  The default is true.
	 * @param omitMetaTag   Whether or not to output a META TAG according to the recommendation.  The default is false.
	 */
	FormatterToHTML(
			Writer&					writer,
			const XalanDOMString&	encoding = XalanDOMString(),
			const XalanDOMString&	mediaType = XalanDOMString(),
			const XalanDOMString&	doctypeSystem = XalanDOMString(),
			const XalanDOMString&	doctypePublic = XalanDOMString(),
			bool					doIndent = true,
			int						indent = eDefaultIndentAmount,
			bool					escapeURLs = true,
			bool					omitMetaTag = false);

	virtual
	~FormatterToHTML();

	// These methods are inherited from DocumentHandler ...

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

	// These methods are inherited from FormatterToXML...

	virtual bool
	accumDefaultEntity(
			XalanDOMChar				ch,
			XalanDOMString::size_type	i,
			const XalanDOMChar			chars[],
			XalanDOMString::size_type	len,
			bool						escLF);

	// These methods are inherited from FormatterListener ...

	virtual void
	entityReference(const XMLCh* const	name);

	virtual void
	cdata(
			const XMLCh* const	ch,
			const unsigned int 	length);

	virtual void
	processingInstruction(
			const XMLCh* const	target,
			const XMLCh* const	data);


	bool
	getEscapeURLs() const
	{
		return m_escapeURLs;
	}

	void
	setEscapeURLs(bool	flag)
	{
		m_escapeURLs = flag;
	}

	struct Entity
	{
		enum { eMaxLength = 8 };

		XalanDOMChar				m_char;

		XalanDOMString::size_type	m_length;

		XalanDOMChar				m_string[eMaxLength + 1];
	};

protected:

	virtual void
	writeAttrString(const XalanDOMChar*		theString);

	virtual void
	accumCommentData(const XalanDOMChar*	data);

	void
	writeCharacters(const XalanDOMString&	theString);

	void
	writeCharacters(
			const XalanDOMChar*			theString,
			XalanDOMString::size_type	theLength = XalanDOMString::npos);

private:

	/**
	 * The string "<!DOCTYPE  HTML".
	 */
	static const XalanDOMString&	s_doctypeHeaderStartString;

	/**
	 * The string " PUBLIC \"".
	 */
	static const XalanDOMString&	s_doctypeHeaderPublicString;

	/**
	 * The string " SYSTEM".
	 */
	static const XalanDOMString&	s_doctypeHeaderSystemString;

	/**
	 * The string "SCRIPT".
	 */
	static const XalanDOMString&	s_scriptString;

	/**
	 * The string "STYLE".
	 */
	static const XalanDOMString&	s_styleString;

	/**
	 * The string "<META http-equiv=\"Content-Type\" content=\"text/html; charset=".
	 */
	static const XalanDOMString&	s_metaString;

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

	void
	copyEntityIntoBuffer(
			const XalanDOMChar*			s,
			XalanDOMString::size_type	theLength);

	void
	copyEntityIntoBuffer(const XalanDOMString&	s);

	/**
	 * Process an attribute.
	 * @param   name	 The name of the attribute.
	 * @param   value   The value of the attribute.
	 */
	virtual void
	processAttribute(
			const XalanDOMChar*										name,
			const XalanDOMChar*										value,
			const XalanHTMLElementsProperties::ElementProperties&	elemProperties);

	/**
	 * Write the specified string after substituting non ASCII characters,
	 * with %HH, where HH is the hex of the byte value.
	 *
	 * @param   theString     String to convert to XML format.
	 */
	void
	writeAttrURI(const XalanDOMChar*	theString);

	/**
	 * Accumulate the specified character by converting its numeric value to
	 * a hex string, making sure that any string of length 1 are written with
	 * a '0' before the number.
	 *
	 * @param theChar The character to accumulate
	 */
	void
	accumHexNumber(const XalanDOMChar	theChar);

	bool
	popHasNamespace();

	bool
	pushHasNamespace(const XalanDOMChar*	theElementName);

	// Data members...
	XalanDOMString			m_currentElementName;

	bool					m_inBlockElem;

	BoolStackType			m_isRawStack;

	bool					m_isScriptOrStyleElem;

	BoolStackType			m_inScriptElemStack;

	bool					m_escapeURLs;

	/**
	 * A flag so we know whether or not we've put out the first
	 * element.
	 */
	bool					m_isFirstElement;

	/**
	 * A flag so we know whether or not we're writing utf-8.
	 */
	bool					m_isUTF8;

	/**
	 * A counter so we can tell if we're inside the document element.
	 */
	int						m_elementLevel;

	/**
	 * A stack to determine if the current element has
	 * a namespace.
	 */
	BoolStackType			m_hasNamespaceStack;

	/**
	 * This is set to true if we should omit the META tag in HTML output (the default is false)
	 */
	bool					m_omitMetaTag;

	ElementPropertiesStackType		m_elementPropertiesStack;

	static const XalanDOMString		s_emptyString;

	static const Entity				s_entities[];

	static const Entity* const		s_lastEntity;
};



#endif	// FORMATTERTOHTML_HEADER_GUARD_1357924680
