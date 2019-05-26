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

// Base header file.  Must be first.
#include <xalanc/Include/PlatformDefinitions.hpp>



#include <xalanc/Include/XalanMemoryManagement.hpp>



#include <cassert>
#include <climits>
#include <cstring>
#include <ctime>



#include <iostream>



#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/Mutexes.hpp>



#include <xalanc/Include/XalanAutoPtr.hpp>



#include <xalanc/XalanTransformer/XalanTransformer.hpp>



#if defined(WINDOWS_THREAD_FUNCTIONS)

#include <csignal>
#include <process.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#elif defined(XALAN_POSIX2_AVAILABLE)

#include <csignal>

// This is a workaround for a Tru64 compiler bug...
#if defined(TRU64)
#include <csetjmp>
typedef long sigjmp_buf[_JBLEN];
#endif
#include <pthread.h>
#include <unistd.h>

#else
#error Unsupported platform!
#endif



using std::cerr;
using std::cout;
using std::endl;


    
using std::atoi;
using std::signal;
using std::strcmp;

    
    
    // This is here for memory leak testing.
#if !defined(NDEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif



typedef xercesc::XMLMutex         XMLMutexType;
typedef xercesc::XMLMutexLock     XMLMutexLockType;



class SynchronizedCounter
{
public:

    SynchronizedCounter();

    ~SynchronizedCounter();

    void
    increment();

    void
    decrement();

    long
    getCounter() const;

private:

    XMLMutexType    m_mutex;

    long            m_counter;
};



SynchronizedCounter::SynchronizedCounter() :
    m_mutex(),
    m_counter(0)
{
}



SynchronizedCounter::~SynchronizedCounter()
{
}



void
SynchronizedCounter::increment()
{
    const XMLMutexLockType  theLock(&m_mutex);

    if (m_counter < LONG_MAX)
    {
        ++m_counter;
    }
}



void
SynchronizedCounter::decrement()
{
    const XMLMutexLockType  theLock(&m_mutex);

    if (m_counter > 0)
    {
        --m_counter;
    }
}



long
SynchronizedCounter::getCounter() const
{
    return m_counter;
}



struct
ThreadInfo
{
    ThreadInfo(
            XMLInt32                theThreadNumber = 0L,
            SynchronizedCounter*    theCounter = 0) :
        m_threadNumber(theThreadNumber),
        m_counter(theCounter),
        m_done(false)
    {
    }

    XMLInt32                m_threadNumber;

    SynchronizedCounter*    m_counter;

    bool                    m_done;
};



using xalanc::XalanCompiledStylesheet;
using xalanc::XalanDOMChar;
using xalanc::XalanDOMString;
using xalanc::XalanParsedSource;



// Used to hold compiled stylesheet and pre-parsed source...
const XalanCompiledStylesheet*  glbCompiledStylesheet = 0;
const XalanParsedSource*        glbParsedSource = 0;
bool                            fContinue = true;

const XalanDOMChar*             theStylesheetFileName = 0;
const XalanDOMChar*             theSourceFileName = 0;


