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

#include <xalanc/Include/PlatformDefinitions.hpp>


#include <xalanc/XalanTransformer/XalanTransformer.hpp>


#include <cstdio>
#include <ctime>



#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#else
#include <iostream>
#endif



#if !defined(NDEBUG) && defined(_MSC_VER)
#include <crtdbg.h>
#endif



#include <xercesc/framework/URLInputSource.hpp>
#include <xercesc/util/PlatformUtils.hpp>



#include <xalanc/Include/XalanVector.hpp>



#include <xalanc/XalanDOM/XalanNode.hpp>
#include <xalanc/XalanDOM/XalanDocument.hpp>
#include <xalanc/XalanDOM/XalanElement.hpp>
#include <xalanc/XalanDOM/XalanNamedNodeMap.hpp>
#include <xalanc/XalanDOM/XalanNodeList.hpp>



#include <xalanc/PlatformSupport/DoubleSupport.hpp>
#include <xalanc/PlatformSupport/DirectoryEnumerator.hpp>
#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanUnicode.hpp>
#include <xalanc/PlatformSupport/XalanOutputStreamPrintWriter.hpp>
#include <xalanc/PlatformSupport/XalanStdOutputStream.hpp>



#include <xalanc/XPath/ElementPrefixResolverProxy.hpp>
#include <xalanc/XPath/XPathInit.hpp>
#include <xalanc/XPath/XObjectFactoryDefault.hpp>
#include <xalanc/XPath/XPathEnvSupportDefault.hpp>
#include <xalanc/XPath/XPathExecutionContextDefault.hpp>
#include <xalanc/XPath/XPath.hpp>
#include <xalanc/XPath/XPathConstructionContextDefault.hpp>
#include <xalanc/XPath/XPathProcessorImpl.hpp>
#include <xalanc/XPath/XPathFactoryDefault.hpp>



#include <xalanc/XMLSupport/FormatterTreeWalker.hpp>
#include <xalanc/XMLSupport/FormatterToXML.hpp>
#include <xalanc/XMLSupport/XMLSupportInit.hpp>



#include <xalanc/XalanSourceTree/XalanSourceTreeDOMSupport.hpp>
#include <xalanc/XalanSourceTree/XalanSourceTreeParserLiaison.hpp>



#include "NodeNameTreeWalker.hpp"



XALAN_CPP_NAMESPACE_USE
XERCES_CPP_NAMESPACE_USE



const XObjectPtr
ExecuteXPath(
            XPathProcessor&             theXPathProcessor,
            XPathConstructionContext&   theXPathConstructionContext,
            XPath&                      theXPath,
            const XalanDOMString&       theXPathString,
            XalanNode*                  theContextNode,
            const PrefixResolver&       thePrefixResolver,
            const NodeRefListBase&      theContextNodeList,
            XPathExecutionContext&      theExecutionContext)
{
    theXPathProcessor.initXPath(
                theXPath,
                theXPathConstructionContext,
                theXPathString,
                thePrefixResolver);

    const XObjectPtr theResult =
        theXPath.execute(theContextNode, thePrefixResolver, theContextNodeList, theExecutionContext);

    return theResult;
}



bool
TestNumericResult(
            XPathProcessor&             theXPathProcessor,
            XPath&                      theXPath,
            XPathConstructionContext&   theXPathConstructionContext,
            const XalanDOMString&       theXPathString,
            PrintWriter&                thePrintWriter,
            double                      theExpectedResult,
            XalanNode*                  theContextNode,
            const PrefixResolver&       thePrefixResolver,
            const NodeRefListBase&      theContextNodeList,
            XPathExecutionContext&      theExecutionContext)
{
    bool    fError = false;

    const XObjectPtr theResult =
        ExecuteXPath(
            theXPathProcessor,
            theXPathConstructionContext,
            theXPath,
            theXPathString,
            theContextNode,
            thePrefixResolver,
            theContextNodeList,
            theExecutionContext);

    thePrintWriter.print("Execution of XPath ");
    thePrintWriter.print(theXPathString);

    if (theResult->num(theExecutionContext) == theExpectedResult)
    {
        thePrintWriter.println(" succeeded.");
        thePrintWriter.print("The result was ");
        thePrintWriter.println(theResult->num(theExecutionContext));
    }
    else
    {
        fError = true;

        thePrintWriter.println(" failed!");
        thePrintWriter.print("The expected result was ");
        thePrintWriter.println(theExpectedResult);
        thePrintWriter.print("The actual result was ");
        thePrintWriter.println(theResult->num(theExecutionContext));
    }

    return fError;
}



