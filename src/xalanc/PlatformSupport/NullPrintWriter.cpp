/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2004 The Apache Software Foundation.  All rights 
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
#include "NullPrintWriter.hpp"



XALAN_CPP_NAMESPACE_BEGIN



NullPrintWriter::NullPrintWriter() :
	PrintWriter(false)
{
}



NullPrintWriter::~NullPrintWriter()
{
}



bool
NullPrintWriter::checkError() const
{
	return false;
}



void
NullPrintWriter::close()
{
}



void
NullPrintWriter::flush()
{
}



void
NullPrintWriter::write(
			const char*		/* s */,
			size_t			/* theOffset */,
			size_t			/* theLength */)
{
}



void
NullPrintWriter::write(
			const XalanDOMChar*			/* s */,
			XalanDOMString::size_type	/* theOffset */,
			XalanDOMString::size_type	/* theLength */)
{
}



void
NullPrintWriter::write(XalanDOMChar	/* c */)
{
}



void
NullPrintWriter::write(
			const XalanDOMString&		/* s */,
			XalanDOMString::size_type	/* theOffset */,
			XalanDOMString::size_type	/* theLength */)
{
}



#if !defined(XALAN_BOOL_AS_INT)
void
NullPrintWriter::print(bool	/* b */)
{
}
#endif



void
NullPrintWriter::print(char	/* c */)
{
}



void
NullPrintWriter::print(
			const char*		/* s */,
			size_t			/* theLength */)
{
}



void
NullPrintWriter::print(
			const XalanDOMChar*			/* s */,
			XalanDOMString::size_type	/* theLength */)
{
}



void
NullPrintWriter::print(double	/* d */)
{
}



void
NullPrintWriter::print(int	/* i */)
{
}



void
NullPrintWriter::print(long	/* l */)
{
}



void
NullPrintWriter::print(const XalanDOMString&	/* s */)
{
}



void
NullPrintWriter::println()
{
}



#if !defined(XALAN_BOOL_AS_INT)
void
NullPrintWriter::println(bool	/* x */)
{
}
#endif



void
NullPrintWriter::println(char	/* x */)
{
}



void
NullPrintWriter::println(
			const char*		/* s */,
			size_t			/* theLength */)
{
}



void
NullPrintWriter::println(
			const XalanDOMChar*			/* s */,
			XalanDOMString::size_type	/* theLength */)
{
}



void
NullPrintWriter::println(double	/* x */)
{
}



void
NullPrintWriter::println(int	/* x */)
{
}



void
NullPrintWriter::println(long	/* x */)
{
}



void
NullPrintWriter::println(const XalanDOMString&	/* s */)
{
}



XALAN_CPP_NAMESPACE_END
