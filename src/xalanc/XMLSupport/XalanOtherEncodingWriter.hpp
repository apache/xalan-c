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
#if !defined(XALANUNICODESUBSETWRITER_HEADER_GUARD_1357924680)
#define XALANUNICODESUBSETWRITER_HEADER_GUARD_1357924680


#include <xalanc/XMLSupport/XalanFormatterWriter.hpp>



XALAN_CPP_NAMESPACE_BEGIN




template <class Predicate,
          class ConstantsType>
class XalanOtherEncodingWriter : public XalanFormatterWriter
{
public:

    typedef XalanOtherEncodingWriter<Predicate, ConstantsType>  ThisType;

    class WriteCharRef
    {
    public:

        WriteCharRef(ThisType&  writer) :
            m_writer(writer)
        {
        }

        void
        operator()(unsigned int value) const
        {
            m_writer.writeNumericCharacterReference(value);
        }

    private:

        ThisType&   m_writer;
    };

    class ThrowTranscodingException
    {
    public:

        ThrowTranscodingException(ThisType&     writer) :
            m_writer(writer)
        {
        }

        void
        operator()(unsigned int  value) const
        {
            m_writer.throwInvalidCharacterException(value, m_writer.getMemoryManager());
        }

    private:

        ThisType&   m_writer;
    };

    friend class WriteCharRef;
    friend class ThrowTranscodingException;

    typedef XalanDOMChar    value_type;

    XalanOtherEncodingWriter(
                Writer&         writer,
                MemoryManager&  theMemoryManager) :
        XalanFormatterWriter(
            writer,
            theMemoryManager),
        m_buffer(),
        m_bufferPosition(m_buffer),
        m_bufferRemaining(kBufferSize),
        m_predicate(writer.getStream()),
        m_constants(),
        m_charRefFunctor(*this),
        m_exceptionFunctor(*this)
    {
    }

    virtual
    ~XalanOtherEncodingWriter()
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

    /**
     * Writes CDATA chars , if not presentable, fixes it 
     * with addition CDATA sections
     */
    size_type
    writeCDATAChar(
                const XalanDOMChar  chars[],
                size_type           start,
                size_type           length,
                bool&               outsideCDATA)
    {
        assert(chars != 0 && length > 0 && start < length);

        const XalanDOMChar  theChar = chars[start];

        unsigned int value = theChar;

        size_type result = start;

        if (isUTF16HighSurrogate(theChar) == true)
        {
            if (start + 1 >= length)
            {
                throwInvalidUTF16SurrogateException(
                    theChar, 
                    0,
                    getMemoryManager());
            }
            else 
            {
                value = decodeUTF16SurrogatePair(theChar, chars[start+1],  getMemoryManager());

                ++result;
            }
        }

        if(m_predicate(value))
        {
            if (outsideCDATA == false)
            {
                // We have a representable char in the normal state,
                // so just print it.
                write(value);
            }
            else
            {
                // The previous character was a not representable.
                // Open the CDATA section again, print the character,
                // then change the flag.
                write(
                    m_constants.s_cdataOpenString,
                    m_constants.s_cdataOpenStringLength);

                write(value);

                outsideCDATA = false;
            }
        }
        else
        {
            if(outsideCDATA == false)
            {
                // we have a non-representable char in the normal state - 
                // close the CDATA section and print the value
                write(
                    m_constants.s_cdataCloseString,
                    m_constants.s_cdataCloseStringLength);

                writeNumericCharacterReference(value);

                outsideCDATA = true;   
            }
            else
            {
                writeNumericCharacterReference(value);
            }
        }        

        return result;
    }

    /**
     * Writes name characters.  If a character is not representable,
     * an exception is thrown.
     */
    void
    writeNameChar(
            const XalanDOMChar*     data,
            size_type               theLength)
    {
        for( size_type i = 0; i < theLength; ++i)
        { 
            i = write(data, i , theLength, m_exceptionFunctor); 
        }
    }

    /**
     * Writes PI characters.  If a character is not representable,
     * an exception is thrown.
     */
    void
    writePIChars(
            const XalanDOMChar*     data,
            size_type               theLength)
    {
        for( size_type i = 0; i < theLength; )
        { 
            i = write(data, i , theLength, m_exceptionFunctor); 
        }
    }

    /**
     * Writes comment characters.  If a character is not representable,
     * or must be written as a character reference for compatibility with
     * XML 1.1, an exception is thrown.
     */
    void
    writeCommentChars(
            const XalanDOMChar*     data,
            size_type               theLength)
    {
        for( size_type i = 0; i < theLength; )
        { 
            i = write(data, i , theLength, m_exceptionFunctor); 
        }
    }

