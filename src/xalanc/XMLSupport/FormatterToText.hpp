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
#if !defined(FORMATTERTOTEXT_HEADER_GUARD_1357924680)
#define FORMATTERTOTEXT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XMLSupport/XMLSupportDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



// Base class header file.
#include <xalanc/PlatformSupport/FormatterListener.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class Writer;



/**
 * This class takes SAX events (in addition to some extra events 
 * that SAX doesn't handle yet) and produces simple text only.
 */
class XALAN_XMLSUPPORT_EXPORT FormatterToText : public FormatterListener
{
public:

	/**
	 * FormatterToText instance constructor.
	 */
	FormatterToText(MemoryManagerType& theManager XALAN_DEFAULT_CONSTRACTOR_MEMORY_MGR);

	/**
	 * FormatterToText instance constructor.
	 *
	 * @param writer writer for output
	 * @param normalizeLindefeed Normalize \n or \r\n (on certain platforms).
	 * @param handleIgnorableWhitespace If true ignorableWhitespace() will write data to the Writer
	 */
	FormatterToText(
			Writer&		        writer,
			bool		        normalizeLinefeed = true,
			bool		        handleIgnorableWhitespace = true,
            MemoryManagerType&  theManager XALAN_DEFAULT_MEMMGR);

	/**
	 * FormatterToText instance constructor.
	 *
	 * @param writer writer for output
	 * @param encoding character encoding for the writer
	 * @param normalizeLindefeed Normalize \n or \r\n on certain platforms.
	 * @param handleIgnorableWhitespace If true ignorableWhitespace() will write data to the Writer
	 */
	FormatterToText(
			Writer&					writer,
			const XalanDOMString&	encoding,
			bool					normalizeLinefeed = true,
			bool					handleIgnorableWhitespace = true,
            MemoryManagerType&      theManager XALAN_DEFAULT_MEMMGR);

	static FormatterToText*
    create(
            MemoryManagerType&      theManager,
			Writer&					writer,
			const XalanDOMString&	encoding,
			bool					normalizeLinefeed = true,
			bool					handleIgnorableWhitespace = true);

	virtual
	~FormatterToText();

    MemoryManagerType&
    getMemoryManager()
    {
        return m_encoding.getMemoryManager();
    }

	Writer*
	getWriter() const
	{
		return m_writer;
	}

	void
	setWriter(Writer*	theWriter)
	{
		m_writer = theWriter;

		update(false);
	}

	void
	clearEncoding();

	const XalanDOMString&
	getEncoding() const
	{
		return m_encoding;
	}

	void
	setEncoding(const XalanDOMString&	theEncoding)
	{
		m_encoding = theEncoding;

		update(false);
	}

	XalanDOMChar
	getMaxCharacter() const
	{
		return m_maxCharacter;
	}

	void
	setMaxCharacter(XalanDOMChar	theMaxChar)
	{
		m_maxCharacter = theMaxChar;
	}

	bool
	getNormalizeLinefeed() const
	{
		return m_normalize;
	}

	void
	setNormalizeLinefeed(bool	fNormalize)
	{
		m_normalize = fNormalize;
	}

	bool
	getHandleIgnorableWhitespace() const
	{
		return m_handleIgnorableWhitespace;
	}

	void
	setHandleIgnorableWhitespace(bool	fHandle)
	{
		m_handleIgnorableWhitespace = fHandle;
	}

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

private:

	// These are not implemented.
	FormatterToText(const FormatterToText&);

	FormatterToText&
	operator=(const FormatterToText&);

	bool
	operator==(const FormatterToText&) const;

	// Utility function to update various member variables
	// when data changes.
	void
	update(bool	fNormalizationOnly);

	// Data members...
	Writer*			m_writer;

	XalanDOMChar	m_maxCharacter;

	XalanDOMString	m_encoding;

	bool			m_haveEncoding;

	bool			m_normalize;

	bool			m_handleIgnorableWhitespace;

	const XalanDOMChar*			m_newlineString;

	XalanDOMString::size_type	m_newlineStringLength;
};



XALAN_CPP_NAMESPACE_END



#endif	// FORMATTERTOTEXT_HEADER_GUARD_1357924680
