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
			int				argc,
			const char*		/* argv */[])
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

		XALAN_USING_XALAN(XalanTransformer)
		XALAN_USING_XALAN(XalanCompiledStylesheet)

		// Call the static initializer for Xerces.
		XMLPlatformUtils::Initialize();

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

		// Terminate Xalan...
		XalanTransformer::terminate();

		// Terminate Xerces...
		XMLPlatformUtils::Terminate();

		// Clean up the ICU, if it's integrated.
		XalanTransformer::ICUCleanUp();
	}

	return theResult;
}
