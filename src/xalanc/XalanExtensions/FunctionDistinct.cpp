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
#include "FunctionDistinct.hpp"



#include <set>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include <xalanc/XPath/XPathExecutionContext.hpp>
#include <xalanc/XPath/XObjectFactory.hpp>



XALAN_CPP_NAMESPACE_BEGIN



FunctionDistinct::FunctionDistinct()
{
}



FunctionDistinct::~FunctionDistinct()
{
}



XObjectPtr
FunctionDistinct::execute(
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

	const NodeRefListBase&	nodeset = args[0]->nodeset();

	typedef XPathExecutionContext::BorrowReturnMutableNodeRefList	BorrowReturnMutableNodeRefList;

	BorrowReturnMutableNodeRefList	theResult(executionContext);

	const NodeRefListBase::size_type	theLength = nodeset.getLength();

	if (theLength == 1)
	{
		theResult->addNode(nodeset.item(0));
	}
	else if (theLength > 1)
	{
		typedef XPathExecutionContext::GetAndReleaseCachedString	GetAndReleaseCachedString;

		GetAndReleaseCachedString	theGuard(executionContext);

		XalanDOMString&				theCachedString = theGuard.get();

#if defined(XALAN_NO_STD_NAMESPACE)
		typedef set<XalanDOMString, less<XalanDOMString> >	SetType;
#else
		typedef std::set<XalanDOMString>					SetType;
#endif

		SetType		theStrings;

		// Check to make sure each node has a unique
		// string value.
		for (NodeRefListBase::size_type i = 0; i < theLength; ++i)
		{
			XalanNode* const	theNode = nodeset.item(i);
			assert(theNode != 0);

			DOMServices::getNodeData(*theNode, theCachedString);

			if (theStrings.find(theCachedString) == theStrings.end())
			{
				theResult->addNodeInDocOrder(theNode, executionContext);

				theStrings.insert(theCachedString);
			}

			clear(theCachedString);
		}
	}

	theResult->setDocumentOrder();

	return executionContext.getXObjectFactory().createNodeSet(theResult);
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
FunctionDistinct*
#endif
FunctionDistinct::clone() const
{
	return new FunctionDistinct(*this);
}



const XalanDOMString
FunctionDistinct::getError() const
{

	return XalanMessageLoader::getMessage(XalanMessages::FunctionAcceptsOneArgument_1Param,"distinct()");

}



XALAN_CPP_NAMESPACE_END
