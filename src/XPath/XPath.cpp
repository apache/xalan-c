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



#include <include/DOMHelper.hpp>
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

const double					XPath::s_MatchScoreNone = -9999999999999;
const double					XPath::s_MatchScoreQName = 0.0;
const double					XPath::s_MatchScoreNSWild = -0.25;
const double					XPath::s_MatchScoreNodeTest = -0.5;
const double					XPath::s_MatchScoreOther = 0.5;

#endif

XPath::FunctionTableType		XPath::s_functions;



XPath::XPath(
			XObjectFactory&		theXObjectFactory,
			XPathEnvSupport&	theXPathEnvSupport,
			XPathSupport&		theXPathSupport) :
	XPathExecutionContext(),
	m_xobjectFactory(theXObjectFactory),
	m_XPathEnvSupport(theXPathEnvSupport),
	m_XPathSupport(theXPathSupport),
	m_defaultXLocator(createXLocatorHandler()),
	m_currentNode(),
	m_prefixResolver(0),
	m_contextNodeList(),
	m_throwFoundIndex(false),
	m_expression()
{
}



XPath::XPath(
			XObjectFactory&		theXObjectFactory,
			XPathEnvSupport&	theXPathEnvSupport,
			XPathSupport&		theXPathSupport,
			bool				createDefaultLocator) :
	XPathExecutionContext(),
	m_xobjectFactory(theXObjectFactory),
	m_XPathEnvSupport(theXPathEnvSupport),
	m_XPathSupport(theXPathSupport),
	m_defaultXLocator(createDefaultLocator == false ? 0 : createXLocatorHandler()),
	m_currentNode(),
	m_prefixResolver(0),
	m_contextNodeList(),
	m_throwFoundIndex(false),
	m_expression()
{
}



XPath::~XPath()
{
	m_defaultXLocator = 0;
	m_prefixResolver = 0;
}



void
XPath::shrink()
{
	m_expression.shrink();
}



DOM_Node
XPath::getCurrentNode() const
{
	return m_currentNode;
}



void
XPath::setCurrentNode(const DOM_Node&	theCurrentNode)
{
	m_currentNode = theCurrentNode;
}



XObjectFactory&
XPath::getXObjectFactory() const
{
	return m_xobjectFactory;
}



DOMString
XPath::getNamespaceOfNode(const DOM_Node&	n) const
{
	return m_XPathSupport.getNamespaceOfNode(n);
}



DOMString
XPath::getLocalNameOfNode(const DOM_Node&	n) const
{
	return m_XPathSupport.getLocalNameOfNode(n);
}



DOM_Node
XPath::getParentOfNode(const DOM_Node&	n) const
{
	return m_XPathSupport.getParentOfNode(n);
}



DOMString
XPath::getNodeData(const DOM_Node&	n) const
{
	return m_XPathSupport.getNodeData(n);
}



DOM_Element
XPath::getElementByID(
			const DOMString&		id,
			const DOM_Document&		doc) const
{
	return m_XPathSupport.getElementByID(id, doc);
}



const NodeRefListBase&
XPath::getContextNodeList() const
{
	return m_contextNodeList;
}



void	
XPath::setContextNodeList(const NodeRefListBase&	theList)
{
	m_contextNodeList = theList;
}



int
XPath::getContextNodeListLength() const
{
	if (m_throwFoundIndex == true)
	{
		throw FoundIndex();
	}

	return m_contextNodeList.getLength();
}



int
XPath::getContextNodeListPosition(const DOM_Node&	contextNode) const
{
	assert(0 != contextNode);

	if (m_throwFoundIndex == true)
	{
		throw FoundIndex();
	}

	int pos = 0;

	const int	nNodes = m_contextNodeList.getLength();

	for(int i = 0; i < nNodes; i++)
	{
		if(m_contextNodeList.item(i) == contextNode)
		{
			pos = i + 1; // for 1-based XSL count.

			break;
		}
	}

	return pos;
}



void
XPath::associateXLocatorToNode(
			const DOM_Node&		node,
			XLocator*			xlocator) const
{
	m_XPathEnvSupport.associateXLocatorToNode(node, xlocator);
}



