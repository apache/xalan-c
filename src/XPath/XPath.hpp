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
#if !defined(XPATH_HEADER_GUARD_1357924680)
#define XPATH_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



#include <XalanDOM/XalanDOMString.hpp>



#include <PlatformSupport/DoubleSupport.hpp>



// Base class header files...
#include <XPath/XPathExecutionContext.hpp>



#include <XPath/XPathExpression.hpp>
#include <XPath/Function.hpp>
#include <XPath/XPathFunctionTable.hpp>



class Locator;
class PrefixResolver;
class XObject;
class XalanNode;



class XALAN_XPATH_EXPORT XPath
{
public:

	static const XalanDOMString&	PSEUDONAME_ANY;
	static const XalanDOMString&	PSEUDONAME_ROOT;
	static const XalanDOMString&	PSEUDONAME_TEXT;
	static const XalanDOMString&	PSEUDONAME_COMMENT;
	static const XalanDOMString&	PSEUDONAME_PI;
	static const XalanDOMString&	PSEUDONAME_OTHER;
	static const XalanDOMString&	PSEUDONAME_NODE;

#if defined(XALAN_NO_NAMESPACES)
	typedef vector<XalanDOMString>			TargetElementStringsVectorType;
#else
	typedef std::vector<XalanDOMString>		TargetElementStringsVectorType;
#endif


	/**
	 * Perform static initialization.  See class XPathInit.
	 */
	static void
	initialize();

	/**
	 * Perform static shut down.  See class XPathInit.
	 */
	static void
	terminate();

	/**
	 * Construct an XPath.
	 *
	 * @param theLocator The applicable Locator, if any.
	 */
	explicit
	XPath(const Locator*	theLocator = 0);

	virtual
	~XPath();

	/**
	 * Shrink internal tables.
	 */
	void
	shrink();

