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
#include <iostream.h>
#else
#include <iostream>
#endif



#include <xercesc/util/PlatformUtils.hpp>



#include <xalanc/XalanTransformer/XalanTransformer.hpp>



int
main(
		  int		argc,
		  char*		argv[])
{
	XALAN_USING_STD(cerr)
	XALAN_USING_STD(endl)

	int	theResult = 0;

	if (argc != 3)
	{
		cerr << "Usage: UseStylesheetParam key expression" << endl;

		theResult = -1;
	}
	else
	{
		XALAN_USING_XERCES(XMLPlatformUtils)
		XALAN_USING_XERCES(XMLException)


		XALAN_USING_XALAN(XalanTransformer)

		// Call the static initializer for Xerces.
		try
		{
			 XMLPlatformUtils::Initialize();
		}
		catch (const XMLException& toCatch)
		{
			 cerr << "Error during Xerces initialization.  Error code was "
                  << toCatch.getCode()
                  << "."
                  << endl;

             theResult = -1;
		}

		if ( theResult == 0)
		{

			// Initialize Xalan.
			XalanTransformer::initialize();

			{
				XALAN_USING_XALAN(XalanDOMString)

				// Create a XalanTransformer.
				XalanTransformer	theXalanTransformer;

				// Set the stylesheet parameter name and
				// expression (a string expression).
				theXalanTransformer.setStylesheetParam(
						XalanDOMString(argv[1]),
						XalanDOMString(argv[2]));

				// Our input files...The assumption is that the executable will be run
				// from same directory as the input files.
				theResult = theXalanTransformer.transform("foo.xml", "foo.xsl", "foo.out");

				if(theResult != 0)
				{
					cerr << "UseStylesheetParam Error: \n" << theXalanTransformer.getLastError()
						 << endl
						 << endl;
				}
			}

			// Terminate Xalan...
			XalanTransformer::terminate();
		}

		// Terminate Xerces...
		XMLPlatformUtils::Terminate();

		// Clean up the ICU, if it's integrated...
		XalanTransformer::ICUCleanUp();
	}

	return theResult;
}
