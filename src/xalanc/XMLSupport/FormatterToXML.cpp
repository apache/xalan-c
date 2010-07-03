/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the  "License");
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


// Class header file.
#include "FormatterToXML.hpp"



#include <xercesc/sax/AttributeList.hpp>
#include <xercesc/sax/SAXException.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/DoubleSupport.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>
#include <xalanc/PlatformSupport/XalanOutputStream.hpp>
#include <xalanc/PlatformSupport/XalanTranscodingServices.hpp>
#include <xalanc/PlatformSupport/Writer.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



XALAN_CPP_NAMESPACE_BEGIN



static const XalanDOMChar   theDefaultAttrSpecialChars[] =
{
    XalanUnicode::charLessThanSign,
    XalanUnicode::charGreaterThanSign,
    XalanUnicode::charAmpersand, 
    XalanUnicode::charQuoteMark,
    XalanUnicode::charCR,
    XalanUnicode::charLF,
    0
};



FormatterToXML::FormatterToXML(
            Writer&                 writer,
            const XalanDOMString&   version,
            bool                    doIndent,
            int                     indent,
            const XalanDOMString&   encoding, 
            const XalanDOMString&   mediaType,
            const XalanDOMString&   doctypeSystem,
            const XalanDOMString&   doctypePublic,
            bool                    xmlDecl,
            const XalanDOMString&   standalone,
            eFormat                 format,
            bool                    fBufferData,
            MemoryManager&      theManager) :
    FormatterListener(format),
    m_writer(&writer),
    m_stream(m_writer->getStream()),
    m_maxCharacter(0),
#if !defined(XALAN_NO_DEFAULT_BUILTIN_ARRAY_INITIALIZATION)
    m_attrCharsMap(),
    m_charsMap(),
#endif
    m_shouldWriteXMLHeader(xmlDecl),
    m_ispreserve(false),
    m_doIndent(doIndent),
    m_startNewLine(false),
    m_needToOutputDocTypeDecl(true),
    m_isprevtext(false),
    m_stripCData(false),
    m_nextIsRaw(false),
    m_inCData(false),
    m_encodingIsUTF(false),
    m_doctypeSystem(doctypeSystem, theManager),
    m_doctypePublic(doctypePublic, theManager),
    m_encoding(theManager),
    m_currentIndent(0),
    m_indent(indent),
    m_preserves(theManager),
    m_stringBuffer(theManager),
    m_bytesEqualChars(false),
    m_shouldFlush(fBufferData),
    m_spaceBeforeClose(false),
    m_escapeCData(false),
    m_version(version, theManager),
    m_standalone(standalone, theManager),
    m_mediaType(mediaType, theManager),
    m_attrSpecialChars(theDefaultAttrSpecialChars, theManager),
    m_charBuf(theManager),
    m_pos(0),
    m_byteBuf(theManager),
    m_elemStack(theManager),
    m_accumNameCharFunction(0),
    m_accumNameStringFunction(0),
    m_accumNameDOMStringFunction(0),
    m_accumNameArrayFunction(0),
    m_accumContentCharFunction(0),
    m_accumContentStringFunction(0),
    m_accumContentDOMStringFunction(0),
    m_accumContentArrayFunction(0),
    m_flushFunction(0),
    m_newlineString(0),
    m_newlineStringLength(0),
    m_isXML1_1(false)
{
    if (encoding.empty() == false)
    {
        m_encoding = encoding;
    }
    else
    {
        m_encoding = XalanDOMString(XalanTranscodingServices::s_utf8String, theManager);
    }

    assert(m_encoding.empty() == false);

    if(m_doctypePublic.empty() == false)
    {
        if(startsWith(
            m_doctypePublic,
            s_xhtmlDocTypeString) == true)
        {
            m_spaceBeforeClose = true;
        }
    }

    if (m_stream == 0)
    {
        m_newlineString = XalanOutputStream::defaultNewlineString();
    }
    else
    {
        try
        {
            m_stream->setOutputEncoding(m_encoding);
        }
        catch(const XalanOutputStream::UnsupportedEncodingException&)
        {
            // Default to UTF-8 if the requested encoding is not supported...
            m_stream->setOutputEncoding(XalanDOMString(XalanTranscodingServices::s_utf8String, theManager));

            m_encoding = XalanTranscodingServices::s_utf8String;
        }

        m_newlineString = m_stream->getNewlineString();
    }

    assert(m_newlineString != 0);

    m_newlineStringLength = length(m_newlineString);

    m_maxCharacter = XalanTranscodingServices::getMaximumCharacterValue(m_encoding);

    // We cannot omit the XML declaration if the encoding is not UTF-8 or
    // UTF-16, or if there is a standalone declaration
    const bool  canOmitXMLDeclaration =
        (XalanTranscodingServices::encodingIsUTF8(m_encoding) ||
         XalanTranscodingServices::encodingIsUTF16(m_encoding)) &&
         m_standalone.length() == 0;

    if (canOmitXMLDeclaration == false && m_shouldWriteXMLHeader == false)
    {
        m_shouldWriteXMLHeader = true;
    }

    //OK, now we can determine if the encoding is UTF*
    m_encodingIsUTF = canOmitXMLDeclaration || XalanTranscodingServices::encodingIsUTF32(m_encoding);

    if (m_version.empty() != true &&
        DoubleSupport::equal(DOMStringToDouble(m_version, theManager), 1.1) == true)
    {
        m_isXML1_1 = true;
    }

#if 1
    if (m_encodingIsUTF == true)
    {
        if (fBufferData == false)
        {
            m_accumNameCharFunction = &FormatterToXML::accumCharUTFDirect;

            m_accumContentCharFunction = &FormatterToXML::accumCharUTFDirect;

            m_accumNameStringFunction = &FormatterToXML::accumStringUTFDirect;

            m_accumContentStringFunction = &FormatterToXML::accumStringUTFDirect;

            m_accumNameDOMStringFunction = &FormatterToXML::accumDOMStringUTFDirect;

            m_accumContentDOMStringFunction = &FormatterToXML::accumDOMStringUTFDirect;

            m_accumNameArrayFunction = &FormatterToXML::accumArrayUTFDirect;

            m_accumContentArrayFunction = &FormatterToXML::accumArrayUTFDirect;
        }
        else
        {
            m_charBuf.resize(s_maxBufferSize);

            m_accumNameCharFunction = &FormatterToXML::accumCharUTF;

            m_accumContentCharFunction = &FormatterToXML::accumCharUTF;

            m_accumNameStringFunction = &FormatterToXML::accumStringUTF;

            m_accumContentStringFunction = &FormatterToXML::accumStringUTF;

            m_accumNameDOMStringFunction = &FormatterToXML::accumDOMStringUTF;

            m_accumContentDOMStringFunction = &FormatterToXML::accumDOMStringUTF;

            m_accumNameArrayFunction = &FormatterToXML::accumArrayUTF;

            m_accumContentArrayFunction = &FormatterToXML::accumArrayUTF;
        }
    }
    else
    {
        if (fBufferData == false)
        {
            m_accumNameCharFunction = &FormatterToXML::accumNameAsCharDirect;

            m_accumContentCharFunction = &FormatterToXML::accumContentAsCharDirect;
        }
        else
        {
            m_charBuf.resize(s_maxBufferSize);

            m_accumNameCharFunction = &FormatterToXML::accumNameAsChar;

            m_accumContentCharFunction = &FormatterToXML::accumContentAsChar;
        }

        m_accumNameStringFunction = &FormatterToXML::accumNameString;

        m_accumContentStringFunction = &FormatterToXML::accumContentString;

        m_accumNameDOMStringFunction = &FormatterToXML::accumNameDOMString;

        m_accumContentDOMStringFunction = &FormatterToXML::accumContentDOMString;

        m_accumNameArrayFunction = &FormatterToXML::accumNameArray;

        m_accumContentArrayFunction = &FormatterToXML::accumContentArray;
    }

    m_flushFunction = &FormatterToXML::flushChars;
#else
    if (XalanTranscodingServices::getBytesEqualChars(m_encoding) == true)
    {
        m_bytesEqualChars = true;

        if (fBufferData == false)
        {
            m_accumNameCharFunction = &FormatterToXML::accumNameAsByteDirect;

            m_accumContentCharFunction = &FormatterToXML::accumContentAsByteDirect;
        }
        else
        {
            m_byteBuf.resize(s_maxBufferSize);

            m_accumNameCharFunction = &FormatterToXML::accumNameAsByte;

            m_accumContentCharFunction = &FormatterToXML::accumContentAsByte;
        }

        m_accumNameStringFunction = &FormatterToXML::accumNameString;

        m_accumContentStringFunction = &FormatterToXML::accumContentString;

        m_accumNameDOMStringFunction = &FormatterToXML::accumNameDOMString;

        m_accumContentDOMStringFunction = &FormatterToXML::accumContentDOMString;

        m_accumNameArrayFunction = &FormatterToXML::accumNameArray;

        m_accumContentArrayFunction = &FormatterToXML::accumContentArray;

        m_flushFunction = &FormatterToXML::flushBytes;
    }
    else
    {
        if (fBufferData == false)
        {
            m_accumNameCharFunction = &FormatterToXML::accumNameAsCharDirect;

            m_accumContentCharFunction = &FormatterToXML::accumContentAsCharDirect;
        }
        else
        {
            m_charBuf.resize(s_maxBufferSize);

            m_accumNameCharFunction = &FormatterToXML::accumNameAsChar;

            m_accumContentCharFunction = &FormatterToXML::accumContentAsChar;
        }

        m_flushFunction = &FormatterToXML::flushChars;
    }
#endif

    // Do this last so we initialize the map according to the value of
    // m_maxCharacter for the encoding.
    initCharsMap();
}



