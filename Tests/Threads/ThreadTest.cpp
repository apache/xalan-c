// Base header file.  Must be first.
#include <Include/PlatformDefinitions.hpp>



#include <cassert>



#if defined(XALAN_OLD_STREAM_HEADERS)
#include <iostream.h>
#else
#include <iostream>
#endif



#include <util/PlatformUtils.hpp>
#include <util/Mutexes.hpp>



#include <Include/XalanAutoPtr.hpp>



#include <XalanTransformer/XalanTransformer.hpp>



#if defined(WIN32)
//This is here for the threads.
#include <process.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#elif defined(XALAN_POSIX2_AVAILABLE)
#include <pthread.h>
#include <unistd.h>
#else
#error Unsupported platform!
#endif



#if !defined(XALAN_NO_NAMESPACES)
	using std::cerr;
	using std::cout;
	using std::endl;
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

	unsigned long
	getCounter() const;

private:

	mutable XMLMutex	m_mutex;

	unsigned long		m_counter;
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



unsigned long
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
		m_counter(theCounter)
	{
	}

	unsigned int			m_threadNumber;

	SynchronizedCounter*	m_counter;
};



// Used to hold compiled stylesheet and pre-parsed source...
const XalanCompiledStylesheet*	glbCompiledStylesheet = 0;
const XalanParsedSource*		glbParsedSource = 0;



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
	const ThreadInfo* const		theInfo = (const ThreadInfo*)param;
#else
	const ThreadInfo* const		theInfo = reinterpret_cast<const ThreadInfo*>(param);
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



void
doThreads(long	theThreadCount)
{
	cout << endl << "Starting " << theThreadCount << " threads." << endl;

	XalanArrayAutoPtr<ThreadInfo>	theThreadInfo(new ThreadInfo[theThreadCount]);

	try
	{
		cout << endl << "Clock before starting threads: " << clock() << endl;

		SynchronizedCounter		theCounter;

		long	i = 0;

		while (i < theThreadCount)
		{
			theThreadInfo[i].m_threadNumber = i;
			theThreadInfo[i].m_counter = &theCounter;

#if defined(WIN32)

			const unsigned long		theThreadID =
					_beginthread(theThreadRoutine, 4096, reinterpret_cast<LPVOID>(&theThreadInfo[i]));

			if (theThreadID == unsigned(-1))
			{
				cerr << endl << "Unable to create thread number " << i + 1 << "." << endl;
			}

#elif defined(XALAN_POSIX2_AVAILABLE)

			pthread_t	theThread;

			const int	theResult = pthread_create(&theThread, 0, theThreadRoutine, (void*)&theThreadInfo[i]);

			if (theResult != 0)
			{
				cerr << endl << "Unable to create thread number " << i + 1 << "." << endl;
			}
			else
			{
#if defined(OS390)
				pthread_detach(&theThread);
#else
				pthread_detach(theThread);
#endif
			}
#else
#error Unsupported platform!
#endif

			++i;
		}

		clock_t		theClock = 0;

		if (i == 0)
		{
			cerr << endl << "No threads were created!" << endl;
		}
		else
		{
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

		cout << endl << "Clock after threads: " << theClock << endl;
	}
	catch(...)
	{
		cerr << "Exception caught!!!"
			 << endl
			<< endl;
	}
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

	if (argc > 2)
	{
		cerr << "Usage: ThreadTest"
			 << endl
			 << endl;
	}
	else
	{
		int		threadCount = 60;

		if (argc == 2)
		{
			threadCount = atoi(argv[1]);
		}

		try
		{
			// Initialize Xerces...
			XMLPlatformUtils::Initialize();

			// Initialize Xalan...
			XalanTransformer::initialize();

			{
				// Create a XalanTransformer.  We won't actually use this to transform --
				// it's just acting likely a factory for the compiled stylesheet and
				// pre-parsed source.
				XalanTransformer	theXalanTransformer;

				const char* const	theXSLFileName = "birds.xsl";

				theXalanTransformer.compileStylesheet(theXSLFileName, glbCompiledStylesheet);
				assert(glbCompiledStylesheet != 0);

				// Compile the XML source document as well. All threads will use
				// this binary representation of the source tree.
				const char* const	theXMLFileName = "birds.xml";

				theXalanTransformer.parseSource(theXMLFileName, glbParsedSource);
				assert(glbParsedSource != 0);

				doThreads(threadCount);
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

	return 0;
}
