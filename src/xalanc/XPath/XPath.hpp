/*
 * Copyright 1999-2004 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#if !defined(XPATH_HEADER_GUARD_1357924680)
#define XPATH_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/PlatformSupport/DoubleSupport.hpp>



// Base class header files...
#include <xalanc/XPath/XPathExecutionContext.hpp>



#include <xalanc/XPath/MutableNodeRefList.hpp>
#include <xalanc/XPath/XPathExpression.hpp>
#include <xalanc/XPath/Function.hpp>
#include <xalanc/XPath/XPathFunctionTable.hpp>



XALAN_DECLARE_XERCES_CLASS(Locator)



XALAN_CPP_NAMESPACE_BEGIN



class PrefixResolver;
class XObject;
class XalanElement;
class XalanNode;
class XPathConstructionContext;



class XALAN_XPATH_EXPORT XPath
{
public:

	typedef XERCES_CPP_NAMESPACE_QUALIFIER Locator	LocatorType;

    typedef XPathExpression::OpCodeMapPositionType              OpCodeMapPositionType;
    typedef XPathExpression::OpCodeMapValueType                 OpCodeMapValueType;
    typedef XPathExpression::TokenQueuePositionType             TokenQueuePositionType;
    typedef XPathExecutionContext::GetAndReleaseCachedString    GetCachedString;
    typedef XPathExecutionContext::PrefixResolverSetAndRestore  PrefixResolverSetAndRestore;
    typedef XPathExecutionContext::CurrentNodePushAndPop        CurrentNodePushAndPop;


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

	typedef XalanVector<TargetData>		TargetDataVectorType;

	/**
	 * Perform static initialization.  See class XPathInit.
	 */
	static void
	initialize(MemoryManagerType& theManager);

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
	XPath(MemoryManagerType& theManager, const LocatorType*	theLocator = 0);

    static XPath*
    create(MemoryManagerType& theManager, const LocatorType*	theLocator = 0);

    MemoryManagerType&
     getMemoryManager()
    {
        return m_expression.getMemoryManager();
    }
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
		// Push and pop the context node list...
		XPathExecutionContext::ContextNodeListPushAndPop	thePushAndPop(
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
		// Push and pop the context node list...
		XPathExecutionContext::ContextNodeListPushAndPop	thePushAndPop(
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
		// Push and pop the context node list...
		XPathExecutionContext::ContextNodeListPushAndPop	thePushAndPop(
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
		// Push and pop the context node list...
		XPathExecutionContext::ContextNodeListPushAndPop	thePushAndPop(
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
		// Push and pop the context node list...
		XPathExecutionContext::ContextNodeListPushAndPop	thePushAndPop(
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
		// Push and pop the context node list...
		XPathExecutionContext::ContextNodeListPushAndPop	thePushAndPop(
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

		return executeMore(
                executionContext.getCurrentNode(),
                getInitialOpCodePosition(),
                executionContext);
	}

	/**
	 * Execute the XPath from the provided context.
	 *
	 * The prefix resolver and current node must already
	 * be set execution context, and must not be 0.
	 *
	 * @param executionContext current execution context
	 * @param result the boolean result
	 */
	void
	execute(
			XPathExecutionContext&	executionContext,
			bool&					result) const
	{
		assert(executionContext.getCurrentNode() != 0);
		assert(executionContext.getPrefixResolver() != 0);

		executeMore(
                executionContext.getCurrentNode(),
                getInitialOpCodePosition(),
                executionContext,
                result);
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

		executeMore(
                executionContext.getCurrentNode(),
                getInitialOpCodePosition(),
                executionContext,
                result);
	}

	/**
	 * Execute the XPath from the provided context. The
	 * result is appended to the supplied string.
	 *
	 * The prefix resolver and current node must already
	 * be set execution context, and must not be 0.
	 *
	 * @param executionContext current execution context
	 * @param result the string result
	 */
	void
	execute(
			XPathExecutionContext&	executionContext,
			XalanDOMString&			result) const
	{
		assert(executionContext.getCurrentNode() != 0);
		assert(executionContext.getPrefixResolver() != 0);

		executeMore(
                executionContext.getCurrentNode(),
                getInitialOpCodePosition(),
                executionContext,
                result);
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

		executeMore(
                executionContext.getCurrentNode(),
                getInitialOpCodePosition(),
                executionContext,
                formatterListener,
                function);
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
	 * @param result A node list for the result.  This may or may not contain the actual result.
	 * @return the node-set result, if the result was not returned in the parameter
	 */
	const XObjectPtr
	execute(
			XPathExecutionContext&	executionContext,
			MutableNodeRefList&		result) const
	{
		assert(executionContext.getCurrentNode() != 0);
		assert(executionContext.getPrefixResolver() != 0);

		return executeMore(
                executionContext.getCurrentNode(),
                getInitialOpCodePosition(),
                executionContext,
                result);
	}

	/**
	 * Execute the XPath from the provided context.
	 *
	 * The current node must already be set execution context,
	 * and must not be 0.
	 *
	 * @param executionContext current execution context
	 * @param prefixResolver   prefix resolver to use
	 * @return smart-pointer to result XObject
	 */
	const XObjectPtr
	execute(
			const PrefixResolver&	prefixResolver,
			XPathExecutionContext&	executionContext) const
	{
		assert(executionContext.getCurrentNode() != 0);

		// Push and pop the PrefixResolver...
		const PrefixResolverSetAndRestore	theResolverSetAndRestore(
										        executionContext,
										        &prefixResolver);

		return executeMore(
                executionContext.getCurrentNode(),
                getInitialOpCodePosition(),
                executionContext);
	}

	/**
	 * Execute the XPath from the provided context.
	 *
	 * The current node must already be set execution context,
	 * and must not be 0.
	 *
	 * @param executionContext current execution context
	 * @param prefixResolver prefix resolver to use
	 * @param result the boolean result
	 */
	void
	execute(
			const PrefixResolver&	prefixResolver,
			XPathExecutionContext&	executionContext,
			bool&					result) const
	{
		assert(executionContext.getCurrentNode() != 0);

		// Push and pop the PrefixResolver...
		const PrefixResolverSetAndRestore	theResolverSetAndRestore(
										        executionContext,
										        &prefixResolver);

		executeMore(
                executionContext.getCurrentNode(),
                getInitialOpCodePosition(),
                executionContext,
                result);
	}

	/**
	 * Execute the XPath from the provided context.
	 *
	 * The current node must already be set execution context,
	 * and must not be 0.
	 *
	 * @param executionContext current execution context
	 * @param prefixResolver prefix resolver to use
	 * @param result the numeric result
	 */
	void
	execute(
			const PrefixResolver&	prefixResolver,
			XPathExecutionContext&	executionContext,
			double&					result) const
	{
		assert(executionContext.getCurrentNode() != 0);

		// Push and pop the PrefixResolver...
		const PrefixResolverSetAndRestore	theResolverSetAndRestore(
										        executionContext,
										        &prefixResolver);

		executeMore(
                executionContext.getCurrentNode(),
                getInitialOpCodePosition(),
                executionContext,
                result);
	}

	/**
	 * Execute the XPath from the provided context. The
	 * result is appended to the supplied string.
	 *
	 * The current node must already be set execution context,
	 * and must not be 0.
	 *
	 * @param executionContext current execution context
	 * @param prefixResolver prefix resolver to use
	 * @param result the string result
	 */
	void
	execute(
			const PrefixResolver&	prefixResolver,
			XPathExecutionContext&	executionContext,
			XalanDOMString&			result) const
	{
		assert(executionContext.getCurrentNode() != 0);

		// Push and pop the PrefixResolver...
		const PrefixResolverSetAndRestore	theResolverSetAndRestore(
										        executionContext,
										        &prefixResolver);

		executeMore(
                executionContext.getCurrentNode(),
                getInitialOpCodePosition(),
                executionContext,
                result);
	}

	/**
	 * Execute the XPath from the provided context.
	 *
	 * @param prefixResolver   prefix resolver to use
	 * @param executionContext current execution context
	 * @param formatterListener the FormatterListener instance to receive the result
	 * @param function A pointer to the member function of FormatterListener to call
	 */
	void
	execute(
			const PrefixResolver&	prefixResolver,
			XPathExecutionContext&	executionContext,
			FormatterListener&		formatterListener,
			MemberFunctionPtr		function) const
	{
		assert(executionContext.getCurrentNode() != 0);

		// Push and pop the PrefixResolver...
		const PrefixResolverSetAndRestore	theResolverSetAndRestore(
										        executionContext,
										        &prefixResolver);

		executeMore(
                executionContext.getCurrentNode(),
                getInitialOpCodePosition(),
                executionContext,
                formatterListener,
                function);
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
	 * The current node must already be set execution context,
	 * and must not be 0.
	 *
	 * @param executionContext current execution context
	 * @param prefixResolver prefix resolver to use
	 * @param result A node list for the result.  This may or may not contain the actual result.
	 * @return the node-set result, if the result was not returned in the parameter
	 */
	XObjectPtr
	execute(
			const PrefixResolver&	prefixResolver,
			XPathExecutionContext&	executionContext,
			MutableNodeRefList&		result) const
	{
		assert(executionContext.getCurrentNode() != 0);

		// Push and pop the PrefixResolver...
		const PrefixResolverSetAndRestore	theResolverSetAndRestore(
										        executionContext,
										        &prefixResolver);

		return executeMore(
                executionContext.getCurrentNode(),
                getInitialOpCodePosition(),
                executionContext,
                result);
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
	 * @param opPos            current position in the Op Map
	 * @param executionContext current execution context
	 * @return pointer to either a boolean or a number
	 */
	const XObjectPtr
	predicate(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
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
		return s_functions.UninstallFunction(funcName);
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
		return s_functions.UninstallFunction(funcName);
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

	class NodeTester
	{
	public:

		NodeTester();

		NodeTester(const NodeTester&	theSource);

		NodeTester(
			const XPath&			xpath,
			XPathExecutionContext&	executionContext,
			OpCodeMapPositionType 	opPos,
			OpCodeMapValueType 	    argLen,
			OpCodeMapValueType 		stepType);

		NodeTester(
            XPathConstructionContext&	theContext,
            const XalanDOMString&       theNameTest,
            const PrefixResolver&       thePrefixResolver,
            const LocatorType*          theLocator = 0,
            eMatchScore*				theMatchScore = 0);

		NodeTester(
            const XalanDOMString&   theNamespaceURI,
            const XalanDOMString&   theLocalName,
            eMatchScore*            theMatchScore = 0);

		eMatchScore
		operator()(
			const XalanNode& 		context,
			XalanNode::NodeType		nodeType) const
		{
			assert(context.getNodeType() == nodeType);

			return (this->*m_testFunction)(context, nodeType);
		}

		eMatchScore
		operator()(const XalanElement&	context) const
		{
			return (this->*m_testFunction2)(context);
		}

		NodeTester&
		operator=(const NodeTester&		theRHS)
		{
			m_executionContext = theRHS.m_executionContext;
			m_targetNamespace = theRHS.m_targetNamespace;
			m_targetLocalName = theRHS.m_targetLocalName;
			m_testFunction = theRHS.m_testFunction;
			m_testFunction2 = theRHS.m_testFunction2;

			return *this;
		}

	protected:

		eMatchScore
		initialize(
            XPathConstructionContext&	theConstructionContext,
            const XalanDOMString&       theNameTest,
            const PrefixResolver&       thePrefixResolver,
            const LocatorType*          theLocator);

		eMatchScore
		initialize(
            const XalanDOMString&   theNamespaceURI,
            const XalanDOMString&   theLocalName);

	private:


		typedef eMatchScore (NodeTester::*TestFunctionPtr)(const XalanNode&, XalanNode::NodeType) const;
		typedef eMatchScore (NodeTester::*TestFunctionPtr2)(const XalanElement&) const;


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
		testElementNCName2(const XalanElement&	context) const;

		eMatchScore
		testElementQName2(const XalanElement&	context) const;

		eMatchScore
		testElementNamespaceOnly2(const XalanElement&	context) const;

		eMatchScore
		testElementTotallyWild2(const XalanElement&		context) const;

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

		eMatchScore
		testDefault2(const XalanElement&	context) const;

		bool
		matchLocalName(const XalanNode&		context) const;

		bool
		matchNamespaceURI(const XalanNode&	context) const;

		bool
		matchLocalNameAndNamespaceURI(const XalanNode&	context) const;

		bool
		matchNamespace(const XalanNode&		context) const;

		bool
		shouldStripSourceNode(const XalanText&	context) const;

		// Data members...
		XPathExecutionContext*	m_executionContext;

		const XalanDOMString*	m_targetNamespace;

		const XalanDOMString*	m_targetLocalName;

		TestFunctionPtr			m_testFunction;

        TestFunctionPtr2        m_testFunction2;
	};

	friend class NodeTester;

protected:

	/**
	 * Execute a location path.
	 *
	 * @param context          current source tree context node
	 * @param opPos            current position in the Op Mpa
	 * @param executionContext current execution context
	 * @return node-set
	 */
	const XObjectPtr
	locationPath(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Execute a location path.
	 *
	 * @param context          current source tree context node
	 * @param opPos            current position in the Op Map
	 * @param executionContext current execution context
	 * @param theResult	       the result as a node list
	 */
	void
	locationPath(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext,
			bool&					theResult) const;

	/**
	 * Execute a location path.
	 *
	 * @param context          current source tree context node
	 * @param opPos            current position in the Op Map
	 * @param executionContext current execution context
	 * @param theResult	       the result as a node list
	 */
	void
	locationPath(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext,
			double&					theResult) const;

	/**
	 * Execute a location path.
	 *
	 * @param context          current source tree context node
	 * @param opPos            current position in the Op Map
	 * @param executionContext current execution context
	 * @param theResult	       the result as a node list
	 */
	void
	locationPath(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext,
			XalanDOMString&			theResult) const;

	/**
	 * Execute a location path.
	 *
	 * @param context          current source tree context node
	 * @param opPos            current position in the Op Map
	 * @param executionContext current execution context
	 * @param formatterListener the FormatterListener instance to receive the result
	 * @param function A pointer to the member function of FormatterListener to call
	 */
	void
	locationPath(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext,
			FormatterListener&		formatterListener,
			MemberFunctionPtr		function) const;

	/**
	 * Execute a location path.
	 *
	 * @param context          current source tree context node
	 * @param opPos            current position in the Op Map
	 * @param executionContext current execution context
	 * @param theResult	       the result as a node list
	 */
	void
	locationPath(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext,
			MutableNodeRefList&		theResult) const
	{
		step(executionContext, context, opPos + 2, theResult);
	}

	/**
	 * Execute the XPath from the provided context.
	 *
	 * @param context          current source tree context node
	 * @param opPos            current position in the Op Map
	 * @param executionContext current execution context
	 * @return pointer to union of node-set operands
	 */
	const XObjectPtr
	executeMore(
			XalanNode* 				context,
			OpCodeMapPositionType 	opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Execute the XPath from the provided context.
	 *
	 * @param context          current source tree context node
	 * @param opPos            current position in the Op Map
	 * @param executionContext current execution context
	 * @param theResult        The result of the execution
	 */
	void
	executeMore(
			XalanNode* 				context,
			OpCodeMapPositionType 	opPos,
			XPathExecutionContext&	executionContext,
			bool&					theResult) const;

	/**
	 * Execute the XPath from the provided context.
	 *
	 * @param context          current source tree context node
	 * @param opPos            current position in the Op Map
	 * @param executionContext current execution context
	 * @param theResult        The result of the execution
	 */
	void
	executeMore(
			XalanNode* 				context,
			OpCodeMapPositionType 	opPos,
			XPathExecutionContext&	executionContext,
			double&					theResult) const;

	/**
	 * Execute the XPath from the provided context.  The result
	 * is appended to the supplied string.
	 *
	 * @param context          current source tree context node
	 * @param opPos            current position in the Op Map
	 * @param executionContext current execution context
	 * @param theResult        The result of the execution
	 */
	void
	executeMore(
			XalanNode* 				context,
			OpCodeMapPositionType 	opPos,
			XPathExecutionContext&	executionContext,
			XalanDOMString&			theResult) const;

	/**
	 * Execute the XPath from the provided context.
	 *
	 * @param context          current source tree context node
	 * @param opPos            current position in the Op Map
	 * @param executionContext current execution context
	 * @param formatterListener the FormatterListener instance to receive the result
	 * @param function A pointer to the member function of FormatterListener to call
	 */
	void
	executeMore(
			XalanNode* 				context,
			OpCodeMapPositionType 	opPos,
			XPathExecutionContext&	executionContext,
			FormatterListener&		formatterListener,
			MemberFunctionPtr		function) const;

	/**
	 * Execute the XPath from the provided context.
	 *
	 * @param context          current source tree context node
	 * @param opPos            current position in the Op Map
	 * @param executionContext current execution context
	 * @param theResult        The result of the execution
	 * @return the node-set result, if the result was not returned in the parameter
	 */
	const XObjectPtr
	executeMore(
			XalanNode* 				context,
			OpCodeMapPositionType 	opPos,
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
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @return true if the one of the two arguments are true.
	 */
	bool
	Or(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * OR two expressions and return the boolean result.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @return true if the two arguments are both true.
	 */
	bool
	And(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Tell if two expressions are functionally not equal.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @return true if the two arguments are not equal.
	 */
	bool
	notequals(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Tell if two expressions are functionally equal.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @return true if the two arguments are equal.
	 */
	bool
	equals(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Tell if one argument is less than or equal to the other argument.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @return true if arg 1 is less than or equal to arg 2.
	 */
	bool
	lte(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Tell if one argument is less than the other argument.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @return true if arg 1 is less than arg 2.
	 */
	bool
	lt(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Tell if one argument is greater than or equal to the other argument.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @return true if arg 1 is greater than or equal to arg 2.
	 */
	bool
	gte(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Tell if one argument is greater than the other argument.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @return true if arg 1 is greater than arg 2.
	 */
	bool
	gt(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Give the sum of two arguments.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @return sum of arg1 and arg2.
	 */
	double
	plus(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Give the sum of two arguments.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @param formatterListener the FormatterListener instance to receive the result
	 * @param function A pointer to the member function of FormatterListener to call
	 */
	void
	plus(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext,
			FormatterListener&		formatterListener,
			MemberFunctionPtr		function) const;

	/**
	 * Give the difference of two arguments.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @return difference of arg1 and arg2.
	 */
	double
	minus(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Give the difference of two arguments.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @param formatterListener the FormatterListener instance to receive the result
	 * @param function A pointer to the member function of FormatterListener to call
	 */
	void
	minus(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext,
			FormatterListener&		formatterListener,
			MemberFunctionPtr		function) const;

	/**
	 * Multiply two arguments.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @return arg1 * arg2.
	 */
	double
	mult(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Multiply two arguments.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @param formatterListener the FormatterListener instance to receive the result
	 * @param function A pointer to the member function of FormatterListener to call
	 */
	void
	mult(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext,
			FormatterListener&		formatterListener,
			MemberFunctionPtr		function) const;

	/**
	 * Divide a number.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @return arg1 / arg2.
	 */
	double
	div(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Divide a number.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @param formatterListener the FormatterListener instance to receive the result
	 * @param function A pointer to the member function of FormatterListener to call
	 */
	void
	div(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext,
			FormatterListener&		formatterListener,
			MemberFunctionPtr		function) const;

	/**
	 * Return the remainder from a truncating division.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @return arg1 mod arg2.
	 */
	double
	mod(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Return the remainder from a truncating division.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @param formatterListener the FormatterListener instance to receive the result
	 * @param function A pointer to the member function of FormatterListener to call
	 */
	void
	mod(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext,
			FormatterListener&		formatterListener,
			MemberFunctionPtr		function) const;

	/**
	 * Return the negation of a number.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @return -arg.
	 */
	double
	neg(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Return the negation of a number.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @param formatterListener the FormatterListener instance to receive the result
	 * @param function A pointer to the member function of FormatterListener to call
	 */
	void
	neg(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext,
			FormatterListener&		formatterListener,
			MemberFunctionPtr		function) const;

	/**
	 * Computes the union of its operands which must be node-sets.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @return the union of node-set operands.
	 */
	const XObjectPtr
	Union(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Computes the union of its operands which must be node-sets.
	 *
	 * @param context The current source tree context node.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @result the result of the union of node-set operands.
	 */
	void
	Union(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext,
			bool&					result) const;

	/**
	 * Computes the union of its operands which must be node-sets.
	 *
	 * @param context The current source tree context node.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @result the result of the union of node-set operands.
	 */
	void
	Union(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext,
			double&					result) const;

	/**
	 * Computes the union of its operands which must be node-sets.
	 *
	 * @param context The current source tree context node.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @result the result of the union of node-set operands.
	 */
	void
	Union(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext,
			XalanDOMString&			result) const;

	/**
	 * Computes the union of its operands which must be node-sets.
	 *
	 * @param context The current source tree context node.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @param formatterListener the FormatterListener instance to receive the result
	 * @param function A pointer to the member function of FormatterListener to call
	 */
	void
	Union(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext,
			FormatterListener&		formatterListener,
			MemberFunctionPtr		function) const;

	/**
	 * Computes the union of its operands which must be node-sets.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @result the result of the union of node-set operands.
	 */
	void
	Union(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext,
			MutableNodeRefList&		result) const;

	/**
	 * Get a literal value.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @return an XObject object.
	 */
	const XObjectPtr
	literal(
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext) const;
  
	/**
	 * Get a literal value as a boolean.
	 *
	 * @param opPos The current position in the Op Map.
	 * @param theResult The value.
	 */
	void
	literal(
			OpCodeMapPositionType	opPos,
			bool&	                theResult) const;

	/**
	 * Get a literal value as a number.
	 *
	 * @param opPos The current position in the Op Map.
	 * @param theResult The value.
	 */
	void
	literal(
			OpCodeMapPositionType	opPos,
			double&		            theResult) const;

	/**
	 * Get a literal value.  The value is appended to the
	 * supplied string.
	 *
	 * @param opPos The current position in the Op Map.
	 * @param theResult The string.
	 */
	void
	literal(
			OpCodeMapPositionType	opPos,
			XalanDOMString&		    theResult) const;

	/**
	 * Get a literal value.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @return The result as a double.
	 */
	void
	literal(
			OpCodeMapPositionType	opPos,
			FormatterListener&	    formatterListener,
			MemberFunctionPtr	    function) const;

	/**
	 * Get the value of a variable.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @return an XObject object.
	 */
	const XObjectPtr
	variable(
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Execute an expression as a group.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @return arg.
	 */
	const XObjectPtr
	group(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext) const
	{
		return executeMore(context, opPos + 2, executionContext);
	}

	/**
	 * Execute an expression as a group.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @param theResult The result of the execution
	 */
	void
	group(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext,
			bool&					theResult) const
	{
		executeMore(context, opPos + 2, executionContext, theResult);
	}

	/**
	 * Execute an expression as a group.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @param theResult The result of the execution
	 */
	void
	group(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext,
			double&					theResult) const
	{
		executeMore(context, opPos + 2, executionContext, theResult);
	}

	/**
	 * Execute an expression as a group.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @param theResult The result of the execution
	 */
	void
	group(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext,
			XalanDOMString&			theResult) const
	{
		executeMore(context, opPos + 2, executionContext, theResult);
	}

	/**
	 * Execute an expression as a group.
	 *
	 * @param context The current source tree context node.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @param formatterListener the FormatterListener instance to receive the result
	 * @param function A pointer to the member function of FormatterListener to call
	 */
	void
	group(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext,
			FormatterListener&		formatterListener,
			MemberFunctionPtr		function) const
	{
		executeMore(context, opPos + 2, executionContext, formatterListener, function);
	}

	/**
	 * Execute an expression as a group.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @param theResult The result of the execution
	 */
	void
	group(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext,
			MutableNodeRefList&		theResult) const
	{
		executeMore(context, opPos + 2, executionContext, theResult);
	}

	/**
	 * Get a literal value.
	 * @param opPos The current position in the Op Map.
	 * @return The result as a double.
	 */
	double
	numberlit(OpCodeMapPositionType	    opPos) const;

	/**
	 * Get a literal value.
	 * @param opPos The current position in the Op Map.
	 * @return The result as a double.
	 */
	const XObjectPtr
	numberlit(
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Get a literal value as a boolean.
	 *
	 * @param opPos The current position in the Op Map.
	 * @param theResult The string.
	 */
	void
	numberlit(
			OpCodeMapPositionType	opPos,
			bool&	                theResult) const;

	/**
	 * Get a literal value.  The value is appended to the
	 * supplied string.
	 *
	 * @param opPos The current position in the Op Map.
	 * @param theResult The string.
	 */
	void
	numberlit(
			OpCodeMapPositionType	opPos,
			XalanDOMString&		    theResult) const;

	/**
	 * Get a literal value.
	 *
	 * @param opPos The current position in the Op Map.
	 * @param formatterListener the FormatterListener instance to receive the result
	 * @param function A pointer to the member function of FormatterListener to call
	 */
	void
	numberlit(
			OpCodeMapPositionType	opPos,
			FormatterListener&	    formatterListener,
			MemberFunctionPtr	    function) const;

	/**
	 * Setup for and run an extension function.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @return the result of the function.
	 */
	const XObjectPtr
	runExtFunction(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Handle an extension function.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the Op Map.
	 * @param theNamespace The namespace of the function.
	 * @param functionName The name of the function.
	 * @param executionContext current execution context
	 * @return the result of the function.
	 */
	const XObjectPtr
	extfunction(
			XalanNode*								context,
			OpCodeMapPositionType					/* opPos */,
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
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @return the result of the function.
	 */
	const XObjectPtr
	runFunction(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Handle a built-in function.
	 * @param context The current source tree context node.
	 * @param funcID The function ID.
	 * @param argVec The arguments for the function.
	 * @param executionContext current execution context
	 * @return the result of the function.
	 */
	const XObjectPtr
	function(
			XalanNode*								context,
			OpCodeMapValueType						funcID,
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
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @return the result of the function.
	 */
	double
	functionCount(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Handle the built-in function "not".
	 *
	 * @param context The current source tree context node, which must not be 0.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @return the result of the function.
	 */
	bool
	functionNot(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext) const
	{
		assert(context != 0);

		return !functionBoolean(context, opPos, executionContext);
	}

	/**
	 * Handle the built-in function "boolean".
	 *
	 * @param context The current source tree context node, which must not be 0.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @return the result of the function.
	 */
	bool
	functionBoolean(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
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
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @return the result of the function.
	 */
	const XalanDOMString&
	functionName(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
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
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @return the result of the function.
	 */
	const XalanDOMString&
	functionLocalName(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
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
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @return the result of the function.
	 */
	double
	functionNumber(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
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
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @return the result of the function.
	 */
	double
	functionFloor(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext) const
	{
		return DoubleSupport::floor(functionNumber(context, opPos, executionContext));
	}

	/**
	 * Handle the built-in function "ceiling".
	 *
	 * @param context The current source tree context node, which must not be 0.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @return the result of the function.
	 */
	double
	functionCeiling(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext) const
	{
		return DoubleSupport::ceiling(functionNumber(context, opPos, executionContext));
	}

	/**
	 * Handle the built-in function "round".
	 *
	 * @param context The current source tree context node, which must not be 0.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @return the result of the function.
	 */
	double
	functionRound(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
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
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @return the result of the function.
	 */
	double
	functionStringLength(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Handle the built-in function "sum".
	 *
	 * @param context The current source tree context node.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @return the result of the function.
	 */
	double
	functionSum(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
			XPathExecutionContext&	executionContext) const;

	/**
	 * Get a numeric operand for an expression.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the Op Map.
	 * @param executionContext current execution context
	 * @return The value of the operand.
	 */
	double
	getNumericOperand(
			XalanNode*				context,
			OpCodeMapPositionType	opPos,
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

    OpCodeMapPositionType
    getInitialOpCodePosition() const
    {
#if defined(XALAN_XPATH_EXPRESSION_USE_ITERATORS)
        assert(m_expression.getOpCodeMapValue(0) == XPathExpression::eOP_XPATH);
#else
        assert(m_expression.getOpCodeMapValue(
                    m_expression.getInitialOpCodePosition()) == XPathExpression::eOP_XPATH);
#endif
        return m_expression.getInitialOpCodePosition() + 2;
    }

	eMatchScore
	locationPathPattern(
			XPathExecutionContext&	executionContext,
			XalanNode& 				context, 
			OpCodeMapPositionType 	opPos) const;

protected:

	void
	step(
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			OpCodeMapPositionType 	opPos,
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
			OpCodeMapPositionType 	opPos,
			eMatchScore& 			scoreHolder) const;

	OpCodeMapPositionType
	findNodeSet(
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			OpCodeMapPositionType 	opPos,
			OpCodeMapValueType 	    stepType,
			MutableNodeRefList& 	subQueryResults) const;

	OpCodeMapPositionType
	findRoot(
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			OpCodeMapPositionType 	opPos,
			OpCodeMapValueType 		stepType,
			MutableNodeRefList& 	subQueryResults) const;

	OpCodeMapPositionType
	findParent(
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			OpCodeMapPositionType 	opPos,
			OpCodeMapValueType 		stepType,
			MutableNodeRefList& 	subQueryResults) const;

	OpCodeMapPositionType
	findSelf(
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			OpCodeMapPositionType 	opPos,
			OpCodeMapValueType 		stepType,
			MutableNodeRefList& 	subQueryResults) const;

	OpCodeMapPositionType
	findAncestors(
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			OpCodeMapPositionType 	opPos,
			OpCodeMapValueType 		stepType,
			MutableNodeRefList& 	subQueryResults) const;

	OpCodeMapPositionType
	findAncestorsOrSelf(
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			OpCodeMapPositionType 	opPos,
			OpCodeMapValueType 		stepType,
			MutableNodeRefList& 	subQueryResults) const;

	OpCodeMapPositionType
	findAttributes(
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			OpCodeMapPositionType 	opPos,
			OpCodeMapValueType 		stepType,
			MutableNodeRefList& 	subQueryResults) const;

	OpCodeMapPositionType
	findChildren(
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			OpCodeMapPositionType 	opPos,
			OpCodeMapValueType 		stepType,
			MutableNodeRefList& 	subQueryResults) const;

	OpCodeMapPositionType
	findDescendants(
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			OpCodeMapPositionType 	opPos,
			OpCodeMapValueType 		stepType,
			MutableNodeRefList& 	subQueryResults) const;

	OpCodeMapPositionType
	findFollowing(
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			OpCodeMapPositionType 	opPos,
			OpCodeMapValueType 		stepType,
			MutableNodeRefList& 	subQueryResults) const;

	OpCodeMapPositionType
	findFollowingSiblings(
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			OpCodeMapPositionType 	opPos,
			OpCodeMapValueType 		stepType,
			MutableNodeRefList& 	subQueryResults) const;

	OpCodeMapPositionType
	findPreceeding(
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			OpCodeMapPositionType 	opPos,
			OpCodeMapValueType 		stepType,
			MutableNodeRefList& 	subQueryResults) const;

	OpCodeMapPositionType
	findPreceedingSiblings(
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			OpCodeMapPositionType 	opPos,
			OpCodeMapValueType 		stepType,
			MutableNodeRefList& 	subQueryResults) const;

	OpCodeMapPositionType
	findNamespace(
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			OpCodeMapPositionType 	opPos,
			OpCodeMapValueType 		stepType,
			MutableNodeRefList& 	subQueryResults) const;

	OpCodeMapPositionType
	findNodesOnUnknownAxis(
			XPathExecutionContext&	executionContext,
			XalanNode* 				context, 
			OpCodeMapPositionType 	opPos,
			OpCodeMapValueType 		stepType,
			MutableNodeRefList& 	subQueryResults) const;

#if !defined(NDEBUG)
	eMatchScore
	nodeTest(
			XPathExecutionContext&	executionContext,
			XalanNode* 				context,
			XalanNode::NodeType		nodeType,
			OpCodeMapPositionType 	opPos,
			OpCodeMapValueType 	    argLen,
			OpCodeMapValueType 		stepType) const;
#endif

	OpCodeMapPositionType
	predicates(
			XPathExecutionContext&	executionContext,
			OpCodeMapPositionType 	opPos,
			MutableNodeRefList& 	subQueryResults) const;

	eMatchScore
	handleFoundIndex(
			XPathExecutionContext&	executionContext,
			XalanNode* 				localContext,
			OpCodeMapPositionType 	startOpPos) const;

	eMatchScore
	handleFoundIndexPositional(
			XPathExecutionContext&	executionContext,
			XalanNode* 				localContext,
			OpCodeMapPositionType 	startOpPos) const;

private:

	void
	unknownOpCodeError(
			XalanNode*				context,
			XPathExecutionContext&	executionContext,
			OpCodeMapPositionType	opPos) const;

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
	static FunctionTableType		s_functions;

	static const XalanDOMString     s_emptyString;
};



XALAN_CPP_NAMESPACE_END



#endif	// XPATH_HEADER_GUARD_1357924680
