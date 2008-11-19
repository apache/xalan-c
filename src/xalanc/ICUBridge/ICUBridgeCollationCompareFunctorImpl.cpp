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

#include "ICUBridgeCollationCompareFunctorImpl.hpp"
#include "ICUBridge.hpp"



#include <algorithm>
#include <cstdlib>



#include <xalanc/Include/XalanAutoPtr.hpp>

#include <xalanc/Include/XalanMemMgrAutoPtr.hpp>

#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanUnicode.hpp>



XALAN_CPP_NAMESPACE_BEGIN



const StylesheetExecutionContextDefault::DefaultCollationCompareFunctor     ICUBridgeCollationCompareFunctorImpl::s_defaultFunctor;



inline CollatorType*
createCollator(
            UErrorCode&         theStatus,
            const Locale&       theLocale,
            XalanDOMString*     theLocaleName = 0)
{

    if (theLocaleName != 0)
    {
        *theLocaleName = theLocale.getName();

        // Replace _ with -, since that's what xml:lang specifies...
        XalanDOMString::size_type   theIndex;

        while((theIndex = indexOf(*theLocaleName, XalanUnicode::charLowLine)) != theLocaleName->length())
        {
            (*theLocaleName)[theIndex] = XalanUnicode::charHyphenMinus;
        }
    }

    return CollatorType::createInstance(theLocale, theStatus);
}



inline CollatorType*
createCollator(
            UErrorCode&         theStatus,
            XalanDOMString*     theLocaleName = 0)
{
    const char*     theLang =
#if defined(XALAN_STRICT_ANSI_HEADERS)
            std::getenv("LANG");
#else
            getenv("LANG");
#endif

    if (theLang == 0)
    {
#if defined(XALAN_ICU_DEFAULT_LOCALE_PROBLEM)
        return createCollator(theStatus, Locale::US, theLocaleName);
#else
        return createCollator(theStatus, Locale::getDefault(), theLocaleName);
#endif
    }
    else
    {
        return createCollator(theStatus, Locale(theLang), theLocaleName);
    }
}



ICUBridgeCollationCompareFunctorImpl::ICUBridgeCollationCompareFunctorImpl(MemoryManager&   theManager,
                                                                           bool                 fCacheCollators) :
    m_isValid(false),
    m_defaultCollator(0),
    m_defaultCollatorLocaleName(theManager),
    m_cacheCollators(fCacheCollators),
    m_collatorCache(theManager)
{
    UErrorCode  theStatus = U_ZERO_ERROR;

    m_defaultCollator = createCollator(theStatus, &m_defaultCollatorLocaleName);

    if (U_SUCCESS(theStatus))
    {
        m_isValid = true;
    }
}


    
ICUBridgeCollationCompareFunctorImpl*
ICUBridgeCollationCompareFunctorImpl::create(
            MemoryManager&  theManager,
            bool            fCacheCollators) 
{
    ICUBridgeCollationCompareFunctorImpl*   theInstance;

    return XalanConstruct(
                theManager,
                theInstance,
                theManager,
                fCacheCollators);
}



ICUBridgeCollationCompareFunctorImpl::~ICUBridgeCollationCompareFunctorImpl()
{
    XALAN_USING_STD(for_each)

    delete m_defaultCollator;

    for_each(
            m_collatorCache.begin(),
            m_collatorCache.end(),
            CollationCacheStruct::CollatorDeleteFunctor(getMemoryManager()));
}



int
ICUBridgeCollationCompareFunctorImpl::doCompare(
            const CollatorType&     theCollator,
            const XalanDOMChar*     theLHS,
            const XalanDOMChar*     theRHS) const
{
    const XalanDOMString::size_type     theLHSLength =
        length(theLHS);

    const XalanDOMString::size_type     theRHSLength =
        length(theRHS);

    assert(
        theLHSLength == static_cast<int32_t>(theLHSLength) &&
        theRHSLength == static_cast<int32_t>(theRHSLength));

    return theCollator.compare(
                theLHS,
                static_cast<int32_t>(theLHSLength),
                theRHS,
                static_cast<int32_t>(theRHSLength));
}