bool
TestStringResult(
            XPathProcessor&             theXPathProcessor,
            XPath&                      theXPath,
            XPathConstructionContext&   theXPathConstructionContext,
            const XalanDOMString&       theXPathString,
            PrintWriter&                thePrintWriter,
            const XalanDOMString&       theExpectedResult,
            XalanNode*                  theContextNode,
            const PrefixResolver&       thePrefixResolver,
            const NodeRefListBase&      theContextNodeList,
            XPathExecutionContext&      theExecutionContext)
{
    bool    fError = false;

    const XObjectPtr theResult =
        ExecuteXPath(
            theXPathProcessor,
            theXPathConstructionContext,
            theXPath,
            theXPathString,
            theContextNode,
            thePrefixResolver,
            theContextNodeList,
            theExecutionContext);

    thePrintWriter.print("Execution of XPath ");
    thePrintWriter.print(theXPathString);

    if (equals(theResult->str(theExecutionContext), theExpectedResult) == true)
    {
        thePrintWriter.println(" succeeded.");
        thePrintWriter.print("The result was \"");
        thePrintWriter.print(theResult->str(theExecutionContext));
        thePrintWriter.println("\".");
    }
    else
    {
        fError = true;

        thePrintWriter.println(" failed!");
        thePrintWriter.print("The expected result was \"");
        thePrintWriter.print(theExpectedResult);
        thePrintWriter.println("\".");
        thePrintWriter.print("The actual result was \"");
        thePrintWriter.print(theResult->str(theExecutionContext));
        thePrintWriter.println("\".");
    }

    return fError;
}



bool
TestBooleanResult(
            XPathProcessor&             theXPathProcessor,
            XPath&                      theXPath,
            XPathConstructionContext&   theXPathConstructionContext,
            const XalanDOMString&       theXPathString,
            PrintWriter&                thePrintWriter,
            bool                        theExpectedResult,
            XalanNode*                  theContextNode,
            const PrefixResolver&       thePrefixResolver,
            const NodeRefListBase&      theContextNodeList,
            XPathExecutionContext&      theExecutionContext)
{
    bool    fError = false;

    const XObjectPtr theResult =
        ExecuteXPath(
            theXPathProcessor,
            theXPathConstructionContext,
            theXPath,
            theXPathString,
            theContextNode,
            thePrefixResolver,
            theContextNodeList,
            theExecutionContext);

    bool    fDump = false;

    if (fDump == true)
    {
        thePrintWriter.println();
        thePrintWriter.println();
        theXPath.getExpression().dumpOpCodeMap(thePrintWriter);
        thePrintWriter.println();
        theXPath.getExpression().dumpTokenQueue(thePrintWriter);
        thePrintWriter.println();
        thePrintWriter.println();
    }

    thePrintWriter.print("Execution of XPath ");
    thePrintWriter.print(theXPathString);

    if (theResult->boolean(theExecutionContext) == theExpectedResult)
    {
        thePrintWriter.println(" succeeded.");
        thePrintWriter.print("The result was ");
        thePrintWriter.print(theResult->boolean(theExecutionContext));
        thePrintWriter.println(".");
    }
    else
    {
        fError = true;

        thePrintWriter.println(" failed!");
        thePrintWriter.print("The expected result was \"");
        thePrintWriter.print(theExpectedResult);
        thePrintWriter.println("\".");
        thePrintWriter.print("The actual result was \"");
        thePrintWriter.print(theResult->boolean(theExecutionContext));
        thePrintWriter.println("\".");
    }

    return fError;
}



XalanDocument*
ParseXML(
            XMLParserLiaison&       theLiaison,
            const XalanDOMString&   theFileName)
{
    const URLInputSource    theURLInputSource(theFileName.c_str());

    const XalanDOMString    empty(XalanMemMgrs::getDummyMemMgr());

    return theLiaison.parseXMLStream(theURLInputSource, empty);
}



XalanDOMString&
GetAttributeFromNode(
            const XalanNode*        theNode,
            const XalanDOMString&   theAttributeName,
            XalanDOMString&         theResult)
{
    

    if (theNode->getNodeType() == XalanNode::ELEMENT_NODE)
    {
        const XalanElement* const   theElement =
                    static_cast<const XalanElement*>(theNode);

        const XalanNamedNodeMap* const  theAttributes =
            theElement->getAttributes();

        if (theAttributes != 0)
        {
            const XalanNode* const  theNode =
                theAttributes->getNamedItem(theAttributeName);

            if (theNode != 0)
            {
                theResult = theNode->getNodeValue();
            }
        }
    }

    return theResult;
}



