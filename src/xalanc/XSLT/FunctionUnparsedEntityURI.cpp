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
#include "FunctionUnparsedEntityURI.hpp"



#include <xalanc/XalanDOM/XalanNode.hpp>
#include <xalanc/XalanDOM/XalanDocument.hpp>



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/XPath/XObjectFactory.hpp>



XALAN_CPP_NAMESPACE_BEGIN



FunctionUnparsedEntityURI::FunctionUnparsedEntityURI()
{
}



FunctionUnparsedEntityURI::~FunctionUnparsedEntityURI()
{
}


XObjectPtr
FunctionUnparsedEntityURI::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				context,			
			const XObjectPtr		arg,
			const LocatorType*		locator) const
{
	assert(arg.null() == false);

	if (context == 0)
	{
		executionContext.error(
			XalanMessageLoader::getMessage(XalanMessages::FunctionRequiresNonNullContextNode_1Param,"unparsed-entity-uri()"),
			context,
			locator);

		return XObjectPtr();
	}
	else
	{
		const XalanDOMString&	name = arg->str();

		XalanDocument* const	doc =
				XalanNode::DOCUMENT_NODE == context->getNodeType() ?
#if defined(XALAN_OLD_STYLE_CASTS)
					(XalanDocument*)context :
#else
					static_cast<XalanDocument*>(context) :
#endif
					context->getOwnerDocument();
		assert(doc != 0);

		const XalanDOMString&	uri = executionContext.getUnparsedEntityURI(name, *doc);

		return executionContext.getXObjectFactory().createStringReference(uri);
	}
}



const XalanDOMString
FunctionUnparsedEntityURI::getError() const
{
	return XalanMessageLoader::getMessage(XalanMessages::FunctionAcceptsOneArgument_1Param,"unparsed-entity-uri");
}



XALAN_CPP_NAMESPACE_END
