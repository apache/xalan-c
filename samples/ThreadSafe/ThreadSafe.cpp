/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2004 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

#include <xalanc/Include/PlatformDefinitions.hpp>



#include <cassert>
#include <ctime>

#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <fstream.h>
#include <iostream.h>
#include <strstream.h>
#else
#include <fstream>
#include <iostream>
#include <strstream>
#endif



#include <xercesc/util/PlatformUtils.hpp>



#include <xalanc/XalanTransformer/XalanTransformer.hpp>



//This is here for the Windows threads.
#if defined(_MSC_VER)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winbase.h>
#define THREADFUNCTIONRETURN DWORD WINAPI
    typedef   DWORD      theThreadIDType;
    typedef   HANDLE     theThreadType;

//This is here for Unix threads
#elif defined(XALAN_POSIX2_AVAILABLE)
#include <pthread.h>
#include <unistd.h>
    typedef   unsigned long     theThreadIDType;
    typedef   pthread_t         theThreadType;

#else
//error Unsupported Platform!
#endif

#define NUM_THREADS 10




XALAN_USING_STD(cerr)
XALAN_USING_STD(cout)
XALAN_USING_STD(endl)
XALAN_USING_STD(ostrstream)



XALAN_USING_XALAN(XalanCompiledStylesheet)
XALAN_USING_XALAN(XalanDOMString)
XALAN_USING_XALAN(XalanParsedSource)
XALAN_USING_XALAN(XalanTransformer)
XALAN_USING_XALAN(XSLTInputSource)
XALAN_USING_XALAN(XSLTResultTarget)



// Used to hold compiled stylesheet and XML source document.
const XalanCompiledStylesheet*	glbCompiledStylesheet = 0;
const XalanParsedSource*		glbParsedSource = 0;
int								glbError = 0;	 



// Print messages tracking the progress of each thread, and the
// beginning and end of the entire operation.
void
outputMessage(
              theThreadIDType  id,
			const char	msg[])
{
	ostrstream threadMsg;
	
	threadMsg << "\n" << msg << " Thread: " << id << '\0';

	cout << threadMsg.str();

    #if defined(HPUX)
	threadMsg.rdbuf() -> freeze(false);
    #else
	threadMsg.freeze(false);
    #endif
}


#if defined(_MSC_VER)
THREADFUNCTIONRETURN
theThread(LPVOID	param)
#elif defined(XALAN_POSIX2_AVAILABLE)
  void  *theThread(void   *param)
#endif
{
// This routine uses a compiled stylesheet (glbCompiledStylesheet), 
// and a binary source tree (glbParsedSource) to perform the 
// transformation.

	int	theResult = 0;
  
   #if defined(_MSC_VER)
	const int	number = reinterpret_cast<int>(param);
        const theThreadIDType         theThreadID = GetCurrentThreadId();

   #elif defined(XALAN_POSIX2_AVAILABLE)
        const int       number = *(int *)(param);
        const theThreadIDType         theThreadID = pthread_self();

   #endif

	outputMessage(theThreadID, "Starting ");

	// Create a XalanTransformer.
	XalanTransformer	theXalanTransformer;

	// Generate the output file name for this thread.
    ostrstream	theFormatterOut;
    theFormatterOut << "birds" << number << ".out" << '\0';

	// Generate the XML output object.
	const XSLTResultTarget	theResultTarget(XalanDOMString(theFormatterOut.str()));

	// Unfreeze the ostrstream, so memory is returned...
     #if defined(HPUX)
        theFormatterOut.rdbuf() -> freeze(false);
     #else
	theFormatterOut.freeze(false);
     #endif

	outputMessage(theThreadID, "Transforming");

 	// Do the transform.
	theResult = theXalanTransformer.transform(*glbParsedSource, glbCompiledStylesheet, theResultTarget);

	if(theResult != 0)
	{
		cerr << "ThreadSafe Error: \n" << theXalanTransformer.getLastError()
			 << endl
			 << endl;

		glbError = theResult;
	}

	outputMessage(theThreadID, "Finishing");
  
  #if defined(_MSC_VER)
	return (theResult);
  #elif defined(XALAN_POSIX2_AVAILABLE)
        return 0;
  #endif
}



