#include "XPathWrapper.hpp"



#include <vector>
#include <cassert>


#if defined(XALAN_OLD_STREAM_HEADERS)
#include <iostream.h>
#else
#include <iostream>
#endif



#include <framework/MemBufInputSource.hpp>
#include <framework/URLInputSource.hpp>
#include <parsers/DOMParser.hpp>
#include <util/PlatformUtils.hpp>



#include <XalanDOM/XalanDocument.hpp>
#include <XalanDOM/XalanElement.hpp>



#include <PlatformSupport/DirectoryEnumerator.hpp>
#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/STLHelper.hpp>



#include <DOMSupport/DOMSupportDefault.hpp>



#include <XPath/XObjectFactoryDefault.hpp>
#include <XPath/XPath.hpp>
#include <XPath/XPathEnvSupportDefault.hpp>
#include <XPath/XPathExecutionContextDefault.hpp>
#include <XPath/XPathInit.hpp>
#include <XPath/XPathSupportDefault.hpp>
#include <XPath/XPathProcessorImpl.hpp>
#include <XPath/XPathFactoryDefault.hpp>
#include <XPath/ElementPrefixResolverProxy.hpp>



#include <XercesParserLiaison/XercesParserLiaison.hpp>
#include <XercesParserLiaison/XercesDOMSupport.hpp>



#if !defined(XALAN_NO_NAMESPACES)
using std::cerr;
using std::endl;
using std::vector;
#endif



////////////////////////////////////////////////////

// the implementation class, that does all calls to XPath and Xerces
class XPathWrapperImpl
{
public:

	XPathWrapper::CharVectorTypeVectorType
	evaluate(
		const CharVectorType&	xml, 
		const CharVectorType&	context, 
		const CharVectorType&	expr)
	{
		//initialize Xerces...
		try
		{
			XMLPlatformUtils::Initialize();
		}
		catch(const XMLException&)
		{
			cerr << "XMLPlatformUtils::Initialize() failed!" << endl;

			throw;
		}

		XPathWrapper::CharVectorTypeVectorType	theResultList;

		{
			// Initialize the XPath subsystem...
			XPathInit						theInit;

			// parse the XML file
			XercesDOMSupport				theDOMSupport;
			XercesParserLiaison				theLiaison(theDOMSupport);

			XalanElement*	rootElem = 0;

			try
			{
				// parse XML and get root element
				MemBufInputSource inStream((XMLByte*)c_str(xml), xml.size(), "foo", false);

				XalanDocument* const	doc = theLiaison.parseXMLStream(inStream);
				assert(doc != 0);

				rootElem = doc->getDocumentElement();
				assert(rootElem != 0);
			}
			catch(const XMLException&)
			{
				cerr << "Caught XMLExecption..." << endl;

				throw;
			}

			// configure the objects needed for XPath to work with the Xerces DOM
			XPathEnvSupportDefault			theEnvSupport;
			XPathSupportDefault				theSupport(theDOMSupport);
			XObjectFactoryDefault			theXObjectFactory;
			XPathExecutionContextDefault	theExecutionContext(theEnvSupport, theSupport, theXObjectFactory);
			XPathFactoryDefault				theXPathFactory;
			XPathProcessorImpl				theXPathProcessor;

			try
			{
				// first get the context nodeset
				XPath* const	contextXPath = theXPathFactory.create();

				theXPathProcessor.initXPath(*contextXPath,										
											TranscodeFromLocalCodePage(context),
											ElementPrefixResolverProxy(rootElem, theEnvSupport, theSupport),
											theEnvSupport);

	   			XObjectPtr	xObj =
					contextXPath->execute(rootElem,
										  ElementPrefixResolverProxy(rootElem, theEnvSupport, theSupport),
										  theExecutionContext);

				const NodeRefListBase&	contextNodeList = xObj->nodeset();

				const unsigned int	theLength =
						contextNodeList.getLength();

				if (theLength == 0)
				{
						cerr << "Warning -- No nodes matched the location path \""
							 << context
							 << "\"."
							 << endl
							 << "Execution cannot continue..."
							 << endl
							 << endl;
				}
				else
				{
					if (theLength > 1)
					{
						cerr << "Warning -- More than one node matched the location path \""
							 << context
							 << "\"."
							 << endl
							 << "The first node matched will be used as the context node."
							 << endl
							 << endl;
					}

					// and now get the result of the primary xpath expression
					XPath* const	xpath = theXPathFactory.create();
					theXPathProcessor.initXPath(*xpath,
												TranscodeFromLocalCodePage(expr),
												ElementPrefixResolverProxy(rootElem, theEnvSupport, theSupport),
												theEnvSupport);

					xObj = xpath->execute(contextNodeList.item(0),
										  ElementPrefixResolverProxy(rootElem, theEnvSupport, theSupport),
										  theExecutionContext);

					// now encode the results.  For all types but nodelist, we'll just convert it to a string
					// but, for nodelist, we'll convert each node to a string and return a list of them
					switch (xObj->getType())
					{
						case XObject::eTypeNodeSet:
						{
							const NodeRefListBase& nodeset = xObj->nodeset();
							size_t len = nodeset.getLength();

							for (size_t i=0; i<len; i++)
							{
								XalanNode* const	node = nodeset.item(i);
								XalanDOMString		str;

								const int theType = node->getNodeType();

								if (theType == XalanNode::COMMENT_NODE ||
									theType == XalanNode::PROCESSING_INSTRUCTION_NODE)
									str = node->getNodeValue();
								else if (theType == XalanNode::ELEMENT_NODE)
									str = node->getNodeName();
								else
									theSupport.getNodeData(*node, str);

								theResultList.push_back(TranscodeToLocalCodePage(str));
							}

							break;
						}

						default:
						{
							theResultList.push_back(TranscodeToLocalCodePage(xObj->str()));

							break;
						}
					}
				}
			}
			catch(const XMLException&)
			{
				cerr << "Caught XMLExecption..." << endl;

				throw;
			}

			// Shut down Xerces...
			XMLPlatformUtils::Initialize();
		}

		return theResultList;
	}
};


////////////////////////////////////////////////////

// The public XPathWrapper methods just delegate to our impl class

XPathWrapper::XPathWrapper() :
	pImpl(new XPathWrapperImpl())
{
}



XPathWrapper::~XPathWrapper()
{
	delete pImpl;
}



XPathWrapper::CharVectorTypeVectorType
XPathWrapper::evaluate(
		const CharVectorType&	xml, 
		const CharVectorType&	context, 
		const CharVectorType&	path)
{
	return pImpl->evaluate(xml, context, path);
}


