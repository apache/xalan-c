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

#include <xercesc/util/PlatformUtils.hpp>



#include <xalanc/XalanTransformer/XalanTransformer.hpp>



#include <cstdio>

#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#else
#include <iostream>
#endif



// This is a simple class that illustrates how XalanTransformer's "callback" API
// is used.  This example just abstracts writing data to a FILE*, but other
// actions are possible.
class CallbackHandler
{
public:

	CallbackHandler(FILE*	theFile) :
		m_file(theFile)
	{
		assert(m_file != 0);
	}

	CallbackSizeType
	write(
			const char*			theData,
			CallbackSizeType	theLength)
	{
		return fwrite(theData, sizeof(char), theLength, m_file);
	}

	void
	flush()
	{
		fflush(m_file);
	}

private:

	FILE* const		m_file;
};



// These functions need to have C linkage, so surround them with an extern C block...
extern "C"
{

// This is the write callback function, which casts the handle
// to the appropriate type, then calls the write() member function
// on the CallbackHandler class.
CallbackSizeType
writeCallback(
			const char*			theData,
			CallbackSizeType	theLength,
			void*				theHandle)
{
#if defined(XALAN_OLD_STYLE_CASTS)
	return ((CallbackHandler*)theHandle)->write(theData, theLength);
#else
	return reinterpret_cast<CallbackHandler*>(theHandle)->write(theData, theLength);
#endif
}

// This is the flush callback function, which casts the handle
// to the appropriate type, then calls the flush() member function
// on the CallbackHandler class.
void
flushCallback(void*	theHandle)
{
#if defined(XALAN_OLD_STYLE_CASTS)
	((CallbackHandler*)theHandle)->flush();
#else
	reinterpret_cast<CallbackHandler*>(theHandle)->flush();
#endif
}

}



int
doTransform(
			const char*		theXMLFile,
			const char*		theXSLFile,
			FILE*			theOutputFile)
{
	XALAN_USING_STD(cerr)
	XALAN_USING_STD(endl)

	XALAN_USING_XALAN(XalanTransformer)

    // Create a XalanTransformer...
	XalanTransformer	theXalanTransformer;

	// Create an instance of the class we wrote to handle
	// the callbacks...
	CallbackHandler		theHandler(theOutputFile);

	// Do the transform...
	const int	theResult = theXalanTransformer.transform(
					theXMLFile,
					theXSLFile,
					&theHandler,
					writeCallback,
					flushCallback);
        
	if(theResult != 0)
	{
		cerr << "XalanError: " << theXalanTransformer.getLastError() << endl;
	}

	return theResult;
}



int
main(
			int		argc,
			char*	argv[])
{
	XALAN_USING_STD(cerr)
	XALAN_USING_STD(cout)
	XALAN_USING_STD(endl)

    if (argc < 3 || argc > 4)
	{
		cerr << "Usage: XalanTransformerCallback XMLFileName XSLFileName [OutFileName]" << endl;

		return -1;
	}

	int		theResult = -1;

	try
	{
		XALAN_USING_XERCES(XMLPlatformUtils)

		XALAN_USING_XALAN(XalanTransformer)

		// Call the static initializer for Xerces.
		XMLPlatformUtils::Initialize();

		// Initialize Xalan.
		XalanTransformer::initialize();

		if (argc == 3)
		{
			// No output file, so use stdout...
			theResult = doTransform(argv[1], argv[2], stdout);
		}
		else
		{
			// Output file specified, so try to open it...
			FILE* const	theOutputFile = fopen(argv[3], "w");

			if (theOutputFile == 0)
			{
				cerr << "Error: " << "Unable to open output file " << argv[3] << endl;
			}
			else
			{
				theResult = doTransform(argv[1], argv[2], theOutputFile);

				fclose(theOutputFile);
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
		cerr << "An unknown error occurred!" << endl;
	}

	return theResult;
}
