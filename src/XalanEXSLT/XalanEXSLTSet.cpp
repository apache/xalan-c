/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2002 The Apache Software Foundation.  All rights 
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
#include "XalanEXSLTSet.hpp"
#include "XalanEXSLTSetImpl.hpp"



#include <PlatformSupport/XalanUnicode.hpp>



#include <XPath/XObjectFactory.hpp>
#include <XPath/XPathEnvSupportDefault.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XObjectPtr
XalanEXSLTFunctionHasSameNode::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const LocatorType*				locator) const
{
	if (args.size() != 2)
	{
		executionContext.error(getError(), context, locator);
	}

	assert(args[0].null() == false && args[1].null() == false);

	const NodeRefListBase&	nodeset1 = args[0]->nodeset();
	const NodeRefListBase&	nodeset2 = args[1]->nodeset();

	const NodeRefListBase::size_type	theLength1 = nodeset1.getLength();
	const NodeRefListBase::size_type	theLength2 = nodeset2.getLength();

	bool	fResult = false;

	if (theLength1 != 0 && theLength2 != 0)
	{
		for (NodeRefListBase::size_type i = 0; i < theLength1 && fResult == false; ++i)
		{
			XalanNode* const	theNode = nodeset1.item(i);
			assert(theNode != 0);

			if (nodeset2.indexOf(theNode) != NodeRefListBase::npos)
			{
				fResult = true;
			}
		}
	}

	return executionContext.getXObjectFactory().createBoolean(fResult);
}



typedef Function::XObjectArgVectorType	XObjectArgVectorType;


template<class PredicateType>
XObjectPtr
findNodes(
			XPathExecutionContext&			executionContext,
			const XObjectArgVectorType&		args,
			PredicateType					thePredicate)
{
	assert(args[0].null() == false && args[1].null() == false);

	const NodeRefListBase&	nodeset1 = args[0]->nodeset();
	const NodeRefListBase&	nodeset2 = args[1]->nodeset();

	const NodeRefListBase::size_type	theLength1 = nodeset1.getLength();
	const NodeRefListBase::size_type	theLength2 = nodeset2.getLength();

	if (theLength1 == 0 || theLength2 == 0)
	{
		return args[0];
	}
	else
	{
		const XalanNode* const	theNode = nodeset2.item(0);
		assert(theNode != 0);

		XPathExecutionContext::BorrowReturnMutableNodeRefList	theNodes(executionContext);

		const NodeRefListBase::size_type	theIndex = nodeset1.indexOf(theNode);

		if (theIndex != NodeRefListBase::npos)
		{
			for (NodeRefListBase::size_type i = 0; i < theLength1; ++i)
			{
				XalanNode* const	theCurrentNode = nodeset1.item(i);
				assert(theCurrentNode != 0);

				if (thePredicate(theCurrentNode, theNode) == true)
				{
					theNodes->addNodeInDocOrder(theCurrentNode, executionContext);
				}
			}
		}

		theNodes->setDocumentOrder();

		return executionContext.getXObjectFactory().createNodeSet(theNodes);
	}
}



struct LeadingCompareFunctor
{
	LeadingCompareFunctor(XPathExecutionContext&	executionContext) :
		m_executionContext(executionContext)
	{
	}

	bool
	operator()(
			const XalanNode*	theLHS,
			const XalanNode*	theRHS) const
	{
		assert(theLHS != 0 && theRHS != 0);

		return theLHS != theRHS && m_executionContext.isNodeAfter(*theLHS, *theRHS) == false;
	}

private:

	XPathExecutionContext&	m_executionContext;
};



XObjectPtr
XalanEXSLTFunctionLeading::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const LocatorType*				locator) const
{
	if (args.size() != 2)
	{
		executionContext.error(getError(), context, locator);
	}

	return findNodes(executionContext, args, LeadingCompareFunctor(executionContext));
}



struct TrailingCompareFunctor
{
	TrailingCompareFunctor(XPathExecutionContext&	executionContext) :
		m_executionContext(executionContext)
	{
	}

	bool
	operator()(
			const XalanNode*	theLHS,
			const XalanNode*	theRHS) const
	{
		assert(theLHS != 0 && theRHS != 0);

		return m_executionContext.isNodeAfter(*theLHS, *theRHS) == true;
	}

private:

	XPathExecutionContext&	m_executionContext;
};



XObjectPtr
XalanEXSLTFunctionTrailing::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const LocatorType*				locator) const
{
	if (args.size() != 2)
	{
		executionContext.error(getError(), context, locator);
	}

	return findNodes(executionContext, args, TrailingCompareFunctor(executionContext));
}



static const XalanDOMChar	s_setNamespace[] =
{
	XalanUnicode::charLetter_h,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_p,
	XalanUnicode::charColon,
	XalanUnicode::charSolidus,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_x,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_t,
	XalanUnicode::charFullStop,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_g,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_s,
	0
};



static const XalanDOMChar	s_differenceFunctionName[] =
{
	XalanUnicode::charLetter_d,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_f,
	XalanUnicode::charLetter_f,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_e,
	0
};



static const XalanDOMChar	s_distinctFunctionName[] =
{
	XalanUnicode::charLetter_d,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_t,
	0
};



static const XalanDOMChar	s_hasSameNodeFunctionName[] =
{
	XalanUnicode::charLetter_h,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_s,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_e,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_d,
	XalanUnicode::charLetter_e,
	0
};



static const XalanDOMChar	s_intersectionFunctionName[] =
{
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_n,
	0
};



static const XalanDOMChar	s_leadingFunctionName[] =
{
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_d,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_g,
	0
};



static const XalanDOMChar	s_trailingFunctionName[] =
{
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_g,
	0
};



static const XalanEXSLTFunctionDifference		s_differenceFunction;
static const XalanEXSLTFunctionDistinct			s_distinctFunction;
static const XalanEXSLTFunctionHasSameNode		s_hasSameNodeFunction;
static const XalanEXSLTFunctionIntersection		s_intersectionFunction;
static const XalanEXSLTFunctionLeading			s_leadingFunction;
static const XalanEXSLTFunctionTrailing			s_trailingFunction;



static const XalanEXSLTSetFunctionsInstaller::FunctionTableEntry	theFunctionTable[] =
{
	{ s_differenceFunctionName, &s_differenceFunction },
	{ s_distinctFunctionName, &s_distinctFunction },
	{ s_hasSameNodeFunctionName, &s_hasSameNodeFunction },
	{ s_intersectionFunctionName, &s_intersectionFunction },
	{ s_leadingFunctionName, &s_leadingFunction },
	{ s_trailingFunctionName, &s_trailingFunction },
	{ 0, 0 }
};



void
XalanEXSLTSetFunctionsInstaller::installLocal(XPathEnvSupportDefault&	theSupport)
{
	doInstallLocal(s_setNamespace, theFunctionTable, theSupport);
}



void
XalanEXSLTSetFunctionsInstaller::installGlobal()
{
	doInstallGlobal(s_setNamespace, theFunctionTable);
}



void
XalanEXSLTSetFunctionsInstaller::uninstallLocal(XPathEnvSupportDefault&	theSupport)
{
	doUninstallLocal(s_setNamespace, theFunctionTable, theSupport);
}



void
XalanEXSLTSetFunctionsInstaller::uninstallGlobal()
{
	doUninstallGlobal(s_setNamespace, theFunctionTable);
}



XALAN_CPP_NAMESPACE_END
