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
#include "FunctionID.hpp"



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "XObjectFactory.hpp"



XALAN_CPP_NAMESPACE_BEGIN



FunctionID::FunctionID()
{
}



FunctionID::~FunctionID()
{
}



XObjectPtr
FunctionID::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				context,
			const XObjectPtr		arg1,
			const LocatorType*		/* locator */) const
{
	assert(arg1.null() == false);	
	assert(context != 0);

	typedef XPathExecutionContext::GetAndReleaseCachedString	GetAndReleaseCachedString;

	GetAndReleaseCachedString	theGuard1(executionContext);

	XalanDOMString&				theResultString = theGuard1.get();

	// Do the callback to get the data.
	FunctionIDXObjectTypeCallback	theCallback(executionContext, theResultString);

	theCallback.processCallback(*arg1);

	// Get the context document, so we can search for nodes.
	const XalanDocument* const	theDocContext = context->getNodeType() == XalanNode::DOCUMENT_NODE ?
#if defined(XALAN_OLD_STYLE_CASTS)
									(const XalanDocument*)context :
#else
									static_cast<const XalanDocument*>(context) :
#endif
									context->getOwnerDocument();
	assert(theDocContext != 0);

	if (theResultString.empty() == true)
	{
		return executionContext.getXObjectFactory().createNodeSet(0);
	}
	else
	{
		StringTokenizer		theTokenizer(theResultString);

		GetAndReleaseCachedString	theGuard2(executionContext);

		XalanDOMString&		theToken = theGuard2.get();

		StringTokenizer::size_type	theTokenCount = theTokenizer.countTokens();

		if (theTokenCount == 1)
		{
			theTokenizer.nextToken(theToken);

			return executionContext.getXObjectFactory().createNodeSet(theDocContext->getElementById(theToken));
		}
		else
		{
			assert(theTokenCount != 0);

			typedef XPathExecutionContext::BorrowReturnMutableNodeRefList	BorrowReturnMutableNodeRefList;

			// This list will hold the nodes we find.
			BorrowReturnMutableNodeRefList	theNodeList(executionContext);

			// Parse the result string...
			while(theTokenCount-- > 0)
			{
				theTokenizer.nextToken(theToken);

				if (length(theToken) > 0)
				{
					XalanNode* const	theNode = theDocContext->getElementById(theToken);

					if (theNode != 0)
					{
						theNodeList->addNodeInDocOrder(theNode, executionContext);
					}
				}
			}

			return executionContext.getXObjectFactory().createNodeSet(theNodeList);
		}
	}
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
FunctionID*
#endif
FunctionID::clone() const
{
	return new FunctionID(*this);
}



const XalanDOMString
FunctionID::getError() const
{

	return XalanMessageLoader::getMessage(XalanMessages::FunctionAcceptsOneArgument_1Param, "id()");
}



FunctionID::FunctionIDXObjectTypeCallback::FunctionIDXObjectTypeCallback(
			XPathExecutionContext&	theExecutionContext,
			XalanDOMString&			theResultString) :
	XObjectTypeCallback(),
	m_resultString(theResultString),
	m_executionContext(theExecutionContext)
{
}



void
FunctionID::FunctionIDXObjectTypeCallback::processCallback(const XObject&	theXObject)
{
	theXObject.ProcessXObjectTypeCallback(*this);
}



void
FunctionID::FunctionIDXObjectTypeCallback::Number(
			const XObject&	theXObject,
			double			/* theValue */)
{
	m_resultString = theXObject.str();
}



void
FunctionID::FunctionIDXObjectTypeCallback::Boolean(
			const XObject&	theXObject,
			bool			/* theValue */)
{
	m_resultString = theXObject.str();
}



void
FunctionID::FunctionIDXObjectTypeCallback::String(
			const XObject&			theXObject,
			const XalanDOMString&	/* theValue */)
{
	m_resultString = theXObject.str();
}



void
FunctionID::FunctionIDXObjectTypeCallback::ResultTreeFragment(
			const XObject&					theXObject,
			const XalanDocumentFragment&	/* theValue */)
{
	m_resultString = theXObject.str();
}



void
FunctionID::FunctionIDXObjectTypeCallback::ResultTreeFragment(
			const XObject&			theXObject,
			XalanDocumentFragment&	/* theValue */)
{
	m_resultString = theXObject.str();
}



void
FunctionID::FunctionIDXObjectTypeCallback::NodeSet(
			const XObject&			/* theXObject */,
			const NodeRefListBase&	theValue)
{
	const NodeRefListBase::size_type	theNodeCount = theValue.getLength();

	for (NodeRefListBase::size_type i = 0 ; i < theNodeCount; i++)
	{
		DOMServices::getNodeData(*theValue.item(i), m_resultString);

		append(m_resultString, XalanDOMChar(XalanUnicode::charSpace));			
	}
}



void
FunctionID::FunctionIDXObjectTypeCallback::Unknown(
			const XObject&			/* theObject */,
			const XalanDOMString&	/* theName */)
{
}


void
FunctionID::FunctionIDXObjectTypeCallback::Null(const XObject&		/* theObject */)
{
}



XALAN_CPP_NAMESPACE_END
