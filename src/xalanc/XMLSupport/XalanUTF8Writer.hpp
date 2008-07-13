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
#if !defined(XALANUTF8WRITER_HEADER_GUARD_1357924680)
#define XALANUTF8WRITER_HEADER_GUARD_1357924680


#include <xalanc/XMLSupport/XalanFormatterWriter.hpp>



XALAN_CPP_NAMESPACE_BEGIN



inline char
bits19to21(XalanUnicodeChar     theChar)
{
    return static_cast<char>((theChar >> 18) & 0x7);
}



inline char
bits13to18(XalanUnicodeChar     theChar)
{
    return static_cast<char>((theChar >> 12) & 0x3F);
}



inline char
bits13to16(XalanUnicodeChar     theChar)
{
    return static_cast<char>((theChar >> 12) & 0xF);
}



inline char
bits7to12(XalanUnicodeChar      theChar)
{
    return static_cast<char>((theChar >> 6) & 0x3f);
}



inline char
bits7to11(XalanUnicodeChar      theChar)
{
    return static_cast<char>((theChar >> 6) & 0x1f);
}



inline char
bits1to6(XalanUnicodeChar       theChar)
{
    return static_cast<char>(theChar & 0x3f);
}



inline char
leadingByteOf2(char     theBits)
{
    return static_cast<char>(0xC0 + theBits);
}



inline char
leadingByteOf3(char     theBits)
{
    return static_cast<char>(0xE0 + theBits);
}



inline char
leadingByteOf4(char     theBits)
{
    return static_cast<char>(0xF0 + theBits);
}



inline char
trailingByte(char   theBits)
{
    return static_cast<char>(0x80 + theBits);
}



class XalanUTF8Writer : public XalanFormatterWriter
{
public:

    typedef char    value_type;
 

    XalanUTF8Writer(
                Writer&         writer,
                MemoryManager&  theMemoryManager);

    virtual
    ~XalanUTF8Writer()
    {
    }

    /**
     * Output a line break.
     */
    void
    outputNewline()
    {
        assert(m_newlineString != 0);
        assert(length(m_newlineString) == m_newlineStringLength);

        write(
            m_newlineString,
            m_newlineStringLength);
    }

    size_type
    writeCDATAChar(
                const XalanDOMChar  chars[],
                size_type           start,
                size_type           length,
                bool&               /* outsideCDATA */)
    {
        assert(chars != 0 && length != 0 && start < length);

        return write(chars, start, length);
    }

    /**
     * Writes name characters.  If characters that are not
     * representable are encountered, an exception is thrown.
     */
    void
    writeNameChar(
            const XalanDOMChar*      data,
            size_type                theLength)
    {
        write(data, theLength);
    }

    /**
     * Writes PI characters.  If characters that are not
     * representable are encountered, an exception is thrown.
     */
    void
    writePIChars(
            const XalanDOMChar*     data,
            size_type               theLength)
    {
        write(data, theLength);
    }

    /**
     * Writes comment characters.  If characters that are not
     * representable are encountered, an exception is thrown.
     */
    void
    writeCommentChars(
            const XalanDOMChar*      data,
            size_type                theLength)
    {
        write(data, theLength);
    }

    void
    safeWriteContent(
            const XalanDOMChar*     theChars,
            size_type               theLength)
    {
        for(size_type i = 0; i < theLength; ++i)
        { 
            write(value_type(theChars[i]));
        }
    }

    void
    write(
            const value_type*   theChars,
            size_type           theLength)
    {
    #if defined(NDEBUG)
        if (theLength > sizeof(m_buffer))
        {
            flushBuffer();
    
            m_writer.write(theChars, 0, theLength);
        }
        else
        {
            if (m_bufferRemaining < theLength)
            {
                flushBuffer();
            }
    
            for(size_type i = 0; i < theLength; ++i)
            {
                *m_bufferPosition = theChars[i];
    
                ++m_bufferPosition;
            }
    
            m_bufferRemaining -= theLength;
        }
    #else
        for(size_type i = 0; i < theLength; ++i)
        {
            write(theChars[i]);
        }
    #endif
    }

    void
    write(const XalanDOMChar*   theChars)
    {
        write(theChars, XalanDOMString::length(theChars));
    }

    void
    write(const XalanDOMString&     theChars)
    {
        write(theChars.c_str(), theChars.length());
    }

    void
    write(value_type    theChar)
    {
        assert(theChar < 128);
    
        if (m_bufferRemaining == 0)
        {
            flushBuffer();
        }

        *m_bufferPosition = theChar;

        ++m_bufferPosition;
        --m_bufferRemaining;
    }