    void
    write(
            const XalanDOMChar*     theChars,
            size_type               theLength)
    {
        for(size_type i = 0; i < theLength; ++i)
        {
            write(theChars[i]);
        }
    }

    void
    write(const XalanDOMString&     theChars)
    {
        write(theChars.c_str(), theChars.length());
    }

    /**
     * Writes writes a UTF-16 code unit that isn't 
     * part of the surrogate pair 
     */
    void
    write(XalanDOMChar    theChar)
    {
        assert(
            isUTF16HighSurrogate(theChar) == false &&
            isUTF16LowSurrogate(theChar) == false);

        if (m_bufferRemaining == 0)
        {
            flushBuffer();
        }

        if(m_predicate(theChar))
        {
            *m_bufferPosition = theChar;

            ++m_bufferPosition;
            --m_bufferRemaining;
        }
        else
        {
            writeNumericCharacterReference(theChar);
        }
    }

    size_type
    write(
            const XalanDOMChar  chars[],
            size_type           start,
            size_type           length)
    {

        return write(chars, start, length, m_charRefFunctor);
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
                    throwInvalidUTF16SurrogateException(ch, 0,  getMemoryManager());
                }
                else 
                {
                    unsigned int value = decodeUTF16SurrogatePair(ch, theChars[i+1],  getMemoryManager());

                    if(this->m_isPresentable(value))
                    {
                        write(value);
                    }
                    else
                    {
                        this->writeNumberedEntityReference(value);
                    }

                    ++i;
                }
            }
            else
            {
                write((unsigned int)ch);
            }
        }
    }

    void
    write(const XalanDOMChar*     theChars)
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

    /**
     * Writes a representable code point
     *
     * @param chars        Array of the characters for transcoding
     *    
     * @param start        Place int the array the transcoding should start
     *    
     * @param length       The length of the array
     *    
     * @param failureHandler  The functor handles the non-representable characters
     *    
     * @return              Place int the array of the next character
     */

    template <class TranscodingFailureFunctor>
    size_type
    write(
            const XalanDOMChar          chars[],
            size_type                   start,
            size_type                   length,
            TranscodingFailureFunctor&  failureHandler)
    {
        assert(chars != 0 && length > 0);
        assert(start <= length);

        size_type result = start;

        const XalanDOMChar  ch = chars[start];

        unsigned int value = ch;

        if (XalanFormatterWriter::isUTF16HighSurrogate(ch) == true)
        {
            if (start + 1 >= length)
            {
                throwInvalidUTF16SurrogateException(
                    ch, 
                    0,
                    getMemoryManager());
            }
            else 
            {
                value = decodeUTF16SurrogatePair(ch, chars[start+1],  getMemoryManager());

                ++result;
            }
        }

        if(m_predicate(value))
        {
            write(value);
        }
        else
        {
            failureHandler(value);
        }

        return result;
    }

    /**
     * Writes a representable code point
     *
     * @param theChar        UTF-32 code point . For passing it to the Xerces 
     *                       transcoder, we convert it back to UTF-16                         
     */
    void
    write(unsigned int  theChar)
    {
        // encode back UTF-32 into UTF-16 

        if( theChar > 0xFFFF )
        {
            if (m_bufferRemaining < 2)
            {
                flushBuffer();
            }

            *m_bufferPosition = (XalanDOMChar((theChar >> 10) + 0xD7C0));

            ++m_bufferPosition;

            *m_bufferPosition = (XalanDOMChar((theChar &  0x03FF) + 0xDC00));

            ++m_bufferPosition;

            m_bufferRemaining = m_bufferRemaining - size_type(2);
        }   
        else
        {
            if (m_bufferRemaining == 0)
            {
                flushBuffer();
            }

            *m_bufferPosition = XalanDOMChar(theChar);

            ++m_bufferPosition;
            --m_bufferRemaining;
        }  
    }

    void
    writeNumericCharacterReference(unsigned int     theNumber)
    {
        write(formatNumericCharacterReference(theNumber));
    }

    enum
    {
        kBufferSize = 512u  // The size of the buffer
    };


    // Data members...
    XalanDOMChar            m_buffer[kBufferSize];

    XalanDOMChar*           m_bufferPosition;

    size_type               m_bufferRemaining;

    const Predicate         m_predicate;

    const ConstantsType     m_constants;

    const WriteCharRef                  m_charRefFunctor;

    const ThrowTranscodingException     m_exceptionFunctor;
};



XALAN_CPP_NAMESPACE_END



#endif  // XALANUNICODESUBSETWRITER_HEADER_GUARD_1357924680
