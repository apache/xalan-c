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
#include "SimpleNodeLocator.hpp"



#include <include/DOMHelper.hpp>
#include <PlatformSupport/DirectoryEnumerator.hpp>



#include <dom/DOM_Node.hpp>
#include <dom/DOM_Attr.hpp>
#include <dom/DOM_Document.hpp>



#include "FoundIndex.hpp"
#include "NodeRefListBase.hpp"
#include "XObject.hpp"
#include "XObjectFactory.hpp"
#include "XPath.hpp"



SimpleNodeLocator	SimpleNodeLocator::s_defaultInstance;



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



XObject*
SimpleNodeLocator::connectToNodes(
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		/* context */, 
			int 					opPos,
			std::vector<XObject*>	connectArgs)
{
	assert(connectArgs.size() > 0 && connectArgs.size() < 3);

	const XPathExpression&	currentExpression =
		xpath.getExpression();

	XObjectFactory& 	theFactory = executionContext.getXObjectFactory();

	XObjectGuard	results(theFactory,
							theFactory.createNodeSet(MutableNodeRefList()));

	const DOMString 	theFileSpec = connectArgs[0]->str();


	const DOMString 	filterSpec = connectArgs.size() > 1 ? connectArgs[0]->str() : "";
	const int			filterSpecLength = length(filterSpec);

	DirectoryEnumeratorFunctor<std::vector<DOMString> > 	theEnumerator;

	const std::vector<DOMString>		theFiles = theEnumerator(theFileSpec);

	const int							nFiles = theFiles.size();

	if (nFiles > 0)
	{
		MutableNodeRefList& 	theNodeList = results->mutableNodeset();

		for(int i = 0; i < nFiles; ++i)
		{
			try
			{
				// If there's no filter spec, or the spec matches the file, then
				// parse the XML.
				if (filterSpecLength == 0 ||
					endsWith(theFiles[i], filterSpec) == true)
				{
					DOM_Document	doc = executionContext.parseXML(theFiles[i], theFileSpec);

					if(0 != doc)
					{
						const int	value =
									currentExpression.getOpCodeMapValue(opPos);

						if(XPathExpression::eOP_LOCATIONPATH == value)
						{
							XObject* const	xnl = xpath.locationPath(doc, opPos);

							if(0 != xnl)
							{
								theNodeList.addNodes(xnl->nodeset());

								executionContext.associateXLocatorToNode(doc, this);
							}
						}
						else
						{
							theNodeList.addNode(doc);

							executionContext.associateXLocatorToNode(doc, this);
						}
					}
				}
			}
			catch(...)
			{
				executionContext.warn(DOMString("Couldn't parse XML file: ") + theFiles[i]);
			}
		}
	}
	else
	{
		executionContext.warn("No files matched the file specification!");
	}

	return results.release();
}



XObject*
SimpleNodeLocator::locationPath(
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		context, 
			int 					opPos)
{
	MutableNodeRefList	mnl = step(xpath, executionContext, context, opPos + 2);

	return executionContext.getXObjectFactory().createNodeSet(mnl);
}



double
SimpleNodeLocator::locationPathPattern(
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		context,
			int 					opPos)
{
	double	score = xpath.s_MatchScoreNone;

	stepPattern(xpath, executionContext, context, opPos + 2, score);

	return score;
}