inline UColAttributeValue
caseOrderConvert(XalanCollationServices::eCaseOrder     theCaseOrder)
{
    switch(theCaseOrder)
    {
    case XalanCollationServices::eLowerFirst:
        return UCOL_LOWER_FIRST;
        break;

    case XalanCollationServices::eUpperFirst:
        return UCOL_UPPER_FIRST;
        break;

    case XalanCollationServices::eDefault:
        break;

    default:
        assert(false);
        break;
    }

    return UCOL_DEFAULT;
}



int
ICUBridgeCollationCompareFunctorImpl::doDefaultCompare(
            const XalanDOMChar*     theLHS,
            const XalanDOMChar*     theRHS) const
{
    if (isValid() == false)
    {
        return s_defaultFunctor(theLHS, theRHS, XalanCollationServices::eDefault);
    }
    else
    {
        assert(m_defaultCollator != 0);

        return doCompare(*m_defaultCollator, theLHS, theRHS);
    }
}



inline CollatorType*
createCollator(
            const XalanDOMChar*     theLocale,
            UErrorCode&             theStatus)
{
    assert(theLocale != 0);

    const XalanDOMString::size_type     theLength = length(theLocale);

    if (theLength >= ULOC_FULLNAME_CAPACITY)
    {
        theStatus = U_ILLEGAL_ARGUMENT_ERROR;

        return 0;
    }
    else
    {
#if defined(XALAN_NON_ASCII_PLATFORM)
        CharVectorType  theVector;

        TranscodeToLocalCodePage(theLocale, theVector, true);

        const char* const   theBuffer = c_str(theVector);
#else
        char    theBuffer[ULOC_FULLNAME_CAPACITY];

        // Since language names must be ASCII, this
        // is the cheapest way to "transcode"...
        for (XalanDOMString::size_type i = 0; i <= theLength; ++i)
        {
            theBuffer[i] = char(theLocale[i]);
        }
#endif
        return CollatorType::createInstance(
                    Locale::createFromName(theBuffer),
                    theStatus);
    }
}



int
ICUBridgeCollationCompareFunctorImpl::doCompare(
            const XalanDOMChar*                 theLHS,
            const XalanDOMChar*                 theRHS,
            const XalanDOMChar*                 theLocale,
            XalanCollationServices::eCaseOrder  theCaseOrder) const
{
    UErrorCode  theStatus = U_ZERO_ERROR;

    XalanAutoPtr<CollatorType>  theCollator(createCollator(theLocale, theStatus));

    if (U_SUCCESS(theStatus))
    {
        assert(theCollator.get() != 0);

        return doCompare(
                *theCollator.get(),
                theLHS,
                theRHS,
                theCaseOrder);
    }
    else
    {
        return s_defaultFunctor(theLHS, theRHS, theCaseOrder);
    }
}



int
ICUBridgeCollationCompareFunctorImpl::doCompareCached(
            const XalanDOMChar*                 theLHS,
            const XalanDOMChar*                 theRHS,
            const XalanDOMChar*                 theLocale,
            XalanCollationServices::eCaseOrder  theCaseOrder) const
{
    CollatorType*   theCollator = getCachedCollator(theLocale);

    if (theCollator != 0)
    {
        return doCompare(*theCollator, theLHS, theRHS, theCaseOrder);
    }
    else
    {
        UErrorCode  theStatus = U_ZERO_ERROR;

        XalanAutoPtr<CollatorType>  theCollatorGuard(createCollator(theLocale, theStatus));

        if (U_SUCCESS(theStatus))
        {
            assert(theCollatorGuard.get() != 0);

            // OK, there was no error, so cache the instance...
            cacheCollator(theCollatorGuard.get(), theLocale);

            // Release the collator, since it's in the cache and
            // will be controlled by the cache...
            theCollator = theCollatorGuard.release();

            return doCompare(
                    *theCollator,
                    theLHS,
                    theRHS,
                    theCaseOrder);
        }
        else
        {
            return s_defaultFunctor(theLHS, theRHS, theCaseOrder);
        }
    }
}