FormatterToXML*
FormatterToXML::create(
                       MemoryManager&           theManager,
                       Writer&                  writer,
                       const XalanDOMString&    version,
                       bool                     doIndent,
                       int                      indent,
                       const XalanDOMString&    encoding,
                       const XalanDOMString&    mediaType,
                       const XalanDOMString&    doctypeSystem,
                       const XalanDOMString&    doctypePublic,
                       bool                     xmlDecl,
                       const XalanDOMString&    standalone,
                       eFormat                  format,
                       bool                     fBufferData)
{
    typedef FormatterToXML  ThisType;

    XalanAllocationGuard    theGuard(theManager, theManager.allocate(sizeof(ThisType)));

    ThisType* const     theResult =
        new (theGuard.get()) ThisType(
                                writer,
                                version,
                                doIndent,
                                indent,
                                encoding,
                                mediaType,
                                doctypeSystem,
                                doctypePublic,
                                xmlDecl,
                                standalone,
                                format,
                                fBufferData,
                                theManager);

    theGuard.release();

    return theResult;
}



FormatterToXML::~FormatterToXML()
{
}



void
FormatterToXML::initAttrCharsMap()
{
#if defined(XALAN_STRICT_ANSI_HEADERS)
    std::memset(m_attrCharsMap, 0, sizeof(m_attrCharsMap));
#else
    memset(m_attrCharsMap, 0, sizeof(m_attrCharsMap));
#endif

    const XalanDOMString::size_type     nSpecials =
        m_attrSpecialChars.length();

    {
        for(XalanDOMString::size_type i = 0; i < nSpecials; ++i)
        {
            m_attrCharsMap[m_attrSpecialChars[i]] = 'S';
        }
    }

    m_attrCharsMap[XalanUnicode::charHTab] = 'S';
    m_attrCharsMap[XalanUnicode::charLF] = 'S';
    m_attrCharsMap[XalanUnicode::charCR] = 'S';

    for(size_t i = 1; i < 0x20; i++)
    {
        m_attrCharsMap[i] = 'S';
    }

    for(size_t j = 0x7F; j < 0x9F; j++)
    {
        m_attrCharsMap[j] = 'S';
    }
}



