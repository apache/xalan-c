/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
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


#define TEST_XALAN_CPP


// This is here for memory leak testing.
#if defined(_DEBUG)
#include <crtdbg.h>
#endif



#include <util/PlatformUtils.hpp>


#if defined(TEST_XALAN_CPP)

#include <XalanTransformer/XalanTransformer.hpp>

#if defined(XALAN_OLD_STREAM_HEADERS)
#include <iostream.h>
#include <strstream.h>
#else
#include <iostream>
#include <strstream>
#endif


#if !defined(XALAN_NO_NAMESPACES)
using std::ostrstream;
using std::cout;
#endif

#else

#include <XalanTransformer/XalanCAPI.h>

#include <stdio.h>

#endif


static unsigned long xalan_output_handler(const void *data, unsigned long length, const void *handle)
{
	FILE *fp = (FILE*)handle;

	char* d = (char *)data;

	
	fwrite( d, sizeof( char ), length, stdout );

	return fwrite( d, sizeof( char ), length, fp );
}



static void xalan_flush_handler(const void *handle)
{
	FILE *fp = (FILE*)handle;

	fflush(fp);
}



int
main(
			int				/* argc */,
			const char*		/* argv[] */)
{
#if !defined(NDEBUG) && defined(_MSC_VER)
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif

	const char* const		theXMLFileName = "d:\\xslt\\xsl-test\\perf\\basic\\basic-all_well.xml";
	const char* const 		theXSLFileName = "d:\\xslt\\xsl-test\\perf\\basic\\basic-all_well.xsl";
	const char* const		theOutFileName = "d:\\Transformer-Results\\basic-all_well.out";

	const char* const 		theXMLFileName2 = "d:\\xslt\\xsl-test\\perf\\basic\\miscfam.xml";
	const char* const		theXSLFileName2 = "d:\\xslt\\xsl-test\\perf\\basic\\miscfam.xsl";

	const char* const 		theXMLFileName3 = "d:\\xslt\\xsl-test\\conf\\embed\\embed01.xml";	
	const char* const		theOutFileName3 = "d:\\Transformer-Results\\embed01.out";

	const char* const 		theXMLFileName4 = "d:\\xml-xalan\\c\\samples\\UseStylesheetParam\\foo.xml";	
	const char* const 		theXSLFileName4 = "d:\\xml-xalan\\c\\samples\\UseStylesheetParam\\foo.xsl";	

#if defined(TEST_XALAN_CPP)

	// Call the static initializer for Xerces.
	XMLPlatformUtils::Initialize();

    // Initialize Xalan.
    XalanTransformer::initialize();

	XalanTransformer xalan;

	const XalanCompiledStylesheet*	css = 0;
	
	if (xalan.compileStylesheet(theXSLFileName, css) != 0)
	{
		cout << xalan.getLastError();

		return 0;	
	}

	for(int i=0; i<1; ++i)
	{

		if(xalan.transform(theXMLFileName, css, "d:\\transformer-results\\css.out"))
		{
			cout << xalan.getLastError();

			return 0;	
		}
		
		if(xalan.transform(theXMLFileName3, theOutFileName3))
		{
			cout << xalan.getLastError();

			return 0;	
		}

		if(xalan.transform(theXMLFileName, theXSLFileName, theOutFileName))
		{
			cout << xalan.getLastError();

			return 0;	
		}

		ostrstream	theOutput;

		if(xalan.transform(theXMLFileName2, theXSLFileName2, &theOutput))
		{
			cout << xalan.getLastError();

			return 0;	
		}

		theOutput << '\0';

		cout << theOutput.str();

		theOutput.freeze(false);

		ostrstream	theOutput3;

		if(xalan.transform(theXMLFileName3, theOutput3))
		{
			cout << xalan.getLastError();

			return 0;	
		}
/*		
		if(xalan.transform(theXMLFileName, css, cout))
		{
			cout << xalan.getLastError();

			return 0;	
		}
*/		
		xalan.setStylesheetParam(XalanDOMString("param1"),
								 XalanDOMString("'What is Up'"));

		if(xalan.transform(theXMLFileName4, theXSLFileName4, &cout))
		{
			cout << xalan.getLastError();

			return 0;	
		}

		if(xalan.transform(theXMLFileName4, theXSLFileName4, &cout))
		{
			cout << xalan.getLastError();

			return 0;	
		}
	}

   // Terminate Xalan.
	XalanTransformer::terminate();

	// Call the static terminator for Xerces.
	XMLPlatformUtils::Terminate();

#else
	XalanInitialize();

	XalanHandle xalan = CreateXalanTransformer();
	XalanCSSHandle theXalanCSS2;
	XalanCSSHandle theXalanCSS4;

	if (XalanCompileStylesheet(theXSLFileName2, xalan, &theXalanCSS2) != 0)
	{
		puts("Error");
		puts(XalanGetLastError(xalan));

		return 0;	
	}

	XalanCompileStylesheet(theXSLFileName4, xalan, &theXalanCSS4) != 0)
	{
		puts("Error");
		puts(XalanGetLastError(xalan));

		return 0;	
	}

	for(int i=0; i<2; ++i)
	{
		if(XalanTransformToFile(theXMLFileName, theXSLFileName, theOutFileName, xalan))
		{
			puts("Error");
			puts(XalanGetLastError(xalan));
			return 0;	
		}

		char* 		theOutput;

		if(XalanTransformToData(theXMLFileName2, theXSLFileName2, &theOutput, xalan))
		{
			puts("Error");
			puts(XalanGetLastError(xalan));
			return 0;	
		}

		puts(theOutput);

		XalanFreeData(theOutput);

		if(XalanTransformToFile(theXMLFileName3, NULL, theOutFileName3, xalan))
		{
			puts("Error");
			puts(XalanGetLastError(xalan));
			return 0;	
		}
		
		if(XalanTransformToData(theXMLFileName3, NULL, &theOutput, xalan))
		{
			puts("Error");
			puts(XalanGetLastError(xalan));
			return 0;	
		}

		puts(theOutput);

		XalanFreeData(theOutput);

		if(XalanTransformToDataCSS(theXMLFileName2, theXalanCSS2, &theOutput, xalan))
		{
			puts("Error");
			puts(XalanGetLastError(xalan));
			return 0;	
		}

		puts(theOutput);

		XalanFreeData(theOutput);

		FILE* fp =0;
		fp = fopen("c:\\temp\\test.out", "w");

		if(XalanTransformToHandlerCSS(theXMLFileName2, theXalanCSS2, xalan, fp, xalan_output_handler, xalan_flush_handler))
		{
			puts("Error");
			puts(XalanGetLastError(xalan));
			return 0;	
		}

		fclose(fp);

		XalanSetStylesheetParam("param1", "'hi'", xalan);

		//if(xalan.transform(theXMLFileName4, theXSLFileName4, &cout))
		if(XalanTransformToDataCSS(theXMLFileName4, theXalanCSS4, &theOutput, xalan))
		{
			puts("Error");
			puts(XalanGetLastError(xalan));
			return 0;
		}

		puts(theOutput);

		XalanFreeData(theOutput);

	}

	DeleteXalanTransformer(xalan);

	XalanTerminate();
#endif

	return 0;
}