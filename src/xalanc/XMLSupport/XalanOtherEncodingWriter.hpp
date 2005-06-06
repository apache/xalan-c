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




template <class PresentableCharacterPredicat,
          class ConstantsType>
class XalanOtherEncodingWriter : public XalanFormatterWriter
{
    typedef XalanOtherEncodingWriter<PresentableCharacterPredicat, ConstantsType> ThisType;

    class WriteCharRef
    {
    public:
        WriteCharRef(ThisType& writer) :
            m_wasCalled(false),
            m_writer(writer),
            m_stringBuffer(5, 0, m_writer.getMemoryManager())
        {
        }

        void
        operator()(unsigned int value)
        {
            m_wasCalled = true;

            m_writer.write(XalanDOMChar(XalanUnicode::charAmpersand));
            m_writer.write(XalanDOMChar(XalanUnicode::charNumberSign));
            
            clear(m_stringBuffer);

            m_writer.write(UnsignedLongToDOMString(value, m_stringBuffer));

            m_writer.write(XalanDOMChar(XalanUnicode::charSemicolon));
        }

        bool
        wasCalled()const
        {
            return m_wasCalled;
        }

    private:
        bool m_wasCalled;

        ThisType& m_writer;

        XalanDOMString m_stringBuffer;

    };

    class ThrowTranscodingException
    {
    public:
        ThrowTranscodingException(ThisType& writer) : 
            m_wasCalled(false),
            m_writer(writer)
        {
        }

        void
        operator()(unsigned int  value)
        {
            m_wasCalled = true;

            m_writer.throwInvalidCharacterException(value, m_writer.getMemoryManager());

        }

        bool
        wasCalled()const
        {
            return m_wasCalled;
        }

    private:
        bool m_wasCalled;

        ThisType& m_writer;

    };


public:
    typedef typename XalanDOMChar           value_type;
    typedef PresentableCharacterPredicat    Predicat;
    typedef XalanDOMString::size_type       size_type;
 
