/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2000 The Apache Software Foundation.  All rights 
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
#include "FunctionKey.hpp"



#include <set>



#include <XalanDOM/XalanNode.hpp>
#include <XalanDOM/XalanDocument.hpp>
#include <XalanDOM/XalanDOMString.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>



#include <DOMSupport/DOMServices.hpp>



#include <XPath/MutableNodeRefList.hpp>
#include <XPath/NodeRefListBase.hpp>
#include <XPath/XObjectFactory.hpp>



#if !defined(XALAN_NO_NAMESPACES)
	using std::set;
#endif 




FunctionKey::FunctionKey()
{
}



FunctionKey::~FunctionKey()
{
}



XObjectPtr
FunctionKey::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				context,			
			const XObjectPtr		arg1,
			const XObjectPtr		arg2,
			const Locator*			locator) const
{
	assert(arg1.null() == false && arg2.null() == false);

	if (context == 0)
	{
		executionContext.error(
			"The key() function requires a non-null context node!",
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

		if(0 == docContext)
		{
			executionContext.error(
				"Context does not have an owner document!",
				context,
				locator);
		}

		assert(executionContext.getPrefixResolver() != 0);

		const XalanDOMString&	keyname = arg1->str();

		assert(arg2.null() == false);

		const bool				argIsNodeSet =
				XObject::eTypeNodeSet == arg2->getType() ? true : false;

		typedef XPathExecutionContext::BorrowReturnMutableNodeRefList	BorrowReturnMutableNodeRefList;

		// This list will hold the nodes...
		BorrowReturnMutableNodeRefList	theNodeRefList(executionContext);

		if(argIsNodeSet == true)
		{
			const NodeRefListBase&	theNodeSet = arg2->nodeset();

			const unsigned int		nRefs = theNodeSet.getLength();

			if (nRefs > 0)
			{
#if defined(XALAN_NO_NAMESPACES)
				typedef set<XalanDOMString, less<XalanDOMString> >	StringSetType;
#else
				typedef std::set<XalanDOMString>	StringSetType;
#endif

				StringSetType	usedrefs;

				XalanDOMString	ref;

				for(unsigned int i = 0; i < nRefs; i++)
				{
					assert(theNodeSet.item(i) != 0);

					DOMServices::getNodeData(*theNodeSet.item(i), ref);

					if(0 != length(ref))
					{
						// Make sure we haven't already processed it...
						if(usedrefs.find(ref) == usedrefs.end())
						{
							usedrefs.insert(ref);

							executionContext.getNodeSetByKey(
											docContext, 
											keyname,
											ref,
											*executionContext.getPrefixResolver(),
											*theNodeRefList.get());
						}
					}

					clear(ref);
				}
			}
		}
		else
		{
			const XalanDOMString&	ref = arg2->str();

					executionContext.getNodeSetByKey(docContext,
											keyname,
											ref,
											*executionContext.getPrefixResolver(),
											*theNodeRefList.get());
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
	return XALAN_STATIC_UCODE_STRING("The key() function takes two arguments!");
}
