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
#include "DOMStringPrintWriter.hpp"



#include <vector>



#include <PlatformSupport/DOMStringHelper.hpp>



DOMStringPrintWriter::DOMStringPrintWriter(const XalanDOMString&	theBaseString) :
	PrintWriter(true),
	m_outputString(theBaseString)
{
}



DOMStringPrintWriter::~DOMStringPrintWriter()
{
}



bool
DOMStringPrintWriter::checkError() const
{
	return false;
}



void
DOMStringPrintWriter::close()
{
}


void
DOMStringPrintWriter::flush()
{
}



void
DOMStringPrintWriter::write(
			const char*		s,
			unsigned int	theOffset,
			unsigned int	theLength)
{
	write(XalanDOMString(s), theOffset, theLength);
}



void
DOMStringPrintWriter::write(
			const XalanDOMChar*		s,
			unsigned int			theOffset,
			unsigned int			theLength)
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::vector;
	using std::copy;
#endif

	assert(s != 0);
	assert(theLength == UINT_MAX || length(s) >= theOffset + theLength);

	if (theLength == UINT_MAX)
	{
			m_outputString += (s + theOffset);
	}
	else
	{
		vector<XalanDOMChar>	theBuffer(theLength + 1);

		// We'll copy the characters into the vector first.
		copy(s + theOffset,
			 s + theOffset + theLength,
			 theBuffer.begin());

		// Now append a terminated 0.
		theBuffer.back() = 0;

		// Now append the data.
		m_outputString += theBuffer.begin();
	}
}



void
DOMStringPrintWriter::write(XalanDOMChar	c)
{
	m_outputString += c;
}



void
DOMStringPrintWriter::write(
			const XalanDOMString&	s,
			unsigned int			theOffset,
			unsigned int			theLength)
{
	assert(s != 0);
	assert(theLength == UINT_MAX || length(s) >= theOffset + theLength);

	if (theOffset == 0 && theLength == UINT_MAX)
	{
		m_outputString += s;
	}
	else
	{
		m_outputString += substring(s, theOffset, theOffset + theLength);
	}
}



void
DOMStringPrintWriter::print(bool	b)
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
DOMStringPrintWriter::print(char	c)
{
	write(c);
}



void
DOMStringPrintWriter::print(
			const char*		s,
			unsigned int	theLength)
{
	write(s,
		  0,
		  theLength);
}



void
DOMStringPrintWriter::print(
			const XalanDOMChar*		s,
			unsigned int			theLength)
{
	write(s,
		  0,
		  theLength);
}



void
DOMStringPrintWriter::print(double	d)
{
	m_outputString += DoubleToDOMString(d);
}



void
DOMStringPrintWriter::print(int	i)
{
	m_outputString += LongToDOMString(i);
}



void
DOMStringPrintWriter::print(long	l)
{
	m_outputString += LongToDOMString(l);
}



void
DOMStringPrintWriter::print(const XalanDOMString&	s)
{
	m_outputString += s;
}



void
DOMStringPrintWriter::println()
{
	m_outputString += XALAN_STATIC_UCODE_STRING("\n");
}



void
DOMStringPrintWriter::println(bool	b)
{
	print(b);

	println();
}



void
DOMStringPrintWriter::println(char	c)
{
	print(c);

	println();
}



void
DOMStringPrintWriter::println(
			const char*		s,
			unsigned int	theLength)
{
	print(s, theLength);

	println();
}



void
DOMStringPrintWriter::println(
			const XalanDOMChar*		s,
			unsigned int			theLength)
{
	print(s, theLength);

	println();
}



void
DOMStringPrintWriter::println(double	d)
{
	print(d);

	println();
}



void
DOMStringPrintWriter::println(int		i)
{
	print(i);

	println();
}



void
DOMStringPrintWriter::println(long	l)
{
	print(l);

	println();
}



void
DOMStringPrintWriter::println(const XalanDOMString&	s)
{
	print(s);

	println();
}
