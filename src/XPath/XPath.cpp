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
 *
 * @author <a href="mailto:david_n_bertoni@lotus.com">David N. Bertoni</a>
 */

// Class header file.
#include "XPath.hpp"



#include <cassert>
#include <cfloat>



#include <Include/STLHelper.hpp>



#include <XalanDOM/XalanNamedNodeMap.hpp>



#include <PlatformSupport/DoubleSupport.hpp>
#include <PlatformSupport/PrefixResolver.hpp>



#include <DOMSupport/DOMServices.hpp>



#include "FoundIndex.hpp"
#include "MutableNodeRefList.hpp"
#include "XalanQNameByReference.hpp"
#include "XLocator.hpp"
#include "XPathException.hpp"
#include "XObject.hpp"
#include "XObjectFactory.hpp"
#include "XPathEnvSupport.hpp"



const XalanDOMString	XPath::s_emptyString;



XPath::XPath(const Locator*	theLocator) :
	m_expression(),
	m_locator(theLocator),
	m_inStylesheet(false)
{
}



XPath::~XPath()
{
}



void
XPath::shrink()
{
	m_expression.shrink();
}



void
XPath::installFunction(
			const XalanDOMString&	funcName,
			const Function& 		func)
{
	s_functions.InstallFunction(funcName,
							    func);
}



bool
XPath::uninstallFunction(const XalanDOMString&	funcName)
{
	return 	s_functions.UninstallFunction(funcName);
}



const XObjectPtr
XPath::execute(
			XalanNode*				context,
			const PrefixResolver&	prefixResolver,
			XPathExecutionContext&	executionContext) const
{
	// Push and pop the PrefixResolver...
	XPathExecutionContext::PrefixResolverSetAndRestore	theResolverSetAndRestore(
									executionContext,
									&prefixResolver);

	// Push and pop the current node...
	XPathExecutionContext::CurrentNodeSetAndRestore		theNodeSetAndRestore(
									executionContext,
									context);

	return executeMore(context, 0, executionContext);
}



