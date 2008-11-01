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

#include <xalanc/Include/PlatformDefinitions.hpp>



#include <cstdlib>
#include <cstring>
#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#else
#include <iostream>
#endif



#if !defined(NDEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif



#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/OutOfMemoryException.hpp>



#include <xalanc/Include/XalanAutoPtr.hpp>
#include <xalanc/Include/XalanMemoryManagement.hpp>



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/XalanTransformer/XalanTransformer.hpp>



#if defined(_MSC_VER)
#define XALAN_USE_WINDOWS_TIMING
#endif

#if defined(XALAN_USE_WINDOWS_TIMING)
#include "windows.h"
#else
#include <ctime>
#if defined(XALAN_STRICT_ANSI_HEADERS)
using std::clock;
using std::clock_t;
#endif
#endif



XALAN_USING_STD(cerr)
XALAN_USING_STD(cin)
XALAN_USING_STD(cout)
XALAN_USING_STD(endl)
XALAN_USING_STD(ostream)

#if defined(XALAN_STRICT_ANSI_HEADERS)
using std::atoi;
using std::strcmp;
using std::strlen;
#endif



void
Usage()
{
    XALAN_USING_XALAN(XalanDOMString)
    XALAN_USING_XALAN(XalanMessageLoader)
    XALAN_USING_XALAN(XalanMessages)
    XALAN_USING_XALAN(XalanMemMgrs)
    XALAN_USING_XERCES(MemoryManager)

    MemoryManager&  theManager = XalanMemMgrs::getDefaultXercesMemMgr();

    bool    bErrorState = true; // means OK

    XalanDOMString  szXalanVersion(theManager);
    XalanMessageLoader::getMessage(
        szXalanVersion,
        XalanMessages::XalanExeHelpMenuXalanVersion_1Param,
        XALAN_FULLVERSIONDOT);

    XalanDOMString  szXercesVersion(theManager);
    XalanMessageLoader::getMessage(
        szXercesVersion,
        XalanMessages::XalanExeHelpMenuXercesVersion_1Param,
        XERCES_FULLVERSIONDOT);

    try
    {
        XalanDOMString::CharVectorType  cvtXalanVersion(theManager);
        szXalanVersion.transcode(cvtXalanVersion);

        XalanDOMString::CharVectorType  cvtXercesVersion(theManager);
        szXercesVersion.transcode(cvtXercesVersion);

        cerr << &cvtXalanVersion[0] << endl;
        cerr << &cvtXercesVersion[0] << endl;
    }
    catch(const XalanDOMString::TranscodingError&) 
    {
        cerr << endl
             << "Transcoding error: wrong XalanC or XercesC versions."
             << endl;

        bErrorState = false;
    } 

    XalanDOMString::CharVectorType  cvtXalanExeHelpMenu(theManager);

    XalanDOMString theBuffer(theManager);

    for (int i = XalanMessages::XalanExeHelpMenu;
            bErrorState && (i <=  XalanMessages::XalanExeHelpMenu12);
                ++i)
    {
        try
        {
            
            XalanMessageLoader::getMessage(
                theBuffer,
                XalanMessages::Codes(i));

            theBuffer.transcode(cvtXalanExeHelpMenu);

            cerr  << &cvtXalanExeHelpMenu[0] << endl;
        }
        catch(const XalanDOMString::TranscodingError&) 
        {
            cerr << endl
                 << "Cannot read help message "
                 << i
                 << "."
                 << endl;

            bErrorState = false;
        }
    }
}



XALAN_USING_XALAN(XalanTransformer)
XALAN_USING_XALAN(XSLTInputSource)
XALAN_USING_XALAN(XSLTResultTarget)



class Params
{
public:

    Params(unsigned long    maxParams) :
        m_validate(false),
        m_useStylesheetPI(false),
        m_omitMETATag(false),
        m_noURLEscaping(false),
        m_showTiming(false),
        m_indentAmount(-1),
        m_inFileName(0),
        m_xslFileName(0),
        m_outFileName(0),
        m_encoding(0),
        m_params(),
        m_maxParams(maxParams),
        m_currentParam(0)
    {
    }