    void
    write(
            const XalanDOMChar*     theChars,
            size_type               theLength)
    {
        for(size_type i = 0; i < theLength; ++i)
        {
            if (isUTF16HighSurrogate(theChars[i]) == false)
            {
                write(static_cast<XalanUnicodeChar>(theChars[i]));
            }
            else if (i + 1 >= theLength)
            {
                throwInvalidUTF16SurrogateException(
                    theChars[i], 
                    0,
                    getMemoryManager());
            }
            else 
            {
                write(
                    decodeUTF16SurrogatePair(
                        theChars[i],
                        theChars[i + 1],
                        getMemoryManager()));

                ++i;
            }
        }
    }

    size_type
    write(
            const XalanDOMChar  chars[],
            size_type           start,
            size_type           length)
    {
        const XalanDOMChar  ch = chars[start];

        if (isUTF16HighSurrogate(ch) == false)
        {
            write(static_cast<XalanUnicodeChar>(ch));
        }
        else if (start + 1 >= length)
        {
            throwInvalidUTF16SurrogateException(
                ch, 
                0,
                getMemoryManager());
        }
        else
        {
            write(
                decodeUTF16SurrogatePair(
                    ch,
                    chars[++start],
                    getMemoryManager()));
        }

        return start;
    }

    void
    writeSafe(
        const XalanDOMChar*     theChars,
        size_type               theLength)
    {
        for(size_type i = 0; i < theLength; ++i)
        {
            const XalanDOMChar  ch = theChars[i];

            if (isUTF16HighSurrogate(ch) == true)
            {
                if (i + 1 >= theLength)
                {
                    throwInvalidUTF16SurrogateException(
                        ch,
                        0,
                        getMemoryManager());
                }
                else 
                {
                    write(
                        decodeUTF16SurrogatePair(
                            ch,
                            theChars[i + 1],
                            getMemoryManager()));

                    ++i;
                }
            }
            else
            {
                write(static_cast<XalanUnicodeChar>(ch));
            }
        }
        
    }

    void
    write(const value_type*     theChars)
    {
        write(theChars, XalanDOMString::length(theChars));
    }

    void
    flushWriter()
    {
        m_writer.flush();
    }    

    void
    flushBuffer()
    {
        m_writer.write(m_buffer, 0, m_bufferPosition - m_buffer);
    
        m_bufferPosition = m_buffer;
        m_bufferRemaining = kBufferSize;
    }

private:

    void
    write(XalanUnicodeChar  theChar)
    {
        if (theChar <= 0x7F)
        {
            write(char(theChar));
        }
        else if (theChar <= 0x7FF)
        {
            if (m_bufferRemaining < 2)
            {
                flushBuffer();
            }

            *m_bufferPosition = leadingByteOf2(bits7to11(theChar));
            ++m_bufferPosition;
            *m_bufferPosition = trailingByte(bits1to6(theChar));
            ++m_bufferPosition;

            m_bufferRemaining -= 2;
        }
        else if (theChar <= 0xFFFF)
        {
            // We should never get a high or low surrogate here...
            assert(theChar < 0xD800 || theChar > 0xDBFF);
            assert(theChar < 0xDC00 || theChar > 0xDFFF);

            if (m_bufferRemaining < 3)
            {
                flushBuffer();
            }

            *m_bufferPosition = leadingByteOf3(bits13to16(theChar));
            ++m_bufferPosition;
            *m_bufferPosition = trailingByte(bits7to12(theChar));
            ++m_bufferPosition;
            *m_bufferPosition = trailingByte(bits1to6(theChar));
            ++m_bufferPosition;

            m_bufferRemaining -= 3;
        }
        else if (theChar <= 0x10FFFF)
        {
            if (m_bufferRemaining < 4)
            {
                flushBuffer();
            }

            *m_bufferPosition = leadingByteOf4(bits19to21(theChar));
            ++m_bufferPosition;
            *m_bufferPosition = trailingByte(bits13to18(theChar));
            ++m_bufferPosition;
            *m_bufferPosition = trailingByte(bits7to12(theChar));
            ++m_bufferPosition;
            *m_bufferPosition = trailingByte(bits1to6(theChar));
            ++m_bufferPosition;

            m_bufferRemaining -= 4;
        }
        else
        {
            throwInvalidCharacterException(theChar, getMemoryManager());
        }
    }

    enum
    {
        kBufferSize = 512       // The size of the buffer
    };


    // Data members...
    value_type      m_buffer[kBufferSize];

    value_type*     m_bufferPosition;

    size_type       m_bufferRemaining;
};



XALAN_CPP_NAMESPACE_END



#endif  // XALANUTF8WRITER_HEADER_GUARD_1357924680
