/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2000-2002 The Apache Software Foundation.  All rights 
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



#include <unicode/dcfmtsym.h>



#if U_ICU_VERSION_MAJOR_NUM >= 2
// this is the ICU's macro for using namespace ...
U_NAMESPACE_USE
#endif



#include <PlatformSupport/DOMStringHelper.hpp>



#include "ICUBridge.hpp"



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
	UnicodeString	theUnicodeResult;

	m_decimalFormat->format(theValue, theUnicodeResult);

	return ICUBridge::UnicodeStringToXalanDOMString(theUnicodeResult);
}



void
ICUXalanNumberFormatProxy::format(
			double				theValue,
			XalanDOMString&		theResult)
{
	UnicodeString	theUnicodeResult;

	m_decimalFormat->format(theValue, theUnicodeResult);

	ICUBridge::UnicodeStringToXalanDOMString(theUnicodeResult, theResult);
}



XalanDOMString
ICUXalanNumberFormatProxy::format(int	theValue)
{
	UnicodeString	theUnicodeResult;

	m_decimalFormat->format(int32_t(theValue), theUnicodeResult);

	return ICUBridge::UnicodeStringToXalanDOMString(theUnicodeResult);
}



void
ICUXalanNumberFormatProxy::format(
			int					theValue,
			XalanDOMString&		theResult)
{
	UnicodeString	theUnicodeResult;

	m_decimalFormat->format(int32_t(theValue), theUnicodeResult);

	ICUBridge::UnicodeStringToXalanDOMString(theUnicodeResult, theResult);
}



XalanDOMString
ICUXalanNumberFormatProxy::format(unsigned int	theValue)
{
	UnicodeString	theUnicodeResult;

	m_decimalFormat->format(int32_t(theValue), theUnicodeResult);

	return ICUBridge::UnicodeStringToXalanDOMString(theUnicodeResult);
}



void
ICUXalanNumberFormatProxy::format(
			unsigned int		theValue,
			XalanDOMString&		theResult)
{
	UnicodeString	theUnicodeResult;

	m_decimalFormat->format(int32_t(theValue), theUnicodeResult);

	ICUBridge::UnicodeStringToXalanDOMString(theUnicodeResult, theResult);
}



XalanDOMString
ICUXalanNumberFormatProxy::format(long	theValue)
{
	UnicodeString	theUnicodeResult;

	m_decimalFormat->format(int32_t(theValue), theUnicodeResult);

	return ICUBridge::UnicodeStringToXalanDOMString(theUnicodeResult);
}



void
ICUXalanNumberFormatProxy::format(
			long				theValue,
			XalanDOMString&		theResult)
{
	UnicodeString	theUnicodeResult;

	m_decimalFormat->format(int32_t(theValue), theUnicodeResult);

	ICUBridge::UnicodeStringToXalanDOMString(theUnicodeResult, theResult);
}



XalanDOMString
ICUXalanNumberFormatProxy::format(unsigned long	theValue)
{
	UnicodeString	theUnicodeResult;

	m_decimalFormat->format(int32_t(theValue), theUnicodeResult);

	return ICUBridge::UnicodeStringToXalanDOMString(theUnicodeResult);
}



void
ICUXalanNumberFormatProxy::format(
			unsigned long		theValue,
			XalanDOMString&		theResult)
{
	UnicodeString	theUnicodeResult;

	m_decimalFormat->format(int32_t(theValue), theUnicodeResult);

	ICUBridge::UnicodeStringToXalanDOMString(theUnicodeResult, theResult);
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

	theNewSymbols->setSymbol(
		DecimalFormatSymbols::kGroupingSeparatorSymbol,
		ICUBridge::XalanDOMStringToUnicodeString(s));

	m_decimalFormat->adoptDecimalFormatSymbols(theNewSymbols);
}
