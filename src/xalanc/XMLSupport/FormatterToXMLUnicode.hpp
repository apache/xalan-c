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
#if !defined(FORMATTERTOXML_UNICODE_HEADER_GUARD_1357924680)
#define FORMATTERTOXML_UNICODE_HEADER_GUARD_1357924680


// Base include file.  Must be first.
#include <xalanc/XMLSupport/XMLSupportDefinitions.hpp>


#include <xercesc/sax/AttributeList.hpp>

#include <xalanc/PlatformSupport/DoubleSupport.hpp>
#include <xalanc/PlatformSupport/XalanOutputStream.hpp>
#include <xalanc/PlatformSupport/XalanUnicode.hpp>

#include <xalanc/XalanDOM/XalanDOMString.hpp>

#include <xalanc/DOMSupport/DOMServices.hpp>



XALAN_CPP_NAMESPACE_BEGIN


enum
{
    kNotSpecial = 0,
    kContentSpecial = 1,	// A flag to indicate a value in s_specialChars applies to content
    kAttributeSpecial = 2,	// A flag to indicate a value in s_specialChars applies to attributes
    kBothSpecial = 3,		// A flag t0 indicate a value in s_specialChars applies to both content and attributes
    kSpecialsSize = 0x97,	// The size of s_specialChars
    kBufferSize = 512		// The size of the buffer
};

// Don't make the table static data member for a template
extern XalanDOMChar	s_specialChars[];

/**
 * FormatterToXMLUnicode formats SAX-style events into XML.
 */
template<class UnicodeWriter>
class XALAN_XMLSUPPORT_EXPORT FormatterToXMLUnicode : public FormatterListener
{
public:

    typedef typename UnicodeWriter::value_type value_type;


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
	FormatterToXMLUnicode(
 			Writer&					writer,
			const XalanDOMString&	version = XalanDOMString(XalanMemMgrs::getDummyMemMgr()),
			const XalanDOMString&	mediaType = XalanDOMString(XalanMemMgrs::getDummyMemMgr()),
			const XalanDOMString&	doctypeSystem = XalanDOMString(XalanMemMgrs::getDummyMemMgr()),
			const XalanDOMString&	doctypePublic = XalanDOMString(XalanMemMgrs::getDummyMemMgr()),
			bool					xmlDecl = true,
			const XalanDOMString&	standalone = XalanDOMString(XalanMemMgrs::getDummyMemMgr()),
            MemoryManagerType&      theManager XALAN_DEFAULT_MEMMGR) : 
	FormatterListener(OUTPUT_METHOD_XML),
	m_nextIsRaw(false),
	m_spaceBeforeClose(false),
	m_doctypeSystem(doctypeSystem,theManager),
	m_doctypePublic(doctypePublic,theManager),
	m_version(version,theManager),
	m_standalone(standalone,theManager),
	m_mediaType(mediaType, theManager),
	m_newlineString(0),
	m_newlineStringLength(0),
	m_needToOutputDoctypeDecl(false),
    // We must write the XML declaration if standalone is specified
    m_shouldWriteXMLHeader(xmlDecl == true ? true : standalone.length() != 0),
	m_elemStack(theManager),
	m_nameFunction(0),
    m_stringBuffer(theManager),
    m_writer(writer, theManager)
    {
        if(isEmpty(m_doctypePublic) == false)
        {
            if(startsWith(
                m_doctypePublic,
                s_xhtmlDocTypeString) == true)
            {
                m_spaceBeforeClose = true;
            }
        }

        const XalanOutputStream* const	theStream = m_writer.getStream();

        if (theStream == 0)
        {
            m_newlineString = XalanOutputStream::defaultNewlineString();
        }
        else
        {
            m_newlineString = theStream->getNewlineString();
        }

        assert(m_newlineString != 0);

        m_newlineStringLength = length(m_newlineString);

        assert(m_newlineString != 0);

        if (m_version.empty() == true ||
            DoubleSupport::equal(DOMStringToDouble(m_version, theManager), 1.0) == true)
        {
            m_nameFunction = &FormatterToXMLUnicode::writeName1_0;
        }
        else
        {
            m_nameFunction = &FormatterToXMLUnicode::writeName1_1;

            setXMLVersion(XML_VERSION_1_1);
        }
    }

	/**
	 * Perform static initialization.  See class XMLSupportInit.
	 */
	static void
	initialize(MemoryManagerType& theManager)
    {

        UnicodeWriter::initialize(theManager);

        /* U+0080-U+009F are known as the "C1" range */
        assert( kSpecialsSize >= 0x80);

        for( size_t i = 0x80; i<= kSpecialsSize; ++i)
        {
            *(const_cast<XalanDOMChar*>(s_specialChars + i)) = kBothSpecial;
        }
    }
 
	/**
	 * Perform static shut down.  See class XMLSupportInit.
	 */
	static void
	terminate()
    {
        UnicodeWriter::terminate();
    }

    static FormatterToXMLUnicode*
    create(
                MemoryManagerType&      theManager,
    			Writer&					writer,
    			const XalanDOMString&	version = XalanDOMString(XalanMemMgrs::getDummyMemMgr()),
    			const XalanDOMString&	mediaType = XalanDOMString(XalanMemMgrs::getDummyMemMgr()),
    			const XalanDOMString&	doctypeSystem = XalanDOMString(XalanMemMgrs::getDummyMemMgr()),
    			const XalanDOMString&	doctypePublic = XalanDOMString(XalanMemMgrs::getDummyMemMgr()),
    			bool					xmlDecl = true,
    			const XalanDOMString&	standalone = XalanDOMString(XalanMemMgrs::getDummyMemMgr()))
    {
        typedef FormatterToXMLUnicode ThisType;
    
        XalanMemMgrAutoPtr<ThisType, false> theGuard( theManager , (ThisType*)theManager.allocate(sizeof(ThisType)));
    
        ThisType* theResult = theGuard.get();
    
        new (theResult) ThisType(
                                writer,
                                version,
                                mediaType, 
                                doctypeSystem,
                                doctypePublic,
                                xmlDecl,
                                standalone,
                                theManager);
    
        theGuard.release();
    
        return theResult;
    }

