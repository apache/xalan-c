#include <string>
#include <vector>
#include <cassert>
#include <iostream>

#include <parsers/DOMParser.hpp>
#include <util/PlatformUtils.hpp>
#include <dom/DOM_Node.hpp>
#include <dom/DOM_Document.hpp>
#include <dom/DOM_Element.hpp>
#include <dom/DOM_NodeList.hpp>
#include <internal/URLInputSource.hpp>
#include <PlatformSupport/DirectoryEnumerator.hpp>
#include <PlatformSupport/DOMStringHelper.hpp>
#include <DOMSupport/DOMSupportDefault.hpp>
#include <XPath/XObjectFactoryDefault.hpp>
#include <XPath/XPathEnvSupportDefault.hpp>
#include <XPath/XPathSupportDefault.hpp>
#include <XPath/XPath.hpp>
#include <XPath/XPathProcessorImpl.hpp>
#include <XPath/XPathFactoryDefault.hpp>
#include <XPath/ElementPrefixResolverProxy.hpp>
#include <XMLSupport/FormatterTreeWalker.hpp>
#include <XMLSupport/FormatterToXML.hpp>
#include <XercesParserLiaison/XercesParserLiaison.hpp>

#include <internal/MemBufInputSource.hpp>

#include "XPathWrapper.hpp"


////////////////////////////////////////////////////

// the implementation class, that does all calls to XPath and Xerces
class XPathWrapperImpl
{
private:
	std::string DOMStringToStdString(const DOMString& domString)
	{
		std::string ret;

		for (int i=0; i< domString.length(); i++)
		{
			ret += (char)domString.charAt(i);
		}

		return ret;
	}

public:
	std::vector<std::string> evaluate(
		const std::string& xml, 
		const std::string& context, 
		const std::string& expr)
	{
		//initialize XML4C
		try
		{
			XMLPlatformUtils::Initialize();
		}

		catch(const XMLException& e)
		{
			throw;
		}

		// parse the XML file
		DOM_Element rootNode;
		DOMParser parser;

		try
		{
			// parse XML and get root element
			MemBufInputSource inStream((const XMLByte*)xml.c_str(), 
				xml.length(), "foo", false);

			parser.parse(inStream);

			DOM_Document doc = parser.getDocument();
			rootNode = doc.getDocumentElement();
		}

		catch(const XMLException& e)
		{
			throw;
		}


		// configure the objects needed for XPath to work with the Xerces DOM
		XPathEnvSupportDefault	theEnvSupport;
		DOMSupportDefault		theDOMSupport;
		XPathSupportDefault		theSupport(theDOMSupport);
		XObjectFactoryDefault	theXObjectFactory(theEnvSupport, theSupport);
		XPathFactoryDefault		theXPathFactory(theXObjectFactory, theEnvSupport, theSupport);
		XPathProcessorImpl		theXPathProcessor(theEnvSupport, theSupport);
		XercesParserLiaison		theLiaison(theDOMSupport);

		std::vector<std::string> theResultList;

		try
		{
			// first get the context nodeset
			XPath *contextXPath = theXPathFactory.create();
			theXPathProcessor.initXPath(*contextXPath, DOMString(context.c_str()), ElementPrefixResolverProxy(rootNode,theSupport));
			contextXPath->shrink();

			MutableNodeRefList contextNodeList;		//default empty context
	   		XObject* xObj = contextXPath->execute(rootNode, ElementPrefixResolverProxy(rootNode,theSupport), contextNodeList);
			contextNodeList = xObj->mutableNodeset();


			// and now get the result of the primary xpath expression
			XPath *xpath = theXPathFactory.create();
			theXPathProcessor.initXPath(*xpath, DOMString(expr.c_str()), ElementPrefixResolverProxy(rootNode, theSupport));
			xpath->shrink();

			xObj = xpath->execute(rootNode, ElementPrefixResolverProxy(rootNode,theSupport), contextNodeList);

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
						DOM_Node node = nodeset.item(i);
						DOMString str;
						
						const int theType = node.getNodeType();

						if (theType==DOM_Node::COMMENT_NODE || theType==DOM_Node::PROCESSING_INSTRUCTION_NODE)
							str = node.getNodeValue();
						else
							str = theSupport.getNodeData(node);

						theResultList.push_back(DOMStringToStdString(str));
					}

					break;
				}

				default:
				{
					theResultList.push_back(DOMStringToStdString(xObj->str()));
					break;
				}
			}
		}

		catch(const XMLException& e)
		{
			throw;
		}

		return theResultList;
	}
};


////////////////////////////////////////////////////

// The public XPathWrapper methods just delegate to our impl class

XPathWrapper::XPathWrapper()
{
	pImpl = new XPathWrapperImpl();
}

XPathWrapper::~XPathWrapper()
{
	delete pImpl;
}

std::vector<std::string> XPathWrapper::evaluate(
	const std::string& xml, 
	const std::string& context, 
	const std::string& path)
{
	return pImpl->evaluate(xml,context,path);
}
