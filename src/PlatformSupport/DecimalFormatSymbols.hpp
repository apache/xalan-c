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
#if !defined(DECIMALFORMATSYMBOLS_HEADER_GUARD_1357924680)
#define DECIMALFORMATSYMBOLS_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <PlatformSupport/PlatformSupportDefinitions.hpp>



#include <dom/DOMString.hpp>



class XALAN_PLATFORMSUPPORT_EXPORT DecimalFormatSymbols
{
public:

	// Eventually, this constructor should take a locale to determine
	// all of the stuff it needs to know.  But locales are implemented
	// on all of our platforms yet.
	explicit
	DecimalFormatSymbols();

	DecimalFormatSymbols(const DecimalFormatSymbols&	theSource);

	~DecimalFormatSymbols();

	DecimalFormatSymbols&
	operator=(const DecimalFormatSymbols&	theRHS);

	/**
	 * Retrieve the string denoting the local currency, "$", for example
	 * 
	 * @return string used for local currency
	 */
	DOMString
	getCurrencySymbol() const
	{
		return m_currencySymbol;
	}

	/**
	 * Retrieve the character used for decimal sign, '.' for example
	 * 
	 * @return character used for decimal sign
	 */
	XMLCh
	getDecimalSeparator() const
	{
		return m_decimalSeparator;
	}

	/**
	 * Retrieve character used for a digit in a pattern
	 * 
	 * @return character used for a digit in a pattern 
	 */
	XMLCh
	getDigit() const
	{
		return m_digit;
	}

	/**
	 * Retrieve the character used for thousands separator, "," for example
	 * 
	 * @return character used for thousands separator
	 */
	XMLCh
	getGroupingSeparator() const
	{
		return m_groupingSeparator;
	}

	/**
	 * Retrieve the string used to represent infinity
	 * 
	 * @return string used to represent infinity
	 */
	DOMString
	getInfinity() const
	{
		return m_infinity;
	}

	/**
	 * Retrieve the international string denoting the local currency
	 * 
	 * @return international string denoting the local currency
	 */
	DOMString
	getInternationalCurrencySymbol() const
	{
		return m_internationalCurrencySymbol;
	}

	/**
	 * Retrieve the character used to represent minus sign
	 * 
	 * @return character used to represent minus sign
	 */
	XMLCh
	getMinusSign() const
	{
		return m_minusSign;
	}

	/**
	 * Retrieve the monetary decimal separator
	 * 
	 * @return character used to separate decimal portion of currency
	 */
	XMLCh
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
	DOMString
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
	XMLCh
	getPatternSeparator() const
	{
		return m_patternSeparator;
	}

	/**
	 * Retrieve the character used for percent sign, "%," for example
	 * 
	 * @return character used for percent sign
	 */
	XMLCh
	getPercent() const
	{
		return m_percent;
	}

	/**
	 * Retrieve the character used for per thousand sign
	 * 
	 * @return character used for per thousand sign
	 */
	XMLCh
	getPerMill() const
	{
		return m_perMill;
	}

	/**
	 * Retrieve the character used for zero
	 * 
	 * @return character used for zero
	 */
	XMLCh
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
	setCurrencySymbol(const DOMString&	theCurrencySymbol)
	{
		m_currencySymbol = theCurrencySymbol;
	}

	/**
	 * Sets the character used for decimal sign, '.' for example
	 * 
	 * @param theDecimalSeparator character used for decimal sign
	 */
	void
	setDecimalSeparator(XMLCh	theDecimalSeparator)
	{
		m_decimalSeparator = theDecimalSeparator;
	}

	/**
	 * Sets the character used for a digit in a pattern
	 * 
	 * @param theDigit character used for a digit in a pattern 
	 */
	void
	setDigit(XMLCh	theDigit)
	{
		m_digit = theDigit;
	}

	/**
	 * Sets the character used for thousands separator, "," for example
	 * 
	 * @param theGroupingSeparator character used for thousands separator
	 */
	void
	setGroupingSeparator(XMLCh	theGroupingSeparator)
	{
		m_groupingSeparator = theGroupingSeparator;
	}

	/**
	 * Sets the string used to represent infinity
	 * 
	 * @param theInfinity string used to represent infinity
	 */
	void
	setInfinity(const DOMString&	theInfinity)
	{
		m_infinity = theInfinity;
	}

	/**
	 * Sets the international string denoting the local currency
	 * 
	 * @param theInternationalCurrencySymbol international string denoting the
	 *                                       local currency
	 */
	void
	setInternationalCurrencySymbol(const DOMString&		theInternationalCurrencySymbol)
	{
		m_internationalCurrencySymbol = theInternationalCurrencySymbol;
	}

	/**
	 * Sets the character used to represent minus sign
	 * 
	 * @param theMinusSign character used to represent minus sign
	 */
	void
	setMinusSign(XMLCh	theMinusSign)
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
	setMonetaryDecimalSeparator(XMLCh	theMonetaryDecimalSeparator)
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
	setNaN(const DOMString&		theNaN)
	{
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
	setPatternSeparator(XMLCh	thePatternSeparator)
	{
		m_patternSeparator = thePatternSeparator;
	}

	/**
	 * Sets the character used for percent sign, "%," for example
	 * 
	 * @param thePercent character used for percent sign
	 */
	void
	setPercent(XMLCh	thePercent)
	{
		m_percent = thePercent;
	}

	/**
	 * Sets the character used for per thousand sign
	 * 
	 * @param thePerMill character used for per thousand sign
	 */
	void
	setPerMill(XMLCh	thePerMill)
	{
		m_perMill = thePerMill;
	}

	/**
	 * Sets the character used for zero
	 * 
	 * @param theZeroDigit character used for zero
	 */
	void
	setZeroDigit(XMLCh	theZeroDigit)
	{
		m_zeroDigit = theZeroDigit;
	}

private:

	// Not implemented
	bool
	operator==(const DecimalFormatSymbols&);

	DOMString		m_currencySymbol;

	XMLCh			m_decimalSeparator;
	XMLCh			m_digit;
	XMLCh			m_groupingSeparator;

	DOMString		m_infinity;
	DOMString		m_internationalCurrencySymbol;

	XMLCh			m_minusSign;
	XMLCh			m_monetaryDecimalSeparator;

	DOMString		m_NaN;

	XMLCh			m_patternSeparator;
	XMLCh			m_percent;
	XMLCh			m_perMill;
	XMLCh			m_zeroDigit;
};



#endif	// DECIMALFORMATSYMBOLS_HEADER_GUARD_1357924680
