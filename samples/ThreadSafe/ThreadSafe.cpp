/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2003 The Apache Software Foundation.  All rights 
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
#include <fstream>
#include <iostream>
#include <strstream>



#include <xercesc/util/PlatformUtils.hpp>



#include <xalanc/XalanTransformer/XalanTransformer.hpp>



//This is here for the Windows threads.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winbase.h>
#define THREADFUNCTIONRETURN DWORD WINAPI



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
			DWORD		id,
			const char	msg[])
{
	ostrstream threadMsg;
	
	threadMsg << "\n" << msg << " Thread: " << id << '\0';

	cout << threadMsg.str();

	threadMsg.freeze(false);
}



THREADFUNCTIONRETURN
theThread(LPVOID	param)
{
// This routine uses a compiled stylesheet (glbCompiledStylesheet), 
// and a binary source tree (glbParsedSource) to perform the 
// transformation.

	int	theResult = 0;

	const int	number = reinterpret_cast<int>(param);

	const DWORD		theThreadID = GetCurrentThreadId();

	outputMessage(theThreadID, "Starting ");

	// Create a XalanTransformer.
	XalanTransformer	theXalanTransformer;

	// Generate the output file name for this thread.
    ostrstream	theFormatterOut;
    theFormatterOut << "birds" << number << ".out" << '\0';

	// Generate the XML output object.
	const XSLTResultTarget	theResultTarget(XalanDOMString(theFormatterOut.str()));

	// Unfreeze the ostrstream, so memory is returned...
	theFormatterOut.freeze(false);

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

	return (theResult);
}



// Create and run the threads...
// Print messages tracking the progress of each thread and of the 
// overall operation...
void
doThreads(int	nThreads)
{
	XALAN_USING_STD(vector)

	vector<HANDLE>	hThreads;

	hThreads.reserve(nThreads);

	cout << endl << "Clock before starting threads: " << clock() << endl;

	int		i = 0;	

	for (; i < nThreads; ++i)
	{
		DWORD  threadID;

		const HANDLE	hThread = CreateThread(
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

	cout << endl << "Clock after threads: " << clock() << endl;

	for (i = 0; i < nThreads; ++i)
	{
		CloseHandle(hThreads[i]);
	}
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
						doThreads(10);
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
