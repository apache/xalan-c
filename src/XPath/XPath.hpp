/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights 
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



#include <XPath/MutableNodeRefList.hpp>
#include <XPath/XPathExpression.hpp>
#include <XPath/Function.hpp>
#include <XPath/XPathFunctionTable.hpp>



XALAN_DECLARE_XERCES_CLASS(Locator)



XALAN_CPP_NAMESPACE_BEGIN



class PrefixResolver;
class XObject;
class XalanNode;



class XALAN_XPATH_EXPORT XPath
{
public:

	typedef XERCES_CPP_NAMESPACE_QUALIFIER Locator	LocatorType;

	static const XalanDOMChar	PSEUDONAME_ANY[];
	static const XalanDOMChar	PSEUDONAME_ROOT[];
	static const XalanDOMChar	PSEUDONAME_TEXT[];
	static const XalanDOMChar	PSEUDONAME_COMMENT[];
	static const XalanDOMChar	PSEUDONAME_PI[];
	static const XalanDOMChar	PSEUDONAME_OTHER[];
	static const XalanDOMChar	PSEUDONAME_NODE[];

	enum eMatchScore
	{
		eMatchScoreNone,
		eMatchScoreNodeTest,
		eMatchScoreNSWild,
		eMatchScoreQName,
		eMatchScoreOther
	};

	class TargetData
	{
	public:

		enum eTargetType { eAttribute, eElement, eAny, eOther };

		TargetData() :
			m_string(0),
			m_priority(eMatchScoreNone),
			m_targetType(eOther)
		{
		}

		TargetData(
				const XalanDOMChar*		theString,
				eMatchScore				thePriority,
				eTargetType				theTargetType) :
			m_string(theString),
			m_priority(thePriority),
			m_targetType(theTargetType)
		{
		}

		const XalanDOMChar*
		getString() const
		{
			return m_string;
		}

		eMatchScore
		getDefaultPriority() const
		{
			return m_priority;
		}

		eTargetType
		getTargetType() const
		{
			return m_targetType;
		}

	private:

		const XalanDOMChar*		m_string;

		eMatchScore				m_priority;

		eTargetType				m_targetType;
	};

#if defined(XALAN_NO_STD_NAMESPACE)
	typedef vector<TargetData>		TargetDataVectorType;
#else
	typedef std::vector<TargetData>	TargetDataVectorType;
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
	 * @param theLocator The applicable LocatorType, if any.
	 */
	explicit
	XPath(const LocatorType*	theLocator = 0);

	~XPath();

	/**
	 * Shrink internal tables.
	 */
	void
	shrink()
	{
		m_expression.shrink();
	}

