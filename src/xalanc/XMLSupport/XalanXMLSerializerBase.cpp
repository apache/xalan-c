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
#include "XalanXMLSerializerBase.hpp"



#include "xercesc/sax/SAXException.hpp"



#include "xalanc/PlatformSupport/DOMStringHelper.hpp"
#include "xalanc/PlatformSupport/XalanMessageLoader.hpp"
#include "xalanc/PlatformSupport/XalanTranscodingServices.hpp"
#include "xalanc/PlatformSupport/Writer.hpp"



XALAN_CPP_NAMESPACE_BEGIN

const size_t  XalanXMLSerializerBase::CharFunctor1_0::s_lastSpecial = 0x7fu;

const char  XalanXMLSerializerBase::CharFunctor1_0::s_specialChars[s_lastSpecial + 1] =
{
 /* 00-07 */    eForb,  eForb,  eForb,  eForb,  eForb,  eForb,  eForb,  eForb,
 /* 08-0F */    eForb,  eAttr,  eBoth,  eForb,  eForb,  eBoth,  eForb,  eForb,
 /* 10-17 */    eForb,  eForb,  eForb,  eForb,  eForb,  eForb,  eForb,  eForb,
 /* 18-1F */    eForb,  eForb,  eForb,  eForb,  eForb,  eForb,  eForb,  eForb,
 /* 20-27 */    eNone,  eNone,  eAttr,  eNone,  eNone,  eNone,  eBoth,  eNone,
 /* 28-2F */    eNone,  eNone,  eNone,  eNone,  eNone,  eNone,  eNone,  eNone,
 /* 30-37 */    eNone,  eNone,  eNone,  eNone,  eNone,  eNone,  eNone,  eNone,
 /* 38-3F */    eNone,  eNone,  eNone,  eNone,  eBoth,  eNone,  eBoth,  eNone,
 /* 40-47 */    eNone,  eNone,  eNone,  eNone,  eNone,  eNone,  eNone,  eNone,
 /* 48-4F */    eNone,  eNone,  eNone,  eNone,  eNone,  eNone,  eNone,  eNone,
 /* 50-57 */    eNone,  eNone,  eNone,  eNone,  eNone,  eNone,  eNone,  eNone,
 /* 58-5F */    eNone,  eNone,  eNone,  eNone,  eNone,  eNone,  eNone,  eNone,
 /* 60-67 */    eNone,  eNone,  eNone,  eNone,  eNone,  eNone,  eNone,  eNone,
 /* 68-6F */    eNone,  eNone,  eNone,  eNone,  eNone,  eNone,  eNone,  eNone,
 /* 70-77 */    eNone,  eNone,  eNone,  eNone,  eNone,  eNone,  eNone,  eNone,
 /* 78-7F */    eNone,  eNone,  eNone,  eNone,  eNone,  eNone,  eNone
};


const size_t  XalanXMLSerializerBase::CharFunctor1_1::s_lastSpecial = 0x9fu;

