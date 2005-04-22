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
#if !defined(XALANOUTPUTSTREAM_HEADER_GUARD_1357924680)
#define XALANOUTPUTSTREAM_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



#include <xalanc/Include/XalanVector.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/PlatformSupport/XalanTranscodingServices.hpp>
#include <xalanc/PlatformSupport/XSLException.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XalanOutputTranscoder;



class XALAN_PLATFORMSUPPORT_EXPORT XalanOutputStream
{
public :

	enum { eDefaultBufferSize = 512u, eDefaultTranscoderBlockSize = 1024u };

	typedef XalanVector<XalanDOMChar>		BufferType;
	typedef XalanVector<char>				TranscodeVectorType;
	typedef XalanTranscodingServices::size_type	        size_type;
    typedef XalanTranscodingServices::UnicodeCharType   UnicodeCharType;

	/**
	 * Constructor.
	 *
	 * @param theBufferSize the size of the transcoding buffer
	 * @param theTranscoderBlockSize the size of the block used by the transcoder
	 * @param fThrowTranscodeException If true, an error transcoding will result in an exception being thrown.
	 */
	explicit
	XalanOutputStream(
            MemoryManagerType&  theManager,
			size_type	        theBufferSize = eDefaultBufferSize,
			size_type	        theTranscoderBlockSize = eDefaultTranscoderBlockSize,
			bool		        fThrowTranscodeException = true);

	virtual
	~XalanOutputStream();

    MemoryManagerType& 
    getMemoryManager()
    {
        return m_buffer.getMemoryManager();
    }

	static const XalanDOMChar*
	defaultNewlineString()
	{
#if defined(XALAN_NEWLINE_IS_CRLF)
		return s_nlCRString;
#else
		return s_nlString;
#endif
	}

	/**
	 * Write the appropriate newline character(s) to the stream.
	 */
	virtual void
	newline();

	/**
	 * Get the string which is appropriate for inserting a line feed in the stream.
	 */
	virtual const XalanDOMChar*
	getNewlineString() const;

	/**
	 * Flush the stream's transcoding buffer, but do not request
	 * the implementation class to flush its buffer.
	 * .
	 */
	void
	flushBuffer();

	/**
	 * Flush the stream's buffer.
	 */
	void
	flush()
	{
		flushBuffer();

		doFlush();
	}

	/**
	 * Write a character to the output stream.	The character
	 * will not be transcoded.
	 *
	 * @param theChar		the character to write
	 */
	void
	write(char	theChar)
	{
		write(&theChar, 1);
	}

	/**
	 * Write a wide character to the output stream.  The character
	 * will be transcoded, if an output encoding is specified.
	 *
	 * @param theChar		the character to write
	 */
	void
	write(XalanDOMChar	theChar)
	{
		assert(m_bufferSize > 0);

		if (m_buffer.size() == m_bufferSize)
		{
			flushBuffer();
		}

		m_buffer.push_back(theChar);
	}

	/**
	 * Write a null-terminated string to the output file.  The character
	 * will not be transcoded.  The caller is responsible for making sure the
	 * buffer is flushed before calling this member function.
	 *
	 * @param theBuffer 	  character buffer to write
	 */
	void
	write(const char*	theBuffer)
	{
		assert(theBuffer != 0);
		assert(m_buffer.empty() == true);

		write(theBuffer, length(theBuffer));
	}

	/**
	 * Write a null-terminated wide string to the output file.	The string
	 * will be transcoded, if an output encoding is specified.
	 *
	 * @param theBuffer 	  character buffer to write
	 */
	void
	write(const XalanDOMChar*	theBuffer)
	{
		write(theBuffer, length(theBuffer));
	}

	/**
	 * Write a specified number of characters to the output stream.  The string
	 * will not be transcoded.  The caller is responsible for making sure the
	 * buffer is flushed before calling this member function.
	 *
	 * @param theBuffer 	  character buffer to write
	 * @param theBufferLength number of characters to write
	 */
	void
	write(
			const char* 	theBuffer,
			size_type		theBufferLength)
	{
		assert(theBuffer != 0);
		assert(m_buffer.empty() == true);

		writeData(theBuffer,
				  theBufferLength);
	}

	/**
	 * Write a specified number of characters to the output stream.  The string
	 * will be transcoded, if an output encoding is specified.
	 *
	 * @param theBuffer 	  character buffer to write
	 * @param theBufferLength number of characters to write
	 */
	void
	write(
			const XalanDOMChar* 	theBuffer,
			size_type				theBufferLength);

	/**
	 * Get the output encoding for the stream.
	 *
	 * @return The encoding name
	 */
	const XalanDOMString&
	getOutputEncoding() const
	{
		return m_encoding;
	}

	/**
	 * Set the output encoding for the stream.
	 *
	 * @param theEncoding The encoding name
	 */
	void
	setOutputEncoding(const XalanDOMString& 	theEncoding);

	/**
	 * Determine if a given value can be represented in
	 * the output encoding.
	 *
	 * @return true if the value can be represented, and false if not.
	 */
	bool
	canTranscodeTo(UnicodeCharType 	theChar) const;


	const XalanOutputTranscoder*
	getTranscoder() const
	{
		return m_transcoder;
	}

