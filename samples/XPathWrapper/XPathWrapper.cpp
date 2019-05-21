/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the  "License");
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

#include "XPathWrapper.hpp"



#include <cassert>


#include <iostream>



#include <xercesc/framework/MemBufInputSource.hpp>



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



using std::cerr;
using std::endl;
using std::ostream;



////////////////////////////////////////////////////

// the implementation class, that does all calls to XPath and Xerces
class XPathWrapperImpl
{
public:

    typedef XPathWrapper::CharVectorTypeVectorType  CharVectorTypeVectorType;

    void
    evaluate(
        const char*     xml,
        const char*     context, 
        const char*     expr,
        ostream&        errorStream,
        CharVectorTypeVectorType& theResultList)
    {
        using std::strlen;


        {
            // Just hoist everything...
            using namespace xalanc;

            typedef XPathConstructionContext::GetAndReleaseCachedString     GetAndReleaseCachedString;

            // Initialize the XPath subsystem...
            XPathInit                       theInit;

            // We'll use these to parse the XML file.
            XalanSourceTreeDOMSupport       theDOMSupport;
            XalanSourceTreeParserLiaison    theLiaison(theDOMSupport);

            // Hook the two together...
            theDOMSupport.setParserLiaison(&theLiaison);

            XalanElement*   rootElem = 0;

            using xercesc::XMLException;

            try
            {
                using xercesc::MemBufInputSource;

                // parse XML and get root element
                MemBufInputSource   inStream((XMLByte*)xml, strlen(xml), "foo", false);

                XalanDocument* const    doc = theLiaison.parseXMLStream(inStream);
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
            XPathEnvSupportDefault          theEnvSupport;
            XObjectFactoryDefault           theXObjectFactory;
            XPathExecutionContextDefault    theExecutionContext(theEnvSupport, theDOMSupport, theXObjectFactory);
            XPathConstructionContextDefault theXPathConstructionContext;
            XPathFactoryDefault             theXPathFactory;
            XPathProcessorImpl              theXPathProcessor;

            try
            {
                // first get the context nodeset
                XPath* const    contextXPath = theXPathFactory.create();

                const ElementPrefixResolverProxy    thePrefixResolver(rootElem, theEnvSupport, theDOMSupport);

                const GetAndReleaseCachedString     theGuard(theXPathConstructionContext);

                XalanDOMString&     theString = theGuard.get();

                theString = context;

                theXPathProcessor.initXPath(
                    *contextXPath,
                    theXPathConstructionContext,
                    theString,
                    thePrefixResolver);

                XObjectPtr  xObj =
                    contextXPath->execute(
                        rootElem,
                        thePrefixResolver,
                        theExecutionContext);

                const NodeRefListBase&  contextNodeList = xObj->nodeset();

                const unsigned int  theLength =
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
                    XPath* const    xpath = theXPathFactory.create();

                    theString = expr;

                    theXPathProcessor.initXPath(
                        *xpath,
                        theXPathConstructionContext,
                        theString,
                        thePrefixResolver);

                    xObj = xpath->execute(
                        contextNodeList.item(0),
                        thePrefixResolver,
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
                                XalanNode* const    node = nodeset.item(i);
                                XalanDOMString      str;

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

        }
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



void
XPathWrapper::evaluate(
        const char*     xml, 
        const char*     context, 
        const char*     path,
        CharVectorTypeVectorType& theResult)
{
    pImpl->evaluate(
            xml,
            context,
            path,
            cerr,
            theResult);
}


