/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2002 The Apache Software Foundation.  All rights 
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

#include <Include/PlatformDefinitions.hpp>



#include <cassert>
#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#else
#include <iostream>
#endif



#include <xercesc/framework/URLInputSource.hpp>
#if XERCES_VERSION_MAJOR >= 2
#include <xercesc/parsers/XercesDOMParser.hpp>
#else
#include <xercesc/parsers/DOMParser.hpp>
#endif
#include <xercesc/util/PlatformUtils.hpp>



#include <PlatformSupport/URISupport.hpp>



#include <XercesParserLiaison/XercesParserLiaison.hpp>
#include <XercesParserLiaison/XercesDOMSupport.hpp>



#include <XalanSourceTree/XalanSourceTreeDOMSupport.hpp>
#include <XalanSourceTree/XalanSourceTreeParserLiaison.hpp>



#include <XalanTransformer/XalanTransformer.hpp>
#include <XalanTransformer/XercesDOMWrapperParsedSource.hpp>
#include <XalanTransformer/XalanSourceTreeWrapperParsedSource.hpp>



int
transformXercesDOM(
			XalanTransformer&				theTransformer,
			const XalanDOMString&			theURI,
			const XalanCompiledStylesheet*	theStylesheet,
			const XSLTResultTarget&			theResultTarget)
{
	const URLInputSource	theInputSource(theURI.c_str());

	XercesParserLiaison::DOMParserType	theParser;

	theParser.parse(theInputSource);

	XercesParserLiaison		theParserLiaison;
	XercesDOMSupport		theDOMSupport;

	const XercesDOMWrapperParsedSource	theWrapper(
				theParser.getDocument(),
				theParserLiaison,
				theDOMSupport,
				theURI);

	return theTransformer.transform(
						theWrapper,
						theStylesheet,
						theResultTarget);
}



int
transformXalanSourceTree(
			XalanTransformer&				theTransformer,
			const XalanDOMString&			theURI,
			const XalanCompiledStylesheet*	theStylesheet,
			const XSLTResultTarget&			theResultTarget)
{
	const URLInputSource			theInputSource(theURI.c_str());

	XalanSourceTreeParserLiaison	theParserLiaison;
	XalanSourceTreeDOMSupport		theDOMSupport(theParserLiaison);

	XalanDocument* const	theDocument = theParserLiaison.parseXMLStream(theInputSource, theURI);

	XalanSourceTreeDocument* const	theSourceTreeDocument = theParserLiaison.mapDocument(theDocument);
	assert(theSourceTreeDocument != 0);

	XalanSourceTreeWrapperParsedSource	theWrapper(theSourceTreeDocument, theParserLiaison, theDOMSupport, theURI);

	return theTransformer.transform(
						theWrapper,
						theStylesheet,
						theResultTarget);
}



int
transform()
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::cout;
	using std::cerr;
    using std::endl;
#endif

	int		theResult = -1;

	try
	{
		// Create a XalanTransformer.
		XalanTransformer	theTransformer;

		const XSLTInputSource	theStylesheetInputSource("foo.xsl");

		// Let's compile the stylesheet and re-use it...
		const XalanCompiledStylesheet*		theStylesheet = 0;

		if (theTransformer.compileStylesheet(theStylesheetInputSource, theStylesheet) != 0)
		{
			cerr << "An error occurred compiling the stylesheet: "
				 << theTransformer.getLastError()
				 << endl;
		}
		else
		{
			assert(theStylesheet != 0);

			const XalanDOMString	theInputFile("foo.xml");

			const XalanDOMString	theURI(URISupport::getURLStringFromString(theInputFile));

			const XSLTResultTarget	theResultTarget(cout);

			theResult = transformXercesDOM(theTransformer, theURI, theStylesheet, theResultTarget);

			if (theResult == 0)
			{
				cout << endl;

				theResult = transformXalanSourceTree(theTransformer, theURI, theStylesheet, theResultTarget);
			}

			if (theResult != 0)
			{
				cerr << "Transformation failed: " << theTransformer.getLastError() << endl;
			}
		}
	}
	catch(...)
	{
		cerr << "An unknown error occurred!" << endl;
	}

	return theResult;
}

// This sample shows how existing Xerces DOM_Document instances and XalanSourceTreeDocument
// instances can be wrapped to use as input for to an instance of XalanTransformer.
int
main(
			int				 argc,
			const char*		 /* argv */[])
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::cout;
	using std::cerr;
    using std::endl;
#endif

	int		theResult = -1;

    if (argc != 1)
	{
		cerr << "Usage: ParsedSourceWrappers" << endl;
	}
	else
	{
		try
		{
			// Call the static initializer for Xerces.
			XMLPlatformUtils::Initialize();

			// Initialize Xalan.
			XalanTransformer::initialize();

			theResult = transform();

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
