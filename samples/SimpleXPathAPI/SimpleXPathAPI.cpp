// Base header file.  Must be first.
#include <Include/PlatformDefinitions.hpp>



#include <cassert>


#if defined(XALAN_OLD_STREAM_HEADERS)
#include <iostream.h>
#else
#include <iostream>
#endif



#include <util/PlatformUtils.hpp>
#include <framework/LocalFileInputSource.hpp>



#include <XalanDOM/XalanDocument.hpp>
#include <XalanDOM/XalanElement.hpp>



#include <XPath/XObject.hpp>
#include <XPath/XPathEvaluator.hpp>



#include <XalanSourceTree/XalanSourceTreeDOMSupport.hpp>
#include <XalanSourceTree/XalanSourceTreeInit.hpp>
#include <XalanSourceTree/XalanSourceTreeParserLiaison.hpp>



int
main(
			int				argc,
			const char*		argv[])
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::cerr;
	using std::cout;
	using std::endl;
#endif

	int		theResult = 0;

	if (argc != 4)
	{
		cerr << "Usage: SimpleXPathAPI XMLFilePath Context XPathExpression" << endl;

		theResult = -1;
	}
	else
	{
		try
		{
			XMLPlatformUtils::Initialize();

			XPathEvaluator::initialize();

			{
				// Initialize the XalanSourceTree subsystem...
				XalanSourceTreeInit		theSourceTreeInit;

				// We'll use these to parse the XML file.
				XalanSourceTreeDOMSupport		theDOMSupport;
				XalanSourceTreeParserLiaison	theLiaison(theDOMSupport);

				// Hook the two together...
				theDOMSupport.setParserLiaison(&theLiaison);

				// Create an input source that represents a local file...
				const LocalFileInputSource	theInputSource(c_wstr(XalanDOMString(argv[1])));

				// Parse the document...
				XalanDocument* const	theDocument =
						theLiaison.parseXMLStream(theInputSource);
				assert(theDocument != 0);

				XPathEvaluator	theEvaluator;

				// OK, let's find the context node...
				XalanNode* const	theContextNode =
						theEvaluator.selectSingleNode(
							theDOMSupport,
							theDocument,
							XalanDOMString(argv[2]).c_str(),
							theDocument->getDocumentElement());

				if (theContextNode == 0)
				{
					cerr << "Warning -- No nodes matched the location path \""
						 << argv[2]
						 << "\"."
						 << endl
						 << "Execution cannot continue..."
						 << endl
						 << endl;
				}
				else
				{
					// OK, let's evaluate the expression...
					const XObjectPtr	theResult(
						theEvaluator.evaluate(
								theDOMSupport,
								theContextNode,
								XalanDOMString(argv[3]).c_str(),
								theDocument->getDocumentElement()));

					assert(theResult.null() == false);

					cout << "The string value of the result is:"
						 << endl
						 << theResult->str()
						 << endl
						 << endl;
				}
			}

			XPathEvaluator::terminate();

			XMLPlatformUtils::Terminate();
		}
		catch(...)
		{
			cerr << "Exception caught!" << endl;

			theResult = -1;
		}
	}

	return theResult;
};
