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



#include <XalanDOM/XalanDOMString.hpp>
#include <XalanDOM/XalanNode.hpp>



// Base class header file
#include <XPath/XLocator.hpp>



#include <XPath/MutableNodeRefList.hpp>



class XPathExpression;



/**
 * SimpleNodeLocator implements a search of one or more DOM trees. By using
 * the connect function as an extension, the user may specify a directory and
 * a filter specification for XML files that will be searched. This is a
 * singleton class.
 */
class XALAN_XPATH_EXPORT SimpleNodeLocator : public XLocator
{
public:

	static SimpleNodeLocator*
	getDefaultInstance();

	/*
	 * Create a SimpleNodeLocator object.
	 */
	explicit
	SimpleNodeLocator();

	virtual
	~SimpleNodeLocator();

	// These methods are inherited from XLocator ...

	virtual const XObjectPtr
	locationPath(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode& 				context, 
			int 					opPos);

	virtual double
	locationPathPattern(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode& 				context, 
			int 					opPos);

protected:

	void
	step(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			MutableNodeRefList&		queryResults);

	/**
	 * Execute a step in a location path.
	 *
	 * @param xpath The xpath that is executing
	 * @param context The current source tree context node
	 * @param opPos The current position in the xpath operation map array
	 * @param scoreHolder a reference to a double to receive the result.
	 * @return the last matched context node
	 */
	XalanNode*
	stepPattern(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			double& 				scoreHolder);

	int
	findNodeSet(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	int
	findRoot(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	int
	findParent(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	int
	findSelf(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	int
	findAncestors(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	int
	findAncestorsOrSelf(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	int
	findAttributes(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	int
	findChildren(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	int
	findDescendants(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	int
	findFollowing(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	int
	findFollowingSiblings(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	int
	findPreceeding(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	int
	findPreceedingSiblings(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	int
	findNamespace(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	int
	findNodesOnUnknownAxis(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults);

	double
	nodeTest(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				context,
			XalanNode::NodeType		nodeType,
			int 					opPos,
			int 					argLen,
			int 					stepType);

	void
	predicates(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			MutableNodeRefList& 	subQueryResults,
			int&					endPredicatesPos);

	double
	handleFoundIndex(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			XalanNode* 				localContext,
			int 					startOpPos);

private:

	static SimpleNodeLocator		s_defaultInstance;

	static const XalanDOMString		s_emptyString;
};



#endif	// SIMPLENODELOCATOR_HEADER_GUARD_1357924680