void
FormatterToXML::initCharsMap()
{
    initAttrCharsMap();

#if defined(XALAN_STRICT_ANSI_HEADERS)
    std::memset(m_charsMap, 0, sizeof(m_charsMap));
#else
    memset(m_charsMap, 0, sizeof(m_charsMap));
#endif

    m_charsMap[XalanUnicode::charLessThanSign] = 'S';
    m_charsMap[XalanUnicode::charGreaterThanSign] = 'S';
    m_charsMap[XalanUnicode::charAmpersand] = 'S';


    for(size_t i = 1; i < 0x20; i++)
    {
        m_charsMap[i] = 'S';
    }

    for(size_t j = 0x7F; j < 0x9F; j++)
    {
        m_charsMap[j] = 'S';
    }

    m_charsMap[9] = '\0';

    assert(m_maxCharacter != 0);

    for(XalanDOMChar k = m_maxCharacter; k < SPECIALSSIZE; ++k)
    {
        m_charsMap[k] = 'S';
    }
}



void
FormatterToXML::outputDocTypeDecl(const XalanDOMChar*   name)
{
    // "<!DOCTYPE "
    accumName(s_doctypeHeaderStartString, 0, s_doctypeHeaderStartStringLength);

    accumName(name);
      
    if (m_doctypePublic.empty() == false)
    {
        // " PUBLIC \""
        accumName(s_doctypeHeaderPublicString, 0, s_doctypeHeaderPublicStringLength);
        accumName(m_doctypePublic);
        accumName(XalanUnicode::charQuoteMark);
        accumName(XalanUnicode::charSpace);
        accumName(XalanUnicode::charQuoteMark);
    }
    else
    {
        // " SYSTEM \""
        accumName(s_doctypeHeaderSystemString, 0, s_doctypeHeaderSystemStringLength);
    }

    accumName(m_doctypeSystem);
    accumName(XalanUnicode::charQuoteMark);
    accumName(XalanUnicode::charGreaterThanSign);

    outputLineSep();
}



void
FormatterToXML::accumNameAsByte(XalanDOMChar    ch)
{
    if (ch > m_maxCharacter)
    {
        m_byteBuf[m_pos++] = char(XalanUnicode::charQuestionMark);
    }
    else
    {
        m_byteBuf[m_pos++] = char(ch);
    }

    if(m_pos == s_maxBufferSize)
    {
        flushBytes();
    }
}



void
FormatterToXML::accumNameAsByteDirect(XalanDOMChar  ch)
{
    assert(m_stream != 0);

    if (ch > m_maxCharacter)
    {
        m_stream->write(char(XalanUnicode::charQuestionMark));
    }
    else
    {
        m_stream->write(char(ch));
    }
}



void
FormatterToXML::accumContentAsByte(XalanDOMChar     ch)
{
    if (ch > m_maxCharacter)
    {
        writeNumberedEntityReference(ch);
    }
    else
    {
        m_byteBuf[m_pos++] = char(ch);
    }

    if(m_pos == s_maxBufferSize)
    {
        flushBytes();
    }
}



void
FormatterToXML::accumContentAsByteDirect(XalanDOMChar   ch)
{
    assert(m_stream != 0);

    if (ch > m_maxCharacter)
    {
        writeNumberedEntityReference(ch);
    }
    else
    {
        m_stream->write(char(ch));
    }
}



void
FormatterToXML::accumNameAsChar(XalanDOMChar    ch)
{
    if (ch > m_maxCharacter)
    {
        m_charBuf[m_pos++] = XalanUnicode::charQuestionMark;
    }
    else
    {
        m_charBuf[m_pos++] = ch;
    }

    if(m_pos == s_maxBufferSize)
    {
        flushChars();
    }
}



void
FormatterToXML::accumNameAsCharDirect(XalanDOMChar  ch)
{
    assert(m_stream != 0);

    if (ch > m_maxCharacter)
    {
        m_stream->write(XalanDOMChar(XalanUnicode::charQuestionMark));
    }
    else
    {
        m_stream->write(ch);
    }
}



void
FormatterToXML::accumContentAsChar(XalanDOMChar ch)
{
    if (ch > m_maxCharacter)
    {
        writeNumberedEntityReference(ch);
    }
    else
    {
        m_charBuf[m_pos++] = ch;
    }

    if(m_pos == s_maxBufferSize)
    {
        flushChars();
    }
}



void
FormatterToXML::accumContentAsCharDirect(XalanDOMChar   ch)
{
    assert(m_stream != 0);

    if (ch > m_maxCharacter)
    {
        writeNumberedEntityReference(ch);
    }
    else
    {
        m_stream->write(ch);
    }
}



void
FormatterToXML::accumCharUTF(XalanDOMChar   ch)
{
    assert(m_maxCharacter >= 65535);

    m_charBuf[m_pos++] = ch;

    if(m_pos == s_maxBufferSize)
    {
        flushChars();
    }
}



void
FormatterToXML::accumCharUTFDirect(XalanDOMChar ch)
{
    assert(m_maxCharacter >= 65535);
    assert(m_stream != 0);

    m_stream->write(ch);
}



void
FormatterToXML::accumNameString(const XalanDOMChar* chars)
{
    for(; *chars!= 0; ++chars)
    {
        accumName(*chars);
    }
}



void
FormatterToXML::accumStringUTF(const XalanDOMChar*  chars)
{
    for(; *chars!= 0; ++chars)
    {
        accumCharUTF(*chars);
    }
}



void
FormatterToXML::accumStringUTFDirect(const XalanDOMChar*    chars)
{
    assert(m_maxCharacter >= 65535);
    assert(m_stream != 0);

    m_stream->write(chars);
}



void
FormatterToXML::accumContentString(const XalanDOMChar*  chars)
{
    for(; *chars!= 0; ++chars)
    {
        accumContent(*chars);
    }
}



void
FormatterToXML::accumNameArray(
            const XalanDOMChar  chars[],
            size_type           start,
            size_type           length)
{
    const size_type     n = start + length;

    for(size_type i = start; i < n; ++i)
    {
        accumName(chars[i]);
    }
}



