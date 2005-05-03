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
#if !defined(XPATHEVALUATOR_HEADER_GUARD_1357924680)
#define XPATHEVALUATOR_HEADER_GUARD_1357924680



#include <xalanc/XPath/XPathDefinitions.hpp>



#if defined(XALAN_AUTO_PTR_REQUIRES_DEFINITION)
#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/XPathFactoryDefault.hpp>
#include <xalanc/XPath/XPathConstructionContextDefault.hpp>
#include <xalanc/XPath/XPathExecutionContextDefault.hpp>
#endif



#include <xalanc/Include/XalanMemMgrAutoPtr.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



XALAN_CPP_NAMESPACE_BEGIN



#if !defined(XALAN_AUTO_PTR_REQUIRES_DEFINITION)
class XObjectFactory;
class XPathFactoryDefault;
class XPathConstructionContextDefault;
class XPathExecutionContextDefault;
#endif



class DOMSupport;
class NodeRefList;
class PrefixResolver;
class XalanNode;
class XalanElement;
class XObjectPtr;
class XPath;
class XPathEnvSupport;



class XALAN_XPATH_EXPORT XPathEvaluator
{
public:

	// Static initializer to be called before any instances are
	// created.  The call is _not_ thread-safe, you must only call
	// it once, unless you have called terminate previously, and
	// you want to re-initialize the library.
	static void
	initialize(MemoryManagerType& theManager XALAN_DEFAULT_MEMMGR);

	// Static terminator to be called after all instances
	// are destroyed.  The call is _not_ thread-safe.  Once
	// terminated, you can call initialize() again, to
	// re-initialize the library.
	static void
	terminate();


	XPathEvaluator(MemoryManagerType& theManager XALAN_DEFAULT_MEMMGR);

	~XPathEvaluator();

	/**
	 * Evaluate the supplied XPath expression, within the given context.  If
	 * the expression doesn't select a node, 0 is returned.  If it selects
	 * more than one node, only the first is returned.
	 *
	 * @param domSupport An instance of the corresponding DOMSupport-derived for the DOM implementation being used.
	 * @param contextNode The source tree context node
	 * @param xpathString The XPath expression to evaluate
	 * @param namespaceNode A node to use for namespace prefix resolution.
	 * @return A pointer to the node selected by the expression, if any.
	 */
	XalanNode*
	selectSingleNode(
			DOMSupport&				domSupport,
			XalanNode*				contextNode,
			const XalanDOMChar*		xpathString,
			const XalanElement*		namespaceNode = 0);

	/**
	 * Evaluate the supplied XPath expression, within the given context.  If
	 * the expression doesn't select a node, 0 is returned.  If it selects
	 * more than one node, only the first is returned.
	 *
	 * @param domSupport An instance of the corresponding DOMSupport-derived for the DOM implementation being used.
	 * @param contextNode The source tree context node
	 * @param xpathString The XPath expression to evaluate
	 * @param prefixResolver A prefix resolver instance to use for namespace prefix resolution.
	 * @return A pointer to the node selected by the expression, if any.
	 */
	XalanNode*
	selectSingleNode(
			DOMSupport&				domSupport,
			XalanNode*				contextNode,
			const XalanDOMChar*		xpathString,
			const PrefixResolver&	prefixResolver);

	/**
	 * Evaluate the supplied XPath, within the given context.  If
	 * the expression doesn't select a node, 0 is returned.  If it selects
	 * more than one node, only the first is returned.
	 *
	 * @param domSupport An instance of the corresponding DOMSupport-derived for the DOM implementation being used.
	 * @param contextNode The source tree context node
	 * @param xpath A reference to a compiled XPath expression.
	 * @param namespaceNode A node to use for namespace prefix resolution.
	 * @return A pointer to the node selected by the expression, if any.
	 */
	XalanNode*
	selectSingleNode(
			DOMSupport&				domSupport,
			XalanNode*				contextNode,
			const XPath&			xpath,
			const XalanElement*		namespaceNode = 0);

