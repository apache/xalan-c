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

#include <xalanc/Include/PlatformDefinitions.hpp>



#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <fstream.h>
#include <iostream.h>
#else
#include <fstream>
#include <iostream>
#endif



#include <xercesc/util/PlatformUtils.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanOutputStreamPrintWriter.hpp>
#include <xalanc/PlatformSupport/XalanStdOutputStream.hpp>



#include <xalanc/XSLT/TraceListenerDefault.hpp>



#include <xalanc/XalanTransformer/XalanTransformer.hpp>



int
main(
			int		argc,
			char*	argv[])
{
	XALAN_USING_STD(cerr)
	XALAN_USING_STD(endl)

#if defined(XALAN_STRICT_ANSI_HEADERS)
	using std::strcmp;
#endif

	// TraceListener flags...
	bool	traceTemplates = false;
	bool	traceTemplateChildren = false;
	bool	traceGenerationEvent = false;
	bool	traceSelectionEvent = false;

	if (argc < 2 || argc > 5)
	{
		cerr << "Usage: TraceListen [+ 1 or more of following] -tt -tg -ts -ttc" << endl;

		return -1;
	}

	// Set the TraceListener flags...
	for (int i = 1;	i < argc;	i ++)
	{
		if(!strcmp("-tt", argv[i]))
		{
			traceTemplates = true;
		}
		else if(!strcmp("-tg", argv[i]))
		{
			traceGenerationEvent = true;
		}
		else if(!strcmp("-ts", argv[i]))
		{
			traceSelectionEvent = true;
		}
		else if(!strcmp("-ttc", argv[i]))
		{
			traceTemplateChildren = true;
		}
		else
		{
  			cerr << "Usage: TraceListen [+ 1 or more of following] -tt -tg -ts -ttc" << endl;

			return -1;
		}
	} 
 
	try
	{
		XALAN_USING_XERCES(XMLPlatformUtils)

		XALAN_USING_XALAN(XalanTransformer)

		// Initialize Xerces...
		XMLPlatformUtils::Initialize();

		// Initialize Xalan...
		XalanTransformer::initialize();

		{
			XALAN_USING_XALAN(TraceListenerDefault)
			XALAN_USING_XALAN(XalanDOMString)
			XALAN_USING_XALAN(XalanOutputStreamPrintWriter)
			XALAN_USING_XALAN(XalanStdOutputStream)
			XALAN_USING_XALAN(XSLTInputSource)
			XALAN_USING_XALAN(XSLTResultTarget)

			// Create a XalanTransformer.
			XalanTransformer	theXalanTransformer;

			// Set up a diagnostic writer to be used by the TraceListener...
			XalanStdOutputStream			theStdErr(cerr);
			XalanOutputStreamPrintWriter	diagnosticsWriter(theStdErr);

			// Make sure that error reporting, which includes any TraceListener output
			// does not throw exceptions when transcoding, since that could result in
			// an exception being thrown while another exception is active.  In particular,
			// characters that the TraceListener writes might not be representable in the
			// local code page.
			theStdErr.setThrowTranscodeException(false);

			// Set up the TraceListener... 
			TraceListenerDefault		theTraceListener(				
					diagnosticsWriter,
					traceTemplates,
					traceTemplateChildren,
					traceGenerationEvent,
					traceSelectionEvent);

			// Add the TraceListener to the XalanTransformer instance...
			theXalanTransformer.addTraceListener(&theTraceListener);

			// Our input files assume the executable will be run
			// from same directory as the input files.
			const int	theResult = theXalanTransformer.transform("birds.xml", "birds.xsl", "birds.out");

			if(theResult != 0)
			{
				cerr << "TraceListen Error: \n" << theXalanTransformer.getLastError()
					 << endl
					 << endl;
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
		cerr << "Exception caught!  Exiting..." << endl;
	}

	return 0;
}
