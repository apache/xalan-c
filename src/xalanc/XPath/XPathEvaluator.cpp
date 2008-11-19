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

#include "XPathEvaluator.hpp"



#include <xalanc/DOMSupport/DOMSupportDefault.hpp>



#include "ElementPrefixResolverProxy.hpp"
#include "XObjectFactoryDefault.hpp"
#include "XPathEnvSupportDefault.hpp"
#include "XPathConstructionContextDefault.hpp"
#include "XPathExecutionContextDefault.hpp"
#include "XPathFactoryDefault.hpp"
#include "XPathProcessorImpl.hpp"
#include "XPathInit.hpp"



XALAN_CPP_NAMESPACE_BEGIN



static XPathInit*       s_xpathInit = 0;

static MemoryManager*   s_memoryManager = 0;

void
XPathEvaluator::initialize(MemoryManager&   theManager)
{
    s_xpathInit = XPathInit::create(theManager);

    s_memoryManager = &theManager;
}



void
XPathEvaluator::terminate()
{
    XalanDestroy(
        *s_memoryManager,
        *s_xpathInit);

    s_xpathInit = 0;

    s_memoryManager = 0;
}



XPathEvaluator::XPathEvaluator(MemoryManager& theManager) :
    m_xobjectFactory(theManager, XObjectFactoryDefault::create(theManager)),
    m_xpathFactory(theManager, XPathFactoryDefault::create(theManager)),
    m_constructionContext(theManager, XPathConstructionContextDefault::create(theManager)),
    m_executionContext(theManager, XPathExecutionContextDefault::create(theManager)),
    m_memoryManager(theManager)

{
}



XPathEvaluator::~XPathEvaluator()
{
    // Just in case these are still set, reset them
    // to 0.
    m_executionContext->setXPathEnvSupport(0);

    m_executionContext->setXObjectFactory(0);

    m_executionContext->setDOMSupport(0);
}



XalanNode*
XPathEvaluator::selectSingleNode(
            DOMSupport&             domSupport,
            XalanNode*              contextNode,
            const XalanDOMChar*     xpathString,
            const XalanElement*     namespaceNode)
{
    const XObjectPtr    theResult(
        evaluate(
            domSupport,
            contextNode,
            xpathString,
            namespaceNode));

    const NodeRefListBase&  theNodeList = theResult->nodeset();

    return theNodeList.getLength() == 0 ? 0 : theNodeList.item(0);
}



XalanNode*
XPathEvaluator::selectSingleNode(
            DOMSupport&             domSupport,
            XalanNode*              contextNode,
            const XalanDOMChar*     xpathString,
            const PrefixResolver&   prefixResolver)
{
    const XObjectPtr    theResult(
        evaluate(
            domSupport,
            contextNode,
            xpathString,
            prefixResolver));

    const NodeRefListBase&  theNodeList = theResult->nodeset();

    return theNodeList.getLength() == 0 ? 0 : theNodeList.item(0);
}



XalanNode*
XPathEvaluator::selectSingleNode(
            DOMSupport&             domSupport,
            XalanNode*              contextNode,
            const XPath&            xpath,
            const XalanElement*     namespaceNode)
{
    const XObjectPtr    theResult(
        evaluate(
            domSupport,
            contextNode,
            xpath,
            namespaceNode));

    const NodeRefListBase&  theNodeList = theResult->nodeset();

    return theNodeList.getLength() == 0 ? 0 : theNodeList.item(0);
}



XalanNode*
XPathEvaluator::selectSingleNode(
            DOMSupport&             domSupport,
            XalanNode*              contextNode,
            const XPath&            xpath,
            const PrefixResolver&   prefixResolver)
{
    const XObjectPtr    theResult(
        evaluate(
            domSupport,
            contextNode,
            xpath,
            prefixResolver));

    const NodeRefListBase&  theNodeList = theResult->nodeset();

    return theNodeList.getLength() == 0 ? 0 : theNodeList.item(0);
}



