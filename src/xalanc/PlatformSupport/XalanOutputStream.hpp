/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2003 The Apache Software Foundation.	All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *	  notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *	  notice, this list of conditions and the following disclaimer in
 *	  the documentation and/or other materials provided with the
 *	  distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *	  if any, must include the following acknowledgment:  
 *		 "This product includes software developed by the
 *		  Apache Software Foundation (http://www.apache.org/)."
 *	  Alternately, this acknowledgment may appear in the software itself,
 *	  if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *	  not be used to endorse or promote products derived from this
 *	  software without prior written permission. For written 
 *	  permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *	  nor may "Apache" appear in their name, without prior written
 *	  permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.	IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */
#if !defined(XALANOUTPUTSTREAM_HEADER_GUARD_1357924680)
#define XALANOUTPUTSTREAM_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



#include <vector>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/PlatformSupport/XSLException.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XalanOutputTranscoder;



class XALAN_PLATFORMSUPPORT_EXPORT XalanOutputStream
{
public :

	enum { eDefaultBufferSize = 512, eDefaultTranscoderBlockSize = 1024 };


#if defined(XALAN_NO_STD_NAMESPACE)
	typedef vector<XalanDOMChar>		BufferType;

	typedef vector<char>				TranscodeVectorType;
#else
	typedef std::vector<XalanDOMChar>	BufferType;

	typedef std::vector<char>			TranscodeVectorType;
#endif

	typedef BufferType::size_type	size_type;

	/**
	 * Constructor.
	 *
	 * @param theBufferSize the size of the transcoding buffer
	 * @param theTranscoderBlockSize the size of the block used by the transcoder
	 * @param fThrowTranscodeException If true, an error transcoding will result in an exception being thrown.
	 */
	explicit
	XalanOutputStream(
			BufferType::size_type			theBufferSize = eDefaultBufferSize,
			TranscodeVectorType::size_type	theTranscoderBlockSize = eDefaultTranscoderBlockSize,
			bool							fThrowTranscodeException = true);

	virtual
	~XalanOutputStream();

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
	 * will not be transcoded.
	 *
	 * @param theBuffer 	  character buffer to write
	 */
	void
	write(const char*	theBuffer)
	{
		assert(theBuffer != 0);

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
	 * will not be transcoded.
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

		flushBuffer();

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
	canTranscodeTo(unsigned int 	theChar) const;

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
	setThrowTranscodeException(bool flag)
	{
		m_throwTranscodeException = flag;
	}

	/**
	 * Set the size of the output buffer.
	 *
	 * @param theBufferSize The buffer size.
	 */
	void
	setBufferSize(BufferType::size_type 	theBufferSize);


	class XALAN_PLATFORMSUPPORT_EXPORT XalanOutputStreamException : public XSLException
	{
	public:

		XalanOutputStreamException(
			const XalanDOMString&	theMessage,
			const XalanDOMString&	theType);

		virtual
		~XalanOutputStreamException();
	};

	class XALAN_PLATFORMSUPPORT_EXPORT UnknownEncodingException : public XalanOutputStreamException
	{
	public:

		explicit
		UnknownEncodingException();

		virtual
		~UnknownEncodingException();
	};

	class XALAN_PLATFORMSUPPORT_EXPORT UnsupportedEncodingException : public XalanOutputStreamException
	{
	public:

		UnsupportedEncodingException(const XalanDOMString&	theEncoding);

		virtual
		~UnsupportedEncodingException();

		const XalanDOMString&
		getEncoding() const
		{
			return m_encoding;
		}

	private:

		const XalanDOMString	m_encoding;
	};

	class XALAN_PLATFORMSUPPORT_EXPORT TranscoderInternalFailureException : public XalanOutputStreamException
	{
	public:

		TranscoderInternalFailureException(const XalanDOMString&	theEncoding);

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
		TranscodingException();

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
			const char* 	theBuffer,
			size_type		theBufferLength) = 0;

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

	// Utility functions...
	void
	flushBuffer();

	void
	doWrite(
			const XalanDOMChar* 	theBuffer,
			size_t					theBufferLength);


	const TranscodeVectorType::size_type	m_transcoderBlockSize;

	XalanOutputTranscoder*					m_transcoder;

	BufferType::size_type					m_bufferSize;

	BufferType								m_buffer;

	XalanDOMString							m_encoding;

	bool									m_writeAsUTF16;

	bool									m_throwTranscodeException;

	TranscodeVectorType 					m_transcodingBuffer;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALANOUTPUTSTREAM_HEADER_GUARD_1357924680