	/**
	 * Evaluate the supplied XPath, within the given context.  If
	 * the expression doesn't select a node, 0 is returned.  If it selects
	 * more than one node, only the first is returned.
	 *
	 * @param domSupport An instance of the corresponding DOMSupport-derived for the DOM implementation being used.
	 * @param contextNode The source tree context node
	 * @param xpath A reference to a compiled XPath expression.
	 * @param prefixResolver A prefix resolver instance to use for namespace prefix resolution.
	 * @return A pointer to the node selected by the expression, if any.
	 */
	XalanNode*
	selectSingleNode(
			DOMSupport&				domSupport,
			XalanNode*				contextNode,
			const XPath&			xpath,
			const PrefixResolver&	prefixResolver);

	/**
	 * Evaluate the supplied XPath expression, within the given context.  If
	 * the expression doesn't select a node, an empty list is returned.
	 *
	 * @param domSupport An instance of the corresponding DOMSupport-derived for the DOM implementation being used.
	 * @param contextNode The source tree context node
	 * @param xpathString The XPath expression to evaluate
	 * @param namespaceNode A node to use for namespace prefix resolution.
	 * @return A list of selected nodes.
	 */
	NodeRefList&
	selectNodeList(
            NodeRefList&            result,
			DOMSupport&				domSupport,
			XalanNode*				contextNode,
			const XalanDOMChar*		xpathString,
			const XalanElement*		namespaceNode = 0);

	/**
	 * Evaluate the supplied XPath expression, within the given context.  If
	 * the expression doesn't select a node, an empty list is returned.
	 *
	 * @param domSupport An instance of the corresponding DOMSupport-derived for the DOM implementation being used.
	 * @param contextNode The source tree context node
	 * @param xpathString The XPath expression to evaluate
	 * @param prefixResolver A prefix resolver instance to use for namespace prefix resolution.
	 * @return A list of selected nodes.
	 */
	NodeRefList&
	selectNodeList(
            NodeRefList&            result,
			DOMSupport&				domSupport,
			XalanNode*				contextNode,
			const XalanDOMChar*		xpathString,
			const PrefixResolver&	prefixResolver);

	/**
	 * Evaluate the supplied XPath, within the given context.  If
	 * the expression doesn't select a node, an empty list is returned.
	 *
	 * @param domSupport An instance of the corresponding DOMSupport-derived for the DOM implementation being used.
	 * @param contextNode The source tree context node
	 * @param xpath A reference to a compiled XPath expression.
	 * @param namespaceNode A node to use for namespace prefix resolution.
	 * @return A list of selected nodes.
	 */
	NodeRefList&
	selectNodeList(
            NodeRefList&            result,
			DOMSupport&				domSupport,
			XalanNode*				contextNode,
			const XPath&			xpath,
			const XalanElement*		namespaceNode = 0);

	/**
	 * Evaluate the supplied XPath, within the given context.  If
	 * the expression doesn't select a node, an empty list is returned.
	 *
	 * @param domSupport An instance of the corresponding DOMSupport-derived for the DOM implementation being used.
	 * @param contextNode The source tree context node
	 * @param xpath A reference to a compiled XPath expression.
	 * @param prefixResolver A prefix resolver instance to use for namespace prefix resolution.
	 * @return A list of selected nodes.
	 */
	NodeRefList&
	selectNodeList(
            NodeRefList&            result,
			DOMSupport&				domSupport,
			XalanNode*				contextNode,
			const XPath&			xpath,
			const PrefixResolver&	prefixResolver);

