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
#if !defined(SIMPLENODELOCATOR_HEADER_GUARD_1357924680)
#define SIMPLENODELOCATOR_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



// Base class header file
#include <XPath/XLocator.hpp>
#include <XPath/MutableNodeRefList.hpp>



class XPathExpression;



/**
 * SimpleNodeLocator implements a search of one or more DOM trees.
 * By using the connect function as an extension, the user may 
 * specify a directory and a filter specification for XML files 
 * that will be searched.
 * This is a singleton class.
 */
class XALAN_XPATH_EXPORT SimpleNodeLocator : public XLocator
{
public:

	static SimpleNodeLocator*
	getDefaultInstance();

	/**
	 * Create a SimpleNodeLocator object.
	 */
	explicit
	SimpleNodeLocator();

	virtual
	~SimpleNodeLocator();

	virtual XObject*
	connectToNodes(
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		context, 
			int 					opPos,
			std::vector<XObject*>	connectArgs);
  
	/**
	 * Execute a location path.  Normally, this method simply 
	 * moves past the OP_LOCATIONPATH and it's length member, 
	 * and calls the Step function, which will recursivly process 
	 * the rest of the LocationPath, and then wraps the NodeList result
	 * in an XNodeSet object.
	 * @param xpath The xpath that is executing.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the xpath.m_opMap array.
	 * @returns the result of the query in an XNodeSet object.
	 */
	virtual XObject*
	locationPath(
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		context, 
			int 					opPos);

	/**
	 * Execute a a location path pattern.  This will return a score
	 * of MATCH_SCORE_NONE, MATCH_SCORE_NODETEST, 
	 * MATCH_SCORE_OTHER, MATCH_SCORE_QNAME.
	 * @param xpath The xpath that is executing.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the xpath.m_opMap array.
	 * @returns score, one of MATCH_SCORE_NODETEST, 
	 * MATCH_SCORE_NONE, MATCH_SCORE_OTHER, MATCH_SCORE_QNAME.
	 */
	virtual double
	locationPathPattern(
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		context, 
			int 					opPos);

protected:

	MutableNodeRefList
	step(
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		context, 
			int 					opPos);

	/**
	 * Execute a step in a location path.
	 * @param xpath The xpath that is executing.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the xpath.m_opMap array.
	 * @returns the last matched context node.
	 */
	DOM_Node
	stepPattern(
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		context, 
			int 					opPos,
			double& 				score);