DOM_Document
XPath::parseXML(
			const DOMString&	urlString,
			const DOMString&	base) const
{
	return m_XPathSupport.parseXML(urlString, base);
}



MutableNodeRefList
XPath::createMutableNodeRefList() const
{
	return MutableNodeRefList(&m_XPathSupport);
}



bool
XPath::getProcessNamespaces() const
{
	return m_XPathSupport.getProcessNamespaces();
}



const NodeRefListBase*
XPath::getNodeSetByKey(
			const DOM_Node&			doc,
			const DOMString&		name,
			const DOMString&		ref,
			const PrefixResolver&	resolver) const
{
	return m_XPathEnvSupport.getNodeSetByKey(doc,
											 name,
											 ref,
											 resolver);
}



const PrefixResolver&
XPath::getPrefixResolver() const
{
	return *m_prefixResolver;
}



void
XPath::setPrefixResolver(const PrefixResolver&	thePrefixResolver)
{
	m_prefixResolver = &thePrefixResolver;
}



DOMString
XPath::getNamespaceForPrefix(const DOMString&	prefix) const
{
	assert(m_prefixResolver != 0);

	return m_prefixResolver->getNamespaceForPrefix(prefix);
}



DOMString
XPath::findURIFromDoc(const DOM_Document&	owner) const
{
	return m_XPathEnvSupport.findURIFromDoc(owner);
}



DOMString
XPath::getUnparsedEntityURI(
			const DOMString&		theName,
			const DOM_Document&		theDocument) const
{
	return m_XPathSupport.getUnparsedEntityURI(theName, theDocument);
}



bool
XPath::shouldStripSourceNode(const DOM_Node&	node) const
{
	return m_XPathEnvSupport.shouldStripSourceNode(node);
}



void
XPath::error(
			const DOMString&	msg,
			const DOM_Node&		sourceNode) const
{
	assert(m_prefixResolver != 0);

	DOMString	emsg;

	const DOMString&	theCurrentPattern =
		m_expression.getCurrentPattern();

	if (length(theCurrentPattern) != 0)
	{
	   emsg = DOMString("pattern = '") +
			  theCurrentPattern +
			  DOMString("'\n");
	}

	emsg += msg;

	if (m_XPathEnvSupport.problem(XPathEnvSupport::eXPATHProcessor, 
								  XPathEnvSupport::eError,
								  *m_prefixResolver, 
								  sourceNode,
								  emsg,
								  0,
								  0) == true)
	{
		// $$$ ToDo: Do something with the PrefixResolver here...
		throw XPathException(msg, DOM_Node());
	}
}



void
XPath::warn(
			const DOMString&	msg,
			const DOM_Node&		sourceNode) const
{
	assert(m_prefixResolver != 0);

	DOMString	emsg;

	const DOMString&	theCurrentPattern =
		m_expression.getCurrentPattern();

	if (length(theCurrentPattern) != 0)
	{
	   emsg = DOMString("pattern = '") +
			  theCurrentPattern +
			  DOMString("'\n");
	}

	emsg += msg;

	if (m_XPathEnvSupport.problem(XPathEnvSupport::eXPATHProcessor, 
								  XPathEnvSupport::eWarning,
								  *m_prefixResolver, 
								  sourceNode,
								  emsg,
								  0,
								  0) == true)
	{
		// $$$ ToDo: Do something with the PrefixResolver here...
		throw XPathException(msg, DOM_Node());
	}
}