	/**
	 * Execute the XPath from the provided context.
	 *
	 * @param context          current source tree context node
	 * @param prefixResolver   prefix resolver to use
	 * @param executionContext current execution context
	 * @return pointer to union of node-set operands
	 */
	const XObjectPtr
	execute(
			XalanNode*				context,
			const PrefixResolver&	prefixResolver,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Execute the XPath from the provided context.
	 *
	 * @param context          current source tree context node
	 * @param prefixResolver   prefix resolver to use
	 * @param contextNodeList  node list for current context
	 * @param executionContext current execution context
	 * @return pointer to union of node-set operands
	 */
	const XObjectPtr
	execute(
			XalanNode*				context,
			const PrefixResolver&	prefixResolver,
			const NodeRefListBase&	contextNodeList,
			XPathExecutionContext&	executionContext) const
	{
		// Set and restore the context node list...
		XPathExecutionContext::ContextNodeListSetAndRestore		theSetAndRestore(
										executionContext,
										contextNodeList);	

		return execute(context, prefixResolver, executionContext);
	}

	/**
	 * Execute the XPath from the provided context.  The
	 * prefix resolver must already be set in the
	 * execution context.
	 *
	 * @param executionContext current execution context
	 * @return pointer to result XObject
	 */
	const XObjectPtr
	execute(XPathExecutionContext&	executionContext) const
	{
		assert(executionContext.getPrefixResolver() != 0);

		return executeMore(executionContext.getCurrentNode(), 0, executionContext);
	}

	/**
	 * Execute the XPath from the provided context.
	 *
	 * @param context          current source tree context node
	 * @param opPos            current position in the m_opMap array
	 * @param executionContext current execution context
	 * @return pointer to union of node-set operands
	 */
	const XObjectPtr
	executeMore(
			XalanNode* 				context,
			int 					opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Execute a location path.
	 *
	 * @param context          current source tree context node
	 * @param opPos            current position in the m_opMap array
	 * @param executionContext current execution context
	 * @return node-set
	 */
	const XObjectPtr
	locationPath(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Retrieve a reference to the current expression.
	 * 
	 * @return current expression
	 */
	XPathExpression&
	getExpression()
	{
		return m_expression;
	}

	/**
	 * Retrieve a reference to the current expression.
	 * 
	 * @return current expression
	 */
	const XPathExpression&
	getExpression() const
	{
		return m_expression;
	}

	enum eMatchScore
	{
		eMatchScoreNone,
		eMatchScoreNodeTest,
		eMatchScoreNSWild,
		eMatchScoreQName,
		eMatchScoreOther
	};

	static double
	getMatchScoreValue(eMatchScore	score)
	{
		switch(score)
		{
		case eMatchScoreNone:
			return DoubleSupport::getNegativeInfinity();
			break;

		case eMatchScoreNodeTest:
			return -0.5;
			break;

		case eMatchScoreNSWild:
			return -0.25;
			break;

		case eMatchScoreOther:
			return 0.5;
			break;

		case eMatchScoreQName:
			return 0.0;
			break;
		};

		assert(false);
		return 0.0;
	}

	/**
	 * Get the match score for the specified node.
	 *
	 * @param node The node for the score
	 * @param resolver The prefix resolver
	 * @param executionContext current execution context
	 * @return union of node-set operands
	 */
	eMatchScore
	getMatchScore(
			XalanNode*				node,
			const PrefixResolver&	resolver,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Evaluate a predicate.
	 *
	 * @param context          current source tree context node
	 * @param opPos            current position in the m_opMap array
	 * @param executionContext current execution context
	 * @return pointer to either a boolean or a number
	 */
	const XObjectPtr
	predicate(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
	{
		return executeMore(context, opPos + 2, executionContext);
	}

	/**
	 * Add the strings for the target element to a vector of strings.
	 * 
	 * @param targetStrings vector of strings
	 */
	void
	getTargetElementStrings(TargetElementStringsVectorType&		targetStrings) const;

	/**
	 * Install a built-in function.
	 *
	 * @param funcName  unqualified name of the function
	 * @param func      instance of an XPath function object
	 */
	static void
	installFunction(
			const XalanDOMString&	funcName,
			const Function& 		func);

	/**
	 * Remove a named function from the function table.
	 * 
	 * @param funcName name of function
	 * @return true if the function was found and removed.
	 */
	static bool
	uninstallFunction(const XalanDOMString&		funcName);

	/**
	 * Whether the named function is installed in the function table.
	 * 
	 * @param name of function
	 * @return true if the function has been installed
	 */
	static bool
	isInstalledFunction(const XalanDOMString&	theFunctionName)
	{
		return s_functions.isInstalledFunction(theFunctionName);
	}

	typedef XPathFunctionTable	FunctionTableType;

	/**
	 * Retrieve the table of installed functions.
	 * 
	 * @return function table
	 */
	static const FunctionTableType&
	getFunctionTable()
	{
		return s_functions;
	}

#if defined(XALAN_NO_MEMBER_TEMPLATES)
	typedef XPathFunctionTable::InstalledFunctionNameVectorType
					InstalledFunctionNameVectorType;

	/**
	 * Add the names for the installed functions to a vector strings.
	 * 
	 * @param theVector added to
	 */
	static void
	getInstalledFunctionNames(InstalledFunctionNameVectorType&	theVector)
	{
		s_functions.getInstalledFunctionNames(theVector);
	}
#else
	/**
	 * Add the names for the installed functions to a vector strings.
	 * 
	 * @param theIterator vector added to
	 */
	template<class OutputIteratorType>
	static void
	getInstalledFunctionNames(OutputIteratorType	theIterator)
	{
		s_functions.getInstalledFunctionNames(theIterator);
	}
#endif

	static void
	destroyTable()
	{
		s_functions.DestroyTable();
	}

	bool
	getInStylesheet() const
	{
		return m_inStylesheet;
	}

	void
	setInStylesheet(bool	fValue)
	{
		m_inStylesheet = fValue;
	}

	const Locator*
	getLocator() const
	{
		return m_locator;
	}

	void
	setLocator(const Locator*	theLocator)
	{
		m_locator = theLocator;
	}

protected:

	/**
	 * createXLocatorHandler.
	 */
	XLocator*
	createXLocatorHandler() const;

	/**
	 * Computes the union of its operands which must be node-sets.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @return the match score in the form of an XObject.
	 */
	const XObjectPtr
	matchPattern(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

protected:

	/**
	 * Helper function to get match score.
	 * @param context The current source tree context node.
	 * @param executionContext The current execution context
	 * @param score The match score
	 */
	void
	doGetMatchScore(
			XalanNode*				context,
			XPathExecutionContext&	executionContext,
			eMatchScore&			score) const;

	/**
	 * OR two expressions and return the boolean result.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @return XBoolean set to true if the one of the two arguments are true.
	 */
	const XObjectPtr
	Or(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * OR two expressions and return the boolean result.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @return XBoolean set to true if the two arguments are both true.
	 */
	const XObjectPtr
	And(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Tell if two expressions are functionally not equal.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @return XBoolean set to true if the two arguments are not equal.
	 */
	const XObjectPtr
	notequals(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Tell if two expressions are functionally equal.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @return XBoolean set to true if the two arguments are equal.
	 */
	const XObjectPtr
	equals(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Tell if one argument is less than or equal to the other argument.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @return XBoolean set to true if arg 1 is less than or equal to arg 2.
	 */
	const XObjectPtr
	lte(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Tell if one argument is less than the other argument.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @return XBoolean set to true if arg 1 is less than arg 2.
	 */
	const XObjectPtr
	lt(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Tell if one argument is greater than or equal to the other argument.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @return XBoolean set to true if arg 1 is greater than or equal to arg 2.
	 */
	const XObjectPtr
	gte(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Tell if one argument is greater than the other argument.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @return XBoolean set to true if arg 1 is greater than arg 2.
	 */
	const XObjectPtr
	gt(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Give the sum of two arguments.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @return sum of arg1 and arg2.
	 */
	const XObjectPtr
	plus(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Give the difference of two arguments.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @return difference of arg1 and arg2.
	 */
	const XObjectPtr
	minus(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Multiply two arguments.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @return arg1 * arg2.
	 */
	const XObjectPtr
	mult(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Divide a number.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @return arg1 / arg2.
	 */
	const XObjectPtr
	div(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Return the remainder from a truncating division.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @return arg1 mod arg2.
	 */
	const XObjectPtr
	mod(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Return the negation of a number.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @return -arg.
	 */
	const XObjectPtr
	neg(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Cast an expression to a string.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @return arg cast to a string.
	 */
	const XObjectPtr
	string(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Cast an expression to a boolean.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @return arg cast to a boolean.
	 */
	const XObjectPtr
	boolean(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;
 
	/**
	 * Computes the union of its operands which must be node-sets.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @return the union of node-set operands.
	 */
	const XObjectPtr
	Union(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Get a literal value.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @return an XObject object.
	 */
	const XObjectPtr
	literal(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;
  
	/**
	 * Get the value of a variable.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @return an XObject object.
	 */
	const XObjectPtr
	variable(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Execute an expression as a group.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @return arg.
	 */
	const XObjectPtr
	group(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
	{
		return executeMore(context, opPos + 2, executionContext);
	}

	/**
	 * Get a literal value.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @return an XObject object.
	 */
	const XObjectPtr
	numberlit(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Execute a function argument.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @return the result of the argument expression.
	 */
	const XObjectPtr
	arg(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
	{
		return executeMore(context, opPos + 2, executionContext);
	}

	/**
	 * Execute a location path.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @return score in an XNumber, one of MATCH_SCORE_NODETEST, 
	 * MATCH_SCORE_NONE, MATCH_SCORE_OTHER, MATCH_SCORE_QNAME.
	 */
	const XObjectPtr
	locationPathPattern(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Setup for and run an extension function.
	 */
	const XObjectPtr
	runExtFunction(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Handle an extension function.
	 */
	const XObjectPtr
	extfunction(
			XalanNode*								context,
			int										/* opPos */,
			const XalanDOMString&					theNamespace,
			const XalanDOMString&					functionName, 
			const Function::XObjectArgVectorType&	argVec,
			XPathExecutionContext&					executionContext) const
	{
		return 	executionContext.extFunction(theNamespace,
											 functionName,
											 context,
											 argVec,
											 m_locator);
	}

	/**
	 * Setup for and run a function.
	 */
	const XObjectPtr
	runFunction(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Handle a built-in function.
	 */
	const XObjectPtr
	function(
			XalanNode*								context,
			int										funcID,
			const Function::XObjectArgVectorType&	argVec,
			XPathExecutionContext&					executionContext) const
	{
		return s_functions[funcID].execute(executionContext, context, argVec, m_locator);
	}

	double
	getNumericOperand(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

private:

	// These are not implemented...
	XPath(const XPath&);

	XPath&
	operator=(const XPath&);

	bool
	operator==(const XPath&) const;

	// Default vector allocation sizes.
	enum
	{
		eDefaultTargetStringsSize = 5
	};

	const XObjectPtr
	locationPath(
			XPathExecutionContext&	executionContext,
			XalanNode& 				context, 
			int 					opPos) const;

	eMatchScore
	locationPathPattern(
			XPathExecutionContext&	executionContext,
			XalanNode& 				context, 
			int 					opPos) const;

	class NodeTester
	{
	public:

		NodeTester(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			int 					opPos,
			int 					argLen,
			int 					stepType);

		eMatchScore
		operator()(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const
		{
			assert(context.getNodeType() == nodeType);

			return (this->*m_testFunction)(context, nodeType);
		}

	private:

		typedef eMatchScore (NodeTester::*MemberFunctionPtr)(const XalanNode&, XalanNode::NodeType) const;


		eMatchScore
		testComment(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const;

		eMatchScore
		testText(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const;

		eMatchScore
		testPI(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const;

		eMatchScore
		testPIName(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const;

		eMatchScore
		testNode(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const;

		eMatchScore
		testRoot(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const;

		eMatchScore
		testAttributeNCName(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const;

		eMatchScore
		testAttributeQName(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const;

		eMatchScore
		testAttributeNamespaceOnly(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const;

		eMatchScore
		testAttributeTotallyWild(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const;

		eMatchScore
		testElementNCName(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const;

		eMatchScore
		testElementQName(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const;

		eMatchScore
		testElementNamespaceOnly(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const;

		eMatchScore
		testElementTotallyWild(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const;

		eMatchScore
		testNamespaceNCName(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const;

		eMatchScore
		testNamespaceTotallyWild(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const;

		eMatchScore
		testDefault(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const;

		bool
		matchLocalName(const XalanNode&		context) const;

		bool
		matchNamespaceURI(const XalanNode&	context) const;

		bool
		matchLocalNameAndNamespaceURI(const XalanNode&	context) const;

		bool
		matchNamespace(const XalanNode&		context) const;

		bool
		shouldStripSourceNode(const XalanNode&	context) const;

		// Data members...
		XPathExecutionContext&	m_executionContext;

		const XalanDOMString*	m_targetNamespace;

		const XalanDOMString*	m_targetLocalName;

		MemberFunctionPtr		m_testFunction;
	};

protected:

	void
	step(
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			MutableNodeRefList&		queryResults) const;

	/**
	 * Execute a step in a location path.
	 *
	 * @param xpath The xpath that is executing
	 * @param context The current source tree context node
	 * @param opPos The current position in the xpath operation map array
	 * @param scoreHolder a reference to an eMatchScore to receive
	 * the result.
	 * @return the last matched context node
	 */
	XalanNode*
	stepPattern(
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			eMatchScore& 			scoreHolder) const;

	int
	findNodeSet(
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults) const;

	int
	findRoot(
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults) const;

	int
	findParent(
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults) const;

	int
	findSelf(
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults) const;

	int
	findAncestors(
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults) const;

	int
	findAncestorsOrSelf(
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults) const;

	int
	findAttributes(
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults) const;

	int
	findChildren(
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults) const;

	int
	findDescendants(
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults) const;

	int
	findFollowing(
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults) const;

	int
	findFollowingSiblings(
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults) const;

	int
	findPreceeding(
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults) const;

	int
	findPreceedingSiblings(
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults) const;

	int
	findNamespace(
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults) const;

	int
	findNodesOnUnknownAxis(
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			int 					stepType,
			MutableNodeRefList& 	subQueryResults) const;

	eMatchScore
	nodeTest(
			XPathExecutionContext&	executionContext,
			XalanNode* 				context,
			XalanNode::NodeType		nodeType,
			int 					opPos,
			int 					argLen,
			int 					stepType) const;

	void
	predicates(
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			int 					opPos,
			MutableNodeRefList& 	subQueryResults,
			int&					endPredicatesPos) const;

	eMatchScore
	handleFoundIndex(
			XPathExecutionContext&	executionContext,
			XalanNode* 				localContext,
			int 					startOpPos) const;

	// Data members...

	/**
	 *
	 * Holds information about the current expression.
	 *
	 */
	XPathExpression						m_expression;

	/**
	 * A Locator for reporting errors.
	 */
	const Locator*						m_locator;

	/**
	 * If true, the XPath can allocated XObjects in more
	 * efficient ways, since its lifetime is guaranteed
	 * to be at least that of the transform.
	 */
	bool								m_inStylesheet;

	/**
	 *
	 * This is the table of installed functions.
	 *
	 */
	static FunctionTableType			s_functions;

	static const XalanDOMString			s_emptyString;
};



#endif	// XPATH_HEADER_GUARD_1357924680