int
ICUBridgeCollationCompareFunctorImpl::doCompare(
            CollatorType&                       theCollator,
            const XalanDOMChar*                 theLHS,
            const XalanDOMChar*                 theRHS,
            XalanCollationServices::eCaseOrder  theCaseOrder) const
{
    UErrorCode  theStatus = U_ZERO_ERROR;

    theCollator.setAttribute(
                UCOL_CASE_FIRST,
                caseOrderConvert(theCaseOrder),
                theStatus);

    const XalanDOMString::size_type     theLHSLength =
        length(theLHS);

    const XalanDOMString::size_type     theRHSLength =
        length(theRHS);

    assert(
        theLHSLength == static_cast<int32_t>(theLHSLength) &&
        theRHSLength == static_cast<int32_t>(theRHSLength));

    return theCollator.compare(
                    theLHS,
                    static_cast<int32_t>(theLHSLength),
                    theRHS,
                    static_cast<int32_t>(theRHSLength));
}



int
ICUBridgeCollationCompareFunctorImpl::operator()(
            const XalanDOMChar*                 theLHS,
            const XalanDOMChar*                 theRHS,
            XalanCollationServices::eCaseOrder  theCaseOrder) const
{
    if (theCaseOrder == XalanCollationServices::eDefault)
    {
        return doDefaultCompare(theLHS, theRHS);
    }
    else
    {
        return doCompare(
                theLHS,
                theRHS,
                m_defaultCollatorLocaleName.c_str(),
                theCaseOrder);
    }
}



int
ICUBridgeCollationCompareFunctorImpl::operator()(
            const XalanDOMChar*                 theLHS,
            const XalanDOMChar*                 theRHS,
            const XalanDOMChar*                 theLocale,
            XalanCollationServices::eCaseOrder  theCaseOrder) const
{
    if (theCaseOrder == XalanCollationServices::eDefault &&
        XalanDOMString::equals(m_defaultCollatorLocaleName, theLocale) == true)
    {
        return doDefaultCompare(theLHS, theRHS);
    }
    else if (m_cacheCollators == true)
    {
        return doCompareCached(theLHS, theRHS, theLocale, theCaseOrder);
    }
    else
    {
        return doCompare(theLHS, theRHS, theLocale, theCaseOrder);
    };
}



CollatorType*
ICUBridgeCollationCompareFunctorImpl::getCachedCollator(const XalanDOMChar*     theLocale) const
{
    XALAN_USING_STD(find_if)

    CollatorCacheListType::iterator i =
        find_if(
            m_collatorCache.begin(),
            m_collatorCache.end(),
            CollationCacheStruct::CollatorFindFunctor(theLocale));

    if (i == m_collatorCache.end())
    {
        return 0;
    }
    else
    {
        // Let's do a quick check to see if we found the first entry.
        // If so, we don't have to update the cache, so just return the
        // appropriate value...
        const CollatorCacheListType::iterator   theBegin =
            m_collatorCache.begin();

        if (i == theBegin)
        {
            return (*i).m_collator;
        }
        else
        {
            // Save the collator, because splice() may invalidate
            // i.
            CollatorType* const     theCollator = (*i).m_collator;

            // Move the entry to the beginning the cache
            m_collatorCache.splice(theBegin, m_collatorCache, i);

            return theCollator;
        }
    }
}



void
ICUBridgeCollationCompareFunctorImpl::cacheCollator(
            CollatorType*           theCollator,
            const XalanDOMChar*     theLocale) const
{
    assert(theCollator != 0);
    assert(theLocale != 0);

    // Is the cache full?
    if (m_collatorCache.size() == eCacheMax)
    {
        // Yes, so guard the collator instance, in case pop_back() throws...
        XalanAutoPtr<CollatorType>  theCollatorGuard(m_collatorCache.back().m_collator);

        m_collatorCache.pop_back();
    }

    m_collatorCache.push_front(CollatorCacheListType::value_type(getMemoryManager()));

    CollatorCacheListType::value_type&      theEntry = 
        m_collatorCache.front();

    // Set the locale first, since that might throw an exception...
    theEntry.m_locale = theLocale;

    theEntry.m_collator = theCollator;
}



XALAN_CPP_NAMESPACE_END