void
FormatterToXML::accumContentArray(
            const XalanDOMChar  chars[],
            size_type           start,
            size_type           length)
{
    const size_type     n = start + length;

    for(size_type i = start; i < n; ++i)
    {
        accumContent(chars[i]);
    }
}



void
FormatterToXML::accumArrayUTF(
            const XalanDOMChar  chars[],
            size_type           start,
            size_type           length)
{
    const size_type     n = start + length;

    for(size_type i = start; i < n; ++i)
    {
        accumCharUTF(chars[i]);
    }
}



void
FormatterToXML::accumArrayUTFDirect(
            const XalanDOMChar  chars[],
            size_type           start,
            size_type           length)
{
    assert(m_maxCharacter >= 65535);
    assert(m_stream != 0);

    m_stream->write(chars + start, length);
}



void
FormatterToXML::accumNameDOMString(const XalanDOMString&    str)
{
    accumName(str.c_str(), 0, str.length());
}



void
FormatterToXML::accumContentDOMString(const XalanDOMString&     str)
{
    accumContent(str.c_str(), 0, str.length());
}



void
FormatterToXML::accumDOMStringUTF(const XalanDOMString&     str)
{
    accumArrayUTF(str.c_str(), 0, str.length());
}



void
FormatterToXML::accumDOMStringUTFDirect(const XalanDOMString&   str)
{
    assert(m_maxCharacter >= 65535);
    assert(m_stream != 0);

    m_stream->write(str.c_str(), str.length());
}



XALAN_USING_XERCES(SAXException)

void
FormatterToXML::throwInvalidUTF16SurrogateException(
            XalanDOMChar        ch,
            MemoryManager&  theManager)
{
    XalanDOMString  chStr(theManager);

    NumberToHexDOMString(ch, chStr);

    XalanDOMString  theMessage(theManager);

    XalanMessageLoader::getMessage(
        theMessage,
        XalanMessages::InvalidHighSurrogate_1Param,
        chStr);

    throw SAXException(theMessage.c_str(), &theManager);
}

void
FormatterToXML::throwInvalidCharacterException(
            XalanUnicodeChar    ch,
            MemoryManager&  theManager)
{
    XalanDOMString  theMessage(theManager);
    XalanDOMString  theBuffer(theManager);  

    XalanMessageLoader::getMessage(
            theMessage,
            XalanMessages::InvalidScalar_1Param,
            NumberToHexDOMString(ch, theBuffer));

    XALAN_USING_XERCES(SAXException)

    throw SAXException(theMessage.c_str(), &theManager);
}

void
FormatterToXML::throwInvalidUTF16SurrogateException(
            XalanDOMChar        ch,
            XalanDOMChar        next,
            MemoryManager&  theManager)
{

    XalanDOMString  chStr1(theManager); 
    XalanDOMString  chStr2(theManager); 

    NumberToHexDOMString(ch, chStr1);
    NumberToHexDOMString(next, chStr2);

    XalanDOMString  theMessage(theManager);

    XalanMessageLoader::getMessage(
        theMessage,
        XalanMessages::InvalidSurrogatePair_2Param,
        chStr1,
        chStr2);

    throw SAXException(theMessage.c_str(), &theManager);
}



FormatterToXML::size_type
FormatterToXML::accumDefaultEscape(
            XalanDOMChar        ch,
            size_type           i,
            const XalanDOMChar  chars[],
            size_type           len,
            bool                escLF)
{
    if(!accumDefaultEntity(ch, escLF))
    {
        if (0xd800 <= ch && ch < 0xdc00) 
        {
            // UTF-16 surrogate
            unsigned long   next = 0;

            if (i + 1 >= len)
            {
                throwInvalidUTF16SurrogateException(ch, getMemoryManager());
            }
            else 
            {
                next = chars[++i];

                if (!(0xdc00u <= next && next < 0xe000u))
                {
                    throwInvalidUTF16SurrogateException(ch, XalanDOMChar(next), getMemoryManager());
                }

                next = ((ch - 0xd800u) << 10) + next - 0xdc00u + 0x00010000u;
            }

            writeNumberedEntityReference(next);
        }
        else 
        {
            if(ch > m_maxCharacter)
            {
                if( !m_isXML1_1 && XalanUnicode::charLSEP == ch ) 
                {
                    throwInvalidCharacterException(ch, getMemoryManager());
                }
                else
                {
                    writeNumberedEntityReference(ch);
                }
            }
            else if(ch < SPECIALSSIZE && m_attrCharsMap[ch] == 'S')
            {
                if(ch < 0x20 )
                {
                    if(m_isXML1_1)
                    {
                        writeNumberedEntityReference(ch);
                    }
                    else
                    {
                         throwInvalidCharacterException(ch, getMemoryManager());
                    }
                }
                else if( XalanUnicode::charNEL == ch )
                {
                    if(m_isXML1_1)
                    {
                        writeNumberedEntityReference(ch);
                    }
                    else
                    {
                        throwInvalidCharacterException(ch, getMemoryManager());
                    }
                }
                else
                {
                    writeNumberedEntityReference(ch);
                }
            }
            else
            {
                accumContent(ch);
            }
        }
    }

    return i;
}



