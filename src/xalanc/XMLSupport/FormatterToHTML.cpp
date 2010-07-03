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
#include "FormatterToHTML.hpp"



#include <cassert>
#include <cstring>



#include <xercesc/sax/AttributeList.hpp>




#include <xalanc/Include/XalanMemMgrAutoPtr.hpp>

#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/PrefixResolver.hpp>
#include <xalanc/PlatformSupport/Writer.hpp>
#include <xalanc/PlatformSupport/XalanTranscodingServices.hpp>
#include <xalanc/PlatformSupport/XalanUnicode.hpp>
#include <xalanc/PlatformSupport/XalanXMLChar.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



XALAN_CPP_NAMESPACE_BEGIN



const XalanDOMString    FormatterToHTML::s_emptyString(XalanMemMgrs::getDummyMemMgr());



FormatterToHTML::FormatterToHTML(
            Writer&                 writer,
            const XalanDOMString&   encoding, 
            const XalanDOMString&   mediaType,
            const XalanDOMString&   doctypeSystem,
            const XalanDOMString&   doctypePublic,
            bool                    doIndent,
            int                     indent,
            bool                    escapeURLs,
            bool                    omitMetaTag,
            MemoryManager&          theManager) :
    FormatterToXML(
            writer,
            s_emptyString,
            doIndent,
            indent,
            encoding,
            mediaType,
            doctypeSystem,
            doctypePublic,
            false,
            s_emptyString,
            OUTPUT_METHOD_HTML,
            true,
            theManager),
    m_currentElementName(theManager),
    m_inBlockElem(false),
    m_isRawStack(theManager),
    m_isScriptOrStyleElem(false),
    m_inScriptElemStack(theManager),
    m_escapeURLs(escapeURLs),
    m_isFirstElement(false),
    m_elementLevel(0),
    m_hasNamespaceStack(theManager),
    m_omitMetaTag(omitMetaTag),
    m_elementPropertiesStack(theManager)
{
    initCharsMap();

    // FormatterToXML may have enabled this property, based on
    // the encoding, so we should force it off.
    m_shouldWriteXMLHeader = false;
}



FormatterToHTML*
FormatterToHTML::create(
            MemoryManager&          theManager,
            Writer&                 writer,
            const XalanDOMString&   encoding, 
            const XalanDOMString&   mediaType,
            const XalanDOMString&   doctypeSystem,
            const XalanDOMString&   doctypePublic,
            bool                    doIndent,
            int                     indent,
            bool                    escapeURLs,
            bool                    omitMetaTag) 
{
    typedef FormatterToHTML     ThisType;

    XalanAllocationGuard    theGuard(theManager, theManager.allocate(sizeof(ThisType)));

    ThisType* const     theResult =
        new (theGuard.get()) ThisType(
                                writer,
                                encoding, 
                                mediaType,
                                doctypeSystem,
                                doctypePublic,
                                doIndent,
                                indent,
                                escapeURLs,
                                omitMetaTag,
                                theManager);

    theGuard.release();

    return theResult;
}



FormatterToHTML::~FormatterToHTML()
{
}



void
FormatterToHTML::initAttrCharsMap()
{
    FormatterToXML::initAttrCharsMap();

    m_attrCharsMap[XalanUnicode::charLF] = 'S';

    // These should _not_ be escaped...
    m_attrCharsMap[XalanUnicode::charHTab] = 0;
    m_attrCharsMap[XalanUnicode::charLessThanSign] = 0;
    m_attrCharsMap[XalanUnicode::charGreaterThanSign] = 0;

    for(size_type i = 160; i < SPECIALSSIZE; i++)
    {
        m_attrCharsMap[i] = 'S';
    }
}



void
FormatterToHTML::initCharsMap()
{
    initAttrCharsMap();

#if defined(XALAN_STRICT_ANSI_HEADERS)
    using std::memset;
#endif

    memset(m_charsMap, 0, sizeof(m_charsMap));

    m_charsMap[XalanUnicode::charLF] = 'S';
    m_charsMap[XalanUnicode::charLessThanSign] = 'S';
    m_charsMap[XalanUnicode::charGreaterThanSign] = 'S';
    m_charsMap[XalanUnicode::charAmpersand] = 'S';

    memset(m_charsMap, 'S', 10);

    m_charsMap[0x0A] = 'S';
    m_charsMap[0x0D] = 'S';

    for(int i = 160; i < SPECIALSSIZE; ++i)
    {
        m_charsMap[i] = 'S';
    }

    for(int j = m_maxCharacter; j < SPECIALSSIZE; ++j)
    {
        m_charsMap[j] = 'S';
    }
}



void
FormatterToHTML::startDocument()
{
    // Clear the buffer, just in case...
    m_stringBuffer.clear();

    // Reset this, just in case...
    m_elementLevel = 0;

    m_isFirstElement = true;
    m_startNewLine = false;
    m_shouldWriteXMLHeader = false;
    m_isScriptOrStyleElem = false;

    m_isRawStack.clear();
    m_inScriptElemStack.push_back(false);
    m_hasNamespaceStack.clear();
    m_elementPropertiesStack.clear();

    const bool  isEmptySystem = m_doctypeSystem.empty();

    const bool  isEmptyPublic = m_doctypePublic.empty();

    // Output the header if either the System or Public attributes are
    // specified
    if(isEmptySystem == false || isEmptyPublic == false)
    {
        accumContent(s_doctypeHeaderStartString, 0, s_doctypeHeaderStartStringLength);

        if(isEmptyPublic == false)
        {
            accumContent(s_doctypeHeaderPublicString, 0, s_doctypeHeaderPublicStringLength);
            accumContent(m_doctypePublic);
            accumContent(XalanUnicode::charQuoteMark);
        }

        if(isEmptySystem == false)
        {
            if(isEmptyPublic == true)
            {
                accumContent(s_doctypeHeaderSystemString, 0, s_doctypeHeaderSystemStringLength);
            }

            accumContent(XalanUnicode::charSpace);
            accumContent(XalanUnicode::charQuoteMark);

            accumContent(m_doctypeSystem);
            accumContent(XalanUnicode::charQuoteMark);
        }

        accumContent(XalanUnicode::charGreaterThanSign);

        outputLineSep();
    }

    m_needToOutputDocTypeDecl = false;
}



void
FormatterToHTML::endDocument()
{
    assert(m_isRawStack.empty() == true);
    assert(m_inScriptElemStack.empty() == false);
    assert(m_hasNamespaceStack.empty() == true);
    assert(m_elementPropertiesStack.empty() == true);
    assert(m_elementLevel == 0);

    m_inScriptElemStack.pop_back();
    assert(m_inScriptElemStack.empty() == true);

    FormatterToXML::endDocument();
}



void
FormatterToHTML::startElement(
            const XMLCh* const  name,
            AttributeListType&  attrs)
{
    if (pushHasNamespace(name) == true)
    {
        FormatterToXML::startElement(name, attrs);
    }
    else
    {
        writeParentTagEnd();

        const XalanHTMLElementsProperties::ElementProperties&   elemProperties =
            XalanHTMLElementsProperties::find(name);
        assert(elemProperties.null() == false);
        assert(
            length(elemProperties.getName()) == 0 ||
            compareIgnoreCaseASCII(name, elemProperties.getName()) == 0);

        // Push a copy onto the stack for endElement().  Don't worry --
        // the copy is cheap!
        m_elementPropertiesStack.push_back(elemProperties);

        const bool  isBlockElement = elemProperties.is(XalanHTMLElementsProperties::BLOCK);

        if (elemProperties.is(XalanHTMLElementsProperties::SCRIPTELEM) == true)
        {
            m_isScriptOrStyleElem = true;

            m_inScriptElemStack.push_back(true);
        }
        else
        {
            if (elemProperties.is(XalanHTMLElementsProperties::STYLEELEM) == true)
            {
                m_isScriptOrStyleElem = true;
            }

            m_inScriptElemStack.push_back(m_inScriptElemStack.back());
        }

        // Increment the level...
        ++m_elementLevel;

        if(m_ispreserve == true)
        {
            m_ispreserve = false;
        }
        else if(m_doIndent &&
                m_elementLevel > 0 && m_isFirstElement == false &&
                (m_inBlockElem == false || isBlockElement == true))
        {
            m_startNewLine = true;

            indent(m_currentIndent);
        }

        m_inBlockElem = !isBlockElement;

        m_isRawStack.push_back(elemProperties.is(XalanHTMLElementsProperties::RAW));

        accumContent(XalanUnicode::charLessThanSign);

        accumName(name);

        const XalanSize_t   nAttrs = attrs.getLength();

        for (XalanSize_t i = 0;  i < nAttrs ;  i++)
        {
            processAttribute(attrs.getName(i), attrs.getValue(i), elemProperties);
        }

        // Flag the current element as not yet having any children.
        openElementForChildren();

        m_currentIndent += m_indent;
    
        m_isprevtext = false;

        if (elemProperties.is(XalanHTMLElementsProperties::HEADELEM) == true)
        {
            writeParentTagEnd();

            if (m_omitMetaTag == false)
            {
                if (m_doIndent)
                {
                    indent(m_currentIndent);
                }

                accumContent(s_metaString, 0, s_metaStringLength);
                accumContent(getEncoding());      
                accumContent(XalanUnicode::charQuoteMark);
                accumContent(XalanUnicode::charGreaterThanSign);
            }
        }

        // We've written the first element, so turn off the flag...
        if (m_isFirstElement == true)
        {
            m_isFirstElement = false;
        }

        assert(m_elementLevel > 0);
    }
}