XalanDOMString&
FindNodeAndGetAttributeValue(
            XalanNode*              theStartNode,
            const XalanDOMString&   theNodeName,
            const XalanDOMString&   theAttributeName,
            XalanDOMString&         theResult,
            MemoryManager&      theMemoryManager)
{
    NodeNameTreeWalker  theTreeWalker(theMemoryManager);

    theTreeWalker.findMatchingNodes(theNodeName,
                                    theStartNode);

    const NodeNameTreeWalker::NodeVectorType&   theMatchingNodes =
            theTreeWalker.getMatchingNodes();

    if (theMatchingNodes.size() == 1)
    {
        const XalanNode* const  theMatchingNode = theMatchingNodes[0];

        if (theMatchingNode != 0)
        {
            GetAttributeFromNode(   theMatchingNode,
                                    theAttributeName,
                                    theResult);
        }
    }

    return theResult;
}



bool
GetXSLInput(
            XMLParserLiaison&       theLiaison,
            const XalanDOMString&   theXSLFileURL,
            XalanDOMString&         theContextNodeMatchPattern,
            XalanDOMString&         theXPathString,
            MemoryManager&      theMemoryManager)
{
    bool                    fResult = false;

    XalanDocument* const    theDocument =
        ParseXML(theLiaison,
                 theXSLFileURL);

    if (theDocument != 0)
    {
        const XalanDOMString    theContextNodeName("xsl:for-each", theMemoryManager);
        const XalanDOMString    theAttributeName("select", theMemoryManager);

         
        FindNodeAndGetAttributeValue(
            theDocument->getDocumentElement(),
            theContextNodeName,
            theAttributeName,
            theContextNodeMatchPattern,
            theMemoryManager);

        if (theContextNodeMatchPattern.empty() == false)
        {
            const XalanDOMString    theSelectNodeName("xsl:apply-templates", theMemoryManager);

             
            FindNodeAndGetAttributeValue(
                theDocument->getDocumentElement(),
                theSelectNodeName,
                theAttributeName,
                theXPathString,
                theMemoryManager);

            if (theXPathString.empty() == false)
            {
                fResult = true;
            }
        }
    }

    return fResult;
}



XalanNode*
FindContextNode(
            XPathProcessor&         theXPathProcessor,
            XPathEnvSupport&        theXPathEnvSupport,
            DOMSupport&             theDOMSupport,
            XPathFactory&           theXPathFactory,
            XalanDocument*          theDocument,
            const XalanDOMString&   theContextNodeMatchPattern,
            PrintWriter&            thePrintWriter,
            XPathExecutionContext&  theExecutionContext)
{
    XalanNode*      theResult = 0;

    MemoryManager&      theMemoryManager =
        theExecutionContext.getMemoryManager();

    XPath* const    theXPath = theXPathFactory.create();

    XPathConstructionContextDefault     theXPathConstructionContext;

    XPathGuard      theGuard(
                        theXPathFactory,
                        theXPath);

    XalanElement*               theNamespaceContext = 0;
    ElementPrefixResolverProxy  thePrefixResolver(theNamespaceContext, theXPathEnvSupport, theDOMSupport);
    NodeRefList                 theContextNodeList(theMemoryManager);

    const XObjectPtr    theXObject =
        ExecuteXPath(
            theXPathProcessor,
            theXPathConstructionContext,
            *theXPath,
            theContextNodeMatchPattern,
            theDocument,
            thePrefixResolver,
            theContextNodeList,
            theExecutionContext);

    try
    {
        assert(theXObject.null() == false);

        const NodeRefListBase&  theResultList =
                        theXObject->nodeset();

        if (theResultList.getLength() == 0)
        {
            thePrintWriter.print("FindContextNode: Unable to find context node using select \"");
            thePrintWriter.print(theContextNodeMatchPattern);
            thePrintWriter.println("\".");
            thePrintWriter.println("FindContextNode: No nodes matched the pattern.");
        }
        else if (theResultList.getLength() != 1)
        {
            thePrintWriter.print("FindContextNode: Unable to find context node using select \"");
            thePrintWriter.print(theContextNodeMatchPattern);
            thePrintWriter.println("\".");
            thePrintWriter.println("FindContextNode: More than one node matched the pattern.");
        }
        else
        {
            theResult = theResultList.item(0);
        }
    }
    catch(...)
    {
        thePrintWriter.print("FindContextNode: Error executing match pattern \"");
        thePrintWriter.print(theContextNodeMatchPattern);
        thePrintWriter.println("\".");
    }

    return theResult;
}



