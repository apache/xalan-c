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
#if !defined(FUNCTIONLOCALNAME_HEADER_GUARD_1357924680)
#define FUNCTIONLOCALNAME_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



#include <vector>



#include <PlatformSupport/DOMStringHelper.hpp>



// Base class header file...
#include <XPath/Function.hpp>



#include <XPath/NodeRefListBase.hpp>
#include <XPath/XObject.hpp>
#include <XPath/XObjectFactory.hpp>
#include <XPath/XPathExecutionContext.hpp>



/**
 * XPath implementation of "local-name" function.
 */
//
// These are all inline, even though
// there are virtual functions, because we expect that they will only be
// needed by the XPath class.
class XALAN_XPATH_EXPORT FunctionLocalName : public Function
{
public:

	// These methods are inherited from Function ...

	virtual XObject*
	execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			int								/* opPos */,
			const XObjectArgVectorType&		args)
	{
		const XObjectArgVectorType::size_type	theSize =
			args.size();

		if(theSize > 1)
		{
			executionContext.error("The local-name() function takes zero or one arguments!",
								   context);
		}

		XalanDOMString	theData;

		if (theSize == 0)
		{
			if (context == 0)
			{
				executionContext.error("The local-name() function requires a non-null context node!");
			}
		}
		else
		{
			assert(args[0] != 0);

			const NodeRefListBase&	theNodeList = args[0]->nodeset();

			if (theNodeList.getLength() > 0)
			{
				context = theNodeList.item(0);
			}
		}

		if (context != 0)
		{
			const XalanNode::NodeType	theType = context->getNodeType();

			if(theType == XalanNode::ATTRIBUTE_NODE ||
				theType == XalanNode::ELEMENT_NODE ||
				theType == XalanNode::PROCESSING_INSTRUCTION_NODE)
			{
				theData = executionContext.getLocalNameOfNode(*context);
			}
		}

		return executionContext.getXObjectFactory().createString(theData);
	}

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual FunctionLocalName*
#endif
	clone() const
	{
		return new FunctionLocalName(*this);
	}

private:

	// Not implemented...
	FunctionLocalName&
	operator=(const FunctionLocalName&);

	bool
	operator==(const FunctionLocalName&) const;
};



#endif	// FUNCTIONLOCALNAME_HEADER_GUARD_1357924680