MutableNodeRefList
SimpleNodeLocator::step(
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		context, 
			int 					opPos)
{
	const XPathExpression&	currentExpression =
		xpath.getExpression();

	// int endStep = xpath.getNextOpPos(opPos);
	const int			stepType =
		currentExpression.getOpCodeMapValue(opPos);

	int 				argLen = 0;

	MutableNodeRefList	subQueryResults(executionContext.createMutableNodeRefList());
	MutableNodeRefList	queryResults(subQueryResults);

	bool				shouldReorder = false;
	bool				continueStepRecursion = true;

	switch(stepType)
	{
	case XPathExpression::eOP_VARIABLE:
	case XPathExpression::eOP_EXTFUNCTION:
	case XPathExpression::eOP_FUNCTION:
	case XPathExpression::eOP_GROUP:
		argLen = findNodeSet(xpath, executionContext, context, opPos, 
							  stepType, subQueryResults);
		break;

	case XPathExpression::eFROM_ROOT:
		argLen = findRoot(xpath, executionContext, context, opPos, stepType, subQueryResults);
		break;

	case XPathExpression::eFROM_PARENT:
		argLen = findParent(xpath, executionContext, context, opPos, stepType, subQueryResults);
		break;

	case XPathExpression::eFROM_SELF:
		argLen = findSelf(xpath, executionContext, context, opPos, stepType, subQueryResults);
		break;

	case XPathExpression::eFROM_ANCESTORS:
		argLen = findAncestors(xpath, executionContext, context, opPos, stepType, subQueryResults);
		shouldReorder = true;
		break;

	case XPathExpression::eFROM_ANCESTORS_OR_SELF:
		argLen = findAncestorsOrSelf(xpath, executionContext, context, opPos, stepType, subQueryResults);
		shouldReorder = true;
		break;

	case XPathExpression::eMATCH_ATTRIBUTE:
		continueStepRecursion = false;
		// fall-through on purpose.

	case XPathExpression::eFROM_ATTRIBUTES:
		argLen = findAttributes(xpath, executionContext, context, opPos, stepType, subQueryResults);
		break;

	case XPathExpression::eMATCH_ANY_ANCESTOR:
	case XPathExpression::eMATCH_IMMEDIATE_ANCESTOR:
	case XPathExpression::eMATCH_ANY_ANCESTOR_WITH_PREDICATE:
		continueStepRecursion = false;
		// fall-through on purpose.

	case XPathExpression::eFROM_CHILDREN:
		argLen = findChildren(xpath, executionContext, context, opPos, stepType, subQueryResults);
		break;

	case XPathExpression::eFROM_DESCENDANTS:
	case XPathExpression::eFROM_DESCENDANTS_OR_SELF:
		argLen = findDescendants(xpath, executionContext, context, opPos, stepType, subQueryResults);
		break;

	case XPathExpression::eFROM_FOLLOWING:
		argLen = findFollowing(xpath, executionContext, context, opPos, stepType, subQueryResults);
		break;

	case XPathExpression::eFROM_FOLLOWING_SIBLINGS:
		argLen = findFollowingSiblings(xpath, executionContext, context, opPos, stepType, subQueryResults);
		break;

	case XPathExpression::eFROM_PRECEDING:
		argLen = findPreceeding(xpath, executionContext, context, opPos, stepType, subQueryResults);
		shouldReorder = true;
		break;

	case XPathExpression::eFROM_PRECEDING_SIBLINGS:
		argLen = findPreceedingSiblings(xpath, executionContext, context, opPos, stepType, subQueryResults);
		shouldReorder = true;
		break;

	case XPathExpression::eFROM_NAMESPACE:
		argLen = findNamespace(xpath, executionContext, context, opPos,  stepType, subQueryResults);
		break;

	default:
		argLen = findNodesOnUnknownAxis(xpath, executionContext, context, opPos, stepType, subQueryResults);
		break;
	}

	MutableNodeRefList	savedContextNodeList = executionContext.createMutableNodeRefList();

	savedContextNodeList = xpath.getContextNodeList();

	xpath.setContextNodeList(subQueryResults);

	try
	{
		opPos += argLen;

		int 	nextStepType = currentExpression.getOpCodeMapValue(opPos);

		if(XPathExpression::eOP_PREDICATE == nextStepType)
		{
			predicates(xpath,
					   executionContext,
					   context,
					   opPos, 
					   subQueryResults,
					   opPos);

			nextStepType = currentExpression.getOpCodeMapValue(opPos);
		}

		if(XPathExpression::eENDOP != nextStepType && continueStepRecursion == true)
		{

			const int	nContexts = subQueryResults.getLength();

			for(int i = 0; i < nContexts; i++)
			{
				const DOM_Node	node = subQueryResults.item(i);

				if(0 != node)
				{
					MutableNodeRefList	mnl = step(xpath, executionContext, node, opPos);

					if(queryResults.getLength() == 0)
					{
						queryResults = mnl;
					}
					else
					{
						queryResults.addNodesInDocOrder(mnl);
					}
				}
			}
		}
		else
		{
			if(shouldReorder == true)
			{
				queryResults.addNodesInDocOrder(subQueryResults);
			}
			else
			{
				queryResults = subQueryResults;
			}
		}
	}
	catch(...)
	{
		xpath.setContextNodeList(savedContextNodeList);

		throw;
	}

	xpath.setContextNodeList(savedContextNodeList);

	return queryResults;
}