#if defined(WINDOWS_THREAD_FUNCTIONS)
static BOOL __stdcall
signalHandler(DWORD     theSignalType)
{
    if (theSignalType == CTRL_C_EVENT ||
        theSignalType == CTRL_BREAK_EVENT)
    {
        fContinue = false;

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#elif defined(XALAN_POSIX2_AVAILABLE)
extern "C"
{
static void
signalHandler(int)
{
    fContinue = false;
}
}
#else
#error Unsupported platform!
#endif



#if defined(WINDOWS_THREAD_FUNCTIONS)

extern "C"
{
    void thePreparsedThreadRoutine(void* param);

    typedef void (*ThreadRoutineType)(void* param);
}

void
#elif defined(XALAN_POSIX2_AVAILABLE)

extern "C"
{
    void* thePreparsedThreadRoutine(void* param);

    typedef void* (*ThreadRoutineType)(void* param);
}

void*
#else
#error Unsupported platform!
#endif
thePreparsedThreadRoutine(void*     param)
{
// This routine uses compiled stylesheet (glbStylesheetRoot), which is set using the 
// theProcessor.setStylesheetRoot method. The transform is done using the theProcessor's
// process() method.

    ThreadInfo* const       theInfo = reinterpret_cast<ThreadInfo*>(param);

    assert(theInfo != 0);

    theInfo->m_counter->increment();

    using xalanc::MemoryManager;
    using xalanc::XalanMemMgrs;

    MemoryManager& theManager = XalanMemMgrs::getDefaultXercesMemMgr();

    try
    {
        using xalanc::NumberToDOMString;
        using xalanc::XalanTransformer;

        // Our input file.  The assumption is that the executable will be run
        // from same directory as the input files.

        // Generate the output file name.
        XalanDOMString  theOutputFile("birds", theManager);

        NumberToDOMString(theInfo->m_threadNumber, theOutputFile);
        theOutputFile.append(".out");

        // Create a transformer...
        XalanTransformer    theTransformer(theManager);

        using xalanc::XSLTResultTarget;

        // Do the transform...
        theTransformer.transform(*glbParsedSource, glbCompiledStylesheet, XSLTResultTarget( theOutputFile, theManager));
    }
    catch(...)
    {
        cerr << "Exception caught in thread " << theInfo->m_threadNumber;
    }

    // Decrement the counter because we're done...
    theInfo->m_counter->decrement();

    theInfo->m_done = true;

#if defined(XALAN_POSIX2_AVAILABLE)
    return 0;
#endif
}



#if defined(WINDOWS_THREAD_FUNCTIONS)

extern "C" void theUnparsedThreadRoutine(void* param);

void
#elif defined(XALAN_POSIX2_AVAILABLE)

extern "C" void* theUnparsedThreadRoutine(void* param);

void*
#else
#error Unsupported platform!
#endif
theUnparsedThreadRoutine(void*      param)
{
// This routine compiles a stylesheet and a source document

    ThreadInfo* const       theInfo = reinterpret_cast<ThreadInfo*>(param);

    assert(theInfo != 0);

    theInfo->m_counter->increment();

    try
    {
        using xalanc::NumberToDOMString;
        using xalanc::XalanTransformer;

        // Our input file.  The assumption is that the executable will be run
        // from same directory as the input files.

        using xalanc::MemoryManager;
        using xalanc::XalanMemMgrs;

        MemoryManager& theManager = XalanMemMgrs::getDefaultXercesMemMgr();

        // Generate the output file name.
        XalanDOMString  theOutputFile("birds", theManager);

        NumberToDOMString(theInfo->m_threadNumber, theOutputFile);
        theOutputFile.append(".out");

        // Create a transformer...
        XalanTransformer    theTransformer( theManager );

        assert(theSourceFileName != 0 && theStylesheetFileName != 0);

        using xalanc::XSLTResultTarget;

        // Do the transform...
        theTransformer.transform(
            theSourceFileName,
            theStylesheetFileName,
            XSLTResultTarget( theOutputFile, theManager));
    }
    catch(...)
    {
        cerr << "Exception caught in thread " << theInfo->m_threadNumber;
    }

    // Decrement the counter because we're done...
    theInfo->m_counter->decrement();

    theInfo->m_done = true;

#if defined(XALAN_POSIX2_AVAILABLE)
    return 0;
#endif
}



inline void
doSleep(unsigned int    theMilliseconds)
{
#if defined(WINDOWS_THREAD_FUNCTIONS)
    Sleep(theMilliseconds);
#elif defined(XALAN_POSIX2_AVAILABLE)
    usleep(theMilliseconds * 10);
#else
#error Unsupported platform!
#endif
}



bool
createThread(
            ThreadInfo&         theThreadInfo,
            ThreadRoutineType   theThreadRoutine)
{
    theThreadInfo.m_done = false;

#if defined(WINDOWS_THREAD_FUNCTIONS)

    const XMLInt32     theThreadID =
            _beginthread(theThreadRoutine, 4096, reinterpret_cast<LPVOID>(&theThreadInfo));

    if (theThreadID == unsigned(-1))
    {
        theThreadInfo.m_done = true;

        return false;
    }
    else
    {
        return true;
    }

#elif defined(XALAN_POSIX2_AVAILABLE)

    pthread_t   theThread;

    const int   theResult = pthread_create(&theThread, 0, theThreadRoutine, (void*)&theThreadInfo);

    if (theResult != 0)
    {
        theThreadInfo.m_done = true;

        return false;
    }
    else
    {
#if defined(OS390)
        pthread_detach(&theThread);
#else
        pthread_detach(theThread);
#endif

        return true;
    }
#else
#error Unsupported platform!
#endif
}



void
doCountedThreads(
            const SynchronizedCounter&  theCounter,
            clock_t&                    theClock)
{
    cout << "Waiting for active threads to finish..." << endl;

    unsigned int    theCheckCount = 0;

    do
    {
        doSleep(2000);

        // Check a couple of times, just in case, since
        // getCounter() is not synchronized...
        if (theCounter.getCounter() == 0)
        {
            if (theCheckCount == 0)
            {
                theClock = clock();
            }

            ++theCheckCount;
        }
    }
    while(theCheckCount < 2);
}



void
startThread(
            ThreadInfo                  theThreadInfo[],
            XMLInt32                    theThreadNumber)
{
    bool    fResult = false;

    const bool  fPreparsed = theThreadNumber % 2 == 0 ? true : false;

    if (fPreparsed == true)
    {
        fResult = createThread(theThreadInfo[theThreadNumber], thePreparsedThreadRoutine);
    }
    else
    {
        fResult = createThread(theThreadInfo[theThreadNumber], theUnparsedThreadRoutine);
    }

    if (fResult == false)
    {
        cerr << endl << "Unable to create thread!" << endl;
    }
    else
    {
        cout << "Started thread number " << theThreadNumber << ", using ";

        if (fPreparsed == true)
        {
            cout << "pre-parsed documents." << endl;
        }
        else
        {
            cout << "unparsed documents." << endl;
        }
    }
}



void
doContinuousThreads(
            const SynchronizedCounter&  theCounter,
            ThreadInfo                  theThreadInfo[],
            XMLInt32                    theThreadCount,
            clock_t&                    theClock)
{
    while(fContinue == true)
    {
        if (theCounter.getCounter() < theThreadCount)
        {
            for (XMLInt32 i = 0; i < theThreadCount && fContinue == true; ++i)
            {
                if (theThreadInfo[i].m_done == true)
                {
                    startThread(theThreadInfo, i);
                }
            }
        }
    }

    doCountedThreads(theCounter, theClock);
}



void
doThreads(
            XMLInt32 theThreadCount,
            bool     fContinuous)
{
    if (fContinuous == true)
    {
#if defined(WINDOWS_THREAD_FUNCTIONS)
        SetConsoleCtrlHandler(
                signalHandler,
                TRUE);
#elif defined(XALAN_POSIX2_AVAILABLE)
        signal(SIGINT, signalHandler);
#else
#error Unsupported platform!
#endif

        cout << endl << "Running in continuous mode." << endl;
    }

    cout << endl << "Starting " << theThreadCount << " threads." << endl;

    using xalanc::XalanArrayAutoPtr;

    XalanArrayAutoPtr<ThreadInfo>   theThreadInfo(new ThreadInfo[theThreadCount]);

    try
    {
        cout << endl << "Clock before starting threads: " << clock() << endl;

        SynchronizedCounter     theCounter;

        XMLInt32    i = 0;

        while (i < theThreadCount && fContinue == true)
        {
            theThreadInfo[i].m_threadNumber = i;
            theThreadInfo[i].m_counter = &theCounter;

            startThread(theThreadInfo.get(), i);

            ++i;
        }

        clock_t     theClock = 0;

        if (i == 0)
        {
            cerr << endl << "No threads were created!" << endl;
        }
        else
        {
            if (fContinuous == true)
            {
                doContinuousThreads(
                    theCounter,
                    theThreadInfo.get(),
                    theThreadCount,
                    theClock);
            }
            else
            {
                doCountedThreads(theCounter, theClock);
            }
        }

        cout << endl << "Clock after threads: " << theClock << endl;
    }
    catch(...)
    {
        cerr << "Exception caught!!!"
             << endl
            << endl;
    }
}



void
Usage()
{
    cerr << "Usage: ThreadTest [-c] [count]"
         << endl
         << endl;
}



int
main(
            int     argc,
            char*   argv[])
{
#if !defined(NDEBUG) && defined(_MSC_VER)
    _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif

    if (argc > 3)
    {
        Usage();
    }
    else
    {
        bool    fContinuous = false;
        int     threadCount = 0;

        if (argc == 1)
        {
            threadCount = 60;
        }
        else
        {
            if (strcmp(argv[1], "-c") == 0)
            {
                fContinuous = true;
            }

            if (argc == 2)
            {
                if (fContinuous == true)
                {
                    threadCount = 60;
                }
                else
                {
                    threadCount = atoi(argv[1]);
                }
            }
            else if (argc == 3)
            {
                if (fContinuous == true)
                {
                    threadCount = atoi(argv[2]);
                }
                else
                {
                    Usage();
                }
            }
        }

        if (threadCount > 0)
        {
            try
            {
                using xercesc::XMLPlatformUtils;

                using xalanc::XalanTransformer;

                // Initialize Xerces...
                XMLPlatformUtils::Initialize();

                // Initialize Xalan...
                XalanTransformer::initialize();

                try
                {
                    // Create a XalanTransformer.  We won't actually use this to transform --
                    // it's just acting as a factory for the compiled stylesheet and
                    // pre-parsed source document.  Note that we can't let the individual
                    // threads use this as a factory without serializing access to it, but
                    // we can share the stylesheet and source document.
                    using xalanc::MemoryManager;
                    using xalanc::XalanMemMgrs;

                    MemoryManager& theManager = XalanMemMgrs::getDefaultXercesMemMgr();

                    XalanTransformer        theXalanTransformer(theManager);

                    const XalanDOMString    theXSLFileName("birds.xsl", theManager);

                    theStylesheetFileName = theXSLFileName.c_str();

                    theXalanTransformer.compileStylesheet(theStylesheetFileName, glbCompiledStylesheet);
                    assert(glbCompiledStylesheet != 0);

                    // Compile the XML source document as well. All threads will use
                    // this binary representation of the source tree.
                    const XalanDOMString    theXMLFileName("birds.xml", theManager);

                    theSourceFileName = theXMLFileName.c_str();

                    theXalanTransformer.parseSource(theSourceFileName, glbParsedSource);
                    assert(glbParsedSource != 0);

                    doThreads(threadCount, fContinuous);

                    theStylesheetFileName = 0;
                    theSourceFileName = 0;
                }
                catch(...)
                {
                    cerr << "Exception caught!!!"
                         << endl
                         << endl;
                }

                // Terminate Xalan...
                XalanTransformer::terminate();

                // Terminate Xerces...
                XMLPlatformUtils::Terminate();

                // Clean up the ICU, if it's integrated.
                XalanTransformer::ICUCleanUp();
            }
            catch(...)
            {
                cerr << "Initialization failed!!!"
                     << endl
                     << endl;
            }
        }
    } 

    return 0;
}