void
FormatterToHTML::endElement(const XMLCh* const  name)
{
    if (popHasNamespace() == true)
    {
        FormatterToXML::endElement(name);
    }
    else
    {
        m_currentIndent -= m_indent;

        const bool  hasChildNodes = childNodesWereAdded();

        assert(m_isRawStack.empty() == false);
        assert(m_inScriptElemStack.empty() == false);
        assert(m_elementPropertiesStack.empty() == false);

        m_isRawStack.pop_back();
        m_inScriptElemStack.pop_back();

        const XalanHTMLElementsProperties::ElementProperties    elemProperties =
                m_elementPropertiesStack.back();
        assert(elemProperties.null() == false);
        assert(
            length(elemProperties.getName()) == 0 ||
            compareIgnoreCaseASCII(name, elemProperties.getName()) == 0);

        m_elementPropertiesStack.pop_back();

        const bool  isBlockElement = elemProperties.is(XalanHTMLElementsProperties::BLOCK);

        bool shouldIndent = false;

        if(m_ispreserve == true)
        {
            m_ispreserve = false;
        }
        else if(m_doIndent == true && (m_inBlockElem == false || isBlockElement == true))
        {
            m_startNewLine = true;

            shouldIndent = true;
        }

        m_inBlockElem = !isBlockElement;

        if (hasChildNodes) 
        {
            if (shouldIndent == true)
            {
                indent(m_currentIndent);
            }

            if(elemProperties.is(XalanHTMLElementsProperties::EMPTY) == false)
            {
                accumContent(XalanUnicode::charLessThanSign);
                accumContent(XalanUnicode::charSolidus);
                accumName(name);
                accumContent(XalanUnicode::charGreaterThanSign);
            }
        }
        else
        {
            if(elemProperties.is(XalanHTMLElementsProperties::EMPTY) == false)
            {
                accumContent(XalanUnicode::charGreaterThanSign);

                accumContent(XalanUnicode::charLessThanSign);
                accumContent(XalanUnicode::charSolidus);
                accumName(name);
                accumContent(XalanUnicode::charGreaterThanSign);
            }
            else
            {
                accumContent(XalanUnicode::charGreaterThanSign);
            }
        }

        if (elemProperties.is(XalanHTMLElementsProperties::WHITESPACESENSITIVE) == true)
        {
            m_ispreserve = true;
        }

        if (hasChildNodes == true)
        {
            if (m_preserves.empty() == false)
            {
                m_preserves.pop_back();
            }
        }

        m_isprevtext = false;

        // Decrement the level...
        --m_elementLevel;
    }
}



void
FormatterToHTML::characters(
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
        else if (m_inScriptElemStack.back() == true)
        {
            charactersRaw(chars, length);
        }
        else if (m_isRawStack.empty() == false &&
                 m_isRawStack.back() == true)
        {
            writeParentTagEnd();

            m_ispreserve = true;

            if (shouldIndent() == true)
            {
                indent(m_currentIndent);
            }

            writeNormalizedChars(chars, 0, length, false);
        }
        else
        {
            writeParentTagEnd();

            m_ispreserve = true;

            writeCharacters(chars, length);
        }
    }

    if (m_isprevtext == false)
    {
        m_isprevtext = true;
    }
}



bool
FormatterToHTML::accumDefaultEntity(
        XalanDOMChar                ch,
        bool                        escLF)
{
    assert(ch != 0);

    if(FormatterToXML::accumDefaultEntity(ch, escLF) == true)
    {
        return true;
    }
    else
    {
        // Find the entity, if any...
        const Entity*   theFirst = s_entities;
        const Entity*   theLast = s_lastEntity;

        while(theFirst <= theLast)
        {
            const Entity* const     theCurrent = theFirst + (theLast - theFirst) / 2;
            assert(theCurrent->m_char != 0);

            if (ch < theCurrent->m_char)
            {
                theLast = theCurrent - 1;
            }
            else if (ch > theCurrent->m_char)
            {
                theFirst = theCurrent + 1;
            }
            else
            {
                assert(length(theCurrent->m_string) == theCurrent->m_length);

                copyEntityIntoBuffer(theCurrent->m_string, theCurrent->m_length);

                return true;
            }
        }

        return false;
    }
}



void
FormatterToHTML::entityReference(const XMLCh* const     name)
{
    accumContent(XalanUnicode::charAmpersand);
    accumName(name);
    accumContent(XalanUnicode::charSemicolon);
}



void
FormatterToHTML::cdata(
            const XMLCh* const  ch,
            const size_type     length)
{
    if(m_isScriptOrStyleElem == true)
    {
        writeParentTagEnd();

        m_ispreserve = true;

        if (shouldIndent() == true)
        {
            indent(m_currentIndent);
        }

        writeNormalizedChars(ch, 0, length, true);
    }
    else if(m_stripCData == true)
    {
        writeParentTagEnd();

        m_ispreserve = true;

        if (shouldIndent() == true)
        {
            indent(m_currentIndent);
        }

        accumContent(ch, 0, length);
    }
    else
    {
        FormatterToXML::cdata(ch, length);
    }
}



void
FormatterToHTML::processingInstruction(
        const XMLCh* const  target,
        const XMLCh* const  data)

{
    const size_type     dataLength = length(data);

    // Use a fairly nasty hack to tell if the next node is supposed to be 
    // unescaped text.
    if(equals(target, length(target), s_piTarget, s_piTargetLength) == true &&
       equals(data, dataLength, s_piData, s_piDataLength) == true)
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

        accumContent(XalanUnicode::charLessThanSign);
        accumContent(XalanUnicode::charQuestionMark);
        accumName(target);

        if (length(data) > 0)
        {
            if(isXMLWhitespace(data[0]) == false)
            {
                accumContent(XalanUnicode::charSpace);
            }

            writeCharacters(data, dataLength);
        }

        accumContent(XalanUnicode::charGreaterThanSign); // different from XML

        // If outside of an element, then put in a new line.  This whitespace
        // is not significant.
        if (m_elementLevel == 0)
        {
            outputLineSep();
        }

        m_startNewLine = true;
    }
}



void
FormatterToHTML::writeCharacters(const XalanDOMString&  theString)
{
    writeCharacters(theString.c_str(), theString.length());
}



void
FormatterToHTML::writeCharacters(
            const XalanDOMChar*     theString,
            size_type               theLength)
{
    assert(theString != 0);

    size_type   i = 0;
    size_type   firstIndex = 0;

    while(i < theLength)
    {
        const XalanDOMChar  ch = theString[i];

        if(ch < SPECIALSSIZE && m_charsMap[ch] != 'S')
        {
            ++i;
        }
        else if (XalanUnicode::charLF == ch) // sta this can be removed?
        {
            accumContent(theString, firstIndex, i - firstIndex);

            outputLineSep();

            ++i;

            firstIndex = i;
        }   
        else
        {
            accumContent(theString, firstIndex, i - firstIndex);

            if (accumDefaultEntity(ch, true) == false)
            {
                if (0xd800 <= ch && ch < 0xdc00)
                {
                    // UTF-16 surrogate
                    if (i + 1 >= theLength) 
                    {
                        throwInvalidUTF16SurrogateException(ch, getMemoryManager());
                    }

                    XalanUnicodeChar    next = theString[++i];

                    if (!(0xdc00 <= next && next < 0xe000))
                    {
                        throwInvalidUTF16SurrogateException(ch, static_cast<XalanDOMChar>(next),
                                                            getMemoryManager());
                    }

                    next = ((ch - 0xd800) << 10) + next - 0xdc00 + 0x00010000;

                    writeNumberedEntityReference(next);
                }
                else if(ch >= 0x007Fu && ch <= m_maxCharacter)
                {
                    // Hope this is right...
                    accumContent(ch);
                }
                else
                {
                    writeNumberedEntityReference(ch);
                }
            }

            ++i;

            firstIndex = i;
        }
    }

    accumContent(theString, firstIndex, theLength - firstIndex);
}



void
FormatterToHTML::writeAttrString(
            const XalanDOMChar*     theString,
            size_type               theStringLength)
{
    assert(theString != 0);

    size_type   i = 0;
    size_type   firstIndex = 0;

    while(i < theStringLength)
    {
        const XalanDOMChar  ch = theString[i];

        if(ch < SPECIALSSIZE && m_attrCharsMap[ch] != 'S')
        {
            ++i;
        }
        else if(XalanUnicode::charAmpersand == ch &&
                i + 1 < theStringLength &&
                XalanUnicode::charLeftCurlyBracket == theString[i + 1])
        {
            ++i;
        }
        else
        {
            accumContent(theString, firstIndex, i - firstIndex);

            if (accumDefaultEntity(ch, true) == false)
            {
                if (0xd800 <= ch && ch < 0xdc00) 
                {
                    // UTF-16 surrogate

                    XalanDOMChar    next = 0;

                    if (i + 1 >= theStringLength) 
                    {
                        throwInvalidUTF16SurrogateException(ch, getMemoryManager());
                    }
                    else 
                    {
                        next = theString[++i];

                        if (!(0xdc00 <= next && next < 0xe000))
                        {
                            throwInvalidUTF16SurrogateException(ch, next, getMemoryManager());
                        }

                        next = XalanDOMChar(((ch - 0xd800) << 10) + next -0xdc00 + 0x00010000);
                    }

                    accumContent(XalanUnicode::charAmpersand);
                    accumContent(XalanUnicode::charNumberSign);

                    accumContent(NumberToDOMString(next, m_stringBuffer));
                    m_stringBuffer.clear();

                    accumContent(XalanUnicode::charSemicolon);
                }
                else
                {
                    writeNumberedEntityReference(ch);
                }
            }

            ++i;

            firstIndex = i;
        }
    }

    accumContent(theString, firstIndex, theStringLength - firstIndex);
}



void
FormatterToHTML::accumCommentData(const XalanDOMChar*   data)
{
    accumName(data);
}



void
FormatterToHTML::copyEntityIntoBuffer(
            const XalanDOMChar*     s,
            size_type               theLength)
{
    assert(s != 0);

    accumName(XalanUnicode::charAmpersand);

    accumName(s, 0, theLength);

    accumName(XalanUnicode::charSemicolon);
}



void
FormatterToHTML::copyEntityIntoBuffer(const XalanDOMString&     s)
{
    accumName(XalanUnicode::charAmpersand);

    accumName(s);

    accumName(XalanUnicode::charSemicolon);
}



void
FormatterToHTML::processAttribute(
            const XalanDOMChar*                                     name,
            const XalanDOMChar*                                     value,
            const XalanHTMLElementsProperties::ElementProperties&   elemProperties)
{
    const size_type     nameLength = length(name);

    accumContent(XalanUnicode::charSpace);

    const size_type     valueLength = length(value);

    if((valueLength == 0 || equalsIgnoreCaseASCII(name, nameLength, value, valueLength)) &&
       elemProperties.isAttribute(name, XalanHTMLElementsProperties::ATTREMPTY) == true)
    {
        accumName(name);
    }
    else
    {
        accumName(name, 0, nameLength);
        accumContent(XalanUnicode::charEqualsSign);
        accumContent(XalanUnicode::charQuoteMark);

        if(elemProperties.isAttribute(name, XalanHTMLElementsProperties::ATTRURL) == true)
        {
            writeAttrURI(value, valueLength);
        }
        else
        {
            writeAttrString(value, valueLength);
        }

        accumContent(XalanUnicode::charQuoteMark);
    }
}



