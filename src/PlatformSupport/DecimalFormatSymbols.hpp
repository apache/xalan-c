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



#if defined(__GNUC__)
// This doesn't exist in g++ yet
#else
#include <locale>
#endif


#include <dom/DOMString.hpp>



class XALAN_PLATFORMSUPPORT_EXPORT DecimalFormatSymbols
{
public:

#if defined(__GNUC__)
	DecimalFormatSymbols();
#else
	explicit
	DecimalFormatSymbols(const std::locale&	theLocale = std::locale());
#endif

	DecimalFormatSymbols(const DecimalFormatSymbols&	theSource);

	~DecimalFormatSymbols();

	DecimalFormatSymbols&
	operator=(const DecimalFormatSymbols&	theRHS);

	DOMString
	getCurrencySymbol() const
	{
		return m_currencySymbol;
	}

	XMLCh
	getDecimalSeparator() const
	{
		return m_decimalSeparator;
	}

	XMLCh
	getDigit() const
	{
		return m_digit;
	}

	XMLCh
	getGroupingSeparator() const
	{
		return m_groupingSeparator;
	}

	DOMString
	getInfinity() const
	{
		return m_infinity;
	}

	DOMString
	getInternationalCurrencySymbol() const
	{
		return m_internationalCurrencySymbol;
	}

	XMLCh
	getMinusSign() const
	{
		return m_minusSign;
	}

	XMLCh
	getMonetaryDecimalSeparator() const
	{
		return m_monetaryDecimalSeparator;
	}

	DOMString
	getNaN() const
	{
		return m_NaN;
	}

	XMLCh
	getPatternSeparator() const
	{
		return m_patternSeparator;
	}

	XMLCh
	getPercent() const
	{
		return m_percent;
	}

	XMLCh
	getPerMill() const
	{
		return m_perMill;
	}

	XMLCh
	getZeroDigit() const
	{
		return m_zeroDigit;
	}

	void
	setCurrencySymbol(const DOMString&	theCurrencySymbol)
	{
		m_currencySymbol = theCurrencySymbol;
	}

	void
	setDecimalSeparator(XMLCh	theDecimalSeparator)
	{
		m_decimalSeparator = theDecimalSeparator;
	}

	void
	setDigit(XMLCh	theDigit)
	{
		m_digit = theDigit;
	}

	void
	setGroupingSeparator(XMLCh	theGoupingSeparator)
	{
		m_groupingSeparator = theGoupingSeparator;
	}

	void
	setInfinity(const DOMString&	theInfinity)
	{
		m_infinity = theInfinity;
	}

	void
	setInternationalCurrencySymbol(const DOMString&		theInternationalCurrencySymbol)
	{
		m_internationalCurrencySymbol = theInternationalCurrencySymbol;
	}

	void
	setMinusSign(XMLCh	theMinusSign)
	{
		m_minusSign = theMinusSign;
	}

	void
	setMonetaryDecimalSeparator(XMLCh	theMonetaryDecimalSeparator)
	{
		m_monetaryDecimalSeparator = theMonetaryDecimalSeparator;
	}

	void
	setNaN(const DOMString&		theNaN)
	{
		m_NaN = theNaN;
	}
				   
	void
	setPatternSeparator(XMLCh	thePatternSeparator)
	{
		m_patternSeparator = thePatternSeparator;
	}

	void
	setPercent(XMLCh	thePercent)
	{
		m_percent = thePercent;
	}

	void
	setPerMill(XMLCh	thePerMill)
	{
		m_perMill = thePerMill;
	}

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
