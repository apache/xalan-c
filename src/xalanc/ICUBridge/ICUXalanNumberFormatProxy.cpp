/*
 * Copyright 1999-2004 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ICUXalanNumberFormatProxy.hpp"



#include <unicode/dcfmtsym.h>



U_NAMESPACE_USE



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



#include "ICUBridge.hpp"



XALAN_CPP_NAMESPACE_BEGIN



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



XALAN_CPP_NAMESPACE_END
