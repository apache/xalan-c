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
#include "xalanc/XMLSupport/XMLSupportDefinitions.hpp"



#include "xalanc/XMLSupport/XalanXMLSerializerBase.hpp"



#include "xercesc/sax/AttributeList.hpp"



#include "xalanc/PlatformSupport/DoubleSupport.hpp"
#include "xalanc/PlatformSupport/XalanOutputStream.hpp"
#include "xalanc/PlatformSupport/XalanUnicode.hpp"



#include <xalanc/DOMSupport/DOMServices.hpp>



XALAN_CPP_NAMESPACE_BEGIN


/**
 * FormatterToXMLUnicode formats SAX-style events into XML.
 */
template<
    class                           UnicodeWriter,
    class                           ConstantsType,
    class                           CharPredicate,
    class                           IndentHandler,
    FormatterListener::eXMLVersion  XMLVersion>
class XALAN_XMLSUPPORT_EXPORT FormatterToXMLUnicode : public XalanXMLSerializerBase
{
public:

    typedef typename UnicodeWriter::value_type value_type;

    enum
    {
        eDefaultIndentAmount = 0
    };

    /**
     * Constructor
     *
     * @param theManager        The MemoryManager instance to use for all memory allocations
     * @param writer            the writer.
     * @param version           the string to write for the XML version number.
     * @param doctypeSystem     system identifier to be used in the document
     *                          type declaration
     * @param doctypePublic     public identifier to be used in the document
     *                          type declaration
     * @param xmlDecl           true if the XSLT processor should output an XML
     *                          declaration
     * @param standalone        The string the XSLT processor should output for
     *                          the standalone document declaration
     *
     */
    FormatterToXMLUnicode(
            MemoryManager&          theManager,
            Writer&                 writer,
            const XalanDOMString&   encoding,
            const XalanDOMString&   doctypeSystem = s_emptyString,
            const XalanDOMString&   doctypePublic = s_emptyString,
            bool                    xmlDecl = true,
            const XalanDOMString&   standalone = s_emptyString,
            size_type               indent = eDefaultIndentAmount) :
        XalanXMLSerializerBase(
            theManager,
            XMLVersion,
            encoding,
            doctypeSystem,
            doctypePublic,
            xmlDecl,
            standalone),
        m_stringBuffer(theManager),
        m_writer(writer, theManager),
        m_constants(),
        m_charPredicate(),
        m_indentHandler(m_writer , indent)
    {
    }

    static FormatterToXMLUnicode*
    create(
                MemoryManagerType&      theManager,
                Writer&                 writer,
                const XalanDOMString&   encoding,
                const XalanDOMString&   doctypeSystem = s_emptyString,
                const XalanDOMString&   doctypePublic = s_emptyString,
                bool                    xmlDecl = true,
                const XalanDOMString&   standalone = s_emptyString,
                size_type               indent = eDefaultIndentAmount)
    {

        typedef FormatterToXMLUnicode ThisType;

        XalanMemMgrAutoPtr<ThisType, false> theGuard( theManager , (ThisType*)theManager.allocate(sizeof(ThisType)));

        ThisType* theResult = theGuard.get();

        new (theResult) ThisType(
            theManager,
            writer,
            encoding,
            doctypeSystem,
            doctypePublic,
            xmlDecl,
            standalone,
            indent);

        theGuard.release();

        return theResult;
    }

    virtual
    ~FormatterToXMLUnicode()
    {
    }

    Writer*
    getWriter() const
    {
        return m_writer.getWriter();
    }

    // These are inherited from XalanXMLSerializerBase...

    virtual void
    endDocument()
    {
        m_indentHandler.setStartNewLine(true);

        m_indentHandler.indent();

        flushBuffer();
    }

