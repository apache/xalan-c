/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2004 The Apache Software Foundation.  All rights 
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
