/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2001 The Apache Software Foundation.  All rights 
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
#if !defined(XPATHEVALUATOR_HEADER_GUARD_1357924680)
#define XPATHEVALUATOR_HEADER_GUARD_1357924680



#include <XPath/XPathDefinitions.hpp>



#if defined(XALAN_AUTO_PTR_REQUIRES_DEFINITION)
#include <XPath/XObjectFactory.hpp>
#include <XPath/XPathExecutionContextDefault.hpp>
#endif



#include <Include/XalanAutoPtr.hpp>



#include <XalanDOM/XalanDOMString.hpp>



#if !defined(XALAN_AUTO_PTR_REQUIRES_DEFINITION)
class XObjectFactory;
class XPathExecutionContextDefault;
#endif



class DOMSupport;
class NodeRefList;
class PrefixResolver;
class XalanNode;
class XalanElement;
class XObjectPtr;
class XPathEnvSupport;



class XALAN_XPATH_EXPORT XPathEvaluator
{
public:

	// Static initializer to be called before any instances are
	// created.
	static void
	initialize();

	// Static terminator to be called when after all instances
	// are destroyed.
	static void
	terminate();


	XPathEvaluator();

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
	 * Evaluate the supplied XPath expression, within the given context.  If
	 * the expression doesn't select a node, an empty list is returned.
	 *
	 * @param domSupport An instance of the corresponding DOMSupport-derived for the DOM implementation being used.
	 * @param contextNode The source tree context node
	 * @param xpathString The XPath expression to evaluate
	 * @param namespaceNode A node to use for namespace prefix resolution.
	 * @return A list of selected nodes.
	 */
	NodeRefList
	selectNodeList(
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
	NodeRefList
	selectNodeList(
			DOMSupport&				domSupport,
			XalanNode*				contextNode,
			const XalanDOMChar*		xpathString,
			const PrefixResolver&	prefixResolver);

	/**
	 * Evaluate the supplied XPath expression, within the given context.  The
	 * result is returned as a generalized object.  The object will be
	 * destroyed when the returned when the user's copy of the returned
	 * XObjectPtr goes out of scope, or when the XalanXPathEvaluator instance
	 * is reset or goes out of scope.  The user's XObjectPtr copy _must_ no
	 * longer be in scope when the XalanXPathEvaluator instance is reset or
	 * out of scope.
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
	 * destroyed when the returned when the user's copy of the returned
	 * XObjectPtr goes out of scope, or when the XalanXPathEvaluator instance
	 * is reset or goes out of scope.  The user's XObjectPtr copy _must_ no
	 * longer be in scope when the XalanXPathEvaluator instance is reset or
	 * out of scope.
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

private:

	/**
	 * A helper function to evaluate the supplied XPath expression, within
	 * the given context.
	 *
	 * @param domSupport An instance of the corresponding DOMSupport-derived for the DOM implementation being used.
	 * @param contextNode The source tree context node
	 * @param xpathString The XPath expression to evaluate
	 * @param prefixResolver A prefix resolver instance to use for namespace prefix resolution.
	 * @param doEvaluate The XPathEnvSupport instance to use.
	 * @return The result of evaluting the XPath expression.
	 */
	XObjectPtr
	evaluate(
			DOMSupport&				domSupport,
			XalanNode*				contextNode,
			const XalanDOMChar*		xpathString,
			const PrefixResolver&	prefixResolver,
			XPathEnvSupport&		doEvaluate);

	// Data members...
	const XalanAutoPtr<XObjectFactory>					m_xobjectFactory;

	const XalanAutoPtr<XPathExecutionContextDefault>	m_executionContext;
};



#endif	// XPATHEVALUATOR_HEADER_GUARD_1357924680
