/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2000 The Apache Software Foundation.  All rights 
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

#include "ICUXalanNumberFormatProxy.hpp"



#include <vector>



#include <unicode/dcfmtsym.h>
#include <unicode/decimfmt.h>



#include <PlatformSupport/DOMStringHelper.hpp>



inline UnicodeString
XalanDOMStringToUnicodeString(const XalanDOMString&		theString)
{
	const XalanDOMChar* const	theBuffer = c_wstr(theString);

	if (theBuffer == 0)
	{
		return UnicodeString();
	}
	else
	{
		return UnicodeString(theBuffer, length(theBuffer));
	}
}



inline XalanDOMString
UnicodeStringToXalanDOMString(const UnicodeString&	theString)
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::vector;
#endif

	vector<XalanDOMChar>	theBuffer;

	const int32_t			theLength = theString.length();

	theBuffer.reserve(theLength);

	for (unsigned int i = 0; i < theLength; ++i)
	{
		theBuffer.push_back(theString[i]);
	}

	return XalanDOMString(&theBuffer[0], theBuffer.size());
}



ICUXalanNumberFormatProxy::ICUXalanNumberFormatProxy() :
	XalanNumberFormat(),
	m_decimalFormat(0)
{
	UErrorCode	theResult = U_ZERO_ERROR;

	m_decimalFormat = new DecimalFormat(theResult);
}



ICUXalanNumberFormatProxy::~ICUXalanNumberFormatProxy()
{
	delete m_decimalFormat;
}



XalanDOMString
ICUXalanNumberFormatProxy::format(double	theValue)
{
	UnicodeString	theResult;

	m_decimalFormat->format(theValue, theResult);

	return UnicodeStringToXalanDOMString(theResult);
}



XalanDOMString
ICUXalanNumberFormatProxy::format(int	theValue)
{
	UnicodeString	theResult;

	m_decimalFormat->format(static_cast<long int>(theValue), theResult);

	return UnicodeStringToXalanDOMString(theResult);
}



XalanDOMString
ICUXalanNumberFormatProxy::format(unsigned int	theValue)
{
	UnicodeString	theResult;

	m_decimalFormat->format(static_cast<long int>(theValue), theResult);

	return UnicodeStringToXalanDOMString(theResult);
}



XalanDOMString
ICUXalanNumberFormatProxy::format(long	theValue)
{
	UnicodeString	theResult;

	m_decimalFormat->format(theValue, theResult);

	return UnicodeStringToXalanDOMString(theResult);
}



XalanDOMString
ICUXalanNumberFormatProxy::format(unsigned long	theValue)
{
	UnicodeString	theResult;

	m_decimalFormat->format(static_cast<long int>(theValue), theResult);

	return UnicodeStringToXalanDOMString(theResult);
}



bool
ICUXalanNumberFormatProxy::isGroupingUsed() const
{
	return m_decimalFormat->isGroupingUsed() ? true : false;
}



void
ICUXalanNumberFormatProxy::setGroupingUsed(bool bUsed)
{
	m_decimalFormat->setGroupingUsed(bUsed);
}



void
ICUXalanNumberFormatProxy::setGroupingSize(unsigned long	size)
{
	assert(size > 0);

	m_decimalFormat->setGroupingSize(size);
}



void
ICUXalanNumberFormatProxy::setGroupingSeparator(const XalanDOMString&	s)
{
	const DecimalFormatSymbols* const	theCurrentSymbols =
				m_decimalFormat->getDecimalFormatSymbols();
	assert(theCurrentSymbols != 0);

	DecimalFormatSymbols* const		theNewSymbols =
		new DecimalFormatSymbols(*theCurrentSymbols);

	theNewSymbols->setGroupingSeparator(charAt(s, 0));

	m_decimalFormat->adoptDecimalFormatSymbols(theNewSymbols);
}