	/**
	 * Evaluate the supplied XPath expression, within the given context.  The
	 * result is returned as a generalized object.  The object will be
	 * destroyed when the user's copy of the returned XObjectPtr goes out of
     * scope, or when the XPathEvaluator goes out of scope or another expression
     * is evaluated.
	 *
	 * The user's XObjectPtr copy _must_ no longer be in scope when the XPathEvaluator
	 * instance goes out of scope, or another expression is evaluated.
	 *
	 * @param domSupport An instance of the corresponding DOMSupport-derived for the DOM implementation being used.
	 * @param contextNode The source tree context node
	 * @param xpathString The XPath expression to evaluate
	 * @param namespaceNode A node to use for namespace prefix resolution.
	 * @return The result of evaluting the XPath expression.
	 */
	XObjectPtr
	evaluate(
			DOMSupport&				domSupport,
			XalanNode*				contextNode,
			const XalanDOMChar*		xpathString,
			const XalanElement*		namespaceNode = 0);

	/**
	 * Evaluate the supplied XPath expression, within the given context.  The
	 * result is returned as a generalized object.  The object will be
	 * destroyed when the user's copy of the returned XObjectPtr goes out of
     * scope, or when the XPathEvaluator goes out of scope or another expression
     * is evaluated.
	 *
	 * The user's XObjectPtr copy _must_ no longer be in scope when the XPathEvaluator
	 * instance goes out of scope, or another expression is evaluated.
	 *
	 * @param domSupport An instance of the corresponding DOMSupport-derived for the DOM implementation being used.
	 * @param contextNode The source tree context node
	 * @param xpathString The XPath expression to evaluate
	 * @param prefixResolver A prefix resolver instance to use for namespace prefix resolution.
	 * @return The result of evaluting the XPath expression.
	 */
	XObjectPtr
	evaluate(
			DOMSupport&				domSupport,
			XalanNode*				contextNode,
			const XalanDOMChar*		xpathString,
			const PrefixResolver&	prefixResolver);

	/**
	 * Evaluate the supplied XPath expression, within the given context.  The
	 * result is returned as a generalized object.  The object will be
	 * destroyed when the user's copy of the returned XObjectPtr goes out of
     * scope, or when the XPathEvaluator goes out of scope or another expression
     * is evaluated.
	 *
	 * The user's XObjectPtr copy _must_ no longer be in scope when the XPathEvaluator
	 * instance goes out of scope, or another expression is evaluated.
	 *
	 * @param domSupport An instance of the corresponding DOMSupport-derived for the DOM implementation being used.
	 * @param contextNode The source tree context node
	 * @param xpath A reference to a compiled XPath expression.
	 * @param namespaceNode A node to use for namespace prefix resolution.
	 * @return The result of evaluting the XPath expression.
	 */
	XObjectPtr
	evaluate(
			DOMSupport&				domSupport,
			XalanNode*				contextNode,
			const XPath&			xpath,
			const XalanElement*		namespaceNode = 0);

	/**
	 * Evaluate the supplied XPath expression, within the given context.  The
	 * result is returned as a generalized object.  The object will be
	 * destroyed when the user's copy of the returned XObjectPtr goes out of
     * scope, or when the XPathEvaluator goes out of scope or another expression
     * is evaluated.
	 *
	 * The user's XObjectPtr copy _must_ no longer be in scope when the XPathEvaluator
	 * instance goes out of scope, or another expression is evaluated.
	 *
	 * @param domSupport An instance of the corresponding DOMSupport-derived for the DOM implementation being used.
	 * @param contextNode The source tree context node
	 * @param xpath A reference to a compiled XPath expression.
	 * @param prefixResolver A prefix resolver instance to use for namespace prefix resolution.
	 * @return The result of evaluting the XPath expression.
	 */
	XObjectPtr
	evaluate(
			DOMSupport&				domSupport,
			XalanNode*				contextNode,
			const XPath&			xpath,
			const PrefixResolver&	prefixResolver);

