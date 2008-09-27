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
#if !defined(XALANFORMATTERWRITER_HEADER_GUARD_1357924680)
#define XALANFORMATTERWRITER_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XMLSupport/XMLSupportDefinitions.hpp>

#include <xercesc/sax/SAXException.hpp>

#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/FormatterListener.hpp>
#include <xalanc/PlatformSupport/Writer.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>
#include <xalanc/PlatformSupport/XalanOutputStream.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XALAN_USING_XERCES(MemoryManager)



class XalanFormatterWriter
{
public:

    typedef FormatterListener::size_type    size_type;


    template <class WriterType>
    class NewLineWriterFunctor
    {
    public:

        typedef WriterType  writer_type;

        NewLineWriterFunctor(WriterType& writer) :
          m_writer(writer),
          m_newlineString(0),
          m_newlineStringLength(0)
        {
            XalanOutputStream* stream = writer.getStream();

            if(stream != 0)
            {
                m_newlineString = stream->getNewlineString();
            }
            else
            {
                m_newlineString = XalanOutputStream::defaultNewlineString();
            }

            assert(m_newlineString != 0);

            m_newlineStringLength = length(m_newlineString);
        }

        void
        operator()() 
        {
            assert(m_newlineString != 0 && length(m_newlineString) == m_newlineStringLength);

            m_writer.write(m_newlineString, m_newlineStringLength);
        }

    private:

        WriterType&     m_writer;

        /**
        * The string of characters that represents the newline
        */
        const XalanDOMChar*         m_newlineString;

        /**
        * The length of the the string of characters that represents the newline
        */
        size_type   m_newlineStringLength;
    };

    template<class WriterType>
    class WhiteSpaceWriterFunctor
    {
        typedef typename WriterType::value_type value_type;

    public:
        typedef WriterType                  writer_type;

        WhiteSpaceWriterFunctor(WriterType& writer) :
          m_writer(writer)
          {
          }

        void
        operator()(size_type count) 
        {
            for ( size_type i = 0 ; i < count ; i++ ) 
            {
                m_writer.write(value_type(XalanUnicode::charSpace));
            }
        }

    private:

        WriterType& m_writer;
    };

    class CommonRepresentableCharFunctor
    {
    public:

        CommonRepresentableCharFunctor(const XalanOutputStream*     stream) :
            m_stream(stream)
        {
            assert(stream != 0);
        }

        bool
        operator()(XalanUnicodeChar     theChar) const
        {
            bool result = true;

            if (m_stream != 0)
            {
                result = m_stream->canTranscodeTo(theChar);
            }
            
            return result;
        }

    private:

        const XalanOutputStream* const  m_stream;
    };

public:

    XalanFormatterWriter(
                Writer&         theWriter, 
                MemoryManager&  theMemoryManager) :
        m_writer(theWriter),
        m_memoryManager(theMemoryManager),
        m_stringBuffer(5, 0, theMemoryManager)
    {
        const XalanOutputStream* const  theStream =
            theWriter.getStream();

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
    }

    MemoryManager&
    getMemoryManager()
    {
        return m_memoryManager;
    }

    virtual
    ~XalanFormatterWriter()
    {
    }
 
    Writer*
    getWriter() const
    {
        return &m_writer;
    }

    XalanOutputStream*
    getStream()
    {
        return m_writer.getStream();
    }

    const XalanOutputStream*
    getStream() const
    {
        return m_writer.getStream();
    }

    void
    flushWriter()
    {
        m_writer.flush();
    }    


    static bool
    isUTF16HighSurrogate(XalanDOMChar   theChar)
    {
        return 0xD800u <= theChar && theChar <= 0xDBFFu ? true : false;
    }

    static bool
    isUTF16LowSurrogate(XalanDOMChar    theChar)
    {
        return 0xDC00u <= theChar && theChar <= 0xDFFFu ? true : false;
    }

    static XalanUnicodeChar
    decodeUTF16SurrogatePair(
                XalanDOMChar    theHighSurrogate,
                XalanDOMChar    theLowSurrogate,
                MemoryManager&  theManager)
    {
        assert(isUTF16HighSurrogate(theHighSurrogate) == true);

        if (isUTF16LowSurrogate(theLowSurrogate) == false)
        {
            throwInvalidUTF16SurrogateException(theHighSurrogate, theLowSurrogate, theManager);
        }

        return ((theHighSurrogate - 0xD800u) << 10) + theLowSurrogate - 0xDC00u + 0x00010000u;
    }

    static void
    throwInvalidCharacterException(
                XalanUnicodeChar    ch,
                MemoryManager&      theManager)
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
    throwUnrepresentableCharacterException(
                XalanUnicodeChar    ch,
                MemoryManager&      theManager)
    {
        XalanDOMString  theBuffer(theManager);  

        const XalanOutputStream* const  theStream =
            m_writer.getStream();

        throw XalanTranscodingServices::UnrepresentableCharacterException(
                    ch,
                    theStream->getOutputEncoding(),
                    theBuffer);
    }

    static void
    throwInvalidUTF16SurrogateException(
                XalanDOMChar        ch,
                XalanDOMChar        next,
                MemoryManager&  theManager)
    {

        XalanDOMString  chStr(theManager); 

        XalanDOMString  nextStr(theManager); 

        NumberToHexDOMString(ch, chStr);

        NumberToHexDOMString(next, nextStr);

        XalanDOMString  theMessage(theManager);

        XalanMessageLoader::getMessage(
                    theMessage,
                    XalanMessages::InvalidSurrogatePair_2Param,
                    theMessage,
                    chStr,
                    nextStr);

        XALAN_USING_XERCES(SAXException)

        throw SAXException(theMessage.c_str(),&theManager);
    }

protected:

    /** 
     * The writer.
     */
    Writer&                     m_writer;

    /**
     * The MemoryManager instance to use for any dynamically-
     * allocated memory.
     */
    MemoryManager&              m_memoryManager;

    XalanDOMString          m_stringBuffer;

    /**
     * The string of characters that represents the newline
     */
    const XalanDOMChar*         m_newlineString;

    /**
     * The length of the the string of characters that represents the newline
     */
    size_type   m_newlineStringLength;

    /**
     * Format a code point as a numeric character reference.
     *
     * @param theChar A Unicode code point.
     */
    const XalanDOMString&
    formatNumericCharacterReference(XalanUnicodeChar    theChar)
    {
        m_stringBuffer.clear();

        m_stringBuffer.push_back(XalanDOMChar(XalanUnicode::charAmpersand));
        m_stringBuffer.push_back(XalanDOMChar(XalanUnicode::charNumberSign));

        NumberToDOMString(theChar, m_stringBuffer);

        m_stringBuffer.push_back(XalanDOMChar(XalanUnicode::charSemicolon));

        return m_stringBuffer;
    }

private:

    // These are not implemented.
    XalanFormatterWriter();

    XalanFormatterWriter&
    operator=(const XalanFormatterWriter&);
};



XALAN_CPP_NAMESPACE_END



#endif  // XALANFORMATTERWRITER_HEADER_GUARD_1357924680
