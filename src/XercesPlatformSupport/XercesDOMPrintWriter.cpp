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
// Class header file.
#include "XercesDOMPrintWriter.hpp"



#include <cassert>



#include <XalanDOM/XalanDOMString.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/TextOutputStream.hpp>



XercesDOMPrintWriter::XercesDOMPrintWriter(
			TextOutputStream&	theOutputStream,
			bool				fAutoFlush) :
	PrintWriter(fAutoFlush),
	m_OutputStream(theOutputStream)
{
}



XercesDOMPrintWriter::~XercesDOMPrintWriter()
{
	flush();
}



bool
XercesDOMPrintWriter::checkError() const
{
	return false;
}



void
XercesDOMPrintWriter::close()
{
}


void
XercesDOMPrintWriter::flush()
{
	m_OutputStream.flush();
}



void
XercesDOMPrintWriter::write(
			const char*		s,
			unsigned int	theOffset,
			unsigned int	theLength)
{
	write(DOMString(s), theOffset, theLength);
}



void
XercesDOMPrintWriter::write(
			const XMLCh*	s,
			unsigned int	theOffset,
			unsigned int	theLength)
{
	assert(s != 0);
	assert(length(s) == 0 || theOffset < length(s));

	if (theLength == -1)
	{
		if (theOffset == 0)
		{
			m_OutputStream.write(s);
		}
		else
		{
			m_OutputStream.write(s + theOffset);
		}
	}
	else
	{
		const long	theStopIndex = theOffset + theLength;

		for (long i = theOffset; i < theStopIndex; i++)
		{
			m_OutputStream.write(s[i]);
		}
	}
}



void
XercesDOMPrintWriter::write(XMLCh	c)
{
	m_OutputStream.write(c);
}



void
XercesDOMPrintWriter::write(
			const DOMString&	s,
			unsigned int		theOffset,
			unsigned int		theLength)
{
	write(c_wstr(s), theOffset, theLength);
}



void
XercesDOMPrintWriter::print(bool	b)
{
	if (b == true)
	{
		print(XALAN_STATIC_UCODE_STRING("true"));
	}
	else
	{
		print(XALAN_STATIC_UCODE_STRING("false"));
	}
}



void
XercesDOMPrintWriter::print(char	c)
{
	write(c);
}



void
XercesDOMPrintWriter::print(
			const char*		s,
			unsigned int	theLength)
{
	write(s,
		  0,
		  theLength);
}



void
XercesDOMPrintWriter::print(
			const XMLCh*	s,
			unsigned int	theLength)
{
	assert(s != 0);
	assert(theLength >= 0 || theLength == -1);

	write(s,
		  0,
		  theLength);
}



void
XercesDOMPrintWriter::print(double	d)
{
	m_OutputStream.write(c_wstr(DoubleToDOMString(d)));
}



void
XercesDOMPrintWriter::print(int	i)
{
	m_OutputStream.write(c_wstr(LongToDOMString(i)));
}



void
XercesDOMPrintWriter::print(long	l)
{
	m_OutputStream.write(c_wstr(LongToDOMString(l)));
}



void
XercesDOMPrintWriter::print(const DOMString&	s)
{
	m_OutputStream.write(c_wstr(s));
}



void
XercesDOMPrintWriter::println()
{
	m_OutputStream.write(XMLCh('\n'));

	flush();
}



void
XercesDOMPrintWriter::println(bool	b)
{
	print(b);

	println();
}



void
XercesDOMPrintWriter::println(char	c)
{
	print(c);

	println();
}



void
XercesDOMPrintWriter::println(
			const char*		s,
			unsigned int	theLength)
{
	print(s, theLength);

	println();
}



void
XercesDOMPrintWriter::println(
			const XMLCh*	s,
			unsigned int	theLength)
{
	print(s, theLength);

	println();
}



void
XercesDOMPrintWriter::println(double	d)
{
	print(d);

	println();
}



void
XercesDOMPrintWriter::println(int		i)
{
	print(i);

	println();
}



void
XercesDOMPrintWriter::println(long	l)
{
	print(l);

	println();
}



void
XercesDOMPrintWriter::println(const DOMString&	s)
{
	print(s);

	println();
}
