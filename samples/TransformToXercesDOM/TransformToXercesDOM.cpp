// Base header file.  Must be first.
#include <Include/PlatformDefinitions.hpp>



#include <cassert>

#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#else
#include <iostream>
#endif



#include <xercesc/dom/impl/DOMDocumentImpl.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>



#include <XalanDOM/XalanDocument.hpp>
#include <XalanDOM/XalanElement.hpp>



#include <PlatformSupport/XalanOutputStreamPrintWriter.hpp>
#include <PlatformSupport/XalanStdOutputStream.hpp>



#include <XMLSupport/FormatterToXML.hpp>



#include <XercesParserLiaison/FormatterToXercesDOM.hpp>
#include <XercesParserLiaison/XercesDOMFormatterWalker.hpp>



#include <XalanTransformer/XalanTransformer.hpp>



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

	// OK, we're going to serialize the nodes that were
	// found.  We should really check to make sure the
	// root (document) has not been selected, since we
	// really can't serialize a node list with the root.
	XalanStdOutputStream			theStream(cout);
	XalanOutputStreamPrintWriter	thePrintWriter(theStream);

	FormatterToXML	theFormatter(thePrintWriter);

	XercesDOMFormatterWalker	theWalker(theFormatter);

	theWalker.traverse(&theDocument);
}



int
transformAndSerialize(
			XalanTransformer&				theTransformer,
			const XalanParsedSource&		theParsedSource,
			const XalanCompiledStylesheet&	theStylesheet)
{
	XALAN_USING_XERCES(DOMDocumentImpl)
	XALAN_USING_XALAN(FormatterToXercesDOM)

	DOMDocumentImpl			theDocument;

	FormatterToXercesDOM	theFormatter(&theDocument, 0);

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
		serialize(theDocument);
	}

	return theResult;
}



int
main(
			int				argc,
			const char*		argv[])
{
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

				XalanParsedSource*	theParsedSource = 0;

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

					XalanCompiledStylesheet*	theCompiledStylesheet = 0;

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
