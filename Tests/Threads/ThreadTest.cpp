// Base header file.  Must be first.
#include <Include/PlatformDefinitions.hpp>



#include <cassert>
#include <ctime>



#if defined(XALAN_OLD_STREAM_HEADERS)
#include <iostream.h>
#else
#include <iostream>
#endif



#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/Mutexes.hpp>



#include <Include/XalanAutoPtr.hpp>



#include <XalanTransformer/XalanTransformer.hpp>



#if defined(WIN32)

#include <csignal>
#include <process.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#elif defined(XALAN_POSIX2_AVAILABLE)

#include <csignal>

// This is a workaround for a Tru64 compiler bug...
#if defined(XALAN_STRICT_ANSI_HEADERS) && defined(TRU64)
#include <setjmp.h>
#endif
#include <pthread.h>
#include <unistd.h>

#else
#error Unsupported platform!
#endif



#if !defined(XALAN_NO_NAMESPACES)
	using std::cerr;
	using std::cout;
	using std::endl;
#if defined(XALAN_STRICT_ANSI_HEADERS)
	using std::atoi;
	using std::signal;
	using std::strcmp;
#endif
#endif


	
// This is here for memory leak testing.
#if defined(_DEBUG)
#include <crtdbg.h>
#endif



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

	mutable XMLMutex	m_mutex;

	long				m_counter;
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
	XMLMutexLock	theLock(&m_mutex);

	if (m_counter < ULONG_MAX)
	{
		++m_counter;
	}
}



void
SynchronizedCounter::decrement()
{
	XMLMutexLock	theLock(&m_mutex);

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
			unsigned int			theThreadNumber = 0,
			SynchronizedCounter*	theCounter = 0) :
		m_threadNumber(theThreadNumber),
		m_counter(theCounter),
		m_done(false)
	{
	}

	unsigned int			m_threadNumber;

	SynchronizedCounter*	m_counter;

	bool					m_done;
};



// Used to hold compiled stylesheet and pre-parsed source...
const XalanCompiledStylesheet*	glbCompiledStylesheet = 0;
const XalanParsedSource*		glbParsedSource = 0;
bool							fContinue = true;


#if defined(WIN32)
static BOOL __stdcall
signalHandler(DWORD		theSignalType)
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



#if defined(WIN32)

extern "C" void theThreadRoutine(void* param);

void
#elif defined(XALAN_POSIX2_AVAILABLE)

extern "C" void* theThreadRoutine(void* param);