const XObjectPtr
XPath::executeMore(
			XalanNode* 				context,
			int 					opPos,
			XPathExecutionContext&	executionContext) const
{
	switch(m_expression.m_opMap[opPos])
	{
	case XPathExpression::eOP_XPATH:
		return executeMore(context, opPos + 2, executionContext);
		break;

	case XPathExpression::eOP_MATCHPATTERN:
		return matchPattern(context, opPos + 2, executionContext);
		break;

	case XPathExpression::eEMPTY:
		opPos++;
		break;

	case XPathExpression::eOP_OR:
		return Or(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_AND:
		return And(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_NOTEQUALS:
		return notequals(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_EQUALS:
		return equals(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_LTE:
		return lte(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_LT:
		return lt(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_GTE:
		return gte(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_GT:
		return gt(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_PLUS:
		return plus(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_MINUS:
	  return minus(context, opPos, executionContext);
	  break;

	case XPathExpression::eOP_MULT:
		return mult(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_DIV:
		return div(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_MOD:
		return mod(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_NEG:
		return neg(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_BOOL:
		return boolean(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_UNION:
		return Union(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_LITERAL:
		return literal(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_VARIABLE:
	  return variable(context, opPos, executionContext);
	  break;

	case XPathExpression::eOP_GROUP:
		return group(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_NUMBERLIT:
		return numberlit(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_ARGUMENT:
		return arg(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_EXTFUNCTION:
		return runExtFunction(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_FUNCTION:
		return runFunction(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_LOCATIONPATH:
		return locationPath(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_LOCATIONPATHPATTERN:
		return locationPathPattern(context, opPos, executionContext);
		break;

	default:
		{
			const XalanDOMString	theOpCode = LongToDOMString(m_expression.m_opMap[opPos]);

			executionContext.error(
				TranscodeFromLocalCodePage("ERROR! Unknown op code: ") + theOpCode,
				context,
				m_locator);
		}
		break;
	}

	return XObjectPtr();
}



inline void
XPath::doGetMatchScore(
			XalanNode*				context,
			XPathExecutionContext&	executionContext,
			eMatchScore&			score) const
{
	assert(context != 0);

	int		opPos = 2;

	while(m_expression.m_opMap[opPos] == XPathExpression::eOP_LOCATIONPATHPATTERN)
	{
		const int	nextOpPos = m_expression.getNextOpCodePosition(opPos);

		score = locationPathPattern(executionContext, *context, opPos);

		if(score == eMatchScoreNone)
		{
			opPos = nextOpPos;
		}
		else
		{
			break;
		}
	}
}



XPath::eMatchScore
XPath::getMatchScore(
			XalanNode*				node,
			const PrefixResolver&	resolver,
			XPathExecutionContext&	executionContext) const
{
	eMatchScore		score = eMatchScoreNone;

	if(m_expression.m_opMap[0] == XPathExpression::eOP_MATCHPATTERN)
	{
		assert(node != 0);

		const PrefixResolver* const		theCurrentResolver =
			executionContext.getPrefixResolver();

		if (theCurrentResolver == &resolver)
		{
			doGetMatchScore(node, executionContext, score);
		}
		else
		{
			// Push and pop the PrefixResolver...
			XPathExecutionContext::PrefixResolverSetAndRestore	theSetAndRestore(
										executionContext,
										theCurrentResolver,
										&resolver);

			doGetMatchScore(node, executionContext, score);
		}
	}
	else
	{
		executionContext.error(
			TranscodeFromLocalCodePage("Expected match pattern in getMatchScore!"),
			node,
			m_locator);
	}
	
	return score;
}



void
XPath::getTargetElementStrings(TargetElementStringsVectorType&	targetStrings) const
{
	int opPos = 2;

	targetStrings.reserve(eDefaultTargetStringsSize);

	while(m_expression.m_opMap[opPos] == XPathExpression::eOP_LOCATIONPATHPATTERN)
	{
		const int	nextOpPos = m_expression.getNextOpCodePosition(opPos);

		opPos += 2;
	  
		while(m_expression.m_opMap[opPos] != XPathExpression::eENDOP)
		{
			const int	nextStepPos =
				m_expression.getNextOpCodePosition(opPos);

			const int	nextOp = m_expression.m_opMap[nextStepPos];

			if(nextOp == XPathExpression::eOP_PREDICATE ||
			   nextOp == XPathExpression::eENDOP)
			{
				const int	stepType = m_expression.m_opMap[opPos];

				opPos += 3;

				switch(stepType)
				{
				case XPathExpression::eOP_FUNCTION:
					targetStrings.push_back(PSEUDONAME_ANY);
					break;

				case XPathExpression::eFROM_ROOT:
					targetStrings.push_back(PSEUDONAME_ROOT);
					break;

				case XPathExpression::eMATCH_ATTRIBUTE:
				case XPathExpression::eMATCH_ANY_ANCESTOR:
				case XPathExpression::eMATCH_IMMEDIATE_ANCESTOR:
					{
						const int	tok = m_expression.m_opMap[opPos];

						opPos++;

						switch(tok)
						{
						case XPathExpression::eNODETYPE_COMMENT:
							targetStrings.push_back(PSEUDONAME_COMMENT);
							break;

						case XPathExpression::eNODETYPE_TEXT:
							targetStrings.push_back(PSEUDONAME_TEXT);
							break;

						case XPathExpression::eNODETYPE_NODE:
							targetStrings.push_back(PSEUDONAME_NODE);
							break;

						case XPathExpression::eNODETYPE_ROOT:
							targetStrings.push_back(PSEUDONAME_ROOT);
							break;

						case XPathExpression::eNODETYPE_ANYELEMENT:
							targetStrings.push_back(PSEUDONAME_ANY);
							break;

						case XPathExpression::eNODETYPE_PI:
							targetStrings.push_back(PSEUDONAME_PI);
							break;

						case XPathExpression::eNODENAME:
							{
								// Skip the namespace
								const int	tokenIndex = m_expression.m_opMap[opPos + 1];

								if(tokenIndex >= 0)
								{
									const XalanDOMString&	targetName =
										m_expression.m_tokenQueue[tokenIndex].str();

									if(::equals(targetName, PSEUDONAME_ANY) == true)
									{
										targetStrings.push_back(PSEUDONAME_ANY);
									}
									else
									{
										targetStrings.push_back(targetName);
									}
								}
								else
								{
									targetStrings.push_back(PSEUDONAME_ANY);
								}
							}
							break;

						default:
							targetStrings.push_back(PSEUDONAME_ANY);
							break;
						}
					}
					break;
				}
			}

			opPos = nextStepPos;
		}

		opPos = nextOpPos;
	}
}



const XObjectPtr
XPath::matchPattern(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const

{
	XObjectPtr	score;

	while(m_expression.m_opMap[opPos] == XPathExpression::eOP_LOCATIONPATHPATTERN)
	{
		const int	nextOpPos = m_expression.getNextOpCodePosition(opPos);

		score = executeMore(context, opPos, executionContext);
		assert(score.null() == false);

		if(score->num() != eMatchScoreNone)
		{
			break;
		}
		else
		{
			opPos = nextOpPos;
		}
	}

	if(score.null() == false)
	{
		return score;
	}
	else
	{
		return executionContext.getXObjectFactory().createNumber(getMatchScoreValue(eMatchScoreNone));
	}
}



const XObjectPtr
XPath::Or(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	opPos += 2;

	const XObjectPtr	expr1(executeMore(context, opPos, executionContext));
	assert(expr1.get() != 0);

	bool	fResult = expr1->boolean();

	if(fResult == false)
	{
		const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

		const XObjectPtr	expr2(executeMore(context, expr2Pos, executionContext));
		assert(expr2.get() != 0);

		fResult = expr2->boolean();
	}

	return executionContext.getXObjectFactory().createBoolean(fResult);
}



const XObjectPtr
XPath::And(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	bool	fResult = false;

	opPos += 2;	

	const XObjectPtr	expr1(executeMore(context, opPos, executionContext));
	assert(expr1.get() != 0);

	if (expr1->boolean() == true)
	{
		const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

		const XObjectPtr	expr2(executeMore(context, expr2Pos, executionContext));
		assert(expr2.get() != 0);

		if (expr2->boolean() == true)
		{
			fResult = true;
		}
	}	

	return executionContext.getXObjectFactory().createBoolean(fResult);
}



const XObjectPtr
XPath::notequals(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	opPos += 2;	

	const XObjectPtr	expr1(executeMore(context, opPos, executionContext));
	assert(expr1.get() != 0);

	const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

	const XObjectPtr	expr2(executeMore(context, expr2Pos, executionContext));
	assert(expr2.get() != 0);

	return executionContext.getXObjectFactory().createBoolean(expr1->notEquals(*expr2.get(), executionContext));
}



const XObjectPtr
XPath::equals(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	opPos += 2;	

	const XObjectPtr	expr1(executeMore(context, opPos, executionContext));
	assert(expr1.get() != 0);

	const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

	const XObjectPtr	expr2(executeMore(context, expr2Pos, executionContext));
	assert(expr2.get() != 0);

	return executionContext.getXObjectFactory().createBoolean(expr1->equals(*expr2.get(), executionContext));
}



const XObjectPtr
XPath::lte(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	opPos += 2;	

	const XObjectPtr	expr1(executeMore(context, opPos, executionContext));
	assert(expr1.get() != 0);

	const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

	const XObjectPtr	expr2(executeMore(context, expr2Pos, executionContext));
	assert(expr2.get() != 0);

	return executionContext.getXObjectFactory().createBoolean(expr1->lessThanOrEquals(*expr2.get(), executionContext));
}



const XObjectPtr
XPath::lt(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	opPos += 2;

	const XObjectPtr	expr1(executeMore(context, opPos, executionContext));
	assert(expr1.get() != 0);

	const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

	const XObjectPtr	expr2(executeMore(context, expr2Pos, executionContext));
	assert(expr2.get() != 0);

	return executionContext.getXObjectFactory().createBoolean(expr1->lessThan(*expr2.get(), executionContext));
}



const XObjectPtr
XPath::gte(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	opPos += 2;

	const XObjectPtr	expr1(executeMore(context, opPos, executionContext));
	assert(expr1.get() != 0);

	const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

	const XObjectPtr	expr2(executeMore(context, expr2Pos, executionContext));
	assert(expr2.get() != 0);

	return executionContext.getXObjectFactory().createBoolean(expr1->greaterThanOrEquals(*expr2.get(), executionContext));
}



const XObjectPtr
XPath::gt(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	opPos += 2;

	const XObjectPtr	expr1(executeMore(context, opPos, executionContext));
	assert(expr1.get() != 0);

	const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

	const XObjectPtr	expr2(executeMore(context, expr2Pos, executionContext));
	assert(expr2.get() != 0);

	return executionContext.getXObjectFactory().createBoolean(expr1->greaterThan(*expr2.get(), executionContext));
}



double
XPath::getNumericOperand(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	if (m_expression.m_opMap[opPos] == XPathExpression::eOP_NUMBERLIT)
	{
		assert(m_expression.m_tokenQueue.size() > unsigned(m_expression.m_opMap[opPos + 3]));

		return m_expression.getNumberLiteral(m_expression.m_opMap[opPos + 2]);
	}
	else
	{
		return executeMore(context, opPos, executionContext)->num();
	}
}



const XObjectPtr
XPath::plus(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	opPos += 2;

	const double	expr1 = getNumericOperand(context, opPos, executionContext);

	const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

	const double	expr2 = getNumericOperand(context, expr2Pos, executionContext);

	return executionContext.getXObjectFactory().createNumber(DoubleSupport::add(expr1, expr2));
}



const XObjectPtr
XPath::minus(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	opPos += 2;

	const double	expr1 = getNumericOperand(context, opPos, executionContext);

	const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

	const double	expr2 = getNumericOperand(context, expr2Pos, executionContext);

	return executionContext.getXObjectFactory().createNumber(DoubleSupport::subtract(expr1, expr2));
}



const XObjectPtr
XPath::mult(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	opPos += 2;

	const double	expr1 = getNumericOperand(context, opPos, executionContext);

	const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

	const double	expr2 = getNumericOperand(context, expr2Pos, executionContext);

	return executionContext.getXObjectFactory().createNumber(DoubleSupport::multiply(expr1, expr2));
}



const XObjectPtr
XPath::div(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	opPos += 2;

	const double	expr1 = getNumericOperand(context, opPos, executionContext);

	const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

	const double	expr2 = getNumericOperand(context, expr2Pos, executionContext);

	return executionContext.getXObjectFactory().createNumber(DoubleSupport::divide(expr1, expr2));
}



const XObjectPtr
XPath::mod(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	opPos += 2;	

	const XObjectPtr	expr1(executeMore(context, opPos, executionContext));
	assert(expr1.get() != 0);

	const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

	const XObjectPtr	expr2(executeMore(context, expr2Pos, executionContext));
	assert(expr2.get() != 0);

	return executionContext.getXObjectFactory().createNumber(DoubleSupport::modulus(expr1->num(), expr2->num()));
}



const XObjectPtr
XPath::neg(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	const XObjectPtr	expr1(executeMore(context, opPos + 2, executionContext));
	assert(expr1.get() != 0);

	return executionContext.getXObjectFactory().createNumber(DoubleSupport::negative(expr1->num()));
}



const XObjectPtr
XPath::boolean(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	const XObjectPtr	expr1(executeMore(context, opPos + 2, executionContext));
	assert(expr1.get() != 0);

	// Try to optimize when the result of the execution is
	// already a boolean.
	if (expr1->getType() == XObject::eTypeBoolean)
	{
		return expr1;
	}
	else
	{
		return executionContext.getXObjectFactory().createBoolean(expr1->boolean());
	}
}


 
const XObjectPtr
XPath::Union(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	opPos += 2;

	typedef XPathExecutionContext::BorrowReturnMutableNodeRefList	BorrowReturnMutableNodeRefList;

	BorrowReturnMutableNodeRefList	resultNodeList(executionContext);

	while(m_expression.m_opMap[opPos] != XPathExpression::eENDOP)
	{
		const int	nextOpPos = m_expression.getNextOpCodePosition(opPos);

		const XObjectPtr	expr(executeMore(context, opPos, executionContext));

		const NodeRefListBase&	nl =
				expr->nodeset();

		resultNodeList->addNodesInDocOrder(nl, executionContext);	

		opPos = nextOpPos;
	}

	return executionContext.getXObjectFactory().createNodeSet(resultNodeList);
}



const XObjectPtr
XPath::literal(
			XalanNode*				/* context */,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	assert(m_expression.m_opMap.size() > unsigned(opPos + 2));
	assert(m_expression.m_tokenQueue.size() > unsigned(m_expression.m_opMap[opPos + 2]));

	const XToken&	theLiteral = m_expression.m_tokenQueue[m_expression.m_opMap[opPos + 2]];

	if (m_inStylesheet == true)
	{
		return executionContext.getXObjectFactory().createString(theLiteral);
	}
	else
	{
		return executionContext.getXObjectFactory().createString(theLiteral.str());
	}
}



const XObjectPtr
XPath::variable(
			XalanNode*				/* context */,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	assert(executionContext.getPrefixResolver() != 0);

	const XObject&	ns = m_expression.m_tokenQueue[m_expression.m_opMap[opPos + 2]];

	const XObject&	varName = m_expression.m_tokenQueue[m_expression.m_opMap[opPos + 3]];

	return executionContext.getVariable(XalanQNameByReference(ns.str(), varName.str()), m_locator);
}



const XObjectPtr
XPath::numberlit(
			XalanNode*				/* context */,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	assert(m_expression.m_opMap.size() > unsigned(opPos + 3));
	assert(m_expression.m_tokenQueue.size() > unsigned(m_expression.m_opMap[opPos + 3]));

	const XToken&	theLiteral = m_expression.m_tokenQueue[m_expression.m_opMap[opPos + 3]];

	if (m_inStylesheet == true)
	{
		return executionContext.getXObjectFactory().createNumber(theLiteral);
	}
	else
	{
		return executionContext.getXObjectFactory().createNumber(theLiteral.num());
	}
}



const XObjectPtr
XPath::locationPath(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{    
	assert(context != 0);

	return locationPath(executionContext, *context, opPos);
}



const XObjectPtr
XPath::locationPathPattern(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	assert(context != 0);

	const eMatchScore	result =
		locationPathPattern(executionContext, *context, opPos);

	return executionContext.getXObjectFactory().createNumber(getMatchScoreValue(result));
}



const XObjectPtr
XPath::runExtFunction(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	const int	endExtFunc = opPos + m_expression.m_opMap[opPos + 1] - 1;

	opPos += 2;

	const XalanDOMString&	ns = m_expression.m_tokenQueue[m_expression.m_opMap[opPos]].str();

	++opPos;

	const XalanDOMString&	funcName = m_expression.m_tokenQueue[m_expression.m_opMap[opPos]].str();

	++opPos;

	typedef XPathExecutionContext::XObjectArgVectorType		XObjectArgVectorType;

	XObjectArgVectorType	args;

	while(opPos < endExtFunc)
	{
		const int	nextOpPos = m_expression.getNextOpCodePosition(opPos);

		args.push_back(executeMore(context, opPos, executionContext));

		opPos = nextOpPos;
	}

	return extfunction(context, opPos, ns, funcName, args, executionContext);
}



const XObjectPtr
XPath::runFunction(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	const int	endFunc = opPos + m_expression.m_opMap[opPos + 1] - 1;

	opPos += 2;

	// This is actually the position in the token queue of the
	// string that contains the name of the function.
	const int	funcID = m_expression.m_opMap[opPos];

	opPos++;

	// Number of args is next, not used for now...
	const int	argCount = m_expression.m_opMap[opPos];

	opPos++;

	if (argCount == 0)
	{
		assert(opPos == endFunc);

		return s_functions[funcID].execute(executionContext, context, m_locator);
	}
	else if (argCount == 1)
	{
		const int	nextOpPos = m_expression.getNextOpCodePosition(opPos);

		const XObjectPtr	theArg(executeMore(context, opPos, executionContext));

		opPos = nextOpPos;
		
		assert(opPos == endFunc);

		return s_functions[funcID].execute(executionContext, context, theArg, m_locator);
	}
	else if (argCount == 2)
	{
		int	nextOpPos = m_expression.getNextOpCodePosition(opPos);

		const XObjectPtr	theArg1(executeMore(context, opPos, executionContext));

		opPos = nextOpPos;

		nextOpPos = m_expression.getNextOpCodePosition(opPos);

		const XObjectPtr	theArg2(executeMore(context, opPos, executionContext));

		opPos = nextOpPos;
		
		assert(opPos == endFunc);

		return s_functions[funcID].execute(executionContext, context, theArg1, theArg2, m_locator);
	}
	else if (argCount == 3)
	{

		int	nextOpPos = m_expression.getNextOpCodePosition(opPos);

		const XObjectPtr	theArg1(executeMore(context, opPos, executionContext));

		opPos = nextOpPos;

		nextOpPos = m_expression.getNextOpCodePosition(opPos);

		const XObjectPtr	theArg2(executeMore(context, opPos, executionContext));

		opPos = nextOpPos;

		nextOpPos = m_expression.getNextOpCodePosition(opPos);

		const XObjectPtr	theArg3(executeMore(context, opPos, executionContext));

		opPos = nextOpPos;

		assert(opPos == endFunc);

		return s_functions[funcID].execute(executionContext, context, theArg1, theArg2, theArg3, m_locator);
	}
	else
	{
		typedef XPathExecutionContext::XObjectArgVectorType		XObjectArgVectorType;

		XObjectArgVectorType	args;

		args.reserve(argCount);

		while(opPos < endFunc)
		{
			const int	nextOpPos = m_expression.getNextOpCodePosition(opPos);

			args.push_back(executeMore(context, opPos, executionContext));

			opPos = nextOpPos;
		}

		return function(context, funcID, args, executionContext);
	}
}



const XObjectPtr
XPath::locationPath(
			XPathExecutionContext&	executionContext,
			XalanNode&				context, 
			int 					opPos) const
{
	typedef XPathExecutionContext::BorrowReturnMutableNodeRefList	BorrowReturnMutableNodeRefList;

	BorrowReturnMutableNodeRefList	mnl(executionContext);

	step(executionContext, &context, opPos + 2, *mnl.get());

	return executionContext.getXObjectFactory().createNodeSet(mnl);
}



XPath::eMatchScore
XPath::locationPathPattern(
			XPathExecutionContext&	executionContext,
			XalanNode&				context, 
			int 					opPos) const
{
	eMatchScore	score = eMatchScoreNone;

	stepPattern(executionContext, &context, opPos + 2, score);

	return score;
}



void
XPath::step(
			XPathExecutionContext&	executionContext,
			XalanNode*				context, 
			int 					opPos,
			MutableNodeRefList& 	queryResults) const
{
	const XPathExpression&	currentExpression = getExpression();

	// int endStep = xpath.getNextOpPos(opPos);
	const int	stepType =
		currentExpression.getOpCodeMapValue(opPos);

	int 		argLen = 0;

	typedef XPathExecutionContext::BorrowReturnMutableNodeRefList	BorrowReturnMutableNodeRefList;

	BorrowReturnMutableNodeRefList	subQueryResults(executionContext);

	bool	shouldReorder = false;
	bool	continueStepRecursion = true;

	switch(stepType)
	{
	case XPathExpression::eOP_VARIABLE:
	case XPathExpression::eOP_EXTFUNCTION:
	case XPathExpression::eOP_FUNCTION:
	case XPathExpression::eOP_GROUP:
		argLen = findNodeSet(executionContext, context, opPos, stepType, *subQueryResults);
		break;

	case XPathExpression::eFROM_ROOT:
		argLen = findRoot(executionContext, context, opPos, stepType, *subQueryResults);
		break;

	case XPathExpression::eFROM_PARENT:
		argLen = findParent(executionContext, context, opPos, stepType, *subQueryResults);
		break;

	case XPathExpression::eFROM_SELF:
		argLen = findSelf(executionContext, context, opPos, stepType, *subQueryResults);
		break;

	case XPathExpression::eFROM_ANCESTORS:
		argLen = findAncestors(executionContext, context, opPos, stepType, *subQueryResults);
		shouldReorder = true;
		break;

	case XPathExpression::eFROM_ANCESTORS_OR_SELF:
		argLen = findAncestorsOrSelf(executionContext, context, opPos, stepType, *subQueryResults);
		shouldReorder = true;
		break;

	case XPathExpression::eMATCH_ATTRIBUTE:
		continueStepRecursion = false;
		// fall-through on purpose.

	case XPathExpression::eFROM_ATTRIBUTES:
		argLen = findAttributes(executionContext, context, opPos, stepType, *subQueryResults);
		break;

	case XPathExpression::eMATCH_ANY_ANCESTOR:
	case XPathExpression::eMATCH_IMMEDIATE_ANCESTOR:
	case XPathExpression::eMATCH_ANY_ANCESTOR_WITH_PREDICATE:
		continueStepRecursion = false;
		// fall-through on purpose.

	case XPathExpression::eFROM_CHILDREN:
		argLen = findChildren(executionContext, context, opPos, stepType, *subQueryResults);
		break;

	case XPathExpression::eFROM_DESCENDANTS:
	case XPathExpression::eFROM_DESCENDANTS_OR_SELF:
		argLen = findDescendants(executionContext, context, opPos, stepType, *subQueryResults);
		break;

	case XPathExpression::eFROM_FOLLOWING:
		argLen = findFollowing(executionContext, context, opPos, stepType, *subQueryResults);
		break;

	case XPathExpression::eFROM_FOLLOWING_SIBLINGS:
		argLen = findFollowingSiblings(executionContext, context, opPos, stepType, *subQueryResults);
		break;

	case XPathExpression::eFROM_PRECEDING:
		argLen = findPreceeding(executionContext, context, opPos, stepType, *subQueryResults);
		shouldReorder = true;
		break;

	case XPathExpression::eFROM_PRECEDING_SIBLINGS:
		argLen = findPreceedingSiblings(executionContext, context, opPos, stepType, *subQueryResults);
		shouldReorder = true;
		break;

	case XPathExpression::eFROM_NAMESPACE:
		argLen = findNamespace(executionContext, context, opPos,  stepType, *subQueryResults);
		break;

	default:
		argLen = findNodesOnUnknownAxis(executionContext, context, opPos, stepType, *subQueryResults);
		break;
	}

	// Push and pop the PrefixResolver...
	XPathExecutionContext::ContextNodeListSetAndRestore		theSetAndRestore(
									executionContext,
									*subQueryResults);

	opPos += argLen;

	int		nextStepType = currentExpression.getOpCodeMapValue(opPos);

	if(XPathExpression::eOP_PREDICATE == nextStepType)
	{
		predicates(executionContext,
				   context,
				   opPos, 
				   *subQueryResults,
				   opPos);

		nextStepType = currentExpression.getOpCodeMapValue(opPos);
	}

	if(XPathExpression::eENDOP != nextStepType && continueStepRecursion == true)
	{
		const NodeRefListBase::size_type	nContexts = subQueryResults->getLength();

		for(NodeRefListBase::size_type i = 0; i < nContexts; i++)
		{
			XalanNode* const	node = subQueryResults->item(i);

			if(0 != node)
			{
				BorrowReturnMutableNodeRefList	mnl(executionContext);

				step(executionContext, node, opPos, *mnl);

				if (mnl->getLength() > 0)
				{
					if(queryResults.getLength() == 0)
					{
						queryResults = *mnl;
					}
					else
					{
						queryResults.addNodesInDocOrder(*mnl, executionContext);
					}
				}
			}
		}
	}
	else
	{
		if (shouldReorder == true)
		{
			queryResults.addNodesInDocOrder(*subQueryResults, executionContext);
		}
		else
		{
			queryResults = *subQueryResults;
		}
	}
}



XalanNode*
XPath::stepPattern(
			XPathExecutionContext&	executionContext,
			XalanNode*				context, 
			int 					opPos,
			eMatchScore& 			scoreHolder) const
{
	const XPathExpression&	currentExpression = getExpression();

	const int	endStep = currentExpression.getNextOpCodePosition(opPos);
	int 		nextStepType = currentExpression.getOpCodeMapValue(endStep);

	if(XPathExpression::eENDOP != nextStepType)
	{
		// Continue step via recursion...
		context = stepPattern(
						executionContext,
						context,
						endStep,
						scoreHolder);

		if(0 == context)
		{
			scoreHolder = eMatchScoreNone;

		}

		if (scoreHolder == eMatchScoreNone)
		{
			// !!!!!!!!!!!!! Big ugly return here !!!!!!!!!!!!!!!!!!!
			return 0;
		}

		scoreHolder = eMatchScoreOther;

		if (nextStepType != XPathExpression::eMATCH_ANY_ANCESTOR_WITH_FUNCTION_CALL)
		{
			context = DOMServices::getParentOfNode(*context);
		}

		if(0 == context)
		{
			// !!!!!!!!!!!!! Big ugly return here !!!!!!!!!!!!!!!!!!!
			return 0;
		}
	}

	assert(context != 0);

	int					argLen = 0;

	eMatchScore	score = eMatchScoreNone;

	const int	startOpPos = opPos;
	const int	stepType = currentExpression.getOpCodeMapValue(opPos);

	switch(stepType)
	{
	case XPathExpression::eMATCH_ANY_ANCESTOR_WITH_FUNCTION_CALL:
		score = scoreHolder;
		break;

	case XPathExpression::eOP_FUNCTION:
		{
			argLen = currentExpression.getOpCodeLengthFromOpMap(opPos);

			const XObjectPtr		obj(executeMore(context, opPos, executionContext));
			assert(obj.get() != 0);

			const NodeRefListBase&				nl = obj->nodeset();

			const NodeRefListBase::size_type	len = nl.getLength();

			if (nextStepType == XPathExpression::eMATCH_ANY_ANCESTOR_WITH_FUNCTION_CALL)
			{
				bool	fFound = false;

				while(context != 0 && fFound == false)
				{
					for(NodeRefListBase::size_type i = 0; i < len; i++)
					{
						XalanNode* const	n = nl.item(i);

						if(n == context)
						{
							score = eMatchScoreOther;

							context = n;

							fFound = true;

							break;
						}
					}

					context = DOMServices::getParentOfNode(*context);
				}
			}
			else
			{
				for(NodeRefListBase::size_type i = 0; i < len; i++)
				{
					XalanNode* const	n = nl.item(i);

					if(n == context)
					{
						score = eMatchScoreOther;

						context = n;

						break;
					}
				}
			}
		}
		break;

	case XPathExpression::eFROM_ROOT:
		{
			// $$ ToDO: Can we reduce this to some call on the
			// XPathExpression interface?
			argLen =
				currentExpression.getOpCodeMapValue(opPos + XPathExpression::s_opCodeMapLengthIndex + 1) - 3;

			opPos += 3;

			const XalanNode::NodeType	nodeType = context->getNodeType();

			if (nodeType == XalanNode::DOCUMENT_NODE ||
				nodeType == XalanNode::DOCUMENT_FRAGMENT_NODE)
			{
				score = eMatchScoreOther;
			}
			else
			{
				const int   prevPos = currentExpression.getNextOpCodePosition(startOpPos);		
				const int	prevStepType = currentExpression.getOpCodeMapValue(prevPos);

				if (eMatchScoreNone == score  && 
				    (prevStepType == XPathExpression::eMATCH_ANY_ANCESTOR ||
					 prevStepType == XPathExpression::eMATCH_ANY_ANCESTOR_WITH_PREDICATE))
				{
					const NodeTester	theTester(
									*this,
									executionContext,
									opPos,
									argLen,
									stepType);

					while(0 != context)
					{
						score =
							theTester(*context, context->getNodeType());
							assert(score == nodeTest(executionContext, context, context->getNodeType(), opPos, argLen, stepType));

						if(eMatchScoreNone != score)
							break;

						context = DOMServices::getParentOfNode(*context);
					}
				}
			}
		}
		break;

	case XPathExpression::eMATCH_ATTRIBUTE:
		{
			// $$ ToDO: Can we reduce this to some call on the
			// XPathExpression interface?
			argLen =
				currentExpression.getOpCodeMapValue(opPos + XPathExpression::s_opCodeMapLengthIndex + 1) - 3;

			opPos += 3;

			score = NodeTester(
							*this,
							executionContext,
							opPos,
							argLen,
							XPathExpression::eFROM_ATTRIBUTES)(*context, context->getNodeType());

			assert(score == nodeTest(
							 executionContext,
							 context,
							 context->getNodeType(),
							 opPos,
							 argLen,
							 XPathExpression::eFROM_ATTRIBUTES));
		}
		break;

	case XPathExpression::eMATCH_ANY_ANCESTOR:
	case XPathExpression::eMATCH_ANY_ANCESTOR_WITH_PREDICATE:
		{
			// $$ ToDO: Can we reduce this to some call on the
			// XPathExpression interface?
			argLen =
					currentExpression.getOpCodeMapValue(opPos + XPathExpression::s_opCodeMapLengthIndex + 1) - 3;

			XalanNode::NodeType		nodeType = context->getNodeType();

			if(nodeType != XalanNode::ATTRIBUTE_NODE)
			{
				opPos += 3;

				const NodeTester	theTester(
									*this,
									executionContext,
									opPos,
									argLen,
									stepType);

				for(;;)
				{
					score = theTester(*context, nodeType);
					assert(score == nodeTest(executionContext, context, nodeType, opPos, argLen, stepType));

					if(eMatchScoreNone != score)
						break;

					context = DOMServices::getParentOfNode(*context);

					if (context == 0)
						break;

					nodeType = context->getNodeType();
				}
			}
		}
		break;

	case XPathExpression::eMATCH_IMMEDIATE_ANCESTOR:
		{
			// $$ ToDO: Can we reduce this to some call on the
			// XPathExpression interface?
			argLen =
				currentExpression.getOpCodeMapValue(opPos + XPathExpression::s_opCodeMapLengthIndex + 1) - 3;

			const XalanNode::NodeType	nodeType = context->getNodeType();

			if(nodeType != XalanNode::ATTRIBUTE_NODE)
			{
				opPos += 3;

				score = NodeTester(
								*this,
								executionContext,
								opPos,
								argLen,
								XPathExpression::eMATCH_IMMEDIATE_ANCESTOR)(*context, nodeType);

				assert(score == nodeTest(
								 executionContext,
								 context,
								 nodeType,
								 opPos,
								 argLen,
								 XPathExpression::eMATCH_IMMEDIATE_ANCESTOR));
			}
		}
		break;

	default:
		executionContext.error(TranscodeFromLocalCodePage("unknown match operation!"), context, getLocator());
		break;
	}

	opPos += argLen;

	nextStepType = currentExpression.getOpCodeMapValue(opPos);

	if(score != eMatchScoreNone && XPathExpression::eOP_PREDICATE == nextStepType)
	{
		score = eMatchScoreOther;

		// Execute the xpath.predicates, but if we have an index, then we have 
		// to start over and do a search from the parent.  It would be nice 
		// if I could sense this condition earlier...
		try
		{
			executionContext.setThrowFoundIndex(true);

			while(XPathExpression::eOP_PREDICATE == nextStepType)
			{
				const XObjectPtr		pred(predicate(context, opPos, executionContext));
				assert(pred.get() != 0);

				if(XObject::eTypeNumber == pred->getType())
				{
					score = handleFoundIndex(executionContext, context, startOpPos);
				}
				else if(pred->boolean() == false)
				{
					score = eMatchScoreNone;

					break;
				}

				opPos = currentExpression.getNextOpCodePosition(opPos);
				nextStepType = currentExpression.getOpCodeMapValue(opPos);
			}

			executionContext.setThrowFoundIndex(false);
		}
		catch(const FoundIndex&)
		{
			score = handleFoundIndex(executionContext, context, startOpPos);
		}
	}

	if (scoreHolder == eMatchScoreNone || 
        score == eMatchScoreNone)
	{
		scoreHolder = score;
	}

	return score == eMatchScoreNone ? 0 : context;
}



XPath::eMatchScore
XPath::handleFoundIndex(
			XPathExecutionContext&	executionContext,
			XalanNode* 				localContext, 
			int 					startOpPos) const
{
	// We have an index somewhere in our pattern.  So, we have 
	// to do a full search for our step, using the parent as 
	// localContext, then see if the current localContext is found in the 
	// node set.  Seems crazy, but, so far, it seems like the 
	// easiest way.
	executionContext.setThrowFoundIndex(false);

	XalanNode* const	parentContext =
				DOMServices::getParentOfNode(*localContext);

	if (parentContext == 0)
	{
		return eMatchScoreNone;
	}
	else
	{
		typedef XPathExecutionContext::BorrowReturnMutableNodeRefList	BorrowReturnMutableNodeRefList;

		BorrowReturnMutableNodeRefList	mnl(executionContext);

		step(executionContext, parentContext, startOpPos, *mnl);

		if (mnl->indexOf(localContext) == MutableNodeRefList::npos)
		{
			return eMatchScoreNone;
		}
		else
		{
			return eMatchScoreOther;
		}
	}
}



int
XPath::findNodeSet(
			XPathExecutionContext&	executionContext,
			XalanNode*				context, 
			int 					opPos,
			int 					/* stepType */,
			MutableNodeRefList& 	subQueryResults) const
{
	const XPathExpression&	currentExpression = getExpression();

	const XObjectPtr		obj(executeMore(context, opPos, executionContext));

	const NodeRefListBase&	nl = obj->nodeset();

	subQueryResults.addNodes(nl);

	return currentExpression.getOpCodeLengthFromOpMap(opPos);
}



int
XPath::findRoot(
			XPathExecutionContext&	/* executionContext */,
			XalanNode*				context, 
			int 					opPos,
			int 					/* stepType */,
			MutableNodeRefList& 	subQueryResults) const
{
	const XPathExpression&	currentExpression = getExpression();

	// $$ ToDO: Can we reduce this to some call on the
	// XPathExpression interface?
	const int	argLen =
		currentExpression.getOpCodeMapValue(opPos + XPathExpression::s_opCodeMapLengthIndex + 1) - 3;

	XalanNode* const	docContext = XalanNode::DOCUMENT_NODE == context->getNodeType() ?
									context :
									context->getOwnerDocument();
	assert(docContext != 0);

	subQueryResults.addNode(docContext);

	return argLen + 3;
}



int
XPath::findParent(
			XPathExecutionContext&	executionContext,
			XalanNode*				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults) const
{
	assert(context != 0);

	const XPathExpression&	currentExpression = getExpression();

	// $$ ToDO: Can we reduce this to some call on the
	// XPathExpression interface?
	const int	argLen =
			currentExpression.getOpCodeMapValue(opPos + XPathExpression::s_opCodeMapLengthIndex + 1) - 3;

	XalanNode* const	theParent = DOMServices::getParentOfNode(*context);

	if(0 != theParent)
	{
		if(argLen > 0)
		{
			opPos += 3;

			const NodeTester	theTester(
							*this,
							executionContext,
							opPos,
							argLen,
							stepType);

			const eMatchScore	score = theTester(*theParent, theParent->getNodeType());
			assert(score == nodeTest(executionContext, theParent, theParent->getNodeType(), opPos, argLen, stepType));

			if(eMatchScoreNone != score)
			{
				subQueryResults.addNode(theParent);
			}
		}
		else
		{
			subQueryResults.addNode(theParent);
		}
	}

	subQueryResults.setDocumentOrder();

	return argLen + 3;
}



int
XPath::findSelf(
			XPathExecutionContext&	executionContext,
			XalanNode*				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults) const
{
	assert(context != 0);

	const XPathExpression&	currentExpression = getExpression();

	// $$ ToDO: Can we reduce this to some call on the
	// XPathExpression interface?
	const int	argLen =
		currentExpression.getOpCodeMapValue(opPos + XPathExpression::s_opCodeMapLengthIndex + 1) - 3;

	if(argLen == 0)
	{
		subQueryResults.addNode(context);
	}
	else
	{
		opPos += 3;

		const NodeTester	theTester(
						*this,
						executionContext,
						opPos,
						argLen,
						stepType);

		const eMatchScore	score =
			theTester(*context, context->getNodeType());
			assert(score == nodeTest(executionContext, context, context->getNodeType(), opPos, argLen, stepType));

		if(eMatchScoreNone != score)
		{
			subQueryResults.addNode(context);
		}
	}

	subQueryResults.setDocumentOrder();

	return argLen + 3;
}



int
XPath::findAncestors(
			XPathExecutionContext&	executionContext,
			XalanNode*				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults) const
{
	assert(context != 0);

	context = DOMServices::getParentOfNode(*context);

	const XPathExpression&	currentExpression = getExpression();

	// $$ ToDO: Can we reduce this to some call on the
	// XPathExpression interface?
	const int	argLen =
			currentExpression.getOpCodeMapValue(opPos + XPathExpression::s_opCodeMapLengthIndex + 1) - 3;

	if (context != 0)
	{
		opPos += 3;

		const NodeTester	theTester(
						*this,
						executionContext,
						opPos,
						argLen,
						stepType);

		do
		{
			const eMatchScore	score =
				theTester(*context, context->getNodeType());
				assert(score == nodeTest(executionContext, context, context->getNodeType(), opPos, argLen, stepType));

			if(eMatchScoreNone != score)
			{
				subQueryResults.addNode(context);
			}

			context = DOMServices::getParentOfNode(*context);
		} while(0 != context);

		subQueryResults.setReverseDocumentOrder();
	}

	return argLen + 3;
}



int
XPath::findAncestorsOrSelf(
			XPathExecutionContext&	executionContext,
			XalanNode*				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults) const
{
	assert(context != 0);

	const XPathExpression&	currentExpression = getExpression();

	// $$ ToDO: Can we reduce this to some call on the
	// XPathExpression interface?
	const int	argLen =
		currentExpression.getOpCodeMapValue(opPos + XPathExpression::s_opCodeMapLengthIndex + 1) - 3;

	opPos += 3;

	const NodeTester	theTester(
					*this,
					executionContext,
					opPos,
					argLen,
					stepType);

	do
	{
		const eMatchScore	score =
				theTester(*context, context->getNodeType());
				assert(score == nodeTest(executionContext, context, context->getNodeType(), opPos, argLen, stepType));

		if(eMatchScoreNone != score)
		{
			subQueryResults.addNode(context);
		}

		context = DOMServices::getParentOfNode(*context);
	} while(0 != context);

	subQueryResults.setReverseDocumentOrder();

	return argLen + 3;
}



int
XPath::findAttributes(
			XPathExecutionContext&	executionContext,
			XalanNode*				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults) const
{
	assert(context != 0);

	const XPathExpression&	currentExpression = getExpression();

	// $$ ToDO: Can we reduce this to some call on the
	// XPathExpression interface?
	const int	argLen =
				currentExpression.getOpCodeMapValue(opPos + XPathExpression::s_opCodeMapLengthIndex + 1) - 3;

	if(context->getNodeType() == XalanNode::ELEMENT_NODE)
	{
		const XalanNamedNodeMap* const	attributeList = context->getAttributes();

		if(attributeList != 0) 
		{
			opPos += 3;

			const unsigned int	nAttrs = attributeList->getLength();

			if (nAttrs != 0)
			{
				const NodeTester	theTester(
								*this,
								executionContext,
								opPos,
								argLen,
								stepType);

				for(unsigned int j = 0; j < nAttrs; j++)
				{
					XalanNode* const	theNode = attributeList->item(j);
					assert(theNode != 0 && theNode->getNodeType() == XalanNode::ATTRIBUTE_NODE);

					const eMatchScore	score =
						theTester(*theNode, XalanNode::ATTRIBUTE_NODE);
						assert(score == nodeTest(executionContext, theNode, XalanNode::ATTRIBUTE_NODE, opPos, argLen, stepType));

					if(eMatchScoreNone != score)
					{
						subQueryResults.addNode(theNode);
					}
				}
			}
		}
	}

	return argLen + 3;
}



int
XPath::findChildren(
			XPathExecutionContext&	executionContext,
			XalanNode*				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults) const
{
	assert(context != 0);

	XalanNode*	child = context->getFirstChild();

	const XPathExpression&	currentExpression = getExpression();

	// $$ ToDO: Can we reduce this to some call on the
	// XPathExpression interface?
	const int	argLen =
		currentExpression.getOpCodeMapValue(opPos + XPathExpression::s_opCodeMapLengthIndex + 1) - 3;

	if (child != 0)
	{
		opPos += 3;

		const NodeTester	theTester(
						*this,
						executionContext,
						opPos,
						argLen,
						stepType);

		do
		{
			const eMatchScore	score =
					theTester(*child, child->getNodeType());
					assert(score == nodeTest(executionContext, child, child->getNodeType(), opPos, argLen, stepType));

			if(eMatchScoreNone != score)
			{
				subQueryResults.addNode(child);
			}

			child = child->getNextSibling();
		} while(0 != child);

		subQueryResults.setDocumentOrder();
	}

	return argLen + 3;
}



int
XPath::findDescendants(
			XPathExecutionContext&	executionContext,
			XalanNode*				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults) const
{
	assert(context != 0);

	const XPathExpression&	currentExpression = getExpression();

	// $$ ToDO: Can we reduce this to some call on the
	// XPathExpression interface?
	const int	argLen =
		currentExpression.getOpCodeMapValue(opPos + XPathExpression::s_opCodeMapLengthIndex + 1) - 3;

	// Perform a pre-order traversal of descendents...
	XalanNode*	pos = context;

	opPos += 3;

	const NodeTester	theTester(
						*this,
						executionContext,
						opPos,
						argLen,
						stepType);

	do
	{					
		if(stepType == XPathExpression::eFROM_DESCENDANTS_OR_SELF ||
		   context != pos)
		{
			const eMatchScore	score =
				theTester(*pos, pos->getNodeType());
				assert(score == nodeTest(executionContext, pos, pos->getNodeType(), opPos, argLen, stepType));

			if(score != eMatchScoreNone)
			{
				subQueryResults.addNode(pos);
			}
		}

		XalanNode*	nextNode = pos->getFirstChild();

		while(0 == nextNode)
		{
			if(context == pos)
				break;

			nextNode = pos->getNextSibling();

			if(0 == nextNode)
			{
				pos = DOMServices::getParentOfNode(*pos);

				if(context == pos || pos == 0)
				{
					nextNode = 0;
					break;
				}
			}
		}

		pos = nextNode;
	} while(0 != pos);

	subQueryResults.setDocumentOrder();

	return argLen + 3;
}



int
XPath::findFollowing(
			XPathExecutionContext&	executionContext,
			XalanNode*				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults) const
{
	assert(context != 0);

	const XPathExpression&	currentExpression = getExpression();

	// $$ ToDO: Can we reduce this to some call on the
	// XPathExpression interface?
	const int	argLen =
		currentExpression.getOpCodeMapValue(opPos + XPathExpression::s_opCodeMapLengthIndex + 1) - 3;

	opPos += 3;

	// What fun...
	XalanDocument* const	doc = context->getOwnerDocument();

	XalanNode*				pos = context;

	const NodeTester	theTester(
					*this,
					executionContext,
					opPos,
					argLen,
					stepType);

	while(0 != pos)
	{
		XalanNode*	nextNode = 0;

		if(pos != context)
		{
			const eMatchScore	score =
				theTester(*pos, pos->getNodeType());
				assert(score == nodeTest(executionContext, pos, pos->getNodeType(), opPos, argLen, stepType));

			if(eMatchScoreNone != score)
			{
				subQueryResults.addNodeInDocOrder(pos, executionContext);
			}

			nextNode = pos->getFirstChild();
		}
		else
		{
			nextNode = 0;
		}

		while(0 == nextNode)
		{
			// This requires some explanation.  pos could be an attribute
			// node, so getNextSibling() will always return 0.  In that
			// case, I want to continue the search with the first child of
			// the owner element, as if attribute nodes are children which
			// are always _before_ the first child element.  I don't have to
			// consider following attributes, since they _never_ match the
			// following axis.
			if (pos->getNodeType() == XalanNode::ATTRIBUTE_NODE)
			{
				assert(DOMServices::getParentOfNode(*pos) != 0);

				nextNode = DOMServices::getParentOfNode(*pos)->getFirstChild();
			}
			else
			{
				nextNode = pos->getNextSibling();
			}

			if(0 == nextNode)
			{
				pos = DOMServices::getParentOfNode(*pos);

				if(doc == pos || 0 == pos)
				{
					nextNode = 0;

					break;
				}
			}
		}

		pos = nextNode;
	}

	subQueryResults.setDocumentOrder();

	return argLen + 3;
}



int
XPath::findFollowingSiblings(
			XPathExecutionContext&	executionContext,
			XalanNode*				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults) const
{
	assert(context != 0);

	const XPathExpression&	currentExpression = getExpression();

	// $$ ToDO: Can we reduce this to some call on the
	// XPathExpression interface?
	const int	argLen =
		currentExpression.getOpCodeMapValue(opPos + XPathExpression::s_opCodeMapLengthIndex + 1) - 3;

	XalanNode*	pos = context->getNextSibling();

	if (pos != 0)
	{
		opPos += 3;

		const NodeTester	theTester(
						*this,
						executionContext,
						opPos,
						argLen,
						stepType);

		do
		{
			const eMatchScore	score =
					theTester(*pos, pos->getNodeType());
					assert(score == nodeTest(executionContext, pos, pos->getNodeType(), opPos, argLen, stepType));

			if(eMatchScoreNone != score)
			{
				subQueryResults.addNode(pos);
			}

			pos = pos->getNextSibling();
		} while(0 != pos);

		subQueryResults.setDocumentOrder();
	}

	return argLen + 3;
}



/*
 * This is bit of a hack to figure out where we should start for
 * the preceeding axis.  It's mostly to support RTFs which have
 * been coerced to nodesets through the nodeset() extension function.
 */
static XalanNode*
findTopNode(
			XalanNode*				context,
			XalanNode::NodeType		theType)
{
	assert(context != 0);

	if (theType == XalanNode::DOCUMENT_NODE)
	{
		return context;
	}
	else
	{
		XalanDocument* const	theDocument = context->getOwnerDocument();
		assert(theDocument != 0);

		if (theDocument->getDocumentElement() != 0)
		{
			return theDocument;
		}
		else
		{
			XalanNode*	theNode = 0;

			while(context != 0 &&
				  context->getNodeType() != XalanNode::DOCUMENT_NODE)
			{
				theNode = context;

				context = DOMServices::getParentOfNode(*context);
			}

			assert(theNode != 0);

			return theNode;
		}
	}
}



int
XPath::findPreceeding(
			XPathExecutionContext&	executionContext,
			XalanNode*				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults) const
{
	assert(context != 0);

	const XPathExpression&	currentExpression = getExpression();

	// $$ ToDO: Can we reduce this to some call on the
	// XPathExpression interface?
	const int	argLen =
		currentExpression.getOpCodeMapValue(opPos + XPathExpression::s_opCodeMapLengthIndex + 1) - 3;

	opPos += 3;

	// Ugh.  Reverse document order, no parents, I guess.
	const XalanNode::NodeType	theType = context->getNodeType();

	XalanNode* const	topNode = findTopNode(context, theType);

	XalanNode*			pos = topNode;

	// If the context node is an attribute, we need to perform some
	// magic to stop the search at the appropriate point, which is when
	// we arrive back at the parent.
	const bool				contextIsAttribute =
			theType == XalanNode::ATTRIBUTE_NODE ? true : false;

	const XalanNode* const	theAttributeContextParent =
		contextIsAttribute == true ? DOMServices::getParentOfNode(*context) : 0;

	const NodeTester	theTester(
						*this,
						executionContext,
						opPos,
						argLen,
						stepType);

	while(0 != pos)
	{
		if(context == pos)
		{
			break;
		}

		const eMatchScore	score =
				theTester(*pos, pos->getNodeType());
				assert(score == nodeTest(executionContext, pos, pos->getNodeType(), opPos, argLen, stepType));

		if(eMatchScoreNone != score)
		{
			// Ugh. If I could think a little better tonight, I'm
			// sure there's a better way to check for the parent.
			bool		isParent = false;

			XalanNode*	parent = DOMServices::getParentOfNode(*context);

			while(0 != parent)
			{
				if(parent == pos)
				{
					isParent = true;
					break;
				}

				parent = DOMServices::getParentOfNode(*parent);
			}

			if(isParent == false)
			{
				subQueryResults.addNode(pos);
			}
		}

		XalanNode*	nextNode = 0;

		// Check to see if we're back at the attribute context node's
		// parent, in which case, we should stop.
		if (contextIsAttribute == true &&
			pos == theAttributeContextParent)
		{
			nextNode = context;
		}
		else
		{
			nextNode = pos->getFirstChild();
		}

		while(0 == nextNode)
		{
			nextNode = pos->getNextSibling();

			if(0 == nextNode)
			{
				pos = DOMServices::getParentOfNode(*pos);

				if(topNode == pos)
				{
					nextNode = 0;
					break;
				}
			}
		}

		pos = nextNode;
	}

	// Now, reverse the order of the nodes, since
	// preceeding is a reverse axis, and we searched
	// the document from the root to this node.
	subQueryResults.reverse();

	subQueryResults.setReverseDocumentOrder();

	return argLen + 3;
}



int
XPath::findPreceedingSiblings(
			XPathExecutionContext&	executionContext,
			XalanNode*				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults) const
{
	assert(context != 0);

	const XPathExpression&	currentExpression = getExpression();

	// $$ ToDO: Can we reduce this to some call on the
	// XPathExpression interface?
	const int	argLen =
		currentExpression.getOpCodeMapValue(opPos + XPathExpression::s_opCodeMapLengthIndex + 1) - 3;

	XalanNode*	pos = context->getPreviousSibling();

	if (pos != 0)
	{
		opPos += 3;

		const NodeTester	theTester(
						*this,
						executionContext,
						opPos,
						argLen,
						stepType);

		do
		{
			const eMatchScore	score = 
				theTester(*pos, pos->getNodeType());
				assert(score == nodeTest(executionContext, pos, pos->getNodeType(), opPos, argLen, stepType));

			if(eMatchScoreNone != score)
			{
				subQueryResults.addNode(pos);
			}

			pos = pos->getPreviousSibling();
		} while(0 != pos);

		subQueryResults.setReverseDocumentOrder();
	}

	return argLen + 3;
}



int
XPath::findNamespace(
			XPathExecutionContext&	executionContext,
			XalanNode*				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults) const
{
	assert(context != 0);

	const XPathExpression&	currentExpression = getExpression();

	// $$ ToDO: Can we reduce this to some call on the
	// XPathExpression interface?
	const int	argLen =
		currentExpression.getOpCodeMapValue(opPos + XPathExpression::s_opCodeMapLengthIndex + 1) - 3;

	if(context->getNodeType() == XalanNode::ELEMENT_NODE)
	{
		opPos += 3;

		// Look up the element chain until we hit the document, so that we
		// get all of the attribute/namespace nodes.
		const XalanNode* const	theOwnerDocument = context->getOwnerDocument();
		assert(theOwnerDocument != 0);

		const XalanNode*		theCurrentNode = context;

		const NodeTester	theTester(
						*this,
						executionContext,
						opPos,
						argLen,
						stepType);

		do
		{
			const XalanNamedNodeMap* const	attributeList =
				theCurrentNode->getAttributes();

			if(attributeList != 0) 
			{
				const unsigned int	nAttrs = attributeList->getLength();

				for(unsigned int i = 0; i < nAttrs; ++i)
				{
					XalanNode* const	attr = attributeList->item(i);
					assert(attr != 0 && attr->getNodeType() == XalanNode::ATTRIBUTE_NODE);

					const XalanDOMString&	theNodeName = attr->getNodeName();

					// This is an optimization to keep non-namespace attributes out of
					// the call to nodeTest().
					if (startsWith(theNodeName, DOMServices::s_XMLNamespaceWithSeparator) == true ||
						::equals(theNodeName, DOMServices::s_XMLNamespace) == true)
					{
						const eMatchScore	score =
							theTester(*attr, XalanNode::ATTRIBUTE_NODE);
							assert(score == nodeTest(executionContext, attr, XalanNode::ATTRIBUTE_NODE, opPos, argLen, stepType));

						if(score != eMatchScoreNone)
						{
							subQueryResults.addNode(attr);
						}
					}
				}
			}

			theCurrentNode = theCurrentNode->getParentNode();
		} while (theCurrentNode != theOwnerDocument && theCurrentNode != 0);
	}

	return argLen + 3;
}



int
XPath::findNodesOnUnknownAxis(
			XPathExecutionContext&	executionContext,
			XalanNode*				context, 
			int 					opPos,
			int 					/* stepType */,
			MutableNodeRefList& 	/* subQueryResults */) const
{
	const XPathExpression&	currentExpression = getExpression();

	// $$ ToDO: Can we reduce this to some call on the
	// XPathExpression interface?
	const int	argLen =
		currentExpression.getOpCodeMapValue(opPos + XPathExpression::s_opCodeMapLengthIndex + 1) - 3;

	executionContext.error(TranscodeFromLocalCodePage("Unknown axis!"), context, getLocator());

	return argLen + 3;
}



XPath::eMatchScore
XPath::nodeTest(
			XPathExecutionContext&	executionContext,
			XalanNode* 				context,
			XalanNode::NodeType		nodeType,
			int 					opPos,
			int 					argLen,
			int 					stepType) const
{
	assert(context->getNodeType() == nodeType);

	const XPathExpression&	currentExpression = getExpression();

	eMatchScore		score = eMatchScoreNone;

	const int	testType = currentExpression.getOpCodeMapValue(opPos);

	switch(testType)
	{
	case XPathExpression::eNODETYPE_COMMENT:
		if (XalanNode::COMMENT_NODE == nodeType)
		{
			score = eMatchScoreNodeTest;
		}
		break;

	case XPathExpression::eNODETYPE_TEXT:
		if ((XalanNode::CDATA_SECTION_NODE == nodeType ||
			 XalanNode::TEXT_NODE == nodeType) &&
			executionContext.shouldStripSourceNode(*context) == false)
		{
			  score = eMatchScoreNodeTest;
		}
	  break;

	case XPathExpression::eNODETYPE_PI:
		if(XalanNode::PROCESSING_INSTRUCTION_NODE == nodeType)
		{
			if(argLen == 1)
			{
				score = eMatchScoreNodeTest;
			}
			else if(argLen == 2)
			{
				opPos++;

				const int				tokenPosition =
					currentExpression.getOpCodeMapValue(opPos);

				const XObject* const	name =
					currentExpression.getToken(tokenPosition);
				assert(name != 0);

				if (::equals(context->getNodeName(), name->str()) == true)
				{
					score = eMatchScoreQName;
				}
			}
			else
			{
				executionContext.error(TranscodeFromLocalCodePage("Arg length of processing-instruction() node test is incorrect!"), context, getLocator());
			}
		}

		break;

	case XPathExpression::eNODETYPE_NODE:
		if (nodeType == XalanNode::CDATA_SECTION_NODE ||
			nodeType == XalanNode::TEXT_NODE)
		{
			if (executionContext.shouldStripSourceNode(*context) == false)
			{
				score = eMatchScoreNodeTest;
			}
		}
		else
		{
			score = eMatchScoreNodeTest;
		}
		break;

	case XPathExpression::eNODETYPE_ROOT:
		if (XalanNode::DOCUMENT_FRAGMENT_NODE == nodeType ||
			XalanNode::DOCUMENT_NODE == nodeType)
		{
			score =  eMatchScoreOther;
		}
		break;

	case XPathExpression::eNODENAME:
		{
			opPos++;

			if (nodeType == XalanNode::ATTRIBUTE_NODE || nodeType == XalanNode::ELEMENT_NODE)
			{
				bool					test = false;

				int 					queueIndex = currentExpression.getOpCodeMapValue(opPos);

				const XalanDOMString&	targetNS = queueIndex >= 0 ?
										currentExpression.getToken(queueIndex)->str() :
											s_emptyString;

				opPos++;

				// From the draft: "Two expanded names are equal if they 
				// have the same local part, and either both have no URI or 
				// both have the same URI."
				// "A node test * is true for any node of the principal node type. 
				// For example, child::* will select all element children of the 
				// context node, and attribute::* will select all attributes of 
				// the context node."
				// "A node test can have the form NCName:*. In this case, the prefix 
				// is expanded in the same way as with a QName using the context 
				// namespace declarations. The node test will be true for any node 
				// of the principal type whose expanded name has the URI to which 
				// the prefix expands, regardless of the local part of the name."
				const bool	isTotallyWild =
							0 == length(targetNS) &&
							currentExpression.getOpCodeMapValue(opPos) == XPathExpression::eELEMWILDCARD;

				bool		didMatchNS = false;

				if(isTotallyWild == false)
				{
					const XalanDOMString&	contextNS = DOMServices::getNamespaceOfNode(*context);

					if(0 != length(targetNS) && 0 != length(contextNS))
					{
						test = ::equals(contextNS, targetNS);

						didMatchNS = true;
					}
					else
					{
						test = XPathExpression::eELEMWILDCARD == queueIndex || 
						   (0 == length(contextNS) && 0 == length(targetNS));
					}
				}
				else
				{
					test = true;
				}

				queueIndex = currentExpression.getOpCodeMapValue(opPos);

				if(test == true)
				{
					switch(nodeType)
					{
					case XalanNode::ATTRIBUTE_NODE:
						if(stepType == XPathExpression::eFROM_ATTRIBUTES ||
							stepType == XPathExpression::eFROM_NAMESPACE)
						{
							assert(context->getNodeType() == XalanNode::ATTRIBUTE_NODE);

							const XalanDOMString&	attrName =
										context->getNodeName();

							const bool				isNamespace =
									startsWith(attrName, DOMServices::s_XMLNamespaceWithSeparator) ||
									::equals(attrName, DOMServices::s_XMLNamespace);

							if(XPathExpression::eELEMWILDCARD == queueIndex)
							{
								if(stepType == XPathExpression::eFROM_ATTRIBUTES)
								{
									if (isNamespace == false)
									{
										score = eMatchScoreNodeTest;
									}
								}
								else
								{
									if (isNamespace == true)
									{
										score = eMatchScoreNodeTest;
									}
								}
							}
							else
							{
								if(stepType == XPathExpression::eFROM_ATTRIBUTES)
								{
									if (isNamespace == false)
									{
										assert(queueIndex >= 0);

										const XalanDOMString&	targetLocalName =
															currentExpression.getToken(queueIndex)->str();

										const XalanDOMString&	localAttrName =
												DOMServices::getLocalNameOfNode(*context);

										if (::equals(localAttrName, targetLocalName) == true)
										{
											score = eMatchScoreQName;
										}
									}
								}
								else
								{
									if (isNamespace == true)
									{
										const XalanAttr* const	theAttrNode =
#if defined(XALAN_OLD_STYLE_CASTS)
											(const XalanAttr*)context;
#else
											static_cast<const XalanAttr*>(context);
#endif
										assert(theAttrNode != 0);

										const XalanDOMString&	theNamespace =
													theAttrNode->getValue();

										assert(queueIndex >= 0);

										const XalanDOMString&	targetLocalName =
															currentExpression.getToken(queueIndex)->str();

										if (::equals(theNamespace, targetLocalName) == true)
										{
											score = eMatchScoreQName;
										}
									}
								}
							}
						}
						break;

					case XalanNode::ELEMENT_NODE:
						if(stepType != XPathExpression::eFROM_ATTRIBUTES)
						{
							if(XPathExpression::eELEMWILDCARD == queueIndex)
							{
								score = didMatchNS == true ?
									eMatchScoreNSWild : eMatchScoreNodeTest;
							}
							else
							{
								assert(queueIndex >= 0);

								const XalanDOMString&	targetLocalName =
															currentExpression.getToken(queueIndex)->str();

								if (::equals(DOMServices::getLocalNameOfNode(*context),
										   targetLocalName) == true)
								{
									score = eMatchScoreQName;
								}
							}
						}
						break;

					default:
						// Trying to match on anything else causes nasty bugs.
						break;
					} // end switch(nodeType)
				} // end if(test)
			} // end if (nodeType == XalanNode::ATTRIBUTE_NODE || nodeType == XalanNode::ELEMENT_NODE)
		} // end case XPathExpression::eNODENAME
		break;

	default:
		break;
	} // end switch(testType)

	return score;
}



void
XPath::predicates(
			XPathExecutionContext&	executionContext,
			XalanNode*				/* context */, 
			int 					opPos,
			MutableNodeRefList& 	subQueryResults,
			int&					endPredicatesPos) const
{
	const XPathExpression&	currentExpression = getExpression();

	int 	nextStepType =
			currentExpression.getOpCodeMapValue(opPos);

	while(XPathExpression::eOP_PREDICATE == nextStepType)
	{
		NodeRefListBase::size_type 			i = 0;

		const NodeRefListBase::size_type	theLength = subQueryResults.getLength();

		while(i < theLength)
		{
			XalanNode* const	theNode = subQueryResults.item(i);
			assert(theNode != 0);

			const XObjectPtr		pred(predicate(theNode, opPos, executionContext));
			assert(pred.get() != 0);

			// Remove any node that doesn't satisfy the predicate.
			if((XObject::eTypeNumber == pred->getType() && i + 1 != pred->num()) ||
			   pred->boolean() == false)
			{
				// Set the node to 0.  After we're done,
				// we'll clear it out.
				subQueryResults.setNode(i, 0);
			}

			++i;
		}

		// Clear out any null entries...
		subQueryResults.clearNulls();

		opPos = currentExpression.getNextOpCodePosition(opPos);

		nextStepType = currentExpression.getOpCodeMapValue(opPos);

		if(XPathExpression::eOP_PREDICATE == nextStepType)
		{
			executionContext.setContextNodeList(subQueryResults);

			// Don't break, loop 'till end so that opPos will be set to end.
			// if(0 == subQueryResults.getLength())
			//	break;
		}
	}

	endPredicatesPos = opPos;
}



inline const XalanDOMString*
getStringFromTokenQueue(
			const XPathExpression&	expression,
			int						opPos)
{
	const int	tokenPosition =
				expression.getOpCodeMapValue(opPos);

	if (tokenPosition < 0)
	{
		return 0;
	}
	else
	{
		const XObject* const	token =
					expression.getToken(tokenPosition);
		assert(token != 0);

		return &token->str();
	}
}



XPath::NodeTester::NodeTester(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			int 					opPos,
			int 					argLen,
			int 					stepType) :
	m_executionContext(executionContext),
	m_targetNamespace(0),
	m_targetLocalName(0),
	m_testFunction(0)
{
	const XPathExpression&	theExpression = xpath.getExpression();

	switch(theExpression.getOpCodeMapValue(opPos))
	{
	case XPathExpression::eNODETYPE_COMMENT:
		m_testFunction = &NodeTester::testComment;
		break;

	case XPathExpression::eNODETYPE_TEXT:
		m_testFunction = &NodeTester::testText;
		break;

	case XPathExpression::eNODETYPE_PI:
		if (argLen == 1)
		{
			m_testFunction = &NodeTester::testPI;
		}
		else if(argLen == 2)
		{
			m_testFunction = &NodeTester::testPIName;

			m_targetLocalName = getStringFromTokenQueue(
				theExpression,
				opPos + 1);
		}
		else
		{
			executionContext.error(TranscodeFromLocalCodePage("Arg length of processing-instruction() node test is incorrect!"), 0, xpath.getLocator());
		}
		break;

	case XPathExpression::eNODETYPE_NODE:
		m_testFunction = &NodeTester::testNode;
		break;

	case XPathExpression::eNODETYPE_ROOT:
		m_testFunction = &NodeTester::testRoot;
		break;

	case XPathExpression::eNODENAME:
		{
			bool	isTotallyWild = false;

			m_targetNamespace = getStringFromTokenQueue(
					theExpression,
					opPos + 1);

			if (m_targetNamespace == 0 &&
				theExpression.getOpCodeMapValue(opPos + 2) == XPathExpression::eELEMWILDCARD)
			{
				isTotallyWild = true;
			}
			else
			{
				m_targetLocalName = getStringFromTokenQueue(
					theExpression,
					opPos + 2);
			}

			if(stepType == XPathExpression::eFROM_ATTRIBUTES)
			{
				if (isTotallyWild == true)
				{
					m_testFunction = &NodeTester::testAttributeTotallyWild;
				}
				else if (m_targetNamespace == 0)
				{
					assert(m_targetLocalName != 0);

					m_testFunction = &NodeTester::testAttributeNCName;
				}
				else if (m_targetLocalName == 0)
				{
					assert(m_targetNamespace != 0);

					m_testFunction = &NodeTester::testAttributeNamespaceOnly;
				}
				else
				{
					assert(m_targetNamespace != 0 && m_targetLocalName != 0);


					m_testFunction = &NodeTester::testAttributeQName;
				}
			}
			else if (stepType == XPathExpression::eFROM_NAMESPACE)
			{
				if (isTotallyWild == true)
				{
					m_testFunction = &NodeTester::testNamespaceTotallyWild;
				}
				else
				{
					m_testFunction = &NodeTester::testNamespaceNCName;
				}
			}
			else
			{
				if (isTotallyWild == true)
				{
					m_testFunction = &NodeTester::testElementTotallyWild;
				}
				else if (m_targetNamespace == 0)
				{
					m_testFunction = &NodeTester::testElementNCName;
				}
				else if (m_targetLocalName == 0)
				{
					assert(m_targetNamespace != 0);

					m_testFunction = &NodeTester::testElementNamespaceOnly;
				}
				else
				{
					assert(m_targetNamespace != 0 && m_targetLocalName != 0);

					m_testFunction = &NodeTester::testElementQName;
				}
			}
		}
		break;

	default:
		m_testFunction = &NodeTester::testDefault;
		break;
	}

	assert(m_testFunction != 0);
}



XPath::eMatchScore
XPath::NodeTester::testComment(
			const XalanNode& 		/* context */,
			XalanNode::NodeType		nodeType) const
{
	if (XalanNode::COMMENT_NODE == nodeType)
	{
		return eMatchScoreNodeTest;
	}
	else
	{
		return eMatchScoreNone;
	}
}



XPath::eMatchScore
XPath::NodeTester::testText(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const
{
	if ((XalanNode::CDATA_SECTION_NODE == nodeType || XalanNode::TEXT_NODE == nodeType) &&
		 shouldStripSourceNode(context) == false)
	{
		return eMatchScoreNodeTest;
	}
	else
	{
		return eMatchScoreNone;
	}
}



XPath::eMatchScore
XPath::NodeTester::testPI(
			const XalanNode& 		/* context */,
			XalanNode::NodeType		nodeType) const
{
	if (XalanNode::PROCESSING_INSTRUCTION_NODE == nodeType)
	{
		return eMatchScoreNodeTest;
	}
	else
	{
		return eMatchScoreNone;
	}
}



XPath::eMatchScore
XPath::NodeTester::testPIName(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const
{
	assert(m_targetLocalName != 0);

	if (XalanNode::PROCESSING_INSTRUCTION_NODE == nodeType &&
		::equals(context.getNodeName(), *m_targetLocalName) == true)
	{
		return eMatchScoreQName;
	}
	else
	{
		return eMatchScoreNone;
	}
}



XPath::eMatchScore
XPath::NodeTester::testNode(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const
{
	if ((nodeType != XalanNode::CDATA_SECTION_NODE && nodeType != XalanNode::TEXT_NODE) ||
		shouldStripSourceNode(context) == false)
	{
		return eMatchScoreNodeTest;
	}
	else
	{
		return eMatchScoreNone;
	}
}



XPath::eMatchScore
XPath::NodeTester::testRoot(
			const XalanNode& 		/* context */,
			XalanNode::NodeType		nodeType) const
{
	if (XalanNode::DOCUMENT_FRAGMENT_NODE == nodeType ||
		XalanNode::DOCUMENT_NODE == nodeType)
	{
		return eMatchScoreOther;
	}
	else
	{
		return eMatchScoreNone;
	}
}



inline bool
isNamespaceDeclaration(const XalanNode&		theAttributeNode)
{
	assert(theAttributeNode.getNodeType() == XalanNode::ATTRIBUTE_NODE);

#if defined(XALAN_OLD_STYLE_CASTS)
	return DOMServices::isNamespaceDeclaration((const XalanAttr&)theAttributeNode);
#else
	return DOMServices::isNamespaceDeclaration(static_cast<const XalanAttr&>(theAttributeNode));
#endif
}



XPath::eMatchScore
XPath::NodeTester::testAttributeNCName(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const
{
	assert(m_targetNamespace == 0 && m_targetLocalName != 0);

	if (XalanNode::ATTRIBUTE_NODE != nodeType ||
		isNamespaceDeclaration(context) == true ||
		matchLocalName(context) == false)
	{
		return eMatchScoreNone;
	}
	else
	{
		return eMatchScoreQName;
	}
}



XPath::eMatchScore
XPath::NodeTester::testAttributeQName(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const
{
	assert(m_targetNamespace != 0 && m_targetLocalName != 0);

	if (XalanNode::ATTRIBUTE_NODE != nodeType ||
		isNamespaceDeclaration(context) == true ||
		matchLocalNameAndNamespaceURI(context) == false)
	{
		return eMatchScoreNone;
	}
	else
	{
		return eMatchScoreQName;
	}
}



XPath::eMatchScore
XPath::NodeTester::testAttributeNamespaceOnly(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const
{
	assert(m_targetNamespace != 0 && m_targetLocalName == 0);

	if (XalanNode::ATTRIBUTE_NODE != nodeType ||
		isNamespaceDeclaration(context) == true ||
		matchNamespaceURI(context) == false)
	{
		return eMatchScoreNone;
	}
	else
	{
		return eMatchScoreNodeTest;
	}
}



XPath::eMatchScore
XPath::NodeTester::testAttributeTotallyWild(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const
{
	if (XalanNode::ATTRIBUTE_NODE != nodeType ||
		isNamespaceDeclaration(context) == true)
	{
		return eMatchScoreNone;
	}
	else
	{
		return eMatchScoreNodeTest;
	}
}



XPath::eMatchScore
XPath::NodeTester::testElementNCName(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const
{
	assert(m_targetNamespace == 0 && m_targetLocalName != 0);

	if (XalanNode::ELEMENT_NODE != nodeType ||
		matchLocalName(context) == false)
	{
		return eMatchScoreNone;
	}
	else
	{
		return eMatchScoreQName;
	}
}



XPath::eMatchScore
XPath::NodeTester::testElementQName(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const
{
	assert(m_targetNamespace != 0 && m_targetLocalName != 0);

	if (XalanNode::ELEMENT_NODE != nodeType ||
		matchLocalNameAndNamespaceURI(context) == false)
	{
		return eMatchScoreNone;
	}
	else
	{
		return eMatchScoreQName;
	}
}



XPath::eMatchScore
XPath::NodeTester::testElementNamespaceOnly(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const
{
	assert(m_targetNamespace != 0 && m_targetLocalName == 0);

	if (XalanNode::ELEMENT_NODE != nodeType ||
		matchNamespaceURI(context) == false)
	{
		return eMatchScoreNone;
	}
	else
	{
		return eMatchScoreNSWild;
	}
}



XPath::eMatchScore
XPath::NodeTester::testElementTotallyWild(
			const XalanNode& 		/* context */,
			XalanNode::NodeType		nodeType) const
{
	assert(m_targetNamespace == 0 && m_targetLocalName == 0);

	if (XalanNode::ELEMENT_NODE != nodeType)
	{
		return eMatchScoreNone;
	}
	else
	{
		return eMatchScoreNodeTest;
	}
}



XPath::eMatchScore
XPath::NodeTester::testNamespaceNCName(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const
{
	assert(m_targetNamespace == 0 && m_targetLocalName != 0);

	if (XalanNode::ATTRIBUTE_NODE != nodeType ||
		isNamespaceDeclaration(context) == false ||
		matchNamespace(context) == false)
	{
		return eMatchScoreNone;
	}
	else
	{
		return eMatchScoreQName;
	}
}



XPath::eMatchScore
XPath::NodeTester::testNamespaceTotallyWild(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const
{
	assert(m_targetNamespace == 0 && m_targetLocalName == 0);

	if (XalanNode::ATTRIBUTE_NODE != nodeType ||
		isNamespaceDeclaration(context) == false)
	{
		return eMatchScoreNone;
	}
	else
	{
		return eMatchScoreNodeTest;
	}
}



XPath::eMatchScore
XPath::NodeTester::testDefault(
			const XalanNode& 		/* context */,
			XalanNode::NodeType		/* nodeType */) const
{
	return eMatchScoreNone;
}



bool
XPath::NodeTester::matchLocalName(const XalanNode&	context) const
{
	assert(m_targetLocalName != 0);

	return length(context.getNamespaceURI()) == 0 &&
		::equals(DOMServices::getLocalNameOfNode(context), *m_targetLocalName);
}



bool
XPath::NodeTester::matchNamespaceURI(const XalanNode&	context) const
{
	assert(m_targetNamespace != 0);

	return ::equals(context.getNamespaceURI(), *m_targetNamespace);
}



bool
XPath::NodeTester::matchLocalNameAndNamespaceURI(const XalanNode&	context) const
{
	assert(m_targetNamespace != 0 && m_targetLocalName != 0);

	return ::equals(DOMServices::getLocalNameOfNode(context), *m_targetLocalName) == true &&
		::equals(context.getNamespaceURI(), *m_targetNamespace) == true;
}



bool
XPath::NodeTester::matchNamespace(const XalanNode&	context) const
{
	assert(m_targetLocalName != 0);

	return ::equals(context.getNodeValue(), *m_targetLocalName);
}



bool
XPath::NodeTester::shouldStripSourceNode(const XalanNode&	context) const
{
	assert(context.getNodeType() == XalanNode::CDATA_SECTION_NODE ||
		   context.getNodeType() == XalanNode::TEXT_NODE);

	return m_executionContext.shouldStripSourceNode(context);
}



static XalanDOMString	PSEUDONAME_ANY;
static XalanDOMString	PSEUDONAME_ROOT;
static XalanDOMString	PSEUDONAME_TEXT;
static XalanDOMString	PSEUDONAME_COMMENT;
static XalanDOMString	PSEUDONAME_PI;
static XalanDOMString	PSEUDONAME_OTHER;
static XalanDOMString	PSEUDONAME_NODE;



const XalanDOMString&	XPath::PSEUDONAME_ANY = ::PSEUDONAME_ANY;
const XalanDOMString&	XPath::PSEUDONAME_ROOT = ::PSEUDONAME_ROOT;
const XalanDOMString&	XPath::PSEUDONAME_TEXT = ::PSEUDONAME_TEXT;
const XalanDOMString&	XPath::PSEUDONAME_COMMENT = ::PSEUDONAME_COMMENT;
const XalanDOMString&	XPath::PSEUDONAME_PI = ::PSEUDONAME_PI;
const XalanDOMString&	XPath::PSEUDONAME_OTHER = ::PSEUDONAME_OTHER;
const XalanDOMString&	XPath::PSEUDONAME_NODE = ::PSEUDONAME_NODE;



// Don't auto-create the table...
XPath::FunctionTableType		XPath::s_functions(false);



void
XPath::initialize()
{
	s_functions.CreateTable();

	::PSEUDONAME_ANY = XALAN_STATIC_UCODE_STRING("*");
	::PSEUDONAME_ROOT = XALAN_STATIC_UCODE_STRING("/");
	::PSEUDONAME_TEXT = XALAN_STATIC_UCODE_STRING("#text");
	::PSEUDONAME_COMMENT = XALAN_STATIC_UCODE_STRING("#comment");
	::PSEUDONAME_PI = XALAN_STATIC_UCODE_STRING("#pi");
	::PSEUDONAME_OTHER = XALAN_STATIC_UCODE_STRING("*");
	::PSEUDONAME_NODE = XALAN_STATIC_UCODE_STRING("#node");
}



void
XPath::terminate()
{
	releaseMemory(::PSEUDONAME_ANY);
	releaseMemory(::PSEUDONAME_ROOT);
	releaseMemory(::PSEUDONAME_TEXT);
	releaseMemory(::PSEUDONAME_COMMENT);
	releaseMemory(::PSEUDONAME_PI);
	releaseMemory(::PSEUDONAME_OTHER);
	releaseMemory(::PSEUDONAME_NODE);

	s_functions.DestroyTable();
}
