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



// Base class header files...
#include <XPath/XPathExecutionContext.hpp>



#include <XPath/XPathExpression.hpp>
#include <XPath/Function.hpp>
#include <XPath/XPathFunctionTable.hpp>



class PrefixResolver;
class XLocator;
class XObject;
class XalanNode;



/**
 * The XPath class represents the semantic parse tree of the XPath pattern. It
 * is the representation of the grammar which filters out the choice for
 * replacement order of the production rules. In order to conserve memory and
 * reduce object creation, the tree is represented as an array of integers:
 *	  [op code][length][...]
 * where strings are represented within the array as indices into the token
 * tree.
 */
class XALAN_XPATH_EXPORT XPath
{
public:

	static const XalanDOMString&	PSEUDONAME_ANY;
	static const XalanDOMString&	PSEUDONAME_ROOT;
	static const XalanDOMString&	PSEUDONAME_TEXT;
	static const XalanDOMString&	PSEUDONAME_COMMENT;
	static const XalanDOMString&	PSEUDONAME_PI;
	static const XalanDOMString&	PSEUDONAME_OTHER;

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
	 * Construct an XPath and optionally a default locator 
	 * 
	 * @param createDefaultLocator true to create a default locator object,
	 *                             default true
	 */
	explicit
	XPath(bool	createDefaultLocator = true);

	virtual
	~XPath();

	/**
	 * Shrink internal tables.
	 */
	virtual void
	shrink();

	/**
	 * Execute the XPath from the provided context.
	 *
	 * @param context          current source tree context node
	 * @param prefixResolver   prefix resolver to use
	 * @param executionContext current execution context
	 * @return pointer to union of node-set operands
	 */
	virtual const XObjectPtr
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
	virtual const XObjectPtr
	execute(
			XalanNode*				context,
			const PrefixResolver&	prefixResolver,
			const NodeRefListBase&	contextNodeList,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Execute the XPath from the provided context.  The
	 * prefix resolver must already be set in the
	 * execution context.
	 *
	 * @param executionContext current execution context
	 * @return pointer to result XObject
	 */
	virtual const XObjectPtr
	execute(XPathExecutionContext&	executionContext) const;

	/**
	 * Execute the XPath from the provided context.
	 *
	 * @param context          current source tree context node
	 * @param opPos            current position in the m_opMap array
	 * @param executionContext current execution context
	 * @return pointer to union of node-set operands
	 */
	virtual const XObjectPtr
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
	virtual const XObjectPtr
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

#if defined(XALAN_INLINE_INITIALIZATION)
	/**
	 * The match score if no match is made.
	 */
	const double					s_MatchScoreNone = -9999999999999.0;

	/**
	 * The match score if the pattern has the form 
	 * of a QName optionally preceded by an @ character.
	 */
	const double					s_MatchScoreQName = 0.0;

	/**
	 * The match score if the pattern has the form NCName:*.
	 */
	const double					s_MatchScoreNSWild = -0.25;

	/**
	 * The match score if the pattern consists of just a NodeTest.
	 */
	const double					s_MatchScoreNodeTest = -0.5;

	/**
	 * The match score if the pattern consists of something 
	 * other than just a NodeTest or just a qname.
	 */
	const double					s_MatchScoreOther = 0.5;
#else
	/**
	 * The match score if no match is made.
	 */
	static const double					s_MatchScoreNone;

	/**
	 * The match score if the pattern has the form 
	 * of a QName optionally preceded by an @ character.
	 */
	static const double					s_MatchScoreQName;

	/**
	 * The match score if the pattern has the form NCName:*.
	 */
	static const double					s_MatchScoreNSWild;

	/**
	 * The match score if the pattern consists of just a NodeTest.
	 */
	static const double					s_MatchScoreNodeTest;

	/**
	 * The match score if the pattern consists of something 
	 * other than just a NodeTest or just a qname.
	 */
	static const double					s_MatchScoreOther;
#endif

	/**
	 * Computes the union of its operands which must be node-sets.
	 *
	 * @param context current source tree context node
	 * @param executionContext current execution context
	 * @return union of node-set operands
	 */
	virtual double
	getMatchScore(
			XalanNode*				context,
			const PrefixResolver&	resolver,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Test a node.  This should be implemented by a derived class.
	 * Returns s_MatchScoreNone by default.
	 *
	 * @param context  current source tree context node
	 * @param opPos    current position in the m_opMap array
	 * @param argLen   argument length
	 * @param stepType type of step
	 * @param executionContext current execution context
	 * @return      one of s_MatchScoreNone, s_MatchScoreNodeTest,
	 *              s_MatchScoreQName
	 */
	virtual double
	nodeTest(
			XalanNode*				context,
			int						opPos,
			int						argLen,
			int						stepType,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Evaluate a predicate.
	 *
	 * @param context          current source tree context node
	 * @param opPos            current position in the m_opMap array
	 * @param executionContext current execution context
	 * @return pointer to either a boolean or a number
	 */
	virtual const XObjectPtr
	predicate(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Add the strings for the target element to a vector of strings.
	 * 
	 * @param targetStrings vector of strings
	 */
	virtual void
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

protected:

	/**
	 * createXLocatorHandler.
	 */
	virtual XLocator*
	createXLocatorHandler() const;

	/**
	 * Execute from the beginning of the xpath.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @return The result of the expression.
	 */
	virtual const XObjectPtr
	xpath(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Computes the union of its operands which must be node-sets.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @return the match score in the form of an XObject.
	 */
	virtual const XObjectPtr
	matchPattern(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Execute a step in a location path.  This must be implemented 
	 * by a derived class of XPath (or don't call at all 
	 * from the derived implementation of locationPath()).
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @return a node-set.
	 */
	MutableNodeRefList*
	step(
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
			double&					score) const;

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
	 * Return the remainder from a truncating division.
	 * (Quo is no longer supported by xpath).
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @return arg1 mod arg2.
	 */
	const XObjectPtr
	quo(
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
	 * Cast an expression to a number.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @return arg cast to a number.
	 */
	const XObjectPtr
	number(
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
			XPathExecutionContext&	executionContext) const;

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
			XPathExecutionContext&	executionContext) const;

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
			int										opPos,
			const XalanDOMString&					theNamespace,
			const XalanDOMString&					functionName, 
			const Function::XObjectArgVectorType&	argVec,
			XPathExecutionContext&					executionContext) const;

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
			int										opPos,
			int										funcID,
			const Function::XObjectArgVectorType&	argVec,
			XPathExecutionContext&					executionContext) const;

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

	// Data members...

	/**
	 *
	 * The default XLocator to use if a custom one is not
	 * available.
	 *
	 */
	XLocator*							m_defaultXLocator;

	/**
	 *
	 * Holds information about the current expression.
	 *
	 */
	XPathExpression						m_expression;

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
};



#endif	// XPATH_HEADER_GUARD_1357924680
