/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the  "License");
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
#include "XalanDecimalFormatSymbols.hpp"
#include "DOMStringHelper.hpp"
#include "XalanUnicode.hpp"



XALAN_CPP_NAMESPACE_BEGIN



static XalanDOMChar     theNaNDefault[] =
{
    XalanUnicode::charLetter_N,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_N,
    0
};



static XalanDOMChar     theInfinityDefault[] =
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



static XalanDOMChar     theCurrencySymbol[] =
{
    XalanUnicode::charDollarSign,
    0
};



XalanDecimalFormatSymbols::XalanDecimalFormatSymbols(MemoryManager&  theManager) :
    m_currencySymbol(theCurrencySymbol,theManager),
    m_decimalSeparator(XalanUnicode::charFullStop),
    m_digit(XalanUnicode::charNumberSign),
    m_groupingSeparator(XalanUnicode::charComma),
    m_infinity(theInfinityDefault, theManager),
    m_internationalCurrencySymbol(theManager),
    m_minusSign(XalanUnicode::charHyphenMinus),
    m_monetaryDecimalSeparator(XalanUnicode::charFullStop),
    m_NaN(theNaNDefault,theManager),
    m_patternSeparator(XalanUnicode::charSemicolon),
    m_percent(XalanUnicode::charPercentSign),
    m_perMill(XalanUnicode::charPerMilleSign),
    m_zeroDigit(XalanUnicode::charDigit_0)
{
}



XalanDecimalFormatSymbols::XalanDecimalFormatSymbols(const XalanDecimalFormatSymbols&   theSource,
                                                     MemoryManager&                 theManager) :
    m_currencySymbol(theSource.m_currencySymbol,theManager),
    m_decimalSeparator(theSource.m_decimalSeparator),
    m_digit(theSource.m_digit),
    m_groupingSeparator(theSource.m_groupingSeparator),
    m_infinity(theSource.m_infinity,theManager),
    m_internationalCurrencySymbol(theSource.m_internationalCurrencySymbol, theManager),
    m_minusSign(theSource.m_minusSign),
    m_monetaryDecimalSeparator(theSource.m_monetaryDecimalSeparator),
    m_NaN(theSource.m_NaN,theManager),
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
XalanDecimalFormatSymbols::operator=(const XalanDecimalFormatSymbols&       theRHS)
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
XalanDecimalFormatSymbols::operator==(const XalanDecimalFormatSymbols&  theRHS) const
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



XALAN_CPP_NAMESPACE_END
