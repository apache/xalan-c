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
			char*	argv[])
{
	XALAN_USING_STD(cerr)
	XALAN_USING_STD(cout)
	XALAN_USING_STD(endl)

    if (argc < 3 || argc > 4)
	{
		cerr << "Usage: XalanTransform XMLFileName XSLFileName [OutFileName]" << endl;
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

		{
			// Create a XalanTransformer.
			XalanTransformer theXalanTransformer;

			if (argc == 4)
			{
				// Do the transform.
				theResult = theXalanTransformer.transform(argv[1], argv[2], argv[3]);
        
				if(theResult != 0)
				{
					cerr << "Error: " << theXalanTransformer.getLastError() << endl;
				}
			}
			else
			{
				// Do the transform.
				theResult = theXalanTransformer.transform(argv[1], argv[2], cout);
        
				if(theResult != 0)
				{
					cerr << "Error: " << theXalanTransformer.getLastError() << endl;
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
		cerr << "An unknown error occurred!" << endl;
	}

	return theResult;
}