NodeRefList&
XPathEvaluator::selectNodeList(
            NodeRefList&            result,
            DOMSupport&             domSupport,
            XalanNode*              contextNode,
            const XalanDOMChar*     xpathString,
            const XalanElement*     namespaceNode)
{
    const XObjectPtr    theResult(
        evaluate(
            domSupport,
            contextNode,
            xpathString,
            namespaceNode));

    result = theResult->nodeset();

    return result;
}



NodeRefList&
XPathEvaluator::selectNodeList(
            NodeRefList&            result,
            DOMSupport&             domSupport,
            XalanNode*              contextNode,
            const XalanDOMChar*     xpathString,
            const PrefixResolver&   prefixResolver)
{
    const XObjectPtr    theResult(
        evaluate(
            domSupport,
            contextNode,
            xpathString,
            prefixResolver));

    result = theResult->nodeset();

    return result;
}



NodeRefList&
XPathEvaluator::selectNodeList(
            NodeRefList&            result,
            DOMSupport&             domSupport,
            XalanNode*              contextNode,
            const XPath&            xpath,
            const XalanElement*     namespaceNode)
{
    const XObjectPtr    theResult(
        evaluate(
            domSupport,
            contextNode,
            xpath,
            namespaceNode));

    result = theResult->nodeset();

    return result;
}



NodeRefList&
XPathEvaluator::selectNodeList(
            NodeRefList&            result,
            DOMSupport&             domSupport,
            XalanNode*              contextNode,
            const XPath&            xpath,
            const PrefixResolver&   prefixResolver)
{
    const XObjectPtr    theResult(
        evaluate(
            domSupport,
            contextNode,
            xpath,
            prefixResolver));

    result = theResult->nodeset();

    return result;
}



XObjectPtr
XPathEvaluator::evaluate(
            DOMSupport&             domSupport,
            XalanNode*              contextNode,
            const XalanDOMChar*     xpathString,
            const XalanElement*     namespaceNode)
{
    XPathEnvSupportDefault  theEnvSupportDefault(m_memoryManager);

    const ElementPrefixResolverProxy    thePrefixResolver(
                    namespaceNode,
                    theEnvSupportDefault,
                    domSupport,
                    m_memoryManager);

    return evaluate(
            domSupport,
            contextNode,
            xpathString,
            thePrefixResolver,
            theEnvSupportDefault);
 
}



XObjectPtr
XPathEvaluator::evaluate(
            DOMSupport&             domSupport,
            XalanNode*              contextNode,
            const XPath&            xpath,
            const XalanElement*     namespaceNode)
{
    XPathEnvSupportDefault  theEnvSupportDefault(m_memoryManager);

    const ElementPrefixResolverProxy    thePrefixResolver(
                    namespaceNode,
                    theEnvSupportDefault,
                    domSupport,
                    m_memoryManager);

    return evaluate(
                domSupport,
                contextNode,
                xpath,
                thePrefixResolver,
                theEnvSupportDefault);
}



XObjectPtr
XPathEvaluator::evaluate(
            DOMSupport&             domSupport,
            XalanNode*              contextNode,
            const XalanDOMChar*     xpathString,
            const PrefixResolver&   prefixResolver)
{
    XPathEnvSupportDefault  theEnvSupportDefault(m_memoryManager);

    return  evaluate(
            domSupport,
            contextNode,
            xpathString,
            prefixResolver,
            theEnvSupportDefault);
}



XObjectPtr
XPathEvaluator::evaluate(
            DOMSupport&             domSupport,
            XalanNode*              contextNode,
            const XPath&            xpath,
            const PrefixResolver&   prefixResolver)
{
    XPathEnvSupportDefault  theEnvSupportDefault(m_memoryManager);

    return evaluate(
                domSupport,
                contextNode,
                xpath,
                prefixResolver,
                theEnvSupportDefault);
}