const char  XalanXMLSerializerBase::CharFunctor1_1::s_specialChars[s_lastSpecial + 1] =
{
 /* 00-07 */    eNone,  eCRFb,  eCRFb,  eCRFb,  eCRFb,  eCRFb,  eCRFb,  eCRFb,
 /* 08-0F */    eCRFb,  eCRFb,  eCRFb,  eCRFb,  eCRFb,  eCRFb,  eCRFb,  eCRFb,
 /* 10-17 */    eCRFb,  eCRFb,  eCRFb,  eCRFb,  eCRFb,  eCRFb,  eCRFb,  eCRFb,
 /* 18-1F */    eCRFb,  eCRFb,  eCRFb,  eCRFb,  eCRFb,  eCRFb,  eCRFb,  eCRFb,
 /* 20-27 */    eNone,  eNone,  eAttr,  eNone,  eNone,  eNone,  eBoth,  eNone,
 /* 28-2F */    eNone,  eNone,  eNone,  eNone,  eNone,  eNone,  eNone,  eNone,
 /* 30-37 */    eNone,  eNone,  eNone,  eNone,  eNone,  eNone,  eNone,  eNone,
 /* 38-3F */    eNone,  eNone,  eNone,  eNone,  eBoth,  eNone,  eBoth,  eNone,
 /* 40-47 */    eNone,  eNone,  eNone,  eNone,  eNone,  eNone,  eNone,  eNone,
 /* 48-4F */    eNone,  eNone,  eNone,  eNone,  eNone,  eNone,  eNone,  eNone,
 /* 50-57 */    eNone,  eNone,  eNone,  eNone,  eNone,  eNone,  eNone,  eNone,
 /* 58-5F */    eNone,  eNone,  eNone,  eNone,  eNone,  eNone,  eNone,  eNone,
 /* 60-67 */    eNone,  eNone,  eNone,  eNone,  eNone,  eNone,  eNone,  eNone,
 /* 68-6F */    eNone,  eNone,  eNone,  eNone,  eNone,  eNone,  eNone,  eNone,
 /* 70-77 */    eNone,  eNone,  eNone,  eNone,  eNone,  eNone,  eNone,  eNone,
 /* 78-7F */    eNone,  eNone,  eNone,  eNone,  eNone,  eNone,  eNone,  eCRFb,
 /* 80-87 */    eCRFb,  eCRFb,  eCRFb,  eCRFb,  eCRFb,  eBoth,  eCRFb,  eCRFb,
 /* 88-8F */    eCRFb,  eCRFb,  eCRFb,  eCRFb,  eCRFb,  eCRFb,  eCRFb,  eCRFb,
 /* 90-97 */    eCRFb,  eCRFb,  eCRFb,  eCRFb,  eCRFb,  eCRFb,  eCRFb,  eCRFb,
 /* 98-9F */    eCRFb,  eCRFb,  eCRFb,  eCRFb,  eCRFb,  eCRFb,  eCRFb,  eCRFb
};


XalanXMLSerializerBase::XalanXMLSerializerBase(
            MemoryManager&              theManager,
            eXMLVersion                 theXMLVersion,
            const XalanDOMString&       theEncoding,
            const XalanDOMString&       theDoctypeSystem,
            const XalanDOMString&       theDoctypePublic,
            bool                        xmlDecl,
            const XalanDOMString&       theStandalone) :
    FormatterListener(OUTPUT_METHOD_XML),
    m_nextIsRaw(false),
    m_spaceBeforeClose(false),
    m_doctypeSystem(theDoctypeSystem, theManager),
    m_doctypePublic(theDoctypePublic, theManager),
    m_version(
        theXMLVersion == XML_VERSION_1_0 ?
            s_1_0String :
            s_1_1String),
    m_standalone(theStandalone, theManager),
    m_encoding(theEncoding, theManager),
    m_needToOutputDoctypeDecl(false),
    // We must write the XML declaration if standalone is specified
    m_shouldWriteXMLHeader(xmlDecl == true ? true : theStandalone.length() != 0),
    m_elemStack(theManager)
{
    setXMLVersion(theXMLVersion);

    if(m_doctypePublic.empty() == false)
    {
        if(startsWith(
            m_doctypePublic,
            s_xhtmlDocTypeString) == true)
        {
            m_spaceBeforeClose = true;
        }
    }
}



XalanXMLSerializerBase::~XalanXMLSerializerBase()
{
}



XalanUnicodeChar
XalanXMLSerializerBase::decodeUTF16SurrogatePair(
            XalanDOMChar    theHighSurrogate,
            XalanDOMChar    theLowSurrogate,
            MemoryManager& theManager)
{
    assert(isUTF16HighSurrogate(theHighSurrogate) == true);

    if (isUTF16LowSurrogate(theLowSurrogate) == false)
    {
        throwInvalidUTF16SurrogateException(
            theHighSurrogate,
            theLowSurrogate,
            theManager);
    }

    return ((theHighSurrogate - 0xD800u) << 10) + theLowSurrogate - 0xDC00u + 0x00010000u;
}



XALAN_USING_XERCES(SAXException)

void
XalanXMLSerializerBase::throwInvalidUTF16SurrogateException(
            XalanDOMChar        ch,
            MemoryManager&  theManager)
{
    XalanDOMString  theMessage(theManager);
    XalanDOMString  theBuffer(theManager);

    XalanMessageLoader::getMessage(
        theMessage,
        XalanMessages::InvalidHighSurrogate_1Param,
        NumberToHexDOMString(ch, theBuffer));

    throw SAXException(theMessage.c_str(), &theManager);
}



