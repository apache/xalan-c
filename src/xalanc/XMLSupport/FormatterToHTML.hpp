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
#if !defined(FORMATTERTOHTML_HEADER_GUARD_1357924680)
#define FORMATTERTOHTML_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XMLSupport/XMLSupportDefinitions.hpp>



#include <xalanc/Include/XalanVector.hpp>



// Base class header file.
#include <xalanc/XMLSupport/FormatterToXML.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



#include <xalanc/XMLSupport/XalanHTMLElementsProperties.hpp>



XALAN_CPP_NAMESPACE_BEGIN



/**
 * FormatterToHTML formats SAX-style events into HTML.
 */
class XALAN_XMLSUPPORT_EXPORT FormatterToHTML : public FormatterToXML 
{  
    
public:


	typedef XalanVector<XalanHTMLElementsProperties::ElementProperties>	ElementPropertiesStackType;


	enum
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
			const XalanDOMString&	encoding = XalanDOMString(XalanMemMgrs::getDummyMemMgr()),
			const XalanDOMString&	mediaType = XalanDOMString(XalanMemMgrs::getDummyMemMgr()),
			const XalanDOMString&	doctypeSystem = XalanDOMString(XalanMemMgrs::getDummyMemMgr()),
			const XalanDOMString&	doctypePublic = XalanDOMString(XalanMemMgrs::getDummyMemMgr()),
			bool					doIndent = true,
			int						indent = eDefaultIndentAmount,
			bool					escapeURLs = true,
			bool					omitMetaTag = false,
            MemoryManagerType&      theManager XALAN_DEFAULT_MEMMGR);

    static FormatterToHTML*
    create(
            MemoryManagerType&      theManager,
			Writer&					writer,
			const XalanDOMString&	encoding, 
			const XalanDOMString&	mediaType,
			const XalanDOMString&	doctypeSystem,
			const XalanDOMString&	doctypePublic,
			bool					doIndent,
			int						indent,
			bool					escapeURLs,
			bool					omitMetaTag);
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
			AttributeListType&		attrs);

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
	writeAttrString(
			const XalanDOMChar*			theString,
			XalanDOMString::size_type	theStringLength);

	virtual void
	accumCommentData(const XalanDOMChar*	data);

	void
	writeCharacters(const XalanDOMString&	theString);

	void
	writeCharacters(
			const XalanDOMChar*			theString,
			XalanDOMString::size_type	theLength);

private:

	typedef XalanDOMString::size_type	size_type;

	/**
	 * The string "<!DOCTYPE  HTML".
	 */
	static const XalanDOMChar	s_doctypeHeaderStartString[];

	static const size_type		s_doctypeHeaderStartStringLength;

	/**
	 * The string " PUBLIC \"".
	 */
	static const XalanDOMChar	s_doctypeHeaderPublicString[];

	static const size_type		s_doctypeHeaderPublicStringLength;

	/**
	 * The string " SYSTEM".
	 */
	static const XalanDOMChar	s_doctypeHeaderSystemString[];

	static const size_type		s_doctypeHeaderSystemStringLength;

	/**
	 * The string "<META http-equiv=\"Content-Type\" content=\"text/html; charset=".
	 */
	static const XalanDOMChar	s_metaString[];

	static const size_type		s_metaStringLength;

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
	 * @param theString String to convert to HTML format.
	 * @param theStringLength The length of the string.
	 */
	void
	writeAttrURI(
			const XalanDOMChar*			theString,
			XalanDOMString::size_type	theStringLength);

	/**
	 * Accumulate the specified character by converting its numeric value to
	 * a hex string, making sure that any string of length 1 are written with
	 * a '0' before the number.
	 *
	 * @param theChar The character to accumulate
	 */
	void
	accumHexNumber(XalanDOMChar		theChar);

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



XALAN_CPP_NAMESPACE_END



#endif	// FORMATTERTOHTML_HEADER_GUARD_1357924680