    bool
    addParam(
            const char*     name,
            const char*     expression)
    {
        if (m_currentParam == m_maxParams)
        {
            return false;
        }
        else
        {
            // Allocate memory if necessary...
            if (m_params.get() == 0)
            {
                m_params.reset(new ParamPair[m_maxParams]);
            }
            assert(m_params.get() != 0);

            m_params[m_currentParam].m_name = name;
            m_params[m_currentParam].m_expression = expression;

            ++m_currentParam;

            return true;
        }
    };

    void
    setParams(XalanTransformer&     theTransformer) const
    {
        theTransformer.setUseValidation(m_validate);

        if (m_omitMETATag == true)
        {
            theTransformer.setOmitMETATag(XalanTransformer::eOmitMETATagYes);
        }

        if (m_noURLEscaping == true)
        {
            theTransformer.setEscapeURLs(XalanTransformer::eEscapeURLsNo);
        }

        if (m_indentAmount >= 0)
        {
            theTransformer.setIndent(m_indentAmount);
        }

        for(unsigned long i = 0; i < m_currentParam; ++i)
        {
            theTransformer.setStylesheetParam(
                m_params[i].m_name,
                m_params[i].m_expression);
        }
    }

    bool            m_validate;
    bool            m_useStylesheetPI;
    bool            m_omitMETATag;
    bool            m_noURLEscaping;
    bool            m_showTiming;

    int             m_indentAmount;

    const char*     m_inFileName;
    const char*     m_xslFileName;
    const char*     m_outFileName;

    const char*     m_encoding;

    unsigned long
    getMaxParams() const
    {
        return m_maxParams;
    }

private:

    struct ParamPair
    {
        ParamPair() :
            m_name(0),
            m_expression(0)
        {
        }

        const char*     m_name;
        const char*     m_expression;
    };

    typedef XALAN_CPP_NAMESPACE_QUALIFIER XalanArrayAutoPtr<ParamPair>  ArrayAutoPtrType;

    ArrayAutoPtrType        m_params;

    const unsigned long     m_maxParams;
    unsigned long           m_currentParam;
};



bool
getArgs(
            int         argc,
            char*       argv[],
            Params&     params)
{
    bool fSuccess = true;

    for (int i = 1; i < argc && fSuccess == true; ++i)
    {
        if (argv[i][0] == '-' && argv[i][1] != '\0')
        {
            if (argv[i][2] != '\0')
            {
                fSuccess = false;
            }
            else if (params.m_inFileName != 0 || params.m_xslFileName != 0)
            {
                fSuccess = false;
            }
            else if (argv[i][1] == 'a') 
            {
                params.m_useStylesheetPI = true;
            }
            else if (argv[i][1] == 'e') 
            {
                ++i;

                if(i < argc && argv[i][0] != '-' &&
                   strlen(argv[i]) != 0)
                {
                    params.m_encoding = argv[i];
                }
                else
                {
                    fSuccess = false;
                }
            }
            else if (argv[i][1] == 'i') 
            {
                ++i;

                if(i < argc && argv[i][0] != '-' &&
                   strlen(argv[i]) != 0)
                {
                    params.m_indentAmount = atoi(argv[i]);
                }
                else
                {
                    fSuccess = false;
                }
            }
            else if (argv[i][1] == 'm') 
            {
                params.m_omitMETATag = true;
            }
            else if (argv[i][1] == 'o') 
            {
                ++i;

                if(i < argc && argv[i][0] != '-' &&
                   strlen(argv[i]) != 0)
                {
                    params.m_outFileName = argv[i];
                }
                else
                {
                    fSuccess = false;
                }
            }
            else if (argv[i][1] == 'p') 
            {
                ++i;

                if(i >= argc || argv[i][0] == '-')
                {
                    fSuccess = false;
                }
                else
                {
                    const char* const   name = argv[i];

                    ++i;

                    // Don't check for '-' here, since that might
                    // be a valid character in a parameter value.
                    if(i >= argc)
                    {
                        fSuccess = false;
                    }
                    else
                    {
                        const char* const   value = argv[i];

                        if (params.addParam(name, value) == false)
                        {
                            cerr << "The maximum number of parameters is "
                                 << params.getMaxParams()
                                 << "."
                                 << endl;

                            fSuccess = false;
                        }
                    }
                }
            }
            else if (argv[i][1] == 't') 
            {
                params.m_showTiming = true;
            }
            else if (argv[i][1] == 'u') 
            {
                params.m_noURLEscaping = true;
            }
            else if (argv[i][1] == 'v')
            {
                params.m_validate = true;
            }
            else
            {
                fSuccess = false;
            }
        }
        else if (params.m_inFileName == 0 &&
                 strlen(argv[i]) != 0)
        {
            params.m_inFileName = argv[i];

            if (strlen(params.m_inFileName) == 0)
            {
                fSuccess = false;
            }
        }
        else if (params.m_xslFileName == 0 &&
                 strlen(argv[i]) != 0 &&
                 params.m_useStylesheetPI == false)
        {
            params.m_xslFileName = argv[i];

            if (strlen(params.m_xslFileName) == 0)
            {
                fSuccess = false;
            }
        }
        else
        {
            fSuccess = false;
        }
    }

    if (fSuccess == true && params.m_inFileName == 0)
    {
        return false;
    }
    else if (params.m_xslFileName == 0)
    {
        return params.m_useStylesheetPI;
    }
    else if (strcmp(params.m_xslFileName, params.m_inFileName) == 0)
    {
        return false;
    }
    else
    {
        return fSuccess;
    }
}