void
FormatterToHTML::writeAttrURI(
            const XalanDOMChar*     theString,
            size_type               theStringLength)
{
    assert(theString != 0);

    // http://www.ietf.org/rfc/rfc2396.txt says:
    // A URI is always in an "escaped" form, since escaping or unescaping a
    // completed URI might change its semantics.  Normally, the only time
    // escape encodings can safely be made is when the URI is being created
    // from its component parts; each component may have its own set of
    // characters that are reserved, so only the mechanism responsible for
    // generating or interpreting that component can determine whether or
    // not escaping a character will change its semantics. Likewise, a URI
    // must be separated into its components before the escaped characters
    // within those components can be safely decoded.
    //
    // ...So we do our best to do limited escaping of the URL, without 
    // causing damage.  If the URL is already properly escaped, in theory, this 
    // function should not change the string value.

    for (size_type i = 0; i < theStringLength; ++i)
    {
        const XalanDOMChar  ch = theString[i];

        if (ch < 33 || ch > 126)
        {
            if (m_escapeURLs == true)
            {
                // For the gory details of encoding these characters as
                // UTF-8 hex, see:
                // 
                // Unicode, A Primer, by Tony Graham, p. 92.
                //
                if (ch == XalanUnicode::charSpace)
                {
                    accumContent(ch);
                }
                else if(ch <= 0x7F)
                {
                    accumHexNumber(ch);
                }
                else if(ch <= 0x7FF)
                {
                    const XalanDOMChar  highByte = XalanDOMChar((ch >> 6) | 0xC0);
                    const XalanDOMChar  lowByte = XalanDOMChar((ch & 0x3F) | 0x80);

                    accumHexNumber(highByte);

                    accumHexNumber(lowByte);
                }
                else if(isUTF16Surrogate(ch) == true) // high surrogate
                {
                    // I'm sure this can be done in 3 instructions, but I choose 
                    // to try and do it exactly like it is done in the book, at least 
                    // until we are sure this is totally clean.  I don't think performance 
                    // is a big issue with this particular function, though I could be 
                    // wrong.  Also, the stuff below clearly does more masking than 
                    // it needs to do.
            
                    // Clear high 6 bits.
                    const XalanDOMChar  highSurrogate = XalanDOMChar(ch & 0x03FF);

                    // Middle 4 bits (wwww) + 1
                    // "Note that the value of wwww from the high surrogate bit pattern
                    // is incremented to make the uuuuu bit pattern in the scalar value 
                    // so the surrogate pair don't address the BMP."
                    const XalanDOMChar  wwww = XalanDOMChar((highSurrogate & 0x03C0) >> 6);
                    const XalanDOMChar  uuuuu = XalanDOMChar(wwww + 1);  

                    // next 4 bits
                    const XalanDOMChar  zzzz = XalanDOMChar((highSurrogate & 0x003C) >> 2);
            
                    // low 2 bits
                    const XalanDOMChar  temp = XalanDOMChar(((highSurrogate & 0x0003) << 4) & 0x30);
            
                    // Get low surrogate character.
                    const XalanDOMChar  nextChar = theString[++i];
            
                    // Clear high 6 bits.
                    const XalanDOMChar  lowSurrogate = XalanDOMChar(nextChar & 0x03FF);
            
                    // put the middle 4 bits into the bottom of yyyyyy (byte 3)
                    const XalanDOMChar  yyyyyy = XalanDOMChar(temp | ((lowSurrogate & 0x03C0) >> 6));
            
                    // bottom 6 bits.
                    const XalanDOMChar  xxxxxx = XalanDOMChar(lowSurrogate & 0x003F);
            
                    const XalanDOMChar  byte1 = XalanDOMChar(0xF0 | (uuuuu >> 2)); // top 3 bits of uuuuu
                    const XalanDOMChar  byte2 = XalanDOMChar(0x80 | (((uuuuu & 0x03) << 4) & 0x30) | zzzz);
                    const XalanDOMChar  byte3 = XalanDOMChar(0x80 | yyyyyy);
                    const XalanDOMChar  byte4 = XalanDOMChar(0x80 | xxxxxx);
            
                    accumHexNumber(byte1);

                    accumHexNumber(byte2);

                    accumHexNumber(byte3);

                    accumHexNumber(byte4);
                }
                else
                {
                    const XalanDOMChar  highByte = XalanDOMChar((ch >> 12) | 0xE0);
                    const XalanDOMChar  middleByte = XalanDOMChar(((ch & 0x0FC0) >> 6) | 0x80);
                    const XalanDOMChar  lowByte = XalanDOMChar((ch & 0x3F) | 0x80);

                    accumHexNumber(highByte);

                    accumHexNumber(middleByte);

                    accumHexNumber(lowByte);
                }
            }
            else if (ch < m_maxCharacter)
            {
                accumContent(ch);
            }
            else
            {
                accumContent(XalanUnicode::charAmpersand);
                accumContent(XalanUnicode::charNumberSign);
    
                accumContent(NumberToDOMString(ch, m_stringBuffer));
                m_stringBuffer.clear();

                accumContent(XalanUnicode::charSemicolon);
            }
        }
        // Since http://www.ietf.org/rfc/rfc2396.txt refers to the URI grammar as
        // not allowing quotes in the URI proper syntax, nor in the fragment 
        // identifier, we believe that double quotes should be escaped.
        else if (ch == XalanUnicode::charQuoteMark)
        {
            if (m_escapeURLs == true)
            {
                accumContent(XalanUnicode::charPercentSign);
                accumContent(XalanUnicode::charDigit_2);
                accumContent(XalanUnicode::charDigit_2);
            }
            else
            {
                accumDefaultEntity(ch, true);
            }
        }
        else if (ch == XalanUnicode::charAmpersand)
        {
            accumDefaultEntity(ch, true);
        }
        else
        {
            accumContent(ch);
        }
    }
}



void
FormatterToHTML::accumHexNumber(XalanDOMChar    theChar)
{
    accumContent(XalanUnicode::charPercentSign);

    assert(m_stringBuffer.empty() == true);

    NumberToHexDOMString(theChar, m_stringBuffer);

    if (m_stringBuffer.length() == 1)
    {
        accumContent(XalanUnicode::charDigit_0);
    }

    accumContent(m_stringBuffer);

    m_stringBuffer.clear();
}



bool
FormatterToHTML::doPopHasNamespace()
{
    assert(m_prefixResolver != 0);

    assert(m_hasNamespaceStack.empty() == false);

    const bool  theValue = m_hasNamespaceStack.back();

    m_hasNamespaceStack.pop_back();
            
    return theValue;
}



bool
FormatterToHTML::doPushHasNamespace(const XalanDOMChar*     theElementName)
{
    assert(m_prefixResolver != 0);

    bool    fHasNamespace = false;

    const size_type     theLength =
        length(theElementName);

    const size_type     theColonIndex =
        indexOf(
            theElementName,
            XalanUnicode::charColon);

    const XalanDOMString*   thePrefix = &s_emptyString;

    if (theColonIndex < theLength)
    {
        substring(
            theElementName,
            m_stringBuffer,
            0,
            theColonIndex);

        thePrefix = &m_stringBuffer;
    }

    assert(thePrefix != 0);

    // Check for the namespace...
    const XalanDOMString* const     theNamespace =
        m_prefixResolver->getNamespaceForPrefix(*thePrefix);

    if (theNamespace != 0 && theNamespace->length() != 0)
    {
        fHasNamespace = true;
    }

    m_stringBuffer.clear();

    m_hasNamespaceStack.push_back(fHasNamespace);

    return fHasNamespace;
}



