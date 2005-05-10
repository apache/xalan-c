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
#if !defined(FUNCTIONICUFORMATNUMBERFUNCTOR_HEADER_GUARD_1357924680)
#define FUNCTIONICUFORMATNUMBERFUNCTOR_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/ICUBridge/ICUBridgeDefinitions.hpp>



#include <xalanc/Include/XalanMemMgrAutoPtr.hpp>



#include <xalanc/PlatformSupport/XalanDecimalFormatSymbols.hpp>



#include <xalanc/XSLT/StylesheetExecutionContextDefault.hpp>



#include <xalanc/XPath/XPathExecutionContext.hpp>



#include <unicode/decimfmt.h>




XALAN_CPP_NAMESPACE_BEGIN

typedef StylesheetExecutionContextDefault::FormatNumberFunctor FormatNumberFunctor;

#if defined(XALAN_HAS_CPP_NAMESPACE)
    typedef U_ICU_NAMESPACE::DecimalFormat  DecimalFormatType;
#else
    typedef DecimalFormat                   DecimalFormatType;
#endif

struct DecimalFormatCacheStruct
{
    DecimalFormatCacheStruct(
        MemoryManagerType&                  theManager,
        const XalanDecimalFormatSymbols&    theDFS,
        DecimalFormatType*                  theFormatter) :

        m_DFS(theDFS, theManager),
        m_formatter(theFormatter)
    {
    }

    DecimalFormatCacheStruct(MemoryManagerType&     theManager) :
        m_DFS(theManager),
        m_formatter(0)
    {
    }

    DecimalFormatCacheStruct(
                const DecimalFormatCacheStruct&     other,
                MemoryManagerType&                  theManager) :
        m_DFS(other.m_DFS, theManager),
        m_formatter(other.m_formatter)
    {
    }

#if defined(XALAN_NO_SELECTIVE_TEMPLATE_INSTANTIATION)
    bool
    operator<(const DecimalFormatCacheStruct&  theRHS) const
    {
        return this < &theRHS;
    }

    bool
    operator==(const DecimalFormatCacheStruct&  theRHS) const
    {
        return this == &theRHS;
    }
#endif

    XalanDecimalFormatSymbols   m_DFS;

    DecimalFormatType * m_formatter;

    class DecimalFormatDeleteFunctor
    {
    public:

        DecimalFormatDeleteFunctor(MemoryManager&   theManager) :
            m_memoryManager(theManager)
        {
        }

        void
        operator()(DecimalFormatCacheStruct&    theStruct) const
        {
            assert(theStruct.m_formatter != 0);

            XalanDestroy(
                m_memoryManager,
                *theStruct.m_formatter);
        }

    private:

        // Not implemented...
        DecimalFormatDeleteFunctor&
        operator=(const DecimalFormatDeleteFunctor&);

        // Data members.
        MemoryManager&  m_memoryManager;
    };

    struct DecimalFormatFindFunctor
    {
        DecimalFormatFindFunctor(const XalanDecimalFormatSymbols*   theDFS) :
            m_DFS(theDFS)
            {
            }

        bool
        operator()(DecimalFormatCacheStruct&    theStruct) const
        {
            return theStruct.m_DFS == (*m_DFS);
        }

        const XalanDecimalFormatSymbols * const m_DFS;
    };

private:

    DecimalFormatCacheStruct();
    DecimalFormatCacheStruct(const DecimalFormatCacheStruct& other);
};


XALAN_USES_MEMORY_MANAGER(DecimalFormatCacheStruct)

// Class that implements the XSLT function format-number using the ICU.
//
class XALAN_ICUBRIDGE_EXPORT ICUFormatNumberFunctor : public FormatNumberFunctor
{
public:

    ICUFormatNumberFunctor(MemoryManagerType& theManager);

    static ICUFormatNumberFunctor*
    create(MemoryManagerType& theManager);

    virtual
    ~ICUFormatNumberFunctor();

    virtual void
    operator() (
        XPathExecutionContext&              executionContext,
        double                              theNumber,
        const XalanDOMString&               thePattern,
        const XalanDecimalFormatSymbols*    theDFS,
        XalanDOMString&                     theResult,
        const XalanNode*                    context = 0,
        const LocatorType*                  locator = 0) const;
    

    class UnlocalizePatternFunctor
    {
    public:
        UnlocalizePatternFunctor(const XalanDecimalFormatSymbols&   theDFS):
            m_DFS(theDFS)
        {
        }

        XalanDOMString&
        operator()(
                const XalanDOMString&   thePattern,
                XalanDOMString&         theResult,
                MemoryManager&          theManager) const;

    private:

        const XalanDecimalFormatSymbols&    m_DFS;
    };

    typedef XalanList<DecimalFormatCacheStruct>         DecimalFormatCacheListType;

private:

    DecimalFormatType*
    getCachedDecimalFormat(const XalanDecimalFormatSymbols &theDFS) const;

    bool
    doFormat(   
        double                              theNumber,
        const XalanDOMString&               thePattern,
        XalanDOMString&                     theResult,
        const XalanDecimalFormatSymbols*    theDFS = 0) const;

    bool
    doICUFormat(
        double                              theNumber,
        const XalanDOMString&               thePattern,
        XalanDOMString&                     theResult,
        DecimalFormatType*                  theFormatter = 0) const;

    void
    cacheDecimalFormat(
        DecimalFormatType*                  theFormatter,
        const XalanDecimalFormatSymbols&    theDFS) const;

    static DecimalFormat*
    createDecimalFormat(
        const XalanDecimalFormatSymbols&    theXalanDFS,
        MemoryManager&                      theManager);

    static DecimalFormat*
    createDecimalFormat(MemoryManager&  theManager)
    {
        const XalanDecimalFormatSymbols     theDFS(theManager);

        return createDecimalFormat(theDFS, theManager);
    }

    enum { eCacheMax = 10u };

private:

    // These are not implemented...
    ICUFormatNumberFunctor&
    operator=(const ICUFormatNumberFunctor&);

    bool
    operator==(const ICUFormatNumberFunctor&) const;

    typedef XalanMemMgrAutoPtr<DecimalFormatType>   DFAutoPtrType;

    // Data members...
    mutable DecimalFormatCacheListType  m_decimalFormatCache;

    const DFAutoPtrType                 m_defaultDecimalFormat;

    MemoryManagerType&                  m_memoryManager;
};



XALAN_CPP_NAMESPACE_END



#endif  // FUNCTIONICUFORMATNUMBERFUNCTOR_HEADER_GUARD_1357924680