#if defined(XALAN_USE_WINDOWS_TIMING)
typedef LARGE_INTEGER   ClockType;
#else
#if defined(XALAN_STRICT_ANSI_HEADERS)
typedef std::clock_t    ClockType;
#else
typedef clock_t         ClockType;
#endif
#endif

typedef ostream         OstreamType;


inline ClockType
getClock()
{
#if defined(XALAN_USE_WINDOWS_TIMING)
    ClockType   theResult;

    QueryPerformanceCounter(&theResult);

    return theResult;
#else
#if defined(XALAN_STRICT_ANSI_HEADERS)
    return std::clock();
#else
    return clock();
#endif
#endif
}



#if defined(XALAN_USE_WINDOWS_TIMING)
inline ClockType
getPerformanceFrequencyInMilliseconds()
{
    ClockType   theInterval;

    QueryPerformanceFrequency(&theInterval);

    theInterval.QuadPart /= 1000UL;

    return theInterval;
}
#endif



void
writeElapsedMilliseconds(
            ClockType       theStartClock,
            ClockType       theEndClock,
            OstreamType&    theStream)
{
#if defined(XALAN_USE_WINDOWS_TIMING)
    static const ClockType  theInterval =
        getPerformanceFrequencyInMilliseconds();

    char        theBuffer[1000];

    ClockType     theDiff;
    
    theDiff.QuadPart =
        theEndClock.QuadPart - theStartClock.QuadPart;

    ClockType     theResult;
    ClockType     theRemainder;
    
    theResult.QuadPart =
        theDiff.QuadPart / theInterval.QuadPart;

    theRemainder.QuadPart =
        theDiff.QuadPart % theInterval.QuadPart;

    sprintf(
        theBuffer,
        "%I64d.%I64d",
        theResult.QuadPart,
        theRemainder.QuadPart);

    theStream << theBuffer;
#else
    theStream << (double(theEndClock - theStartClock) /
                        CLOCKS_PER_SEC) * 1000.0;
#endif
}



inline void
reportElapsedMilliseconds(
            const char*     theString,
            ClockType       theStartClock,
            ClockType       theEndClock,
            OstreamType&    theStream)
{
    theStream << theString;

    writeElapsedMilliseconds(
        theStartClock,
        theEndClock,
        theStream);

    theStream << " milliseconds.\n";
}



XALAN_USING_XALAN(XalanParsedSource)
XALAN_USING_XALAN(XalanCompiledStylesheet)


typedef XalanTransformer::EnsureDestroyParsedSource         SourceGuard;
typedef XalanTransformer::EnsureDestroyCompiledStylesheet   StylesheetGuard;