	virtual
	~FormatterToXMLUnicode()
	{
	}


	// These methods are inherited from FormatterListener ...

    MemoryManagerType&
    getMemoryManager()
    {
        return m_elemStack.getMemoryManager();
    }

    Writer*
	getWriter() const
    {
        return m_writer.getWriter();
    }


	virtual void
	setDocumentLocator(const LocatorType* const	/*locator*/)
    {
    }

	virtual void
	startDocument()
    {
	    if (m_doctypeSystem.empty() == false)
	    {
		    m_needToOutputDoctypeDecl = true;
	    }

	    if(m_shouldWriteXMLHeader == true)
	    {
		    writeXMLHeader();

            // Write a newline here, so the DOCTYPE comes out on a separate line
            if (m_needToOutputDoctypeDecl == true)
            {
                outputNewline();
            }
	    }
    }

	virtual void
	endDocument()
    {
	    m_needToOutputDoctypeDecl = false;

	    m_writer.flushBuffer();
    }

    virtual void
	characters(
			const XMLCh* const	chars,
			const unsigned int	length)
    {
	    if(length != 0)
	    {
		    if(m_nextIsRaw)
		    {
			    m_nextIsRaw = false;

			    charactersRaw(chars, length);
		    }
		    else
		    {
			    writeCharacters(chars, length);
		    }
	    }
    }

	virtual void
	ignorableWhitespace(
			const XMLCh* const	chars,
			const unsigned int	length)
    {
	    if (length > 0)
	    {
		    characters(chars, length);
	    }
    }

	virtual void
	processingInstruction(
			const XMLCh* const	target,
			const XMLCh* const	data)
    {
	    // Use a fairly nasty hack to tell if the next node is supposed to be 
	    // unescaped text.
	    if(equals(target, length(target), s_piTarget, s_piTargetLength) == true &&
	    equals(data, length(data), s_piData, s_piDataLength) == true)
	    {
		    m_nextIsRaw = true;
	    }
	    else	
	    {
		    writeProcessingInstruction(target, data);
	    }
    }

    virtual void
	resetDocument()
    {
        // I don't do anything with this yet.
    }

	virtual void
	cdata(
			const XMLCh* const	ch,
			const unsigned int 	length)
    {
	    if (length != 0)
	    {
		    if(m_nextIsRaw == true)
		    {
			    m_nextIsRaw = false;

			    charactersRaw(ch, length);
		    }
		    else
		    {
			    writeCDATA(ch, length);
		    }
	    }
    }

	virtual const XalanDOMString&
	getDoctypeSystem() const
    {
	    return m_doctypeSystem;
    }

	virtual const XalanDOMString&
	getDoctypePublic() const
    {
	    return m_doctypePublic;
    }

	virtual const XalanDOMString&
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

	void
	setShouldWriteXMLHeader(bool	b)
	{
		m_shouldWriteXMLHeader = b;
	}

	typedef XalanVector<bool>		BoolStackType;

    // End are inherited from FormatterToXMLBase  ...
	virtual void
	startElement(
			const XMLCh* const	name,
			AttributeListType&	attrs)
    {
    	generateDoctypeDecl(name);
    
    	writeParentTagEnd();
    
    	m_writer.write(value_type(XalanUnicode::charLessThanSign));
    	writeName(name);
    
    	const unsigned int	nAttrs = attrs.getLength();
    
    	for (unsigned int i = 0;  i < nAttrs ;  i++)
    	{
    		processAttribute(attrs.getName(i), attrs.getValue(i));
    	}
    
    	// Flag the current element as not yet having any children.
    	openElementForChildren();
    }

    virtual void
	endElement(const XMLCh* const	name)
    {
    	const bool	hasChildNodes = childNodesWereAdded();
    
    	if (hasChildNodes == true) 
    	{
    		m_writer.write(value_type(XalanUnicode::charLessThanSign));
    		m_writer.write(value_type(XalanUnicode::charSolidus));
    
    		writeName(name);
    	}
    	else
    	{
    		if(m_spaceBeforeClose == true)
    		{
    			m_writer.write(value_type(XalanUnicode::charSpace));
    		}
    
    		m_writer.write(value_type(XalanUnicode::charSolidus));
    	}
    
    	m_writer.write(value_type(XalanUnicode::charGreaterThanSign));
    }	

    virtual void
	charactersRaw(
			const XMLCh* const	chars,
			const unsigned int	length)
    {
    	writeParentTagEnd();
    
    	m_writer.write(chars, length);
    }


    virtual void
    entityReference(const XMLCh* const	name)
    {
	    writeParentTagEnd();
    	  
	    m_writer.write(value_type(XalanUnicode::charAmpersand));
	    writeName(name);
	    m_writer.write(value_type(XalanUnicode::charSemicolon));
    }

	virtual void
	comment(const XMLCh* const	data)
    {
	    writeParentTagEnd();

	    m_writer.write(value_type(XalanUnicode::charLessThanSign));
	    m_writer.write(value_type(XalanUnicode::charExclamationMark));
	    m_writer.write(value_type(XalanUnicode::charHyphenMinus));
	    m_writer.write(value_type(XalanUnicode::charHyphenMinus));

	    writeCommentData(data);

	    m_writer.write(value_type(XalanUnicode::charHyphenMinus));
	    m_writer.write(value_type(XalanUnicode::charHyphenMinus));
	    m_writer.write(value_type(XalanUnicode::charGreaterThanSign));
    }

