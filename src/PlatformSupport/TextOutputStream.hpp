/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
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
#if !defined(TEXTOUTPUTSTREAM_HEADER_GUARD_1357924680)
#define TEXTOUTPUTSTREAM_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <PlatformSupport/PlatformSupportDefinitions.hpp>



#include <vector>



#include <XalanDOM/XalanDOMString.hpp>



#include <PlatformSupport/XSLException.hpp>



class XMLTranscoder;



class XALAN_PLATFORMSUPPORT_EXPORT TextOutputStream
{
public :

	enum { eDefaultBlockSize = 1024 };

	explicit
	TextOutputStream(unsigned int	theTranscoderBlockSize = eDefaultBlockSize);

	virtual
	~TextOutputStream();

	/**
	 * Flush the stream's buffer.
	 */
    virtual void
	flush() = 0;

	/**
	 * Write a character to the output stream.  The character
	 * will not be transcoded.
	 *
	 * @param theChar       the character to write
	 */
    virtual void
	write(char	theChar) = 0;

	/**
	 * Write a wide character to the output stream.  The character
	 * will be transcoded, if an output encoding is specified.
	 *
	 * @param theChar       the character to write
	 */
    virtual void
	write(XalanDOMChar	theChar) = 0;

	/**
	 * Write a null-terminated string to the output file.  The character
	 * will not be transcoded.
	 *
	 * @param theBuffer       character buffer to write
	 */
    virtual void
	write(const char*	theBuffer) = 0;

	/**
	 * Write a null-terminated wide string to the output file.  The string
	 * will be transcoded, if an output encoding is specified.
	 *
	 * @param theBuffer       character buffer to write
	 */
    virtual void
	write(const XalanDOMChar*	theBuffer) = 0;

	/**
	 * Write a specified number of characters to the output stream.  The string
	 * will not be transcoded.
	 *
	 * @param theBuffer       character buffer to write
	 * @param theBufferLength number of characters to write
	 */
    virtual void
	write(
			const char*		theBuffer,
			unsigned long	theBufferLength) = 0;

	/**
	 * Write a specified number of characters to the output stream.  The string
	 * will be transcoded, if an output encoding is specified.
	 *
	 * @param theBuffer       character buffer to write
	 * @param theBufferLength number of characters to write
	 */
    virtual void
	write(
			const XalanDOMChar*		theBuffer,
			unsigned long			theBufferLength) = 0;

	/**
	 * Get the output encoding for the stream.
	 *
	 * @return The encoding name
	 */
	virtual const XalanDOMString&
	getOutputEncoding() const;

	/**
	 * Set the output encoding for the stream.
	 *
	 * @param theEncoding The encoding name
	 */
	virtual void
	setOutputEncoding(const XalanDOMString&		theEncoding);

	class TextOutputStreamException : public XSLException
	{
	public:

		TextOutputStreamException(
			const XalanDOMString&	theMessage,
			const XalanDOMString&	theType);

		virtual
		~TextOutputStreamException();
	};

	class UnknownEncodingException : public TextOutputStreamException
	{
	public:

		explicit
		UnknownEncodingException();

		virtual
		~UnknownEncodingException();
	};

	class UnsupportedEncodingException : public TextOutputStreamException
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

		const XalanDOMString&	m_encoding;
	};

	class TranscoderInternalFailureException : public TextOutputStreamException
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

		const XalanDOMString&	m_encoding;
	};

	class TranscodingException : public TextOutputStreamException
	{
	public:

		explicit
		TranscodingException();

		virtual
		~TranscodingException();
	};

protected:

#if defined(XALAN_NO_NAMESPACES)
	typedef vector<char>		TranscodeVectorType;
#else
	typedef std::vector<char>	TranscodeVectorType;
#endif

	void
	transcode(
			const XalanDOMChar*		theBuffer,
			unsigned long			theBufferLength,
			TranscodeVectorType&	theDestination);

private:

    // These are not implemented...
    TextOutputStream(const TextOutputStream&);

    TextOutputStream&
	operator=(const TextOutputStream&);

    bool
	operator==(const TextOutputStream&) const;


	const unsigned int	m_transcoderBlockSize;

	XalanDOMString		m_encoding;

	XMLTranscoder*		m_transcoder;
};



#endif	// TEXTOUTPUTSTREAM_HEADER_GUARD_1357924680
