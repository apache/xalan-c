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

#if !defined(XALAN_NUMBERINGRESOURCEBUNDLE_HEADER_GUARD)
#define XALAN_NUMBERINGRESOURCEBUNDLE_HEADER_GUARD



// Base include file.  Must be first.
#include <XSLT/XSLTDefinitions.hpp>



#include <vector>



#include <PlatformSupport/DOMStringHelper.hpp>



class XalanNumberingResourceBundle
{
public:

	typedef unsigned long							NumberType;

#if defined(XALAN_NO_NAMESPACES)
	typedef vector<NumberType>						NumberTypeVectorType;
	typedef vector<XalanDOMCharVectorType>			DigitsTableVectorType;
#else
	typedef std::vector<unsigned long>				NumberTypeVectorType;
	typedef std::vector<XalanDOMCharVectorType>		DigitsTableVectorType;
#endif

	// Enum to describe language orientation.  (English is left-to-right,
	// Hebrew is right-to-left, etc.)
	enum eOrientation { eLeftToRight, eRightToLeft, eTopToBottom, eBottomToTop };

	// Enum to describe how numbering is done.
	enum eNumberingMethod { eAdditive, eMultiplicativeAdditive };

	// Enum to describe the where the multiplier goes.
	enum eMultiplierOrder { eFollows, ePrecedes };


	XalanNumberingResourceBundle(
		const XalanDOMString&			theLanguage,
		const XalanDOMString&			theUILanguage,
		const XalanDOMString&			theHelpLanguage,
		const XalanDOMCharVectorType&	theAlphabet,
		const XalanDOMCharVectorType&	theTraditionalAlphabet,
		eOrientation					theOrientation,
		eNumberingMethod				theNumberingMethod,
		eMultiplierOrder				theMultiplierOrder,
		NumberType						theMaxNumericalValue,
		const NumberTypeVectorType&		theNumberGroups,
		const NumberTypeVectorType&		theMultipliers,
		const XalanDOMCharVectorType&	theZeroChar,
		const XalanDOMCharVectorType&	theMultiplierChars,
		const DigitsTableVectorType&	theDigitsTable,
		const NumberTypeVectorType&		theDigitsTableTable);

	explicit
	XalanNumberingResourceBundle();

	XalanNumberingResourceBundle(const XalanNumberingResourceBundle&	theSource);

	~XalanNumberingResourceBundle();

	bool
	operator==(const XalanNumberingResourceBundle&	theRHS) const
	{
		return equals(m_language, theRHS.m_language);
	}

	const XalanDOMString&
	getLanguage() const
	{
		return m_language;
	}

	const XalanDOMString&
	getUILanguage() const
	{
		return m_uiLanguage;
	}

	const XalanDOMString&
	getHelpLanguage() const
	{
		return m_helpLanguage;
	}

	const XalanDOMCharVectorType&
	getAlphabet() const
	{
		return m_alphabet;
	}

	const XalanDOMCharVectorType&
	getTraditionalAlphabet() const
	{
		return m_traditionalAlphabet;
	}

	eOrientation
	getOrientation() const
	{
		return m_orientation;
	}

	eNumberingMethod
	getNumberingMethod() const
	{
		return m_numberingMethod;
	}

	eMultiplierOrder
	getMultiplierOrder() const
	{
		return m_multiplierOrder;
	}

	NumberType
	getMaxNumericalValue() const
	{
		return m_maxNumericalValue;
	}

	const NumberTypeVectorType&
	getNumberGroups() const
	{
		return m_numberGroups;
	}

	const NumberTypeVectorType&
	getMultipliers() const
	{
		return m_multipliers;
	}

	const XalanDOMCharVectorType&
	getZeroChar() const
	{
		return m_zeroChar;
	}

	const XalanDOMCharVectorType&
	getMultiplierChars() const
	{
		return m_multiplierChars;
	}

	const DigitsTableVectorType&
	getDigitsTable() const
	{
		return m_digitsTable;
	}

	const NumberTypeVectorType&
	getDigitsTableTable() const
	{
		return m_digitsTableTable;
	}

	void
	swap(XalanNumberingResourceBundle&	theOther);

private:

	XalanDOMString			m_language;

	XalanDOMString			m_uiLanguage;

	XalanDOMString			m_helpLanguage;

	XalanDOMCharVectorType	m_alphabet;

	XalanDOMCharVectorType	m_traditionalAlphabet;

	eOrientation			m_orientation;

	eNumberingMethod		m_numberingMethod;

	eMultiplierOrder		m_multiplierOrder;

	NumberType				m_maxNumericalValue;

	NumberTypeVectorType	m_numberGroups;

	NumberTypeVectorType	m_multipliers;

	XalanDOMCharVectorType	m_zeroChar;

	XalanDOMCharVectorType	m_multiplierChars;

	DigitsTableVectorType	m_digitsTable;

	NumberTypeVectorType	m_digitsTableTable;
};



#endif	// XALAN_NUMBERINGRESOURCEBUNDLE_HEADER_GUARD