XObject*
XPath::execute(
			const DOM_Node& 		contextNode, 
			const PrefixResolver&	resolver,
			const NodeRefListBase& 	contextNodeList)
{
	m_prefixResolver = &resolver;
	m_contextNodeList = contextNodeList;
	m_currentNode = contextNode;

	XObject* const	xobj = execute(contextNode, 0);

	m_currentNode = 0;
	m_contextNodeList.clear();
	m_prefixResolver = 0;

	return xobj;
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
			const DOM_Node& 	context,
			int 				opPos)
{
	XObject*	result = 0;

	switch(m_expression.m_opMap[opPos])
	{
	case XPathExpression::eOP_XPATH:
		result = execute(context, opPos + 2);
		break;

	case XPathExpression::eOP_MATCHPATTERN:
		result = matchPattern(context, opPos + 2);
		break;

	case XPathExpression::eEMPTY:
		opPos++;
		break;

	case XPathExpression::eOP_OR:
		result = or(context, opPos);
		break;

	case XPathExpression::eOP_AND:
		result = and(context, opPos);
		break;

	case XPathExpression::eOP_NOTEQUALS:
		result = notequals(context, opPos);
		break;

	case XPathExpression::eOP_EQUALS:
		result = equals(context, opPos);
		break;

	case XPathExpression::eOP_LTE:
		result = lte(context, opPos);
		break;

	case XPathExpression::eOP_LT:
		result = lt(context, opPos);
		break;

	case XPathExpression::eOP_GTE:
		result = gte(context, opPos);
		break;

	case XPathExpression::eOP_GT:
		result = gt(context, opPos);
		break;

	case XPathExpression::eOP_PLUS:
		result = plus(context, opPos);
		break;

	case XPathExpression::eOP_MINUS:
	  result = minus(context, opPos);
	  break;

	case XPathExpression::eOP_MULT:
		result = mult(context, opPos);
		break;

	case XPathExpression::eOP_DIV:
		result = div(context, opPos);
		break;

	case XPathExpression::eOP_MOD:
		result = mod(context, opPos);
		break;

	case XPathExpression::eOP_QUO:
		result = quo(context, opPos);
		break;

	case XPathExpression::eOP_NEG:
		result = neg(context, opPos);
		break;

	case XPathExpression::eOP_STRING:
		result = string(context, opPos);
		break;

	case XPathExpression::eOP_BOOL:
		result = boolean(context, opPos);
		break;

	case XPathExpression::eOP_NUMBER:
		result = number(context, opPos);
		break;

	case XPathExpression::eOP_UNION:
		result = Union(context, opPos);
		break;

	case XPathExpression::eOP_LITERAL:
		result = literal(context, opPos);
		break;

	case XPathExpression::eOP_VARIABLE:
	  result = variable(context, opPos);
	  break;

	case XPathExpression::eOP_GROUP:
		result = group(context, opPos);
		break;

	case XPathExpression::eOP_NUMBERLIT:
		result = numberlit(context, opPos);
		break;

	case XPathExpression::eOP_ARGUMENT:
		result = arg(context, opPos);
		break;

	case XPathExpression::eOP_EXTFUNCTION:
		result = runExtFunction(context, opPos);
		break;

	case XPathExpression::eOP_FUNCTION:
		result = runFunction(context, opPos);
		break;

	case XPathExpression::eOP_LOCATIONPATH:
		result = locationPath(context, opPos);
		break;

	case XPathExpression::eOP_LOCATIONPATHPATTERN:
		result = locationPathPattern(context, opPos);
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
		error(DOMString("ERROR! Unknown op code: ") + static_cast<int>(m_expression.m_opMap[opPos]),
			  context);
		break;
	}

	return result;
}



double
XPath::getMatchScore(const DOM_Node&	context) 
{
	double	score = s_MatchScoreNone;

	int		opPos = 0;

	if(m_expression.m_opMap[opPos] == XPathExpression::eOP_MATCHPATTERN)
	{
		opPos += 2;

		XLocator*	locator = m_XPathEnvSupport.getXLocatorFromNode(context);

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
			score = locator->locationPathPattern(*this, *this, context, opPos);

			if(score == s_MatchScoreNone)
			{
				opPos = nextOpPos;
			}
		}
	  
	}
	else
	{
		error("Expected match pattern in getMatchScore!",
			  context);
	}
	
	return score;
}



double
XPath::nodeTest(
			const DOM_Node&		context,
			int					/* opPos */,
			int					/* argLen */,
			int					/* stepType */)
{
	warn("XPath needs a derived object to implement nodeTest!",
		 context);

    return s_MatchScoreNone;
}



void
XPath::getTargetElementStrings(TargetElementStringsVectorType&	targetStrings)
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
			const DOM_Node&		context,
			int					opPos)
{
	return execute(context, opPos + 2);
}



