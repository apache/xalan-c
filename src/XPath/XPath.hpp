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



#include <map>
#include <memory>
#include <vector>



#include <XalanDOM/XalanDOMString.hpp>



#include <PlatformSupport/FactoryObject.hpp>
#include <PlatformSupport/STLHelper.hpp>



// Base class header files...
#include <XPath/XPathExecutionContext.hpp>



#include <XPath/MutableNodeRefList.hpp>
#include <XPath/XPathExpression.hpp>
#include <XPath/Function.hpp>
#include <XPath/XPathFunctionTable.hpp>



class PrefixResolver;
class XLocator;
class XObject;
class XObjectFactory;
class XPathEnvSupport;
class XPathSupport;
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
class XALAN_XPATH_EXPORT XPath : public FactoryObject
{
public:

#if defined(XALAN_INLINE_INITIALIZATION)
	const XalanDOMString			PSEUDONAME_ANY(XALAN_STATIC_UCODE_STRING("*"));
	const XalanDOMString			PSEUDONAME_ROOT(XALAN_STATIC_UCODE_STRING("/"));
	const XalanDOMString			PSEUDONAME_TEXT(XALAN_STATIC_UCODE_STRING("#text"));
	const XalanDOMString			PSEUDONAME_COMMENT(XALAN_STATIC_UCODE_STRING("#comment"));
	const XalanDOMString			PSEUDONAME_PI(XALAN_STATIC_UCODE_STRING("#pi"));
	const XalanDOMString			PSEUDONAME_OTHER(XALAN_STATIC_UCODE_STRING("*"));
#else
	static const XalanDOMString		PSEUDONAME_ANY;
	static const XalanDOMString		PSEUDONAME_ROOT;
	static const XalanDOMString		PSEUDONAME_TEXT;
	static const XalanDOMString		PSEUDONAME_COMMENT;
	static const XalanDOMString		PSEUDONAME_PI;
	static const XalanDOMString		PSEUDONAME_OTHER;
#endif

#if defined(XALAN_NO_NAMESPACES)
	typedef vector<XalanDOMString>			TargetElementStringsVectorType;
#else
	typedef std::vector<XalanDOMString>		TargetElementStringsVectorType;
#endif


	/**
	 * Construct an XPath and optionally a default locator 
	 * 
	 * @param createDefaultLocator true to create a default locator object,
	 *                             default true
	 */
	explicit
	XPath(bool	createDefaultLocator = true);

	/**
	 * Shrink internal tables.
	 */
	virtual void
	shrink();

public:

	/**
	 * Execute the XPath from the provided context.
	 *
	 * @param context          current source tree context node
	 * @param prefixResolver   prefix resolver to use
	 * @param executionContext current execution context
	 * @return pointer to union of node-set operands
	 */
	virtual XObject*
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
	virtual XObject*
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
	virtual XObject*
	execute(XPathExecutionContext&	executionContext) const;

	/**
	 * Execute the XPath from the provided context.
	 *
	 * @param context          current source tree context node
	 * @param opPos            current position in the m_opMap array
	 * @param executionContext current execution context
	 * @return pointer to union of node-set operands
	 */
	virtual XObject*
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
	virtual XObject*
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
	virtual XObject*
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
			const Function& 	func);

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

protected:

	virtual
	~XPath();

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
	virtual XObject*
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
	virtual XObject*
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
	 * OR two expressions and return the boolean result.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @return XBoolean set to true if the one of the two arguments are true.
	 */
	virtual XObject*
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
	virtual XObject*
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
	virtual XObject*
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
	virtual XObject*
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
	virtual XObject*
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
	virtual XObject*
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
	virtual XObject*
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
	virtual XObject*
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
	virtual XObject*
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
	virtual XObject*
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
	virtual XObject*
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
	virtual XObject*
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
	virtual XObject*
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
	virtual XObject*
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
	virtual XObject*
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
	virtual XObject*
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
	virtual XObject*
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
	virtual XObject*
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
	virtual XObject*
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
	virtual XObject*
	literal(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;
  
	/**
	 * Get a literal value.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @return an XObject object.
	 */
	virtual XObject*
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
	virtual XObject*
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
	virtual XObject*
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
	virtual XObject*
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
	virtual XObject*
	locationPathPattern(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Setup for and run an extension function.
	 */
	virtual XObject*
	runExtFunction(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Handle an extension function.
	 */
	virtual XObject*
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
	virtual XObject*
	runFunction(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Handle a built-in function.
	 */
	virtual XObject*
	function(
			XalanNode*								context,
			int										opPos,
			int										funcID,
			const Function::XObjectArgVectorType&	argVec,
			XPathExecutionContext&					executionContext) const;

private:

	// These are not implemented...
	XPath(const XPath&);

	XPath&
	operator=(const XPath&);

	bool
	operator==(const XPath&) const;

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
	 *
	 * This is the table of installed functions.
	 *
	 */
	static FunctionTableType			s_functions;
};



#endif	// XPATH_HEADER_GUARD_1357924680