bool
TestAxisResult(
            XPathProcessor&         theXPathProcessor,
            XPathEnvSupport&        theXPathEnvSupport,
            DOMSupport&             theDOMSupport,
            XMLParserLiaison&       theLiaison,
            XPathFactory&           theXPathFactory,
            const XalanDOMString&   theXMLFileURL,
            const XalanDOMString&   theXSLFileURL,
            PrintWriter&            thePrintWriter,
            XPathExecutionContext&  theExecutionContext)
{
    bool                    fError = false;

    XalanDocument* const    theXMLDocument = ParseXML(theLiaison,
                                                      theXMLFileURL);

    MemoryManager&      theMemoryManager =
        theExecutionContext.getMemoryManager();

    if (theXMLDocument != 0)
    {
        XalanDOMString      theContextNodeMatchPattern(theMemoryManager);
        XalanDOMString      theXPathString(theMemoryManager);

        if (GetXSLInput(theLiaison,
                        theXSLFileURL,
                        theContextNodeMatchPattern,
                        theXPathString,
                        theMemoryManager) == true)
        {
            XalanNode* const    theContextNode =
                FindContextNode(theXPathProcessor,
                                theXPathEnvSupport,
                                theDOMSupport,
                                theXPathFactory,
                                theXMLDocument,
                                theContextNodeMatchPattern,
                                thePrintWriter,
                                theExecutionContext);

            if (theContextNode != 0)
            {
                XalanElement* const             theNamespaceContext = 0;
                ElementPrefixResolverProxy      thePrefixResolver(theNamespaceContext, theXPathEnvSupport, theDOMSupport);
                NodeRefList                     theContextNodeList(theMemoryManager);

                XPath* const    theXPath = theXPathFactory.create();

                XPathConstructionContextDefault     theXPathConstructionContext(theMemoryManager);

                XPathGuard      theGuard(theXPathFactory,
                                         theXPath);

                theXPathProcessor.initXPath(*theXPath,
                                            theXPathConstructionContext,
                                            theXPathString,
                                            thePrefixResolver);

                bool    fDump = false;

                if (fDump == true)
                {
                    thePrintWriter.println();
                    thePrintWriter.println();
                    theXPath->getExpression().dumpOpCodeMap(thePrintWriter);
                    thePrintWriter.println();
                    theXPath->getExpression().dumpTokenQueue(thePrintWriter);
                    thePrintWriter.println();
                    thePrintWriter.println();
                }

                const XObjectPtr theResult =
                    theXPath->execute(theContextNode, thePrefixResolver, theContextNodeList, theExecutionContext);

                try
                {
                    assert(theResult.null() == false);

                    const NodeRefListBase&  theResultList =
                        theResult->nodeset();

                    const NodeRefListBase::size_type    theLength = theResultList.getLength();

                    if (theLength == 0)
                    {
                        thePrintWriter.println("<out/>");
                    }
                    else
                    {
                        thePrintWriter.print("<out>");

                        for (NodeRefListBase::size_type i = 0; i < theLength; i++)
                        {
                            thePrintWriter.print(theResultList.item(i)->getNodeName());
                            thePrintWriter.print(" ");
                        }

                        thePrintWriter.println("</out>");
                    }
                }
                catch(...)
                {
                    thePrintWriter.print("Execution of XPath ");
                    thePrintWriter.print(theXPathString);
                    thePrintWriter.println(" failed!");
                }
            }
        }
    }

    theLiaison.reset();

    return fError;
}