    virtual void
    startElement(
            const XMLCh* const  name,
            AttributeList&      attrs)
    {
        generateDoctypeDecl(name);
    
        writeParentTagEnd();
    
        m_indentHandler.setPreserve(false);

        m_indentHandler.indent();

        m_indentHandler.setStartNewLine(true);

        m_writer.write(value_type(XalanUnicode::charLessThanSign));

        writeName(name);
    
        const unsigned int  nAttrs = attrs.getLength();
    
        for (unsigned int i = 0;  i < nAttrs ;  i++)
        {
            processAttribute(attrs.getName(i), attrs.getValue(i));
        }
    
        // Flag the current element as not yet having any children.
        openElementForChildren();

        m_indentHandler.increaseIndent();

        m_indentHandler.setPrevText(false);
    }

    virtual void
    endElement(const XMLCh* const   name)
    {
        m_indentHandler.decreaseIndent();

        const bool  hasChildNodes = childNodesWereAdded();
    
        if (hasChildNodes == true) 
        {
            m_indentHandler.indent();

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
 
        if (hasChildNodes == true) 
        {
            m_indentHandler.pop_preserve();
        }

        m_indentHandler.setPrevText(false);
    }   

    virtual void
    charactersRaw(
            const XMLCh* const  chars,
            const unsigned int  length)
    {
        writeParentTagEnd();
    
        m_indentHandler.setPreserve(true);

        m_writer.write(chars, length);
    }


    virtual void
    entityReference(const XMLCh* const  name)
    {
        writeParentTagEnd();
          
        m_indentHandler.indent();

        m_writer.write(value_type(XalanUnicode::charAmpersand));

        writeName(name);

        m_writer.write(value_type(XalanUnicode::charSemicolon));
    }

    virtual void
    comment(const XMLCh* const  data)
    {
        writeParentTagEnd();

        m_indentHandler.indent();

        m_writer.write(value_type(XalanUnicode::charLessThanSign));
        m_writer.write(value_type(XalanUnicode::charExclamationMark));
        m_writer.write(value_type(XalanUnicode::charHyphenMinus));
        m_writer.write(value_type(XalanUnicode::charHyphenMinus));

        writeNormalizedData(data, XalanDOMString::length(data));

        m_writer.write(value_type(XalanUnicode::charHyphenMinus));
        m_writer.write(value_type(XalanUnicode::charHyphenMinus));
        m_writer.write(value_type(XalanUnicode::charGreaterThanSign));

        m_indentHandler.setStartNewLine(true);
    }

    virtual const XalanDOMString&
    getEncoding() const
    {
        return m_constants.s_encodingString;
    }

protected:

    virtual void
    flushBuffer()
    {
        m_writer.flushBuffer();
    }

    void
    writeXMLHeader()
    {
            // "<?xml version=\""
        m_writer.write(
            m_constants.s_xmlHeaderStartString,
            m_constants.s_xmlHeaderStartStringLength);

        if (length(m_version) != 0)
        {
            m_writer.write(m_version);
        }
        else
        {
            m_writer.write(
                m_constants.s_defaultVersionString,
                m_constants.s_defaultVersionStringLength);
        }

        // "\" encoding=\""
        m_writer.write(
                m_constants.s_xmlHeaderEncodingString,
                m_constants.s_xmlHeaderEncodingStringLength);

        m_writer.write(m_encoding);

        if (length(m_standalone) != 0)
        {
            m_writer.write(
                m_constants.s_xmlHeaderStandaloneString,
                m_constants.s_xmlHeaderStandaloneStringLength);

            m_writer.write(m_standalone);
        }

        m_writer.write(
            m_constants.s_xmlHeaderEndString,
            m_constants.s_xmlHeaderEndStringLength);

        if (getNeedToOutputDoctypeDecl() == false)
        {
            m_indentHandler.outputLineSep();
        }
    }


    void
    writeDoctypeDecl(const XalanDOMChar*    name)
    {
        // "<!DOCTYPE "
        m_writer.write(
            m_constants.s_doctypeHeaderStartString,
            m_constants.s_doctypeHeaderStartStringLength);

        m_writer.write(name);

        if(length(m_doctypePublic) != 0)
        {
            // " PUBLIC \""
            m_writer.write(
                m_constants.s_doctypeHeaderPublicString,
                m_constants.s_doctypeHeaderPublicStringLength);

            writeName(m_doctypePublic.c_str());

            m_writer.write(value_type(XalanUnicode::charQuoteMark));
            m_writer.write(value_type(XalanUnicode::charSpace));
            m_writer.write(value_type(XalanUnicode::charQuoteMark));
        }
        else
        {
            // " SYSTEM \""
            m_writer.write(
                m_constants.s_doctypeHeaderSystemString,
                m_constants.s_doctypeHeaderSystemStringLength);
        }

        writeName(m_doctypeSystem.c_str());

        m_writer.write(value_type(XalanUnicode::charQuoteMark));
        m_writer.write(value_type(XalanUnicode::charGreaterThanSign));

        outputNewline();
    }


    void
    writeProcessingInstruction(
            const XMLCh*    target,
            const XMLCh*    data)
    {
        writeParentTagEnd();

        m_indentHandler.indent();

        m_writer.write(value_type(XalanUnicode::charLessThanSign));
        m_writer.write(value_type(XalanUnicode::charQuestionMark));
        writeName(target);

        const XalanDOMString::size_type     len = length(data);

        // We need to make sure there is a least one whitespace character
        // between the target and the data.
        if ( len > 0 && !isXMLWhitespace(data[0]))
        {
            m_writer.write(value_type(XalanUnicode::charSpace));
        }

        writeNormalizedData(data, len);

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
            const XMLCh*    chars,
            unsigned int    length)
    {
        assert(length != 0);

        writeParentTagEnd();

        m_indentHandler.setPreserve(true);

        unsigned int    i = 0;
        unsigned int    firstIndex = 0;

        while(i < length) 
        {
            const XalanDOMChar  ch = chars[i];

            if(m_charPredicate.range(ch) == true)
            {
                safeWriteContent(chars + firstIndex, i - firstIndex);

                i = writeNormalizedCharBig(chars, i, length);

                ++i;

                firstIndex = i;
            }
            else if(m_charPredicate.content(ch) == false)
            {
                ++i;
            }
            else
            {
                safeWriteContent(chars + firstIndex, i - firstIndex);

                writeDefaultEscape(ch);

                ++i;

                firstIndex = i;
            }
        }

        safeWriteContent(chars + firstIndex, i - firstIndex);

        m_indentHandler.setPrevText(true);
    }


    void
    writeCDATA(
            const XMLCh*    chars,
            unsigned int    length)
    {
        assert(length != 0);
    
        writeParentTagEnd();
    
        m_indentHandler.setPreserve(true);

        m_indentHandler.indent();

        m_writer.write(
            m_constants.s_cdataOpenString,
            m_constants.s_cdataOpenStringLength);
    
        bool    outsideCDATA = false;

        writeCDATAChars(chars, length, outsideCDATA);
    
        if (outsideCDATA == false)
        {
            m_writer.write(
                m_constants.s_cdataCloseString,
                m_constants.s_cdataCloseStringLength);
        }
    }

    /**
     * Output a line break.
     */
    void
    outputNewline()
    {
        m_writer.outputNewline();
    }

    /**
     * Escape and write a character.
     */
    void
    writeDefaultEscape(XalanDOMChar     ch)
    {
        assert(m_charPredicate.content(ch) == true);

        if(!writeDefaultEntity(ch))
        {
            if (XalanUnicode::charLF == ch)
            {
                outputNewline();
            }
            else
            {
                if(m_charPredicate.isForbidden(ch) == true)
                {
                    throwInvalidXMLCharacterException(
                        ch,
                        m_version,
                        getMemoryManager());
                }
                else
                {
                    writeNumericCharacterReference(ch);
                }
            }
        }
    }

    /**
     * Escape and write a character in an attribute.
     */
    void
    writeDefaultAttributeEscape(XalanDOMChar    ch)
    {
        assert(m_charPredicate.attribute(ch) == true);

        if(writeDefaultAttributeEntity(ch) == false)
        {
            if(m_charPredicate.isForbidden(ch) == true)
            {
                throwInvalidXMLCharacterException(
                    ch,
                    m_version,
                    getMemoryManager());
            }
            else
            {
                writeNumericCharacterReference(ch);
            }
           
        }
    }
    
    /**
     * Handle one of the default entities, return false if it 
     * is not a default entity.
     */
    bool
    writeDefaultEntity(XalanDOMChar     ch)
    {
        if (XalanUnicode::charLessThanSign == ch)
        {
            m_writer.write(
                m_constants.s_lessThanEntityString,
                m_constants.s_lessThanEntityStringLength);
        }
        else if (XalanUnicode::charGreaterThanSign == ch)
        {
            m_writer.write(
                m_constants.s_greaterThanEntityString,
                m_constants.s_greaterThanEntityStringLength);
        }
        else if (XalanUnicode::charAmpersand == ch)
        {
            m_writer.write(
                m_constants.s_ampersandEntityString,
                m_constants.s_ampersandEntityStringLength);
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
    writeDefaultAttributeEntity(XalanDOMChar    ch)
    {
        if (writeDefaultEntity(ch) == true)
        {
            return true;
        }
        else if (XalanUnicode::charQuoteMark == ch) 
        {
            m_writer.write(
                m_constants.s_quoteEntityString,
                m_constants.s_quoteEntityStringLength);
        }
        else
        {
            return false;
        }
    
        return true;
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

            m_indentHandler.setPrevText(false);

            m_indentHandler.push_preserve();
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
        XalanDOMChar                ch,
        const XalanDOMChar          chars[],
        XalanDOMString::size_type   start,
        XalanDOMString::size_type   length)
    {
        if (XalanUnicode::charLF == ch)
        {
            outputNewline();
        }
        else 
        {
            if(m_charPredicate.isCharRefForbidden(ch))
            {
                throwInvalidXMLCharacterException(
                    ch,
                    m_version,
                    getMemoryManager());
            }
            else
            {
                start = m_writer.write( chars, start, length);
            }
        }

        return start;
    }

    void
    writeNumericCharacterReference(unsigned long  theNumber)
    {
        m_writer.write(value_type(XalanUnicode::charAmpersand));
        m_writer.write(value_type(XalanUnicode::charNumberSign));

        m_writer.write(UnsignedLongToDOMString(theNumber, m_stringBuffer));
        clear(m_stringBuffer);

        m_writer.write(value_type(XalanUnicode::charSemicolon));
    }

    XalanDOMString::size_type
    writeNormalizedCharBig(
            const XalanDOMChar          chars[],
            XalanDOMString::size_type   start,
            XalanDOMString::size_type   length)
    {
        assert( start < length);

        const XalanDOMChar  ch = chars[start];

        assert(m_charPredicate.range(ch) == true);

        if (XMLVersion == XML_VERSION_1_1 &&
            XalanUnicode::charLSEP == ch)
        {
            writeNumericCharacterReference(ch);
        }
        else 
        {
            start = m_writer.write(chars, start, length);
        }

        return start;
    }

    /**
     * Write characters for a CDATA section
     *
     * @param ch the string to write.
     * @param length the length of the string.
     */
    void
    writeCDATAChars(
            const XalanDOMChar          chars[],
            XalanDOMString::size_type   length,
            bool&                       outsideCDATA)
    {
        XalanDOMString::size_type i = 0;

        while(i < length)
        {
            // If "]]>", which would close the CDATA appears in
            // the content, we have to put the first two characters
            // in the CDATA section, close the CDATA section, then
            // open a new one and add the last character.

            const XalanDOMChar  theChar = chars[i];

            if (theChar == XalanUnicode::charRightSquareBracket &&
                i - length > 2 &&
                XalanUnicode::charRightSquareBracket == chars[i + 1] &&
                XalanUnicode::charGreaterThanSign == chars[i + 2])
            {
                if (outsideCDATA == true)
                {
                    m_writer.write(
                        m_constants.s_cdataCloseString,
                        m_constants.s_cdataCloseStringLength);
                }

                m_writer.write(value_type(XalanUnicode::charRightSquareBracket));
                m_writer.write(value_type(XalanUnicode::charRightSquareBracket));

                m_writer.write(
                    m_constants.s_cdataCloseString,
                    m_constants.s_cdataCloseStringLength);

                m_writer.write(
                    m_constants.s_cdataOpenString,
                    m_constants.s_cdataOpenStringLength);

                m_writer.write(value_type(XalanUnicode::charGreaterThanSign));

                outsideCDATA = false;

                i += 2;
            }
            else
            {
                if (XalanUnicode::charLF == theChar)
                {
                    outputNewline();
                }
                else if(m_charPredicate.isCharRefForbidden(theChar))
                {
                     throwInvalidXMLCharacterException(
                            theChar,
                            m_version,
                            getMemoryManager());
                }
                else
                {
                    i = m_writer.writeCDATAChar(chars, i, length, outsideCDATA);
                }
            }

            ++i;
        }

        if(outsideCDATA == true)
        {
            m_writer.write(
                m_constants.s_cdataOpenString,
                m_constants.s_cdataOpenStringLength);
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
            const XalanDOMChar*         theString,
            XalanDOMString::size_type   theStringLength)
    {
        assert(theString != 0);

        XalanDOMString::size_type   i = 0;
        XalanDOMString::size_type   firstIndex = 0;

        while(i < theStringLength)
        {
            const XalanDOMChar  ch = theString[i];

            if(m_charPredicate.range(ch) == true)
            {
                safeWriteContent(theString + firstIndex, i - firstIndex);

                i = writeNormalizedCharBig(theString, i, theStringLength);

                ++i;

                firstIndex = i;
            }
            else if (m_charPredicate.attribute(ch) == false)
            {
                ++i;
            }
            else
            {
                safeWriteContent(theString + firstIndex, i - firstIndex);

                writeDefaultAttributeEscape(ch);

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
        const XalanDOMChar*     name,
        const XalanDOMChar*     value)
    {
        m_writer.write(value_type(XalanUnicode::charSpace));
        writeName(name);
        m_writer.write(value_type(XalanUnicode::charEqualsSign));
        m_writer.write(value_type(XalanUnicode::charQuoteMark));
        writeAttrString(value, length(value));
        m_writer.write(value_type(XalanUnicode::charQuoteMark));
    }

    /**
     * Write normalized data.
     * @param theData the data to write.
     * @param theLength the data to write.
     */
    void
    writeNormalizedData(
            const XalanDOMChar*         theData,
            XalanDOMString::size_type   theLength)
    {
        for (XalanDOMString::size_type i = 0; i < theLength; ++i)
        {
            const XalanDOMChar  theChar = theData[i];

            i = writeNormalizedChar(theChar, theData, i, theLength);
        }
    }

    void
    safeWriteContent(
                const XalanDOMChar*         theChars,
                XalanDOMString::size_type   theLength)
    {
        for(size_type i = 0; i < theLength; ++i)
        {
            m_writer.write(value_type(theChars[i]));
        }
    }

    void
    writeName(const XalanDOMChar*   theChars)
    {
        assert( theChars != 0);

        m_writer.writeNameChar(theChars, length(theChars));
    }

private:

    // These are not implemented.
    FormatterToXMLUnicode(const FormatterToXMLUnicode&);

    FormatterToXMLUnicode&
    operator=(const FormatterToXMLUnicode&);

    bool
    operator==(const FormatterToXMLUnicode&) const;
    

    // Data members...
    XalanDOMString  m_stringBuffer;

    UnicodeWriter   m_writer;

    ConstantsType   m_constants;

    CharPredicate   m_charPredicate;

    IndentHandler   m_indentHandler;
};



XALAN_CPP_NAMESPACE_END



#endif  // FORMATTERTOXML_UNICODE_HEADER_GUARD_1357924680
