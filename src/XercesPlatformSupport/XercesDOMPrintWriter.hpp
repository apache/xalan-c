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
#if !defined(XercesDOMPRINTWRITER_HEADER_GUARD_1357924680)
#define XercesDOMPRINTWRITER_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XercesPlatformSupport/XercesPlatformSupportDefinitions.hpp>



#include <PlatformSupport/PrintWriter.hpp>



class TextOutputStream;



class XALAN_XERCESPLATFORMSUPPORT_EXPORT XercesDOMPrintWriter : public PrintWriter
{
public:

	// If fAutoFlush is true, the output will not
	// be buffered.
	XercesDOMPrintWriter(
			TextOutputStream&	theOutputStream,
			bool	fAutoFlush = false);

	virtual
	~XercesDOMPrintWriter();

	// Flush the stream, then check the error status.
	virtual bool
    checkError() const;

	virtual void
	close();

	virtual void
	flush();


	// Output functions...

	// If the length is UINT_MAX, then the array is assumed to be null-terminated.
	virtual void
	write(const char*	s,
		  unsigned int	theOffset = 0,
		  unsigned int	theLength = UINT_MAX);

	// If the length is UINT_MAX, then the entire string is printed.
	virtual void
	write(const XMLCh*	s,
		  unsigned int	theOffset = 0,
		  unsigned int	theLength = UINT_MAX);

	virtual void
	write(XMLCh		c);

	// If the length is UINT_MAX, then the entire string is printed.
	virtual void
	write(const DOMString&	s,
		  unsigned int		theOffset = 0,
		  unsigned int		theLength = UINT_MAX);

	virtual void
	print(bool	b);

	virtual void
	print(char	c);

	virtual void
	print(const char*	s,
		  unsigned int	theLength = UINT_MAX);

	virtual void
	print(const XMLCh*	s,
		  unsigned int	theLength = UINT_MAX);

	virtual void
	print(double	d);

	virtual void
	print(int	i);

	virtual void
	print(long	l);

	virtual void
	print(const DOMString&	s);

	virtual void
	println();

	virtual void
	println(bool	b);

	virtual void
	println(char	c);

	virtual void
	println(const char*		s,
		    unsigned int	theLength = UINT_MAX);

	virtual void
	println(const XMLCh*	s,
			unsigned int	theLength = UINT_MAX);

	virtual void
	println(double	d);

	virtual void
	println(int		i);

	virtual void
	println(long	l);

	virtual void
	println(const DOMString&	s);

protected:

	TextOutputStream&	m_OutputStream;

private:

	// Not implemented
	XercesDOMPrintWriter(const XercesDOMPrintWriter&);

	XercesDOMPrintWriter&
	operator=(const XercesDOMPrintWriter&);

	bool
	operator==(const XercesDOMPrintWriter&);
};



#endif	// XercesDOMPRINTWRITER_HEADER_GUARD_1357924680