	/**
	 * Compile an XPath expression into an object which can be used multiple times.
	 * Call destroyXPath() when finished with the instance.  Otherwise, the object
	 * will be destroyed when the XPathEvaluator instance goes out of scope.  Since
	 * no PrefixResolver is supplied, the XPath expression may not contain any
	 * namespace prefixes.
	 *
	 * @param xpathString The XPath expression to evaluate
	 * @return A pointer to an XPath instance.
	 */
	XPath*
	createXPath(const XalanDOMChar*		xpathString);

	/**
	 * Compile an XPath expression into an object which can be used multiple times.
	 * Call destroyXPath() when finished with the instance.  Otherwise, the object
	 * will be destroyed when the XPathEvaluator instance goes out of scope.
	 *
	 * @param xpathString The XPath expression to evaluate
	 * @param domSupport An instance of the corresponding DOMSupport-derived for the DOM implementation being used.
	 * @param namespaceNode A node to use for namespace prefix resolution.
	 * @return A pointer to an XPath instance.
	 */
	XPath*
	createXPath(
			const XalanDOMChar*		xpathString,
			DOMSupport&				domSupport,
			const XalanElement*		namespaceNode);

	/**
	 * Compile an XPath expression into an object which can be used multiple times.
	 * Call destroyXPath() when finished with the instance.  Otherwise, the object
	 * will be destroyed when the XPathEvaluator instance goes out of scope.
	 *
	 * @param xpathString The XPath expression to evaluate
	 * @param prefixResolver A prefix resolver instance to use for namespace prefix resolution.
	 * @return A pointer to an XPath instance.
	 */
	XPath*
	createXPath(
			const XalanDOMChar*		xpathString,
			const PrefixResolver&	prefixResolver);

	/**
	 * Destory a compiled XPath instance.  The instance must have
	 * been created using createXPath().
	 *
	 * @param theXPath The XPath instance to destroy
	 * @return true if the instance was successfully destroyed
	 */
	bool
	destroyXPath(XPath*		theXPath);

private:

	/**
	 * A helper function to evaluate the supplied XPath expression, within
	 * the given context.
	 *
	 * @param domSupport An instance of the corresponding DOMSupport-derived for the DOM implementation being used.
	 * @param contextNode The source tree context node
	 * @param xpathString The XPath expression to evaluate
	 * @param prefixResolver A prefix resolver instance to use for namespace prefix resolution.
	 * @param envSupport The XPathEnvSupport instance to use.
	 * @return The result of evaluting the XPath expression.
	 */
	XObjectPtr
	evaluate(
			DOMSupport&				domSupport,
			XalanNode*				contextNode,
			const XalanDOMChar*		xpathString,
			const PrefixResolver&	prefixResolver,
			XPathEnvSupport&		envSupport);

	/**
	 * A helper function to evaluate the supplied XPath expression, within
	 * the given context.
	 *
	 * @param domSupport An instance of the corresponding DOMSupport-derived for the DOM implementation being used.
	 * @param contextNode The source tree context node
	 * @param xpath The XPath to evaluate
	 * @param prefixResolver A prefix resolver instance to use for namespace prefix resolution.
	 * @param envSupport The XPathEnvSupport instance to use.
	 * @return The result of evaluting the XPath expression.
	 */
	XObjectPtr
	evaluate(
			DOMSupport&				domSupport,
			XalanNode*				contextNode,
			const XPath&			xpath,
			const PrefixResolver&	prefixResolver,
			XPathEnvSupport&		envSupport);

	// Data members...
	const XalanMemMgrAutoPtr<XObjectFactory>					m_xobjectFactory;

	const XalanMemMgrAutoPtr<XPathFactoryDefault>				m_xpathFactory;

	const XalanMemMgrAutoPtr<XPathConstructionContextDefault>	m_constructionContext;

	const XalanMemMgrAutoPtr<XPathExecutionContextDefault>	    m_executionContext;

    MemoryManagerType&                                          m_memoryManager;
};



XALAN_CPP_NAMESPACE_END



#endif	// XPATHEVALUATOR_HEADER_GUARD_1357924680
