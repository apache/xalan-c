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
#if !defined(XALANSTDOUTPUTSTREAM_HEADER_GUARD_1357924680)
#define XALANSTDOUTPUTSTREAM_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#else
#include <iosfwd>
#include <ios>
#endif



// Base class header file.
#include <xalanc/PlatformSupport/XalanOutputStream.hpp>



XALAN_CPP_NAMESPACE_BEGIN



// A base class for all text output streams.
class XALAN_PLATFORMSUPPORT_EXPORT XalanStdOutputStream : public XalanOutputStream
{
public:

#if defined(XALAN_NO_STD_NAMESPACE)
	typedef ostream				StreamType;
#else
	typedef std::ostream		StreamType;
#endif

#if defined(XALAN_CLASSIC_IOSTREAMS)
	typedef int					StreamSizeType;
#else
#if defined(XALAN_NO_STD_NAMESPACE)
	typedef streamsize			StreamSizeType;
#else
	typedef std::streamsize		StreamSizeType;
#endif
#endif

	/**
	 * Construct a XalanStdOutputStream instance for output to the
	 * standard output device.
	 *
	 * @param theOutputStream output stream to use
	 */
    XalanStdOutputStream(StreamType&	theOutputStream,
                         MemoryManagerType&  theManager XALAN_DEFAULT_MEMMGR);

    static XalanStdOutputStream*
    create( StreamType&	theOutputStream,
            MemoryManagerType&  theManager);

    virtual
	~XalanStdOutputStream();

#if defined(XALAN_NEWLINE_IS_CRLF)
	virtual void
	newline();

	virtual const XalanDOMChar*
	getNewlineString() const;
#endif


	class XALAN_PLATFORMSUPPORT_EXPORT XalanStdOutputStreamWriteException : public XalanOutputStream::XalanOutputStreamException
	{
	public:

		XalanStdOutputStreamWriteException(int	theErrorCode,
                                            XalanDOMString&		theResult);

		virtual
		~XalanStdOutputStreamWriteException();

		virtual const XalanDOMChar*
		getType() const
		{
			return m_type;
		}

	private:

		static const XalanDOMChar	m_type[];

	};

protected:

	virtual void
	writeData(
			const char*		theBuffer,
			size_type		theBufferLength);

	virtual void
	doFlush();

private:

    // These are not implemented...
    XalanStdOutputStream(const XalanStdOutputStream&);

    XalanStdOutputStream&
	operator=(const XalanStdOutputStream&);

	// Data members...
	StreamType&					m_outputStream;

#if defined(XALAN_NEWLINE_IS_CRLF)
	const XalanDOMChar*			m_newlineString;

	XalanDOMString::size_type	m_newlineStringLength;
#endif
};



XALAN_CPP_NAMESPACE_END



#endif	// XERCESSTDTEXTOUTPUTSTREAM_HEADER_GUARD_1357924680
