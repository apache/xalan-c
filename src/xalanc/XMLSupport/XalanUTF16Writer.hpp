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
    typedef XalanDOMChar                value_type;
    typedef XalanDOMString::size_type   size_type;
 
	/**
	 * Perform static initialization.  See class XMLSupportInit.
	 */
	static void
	initialize(MemoryManagerType& theManager);

	static void
	terminate();

    XalanUTF16Writer(  Writer&	            writer , 
                        MemoryManagerType&  theMemoryManager ) :
        XalanFormatterWriter( writer, theMemoryManager),
    	m_buffer(),
	    m_bufferPosition(m_buffer),
	    m_bufferRemaining(kBufferSize)
    {
    }

    ~XalanUTF16Writer()
    {
    }

	const XalanDOMString&
	getEncoding() const
    {
	    return s_utf16String;
    }
 
	void
	safeWriteContent(
			const XalanDOMChar*			theChars,
			XalanDOMString::size_type	theLength)
    {
    	write( theChars, theLength);
    }

	void
	write(
			const value_type*			theChars,
			XalanDOMString::size_type	theLength)
    {
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
  
    }

	void
	write(const XalanDOMString&		theChars)
	{
		write(theChars.c_str(), theChars.length());
	}

	void
	write(value_type	theChar)
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
			value_type				    ch,
			const value_type			/*chars*/[],
			XalanDOMString::size_type	start,
			XalanDOMString::size_type	/*length*/)
    {
		write(ch);
		
	    return start;
    }

    void
    writeSafe(
        const XalanDOMChar*         theChars,
        XalanDOMString::size_type   theLength)
    {
        write(theChars, theLength);   
    }

	void
	write(const value_type*	theChars)
	{
		write(theChars, XalanDOMString::length(theChars));
	}

    void
    flushBuffer()
    {
    	//m_writer->write(m_buffer, size_type(0), size_type(m_bufferPosition - m_buffer));
    
    	//m_bufferPosition = m_buffer;
    	//m_bufferRemaining = kBufferSize;
	    m_writer->write(
            reinterpret_cast<const char*>(m_buffer),
            0,
            (m_bufferPosition - m_buffer) * sizeof m_buffer[0]);
    
        
	    m_bufferPosition = m_buffer;
	    m_bufferRemaining = kBufferSize;
    }
    
   
private:


	enum
	{
		kBufferSize = 512		// The size of the buffer
	};

	/** 
	 * The data.
	 */

	value_type			    m_buffer[kBufferSize];

	value_type*			    m_bufferPosition;

	XalanDOMString::size_type	m_bufferRemaining;

	/**
	 * The string "UTF-16".
	 */
	static const XalanDOMString&	s_utf16String;

};


XALAN_CPP_NAMESPACE_END



#endif	// XALANUTF16WRITER_HEADER_GUARD_1357924680
