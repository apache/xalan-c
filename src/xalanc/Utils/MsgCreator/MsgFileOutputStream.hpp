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

// This file is simplified version of XalanFileOutputStream.hpp / .cpp

#if !defined(XALANOUTPUTFILE_1357924680)
#define XALANOUTPUTFILE_1357924680

#include <xercesc/util/XMLUniDefs.hpp>


#if defined(WIN32)
#include <windows.h>
#else
#include <cstdio>
#endif

#include <vector>



XERCES_CPP_NAMESPACE_USE




// Class responcible for printing into file with UTF16


class  XalanFileOutputStream 
{
public :

#if defined(WIN32)
	typedef HANDLE		HandleType;
#else
#if defined(XALAN_STRICT_ANSI_HEADERS)
	typedef std::FILE*	HandleType;
#else
	typedef FILE*		HandleType;
#endif
#endif

	/**
	 * Construct an XalanFileOutputStream object.
	 * 
	 * @param theFileName name of file
	 * @param theBufferSize The size of the transcoding buffer
	 */
    XalanFileOutputStream(
			const char*	theFileName);

    virtual
	~XalanFileOutputStream();


	class  XalanFileOutputStreamOpenException 
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
			const char*		theFileName,
			int				theErrorCode);

		virtual
		~XalanFileOutputStreamOpenException();
		
		char*	m_pMessage;
	};

	class XalanFileOutputStreamWriteException 
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
			const char*			theFileName,
			int						theErrorCode);

		virtual
		~XalanFileOutputStreamWriteException();

		char* m_pMessage;
	};


	void 
	write(const UTF16Ch*	theString, unsigned int		theLength);
	void 
	write(const char*	theString, unsigned int		theLength);

	void 
	writeAsASCII(const char*	theString, unsigned int		theLengts);

	void
	writeAsASCII(const UTF16Ch*	theString, unsigned int		theLengts);
	void
	writeUTFprefix();
protected:

	void
	writeData(
			const char*		theBuffer,
			unsigned int	theBufferLength);

	void
	doFlush();

private:

    // These are not implemented...
    XalanFileOutputStream(const XalanFileOutputStream&);

    XalanFileOutputStream&
	operator=(const XalanFileOutputStream&);

    bool
	operator==(const XalanFileOutputStream&) const;


	// Data members...
	const char*			m_fileName;

	const HandleType	m_handle;
};


#endif //XALANOUTPUTFILE_1357924680