bool
TestPredicateResult(
            XPathProcessor&         theXPathProcessor,
            XPathEnvSupport&        theXPathEnvSupport,
            DOMSupport&             theDOMSupport,
            XMLParserLiaison&       theLiaison,
            XPathFactory&           theXPathFactory,
            const XalanDOMString&   theXMLFileURL,
            const XalanDOMString&   theXSLFileURL,
            PrintWriter&            thePrintWriter,
            XPathExecutionContext&  theExecutionContext)
{
    bool                    fError = false;

    XalanDocument* const    theXMLDocument =
                ParseXML(theLiaison,
                         theXMLFileURL);

    MemoryManager&      theMemoryManager =
        theExecutionContext.getMemoryManager();

    if (theXMLDocument != 0)
    {
        XalanDOMString      theContextNodeMatchPattern(theMemoryManager);
        XalanDOMString      theXPathString(theMemoryManager);

        if (GetXSLInput(theLiaison,
                        theXSLFileURL,
                        theContextNodeMatchPattern,
                        theXPathString,
                        theMemoryManager) == true)
        {
            XalanNode* const    theContextNode =
                FindContextNode(theXPathProcessor,
                                theXPathEnvSupport,
                                theDOMSupport,
                                theXPathFactory,
                                theXMLDocument,
                                theContextNodeMatchPattern,
                                thePrintWriter,
                                theExecutionContext);

            if (theContextNode != 0)
            {
                XalanElement* const             theNamespaceContext = 0;
                ElementPrefixResolverProxy      thePrefixResolver(theNamespaceContext, theXPathEnvSupport, theDOMSupport);
                NodeRefList                     theContextNodeList(theMemoryManager);

                XPath* const    theXPath1 = theXPathFactory.create();

                XPathConstructionContextDefault     theXPathConstructionContext(theMemoryManager);

                XPathGuard  theGuard1(theXPathFactory,
                                      theXPath1);

                XalanDOMString  theResult(theMemoryManager);

                theXPathProcessor.initXPath(*theXPath1,
                                            theXPathConstructionContext,
                                            TranscodeFromLocalCodePage("following-sibling::*", theResult),
                                            thePrefixResolver);

                XPath* const    theXPath2 = theXPathFactory.create();

                XPathGuard  theGuard2(theXPathFactory,
                                      theXPath2);

                theXPathProcessor.initXPath(*theXPath2,
                                            theXPathConstructionContext,
                                            TranscodeFromLocalCodePage("descendant::*", theResult),
                                            thePrefixResolver);

                bool    fDump = false;

                if (fDump == true)
                {
                    thePrintWriter.println();
                    thePrintWriter.println();
                    theXPath1->getExpression().dumpOpCodeMap(thePrintWriter);
                    thePrintWriter.println();
                    theXPath1->getExpression().dumpTokenQueue(thePrintWriter);
                    thePrintWriter.println();
                    thePrintWriter.println();
                    theXPath2->getExpression().dumpOpCodeMap(thePrintWriter);
                    thePrintWriter.println();
                    theXPath2->getExpression().dumpTokenQueue(thePrintWriter);
                    thePrintWriter.println();
                    thePrintWriter.println();
                }

                XalanNode*  theContextNode = theXMLDocument->getFirstChild()->getFirstChild();

                for( ; theContextNode != 0; theContextNode = theContextNode->getNextSibling())
                {
                    if (theContextNode->getNodeType() != XalanNode::ELEMENT_NODE)
                    {
                        continue;
                    }

                    const XObjectPtr theResult1 =
                            theXPath1->execute(theExecutionContext);

                    try
                    {
                        assert(theResult1.null() == false);

                        const NodeRefListBase&  theResultList =
                                theResult1->nodeset();

                        const NodeRefListBase::size_type    theLength = theResultList.getLength();

                        thePrintWriter.print("theResult1->str() == \"");
                        thePrintWriter.print(theResult1->str(theExecutionContext));
                        thePrintWriter.print("\"");
                        thePrintWriter.println();

                        for (NodeRefListBase::size_type i = 0; i < theLength; i++)
                        {
                            thePrintWriter.print(theResultList.item(i)->getNodeName());
                            thePrintWriter.println();
                        }
                    }
                    catch(...)
                    {
                        thePrintWriter.print("Execution of XPath ");
                        thePrintWriter.print(theXPathString);
                        thePrintWriter.println(" failed!");
                    }

                    const XObjectPtr    theResult2 =
                            theXPath2->execute(theExecutionContext);

                    try
                    {
                        assert(theResult2.null() == false);

                        const NodeRefListBase&  theResultList =
                                theResult2->nodeset();

                        const NodeRefListBase::size_type    theLength = theResultList.getLength();

                        thePrintWriter.print("theResult2->str() == \"");
                        thePrintWriter.print(theResult2->str(theExecutionContext));
                        thePrintWriter.print("\"");
                        thePrintWriter.println();

                        for (NodeRefListBase::size_type i = 0; i < theLength; i++)
                        {
                            thePrintWriter.print(theResultList.item(i)->getNodeName());
                            thePrintWriter.println();
                        }
                    }
                    catch(...)
                    {
                        thePrintWriter.print("Execution of XPath ");
                        thePrintWriter.print(theXPathString);
                        thePrintWriter.println(" failed!");
                    }

                    if (theResult1->equals(*theResult2, theExecutionContext) == true)
                    {
                        thePrintWriter.print("theResult1 is equal to theResult2");
                        thePrintWriter.println();
                        thePrintWriter.print("theContextNode->getNodeName() == \"");
                        thePrintWriter.print(theContextNode->getNodeName());
                        thePrintWriter.print("\"  theContextNode->getNodeValue() == \"");
                        thePrintWriter.print(theContextNode->getNodeValue());
                        thePrintWriter.println("\"");
                    }
                }
            }
        }
    }

    return fError;
}



const char* const   theNumericTestInput[] =
{
    "9 + 7 - 6 * 42 + 36 div 3 + 100 - 2 * 6",
    "9 + 7 - 6 + 3 + 24 - 32 - 89 + 123 - 12256 + 89957",
    "0 + 0",
    "1 div 2",
    "1 * 0.25",
    "3 * 3 * 3",
    "9 div 3 div 3",
    "15.95 - 13.56",
    0
};



const double    theNumericTestExpectedOutput[] =
{
    -136.0,
    77740.0,
    0,
    0.5,
    0.25,
    27,
    1,
    15.95 - 13.56,
    0   // This is a dummy entry and should be last.
};