	int
	findNodeSet(
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		context,
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	int
	findRoot(
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		context,
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	int
	findParent(
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		context,
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	int
	findSelf(
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		context,
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	int
	findAncestors(
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		context,
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	int
	findAncestorsOrSelf(
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		context,
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	int
	findAttributes(
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		context,
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	int
	findChildren(
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		context,
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	int
	findDescendants(
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		context,
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	int
	findFollowing(
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		context,
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	int
	findFollowingSiblings(
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		context,
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	int
	findPreceeding(
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		context,
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	int
	findPreceedingSiblings(
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		context,
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	int
	findNamespace(
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		context,
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	int
	findNodesOnUnknownAxis(
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		context,
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	double
	nodeTest(
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		context,
			int 					opPos,
			int 					argLen,
			int 					stepType);

	void
	predicates(
			XPath&					xpath,
			XPathExecutionContext&	executionContext,
			const DOM_Node& 		context,
			int 					opPos,
			MutableNodeRefList& 	subQueryResults,
			int&					endPredicatesPos);

private:

	static SimpleNodeLocator	s_defaultInstance;
};



#if 0
  /**
   * The singleton instance of this class.
   */
  private static SimpleNodeLocator m_locater = null;
  
  /**
   * The the default locator.
   */
  public static SimpleNodeLocator getDefaultLocator()
  {
	m_locater = (null == m_locater) ? new SimpleNodeLocator() : m_locater;
	return m_locater;
  }
	  
  /**
   * Execute the proprietary connect() function, which returns an 
   * instance of XLocator.	When the XPath object sees a return type 
   * of XLocator, it will call the locationPath function that passes 
   * in the connectArgs.  The opPos and args params are not used 
   * by this function.	This really is just a factory function 
   * for the XLocator instance, but this fact is hidden from the 
   * user.
   * @param opPos The current position in the xpath.m_opMap array.
   * @param args The function args.
   * @returns A node set of Document nodes.
   */
  public static XLocator connect(String path, String fileSpec) 
  {    
	m_locater = (null == m_locater) ? new SimpleNodeLocator() : m_locater;
	return m_locater;
  }
 
  /**
   * Execute a connection (if it was not executed by the static 
   * connect method) and process the following LocationPath, 
   * if it is present.	Normally, the connection functionality 
   * should be executed by this function and not the static connect 
   * function, which is really a factory method for the XLocator 
   * instance.	The arguments to the static connect function
   * are re-passed to this function.
   * @param xpath The xpath that is executing.
   * @param context The current source tree context node.
   * @param opPos The current position in the xpath.m_opMap array.
   * @param connectArgs The same arguments that were passed to the 
   * static connect function.
   * @returns the result of the query in an XNodeSet object.
   */
  public XNodeSet connectToNodes(XPath xpath, Node context, 
							   int opPos, Vector connectArgs) 
  {    
	String fileSpec = ((XObject)connectArgs.elementAt(0)).str();
	FileFilter filter = null;
	String filterSpec = null;
	if(connectArgs.size() > 1)
	{
	  filterSpec = ((XObject)connectArgs.elementAt(1)).str();
	  filter = new FileFilter(filterSpec);
	}
	
	File dir = new File(fileSpec);
	
	XNodeSet results = new XNodeSet(xpath.m_callbacks);
	MutableNodeList mnl = results.mutableNodeset();
	
	if(null != dir)
	{
	  String filenames[] = (filter != null) ? dir.list(filter) : dir.list();
	  if(null != filenames)
	  {
		int nFiles = filenames.length;
		for(int i = 0; i < nFiles; i++)
		{
		  try
		  {
			Document doc = xpath.m_callbacks.parseXML(filenames[i], fileSpec);
			if(null != doc)
			{
			  if((xpath.OP_LOCATIONPATH == xpath.m_opMap[opPos]))
			  {
				XNodeSet xnl = xpath.locationPath(doc, opPos);
				if(null != xnl)
				{
				  mnl.addNodes(xnl.nodeset());
				  xpath.m_callbacks.associateXLocatorToNode(doc, this);
				}
			  }
			  else
			  {
				mnl.addNode(doc);
				xpath.m_callbacks.associateXLocatorToNode(doc, this);
			  }
			}
		  }
		  catch(Exception e)
		  {
			System.out.println("Couldn't parse file: "+e.getMessage());
		  }
		}
	  }
	  else
	  {
		System.out.println("Couldn't get a file list from filespec");
	  }
	}
	else
	{
	  System.out.println("Filespec was bad in connect");
	}
			
	return results;
  }

  /**
   * Execute a location path.  Normally, this method simply 
   * moves past the OP_LOCATIONPATH and it's length member, 
   * and calls the Step function, which will recursivly process 
   * the rest of the LocationPath, and then wraps the NodeList result
   * in an XNodeSet object.
   * @param xpath The xpath that is executing.
   * @param context The current source tree context node.
   * @param opPos The current position in the xpath.m_opMap array.
   * @returns the result of the query in an XNodeSet object.
   */
  public XNodeSet locationPath(XPath xpath, Node context, int opPos) 
  {    
	opPos+=2;
	XNodeSet results;
	MutableNodeList mnl = step(xpath, context, opPos);
	if(null != mnl)
	{
	  results = new XNodeSet(mnl, xpath.m_callbacks);
	}
	else
	{
	  results = new XNodeSet(new MutableNodeListImpl(xpath.m_callbacks), xpath.m_callbacks);
	}
	
	return results;
  }
  
  /**
   * Execute a step and predicates in a location path.	This recursivly 
   * executes each of the steps and predicates.
   * The step can be oneof xpath.OP_VARIABLE, OP_EXTFUNCTION,
   * OP_FUNCTION, OP_GROUP, FROM_ROOT, FROM_PARENT, FROM_SELF,
   * FROM_ANCESTORS, FROM_ANCESTORS_OR_SELF, MATCH_ATTRIBUTE,
   * FROM_ATTRIBUTES, MATCH_ANY_ANCESTOR, MATCH_IMMEDIATE_ANCESTOR,
   * FROM_CHILDREN, FROM_DESCENDANTS,
   * FROM_DESCENDANTS_OR_SELF, FROM_FOLLOWING, FROM_FOLLOWING_SIBLINGS,
   * FROM_PRECEDING, FROM_PRECEDING_SIBLINGS, or FROM_NAMESPACE.
   * Normally, this function should recurse to process the next 
   * step.	However, it should not continue to process the location 
   * path if the step is oneof MATCH_ATTRIBUTE, MATCH_ANY_ANCESTOR, or 
   * match MATCH_IMMEDIATE_ANCESTOR.
   * This method may be overridden to process the LocationPath as 
   * a whole, or the fromXXX methods may be overridden to process  
   * the steps as individual units.
   * @param xpath The xpath that is executing.
   * @param context The current source tree context node.
   * @param opPos The current position in the xpath.m_opMap array.
   * @returns a node-set.
   */
  protected MutableNodeList step(XPath xpath, Node context, int opPos) 
  {    
	// int endStep = xpath.getNextOpPos(opPos);
	int stepType = xpath.m_opMap[opPos];
	int argLen;
	MutableNodeList subQueryResults = new MutableNodeListImpl(xpath.m_callbacks);
	MutableNodeList queryResults = null;
	boolean shouldReorder = false;
	boolean continueStepRecursion = true;
	switch(stepType)
	{
	case xpath.OP_VARIABLE:
	case xpath.OP_EXTFUNCTION:
	case xpath.OP_FUNCTION:
	case xpath.OP_GROUP:
		argLen = findNodeSet(xpath, context, opPos, 
							  stepType, subQueryResults);
	  break;
	case xpath.FROM_ROOT:
	  argLen = findRoot(xpath, context, opPos, stepType, subQueryResults);
	  break;
	case xpath.FROM_PARENT:
	  argLen = findParent(xpath, context, opPos, stepType, subQueryResults);
	  break;
	case xpath.FROM_SELF:
	  argLen = findSelf(xpath, context, opPos, stepType, subQueryResults);
	  break;
	case xpath.FROM_ANCESTORS:
	  argLen = findAncestors(xpath, context, opPos,  stepType, subQueryResults);
	  shouldReorder = true;
	  break;
	case xpath.FROM_ANCESTORS_OR_SELF:
	  argLen = findAncestorsOrSelf(xpath, context, opPos,  stepType, subQueryResults);
	  shouldReorder = true;
	  break;
	case xpath.MATCH_ATTRIBUTE:
	  continueStepRecursion = false;
	  // fall-through on purpose.
	case xpath.FROM_ATTRIBUTES:
	  argLen = findAttributes(xpath, context, opPos,  stepType, subQueryResults);
	  break;
	case xpath.MATCH_ANY_ANCESTOR:
	case xpath.MATCH_IMMEDIATE_ANCESTOR:
	  continueStepRecursion = false;
	  // fall-through on purpose.
	case xpath.FROM_CHILDREN:
	  argLen = findChildren(xpath, context, opPos,	stepType, subQueryResults);
	  break;
	case xpath.FROM_DESCENDANTS:
	case xpath.FROM_DESCENDANTS_OR_SELF:
	  argLen = findDescendants(xpath, context, opPos,  stepType, subQueryResults);
	  break;
	case xpath.FROM_FOLLOWING:
	  argLen = findFollowing(xpath, context, opPos,  stepType, subQueryResults);
	  break;
	case xpath.FROM_FOLLOWING_SIBLINGS:
	  argLen = findFollowingSiblings(xpath, context, opPos,  stepType, subQueryResults);
	  break;
	case xpath.FROM_PRECEDING:
	  argLen = findPreceding(xpath, context, opPos,  stepType, subQueryResults);
	  shouldReorder = true;
	  break;
	case xpath.FROM_PRECEDING_SIBLINGS:
	  argLen = findPrecedingSiblings(xpath, context, opPos,  stepType, subQueryResults);
	  shouldReorder = true;
	  break;
	case xpath.FROM_NAMESPACE:
	  argLen = findNamespace(xpath, context, opPos,  stepType, subQueryResults);
	  break;
	default:
	  argLen = findNodesOnUnknownAxis(xpath, context, opPos,  stepType, subQueryResults);
	  break;
	}
	opPos += argLen;
	int nextStepType = xpath.m_opMap[opPos];
	
	NodeList savedContextNodeList = xpath.m_contextNodeList;
	xpath.m_contextNodeList = subQueryResults;
	
	if(xpath.OP_PREDICATE == nextStepType)
	{
	  int[] endPredicatesPos = {-42};
	  subQueryResults = predicates(xpath, context, opPos, 
								   subQueryResults, endPredicatesPos);
	  opPos = endPredicatesPos[0];
	  nextStepType = xpath.m_opMap[opPos];
	}
	
	if((xpath.ENDOP != nextStepType) && continueStepRecursion)
	{
	  int nContexts = subQueryResults.getLength();
	  for(int i = 0; i < nContexts; i++)
	  {
		Node node = subQueryResults.item(i);
		if(null != node)
		{
		  MutableNodeList mnl = step(xpath, node, opPos);
		  if(queryResults == null)
		  {
			queryResults = mnl;
		  }
		  else
		  {
			queryResults.addNodesInDocOrder(mnl);
		  }
		}
	  }
	  if(null == queryResults)
	  {
		queryResults = new MutableNodeListImpl(xpath.m_callbacks);
	  }
	}
	else
	{
	  if(shouldReorder)
	  {
		queryResults = new MutableNodeListImpl(xpath.m_callbacks);
		queryResults.addNodesInDocOrder(subQueryResults);
	  }
	  else
	  {
		queryResults = subQueryResults;
	  }
	}
	xpath.m_contextNodeList = savedContextNodeList;
	return queryResults;
  }
  
  /**
   * Execute a step that performs an OP_VARIABLE, OP_EXTFUNCTION,
   * OP_FUNCTION, or OP_GROUP function.
   * @param xpath The xpath that is executing.
   * @param context The current source tree context node.
   * @param opPos The current position in the xpath.m_opMap array.
   * @param stepType One of OP_VARIABLE, OP_EXTFUNCTION,
   * OP_FUNCTION, or OP_GROUP.
   * @param subQueryResults Should be an empty node list where the 
   * results of the step will be put.
   * @returns the length of the argument (i.e. amount to add to predicate pos 
   * or end of step).
   */
  protected int findNodeSet(XPath xpath, Node context, int opPos, 
							  int stepType, MutableNodeList subQueryResults)
  {
		XObject obj = xpath.execute(context, opPos);
		NodeList nl = obj.nodeset();
		
		// Should this be adding in doc order?
		// We can not simply assign the nl value to 
		// subQueryResults, because nl may be a ref to 
		// a variable or the like, and we may mutate 
		// below... which results in a hard-to-find bug!
		subQueryResults.addNodes(nl);
		return xpath.m_opMap[opPos+xpath.MAPINDEX_LENGTH];
  }
  
  /**
   * Execute a step to the root.
   * @param xpath The xpath that is executing.
   * @param context The current source tree context node.
   * @param opPos The current position in the xpath.m_opMap array.
   * @param stepType Value of xpath.FROM_ROOT.
   * @param subQueryResults Should be an empty node list where the 
   * results of the step will be put.
   * @returns the length of the argument (i.e. amount to add to predicate pos 
   * or end of step).
   */
  protected int findRoot(XPath xpath, Node context, int opPos, 
						   int stepType, MutableNodeList subQueryResults)
  {
	int argLen = xpath.m_opMap[opPos+xpath.MAPINDEX_LENGTH+1]-3;
	opPos += 3;

	Document docContext = (Node.DOCUMENT_NODE == context.getNodeType()) 
						  ? (Document)context : context.getOwnerDocument();
	subQueryResults.addNode(docContext);
	return argLen+3;
  }
  
  /**
   * Add the parent to the list if it meets the NodeTest qualification.
   * @param xpath The xpath that is executing.
   * @param context The current source tree context node.
   * @param opPos The current position in the xpath.m_opMap array.
   * @param stepType Value of xpath.FROM_PARENT.
   * @param subQueryResults Should be an empty node list where the 
   * results of the step will be put.
   * @returns the length of the argument (i.e. amount to add to predicate pos 
   * or end of step).
   */
  protected int findParent(XPath xpath, Node context, int opPos, 
							 int stepType, MutableNodeList subQueryResults)
  {
	context = xpath.m_callbacks.getParentOfNode(context);
	int argLen = xpath.m_opMap[opPos+xpath.MAPINDEX_LENGTH+1]-3;
	if(null != context)
	{
	  opPos += 3;
	  if(argLen > 0)
	  {
		if(xpath.MATCH_SCORE_NONE != nodeTest(xpath, context, opPos, argLen, stepType))
		{
		  subQueryResults.addNode(context);
		}
	  }
	  else
	  {
		subQueryResults.addNode(context);
	  }
	}
	return argLen+3;
  }
  
  /**
   * Add the context to the list if it meets the NodeTest qualification.
   * @param xpath The xpath that is executing.
   * @param context The current source tree context node.
   * @param opPos The current position in the xpath.m_opMap array.
   * @param stepType Value of xpath.FROM_SELF.
   * @param subQueryResults Should be an empty node list where the 
   * results of the step will be put.
   * @returns the length of the argument (i.e. amount to add to predicate pos 
   * or end of step).
   */
  protected int findSelf(XPath xpath, Node context, int opPos, 
							 int stepType, MutableNodeList subQueryResults)
  {
	int argLen = xpath.m_opMap[opPos+xpath.MAPINDEX_LENGTH+1]-3;
	opPos += 3;
	if(argLen > 0)
	{
	  if(xpath.MATCH_SCORE_NONE != nodeTest(xpath, context, opPos, argLen, stepType))
	  {
		subQueryResults.addNode(context);
	  }
	}
	else
	{
	  subQueryResults.addNode(context);
	}
	return argLen+3;
  }

  /**
   * Add ancestors to the list if they meet the NodeTest qualification.
   * @param xpath The xpath that is executing.
   * @param context The current source tree context node.
   * @param opPos The current position in the xpath.m_opMap array.
   * @param stepType Value of xpath.FROM_ANCESTORS.
   * @param subQueryResults Should be an empty node list where the 
   * results of the step will be put.
   * @returns the length of the argument (i.e. amount to add to predicate pos 
   * or end of step).
   */
  protected int findAncestors(XPath xpath, Node context, int opPos, 
							 int stepType, MutableNodeList subQueryResults)
  {
	context = xpath.m_callbacks.getParentOfNode(context);
	int argLen = xpath.m_opMap[opPos+xpath.MAPINDEX_LENGTH+1]-3;
	opPos += 3;
	while(null != context)
	{
	  if(xpath.MATCH_SCORE_NONE != nodeTest(xpath, context, opPos, argLen, stepType))
	  {
		subQueryResults.addNode(context);
	  }
	  context = xpath.m_callbacks.getParentOfNode(context);
	}
	return argLen+3;
  }
 
  /**
   * Add ancestors or the context to the list if they meet 
   * the NodeTest qualification.
   * @param xpath The xpath that is executing.
   * @param context The current source tree context node.
   * @param opPos The current position in the xpath.m_opMap array.
   * @param stepType Value of xpath.FROM_ANCESTORS_OR_SELF.
   * @param subQueryResults Should be an empty node list where the 
   * results of the step will be put.
   * @returns the length of the argument (i.e. amount to add to predicate pos 
   * or end of step).
   */
  protected int findAncestorsOrSelf(XPath xpath, Node context, int opPos, 
							 int stepType, MutableNodeList subQueryResults)
  {
	int argLen = xpath.m_opMap[opPos+xpath.MAPINDEX_LENGTH+1]-3;
	opPos += 3;
	while(null != context)
	{
	  if(xpath.MATCH_SCORE_NONE != nodeTest(xpath, context, opPos, argLen, stepType))
	  {
		subQueryResults.addNode(context);
	  }
	  context = xpath.m_callbacks.getParentOfNode(context);
	}
	return argLen+3;
  }

  /**
   * Add attributes to the list if they meet 
   * the NodeTest qualification.
   * @param xpath The xpath that is executing.
   * @param context The current source tree context node.
   * @param opPos The current position in the xpath.m_opMap array.
   * @param stepType Value of xpath.FROM_ATTRIBUTES.
   * @param subQueryResults Should be an empty node list where the 
   * results of the step will be put.
   * @returns the length of the argument (i.e. amount to add to predicate pos 
   * or end of step).
   */
  protected int findAttributes(XPath xpath, Node context, int opPos, 
								 int stepType, MutableNodeList subQueryResults)
  {
	int argLen = xpath.m_opMap[opPos+xpath.MAPINDEX_LENGTH+1]-3;
	opPos += 3;
	if( (null != context) && (context.getNodeType()==Node.ELEMENT_NODE) )
	{
	  Element e = (Element)context;
	  NamedNodeMap attributeList = e.getAttributes();
	  if( attributeList != null ) 
	  {
		int nAttrs = attributeList.getLength();
		for( int j=0; j < nAttrs; j++ )
		{
		  Attr attr = (Attr)attributeList.item(j);
		  if(xpath.MATCH_SCORE_NONE != nodeTest(xpath, attr, opPos, argLen, stepType))
		  {
			subQueryResults.addNode(attr);
		  }
		}
	  }
	}
	return argLen+3;
  }

  /**
   * Add children to the list if they meet 
   * the NodeTest qualification.
   * @param xpath The xpath that is executing.
   * @param context The current source tree context node.
   * @param opPos The current position in the xpath.m_opMap array.
   * @param stepType Value of xpath.FROM_CHILDREN.
   * @param subQueryResults Should be an empty node list where the 
   * results of the step will be put.
   * @returns the length of the argument (i.e. amount to add to predicate pos 
   * or end of step).
   */
  protected int findChildren(XPath xpath, Node context, int opPos, 
							   int stepType, MutableNodeList subQueryResults)
  {
	int argLen = xpath.m_opMap[opPos+xpath.MAPINDEX_LENGTH+1]-3;
	opPos += 3;
	Node c = context.getFirstChild();
	while( null != c )
	{
	  if(xpath.MATCH_SCORE_NONE != nodeTest(xpath, c, opPos, argLen, stepType))
	  {
		subQueryResults.addNode(c);
	  }
	  c = c.getNextSibling();
	}
	return argLen+3;
  }
  
  /**
   * Add the descendants (and the context if the stepType is 
   * FROM_DESCENDANTS_OR_SELF) to the list if they meet 
   * the NodeTest qualification.
   * @param xpath The xpath that is executing.
   * @param context The current source tree context node.
   * @param opPos The current position in the xpath.m_opMap array.
   * @param stepType Value of xpath.FROM_DESCENDANTS or xpath.FROM_DESCENDANTS_OR_SELF.
   * @param subQueryResults Should be an empty node list where the 
   * results of the step will be put.
   * @returns the length of the argument (i.e. amount to add to predicate pos 
   * or end of step).
   */
  protected int findDescendants(XPath xpath, Node context, int opPos, 
								int stepType, MutableNodeList subQueryResults)
  {
	int argLen = xpath.m_opMap[opPos+xpath.MAPINDEX_LENGTH+1]-3;
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
	Node pos = context;
	
	while(null != pos)
	{					
	  if((stepType == xpath.FROM_DESCENDANTS_OR_SELF) || (context != pos))
	  {
		if(xpath.MATCH_SCORE_NONE != nodeTest(xpath, pos, opPos, argLen, stepType))
		{
		  subQueryResults.addNode(pos);
		}
	  }
	  Node nextNode = pos.getFirstChild();
	  while(null == nextNode)
	  {
		if(context == pos)
		  break;
		nextNode = pos.getNextSibling();
		if(null == nextNode)
		{
		  pos = pos.getParentNode();
		  if((context == pos) || (pos == null))
		  {
			nextNode = null;
			break;
		  }
		}
	  }
	  pos = nextNode;
	}
	return argLen+3;
  }

  /**
   * Add the nodes following the context to the list if they meet 
   * the NodeTest qualification.
   * The following axis contains all nodes in the same document as 
   * the context node that are after the context node in document 
   * order, excluding any descendants and excluding attribute nodes 
   * and namespace nodes; the nodes are ordered in document order.
   * Note that the ancestor, descendant, following, preceding and 
   * self axes partition a document (ignoring attribute and namespace 
   * nodes): they do not overlap and together they contain all the 
   * nodes in the document.
   * @param xpath The xpath that is executing.
   * @param context The current source tree context node.
   * @param opPos The current position in the xpath.m_opMap array.
   * @param stepType Value of xpath.FROM_FOLLOWING.
   * @param subQueryResults Should be an empty node list where the 
   * results of the step will be put.
   * @returns the length of the argument (i.e. amount to add to predicate pos 
   * or end of step).
   */
  protected int findFollowing(XPath xpath, Node context, int opPos, 
								int stepType, MutableNodeList subQueryResults)
  {
	int argLen = xpath.m_opMap[opPos+xpath.MAPINDEX_LENGTH+1]-3;
	opPos += 3;

	// What fun...
	Document doc = context.getOwnerDocument();
	Node pos = context;
	while(null != pos)
	{  
	  Node nextNode;
	  if(pos != context)
	  {
		if(xpath.MATCH_SCORE_NONE != nodeTest(xpath, pos, opPos, argLen, stepType))
		{
		  subQueryResults.addNodeInDocOrder(pos);
		}
		nextNode = pos.getFirstChild();
	  }
	  else
	  {
		nextNode = null;
	  }
	  while(null == nextNode)
	  {
		nextNode = pos.getNextSibling();
		if(null == nextNode)
		{
		  pos = pos.getParentNode();
		  if((doc == pos) || (null == pos))
		  {
			nextNode = null;
			break;
		  }
		}
	  }
	  pos = nextNode;
	}
	return argLen+3;
  }
  


  /**
   * Add the sibling nodes following the context to the list if they meet 
   * the NodeTest qualification.
   * @param xpath The xpath that is executing.
   * @param context The current source tree context node.
   * @param opPos The current position in the xpath.m_opMap array.
   * @param stepType Value of xpath.FROM_FOLLOWING_SIBLINGS.
   * @param subQueryResults Should be an empty node list where the 
   * results of the step will be put.
   * @returns the length of the argument (i.e. amount to add to predicate pos 
   * or end of step).
   */
  protected int findFollowingSiblings(XPath xpath, Node context, int opPos, 
									  int stepType, MutableNodeList subQueryResults)
  {
	int argLen = xpath.m_opMap[opPos+xpath.MAPINDEX_LENGTH+1]-3;
	opPos += 3;

	Node pos = context.getNextSibling();
	while(null != pos)
	{					
	  if(xpath.MATCH_SCORE_NONE != nodeTest(xpath, pos, opPos, argLen, stepType))
	  {
		subQueryResults.addNode(pos);
	  }
	  pos = pos.getNextSibling();
	}
	return argLen+3;
  }
  
  /**
   * Add the nodes preceding the context to the list if they meet 
   * the NodeTest qualification.
   * The preceding axis contains all nodes in the same document 
   * as the context node that are before the context node in document 
   * order, excluding any ancestors and excluding attribute nodes 
   * and namespace nodes; the nodes are ordered in reverse document order.
   * Note that the ancestor, descendant, following, preceding and 
   * self axes partition a document (ignoring attribute and namespace 
   * nodes): they do not overlap and together they contain all the 
   * nodes in the document.
   * @param xpath The xpath that is executing.
   * @param context The current source tree context node.
   * @param opPos The current position in the xpath.m_opMap array.
   * @param stepType Value of xpath.FROM_PRECEDING.
   * @param subQueryResults Should be an empty node list where the 
   * results of the step will be put.
   * @returns the length of the argument (i.e. amount to add to predicate pos 
   * or end of step).
   */
  protected int findPreceding(XPath xpath, Node context, int opPos, 
								int stepType, MutableNodeList subQueryResults)
  {
	int argLen = xpath.m_opMap[opPos+xpath.MAPINDEX_LENGTH+1]-3;
	opPos += 3;

	// Ugh.  Reverse document order, no parents, I guess.
	Document doc = context.getOwnerDocument();
	Node pos = doc;
	while(null != pos)
	{		
	  if(context == pos)
		break;

	  if(xpath.MATCH_SCORE_NONE != nodeTest(xpath, pos, opPos, argLen, stepType))
	  {
		// Ugh. If I could think a little better tonight, I'm
		// sure there's a better way to check for the parent.
		boolean isParent = false;
		Node parent = xpath.m_callbacks.getParentOfNode(context);
		while(null != parent)
		{
		  if(parent == pos)
		  {
			isParent = true;
			break;
		  }
		  parent = xpath.m_callbacks.getParentOfNode(parent);
		}
		
		if(!isParent)
		{
		  subQueryResults.insertNode(pos, 0);
		}
	  }
	  Node nextNode = pos.getFirstChild();
	  while(null == nextNode)
	  {
		nextNode = pos.getNextSibling();
		if(null == nextNode)
		{
		  pos = pos.getParentNode();
		  if(doc == pos)
		  {
			nextNode = null;
			break;
		  }
		}
	  }
	  pos = nextNode;
	}
	return argLen+3;
  }
  
  /**
   * Add the sibling nodes preceding the context to the list if they meet 
   * the NodeTest qualification.
   * @param xpath The xpath that is executing.
   * @param context The current source tree context node.
   * @param opPos The current position in the xpath.m_opMap array.
   * @param stepType Value of xpath.FROM_PRECEDING_SIBLINGS.
   * @param subQueryResults Should be an empty node list where the 
   * results of the step will be put.
   * @returns the length of the argument (i.e. amount to add to predicate pos 
   * or end of step).
   */
  protected int findPrecedingSiblings(XPath xpath, Node context, int opPos, 
									  int stepType, MutableNodeList subQueryResults)
  {
	int argLen = xpath.m_opMap[opPos+xpath.MAPINDEX_LENGTH+1]-3;
	opPos += 3;

	Node pos = context.getPreviousSibling();
	while(null != pos)
	{					
	  if(xpath.MATCH_SCORE_NONE != nodeTest(xpath, pos, opPos, argLen, stepType))
	  {
		subQueryResults.addNode(pos);
	  }
	  pos = pos.getPreviousSibling();
	}
	return argLen+3;
  }
  
  /**
   * Add the namespace node of the context.
   * @param xpath The xpath that is executing.
   * @param context The current source tree context node.
   * @param opPos The current position in the xpath.m_opMap array.
   * @param stepType Value of xpath.FROM_NAMESPACE.
   * @param subQueryResults Should be an empty node list where the 
   * results of the step will be put.
   * @returns the length of the argument (i.e. amount to add to predicate pos 
   * or end of step).
   */
  protected int findNamespace(XPath xpath, Node context, int opPos, 
							  int stepType, MutableNodeList subQueryResults)
  {
	int argLen = xpath.m_opMap[opPos+xpath.MAPINDEX_LENGTH+1]-3;
	opPos += 3;
	xpath.error(context, "namespace axis not implemented yet!");
	return argLen+3;
  }
  
  /**
   * Add the namespace node of the context.
   * @param xpath The xpath that is executing.
   * @param context The current source tree context node.
   * @param opPos The current position in the xpath.m_opMap array.
   * @param stepType Value of xpath.FROM_NAMESPACE.
   * @param subQueryResults Should be an empty node list where the 
   * results of the step will be put.
   * @returns the length of the argument (i.e. amount to add to predicate pos 
   * or end of step).
   */
  protected int findNodesOnUnknownAxis(XPath xpath, Node context, int opPos, 
							  int stepType, MutableNodeList subQueryResults)
  {
	int argLen = xpath.m_opMap[opPos+xpath.MAPINDEX_LENGTH+1]-3;
	opPos += 3;
	xpath.error(context, "unknown axis: "+stepType);
	return argLen+3;
  }


  /**
   * Qualify a node list by it's predicates.
   * @param xpath The xpath that is executing.
   * @param context The current source tree context node.
   * @param opPos The current position in the xpath.m_opMap array.
   * @param subQueryResults The list of nodes that need to be qualified.
   * @param endPredicatesPos The end position in the xpath.m_opMap array will be filled in.
   * @returns the qualified subset of subQueryResults.
   */
  protected MutableNodeList predicates(XPath xpath, Node context, int opPos, 
									   MutableNodeList subQueryResults,
									   int[] endPredicatesPos) 
  {
	boolean hasNulls = false;
	int nextStepType = xpath.m_opMap[opPos];
	while(xpath.OP_PREDICATE == nextStepType)
	{
	  int nContexts = subQueryResults.getLength();
	  for(int i = 0; i < nContexts; i++)
	  {
		XObject pred = xpath.predicate(subQueryResults.item(i), opPos);
		if(XObject.CLASS_NUMBER == pred.getType())
		{
		  if((i+1) != (int)pred.num())
		  {
			hasNulls = true;
			subQueryResults.setItemNull(i);
		  }
		}
		else if(!pred.bool())
		{
		  hasNulls = true;
		  subQueryResults.setItemNull(i);
		}
	  }
	  opPos = xpath.getNextOpPos(opPos);
	  nextStepType = xpath.m_opMap[opPos];
	  if(xpath.OP_PREDICATE == nextStepType)
	  {
		// This will reconstruct the node list without the nulls.
		subQueryResults = new MutableNodeListImpl(subQueryResults, xpath.m_callbacks);
	 
		xpath.m_contextNodeList = subQueryResults;
		// Don't break, loop 'till end so that opPos will be set to end.
		// if(0 == subQueryResults.getLength())
		//	break;
	  }
	}
	endPredicatesPos[0] = opPos;
	if(hasNulls)
	{
	  subQueryResults = new MutableNodeListImpl(subQueryResults, xpath.m_callbacks);
	}
	return subQueryResults;
  }
	
  /**
   * Execute a a location path pattern.  This will return a score
   * of MATCH_SCORE_NONE, MATCH_SCORE_NODETEST, 
   * MATCH_SCORE_OTHER, MATCH_SCORE_QNAME.
   * @param xpath The xpath that is executing.
   * @param context The current source tree context node.
   * @param opPos The current position in the xpath.m_opMap array.
   * @returns score, one of MATCH_SCORE_NODETEST, 
   * MATCH_SCORE_NONE, MATCH_SCORE_OTHER, MATCH_SCORE_QNAME.
   */
  public double locationPathPattern(XPath xpath, Node context, int opPos) 
  {    
	opPos+=2;
	double[] scoreHolder = {xpath.MATCH_SCORE_NONE};
	stepPattern(xpath, context, opPos, scoreHolder);
	return scoreHolder[0];
  }

  /**
   * Execute a step in a location path.
   * @param xpath The xpath that is executing.
   * @param context The current source tree context node.
   * @param opPos The current position in the xpath.m_opMap array.
   * @returns the last matched context node.
   */
  protected Node stepPattern(XPath xpath, Node context, int opPos, double scoreHolder[]) 
  {    
	int startOpPos = opPos;
	int stepType = xpath.m_opMap[opPos];
	
	int endStep = xpath.getNextOpPos(opPos);
	int nextStepType = xpath.m_opMap[endStep];
	double score;
	
	if(xpath.ENDOP != nextStepType)
	{
	  // Continue step via recursion...
	  context = stepPattern(xpath, context, endStep, scoreHolder);
	  if(null == context)
		scoreHolder[0] = xpath.MATCH_SCORE_NONE;
	  if(scoreHolder[0] == xpath.MATCH_SCORE_NONE)
		return null;
	  scoreHolder[0] = xpath.MATCH_SCORE_OTHER;
	  context = xpath.m_callbacks.getParentOfNode(context);
	}
	
	int argLen;

	switch(stepType)
	{
	case xpath.OP_FUNCTION:
	  {
		argLen = xpath.m_opMap[opPos+xpath.MAPINDEX_LENGTH];
		XObject obj = xpath.execute(context, opPos);
		NodeList nl = obj.nodeset();
		int len = nl.getLength();
		score = xpath.MATCH_SCORE_NONE;
		for(int i = 0; i < len; i++)
		{
		  Node n = nl.item(i);
		  score = (n == context) ? xpath.MATCH_SCORE_OTHER : xpath.MATCH_SCORE_NONE;
		  if(score == xpath.MATCH_SCORE_OTHER)
		  {
			context = n;
			break;
		  }
		}
	  }
	  break;
	case xpath.FROM_ROOT:
	  {
		argLen = xpath.m_opMap[opPos+xpath.MAPINDEX_LENGTH+1]-3;
		opPos += 3;
		Document docContext = (Node.DOCUMENT_NODE == context.getNodeType()) 
							  ? (Document)context : context.getOwnerDocument();
		score = (docContext == context) ? xpath.MATCH_SCORE_OTHER : xpath.MATCH_SCORE_NONE;
		if(score == xpath.MATCH_SCORE_OTHER)
		{
		  context = docContext;
		}
	  }
	  break;
	case xpath.MATCH_ATTRIBUTE:
	  {
		argLen = xpath.m_opMap[opPos+xpath.MAPINDEX_LENGTH+1]-3;
		opPos += 3;
		score = nodeTest(xpath, context, opPos, argLen, xpath.FROM_ATTRIBUTES);
		break;
	  }
	case xpath.MATCH_ANY_ANCESTOR:
	  {
		argLen = xpath.m_opMap[opPos+xpath.MAPINDEX_LENGTH+1]-3;
		opPos += 3;
		score = xpath.MATCH_SCORE_NONE;
		while(null != context)
		{
		  score = nodeTest(xpath, context, opPos, argLen, stepType);
		  if(xpath.MATCH_SCORE_NONE != score)
			break;
		  context = xpath.m_callbacks.getParentOfNode(context);
		}
	  }
	  break;
	case xpath.MATCH_IMMEDIATE_ANCESTOR:
	  argLen = xpath.m_opMap[opPos+xpath.MAPINDEX_LENGTH+1]-3;
	  opPos += 3;
	  score = nodeTest(xpath, context, opPos, argLen, stepType);
	  break;
	default:
	  argLen = xpath.m_opMap[opPos+xpath.MAPINDEX_LENGTH+1]-3;
	  opPos += 3;
	  score = xpath.MATCH_SCORE_NONE;
	  xpath.error(context, "unknown match operation!");
	  break;
	}
	opPos += argLen;
	nextStepType = xpath.m_opMap[opPos];
	
	if(((score != xpath.MATCH_SCORE_NONE)) && (xpath.OP_PREDICATE == nextStepType))
	{
	  score = xpath.MATCH_SCORE_OTHER;
	  // Execute the xpath.predicates, but if we have an index, then we have 
	  // to start over and do a search from the parent.  It would be nice 
	  // if I could sense this condition earlier...
	  try
	  {
		xpath.m_throwFoundIndex = true;
		int startPredicates = opPos;
		opPos = startPredicates;
		nextStepType = xpath.m_opMap[opPos];
		while(xpath.OP_PREDICATE == nextStepType)
		{
		  XObject pred = xpath.predicate(context, opPos);
		  if(XObject.CLASS_NUMBER == pred.getType())
		  {
			throw new FoundIndex();
		  }
		  else if(!pred.bool())
		  {
			score = xpath.MATCH_SCORE_NONE;
			break; // from while(xpath.OP_PREDICATE == nextStepType)
		  }
		  opPos = xpath.getNextOpPos(opPos);
		  nextStepType = xpath.m_opMap[opPos];
		}
		xpath.m_throwFoundIndex = false;
	  }
	  catch(FoundIndex fi)
	  {
		// We have an index somewhere in our pattern.  So, we have 
		// to do a full search for our step, using the parent as 
		// context, then see if the current context is found in the 
		// node set.  Seems crazy, but, so far, it seems like the 
		// easiest way.
		xpath.m_throwFoundIndex = false;
		Node parentContext = xpath.m_callbacks.getParentOfNode(context);
		MutableNodeList mnl = step(xpath, parentContext, startOpPos);
		int nNodes = mnl.getLength();
		score = xpath.MATCH_SCORE_NONE;
		for(int i = 0; i < nNodes; i++)
		{
		  if(mnl.item(i) == context)
		  {
			score = xpath.MATCH_SCORE_OTHER;
			break;
		  }
		}
	  }
	}
	// If we haven't found a score yet, or the test was 
	// negative, assign the score.
	if((scoreHolder[0] == xpath.MATCH_SCORE_NONE) || 
	   (score == xpath.MATCH_SCORE_NONE))
	  scoreHolder[0] = score;
			
	return (score == xpath.MATCH_SCORE_NONE) ? null : context;
  }

  
  /**
   * Test a node to see if it matches the given node test.
   * @param xpath The xpath that is executing.
   * @param context The current source tree context node.
   * @param opPos The current position in the xpath.m_opMap array.
   * @param len The length of the argument.
   * @param len The type of the step.
   * @returns score in an XNumber, one of MATCH_SCORE_NODETEST, 
   * MATCH_SCORE_NONE, MATCH_SCORE_OTHER, MATCH_SCORE_QNAME.
   */
  public double nodeTest(XPath xpath, Node context, int opPos, int argLen, int stepType)
  {
	double score;
	int testType = xpath.m_opMap[opPos];
	int nodeType = context.getNodeType();
	opPos++;
	switch(testType)
	{
	case xpath.NODETYPE_COMMENT:
	  score = (Node.COMMENT_NODE == nodeType)
			  ? xpath.MATCH_SCORE_NODETEST : xpath.MATCH_SCORE_NONE;
	  break;
	case xpath.NODETYPE_TEXT:
	  score = ((Node.CDATA_SECTION_NODE == nodeType) 
			 || (Node.TEXT_NODE == nodeType))
			  ? xpath.MATCH_SCORE_NODETEST : xpath.MATCH_SCORE_NONE;
	  break;
	case xpath.NODETYPE_PI:
	  if( (Node.PROCESSING_INSTRUCTION_NODE == nodeType) && (argLen > 0) )
	  {
		XString name = (XString)xpath.m_tokenQueue[xpath.m_opMap[opPos]];
		score = ((ProcessingInstruction)context).getNodeName().equals(name.str())
				? xpath.MATCH_SCORE_QNAME : xpath.MATCH_SCORE_NONE;
	  }
	  else
	  {
		score = xpath.MATCH_SCORE_NONE;
	  }
	  break;
	case xpath.NODETYPE_NODE:
	  score = xpath.MATCH_SCORE_NODETEST;
	  break;
	case xpath.NODETYPE_ROOT:
	  score = ( (Node.DOCUMENT_FRAGMENT_NODE == nodeType) 
				|| (Node.DOCUMENT_NODE == nodeType))
			  ? xpath.MATCH_SCORE_OTHER : xpath.MATCH_SCORE_NONE;
	  break;
	  
	case xpath.NODENAME:
	  {
		boolean test;
		int queueIndex = xpath.m_opMap[opPos];
		String targetNS = (queueIndex >= 0) ? (String)xpath.m_tokenQueue[xpath.m_opMap[opPos]]
											  : null;
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
		boolean isTotallyWild = (null == targetNS) && (xpath.m_opMap[opPos] == xpath.ELEMWILDCARD);
		boolean processNamespaces = xpath.m_callbacks.getProcessNamespaces();
		if(!isTotallyWild && processNamespaces)
		{
		  String contextNS = xpath.m_callbacks.getNamespaceOfNode(context);
		  if((null != targetNS) && (null != contextNS))
		  {
			test = contextNS.equals(targetNS);
		  }
		  else
		  {
			test = (xpath.ELEMWILDCARD == queueIndex) || 
				   ((null == contextNS) || (contextNS.length() == 0));
		  }
		}
		else 
		  test = true;
		
		queueIndex = xpath.m_opMap[opPos];
		String targetLocalName = (queueIndex >= 0) ? (String)xpath.m_tokenQueue[xpath.m_opMap[opPos]]
													 : null;
		
		if(!test)
		{
		  score = xpath.MATCH_SCORE_NONE;
		}
		else
		{
		  switch(nodeType)
		  {
		  case Node.ATTRIBUTE_NODE:
			if(stepType == xpath.FROM_ATTRIBUTES)
			{			 
			  
			  if(xpath.ELEMWILDCARD == queueIndex)
			  {
				String attrName = ((Attr)context).getNodeName();
				if(processNamespaces)
				{
				  score = (!((attrName.startsWith("xmlns:") 
							  || attrName.equals("xmlns"))))
						  ? xpath.MATCH_SCORE_NODETEST : xpath.MATCH_SCORE_NONE;
				}
				else
				{
				  score = xpath.MATCH_SCORE_NODETEST;
				}
			  }
			  else
			  {
				String localAttrName 
				  = xpath.m_callbacks.getLocalNameOfNode(context);
				score = localAttrName.equals(targetLocalName)
						? xpath.MATCH_SCORE_QNAME : xpath.MATCH_SCORE_NONE;
			  }
			}
			else
			{
			  score  = xpath.MATCH_SCORE_NONE;
			}
			break;

		  case Node.ELEMENT_NODE:
			if(stepType != xpath.FROM_ATTRIBUTES)
			{
			  if(xpath.ELEMWILDCARD == queueIndex)
			  {
				score = xpath.MATCH_SCORE_NODETEST;
			  }
			  else
			  {
				
				score = (xpath.m_callbacks.getLocalNameOfNode((Element)context).equals(targetLocalName))
						? xpath.MATCH_SCORE_QNAME : xpath.MATCH_SCORE_NONE;
			  }
			}
			else
			{
			  score  = xpath.MATCH_SCORE_NONE;
			}
			break;
			
		  default:
			// Trying to match on anything else causes nasty bugs.
			  score  = xpath.MATCH_SCORE_NONE;
			break;

		  } // end switch(nodeType)
		} // end if(test)
	  } // end case xpath.NODENAME
	  break;
	default:
	  score  = xpath.MATCH_SCORE_NONE;
	} // end switch(testType)
	
	return score;	 
  }
  
  public static XPathFactory factory() 
  {
	return new SimpleNodeLocatorFactory();
  }

  /**
   * Very crude file filter.
   */
  class FileFilter implements FilenameFilter
  {
	private String m_filterSpec;
	
	public FileFilter(String filter)
	{
	  m_filterSpec = filter;
	}
	
	/**
	 * Tests if a specified file should be included in a file list.
	 *
	 * @param	dir    the directory in which the file was found.
	 * @param	name   the name of the file.
	 * @return	<code>true</code> if the name should be included in the file
	 *			list; <code>false</code> otherwise.
	 * @since	JDK1.0
	 */
	public boolean accept(File dir, String name)
	{
	  return name.endsWith(m_filterSpec);
	}
  }
  
}

/**
 * Override the createXLocatorHandler method.
 */
class DOMXPath extends XPath
{
  public DOMXPath(XPathSupport callbacks)
  {
	super(callbacks);
  }
  
  /**
   * getXLocatorHandler.
   */
  XLocator createXLocatorHandler(XPath xpath)
  {
	return new SimpleNodeLocator();
  }
}

/**
 * Implement an XPath factory.
 */
class SimpleNodeLocatorFactory implements XPathFactory
{
  public XPath create(XPathSupport callbacks)
  {
	return new DOMXPath(callbacks);
  }
}
#endif


#endif	// SIMPLENODELOCATOR_HEADER_GUARD_1357924680
