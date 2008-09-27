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
#if !defined(XALANDECIMALFORMATSYMBOLS_HEADER_GUARD_1357924680)
#define XALANDECIMALFORMATSYMBOLS_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



#include <cassert>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_PLATFORMSUPPORT_EXPORT XalanDecimalFormatSymbols
{
public:

    // Eventually, this constructor should take a locale to determine
    // all of the stuff it needs to know.  But locales are implemented
    // on all of our platforms yet.
    explicit
    XalanDecimalFormatSymbols(MemoryManager&  theManager);

    XalanDecimalFormatSymbols(const XalanDecimalFormatSymbols&  theSource,
                                MemoryManager&              theManager);

    ~XalanDecimalFormatSymbols();

    XalanDecimalFormatSymbols&
    operator=(const XalanDecimalFormatSymbols&  theRHS);

    /**
     * Retrieve the string denoting the local currency, "$", for example
     * 
     * @return string used for local currency
     */
    const XalanDOMString&
    getCurrencySymbol() const
    {
        return m_currencySymbol;
    }

    /**
     * Retrieve the character used for decimal sign, '.' for example
     * 
     * @return character used for decimal sign
     */
    XalanDOMChar
    getDecimalSeparator() const
    {
        return m_decimalSeparator;
    }

    /**
     * Retrieve character used for a digit in a pattern
     * 
     * @return character used for a digit in a pattern 
     */
    XalanDOMChar
    getDigit() const
    {
        return m_digit;
    }

    /**
     * Retrieve the character used for thousands separator, "," for example
     * 
     * @return character used for thousands separator
     */
    XalanDOMChar
    getGroupingSeparator() const
    {
        return m_groupingSeparator;
    }

    /**
     * Retrieve the string used to represent infinity
     * 
     * @return string used to represent infinity
     */
    const XalanDOMString&
    getInfinity() const
    {
        return m_infinity;
    }

    /**
     * Retrieve the international string denoting the local currency
     * 
     * @return international string denoting the local currency
     */
    const XalanDOMString&
    getInternationalCurrencySymbol() const
    {
        return m_internationalCurrencySymbol;
    }

    /**
     * Retrieve the character used to represent minus sign
     * 
     * @return character used to represent minus sign
     */
    XalanDOMChar
    getMinusSign() const
    {
        return m_minusSign;
    }

    /**
     * Retrieve the monetary decimal separator
     * 
     * @return character used to separate decimal portion of currency
     */
    XalanDOMChar
    getMonetaryDecimalSeparator() const
    {
        return m_monetaryDecimalSeparator;
    }

    /**
     * Retrieve the string used for a numeric value that cannot be represented
     * as a number
     * 
     * @return string representing "not a number" value
     */
    const XalanDOMString&
    getNaN() const
    {
        return m_NaN;
    }

    /**
     * Retrieve the character used to separate positive and negative
     * subpatterns in a pattern
     * 
     * @return character used to separate positive and negative subpatterns
     */
    XalanDOMChar
    getPatternSeparator() const
    {
        return m_patternSeparator;
    }

    /**
     * Retrieve the character used for percent sign, "%," for example
     * 
     * @return character used for percent sign
     */
    XalanDOMChar
    getPercent() const
    {
        return m_percent;
    }

    /**
     * Retrieve the character used for per thousand sign
     * 
     * @return character used for per thousand sign
     */
    XalanDOMChar
    getPerMill() const
    {
        return m_perMill;
    }

    /**
     * Retrieve the character used for zero
     * 
     * @return character used for zero
     */
    XalanDOMChar
    getZeroDigit() const
    {
        return m_zeroDigit;
    }

    /**
     * Sets the string denoting the local currency, "$", for example
     * 
     * @param theCurrencySymbol symbol used for local currency
     */
    void
    setCurrencySymbol(const XalanDOMString&     theCurrencySymbol)
    {
        m_currencySymbol = theCurrencySymbol;
    }

    /**
     * Sets the string denoting the local currency, "$", for example
     * 
     * @param theCurrencySymbol symbol used for local currency
     */
    void
    setCurrencySymbol(const XalanDOMChar*   theCurrencySymbol)
    {
        assert(theCurrencySymbol != 0);

        m_currencySymbol = theCurrencySymbol;
    }

    /**
     * Sets the character used for decimal sign, '.' for example
     * 
     * @param theDecimalSeparator character used for decimal sign
     */
    void
    setDecimalSeparator(XalanDOMChar    theDecimalSeparator)
    {
        m_decimalSeparator = theDecimalSeparator;
    }

    /**
     * Sets the character used for a digit in a pattern
     * 
     * @param theDigit character used for a digit in a pattern 
     */
    void
    setDigit(XalanDOMChar   theDigit)
    {
        m_digit = theDigit;
    }

    /**
     * Sets the character used for thousands separator, "," for example
     * 
     * @param theGroupingSeparator character used for thousands separator
     */
    void
    setGroupingSeparator(XalanDOMChar   theGroupingSeparator)
    {
        m_groupingSeparator = theGroupingSeparator;
    }

    /**
     * Sets the string used to represent infinity
     * 
     * @param theInfinity string used to represent infinity
     */
    void
    setInfinity(const XalanDOMString&   theInfinity)
    {
        m_infinity = theInfinity;
    }

    /**
     * Sets the string used to represent infinity
     * 
     * @param theInfinity string used to represent infinity
     */
    void
    setInfinity(const XalanDOMChar*     theInfinity)
    {
        assert(theInfinity != 0);

        m_infinity = theInfinity;
    }

    /**
     * Sets the international string denoting the local currency
     * 
     * @param theInternationalCurrencySymbol international string denoting the
     *                                       local currency
     */
    void
    setInternationalCurrencySymbol(const XalanDOMString&    theInternationalCurrencySymbol)
    {
        m_internationalCurrencySymbol = theInternationalCurrencySymbol;
    }

    /**
     * Sets the international string denoting the local currency
     * 
     * @param theInternationalCurrencySymbol international string denoting the
     *                                       local currency
     */
    void
    setInternationalCurrencySymbol(const XalanDOMChar*  theInternationalCurrencySymbol)
    {
        assert(theInternationalCurrencySymbol != 0);

        m_internationalCurrencySymbol = theInternationalCurrencySymbol;
    }

    /**
     * Sets the character used to represent minus sign
     * 
     * @param theMinusSign character used to represent minus sign
     */
    void
    setMinusSign(XalanDOMChar   theMinusSign)
    {
        m_minusSign = theMinusSign;
    }

    /**
     * Sets the monetary decimal separator
     * 
     * @param theMonetaryDecimalSeparator character used to separate decimal
     *                                    portion of currency
     */
    void
    setMonetaryDecimalSeparator(XalanDOMChar    theMonetaryDecimalSeparator)
    {
        m_monetaryDecimalSeparator = theMonetaryDecimalSeparator;
    }

    /**
     * Sets the string used for a numeric value that cannot be represented
     * as a number
     * 
     * @param theNaN string representing "not a number" value
     */
    void
    setNaN(const XalanDOMString&    theNaN)
    {
        m_NaN = theNaN;
    }

    /**
     * Sets the string used for a numeric value that cannot be represented
     * as a number
     * 
     * @param theNaN string representing "not a number" value
     */
    void
    setNaN(const XalanDOMChar*  theNaN)
    {
        assert(theNaN != 0);

        m_NaN = theNaN;
    }

    /**
     * Sets the character used to separate positive and negative subpatterns in
     * a pattern
     * 
     * @param thePatternSeparator character used to separate positive and
     *                            negative subpatterns
     */
    void
    setPatternSeparator(XalanDOMChar    thePatternSeparator)
    {
        m_patternSeparator = thePatternSeparator;
    }

    /**
     * Sets the character used for percent sign, "%," for example
     * 
     * @param thePercent character used for percent sign
     */
    void
    setPercent(XalanDOMChar thePercent)
    {
        m_percent = thePercent;
    }

    /**
     * Sets the character used for per thousand sign
     * 
     * @param thePerMill character used for per thousand sign
     */
    void
    setPerMill(XalanDOMChar thePerMill)
    {
        m_perMill = thePerMill;
    }

    /**
     * Sets the character used for zero
     * 
     * @param theZeroDigit character used for zero
     */
    void
    setZeroDigit(XalanDOMChar   theZeroDigit)
    {
        m_zeroDigit = theZeroDigit;
    }

    bool
    operator==(const XalanDecimalFormatSymbols&     theRHS) const;

    bool
    operator!=(const XalanDecimalFormatSymbols&     theRHS) const
    {
        return !operator==(theRHS);
    }

private:
    // not implemented
    XalanDecimalFormatSymbols();
    XalanDecimalFormatSymbols(const XalanDecimalFormatSymbols&);

    XalanDOMString  m_currencySymbol;

    XalanDOMChar    m_decimalSeparator;
    XalanDOMChar    m_digit;
    XalanDOMChar    m_groupingSeparator;

    XalanDOMString  m_infinity;
    XalanDOMString  m_internationalCurrencySymbol;

    XalanDOMChar    m_minusSign;
    XalanDOMChar    m_monetaryDecimalSeparator;

    XalanDOMString  m_NaN;

    XalanDOMChar    m_patternSeparator;
    XalanDOMChar    m_percent;
    XalanDOMChar    m_perMill;
    XalanDOMChar    m_zeroDigit;
};



XALAN_CPP_NAMESPACE_END



#endif  // XALANDECIMALFORMATSYMBOLS_HEADER_GUARD_1357924680