XPath*
XPathEvaluator::createXPath(const XalanDOMChar*     xpathString)
{
    DOMSupportDefault       theDOMSupport(m_memoryManager);

    XPathEnvSupportDefault  theEnvSupportDefault(m_memoryManager);

    const ElementPrefixResolverProxy    thePrefixResolver(
                    0,
                    theEnvSupportDefault,
                    theDOMSupport,
                    m_memoryManager);

    return createXPath(xpathString, thePrefixResolver);
}



XPath*
XPathEvaluator::createXPath(
            const XalanDOMChar*     xpathString,
            DOMSupport&             domSupport,
            const XalanElement*     namespaceNode)
{
    XPathEnvSupportDefault  theEnvSupportDefault(m_memoryManager);

    const ElementPrefixResolverProxy    thePrefixResolver(
                    namespaceNode,
                    theEnvSupportDefault,
                    domSupport,
                    m_memoryManager);

    return createXPath(xpathString, thePrefixResolver);
}



typedef XPathConstructionContext::GetCachedString   GetCachedString;



XPath*
XPathEvaluator::createXPath(
            const XalanDOMChar*     xpathString,
            const PrefixResolver&   prefixResolver)
{
    XPath* const    theXPath = m_xpathFactory->create();
    assert(theXPath != 0);

    XPathProcessorImpl      theProcessor(m_memoryManager);

    const GetCachedString   theGuard(*m_constructionContext.get());

    XalanDOMString&     theTempString = theGuard.get();

    theTempString = xpathString;

    theProcessor.initXPath(
            *theXPath,
            *m_constructionContext.get(),
            theTempString,
            prefixResolver);

    return theXPath;
}



bool
XPathEvaluator::destroyXPath(XPath*     theXPath)
{
    assert(theXPath != 0);

    const bool  theResult = m_xpathFactory->returnObject(theXPath);

    if (m_xpathFactory->getInstanceCount() == 0)
    {
        m_constructionContext->reset();
    }

    return theResult;
}



XObjectPtr
XPathEvaluator::evaluate(
            DOMSupport&             domSupport,
            XalanNode*              contextNode,
            const XalanDOMChar*     xpathString,
            const PrefixResolver&   prefixResolver,
            XPathEnvSupport&        envSupport)
{
    assert(contextNode != 0);
    assert(xpathString != 0);

    // Create an XPath, and an XPathProcessorImpl to process
    // the XPath.
    XPath               theXPath(m_memoryManager);

    XPathProcessorImpl  theProcessor(m_memoryManager);

    const GetCachedString   theGuard(*m_constructionContext.get());

    XalanDOMString&     theTempString = theGuard.get();

    theTempString = xpathString;

    theProcessor.initXPath(
            theXPath,
            *m_constructionContext.get(),
            theTempString,
            prefixResolver);

    return evaluate(
                domSupport,
                contextNode,
                theXPath,
                prefixResolver,
                envSupport);
}



XObjectPtr
XPathEvaluator::evaluate(
            DOMSupport&             domSupport,
            XalanNode*              contextNode,
            const XPath&            xpath,
            const PrefixResolver&   prefixResolver,
            XPathEnvSupport&        envSupport)
{
    // Reset these, in case we've been here before...
    m_executionContext->reset();
    m_xobjectFactory->reset();

    // Set up the connections between the execution context and
    // the provided support objects...
    m_executionContext->setXPathEnvSupport(&envSupport);

    m_executionContext->setXObjectFactory(m_xobjectFactory.get());

    m_executionContext->setDOMSupport(&domSupport);

    // OK, evaluate the expression...
    const XObjectPtr    theResult(
        xpath.execute(
            contextNode,
            prefixResolver,
            *m_executionContext.get()));

    // Break the connectons we set...
    m_executionContext->setXPathEnvSupport(0);

    m_executionContext->setXObjectFactory(0);

    m_executionContext->setDOMSupport(0);

    return theResult;
}



XPathExecutionContext&
XPathEvaluator::getExecutionContext()
{
    assert(m_executionContext.get() != 0);

    return *m_executionContext.get();
}



XALAN_CPP_NAMESPACE_END
