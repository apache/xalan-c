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
 *	  notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *	  notice, this list of conditions and the following disclaimer in
 *	  the documentation and/or other materials provided with the
 *	  distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *	  if any, must include the following acknowledgment:  
 *		 "This product includes software developed by the
 *		  Apache Software Foundation (http://www.apache.org/)."
 *	  Alternately, this acknowledgment may appear in the software itself,
 *	  if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *	  not be used to endorse or promote products derived from this
 *	  software without prior written permission. For written 
 *	  permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *	  nor may "Apache" appear in their name, without prior written
 *	  permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.	IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
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
#include "SimpleNodeLocator.hpp"



#include <XalanDOM/XalanAttr.hpp>
#include <XalanDOM/XalanElement.hpp>
#include <XalanDOM/XalanNamedNodeMap.hpp>
#include <XalanDOM/XalanNode.hpp>
#include <XalanDOM/XalanDocument.hpp>



#include <DOMSupport/DOMServices.hpp>



#include "FoundIndex.hpp"
#include "NodeRefListBase.hpp"
#include "XObject.hpp"
#include "XObjectFactory.hpp"
#include "XPath.hpp"



SimpleNodeLocator		SimpleNodeLocator::s_defaultInstance;

const XalanDOMString	SimpleNodeLocator::s_emptyString;



SimpleNodeLocator*
SimpleNodeLocator::getDefaultInstance()
{
	return &s_defaultInstance;
}



SimpleNodeLocator::SimpleNodeLocator() :
	XLocator()
{
}



SimpleNodeLocator::~SimpleNodeLocator()
{
}



const XObjectPtr
SimpleNodeLocator::locationPath(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode&				context, 
			int 					opPos)
{
	typedef XPathExecutionContext::BorrowReturnMutableNodeRefList	BorrowReturnMutableNodeRefList;

	BorrowReturnMutableNodeRefList	mnl(executionContext);

	step(xpath, executionContext, &context, opPos + 2, *mnl.get());

	return executionContext.getXObjectFactory().createNodeSet(mnl);
}



double
SimpleNodeLocator::locationPathPattern(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode&				context, 
			int 					opPos)
{
	double	score = xpath.s_MatchScoreNone;

	stepPattern(xpath, executionContext, &context, opPos + 2, score);

	return score;
}



void
SimpleNodeLocator::step(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode*				context, 
			int 					opPos,
			MutableNodeRefList& 	queryResults)
{
	const XPathExpression&	currentExpression =
		xpath.getExpression();

	// int endStep = xpath.getNextOpPos(opPos);
	const int			stepType =
		currentExpression.getOpCodeMapValue(opPos);

	int 				argLen = 0;

	typedef XPathExecutionContext::BorrowReturnMutableNodeRefList	BorrowReturnMutableNodeRefList;

	BorrowReturnMutableNodeRefList	subQueryResults(executionContext);

	bool				shouldReorder = false;
	bool				continueStepRecursion = true;

	switch(stepType)
	{
	case XPathExpression::eOP_VARIABLE:
	case XPathExpression::eOP_EXTFUNCTION:
	case XPathExpression::eOP_FUNCTION:
	case XPathExpression::eOP_GROUP:
		argLen = findNodeSet(xpath, executionContext, context, opPos, 
							  stepType, *subQueryResults);
		break;

	case XPathExpression::eFROM_ROOT:
		argLen = findRoot(xpath, executionContext, context, opPos, stepType, *subQueryResults);
		break;

	case XPathExpression::eFROM_PARENT:
		argLen = findParent(xpath, executionContext, context, opPos, stepType, *subQueryResults);
		break;

	case XPathExpression::eFROM_SELF:
		argLen = findSelf(xpath, executionContext, context, opPos, stepType, *subQueryResults);
		break;

	case XPathExpression::eFROM_ANCESTORS:
		argLen = findAncestors(xpath, executionContext, context, opPos, stepType, *subQueryResults);
		shouldReorder = true;
		break;

	case XPathExpression::eFROM_ANCESTORS_OR_SELF:
		argLen = findAncestorsOrSelf(xpath, executionContext, context, opPos, stepType, *subQueryResults);
		shouldReorder = true;
		break;

	case XPathExpression::eMATCH_ATTRIBUTE:
		continueStepRecursion = false;
		// fall-through on purpose.

	case XPathExpression::eFROM_ATTRIBUTES:
		argLen = findAttributes(xpath, executionContext, context, opPos, stepType, *subQueryResults);
		break;

	case XPathExpression::eMATCH_ANY_ANCESTOR:
	case XPathExpression::eMATCH_IMMEDIATE_ANCESTOR:
	case XPathExpression::eMATCH_ANY_ANCESTOR_WITH_PREDICATE:
		continueStepRecursion = false;
		// fall-through on purpose.

	case XPathExpression::eFROM_CHILDREN:
		argLen = findChildren(xpath, executionContext, context, opPos, stepType, *subQueryResults);
		break;

	case XPathExpression::eFROM_DESCENDANTS:
	case XPathExpression::eFROM_DESCENDANTS_OR_SELF:
		argLen = findDescendants(xpath, executionContext, context, opPos, stepType, *subQueryResults);
		break;

	case XPathExpression::eFROM_FOLLOWING:
		argLen = findFollowing(xpath, executionContext, context, opPos, stepType, *subQueryResults);
		break;

	case XPathExpression::eFROM_FOLLOWING_SIBLINGS:
		argLen = findFollowingSiblings(xpath, executionContext, context, opPos, stepType, *subQueryResults);
		break;

	case XPathExpression::eFROM_PRECEDING:
		argLen = findPreceeding(xpath, executionContext, context, opPos, stepType, *subQueryResults);
		shouldReorder = true;
		break;

	case XPathExpression::eFROM_PRECEDING_SIBLINGS:
		argLen = findPreceedingSiblings(xpath, executionContext, context, opPos, stepType, *subQueryResults);
		shouldReorder = true;
		break;

	case XPathExpression::eFROM_NAMESPACE:
		argLen = findNamespace(xpath, executionContext, context, opPos,  stepType, *subQueryResults);
		break;

	default:
		argLen = findNodesOnUnknownAxis(xpath, executionContext, context, opPos, stepType, *subQueryResults);
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
		predicates(xpath,
				   executionContext,
				   context,
				   opPos, 
				   *subQueryResults,
				   opPos);

		nextStepType = currentExpression.getOpCodeMapValue(opPos);
	}

	if(XPathExpression::eENDOP != nextStepType && continueStepRecursion == true)
	{
		const unsigned int	nContexts = subQueryResults->getLength();

		for(unsigned int i = 0; i < nContexts; i++)
		{
			XalanNode* const	node = subQueryResults->item(i);

			if(0 != node)
			{
				BorrowReturnMutableNodeRefList	mnl(executionContext);

				step(xpath, executionContext, node, opPos, *mnl);

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
SimpleNodeLocator::stepPattern(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode*				context, 
			int 					opPos,
			double& 				scoreHolder)
{
	const XPathExpression&	currentExpression =
		xpath.getExpression();

	const int	endStep = currentExpression.getNextOpCodePosition(opPos);
	int 		nextStepType = currentExpression.getOpCodeMapValue(endStep);

	if(XPathExpression::eENDOP != nextStepType)
	{
		// Continue step via recursion...
		context = stepPattern(
						xpath,
						executionContext,
						context,
						endStep,
						scoreHolder);

		if(0 == context)
		{
			scoreHolder = xpath.s_MatchScoreNone;

		}

		if (scoreHolder == xpath.s_MatchScoreNone)
		{
			// !!!!!!!!!!!!! Big ugly return here !!!!!!!!!!!!!!!!!!!
			return 0;
		}

		scoreHolder = xpath.s_MatchScoreOther;

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

	int			argLen = 0;

	double		score = xpath.s_MatchScoreNone;

	const int	startOpPos = opPos;
	const int	stepType = currentExpression.getOpCodeMapValue(opPos);

	switch(stepType)
	{
	case XPathExpression::eMATCH_ANY_ANCESTOR_WITH_FUNCTION_CALL:
		score = scoreHolder;
		break;

	case XPathExpression::eOP_FUNCTION:
		{
			argLen = currentExpression.getOpCodeLength(opPos);

			const XObjectPtr		obj(xpath.executeMore(context, opPos, executionContext));
			assert(obj.get() != 0);

			const NodeRefListBase&	nl = obj->nodeset();

			const unsigned int		len = nl.getLength();

			score = xpath.s_MatchScoreNone;

			if (nextStepType == XPathExpression::eMATCH_ANY_ANCESTOR_WITH_FUNCTION_CALL)
			{
				bool	fFound = false;

				while(context != 0 && fFound == false)
				{
					for(unsigned int i = 0; i < len; i++)
					{
						XalanNode* const	n = nl.item(i);

						if(n == context)
						{
							score = xpath.s_MatchScoreOther;

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
				for(unsigned int i = 0; i < len; i++)
				{
					XalanNode* const	n = nl.item(i);

					if(n == context)
					{
						score = xpath.s_MatchScoreOther;

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
				currentExpression.getOpCodeMapValue(opPos + XPathExpression::s__opCodeMapLengthIndex + 1) - 3;

			opPos += 3;

			const XalanNode::NodeType	nodeType = context->getNodeType();

			if (nodeType == XalanNode::DOCUMENT_NODE ||
				nodeType == XalanNode::DOCUMENT_FRAGMENT_NODE)
			{
				score = XPath::s_MatchScoreOther;
			}
			else
			{
				const int   prevPos = currentExpression.getNextOpCodePosition(startOpPos);		
				const int	prevStepType = currentExpression.getOpCodeMapValue(prevPos);

				if (xpath.s_MatchScoreNone == score  && 
				    (prevStepType == XPathExpression::eMATCH_ANY_ANCESTOR ||
					 prevStepType == XPathExpression::eMATCH_ANY_ANCESTOR_WITH_PREDICATE))
				{
					while(0 != context)
					{
						score = nodeTest(
							xpath,
							executionContext,
							context,
							context->getNodeType(),
							opPos,
							argLen,
							stepType);

						if(xpath.s_MatchScoreNone != score)
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
				currentExpression.getOpCodeMapValue(opPos + XPathExpression::s__opCodeMapLengthIndex + 1) - 3;

			opPos += 3;

			score = nodeTest(xpath,
							 executionContext,
							 context,
							 context->getNodeType(),
							 opPos,
							 argLen,
							 XPathExpression::eFROM_ATTRIBUTES);
		}
		break;

	case XPathExpression::eMATCH_ANY_ANCESTOR:
	case XPathExpression::eMATCH_ANY_ANCESTOR_WITH_PREDICATE:
		{
			// $$ ToDO: Can we reduce this to some call on the
			// XPathExpression interface?
			argLen =
					currentExpression.getOpCodeMapValue(opPos + XPathExpression::s__opCodeMapLengthIndex + 1) - 3;

			XalanNode::NodeType		nodeType = context->getNodeType();

			if(nodeType != XalanNode::ATTRIBUTE_NODE)
			{
				opPos += 3;

				for(;;)
				{
					score = nodeTest(xpath, executionContext, context, nodeType, opPos, argLen, stepType);

					if(xpath.s_MatchScoreNone != score)
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
				currentExpression.getOpCodeMapValue(opPos + XPathExpression::s__opCodeMapLengthIndex + 1) - 3;

			const XalanNode::NodeType	nodeType = context->getNodeType();

			if(nodeType != XalanNode::ATTRIBUTE_NODE)
			{
				opPos += 3;

				score = nodeTest(xpath, executionContext, context, nodeType, opPos, argLen, stepType);
			}
		}
		break;

	default:
		executionContext.error(TranscodeFromLocalCodePage("unknown match operation!"), context);
		break;
	}

	opPos += argLen;

	nextStepType = currentExpression.getOpCodeMapValue(opPos);

	if(score != xpath.s_MatchScoreNone && XPathExpression::eOP_PREDICATE == nextStepType)
	{
		score = xpath.s_MatchScoreOther;

		// Execute the xpath.predicates, but if we have an index, then we have 
		// to start over and do a search from the parent.  It would be nice 
		// if I could sense this condition earlier...
		try
		{
			executionContext.setThrowFoundIndex(true);

			while(XPathExpression::eOP_PREDICATE == nextStepType)
			{
				const XObjectPtr		pred(xpath.predicate(context, opPos, executionContext));
				assert(pred.get() != 0);

				if(XObject::eTypeNumber == pred->getType())
				{
					score = handleFoundIndex(xpath, executionContext, context, startOpPos);
				}
				else if(pred->boolean() == false)
				{
					score = xpath.s_MatchScoreNone;

					break; // from while(xpath.OP_PREDICATE == nextStepType)
				}

				opPos = currentExpression.getNextOpCodePosition(opPos);
				nextStepType = currentExpression.getOpCodeMapValue(opPos);
			}

			executionContext.setThrowFoundIndex(false);
		}
		catch(const FoundIndex&)
		{
			score = handleFoundIndex(xpath, executionContext, context, startOpPos);
		}
	}

	if (scoreHolder == xpath.s_MatchScoreNone || 
        score == xpath.s_MatchScoreNone)
	{
		scoreHolder = score;
	}

	return score == xpath.s_MatchScoreNone ? 0 : context;
}



double
SimpleNodeLocator::handleFoundIndex(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				localContext, 
			int 					startOpPos)
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
		return XPath::s_MatchScoreNone;
	}
	else
	{
		typedef XPathExecutionContext::BorrowReturnMutableNodeRefList	BorrowReturnMutableNodeRefList;

		BorrowReturnMutableNodeRefList	mnl(executionContext);

		step(xpath, executionContext, parentContext, startOpPos, *mnl);

		if (mnl->indexOf(localContext) == MutableNodeRefList::npos)
		{
			return XPath::s_MatchScoreNone;
		}
		else
		{
			return XPath::s_MatchScoreOther;
		}
	}
}



int
SimpleNodeLocator::findNodeSet(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode*				context, 
			int 					opPos,
			int 					/* stepType */,
			MutableNodeRefList& 	subQueryResults)
{
	const XPathExpression&	currentExpression =
		xpath.getExpression();


	const XObjectPtr		obj(xpath.executeMore(context, opPos, executionContext));

	const NodeRefListBase&	nl = obj->nodeset();

	// $$$ ToDo: Should this be adding in doc order?
	subQueryResults.addNodes(nl);

	return currentExpression.getOpCodeLength(opPos);
}



int
SimpleNodeLocator::findRoot(
			const XPath&			xpath,
			XPathExecutionContext&	/* executionContext */,
			XalanNode*				context, 
			int 					opPos,
			int 					/* stepType */,
			MutableNodeRefList& 	subQueryResults)
{
	const XPathExpression&	currentExpression =
		xpath.getExpression();

	// $$ ToDO: Can we reduce this to some call on the
	// XPathExpression interface?
	const int	argLen =
		currentExpression.getOpCodeMapValue(opPos + XPathExpression::s__opCodeMapLengthIndex + 1) - 3;

	XalanNode* const	docContext = XalanNode::DOCUMENT_NODE == context->getNodeType() ?
									context :
									context->getOwnerDocument();
	assert(docContext != 0);

	subQueryResults.addNode(docContext);

	return argLen + 3;
}



int
SimpleNodeLocator::findParent(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode*				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults)
{
	const XPathExpression&	currentExpression =
		xpath.getExpression();

	// $$ ToDO: Can we reduce this to some call on the
	// XPathExpression interface?
	const int	argLen =
		currentExpression.getOpCodeMapValue(opPos + XPathExpression::s__opCodeMapLengthIndex + 1) - 3;

	opPos += 3;

	XalanNode* const	theParent = DOMServices::getParentOfNode(*context);

	if(0 != theParent)
	{
		if(argLen > 0)
		{
			const double	score = nodeTest(xpath,
											 executionContext,
											 theParent,
											 theParent->getNodeType(),
											 opPos,
											 argLen,
											 stepType);

			if(xpath.s_MatchScoreNone != score)
			{
				subQueryResults.addNode(theParent);
			}
		}
		else
		{
			subQueryResults.addNode(theParent);
		}
	}

	return argLen + 3;
}



int
SimpleNodeLocator::findSelf(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode*				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults)
{
	const XPathExpression&	currentExpression =
		xpath.getExpression();

	// $$ ToDO: Can we reduce this to some call on the
	// XPathExpression interface?
	const int	argLen =
		currentExpression.getOpCodeMapValue(opPos + XPathExpression::s__opCodeMapLengthIndex + 1) - 3;

	opPos += 3;

	if(argLen > 0)
	{
		const double	score = nodeTest(xpath,
										 executionContext,
										 context,
			 							 context->getNodeType(),
										 opPos,
										 argLen,
										 stepType);

		if(xpath.s_MatchScoreNone != score)
		{
			subQueryResults.addNode(context);
		}
	}
	else
	{
		subQueryResults.addNode(context);
	}

	return argLen + 3;
}



int
SimpleNodeLocator::findAncestors(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode*				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults)
{
	const XPathExpression&	currentExpression =
		xpath.getExpression();

	XalanNode*				contextNode =
		DOMServices::getParentOfNode(*context);

	// $$ ToDO: Can we reduce this to some call on the
	// XPathExpression interface?
	const int	argLen =
		currentExpression.getOpCodeMapValue(opPos + XPathExpression::s__opCodeMapLengthIndex + 1) - 3;

	opPos += 3;

	while(0 != contextNode)
	{
		const double	score = nodeTest(xpath,
										 executionContext,
										 contextNode,
										 contextNode->getNodeType(),
										 opPos,
										 argLen,
										 stepType);

		if(xpath.s_MatchScoreNone != score)
		{
			subQueryResults.addNode(contextNode);
		}

		contextNode = DOMServices::getParentOfNode(*contextNode);
	}

	return argLen + 3;
}



int
SimpleNodeLocator::findAncestorsOrSelf(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode*				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults)
{
	// $$ ToDO: Can this function be reduced to calling
	// findSelf() and findAncestors()?	I would think so...

	const XPathExpression&	currentExpression =
		xpath.getExpression();

	XalanNode*				contextNode = context;

	// $$ ToDO: Can we reduce this to some call on the
	// XPathExpression interface?
	const int	argLen =
		currentExpression.getOpCodeMapValue(opPos + XPathExpression::s__opCodeMapLengthIndex + 1) - 3;

	opPos += 3;

	while(0 != contextNode)
	{
		const double	score = nodeTest(xpath,
										 executionContext,
										 contextNode,
										 contextNode->getNodeType(),
										 opPos,
										 argLen,
										 stepType);

		if(xpath.s_MatchScoreNone != score)
		{
			subQueryResults.addNode(contextNode);
		}

		contextNode = DOMServices::getParentOfNode(*contextNode);
	}

	return argLen + 3;
}



int
SimpleNodeLocator::findAttributes(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode*				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults)
{
	const XPathExpression&	currentExpression =
		xpath.getExpression();

	// $$ ToDO: Can we reduce this to some call on the
	// XPathExpression interface?
	const int	argLen =
		currentExpression.getOpCodeMapValue(opPos + XPathExpression::s__opCodeMapLengthIndex + 1) - 3;

	if(0 != context && context->getNodeType() == XalanNode::ELEMENT_NODE)
	{
		const XalanElement* const		e =
#if defined(XALAN_OLD_STYLE_CASTS)
					(const XalanElement*)context;
#else
					static_cast<const XalanElement*>(context);
#endif

		const XalanNamedNodeMap* const	attributeList = e->getAttributes();

		if(attributeList != 0) 
		{
			opPos += 3;

			const unsigned int	nAttrs = attributeList->getLength();

			for(unsigned int j = 0; j < nAttrs; j++)
			{
				XalanNode* const	theNode = attributeList->item(j);
				assert(theNode != 0 && theNode->getNodeType() == XalanNode::ATTRIBUTE_NODE);

				const double	score = nodeTest(xpath,
												 executionContext,
												 theNode,
												 XalanNode::ATTRIBUTE_NODE,
												 opPos,
												 argLen,
												 stepType);

				if(xpath.s_MatchScoreNone != score)
				{
					subQueryResults.addNode(theNode);
				}
			}
		}
	}

	return argLen + 3;
}



int
SimpleNodeLocator::findChildren(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode*				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults)
{
	const XPathExpression&	currentExpression =
		xpath.getExpression();

	// $$ ToDO: Can we reduce this to some call on the
	// XPathExpression interface?
	const int	argLen =
		currentExpression.getOpCodeMapValue(opPos + XPathExpression::s__opCodeMapLengthIndex + 1) - 3;

	opPos += 3;

	XalanNode*	child = context->getFirstChild();

	while(0 != child)
	{
		const double	score = nodeTest(xpath,
										 executionContext,
										 child,
										 child->getNodeType(),
										 opPos,
										 argLen,
										 stepType);

		if(xpath.s_MatchScoreNone != score)
		{
			subQueryResults.addNode(child);
		}

		child = child->getNextSibling();
	}

	return argLen + 3;
}



int
SimpleNodeLocator::findDescendants(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode*				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults)
{
	const XPathExpression&	currentExpression =
		xpath.getExpression();

	// $$ ToDO: Can we reduce this to some call on the
	// XPathExpression interface?
	const int	argLen =
		currentExpression.getOpCodeMapValue(opPos + XPathExpression::s__opCodeMapLengthIndex + 1) - 3;

	opPos += 3;

	// Perform a pre-order traversal of descendents.
	// Note that I would like to be able to do optimization here 
	// where if I have a simple tag name node test, I would 
	// like to be able to call 
	// ((Element)context).getElementsByTagName(m_token).
	// One problem is that it would return a NodeList, not a 
	// NodeListImpl.
	/*
	if(lookahead('[', 1) || lookahead('(', 1) 
	|| (Node.ELEMENT_NODE != context.getNodeType()))
	{
	*/
	XalanNode*	pos = context;

	while(0 != pos)
	{					
		if(stepType == XPathExpression::eFROM_DESCENDANTS_OR_SELF ||
		   context != pos)
		{
			const double	score = nodeTest(xpath,
											 executionContext,
											 pos,
											 pos->getNodeType(),
											 opPos,
											 argLen,
											 stepType);

			if(xpath.s_MatchScoreNone != score)
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
	}

	return argLen + 3;
}



int
SimpleNodeLocator::findFollowing(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode*				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults)
{
	const XPathExpression&	currentExpression =
		xpath.getExpression();

	// $$ ToDO: Can we reduce this to some call on the
	// XPathExpression interface?
	const int	argLen =
		currentExpression.getOpCodeMapValue(opPos + XPathExpression::s__opCodeMapLengthIndex + 1) - 3;

	opPos += 3;

	// What fun...
	XalanDocument* const	doc = context->getOwnerDocument();

	XalanNode*				pos = context;

	while(0 != pos)
	{
		XalanNode*	nextNode = 0;

		if(pos != context)
		{
			const double	score = nodeTest(xpath,
											 executionContext,
											 pos,
											 pos->getNodeType(),
											 opPos,
											 argLen,
											 stepType);

			if(xpath.s_MatchScoreNone != score)
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

	return argLen + 3;
}



int
SimpleNodeLocator::findFollowingSiblings(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode*				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults)
{
	const XPathExpression&	currentExpression =
		xpath.getExpression();

	// $$ ToDO: Can we reduce this to some call on the
	// XPathExpression interface?
	const int	argLen =
		currentExpression.getOpCodeMapValue(opPos + XPathExpression::s__opCodeMapLengthIndex + 1) - 3;

	opPos += 3;

	XalanNode*	pos = context->getNextSibling();

	while(0 != pos)
	{
		const double	score = nodeTest(xpath,
										 executionContext,
										 pos,
										 pos->getNodeType(),
										 opPos,
										 argLen,
										 stepType);

		if(xpath.s_MatchScoreNone != score)
		{
			subQueryResults.addNode(pos);
		}

		pos = pos->getNextSibling();
	}

	return argLen + 3;
}



int
SimpleNodeLocator::findPreceeding(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode*				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults)
{
	const XPathExpression&	currentExpression =
		xpath.getExpression();

	// $$ ToDO: Can we reduce this to some call on the
	// XPathExpression interface?
	const int	argLen =
		currentExpression.getOpCodeMapValue(opPos + XPathExpression::s__opCodeMapLengthIndex + 1) - 3;

	opPos += 3;

	// Ugh.  Reverse document order, no parents, I guess.
	XalanDocument* const	doc = context->getOwnerDocument();

	XalanNode*				pos = doc;

	// If the context node is an attribute, we need to perform some
	// magic to stop the search at the appropriate point, which is when
	// we arrive back at the parent.
	const bool				contextIsAttribute =
			context->getNodeType() == XalanNode::ATTRIBUTE_NODE ? true : false;

	const XalanNode* const	theAttributeContextParent =
		contextIsAttribute == true ? DOMServices::getParentOfNode(*context) : 0;

	while(0 != pos)
	{
		if(context == pos)
		{
			break;
		}

		const double	score = nodeTest(xpath,
										 executionContext,
										 pos,
										 pos->getNodeType(),
										 opPos,
										 argLen,
										 stepType);

		if(xpath.s_MatchScoreNone != score)
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
				subQueryResults.insertNode(pos, 0);
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

				if(doc == pos)
				{
					nextNode = 0;
					break;
				}
			}
		}

		pos = nextNode;
	}

	return argLen + 3;
}


int
SimpleNodeLocator::findPreceedingSiblings(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode*				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults)
{
	const XPathExpression&	currentExpression =
		xpath.getExpression();

	// $$ ToDO: Can we reduce this to some call on the
	// XPathExpression interface?
	const int	argLen =
		currentExpression.getOpCodeMapValue(opPos + XPathExpression::s__opCodeMapLengthIndex + 1) - 3;

	opPos += 3;

	XalanNode*	pos = context->getPreviousSibling();

	while(0 != pos)
	{
		const double	score = nodeTest(xpath,
										 executionContext,
										 pos,
										 pos->getNodeType(),
										 opPos,
										 argLen,
										 stepType);

		if(xpath.s_MatchScoreNone != score)
		{
			subQueryResults.addNode(pos);
		}

		pos = pos->getPreviousSibling();
	}

	return argLen + 3;
}



int
SimpleNodeLocator::findNamespace(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode*				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults)
{
	const XPathExpression&	currentExpression =
		xpath.getExpression();

	// $$ ToDO: Can we reduce this to some call on the
	// XPathExpression interface?
	const int	argLen =
		currentExpression.getOpCodeMapValue(opPos + XPathExpression::s__opCodeMapLengthIndex + 1) - 3;

	opPos += 3;

	if(context != 0 && context->getNodeType() == XalanNode::ELEMENT_NODE)
	{
		// Look up the element chain until we hit the document, so that we
		// get all of the attribute/namespace nodes.
		const XalanNode* const	theOwnerDocument = context->getOwnerDocument();
		assert(theOwnerDocument != 0);

		const XalanNode*		theCurrentNode = context;

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
						equals(theNodeName, DOMServices::s_XMLNamespace) == true)
					{
						if(nodeTest(xpath,
									executionContext,
									attr,
									XalanNode::ATTRIBUTE_NODE,
									opPos,
									argLen,
									stepType) != xpath.s_MatchScoreNone)
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
SimpleNodeLocator::findNodesOnUnknownAxis(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode*				context, 
			int 					opPos,
			int 					/* stepType */,
			MutableNodeRefList& 	/* subQueryResults */)
{
	const XPathExpression&	currentExpression =
		xpath.getExpression();

	// $$ ToDO: Can we reduce this to some call on the
	// XPathExpression interface?
	const int	argLen =
		currentExpression.getOpCodeMapValue(opPos + XPathExpression::s__opCodeMapLengthIndex + 1) - 3;

	executionContext.error(TranscodeFromLocalCodePage("Unknown axis!"), context);

	return argLen + 3;
}



double
SimpleNodeLocator::nodeTest(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				context,
			XalanNode::NodeType		nodeType,
			int 					opPos,
			int 					argLen,
			int 					stepType)
{
	assert(context->getNodeType() == nodeType);

	const XPathExpression&	currentExpression =
		xpath.getExpression();

	double score = xpath.s_MatchScoreNone;

	const int	testType = currentExpression.getOpCodeMapValue(opPos);

	switch(testType)
	{
	case XPathExpression::eNODETYPE_COMMENT:
		if (XalanNode::COMMENT_NODE == nodeType)
		{
			score = xpath.s_MatchScoreNodeTest;
		}
		break;

	case XPathExpression::eNODETYPE_TEXT:
		if ((XalanNode::CDATA_SECTION_NODE == nodeType ||
			 XalanNode::TEXT_NODE == nodeType) &&
			executionContext.shouldStripSourceNode(*context) == false)
		{
			  score = xpath.s_MatchScoreNodeTest;
		}
	  break;

	case XPathExpression::eNODETYPE_PI:
		if(XalanNode::PROCESSING_INSTRUCTION_NODE == nodeType)
		{
			opPos++;

			if(argLen == 1)
			{
				score = xpath.s_MatchScoreNodeTest;
			}
			else if(argLen == 2)
			{
				const int				tokenPosition =
					currentExpression.getOpCodeMapValue(opPos);

				const XObject* const	name =
					currentExpression.getToken(tokenPosition);
				assert(name != 0);

				if (equals(context->getNodeName(), name->str()) == true)
				{
					score = xpath.s_MatchScoreQName;
				}
			}
			else
			{
				executionContext.error(TranscodeFromLocalCodePage("Arg length of processing-instruction() node test is incorrect!"));
			}
		}

		break;

	case XPathExpression::eNODETYPE_NODE:
		if (nodeType == XalanNode::CDATA_SECTION_NODE ||
			nodeType == XalanNode::TEXT_NODE)
		{
			if (executionContext.shouldStripSourceNode(*context) == false)
			{
				score = xpath.s_MatchScoreNodeTest;
			}
		}
		else
		{
			score = xpath.s_MatchScoreNodeTest;
		}
		break;

	case XPathExpression::eNODETYPE_ROOT:
		if (XalanNode::DOCUMENT_FRAGMENT_NODE == nodeType ||
			XalanNode::DOCUMENT_NODE == nodeType)
		{
			score =  xpath.s_MatchScoreOther;
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
						test = equals(contextNS, targetNS);

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
									equals(attrName, DOMServices::s_XMLNamespace);

							if(XPathExpression::eELEMWILDCARD == queueIndex)
							{
								if(stepType == XPathExpression::eFROM_ATTRIBUTES)
								{
									if (isNamespace == false)
									{
										score = xpath.s_MatchScoreNodeTest;
									}
								}
								else
								{
									if (isNamespace == true)
									{
										score = xpath.s_MatchScoreNodeTest;
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

										if (equals(localAttrName, targetLocalName) == true)
										{
											score = xpath.s_MatchScoreQName;
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

										if (equals(theNamespace, targetLocalName) == true)
										{
											score = xpath.s_MatchScoreQName;
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
									XPath::s_MatchScoreNSWild : XPath::s_MatchScoreNodeTest;
							}
							else
							{
								assert(queueIndex >= 0);

								const XalanDOMString&	targetLocalName =
															currentExpression.getToken(queueIndex)->str();

								if (equals(DOMServices::getLocalNameOfNode(*context),
										   targetLocalName) == true)
								{
									score = xpath.s_MatchScoreQName;
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
SimpleNodeLocator::predicates(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode*				/* context */, 
			int 					opPos,
			MutableNodeRefList& 	subQueryResults,
			int&					endPredicatesPos)
{
	const XPathExpression&	currentExpression =
		xpath.getExpression();

	int 					nextStepType =
			currentExpression.getOpCodeMapValue(opPos);

	while(XPathExpression::eOP_PREDICATE == nextStepType)
	{
		unsigned int 		i = 0;

		const unsigned int	theLength = subQueryResults.getLength();

		while(i < theLength)
		{
			XalanNode* const	theNode = subQueryResults.item(i);
			assert(theNode != 0);

			const XObjectPtr		pred(xpath.predicate(theNode, opPos, executionContext));
			assert(pred.get() != 0);

			// Remove any node that doesn't satisfy the predicate.
			if(XObject::eTypeNumber == pred->getType() &&
					i + 1 != pred->num() ||
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
