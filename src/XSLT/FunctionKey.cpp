/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
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



#include <dom/DOM_Node.hpp>
#include <dom/DOM_Document.hpp>
#include <dom/DOMString.hpp>



#include <Include/DOMHelper.hpp>
#include <PlatformSupport/DOMStringHelper.hpp>
#include <XPath/MutableNodeRefList.hpp>
#include <XPath/NodeRefListBase.hpp>
#include <XPath/XObject.hpp>
#include <XPath/XObjectFactory.hpp>
#include <XPath/XPathExecutionContext.hpp>






FunctionKey::FunctionKey()
{
}



FunctionKey::~FunctionKey()
{
}



XObject*
FunctionKey::execute(
			XPathExecutionContext&			executionContext,
			const DOM_Node&					context,
			int								/* opPos */,
			const std::vector<XObject*>&	args)
{
	if (args.size() != 2)
	{
		executionContext.error("The key() function takes two arguments!",
							   context);

		return 0;
	}
	else
	{
		DOM_Document docContext = 
				DOM_Node::DOCUMENT_NODE == context.getNodeType() ?
						static_cast<const DOM_Document&>(context) :
							context.getOwnerDocument();

		if(0 == docContext)
		{
			executionContext.error("Context does not have an owner document!",
								   context);
		}

		const DOMString			keyname = args[0]->str();

		const XObject* const	arg = args[1];
		assert(arg != 0);

		const bool				argIsNodeSet =
				XObject::eTypeNodeSet == arg->getType() ? true : false;

		MutableNodeRefList	theNodeRefList =
				executionContext.createMutableNodeRefList();

		if(argIsNodeSet == true)
		{
			const NodeRefListBase&	theNodeSet = arg->nodeset();

			const int				nRefs = theNodeSet.getLength();

			if (nRefs > 0)
			{
				std::set<DOMString>		usedrefs;

				const PrefixResolver&	thePrefixResolver =
											executionContext.getPrefixResolver();

				for(int i = 0; i < nRefs; i++)
				{
					const DOMString		ref =
							executionContext.getNodeData(theNodeSet.item(i));

					if(0 != length(ref))
					{
						// Make sure we haven't already processed it...
						if(usedrefs.find(ref) == usedrefs.end())
						{
							usedrefs.insert(ref);

							const NodeRefListBase* const		nl =
									executionContext.getNodeSetByKey(docContext, 
																	 keyname,
																	 ref, 
																	 thePrefixResolver);

							if (nl != 0)
							{
								theNodeRefList.addNodesInDocOrder(*nl);
							}
						}
					}
				}
			}
		}
		else
		{
			const DOMString		ref = arg->str();

			const NodeRefListBase* const	nl =
					executionContext.getNodeSetByKey(docContext, 
													 keyname,
													 ref, 
													 executionContext.getPrefixResolver());

			if (nl != 0)
			{
				theNodeRefList.addNodesInDocOrder(*nl);
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
