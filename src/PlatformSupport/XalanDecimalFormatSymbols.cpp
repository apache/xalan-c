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
#include "XalanDecimalFormatSymbols.hpp"
#include "DOMStringHelper.hpp"
#include "XalanUnicode.hpp"



static XalanDOMChar		theNaNDefault[] =
{
	XalanUnicode::charLetter_N,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_N,
	0
};



static XalanDOMChar		theInfinityDefault[] =
{
	XalanUnicode::charLetter_I,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_f,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_y,
	0
};



static XalanDOMChar		theCurrencySymbol[] =
{
	XalanUnicode::charDollarSign,
	0
};



XalanDecimalFormatSymbols::XalanDecimalFormatSymbols() :
	m_currencySymbol(theCurrencySymbol),
	m_decimalSeparator(XalanUnicode::charFullStop),
	m_digit(0),
	m_groupingSeparator(XalanUnicode::charComma),
	m_infinity(theInfinityDefault),
	m_internationalCurrencySymbol(),
	m_minusSign(XalanUnicode::charHyphenMinus),
	m_monetaryDecimalSeparator(XalanUnicode::charFullStop),
	m_NaN(theNaNDefault),
	m_patternSeparator(XalanUnicode::charSemicolon),
	m_percent(XalanUnicode::charPercentSign),
	m_perMill(0),
	m_zeroDigit(XalanUnicode::charDigit_0)
{
}



XalanDecimalFormatSymbols::XalanDecimalFormatSymbols(const XalanDecimalFormatSymbols&	theSource) :
	m_currencySymbol(theSource.m_currencySymbol),
	m_decimalSeparator(theSource.m_decimalSeparator),
	m_digit(theSource.m_digit),
	m_groupingSeparator(theSource.m_groupingSeparator),
	m_infinity(theSource.m_infinity),
	m_internationalCurrencySymbol(theSource.m_internationalCurrencySymbol),
	m_minusSign(theSource.m_minusSign),
	m_monetaryDecimalSeparator(theSource.m_monetaryDecimalSeparator),
	m_NaN(theSource.m_NaN),
	m_patternSeparator(theSource.m_patternSeparator),
	m_percent(theSource.m_percent),
	m_perMill(theSource.m_perMill),
	m_zeroDigit(theSource.m_zeroDigit)
{
}



XalanDecimalFormatSymbols::~XalanDecimalFormatSymbols()
{
}



XalanDecimalFormatSymbols&
XalanDecimalFormatSymbols::operator=(const XalanDecimalFormatSymbols&		theRHS)
{
	if (&theRHS != this)
	{
		m_currencySymbol = theRHS.m_currencySymbol;
		m_decimalSeparator = theRHS.m_decimalSeparator;
		m_digit = theRHS.m_digit;
		m_groupingSeparator = theRHS.m_groupingSeparator;
		m_infinity = theRHS.m_infinity;
		m_internationalCurrencySymbol = theRHS.m_internationalCurrencySymbol;
		m_minusSign = theRHS.m_minusSign;
		m_monetaryDecimalSeparator = theRHS.m_monetaryDecimalSeparator;
		m_NaN = theRHS.m_NaN;
		m_patternSeparator = theRHS.m_patternSeparator;
		m_percent = theRHS.m_percent;
		m_perMill = theRHS.m_perMill;
		m_zeroDigit = theRHS.m_zeroDigit;
	}

	return *this;
}



bool
XalanDecimalFormatSymbols::operator==(const XalanDecimalFormatSymbols&	theRHS) const
{
	return 
		m_currencySymbol == theRHS.m_currencySymbol &&
		m_decimalSeparator == theRHS.m_decimalSeparator &&
		m_digit == theRHS.m_digit &&
		m_groupingSeparator == theRHS.m_groupingSeparator &&
		m_infinity == theRHS.m_infinity &&
		m_internationalCurrencySymbol == theRHS.m_internationalCurrencySymbol &&
		m_minusSign == theRHS.m_minusSign &&
		m_monetaryDecimalSeparator == theRHS.m_monetaryDecimalSeparator &&
		m_NaN == theRHS.m_NaN &&
		m_patternSeparator == theRHS.m_patternSeparator &&
		m_percent == theRHS.m_percent &&
		m_perMill == theRHS.m_perMill &&
		m_zeroDigit == theRHS.m_zeroDigit;
}