bool
FormatterToXML::accumDefaultEntity(
            XalanDOMChar    ch,
            bool            escLF)
{
    if (escLF == false && XalanUnicode::charLF == ch) 
    {
        outputLineSep();
    }
    else if (XalanUnicode::charLessThanSign == ch) 
    {
        accumContent(XalanUnicode::charAmpersand);
        accumContent(XalanUnicode::charLetter_l);
        accumContent(XalanUnicode::charLetter_t);
        accumContent(XalanUnicode::charSemicolon);
    }
    else if (XalanUnicode::charGreaterThanSign == ch) 
    {
        accumContent(XalanUnicode::charAmpersand);
        accumContent(XalanUnicode::charLetter_g);
        accumContent(XalanUnicode::charLetter_t);
        accumContent(XalanUnicode::charSemicolon);
    }
    else if (XalanUnicode::charAmpersand == ch) 
    {
        accumContent(XalanUnicode::charAmpersand);
        accumContent(XalanUnicode::charLetter_a);
        accumContent(XalanUnicode::charLetter_m);
        accumContent(XalanUnicode::charLetter_p);
        accumContent(XalanUnicode::charSemicolon);
    }
    else if (XalanUnicode::charQuoteMark == ch) 
    {
        accumContent(XalanUnicode::charAmpersand);
        accumContent(XalanUnicode::charLetter_q);
        accumContent(XalanUnicode::charLetter_u);
        accumContent(XalanUnicode::charLetter_o);
        accumContent(XalanUnicode::charLetter_t);
        accumContent(XalanUnicode::charSemicolon);
    }
    else if (XalanUnicode::charApostrophe == ch) 
    {
        accumContent(XalanUnicode::charAmpersand);
        accumContent(XalanUnicode::charLetter_a);
        accumContent(XalanUnicode::charLetter_p);
        accumContent(XalanUnicode::charLetter_o);
        accumContent(XalanUnicode::charLetter_s);
        accumContent(XalanUnicode::charSemicolon);
    }
    else
    {
        return false;
    }

    return true;
}



void
FormatterToXML::flushChars()
{
    assert(m_charBuf.empty() == false && m_charBuf.size() >= m_pos);

    m_writer->write(&m_charBuf[0], 0, m_pos);

    m_pos = 0;
}



void
FormatterToXML::flushBytes()
{
    assert(m_byteBuf.empty() == false && m_byteBuf.size() >= m_pos);

    m_writer->write(&m_byteBuf[0], 0, m_pos);

    m_pos = 0;
}



void
FormatterToXML::flushWriter()
{
    assert(m_writer != 0);

    m_writer->flush();
}



void
FormatterToXML::setDocumentLocator(const Locator* const     /* locator */)
{
    // I don't do anything with this yet.
}



void
FormatterToXML::startDocument()
{
    // Clear the buffer, just in case...
    m_stringBuffer.clear();

    m_needToOutputDocTypeDecl = true;
    m_startNewLine = false;

    if(m_shouldWriteXMLHeader == true)
    {
        // "<?xml version=\""
        accumName(s_xmlHeaderStartString, 0, s_xmlHeaderStartStringLength);

        if (m_version.empty() == false)
        {
            accumName(m_version);
        }
        else
        {
            accumName(s_defaultVersionString, 0, s_defaultVersionStringLength);
        }

        // "\" encoding=\""
        accumName(s_xmlHeaderEncodingString, 0, s_xmlHeaderEncodingStringLength);

        accumName(m_encoding);

        if (m_standalone.empty() == false)
        {
            accumName(s_xmlHeaderStandaloneString, 0, s_xmlHeaderStandaloneStringLength);
            accumName(m_standalone);
        }

        accumName(s_xmlHeaderEndString, 0, s_xmlHeaderEndStringLength);

        if(m_doIndent)
        {
            outputLineSep();
        }
    }      

}



void
FormatterToXML::endDocument()
{
    assert(m_flushFunction != 0);

    if(m_doIndent == true && m_isprevtext == false)
    {
        outputLineSep();
    }

    if (m_shouldFlush == true)
    {
        (this->*m_flushFunction)();
    }

    flushWriter();

}



void
FormatterToXML::startElement(
            const XMLCh* const  name,
            AttributeListType&  attrs)
{
    if(true == m_needToOutputDocTypeDecl &&
        m_doctypeSystem.empty() == false)
    {
        outputDocTypeDecl(name);

        m_needToOutputDocTypeDecl = false;
    }

    writeParentTagEnd();

    m_ispreserve = false;

    if (shouldIndent() == true &&
        m_startNewLine == true) 
    {
        indent(m_currentIndent);
    }

    m_startNewLine = true;

    accumName(XalanUnicode::charLessThanSign);
    accumName(name);

    const XalanSize_t  nAttrs = attrs.getLength();

    for (XalanSize_t i = 0;  i < nAttrs ;  i++)
    {
        processAttribute(attrs.getName(i), attrs.getValue(i));
    }

    // Flag the current element as not yet having any children.
    openElementForChildren();

    m_currentIndent += m_indent;

    m_isprevtext = false;
}



void
FormatterToXML::endElement(const XMLCh* const   name)
{
    m_currentIndent -= m_indent;

    const bool  hasChildNodes = childNodesWereAdded();

    if (hasChildNodes == true) 
    {
        if (shouldIndent() == true)
        {
            indent(m_currentIndent);
        }

        accumName(XalanUnicode::charLessThanSign);
        accumName(XalanUnicode::charSolidus);
        accumName(name);
    }
    else
    {
        if(m_spaceBeforeClose == true)
        {
            accumName(XalanUnicode::charSpace);
        }

        accumName(XalanUnicode::charSolidus);
    }

    accumName(XalanUnicode::charGreaterThanSign);

    if (hasChildNodes == true) 
    {
        if (m_preserves.empty() == true)
        {
            m_ispreserve = false;
        }
        else
        {
            m_ispreserve = m_preserves.back();

            m_preserves.pop_back();
        }
    }

    m_isprevtext = false;
}



void
FormatterToXML::processingInstruction(
            const XMLCh* const  target,
            const XMLCh* const  data)
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
        writeParentTagEnd();

        if (shouldIndent() == true)  
        {
            indent(m_currentIndent);
        }

        accumName(XalanUnicode::charLessThanSign);
        accumName(XalanUnicode::charQuestionMark);
        accumName(target);

        const XalanDOMString::size_type     len = length(data);

        if ( len > 0 && !isXMLWhitespace(data[0]))
        {
            accumName(XalanUnicode::charSpace);
        }

        accumNormalizedPIData(data, len);

        accumName(XalanUnicode::charQuestionMark);
        accumName(XalanUnicode::charGreaterThanSign);

        m_startNewLine = true;
    }

}



