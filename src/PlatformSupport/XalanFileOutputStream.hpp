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
#if !defined(XALANFILEOUTPUTSTREAM_HEADER_GUARD_1357924680)
#define XALANFILEOUTPUTSTREAM_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <PlatformSupport/PlatformSupportDefinitions.hpp>



#if defined(WIN32)
#include <windows.h>
#else
#include <cstdio>
#endif

#include <vector>



// Base class header file.
#include <PlatformSupport/XalanOutputStream.hpp>



class XALAN_PLATFORMSUPPORT_EXPORT XalanFileOutputStream : public XalanOutputStream
{
public :

	enum { eDefaultBufferSize = 8192 };

#if defined(WIN32)
	typedef HANDLE	HandleType;
#else
	typedef FILE*	HandleType;
#endif

	/**
	 * Construct an XalanFileOutputStream object.
	 * 
	 * @param theFileName name of file
	 * @param theBufferSize The size of the buffer
	 */
    XalanFileOutputStream(
			const XalanDOMString&	theFileName,
			unsigned int			theBufferSize = eDefaultBufferSize);

    virtual
	~XalanFileOutputStream();


	class XALAN_PLATFORMSUPPORT_EXPORT XalanFileOutputStreamOpenException : public XalanOutputStream::XalanOutputStreamException
	{
	public:

		/**
		 * Construct an XalanFileOutputStreamOpen exception object for an exception
		 * that occurred on opening a text file stream.
		 * 
		 * @param theFileName  name of file causing the exception
		 * @param theErrorCode number of error encountered
		 */
		XalanFileOutputStreamOpenException(
			const XalanDOMString&	theFileName,
			int						theErrorCode);

		virtual
		~XalanFileOutputStreamOpenException();
	};

	class XALAN_PLATFORMSUPPORT_EXPORT XalanFileOutputStreamWriteException : public XalanOutputStream::XalanOutputStreamException
	{
	public:

		/**
		 * Construct an XalanFileOutputStreamOpen exception object for an exception
		 * that occurred while writing to a text file stream.
		 * 
		 * @param theFileName  name of file causing the exception
		 * @param theErrorCode number of error encountered
		 */
		XalanFileOutputStreamWriteException(
			const XalanDOMString&	theFileName,
			int						theErrorCode);

		virtual
		~XalanFileOutputStreamWriteException();
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
    XalanFileOutputStream(const XalanFileOutputStream&);

    XalanFileOutputStream&
	operator=(const XalanFileOutputStream&);

    bool
	operator==(const XalanFileOutputStream&) const;


	// Data members...
	const XalanDOMString	m_fileName;

	const HandleType		m_handle;
};



#endif	// XALANFILEOUTPUTSTREAM_HEADER_GUARD_1357924680
