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



static XPathInit*	theXPathInit = 0;



void
XPathEvaluator::initialize()
{
	theXPathInit = new XPathInit;
}



void
XPathEvaluator::terminate()
{
	delete theXPathInit;

	theXPathInit = 0;
}



XPathEvaluator::XPathEvaluator() :
	m_xobjectFactory(new XObjectFactoryDefault),
	m_xpathFactory(new XPathFactoryDefault),
	m_constructionContext(new XPathConstructionContextDefault),
	m_executionContext(new XPathExecutionContextDefault)

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
			DOMSupport&				domSupport,
			XalanNode*				contextNode,
			const XalanDOMChar*		xpathString,
			const XalanElement*		namespaceNode)
{
	const XObjectPtr	theResult(
		evaluate(
			domSupport,
			contextNode,
			xpathString,
			namespaceNode));

	const NodeRefListBase&	theNodeList = theResult->nodeset();

	return theNodeList.getLength() == 0 ? 0 : theNodeList.item(0);
}



XalanNode*
XPathEvaluator::selectSingleNode(
			DOMSupport&				domSupport,
			XalanNode*				contextNode,
			const XalanDOMChar*		xpathString,
			const PrefixResolver&	prefixResolver)
{
	const XObjectPtr	theResult(
		evaluate(
			domSupport,
			contextNode,
			xpathString,
			prefixResolver));

	const NodeRefListBase&	theNodeList = theResult->nodeset();

	return theNodeList.getLength() == 0 ? 0 : theNodeList.item(0);
}



XalanNode*
XPathEvaluator::selectSingleNode(
			DOMSupport&				domSupport,
			XalanNode*				contextNode,
			const XPath&			xpath,
			const XalanElement*		namespaceNode)
{
	const XObjectPtr	theResult(
		evaluate(
			domSupport,
			contextNode,
			xpath,
			namespaceNode));

	const NodeRefListBase&	theNodeList = theResult->nodeset();

	return theNodeList.getLength() == 0 ? 0 : theNodeList.item(0);
}



XalanNode*
XPathEvaluator::selectSingleNode(
			DOMSupport&				domSupport,
			XalanNode*				contextNode,
			const XPath&			xpath,
			const PrefixResolver&	prefixResolver)
{
	const XObjectPtr	theResult(
		evaluate(
			domSupport,
			contextNode,
			xpath,
			prefixResolver));

	const NodeRefListBase&	theNodeList = theResult->nodeset();

	return theNodeList.getLength() == 0 ? 0 : theNodeList.item(0);
}



NodeRefList
XPathEvaluator::selectNodeList(
			DOMSupport&				domSupport,
			XalanNode*				contextNode,
			const XalanDOMChar*		xpathString,
			const XalanElement*		namespaceNode)
{
	const XObjectPtr	theResult(
		evaluate(
			domSupport,
			contextNode,
			xpathString,
			namespaceNode));

	return NodeRefList(theResult->nodeset());
}



NodeRefList
XPathEvaluator::selectNodeList(
			DOMSupport&				domSupport,
			XalanNode*				contextNode,
			const XalanDOMChar*		xpathString,
			const PrefixResolver&	prefixResolver)
{
	const XObjectPtr	theResult(
		evaluate(
			domSupport,
			contextNode,
			xpathString,
			prefixResolver));

	return NodeRefList(theResult->nodeset());
}



NodeRefList
XPathEvaluator::selectNodeList(
			DOMSupport&				domSupport,
			XalanNode*				contextNode,
			const XPath&			xpath,
			const XalanElement*		namespaceNode)
{
	const XObjectPtr	theResult(
		evaluate(
			domSupport,
			contextNode,
			xpath,
			namespaceNode));

	return NodeRefList(theResult->nodeset());
}



NodeRefList
XPathEvaluator::selectNodeList(
			DOMSupport&				domSupport,
			XalanNode*				contextNode,
			const XPath&			xpath,
			const PrefixResolver&	prefixResolver)
{
	const XObjectPtr	theResult(
		evaluate(
			domSupport,
			contextNode,
			xpath,
			prefixResolver));

	return NodeRefList(theResult->nodeset());
}