XObject*
XPath::matchPattern(
			const DOM_Node&		context,
			int					opPos)

{
	XObject*	score = 0;

	while(m_expression.m_opMap[opPos] == XPathExpression::eOP_LOCATIONPATHPATTERN)
	{
		const int	nextOpPos = m_expression.getNextOpCodePosition(opPos);

		score = execute(context, opPos);

		if(score->num() != s_MatchScoreNone)
		{
			break;
		}

		opPos = nextOpPos;
	}

	if(0 == score)
	{
		score = m_xobjectFactory.createNumber(s_MatchScoreNone);
	}

	return score;
}



MutableNodeRefList*
XPath::step(
			const DOM_Node&		context,
			int					/* opPos */)
{    
	warn("XPath needs a derived object to implement step!",
		 context);

	return 0;
}



XObject*
XPath::or(
			const DOM_Node&		context,
			int					opPos)
{
	opPos += 2;

	XObject* const	expr1 = execute(context, opPos);
	assert(expr1 != 0);

	bool	fResult = expr1->boolean();

	if(fResult == false)
	{
		const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

		XObject* const	expr2 = execute(context, expr2Pos);
		assert(expr2 != 0);

		fResult = expr2->boolean();
	}

	return m_xobjectFactory.createBoolean(fResult);
}



XObject*
XPath::and(
			const DOM_Node&		context,
			int					opPos)
{
	bool	fResult = false;

	opPos += 2;

	XObject* const	expr1 = execute(context, opPos);
	assert(expr1 != 0);

	if (expr1->boolean() == true)
	{
		const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

		XObject* const	expr2 = execute(context, expr2Pos);
		assert(expr2 != 0);

		if (expr2->boolean() == true)
		{
			fResult = true;
		}
	}

	return m_xobjectFactory.createBoolean(fResult);
}



XObject*
XPath::notequals(
			const DOM_Node&		context,
			int					opPos)
{
	opPos += 2;

	XObject* const	expr1 = execute(context, opPos);
	assert(expr1 != 0);

	const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

	XObject* const	expr2 = execute(context, expr2Pos);
	assert(expr2 != 0);

	return m_xobjectFactory.createBoolean(!expr1->equals(*expr2));
}



XObject*
XPath::equals(
			const DOM_Node&		context,
			int					opPos)
{
	opPos += 2;

	XObject* const	expr1 = execute(context, opPos);
	assert(expr1 != 0);

	const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

	XObject* const	expr2 = execute(context, expr2Pos);
	assert(expr2 != 0);

	return m_xobjectFactory.createBoolean(expr1->equals(*expr2));
}



