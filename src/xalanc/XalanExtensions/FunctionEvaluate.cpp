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
#include "FunctionEvaluate.hpp"



#include <xalanc/PlatformSupport/PrefixResolver.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include <xalanc/XalanDOM/XalanElement.hpp>



#include <xalanc/XPath/ElementPrefixResolverProxy.hpp>
#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/XPath.hpp>
#include <xalanc/XPath/XPathConstructionContextDefault.hpp>
#include <xalanc/XPath/XPathProcessorImpl.hpp>



XALAN_CPP_NAMESPACE_BEGIN



FunctionEvaluate::FunctionEvaluate()
{
}



FunctionEvaluate::~FunctionEvaluate()
{
}



inline XObjectPtr
doExecute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XalanDOMString&			expression,
			const PrefixResolver&			resolver,
			const Function::LocatorType*	locator)
{
	// $$$ ToDo: Consider moving all of this into a member function of
	// XPathExecutionContext.
	XPathProcessorImpl					theProcessor;

	XPathConstructionContextDefault		theConstructionContext;

	XPath								theXPath(locator);

	theProcessor.initXPath(
			theXPath,
			theConstructionContext,
			expression,
			resolver,
			locator);

	return theXPath.execute(context, resolver, executionContext);
}



inline XObjectPtr
doExecute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XalanDOMString&			expression,
			const XalanNode*				resolver,
			const Function::LocatorType*	locator)
{
	assert(resolver == 0 || resolver->getNodeType() == XalanNode::ELEMENT_NODE);

#if defined(XALAN_OLD_STYLE_CASTS)
	ElementPrefixResolverProxy	theProxy((const XalanElement*)resolver);
#else
	ElementPrefixResolverProxy	theProxy(static_cast<const XalanElement*>(resolver));
#endif

	return doExecute(executionContext, context, expression, theProxy, locator);
}



XObjectPtr
FunctionEvaluate::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const LocatorType*				locator) const
{
	if (args.size() != 1)
	{
		executionContext.error(getError(), context, locator);
	}

	assert(args[0].null() == false);

	const XalanDOMString&	theExpression = args[0]->str();

	const PrefixResolver* const	theResolver =
		executionContext.getPrefixResolver();

	if (theResolver != 0)
	{
		return doExecute(executionContext, context, theExpression, *theResolver, locator);
	}
	else
	{
		const XalanNode*	resolverNode = context;

		if (resolverNode->getNodeType() != XalanNode::ELEMENT_NODE)
		{
			resolverNode = DOMServices::getParentOfNode(*resolverNode);

			if (context->getNodeType() != XalanNode::ELEMENT_NODE)
			{
				executionContext.warn(
					XalanMessageLoader::getMessage(XalanMessages::NoPrefixResolverAvailable),
					context,
					locator);

				resolverNode = 0;
			}
		}

		return doExecute(executionContext, context, theExpression, resolverNode, locator);
	}
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
FunctionEvaluate*
#endif
FunctionEvaluate::clone() const
{
	return new FunctionEvaluate(*this);
}



const XalanDOMString
FunctionEvaluate::getError() const
{
	return XalanMessageLoader::getMessage(XalanMessages::FunctionAcceptsOneArgument_1Param,"evaluate()");
}



XALAN_CPP_NAMESPACE_END
