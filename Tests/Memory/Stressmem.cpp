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
#include <iostream>



#if !defined(NDEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif



#include <vector>



#include <util/PlatformUtils.hpp>



#include <XSLT/XSLTInputSource.hpp>
#include <XSLT/XSLTResultTarget.hpp>



#include <XalanTransformer/XalanTransformer.hpp>



#include <XMLFileReporter.hpp>
#include <FileUtility.hpp>



#if !defined(XALAN_NO_NAMESPACES)
	using std::cerr;
	using std::cout;
	using std::endl;
#endif



// This is here for memory leak testing.
#if defined(_DEBUG)
#include <crtdbg.h>
#endif


	
static const char* const	excludeStylesheets[] =
{
//	"impincl16.xml",
	0
};



inline bool
checkForExclusion(XalanDOMString currentFile)
{
	for (int i = 0; excludeStylesheets[i] != 0; ++i)
	{	
		if (equals(currentFile, XalanDOMString(excludeStylesheets[i])))
		{
			return true;
		}
	}

	return false;
}



bool
getParams(int			argc, 
		  const char*	/* argv */[])
{
	// This needs additional work.
	if (argc != 1)
	{
		cerr << "Usage: ThreadTest" << endl;

		return false;
	}
	else
	{
		return true;
	}
}



#if defined(XALAN_NO_NAMESPACES)
typedef vector<XalanDOMString>		FileNameVectorType;
#else
typedef std::vector<XalanDOMString>	FileNameVectorType;
#endif



int
main(
	 int			argc,
	 const char*	argv[])
{

#if !defined(NDEBUG) && defined(_MSC_VER)
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif

	if (getParams(argc, argv) == true)
	{
		// Defined root for performance directory. Based on PD's machine. 
		const XalanDOMString	confDir(XALAN_STATIC_UCODE_STRING("\\xsl-test\\conf\\"));
		const XalanDOMString	outDir(XALAN_STATIC_UCODE_STRING("\\xsl-test\\cplus-mem\\"));

		FileUtility			f;

		// Get the list of Directories that are below perf
		const FileNameVectorType	dirs = f.getDirectoryNames(confDir);

		//XMLFileReporter	logFile("cpp.xml");
		//logFile.logTestFileInit("Memory Testing - Memory leaks detected during ConformanceTests. ");

		try
		{
			// Call the static initializers...
			XMLPlatformUtils::Initialize();

			XalanTransformer::initialize();

			{
				XalanTransformer		transformEngine;

				const XalanDOMString	theXSLSuffix(XALAN_STATIC_UCODE_STRING(".xsl"));
				const XalanDOMString	theXMLSuffix(XALAN_STATIC_UCODE_STRING(".xml"));
				const XalanDOMString	pathSep(XALAN_STATIC_UCODE_STRING("\\"));  

				for(FileNameVectorType::size_type	j = 0; j < dirs.size(); ++j)
				{
					const FileNameVectorType	files = f.getTestFileNames(confDir, dirs[j]);

					for(FileNameVectorType::size_type i = 0; i < files.size(); ++i)
					{
						if (checkForExclusion(files[i]) == false)
						{
							// Output file name to result log.
							//logFile.logTestCaseInit(files[i]);
							cout << files[i] << endl;

							const XalanDOMString	theXMLFile= confDir + dirs[j] + pathSep + files[i];
							const XalanDOMString	outFile = outDir + dirs[j] + pathSep + files[i];
							const XalanDOMString	theXSLFile = f.GenerateFileName(theXMLFile,"xsl");
							const XalanDOMString	theOutputFile = f.GenerateFileName(outFile, "out");

							// Do a total end to end transform with no pre parsing of either xsl or xml files.
							XSLTResultTarget		theResultTarget(theOutputFile);

							const XSLTInputSource	xslInputSource(c_wstr(theXSLFile));
							const XSLTInputSource	xmlInputSource(c_wstr(theXMLFile));

							int		theResult =
								transformEngine.transform(xmlInputSource, xslInputSource, theResultTarget);

							if(theResult != 0)
							{
								cerr << "XalanError: \n" << transformEngine.getLastError();
							}
						}
					}
				}
			}

			XalanTransformer::terminate();

			XMLPlatformUtils::Terminate();
		}
		catch(...)
		{
			cerr << "Exception caught!!!" << endl << endl;
		}
	}

	return 0;
}