void
XalanXMLSerializerBase::throwInvalidUTF16SurrogateException(
            XalanDOMChar    ch,
            XalanDOMChar    next,
            MemoryManager&  theManager)
{
    XalanDOMString  theMessage(theManager);
    XalanDOMString  theBuffer1(theManager);
    XalanDOMString  theBuffer2(theManager);

    XalanMessageLoader::getMessage(
        theMessage,
        XalanMessages::InvalidSurrogatePair_2Param,
        NumberToHexDOMString(ch, theBuffer1),
        NumberToHexDOMString(next, theBuffer2));

    throw SAXException(theMessage.c_str(), &theManager);
}



void
XalanXMLSerializerBase::throwInvalidCharacterException(
            XalanUnicodeChar    ch,
            MemoryManager&      theManager)
{
    XalanDOMString  theMessage(theManager);
    XalanDOMString  theBuffer(theManager);  

    XalanMessageLoader::getMessage(
        theMessage,
        XalanMessages::InvalidScalar_1Param,
        NumberToHexDOMString(ch, theBuffer));

    throw SAXException(theMessage.c_str(), &theManager);
}



void
XalanXMLSerializerBase::throwInvalidXMLCharacterException(
            XalanUnicodeChar        ch,
            const XalanDOMString&   theXMLversion,
            MemoryManager&          theManager)
{
    XalanDOMString  theMessage(theManager);
    XalanDOMString  theBuffer(theManager);  

    XalanMessageLoader::getMessage(
        theMessage,
        XalanMessages::ForbiddenXMLCharacter_2Param,
        NumberToHexDOMString(ch, theBuffer),
        theXMLversion);

    throw SAXException(theMessage.c_str(), &theManager);
}


void
XalanXMLSerializerBase::setDocumentLocator(const Locator* const     /* locator */)
{
}



void
XalanXMLSerializerBase::startDocument()
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




void
XalanXMLSerializerBase::characters(
            const XMLCh* const  chars,
            const size_type     length)
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



void
XalanXMLSerializerBase::cdata(
            const XMLCh* const  ch,
            const size_type     length)
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



void
XalanXMLSerializerBase::processingInstruction(
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
        writeProcessingInstruction(target, data);
    }
}



void
XalanXMLSerializerBase::ignorableWhitespace(
            const XMLCh* const  chars,
            const size_type     length)
{
    if (length > 0)
    {
        characters(chars, length);
    }
}



const XalanDOMString&
XalanXMLSerializerBase::getDoctypeSystem() const
{
    return m_doctypeSystem;
}



const XalanDOMString&
XalanXMLSerializerBase::getDoctypePublic() const
{
    return m_doctypePublic;
}



const XalanDOMString&
XalanXMLSerializerBase::getEncoding() const
{
    return m_encoding;
}



void
XalanXMLSerializerBase::resetDocument()
{
    // I don't do anything with this yet.
}



#define FXML_SIZE(str)  ((sizeof(str) / sizeof(str[0]) - 1))

const XalanDOMChar  XalanXMLSerializerBase::s_xhtmlDocTypeString[] =
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

const XalanXMLSerializerBase::size_type     XalanXMLSerializerBase::s_xhtmlDocTypeStringLength =
        FXML_SIZE(s_xhtmlDocTypeString);


