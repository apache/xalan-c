/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights 
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
 *
 * $ Id: $
 
 */

#if !defined(XALAN_XSLTRESULTTARGET_HEADER_GUARD)
#define XALAN_XSLTRESULTTARGET_HEADER_GUARD

// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"



#include <cstdio>
#if defined(XALAN_CLASSIC_IOSTREAMS)
class ostream;
#else
#include <iosfwd>
#endif



#include <xalanc/XalanDOM/XalanDOMString.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class FormatterListener;
class Writer;



class XALAN_XSLT_EXPORT XSLTResultTarget
{
public:

#if defined(XALAN_NO_STD_NAMESPACE)
	typedef ostream			StreamType;
#else
	typedef std::ostream	StreamType;
#endif

	explicit
	XSLTResultTarget();

	/**
	 * Create a new output target with a file name.
	 *
	 * @param fileName valid system file name
	 */
	XSLTResultTarget(const XalanDOMString&	fileName);

	/**
	 * Create a new output target with a file name.
	 *
	 * @param fileName valid system file name
	 */
	XSLTResultTarget(const XalanDOMChar*	fileName);

	/**
	 * Create a new output target with a file name.
	 *
	 * @param fileName valid system file name
	 */
	XSLTResultTarget(const char*	fileName);

	/**
	 * Create a new output target with a stream.
	 *
	 * @param byteStream a pointer to a std ostream for the output
	 */
	XSLTResultTarget(StreamType*	theStream);

	/**
	 * Create a new output target with a stream.
	 *
	 * @param byteStream a reference to a std ostream for the output
	 */
	XSLTResultTarget(StreamType&	theStream);

	/**
	 * Create a new output target with a character stream.
	 *
	 * @param characterStream pointer to character stream where the results
	 *                        will be written
	 */ 
	XSLTResultTarget(Writer*	characterStream);

	/**
	 * Create a new output target with a stream.
	 *
	 * @param characterStream pointer to character stream where the results
	 *                        will be written
	 */ 
	XSLTResultTarget(FILE*	characterStream);

	/**
	 * Create a new output target with a FormatterListener.
	 *
	 * @param flistener A FormatterListener instance for result tree events.
	 */
	XSLTResultTarget(FormatterListener&		flistener);

	~XSLTResultTarget();

	/**
	 * Set the file name where the results will be written.
	 *
	 * @param fileName system identifier as a string
	 */
	void
	setFileName(const char*		fileName)
	{
		if (fileName == 0)
		{
			m_fileName.clear();
		}
		else
		{
			m_fileName = fileName;
		}
	}

	/**
	 * Set the file name where the results will be written.
	 *
	 * @param fileName system identifier as a string
	 */
	void
	setFileName(const XalanDOMString&	fileName)
	{
		m_fileName = fileName;
	}

	/**
	 * Get the file name where the results will be written to.
	 * 
	 * @return file name string
	 */
	const XalanDOMString&
	getFileName() const
	{
		return m_fileName;
	}

	/**
	 * Set the byte stream for this output target.
	 *
	 * @param byteStream pointer to byte stream that will contain the result
	 *                   document
	 */
	void
	setByteStream(StreamType*			byteStream)
	{
		m_byteStream = byteStream;
	}

	/**
	 * Get the byte stream for this output target.
	 *
	 * @return pointer to byte stream, or null if none was supplied.
	 */
	StreamType*
	getByteStream() const
	{
		return m_byteStream;
	}

	/** 
	 * Set the character encoding, if known.
	 *
	 * @param encoding new encoding string
	 */
	void
	setEncoding(const XalanDOMChar*		encoding)
	{
		if (encoding == 0)
		{
			m_encoding.clear();
		}
		else
		{
			m_encoding = encoding;
		}
	}

	/** 
	 * Set the character encoding, if known.
	 *
	 * @param encoding new encoding string
	 */
	void
	setEncoding(const XalanDOMString&	encoding)
	{
		m_encoding = encoding;
	}

	/**
	 * Get the character encoding in use.
	 *
	 * @return encoding string, or empty string if none was supplied.
	 */
	const XalanDOMString&
	getEncoding() const
	{
		return m_encoding;
	}

	/**
	 * Set the character stream for this output target.
	 *
	 * @param characterStream pointer to character stream that will contain 
	 *                        the result document
	 */
	void
	setCharacterStream(Writer*	characterStream)
	{
		m_characterStream = characterStream;
	}

	/**
	 * Get the character stream for this output target.
	 *
	 * @return pointer to character stream, or null if none was supplied.
	 */
	Writer*
	getCharacterStream() const
	{
		return m_characterStream;
	}

	/**
	 * Get the stream for this output target.
	 *
	 * @return pointer to stream, or null if none was supplied.
	 */
	FILE*
	getStream() const
	{
		return m_stream;
	}

	/**
	 * Set a FormatterListener to process the result tree events.
	 *
	 * @param handler pointer to new listener
	 */
	void
	setFormatterListener(FormatterListener*		handler)
	{
		m_formatterListener = handler;
	}

	/**
	 * Get the FormatterListener that will process the result tree events.
	 *
	 * @return pointer to new listener
	 */
	FormatterListener*
	getFormatterListener() const
	{
		return m_formatterListener;
	}

private:

	XalanDOMString			m_fileName;

	StreamType*				m_byteStream;

	XalanDOMString			m_encoding;

	Writer*					m_characterStream;

	FormatterListener*		m_formatterListener;

	FILE*					m_stream;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALAN_XSLTRESULTTARGET_HEADER_GUARD
