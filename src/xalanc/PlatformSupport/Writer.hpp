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
#if !defined(WRITER_HEADER_GUARD_1357924680)
#define WRITER_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



#include <cstddef>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XalanOutputStream;



class XALAN_PLATFORMSUPPORT_EXPORT Writer
{
public:

	Writer();

	virtual
	~Writer();

	/**
	 * Close the stream
	 */
	virtual void
	close() = 0;

	/**
	 * Flush the stream
	 */
	virtual void
	flush() = 0;

	/**
	 * Get the stream associated with the writer...
	 */
	virtual XalanOutputStream*
	getStream();

	/**
	 * Get the stream associated with the writer...
	 */
	virtual const XalanOutputStream*
	getStream() const;


	// Output functions

#if defined(XALAN_INLINE_INITIALIZATION)
	static const size_t		npos = ~0u;
#else
	static const size_t		npos;
#endif

	/**
	 * Writes a string
	 * 
	 * @param 	s         string to write
	 * @param 	theOffset starting offset in string to begin writing, default 0
	 * @param 	theLength number of characters to write. If the length is npos, then the array is assumed to be null-terminated.
	 */
	virtual void
	write(
			const char*		s,
			size_t			theOffset = 0,
			size_t			theLength = npos) = 0;

	/**
	 * Writes a string
	 * 
	 * @param 	s         string to write
	 * @param 	theOffset starting offset in string to begin writing, default 0
	 * @param 	theLength number of characters to write. If the length is XalanDOMString::npos, then the array is assumed to be null-terminated.
	 */
	virtual void
	write(
			const XalanDOMChar*			s,
			XalanDOMString::size_type	theOffset = 0,
			XalanDOMString::size_type	theLength = XalanDOMString::npos) = 0;

	/**
	 * Writes a character
	 * 
	 * @param 	c         character to write
	 */
	virtual void
	write(XalanDOMChar	c) = 0;

	/**
	 * Writes a string
	 * 
	 * @param 	s         string to write
	 * @param 	theOffset starting offset in string to begin writing, default 0
	 * @param 	theLength number of characters to write. If the length is XalanDOMString::npos,  then the entire string is printed.
	 */
	virtual void
	write(
			const XalanDOMString&		s,
			XalanDOMString::size_type	theOffset = 0,
			XalanDOMString::size_type	theLength = XalanDOMString::npos) = 0;

private:

	// Not implemented
	Writer(const Writer&);

	Writer&
	operator=(const Writer&);

	bool
	operator==(const Writer&);
};



XALAN_CPP_NAMESPACE_END



#endif	// WRITER_HEADER_GUARD_1357924680
