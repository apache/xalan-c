/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2003, 2000 The Apache Software Foundation.  All rights 
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



#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include "DOMStringHelper.hpp"
#include "XalanOutputStream.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XalanOutputStreamPrintWriter::XalanOutputStreamPrintWriter(
			XalanOutputStream&	theOutputStream,
			bool				fAutoFlush) :
	PrintWriter(fAutoFlush),
	m_outputStream(theOutputStream),
	m_buffer(),
	m_flushWideChars(false)
{
}



XalanOutputStreamPrintWriter::~XalanOutputStreamPrintWriter()
{
	flush();
}



void
XalanOutputStreamPrintWriter::flushWideChars()
{
	if (m_flushWideChars == true)
	{
		m_outputStream.flushBuffer();

		m_flushWideChars = false;
	}
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
	m_outputStream.flush();
}



XalanOutputStream*
XalanOutputStreamPrintWriter::getStream()
{
	return &m_outputStream;
}



const XalanOutputStream*
XalanOutputStreamPrintWriter::getStream() const
{
	return &m_outputStream;
}



void
XalanOutputStreamPrintWriter::write(
			const char*		s,
			size_t			theOffset,
			size_t			theLength)
{
	assert(s != 0);

	flushWideChars();

	if (theLength == npos)
	{
		if (theOffset == 0)
		{
			m_outputStream.write(s);
		}
		else
		{
			m_outputStream.write(s + theOffset);
		}
	}
	else
	{
		m_outputStream.write(s + theOffset, theLength);
	}
}



void
XalanOutputStreamPrintWriter::write(
			const XalanDOMChar*			s,
			XalanDOMString::size_type	theOffset,
			XalanDOMString::size_type	theLength)
{
	assert(s != 0);

	if (theLength == XalanDOMString::npos)
	{
		if (theOffset == 0)
		{
			m_outputStream.write(s);
		}
		else
		{
			m_outputStream.write(s + theOffset);
		}
	}
	else
	{
		m_outputStream.write(s + theOffset, theLength);
	}

	m_flushWideChars = true;
}



void
XalanOutputStreamPrintWriter::write(XalanDOMChar	c)
{
	m_outputStream.write(c);

	m_flushWideChars = true;
}



void
XalanOutputStreamPrintWriter::write(
			const XalanDOMString&		s,
			XalanDOMString::size_type	theOffset,
			XalanDOMString::size_type	theLength)
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
			size_t			theLength)
{
	write(s,
		  0,
		  theLength);
}



void
XalanOutputStreamPrintWriter::print(
			const XalanDOMChar*			s,
			XalanDOMString::size_type	theLength)
{
	assert(s != 0);

	write(s,
		  0,
		  theLength);
}



void
XalanOutputStreamPrintWriter::print(double	d)
{
	m_buffer.clear();

	DoubleToDOMString(d, m_buffer);

	print(m_buffer);
}



void
XalanOutputStreamPrintWriter::print(int		i)
{
	m_buffer.clear();

	LongToDOMString(i, m_buffer);

	print(m_buffer);
}



void
XalanOutputStreamPrintWriter::print(long	l)
{
	m_buffer.clear();

	LongToDOMString(l, m_buffer);

	print(m_buffer);
}



void
XalanOutputStreamPrintWriter::print(const XalanDOMString&	s)
{
	write(c_wstr(s), 0, length(s));
}



void
XalanOutputStreamPrintWriter::println()
{
	write(c_wstr(s_newlineString), 0, length(s_newlineString));

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
			size_t			theLength)
{
	print(s, theLength);

	println();
}



void
XalanOutputStreamPrintWriter::println(
			const XalanDOMChar*			s,
			XalanDOMString::size_type	theLength)
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
XalanOutputStreamPrintWriter::println(int	i)
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
XalanOutputStreamPrintWriter::println(const XalanDOMString&		s)
{
	print(s);

	println();
}



XALAN_CPP_NAMESPACE_END
