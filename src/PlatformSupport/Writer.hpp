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
#if !defined(WRITER_HEADER_GUARD_1357924680)
#define WRITER_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <PlatformSupport/PlatformSupportDefinitions.hpp>



#include <cstddef>



#include <XalanDOM/XalanDOMString.hpp>



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



#endif	// WRITER_HEADER_GUARD_1357924680