    XalanOtherEncodingWriter(
                Writer&         writer,
                MemoryManager&  theMemoryManager) :
        XalanFormatterWriter(
            writer,
            theMemoryManager),
        m_buffer(),
        m_bufferPosition(m_buffer),
        m_bufferRemaining(kBufferSize),
        m_stringBuffer(5, 0, theMemoryManager),
        m_isPresentable(writer.getStream()),
        m_constants()
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
                            const XalanDOMChar          chars[],
                            size_type                   start,
                            size_type                   length,
                            int&                        state)
    {
        assert(chars != 0 && length > 0 && start < length);

        // enum for a cheezy little state machine.
        enum eState { eNormalState = 0, eOutOfCDATA = 1};

        const XalanDOMChar          theChar = chars[start];

        unsigned int value = 0;

        size_type result = start;

        if (XalanFormatterWriter::isUTF16HighSurrogate(theChar) == false)
        {
            value = theChar;
        }
        else
        {
            if (start + 1 >= length)
            {
                XalanFormatterWriter::throwInvalidUTF16SurrogateException(
                    theChar, 
                    0,
                    getMemoryManager());
            }
            else 
            {
                value = XalanFormatterWriter::decodeUTF16SurrogatePair(theChar, chars[start+1],  getMemoryManager());

                ++result;
            }
        }

        WriteCharRef   charRefsWriter(*this);

        if(m_isPresentable(value))
        {
            if( eNormalState == state)
            {
                // we have a presentable char in the normal state - just print it
                write(value);
            }
            else
            {
                // previose char was a non-presentable one . Open CDATA section again,
                // change the state to normal and print the char
                write(
                    m_constants.s_cdataOpenString,
                    m_constants.s_cdataOpenStringLength);

                write(value);

                state = eNormalState;
            }
        }
        else // not presentable chracter
        {
            if( eNormalState == state)
            {
                // we have a non presentable char in the normal state - 
                // close the CDATA section and print the value
                write(
                    m_constants.s_cdataCloseString,
                    m_constants.s_cdataCloseStringLength);

                charRefsWriter( value );

                state = eOutOfCDATA;   
            }
            else
            {
                charRefsWriter( value );
            }
        }        

        return result;
    }

    /**
     * Writes name chars , if not presentable, throws 
     */
    void writeNameChar(const XalanDOMChar*            data,
                       size_type                theLength)
    {
        ThrowTranscodingException functor(*this);

        for( size_type i = 0; i < theLength; ++i)
        { 
            i = write(data, i , theLength, functor); 
        }
    }

    /**
     * Writes name chars , if not presentable, throws 
     */
    void writePIChars(const XalanDOMChar*       data,
                        size_type               theLength)
    {
        ThrowTranscodingException functor(*this);

        for( size_type i = 0; i < theLength; )
        { 
            i = write(data, i , theLength, functor); 
        }
    }

    /**
     * Writes name chars , if not presentable, throws 
     */
    void writeCommentChars(const XalanDOMChar*      data,
                           size_type                theLength)
    {
        ThrowTranscodingException functor(*this);

        for( size_type i = 0; i < theLength; )
        { 
            i = write(data, i , theLength, functor); 
        }
    }

    void
    write(
            const XalanDOMChar*     theChars,
            size_type               theLength)
    {

        for(XalanDOMString::size_type i = 0; i < theLength; ++i)
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
     * Writes writes a code point that isn't a part of the surrogate pair 
     */
    void
    write(XalanDOMChar    theChar)
    {
  
        if (m_bufferRemaining == 0)
        {
            flushBuffer();
        }
    
        if(m_isPresentable(theChar))
        {
            *m_bufferPosition = theChar;
    
            ++m_bufferPosition;
            --m_bufferRemaining;
        }
        else
        {
            writeNumberedEntityReference(theChar);
        }
    }


    size_type
    write(
            const XalanDOMChar          chars[],
            XalanDOMString::size_type   start,
            XalanDOMString::size_type   length)
    {

        WriteCharRef functor(*this);

        return write(chars, start, length, functor);
    }

    void
    writeSafe(
        const XalanDOMChar*         theChars,
        XalanDOMString::size_type   theLength)
    {
        XalanDOMChar ch = 0;

        for(size_type i = 0; i < theLength; ++i)
        {
            ch = theChars[i];

            if (XalanFormatterWriter::isUTF16HighSurrogate(ch) == true)
            {
                if (i + 1 >= theLength)
                {
                    XalanFormatterWriter::throwInvalidUTF16SurrogateException(ch, 0,  getMemoryManager());
                }
                else 
                {
                    unsigned int value = XalanFormatterWriter::decodeUTF16SurrogatePair(ch, theChars[i+1],  getMemoryManager());
                    
                    if(m_isPresentable(value))
                    {
                        write(value);
                    }
                    else
                    {
                        writeNumberedEntityReference(value);
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
        m_writer.write(m_buffer, 0, XalanDOMChar(m_bufferPosition - m_buffer));
    
        m_bufferPosition = m_buffer;
        m_bufferRemaining = kBufferSize;
    }
    
private:

    /**
     * Writes a presentable code point
     *
     * @param chars        Array of the characters for transcoding
     *    
     * @param start        Place int the array the transcoding should start
     *    
     * @param length       The length of the array
     *    
     * @param failreHandeler  The functor handeles the non-representable characters
     *    
     * @return              Place int the array of the next character
     */

    template <class TranscodingFailureFunctor>
    size_type
    write(
            const XalanDOMChar          chars[],
            size_type                   start,
            size_type                   length,
            TranscodingFailureFunctor&  failreHandeler)
    {
        assert( chars != 0 && length > 0 );
        assert( start <= length);
        
        size_type result = start;

        XalanDOMChar ch = chars[start];

        unsigned int value = 0;

        if (XalanFormatterWriter::isUTF16HighSurrogate(ch) == false)
        {
            value = ch;
        }
        else
        {
            if (start + 1 >= length)
            {
                XalanFormatterWriter::throwInvalidUTF16SurrogateException(
                    ch, 
                    0,
                    getMemoryManager());
            }
            else 
            {
                value = XalanFormatterWriter::decodeUTF16SurrogatePair(ch, chars[start+1],  getMemoryManager());

                ++result;
            }
        }

        if(m_isPresentable(value))
        {
            write(value);
        }
        else
        {
            failreHandeler(value);
        }

        return result;
    }

    /**
     * Writes a presentable code point
     *
     * @param theChar        UTF-32 code point . For passing it to the Xerces 
     *                       transcoder, we convert it back to UTF-16                         
     */

    void
    writeNumberedEntityReference(unsigned long  theNumber)
    {
        m_writer.write(XalanDOMChar(XalanUnicode::charAmpersand));
        m_writer.write(XalanDOMChar(XalanUnicode::charNumberSign));

        m_writer.write(UnsignedLongToDOMString(theNumber, m_stringBuffer));
        clear(m_stringBuffer);

        m_writer.write(XalanDOMChar(XalanUnicode::charSemicolon));
    }
 
    /**
     * Writes a presentable code point
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

    enum
    {
        kBufferSize = 512       // The size of the buffer
    };


    // Data members...
    XalanDOMChar                m_buffer[kBufferSize];

    XalanDOMChar*               m_bufferPosition;

    size_type                   m_bufferRemaining;
    
    XalanDOMString              m_stringBuffer;
    
    Predicat                    m_isPresentable;

    ConstantsType               m_constants;          
    
    
};



XALAN_CPP_NAMESPACE_END



#endif  // XALANUNICODESUBSETWRITER_HEADER_GUARD_1357924680