// Some of these are now commented out to match Xalan-J, which claims that Netscape cannot handle many of these entities.
const FormatterToHTML::Entity   FormatterToHTML::s_entities[] =
{
    // These must always be in order by the character.
    // Otherwise, the binary search for them will fail.
    { 160, 4, { XalanUnicode::charLetter_n, XalanUnicode::charLetter_b, XalanUnicode::charLetter_s, XalanUnicode::charLetter_p, 0 } },
    { 161, 5, { XalanUnicode::charLetter_i, XalanUnicode::charLetter_e, XalanUnicode::charLetter_x, XalanUnicode::charLetter_c, XalanUnicode::charLetter_l, 0 } },
    { 162, 4, { XalanUnicode::charLetter_c, XalanUnicode::charLetter_e, XalanUnicode::charLetter_n, XalanUnicode::charLetter_t, 0 } },
    { 163, 5, { XalanUnicode::charLetter_p, XalanUnicode::charLetter_o, XalanUnicode::charLetter_u, XalanUnicode::charLetter_n, XalanUnicode::charLetter_d, 0 } },
    { 164, 6, { XalanUnicode::charLetter_c, XalanUnicode::charLetter_u, XalanUnicode::charLetter_r, XalanUnicode::charLetter_r, XalanUnicode::charLetter_e, XalanUnicode::charLetter_n, 0 } },
    { 165, 3, { XalanUnicode::charLetter_y, XalanUnicode::charLetter_e, XalanUnicode::charLetter_n, 0 } },
    { 166, 6, { XalanUnicode::charLetter_b, XalanUnicode::charLetter_r, XalanUnicode::charLetter_v, XalanUnicode::charLetter_b, XalanUnicode::charLetter_a, XalanUnicode::charLetter_r, 0 } },
    { 167, 4, { XalanUnicode::charLetter_s, XalanUnicode::charLetter_e, XalanUnicode::charLetter_c, XalanUnicode::charLetter_t, 0 } },
    { 168, 3, { XalanUnicode::charLetter_u, XalanUnicode::charLetter_m, XalanUnicode::charLetter_l, 0 } },
    { 169, 4, { XalanUnicode::charLetter_c, XalanUnicode::charLetter_o, XalanUnicode::charLetter_p, XalanUnicode::charLetter_y, 0 } },
    { 170, 4, { XalanUnicode::charLetter_o, XalanUnicode::charLetter_r, XalanUnicode::charLetter_d, XalanUnicode::charLetter_f, 0 } },
    { 171, 5, { XalanUnicode::charLetter_l, XalanUnicode::charLetter_a, XalanUnicode::charLetter_q, XalanUnicode::charLetter_u, XalanUnicode::charLetter_o, 0 } },
    { 172, 3, { XalanUnicode::charLetter_n, XalanUnicode::charLetter_o, XalanUnicode::charLetter_t, 0 } },
    { 173, 3, { XalanUnicode::charLetter_s, XalanUnicode::charLetter_h, XalanUnicode::charLetter_y, 0 } },
    { 174, 3, { XalanUnicode::charLetter_r, XalanUnicode::charLetter_e, XalanUnicode::charLetter_g, 0 } },
    { 175, 4, { XalanUnicode::charLetter_m, XalanUnicode::charLetter_a, XalanUnicode::charLetter_c, XalanUnicode::charLetter_r, 0 } },
    { 176, 3, { XalanUnicode::charLetter_d, XalanUnicode::charLetter_e, XalanUnicode::charLetter_g, 0 } },
    { 177, 6, { XalanUnicode::charLetter_p, XalanUnicode::charLetter_l, XalanUnicode::charLetter_u, XalanUnicode::charLetter_s, XalanUnicode::charLetter_m, XalanUnicode::charLetter_n, 0 } },
    { 178, 4, { XalanUnicode::charLetter_s, XalanUnicode::charLetter_u, XalanUnicode::charLetter_p, XalanUnicode::charDigit_2, 0 } },
    { 179, 4, { XalanUnicode::charLetter_s, XalanUnicode::charLetter_u, XalanUnicode::charLetter_p, XalanUnicode::charDigit_3, 0 } },
    { 180, 5, { XalanUnicode::charLetter_a, XalanUnicode::charLetter_c, XalanUnicode::charLetter_u, XalanUnicode::charLetter_t, XalanUnicode::charLetter_e, 0 } },
    { 181, 5, { XalanUnicode::charLetter_m, XalanUnicode::charLetter_i, XalanUnicode::charLetter_c, XalanUnicode::charLetter_r, XalanUnicode::charLetter_o, 0 } },
    { 182, 4, { XalanUnicode::charLetter_p, XalanUnicode::charLetter_a, XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, 0 } },
    { 183, 6, { XalanUnicode::charLetter_m, XalanUnicode::charLetter_i, XalanUnicode::charLetter_d, XalanUnicode::charLetter_d, XalanUnicode::charLetter_o, XalanUnicode::charLetter_t, 0 } },
    { 184, 5, { XalanUnicode::charLetter_c, XalanUnicode::charLetter_e, XalanUnicode::charLetter_d, XalanUnicode::charLetter_i, XalanUnicode::charLetter_l, 0 } },
    { 185, 4, { XalanUnicode::charLetter_s, XalanUnicode::charLetter_u, XalanUnicode::charLetter_p, XalanUnicode::charDigit_1, 0 } },
    { 186, 4, { XalanUnicode::charLetter_o, XalanUnicode::charLetter_r, XalanUnicode::charLetter_d, XalanUnicode::charLetter_m, 0 } },
    { 187, 5, { XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_q, XalanUnicode::charLetter_u, XalanUnicode::charLetter_o, 0 } },
    { 188, 6, { XalanUnicode::charLetter_f, XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_c, XalanUnicode::charDigit_1, XalanUnicode::charDigit_4, 0 } },
    { 189, 6, { XalanUnicode::charLetter_f, XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_c, XalanUnicode::charDigit_1, XalanUnicode::charDigit_2, 0 } },
    { 190, 6, { XalanUnicode::charLetter_f, XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_c, XalanUnicode::charDigit_3, XalanUnicode::charDigit_4, 0 } },
    { 191, 6, { XalanUnicode::charLetter_i, XalanUnicode::charLetter_q, XalanUnicode::charLetter_u, XalanUnicode::charLetter_e, XalanUnicode::charLetter_s, XalanUnicode::charLetter_t, 0 } },
    { 192, 6, { XalanUnicode::charLetter_A, XalanUnicode::charLetter_g, XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_v, XalanUnicode::charLetter_e, 0 } },
    { 193, 6, { XalanUnicode::charLetter_A, XalanUnicode::charLetter_a, XalanUnicode::charLetter_c, XalanUnicode::charLetter_u, XalanUnicode::charLetter_t, XalanUnicode::charLetter_e, 0 } },
    { 194, 5, { XalanUnicode::charLetter_A, XalanUnicode::charLetter_c, XalanUnicode::charLetter_i, XalanUnicode::charLetter_r, XalanUnicode::charLetter_c, 0 } },
    { 195, 6, { XalanUnicode::charLetter_A, XalanUnicode::charLetter_t, XalanUnicode::charLetter_i, XalanUnicode::charLetter_l, XalanUnicode::charLetter_d, XalanUnicode::charLetter_e, 0 } },
    { 196, 4, { XalanUnicode::charLetter_A, XalanUnicode::charLetter_u, XalanUnicode::charLetter_m, XalanUnicode::charLetter_l, 0 } },
    { 197, 5, { XalanUnicode::charLetter_A, XalanUnicode::charLetter_r, XalanUnicode::charLetter_i, XalanUnicode::charLetter_n, XalanUnicode::charLetter_g, 0 } },
    { 198, 5, { XalanUnicode::charLetter_A, XalanUnicode::charLetter_E, XalanUnicode::charLetter_l, XalanUnicode::charLetter_i, XalanUnicode::charLetter_g, 0 } },
    { 199, 6, { XalanUnicode::charLetter_C, XalanUnicode::charLetter_c, XalanUnicode::charLetter_e, XalanUnicode::charLetter_d, XalanUnicode::charLetter_i, XalanUnicode::charLetter_l, 0 } },
    { 200, 6, { XalanUnicode::charLetter_E, XalanUnicode::charLetter_g, XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_v, XalanUnicode::charLetter_e, 0 } },
    { 201, 6, { XalanUnicode::charLetter_E, XalanUnicode::charLetter_a, XalanUnicode::charLetter_c, XalanUnicode::charLetter_u, XalanUnicode::charLetter_t, XalanUnicode::charLetter_e, 0 } },
    { 202, 5, { XalanUnicode::charLetter_E, XalanUnicode::charLetter_c, XalanUnicode::charLetter_i, XalanUnicode::charLetter_r, XalanUnicode::charLetter_c, 0 } },
    { 203, 4, { XalanUnicode::charLetter_E, XalanUnicode::charLetter_u, XalanUnicode::charLetter_m, XalanUnicode::charLetter_l, 0 } },
    { 204, 6, { XalanUnicode::charLetter_I, XalanUnicode::charLetter_g, XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_v, XalanUnicode::charLetter_e, 0 } },
    { 205, 6, { XalanUnicode::charLetter_I, XalanUnicode::charLetter_a, XalanUnicode::charLetter_c, XalanUnicode::charLetter_u, XalanUnicode::charLetter_t, XalanUnicode::charLetter_e, 0 } },
    { 206, 5, { XalanUnicode::charLetter_I, XalanUnicode::charLetter_c, XalanUnicode::charLetter_i, XalanUnicode::charLetter_r, XalanUnicode::charLetter_c, 0 } },
    { 207, 4, { XalanUnicode::charLetter_I, XalanUnicode::charLetter_u, XalanUnicode::charLetter_m, XalanUnicode::charLetter_l, 0 } },
    { 208, 3, { XalanUnicode::charLetter_E, XalanUnicode::charLetter_T, XalanUnicode::charLetter_H, 0 } },
    { 209, 6, { XalanUnicode::charLetter_N, XalanUnicode::charLetter_t, XalanUnicode::charLetter_i, XalanUnicode::charLetter_l, XalanUnicode::charLetter_d, XalanUnicode::charLetter_e, 0 } },
    { 210, 6, { XalanUnicode::charLetter_O, XalanUnicode::charLetter_g, XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_v, XalanUnicode::charLetter_e, 0 } },
    { 211, 6, { XalanUnicode::charLetter_O, XalanUnicode::charLetter_a, XalanUnicode::charLetter_c, XalanUnicode::charLetter_u, XalanUnicode::charLetter_t, XalanUnicode::charLetter_e, 0 } },
    { 212, 5, { XalanUnicode::charLetter_O, XalanUnicode::charLetter_c, XalanUnicode::charLetter_i, XalanUnicode::charLetter_r, XalanUnicode::charLetter_c, 0 } },
    { 213, 6, { XalanUnicode::charLetter_O, XalanUnicode::charLetter_t, XalanUnicode::charLetter_i, XalanUnicode::charLetter_l, XalanUnicode::charLetter_d, XalanUnicode::charLetter_e, 0 } },
    { 214, 4, { XalanUnicode::charLetter_O, XalanUnicode::charLetter_u, XalanUnicode::charLetter_m, XalanUnicode::charLetter_l, 0 } },
    { 215, 5, { XalanUnicode::charLetter_t, XalanUnicode::charLetter_i, XalanUnicode::charLetter_m, XalanUnicode::charLetter_e, XalanUnicode::charLetter_s, 0 } },
    { 216, 6, { XalanUnicode::charLetter_O, XalanUnicode::charLetter_s, XalanUnicode::charLetter_l, XalanUnicode::charLetter_a, XalanUnicode::charLetter_s, XalanUnicode::charLetter_h, 0 } },
    { 217, 6, { XalanUnicode::charLetter_U, XalanUnicode::charLetter_g, XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_v, XalanUnicode::charLetter_e, 0 } },
    { 218, 6, { XalanUnicode::charLetter_U, XalanUnicode::charLetter_a, XalanUnicode::charLetter_c, XalanUnicode::charLetter_u, XalanUnicode::charLetter_t, XalanUnicode::charLetter_e, 0 } },
    { 219, 5, { XalanUnicode::charLetter_U, XalanUnicode::charLetter_c, XalanUnicode::charLetter_i, XalanUnicode::charLetter_r, XalanUnicode::charLetter_c, 0 } },
    { 220, 4, { XalanUnicode::charLetter_U, XalanUnicode::charLetter_u, XalanUnicode::charLetter_m, XalanUnicode::charLetter_l, 0 } },
    { 221, 6, { XalanUnicode::charLetter_Y, XalanUnicode::charLetter_a, XalanUnicode::charLetter_c, XalanUnicode::charLetter_u, XalanUnicode::charLetter_t, XalanUnicode::charLetter_e, 0 } },
    { 222, 5, { XalanUnicode::charLetter_T, XalanUnicode::charLetter_H, XalanUnicode::charLetter_O, XalanUnicode::charLetter_R, XalanUnicode::charLetter_N, 0 } },
    { 223, 5, { XalanUnicode::charLetter_s, XalanUnicode::charLetter_z, XalanUnicode::charLetter_l, XalanUnicode::charLetter_i, XalanUnicode::charLetter_g, 0 } },
    { 224, 6, { XalanUnicode::charLetter_a, XalanUnicode::charLetter_g, XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_v, XalanUnicode::charLetter_e, 0 } },
    { 225, 6, { XalanUnicode::charLetter_a, XalanUnicode::charLetter_a, XalanUnicode::charLetter_c, XalanUnicode::charLetter_u, XalanUnicode::charLetter_t, XalanUnicode::charLetter_e, 0 } },
    { 226, 5, { XalanUnicode::charLetter_a, XalanUnicode::charLetter_c, XalanUnicode::charLetter_i, XalanUnicode::charLetter_r, XalanUnicode::charLetter_c, 0 } },
    { 227, 6, { XalanUnicode::charLetter_a, XalanUnicode::charLetter_t, XalanUnicode::charLetter_i, XalanUnicode::charLetter_l, XalanUnicode::charLetter_d, XalanUnicode::charLetter_e, 0 } },
    { 228, 4, { XalanUnicode::charLetter_a, XalanUnicode::charLetter_u, XalanUnicode::charLetter_m, XalanUnicode::charLetter_l, 0 } },
    { 229, 5, { XalanUnicode::charLetter_a, XalanUnicode::charLetter_r, XalanUnicode::charLetter_i, XalanUnicode::charLetter_n, XalanUnicode::charLetter_g, 0 } },
    { 230, 5, { XalanUnicode::charLetter_a, XalanUnicode::charLetter_e, XalanUnicode::charLetter_l, XalanUnicode::charLetter_i, XalanUnicode::charLetter_g, 0 } },
    { 231, 6, { XalanUnicode::charLetter_c, XalanUnicode::charLetter_c, XalanUnicode::charLetter_e, XalanUnicode::charLetter_d, XalanUnicode::charLetter_i, XalanUnicode::charLetter_l, 0 } },
    { 232, 6, { XalanUnicode::charLetter_e, XalanUnicode::charLetter_g, XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_v, XalanUnicode::charLetter_e, 0 } },
    { 233, 6, { XalanUnicode::charLetter_e, XalanUnicode::charLetter_a, XalanUnicode::charLetter_c, XalanUnicode::charLetter_u, XalanUnicode::charLetter_t, XalanUnicode::charLetter_e, 0 } },
    { 234, 5, { XalanUnicode::charLetter_e, XalanUnicode::charLetter_c, XalanUnicode::charLetter_i, XalanUnicode::charLetter_r, XalanUnicode::charLetter_c, 0 } },
    { 235, 4, { XalanUnicode::charLetter_e, XalanUnicode::charLetter_u, XalanUnicode::charLetter_m, XalanUnicode::charLetter_l, 0 } },
    { 236, 6, { XalanUnicode::charLetter_i, XalanUnicode::charLetter_g, XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_v, XalanUnicode::charLetter_e, 0 } },
    { 237, 6, { XalanUnicode::charLetter_i, XalanUnicode::charLetter_a, XalanUnicode::charLetter_c, XalanUnicode::charLetter_u, XalanUnicode::charLetter_t, XalanUnicode::charLetter_e, 0 } },
    { 238, 5, { XalanUnicode::charLetter_i, XalanUnicode::charLetter_c, XalanUnicode::charLetter_i, XalanUnicode::charLetter_r, XalanUnicode::charLetter_c, 0 } },
    { 239, 4, { XalanUnicode::charLetter_i, XalanUnicode::charLetter_u, XalanUnicode::charLetter_m, XalanUnicode::charLetter_l, 0 } },
    { 240, 3, { XalanUnicode::charLetter_e, XalanUnicode::charLetter_t, XalanUnicode::charLetter_h, 0 } },
    { 241, 6, { XalanUnicode::charLetter_n, XalanUnicode::charLetter_t, XalanUnicode::charLetter_i, XalanUnicode::charLetter_l, XalanUnicode::charLetter_d, XalanUnicode::charLetter_e, 0 } },
    { 242, 6, { XalanUnicode::charLetter_o, XalanUnicode::charLetter_g, XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_v, XalanUnicode::charLetter_e, 0 } },
    { 243, 6, { XalanUnicode::charLetter_o, XalanUnicode::charLetter_a, XalanUnicode::charLetter_c, XalanUnicode::charLetter_u, XalanUnicode::charLetter_t, XalanUnicode::charLetter_e, 0 } },
    { 244, 5, { XalanUnicode::charLetter_o, XalanUnicode::charLetter_c, XalanUnicode::charLetter_i, XalanUnicode::charLetter_r, XalanUnicode::charLetter_c, 0 } },
    { 245, 6, { XalanUnicode::charLetter_o, XalanUnicode::charLetter_t, XalanUnicode::charLetter_i, XalanUnicode::charLetter_l, XalanUnicode::charLetter_d, XalanUnicode::charLetter_e, 0 } },
    { 246, 4, { XalanUnicode::charLetter_o, XalanUnicode::charLetter_u, XalanUnicode::charLetter_m, XalanUnicode::charLetter_l, 0 } },
    { 247, 6, { XalanUnicode::charLetter_d, XalanUnicode::charLetter_i, XalanUnicode::charLetter_v, XalanUnicode::charLetter_i, XalanUnicode::charLetter_d, XalanUnicode::charLetter_e, 0 } },
    { 248, 6, { XalanUnicode::charLetter_o, XalanUnicode::charLetter_s, XalanUnicode::charLetter_l, XalanUnicode::charLetter_a, XalanUnicode::charLetter_s, XalanUnicode::charLetter_h, 0 } },
    { 249, 6, { XalanUnicode::charLetter_u, XalanUnicode::charLetter_g, XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_v, XalanUnicode::charLetter_e, 0 } },
    { 250, 6, { XalanUnicode::charLetter_u, XalanUnicode::charLetter_a, XalanUnicode::charLetter_c, XalanUnicode::charLetter_u, XalanUnicode::charLetter_t, XalanUnicode::charLetter_e, 0 } },
    { 251, 5, { XalanUnicode::charLetter_u, XalanUnicode::charLetter_c, XalanUnicode::charLetter_i, XalanUnicode::charLetter_r, XalanUnicode::charLetter_c, 0 } },
    { 252, 4, { XalanUnicode::charLetter_u, XalanUnicode::charLetter_u, XalanUnicode::charLetter_m, XalanUnicode::charLetter_l, 0 } },
    { 253, 6, { XalanUnicode::charLetter_y, XalanUnicode::charLetter_a, XalanUnicode::charLetter_c, XalanUnicode::charLetter_u, XalanUnicode::charLetter_t, XalanUnicode::charLetter_e, 0 } },
    { 254, 5, { XalanUnicode::charLetter_t, XalanUnicode::charLetter_h, XalanUnicode::charLetter_o, XalanUnicode::charLetter_r, XalanUnicode::charLetter_n, 0 } },
    { 255, 4, { XalanUnicode::charLetter_y, XalanUnicode::charLetter_u, XalanUnicode::charLetter_m, XalanUnicode::charLetter_l, 0 } },
    { 338, 5, { XalanUnicode::charLetter_O, XalanUnicode::charLetter_E, XalanUnicode::charLetter_l, XalanUnicode::charLetter_i, XalanUnicode::charLetter_g, 0 } },
    { 339, 5, { XalanUnicode::charLetter_o, XalanUnicode::charLetter_e, XalanUnicode::charLetter_l, XalanUnicode::charLetter_i, XalanUnicode::charLetter_g, 0 } },
    { 352, 6, { XalanUnicode::charLetter_S, XalanUnicode::charLetter_c, XalanUnicode::charLetter_a, XalanUnicode::charLetter_r, XalanUnicode::charLetter_o, XalanUnicode::charLetter_n, 0 } },
    { 353, 6, { XalanUnicode::charLetter_s, XalanUnicode::charLetter_c, XalanUnicode::charLetter_a, XalanUnicode::charLetter_r, XalanUnicode::charLetter_o, XalanUnicode::charLetter_n, 0 } },
    { 376, 4, { XalanUnicode::charLetter_Y, XalanUnicode::charLetter_u, XalanUnicode::charLetter_m, XalanUnicode::charLetter_l, 0 } },
#if 0
    { 402, 4, { XalanUnicode::charLetter_f, XalanUnicode::charLetter_n, XalanUnicode::charLetter_o, XalanUnicode::charLetter_f, 0 } },
#endif
    { 710, 4, { XalanUnicode::charLetter_c, XalanUnicode::charLetter_i, XalanUnicode::charLetter_r, XalanUnicode::charLetter_c, 0 } },
    { 732, 5, { XalanUnicode::charLetter_t, XalanUnicode::charLetter_i, XalanUnicode::charLetter_l, XalanUnicode::charLetter_d, XalanUnicode::charLetter_e, 0 } },
#if 0
    { 913, 5, { XalanUnicode::charLetter_A, XalanUnicode::charLetter_l, XalanUnicode::charLetter_p, XalanUnicode::charLetter_h, XalanUnicode::charLetter_a, 0 } },
    { 914, 4, { XalanUnicode::charLetter_B, XalanUnicode::charLetter_e, XalanUnicode::charLetter_t, XalanUnicode::charLetter_a, 0 } },
    { 915, 5, { XalanUnicode::charLetter_G, XalanUnicode::charLetter_a, XalanUnicode::charLetter_m, XalanUnicode::charLetter_m, XalanUnicode::charLetter_a, 0 } },
    { 916, 5, { XalanUnicode::charLetter_D, XalanUnicode::charLetter_e, XalanUnicode::charLetter_l, XalanUnicode::charLetter_t, XalanUnicode::charLetter_a, 0 } },
    { 917, 7, { XalanUnicode::charLetter_E, XalanUnicode::charLetter_p, XalanUnicode::charLetter_s, XalanUnicode::charLetter_i, XalanUnicode::charLetter_l, XalanUnicode::charLetter_o, XalanUnicode::charLetter_n, 0 } },
    { 918, 4, { XalanUnicode::charLetter_Z, XalanUnicode::charLetter_e, XalanUnicode::charLetter_t, XalanUnicode::charLetter_a, 0 } },
    { 919, 3, { XalanUnicode::charLetter_E, XalanUnicode::charLetter_t, XalanUnicode::charLetter_a, 0 } },
    { 920, 5, { XalanUnicode::charLetter_T, XalanUnicode::charLetter_h, XalanUnicode::charLetter_e, XalanUnicode::charLetter_t, XalanUnicode::charLetter_a, 0 } },
    { 921, 4, { XalanUnicode::charLetter_I, XalanUnicode::charLetter_o, XalanUnicode::charLetter_t, XalanUnicode::charLetter_a, 0 } },
    { 922, 5, { XalanUnicode::charLetter_K, XalanUnicode::charLetter_a, XalanUnicode::charLetter_p, XalanUnicode::charLetter_p, XalanUnicode::charLetter_a, 0 } },
    { 923, 6, { XalanUnicode::charLetter_L, XalanUnicode::charLetter_a, XalanUnicode::charLetter_m, XalanUnicode::charLetter_b, XalanUnicode::charLetter_d, XalanUnicode::charLetter_a, 0 } },
    { 924, 2, { XalanUnicode::charLetter_M, XalanUnicode::charLetter_u, 0 } },
    { 925, 2, { XalanUnicode::charLetter_N, XalanUnicode::charLetter_u, 0 } },
    { 926, 2, { XalanUnicode::charLetter_X, XalanUnicode::charLetter_i, 0 } },
    { 927, 7, { XalanUnicode::charLetter_O, XalanUnicode::charLetter_m, XalanUnicode::charLetter_i, XalanUnicode::charLetter_c, XalanUnicode::charLetter_r, XalanUnicode::charLetter_o, XalanUnicode::charLetter_n, 0 } },
    { 928, 2, { XalanUnicode::charLetter_P, XalanUnicode::charLetter_i, 0 } },
    { 929, 3, { XalanUnicode::charLetter_R, XalanUnicode::charLetter_h, XalanUnicode::charLetter_o, 0 } },
    { 931, 5, { XalanUnicode::charLetter_S, XalanUnicode::charLetter_i, XalanUnicode::charLetter_g, XalanUnicode::charLetter_m, XalanUnicode::charLetter_a, 0 } },
    { 932, 3, { XalanUnicode::charLetter_T, XalanUnicode::charLetter_a, XalanUnicode::charLetter_u, 0 } },
    { 933, 7, { XalanUnicode::charLetter_U, XalanUnicode::charLetter_p, XalanUnicode::charLetter_s, XalanUnicode::charLetter_i, XalanUnicode::charLetter_l, XalanUnicode::charLetter_o, XalanUnicode::charLetter_n, 0 } },
    { 934, 3, { XalanUnicode::charLetter_P, XalanUnicode::charLetter_h, XalanUnicode::charLetter_i, 0 } },
    { 935, 3, { XalanUnicode::charLetter_C, XalanUnicode::charLetter_h, XalanUnicode::charLetter_i, 0 } },
    { 936, 3, { XalanUnicode::charLetter_P, XalanUnicode::charLetter_s, XalanUnicode::charLetter_i, 0 } },
    { 937, 5, { XalanUnicode::charLetter_O, XalanUnicode::charLetter_m, XalanUnicode::charLetter_e, XalanUnicode::charLetter_g, XalanUnicode::charLetter_a, 0 } },
    { 945, 5, { XalanUnicode::charLetter_a, XalanUnicode::charLetter_l, XalanUnicode::charLetter_p, XalanUnicode::charLetter_h, XalanUnicode::charLetter_a, 0 } },
    { 946, 4, { XalanUnicode::charLetter_b, XalanUnicode::charLetter_e, XalanUnicode::charLetter_t, XalanUnicode::charLetter_a, 0 } },
    { 947, 5, { XalanUnicode::charLetter_g, XalanUnicode::charLetter_a, XalanUnicode::charLetter_m, XalanUnicode::charLetter_m, XalanUnicode::charLetter_a, 0 } },
    { 948, 5, { XalanUnicode::charLetter_d, XalanUnicode::charLetter_e, XalanUnicode::charLetter_l, XalanUnicode::charLetter_t, XalanUnicode::charLetter_a, 0 } },
    { 949, 7, { XalanUnicode::charLetter_e, XalanUnicode::charLetter_p, XalanUnicode::charLetter_s, XalanUnicode::charLetter_i, XalanUnicode::charLetter_l, XalanUnicode::charLetter_o, XalanUnicode::charLetter_n, 0 } },
    { 950, 4, { XalanUnicode::charLetter_z, XalanUnicode::charLetter_e, XalanUnicode::charLetter_t, XalanUnicode::charLetter_a, 0 } },
    { 951, 3, { XalanUnicode::charLetter_e, XalanUnicode::charLetter_t, XalanUnicode::charLetter_a, 0 } },
    { 952, 5, { XalanUnicode::charLetter_t, XalanUnicode::charLetter_h, XalanUnicode::charLetter_e, XalanUnicode::charLetter_t, XalanUnicode::charLetter_a, 0 } },
    { 953, 4, { XalanUnicode::charLetter_i, XalanUnicode::charLetter_o, XalanUnicode::charLetter_t, XalanUnicode::charLetter_a, 0 } },
    { 954, 5, { XalanUnicode::charLetter_k, XalanUnicode::charLetter_a, XalanUnicode::charLetter_p, XalanUnicode::charLetter_p, XalanUnicode::charLetter_a, 0 } },
    { 955, 6, { XalanUnicode::charLetter_l, XalanUnicode::charLetter_a, XalanUnicode::charLetter_m, XalanUnicode::charLetter_b, XalanUnicode::charLetter_d, XalanUnicode::charLetter_a, 0 } },
    { 956, 2, { XalanUnicode::charLetter_m, XalanUnicode::charLetter_u, 0 } },
    { 957, 2, { XalanUnicode::charLetter_n, XalanUnicode::charLetter_u, 0 } },
    { 958, 2, { XalanUnicode::charLetter_x, XalanUnicode::charLetter_i, 0 } },
    { 959, 7, { XalanUnicode::charLetter_o, XalanUnicode::charLetter_m, XalanUnicode::charLetter_i, XalanUnicode::charLetter_c, XalanUnicode::charLetter_r, XalanUnicode::charLetter_o, XalanUnicode::charLetter_n, 0 } },
    { 960, 2, { XalanUnicode::charLetter_p, XalanUnicode::charLetter_i, 0 } },
    { 961, 3, { XalanUnicode::charLetter_r, XalanUnicode::charLetter_h, XalanUnicode::charLetter_o, 0 } },
    { 962, 6, { XalanUnicode::charLetter_s, XalanUnicode::charLetter_i, XalanUnicode::charLetter_g, XalanUnicode::charLetter_m, XalanUnicode::charLetter_a, XalanUnicode::charLetter_f, 0 } },
    { 963, 5, { XalanUnicode::charLetter_s, XalanUnicode::charLetter_i, XalanUnicode::charLetter_g, XalanUnicode::charLetter_m, XalanUnicode::charLetter_a, 0 } },
    { 964, 3, { XalanUnicode::charLetter_t, XalanUnicode::charLetter_a, XalanUnicode::charLetter_u, 0 } },
    { 965, 7, { XalanUnicode::charLetter_u, XalanUnicode::charLetter_p, XalanUnicode::charLetter_s, XalanUnicode::charLetter_i, XalanUnicode::charLetter_l, XalanUnicode::charLetter_o, XalanUnicode::charLetter_n, 0 } },
    { 966, 3, { XalanUnicode::charLetter_p, XalanUnicode::charLetter_h, XalanUnicode::charLetter_i, 0 } },
    { 967, 3, { XalanUnicode::charLetter_c, XalanUnicode::charLetter_h, XalanUnicode::charLetter_i, 0 } },
    { 968, 3, { XalanUnicode::charLetter_p, XalanUnicode::charLetter_s, XalanUnicode::charLetter_i, 0 } },
    { 969, 5, { XalanUnicode::charLetter_o, XalanUnicode::charLetter_m, XalanUnicode::charLetter_e, XalanUnicode::charLetter_g, XalanUnicode::charLetter_a, 0 } },
    { 977, 8, { XalanUnicode::charLetter_t, XalanUnicode::charLetter_h, XalanUnicode::charLetter_e, XalanUnicode::charLetter_t, XalanUnicode::charLetter_a, XalanUnicode::charLetter_s, XalanUnicode::charLetter_y, XalanUnicode::charLetter_m, 0 } },
    { 978, 5, { XalanUnicode::charLetter_u, XalanUnicode::charLetter_p, XalanUnicode::charLetter_s, XalanUnicode::charLetter_i, XalanUnicode::charLetter_h, 0 } },
    { 982, 3, { XalanUnicode::charLetter_p, XalanUnicode::charLetter_i, XalanUnicode::charLetter_v, 0 } },
#endif
    { 8194, 4, { XalanUnicode::charLetter_e, XalanUnicode::charLetter_n, XalanUnicode::charLetter_s, XalanUnicode::charLetter_p, 0 } },
    { 8195, 4, { XalanUnicode::charLetter_e, XalanUnicode::charLetter_m, XalanUnicode::charLetter_s, XalanUnicode::charLetter_p, 0 } },
    { 8201, 6, { XalanUnicode::charLetter_t, XalanUnicode::charLetter_h, XalanUnicode::charLetter_i, XalanUnicode::charLetter_n, XalanUnicode::charLetter_s, XalanUnicode::charLetter_p, 0 } },
    { 8204, 4, { XalanUnicode::charLetter_z, XalanUnicode::charLetter_w, XalanUnicode::charLetter_n, XalanUnicode::charLetter_j, 0 } },
    { 8205, 3, { XalanUnicode::charLetter_z, XalanUnicode::charLetter_w, XalanUnicode::charLetter_j, 0 } },
    { 8206, 3, { XalanUnicode::charLetter_l, XalanUnicode::charLetter_r, XalanUnicode::charLetter_m, 0 } },
    { 8207, 3, { XalanUnicode::charLetter_r, XalanUnicode::charLetter_l, XalanUnicode::charLetter_m, 0 } },
    { 8211, 5, { XalanUnicode::charLetter_n, XalanUnicode::charLetter_d, XalanUnicode::charLetter_a, XalanUnicode::charLetter_s, XalanUnicode::charLetter_h, 0 } },
    { 8212, 5, { XalanUnicode::charLetter_m, XalanUnicode::charLetter_d, XalanUnicode::charLetter_a, XalanUnicode::charLetter_s, XalanUnicode::charLetter_h, 0 } },
    { 8216, 5, { XalanUnicode::charLetter_l, XalanUnicode::charLetter_s, XalanUnicode::charLetter_q, XalanUnicode::charLetter_u, XalanUnicode::charLetter_o, 0 } },
    { 8217, 5, { XalanUnicode::charLetter_r, XalanUnicode::charLetter_s, XalanUnicode::charLetter_q, XalanUnicode::charLetter_u, XalanUnicode::charLetter_o, 0 } },
    { 8218, 5, { XalanUnicode::charLetter_s, XalanUnicode::charLetter_b, XalanUnicode::charLetter_q, XalanUnicode::charLetter_u, XalanUnicode::charLetter_o, 0 } },
    { 8220, 5, { XalanUnicode::charLetter_l, XalanUnicode::charLetter_d, XalanUnicode::charLetter_q, XalanUnicode::charLetter_u, XalanUnicode::charLetter_o, 0 } },
    { 8221, 5, { XalanUnicode::charLetter_r, XalanUnicode::charLetter_d, XalanUnicode::charLetter_q, XalanUnicode::charLetter_u, XalanUnicode::charLetter_o, 0 } },
    { 8222, 5, { XalanUnicode::charLetter_b, XalanUnicode::charLetter_d, XalanUnicode::charLetter_q, XalanUnicode::charLetter_u, XalanUnicode::charLetter_o, 0 } },
    { 8224, 6, { XalanUnicode::charLetter_d, XalanUnicode::charLetter_a, XalanUnicode::charLetter_g, XalanUnicode::charLetter_g, XalanUnicode::charLetter_e, XalanUnicode::charLetter_r, 0 } },
    { 8225, 6, { XalanUnicode::charLetter_D, XalanUnicode::charLetter_a, XalanUnicode::charLetter_g, XalanUnicode::charLetter_g, XalanUnicode::charLetter_e, XalanUnicode::charLetter_r, 0 } },
    { 8226, 4, { XalanUnicode::charLetter_b, XalanUnicode::charLetter_u, XalanUnicode::charLetter_l, XalanUnicode::charLetter_l, 0 } },
    { 8230, 6, { XalanUnicode::charLetter_h, XalanUnicode::charLetter_e, XalanUnicode::charLetter_l, XalanUnicode::charLetter_l, XalanUnicode::charLetter_i, XalanUnicode::charLetter_p, 0 } },
    { 8240, 6, { XalanUnicode::charLetter_p, XalanUnicode::charLetter_e, XalanUnicode::charLetter_r, XalanUnicode::charLetter_m, XalanUnicode::charLetter_i, XalanUnicode::charLetter_l, 0 } },
    { 8242, 5, { XalanUnicode::charLetter_p, XalanUnicode::charLetter_r, XalanUnicode::charLetter_i, XalanUnicode::charLetter_m, XalanUnicode::charLetter_e, 0 } },
    { 8243, 5, { XalanUnicode::charLetter_P, XalanUnicode::charLetter_r, XalanUnicode::charLetter_i, XalanUnicode::charLetter_m, XalanUnicode::charLetter_e, 0 } },
    { 8249, 6, { XalanUnicode::charLetter_l, XalanUnicode::charLetter_s, XalanUnicode::charLetter_a, XalanUnicode::charLetter_q, XalanUnicode::charLetter_u, XalanUnicode::charLetter_o, 0 } },
    { 8250, 6, { XalanUnicode::charLetter_r, XalanUnicode::charLetter_s, XalanUnicode::charLetter_a, XalanUnicode::charLetter_q, XalanUnicode::charLetter_u, XalanUnicode::charLetter_o, 0 } },
    { 8254, 5, { XalanUnicode::charLetter_o, XalanUnicode::charLetter_l, XalanUnicode::charLetter_i, XalanUnicode::charLetter_n, XalanUnicode::charLetter_e, 0 } },
    { 8260, 5, { XalanUnicode::charLetter_f, XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_s, XalanUnicode::charLetter_l, 0 } },
    { 8364, 4, { XalanUnicode::charLetter_e, XalanUnicode::charLetter_u, XalanUnicode::charLetter_r, XalanUnicode::charLetter_o, 0 } },
    { 8465, 5, { XalanUnicode::charLetter_i, XalanUnicode::charLetter_m, XalanUnicode::charLetter_a, XalanUnicode::charLetter_g, XalanUnicode::charLetter_e, 0 } },
    { 8472, 6, { XalanUnicode::charLetter_w, XalanUnicode::charLetter_e, XalanUnicode::charLetter_i, XalanUnicode::charLetter_e, XalanUnicode::charLetter_r, XalanUnicode::charLetter_p, 0 } },
    { 8476, 4, { XalanUnicode::charLetter_r, XalanUnicode::charLetter_e, XalanUnicode::charLetter_a, XalanUnicode::charLetter_l, 0 } },
    { 8482, 5, { XalanUnicode::charLetter_t, XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_d, XalanUnicode::charLetter_e, 0 } },
    { 8501, 7, { XalanUnicode::charLetter_a, XalanUnicode::charLetter_l, XalanUnicode::charLetter_e, XalanUnicode::charLetter_f, XalanUnicode::charLetter_s, XalanUnicode::charLetter_y, XalanUnicode::charLetter_m, 0 } },
    { 8592, 4, { XalanUnicode::charLetter_l, XalanUnicode::charLetter_a, XalanUnicode::charLetter_r, XalanUnicode::charLetter_r, 0 } },
    { 8593, 4, { XalanUnicode::charLetter_u, XalanUnicode::charLetter_a, XalanUnicode::charLetter_r, XalanUnicode::charLetter_r, 0 } },
    { 8594, 4, { XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_r, XalanUnicode::charLetter_r, 0 } },
    { 8595, 4, { XalanUnicode::charLetter_d, XalanUnicode::charLetter_a, XalanUnicode::charLetter_r, XalanUnicode::charLetter_r, 0 } },
    { 8596, 4, { XalanUnicode::charLetter_h, XalanUnicode::charLetter_a, XalanUnicode::charLetter_r, XalanUnicode::charLetter_r, 0 } },
    { 8629, 5, { XalanUnicode::charLetter_c, XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_r, XalanUnicode::charLetter_r, 0 } },
    { 8656, 4, { XalanUnicode::charLetter_l, XalanUnicode::charLetter_A, XalanUnicode::charLetter_r, XalanUnicode::charLetter_r, 0 } },
    { 8657, 4, { XalanUnicode::charLetter_u, XalanUnicode::charLetter_A, XalanUnicode::charLetter_r, XalanUnicode::charLetter_r, 0 } },
    { 8658, 4, { XalanUnicode::charLetter_r, XalanUnicode::charLetter_A, XalanUnicode::charLetter_r, XalanUnicode::charLetter_r, 0 } },
    { 8659, 4, { XalanUnicode::charLetter_d, XalanUnicode::charLetter_A, XalanUnicode::charLetter_r, XalanUnicode::charLetter_r, 0 } },
    { 8660, 4, { XalanUnicode::charLetter_h, XalanUnicode::charLetter_A, XalanUnicode::charLetter_r, XalanUnicode::charLetter_r, 0 } },
    { 8704, 6, { XalanUnicode::charLetter_f, XalanUnicode::charLetter_o, XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_l, XalanUnicode::charLetter_l, 0 } },
    { 8706, 4, { XalanUnicode::charLetter_p, XalanUnicode::charLetter_a, XalanUnicode::charLetter_r, XalanUnicode::charLetter_t, 0 } },
    { 8707, 5, { XalanUnicode::charLetter_e, XalanUnicode::charLetter_x, XalanUnicode::charLetter_i, XalanUnicode::charLetter_s, XalanUnicode::charLetter_t, 0 } },
    { 8709, 5, { XalanUnicode::charLetter_e, XalanUnicode::charLetter_m, XalanUnicode::charLetter_p, XalanUnicode::charLetter_t, XalanUnicode::charLetter_y, 0 } },
    { 8711, 5, { XalanUnicode::charLetter_n, XalanUnicode::charLetter_a, XalanUnicode::charLetter_b, XalanUnicode::charLetter_l, XalanUnicode::charLetter_a, 0 } },
    { 8712, 4, { XalanUnicode::charLetter_i, XalanUnicode::charLetter_s, XalanUnicode::charLetter_i, XalanUnicode::charLetter_n, 0 } },
    { 8713, 5, { XalanUnicode::charLetter_n, XalanUnicode::charLetter_o, XalanUnicode::charLetter_t, XalanUnicode::charLetter_i, XalanUnicode::charLetter_n, 0 } },
    { 8715, 2, { XalanUnicode::charLetter_n, XalanUnicode::charLetter_i, 0 } },
    { 8719, 4, { XalanUnicode::charLetter_p, XalanUnicode::charLetter_r, XalanUnicode::charLetter_o, XalanUnicode::charLetter_d, 0 } },
    { 8721, 3, { XalanUnicode::charLetter_s, XalanUnicode::charLetter_u, XalanUnicode::charLetter_m, 0 } },
    { 8722, 5, { XalanUnicode::charLetter_m, XalanUnicode::charLetter_i, XalanUnicode::charLetter_n, XalanUnicode::charLetter_u, XalanUnicode::charLetter_s, 0 } },
    { 8727, 6, { XalanUnicode::charLetter_l, XalanUnicode::charLetter_o, XalanUnicode::charLetter_w, XalanUnicode::charLetter_a, XalanUnicode::charLetter_s, XalanUnicode::charLetter_t, 0 } },
    { 8730, 5, { XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_d, XalanUnicode::charLetter_i, XalanUnicode::charLetter_c, 0 } },
    { 8733, 4, { XalanUnicode::charLetter_p, XalanUnicode::charLetter_r, XalanUnicode::charLetter_o, XalanUnicode::charLetter_p, 0 } },
    { 8734, 5, { XalanUnicode::charLetter_i, XalanUnicode::charLetter_n, XalanUnicode::charLetter_f, XalanUnicode::charLetter_i, XalanUnicode::charLetter_n, 0 } },
    { 8736, 3, { XalanUnicode::charLetter_a, XalanUnicode::charLetter_n, XalanUnicode::charLetter_g, 0 } },
    { 8743, 3, { XalanUnicode::charLetter_a, XalanUnicode::charLetter_n, XalanUnicode::charLetter_d, 0 } },
    { 8744, 2, { XalanUnicode::charLetter_o, XalanUnicode::charLetter_r, 0 } },
    { 8745, 3, { XalanUnicode::charLetter_c, XalanUnicode::charLetter_a, XalanUnicode::charLetter_p, 0 } },
    { 8746, 3, { XalanUnicode::charLetter_c, XalanUnicode::charLetter_u, XalanUnicode::charLetter_p, 0 } },
    { 8747, 3, { XalanUnicode::charLetter_i, XalanUnicode::charLetter_n, XalanUnicode::charLetter_t, 0 } },
    { 8756, 6, { XalanUnicode::charLetter_t, XalanUnicode::charLetter_h, XalanUnicode::charLetter_e, XalanUnicode::charLetter_r, XalanUnicode::charLetter_e, XalanUnicode::charDigit_4, 0 } },
    { 8764, 3, { XalanUnicode::charLetter_s, XalanUnicode::charLetter_i, XalanUnicode::charLetter_m, 0 } },
    { 8773, 4, { XalanUnicode::charLetter_c, XalanUnicode::charLetter_o, XalanUnicode::charLetter_n, XalanUnicode::charLetter_g, 0 } },
    { 8776, 5, { XalanUnicode::charLetter_a, XalanUnicode::charLetter_s, XalanUnicode::charLetter_y, XalanUnicode::charLetter_m, XalanUnicode::charLetter_p, 0 } },
    { 8800, 2, { XalanUnicode::charLetter_n, XalanUnicode::charLetter_e, 0 } },
    { 8801, 5, { XalanUnicode::charLetter_e, XalanUnicode::charLetter_q, XalanUnicode::charLetter_u, XalanUnicode::charLetter_i, XalanUnicode::charLetter_v, 0 } },
    { 8804, 2, { XalanUnicode::charLetter_l, XalanUnicode::charLetter_e, 0 } },
    { 8805, 2, { XalanUnicode::charLetter_g, XalanUnicode::charLetter_e, 0 } },
    { 8834, 3, { XalanUnicode::charLetter_s, XalanUnicode::charLetter_u, XalanUnicode::charLetter_b, 0 } },
    { 8835, 3, { XalanUnicode::charLetter_s, XalanUnicode::charLetter_u, XalanUnicode::charLetter_p, 0 } },
    { 8836, 4, { XalanUnicode::charLetter_n, XalanUnicode::charLetter_s, XalanUnicode::charLetter_u, XalanUnicode::charLetter_b, 0 } },
    { 8838, 4, { XalanUnicode::charLetter_s, XalanUnicode::charLetter_u, XalanUnicode::charLetter_b, XalanUnicode::charLetter_e, 0 } },
    { 8839, 4, { XalanUnicode::charLetter_s, XalanUnicode::charLetter_u, XalanUnicode::charLetter_p, XalanUnicode::charLetter_e, 0 } },
    { 8853, 5, { XalanUnicode::charLetter_o, XalanUnicode::charLetter_p, XalanUnicode::charLetter_l, XalanUnicode::charLetter_u, XalanUnicode::charLetter_s, 0 } },
    { 8855, 6, { XalanUnicode::charLetter_o, XalanUnicode::charLetter_t, XalanUnicode::charLetter_i, XalanUnicode::charLetter_m, XalanUnicode::charLetter_e, XalanUnicode::charLetter_s, 0 } },
    { 8869, 4, { XalanUnicode::charLetter_p, XalanUnicode::charLetter_e, XalanUnicode::charLetter_r, XalanUnicode::charLetter_p, 0 } },
    { 8901, 4, { XalanUnicode::charLetter_s, XalanUnicode::charLetter_d, XalanUnicode::charLetter_o, XalanUnicode::charLetter_t, 0 } },
    { 8968, 5, { XalanUnicode::charLetter_l, XalanUnicode::charLetter_c, XalanUnicode::charLetter_e, XalanUnicode::charLetter_i, XalanUnicode::charLetter_l, 0 } },
    { 8969, 5, { XalanUnicode::charLetter_r, XalanUnicode::charLetter_c, XalanUnicode::charLetter_e, XalanUnicode::charLetter_i, XalanUnicode::charLetter_l, 0 } },
    { 8970, 6, { XalanUnicode::charLetter_l, XalanUnicode::charLetter_f, XalanUnicode::charLetter_l, XalanUnicode::charLetter_o, XalanUnicode::charLetter_o, XalanUnicode::charLetter_r, 0 } },
    { 8971, 6, { XalanUnicode::charLetter_r, XalanUnicode::charLetter_f, XalanUnicode::charLetter_l, XalanUnicode::charLetter_o, XalanUnicode::charLetter_o, XalanUnicode::charLetter_r, 0 } },
    { 9001, 4, { XalanUnicode::charLetter_l, XalanUnicode::charLetter_a, XalanUnicode::charLetter_n, XalanUnicode::charLetter_g, 0 } },
    { 9002, 4, { XalanUnicode::charLetter_r, XalanUnicode::charLetter_a, XalanUnicode::charLetter_n, XalanUnicode::charLetter_g, 0 } },
    { 9674, 3, { XalanUnicode::charLetter_l, XalanUnicode::charLetter_o, XalanUnicode::charLetter_z, 0 } },
    { 9824, 6, { XalanUnicode::charLetter_s, XalanUnicode::charLetter_p, XalanUnicode::charLetter_a, XalanUnicode::charLetter_d, XalanUnicode::charLetter_e, XalanUnicode::charLetter_s, 0 } },
    { 9827, 5, { XalanUnicode::charLetter_c, XalanUnicode::charLetter_l, XalanUnicode::charLetter_u, XalanUnicode::charLetter_b, XalanUnicode::charLetter_s, 0 } },
    { 9829, 6, { XalanUnicode::charLetter_h, XalanUnicode::charLetter_e, XalanUnicode::charLetter_a, XalanUnicode::charLetter_r, XalanUnicode::charLetter_t, XalanUnicode::charLetter_s, 0 } },
    { 9830, 5, { XalanUnicode::charLetter_d, XalanUnicode::charLetter_i, XalanUnicode::charLetter_a, XalanUnicode::charLetter_m, XalanUnicode::charLetter_s, 0 } }
};

