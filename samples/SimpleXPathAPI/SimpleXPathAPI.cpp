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
#include <iostream.h>
#else
#include <iostream>
#endif



#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>



#include <xalanc/PlatformSupport/XSLException.hpp>



#include <xalanc/DOMSupport/XalanDocumentPrefixResolver.hpp>



#include <xalanc/XPath/XObject.hpp>
#include <xalanc/XPath/XPathEvaluator.hpp>



#include <xalanc/XalanSourceTree/XalanSourceTreeDOMSupport.hpp>
#include <xalanc/XalanSourceTree/XalanSourceTreeInit.hpp>
#include <xalanc/XalanSourceTree/XalanSourceTreeParserLiaison.hpp>



int
main(
			int		argc,
			char*	argv[])
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
        XALAN_USING_XALAN(XSLException)

        try
		{
			XALAN_USING_XERCES(XMLPlatformUtils)

			XALAN_USING_XALAN(XPathEvaluator)


			XMLPlatformUtils::Initialize();

			XPathEvaluator::initialize();

			{
				XALAN_USING_XERCES(LocalFileInputSource)

				XALAN_USING_XALAN(XalanDocument)
				XALAN_USING_XALAN(XalanDocumentPrefixResolver)
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

				XalanDocumentPrefixResolver		thePrefixResolver(theDocument);

				XPathEvaluator	theEvaluator;

				// OK, let's find the context node...
				XalanNode* const	theContextNode =
						theEvaluator.selectSingleNode(
							theDOMSupport,
							theDocument,
							XalanDOMString(argv[2]).c_str(),
							thePrefixResolver);

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
								thePrefixResolver));

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
        catch(const XSLException&   theException)
        {
			cerr << "XSL exception: "
                 << theException.getMessage()
                 << endl;

			theResult = -1;
        }
		catch(...)
		{
			cerr << "Generic exception caught!" << endl;

			theResult = -1;
		}
	}

	return theResult;
}
