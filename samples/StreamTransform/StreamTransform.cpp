/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2004 The Apache Software Foundation.  All rights 
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
#include <iostream.h>
#include <strstream.h>
#include <fstream.h>
#else
#include <iostream>
#include <strstream>
#include <fstream>
#endif



#include <xercesc/util/PlatformUtils.hpp>



#include <xalanc/XalanTransformer/XalanTransformer.hpp>



int
main(
			int		argc,
			char*	/* argv */[])
{
	XALAN_USING_STD(cerr)
	XALAN_USING_STD(cout)
	XALAN_USING_STD(endl)
	XALAN_USING_STD(istrstream)
	XALAN_USING_STD(ofstream)
	XALAN_USING_STD(ostrstream)

#if defined(XALAN_STRICT_ANSI_HEADERS)
	using std::strlen;
#endif

	int	theResult = -1;

	if (argc != 1)
	{
		cerr << "Usage: StreamTransform"
			 << endl
			 << endl;
	}
	else
	{
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

				// A simple input document...
#if defined(XALAN_NON_ASCII_PLATFORM)
				const char* const  theInputDocument = "<?xml version='1.0' encoding='EBCDIC-CP-US' ?><doc>Hello world!</doc>";
#else
				const char* const  theInputDocument = "<?xml version='1.0' encoding='ISO-8859-1' ?><doc>Hello world!</doc>";
#endif

				// A "hello world" stylesheet.  Note that the encoding for the output is platform-dependent,
				// since we're writing to a string.  It could be any encoding, but "binary" encodings,
				// or encodings that could produce multi-byte characters would require transcoding on
				// some platforms.
				const char* const  theStylesheet =
#if defined(XALAN_NON_ASCII_PLATFORM)
"<?xml version='1.0' encoding='EBCDIC-CP-US'?>\
<xsl:stylesheet xmlns:xsl='http://www.w3.org/1999/XSL/Transform' version='1.0'>\
<xsl:output encoding='EBCDIC-CP-US'/>\
<xsl:template match='doc'>\
<out><xsl:value-of select='.'/></out>\
</xsl:template>\
</xsl:stylesheet>";
#else
"<?xml version='1.0'?>\
<xsl:stylesheet xmlns:xsl='http://www.w3.org/1999/XSL/Transform' version='1.0'>\
<xsl:output encoding='US-ASCII'/>\
<xsl:template match='doc'>\
<out><xsl:value-of select='.'/></out>\
</xsl:template>\
</xsl:stylesheet>";
#endif

				// Our input streams...
				istrstream	theXMLStream(theInputDocument, strlen(theInputDocument));
				istrstream	theXSLStream(theStylesheet, strlen(theStylesheet));

				XALAN_USING_XALAN(XalanDOMString)
				XALAN_USING_XALAN(XSLTInputSource)

				XSLTInputSource		inputSource(&theXSLStream);

				inputSource.setSystemId(XalanDOMString("foo").c_str());

				// Do the transform.
				theResult = theXalanTransformer.transform(&theXMLStream, inputSource, cout);

				if(theResult != 0)
				{
					cerr << "StreamTransform Error: \n" << theXalanTransformer.getLastError()
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
			cerr << "Initialization failed!" << endl;
		}
	}

	return theResult;
}