	/**
	 * Set the flag that indicates whether a transcoding
	 * error should throw an exception.  The default is
	 * to throw an exception.  If this flag is false, and
	 * and an error occurs transcoding, then data will
	 * likely be lost.
	 *
	 * @return the value of the flag.
	 */
	bool
	getThrowTranscodeException() const
	{
		return m_throwTranscodeException;
	}

	/**
	 * Set the flag that indicates whether a transcoding
	 * error should throw an exception.  The default is
	 * to throw an exception.  If this flag is false, and
	 * and an error occurs transcoding, then data will
	 * likely be lost.
	 *
	 * @param the new value of the flag.
	 */
	void
	setThrowTranscodeException(bool     flag)
	{
		m_throwTranscodeException = flag;
	}

	/**
	 * Set the size of the output buffer.
	 *
	 * @param theBufferSize The buffer size.
	 */
	void
	setBufferSize(size_type     theBufferSize);


	class XALAN_PLATFORMSUPPORT_EXPORT XalanOutputStreamException : public XSLException
	{
	public:

		XalanOutputStreamException(
			const XalanDOMString&	theMessage,
            MemoryManagerType&      theManager);

        XalanOutputStreamException( const XalanOutputStreamException& other):
            XSLException(other)
            {
            }

		virtual
		~XalanOutputStreamException();

		virtual const XalanDOMChar*
		getType() const
		{
			return m_type;
		}

	private:
		static const XalanDOMChar	m_type[];

	};

	class XALAN_PLATFORMSUPPORT_EXPORT UnknownEncodingException : public XalanOutputStreamException
	{
	public:

		explicit
		UnknownEncodingException(XalanDOMString& theBuffer);

		virtual
		~UnknownEncodingException();

		virtual const XalanDOMChar*
		getType() const
		{
			return m_type;
		}

	private:
		static const XalanDOMChar	m_type[];

	};

	class XALAN_PLATFORMSUPPORT_EXPORT UnsupportedEncodingException : public XalanOutputStreamException
	{
	public:

		UnsupportedEncodingException(const XalanDOMString&	theEncoding,
                                        XalanDOMString& theBuffer);

        UnsupportedEncodingException(const UnsupportedEncodingException& other) :
        XalanOutputStreamException(other),
        m_encoding(other.getEncoding(),(const_cast<XalanDOMString*>(&(other.m_encoding)))->getMemoryManager())
        {
        }

		virtual
		~UnsupportedEncodingException();

		const XalanDOMString&
		getEncoding() const
		{
			return m_encoding;
		}


		virtual const XalanDOMChar*
		getType() const
		{
			return m_type;
		}
	private:

		static const XalanDOMChar	m_type[];

		const XalanDOMString	m_encoding;
	};

	class XALAN_PLATFORMSUPPORT_EXPORT TranscoderInternalFailureException : public XalanOutputStreamException
	{
	public:

		TranscoderInternalFailureException(const XalanDOMString&	theEncoding,
                                            XalanDOMString& theBuffer);

        TranscoderInternalFailureException(const TranscoderInternalFailureException& other) :
            XalanOutputStreamException(other),
            m_encoding(other.getEncoding(),(const_cast<XalanDOMString*>(&(other.m_encoding)))->getMemoryManager())
            {
            }

		virtual
		~TranscoderInternalFailureException();

		const XalanDOMString&
		getEncoding() const
		{
			return m_encoding;
		}

	private:

		const XalanDOMString	m_encoding;
	};

	class XALAN_PLATFORMSUPPORT_EXPORT TranscodingException : public XalanOutputStreamException
	{
	public:

		explicit
		TranscodingException(XalanDOMString& theBuffer);

        TranscodingException(const TranscodingException& other) :
        XalanOutputStreamException(other)
        {
        }

		virtual
		~TranscodingException();
	};

protected:

	/**
	 * Transcode a wide string.
	 *
	 * @param theBuffer The string to transcode.
	 * @param theBufferLength The length of the string.
	 * @param theDestination The destination vector.
	 */
	void
	transcode(
			const XalanDOMChar* 	theBuffer,
			size_type				theBufferLength,
			TranscodeVectorType&	theDestination);

	/**
	 * Write the data in the buffer
	 *
	 * @param theBuffer The data to write
	 * @param theBufferLength The length of theBuffer.
	 */
	virtual void
	writeData(
			const char* 	    theBuffer,
			size_type		    theBufferLength) = 0;

	/**
	 * Flush the stream.
	 */
	virtual void
	doFlush() = 0;

	static const XalanDOMChar				s_nlString[];
	static const XalanDOMChar				s_nlCRString[];

	static const XalanDOMString::size_type	s_nlStringLength;
	static const XalanDOMString::size_type	s_nlCRStringLength;

private:

	// These are not implemented...
	XalanOutputStream(const XalanOutputStream&);

	XalanOutputStream&
	operator=(const XalanOutputStream&);

	bool
	operator==(const XalanOutputStream&) const;

	void
	doWrite(
			const XalanDOMChar* 	theBuffer,
			size_type				theBufferLength);


	const size_type	        m_transcoderBlockSize;

	XalanOutputTranscoder*	m_transcoder;

	size_type	            m_bufferSize;

	BufferType				m_buffer;

	XalanDOMString			m_encoding;

	bool					m_writeAsUTF16;

	bool					m_throwTranscodeException;

	TranscodeVectorType 	m_transcodingBuffer;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALANOUTPUTSTREAM_HEADER_GUARD_1357924680