void
TestNumericResults(
            XPathFactory&           theXPathFactory,
            XPathProcessor&         theXPathProcessor,
            XPathEnvSupport&        theXPathEnvSupport,
            DOMSupport&             theDOMSupport,
            PrintWriter&            thePrintWriter,
            XPathExecutionContext&  theExecutionContext)
{
    assert(sizeof(theNumericTestInput) / sizeof(theNumericTestInput[0]) ==
            sizeof(theNumericTestExpectedOutput) / sizeof(theNumericTestExpectedOutput[0]));

    MemoryManager&      theMemoryManager =
        theExecutionContext.getMemoryManager();

    for(int i = 0; theNumericTestInput[i] != 0; i++)
    {
        try
        {
            XPath* const    theXPath = theXPathFactory.create();

            XPathConstructionContextDefault     theXPathConstructionContext(theMemoryManager);

            XPathGuard      theGuard(theXPathFactory,
                                     theXPath);

            XalanDOMString  theInputString(theMemoryManager);

            const ElementPrefixResolverProxy    thePrefixResolver(
                                                    0,
                                                    theXPathEnvSupport,
                                                    theDOMSupport);

            const NodeRefList   theDummyNodeList(theMemoryManager);

            TestNumericResult(
                theXPathProcessor,
                *theXPath,
                theXPathConstructionContext,
                TranscodeFromLocalCodePage(theNumericTestInput[i], theInputString),
                thePrintWriter,
                theNumericTestExpectedOutput[i],
                0,
                thePrefixResolver,
                theDummyNodeList,
                theExecutionContext);
        }
        catch(...)
        {
            thePrintWriter.print("Exception caught evaluating XPath \"");
            thePrintWriter.print(theNumericTestInput[i]);
            thePrintWriter.println();
        }
    }
}



const char* const   theStringTestInput[] =
{
    "string(0)",
    "string(1.5)",
    "string(-1.5)",
    "string(0.5)",
    "string(-0.5)",
    "string(9)",
    "string(-9)",
    "string(\"9\")",
    "string(0.25)",

    "concat(\"foo\", \"bar\")",
    "concat(\"foo\", \"bar\", \"sky\")",

    "contains(\"foobar\", \"oba\")",
    "contains(\"LotusXSL4C is great\", \"boag\")",

    "starts-with(\"foobar\", \"foo\")",
    "starts-with(\"LotusXSL4C is great\", \"boag\")",

    "substring-after(\"1999/04/01\", \"/\")",
    "substring-after(\"1999/04/01\", \"19\")",

    "substring-before(\"1999/04/01\", \"/\")",
    "substring-before(\"1999/04/01\", \"01\")",

    "substring(\"12345\", 1.5, 2.6)",
    "substring(\"12345\", 0, 3)",
    "substring(\"12345\", 5, 29)",
    "substring(\"12345\", -1, 2)",
    "substring(\"12345\", -2, -1)",
    "substring(\"12345\", -2)",

    "normalize-space(\"   aa   a  \")",

    "translate(\"---aaa--\", \"abc-\", \"ABC\")",

    "5 = 0",
    "5 = 5",
    0
};


const char* const   theStringTestExpectedOutput[] =
{
    "0",
    "1.5",
    "-1.5",
    "0.5",
    "-0.5",
    "9",
    "-9",
    "9",
    "0.25",

    "foobar",
    "foobarsky",

    "true",
    "false",

    "true",
    "false",

    "04/01",
    "99/04/01",

    "1999",
    "1999/04/",

    "234",
    "12",
    "5",
    "",
    "",
    "12345",

    "aa a",

    "AAA",

    "false",
    "true",
    0
};



void
TestStringResults(
            XPathFactory&           theXPathFactory,
            XPathProcessor&         theXPathProcessor,
            XPathEnvSupport&        theXPathEnvSupport,
            DOMSupport&             theDOMSupport,
            PrintWriter&            thePrintWriter,
            XPathExecutionContext&  theExecutionContext)
{
    assert(sizeof(theStringTestInput) == sizeof(theStringTestExpectedOutput));

    MemoryManager&      theMemoryManager =
        theExecutionContext.getMemoryManager();

    for(int i = 0; theStringTestInput[i] != 0; i++)
    {
        try
        {
            XPath* const    theXPath = theXPathFactory.create();

            XPathConstructionContextDefault     theXPathConstructionContext(theMemoryManager);

            const XPathGuard    theGuard(
                                    theXPathFactory,
                                    theXPath);

            XalanDOMString  theInputString(theMemoryManager);
            XalanDOMString  theResult(theMemoryManager);

            const ElementPrefixResolverProxy    thePrefixResolver(
                                                    0,
                                                    theXPathEnvSupport,
                                                    theDOMSupport);

            const NodeRefList   theDummyNodeList(theMemoryManager);

            TestStringResult(
                theXPathProcessor,
                *theXPath,
                theXPathConstructionContext,
                TranscodeFromLocalCodePage(theStringTestInput[i], theInputString),
                thePrintWriter,
                TranscodeFromLocalCodePage(theStringTestExpectedOutput[i], theResult),
                0,
                thePrefixResolver,
                theDummyNodeList,
                theExecutionContext);
        }
        catch(...)
        {
            thePrintWriter.print("Exception caught evaluating XPath \"");
            thePrintWriter.print(theStringTestInput[i]);
            thePrintWriter.println();
        }
    }
}



