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
#include "XalanNumberFormat.hpp"



#include <Include/XalanAutoPtr.hpp>



#include "DOMStringHelper.hpp"
#include "STLHelper.hpp"



XalanNumberFormat::XalanNumberFormat() :
	m_isGroupingUsed(false),
	m_groupingSeparator(s_defaultGroupingSeparator),
	m_groupingSize(3)	// Default to US values
{
}



XalanNumberFormat::~XalanNumberFormat()
{
}



XalanDOMString
XalanNumberFormat::format(double		theValue)
{
	// $$$ ToDo: Fix this!
	return applyGrouping(DoubleToDOMString(theValue));
}



XalanDOMString
XalanNumberFormat::format(int	theValue)
{
	// $$$ ToDo: Fix this!
	return applyGrouping(LongToDOMString(theValue));
}



XalanDOMString
XalanNumberFormat::format(unsigned int	theValue)
{
	// $$$ ToDo: Fix this!
	return applyGrouping(UnsignedLongToDOMString(theValue));
}



XalanDOMString
XalanNumberFormat::format(long	theValue)
{
	// $$$ ToDo: Fix this!
	return applyGrouping(LongToDOMString(theValue));
}



XalanDOMString
XalanNumberFormat::applyGrouping(const XalanDOMString& value)
/*
 * Convert a string value using the currently active values for grouping size
 * and separator; returns the converted string
 */
{
	if (!m_isGroupingUsed) return value;
	if (m_groupingSize == 0) return value;
	const unsigned int len = length(value);
	if (len == 0) return value;

	const unsigned int	bufsize = len + len/m_groupingSize + 1;

	XalanDOMChar* const		buffer = new XalanDOMChar[bufsize];

	XalanArrayAutoPtr<XalanDOMChar>		theGuard(buffer);

	XalanDOMChar*			p = buffer + bufsize - 1;

	*p-- = 0;	// null terminate

	for (unsigned int i = 0, ix = len - 1; i < len; i++, ix--)
	{
		const XalanDOMChar		c = charAt(value, ix);

		if (i && !(i% m_groupingSize))
		{
			// Could be a multiple character separator??
			for (int j= m_groupingSeparator.length()-1; j>=0; j--)
				*p-- = charAt(m_groupingSeparator, j);
		}

		*p-- = c;
	}

	return XalanDOMString(++p);
}



XalanDOMString
XalanNumberFormat::format(unsigned long	theValue)
{
	// $$$ ToDo: Fix this!
	return UnsignedLongToDOMString(theValue);
}



bool
XalanNumberFormat::isGroupingUsed() const
{
	return m_isGroupingUsed;
}



void
XalanNumberFormat::setGroupingUsed(bool bUsed)
{
	m_isGroupingUsed = bUsed;
}



void
XalanNumberFormat::setGroupingSize(unsigned long	size)
{
	assert(size > 0);

	m_groupingSize = size;
}



void
XalanNumberFormat::setGroupingSeparator(const XalanDOMString&	s)
{
	m_groupingSeparator = s;
}



static XalanDOMString	s_defaultGroupingSeparator;



const XalanDOMString&	XalanNumberFormat::s_defaultGroupingSeparator = ::s_defaultGroupingSeparator;



void
XalanNumberFormat::initialize()
{
	::s_defaultGroupingSeparator = XALAN_STATIC_UCODE_STRING(",");
}



void
XalanNumberFormat::terminate()
{
	clear(::s_defaultGroupingSeparator);
}
