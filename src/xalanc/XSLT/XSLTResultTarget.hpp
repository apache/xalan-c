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

#if defined(XALAN_STRICT_ANSI_HEADERS)
	typedef std::FILE	FILE;
#endif


	explicit
	XSLTResultTarget(MemoryManagerType& theManager XALAN_DEFAULT_CONSTRACTOR_MEMORY_MGR);

	/**
	 * Create a new output target with a file name.
	 *
	 * @param fileName valid system file name
	 */
	XSLTResultTarget(const XalanDOMString&	fileName,
                    MemoryManagerType& theManager XALAN_DEFAULT_CONSTRACTOR_MEMORY_MGR);

	/**
	 * Create a new output target with a file name.
	 *
	 * @param fileName valid system file name
	 */
	XSLTResultTarget(const XalanDOMChar*	fileName,
                    MemoryManagerType& theManager XALAN_DEFAULT_CONSTRACTOR_MEMORY_MGR);

	/**
	 * Create a new output target with a file name.
	 *
	 * @param fileName valid system file name
	 */
	XSLTResultTarget(const char*	fileName,
                    MemoryManagerType& theManager XALAN_DEFAULT_CONSTRACTOR_MEMORY_MGR);

	/**
	 * Create a new output target with a stream.
	 *
	 * @param byteStream a pointer to a std ostream for the output
	 */
	XSLTResultTarget(StreamType*	theStream,
                    MemoryManagerType& theManager XALAN_DEFAULT_CONSTRACTOR_MEMORY_MGR);

	/**
	 * Create a new output target with a stream.
	 *
	 * @param byteStream a reference to a std ostream for the output
	 */
	XSLTResultTarget(StreamType&	theStream,
                    MemoryManagerType& theManager XALAN_DEFAULT_CONSTRACTOR_MEMORY_MGR);

	/**
	 * Create a new output target with a character stream.
	 *
	 * @param characterStream pointer to character stream where the results
	 *                        will be written
	 */ 
	XSLTResultTarget(Writer*	characterStream,
                    MemoryManagerType& theManager XALAN_DEFAULT_CONSTRACTOR_MEMORY_MGR);

	/**
	 * Create a new output target with a stream.
	 *
	 * @param characterStream pointer to character stream where the results
	 *                        will be written
	 */ 
	XSLTResultTarget(FILE*	characterStream,
                    MemoryManagerType& theManager XALAN_DEFAULT_CONSTRACTOR_MEMORY_MGR);

	/**
	 * Create a new output target with a FormatterListener.
	 *
	 * @param flistener A FormatterListener instance for result tree events.
	 */
	XSLTResultTarget(FormatterListener&		flistener,
                    MemoryManagerType& theManager XALAN_DEFAULT_CONSTRACTOR_MEMORY_MGR);

    XSLTResultTarget(const XSLTResultTarget& other,
                    MemoryManagerType&      theManager XALAN_DEFAULT_CONSTRACTOR_MEMORY_MGR);
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
	 * Set the stream for this output target.
	 *
	 * @theStream pointer to stream.
	 */
	void
	setStream(FILE*		theStream)
	{
		m_stream = theStream;
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

#if defined(XALAN_DEVELOPMENT)
    XSLTResultTarget();
	XSLTResultTarget(const XSLTResultTarget&);
#endif

	XalanDOMString			m_fileName;

	StreamType*				m_byteStream;

	XalanDOMString			m_encoding;

	Writer*					m_characterStream;

	FormatterListener*		m_formatterListener;

	FILE*					m_stream;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALAN_XSLTRESULTTARGET_HEADER_GUARD
