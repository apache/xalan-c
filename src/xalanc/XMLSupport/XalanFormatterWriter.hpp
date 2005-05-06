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
#if !defined(XALANFORMATTERWRITER_HEADER_GUARD_1357924680)
#define XALANFORMATTERWRITER_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XMLSupport/XMLSupportDefinitions.hpp>

#include <xercesc/sax/SAXException.hpp>

#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/Writer.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>


XALAN_CPP_NAMESPACE_BEGIN





class XalanFormatterWriter
{
  public:
 
  
    XalanFormatterWriter(  Writer&	            writer , 
                        MemoryManagerType&  theMemoryManager ) :
        m_writer(&writer),
        m_memoryManager(theMemoryManager)
    {
    }
  
    MemoryManagerType&
    getMemoryManager()
    {
        return m_memoryManager;
    }

    ~XalanFormatterWriter()
    {
    }
 
    Writer*
	getWriter() const
    {
        return m_writer;
    }

    XalanOutputStream*
    getStream()
    {
        assert(m_writer != 0);

	    return m_writer->getStream();
    }



    const XalanOutputStream*
    getStream() const
    {
        assert(m_writer != 0);

	    return m_writer->getStream();
    }
    void
    flushWriter()
    {
    	m_writer->flush();
    }    
       
protected:

	static bool
	isUTF16HighSurrogate(XalanDOMChar	theChar)
	{
		return 0xD800u <= theChar && theChar <= 0xDBFFu ? true : false;
	}

	static bool
	isUTF16LowSurrogate(XalanDOMChar	theChar)
	{
		return 0xDC00u <= theChar && theChar <= 0xDFFFu ? true : false;
	}

    static unsigned int
    decodeUTF16SurrogatePair(
			    XalanDOMChar	theHighSurrogate,
			    XalanDOMChar	theLowSurrogate,
                MemoryManagerType& theManager)
    {
	    assert(isUTF16HighSurrogate(theHighSurrogate) == true);

	    if (isUTF16LowSurrogate(theLowSurrogate) == false)
	    {
            throwInvalidUTF16SurrogateException(theHighSurrogate, theLowSurrogate, theManager);
	    }

	    return ((theHighSurrogate - 0xD800u) << 10) + theLowSurrogate - 0xDC00u + 0x00010000u;
    }

    static void
    throwInvalidCharacterException( unsigned int		ch,
                                    MemoryManagerType&  theManager)
    {
        XalanDOMString	theMessage(theManager);
        XalanDOMString	theBuffer(theManager);  

        XalanMessageLoader::getMessage(
                    theMessage,
                    XalanMessages::InvalidScalar_1Param, 
                    UnsignedLongToHexDOMString(ch, theBuffer));

        XALAN_USING_XERCES(SAXException)

        throw SAXException(c_wstr(theMessage),&theManager);
    }

    static void
    throwInvalidUTF16SurrogateException(
			    XalanDOMChar	ch,
			    XalanDOMChar	next,
                MemoryManagerType& theManager)
    {

	    XalanDOMString chStr(theManager); 

        XalanDOMString nextStr(theManager); 

        UnsignedLongToHexDOMString(ch, chStr);

        UnsignedLongToHexDOMString(next, nextStr);

	    XalanDOMString	theMessage(theManager);

        XalanMessageLoader::getMessage(
                    theMessage,
                    XalanMessages::InvalidSurrogatePair_2Param,
                    theMessage,
                    chStr,
                    nextStr);

        XALAN_USING_XERCES(SAXException)

	    throw SAXException(c_wstr(theMessage),&theManager);
    }


	/** 
	 * The writer.
	 */
	Writer*					m_writer;

    MemoryManagerType&      m_memoryManager;

	
};

XALAN_CPP_NAMESPACE_END



#endif	// XALANFORMATTERWRITER_HEADER_GUARD_1357924680