DOM_Node
SimpleNodeLocator:: stepPattern(
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		context, 
			int 					opPos,
			double& 				score)
{
	const XPathExpression&	currentExpression =
		xpath.getExpression();

	const int	startOpPos = opPos;
	const int	stepType = currentExpression.getOpCodeMapValue(opPos);

	const int	endStep = currentExpression.getNextOpCodePosition(opPos);
	int			nextStepType = currentExpression.getOpCodeMapValue(endStep);

	DOM_Node	localContext(context);

	if(XPathExpression::eENDOP != nextStepType)
	{
		// Continue step via recursion...
		localContext = stepPattern(xpath,
								   executionContext,
								   localContext,
								   endStep,
								   score);

		if(0 == localContext)
		{
			score = xpath.s_MatchScoreNone;

			// !!!!!!!!!!!!! Big ugly return here !!!!!!!!!!!!!!!!!!!
			return DOM_Node();
		}

		score = xpath.s_MatchScoreOther;

		localContext = executionContext.getParentOfNode(localContext);

		if(0 == localContext)
		{
			// !!!!!!!!!!!!! Big ugly return here !!!!!!!!!!!!!!!!!!!
			return DOM_Node();
		}
	}

	int argLen = 0;

	switch(stepType)
	{
	case XPathExpression::eOP_FUNCTION:
		{
			argLen = currentExpression.getOpCodeLength(opPos);

			const XObject*	const	obj = xpath.execute(localContext, opPos);

			const NodeRefListBase&	nl = obj->nodeset();

			const int				len = nl.getLength();

			score = xpath.s_MatchScoreNone;

			for(int i = 0; i < len; i++)
			{
				const DOM_Node	n = nl.item(i);

				score = n == localContext ?
					xpath.s_MatchScoreOther : xpath.s_MatchScoreNone;

				if(score == xpath.s_MatchScoreOther)
				{
					localContext = n;

					break;
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

			const DOM_Document	docContext = DOM_Node::DOCUMENT_NODE == localContext.getNodeType() ? 
								static_cast<const DOM_Document&>(localContext) :
									localContext.getOwnerDocument();

			score = docContext == localContext ? xpath.s_MatchScoreOther : xpath.s_MatchScoreNone;

			if(score == xpath.s_MatchScoreOther)
			{
				localContext = docContext;
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
							 localContext,
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

			opPos += 3;

			score = xpath.s_MatchScoreNone;

			while(0 != localContext)
			{
				score = nodeTest(xpath, executionContext, localContext, opPos, argLen, stepType);

				if(xpath.s_MatchScoreNone != score)
					break;

				localContext = executionContext.getParentOfNode(localContext);
			}
		}
		break;

	case XPathExpression::eMATCH_IMMEDIATE_ANCESTOR:
		// $$ ToDO: Can we reduce this to some call on the
		// XPathExpression interface?
		argLen =
				currentExpression.getOpCodeMapValue(opPos + XPathExpression::s__opCodeMapLengthIndex + 1) - 3;

		opPos += 3;

		score = nodeTest(xpath, executionContext, localContext, opPos, argLen, stepType);
		break;

	default:
		// $$ ToDO: Can we reduce this to some call on the
		// XPathExpression interface?
		argLen =
				currentExpression.getOpCodeMapValue(opPos + XPathExpression::s__opCodeMapLengthIndex + 1) - 3;

		opPos += 3;

		score = xpath.s_MatchScoreNone;
	  
		executionContext.error("unknown match operation!", localContext);
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
			xpath.setThrowFoundIndex(true);

			// $$$ ToDo: Why is this variable introduced?
			int startPredicates = opPos;
			opPos = startPredicates;

			while(XPathExpression::eOP_PREDICATE == nextStepType)
			{
				const XObject* const	pred =
					xpath.predicate(localContext, opPos);

				if(XObject::eTypeNumber == pred->getType())
				{
					throw FoundIndex();
				}
				else if(pred->boolean() == false)
				{
					score = xpath.s_MatchScoreNone;

					break; // from while(xpath.OP_PREDICATE == nextStepType)
				}

				opPos = currentExpression.getNextOpCodePosition(opPos);
				nextStepType = currentExpression.getOpCodeMapValue(opPos);
			}

			xpath.setThrowFoundIndex(false);
		}
		catch(const FoundIndex&)
		{
			// We have an index somewhere in our pattern.  So, we have 
			// to do a full search for our step, using the parent as 
			// localContext, then see if the current localContext is found in the 
			// node set.  Seems crazy, but, so far, it seems like the 
			// easiest way.
			xpath.setThrowFoundIndex(false);

			const DOM_Node		parentContext =
				executionContext.getParentOfNode(localContext);

			MutableNodeRefList	mnl = step(xpath, executionContext, parentContext, startOpPos);

			const int			nNodes = mnl.getLength();

			score = xpath.s_MatchScoreNone;

			for(int i = 0; i < nNodes; i++)
			{
				if(mnl.item(i) == localContext)
				{
					score = xpath.s_MatchScoreOther;

					break;
				}
			}
		}
	}

	return score == xpath.s_MatchScoreNone ? DOM_Node() : localContext;
}



int
SimpleNodeLocator::findNodeSet(
			XPath&					xpath,
			XPathExecutionContext&	/* executionContext */,
			const DOM_Node& 		context,
			int 					opPos,
			int 					/* stepType */,
			MutableNodeRefList& 	subQueryResults)
{
	const XPathExpression&	currentExpression =
		xpath.getExpression();

	const XObject* const	obj = xpath.execute(context, opPos);

	const NodeRefListBase&	nl = obj->nodeset();

	// Should this be adding in doc order?
	// We can not simply assign the nl value to 
	// subQueryResults, because nl may be a ref to 
	// a variable or the like, and we may mutate 
	// below... which results in a hard-to-find bug!
	subQueryResults.addNodes(nl);

	return currentExpression.getOpCodeLength(opPos);
}



int
SimpleNodeLocator::findRoot(
			XPath&					xpath,
			XPathExecutionContext&	/* executionContext */,
			const DOM_Node& 		context,
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

	DOM_Document	docContext = DOM_Node::DOCUMENT_NODE == context.getNodeType() ?
									static_cast<const DOM_Document&>(context) :
										context.getOwnerDocument();

	subQueryResults.addNode(docContext);

	return argLen + 3;
}



int
SimpleNodeLocator::findParent(
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		context,
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

	const DOM_Node	theParent = executionContext.getParentOfNode(context);

	if(0 != theParent)
	{
		if(argLen > 0)
		{
			const double	score = nodeTest(xpath,
											 executionContext,
											 theParent,
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
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		context,
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
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		context,
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults)
{
	const XPathExpression&	currentExpression =
		xpath.getExpression();

	DOM_Node	contextNode =
		executionContext.getParentOfNode(context);

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
										 opPos,
										 argLen,
										 stepType);

		if(xpath.s_MatchScoreNone != score)
		{
			subQueryResults.addNode(contextNode);
		}

		contextNode = executionContext.getParentOfNode(contextNode);
	}

	return argLen + 3;
}



int
SimpleNodeLocator::findAncestorsOrSelf(
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		context,
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults)
{
	// $$ ToDO: Can this function be reduced to calling
	// findSelf() and findAncestors()?	I would think so...

	const XPathExpression&	currentExpression =
		xpath.getExpression();

	DOM_Node	contextNode(context);

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
										 opPos,
										 argLen,
										 stepType);

		if(xpath.s_MatchScoreNone != score)
		{
			subQueryResults.addNode(contextNode);
		}

		contextNode = executionContext.getParentOfNode(contextNode);
	}

	return argLen + 3;
}



int
SimpleNodeLocator::findAttributes(
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		context,
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

	if(0 != context && context.getNodeType() == DOM_Node::ELEMENT_NODE)
	{
		const DOM_Element&	e =
			static_cast<const DOM_Element&>(context);

		DOM_NamedNodeMap	attributeList = e.getAttributes();

		if(attributeList != 0) 
		{
			opPos += 3;

			const int	nAttrs = attributeList.getLength();

			for(int j = 0; j < nAttrs; j++)
			{
				const DOM_Node	theNode = attributeList.item(j);
				assert(theNode.getNodeType() == DOM_Node::ATTRIBUTE_NODE);

				const double	score = nodeTest(xpath,
												 executionContext,
												 theNode,
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
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		context,
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

	DOM_Node	child = context.getFirstChild();

	while(0 != child)
	{
		const double	score = nodeTest(xpath,
										 executionContext,
										 child,
										 opPos,
										 argLen,
										 stepType);

		if(xpath.s_MatchScoreNone != score)
		{
			subQueryResults.addNode(child);
		}

		child = child.getNextSibling();
	}

	return argLen + 3;
}



int
SimpleNodeLocator::findDescendants(
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		context,
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
	DOM_Node	pos = context;

	while(0 != pos)
	{					
		if(stepType == XPathExpression::eFROM_DESCENDANTS_OR_SELF ||
		   context != pos)
		{
			const double	score = nodeTest(xpath,
											 executionContext,
											 pos,
											 opPos,
											 argLen,
											 stepType);

			if(xpath.s_MatchScoreNone != score)
			{
				subQueryResults.addNode(pos);
			}
		}

		DOM_Node	nextNode = pos.getFirstChild();

		while(0 == nextNode)
		{
			if(context == pos)
				break;

			nextNode = pos.getNextSibling();

			if(0 == nextNode)
			{
				pos = pos.getParentNode();

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
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		context,
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
	const DOM_Document	doc = context.getOwnerDocument();

	DOM_Node			pos = context;

	while(0 != pos)
	{
		DOM_Node	nextNode;

		if(pos != context)
		{
			const double	score = nodeTest(xpath,
											 executionContext,
											 pos,
											 opPos,
											 argLen,
											 stepType);

			if(xpath.s_MatchScoreNone != score)
			{
				subQueryResults.addNodeInDocOrder(pos);
			}

			nextNode = pos.getFirstChild();
		}
		else
		{
			nextNode = 0;
		}

		while(0 == nextNode)
		{
			nextNode = pos.getNextSibling();

			if(0 == nextNode)
			{
				pos = pos.getParentNode();

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
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		context,
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

	DOM_Node	pos = context.getNextSibling();

	while(0 != pos)
	{
		const double	score = nodeTest(xpath,
										 executionContext,
										 pos,
										 opPos,
										 argLen,
										 stepType);

		if(xpath.s_MatchScoreNone != score)
		{
			subQueryResults.addNode(pos);
		}

		pos = pos.getNextSibling();
	}

	return argLen + 3;
}



int
SimpleNodeLocator::findPreceeding(
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		context,
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
	const DOM_Document	doc = context.getOwnerDocument();

	DOM_Node			pos = doc;

	while(0 != pos)
	{
		if(context == pos)
			break;

		const double	score = nodeTest(xpath,
										 executionContext,
										 pos,
										 opPos,
										 argLen,
										 stepType);

		if(xpath.s_MatchScoreNone != score)
		{
			// Ugh. If I could think a little better tonight, I'm
			// sure there's a better way to check for the parent.
			bool		isParent = false;

			DOM_Node	parent = executionContext.getParentOfNode(context);

			while(0 != parent)
			{
				if(parent == pos)
				{
					isParent = true;
					break;
				}

				parent = executionContext.getParentOfNode(parent);
			}

			if(isParent == false)
			{
			  subQueryResults.insertNode(pos, 0);
			}
		}

		DOM_Node	nextNode = pos.getFirstChild();

		while(0 == nextNode)
		{
			nextNode = pos.getNextSibling();

			if(0 == nextNode)
			{
				pos = pos.getParentNode();

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
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		context,
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

	DOM_Node	pos = context.getPreviousSibling();

	while(0 != pos)
	{
		const double	score = nodeTest(xpath,
										 executionContext,
										 pos,
										 opPos,
										 argLen,
										 stepType);

		if(xpath.s_MatchScoreNone != score)
		{
			subQueryResults.addNode(pos);
		}

		pos = pos.getPreviousSibling();
	}

	return argLen + 3;
}



int
SimpleNodeLocator::findNamespace(
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		context,
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

	executionContext.error("namespace axis not implemented yet!", context);

	return argLen + 3;
}



int
SimpleNodeLocator::findNodesOnUnknownAxis(
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		context,
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	/* subQueryResults */)
{
	const XPathExpression&	currentExpression =
		xpath.getExpression();

	// $$ ToDO: Can we reduce this to some call on the
	// XPathExpression interface?
	const int	argLen =
		currentExpression.getOpCodeMapValue(opPos + XPathExpression::s__opCodeMapLengthIndex + 1) - 3;

	executionContext.error("unknown axis: " + stepType, context);

	return argLen + 3;
}



double
SimpleNodeLocator::nodeTest(
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		context,
			int 					opPos,
			int 					argLen,
			int 					stepType)
{
	const XPathExpression&	currentExpression =
		xpath.getExpression();

	double score = 0.0L;

	const int	testType = currentExpression.getOpCodeMapValue(opPos);

	const int	nodeType = context.getNodeType();

	opPos++;

	switch(testType)
	{
	case XPathExpression::eNODETYPE_COMMENT:
		score = DOM_Node::COMMENT_NODE == nodeType
			  ? xpath.s_MatchScoreNodeTest : xpath.s_MatchScoreNone;
		break;

	case XPathExpression::eNODETYPE_TEXT:
	  score = (DOM_Node::CDATA_SECTION_NODE == nodeType ||
					DOM_Node::TEXT_NODE == nodeType) &&
			  executionContext.shouldStripSourceNode(context) == false
			  ? xpath.s_MatchScoreNodeTest : xpath.s_MatchScoreNone;
	  break;

	case XPathExpression::eNODETYPE_PI:
		if(DOM_Node::PROCESSING_INSTRUCTION_NODE != nodeType)
		{
			score = xpath.s_MatchScoreNone;
		}
		else
		{
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

				const DOM_ProcessingInstruction&	theNode =
					reinterpret_cast<const DOM_ProcessingInstruction&>(context);

				score = equals(theNode.getNodeName(), name->str())
						? xpath.s_MatchScoreQName : xpath.s_MatchScoreNone;
			}
			else
			{
				score = xpath.s_MatchScoreNone;

				executionContext.error("Arg length of processing-instruction() node test is incorrect!");
			}
		}

		break;

	case XPathExpression::eNODETYPE_NODE:
		if (nodeType == DOM_Node::CDATA_SECTION_NODE ||
			nodeType ==	DOM_Node::TEXT_NODE)
		{
			if (executionContext.shouldStripSourceNode(context) == false)
			{
				score = xpath.s_MatchScoreNodeTest;
			}
			else
			{
				score = xpath.s_MatchScoreNone;
			}
		}
		else
		{
			score = xpath.s_MatchScoreNodeTest;
		}
		break;

	case XPathExpression::eNODETYPE_ROOT:
		score = DOM_Node::DOCUMENT_FRAGMENT_NODE == nodeType ||
				DOM_Node::DOCUMENT_NODE == nodeType ?
					xpath.s_MatchScoreOther :
						xpath.s_MatchScoreNone;
		break;

	case XPathExpression::eNODENAME:
		{
			bool				test = false;

			int 				queueIndex = currentExpression.getOpCodeMapValue(opPos);

			const DOMString 	targetNS = queueIndex >= 0 ?
									currentExpression.getToken(queueIndex)->str() :
										"";

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

			const bool	processNamespaces = executionContext.getProcessNamespaces();

			bool		didMatchNS = false;

			if(isTotallyWild == false && processNamespaces == true)
			{
				const DOMString 	contextNS = executionContext.getNamespaceOfNode(context);

				if(0 != length(targetNS) && 0 != length(contextNS))
				{
					test = equals(contextNS, targetNS);

					didMatchNS = true;
				}
				else
				{
					test = XPathExpression::eELEMWILDCARD == queueIndex || 
					   0 == length(contextNS) || 0 == length(contextNS);
				}
			}
			else
			{
				test = true;
			}

			queueIndex = currentExpression.getOpCodeMapValue(opPos);

			const DOMString 	targetLocalName =
						queueIndex >= 0 ? currentExpression.getToken(queueIndex)->str() : "";

			if(test == false)
			{
				score = XPath::s_MatchScoreNone;
			}
			else
			{
				switch(nodeType)
				{
				case DOM_Node::ATTRIBUTE_NODE:
					if(stepType == XPathExpression::eFROM_ATTRIBUTES)
					{
						assert(context.getNodeType() == DOM_Node::ATTRIBUTE_NODE);

						if(XPathExpression::eELEMWILDCARD == queueIndex)
						{
							if(processNamespaces == true)
							{
								const DOM_Attr& 	theNode =
									reinterpret_cast<const DOM_Attr&>(context);

								const DOMString 	attrName =
									theNode.getNodeName();

								score = !(startsWith(attrName, "xmlns:") ||
										  equals(attrName, "xmlns"))
										? xpath.s_MatchScoreNodeTest : xpath.s_MatchScoreNone;
							}
							else
							{
								score = xpath.s_MatchScoreNodeTest;
							}
						}
						else
						{
							const DOMString 	localAttrName =
								executionContext.getLocalNameOfNode(context);

							score = equals(localAttrName, targetLocalName) ?
									xpath.s_MatchScoreQName : xpath.s_MatchScoreNone;
						}
					}
					else
					{
						score  = xpath.s_MatchScoreNone;
					}
					break;

				case DOM_Node::ELEMENT_NODE:
					if(stepType != XPathExpression::eFROM_ATTRIBUTES)
					{
						if(XPathExpression::eELEMWILDCARD == queueIndex)
						{
							score = didMatchNS == true ?
								XPath::s_MatchScoreNSWild : XPath::s_MatchScoreNodeTest;
						}
						else
						{
							const DOM_Element&	theNode =
								static_cast<const DOM_Element&>(context);

							score = equals(executionContext.getLocalNameOfNode(theNode), targetLocalName) ?
								xpath.s_MatchScoreQName : xpath.s_MatchScoreNone;
						}
					}
					else
					{
						score  = xpath.s_MatchScoreNone;
					}
					break;

				default:
					// Trying to match on anything else causes nasty bugs.
					score  = xpath.s_MatchScoreNone;
					break;
				} // end switch(nodeType)
			} // end if(test)
		} // end case XPathExpression::eNODENAME
		break;

	default:
		score  = xpath.s_MatchScoreNone;
		break;
	} // end switch(testType)

	return score;
}



void
SimpleNodeLocator::predicates(
			XPath&					xpath,
			XPathExecutionContext&	/* executionContext */,
			const DOM_Node& 		/* context */,
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
		int 		i = 0;

		const int	theLength = subQueryResults.getLength();

		// We'll accumulate the entries that we want to remove
		// here, then remove them all at once.
		std::vector<int>	theFailedEntries;

		// Might as well reserve some space now, although it's
		// probably bad to reserve the entire size of the
		// list results.
		theFailedEntries.reserve(theLength / 2);

		while(i < theLength)
		{
			const DOM_Node	theNode = subQueryResults.item(i);

			XObject* const	pred = xpath.predicate(theNode, opPos);

			// Remove any node that doesn't satisfy the predicate.
			if(XObject::eTypeNumber == pred->getType() &&
					i + 1 != pred->num() ||
			   pred->boolean() == false)
			{
				theFailedEntries.push_back(i);
			}

			++i;
		}

		// Erase from the back to the front, to preserve the validity
		// of the indesing, and so that we don't end up moving entries
		// that we would already be erasing...
		std::vector<int>::reverse_iterator	theIterator =
			theFailedEntries.rbegin();

		while(theIterator != theFailedEntries.rend())
		{
			subQueryResults.removeNode(*theIterator);

			theIterator++;
		}

		opPos = currentExpression.getNextOpCodePosition(opPos);

		nextStepType = currentExpression.getOpCodeMapValue(opPos);

		if(XPathExpression::eOP_PREDICATE == nextStepType)
		{
			xpath.setContextNodeList(subQueryResults);

			// Don't break, loop 'till end so that opPos will be set to end.
			// if(0 == subQueryResults.getLength())
			//	break;
		}
	}

	endPredicatesPos = opPos;
}