inline int
transform(
            XalanTransformer&           theTransformer,
            const Params&               theParams,
            const XSLTInputSource&      theSource,
            const XSLTResultTarget&     theTarget)
{
    if (theParams.m_showTiming == false)
    {
        return theTransformer.transform(
                    theSource,
                    theTarget);
    }
    else
    {
        ClockType   theStartClock = getClock();

        const XalanParsedSource*    theParsedSource = 0;

        int theResult = theTransformer.parseSource(theSource, theParsedSource);

        if (theResult == 0)
        {
            ClockType       theEndClock = getClock();

            reportElapsedMilliseconds(
                "Source tree parsing time: ",
                theStartClock,
                theEndClock,
                cerr);

            const SourceGuard   theGuard(theTransformer, theParsedSource);

            theStartClock = getClock();

            theResult = theTransformer.transform(*theParsedSource, theTarget);

            theEndClock = getClock();

            reportElapsedMilliseconds(
                "Transformation time, including stylesheet compilation: ",
                theStartClock,
                theEndClock,
                cerr);
        }

        return theResult;
    }
}



inline int
transform(
            XalanTransformer&           theTransformer,
            const Params&               theParams,
            const XSLTInputSource&      theSource,
            const XSLTInputSource&      theStylesheetSource,
            const XSLTResultTarget&     theTarget)
{
    if (theParams.m_showTiming == false)
    {
        return theTransformer.transform(
                theSource,
                theStylesheetSource,
                theTarget);
    }
    else
    {
        ClockType   theStartClock = getClock();

        const XalanParsedSource*    theParsedSource = 0;

        int     theResult =
            theTransformer.parseSource(
                                theSource,
                                theParsedSource);

        if (theResult == 0)
        {
            ClockType   theEndClock = getClock();

            reportElapsedMilliseconds(
                "Source tree parsing time: ",
                theStartClock,
                theEndClock,
                cerr);

            const SourceGuard   theGuard(theTransformer, theParsedSource);

            const XalanCompiledStylesheet*  theStylesheet = 0;

            theStartClock = getClock();

            theResult =
                theTransformer.compileStylesheet(
                    theStylesheetSource,
                    theStylesheet);

            if (theResult == 0)
            {
                theEndClock = getClock();

                reportElapsedMilliseconds(
                    "Stylesheet compilation time: ",
                    theStartClock,
                    theEndClock,
                    cerr);

                assert(theStylesheet != 0);

                const StylesheetGuard   theGuard(theTransformer, theStylesheet);

                theStartClock = getClock();

                theResult =
                    theTransformer.transform(
                        *theParsedSource,
                        theStylesheet,
                        theTarget);

                theEndClock = getClock();

                reportElapsedMilliseconds(
                    "Transformation time: ",
                    theStartClock,
                    theEndClock,
                    cerr);
            }
        }

        return theResult;
    }
}



inline int
transform(
            XalanTransformer&       theTransformer,
            const Params&           theParams,
            const XSLTInputSource&  theSource,
            const XSLTInputSource&  theStylesheetSource)
{
    XALAN_USING_XALAN(XalanDOMString)
    XALAN_USING_XALAN(XSLTResultTarget)
    XALAN_USING_XERCES(MemoryManager)

    MemoryManager&  theManager = theTransformer.getMemoryManager();

    XSLTResultTarget    theTarget(theManager);

    if (theParams.m_encoding != 0)
    {
        const XalanDOMString    theEncoding(theParams.m_encoding, theManager);

        theTarget.setEncoding(theEncoding);
    }

    if (theParams.m_outFileName != 0)
    {
        theTarget.setFileName(theParams.m_outFileName);
    }
    else
    {
        theTarget.setByteStream(&cout);
    }

    if (theParams.m_useStylesheetPI == true)
    {
        return transform(
                    theTransformer,
                    theParams,
                    theSource,
                    theTarget);
    }
    else
    {
        return transform(
                    theTransformer,
                    theParams,
                    theSource,
                    theStylesheetSource,
                    theTarget);
    }
}



inline int
transform(
            XalanTransformer&       theTransformer,
            const Params&           theParams,
            const XSLTInputSource&  theSource)
{
    assert(
        theParams.m_useStylesheetPI == true ||
        theParams.m_xslFileName != 0);

    if (theParams.m_useStylesheetPI == true ||
        (theParams.m_xslFileName[0] == '-' &&
         theParams.m_xslFileName[1] == '\0'))
    {
        return transform(
                theTransformer,
                theParams,
                theSource,
                &cin);
    }
    else
    {
        return transform(
                theTransformer,
                theParams,
                theSource,
                XSLTInputSource(theParams.m_xslFileName));
    }
}



