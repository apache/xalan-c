/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2001 The Apache Software Foundation.  All rights 
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

#include "XPathEvaluator.hpp"



#include <XPath/ElementPrefixResolverProxy.hpp>
#include <XPath/XObjectFactoryDefault.hpp>
#include <XPath/XPathEnvSupportDefault.hpp>
#include <XPath/XPathExecutionContextDefault.hpp>
#include <XPath/XPathFactoryDefault.hpp>
#include <XPath/XPathProcessorImpl.hpp>
#include <XPath/XPathInit.hpp>



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
XPathEvaluator::createXPath(
			const XalanDOMChar*		xpathString,
			const PrefixResolver&	prefixResolver)
{
	XPath* const	theXPath = m_xpathFactory->create();
	assert(theXPath != 0);

	XPathProcessorImpl		theProcessor;

    theProcessor.initXPath(
			*theXPath,
			XalanDOMString(xpathString),
			prefixResolver);

	return theXPath;
}



bool
XPathEvaluator::destroyXPath(XPath*		theXPath)
{
	assert(theXPath != 0);

	return m_xpathFactory->returnObject(theXPath);
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