XObject*
XPath::lte(
			const DOM_Node&		context,
			int					opPos)
{
	bool	fResult = false;

	opPos += 2;

	XObject* const	expr1 = execute(context, opPos);
	assert(expr1 != 0);

	const double	expr1Value = expr1->num();

	if (DoubleSupport::isNaN(expr1Value) == false)
	{
		const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

		XObject* const	expr2 = execute(context, expr2Pos);
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

	return m_xobjectFactory.createBoolean(fResult);
}



XObject*
XPath::lt(
			const DOM_Node&		context,
			int					opPos)
{
	bool	fResult = false;

	opPos += 2;

	XObject* const	expr1 = execute(context, opPos);
	assert(expr1 != 0);

	const double	expr1Value = expr1->num();

	if (DoubleSupport::isNaN(expr1Value) == false)
	{
		const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

		XObject* const	expr2 = execute(context, expr2Pos);
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

	return m_xobjectFactory.createBoolean(fResult);
}



XObject*
XPath::gte(
			const DOM_Node&		context,
			int					opPos)
{
	bool	fResult = false;

	opPos += 2;

	XObject* const	expr1 = execute(context, opPos);
	assert(expr1 != 0);

	const double	expr1Value = expr1->num();

	if (DoubleSupport::isNaN(expr1Value) == false)
	{
		const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

		XObject* const	expr2 = execute(context, expr2Pos);
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

	return m_xobjectFactory.createBoolean(fResult);
}



XObject*
XPath::gt(
			const DOM_Node&		context,
			int					opPos)
{
	bool	fResult = false;

	opPos += 2;

	XObject* const	expr1 = execute(context, opPos);
	assert(expr1 != 0);

	const double	expr1Value = expr1->num();

	if (DoubleSupport::isNaN(expr1Value) == false)
	{
		const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

		XObject* const	expr2 = execute(context, expr2Pos);
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

	return m_xobjectFactory.createBoolean(fResult);
}



XObject*
XPath::plus(
			const DOM_Node&		context,
			int					opPos)
{
	opPos += 2;

	XObject* const	expr1 = execute(context, opPos);
	assert(expr1 != 0);

	const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

	XObject* const	expr2 = execute(context, expr2Pos);
	assert(expr2 != 0);

	return m_xobjectFactory.createNumber(expr1->num() + expr2->num());
}



XObject*
XPath::minus(
			const DOM_Node&		context,
			int					opPos)
{
	opPos += 2;

	XObject* const	expr1 = execute(context, opPos);
	assert(expr1 != 0);

	const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

	XObject* const	expr2 = execute(context, expr2Pos);
	assert(expr2 != 0);

	return m_xobjectFactory.createNumber(expr1->num() - expr2->num());
}



XObject*
XPath::mult(
			const DOM_Node&		context,
			int					opPos)
{
	opPos += 2;

	XObject* const	expr1 = execute(context, opPos);
	assert(expr1 != 0);

	const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

	XObject* const	expr2 = execute(context, expr2Pos);
	assert(expr2 != 0);

	return m_xobjectFactory.createNumber(expr1->num() * expr2->num());
}



XObject*
XPath::div(
			const DOM_Node&		context,
			int					opPos)
{
	opPos += 2;

	XObject* const	expr1 = execute(context, opPos);
	assert(expr1 != 0);

	const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

	XObject* const	expr2 = execute(context, expr2Pos);
	assert(expr2 != 0);

	return m_xobjectFactory.createNumber(expr1->num() / expr2->num());
}



XObject*
XPath::mod(
			const DOM_Node&		context,
			int					opPos)
{
	opPos += 2;

	XObject* const	expr1 = execute(context, opPos);
	assert(expr1 != 0);

	const int		expr2Pos = m_expression.getNextOpCodePosition(opPos);

	XObject* const	expr2 = execute(context, expr2Pos);
	assert(expr2 != 0);

	return m_xobjectFactory.createNumber(static_cast<int>(expr1->num()) %
										  static_cast<int>(expr2->num()));
}



XObject*
XPath::quo(
			const DOM_Node&		context,
			int					opPos)
{
	// Actually, this is no longer supported by xpath...
	warn("Old syntax: quo(...) is no longer defined in XPath.");

	return div(context, opPos);
}



XObject*
XPath::neg(
			const DOM_Node&		context,
			int					opPos)
{
	XObject* const	expr1 = execute(context, opPos + 2);
	assert(expr1 != 0);

	return m_xobjectFactory.createNumber(-expr1->num());
}



XObject*
XPath::string(
			const DOM_Node&		context,
			int					opPos)
{
	XObject* const	expr1 = execute(context, opPos + 2);
	assert(expr1 != 0);

	return m_xobjectFactory.createString(expr1->str());
}



XObject*
XPath::boolean(
			const DOM_Node&		context,
			int					opPos)
{
	XObject* const	expr1 = execute(context, opPos + 2);
	assert(expr1 != 0);

	return m_xobjectFactory.createBoolean(expr1->boolean());
}


 
XObject*
XPath::number(
			const DOM_Node&		context,
			int					opPos)
{
	XObject* const	expr1 = execute(context, opPos + 2);
	assert(expr1 != 0);

	return m_xobjectFactory.createNumber(expr1->num());
}



XObject*
XPath::Union(
			const DOM_Node&		context,
			int					opPos)
{
	opPos += 2;

	XObject*	resultNodeSet = 0;

	while(m_expression.m_opMap[opPos] == XPathExpression::eOP_LOCATIONPATH)
	{
		const int	nextOpPos = m_expression.getNextOpCodePosition(opPos);

		XObject*	expr = execute(context, opPos);

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
			const DOM_Node&		/* context */,
			int					opPos)
{
	assert(m_expression.m_opMap.size() > static_cast<unsigned>(opPos + 2));
	assert(m_expression.m_tokenQueue.size() > static_cast<unsigned>(m_expression.m_opMap[opPos + 2]));

	return m_xobjectFactory.createString(m_expression.m_tokenQueue[m_expression.m_opMap[opPos + 2]]->str());
}
  


XObject*
XPath::variable(
			const DOM_Node&		context,
			int					opPos)
{
	assert(m_prefixResolver != 0);

	XObject* const	varName = m_expression.m_tokenQueue[m_expression.m_opMap[opPos + 2]];

	// $$$ ToDo: I don't this will be parsed right in the first place...
	const QName		qname(varName->str(),
						  *m_prefixResolver);

	XObject*	result = 0;

	try
	{
		result = m_XPathEnvSupport.getVariable(*this, qname);
	}
	catch(...)
	{
		error(DOMString("Could not get variable named ") + varName->str());
	}

	if(0 == result)
	{
		warn(DOMString("VariableReference given for variable out ") +
				DOMString("of context or without definition!  Name = ") +
				varName->str(),
			 context);
	}

	return result;
}



XObject*
XPath::group(
			const DOM_Node&		context,
			int					opPos)
{    
	return execute(context, opPos + 2);
}



XObject*
XPath::numberlit(
			const DOM_Node&		/* context */,
			int					opPos)
{
	assert(m_expression.m_opMap.size() > static_cast<unsigned>(opPos + 2));
	assert(m_expression.m_tokenQueue.size() > static_cast<unsigned>(m_expression.m_opMap[opPos + 2]));

	return m_xobjectFactory.createNumber(m_expression.m_tokenQueue[m_expression.m_opMap[opPos + 2]]->num());
}
  


XObject*
XPath::arg(
			const DOM_Node&		context,
			int					opPos)
{    
	return execute(context, opPos + 2);
}



XObject*
XPath::locationPath(
			const DOM_Node&		context,
			int					opPos)
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

	XLocator*	xlocator = m_XPathEnvSupport.getXLocatorFromNode(context);

	if(0 == xlocator)
	{
		xlocator = m_defaultXLocator;
	}

	return xlocator->locationPath(*this, *this, context, opPos);
}



XObject*
XPath::predicate(
			const DOM_Node&		context,
			int					opPos)
{
	XObject* const	expr1 = execute(context, opPos + 2);

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
			const DOM_Node&		context,
			int					opPos)
{
	XLocator*	locator = m_XPathEnvSupport.getXLocatorFromNode(context);

	if(0 == locator)
	{
		locator = m_defaultXLocator;
	}

	const double	result = locator->locationPathPattern(*this, *this, context, opPos);

	return m_xobjectFactory.createNumber(result);
}



XObject*
XPath::runExtFunction(
			const DOM_Node&		context,
			int					opPos)
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

		args.push_back(execute(context, opPos));

		opPos = nextOpPos;
	}

	return extfunction(context, opPos, ns->str(), funcName->str(), args);
}



XObject*
XPath::extfunction(
			const DOM_Node&					/* context */,
			int								/* opPos */,
			const DOMString&				theNamespace,
			const DOMString&				extensionName, 
			const std::vector<XObject*>&	argVec)
{
	return 	m_XPathEnvSupport.extFunction(*this,
										  theNamespace,
										  extensionName, 
										  argVec);
}



XObject*
XPath::runFunction(
			const DOM_Node&		context,
			int					opPos)
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

		args.push_back(execute(context, opPos));

		opPos = nextOpPos;
	}

	return function(context, opPos, funcID, args);
}



XObject*
XPath::function(
			const DOM_Node&					context,
			int								opPos,
			int								funcID,
			const std::vector<XObject*>&	argVec)
 
{
	assert(m_expression.getToken(funcID) != 0);

	const DOMString		theFunctionName(m_expression.getToken(funcID)->str());

	return s_functions[theFunctionName].execute(*this, context, opPos, argVec);
}