const char* const   theBooleanTestInput[] =
{
    "0 < 2 < 3",
    "0 < 2 > 3",
    "1 < 2 < 3",

    "0 div 0 != 0 div 0",
    "0 div 0 = 0 div 0",
    "1 < 0 div 0",
    "1 <= 0 div 0",
    "2 > 0 div 0",
    "2 >= 0 div 0",

    "1 < 2",
    "1500 < 657",
    "1465787676 < 5 div 0",
    "4657 div 0 < 4563745658",

    "1000 <= 256000",
    "1000 <= 1000",
    "200564 <= 1999",
    "5768594 <= 56 div 0",
    "564783 div 0 <= 6758494857",

    "2015 > 100",
    "56478 > 240000",
    "4657 div 0 > 57683946",
    "573068574 > 5 div 0",

    "46000 >= 1500",
    "56983 >= 56983",
    "460983 >= 1500000",
    "67594876 div 0 >= 576849",

    "1465787676 >= 5 div 0",
    "-1465787676 >= -5 div 0",
    "-1679 < -87 div 0",

    "1 = 2 = 0",
    "1 = 2 != 0",

    0
};



const bool  theBooleanTestExpectedOutput[] =
{
    true,
    false,
    true,

    true,
    false,
    false,
    false,
    false,
    false,

    true,
    false,
    true,
    false,

    true,
    true,
    false,
    true,
    false,

    true,
    false,
    true,
    false,

    true,
    true,
    false,
    true,

    false,
    true,
    false,

    true,
    false,

    0
};



void
TestBooleanResults(
            XPathFactory&           theXPathFactory,
            XPathProcessor&         theXPathProcessor,
            XPathEnvSupport&        theXPathEnvSupport,
            DOMSupport&             theDOMSupport,
            PrintWriter&            thePrintWriter,
            XPathExecutionContext&  theExecutionContext)
{
    assert(sizeof(theBooleanTestInput) / sizeof(theBooleanTestInput[0]) ==
                sizeof(theBooleanTestExpectedOutput) / sizeof(theBooleanTestExpectedOutput[0]));

    MemoryManager&      theMemoryManager =
        theExecutionContext.getMemoryManager();

    for(int i = 0; theBooleanTestInput[i] != 0; i++)
    {
        try
        {
            XPath* const    theXPath = theXPathFactory.create();

            XPathConstructionContextDefault     theXPathConstructionContext(theMemoryManager);

            XPathGuard  theGuard(theXPathFactory,
                                 theXPath);

            XalanDOMString  theInputString(theMemoryManager);

            const ElementPrefixResolverProxy    thePrefixResolver(
                                                    0,
                                                    theXPathEnvSupport,
                                                    theDOMSupport);

            const NodeRefList   theDummyNodeList(theMemoryManager);

            TestBooleanResult(
                theXPathProcessor,
                *theXPath,
                theXPathConstructionContext,
                TranscodeFromLocalCodePage(theBooleanTestInput[i], theInputString),
                thePrintWriter,
                theBooleanTestExpectedOutput[i],
                0,
                thePrefixResolver,
                theDummyNodeList,
                theExecutionContext);
        }
        catch(...)
        {
            thePrintWriter.print("Exception caught evaluating XPath \"");
            thePrintWriter.print(theStringTestInput[i]);
            thePrintWriter.println();
        }
    }
}



XalanDOMString&
GetXSLFileName(
            const XalanDOMString&   theXMLFileName,
            XalanDOMString&         theResult)
{
    const XalanDOMString::size_type     thePeriodIndex =
        lastIndexOf(theXMLFileName, XalanUnicode::charFullStop);

    if (thePeriodIndex != XalanDOMString::npos)
    {
         substring(
             theXMLFileName,
             theResult,
             0,
             thePeriodIndex + 1);

        theResult.append("xsl");
    }

    return theResult;
}