void
FormatterToXML::characters(
            const XMLCh* const  chars,
            const size_type     length)
{
    if(length != 0)
    {
        if(m_inCData == true)
        {
            cdata(chars, length);
        }
        else if(m_nextIsRaw)
        {
            m_nextIsRaw = false;

            charactersRaw(chars, length);
        }
        else
        {
            writeParentTagEnd();

            m_ispreserve = true;

            size_type    i = 0;
            size_type   firstIndex = 0;

            while(i < length) 
            {
                const XalanDOMChar  ch = chars[i];

                if((ch < SPECIALSSIZE &&
                    m_charsMap[ch] == 'S') ||
                    ch > m_maxCharacter)
                {
                    accumContent(chars, firstIndex, i - firstIndex);

                    i = accumDefaultEscape(ch, i, chars, length, false);

                    ++i;

                    firstIndex = i;
                }
                else
                {
                    ++i;
                }
            }

            accumContent(chars, firstIndex, i - firstIndex);

            if (m_isprevtext == false)
            {
                m_isprevtext = true;
            }
        }
    }
}



void
FormatterToXML::charactersRaw(
        const XMLCh* const  chars,
        const size_type     length)
{
    writeParentTagEnd();

    m_ispreserve = true;

    accumContent(chars, 0, length);
}



Writer*
FormatterToXML::getWriter() const
{
    return m_writer;
}



const XalanDOMString&
FormatterToXML::getDoctypeSystem() const
{
    return m_doctypeSystem;
}



const XalanDOMString&
FormatterToXML::getDoctypePublic() const
{
    return m_doctypePublic;
}



const XalanDOMString&
FormatterToXML::getEncoding() const
{
    return m_encoding;
}



const XalanDOMString&
FormatterToXML::getMediaType() const
{
    return m_mediaType;
}


int
FormatterToXML::getIndent() const
{
    return m_indent;
}



void
FormatterToXML::writeAttrString(
            const XalanDOMChar*     theString,
            size_type               theStringLength)
{
    assert(theString != 0);

    size_type   i = 0;
    size_type   firstIndex = 0;

    while(i < theStringLength)
    {
        const XalanDOMChar  ch = theString[i];

        if((ch < SPECIALSSIZE &&
            m_attrCharsMap[ch] == 'S') ||
            ch > m_maxCharacter)
        {
            accumContent(theString, firstIndex, i - firstIndex);

            i = accumDefaultEscape(ch, i, theString, theStringLength, true);

            ++i;

            firstIndex = i;
        }
        else
        {
            ++i;
        }
    }

    accumContent(theString, firstIndex, i - firstIndex);
}



void
FormatterToXML::accumCommentData(const XalanDOMChar*    data)
{
    accumContent(data);
}



void
FormatterToXML::writeNormalizedChars(
            const XalanDOMChar  ch[],
            size_type           start,
            size_type           length,
            bool                isCData)
{
    size_type   end = start + length;

    for(size_type i = start; i < end; ++i)
    {
        const XalanDOMChar  c = ch[i];

        if (XalanUnicode::charCR == c &&
            i + 1 < end &&
            XalanUnicode::charLF == ch[i + 1])
        {
            outputLineSep();

            i++;
        }
        else if(XalanUnicode::charLF == c)
        {
            outputLineSep();
        }
        else if(isCData == true && c > m_maxCharacter)
        {
            if(i != 0)
            {
                accumContent(s_dtdCDATACloseString, 0, s_dtdCDATACloseStringLength);
            }

            // This needs to go into a function... 
            if (0xd800u <= unsigned(c) && unsigned(c) < 0xdc00) 
            {
                // UTF-16 surrogate
                XalanDOMChar    next = 0;

                if (i + 1 >= end) 
                {
                    throwInvalidUTF16SurrogateException(c,getMemoryManager());
                }
                else 
                {
                    next = ch[++i];

                    if (!(0xdc00 <= next && next < 0xe000))
                    {
                        throwInvalidUTF16SurrogateException(c, next, getMemoryManager());
                    }

                    next = XalanDOMChar(((c - 0xd800) << 10) + next - 0xdc00 + 0x00010000);
                }

                writeNumberedEntityReference(next);
            }
            else
            {
                writeNumberedEntityReference(c);
            }

            if(i != 0 && i < end - 1)
            {
                // "<![CDATA["
                accumContent(XalanUnicode::charLessThanSign);
                accumContent(XalanUnicode::charExclamationMark);
                accumContent(XalanUnicode::charLeftSquareBracket);
                accumContent(XalanUnicode::charLetter_C);
                accumContent(XalanUnicode::charLetter_D);
                accumContent(XalanUnicode::charLetter_A);
                accumContent(XalanUnicode::charLetter_T);
                accumContent(XalanUnicode::charLetter_A);
                accumContent(XalanUnicode::charLeftSquareBracket);
            }
        }
        else if(isCData == true &&
                i < end - 2 &&
                XalanUnicode::charRightSquareBracket == c &&
                XalanUnicode::charRightSquareBracket == ch[i + 1] &&
                XalanUnicode::charGreaterThanSign == ch[ i + 2])
        {
            // "]]]]><![CDATA[>"
            accumContent(XalanUnicode::charRightSquareBracket);
            accumContent(XalanUnicode::charRightSquareBracket);
            accumContent(XalanUnicode::charRightSquareBracket);
            accumContent(XalanUnicode::charRightSquareBracket);
            accumContent(XalanUnicode::charGreaterThanSign);
            accumContent(XalanUnicode::charLessThanSign);
            accumContent(XalanUnicode::charExclamationMark);
            accumContent(XalanUnicode::charLeftSquareBracket);
            accumContent(XalanUnicode::charLetter_C);
            accumContent(XalanUnicode::charLetter_D);
            accumContent(XalanUnicode::charLetter_A);
            accumContent(XalanUnicode::charLetter_T);
            accumContent(XalanUnicode::charLetter_A);
            accumContent(XalanUnicode::charLeftSquareBracket);
            accumContent(XalanUnicode::charGreaterThanSign);

            i += 2;
        }
        else
        {
            if(c <= m_maxCharacter)
            {
                accumContent(c);
            }
            // This needs to go into a function...
            else if (0xd800 <= c && c < 0xdc00)
            {
                // UTF-16 surrogate
                if (i + 1 >= end) 
                {
                    throwInvalidUTF16SurrogateException(c, getMemoryManager());
                }
                else
                {
                    XalanUnicodeChar    next = ch[++i];

                    if (!(0xdc00 <= next && next < 0xe000))
                    {
                        throwInvalidUTF16SurrogateException(c, static_cast<XalanDOMChar>(next),
                                                            getMemoryManager());
                    }

                    next = ((c - 0xd800) << 10) + next - 0xdc00 + 0x00010000;

                    writeNumberedEntityReference(next);
                }
            }
            else
            {
                writeNumberedEntityReference(c);
            }
        }
    }
}



