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
// Class header file.
#include "XPath.hpp"



#include <cassert>
#include <cfloat>



#include <Include/DOMHelper.hpp>
#include <PlatformSupport/DoubleSupport.hpp>
#include <PlatformSupport/STLHelper.hpp>



#include "FoundIndex.hpp"
#include "MutableNodeRefList.hpp"
#include "QName.hpp"
#include "PrefixResolver.hpp"
#include "SimpleNodeLocator.hpp"
#include "XLocator.hpp"
#include "XPathException.hpp"
#include "XObject.hpp"
#include "XObjectFactory.hpp"
#include "XPathEnvSupport.hpp"
#include "XPathSupport.hpp"



#if !defined(XALAN_INLINE_INITIALIZATION)
const char* const				XPath::PSEUDONAME_ANY = "*";
const char* const				XPath::PSEUDONAME_ROOT = "/";
const char* const				XPath::PSEUDONAME_TEXT = "#text";
const char* const				XPath::PSEUDONAME_COMMENT = "#comment";
const char* const				XPath::PSEUDONAME_PI = "#pi";
const char* const				XPath::PSEUDONAME_OTHER = "*";

const double					XPath::s_MatchScoreNone = -9999999999999.0;
const double					XPath::s_MatchScoreQName = 0.0;
const double					XPath::s_MatchScoreNSWild = -0.25;
const double					XPath::s_MatchScoreNodeTest = -0.5;
const double					XPath::s_MatchScoreOther = 0.5;

#endif

XPath::FunctionTableType		XPath::s_functions;



XPath::XPath(bool	createDefaultLocator) :
	FactoryObject(),
	m_defaultXLocator(createDefaultLocator == false ? 0 : createXLocatorHandler()),
	m_expression()
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



XObject*
XPath::execute(XPathExecutionContext&	executionContext) const
{
	return execute(executionContext.getCurrentNode(), 0, executionContext);
}



void
XPath::installFunction(
			const DOMString&	funcName,
			const Function& 	func)
{
	s_functions.InstallFunction(funcName,
							    func);
}



XLocator*
XPath::createXLocatorHandler() const
{
	return SimpleNodeLocator::getDefaultInstance();
}



XObject*
XPath::execute(
			const DOM_Node&			context,
			const PrefixResolver&	prefixResolver,
			XPathExecutionContext&	executionContext) const
{
	const PrefixResolver* const		theSavedResolver =
			executionContext.getPrefixResolver();

	const DOM_Node					theSavedCurrentNode =
			executionContext.getCurrentNode();

	XObject*	theResult = 0;

	try
	{
		executionContext.setPrefixResolver(&prefixResolver);
		executionContext.setCurrentNode(context);

		theResult = execute(executionContext);
	}
	catch(...)
	{
		executionContext.setPrefixResolver(theSavedResolver);
		executionContext.setCurrentNode(theSavedCurrentNode);

		throw;
	}

	executionContext.setPrefixResolver(theSavedResolver);
	executionContext.setCurrentNode(theSavedCurrentNode);

	return theResult;
}



XObject*
XPath::execute(
			const DOM_Node&			context,
			const PrefixResolver&	prefixResolver,
			const NodeRefListBase&	contextNodeList,
			XPathExecutionContext&	executionContext) const
{
	const MutableNodeRefList	theSavedContextNodeList(executionContext.getContextNodeList());

	XObject*	theResult = 0;

	try
	{
		executionContext.setContextNodeList(contextNodeList);

		theResult = execute(context, prefixResolver, executionContext);
	}
	catch(...)
	{
		executionContext.setContextNodeList(theSavedContextNodeList);

		throw;
	}

	executionContext.setContextNodeList(theSavedContextNodeList);

	return theResult;
}



