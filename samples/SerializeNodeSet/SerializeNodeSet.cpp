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



#include <xalanc/XalanDOM/XalanDocument.hpp>
#include <xalanc/XalanDOM/XalanElement.hpp>



#include <xalanc/PlatformSupport/XalanOutputStreamPrintWriter.hpp>
#include <xalanc/PlatformSupport/XalanStdOutputStream.hpp>



#include <xalanc/XPath/NodeRefList.hpp>
#include <xalanc/XPath/XObject.hpp>
#include <xalanc/XPath/XPathEvaluator.hpp>



#include <xalanc/XMLSupport/FormatterToXML.hpp>
#include <xalanc/XMLSupport/FormatterTreeWalker.hpp>
#include <xalanc/XMLSupport/XMLSupportInit.hpp>



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
		cerr << "Usage: SerializeNodeSet XMLFilePath Context XPathExpression" << endl;

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

				XALAN_USING_XALAN(NodeRefList)
				XALAN_USING_XALAN(XalanDocument)
				XALAN_USING_XALAN(XalanDOMString)
				XALAN_USING_XALAN(XalanNode)
				XALAN_USING_XALAN(XalanSourceTreeInit)
				XALAN_USING_XALAN(XMLSupportInit)
				XALAN_USING_XALAN(XalanSourceTreeDOMSupport)
				XALAN_USING_XALAN(XalanSourceTreeParserLiaison)


				// Initialize the XalanSourceTree subsystem...
				XalanSourceTreeInit		theSourceTreeInit;

				// Initialize the XMLSupport subsystem...
				XMLSupportInit		theXMLSupportInit;

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
					cerr << "Error: No nodes matched the location path \""
						 << argv[2]
						 << "\"."
						 << endl
						 << "Execution cannot continue."
						 << endl
						 << endl;
				}
				else
				{
					// OK, let's evaluate the expression...
					NodeRefList	theResult;
				    theEvaluator.selectNodeList(
                                theResult,
								theDOMSupport,
								theContextNode,
								XalanDOMString(argv[3]).c_str(),
								theDocument->getDocumentElement());

					const NodeRefList::size_type	theLength = theResult.getLength();

					if (theLength == 0)
					{
						cerr << endl
							 << "Warning: No nodes matched the location path \""
							 << argv[3]
							 << "\"."
							 << endl
							 << endl;
					}
					else
					{
						XALAN_USING_XALAN(XalanStdOutputStream)
						XALAN_USING_XALAN(XalanOutputStreamPrintWriter)
						XALAN_USING_XALAN(FormatterToXML)
						XALAN_USING_XALAN(FormatterTreeWalker)

						// OK, we're going to serialize the nodes that were
						// found.  We should really check to make sure the
						// root (document) has not been selected, since we
						// really can't serialize a node list with the root.
						XalanStdOutputStream			theStream(cout);
						XalanOutputStreamPrintWriter	thePrintWriter(theStream);

						FormatterToXML			theFormatter(thePrintWriter);
						FormatterTreeWalker		theWalker(theFormatter);

						// Don't write a header...
						theFormatter.setShouldWriteXMLHeader(false);

						// It's required that we do this...
						theFormatter.startDocument();

						// Traverse the subtree of the document rooted at
						// each node we've selected...
						for (NodeRefList::size_type i = 0; i < theLength; ++i)
						{
							const XalanNode* const	theNode = theResult.item(i);
							assert(theNode != 0);

							const XalanNode::NodeType	theNodeType =
								theNode->getNodeType();

							if (theNodeType == XalanNode::DOCUMENT_NODE)
							{
								cerr << endl
									 << "Warning: The root was selected.  The root cannot be serialized."
									 << endl;
							}
							else if (theNodeType == XalanNode::ATTRIBUTE_NODE)
							{
								cerr << endl
									 << "Warning: An attribute or namespace node was selected.  Attribute and namespace nodes cannot be serialized."
									 << endl;
							}
							else
							{
								theWalker.traverseSubtree(theNode);
							}
						}

						// It's required that we do this...
						theFormatter.endDocument();
					}
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