XObjectPtr
XPathEvaluator::evaluate(
			DOMSupport&				domSupport,
			XalanNode*				contextNode,
			const XalanDOMChar*		xpathString,
			const XalanElement*		namespaceNode)
{
	XPathEnvSupportDefault	theEnvSupportDefault;

	return	evaluate(
			domSupport,
			contextNode,
			xpathString,
			ElementPrefixResolverProxy(namespaceNode, theEnvSupportDefault, domSupport),
			theEnvSupportDefault);
}



XObjectPtr
XPathEvaluator::evaluate(
			DOMSupport&				domSupport,
			XalanNode*				contextNode,
			const XPath&			xpath,
			const XalanElement*		namespaceNode)
{
	XPathEnvSupportDefault	theEnvSupportDefault;

	return evaluate(
				domSupport,
				contextNode,
				xpath,
				ElementPrefixResolverProxy(namespaceNode, theEnvSupportDefault, domSupport),
				theEnvSupportDefault);
}



XObjectPtr
XPathEvaluator::evaluate(
			DOMSupport&				domSupport,
			XalanNode*				contextNode,
			const XalanDOMChar*		xpathString,
			const PrefixResolver&	prefixResolver)
{
	XPathEnvSupportDefault	theEnvSupportDefault;

	return	evaluate(
			domSupport,
			contextNode,
			xpathString,
			prefixResolver,
			theEnvSupportDefault);
}



XObjectPtr
XPathEvaluator::evaluate(
			DOMSupport&				domSupport,
			XalanNode*				contextNode,
			const XPath&			xpath,
			const PrefixResolver&	prefixResolver)
{
	XPathEnvSupportDefault	theEnvSupportDefault;

	return evaluate(
				domSupport,
				contextNode,
				xpath,
				prefixResolver,
				theEnvSupportDefault);
}



XPath*
XPathEvaluator::createXPath(const XalanDOMChar*		xpathString)
{
	DOMSupportDefault		theDOMSupport;
	XPathEnvSupportDefault	theEnvSupportDefault;

	return createXPath(xpathString, ElementPrefixResolverProxy(0, theEnvSupportDefault, theDOMSupport));
}



XPath*
XPathEvaluator::createXPath(
			const XalanDOMChar*		xpathString,
			DOMSupport&				domSupport,
			const XalanElement*		namespaceNode)
{
	XPathEnvSupportDefault	theEnvSupportDefault;

	return createXPath(xpathString, ElementPrefixResolverProxy(namespaceNode, theEnvSupportDefault, domSupport));
}



XPath*
XPathEvaluator::createXPath(
			const XalanDOMChar*		xpathString,
			const PrefixResolver&	prefixResolver)
{
	XPath* const	theXPath = m_xpathFactory->create();
	assert(theXPath != 0);

	XPathProcessorImpl		theProcessor;

    theProcessor.initXPath(
			*theXPath,
			*m_constructionContext.get(),
			XalanDOMString(xpathString),
			prefixResolver);

	return theXPath;
}



bool
XPathEvaluator::destroyXPath(XPath*		theXPath)
{
	assert(theXPath != 0);

	const bool	theResult = m_xpathFactory->returnObject(theXPath);

	if (m_xpathFactory->getInstanceCount() == 0)
	{
		m_constructionContext->reset();
	}

	return theResult;
}



XObjectPtr
XPathEvaluator::evaluate(
			DOMSupport&				domSupport,
			XalanNode*				contextNode,
			const XalanDOMChar*		xpathString,
			const PrefixResolver&	prefixResolver,
			XPathEnvSupport&		envSupport)
{
	assert(contextNode != 0);
	assert(xpathString != 0);

	// Create an XPath, and an XPathProcessorImpl to process
	// the XPath.
	XPath					theXPath;

	XPathProcessorImpl		theProcessor;

    theProcessor.initXPath(
			theXPath,
			*m_constructionContext.get(),
			XalanDOMString(xpathString),
			prefixResolver);

	return evaluate(domSupport, contextNode, theXPath, prefixResolver, envSupport);
}



XObjectPtr
XPathEvaluator::evaluate(
			DOMSupport&				domSupport,
			XalanNode*				contextNode,
			const XPath&			xpath,
			const PrefixResolver&	prefixResolver,
			XPathEnvSupport&		envSupport)
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
	const XObjectPtr	theResult(
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



XALAN_CPP_NAMESPACE_END