const FormatterToHTML::Entity* const    FormatterToHTML::s_lastEntity = 
    FormatterToHTML::s_entities + (sizeof(s_entities) / sizeof (s_entities[0])) - 1;



#define FHTML_SIZE(str) ((sizeof(str) / sizeof(str[0]) - 1))

const XalanDOMChar                      FormatterToHTML::s_doctypeHeaderStartString[] =
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
    XalanUnicode::charLetter_H,
    XalanUnicode::charLetter_T,
    XalanUnicode::charLetter_M,
    XalanUnicode::charLetter_L,
    0
};

const FormatterToHTML::size_type        FormatterToHTML::s_doctypeHeaderStartStringLength =
        FHTML_SIZE(s_doctypeHeaderStartString);

const XalanDOMChar                      FormatterToHTML::s_doctypeHeaderPublicString[] =
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

const FormatterToHTML::size_type        FormatterToHTML::s_doctypeHeaderPublicStringLength =
        FHTML_SIZE(s_doctypeHeaderPublicString);

const XalanDOMChar                      FormatterToHTML::s_doctypeHeaderSystemString[] =
{
    XalanUnicode::charSpace,
    XalanUnicode::charLetter_S,
    XalanUnicode::charLetter_Y,
    XalanUnicode::charLetter_S,
    XalanUnicode::charLetter_T,
    XalanUnicode::charLetter_E,
    XalanUnicode::charLetter_M,
    0
};

