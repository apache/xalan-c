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
#if !defined(XALANUTF16WRITER_HEADER_GUARD_1357924680)
#define XALANUTF16WRITER_HEADER_GUARD_1357924680


#include <xalanc/XMLSupport/XalanFormatterWriter.hpp>


XALAN_CPP_NAMESPACE_BEGIN


class XalanUTF16Writer : public XalanFormatterWriter
{
public:

    typedef XalanDOMChar    value_type;
 

    XalanUTF16Writer(
                Writer&         writer,
                MemoryManager&  theMemoryManager);

    virtual
    ~XalanUTF16Writer()
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
                size_type           /*length*/,
                bool&               /* outsideCDATA */)
    {
        assert( chars != 0 );

        write(chars[start]);

        return start;
    }

    /**
     * Writes name chars , if not presentable, throws 
     */
    void writeNameChar(const XalanDOMChar*            data,
                       size_type                theLength)
    {
        write(data, theLength);
    }

    /**
     * Writes name chars , if not presentable, throws 
     */
    void writePIChars(const XalanDOMChar*       data,
                        size_type               theLength)
    {
        write(data, theLength);
    }

    /**
     * Writes name chars , if not presentable, throws 
     */
    void writeCommentChars(const XalanDOMChar*      data,
                           size_type                theLength)
    {
        write(data, theLength);
    }

    void
    safeWriteContent(
            const XalanDOMChar*     theChars,
            size_type               theLength)
    {
        write(theChars, theLength);
    }

    void
    write(
            const value_type*   theChars,
            size_type           theLength)
    {
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
  
    }

    void
    write(const XalanDOMString&     theChars)
    {
        write(theChars.c_str(), theChars.length());
    }

    void
    write(value_type    theChar)
    {
   
        if (m_bufferRemaining == 0)
        {
            flushBuffer();
        }
    
        *m_bufferPosition = theChar;
    
        ++m_bufferPosition;
        --m_bufferRemaining;
    }


    size_type
    write(
            const value_type    chars[],
            size_type           start,
            size_type           /*length*/)
    {
        write(chars[start]);
        
        return start;
    }

    void
    writeSafe(
            const XalanDOMChar*     theChars,
            size_type               theLength)
    {
        write(theChars, theLength);   
    }

    void
    write(const value_type* theChars)
    {
        write(theChars, XalanDOMString::length(theChars));
    }

    void
    flushBuffer()
    {
        m_writer.write(
            reinterpret_cast<const char*>(m_buffer),
            0,
            (m_bufferPosition - m_buffer) * sizeof m_buffer[0]);
    
        
        m_bufferPosition = m_buffer;
        m_bufferRemaining = kBufferSize;
    }
    
   
private:

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



#endif  // XALANUTF16WRITER_HEADER_GUARD_1357924680
