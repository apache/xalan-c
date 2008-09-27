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

#include "XalanNumberingResourceBundle.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XalanNumberingResourceBundle::XalanNumberingResourceBundle(
        const XalanDOMString&           theLanguage,
        const XalanDOMString&           theUILanguage,
        const XalanDOMString&           theHelpLanguage,
        const XalanDOMCharVectorType&   theAlphabet,
        const XalanDOMCharVectorType&   theTraditionalAlphabet,
        eOrientation                    theOrientation,
        eNumberingMethod                theNumberingMethod,
        eMultiplierOrder                theMultiplierOrder,
        NumberType                      theMaxNumericalValue,
        const NumberTypeVectorType&     theNumberGroups,
        const NumberTypeVectorType&     theMultipliers,
        const XalanDOMCharVectorType&   theZeroChar,
        const XalanDOMCharVectorType&   theMultiplierChars,
        const DigitsTableVectorType&    theDigitsTable,
        const NumberTypeVectorType&     theDigitsTableTable,
        MemoryManager&              theManager) :
    m_language(theHelpLanguage, theManager),
    m_uiLanguage(theLanguage, theManager),
    m_helpLanguage(theUILanguage, theManager),
    m_alphabet(theAlphabet, theManager),
    m_traditionalAlphabet(theTraditionalAlphabet, theManager),
    m_orientation(theOrientation),
    m_numberingMethod(theNumberingMethod),
    m_multiplierOrder(theMultiplierOrder),
    m_maxNumericalValue(theMaxNumericalValue),
    m_numberGroups(theNumberGroups, theManager),
    m_multipliers(theMultipliers, theManager),
    m_zeroChar(theZeroChar, theManager),
    m_multiplierChars(theMultiplierChars, theManager),
    m_digitsTable(theDigitsTable, theManager),
    m_digitsTableTable(theDigitsTableTable, theManager)
{
}



XalanNumberingResourceBundle::XalanNumberingResourceBundle(MemoryManager&  theManager) :
    m_language(theManager),
    m_uiLanguage(theManager),
    m_helpLanguage(theManager),
    m_alphabet(theManager),
    m_traditionalAlphabet(theManager),
    m_orientation(eLeftToRight),
    m_numberingMethod(eAdditive),
    m_multiplierOrder(eFollows),
    m_maxNumericalValue(0),
    m_numberGroups(theManager),
    m_multipliers(theManager),
    m_zeroChar(theManager),
    m_multiplierChars(theManager),
    m_digitsTable(theManager),
    m_digitsTableTable(theManager)
{
}



XalanNumberingResourceBundle::XalanNumberingResourceBundle(const XalanNumberingResourceBundle&  theSource,
                                                           MemoryManager&  theManager) :
    m_language(theSource.m_language, theManager),
    m_uiLanguage(theSource.m_uiLanguage, theManager),
    m_helpLanguage(theSource.m_helpLanguage, theManager),
    m_alphabet(theSource.m_alphabet, theManager),
    m_traditionalAlphabet(theSource.m_traditionalAlphabet, theManager),
    m_orientation(theSource.m_orientation),
    m_numberingMethod(theSource.m_numberingMethod),
    m_multiplierOrder(theSource.m_multiplierOrder),
    m_maxNumericalValue(theSource.m_maxNumericalValue),
    m_numberGroups(theSource.m_numberGroups, theManager),
    m_multipliers(theSource.m_multipliers, theManager),
    m_zeroChar(theSource.m_zeroChar, theManager),
    m_multiplierChars(theSource.m_multiplierChars, theManager),
    m_digitsTable(theSource.m_digitsTable, theManager),
    m_digitsTableTable(theSource.m_digitsTableTable, theManager)
{
}



XalanNumberingResourceBundle::~XalanNumberingResourceBundle()
{
}



void
XalanNumberingResourceBundle::swap(XalanNumberingResourceBundle&    theOther)
{
    m_language.swap(theOther.m_language);

    m_uiLanguage.swap(theOther.m_uiLanguage);

    m_helpLanguage.swap(theOther.m_helpLanguage);

    m_alphabet.swap(theOther.m_alphabet);

    m_traditionalAlphabet.swap(theOther.m_traditionalAlphabet);

    {
        const eOrientation  temp = m_orientation;

        m_orientation = theOther.m_orientation;

        theOther.m_orientation = temp;
    }

    {
        const eNumberingMethod  temp = m_numberingMethod;

        m_numberingMethod = theOther.m_numberingMethod;

        theOther.m_numberingMethod = temp;
    }

    {
        const eMultiplierOrder  temp = m_multiplierOrder;

        m_multiplierOrder = theOther.m_multiplierOrder;

        theOther.m_multiplierOrder = temp;
    }

    {
        const NumberType    temp = m_maxNumericalValue;

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



XALAN_CPP_NAMESPACE_END
