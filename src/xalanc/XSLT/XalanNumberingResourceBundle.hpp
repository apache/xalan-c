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

#if !defined(XALAN_NUMBERINGRESOURCEBUNDLE_HEADER_GUARD)
#define XALAN_NUMBERINGRESOURCEBUNDLE_HEADER_GUARD



// Base include file.  Must be first.
#include <xalanc/XSLT/XSLTDefinitions.hpp>



#include <xalanc/Include/XalanVector.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



XALAN_CPP_NAMESPACE_BEGIN

XALAN_USES_MEMORY_MANAGER(XalanDOMCharVectorType)

class XalanNumberingResourceBundle
{
public:

	typedef unsigned long							NumberType;

	typedef XalanVector<NumberType>					NumberTypeVectorType;
	typedef XalanVector<XalanDOMCharVectorType>		DigitsTableVectorType;

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
		const NumberTypeVectorType&		theDigitsTableTable,
        MemoryManagerType&              theManager);

	explicit
	XalanNumberingResourceBundle(MemoryManagerType&  theManager);

	XalanNumberingResourceBundle(const XalanNumberingResourceBundle&	theSource,
                                    MemoryManagerType&                  theManager);

	~XalanNumberingResourceBundle();

    MemoryManagerType&
    getMemoryManager()
    {
        return m_language.getMemoryManager();
    }

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



XALAN_CPP_NAMESPACE_END



#endif	// XALAN_NUMBERINGRESOURCEBUNDLE_HEADER_GUARD