static XalanDOMString   s_localUTF8String(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString   s_localUTF16String(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString   s_local1_0String(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString   s_local1_1String(XalanMemMgrs::getDummyMemMgr());

static const XalanDOMChar   s_1_1VersionString[] =
{
    XalanUnicode::charDigit_1,
    XalanUnicode::charFullStop,
    XalanUnicode::charDigit_1,
    XalanDOMChar(0)
};



void
XalanXMLSerializerBase::initialize(MemoryManager&   theManager)
{
    {
        XalanDOMString theTmp(XalanTranscodingServices::s_utf8String, theManager);

        s_localUTF8String.swap(theTmp);
    }

    {
        XalanDOMString theTmp(XalanTranscodingServices::s_utf16String, theManager);

        s_localUTF16String.swap(theTmp);
    }

    {
        XalanDOMString theTmp(UTF16::s_defaultVersionString, theManager);

        s_local1_0String.swap(theTmp);
    }

    {
        XalanDOMString theTmp(s_1_1VersionString, theManager);

        s_local1_1String.swap(theTmp);
    }
}



void
XalanXMLSerializerBase::terminate()
{
    {
        XalanDOMString   temp(XalanMemMgrs::getDummyMemMgr());

        temp.swap(s_localUTF8String);
    }

    {
        XalanDOMString   temp(XalanMemMgrs::getDummyMemMgr());

        temp.swap(s_localUTF16String);
    }

    {
        XalanDOMString   temp(XalanMemMgrs::getDummyMemMgr());

        temp.swap(s_local1_0String);
    }

    {
        XalanDOMString   temp(XalanMemMgrs::getDummyMemMgr());

        temp.swap(s_local1_1String);
    }
}



const XalanDOMString&   XalanXMLSerializerBase::UTF8::s_encodingString = s_localUTF8String;
const XalanDOMString&   XalanXMLSerializerBase::UTF16::s_encodingString = s_localUTF16String;
const XalanDOMString&   XalanXMLSerializerBase::s_1_0String = s_local1_0String;
const XalanDOMString&   XalanXMLSerializerBase::s_1_1String = s_local1_1String;



const char  XalanXMLSerializerBase::UTF8::s_doctypeHeaderStartString[] =
{
    char(XalanUnicode::charLessThanSign),
    char(XalanUnicode::charExclamationMark),
    char(XalanUnicode::charLetter_D),
    char(XalanUnicode::charLetter_O),
    char(XalanUnicode::charLetter_C),
    char(XalanUnicode::charLetter_T),
    char(XalanUnicode::charLetter_Y),
    char(XalanUnicode::charLetter_P),
    char(XalanUnicode::charLetter_E),
    char(XalanUnicode::charSpace),
    char(0)
};

const XalanXMLSerializerBase::size_type     XalanXMLSerializerBase::UTF8::s_doctypeHeaderStartStringLength =
        FXML_SIZE(s_doctypeHeaderStartString);

const char  XalanXMLSerializerBase::UTF8::s_doctypeHeaderPublicString[] =
{
    char(XalanUnicode::charSpace),
    char(XalanUnicode::charLetter_P),
    char(XalanUnicode::charLetter_U),
    char(XalanUnicode::charLetter_B),
    char(XalanUnicode::charLetter_L),
    char(XalanUnicode::charLetter_I),
    char(XalanUnicode::charLetter_C),
    char(XalanUnicode::charSpace),
    char(XalanUnicode::charQuoteMark),
    char(0)
};

const XalanXMLSerializerBase::size_type     XalanXMLSerializerBase::UTF8::s_doctypeHeaderPublicStringLength =
        FXML_SIZE(s_doctypeHeaderPublicString);

const char  XalanXMLSerializerBase::UTF8::s_doctypeHeaderSystemString[] =
{
    char(XalanUnicode::charSpace),
    char(XalanUnicode::charLetter_S),
    char(XalanUnicode::charLetter_Y),
    char(XalanUnicode::charLetter_S),
    char(XalanUnicode::charLetter_T),
    char(XalanUnicode::charLetter_E),
    char(XalanUnicode::charLetter_M),
    char(XalanUnicode::charSpace),
    char(XalanUnicode::charQuoteMark),
    char(0)
};

const XalanXMLSerializerBase::size_type     XalanXMLSerializerBase::UTF8::s_doctypeHeaderSystemStringLength =
        FXML_SIZE(s_doctypeHeaderSystemString);

const char  XalanXMLSerializerBase::UTF8::s_xmlHeaderStartString[] =
{
    char(XalanUnicode::charLessThanSign),
    char(XalanUnicode::charQuestionMark),
    char(XalanUnicode::charLetter_x),
    char(XalanUnicode::charLetter_m),
    char(XalanUnicode::charLetter_l),
    char(XalanUnicode::charSpace),
    char(XalanUnicode::charLetter_v),
    char(XalanUnicode::charLetter_e),
    char(XalanUnicode::charLetter_r),
    char(XalanUnicode::charLetter_s),
    char(XalanUnicode::charLetter_i),
    char(XalanUnicode::charLetter_o),
    char(XalanUnicode::charLetter_n),
    char(XalanUnicode::charEqualsSign),
    char(XalanUnicode::charQuoteMark),
    char(0)
};

const XalanXMLSerializerBase::size_type     XalanXMLSerializerBase::UTF8::s_xmlHeaderStartStringLength =
        FXML_SIZE(s_xmlHeaderStartString);

const char  XalanXMLSerializerBase::UTF8::s_xmlHeaderEncodingString[] =
{
    char(XalanUnicode::charQuoteMark),
    char(XalanUnicode::charSpace),
    char(XalanUnicode::charLetter_e),
    char(XalanUnicode::charLetter_n),
    char(XalanUnicode::charLetter_c),
    char(XalanUnicode::charLetter_o),
    char(XalanUnicode::charLetter_d),
    char(XalanUnicode::charLetter_i),
    char(XalanUnicode::charLetter_n),
    char(XalanUnicode::charLetter_g),
    char(XalanUnicode::charEqualsSign),
    char(XalanUnicode::charQuoteMark),
    char(0)
};

const XalanXMLSerializerBase::size_type     XalanXMLSerializerBase::UTF8::s_xmlHeaderEncodingStringLength =
        FXML_SIZE(s_xmlHeaderEncodingString);

const char  XalanXMLSerializerBase::UTF8::s_xmlHeaderStandaloneString[] =
{
    char(XalanUnicode::charQuoteMark),
    char(XalanUnicode::charSpace),
    char(XalanUnicode::charLetter_s),
    char(XalanUnicode::charLetter_t),
    char(XalanUnicode::charLetter_a),
    char(XalanUnicode::charLetter_n),
    char(XalanUnicode::charLetter_d),
    char(XalanUnicode::charLetter_a),
    char(XalanUnicode::charLetter_l),
    char(XalanUnicode::charLetter_o),
    char(XalanUnicode::charLetter_n),
    char(XalanUnicode::charLetter_e),
    char(XalanUnicode::charEqualsSign),
    char(XalanUnicode::charQuoteMark),
    char(0)
};

const XalanXMLSerializerBase::size_type     XalanXMLSerializerBase::UTF8::s_xmlHeaderStandaloneStringLength =
        FXML_SIZE(s_xmlHeaderStandaloneString);

const char  XalanXMLSerializerBase::UTF8::s_xmlHeaderEndString[] =
{
    char(XalanUnicode::charQuoteMark),
    char(XalanUnicode::charQuestionMark),
    char(XalanUnicode::charGreaterThanSign),
    char(0)
};

const XalanXMLSerializerBase::size_type     XalanXMLSerializerBase::UTF8::s_xmlHeaderEndStringLength =
        FXML_SIZE(s_xmlHeaderEndString);

const char  XalanXMLSerializerBase::UTF8::s_defaultVersionString[] =
{
    char(XalanUnicode::charDigit_1),
    char(XalanUnicode::charFullStop),
    char(XalanUnicode::charDigit_0),
    char(0)
};

const XalanXMLSerializerBase::size_type     XalanXMLSerializerBase::UTF8::s_defaultVersionStringLength =
        FXML_SIZE(s_defaultVersionString);

const char  XalanXMLSerializerBase::UTF8::s_cdataOpenString[] =
{
    char(XalanUnicode::charLessThanSign),
    char(XalanUnicode::charExclamationMark),
    char(XalanUnicode::charLeftSquareBracket),
    char(XalanUnicode::charLetter_C),
    char(XalanUnicode::charLetter_D),
    char(XalanUnicode::charLetter_A),
    char(XalanUnicode::charLetter_T),
    char(XalanUnicode::charLetter_A),
    char(XalanUnicode::charLeftSquareBracket),
    char(0)
};

const XalanXMLSerializerBase::size_type     XalanXMLSerializerBase::UTF8::s_cdataOpenStringLength =
        FXML_SIZE(s_cdataOpenString);

const char  XalanXMLSerializerBase::UTF8::s_cdataCloseString[] =
{
    char(XalanUnicode::charRightSquareBracket),
    char(XalanUnicode::charRightSquareBracket),
    char(XalanUnicode::charGreaterThanSign),
    char(0)
};

const XalanXMLSerializerBase::size_type     XalanXMLSerializerBase::UTF8::s_cdataCloseStringLength =
        FXML_SIZE(s_cdataCloseString);


const XalanDOMChar  XalanXMLSerializerBase::UTF8::s_xhtmlDocTypeString[] =
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

const XalanXMLSerializerBase::size_type     XalanXMLSerializerBase::UTF8::s_xhtmlDocTypeStringLength =
        FXML_SIZE(s_xhtmlDocTypeString);

const char  XalanXMLSerializerBase::UTF8::s_lessThanEntityString[] =
{
    char(XalanUnicode::charAmpersand),
    char(XalanUnicode::charLetter_l),
    char(XalanUnicode::charLetter_t),
    char(XalanUnicode::charSemicolon),
    char(0)
};

const XalanXMLSerializerBase::size_type     XalanXMLSerializerBase::UTF8::s_lessThanEntityStringLength =
        FXML_SIZE(s_lessThanEntityString);

const char  XalanXMLSerializerBase::UTF8::s_greaterThanEntityString[] =
{
    char(XalanUnicode::charAmpersand),
    char(XalanUnicode::charLetter_g),
    char(XalanUnicode::charLetter_t),
    char(XalanUnicode::charSemicolon),
    char(0)
};

const XalanXMLSerializerBase::size_type     XalanXMLSerializerBase::UTF8::s_greaterThanEntityStringLength =
        FXML_SIZE(s_greaterThanEntityString);

const char  XalanXMLSerializerBase::UTF8::s_ampersandEntityString[] =
{
    char(XalanUnicode::charAmpersand),
    char(XalanUnicode::charLetter_a),
    char(XalanUnicode::charLetter_m),
    char(XalanUnicode::charLetter_p),
    char(XalanUnicode::charSemicolon),
    char(0)
};

const XalanXMLSerializerBase::size_type     XalanXMLSerializerBase::UTF8::s_ampersandEntityStringLength =
        FXML_SIZE(s_ampersandEntityString);

const char  XalanXMLSerializerBase::UTF8::s_quoteEntityString[] =
{
    char(XalanUnicode::charAmpersand),
    char(XalanUnicode::charLetter_q),
    char(XalanUnicode::charLetter_u),
    char(XalanUnicode::charLetter_o),
    char(XalanUnicode::charLetter_t),
    char(XalanUnicode::charSemicolon),
    char(0)
};

const XalanXMLSerializerBase::size_type     XalanXMLSerializerBase::UTF8::s_quoteEntityStringLength =
        FXML_SIZE(s_quoteEntityString);

const XalanDOMChar  XalanXMLSerializerBase::UTF16::s_doctypeHeaderStartString[] =
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
    XalanDOMChar(0)
};

const XalanXMLSerializerBase::size_type     XalanXMLSerializerBase::UTF16::s_doctypeHeaderStartStringLength =
        FXML_SIZE(s_doctypeHeaderStartString);

const XalanDOMChar  XalanXMLSerializerBase::UTF16::s_doctypeHeaderPublicString[] =
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
    XalanDOMChar(0)
};

const XalanXMLSerializerBase::size_type     XalanXMLSerializerBase::UTF16::s_doctypeHeaderPublicStringLength =
        FXML_SIZE(s_doctypeHeaderPublicString);

const XalanDOMChar  XalanXMLSerializerBase::UTF16::s_doctypeHeaderSystemString[] =
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
    XalanDOMChar(0)
};