// Create and run the threads...
// Print messages tracking the progress of each thread and of the 
// overall operation...
void
doThreads(int	nThreads)
{
      int   i=0;
      cout << endl << "Clock before starting threads: " << clock() << endl;
      

	XALAN_USING_STD(vector)

        vector<theThreadType>   hThreads;

	hThreads.reserve(nThreads);

#if defined(_MSC_VER)

	for (; i < nThreads; ++i)
	{
                theThreadIDType  threadID;

                const theThreadType  hThread = CreateThread(
				0, 
				4096,							// Stack size for thread.
				theThread,						// pointer to thread function
				reinterpret_cast<LPVOID>(i),	// argument for new thread
				0,								// creation flags
				&threadID);

		assert(hThread != 0);

		hThreads.push_back(hThread);
	}

	WaitForMultipleObjects(hThreads.size(), &hThreads[0], TRUE, INFINITE);

	for (i = 0; i < nThreads; ++i)
	{
		CloseHandle(hThreads[i]);
	}

#elif defined(XALAN_POSIX2_AVAILABLE)

  int result;
  void *thread_result;
  
  for(; i < nThreads; ++i)
  { 
   result = pthread_create(
                          &hThreads[i],    //thread pointer
                          NULL,            //thread's attribute default
                          theThread,       //thread function
                          (void *) &i      //thread function argument
                         );
  if (result != 0)
    {
      perror ("Thread creation failed");
      exit(EXIT_FAILURE);
    }
  }

  cout << endl << "Waiting for threads to finish..." << endl << endl;
  for( i = nThreads - 1; i>=0; i-- )
  {
    result = pthread_join(
                           hThreads[i],
                           &thread_result
                         );
    if (result != 0)
     {
       perror ("Thread join failed");
       exit (EXIT_FAILURE);
     }
   //for UNIX debugging
   // cout << "Thread joined, return value: " << (char *)thread_result << endl;
  } 
 
#endif

cout << endl << endl << "Clock after threads: " << clock() << endl;

}



int
main(
			int		argc,
			char*	/* argv */[])
{
	if (argc != 1)
	{
		cerr << "Usage: ThreadTest"
			 << endl
			 << endl;
	}
	else
	{
		try
		{
			XALAN_USING_XERCES(XMLPlatformUtils)

			// Call the static initializer for Xerces.
			XMLPlatformUtils::Initialize();

			// Initialize Xalan.
			XalanTransformer::initialize();

			{
				// Create a XalanTransformer.  We won't actually use this to transform --
				// it's just acting likely a factory for the compiled stylesheet and
				// pre-parsed source.
				XalanTransformer	theXalanTransformer;

				glbError = theXalanTransformer.compileStylesheet("birds.xsl", glbCompiledStylesheet);

				if (glbError != 0)
				{
					cerr << "ThreadSafe Error: \n" << theXalanTransformer.getLastError()
						 << endl
						 << endl;
				}
				else
				{
					assert(glbCompiledStylesheet != 0);

					// Compile the XML source document as well. All threads will use
					// this binary representation of the source tree.
					glbError = theXalanTransformer.parseSource("birds.xml", glbParsedSource);

					if (glbError != 0)
					{
						cerr << "ThreadSafe Error: \n" << theXalanTransformer.getLastError()
							 << endl
							 << endl;
					}
					else
					{
						assert(glbParsedSource != 0);

						// Create and run the threads...
						// Each thread uses the same document and 
						// stylesheet to perform a transformation.
                                    doThreads(NUM_THREADS);

					}
				}
			}

			// Terminate Xalan...
			XalanTransformer::terminate();

			// Terminate Xerces...
			XMLPlatformUtils::Terminate();

			// Clean up the ICU, if it's integrated...
			XalanTransformer::ICUCleanUp();
		}
		catch(...)
		{
			cerr << "Initialization failed!" << endl;

			glbError = -1;
		}
	}

	return glbError;
}