	const XalanDOMString&
	getEncoding() const
    {
	    return m_writer.getEncoding();
    }

protected:

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

	bool
	getShouldWriteXMLHeader() const
	{
		return m_shouldWriteXMLHeader;
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

    void
    writeName1_0(const XalanDOMChar*	theChars)
    {
        m_writer.write(theChars);
    }



    void
    writeName1_1(const XalanDOMChar*	theChars)
    {
        m_writer.writeSafe(theChars , XalanDOMString::length(theChars));
    }

	void
	writeXMLHeader()
    {
		    // "<?xml version=\""
	    m_writer.write(s_xmlHeaderStartString, s_xmlHeaderStartStringLength);

	    if (length(m_version) != 0)
	    {
		    m_writer.write(m_version);
	    }
	    else
	    {
		    m_writer.write(s_defaultVersionString, s_defaultVersionStringLength);
	    }

	    // "\" encoding=\""
	    m_writer.write(s_xmlHeaderEncodingString, s_xmlHeaderEncodingStringLength);

	    m_writer.write(m_writer.getEncoding());

	    if (length(m_standalone) != 0)
	    {
		    m_writer.write(s_xmlHeaderStandaloneString, s_xmlHeaderStandaloneStringLength);
		    m_writer.write(m_standalone);
	    }

	    m_writer.write(s_xmlHeaderEndString, s_xmlHeaderEndStringLength);
    }


	void
	writeDoctypeDecl(const XalanDOMChar*	name)
    {
	    // "<!DOCTYPE "
	    m_writer.write(s_doctypeHeaderStartString, s_doctypeHeaderStartStringLength);

	    m_writer.write(name);

	    if(length(m_doctypePublic) != 0)
	    {
		    // " PUBLIC \""
		    m_writer.write(s_doctypeHeaderPublicString, s_doctypeHeaderPublicStringLength);
		    writeName(m_doctypePublic.c_str());
		    m_writer.write(value_type(XalanUnicode::charQuoteMark));
		    m_writer.write(value_type(XalanUnicode::charSpace));
		    m_writer.write(value_type(XalanUnicode::charQuoteMark));
	    }
	    else
	    {
		    // " SYSTEM \""
		    m_writer.write(s_doctypeHeaderSystemString, s_doctypeHeaderSystemStringLength);
	    }

	    writeName(m_doctypeSystem.c_str());
	    m_writer.write(value_type(XalanUnicode::charQuoteMark));
	    m_writer.write(value_type(XalanUnicode::charGreaterThanSign));

	    outputNewline();
    }


	void
	writeProcessingInstruction(
			const XMLCh*	target,
			const XMLCh*	data)
    {
	    writeParentTagEnd();

	    m_writer.write(value_type(XalanUnicode::charLessThanSign));
	    m_writer.write(value_type(XalanUnicode::charQuestionMark));
	    writeName(target);

	    const XalanDOMString::size_type		len = length(data);

	    // We need to make sure there is a least one whitespace character
	    // between the target and the data.
	    if ( len > 0 && !isXMLWhitespace(data[0]))
	    {
		    m_writer.write(value_type(XalanUnicode::charSpace));
	    }

	    writeNormalizedPIData(data, len);

	    m_writer.write(value_type(XalanUnicode::charQuestionMark));
	    m_writer.write(value_type(XalanUnicode::charGreaterThanSign));

	    // If outside of an element, then put in a new line.  This whitespace
	    // is not significant.
	    if (outsideDocumentElement() == true)
	    {
		    outputNewline();
	    }
    }

	void
	writeCharacters(
			const XMLCh*	chars,
			unsigned int	length)
    {
	    assert(length != 0);

	    writeParentTagEnd();

	    unsigned int	i = 0;
	    unsigned int	firstIndex = 0;

	    while(i < length) 
	    {
		    const XalanDOMChar	ch = chars[i];
            if(ch >= kSpecialsSize)
            {
			    safeWriteContent(chars + firstIndex, i - firstIndex);

                i = writeNormalizedCharBig(ch, chars, i, length);

			    ++i;

			    firstIndex = i;
            }
            else if( !(s_specialChars[ch] & kContentSpecial) )
            {
                ++i;
            }
            else
            {
			    safeWriteContent(chars + firstIndex, i - firstIndex);

			    i = writeDefaultEscape(ch, i, chars, length);

			    ++i;

			    firstIndex = i;
		    }
	    }

	    safeWriteContent(chars + firstIndex, i - firstIndex);
    }


	void
	writeCDATA(
			const XMLCh*	chars,
			unsigned int	length)
    {
    	assert(length != 0);
    
    	writeParentTagEnd();
    
    	m_writer.write(s_cdataOpenString, s_cdataOpenStringLength);
    
    	writeCDATAChars(chars, length);
    
    	m_writer.write(s_cdataCloseString, s_cdataCloseStringLength);
    }

	/**
	 * Output a line break.
	 */
	void
	outputNewline()
    {
	    assert(m_newlineString != 0 && length(m_newlineString) == m_newlineStringLength);

	    m_writer.write(m_newlineString, m_newlineStringLength);
    }

	/**
	 * Escape and write a character.
	 */
	XalanDOMString::size_type
	writeDefaultEscape(
			XalanDOMChar				ch,
			XalanDOMString::size_type	i,
			const XalanDOMChar			/*chars*/[],
			XalanDOMString::size_type	/*len*/)
    {
    	if(!writeDefaultEntity(ch))
    	{
            if (XalanUnicode::charLF == ch)
            {
                outputNewline();
            }
            else
            {
    		    writeC0C1NormalizedChar(ch);
            }
    	}
    
    	return i;
    }

	/**
	 * Escape and write a character in an attribute.
	 */
	XalanDOMString::size_type
	writeDefaultAttributeEscape(
			XalanDOMChar				ch,
			XalanDOMString::size_type	i,
			const XalanDOMChar			/*chars*/[],
			XalanDOMString::size_type	/*len*/)
    {
    	if(!writeDefaultAttributeEntity(ch))
    	{
    		i = writeC0C1NormalizedChar(ch);
    	}
    
    	return i;
    }
    
	/**
	 * Handle one of the default entities, return false if it 
	 * is not a default entity.
	 */
	bool
	writeDefaultEntity(XalanDOMChar		ch)
    {
	    if (XalanUnicode::charLessThanSign == ch)
	    {
		    m_writer.write(s_lessThanEntityString, s_lessThanEntityStringLength);
	    }
	    else if (XalanUnicode::charGreaterThanSign == ch)
	    {
		    m_writer.write(s_greaterThanEntityString, s_greaterThanEntityStringLength);
	    }
	    else if (XalanUnicode::charAmpersand == ch)
	    {
		    m_writer.write(s_ampersandEntityString, s_ampersandEntityStringLength);
	    }
	    else
	    {
		    return false;
	    }

	    return true;
    }

	/**
	 * Handle one of the default entities, return false if it 
	 * is not a default entity.
	 */
	bool
	writeDefaultAttributeEntity(XalanDOMChar	ch)
    {
    	if (writeDefaultEntity(ch) == true)
    	{
    		return true;
    	}
    	else if (XalanUnicode::charLF == ch) 
    	{
    		m_writer.write(s_linefeedNCRString, s_linefeedNCRStringLength);
    	}
    	else if (XalanUnicode::charCR == ch) 
    	{
    		m_writer.write(s_carriageReturnNCRString, s_carriageReturnNCRStringLength);
    	}
    	else if (XalanUnicode::charQuoteMark == ch) 
    	{
    		m_writer.write(s_quoteEntityString, s_quoteEntityStringLength);
    	}
    	else if (XalanUnicode::charHTab == ch) 
    	{
    		m_writer.write(s_htabNCRString, s_htabNCRStringLength);
    	}
    	else
    	{
    		return false;
    	}
    
    	return true;
    }	

	/**
	 * Write the data for a comment
	 * @param data The comment's data.
	 */
	void
	writeCommentData(const XalanDOMChar*	data)
    {
	    const XalanDOMString::size_type		len = length(data);
	    XalanDOMChar						previousChar = 0;

	    for (XalanDOMString::size_type i = 0; i < len; ++i)
	    {
		    const XalanDOMChar	currentChar = data[i];

		    if (currentChar == XalanUnicode::charHyphenMinus &&
			    previousChar == XalanUnicode::charHyphenMinus)
		    {
			    m_writer.write(value_type(XalanUnicode::charSpace));
		    }

            i = writeNormalizedChar(currentChar, data, i, len);

		    previousChar = currentChar;
	    }
    		
	    if (previousChar == XalanUnicode::charHyphenMinus)
	    {
		    m_writer.write(value_type(XalanUnicode::charSpace));
	    }
    }

	/**
	 * Check to see if a parent's ">" has been written, and, if 
	 * it has not, write it.
	 */
	void
	writeParentTagEnd()
    {
	    if(markParentForChildren() == true)
	    {
		    m_writer.write(value_type(XalanUnicode::charGreaterThanSign));
	    }
    }

    /**
    * Write a normalized character to the stream.
    * @param ch the string to write.
    * @param start the start offset into the string.
    * @param length the length of the string.
    */
    XalanDOMString::size_type
        writeNormalizedChar(
        XalanDOMChar				ch,
        const XalanDOMChar			chars[],
        XalanDOMString::size_type	start,
        XalanDOMString::size_type	length)
    {
        if (XalanUnicode::charLF == ch)
        {
            outputNewline();
        }
        else 
        {
            start = m_writer.write(ch, chars, start, length);
        }

        return start;
    }

    void
    writeNumberedEntityReference(unsigned long	theNumber)
    {
        m_writer.write(value_type(XalanUnicode::charAmpersand));
        m_writer.write(value_type(XalanUnicode::charNumberSign));

        m_writer.write(UnsignedLongToDOMString(theNumber, m_stringBuffer));
        clear(m_stringBuffer);

        m_writer.write(value_type(XalanUnicode::charSemicolon));
    }

	XalanDOMString::size_type
	writeNormalizedCharBig(
			XalanDOMChar				ch,
			const XalanDOMChar			chars[],
			XalanDOMString::size_type	start,
			XalanDOMString::size_type	length)
    {
        assert(ch >= kSpecialsSize);

	    if (XalanUnicode::charLSEP == ch)
	    {
		    writeNumberedEntityReference(ch);
	    }
	    else 
        {
            start = m_writer.write(ch, chars, start, length);
        }

	    return start;
    }

	bool
	writeC0C1NormalizedChar( XalanDOMChar	ch)
    {
        assert ( ch < kSpecialsSize );

        return writeXML11SpecialChar(ch, isXML1_1Version());  
    }

	/**
	 * Write characters for a CDATA section
	 *
	 * @param ch the string to write.
	 * @param length the length of the string.
	 */
	void
	writeCDATAChars(
			const XalanDOMChar			ch[],
			XalanDOMString::size_type	length)
    {
	    XalanDOMString::size_type i = 0;

	    // enum for a cheezy little state machine.
	    enum eState { eInitialState, eFirstRightSquareBracket, eSecondRightSquareBracket };

	    eState	theCurrentState = eInitialState;

	    while(i < length)
        {
		    // If "]]>", which would close the CDATA appears in
		    // the content, we have to put the first two characters
		    // in the CDATA section, close the CDATA section, then
		    // open a new one and add the last character.

		    const XalanDOMChar	theChar = ch[i];

		    if (theChar == XalanUnicode::charRightSquareBracket)
		    {
			    if (theCurrentState == eInitialState)
			    {
				    theCurrentState = eFirstRightSquareBracket;
			    }
			    else if (theCurrentState == eFirstRightSquareBracket)
			    {
				    theCurrentState = eSecondRightSquareBracket;
			    }

			    m_writer.write(value_type(XalanUnicode::charRightSquareBracket));
		    }
		    else if (theChar == XalanUnicode::charGreaterThanSign)
		    {
			    if (theCurrentState != eInitialState)
			    {
				    if (theCurrentState == eFirstRightSquareBracket)
				    {
					    theCurrentState = eInitialState;
				    }
				    else
				    {
					    theCurrentState = eInitialState;

					    m_writer.write(s_cdataCloseString, s_cdataCloseStringLength);
					    m_writer.write(s_cdataOpenString, s_cdataOpenStringLength);
				    }
			    }

			    m_writer.write(value_type(XalanUnicode::charGreaterThanSign));
		    }
		    else
		    {
			    if (theCurrentState != eInitialState)
			    {
				    theCurrentState = eInitialState;
			    }

			    i = writeNormalizedChar(theChar, ch, i, length);
		    }

		    ++i;
        }
    }


	/**
	 * Write an attribute string.
	 *
	 * @param theString The string to write.
	 * @param theStringLength The length of the string.
	 */
	void
	writeAttrString(
			const XalanDOMChar*			theString,
			XalanDOMString::size_type	theStringLength)
    {
	    assert(theString != 0);

	    XalanDOMString::size_type	i = 0;
	    XalanDOMString::size_type	firstIndex = 0;

        while(i < theStringLength)
        {
		    const XalanDOMChar	ch = theString[i];

            if(ch >= kSpecialsSize)
            {
			    safeWriteContent(theString + firstIndex, i - firstIndex);

                i = writeNormalizedCharBig(ch, theString, i, theStringLength);

			    ++i;

			    firstIndex = i;
            }
            else if( !(s_specialChars[ch] & kAttributeSpecial) )
            {
                ++i;
            }
            else
            {
			    safeWriteContent(theString + firstIndex, i - firstIndex);

			    i = writeDefaultAttributeEscape(ch, i, theString, theStringLength);

			    ++i;

			    firstIndex = i;
		    }
        }

	    safeWriteContent(theString + firstIndex, i - firstIndex);
    }

private:


	/**
	 * Process an attribute.
     * @param name The name of the attribute.
     * @param value The value of the attribute.
     */
    void
    processAttribute(
        const XalanDOMChar*		name,
        const XalanDOMChar*		value)
    {
        // We add a fake attribute to the source tree to
        // declare the xml prefix, so we filter it back out
        // here...
        // $$$ ToDo: It would be better if we didn't have to do
        // this here.
        if (equals(name, DOMServices::s_XMLNamespacePrefix) == false)
        {
            m_writer.write(value_type(XalanUnicode::charSpace));
            writeName(name);
            m_writer.write(value_type(XalanUnicode::charEqualsSign));
            m_writer.write(value_type(XalanUnicode::charQuoteMark));
            writeAttrString(value, length(value));
            m_writer.write(value_type(XalanUnicode::charQuoteMark));
        }
    }

	/**
	 * Normalize the data in a PI, to replace any
	 * "?>" pairs with "? >"
	 * @param theData the data to normalize.
	 */
	void
	writeNormalizedPIData(
			const XalanDOMChar*			theData,
			XalanDOMString::size_type	theLength)
    {
	    // If there are any "?>" pairs in the string,
	    // we have to normalize them to "? >", so they
	    // won't be confused with the end tag.

	    for (XalanDOMString::size_type i = 0; i < theLength; ++i)
	    {
		    const XalanDOMChar	theChar = theData[i];

		    if (theChar == XalanUnicode::charQuestionMark &&
			    i + 1 < theLength &&
			    theData[i + 1] == XalanUnicode::charGreaterThanSign)
		    {
			    m_writer.write(value_type(XalanUnicode::charQuestionMark));
			    m_writer.write(value_type(XalanUnicode::charSpace));
		    }
		    else
		    {
                i = writeNormalizedChar(theChar, theData, i, theLength);
		    }
	    }
    }

    void
    safeWriteContent(
			    const XalanDOMChar*			theChars,
			    XalanDOMString::size_type	theLength)
    {
	    for(size_type i = 0; i < theLength; ++i)
	    {
		    m_writer.write(value_type(theChars[i]));
	    }
    }

	// Data members...

	/**
	 * The string "<!DOCTYPE ".
	 */
	static const value_type						s_doctypeHeaderStartString[];

	static const XalanDOMString::size_type	s_doctypeHeaderStartStringLength;

	/**
	 * The string " PUBLIC \"".
	 */
	static const value_type						s_doctypeHeaderPublicString[];

	static const XalanDOMString::size_type	s_doctypeHeaderPublicStringLength;

	/**
	 * The string " SYSTEM \"".
	 */
	static const value_type						s_doctypeHeaderSystemString[];

	static const XalanDOMString::size_type	s_doctypeHeaderSystemStringLength;

	/**
	 * The string "<?xml version=\"".
	 */
	static const value_type						s_xmlHeaderStartString[];

	static const XalanDOMString::size_type	s_xmlHeaderStartStringLength;

	/**
	 * The string "\" encoding=\"".
	 */
	static const value_type						s_xmlHeaderEncodingString[];

	static const XalanDOMString::size_type	s_xmlHeaderEncodingStringLength;

	/**
	 * The string "\" standalone=\"".
	 */
	static const value_type						s_xmlHeaderStandaloneString[];

	static const XalanDOMString::size_type	s_xmlHeaderStandaloneStringLength;

	/**
	 * The string "\"?>".
	 */
	static const value_type						s_xmlHeaderEndString[];

	static const XalanDOMString::size_type	s_xmlHeaderEndStringLength;

	/**
	 * The string "1.0".
	 */
	static const value_type						s_defaultVersionString[];

	static const XalanDOMString::size_type	s_defaultVersionStringLength;

	/**
 	 * The string "-//W3C//DTD XHTML".
 	 */
 	static const XalanDOMChar				s_xhtmlDocTypeString[];

	static const XalanDOMString::size_type	s_xhtmlDocTypeStringLength;

	/**
 	 * The string "<![CDATA[".
 	 */
 	static const value_type						s_cdataOpenString[];

	static const XalanDOMString::size_type	s_cdataOpenStringLength;

	/**
 	 * The string "]]>".
 	 */
 	static const value_type						s_cdataCloseString[];

	static const XalanDOMString::size_type	s_cdataCloseStringLength;

	/**
 	 * The string "&lt;".
 	 */
 	static const value_type						s_lessThanEntityString[];

	static const XalanDOMString::size_type	s_lessThanEntityStringLength;

	/**
 	 * The string "&gt;".
 	 */
 	static const value_type						s_greaterThanEntityString[];

	static const XalanDOMString::size_type	s_greaterThanEntityStringLength;

	/**
 	 * The string "&amp;".
 	 */
 	static const value_type						s_ampersandEntityString[];

	static const XalanDOMString::size_type	s_ampersandEntityStringLength;

	/**
 	 * The string "&quot;".
 	 */
 	static const value_type						s_quoteEntityString[];

	static const XalanDOMString::size_type	s_quoteEntityStringLength;

	/**
 	 * The string "&#10;".
 	 */
 	static const value_type						s_linefeedNCRString[];

	static const XalanDOMString::size_type	s_linefeedNCRStringLength;

	/**
 	 * The string "&#13;".
 	 */
 	static const value_type						s_carriageReturnNCRString[];

	static const XalanDOMString::size_type	s_carriageReturnNCRStringLength;

	/**
 	 * The string "&#9;".
 	 */
 	static const value_type						s_htabNCRString[];

	static const XalanDOMString::size_type	s_htabNCRStringLength;


	void
	writeName(const XalanDOMChar*	theChars)
	{
		assert(m_nameFunction != 0);

		(this->*m_nameFunction)(theChars);
	}


	static bool
	isContentSpecial(XalanDOMChar	theChar)
	{
		if ((theChar >= kSpecialsSize) || (s_specialChars[theChar] & kContentSpecial))
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
		if ((theChar >= kSpecialsSize) || (s_specialChars[theChar] & kAttributeSpecial))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

    bool
    writeXML11SpecialChar(XalanDOMChar		theChar, bool isXML11Output)
    {
        bool result = false;

        if(theChar < XalanUnicode::charSpace /*0x20*/)
        {
            if(!isXML11Output)
            {
//                throwInvalidXMLCharacterException( theChar, getMemoryManager());
            }
            else
            {
                writeNumberedEntityReference(theChar);

                result = true;
            }
        }
        else if( theChar <= 0x97 && theChar >= 0x80 )
        {
            if(theChar == XalanUnicode::charNEL && !isXML11Output)
            {
  //              throwInvalidXMLCharacterException( theChar, getMemoryManager());
            }
            else
            {
                writeNumberedEntityReference(theChar);

                result = true;
            }
        }

        return result; 
    }
private:
    // not implemented

	// These are not implemented.
	FormatterToXMLUnicode(const FormatterToXMLUnicode&);

	FormatterToXMLUnicode&
	operator=(const FormatterToXMLUnicode&);

	bool
	operator==(const FormatterToXMLUnicode&) const;
	
	// Data members...

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

	typedef void (FormatterToXMLUnicode::*NameFunctionType)(const XalanDOMChar*);

	NameFunctionType	m_nameFunction;

    XalanDOMString m_stringBuffer;

	UnicodeWriter   m_writer;
};


#define FXML_SIZE(str)	((sizeof(str) / sizeof(str[0]) - 1))

template<class T>
const typename T::value_type	FormatterToXMLUnicode<T>::s_doctypeHeaderStartString[] =
{
	value_type(XalanUnicode::charLessThanSign),
	value_type(XalanUnicode::charExclamationMark),
	value_type(XalanUnicode::charLetter_D),
	value_type(XalanUnicode::charLetter_O),
	value_type(XalanUnicode::charLetter_C),
	value_type(XalanUnicode::charLetter_T),
	value_type(XalanUnicode::charLetter_Y),
	value_type(XalanUnicode::charLetter_P),
	value_type(XalanUnicode::charLetter_E),
	value_type(XalanUnicode::charSpace),
	value_type(0)
};

template<class T>
const XalanDOMString::size_type	FormatterToXMLUnicode<T>::s_doctypeHeaderStartStringLength = 10;
                                                    //		FXML_SIZE(s_doctypeHeaderStartString); 

template<class T>
const typename T::value_type	FormatterToXMLUnicode<T>::s_doctypeHeaderPublicString[] =
{
	value_type(XalanUnicode::charSpace),
	value_type(XalanUnicode::charLetter_P),
	value_type(XalanUnicode::charLetter_U),
	value_type(XalanUnicode::charLetter_B),
	value_type(XalanUnicode::charLetter_L),
	value_type(XalanUnicode::charLetter_I),
	value_type(XalanUnicode::charLetter_C),
	value_type(XalanUnicode::charSpace),
	value_type(XalanUnicode::charQuoteMark),
	value_type(0)
};

template<class T>
const XalanDOMString::size_type	FormatterToXMLUnicode<T>::s_doctypeHeaderPublicStringLength = 9;
		                                            // FXML_SIZE(s_doctypeHeaderPublicString);

template<class T>
const typename T::value_type	FormatterToXMLUnicode<T>::s_doctypeHeaderSystemString[] =
{
	value_type(XalanUnicode::charSpace),
	value_type(XalanUnicode::charLetter_S),
	value_type(XalanUnicode::charLetter_Y),
	value_type(XalanUnicode::charLetter_S),
	value_type(XalanUnicode::charLetter_T),
	value_type(XalanUnicode::charLetter_E),
	value_type(XalanUnicode::charLetter_M),
	value_type(XalanUnicode::charSpace),
	value_type(XalanUnicode::charQuoteMark),
	value_type(0)
};

template<class T>
const XalanDOMString::size_type		FormatterToXMLUnicode<T>::s_doctypeHeaderSystemStringLength = 9;
		                                            // FXML_SIZE(s_doctypeHeaderSystemString);

template<class T>
const typename T::value_type	FormatterToXMLUnicode<T>::s_xmlHeaderStartString[] =
{
	value_type(XalanUnicode::charLessThanSign),
	value_type(XalanUnicode::charQuestionMark),
	value_type(XalanUnicode::charLetter_x),
	value_type(XalanUnicode::charLetter_m),
	value_type(XalanUnicode::charLetter_l),
	value_type(XalanUnicode::charSpace),
	value_type(XalanUnicode::charLetter_v),
	value_type(XalanUnicode::charLetter_e),
	value_type(XalanUnicode::charLetter_r),
	value_type(XalanUnicode::charLetter_s),
	value_type(XalanUnicode::charLetter_i),
	value_type(XalanUnicode::charLetter_o),
	value_type(XalanUnicode::charLetter_n),
	value_type(XalanUnicode::charEqualsSign),
	value_type(XalanUnicode::charQuoteMark),
	value_type(0)
};

template<class T>
const XalanDOMString::size_type		FormatterToXMLUnicode<T>::s_xmlHeaderStartStringLength = 15;
		                                                // FXML_SIZE(s_xmlHeaderStartString);

template<class T>
const typename T::value_type	FormatterToXMLUnicode<T>::s_xmlHeaderEncodingString[] =
{
	value_type(XalanUnicode::charQuoteMark),
	value_type(XalanUnicode::charSpace),
	value_type(XalanUnicode::charLetter_e),
	value_type(XalanUnicode::charLetter_n),
	value_type(XalanUnicode::charLetter_c),
	value_type(XalanUnicode::charLetter_o),
	value_type(XalanUnicode::charLetter_d),
	value_type(XalanUnicode::charLetter_i),
	value_type(XalanUnicode::charLetter_n),
	value_type(XalanUnicode::charLetter_g),
	value_type(XalanUnicode::charEqualsSign),
	value_type(XalanUnicode::charQuoteMark),
	value_type(0)
};

template<class T>
const XalanDOMString::size_type		FormatterToXMLUnicode<T>::s_xmlHeaderEncodingStringLength = 12;
		                                                // FXML_SIZE(s_xmlHeaderEncodingString);

template<class T>
const typename T::value_type	FormatterToXMLUnicode<T>::s_xmlHeaderStandaloneString[] =
{
	value_type(XalanUnicode::charQuoteMark),
	value_type(XalanUnicode::charSpace),
	value_type(XalanUnicode::charLetter_s),
	value_type(XalanUnicode::charLetter_t),
	value_type(XalanUnicode::charLetter_a),
	value_type(XalanUnicode::charLetter_n),
	value_type(XalanUnicode::charLetter_d),
	value_type(XalanUnicode::charLetter_a),
	value_type(XalanUnicode::charLetter_l),
	value_type(XalanUnicode::charLetter_o),
	value_type(XalanUnicode::charLetter_n),
	value_type(XalanUnicode::charLetter_e),
	value_type(XalanUnicode::charEqualsSign),
	value_type(XalanUnicode::charQuoteMark),
	value_type(0)
};

template<class T>
const XalanDOMString::size_type		FormatterToXMLUnicode<T>::s_xmlHeaderStandaloneStringLength = 14;
		                                                    //FXML_SIZE(s_xmlHeaderStandaloneString);

template<class T>
const typename T::value_type	FormatterToXMLUnicode<T>::s_xmlHeaderEndString[] =
{
	value_type(XalanUnicode::charQuoteMark),
	value_type(XalanUnicode::charQuestionMark),
	value_type(XalanUnicode::charGreaterThanSign),
	value_type(0)
};

template<class T>
const XalanDOMString::size_type		FormatterToXMLUnicode<T>::s_xmlHeaderEndStringLength = 3;
		                                                    // FXML_SIZE(s_xmlHeaderEndString);

template<class T>
const typename T::value_type	FormatterToXMLUnicode<T>::s_defaultVersionString[] =
{
	value_type(XalanUnicode::charDigit_1),
	value_type(XalanUnicode::charFullStop),
	value_type(XalanUnicode::charDigit_0),
	value_type(0)
};

template<class T>
const XalanDOMString::size_type		FormatterToXMLUnicode<T>::s_defaultVersionStringLength = 3;
		//FXML_SIZE(s_defaultVersionString);

template<class T>
const typename T::value_type	FormatterToXMLUnicode<T>::s_cdataOpenString[] =
{
	value_type(XalanUnicode::charLessThanSign),
	value_type(XalanUnicode::charExclamationMark),
	value_type(XalanUnicode::charLeftSquareBracket),
	value_type(XalanUnicode::charLetter_C),
	value_type(XalanUnicode::charLetter_D),
	value_type(XalanUnicode::charLetter_A),
	value_type(XalanUnicode::charLetter_T),
	value_type(XalanUnicode::charLetter_A),
	value_type(XalanUnicode::charLeftSquareBracket),
	value_type(0)
};

template<class T>
const XalanDOMString::size_type		FormatterToXMLUnicode<T>::s_cdataOpenStringLength = 9;
		//FXML_SIZE(s_cdataOpenString);

template<class T>
const typename T::value_type	FormatterToXMLUnicode<T>::s_cdataCloseString[] =
{
	value_type(XalanUnicode::charRightSquareBracket),
	value_type(XalanUnicode::charRightSquareBracket),
	value_type(XalanUnicode::charGreaterThanSign),
	value_type(0)
};

template<class T>
const XalanDOMString::size_type		FormatterToXMLUnicode<T>::s_cdataCloseStringLength = 3;
		//FXML_SIZE(s_cdataCloseString);


template<class T>
const XalanDOMChar	FormatterToXMLUnicode<T>::s_xhtmlDocTypeString[] =
{
	 XalanUnicode::charHyphenMinus,
	 XalanUnicode::charSolidus,
	 XalanUnicode::charSolidus,
	 XalanUnicode::charLetter_W,
	 XalanUnicode::charDigit_3,
	 XalanUnicode::charLetter_C,
	 XalanUnicode::charSolidus,
	 XalanUnicode::charSolidus,
	 XalanUnicode::charLetter_D,
	 XalanUnicode::charLetter_T,
	 XalanUnicode::charLetter_D,
	 XalanUnicode::charSpace,
	 XalanUnicode::charLetter_X,
	 XalanUnicode::charLetter_H,
	 XalanUnicode::charLetter_T,
	 XalanUnicode::charLetter_M,
	 XalanUnicode::charLetter_L,
	XalanDOMChar(0)
};

template<class T>
const XalanDOMString::size_type		FormatterToXMLUnicode<T>::s_xhtmlDocTypeStringLength =17;
		//FXML_SIZE(s_xhtmlDocTypeString);

template<class T>
const typename T::value_type	FormatterToXMLUnicode<T>::s_lessThanEntityString[] =
{
	value_type(XalanUnicode::charAmpersand),
	value_type(XalanUnicode::charLetter_l),
	value_type(XalanUnicode::charLetter_t),
	value_type(XalanUnicode::charSemicolon),
	value_type(0)
};

template<class T>
const XalanDOMString::size_type		FormatterToXMLUnicode<T>::s_lessThanEntityStringLength =4;
		//FXML_SIZE(s_lessThanEntityString);

template<class T>
const typename T::value_type	FormatterToXMLUnicode<T>::s_greaterThanEntityString[] =
{
	value_type(XalanUnicode::charAmpersand),
	value_type(XalanUnicode::charLetter_g),
	value_type(XalanUnicode::charLetter_t),
	value_type(XalanUnicode::charSemicolon),
	value_type(0)
};

template<class T>
const XalanDOMString::size_type		FormatterToXMLUnicode<T>::s_greaterThanEntityStringLength = 4;
		//FXML_SIZE(s_greaterThanEntityString);

template<class T>
const typename T::value_type	FormatterToXMLUnicode<T>::s_ampersandEntityString[] =
{
	value_type(XalanUnicode::charAmpersand),
	value_type(XalanUnicode::charLetter_a),
	value_type(XalanUnicode::charLetter_m),
	value_type(XalanUnicode::charLetter_p),
	value_type(XalanUnicode::charSemicolon),
	value_type(0)
};

template<class T>
const XalanDOMString::size_type		FormatterToXMLUnicode<T>::s_ampersandEntityStringLength = 5;
		//FXML_SIZE(s_ampersandEntityString);

template<class T>
const typename T::value_type	FormatterToXMLUnicode<T>::s_quoteEntityString[] =
{
	value_type(XalanUnicode::charAmpersand),
	value_type(XalanUnicode::charLetter_q),
	value_type(XalanUnicode::charLetter_u),
	value_type(XalanUnicode::charLetter_o),
	value_type(XalanUnicode::charLetter_t),
	value_type(XalanUnicode::charSemicolon),
	value_type(0)
};

template<class T>
const XalanDOMString::size_type		FormatterToXMLUnicode<T>::s_quoteEntityStringLength =6;
		//FXML_SIZE(s_quoteEntityString);

template<class T>
const typename T::value_type	FormatterToXMLUnicode<T>::s_linefeedNCRString[] =
{
	typename T::value_type(XalanUnicode::charAmpersand),
	typename T::value_type(XalanUnicode::charNumberSign),
	typename T::value_type(XalanUnicode::charDigit_1),
	typename T::value_type(XalanUnicode::charDigit_0),
	typename T::value_type(XalanUnicode::charSemicolon),
	typename T::value_type(0)
};

template<class T>
const XalanDOMString::size_type		FormatterToXMLUnicode<T>::s_linefeedNCRStringLength = 5;
		//FXML_SIZE(s_linefeedNCRString);

template<class T>
const typename T::value_type	FormatterToXMLUnicode<T>::s_carriageReturnNCRString[] =
{
	value_type(XalanUnicode::charAmpersand),
	value_type(XalanUnicode::charNumberSign),
	value_type(XalanUnicode::charDigit_1),
	value_type(XalanUnicode::charDigit_3),
	value_type(XalanUnicode::charSemicolon),
	value_type(0)
};

template<class T>
const XalanDOMString::size_type		FormatterToXMLUnicode<T>::s_carriageReturnNCRStringLength =5;
		//FXML_SIZE(s_carriageReturnNCRString);

template<class T>
const typename T::value_type	FormatterToXMLUnicode<T>::s_htabNCRString[] =
{
	value_type(XalanUnicode::charAmpersand),
	value_type(XalanUnicode::charNumberSign),
	value_type(XalanUnicode::charDigit_9),
	value_type(XalanUnicode::charSemicolon),
	value_type(0)
};

template<class T>
const XalanDOMString::size_type		FormatterToXMLUnicode<T>::s_htabNCRStringLength =4;
		//FXML_SIZE(s_htabNCRString);


XALAN_CPP_NAMESPACE_END



#endif	// FORMATTERTOXML_UNICODE_HEADER_GUARD_1357924680
