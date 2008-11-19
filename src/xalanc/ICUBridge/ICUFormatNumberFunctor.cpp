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



#include "ICUFormatNumberFunctor.hpp"



#include <algorithm>



#include <xalanc/ICUBridge/ICUBridge.hpp>



#include <xalanc/Include/XalanAutoPtr.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanDecimalFormatSymbols.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/XPath/XPathExecutionContext.hpp>



XALAN_CPP_NAMESPACE_BEGIN



ICUFormatNumberFunctor::ICUFormatNumberFunctor(MemoryManager& theManager) :
    m_decimalFormatCache(theManager),
    m_defaultDecimalFormat(theManager, createDecimalFormat(theManager)),
    m_memoryManager(theManager)
{
}



ICUFormatNumberFunctor*
ICUFormatNumberFunctor::create(MemoryManager& theManager) 
{
    typedef ICUFormatNumberFunctor  ThisType;

    XalanAllocationGuard    theGuard(theManager, theManager.allocate(sizeof(ThisType)));

    ThisType* const     theResult =
        new (theGuard.get()) ThisType(theManager);

    theGuard.release();

    return theResult;
}



ICUFormatNumberFunctor::~ICUFormatNumberFunctor()
{
    XALAN_USING_STD(for_each)

    for_each(
        m_decimalFormatCache.begin(),
        m_decimalFormatCache.end(),
        DecimalFormatCacheStruct::DecimalFormatDeleteFunctor(m_memoryManager));
}



void
ICUFormatNumberFunctor::operator() (
        XPathExecutionContext&              executionContext,   
        double                              theNumber,
        const XalanDOMString&               thePattern,
        const XalanDecimalFormatSymbols*    theDFS,
        XalanDOMString&                     theResult,
        const XalanNode*                    context,
        const Locator*                      locator) const 
{
    if (!doFormat(theNumber, thePattern, theResult, theDFS))
    {
        const XPathExecutionContext::GetCachedString    theGuard(executionContext);

        executionContext.problem(
            XPathExecutionContext::eXSLTProcessor,
            XPathExecutionContext::eWarning,
            XalanMessageLoader::getMessage(
                theGuard.get(),
                XalanMessages::FormatNumberFailed),
            locator,
            context);
    }       
}



DecimalFormatType * 
ICUFormatNumberFunctor::getCachedDecimalFormat(const XalanDecimalFormatSymbols &theDFS) const
{
    XALAN_USING_STD(find_if)

    DecimalFormatCacheListType::iterator i =
        find_if(
            m_decimalFormatCache.begin(),
            m_decimalFormatCache.end(),
            DecimalFormatCacheStruct::DecimalFormatFindFunctor(&theDFS));

    if (i == m_decimalFormatCache.end())
    {
        return 0;
    }
    else
    {
        // Let's do a quick check to see if we found the first entry.
        // If so, we don't have to update the cache, so just return the
        // appropriate value...
        const DecimalFormatCacheListType::iterator  theBegin =
            m_decimalFormatCache.begin();

        if (i == theBegin)
        {
            return (*i).m_formatter;
        }
        else
        {
            // Save the formatter, because splice() may invalidate
            // the iterator.
            DecimalFormatType* const    theFormatter = (*i).m_formatter;

            // Move the entry to the beginning the cache
            m_decimalFormatCache.splice(theBegin, m_decimalFormatCache, i);

            return theFormatter;
        }
    }
}



bool
ICUFormatNumberFunctor::doFormat(   
        double                              theNumber,
        const XalanDOMString&               thePattern,
        XalanDOMString&                     theResult,
        const XalanDecimalFormatSymbols*    theDFS) const
{

    if (theDFS == 0)
    {
        return doICUFormat(theNumber, thePattern, theResult);
    }

    XalanDOMString  nonLocalPattern(m_memoryManager);

    UnlocalizePatternFunctor    formatter(*theDFS);

    formatter(thePattern, nonLocalPattern, m_memoryManager);


    DecimalFormatType* const    theFormatter =
        getCachedDecimalFormat(*theDFS);

    if (theFormatter != 0)
    {
        return doICUFormat(
                    theNumber,
                    nonLocalPattern,
                    theResult,
                    theFormatter);
    }
    else
    {
        DFAutoPtrType   theDecimalFormatGuard(
                            m_memoryManager,
                            createDecimalFormat(*theDFS, m_memoryManager));

        if (theDecimalFormatGuard.get() != 0)
        {
            // OK, there was no error, so cache the instance...
            cacheDecimalFormat(theDecimalFormatGuard.get(), *theDFS);

            // Release the collator, since it's in the cache and
            // will be controlled by the cache...
            DecimalFormatType* const    theDecimalFormat =
                theDecimalFormatGuard.releasePtr();

            return doICUFormat(
                        theNumber,
                        nonLocalPattern,
                        theResult,
                        theDecimalFormat);
        }
        else
        {
            return doICUFormat(theNumber,nonLocalPattern,theResult);
        }
    }
}



