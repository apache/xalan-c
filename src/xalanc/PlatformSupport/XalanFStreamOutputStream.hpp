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
#if !defined(XALANFSTREAMOUTPUTSTREAM_HEADER_GUARD_1357924680)
#define XALANFSTREAMOUTPUTSTREAM_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



#include <cstdio>



// Base class header file.
#include <xalanc/PlatformSupport/XalanOutputStream.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_PLATFORMSUPPORT_EXPORT XalanFStreamOutputStream : public XalanOutputStream
{
public :

	enum { eDefaultBufferSize = 8192 };


	/**
	 * Construct a XalanFStreamOutputStream object.
	 * 
	 * @param theHandle handle of file.  Must not be null.
	 * @param theBufferSize The size of the transcoding buffer
	 */
    XalanFStreamOutputStream(
			FILE*			    theFileHandle,
            MemoryManagerType&   theManager,
			unsigned int	    theBufferSize = eDefaultBufferSize);

    static XalanFStreamOutputStream*
    create(	FILE*			    theFileHandle,
            MemoryManagerType&   theManager,
			unsigned int	    theBufferSize = eDefaultBufferSize);
    virtual
	~XalanFStreamOutputStream();

	class XALAN_PLATFORMSUPPORT_EXPORT XalanFStreamOutputStreamWriteException : public XalanOutputStream::XalanOutputStreamException
	{
	public:

		/**
		 * Construct an XalanFStreamOutputStreamWriteException exception object for an exception
		 * that occurred while writing to a file stream.
		 * 
		 * @param theErrorCode number of error encountered
		 */
		XalanFStreamOutputStreamWriteException(int	theErrorCode,
                                                XalanDOMString& theBuffer);

		virtual
		~XalanFStreamOutputStreamWriteException();

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
    XalanFStreamOutputStream(const XalanFStreamOutputStream&);

    XalanFStreamOutputStream&
	operator=(const XalanFStreamOutputStream&);

    bool
	operator==(const XalanFStreamOutputStream&) const;


	// Data members...
	FILE* const		m_handle;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALANFSTREAMOUTPUTSTREAM_HEADER_GUARD_1357924680
