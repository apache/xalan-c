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
#if !defined(DOMSTRING_PRINTWRITER_HEADER_GUARD_1357924680)
#define DOMSTRING_PRINTWRITER_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <PlatformSupport/PlatformSupportDefinitions.hpp>






#include <PlatformSupport/PrintWriter.hpp>



#include <XalanDOM/XalanDOMString.hpp>



class XALAN_PLATFORMSUPPORT_EXPORT DOMStringPrintWriter : public PrintWriter
{
public:

	// If a string is passed in, a copy will be made, and all
	// output will be appended to that string.
	explicit
	DOMStringPrintWriter(const XalanDOMString&	theBaseString = XalanDOMString());

	virtual
	~DOMStringPrintWriter();

	const XalanDOMString&
	getString() const
	{
		return m_outputString;
	}
	// Output functions inherited from PrintWriter...

	// Flush the stream, then check the error status.
	virtual bool
    checkError() const;

	virtual void
	close();

	virtual void
	flush();


	// Output functions

	// If the length is UINT_MAX, then the array is assumed to be null-terminated.
	virtual void
	write(const char*	s,
		  unsigned int	theOffset = 0,
		  unsigned int	theLength = UINT_MAX);

	// If the length is UINT_MAX, then the array is assumed to be null-terminated.
	virtual void
	write(const XalanDOMChar*	s,
		  unsigned int			theOffset = 0,
		  unsigned int			theLength = UINT_MAX);

	virtual void
	write(XalanDOMChar	c);

	// If the length is UINT_MAX, then the entire string is printed.
	virtual void
	write(const XalanDOMString&		s,
		  unsigned int				theOffset = 0,
		  unsigned int				theLength = UINT_MAX);

	virtual void
	print(bool	b);

	virtual void
	print(char	c);

	virtual void
	print(const char*	s,
		  unsigned int	theLength = UINT_MAX);

	virtual void
	print(const XalanDOMChar*	s,
		  unsigned int			theLength = UINT_MAX);

	virtual void
	print(double	d);

	virtual void
	print(int	i);

	virtual void
	print(long	l);

	virtual void
	print(const XalanDOMString&		s);

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
	println(const XalanDOMChar*		s,
			unsigned int			theLength = UINT_MAX);

	virtual void
	println(double	d);

	virtual void
	println(int		i);

	virtual void
	println(long	l);

	virtual void
	println(const XalanDOMString&	s);

protected:

	XalanDOMString	m_outputString;

private:

	// Not implemented
	DOMStringPrintWriter(const DOMStringPrintWriter&);

	DOMStringPrintWriter&
	operator=(const DOMStringPrintWriter&);

	bool
	operator==(const DOMStringPrintWriter&);
};



#endif	// DOMSTRING_PRINTWRITER_HEADER_GUARD_1357924680