void
TestAxes(
            XPathFactory&           theXPathFactory,
            XPathProcessor&         theXPathProcessor,
            XPathEnvSupport&        theXPathEnvSupport,
            DOMSupport&             theDOMSupport,
            XMLParserLiaison&       theLiaison,
            const XalanDOMString&   theDirectory,
            PrintWriter&            thePrintWriter,
            XPathExecutionContext&  theExecutionContext)
{
    MemoryManager& theMemoryManager = XalanMemMgrs::getDefaultXercesMemMgr();

    const XalanDOMString    theProtocol("file://", theMemoryManager);

    XalanDOMString  theBaseURL (theProtocol , theMemoryManager);
    theBaseURL += theDirectory;

    XalanDOMString thetmp(theDirectory, theMemoryManager);
#if defined(XALAN_WINDOWS)
    thetmp.append("\\*");
#else
    thetmp.append("/*");
#endif
    const XalanDOMString    theSearchSpecification(thetmp, theMemoryManager);

    const XalanDOMString    theXMLSuffix(".xml", theMemoryManager);

    typedef XalanVector<XalanDOMString>     FileNameVectorType;

    DirectoryEnumeratorFunctor<FileNameVectorType, XalanDOMString>  theEnumerator(theMemoryManager);

    FileNameVectorType  theFiles(theMemoryManager);

    theEnumerator(theSearchSpecification,
                  theFiles);

    thePrintWriter.println();

    for(FileNameVectorType::size_type   i = 0; i < theFiles.size(); i++)
    {
        if (endsWith(theFiles[i], theXMLSuffix) == true)
        {
            try
            {
                // Create a fully qualified URL specification...
                XalanDOMString  theXMLFileName (theBaseURL, theMemoryManager);
                theXMLFileName += theFiles[i];

                // Get the name of the corresponding XSL file...
                XalanDOMString  theXSLFileName (theMemoryManager);

                GetXSLFileName(theXMLFileName, theXSLFileName);

                XPathGuard  theXPath(theXPathFactory,
                                     theXPathFactory.create());

                TestAxisResult(theXPathProcessor,
                               theXPathEnvSupport,
                               theDOMSupport,
                               theLiaison,
                               theXPathFactory,
                               theXMLFileName,
                               theXSLFileName,
                               thePrintWriter,
                               theExecutionContext);
            }
            catch(...)
            {
                thePrintWriter.print("Exception caught evaluating XPath!");
                thePrintWriter.println();
            }
        }
    }
}



void
RunTests(
            XPathFactory&           theXPathFactory,
            XPathProcessor&         theXPathProcessor,
            XPathEnvSupport&        theXPathEnvSupport,
            DOMSupport&             theDOMSupport,
            XMLParserLiaison&       theLiaison,
            PrintWriter&            thePrintWriter,
            XPathExecutionContext&  theExecutionContext)
{
    TestNumericResults(theXPathFactory,
                       theXPathProcessor,
                       theXPathEnvSupport,
                       theDOMSupport,
                       thePrintWriter,
                       theExecutionContext);

    TestStringResults(theXPathFactory,
                      theXPathProcessor,
                      theXPathEnvSupport,
                      theDOMSupport,
                      thePrintWriter,
                      theExecutionContext);

    TestBooleanResults(theXPathFactory,
                       theXPathProcessor,
                       theXPathEnvSupport,
                       theDOMSupport,
                       thePrintWriter,
                       theExecutionContext);

    const XalanDOMString    theDir(
                                "/xsl-test/conf/axes/",
                                XalanMemMgrs::getDefaultXercesMemMgr());

    TestAxes(theXPathFactory,
             theXPathProcessor,
             theXPathEnvSupport,
             theDOMSupport,
             theLiaison,
             theDir,
             thePrintWriter,
             theExecutionContext);
}



int
main(
            int     /* argc */,
            char*   /* argv */ [])
{
    XALAN_USING_STD(cout)

#if !defined(NDEBUG) && defined(_MSC_VER)
    _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

   _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
   _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif

    XMLPlatformUtils::Initialize();

    XalanTransformer::initialize();

    {
        MemoryManager& theMemoryManager = XalanMemMgrs::getDefaultXercesMemMgr();

        XMLSupportInit                  theXMLSupportInit;
        XPathInit                       theXPathInit( theMemoryManager );

        XPathEnvSupportDefault          theXPathEnvSupport( theMemoryManager );
        XalanSourceTreeDOMSupport       theDOMSupport;
        XObjectFactoryDefault           theXObjectFactory( theMemoryManager );
        XPathFactoryDefault             theXPathFactory( theMemoryManager );
        XPathProcessorImpl              theXPathProcessor( theMemoryManager );

        XPathExecutionContextDefault    theExecutionContext(theXPathEnvSupport,
                                                            theDOMSupport,
                                                            theXObjectFactory);

        XalanStdOutputStream            theStdOut(cout, theMemoryManager);
        XalanOutputStreamPrintWriter    thePrintWriter(theStdOut);
        XalanSourceTreeParserLiaison    theLiaison(theDOMSupport, theMemoryManager);

        RunTests(theXPathFactory,
                 theXPathProcessor,
                 theXPathEnvSupport,
                 theDOMSupport,
                 theLiaison,
                 thePrintWriter,
                 theExecutionContext);
    }
    XalanTransformer::terminate();

    XMLPlatformUtils::Terminate();

    return 0;
}