void
FormatterToXML::writeNumberedEntityReference(XalanUnicodeChar   theNumber)
{
    accumContent(XalanUnicode::charAmpersand);
    accumContent(XalanUnicode::charNumberSign);

    accumContent(NumberToDOMString(theNumber, m_stringBuffer));
    m_stringBuffer.clear();

    accumContent(XalanUnicode::charSemicolon);
}



void
FormatterToXML::entityReference(const XMLCh* const  name)
{
    writeParentTagEnd();
      
    if (shouldIndent() == true)  
    {
        indent(m_currentIndent);
    }

    accumName(XalanUnicode::charAmpersand);
    accumName(name);
    accumName(XalanUnicode::charSemicolon);
}



void
FormatterToXML::ignorableWhitespace(
            const XMLCh* const  chars,
            const size_type     length)
{
#if 1
    // We need to do normalization, which is slower,
    // but there you have it...
    if (length > 0)
    {
        characters(chars, length);
    }
#else
    // We'ed like to be able to do this...
    if(length != 0)
    {
        assert(isXMLWhitespace(chars, 0, length));

        if(m_inCData == true)
        {
            cdata(chars, length);
        }
        else if(m_nextIsRaw)
        {
            m_nextIsRaw = false;

            charactersRaw(chars, length);
        }
        else
        {
            writeParentTagEnd();

            m_ispreserve = true;

            accumContent(chars, 0, length);

            if (m_isprevtext == false)
            {
                m_isprevtext = true;
            }
        }
    }
#endif
}



void
FormatterToXML::resetDocument()
{
    // I don't do anything with this yet.
}



void
FormatterToXML::comment(const XMLCh* const  data)
{

    writeParentTagEnd();

    if (shouldIndent() == true)  
    {
        indent(m_currentIndent);
    }

    accumName(XalanUnicode::charLessThanSign);
    accumName(XalanUnicode::charExclamationMark);
    accumName(XalanUnicode::charHyphenMinus);
    accumName(XalanUnicode::charHyphenMinus);

    accumCommentData(data);

    accumName(XalanUnicode::charHyphenMinus);
    accumName(XalanUnicode::charHyphenMinus);
    accumName(XalanUnicode::charGreaterThanSign);

    m_startNewLine = true;

}



void
FormatterToXML::cdata(
            const XMLCh* const  ch,
            const size_type     length)
{
    if(m_nextIsRaw == true)
    {
        m_nextIsRaw = false;

        charactersRaw(ch, length);
    }
    else
    {
        if(m_escapeCData) // Should normally always be false.
        {
            characters(ch, length);
        }
        else
        {
            writeParentTagEnd();

            m_ispreserve = true;

            if (shouldIndent() == true)
            {
                indent(m_currentIndent);
            }

            if(m_stripCData == false)
            {
                if(length >= 1 &&
                   ch[0] <= m_maxCharacter)
                {
                    // "<![CDATA["
                    accumContent(XalanUnicode::charLessThanSign);
                    accumContent(XalanUnicode::charExclamationMark);
                    accumContent(XalanUnicode::charLeftSquareBracket);
                    accumContent(XalanUnicode::charLetter_C);
                    accumContent(XalanUnicode::charLetter_D);
                    accumContent(XalanUnicode::charLetter_A);
                    accumContent(XalanUnicode::charLetter_T);
                    accumContent(XalanUnicode::charLetter_A);
                    accumContent(XalanUnicode::charLeftSquareBracket);
                }
            }

            writeNormalizedChars(ch, 0, length, !m_stripCData);

            if(m_stripCData == false)
            {
                if(length >= 1 &&
                   ch[length - 1] <= m_maxCharacter)
                {
                    accumContent(XalanUnicode::charRightSquareBracket);
                    accumContent(XalanUnicode::charRightSquareBracket);
                    accumContent(XalanUnicode::charGreaterThanSign);
                }
            }
        }
    }
}



void
FormatterToXML::writeParentTagEnd()
{
    if(!m_elemStack.empty())
    {
        // See if the parent element has already been flagged as having children.
        if(false == m_elemStack.back())
        {
            accumContent(XalanUnicode::charGreaterThanSign);

            m_isprevtext = false;

            m_elemStack.back() = true;

            m_preserves.push_back(m_ispreserve);
        }
    }
}



void
FormatterToXML::openElementForChildren()
{
    m_elemStack.push_back(false);
}



bool
FormatterToXML::childNodesWereAdded()
{
    bool    fResult = false;

    if (m_elemStack.empty() == false)
    {
        fResult = m_elemStack.back();

        m_elemStack.pop_back();
    }

    return fResult;
}



void
FormatterToXML::processAttribute(
            const XalanDOMChar*     name,
            const XalanDOMChar*     value)
{
    accumContent(XalanUnicode::charSpace);
    accumName(name);
    accumContent(XalanUnicode::charEqualsSign);
    accumContent(XalanUnicode::charQuoteMark);
    writeAttrString(value, length(value));
    accumContent(XalanUnicode::charQuoteMark);
}



