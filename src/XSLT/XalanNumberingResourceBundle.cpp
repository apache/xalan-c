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
  THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
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

#include "XalanNumberingResourceBundle.hpp"



XalanNumberingResourceBundle::XalanNumberingResourceBundle(
		const XalanDOMString&			theLanguage,
		const XalanDOMString&			theUILanguage,
		const XalanDOMString&			theHelpLanguage,
		const XalanDOMCharVectorType&	theAlphabet,
		const XalanDOMCharVectorType&	theTraditionalAlphabet,
		eOrientation					theOrientation,
		eNumberingMethod				theNumberingMethod,
		eMultiplierOrder				theMultiplierOrder,
		int								theMaxNumericalValue,
		const IntVectorType&			theNumberGroups,
		const IntVectorType&			theMultipliers,
		const XalanDOMCharVectorType&	theZeroChar,
		const XalanDOMCharVectorType&	theMultiplierChars,
		const DigitsTableVectorType&	theDigitsTable,
		const IntVectorType&			theDigitsTableTable) :
	m_language(theHelpLanguage),
	m_uiLanguage(theLanguage),
	m_helpLanguage(theUILanguage),
	m_alphabet(theAlphabet),
	m_traditionalAlphabet(theTraditionalAlphabet),
	m_orientation(theOrientation),
	m_numberingMethod(theNumberingMethod),
	m_multiplierOrder(theMultiplierOrder),
	m_maxNumericalValue(theMaxNumericalValue),
	m_numberGroups(theNumberGroups),
	m_multipliers(theMultipliers),
	m_zeroChar(theZeroChar),
	m_multiplierChars(theMultiplierChars),
	m_digitsTable(theDigitsTable),
	m_digitsTableTable(theDigitsTableTable)
{
}



XalanNumberingResourceBundle::XalanNumberingResourceBundle()
{
}



XalanNumberingResourceBundle::XalanNumberingResourceBundle(const XalanNumberingResourceBundle&	theSource) :
	m_language(theSource.m_language),
	m_uiLanguage(theSource.m_uiLanguage),
	m_helpLanguage(theSource.m_helpLanguage),
	m_alphabet(theSource.m_alphabet),
	m_traditionalAlphabet(theSource.m_traditionalAlphabet),
	m_orientation(theSource.m_orientation),
	m_numberingMethod(theSource.m_numberingMethod),
	m_multiplierOrder(theSource.m_multiplierOrder),
	m_maxNumericalValue(theSource.m_maxNumericalValue),
	m_numberGroups(theSource.m_numberGroups),
	m_multipliers(theSource.m_multipliers),
	m_zeroChar(theSource.m_zeroChar),
	m_multiplierChars(theSource.m_multiplierChars),
	m_digitsTable(theSource.m_digitsTable),
	m_digitsTableTable(theSource.m_digitsTableTable)
{
}



XalanNumberingResourceBundle::~XalanNumberingResourceBundle()
{
}



void
XalanNumberingResourceBundle::swap(XalanNumberingResourceBundle&	theOther)
{
	m_language.swap(theOther.m_language);

	m_uiLanguage.swap(theOther.m_uiLanguage);

	m_helpLanguage.swap(theOther.m_helpLanguage);

	m_alphabet.swap(theOther.m_alphabet);

	m_traditionalAlphabet.swap(theOther.m_traditionalAlphabet);

	{
		const eOrientation	temp = m_orientation;

		m_orientation = theOther.m_orientation;

		theOther.m_orientation = temp;
	}

	{
		const eNumberingMethod	temp = m_numberingMethod;

		m_numberingMethod = theOther.m_numberingMethod;

		theOther.m_numberingMethod = temp;
	}

	{
		const eMultiplierOrder	temp = m_multiplierOrder;

		m_multiplierOrder = theOther.m_multiplierOrder;

		theOther.m_multiplierOrder = temp;
	}

	{
		const int	temp = m_maxNumericalValue;

		m_maxNumericalValue = theOther.m_maxNumericalValue;

		theOther.m_maxNumericalValue = temp;
	}

	m_numberGroups.swap(theOther.m_numberGroups);

	m_multipliers.swap(theOther.m_multipliers);

	m_zeroChar.swap(theOther.m_zeroChar);

	m_multiplierChars.swap(theOther.m_multiplierChars);

	m_digitsTable.swap(theOther.m_digitsTable);

	m_digitsTableTable.swap(theOther.m_digitsTableTable);
}
