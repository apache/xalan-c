// Base header file.  Must be first.
#include <Include/PlatformDefinitions.hpp>



#include <cassert>


#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#else
#include <iostream>
#endif



#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>



#include <XalanDOM/XalanDocument.hpp>
#include <XalanDOM/XalanElement.hpp>



#include <XPath/XObject.hpp>
#include <XPath/XPathEvaluator.hpp>



#include <XalanSourceTree/XalanSourceTreeDOMSupport.hpp>
#include <XalanSourceTree/XalanSourceTreeInit.hpp>
#include <XalanSourceTree/XalanSourceTreeParserLiaison.hpp>



XALAN_USING_XALAN(XalanNode)
XALAN_USING_XALAN(XalanElement)



const XalanElement*
getPrefixResolver(const XalanNode*	node)
{
	if (node == 0)
	{
		return 0;
	}
	else if (node->getNodeType() == XalanNode::ELEMENT_NODE)
	{
#if defined(XALAN_OLD_STYLE_CASTS)
		return (const XalanElement*)node;
#else
		return static_cast<const XalanElement*>(node);
#endif
	}
	else if (node->getNodeType() == XalanNode::DOCUMENT_NODE)
	{
		XALAN_USING_XALAN(XalanDocument)

#if defined(XALAN_OLD_STYLE_CASTS)
		return ((const XalanDocument*)node)->getDocumentElement();
#else
		return static_cast<const XalanDocument*>(node)->getDocumentElement();
#endif
	}
	else
	{
		return getPrefixResolver(node->getParentNode());
	}
}



int
main(
			int				argc,
			const char*		argv[])
{
	XALAN_USING_STD(cerr)
	XALAN_USING_STD(cout)
	XALAN_USING_STD(endl)

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
			XALAN_USING_XERCES(XMLPlatformUtils)

			XALAN_USING_XALAN(XPathEvaluator)


			XMLPlatformUtils::Initialize();

			XPathEvaluator::initialize();

			{
				XALAN_USING_XERCES(LocalFileInputSource)

				XALAN_USING_XALAN(XalanDocument)
				XALAN_USING_XALAN(XalanDOMString)
				XALAN_USING_XALAN(XalanNode)
				XALAN_USING_XALAN(XalanSourceTreeInit)
				XALAN_USING_XALAN(XalanSourceTreeDOMSupport)
				XALAN_USING_XALAN(XalanSourceTreeParserLiaison)
				XALAN_USING_XALAN(XObjectPtr)

				// Initialize the XalanSourceTree subsystem...
				XalanSourceTreeInit		theSourceTreeInit;

				// We'll use these to parse the XML file.
				XalanSourceTreeDOMSupport		theDOMSupport;
				XalanSourceTreeParserLiaison	theLiaison(theDOMSupport);

				// Hook the two together...
				theDOMSupport.setParserLiaison(&theLiaison);

				const XalanDOMString	theFileName(argv[1]);

				// Create an input source that represents a local file...
				const LocalFileInputSource	theInputSource(theFileName.c_str());

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
								getPrefixResolver(theContextNode)));

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
}