XObject*
XPath::execute(
			const DOM_Node& 		context,
			int 					opPos,
			XPathExecutionContext&	executionContext) const
{
	XObject*	result = 0;

	switch(m_expression.m_opMap[opPos])
	{
	case XPathExpression::eOP_XPATH:
		result = execute(context, opPos + 2, executionContext);
		break;

	case XPathExpression::eOP_MATCHPATTERN:
		result = matchPattern(context, opPos + 2, executionContext);
		break;

	case XPathExpression::eEMPTY:
		opPos++;
		break;

	case XPathExpression::eOP_OR:
		result = or(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_AND:
		result = and(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_NOTEQUALS:
		result = notequals(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_EQUALS:
		result = equals(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_LTE:
		result = lte(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_LT:
		result = lt(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_GTE:
		result = gte(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_GT:
		result = gt(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_PLUS:
		result = plus(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_MINUS:
	  result = minus(context, opPos, executionContext);
	  break;

	case XPathExpression::eOP_MULT:
		result = mult(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_DIV:
		result = div(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_MOD:
		result = mod(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_QUO:
		result = quo(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_NEG:
		result = neg(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_STRING:
		result = string(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_BOOL:
		result = boolean(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_NUMBER:
		result = number(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_UNION:
		result = Union(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_LITERAL:
		result = literal(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_VARIABLE:
	  result = variable(context, opPos, executionContext);
	  break;

	case XPathExpression::eOP_GROUP:
		result = group(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_NUMBERLIT:
		result = numberlit(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_ARGUMENT:
		result = arg(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_EXTFUNCTION:
		result = runExtFunction(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_FUNCTION:
		result = runFunction(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_LOCATIONPATH:
		result = locationPath(context, opPos, executionContext);
		break;

	case XPathExpression::eOP_LOCATIONPATHPATTERN:
		result = locationPathPattern(context, opPos, executionContext);
		break;

/*
	case XPathExpression::eOP_PREDICATE:
		break;

	case XPathExpression::XPathExpression::eFROM_ANCESTORS:
		break;

	case XPathExpression::eFROM_ANCESTORS_OR_SELF:
		break;

	case XPathExpression::eFROM_ATTRIBUTES:
		break;

	case XPathExpression::eFROM_CHILDREN:
		break;

	case XPathExpression::eFROM_DESCENDANTS:
		break;

	case XPathExpression::eFROM_DESCENDANTS_OR_SELF:
		break;

	case XPathExpression::eFROM_FOLLOWING:
		break;

	case XPathExpression::eFROM_FOLLOWING_SIBLINGS:
		break;

	case XPathExpression::eFROM_PARENT:
		break;

	case XPathExpression::eFROM_PRECEDING:
		break;

	case XPathExpression::eFROM_PRECEDING_SIBLINGS:
		break;

	case XPathExpression::eFROM_SELF:
		break;

	case XPathExpression::eFROM_NAMESPACE:
		break;

	case XPathExpression::eFROM_ATTRIBUTE:
		break;

	case XPathExpression::eFROM_DOC:
		break;

	case XPathExpression::eFROM_DOCREF:
		break;

	case XPathExpression::eFROM_ID:
		break;
	case XPathExpression::eFROM_IDREF:
		break;

	case XPathExpression::eFROM_ROOT:
		break;

	case eNODETYPE_COMMENT:
		break;

	case eNODETYPE_TEXT:
		break;

	case eNODETYPE_PI:
		break;

	case eNODETYPE_NODE:
		break;

	case eNODETYPE_ROOT:
		break;

	case eNODETYPE_ANYELEMENT:
		break;

	case eNODENAME:
		break;
*/

	default:
		{
			const DOMString		theOpCode = LongToDOMString(m_expression.m_opMap[opPos]);

			executionContext.error(DOMString("ERROR! Unknown op code: ") + theOpCode,
								   context);
		}
		break;
	}

	return result;
}



double
XPath::getMatchScore(const DOM_Node&			context,
					 XPathExecutionContext&		executionContext) const
{
	double	score = s_MatchScoreNone;

	int		opPos = 0;

	if(m_expression.m_opMap[opPos] == XPathExpression::eOP_MATCHPATTERN)
	{
		opPos += 2;

		XLocator*	locator = executionContext.getXLocatorFromNode(context);

		if(0 == locator)
		{
			locator = m_defaultXLocator;
		}
		assert(locator != 0);

		while(m_expression.m_opMap[opPos] == XPathExpression::eOP_LOCATIONPATHPATTERN &&
			  score == s_MatchScoreNone)
		{
			const int	nextOpPos = m_expression.getNextOpCodePosition(opPos);

			// opPos+=2;		
			score = locator->locationPathPattern(*this, executionContext, context, opPos);

			if(score == s_MatchScoreNone)
			{
				opPos = nextOpPos;
			}
		}
	  
	}
	else
	{
		executionContext.error("Expected match pattern in getMatchScore!",
							   context);
	}
	
	return score;
}



double
XPath::nodeTest(
			const DOM_Node&			context,
			int						/* opPos */,
			int						/* argLen */,
			int						/* stepType */,
			XPathExecutionContext&	executionContext) const
{
	executionContext.warn("XPath needs a derived object to implement nodeTest!",
						  context);

    return s_MatchScoreNone;
}



void
XPath::getTargetElementStrings(TargetElementStringsVectorType&	targetStrings) const
{
	int opPos = 2;

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
							targetStrings.push_back(PSEUDONAME_ANY);
							break;

						case XPathExpression::eNODETYPE_ROOT:
							targetStrings.push_back(PSEUDONAME_ROOT);
							break;

						case XPathExpression::eNODETYPE_ANYELEMENT:
							targetStrings.push_back(PSEUDONAME_ANY);
							break;

						case XPathExpression::eNODETYPE_PI:
							targetStrings.push_back(PSEUDONAME_ANY);
							break;

						case XPathExpression::eNODENAME:
							{
								// Skip the namespace
								const int	tokenIndex = m_expression.m_opMap[opPos + 1];

								if(tokenIndex >= 0)
								{
									const DOMString		targetName =
										m_expression.m_tokenQueue[tokenIndex]->str();

									if(::equals(targetName, "*") == true)
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



XObject*
XPath::xpath(
			const DOM_Node&			context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	return execute(context, opPos + 2, executionContext);
}



XObject*
XPath::matchPattern(
			const DOM_Node&			context,
			int						opPos,
			XPathExecutionContext&	executionContext) const

{
	XObject*	score = 0;

	while(m_expression.m_opMap[opPos] == XPathExpression::eOP_LOCATIONPATHPATTERN)
	{
		const int	nextOpPos = m_expression.getNextOpCodePosition(opPos);

		score = execute(context, opPos, executionContext);

		if(score->num() != s_MatchScoreNone)
		{
			break;
		}

		opPos = nextOpPos;
	}

	if(0 == score)
	{
		score = executionContext.getXObjectFactory().createNumber(s_MatchScoreNone);
	}

	return score;
}



MutableNodeRefList*
XPath::step(
			const DOM_Node&			context,
			int						/* opPos */,
			XPathExecutionContext&	executionContext) const
{    
	executionContext.warn("XPath needs a derived object to implement step!",
						  context);

	return 0;
}



XObject*
XPath::or(
			const DOM_Node&			context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	opPos += 2;

	XObject* const	expr1 = execute(context, opPos, executionContext);
	assert(expr1 != 0);

	bool	fResult = expr1->boolean();

	if(fResult == false)
	{
		const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

		XObject* const	expr2 = execute(context, expr2Pos, executionContext);
		assert(expr2 != 0);

		fResult = expr2->boolean();
	}

	return executionContext.getXObjectFactory().createBoolean(fResult);
}



XObject*
XPath::and(
			const DOM_Node&			context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	bool	fResult = false;

	opPos += 2;

	XObject* const	expr1 = execute(context, opPos, executionContext);
	assert(expr1 != 0);

	if (expr1->boolean() == true)
	{
		const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

		XObject* const	expr2 = execute(context, expr2Pos, executionContext);
		assert(expr2 != 0);

		if (expr2->boolean() == true)
		{
			fResult = true;
		}
	}

	return executionContext.getXObjectFactory().createBoolean(fResult);
}



XObject*
XPath::notequals(
			const DOM_Node&			context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	opPos += 2;

	XObject* const	expr1 = execute(context, opPos, executionContext);
	assert(expr1 != 0);

	const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

	XObject* const	expr2 = execute(context, expr2Pos, executionContext);
	assert(expr2 != 0);

	return executionContext.getXObjectFactory().createBoolean(!expr1->equals(*expr2));
}



XObject*
XPath::equals(
			const DOM_Node&			context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	opPos += 2;

	XObject* const	expr1 = execute(context, opPos, executionContext);
	assert(expr1 != 0);

	const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

	XObject* const	expr2 = execute(context, expr2Pos, executionContext);
	assert(expr2 != 0);

	return executionContext.getXObjectFactory().createBoolean(expr1->equals(*expr2));
}



XObject*
XPath::lte(
			const DOM_Node&			context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	bool	fResult = false;

	opPos += 2;

	XObject* const	expr1 = execute(context, opPos, executionContext);
	assert(expr1 != 0);

	const double	expr1Value = expr1->num();

	if (DoubleSupport::isNaN(expr1Value) == false)
	{
		const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

		XObject* const	expr2 = execute(context, expr2Pos, executionContext);
		assert(expr2 != 0);

		const double	expr2Value = expr2->num();

		if (DoubleSupport::isNaN(expr2Value) == false)
		{
			if (expr1Value <= expr2Value)
			{
				fResult = true;
			}
		}
	}

	return executionContext.getXObjectFactory().createBoolean(fResult);
}



XObject*
XPath::lt(
			const DOM_Node&			context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	bool	fResult = false;

	opPos += 2;

	XObject* const	expr1 = execute(context, opPos, executionContext);
	assert(expr1 != 0);

	const double	expr1Value = expr1->num();

	if (DoubleSupport::isNaN(expr1Value) == false)
	{
		const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

		XObject* const	expr2 = execute(context, expr2Pos, executionContext);
		assert(expr2 != 0);

		const double	expr2Value = expr2->num();

		if (DoubleSupport::isNaN(expr2Value) == false)
		{
			if (expr1Value < expr2Value)
			{
				fResult = true;
			}
		}
	}

	return executionContext.getXObjectFactory().createBoolean(fResult);
}



XObject*
XPath::gte(
			const DOM_Node&			context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	bool	fResult = false;

	opPos += 2;

	XObject* const	expr1 = execute(context, opPos, executionContext);
	assert(expr1 != 0);

	const double	expr1Value = expr1->num();

	if (DoubleSupport::isNaN(expr1Value) == false)
	{
		const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

		XObject* const	expr2 = execute(context, expr2Pos, executionContext);
		assert(expr2 != 0);

		const double	expr2Value = expr2->num();

		if (DoubleSupport::isNaN(expr2Value) == false)
		{
			if (expr1Value >= expr2Value)
			{
				fResult = true;
			}
		}
	}

	return executionContext.getXObjectFactory().createBoolean(fResult);
}



XObject*
XPath::gt(
			const DOM_Node&			context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	bool	fResult = false;

	opPos += 2;

	XObject* const	expr1 = execute(context, opPos, executionContext);
	assert(expr1 != 0);

	const double	expr1Value = expr1->num();

	if (DoubleSupport::isNaN(expr1Value) == false)
	{
		const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

		XObject* const	expr2 = execute(context, expr2Pos, executionContext);
		assert(expr2 != 0);

		const double	expr2Value = expr2->num();

		if (DoubleSupport::isNaN(expr2Value) == false)
		{
			if (expr1Value > expr2Value)
			{
				fResult = true;
			}
		}
	}

	return executionContext.getXObjectFactory().createBoolean(fResult);
}



XObject*
XPath::plus(
			const DOM_Node&			context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	opPos += 2;

	XObject* const	expr1 = execute(context, opPos, executionContext);
	assert(expr1 != 0);

	const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

	XObject* const	expr2 = execute(context, expr2Pos, executionContext);
	assert(expr2 != 0);

	return executionContext.getXObjectFactory().createNumber(expr1->num() + expr2->num());
}



XObject*
XPath::minus(
			const DOM_Node&			context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	opPos += 2;

	XObject* const	expr1 = execute(context, opPos, executionContext);
	assert(expr1 != 0);

	const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

	XObject* const	expr2 = execute(context, expr2Pos, executionContext);
	assert(expr2 != 0);

	return executionContext.getXObjectFactory().createNumber(expr1->num() - expr2->num());
}



XObject*
XPath::mult(
			const DOM_Node&			context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	opPos += 2;

	XObject* const	expr1 = execute(context, opPos, executionContext);
	assert(expr1 != 0);

	const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

	XObject* const	expr2 = execute(context, expr2Pos, executionContext);
	assert(expr2 != 0);

	return executionContext.getXObjectFactory().createNumber(expr1->num() * expr2->num());
}



XObject*
XPath::div(
			const DOM_Node&			context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	opPos += 2;

	XObject* const	expr1 = execute(context, opPos, executionContext);
	assert(expr1 != 0);

	const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

	XObject* const	expr2 = execute(context, expr2Pos, executionContext);
	assert(expr2 != 0);

	return executionContext.getXObjectFactory().createNumber(expr1->num() / expr2->num());
}



XObject*
XPath::mod(
			const DOM_Node&			context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	opPos += 2;

	XObject* const	expr1 = execute(context, opPos, executionContext);
	assert(expr1 != 0);

	const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

	XObject* const	expr2 = execute(context, expr2Pos, executionContext);
	assert(expr2 != 0);

	return executionContext.getXObjectFactory().createNumber(static_cast<int>(expr1->num()) %
															 static_cast<int>(expr2->num()));
}



XObject*
XPath::quo(
			const DOM_Node&			context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	// Actually, this is no longer supported by xpath...
	executionContext.warn("Old syntax: quo(...) is no longer defined in XPath.");

	return div(context, opPos, executionContext);
}



XObject*
XPath::neg(
			const DOM_Node&			context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	XObject* const	expr1 = execute(context, opPos + 2, executionContext);
	assert(expr1 != 0);

	return executionContext.getXObjectFactory().createNumber(-expr1->num());
}



XObject*
XPath::string(
			const DOM_Node&			context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	XObject* const	expr1 = execute(context, opPos + 2, executionContext);
	assert(expr1 != 0);

	return executionContext.getXObjectFactory().createString(expr1->str());
}



XObject*
XPath::boolean(
			const DOM_Node&			context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	XObject* const	expr1 = execute(context, opPos + 2, executionContext);
	assert(expr1 != 0);

	return executionContext.getXObjectFactory().createBoolean(expr1->boolean());
}


 
XObject*
XPath::number(
			const DOM_Node&			context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	XObject* const	expr1 = execute(context, opPos + 2, executionContext);
	assert(expr1 != 0);

	return executionContext.getXObjectFactory().createNumber(expr1->num());
}



XObject*
XPath::Union(
			const DOM_Node&			context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	opPos += 2;

	XObject*	resultNodeSet = 0;

	while(m_expression.m_opMap[opPos] == XPathExpression::eOP_LOCATIONPATH)
	{
		const int	nextOpPos = m_expression.getNextOpCodePosition(opPos);

		XObject*	expr = execute(context, opPos, executionContext);

		if(0 == resultNodeSet)
		{
			resultNodeSet = expr;
		}
		else
		{
			MutableNodeRefList&		nl =
				resultNodeSet->mutableNodeset();

			nl.addNodesInDocOrder(expr->nodeset());
		}

		opPos = nextOpPos;
	}

	return resultNodeSet;
}



XObject*
XPath::literal(
			const DOM_Node&			/* context */,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	assert(m_expression.m_opMap.size() > static_cast<unsigned>(opPos + 2));
	assert(m_expression.m_tokenQueue.size() > static_cast<unsigned>(m_expression.m_opMap[opPos + 2]));

	return executionContext.getXObjectFactory().createString(m_expression.m_tokenQueue[m_expression.m_opMap[opPos + 2]]->str());
}



XObject*
XPath::variable(
			const DOM_Node&			context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	assert(executionContext.getPrefixResolver() != 0);

	XObject* const	varName = m_expression.m_tokenQueue[m_expression.m_opMap[opPos + 2]];

	// $$$ ToDo: I don't this will be parsed right in the first place...
	const QName		qname(varName->str(),
						  *executionContext.getPrefixResolver());

	XObject*	result = 0;

	try
	{
		result = executionContext.getVariable(qname);
	}
	catch(...)
	{
		executionContext.error(DOMString("Could not get variable named ") + varName->str());
	}

	if(0 == result)
	{
		executionContext.warn(DOMString("VariableReference given for variable out ") +
								DOMString("of context or without definition!  Name = ") +
							  varName->str(),
			 context);
	}

	return result;
}



XObject*
XPath::group(
			const DOM_Node&			context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{    
	return execute(context, opPos + 2, executionContext);
}



XObject*
XPath::numberlit(
			const DOM_Node&			/* context */,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	assert(m_expression.m_opMap.size() > static_cast<unsigned>(opPos + 2));
	assert(m_expression.m_tokenQueue.size() > static_cast<unsigned>(m_expression.m_opMap[opPos + 2]));

	return executionContext.getXObjectFactory().createNumber(m_expression.m_tokenQueue[m_expression.m_opMap[opPos + 2]]->num());
}
  


XObject*
XPath::arg(
			const DOM_Node&			context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{    
	return execute(context, opPos + 2, executionContext);
}



XObject*
XPath::locationPath(
			const DOM_Node&			context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{    
/*
	const int	stepType = m_expression.m_opMap[opPos + 2];

	XLocator xlocator = null;
	if(OP_VARIABLE == stepType)
	{
	  // Bit of a hack here...
	  XObject obj = execute(context, opPos+2);
	  NodeList nl = obj.nodeset();
	  if(nl.getLength() > 0)
	  {
		// Use the xlocator of the first node...
		// I guess we should really loop through the contexts, but things 
		// will get very nasty quick, so just do this for now.
		xlocator = m_callbacks.getXLocatorFromNode(nl.item(0));
	  }
	  else
	  {
		xlocator = m_callbacks.getXLocatorFromNode(context);
	  }
	}
	else
	{
	  xlocator = m_callbacks.getXLocatorFromNode(context);
	}
*/

	XLocator*	xlocator = executionContext.getXLocatorFromNode(context);

	if(0 == xlocator)
	{
		xlocator = m_defaultXLocator;
	}

	return xlocator->locationPath(*this, executionContext, context, opPos);
}



XObject*
XPath::predicate(
			const DOM_Node&			context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	XObject* const	expr1 = execute(context, opPos + 2, executionContext);

	// $$$ ToDo: This appears to be just an optimization, but is it really?
/*
	int objType = expr1.getType();
	if((XObject.CLASS_NUMBER != objType) && (XObject.CLASS_BOOLEAN != objType))
	{
	  expr1 = expr1.bool() ? m_true : m_false;
	}
*/

	return expr1;
}



XObject*
XPath::locationPathPattern(
			const DOM_Node&			context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	XLocator*	locator = executionContext.getXLocatorFromNode(context);

	if(0 == locator)
	{
		locator = m_defaultXLocator;
	}

	const double	result = locator->locationPathPattern(*this, executionContext, context, opPos);

	return executionContext.getXObjectFactory().createNumber(result);
}



XObject*
XPath::runExtFunction(
			const DOM_Node&			context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	const int	endExtFunc = opPos + m_expression.m_opMap[opPos + 1] - 1;

	opPos += 2;

	const XObject* const	ns = m_expression.m_tokenQueue[m_expression.m_opMap[opPos]];

	opPos++;

	const XObject* const	funcName = m_expression.m_tokenQueue[m_expression.m_opMap[opPos]];

	opPos++;

	std::vector<XObject*>	args;

	while(opPos < endExtFunc)
	{
		const int	nextOpPos = m_expression.getNextOpCodePosition(opPos);

		args.push_back(execute(context, opPos, executionContext));

		opPos = nextOpPos;
	}

	return extfunction(context, opPos, ns->str(), funcName->str(), args, executionContext);
}



XObject*
XPath::extfunction(
			const DOM_Node&					/* context */,
			int								/* opPos */,
			const DOMString&				theNamespace,
			const DOMString&				extensionName, 
			const std::vector<XObject*>&	argVec,
			XPathExecutionContext&			executionContext) const
{
	return 	executionContext.extFunction(theNamespace,
										 extensionName, 
										 argVec);
}



XObject*
XPath::runFunction(
			const DOM_Node&			context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
{
	const int	endFunc = opPos + m_expression.m_opMap[opPos + 1] - 1;

	opPos += 2;

	// This is actually the position in the token queue of the
	// string that contains the name of the function.
	const int	funcID = m_expression.m_opMap[opPos];

	opPos++;

	std::vector<XObject*>	args;

	while(opPos < endFunc)
	{
		const int	nextOpPos = m_expression.getNextOpCodePosition(opPos);

		args.push_back(execute(context, opPos, executionContext));

		opPos = nextOpPos;
	}

	return function(context, opPos, funcID, args, executionContext);
}



XObject*
XPath::function(
			const DOM_Node&					context,
			int								opPos,
			int								funcID,
			const std::vector<XObject*>&	argVec,
			XPathExecutionContext&			executionContext) const
 
{
	assert(m_expression.getToken(funcID) != 0);

	const DOMString		theFunctionName(m_expression.getToken(funcID)->str());
	assert(!isEmpty(theFunctionName));

	return s_functions[theFunctionName].execute(executionContext, context, opPos, argVec);
}
