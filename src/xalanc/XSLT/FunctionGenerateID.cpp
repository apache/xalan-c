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
#include "FunctionGenerateID.hpp"



#include <xalanc/XalanDOM/XalanDocument.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/XPath/NodeRefListBase.hpp>
#include <xalanc/XPath/XObjectFactory.hpp>



XALAN_CPP_NAMESPACE_BEGIN



const XalanDOMString	FunctionGenerateID::s_emptyString;



FunctionGenerateID::FunctionGenerateID() :
	Function()
{
}



FunctionGenerateID::~FunctionGenerateID()
{
}



// Append the suffix to the provided string.
void
getSuffix(
		const XalanNode*	theNode,
		XalanDOMString&		theResult)
{
	assert(theNode != 0);

	// We're assuming here that each node has an implementation with a 
	// unique address that we can convert into a string...
	if (theNode->getOwnerDocument() != 0)
	{
		PointerToDOMString(theNode->getOwnerDocument(), theResult);

		append(theResult, XalanDOMChar(XalanUnicode::charFullStop));
	}

	PointerToDOMString(theNode, theResult);
}



XObjectPtr
FunctionGenerateID::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				context,
			const LocatorType*		locator) const
{
	if (context == 0)
	{
		executionContext.error(
				XalanMessageLoader::getMessage(XalanMessages::FunctionRequiresNonNullContextNode_1Param,"generate-id()"),
				context,
				locator);

		return XObjectPtr();
	}
	else
	{
		XPathExecutionContext::GetAndReleaseCachedString	theID(executionContext);

		theID.get() = XalanUnicode::charLetter_N;

		getSuffix(context, theID.get());

		return executionContext.getXObjectFactory().createString(theID);
	}
}



XObjectPtr
FunctionGenerateID::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				/* context */,
			const XObjectPtr		arg1,
			const LocatorType*		locator) const
{
	assert(arg1.null() == false);

	const NodeRefListBase&	theNodeList = arg1->nodeset();

	if (theNodeList.getLength() == 0)
	{
		return executionContext.getXObjectFactory().createStringReference(s_emptyString);
	}
	else
	{
		return execute(executionContext, theNodeList.item(0), locator);
	}
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
FunctionGenerateID*
#endif
FunctionGenerateID::clone() const
{
	return new FunctionGenerateID(*this);
}



const XalanDOMString
FunctionGenerateID::getError() const
{
	return XalanMessageLoader::getMessage(XalanMessages::FunctionTakesZeroOrOneArg_1Param,"generate-id");
}



XALAN_CPP_NAMESPACE_END