const XalanXMLSerializerBase::size_type     XalanXMLSerializerBase::UTF16::s_doctypeHeaderSystemStringLength =
        FXML_SIZE(s_doctypeHeaderSystemString);

const XalanDOMChar  XalanXMLSerializerBase::UTF16::s_xmlHeaderStartString[] =
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
    XalanDOMChar(0)
};

const XalanXMLSerializerBase::size_type     XalanXMLSerializerBase::UTF16::s_xmlHeaderStartStringLength =
        FXML_SIZE(s_xmlHeaderStartString);

const XalanDOMChar  XalanXMLSerializerBase::UTF16::s_xmlHeaderEncodingString[] =
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
    XalanDOMChar(0)
};

const XalanXMLSerializerBase::size_type     XalanXMLSerializerBase::UTF16::s_xmlHeaderEncodingStringLength =
        FXML_SIZE(s_xmlHeaderEncodingString);

const XalanDOMChar  XalanXMLSerializerBase::UTF16::s_xmlHeaderStandaloneString[] =
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
    XalanDOMChar(0)
};

const XalanXMLSerializerBase::size_type     XalanXMLSerializerBase::UTF16::s_xmlHeaderStandaloneStringLength =
        FXML_SIZE(s_xmlHeaderStandaloneString);

const XalanDOMChar  XalanXMLSerializerBase::UTF16::s_xmlHeaderEndString[] =
{
    XalanUnicode::charQuoteMark,
    XalanUnicode::charQuestionMark,
    XalanUnicode::charGreaterThanSign,
    XalanDOMChar(0)
};

