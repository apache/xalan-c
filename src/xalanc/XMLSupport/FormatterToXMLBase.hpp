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
#if !defined(FORMATTERTOXMLBASE_HEADER_GUARD_1357924680)
#define FORMATTERTOXMLBASE_HEADER_GUARD_1357924680




// Base include file.  Must be first.
#include <xalanc/XMLSupport/XMLSupportDefinitions.hpp>



#include <vector>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



// Base class header file.
#include <xalanc/PlatformSupport/FormatterListener.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class Writer;



/**
 * FormatterToXML_UTF8 formats SAX-style events into XML.
 */
class XALAN_XMLSUPPORT_EXPORT FormatterToXMLBase : public FormatterListener 
{
public:

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
	FormatterToXMLBase(
			Writer&					writer,
			const XalanDOMString&	version,
			const XalanDOMString&	mediaType,
			const XalanDOMString&	doctypeSystem,
			const XalanDOMString&	doctypePublic,
			bool					xmlDecl,
			const XalanDOMString&	standalone);

	virtual
	~FormatterToXMLBase();

	// These methods are inherited from FormatterListener ...

	virtual void
	setDocumentLocator(const LocatorType* const	locator);

	virtual void
	startDocument();

	virtual void
	endDocument();

	virtual void
	startElement(
			const XMLCh* const	name,
			AttributeListType&	attrs) = 0;

    virtual void
	endElement(const XMLCh* const	name) = 0;

    virtual void
	characters(
			const XMLCh* const	chars,
			const unsigned int	length);

    virtual void
	charactersRaw(
			const XMLCh* const	chars,
			const unsigned int	length) = 0;

	virtual void
	entityReference(const XMLCh* const	name) = 0;

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
	comment(const XMLCh* const	data) = 0;

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
	getEncoding() const = 0;

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

	virtual void
	writeXMLHeader() = 0;

	virtual void
	flushBuffer() = 0;

	virtual void
	writeDoctypeDecl(const XalanDOMChar*	name) = 0;

	virtual void
	writeProcessingInstruction(
			const XMLCh*	target,
			const XMLCh*	data) = 0;

	virtual void
	writeCharacters(
			const XMLCh*	chars,
			unsigned int	length) = 0;

	virtual void
	writeCDATA(
			const XMLCh*	chars,
			unsigned int	length) = 0;

	virtual void
	outputNewline() = 0;

	/**
	 * Mark the parent element as having a child.  If this
	 * is the first child, return true, otherwise, return
	 * false.  This allows the child element to determine
	 * if the parent tag has already been closed.
	 *
	 * @return true if the parent element has not been previously marked for children.
	 */
	bool
	markParentForChildren()
	{
		if(!m_elemStack.empty())
		{
			// See if the parent element has already been flagged as having children.
			if(false == m_elemStack.back())
			{
				m_elemStack.back() = true;

				return true;
			}
		}

		return false;
	}

	/**
	 * Open an element for possibile children
	 */
	void
	openElementForChildren()
	{
		m_elemStack.push_back(false);
	}

	bool
	outsideDocumentElement() const
	{
		return m_elemStack.empty();
	}

	/**
	 * Determine if an element ever had any children added.
	 *
	 * @return true if the children were added, false if not.
	 */
	bool
	childNodesWereAdded()
	{
		bool	fResult = false;

		if (m_elemStack.empty() == false)
		{
			fResult = m_elemStack.back();

			m_elemStack.pop_back();
		}

		return fResult;
	}

	void
	generateDoctypeDecl(const XalanDOMChar*		name)
	{
		if(true == m_needToOutputDoctypeDecl)		   
		{
			assert(m_doctypeSystem.empty() == false);

			writeDoctypeDecl(name);

			m_needToOutputDoctypeDecl = false;
		}
	}

	/** 
	 * The writer.
	 */
	Writer*					m_writer;

	void
	flushWriter();

	/**
	 * Tell if the next text should be raw.
	 */
	bool		m_nextIsRaw;

	/**
	 * Add space before '/>' for XHTML.
	 */
	bool		m_spaceBeforeClose;

	/**
	 * The System ID for the doc type.
	 */
	const XalanDOMString	m_doctypeSystem;

	/**
	 * The public ID for the doc type.
	 */
	const XalanDOMString	m_doctypePublic;

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
	 * The string of characters that represents the newline
	 */
	const XalanDOMChar*		m_newlineString;

	/**
	 * The length of the the string of characters that represents the newline
	 */
	XalanDOMString::size_type	m_newlineStringLength;

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

	static unsigned int
	decodeUTF16SurrogatePair(
			XalanDOMChar	theHighSurrogate,
			XalanDOMChar	theLowSurrogate);

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
	 * Throw an exception when an invalid
	 * character is encountered.
	 * @param ch The first character in the surrogate
	 * @param next The next character in the surrogate
	 */
	static void
	throwInvalidCharacterException(unsigned int		ch);

	enum
	{
			kNotSpecial = 0,
			kContentSpecial = 1,	// A flag to indicate a value in s_specialChars applies to content
			kAttributeSpecial = 2,	// A flag to indicate a value in s_specialChars applies to attributes
			kBothSpecial = 3,		// A flag t0 indicate a value in s_specialChars applies to both content and attributes
			kSpecialsSize = 0x80,	// The size of s_specialChars
			kBufferSize = 512		// The size of the buffer
	};

	static const XalanDOMChar	s_specialChars[];

private:

	// These are not implemented.
	FormatterToXMLBase(const FormatterToXMLBase&);

	FormatterToXMLBase&
	operator=(const FormatterToXMLBase&);

	bool
	operator==(const FormatterToXMLBase&) const;

	// Data members...
	/**
	 * Flag to tell that we need to add the doctype decl, 
	 * which we can't do until the first element is 
	 * encountered.
	 */
	bool		m_needToOutputDoctypeDecl;

	/**
	 * If true, XML header should be written to output.
	 */
	bool		m_shouldWriteXMLHeader;

	/**
	 * A stack of Boolean objects that tell if the given element 
	 * has children.
	 */
	BoolStackType	m_elemStack;

	/**
 	 * The string "-//W3C//DTD XHTML".
 	 */
 	static const XalanDOMChar				s_xhtmlDocTypeString[];

	static const XalanDOMString::size_type	s_xhtmlDocTypeStringLength;
};



XALAN_CPP_NAMESPACE_END



#endif	// FORMATTERTOXMLBASE_HEADER_GUARD_1357924680