inline int
transform(
            XalanTransformer&   theTransformer,
            const Params&       theParams)
{
    assert(theParams.m_inFileName != 0);

    if (theParams.m_inFileName[0] == '-' &&
        theParams.m_inFileName[1] == '\0')
    {
        return transform(
                    theTransformer,
                    theParams,
                    &cin);
    }
    else
    {
        return transform(
                    theTransformer,
                    theParams,
                    theParams.m_inFileName);
    }
}



#if defined(XALAN_WINDOWS)

XALAN_USING_XERCES(MemoryManager)

class WindowsMemoryManager : public XALAN_CPP_NAMESPACE_QUALIFIER XalanMemoryManager
{
public:

    typedef XALAN_CPP_NAMESPACE_QUALIFIER XalanSize_t   XalanSize_t;

    WindowsMemoryManager() :
        XalanMemoryManager(),
        m_handle(HeapCreate(HEAP_NO_SERIALIZE, 0, 0))
    {
        assert(m_handle != 0);
    }

    virtual
    ~WindowsMemoryManager()
    {
        HeapDestroy(m_handle);
    }

    /**
      * This method allocates requested memory.
      *
      * size The requested memory size
      *
      * Returns a pointer to the allocated memory
      */
    virtual void*
    allocate(size_type  size)
    {
        void* const     value =
                HeapAlloc(m_handle, HEAP_NO_SERIALIZE, size);

        if (value == 0)
        {
            throw XERCES_CPP_NAMESPACE_QUALIFIER OutOfMemoryException();
        }

        return value;
    }

    /**
      * This method deallocates memory
      *
      */
    virtual void
    deallocate(void*    pointer)
    {
        HeapFree(m_handle, HEAP_NO_SERIALIZE, pointer);
    }

    MemoryManager*
    getExceptionMemoryManager()
    {
        return this;
    }

protected:

private:

    // These are not implemented.
    WindowsMemoryManager(const WindowsMemoryManager&);

    WindowsMemoryManager&
    operator=(const WindowsMemoryManager&);


    // Data members.
    const HANDLE    m_handle;
};
#endif



int
xsltMain(
            int     argc,
            char*   argv[])

{
    int theResult = -1;

    XALAN_USING_XERCES(XMLPlatformUtils)

#if defined(XALAN_WINDOWS) && defined(NDEBUG)
    WindowsMemoryManager  theMemoryManager;

    // Call the static initializer for Xerces...
    XMLPlatformUtils::Initialize(
        XERCES_CPP_NAMESPACE_QUALIFIER XMLUni::fgXercescDefaultLocale,
        0,
        0,
        &theMemoryManager);
#else
    // Call the static initializer for Xerces...
    XMLPlatformUtils::Initialize();

    XALAN_USING_XERCES(MemoryManager)
 
    MemoryManager&  theMemoryManager =
            *XMLPlatformUtils::fgMemoryManager;
#endif

    // Initialize Xalan...
    XalanTransformer::initialize(theMemoryManager);

    {
        // we need to read the params after the XMLPlatformUtils::Initialize(),
        // because we may need the local and the local dlls for usage of the
        // Usage function.

        // Set the maximum number of params as half of argc - 1.
        // It's actually argc - 2, but that could get us into negative
        // numbers, so don't bother.  Also, always add 1, in case
        // (argc - 1) / 2 is 0.
        Params  theParams((argc - 1) / 2 + 1);

        if (getArgs(argc, argv, theParams) == false)
        {
            Usage();
        }
        else
        {
            // Create a XalanTransformer instance...
            XalanTransformer    theTransformer(theMemoryManager);

            // Set any options...
            theParams.setParams(theTransformer);

            theResult = transform(theTransformer, theParams);

            if (theResult != 0)
            {
                cerr << theTransformer.getLastError() << endl;
            }
        }
    }

    // Terminate Xalan...
    XalanTransformer::terminate();

    // Terminate Xerces...
    XMLPlatformUtils::Terminate();

    // Clean up the ICU, if it's integrated...
    XalanTransformer::ICUCleanUp();

    return theResult;
}



int
main(
            int     argc,
            char*   argv[])
 {
#if defined(_DEBUG) && defined(_MSC_VER)
    _CrtSetDbgFlag(
        _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif

    return xsltMain(argc, argv);
}