DecimalFormatType*
ICUFormatNumberFunctor::createDecimalFormat(
        const XalanDecimalFormatSymbols&    theXalanDFS,
        MemoryManager&                      theManager)
{
    UErrorCode theStatus = U_ZERO_ERROR;    

    // Use a XalanAutoPtr, to keep this safe until we construct the DecimalFormat instance.
    XalanAutoPtr<DecimalFormatSymbols>  theDFS(new DecimalFormatSymbols(theStatus));
    // We got a XalanDecimalFormatSymbols, so set the
    // corresponding data in the ICU DecimalFormatSymbols.
    theDFS->setSymbol(
        DecimalFormatSymbols::kZeroDigitSymbol,
        UChar(theXalanDFS.getZeroDigit()));

    theDFS->setSymbol(
        DecimalFormatSymbols::kGroupingSeparatorSymbol,
        UChar(theXalanDFS.getGroupingSeparator()));

    theDFS->setSymbol(
        DecimalFormatSymbols::kDecimalSeparatorSymbol,
        UChar(theXalanDFS.getDecimalSeparator()));

    theDFS->setSymbol(
        DecimalFormatSymbols::kPerMillSymbol,
        UChar(theXalanDFS.getPerMill()));

    theDFS->setSymbol(
        DecimalFormatSymbols::kPercentSymbol,
        UChar(theXalanDFS.getPercent()));

    theDFS->setSymbol(
        DecimalFormatSymbols::kDigitSymbol,
        UChar(theXalanDFS.getDigit()));

    theDFS->setSymbol(
        DecimalFormatSymbols::kPatternSeparatorSymbol,
        UChar(theXalanDFS.getPatternSeparator()));

    theDFS->setSymbol(
        DecimalFormatSymbols::kInfinitySymbol,
        ICUBridge::XalanDOMStringToUnicodeString(
            theManager,
            theXalanDFS.getInfinity()));

    theDFS->setSymbol(
        DecimalFormatSymbols::kNaNSymbol,
        ICUBridge::XalanDOMStringToUnicodeString(
            theManager,
            theXalanDFS.getNaN()));

    theDFS->setSymbol(
        DecimalFormatSymbols::kMinusSignSymbol,
        UChar(theXalanDFS.getMinusSign()));

    theDFS->setSymbol(
        DecimalFormatSymbols::kCurrencySymbol,
        ICUBridge::XalanDOMStringToUnicodeString(
            theManager,
            theXalanDFS.getCurrencySymbol()));

    theDFS->setSymbol(
        DecimalFormatSymbols::kIntlCurrencySymbol,
        ICUBridge::XalanDOMStringToUnicodeString(
            theManager,
            theXalanDFS.getInternationalCurrencySymbol()));

    theDFS->setSymbol(
        DecimalFormatSymbols::kMonetarySeparatorSymbol,
        UChar(theXalanDFS.getMonetaryDecimalSeparator()));

    // Construct a DecimalFormat instance.
    DecimalFormatType*  theFormatter = 0;

    XalanConstruct(
        theManager,
        theFormatter,
        theStatus);

    // Guard this, just in case something happens before
    // we return it. 
    DFAutoPtrType   theGuard(theManager, theFormatter);

    if (U_SUCCESS(theStatus))
    {
        // Note that we release the XalanAutoPtr, since the
        // DecimalFormat will adopt the DecimalFormatSymbols instance.
        theGuard->adoptDecimalFormatSymbols(theDFS.release());

        return theGuard.releasePtr();
    } 
    else 
    {
        assert(false);

        return 0;
    }
}



