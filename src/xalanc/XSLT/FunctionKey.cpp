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
#include "FunctionKey.hpp"



#include <set>



#include <xalanc/XalanDOM/XalanNode.hpp>
#include <xalanc/XalanDOM/XalanDocument.hpp>
#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include <xalanc/XPath/MutableNodeRefList.hpp>
#include <xalanc/XPath/NodeRefListBase.hpp>
#include <xalanc/XPath/XalanQNameByReference.hpp>
#include <xalanc/XPath/XObjectFactory.hpp>



XALAN_CPP_NAMESPACE_BEGIN



FunctionKey::FunctionKey()
{
}



FunctionKey::~FunctionKey()
{
}



inline void
getNodeSet(
			XPathExecutionContext&	executionContext,
			XalanDocument*			document,
			const XalanDOMString&	keyname,
			const XalanDOMString&	ref,
			const LocatorType*		locator,
			MutableNodeRefList&		theNodeRefList)
{
	assert(document != 0);

	if (indexOf(keyname, XalanUnicode::charColon) < length(keyname))
	{
		executionContext.getNodeSetByKey(
				document,
				keyname,
				ref,
				locator,
				theNodeRefList);
	}
	else
	{
		const XalanQNameByReference		theQName(keyname);

		executionContext.getNodeSetByKey(
				document,
				theQName,
				ref,
				theNodeRefList);
	}
}



XObjectPtr
FunctionKey::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				context,
			const XObjectPtr		arg1,
			const XObjectPtr		arg2,
			const LocatorType*		locator) const
{
	assert(arg1.null() == false && arg2.null() == false);

	if (context == 0)
	{
		executionContext.error(
			XalanMessageLoader::getMessage(XalanMessages::FunctionRequiresNonNullContextNode_1Param,"key()"),
			context,
			locator);

		return XObjectPtr();
	}
	else
	{
		XalanDocument* const	docContext = 
				XalanNode::DOCUMENT_NODE == context->getNodeType() ?
#if defined(XALAN_OLD_STYLE_CASTS)
						(XalanDocument*)context :
#else
						static_cast<XalanDocument*>(context) :
#endif
							context->getOwnerDocument();

		assert(docContext != 0);
		assert(executionContext.getPrefixResolver() != 0);

		const XalanDOMString&	keyname = arg1->str();

		assert(arg2.null() == false);

		typedef XPathExecutionContext::BorrowReturnMutableNodeRefList	BorrowReturnMutableNodeRefList;

		// This list will hold the nodes...
		BorrowReturnMutableNodeRefList	theNodeRefList(executionContext);

		if(arg2->getType() != XObject::eTypeNodeSet)
		{
			getNodeSet(
				executionContext,
				docContext,
				keyname,
				arg2->str(),
				locator,
				*theNodeRefList.get());
		}
		else
		{
			const NodeRefListBase&	theNodeSet = arg2->nodeset();

			const NodeRefListBase::size_type	nRefs = theNodeSet.getLength();

			if (nRefs == 1)
			{
				getNodeSet(
					executionContext,
					docContext,
					keyname,
					arg2->str(),
					locator,
					*theNodeRefList.get());
			}
			else if (nRefs > 1)
			{
				XPathExecutionContext::GetAndReleaseCachedString	theResult(executionContext);

				XalanDOMString&		ref = theResult.get();

				for(NodeRefListBase::size_type i = 0; i < nRefs; i++)
				{
					assert(theNodeSet.item(i) != 0);

					DOMServices::getNodeData(*theNodeSet.item(i), ref);

					if(0 != length(ref))
					{
						getNodeSet(
							executionContext,
							docContext,
							keyname,
							ref,
							locator,
							*theNodeRefList.get());
					}

					clear(ref);
				}
			}
		}

		return executionContext.getXObjectFactory().createNodeSet(theNodeRefList);
	}
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
FunctionKey*
#endif
FunctionKey::clone() const
{
	return new FunctionKey(*this);
}



const XalanDOMString
FunctionKey::getError() const
{
	return XalanMessageLoader::getMessage(XalanMessages::FunctionTakesTwoArguments_1Param,"key()");
}



XALAN_CPP_NAMESPACE_END