	/**
	 * Execute the XPath from the provided context.
	 *
	 * @param context          current source tree context node, which must not be 0
	 * @param prefixResolver   prefix resolver to use
	 * @param executionContext current execution context
	 * @return smart-pointer to result XObject
	 */
	const XObjectPtr
	execute(
			XalanNode*				context,
			const PrefixResolver&	prefixResolver,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Execute the XPath from the provided context.
	 *
	 * @param context          current source tree context node, which must not be 0
	 * @param prefixResolver   prefix resolver to use
	 * @param executionContext current execution context
	 * @param result           the boolean result
	 */
	void
	execute(
			XalanNode*				context,
			const PrefixResolver&	prefixResolver,
			XPathExecutionContext&	executionContext,
			bool&					result) const;

	/**
	 * Execute the XPath from the provided context.
	 *
	 * @param context          current source tree context node, which must not be 0
	 * @param prefixResolver   prefix resolver to use
	 * @param executionContext current execution context
	 * @param result           the numeric result
	 */
	void
	execute(
			XalanNode*				context,
			const PrefixResolver&	prefixResolver,
			XPathExecutionContext&	executionContext,
			double&					result) const;

	/**
	 * Execute the XPath from the provided context.  The
	 * result is appended to the supplied string.
	 *
	 * @param context          current source tree context node, which must not be 0
	 * @param prefixResolver   prefix resolver to use
	 * @param executionContext current execution context
	 * @param result           the string result
	 */
	void
	execute(
			XalanNode*				context,
			const PrefixResolver&	prefixResolver,
			XPathExecutionContext&	executionContext,
			XalanDOMString&			result) const;

	typedef void (FormatterListener::*MemberFunctionPtr)(const XMLCh* const, const unsigned int);

	/**
	 * Execute the XPath from the provided context.
	 *
	 * @param context          current source tree context node, which must not be 0
	 * @param prefixResolver   prefix resolver to use
	 * @param executionContext current execution context
	 * @param formatterListener the FormatterListener instance to receive the result
	 * @param function A pointer to the member function of FormatterListener to call
	 */
	void
	execute(
			XalanNode*				context,
			const PrefixResolver&	prefixResolver,
			XPathExecutionContext&	executionContext,
			FormatterListener&		formatterListener,
			MemberFunctionPtr		function) const;

	/**
	 * Execute the XPath from the provided context.  Normally,
	 * the expression will be evaluated and the result placed
	 * in the parameter result.  However, some cases (such as
	 * the evalution of a variable) could result in the copying
	 * of a node-set, which is extremely expensive.  In that
	 * case, the return value will contain the result of the
	 * evaluation.  If the call to XObject::null() on the return
	 * value is true, that indicates the value was executed
	 * directly into the parameter.  Otherwise, the parameter
	 * will be empty, and the result will be in the XObject
	 * instance returned.
	 *
	 * @param context          current source tree context node, which must not be 0
	 * @param prefixResolver   prefix resolver to use
	 * @param executionContext current execution context
	 * @param result           the node-set result
	 * @return the node-set result, if the result was not returned in the parameter
	 */
	const XObjectPtr
	execute(
			XalanNode*				context,
			const PrefixResolver&	prefixResolver,
			XPathExecutionContext&	executionContext,
			MutableNodeRefList&		result) const;

	/**
	 * Execute the XPath from the provided context.
	 *
	 * @param context          current source tree context node, which must not be 0
	 * @param prefixResolver   prefix resolver to use
	 * @param contextNodeList  node list for current context
	 * @param executionContext current execution context
	 * @return smart-pointer to result XObject
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
	 * Execute the XPath from the provided context.
	 *
	 * @param context          current source tree context node, which must not be 0
	 * @param prefixResolver   prefix resolver to use
	 * @param contextNodeList  node list for current context
	 * @param executionContext current execution context
	 * @param result           the boolean result
	 */
	void
	execute(
			XalanNode*				context,
			const PrefixResolver&	prefixResolver,
			const NodeRefListBase&	contextNodeList,
			XPathExecutionContext&	executionContext,
			bool&					result) const
	{
		// Set and restore the context node list...
		XPathExecutionContext::ContextNodeListSetAndRestore		theSetAndRestore(
										executionContext,
										contextNodeList);

		execute(context, prefixResolver, executionContext, result);
	}

	/**
	 * Execute the XPath from the provided context.
	 *
	 * @param context          current source tree context node, which must not be 0
	 * @param prefixResolver   prefix resolver to use
	 * @param contextNodeList  node list for current context
	 * @param executionContext current execution context
	 * @param result           the numeric result
	 */
	void
	execute(
			XalanNode*				context,
			const PrefixResolver&	prefixResolver,
			const NodeRefListBase&	contextNodeList,
			XPathExecutionContext&	executionContext,
			double&					result) const
	{
		// Set and restore the context node list...
		XPathExecutionContext::ContextNodeListSetAndRestore		theSetAndRestore(
										executionContext,
										contextNodeList);

		execute(context, prefixResolver, executionContext, result);
	}

	/**
	 * Execute the XPath from the provided context.  The
	 * result is appended to the supplied string.
	 *
	 * @param context          current source tree context node, which must not be 0
	 * @param prefixResolver   prefix resolver to use
	 * @param contextNodeList  node list for current context
	 * @param executionContext current execution context
	 * @param result           the string result
	 */
	void
	execute(
			XalanNode*				context,
			const PrefixResolver&	prefixResolver,
			const NodeRefListBase&	contextNodeList,
			XPathExecutionContext&	executionContext,
			XalanDOMString&			result) const
	{
		// Set and restore the context node list...
		XPathExecutionContext::ContextNodeListSetAndRestore		theSetAndRestore(
										executionContext,
										contextNodeList);

		execute(context, prefixResolver, executionContext, result);
	}

	/**
	 * Execute the XPath from the provided context.
	 *
	 * @param context          current source tree context node, which must not be 0
	 * @param prefixResolver   prefix resolver to use
	 * @param contextNodeList  node list for current context
	 * @param executionContext current execution context
	 * @param formatterListener the FormatterListener instance to receive the result
	 * @param function A pointer to the member function of FormatterListener to call
	 */
	void
	execute(
			XalanNode*				context,
			const PrefixResolver&	prefixResolver,
			const NodeRefListBase&	contextNodeList,
			XPathExecutionContext&	executionContext,
			FormatterListener&		formatterListener,
			MemberFunctionPtr		function) const
	{
		// Set and restore the context node list...
		XPathExecutionContext::ContextNodeListSetAndRestore		theSetAndRestore(
										executionContext,
										contextNodeList);

		execute(context, prefixResolver, executionContext, formatterListener, function);
	}

	/**
	 * Execute the XPath from the provided context.  Normally,
	 * the expression will be evaluated and the result placed
	 * in the parameter result.  However, some cases (such as
	 * the evalution of a variable) could result in the copying
	 * of a node-set, which is extremely expensive.  In that
	 * case, the return value will contain the result of the
	 * evaluation.  If the call to XObject::null() on the return
	 * value is true, that indicates the value was executed
	 * directly into the parameter.  Otherwise, the parameter
	 * will be empty, and the result will be in the XObject
	 * instance returned.
	 *
	 * @param context          current source tree context node, which must not be 0
	 * @param prefixResolver   prefix resolver to use
	 * @param contextNodeList  node list for current context
	 * @param executionContext current execution context
	 * @param result           the result as a set of nodes
	 * @return the node-set result, if the result was not returned in the parameter
	 */
	const XObjectPtr
	execute(
			XalanNode*				context,
			const PrefixResolver&	prefixResolver,
			const NodeRefListBase&	contextNodeList,
			XPathExecutionContext&	executionContext,
			MutableNodeRefList&		result) const
	{
		// Set and restore the context node list...
		XPathExecutionContext::ContextNodeListSetAndRestore		theSetAndRestore(
										executionContext,
										contextNodeList);

		return execute(context, prefixResolver, executionContext, result);
	}

	/**
	 * Execute the XPath from the provided context.
	 *
	 * The prefix resolver and current node must already
	 * be set execution context, and must not be 0.
	 *
	 * @param executionContext current execution context
	 * @return smart-pointer to result XObject
	 */
	const XObjectPtr
	execute(XPathExecutionContext&	executionContext) const
	{
		assert(executionContext.getCurrentNode() != 0);
		assert(executionContext.getPrefixResolver() != 0);

		return executeMore(executionContext.getCurrentNode(), 0, executionContext);
	}

	/**
	 * Execute the XPath from the provided context.
	 *
	 * The prefix resolver and current node must already
	 * be set execution context, and must not be 0.
	 *
	 * @param executionContext current execution context
	 * @param result           the boolean result
	 */
	void
	execute(
			XPathExecutionContext&	executionContext,
			bool&					result) const
	{
		assert(executionContext.getCurrentNode() != 0);
		assert(executionContext.getPrefixResolver() != 0);

		executeMore(executionContext.getCurrentNode(), 0, executionContext, result);
	}

	/**
	 * Execute the XPath from the provided context.
	 *
	 * The prefix resolver must already be set in the
	 * execution context.
	 *
	 * @param executionContext current execution context
	 * @param result           the numeric result
	 */
	void
	execute(
			XPathExecutionContext&	executionContext,
			double&					result) const
	{
		assert(executionContext.getCurrentNode() != 0);
		assert(executionContext.getPrefixResolver() != 0);

		executeMore(executionContext.getCurrentNode(), 0, executionContext, result);
	}

	/**
	 * Execute the XPath from the provided context. The
	 * result is appended to the supplied string.
	 *
	 * The prefix resolver and current node must already
	 * be set execution context, and must not be 0.
	 *
	 * @param executionContext current execution context
	 * @param result           the string result
	 */
	void
	execute(
			XPathExecutionContext&	executionContext,
			XalanDOMString&			result) const
	{
		assert(executionContext.getCurrentNode() != 0);
		assert(executionContext.getPrefixResolver() != 0);

		executeMore(executionContext.getCurrentNode(), 0, executionContext, result);
	}

	/**
	 * Execute the XPath from the provided context.
	 * 
	 * The prefix resolver and current node must already
	 * be set execution context, and must not be 0.
	 *
	 * @param executionContext current execution context
	 * @param formatterListener the FormatterListener instance to receive the result
	 * @param function A pointer to the member function of FormatterListener to call
	 */
	void
	execute(
			XPathExecutionContext&	executionContext,
			FormatterListener&		formatterListener,
			MemberFunctionPtr		function) const
	{
		assert(executionContext.getCurrentNode() != 0);
		assert(executionContext.getPrefixResolver() != 0);

		executeMore(executionContext.getCurrentNode(), 0, executionContext, formatterListener, function);
	}

	/**
	 * Execute the XPath from the provided context.  Normally,
	 * the expression will be evaluated and the result placed
	 * in the parameter result.  However, some cases (such as
	 * the evalution of a variable) could result in the copying
	 * of a node-set, which is extremely expensive.  In that
	 * case, the return value will contain the result of the
	 * evaluation.  If the call to XObject::null() on the return
	 * value is true, that indicates the value was executed
	 * directly into the parameter.  Otherwise, the parameter
	 * will be empty, and the result will be in the XObject
	 * instance returned.
	 * 
	 * The prefix resolver and current node must already
	 * be set execution context, and must not be 0.
	 *
	 * @param executionContext current execution context
	 * @return the node-set result, if the result was not returned in the parameter
	 */
	const XObjectPtr
	execute(
			XPathExecutionContext&	executionContext,
			MutableNodeRefList&		result) const
	{
		assert(executionContext.getCurrentNode() != 0);
		assert(executionContext.getPrefixResolver() != 0);

		return executeMore(executionContext.getCurrentNode(), 0, executionContext, result);
	}

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
	 * @param executionContext current execution context
	 * @return union of node-set operands
	 */
	eMatchScore
	getMatchScore(
			XalanNode*				node,
			XPathExecutionContext&	executionContext) const;

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
	 * Add the data for the target of match pattern to a vector.
	 * 
	 * @param targetData The vector for the data
	 */
	void
	getTargetData(TargetDataVectorType&		targetData) const;

	/**
	 * Install a built-in function.
	 *
	 * @param funcName  unqualified name of the function
	 * @param func      instance of an XPath function object
	 */
	static void
	installFunction(
			const XalanDOMString&	funcName,
			const Function& 		func)
	{
		s_functions.InstallFunction(funcName,
									func);
	}

	/**
	 * Install a built-in function.
	 *
	 * @param funcName  unqualified name of the function
	 * @param func      instance of an XPath function object
	 */
	static void
	installFunction(
			const XalanDOMChar*		funcName,
			const Function& 		func)
	{
		s_functions.InstallFunction(funcName,
									func);
	}

	/**
	 * Remove a named function from the function table.
	 * 
	 * @param funcName name of function
	 * @return true if the function was found and removed.
	 */
	static bool
	uninstallFunction(const XalanDOMString&		funcName)
	{
		return 	s_functions.UninstallFunction(funcName);
	}

	/**
	 * Remove a named function from the function table.
	 * 
	 * @param funcName name of function
	 * @return true if the function was found and removed.
	 */
	static bool
	uninstallFunction(const XalanDOMChar*	funcName)
	{
		return 	s_functions.UninstallFunction(funcName);
	}

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

	const LocatorType*
	getLocator() const
	{
		return m_locator;
	}

	void
	setLocator(const LocatorType*	theLocator)
	{
		m_locator = theLocator;
	}

protected:

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
	 * Execute a location path.
	 *
	 * @param context          current source tree context node
	 * @param opPos            current position in the m_opMap array
	 * @param executionContext current execution context
	 * @param theResult	       the result as a node list
	 */
	void
	locationPath(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext,
			bool&					theResult) const;

	/**
	 * Execute a location path.
	 *
	 * @param context          current source tree context node
	 * @param opPos            current position in the m_opMap array
	 * @param executionContext current execution context
	 * @param theResult	       the result as a node list
	 */
	void
	locationPath(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext,
			double&					theResult) const;

	/**
	 * Execute a location path.
	 *
	 * @param context          current source tree context node
	 * @param opPos            current position in the m_opMap array
	 * @param executionContext current execution context
	 * @param theResult	       the result as a node list
	 */
	void
	locationPath(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext,
			XalanDOMString&			theResult) const;

	/**
	 * Execute a location path.
	 *
	 * @param context          current source tree context node
	 * @param opPos            current position in the m_opMap array
	 * @param executionContext current execution context
	 * @param formatterListener the FormatterListener instance to receive the result
	 * @param function A pointer to the member function of FormatterListener to call
	 */
	void
	locationPath(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext,
			FormatterListener&		formatterListener,
			MemberFunctionPtr		function) const;

	/**
	 * Execute a location path.
	 *
	 * @param context          current source tree context node
	 * @param opPos            current position in the m_opMap array
	 * @param executionContext current execution context
	 * @param theResult	       the result as a node list
	 */
	void
	locationPath(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext,
			MutableNodeRefList&		theResult) const
	{
		step(executionContext, context, opPos + 2, theResult);
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
	 * Execute the XPath from the provided context.
	 *
	 * @param context          current source tree context node
	 * @param opPos            current position in the m_opMap array
	 * @param executionContext current execution context
	 * @param theResult        The result of the execution
	 */
	void
	executeMore(
			XalanNode* 				context,
			int 					opPos,
			XPathExecutionContext&	executionContext,
			bool&					theResult) const;

	/**
	 * Execute the XPath from the provided context.
	 *
	 * @param context          current source tree context node
	 * @param opPos            current position in the m_opMap array
	 * @param executionContext current execution context
	 * @param theResult        The result of the execution
	 */
	void
	executeMore(
			XalanNode* 				context,
			int 					opPos,
			XPathExecutionContext&	executionContext,
			double&					theResult) const;

	/**
	 * Execute the XPath from the provided context.  The result
	 * is appended to the supplied string.
	 *
	 * @param context          current source tree context node
	 * @param opPos            current position in the m_opMap array
	 * @param executionContext current execution context
	 * @param theResult        The result of the execution
	 */
	void
	executeMore(
			XalanNode* 				context,
			int 					opPos,
			XPathExecutionContext&	executionContext,
			XalanDOMString&			theResult) const;

	/**
	 * Execute the XPath from the provided context.
	 *
	 * @param context          current source tree context node
	 * @param opPos            current position in the m_opMap array
	 * @param executionContext current execution context
	 * @param formatterListener the FormatterListener instance to receive the result
	 * @param function A pointer to the member function of FormatterListener to call
	 */
	void
	executeMore(
			XalanNode* 				context,
			int 					opPos,
			XPathExecutionContext&	executionContext,
			FormatterListener&		formatterListener,
			MemberFunctionPtr		function) const;

	/**
	 * Execute the XPath from the provided context.
	 *
	 * @param context          current source tree context node
	 * @param opPos            current position in the m_opMap array
	 * @param executionContext current execution context
	 * @param theResult        The result of the execution
	 * @return the node-set result, if the result was not returned in the parameter
	 */
	const XObjectPtr
	executeMore(
			XalanNode* 				context,
			int 					opPos,
			XPathExecutionContext&	executionContext,
			MutableNodeRefList&		theResult) const;

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
	 * @param executionContext current execution context
	 * @return true if the one of the two arguments are true.
	 */
	bool
	Or(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * OR two expressions and return the boolean result.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @return true if the two arguments are both true.
	 */
	bool
	And(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Tell if two expressions are functionally not equal.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @return true if the two arguments are not equal.
	 */
	bool
	notequals(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Tell if two expressions are functionally equal.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @return true if the two arguments are equal.
	 */
	bool
	equals(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Tell if one argument is less than or equal to the other argument.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @return true if arg 1 is less than or equal to arg 2.
	 */
	bool
	lte(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Tell if one argument is less than the other argument.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @return true if arg 1 is less than arg 2.
	 */
	bool
	lt(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Tell if one argument is greater than or equal to the other argument.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @return true if arg 1 is greater than or equal to arg 2.
	 */
	bool
	gte(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Tell if one argument is greater than the other argument.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @return true if arg 1 is greater than arg 2.
	 */
	bool
	gt(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Give the sum of two arguments.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @return sum of arg1 and arg2.
	 */
	double
	plus(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Give the sum of two arguments.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @param formatterListener the FormatterListener instance to receive the result
	 * @param function A pointer to the member function of FormatterListener to call
	 */
	void
	plus(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext,
			FormatterListener&		formatterListener,
			MemberFunctionPtr		function) const;

	/**
	 * Give the difference of two arguments.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @return difference of arg1 and arg2.
	 */
	double
	minus(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Give the difference of two arguments.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @param formatterListener the FormatterListener instance to receive the result
	 * @param function A pointer to the member function of FormatterListener to call
	 */
	void
	minus(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext,
			FormatterListener&		formatterListener,
			MemberFunctionPtr		function) const;

	/**
	 * Multiply two arguments.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @return arg1 * arg2.
	 */
	double
	mult(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Multiply two arguments.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @param formatterListener the FormatterListener instance to receive the result
	 * @param function A pointer to the member function of FormatterListener to call
	 */
	void
	mult(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext,
			FormatterListener&		formatterListener,
			MemberFunctionPtr		function) const;

	/**
	 * Divide a number.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @return arg1 / arg2.
	 */
	double
	div(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Divide a number.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @param formatterListener the FormatterListener instance to receive the result
	 * @param function A pointer to the member function of FormatterListener to call
	 */
	void
	div(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext,
			FormatterListener&		formatterListener,
			MemberFunctionPtr		function) const;

	/**
	 * Return the remainder from a truncating division.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @return arg1 mod arg2.
	 */
	double
	mod(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Return the remainder from a truncating division.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @param formatterListener the FormatterListener instance to receive the result
	 * @param function A pointer to the member function of FormatterListener to call
	 */
	void
	mod(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext,
			FormatterListener&		formatterListener,
			MemberFunctionPtr		function) const;

	/**
	 * Return the negation of a number.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @return -arg.
	 */
	double
	neg(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Return the negation of a number.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @param formatterListener the FormatterListener instance to receive the result
	 * @param function A pointer to the member function of FormatterListener to call
	 */
	void
	neg(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext,
			FormatterListener&		formatterListener,
			MemberFunctionPtr		function) const;

	/**
	 * Computes the union of its operands which must be node-sets.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @return the union of node-set operands.
	 */
	const XObjectPtr
	Union(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Computes the union of its operands which must be node-sets.
	 *
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @result the result of the union of node-set operands.
	 */
	void
	Union(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext,
			bool&					result) const;

	/**
	 * Computes the union of its operands which must be node-sets.
	 *
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @result the result of the union of node-set operands.
	 */
	void
	Union(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext,
			double&					result) const;

	/**
	 * Computes the union of its operands which must be node-sets.
	 *
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @result the result of the union of node-set operands.
	 */
	void
	Union(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext,
			XalanDOMString&			result) const;

	/**
	 * Computes the union of its operands which must be node-sets.
	 *
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @param formatterListener the FormatterListener instance to receive the result
	 * @param function A pointer to the member function of FormatterListener to call
	 */
	void
	Union(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext,
			FormatterListener&		formatterListener,
			MemberFunctionPtr		function) const;

	/**
	 * Computes the union of its operands which must be node-sets.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @result the result of the union of node-set operands.
	 */
	void
	Union(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext,
			MutableNodeRefList&		result) const;

	/**
	 * Get a literal value.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @return an XObject object.
	 */
	const XObjectPtr
	literal(
			int						opPos,
			XPathExecutionContext&	executionContext) const;
  
	/**
	 * Get a literal value as a boolean.
	 *
	 * @param opPos The current position in the m_opMap array.
	 * @param theResult The value.
	 */
	void
	literal(
			int		opPos,
			bool&	theResult) const;

	/**
	 * Get a literal value as a number.
	 *
	 * @param opPos The current position in the m_opMap array.
	 * @param theResult The value.
	 */
	void
	literal(
			int			opPos,
			double&		theResult) const;

	/**
	 * Get a literal value.  The value is appended to the
	 * supplied string.
	 *
	 * @param opPos The current position in the m_opMap array.
	 * @param theResult The string.
	 */
	void
	literal(
			int					opPos,
			XalanDOMString&		theResult) const;

	/**
	 * Get a literal value.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @return The result as a double.
	 */
	void
	literal(
			int					opPos,
			FormatterListener&	formatterListener,
			MemberFunctionPtr	function) const;

	/**
	 * Get the value of a variable.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @return an XObject object.
	 */
	const XObjectPtr
	variable(
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Execute an expression as a group.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
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
	 * Execute an expression as a group.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @param theResult The result of the execution
	 */
	void
	group(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext,
			bool&					theResult) const
	{
		executeMore(context, opPos + 2, executionContext, theResult);
	}

	/**
	 * Execute an expression as a group.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @param theResult The result of the execution
	 */
	void
	group(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext,
			double&					theResult) const
	{
		executeMore(context, opPos + 2, executionContext, theResult);
	}

	/**
	 * Execute an expression as a group.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @param theResult The result of the execution
	 */
	void
	group(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext,
			XalanDOMString&			theResult) const
	{
		executeMore(context, opPos + 2, executionContext, theResult);
	}

	/**
	 * Execute an expression as a group.
	 *
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @param formatterListener the FormatterListener instance to receive the result
	 * @param function A pointer to the member function of FormatterListener to call
	 */
	void
	group(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext,
			FormatterListener&		formatterListener,
			MemberFunctionPtr		function) const
	{
		executeMore(context, opPos + 2, executionContext, formatterListener, function);
	}

	/**
	 * Execute an expression as a group.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @param theResult The result of the execution
	 */
	void
	group(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext,
			MutableNodeRefList&		theResult) const
	{
		executeMore(context, opPos + 2, executionContext, theResult);
	}

	/**
	 * Get a literal value.
	 * @param opPos The current position in the m_opMap array.
	 * @return The result as a double.
	 */
	double
	numberlit(int	opPos) const;

	/**
	 * Get a literal value.
	 * @param opPos The current position in the m_opMap array.
	 * @return The result as a double.
	 */
	const XObjectPtr
	numberlit(
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Get a literal value as a boolean.
	 *
	 * @param opPos The current position in the m_opMap array.
	 * @param theResult The string.
	 */
	void
	numberlit(
			int		opPos,
			bool&	theResult) const;

	/**
	 * Get a literal value.  The value is appended to the
	 * supplied string.
	 *
	 * @param opPos The current position in the m_opMap array.
	 * @param theResult The string.
	 */
	void
	numberlit(
			int					opPos,
			XalanDOMString&		theResult) const;

	/**
	 * Get a literal value.
	 *
	 * @param opPos The current position in the m_opMap array.
	 * @param formatterListener the FormatterListener instance to receive the result
	 * @param function A pointer to the member function of FormatterListener to call
	 */
	void
	numberlit(
			int					opPos,
			FormatterListener&	formatterListener,
			MemberFunctionPtr	function) const;

	/**
	 * Setup for and run an extension function.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @return the result of the function.
	 */
	const XObjectPtr
	runExtFunction(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Handle an extension function.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @param theNamespace The namespace of the function.
	 * @param functionName The name of the function.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @return the result of the function.
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
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @return the result of the function.
	 */
	const XObjectPtr
	runFunction(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Handle a built-in function.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @param argVec The arguments for the function.
	 * @param executionContext current execution context
	 * @return the result of the function.
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

	/**
	 * Handle the built-in function "position".
	 *
	 * @param context The current source tree context node, which must not be 0.
	 * @param executionContext current execution context
	 * @return the result of the function.
	 */
	double
	functionPosition(
			XalanNode*				context,
			XPathExecutionContext&	executionContext) const
	{
		assert(context != 0);

		return executionContext.getContextNodeListPosition(*context);
	}

	/**
	 * Handle the built-in function "last".
	 *
	 * @param executionContext current execution context
	 * @return the result of the function.
	 */
	double
	functionLast(XPathExecutionContext&		executionContext) const
	{
		return executionContext.getContextNodeListLength();
	}

	/**
	 * Handle the built-in function "count".
	 *
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @return the result of the function.
	 */
	double
	functionCount(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Handle the built-in function "not".
	 *
	 * @param context The current source tree context node, which must not be 0.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @return the result of the function.
	 */
	bool
	functionNot(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
	{
		assert(context != 0);

		return !functionBoolean(context, opPos, executionContext);
	}

	/**
	 * Handle the built-in function "boolean".
	 *
	 * @param context The current source tree context node, which must not be 0.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @return the result of the function.
	 */
	bool
	functionBoolean(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
	{
		assert(context != 0);

		bool	result;

		executeMore(context, opPos + 2, executionContext, result);

		return result;
	}

	/**
	 * Handle the built-in function "name".
	 *
	 * @param context The current source tree context node, which must not be 0.
	 * @return the result of the function.
	 */
	const XalanDOMString&
	functionName(XalanNode*		context) const
	{
		assert(context != 0);

		return DOMServices::getNameOfNode(*context);
	}

	/**
	 * Handle the built-in function "name".
	 *
	 * @param context The current source tree context node, which must not be 0.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @return the result of the function.
	 */
	const XalanDOMString&
	functionName(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Handle the built-in function "local-name".
	 *
	 * @param context The current source tree context node, which must not be 0.
	 * @return the result of the function.
	 */
	const XalanDOMString&
	functionLocalName(XalanNode*	context) const;

	/**
	 * Handle the built-in function "local-name".
	 *
	 * @param context The current source tree context node, which must not be 0.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @return the result of the function.
	 */
	const XalanDOMString&
	functionLocalName(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Handle the built-in function "number".
	 *
	 * @param context The current source tree context node, which must not be 0.
	 * @param executionContext current execution context
	 * @return the result of the function.
	 */
	double
	functionNumber(
			XalanNode*				context,
			XPathExecutionContext&	executionContext) const
	{
		assert(context != 0);

		return XObject::number(executionContext, *context);
	}

	/**
	 * Handle the built-in function "number".
	 *
	 * @param context The current source tree context node, which must not be 0.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @return the result of the function.
	 */
	double
	functionNumber(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
	{
		double	result;

		executeMore(context, opPos + 2, executionContext, result);

		return result;
	}

	/**
	 * Handle the built-in function "floor".
	 *
	 * @param context The current source tree context node, which must not be 0.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @return the result of the function.
	 */
	double
	functionFloor(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
	{
		return DoubleSupport::floor(functionNumber(context, opPos, executionContext));
	}

	/**
	 * Handle the built-in function "ceiling".
	 *
	 * @param context The current source tree context node, which must not be 0.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @return the result of the function.
	 */
	double
	functionCeiling(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
	{
		return DoubleSupport::ceiling(functionNumber(context, opPos, executionContext));
	}

	/**
	 * Handle the built-in function "round".
	 *
	 * @param context The current source tree context node, which must not be 0.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @return the result of the function.
	 */
	double
	functionRound(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const
	{
		return DoubleSupport::round(functionNumber(context, opPos, executionContext));
	}

	/**
	 * Handle the built-in function "string-length".
	 *
	 * @param context The current source tree context node, which must not be 0.
	 * @return the result of the function.
	 */
	double
	functionStringLength(XalanNode*		context) const;

	/**
	 * Handle the built-in function "string-length".
	 *
	 * @param context The current source tree context node, which must not be 0.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @return the result of the function.
	 */
	double
	functionStringLength(
			XalanNode*				context,
			int						opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Get a numeric operand for an expression.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @param executionContext current execution context
	 * @return The value of the operand.
	 */
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
		eDefaultTargetDataSize = 5
	};

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

		typedef eMatchScore (NodeTester::*TestFunctionPtr)(const XalanNode&, XalanNode::NodeType) const;


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

		TestFunctionPtr			m_testFunction;
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

	int
	predicates(
			XPathExecutionContext&	executionContext,
			int 					opPos,
			MutableNodeRefList& 	subQueryResults) const;

	eMatchScore
	handleFoundIndex(
			XPathExecutionContext&	executionContext,
			XalanNode* 				localContext,
			int 					startOpPos) const;

	eMatchScore
	handleFoundIndexPositional(
			XPathExecutionContext&	executionContext,
			XalanNode* 				localContext,
			int 					startOpPos) const;

private:

	void
	unknownOpCodeError(
			XalanNode*				context,
			XPathExecutionContext&	executionContext,
			int						opPos) const;

	void
	notNodeSetError(
			XalanNode*				context,
			XPathExecutionContext&	executionContext) const;

	// Data members...

	/**
	 *
	 * Holds information about the current expression.
	 *
	 */
	XPathExpression		m_expression;

	/**
	 * A LocatorType for reporting errors.
	 */
	const LocatorType*	m_locator;

	/**
	 * If true, the XPath can allocated XObjects in more
	 * efficient ways, since its lifetime is guaranteed
	 * to be at least that of the transformation.
	 */
	bool				m_inStylesheet;

	/**
	 *
	 * This is the table of installed functions.
	 *
	 */
	static FunctionTableType			s_functions;

	static const XalanDOMString			s_emptyString;
};



XALAN_CPP_NAMESPACE_END



#endif	// XPATH_HEADER_GUARD_1357924680