const XalanXMLSerializerBase::size_type     XalanXMLSerializerBase::UTF16::s_xmlHeaderEndStringLength =
        FXML_SIZE(s_xmlHeaderEndString);

const XalanDOMChar  XalanXMLSerializerBase::UTF16::s_defaultVersionString[] =
{
    XalanUnicode::charDigit_1,
    XalanUnicode::charFullStop,
    XalanUnicode::charDigit_0,
    XalanDOMChar(0)
};

const XalanXMLSerializerBase::size_type     XalanXMLSerializerBase::UTF16::s_defaultVersionStringLength =
        FXML_SIZE(s_defaultVersionString);

const XalanDOMChar  XalanXMLSerializerBase::UTF16::s_cdataOpenString[] =
{
    XalanUnicode::charLessThanSign,
    XalanUnicode::charExclamationMark,
    XalanUnicode::charLeftSquareBracket,
    XalanUnicode::charLetter_C,
    XalanUnicode::charLetter_D,
    XalanUnicode::charLetter_A,
    XalanUnicode::charLetter_T,
    XalanUnicode::charLetter_A,
    XalanUnicode::charLeftSquareBracket,
    XalanDOMChar(0)
};

const XalanXMLSerializerBase::size_type     XalanXMLSerializerBase::UTF16::s_cdataOpenStringLength =
        FXML_SIZE(s_cdataOpenString);

