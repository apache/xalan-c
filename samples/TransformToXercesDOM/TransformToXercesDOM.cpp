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
#include <iostream.h>
#else
#include <iostream>
#endif



#if !defined(NDEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif



#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>



#include <xalanc/Include/XalanAutoPtr.hpp>



#include <xalanc/XalanDOM/XalanDocument.hpp>
#include <xalanc/XalanDOM/XalanElement.hpp>



#include <xalanc/PlatformSupport/XalanOutputStreamPrintWriter.hpp>
#include <xalanc/PlatformSupport/XalanStdOutputStream.hpp>



#include <xalanc/XMLSupport/FormatterToXML.hpp>



#include <xalanc/XercesParserLiaison/FormatterToXercesDOM.hpp>
#include <xalanc/XercesParserLiaison/XercesDOMFormatterWalker.hpp>



#include <xalanc/XalanTransformer/XalanTransformer.hpp>



XALAN_USING_STD(cerr)
XALAN_USING_STD(cout)
XALAN_USING_STD(endl)

XALAN_USING_XERCES(DOMDocument)

XALAN_USING_XALAN(XalanCompiledStylesheet)
XALAN_USING_XALAN(XalanParsedSource)
XALAN_USING_XALAN(XalanTransformer)



void
serialize(const DOMDocument&	theDocument)
{
	XALAN_USING_XALAN(XalanStdOutputStream)
	XALAN_USING_XALAN(XalanOutputStreamPrintWriter)
	XALAN_USING_XALAN(FormatterToXML)
	XALAN_USING_XALAN(XercesDOMFormatterWalker)

	// Create an output stream and a PrintWriter for the
	// output
	XalanStdOutputStream			theStream(cout);
	XalanOutputStreamPrintWriter	thePrintWriter(theStream);

	// We're going to serialize XML...
	FormatterToXML	theFormatter(thePrintWriter);

	// Do pretty-printing...
	theFormatter.setDoIndent(true);

	theFormatter.setIndent(2);

	XercesDOMFormatterWalker	theWalker(theFormatter);

	// This will walk the document and send
	// events to the FormatterToXML.
	theWalker.traverse(&theDocument);
}



int
transformAndSerialize(
			XalanTransformer&				theTransformer,
			const XalanParsedSource&		theParsedSource,
			const XalanCompiledStylesheet&	theStylesheet)
{
	XALAN_USING_XERCES(DOMDocument)
	XALAN_USING_XERCES(DOMImplementation)
	XALAN_USING_XALAN(FormatterToXercesDOM)
	XALAN_USING_XALAN(XalanAutoPtr)

	// This is the document which we'll build...
	const XalanAutoPtr<DOMDocument>		theDocument(DOMImplementation::getImplementation()->createDocument());
	assert(theDocument.get() != 0);

	// This is a class derived from FormatterListener, which
	// we'll hook up to Xalan's output stage...
	FormatterToXercesDOM	theFormatter(theDocument.get(), 0);

	// Do the transformation...
	int		theResult =
		theTransformer.transform(
			theParsedSource,
			&theStylesheet,
			theFormatter);

	if (theResult != 0)
	{
		cerr << "Error transforming: "
			 << theTransformer.getLastError()
			 << endl;
	}
	else
	{
		serialize(*theDocument.get());
	}

	return theResult;
}



int
main(
			int				argc,
			const char*		argv[])
{
#if !defined(NDEBUG) && defined(_MSC_VER)
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif

	int		theResult = 0;

	if (argc != 3)
	{
		cerr << "Usage: TransformToXercesDOM XMLFilePath XSLFilePath" << endl;

		theResult = -1;
	}
	else
	{
		try
		{
			XALAN_USING_XERCES(XMLPlatformUtils)

			XMLPlatformUtils::Initialize();

			XalanTransformer::initialize();

			{

				XalanTransformer	theTransformer;

				const XalanParsedSource*	theParsedSource = 0;

				theResult = theTransformer.parseSource(argv[1], theParsedSource);

				if (theResult != 0)
				{
					cerr << "Error parsing source document: "
						 << theTransformer.getLastError()
						 << endl;
				}
				else
				{
					assert(theParsedSource != 0);

					const XalanCompiledStylesheet*	theCompiledStylesheet = 0;

					theResult = theTransformer.compileStylesheet(argv[2], theCompiledStylesheet);

					if (theResult != 0)
					{
						cerr << "Error compiling stylesheet: "
							 << theTransformer.getLastError()
							 << endl;
					}
					else
					{
						theResult =
							transformAndSerialize(theTransformer, *theParsedSource, *theCompiledStylesheet);
					}
				}
			}

			XalanTransformer::terminate();

			XMLPlatformUtils::Terminate();
		}
		catch(...)
		{
			cerr << "Exception caught!" << endl;

			theResult = -1;
		}
	}

	return theResult;
}
