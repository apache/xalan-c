/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999, 2000 The Apache Software Foundation.  All rights 
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
#include "XalanOutputStreamPrintWriter.hpp"



#include <cassert>



#include <XalanDOM/XalanDOMString.hpp>



#include "DOMStringHelper.hpp"
#include "XalanOutputStream.hpp"



XalanOutputStreamPrintWriter::XalanOutputStreamPrintWriter(
			XalanOutputStream&	theOutputStream,
			bool				fAutoFlush) :
	PrintWriter(fAutoFlush),
	m_OutputStream(theOutputStream)
{
}



XalanOutputStreamPrintWriter::~XalanOutputStreamPrintWriter()
{
	flush();
}



bool
XalanOutputStreamPrintWriter::checkError() const
{
	return false;
}



void
XalanOutputStreamPrintWriter::close()
{
	flush();
}


void
XalanOutputStreamPrintWriter::flush()
{
	m_OutputStream.flush();
}



XalanOutputStream*
XalanOutputStreamPrintWriter::getStream()
{
	return &m_OutputStream;
}



const XalanOutputStream*
XalanOutputStreamPrintWriter::getStream() const
{
	return &m_OutputStream;
}



void
XalanOutputStreamPrintWriter::write(
			const char*		s,
			unsigned int	theOffset,
			unsigned int	theLength)
{
	assert(s != 0);

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
		m_OutputStream.write(s + theOffset, theLength);
	}
}



void
XalanOutputStreamPrintWriter::write(
			const XMLCh*	s,
			unsigned int	theOffset,
			unsigned int	theLength)
{
	assert(s != 0);

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
		m_OutputStream.write(s + theOffset, theLength);
	}
}



void
XalanOutputStreamPrintWriter::write(XMLCh	c)
{
	m_OutputStream.write(c);
}



void
XalanOutputStreamPrintWriter::write(
			const DOMString&	s,
			unsigned int		theOffset,
			unsigned int		theLength)
{
	write(c_wstr(s), theOffset, theLength);
}



#if !defined(XALAN_BOOL_AS_INT)
void
XalanOutputStreamPrintWriter::print(bool	b)
{
	if (b == true)
	{
		print(s_trueString);
	}
	else
	{
		print(s_falseString);
	}
}
#endif



void
XalanOutputStreamPrintWriter::print(char	c)
{
	write(c);
}



void
XalanOutputStreamPrintWriter::print(
			const char*		s,
			unsigned int	theLength)
{
	write(s,
		  0,
		  theLength);
}



void
XalanOutputStreamPrintWriter::print(
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
XalanOutputStreamPrintWriter::print(double	d)
{
	m_OutputStream.write(c_wstr(DoubleToDOMString(d)));
}



void
XalanOutputStreamPrintWriter::print(int	i)
{
	m_OutputStream.write(c_wstr(LongToDOMString(i)));
}



void
XalanOutputStreamPrintWriter::print(long	l)
{
	m_OutputStream.write(c_wstr(LongToDOMString(l)));
}



void
XalanOutputStreamPrintWriter::print(const DOMString&	s)
{
	m_OutputStream.write(c_wstr(s));
}



void
XalanOutputStreamPrintWriter::println()
{
	m_OutputStream.write(c_wstr(s_newlineString));

	flush();
}



#if !defined(XALAN_BOOL_AS_INT)
void
XalanOutputStreamPrintWriter::println(bool	b)
{
	print(b);

	println();
}
#endif



void
XalanOutputStreamPrintWriter::println(char	c)
{
	print(c);

	println();
}



void
XalanOutputStreamPrintWriter::println(
			const char*		s,
			unsigned int	theLength)
{
	print(s, theLength);

	println();
}



void
XalanOutputStreamPrintWriter::println(
			const XMLCh*	s,
			unsigned int	theLength)
{
	print(s, theLength);

	println();
}



void
XalanOutputStreamPrintWriter::println(double	d)
{
	print(d);

	println();
}



void
XalanOutputStreamPrintWriter::println(int		i)
{
	print(i);

	println();
}



void
XalanOutputStreamPrintWriter::println(long	l)
{
	print(l);

	println();
}



void
XalanOutputStreamPrintWriter::println(const DOMString&	s)
{
	print(s);

	println();
}