const XalanDOMChar  XalanXMLSerializerBase::UTF16::s_cdataCloseString[] =
{
    XalanUnicode::charRightSquareBracket,
    XalanUnicode::charRightSquareBracket,
    XalanUnicode::charGreaterThanSign,
    XalanDOMChar(0)
};

const XalanXMLSerializerBase::size_type     XalanXMLSerializerBase::UTF16::s_cdataCloseStringLength =
        FXML_SIZE(s_cdataCloseString);


const XalanDOMChar  XalanXMLSerializerBase::UTF16::s_xhtmlDocTypeString[] =
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

const XalanXMLSerializerBase::size_type     XalanXMLSerializerBase::UTF16::s_xhtmlDocTypeStringLength =
        FXML_SIZE(s_xhtmlDocTypeString);

const XalanDOMChar  XalanXMLSerializerBase::UTF16::s_lessThanEntityString[] =
{
    XalanUnicode::charAmpersand,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_t,
    XalanUnicode::charSemicolon,
    XalanDOMChar(0)
};

const XalanXMLSerializerBase::size_type     XalanXMLSerializerBase::UTF16::s_lessThanEntityStringLength =
        FXML_SIZE(s_lessThanEntityString);

const XalanDOMChar  XalanXMLSerializerBase::UTF16::s_greaterThanEntityString[] =
{
    XalanUnicode::charAmpersand,
    XalanUnicode::charLetter_g,
    XalanUnicode::charLetter_t,
    XalanUnicode::charSemicolon,
    XalanDOMChar(0)
};

const XalanXMLSerializerBase::size_type     XalanXMLSerializerBase::UTF16::s_greaterThanEntityStringLength =
        FXML_SIZE(s_greaterThanEntityString);

const XalanDOMChar  XalanXMLSerializerBase::UTF16::s_ampersandEntityString[] =
{
    XalanUnicode::charAmpersand,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_m,
    XalanUnicode::charLetter_p,
    XalanUnicode::charSemicolon,
    XalanDOMChar(0)
};

const XalanXMLSerializerBase::size_type     XalanXMLSerializerBase::UTF16::s_ampersandEntityStringLength =
        FXML_SIZE(s_ampersandEntityString);

const XalanDOMChar  XalanXMLSerializerBase::UTF16::s_quoteEntityString[] =
{
    XalanUnicode::charAmpersand,
    XalanUnicode::charLetter_q,
    XalanUnicode::charLetter_u,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_t,
    XalanUnicode::charSemicolon,
    XalanDOMChar(0)
};

const XalanXMLSerializerBase::size_type     XalanXMLSerializerBase::UTF16::s_quoteEntityStringLength =
        FXML_SIZE(s_quoteEntityString);



XALAN_CPP_NAMESPACE_END
