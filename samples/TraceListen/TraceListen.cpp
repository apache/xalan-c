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
			int				argc,
			const char*		argv[])
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

			const XalanDOMString	theXMLFileName("birds.xml");
			const XalanDOMString	theXSLFileName("birds.xsl");

			// Our input sources...
			const XSLTInputSource	theInputSource(theXMLFileName);
			const XSLTInputSource	theStylesheetSource(theXSLFileName);

			// Our output target...
			const XalanDOMString	theOutputFile("birds.out");
			XSLTResultTarget		theResultTarget(theOutputFile);

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
