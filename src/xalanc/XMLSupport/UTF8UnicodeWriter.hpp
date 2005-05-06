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
#if !defined(UTF8UNICODEWRITER_HEADER_GUARD_1357924680)
#define UTF8UNICODEWRITER_HEADER_GUARD_1357924680




// Base include file.  Must be first.
#include <xalanc/XMLSupport/XMLSupportDefinitions.hpp>

#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/Writer.hpp>


#include <xalanc/XMLSupport/FormatterToXMLBase.hpp>



XALAN_CPP_NAMESPACE_BEGIN



inline char
bits19to21(unsigned int	theChar)
{
	return char((theChar >> 18) & 0x7);
}



inline char
bits13to18(unsigned int	theChar)
{
	return char((theChar >> 12) & 0x3F);
}



inline char
bits13to16(unsigned int	theChar)
{
	return char((theChar >> 12) & 0xF);
}



inline char
bits7to12(unsigned int	theChar)
{
	return char((theChar >> 6) & 0x3f);
}



inline char
bits7to11(unsigned int	theChar)
{
	return char((theChar >> 6) & 0x1f);
}



inline char
bits1to6(unsigned int	theChar)
{
	return char(theChar & 0x3f);
}



inline char
leadingByteOf2(char		theBits)
{
	return char(0xC0 + theBits);
}



inline char
leadingByteOf3(char		theBits)
{
	return char(0xE0 + theBits);
}



inline char
leadingByteOf4(char		theBits)
{
	return char(0xF0 + theBits);
}



inline char
trailingByte(char	theBits)
{
	return char(0x80 + theBits);
}



class XalanUTF8Writer
{
  public:
    typedef char                        value_type;
    typedef XalanDOMString::size_type   size_type;
 
  
    XalanUTF8Writer(  Writer&	            writer , 
                        MemoryManagerType&  theMemoryManager ) :
        m_writer(&writer),
        m_memoryManager(theMemoryManager),
    	m_buffer(),
	    m_bufferPosition(m_buffer),
	    m_bufferRemaining(kBufferSize)
    {
    }
  
    MemoryManagerType&
    getMemoryManager()
    {
        return m_memoryManager;
    }

    ~XalanUTF8Writer()
    {
    }
  
	void
	safeWriteContent(
			const XalanDOMChar*			theChars,
			XalanDOMString::size_type	theLength)
    {
    	for(size_type i = 0; i < theLength; ++i)
    	{ 
    		write(value_type(theChars[i]));
    	}
    }

	void
	write(
			const XalanDOMChar*			theChars,
			XalanDOMString::size_type	theLength)
    {
    #if defined(NDEBUG)
    	if (theLength > sizeof(m_buffer))
    	{
    		flushBuffer();
    
    		m_writer->write(theChars, 0, theLength);
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
    	for(XalanDOMString::size_type i = 0; i < theLength; ++i)
    	{
    		write(theChars[i]);
    	}
    #endif
    }

	void
	write(const XalanDOMChar*	theChars)
	{
		write(theChars, XalanDOMString::length(theChars));
	}

	void
	write(XalanDOMChar	theChar)
	{
		write((unsigned int)theChar);
	}

	void
	write(unsigned int	theChar)
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
//    		throwInvalidCharacterException(theChar, getMemoryManager());
    	}
    }

	void
	write(const XalanDOMString&		theChars)
	{
		write(theChars.c_str(), theChars.length());
	}

	void
	write(value_type	theChar)
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
			const value_type*			theChars,
			XalanDOMString::size_type	theLength);

	void
	write(const value_type*	theChars)
	{
		write(theChars, XalanDOMString::length(theChars));
	}

    void
    writeNumberedEntityReference(unsigned long	theNumber);
    
    void
    flushWriter()
    {
    	m_writer->flush();
    }    
    
    void
    flushBuffer()
    {
    	m_writer->write(m_buffer, 0, m_bufferPosition - m_buffer);
    
    	m_bufferPosition = m_buffer;
    	m_bufferRemaining = kBufferSize;
    }
    
	/**
	 * Output a line break.
	 */
	void
	outputNewline();    
private:

    //static void
    //throwInvalidCharacterException( unsigned int		ch,
    //                                MemoryManagerType&  theManager)
    //{
    //    XalanDOMString	theMessage(theManager);
    //    XalanDOMString	theBuffer(theManager);  

    //    XalanMessageLoader::getMessage(XalanMessages::InvalidCharDetected_1Param, theMessage, UnsignedLongToHexDOMString(ch, theBuffer));

    //    throw SAXException(c_wstr(theMessage),&theManager);
    //}

	enum
	{
		kBufferSize = 512		// The size of the buffer
	};

	/** 
	 * The writer.
	 */
	Writer*					m_writer;

    MemoryManagerType&      m_memoryManager;

	value_type			    m_buffer[kBufferSize];

	value_type*			    m_bufferPosition;

	XalanDOMString::size_type	m_bufferRemaining;
	
};

XALAN_CPP_NAMESPACE_END



#endif	// UTF8UNICODEWRITER_HEADER_GUARD_1357924680