void
ICUFormatNumberFunctor::cacheDecimalFormat(
    DecimalFormatType *                     theFormatter,
    const XalanDecimalFormatSymbols&        theDFS) const
{

    assert(theFormatter != 0);

    // Is the cache full?
    if (m_decimalFormatCache.size() == eCacheMax)
    {
        // Yes, so guard the collator instance, in case pop_back() throws...
        DFAutoPtrType   theDecimalFormatGuard(
                            m_memoryManager,
                            m_decimalFormatCache.back().m_formatter);

        m_decimalFormatCache.pop_back();
    }

    const DecimalFormatCacheListType::value_type    emptyDFC(m_memoryManager);

    m_decimalFormatCache.push_front(emptyDFC);

    DecimalFormatCacheListType::value_type&     theEntry = 
        m_decimalFormatCache.front();

    theEntry.m_formatter = theFormatter;
    theEntry.m_DFS = theDFS;
}



bool
ICUFormatNumberFunctor::doICUFormat(
        double                              theNumber,
        const XalanDOMString&               thePattern,
        XalanDOMString&                     theResult,
        DecimalFormatType*                  theFormatter) const
{
    UErrorCode theStatus = U_ZERO_ERROR;

    if (theFormatter == 0)
    {
        if (m_defaultDecimalFormat.get() != 0) 
        {
            theFormatter = m_defaultDecimalFormat.get();
        }
        else
        {
            return false;
        }
    }

    theFormatter->applyPattern(
        ICUBridge::XalanDOMStringToUnicodeString(m_memoryManager, thePattern),
        theStatus);

    if (U_SUCCESS(theStatus))
    {
        // Do the format...
        UnicodeString   theUnicodeResult;
        theFormatter->format(theNumber, theUnicodeResult);
        ICUBridge::UnicodeStringToXalanDOMString(theUnicodeResult, theResult);
    }

    return U_SUCCESS(theStatus) ? true : false;
}

XalanDOMString&
ICUFormatNumberFunctor::UnlocalizePatternFunctor::operator()(
            const XalanDOMString&   thePattern,
            XalanDOMString&         theResult,
            MemoryManager&          theManager) const
{

    XalanDecimalFormatSymbols   defaultDFS(theManager);

    XalanDOMString::const_iterator iterator = thePattern.begin();

    while( iterator != thePattern.end())
    {
        
        if( m_DFS.getDecimalSeparator() == *iterator )
        {
            theResult.push_back(defaultDFS.getDecimalSeparator());
        }
        else if(m_DFS.getDigit() == *iterator)
        {
            theResult.push_back(defaultDFS.getDigit());
        }
        else if(m_DFS.getGroupingSeparator() == *iterator)
        {
            theResult.push_back(defaultDFS.getGroupingSeparator());
        }
        else if(m_DFS.getMinusSign() == *iterator)
        {
            theResult.push_back(defaultDFS.getMinusSign());
        }
        else if(m_DFS.getPatternSeparator() == *iterator)
        {
            theResult.push_back(defaultDFS.getPatternSeparator());
        }
        else if(m_DFS.getPercent() == *iterator)
        {
            theResult.push_back(defaultDFS.getPercent());
        }
        else if(m_DFS.getPerMill() == *iterator)
        {
            theResult.push_back(defaultDFS.getPerMill());
        }
        else if(m_DFS.getZeroDigit() == *iterator)
        {
            theResult.push_back(defaultDFS.getZeroDigit());
        }
        else
        {   
            switch(*iterator)
            {
            case XalanUnicode::charFullStop:
            case XalanUnicode::charNumberSign:
            case XalanUnicode::charComma:
            case XalanUnicode::charHyphenMinus:
            case XalanUnicode::charSemicolon:
            case XalanUnicode::charPercentSign:
            case XalanUnicode::charPerMilleSign:
            case XalanUnicode::charDigit_0:
                {
                    theResult.push_back(XalanUnicode::charAmpersand);
                    theResult.push_back(*iterator);
                    theResult.push_back(XalanUnicode::charAmpersand);
                }
            }
        }

        iterator++;
    }

    return theResult;
}

XALAN_CPP_NAMESPACE_END
