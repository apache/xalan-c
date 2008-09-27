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

#if !defined(ICUBRIDGE_COLLATIONCOMPAREFUNCTORIMPL_GUARD_1357924680)
#define ICUBRIDGE_COLLATIONCOMPAREFUNCTORIMPL_GUARD_1357924680



#include <xalanc/ICUBridge/ICUBridgeDefinitions.hpp>



#include <xalanc/Include/XalanList.hpp>



#include <xalanc/XSLT/StylesheetExecutionContextDefault.hpp>



#include <unicode/coll.h>




XALAN_CPP_NAMESPACE_BEGIN


#if defined(XALAN_HAS_CPP_NAMESPACE)
typedef U_ICU_NAMESPACE::Collator   CollatorType;
#else
typedef Collator                    CollatorType;
#endif

struct CollationCacheStruct
{
    CollationCacheStruct(
        MemoryManager&      theManager,
        const XalanDOMString&   theLocale,
        CollatorType*           theCollator) :
    m_locale(theLocale, theManager),
        m_collator(theCollator)
    {
    }

    CollationCacheStruct(MemoryManager& theManager) :
    m_locale(theManager),
        m_collator(0)
    {
    }

    CollationCacheStruct(   const CollationCacheStruct& other,
                            MemoryManager&          theManager) :
    m_locale(other.m_locale,theManager),
    m_collator(other.m_collator)
    {
    }
    void
        swap(CollationCacheStruct&  theOther)
    {
        m_locale.swap(theOther.m_locale);

        CollatorType* const     theTemp = m_collator;

        m_collator = theOther.m_collator;

        theOther.m_collator = theTemp;
    }

#if defined(XALAN_NO_SELECTIVE_TEMPLATE_INSTANTIATION)
    bool
        operator<(const CollationCacheStruct&  theRHS) const
    {
        return this < &theRHS;
    }

    bool
        operator==(const CollationCacheStruct&  theRHS) const
    {
        return this == &theRHS;
    }
#endif

    XalanDOMString  m_locale;

    CollatorType*   m_collator;

    struct CollatorDeleteFunctor
    {
     CollatorDeleteFunctor(MemoryManager& theManager):
        m_memoryManager(theManager)
    {
    }

    void
        operator()(CollationCacheStruct&    theStruct) const
    {
        delete theStruct.m_collator;
    }
    private:
        MemoryManager& m_memoryManager;
    };

    struct CollatorFindFunctor
    {
        CollatorFindFunctor(const XalanDOMChar* theLocale) :
    m_locale(theLocale)
    {
    }

    bool
        operator()(CollationCacheStruct&    theStruct) const
    {
        return XalanDOMString::equals(theStruct.m_locale ,m_locale);
    }

    const XalanDOMChar* const   m_locale;
    };
};

XALAN_USES_MEMORY_MANAGER(CollationCacheStruct)

class XALAN_ICUBRIDGE_EXPORT ICUBridgeCollationCompareFunctorImpl : public XalanCollationServices::CollationCompareFunctor
{
public:

    /**
     * Constructor.
     * 
     * @param fCacheCollators If true, the instance will cache collators.  This is not thread-safe, so each thread must have its own instance.
     */
    ICUBridgeCollationCompareFunctorImpl(   MemoryManager&  theManager ,
                                            bool                fCacheCollators = false);

    static ICUBridgeCollationCompareFunctorImpl*
    create( MemoryManager&  theManager,
            bool                fCacheCollators = false);


    ~ICUBridgeCollationCompareFunctorImpl();

    MemoryManager& 
    getMemoryManager()const
    {
        return m_collatorCache.getMemoryManager();
    }

    int
    operator()(
            const XalanDOMChar*                 theLHS,
            const XalanDOMChar*                 theRHS,
            XalanCollationServices::eCaseOrder  theCaseOrder = XalanCollationServices::eDefault) const;

    int
    operator()(
            const XalanDOMChar*                 theLHS,
            const XalanDOMChar*                 theRHS,
            const XalanDOMChar*                 theLocale,
            XalanCollationServices::eCaseOrder  theCaseOrder = XalanCollationServices::eDefault) const;

    bool
    isValid() const
    {
        return m_isValid;
    }


    typedef XalanList<CollationCacheStruct>         CollatorCacheListType;

    enum { eCacheMax = 10 };

private:

    int
    doDefaultCompare(
            const XalanDOMChar*     theLHS,
            const XalanDOMChar*     theRHS) const;

    int
    doCompare(
            const XalanDOMChar*                 theLHS,
            const XalanDOMChar*                 theRHS,
            const XalanDOMChar*                 theLocale,
            XalanCollationServices::eCaseOrder  theCaseOrder) const;

    int
    doCompareCached(
            const XalanDOMChar*                 theLHS,
            const XalanDOMChar*                 theRHS,
            const XalanDOMChar*                 theLocale,
            XalanCollationServices::eCaseOrder  theCaseOrder) const;

    int
    doCompare(
            const CollatorType&     theCollator,
            const XalanDOMChar*     theLHS,
            const XalanDOMChar*     theRHS) const;

    int
    doCompare(
            CollatorType&                       theCollator,
            const XalanDOMChar*                 theLHS,
            const XalanDOMChar*                 theRHS,
            XalanCollationServices::eCaseOrder  theCaseOrder) const;

    CollatorType*
    getCachedCollator(const XalanDOMChar*   theLocale) const;

    void
    cacheCollator(
            CollatorType*           theCollator,
            const XalanDOMChar*     theLocale) const;


    // Data members...
    bool                            m_isValid;

    CollatorType*                   m_defaultCollator;

    XalanDOMString                  m_defaultCollatorLocaleName;

    bool                            m_cacheCollators;

    mutable CollatorCacheListType   m_collatorCache;

    static const StylesheetExecutionContextDefault::DefaultCollationCompareFunctor  s_defaultFunctor;
};



XALAN_CPP_NAMESPACE_END



#endif  // ICUBRIDGE_COLLATIONCOMPAREFUNCTORIMPL_GUARD_1357924680