const FormatterToHTML::size_type        FormatterToHTML::s_doctypeHeaderSystemStringLength =
        FHTML_SIZE(s_doctypeHeaderSystemString);

const XalanDOMChar                      FormatterToHTML::s_metaString[] =
{
    XalanUnicode::charLessThanSign,
    XalanUnicode::charLetter_M,
    XalanUnicode::charLetter_E,
    XalanUnicode::charLetter_T,
    XalanUnicode::charLetter_A,
    XalanUnicode::charSpace,
    XalanUnicode::charLetter_h,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_p,
    XalanUnicode::charHyphenMinus,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_q,
    XalanUnicode::charLetter_u,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_v,
    XalanUnicode::charEqualsSign,
    XalanUnicode::charQuoteMark,
    XalanUnicode::charLetter_C,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_t,
    XalanUnicode::charHyphenMinus,
    XalanUnicode::charLetter_T,
    XalanUnicode::charLetter_y,
    XalanUnicode::charLetter_p,
    XalanUnicode::charLetter_e,
    XalanUnicode::charQuoteMark,
    XalanUnicode::charSpace,
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_t,
    XalanUnicode::charEqualsSign,
    XalanUnicode::charQuoteMark,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_x,
    XalanUnicode::charLetter_t,
    XalanUnicode::charSolidus,
    XalanUnicode::charLetter_h,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_m,
    XalanUnicode::charLetter_l,
    XalanUnicode::charSemicolon,
    XalanUnicode::charSpace,
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_h,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_t,
    XalanUnicode::charEqualsSign,
    0
};


const FormatterToHTML::size_type        FormatterToHTML::s_metaStringLength =
        FHTML_SIZE(s_metaString);



XALAN_CPP_NAMESPACE_END
