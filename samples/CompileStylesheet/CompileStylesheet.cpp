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



#include <cassert>



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



int
main(
			int		argc,
			char*	/* argv */[])
{
	XALAN_USING_STD(cerr)
	XALAN_USING_STD(endl)
	XALAN_USING_STD(ostrstream)

	int	theResult = 0;

	if (argc != 1)
	{
		cerr << "Usage: CompileStylesheet"
			 << endl
			 << endl;
	}
	else
	{
		XALAN_USING_XERCES(XMLPlatformUtils)
		XALAN_USING_XERCES(XMLException)

		XALAN_USING_XALAN(XalanTransformer)
		XALAN_USING_XALAN(XalanCompiledStylesheet)

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
				// Create a XalanTransformer.
				XalanTransformer theXalanTransformer;
				
				// Our input files...The assumption is that the executable will be run
				// from same directory as the input files.
				const char*		theXSLFileName = "foo.xsl";
				
				// Compile the stylesheet.
				const XalanCompiledStylesheet*	theCompiledStylesheet = 0;
				
				theResult =	theXalanTransformer.compileStylesheet(theXSLFileName, theCompiledStylesheet);
				
				if (theResult == 0)
				{
					assert(theCompiledStylesheet != 0);
					
					for (unsigned int i = 0; i < 10; ++i)
					{		
						// Buffers passed in to ostrstream.
						char	inBuffer[10];
						char	outBuffer[10];	
						
						// Generate the input and output file names.
						ostrstream	theFormatterIn(inBuffer, sizeof(inBuffer));
						ostrstream	theFormatterOut(outBuffer, sizeof(outBuffer));
						
						theFormatterIn << "foo" << i + 1 << ".xml" << '\0';
						theFormatterOut << "foo" << i + 1 << ".out" << '\0';
						
						// Do the transform.
						theResult = theXalanTransformer.transform(
							inBuffer,
							theCompiledStylesheet,
							outBuffer);
					}
					
					// I don't have to do this, since the transformer will
					// clean this up when it's destroyed, but why not...
					theResult = theXalanTransformer.destroyStylesheet(theCompiledStylesheet);
				}
				
				if(theResult != 0)
				{
					cerr << "CompileStylesheet Error: " << theXalanTransformer.getLastError()
						<< endl
						<< endl;
				}
				
			}
		}

		// Terminate Xalan...
		XalanTransformer::terminate();

		// Terminate Xerces...
		XMLPlatformUtils::Terminate();

		// Clean up the ICU, if it's integrated.
		XalanTransformer::ICUCleanUp();
	}

	return theResult;
}
