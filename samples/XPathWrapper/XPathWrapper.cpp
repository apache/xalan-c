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

#include "XPathWrapper.hpp"



#include <cassert>


#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#else
#include <iostream>
#endif



#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/util/PlatformUtils.hpp>



#include <xalanc/Include/STLHelper.hpp>



#include <xalanc/XalanDOM/XalanDocument.hpp>
#include <xalanc/XalanDOM/XalanElement.hpp>



#include <xalanc/PlatformSupport/DirectoryEnumerator.hpp>
#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include <xalanc/XPath/XObjectFactoryDefault.hpp>
#include <xalanc/XPath/XPath.hpp>
#include <xalanc/XPath/XPathConstructionContextDefault.hpp>
#include <xalanc/XPath/XPathEnvSupportDefault.hpp>
#include <xalanc/XPath/XPathExecutionContextDefault.hpp>
#include <xalanc/XPath/XPathInit.hpp>
#include <xalanc/XPath/XPathProcessorImpl.hpp>
#include <xalanc/XPath/XPathFactoryDefault.hpp>
#include <xalanc/XPath/ElementPrefixResolverProxy.hpp>



#include <xalanc/XalanSourceTree/XalanSourceTreeDOMSupport.hpp>
#include <xalanc/XalanSourceTree/XalanSourceTreeParserLiaison.hpp>



XALAN_USING_STD(cerr)
XALAN_USING_STD(endl)
XALAN_USING_STD(ostream)



////////////////////////////////////////////////////

// the implementation class, that does all calls to XPath and Xerces
class XPathWrapperImpl
{
public:

	XPathWrapper::CharVectorTypeVectorType
	evaluate(
		const char*		xml,
		const char*		context, 
		const char*		expr,
		ostream&		errorStream)
	{
#if defined(XALAN_STRICT_ANSI_HEADERS)
		using std::strlen;
#endif

		XALAN_USING_XERCES(XMLPlatformUtils)
		XALAN_USING_XERCES(XMLException)

		//initialize Xerces...
		try
		{
			XMLPlatformUtils::Initialize();
		}
		catch(const XMLException&)
		{
			errorStream << "XMLPlatformUtils::Initialize() failed!" << endl;

			throw;
		}

		XPathWrapper::CharVectorTypeVectorType	theResultList;

		{
			// Just hoist everything...
			XALAN_CPP_NAMESPACE_USE

			// Initialize the XPath subsystem...
			XPathInit						theInit;

			// We'll use these to parse the XML file.
			XalanSourceTreeDOMSupport		theDOMSupport;
			XalanSourceTreeParserLiaison	theLiaison(theDOMSupport);

			// Hook the two together...
			theDOMSupport.setParserLiaison(&theLiaison);

			XalanElement*	rootElem = 0;

			try
			{
				XALAN_USING_XERCES(MemBufInputSource)

				// parse XML and get root element
				MemBufInputSource	inStream((XMLByte*)xml, strlen(xml), "foo", false);

				XalanDocument* const	doc = theLiaison.parseXMLStream(inStream);
				assert(doc != 0);

				rootElem = doc->getDocumentElement();
				assert(rootElem != 0);
			}
			catch(const XMLException&)
			{
				errorStream << "Caught XMLExecption..." << endl;

				throw;
			}

			// configure the objects needed for XPath to work with the Xerces DOM
			XPathEnvSupportDefault			theEnvSupport;
			XObjectFactoryDefault			theXObjectFactory;
			XPathExecutionContextDefault	theExecutionContext(theEnvSupport, theDOMSupport, theXObjectFactory);
			XPathConstructionContextDefault	theXPathConstructionContext;
			XPathFactoryDefault				theXPathFactory;
			XPathProcessorImpl				theXPathProcessor;

			try
			{
				// first get the context nodeset
				XPath* const	contextXPath = theXPathFactory.create();

				theXPathProcessor.initXPath(*contextXPath,
											theXPathConstructionContext,
											XalanDOMString(context),
											ElementPrefixResolverProxy(rootElem, theEnvSupport, theDOMSupport));

	   			XObjectPtr	xObj =
					contextXPath->execute(rootElem,
										  ElementPrefixResolverProxy(rootElem, theEnvSupport, theDOMSupport),
										  theExecutionContext);

				const NodeRefListBase&	contextNodeList = xObj->nodeset();

				const unsigned int	theLength =
						contextNodeList.getLength();

				if (theLength == 0)
				{
						errorStream << "Warning -- No nodes matched the location path \""
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
						errorStream << "Warning -- More than one node matched the location path \""
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
												theXPathConstructionContext,
												TranscodeFromLocalCodePage(expr),
												ElementPrefixResolverProxy(rootElem, theEnvSupport, theDOMSupport));

					xObj = xpath->execute(contextNodeList.item(0),
										  ElementPrefixResolverProxy(rootElem, theEnvSupport, theDOMSupport),
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
									DOMServices::getNodeData(*node, str);

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
				errorStream << "Caught XMLExecption..." << endl;

				throw;
			}

			// Shut down Xerces...
			XMLPlatformUtils::Terminate();
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
		const char*		xml, 
		const char*		context, 
		const char*		path)
{
	return pImpl->evaluate(
			xml,
			context,
			path,
			cerr);
}