void
FormatterToXML::outputLineSep()
{
    assert(m_newlineString != 0 && length(m_newlineString) == m_newlineStringLength);

    accumContent(m_newlineString, 0, m_newlineStringLength);
}



void
FormatterToXML::printSpace(int n)
{
    for (int i = 0;  i < n;  i ++)
    {
        accumContent(XalanUnicode::charSpace);
    }
}



void
FormatterToXML::indent(int  n)
{
    if(m_startNewLine == true)
    {
        outputLineSep();
    }

    if(m_doIndent == true)
    {
        printSpace(n);
    }
}



void
FormatterToXML::accumNormalizedPIData(
            const XalanDOMChar*     theData,
            size_type               theLength)
{
    for (size_type i = 0; i < theLength; ++i)
    {
        accumContent(theData[i]);
    }
}



#define FXML_SIZE(str)  ((sizeof(str) / sizeof(str[0]) - 1))

const XalanDOMChar                      FormatterToXML::s_doctypeHeaderStartString[] =
{
    XalanUnicode::charLessThanSign,
    XalanUnicode::charExclamationMark,
    XalanUnicode::charLetter_D,
    XalanUnicode::charLetter_O,
    XalanUnicode::charLetter_C,
    XalanUnicode::charLetter_T,
    XalanUnicode::charLetter_Y,
    XalanUnicode::charLetter_P,
    XalanUnicode::charLetter_E,
    XalanUnicode::charSpace,
    0
};

const FormatterToXML::size_type         FormatterToXML::s_doctypeHeaderStartStringLength =
        FXML_SIZE(s_doctypeHeaderStartString);

const XalanDOMChar                      FormatterToXML::s_doctypeHeaderPublicString[] =
{
    XalanUnicode::charSpace,
    XalanUnicode::charLetter_P,
    XalanUnicode::charLetter_U,
    XalanUnicode::charLetter_B,
    XalanUnicode::charLetter_L,
    XalanUnicode::charLetter_I,
    XalanUnicode::charLetter_C,
    XalanUnicode::charSpace,
    XalanUnicode::charQuoteMark,
    0
};

const FormatterToXML::size_type     FormatterToXML::s_doctypeHeaderPublicStringLength =
        FXML_SIZE(s_doctypeHeaderPublicString);

const XalanDOMChar                  FormatterToXML::s_doctypeHeaderSystemString[] =
{
    XalanUnicode::charSpace,
    XalanUnicode::charLetter_S,
    XalanUnicode::charLetter_Y,
    XalanUnicode::charLetter_S,
    XalanUnicode::charLetter_T,
    XalanUnicode::charLetter_E,
    XalanUnicode::charLetter_M,
    XalanUnicode::charSpace,
    XalanUnicode::charQuoteMark,
    0
};

const FormatterToXML::size_type     FormatterToXML::s_doctypeHeaderSystemStringLength =
        FXML_SIZE(s_doctypeHeaderSystemString);

const XalanDOMChar                  FormatterToXML::s_xmlHeaderStartString[] =
{
    XalanUnicode::charLessThanSign,
    XalanUnicode::charQuestionMark,
    XalanUnicode::charLetter_x,
    XalanUnicode::charLetter_m,
    XalanUnicode::charLetter_l,
    XalanUnicode::charSpace,
    XalanUnicode::charLetter_v,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_n,
    XalanUnicode::charEqualsSign,
    XalanUnicode::charQuoteMark,
    0
};

const FormatterToXML::size_type     FormatterToXML::s_xmlHeaderStartStringLength =
        FXML_SIZE(s_xmlHeaderStartString);

const XalanDOMChar                  FormatterToXML::s_xmlHeaderEncodingString[] =
{
    XalanUnicode::charQuoteMark,
    XalanUnicode::charSpace,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_d,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_g,
    XalanUnicode::charEqualsSign,
    XalanUnicode::charQuoteMark,
    0
};

const FormatterToXML::size_type     FormatterToXML::s_xmlHeaderEncodingStringLength =
        FXML_SIZE(s_xmlHeaderEncodingString);

const XalanDOMChar                  FormatterToXML::s_xmlHeaderStandaloneString[] =
{
    XalanUnicode::charQuoteMark,
    XalanUnicode::charSpace,
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_d,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_e,
    XalanUnicode::charEqualsSign,
    XalanUnicode::charQuoteMark,
    0
};

const FormatterToXML::size_type     FormatterToXML::s_xmlHeaderStandaloneStringLength =
        FXML_SIZE(s_xmlHeaderStandaloneString);

const XalanDOMChar                  FormatterToXML::s_xmlHeaderEndString[] =
{
    XalanUnicode::charQuoteMark,
    XalanUnicode::charQuestionMark,
    XalanUnicode::charGreaterThanSign,
    0
};

const FormatterToXML::size_type     FormatterToXML::s_xmlHeaderEndStringLength =
        FXML_SIZE(s_xmlHeaderEndString);

const XalanDOMChar                  FormatterToXML::s_defaultVersionString[] =
{
    XalanUnicode::charDigit_1,
    XalanUnicode::charFullStop,
    XalanUnicode::charDigit_0,
    0
};

const FormatterToXML::size_type     FormatterToXML::s_defaultVersionStringLength =
        FXML_SIZE(s_defaultVersionString);

const XalanDOMChar                  FormatterToXML::s_dtdCDATACloseString[] =
{
    XalanUnicode::charRightSquareBracket,
    XalanUnicode::charRightSquareBracket,
    XalanUnicode::charGreaterThanSign,
    0
};

const FormatterToXML::size_type     FormatterToXML::s_dtdCDATACloseStringLength =
        FXML_SIZE(s_dtdCDATACloseString);


const XalanDOMChar                  FormatterToXML::s_xhtmlDocTypeString[] =
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
    0
};

const FormatterToXML::size_type     FormatterToXML::s_xhtmlDocTypeStringLength =
        FXML_SIZE(s_xhtmlDocTypeString);


const XalanDOMString::size_type     FormatterToXML::s_maxBufferSize = 512;



XALAN_CPP_NAMESPACE_END