void*
#else
#error Unsupported platform!
#endif
theThreadRoutine(void*		param)
{
// This routine uses compiled stylesheet (glbStylesheetRoot), which is set using the 
// theProcessor.setStylesheetRoot method. The transform is done using the theProcessor's
// process() method.

#if defined(XALAN_OLD_STYLE_CASTS)
	ThreadInfo* const		theInfo = (ThreadInfo*)param;
#else
	ThreadInfo* const		theInfo = reinterpret_cast<ThreadInfo*>(param);
#endif

	assert(theInfo != 0);

	theInfo->m_counter->increment();

	try
	{
		// Our input file.  The assumption is that the executable will be run
		// from same directory as the input files.

		// Generate the output file name.
		const XalanDOMString	theOutputFile(
				XalanDOMString("birds") +
				UnsignedLongToDOMString(theInfo->m_threadNumber) +
				XalanDOMString(".out"));

		// Create a transformer...
		XalanTransformer	theTransformer;

		// Do the transform...
		theTransformer.transform(*glbParsedSource, glbCompiledStylesheet, XSLTResultTarget(theOutputFile));
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
doSleep(unsigned int	theMilliseconds)
{
#if defined(WIN32)
	Sleep(theMilliseconds);
#elif defined(XALAN_POSIX2_AVAILABLE)
	usleep(theMilliseconds * 10);
#else
#error Unsupported platform!
#endif
}



bool
createThread(ThreadInfo&	theThreadInfo)
{
	theThreadInfo.m_done = false;

#if defined(WIN32)

	const unsigned long		theThreadID =
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

	pthread_t	theThread;

	const int	theResult = pthread_create(&theThread, 0, theThreadRoutine, (void*)&theThreadInfo);

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
			const SynchronizedCounter&	theCounter,
			clock_t&					theClock)
{
	cout << "Waiting for active threads to finish..." << endl;

	unsigned int	theCheckCount = 0;

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
doContinuousThreads(
			const SynchronizedCounter&	theCounter,
			ThreadInfo					theThreadInfo[],
			long						theThreadCount,
			clock_t&					theClock)
{
	while(fContinue == true)
	{
//		doSleep(100);

		if (theCounter.getCounter() < theThreadCount)
		{
			for (long i = 0; i < theThreadCount && fContinue == true; ++i)
			{
				if (theThreadInfo[i].m_done == true)
				{
					if (createThread(theThreadInfo[i]) == false)
					{
						cerr << endl << "Unable to create thread!" << endl;
					}
					else
					{
						cout << "Started thread number " << i << "." << endl;
					}
				}
			}
		}
	}

	doCountedThreads(theCounter, theClock);
}



void
doThreads(
			long	theThreadCount,
			bool	fContinuous)
{
	if (fContinuous == true)
	{
#if defined(WIN32)
		SetConsoleCtrlHandler(
				signalHandler,
				TRUE);
#elif defined(XALAN_POSIX2_AVAILABLE)
#if defined(XALAN_SIGNAL_IN_STD)
		std::signal(SIGINT, signalHandler);
#else
		signal(SIGINT, signalHandler);
#endif
#else
#error Unsupported platform!
#endif

		cout << endl << "Running in continuous mode." << endl;
	}

	cout << endl << "Starting " << theThreadCount << " threads." << endl;

	XalanArrayAutoPtr<ThreadInfo>	theThreadInfo(new ThreadInfo[theThreadCount]);

	try
	{
		cout << endl << "Clock before starting threads: " << clock() << endl;

		SynchronizedCounter		theCounter;

		long	i = 0;

		while (i < theThreadCount && fContinue == true)
		{
			theThreadInfo[i].m_threadNumber = i;
			theThreadInfo[i].m_counter = &theCounter;

			if (createThread(theThreadInfo[i]) == false)
			{
				cerr << endl << "Unable to create thread!" << endl;
			}
			else
			{
				cout << "Started thread number " << i << "." << endl;
			}

			++i;
		}

		clock_t		theClock = 0;

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
			int				argc,
			const char*		argv[])
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
		bool	fContinuous = false;
		int		threadCount = 0;

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
				// Initialize Xerces...
				XMLPlatformUtils::Initialize();

				// Initialize Xalan...
				XalanTransformer::initialize();

				{
					// Create a XalanTransformer.  We won't actually use this to transform --
					// it's just acting as a factory for the compiled stylesheet and
					// pre-parsed source document.  Note that we can't let the individual
					// threads use this as a factory without serializing access to it, but
					// we can share the stylesheet and source document.
					XalanTransformer	theXalanTransformer;

					const char* const	theXSLFileName = "birds.xsl";

					theXalanTransformer.compileStylesheet(theXSLFileName, glbCompiledStylesheet);
					assert(glbCompiledStylesheet != 0);

					// Compile the XML source document as well. All threads will use
					// this binary representation of the source tree.
					const char* const	theXMLFileName = "birds.xml";

					theXalanTransformer.parseSource(theXMLFileName, glbParsedSource);
					assert(glbParsedSource != 0);

					doThreads(threadCount, fContinuous);
				}

				// Terminate Xalan...
				XalanTransformer::terminate();

				// Terminate Xerces...
				XMLPlatformUtils::Terminate();
			}
			catch(...)
			{
				cerr << "Exception caught!!!"
					 << endl
					 << endl;
			}
		}
	} 

	return 0;
}
