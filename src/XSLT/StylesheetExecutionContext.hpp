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
#if !defined(STYLESHEETEXECUTIONCONTEXT_HEADER_GUARD_1357924680)
#define STYLESHEETEXECUTIONCONTEXT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XSLT/XSLTDefinitions.hpp>



#include <dom/DOM_Node.hpp>
#include <dom/DOM_Document.hpp>
#include <dom/DOMString.hpp>



// Base class header file...
#include <PlatformSupport/ExecutionContext.hpp>



class AttributeList;
class DOM_Element;
class DOMString;
class ElemTemplateElement;
class FormatterListener;
class PrefixResolver;
class NodeRefListBase;
class QName;
class Stylesheet;
class StylesheetRoot;
class XPath;
class XPathExecutionContext;
class XObject;



//
// An abstract class which provides support for executing stylesheets.
//
class XALAN_XSLT_EXPORT StylesheetExecutionContext : public ExecutionContext
{
public:

	explicit
	StylesheetExecutionContext();

	virtual
	~StylesheetExecutionContext();

	// These interfaces are inherited from ExecutionContext...

	/**
	 * Tell the user of an error, and probably throw an 
	 * exception.
	 */
	virtual void
	error(
			const DOMString&	msg,
			const DOM_Node& 	sourceNode = DOM_Node(),
			const DOM_Node&		styleNode = DOM_Node()) const = 0;

	/**
	 * Tell the user of an warning, and probably throw an 
	 * exception.
	 */
	virtual void
	warn(
			const DOMString&	msg,
			const DOM_Node& 	sourceNode = DOM_Node(),
			const DOM_Node&		styleNode = DOM_Node()) const = 0;

	/**
	 * Output a message.
	 */
	virtual void
	message(
			const DOMString&	msg,
			const DOM_Node& 	sourceNode = DOM_Node(),
			const DOM_Node&		styleNode = DOM_Node()) const = 0;

	// These interfaces are new...

	virtual const DOM_Node
	getParentOfNode(const DOM_Node&	theNode) const = 0;

	virtual XPathExecutionContext&
	getXPathExecutionContext() = 0;

	virtual const NodeRefListBase&
	getContextNodeList() const = 0;

	virtual void
	setContextNodeList(const NodeRefListBase&	theContextNodeList) = 0;

	virtual XObject*
	getTopLevelVariable(const DOMString&	theName) const = 0;

	virtual bool
	getQuietConflictWarnings() const = 0;

	virtual const DOM_Document
	getRootDocument() const = 0;

	virtual void
	setRootDocument(const DOM_Document&		theDocument) = 0;

	virtual const DOM_Document
	createDocument() const = 0;

	virtual void
	setStylesheetRoot(StylesheetRoot*	theStylesheet) = 0;

	virtual void
	resetCurrentState(
			const DOM_Node&		sourceTree,
			const DOM_Node&		xmlNode) = 0;

	virtual bool
	doDiagnosticsOutput() const = 0;

	virtual void
	diag(const DOMString&	theString) = 0;

	virtual void
	pushTime(const void*	theKey) = 0;

	virtual void
	displayDuration(
			const DOMString&	theMessage,
			const void*			theKey) = 0;

	virtual const AttributeList&
	getPendingAttributes() const = 0;

	virtual DOMString
	getPendingElementName() const = 0;

	virtual void
	setPendingAttributes(const AttributeList&	pendingAttributes) = 0;

	virtual void
	replacePendingAttribute(
			const XMLCh*	theName,
			const XMLCh*	theNewType,
			const XMLCh*	theNewValue) = 0;

	virtual void
	setPendingElementName(const DOMString&	elementName) = 0;

	virtual void
	addResultAttribute(
			const DOMString&	aname,
			const DOMString&	value) = 0;

	virtual void
	copyNamespaceAttributes(
			const DOM_Node&		src,
			bool				srcIsStylesheetTree) = 0;

	virtual DOMString
	getResultPrefixForNamespace(
			const DOMString&	theNamespace) const = 0;

	virtual DOMString
	getResultNamespaceForPrefix(
			const DOMString&	thePrefix) const = 0;

	virtual DOMString
	getUniqueNameSpaceValue() const = 0;

	/**
	 * Get the current formatter listener.
	 */
	virtual FormatterListener*
	getFormatterListener() const = 0;

	/**
	 * Set the current formatter listener.
	 */
	virtual void
	setFormatterListener(FormatterListener*		flistener) = 0;

	virtual int
	getIndent() const = 0;

	/**
	 * Execute an XPath and return the resulting XObject.
	 * The lifetime of this XObject is _not_ necessarily that
	 * of the Stylesheet.
	 */
	virtual XObject*
	executeXPath(
			const DOMString&		str,
			const DOM_Node&			contextNode,
			const PrefixResolver&	resolver) = 0;

	/**
	 * Execute an XPath and return the resulting XObject.
	 * The lifetime of this XObject is _not_ necessarily that
	 * of the Stylesheet.
	 */
	virtual XObject*
	executeXPath(
			const DOMString&	str,
			const DOM_Node&		contextNode,
			const DOM_Element&	resolver) = 0;

	/**
	 * Create and initialize an xpath and return it.
	 * This is to be used to create an XPath that is
	 * is only used during execution.
	 */
	virtual XPath*
	createMatchPattern(
			const DOMString&		str,
			const PrefixResolver&	resolver) = 0;

	virtual const DOMString
	evaluateAttrVal(
			const DOM_Node&		contextNode,
			const DOM_Element&	namespaceContext,
			const DOMString&	stringedValue) = 0;

	virtual void
	pushVariable(
			const QName&		name,
			XObject*			var,
			const DOM_Node&		element) = 0;

	virtual void
	pushContextMarker(
			const DOM_Node&		caller,
			const DOM_Node&		sourceNode) = 0;

	/**
	 * Pop the current context from the current context stack.
	 */
	virtual void
	popCurrentContext() = 0;

	virtual void
	resolveTopLevelParams() = 0;

	virtual void
	clearTopLevelParams() = 0;

	virtual	void
	pushParams(
			const ElemTemplateElement&	xslCallTemplateElement,
			const DOM_Node&				sourceTree, 
			const DOM_Node&				sourceNode,
			const QName&				mode,
			const DOM_Node&				targetTemplate) = 0;

	virtual XObject*
	getParamVariable(const QName&	theName) const = 0;

	virtual void
	startDocument() = 0;

	virtual void
	endDocument() = 0;

	virtual void
	characters(
			const XMLCh*	ch,
			unsigned int	start,
			unsigned int	length) = 0;

	virtual void
	charactersRaw(
			const XMLCh*	ch,
			unsigned int	start,
			unsigned int	length) = 0;

	virtual void
	comment(
			const XMLCh*	data) = 0;

	virtual void
	processingInstruction(
			const XMLCh*	target,
			const XMLCh*	data) = 0;

	virtual void
	startElement(
			const XMLCh*	name) = 0;

	virtual void
	endElement(
			const XMLCh*	name) = 0;

	virtual void
	flushPending() = 0;

	virtual void
	cloneToResultTree(
			const DOM_Node&			node, 
			bool					isLiteral,
			bool					overrideStrip,
			bool					shouldCloneAttributes) = 0;

	virtual XObject*
	createXResultTreeFrag(
			const ElemTemplateElement&		templateChild,
			const DOM_Node&					sourceTree,
			const DOM_Node&					sourceNode) = 0;

	virtual XObject*
	createXResultTreeFrag(
			const ElemTemplateElement&		templateChild,
			const DOM_Node&					sourceTree,
			const DOM_Node&					sourceNode,
			const QName&					mode) = 0;

	/**
	 * Given a result tree fragment, walk the tree and
	 * output it to the result stream.
	 */
	virtual void
	outputResultTreeFragment(const XObject&		theTree) = 0;

	virtual const DOMString&
	getXSLNameSpaceURL() const = 0;

	virtual const DOMString&
	getXSLT4JNameSpaceURL() const = 0;

	virtual bool
	isTraceSelect() const = 0;

	virtual void
	traceSelect(
			const DOM_Element&		theTemplate,
			const NodeRefListBase&	nl) const = 0;

	virtual bool
	findOnElementRecursionStack(
			const ElemTemplateElement*	theElement) const = 0;

	virtual void
	pushOnElementRecursionStack(
			const ElemTemplateElement*	theElement) = 0;

	virtual const ElemTemplateElement*
	popElementRecursionStack() = 0;

	class ElementRecursionStackPusher
	{
	public:

		ElementRecursionStackPusher(
					StylesheetExecutionContext&		executionContext,
					const ElemTemplateElement*		element) :
			m_executionContext(executionContext)
		{
			m_executionContext.pushOnElementRecursionStack(element);
		}

		~ElementRecursionStackPusher()
		{
			m_executionContext.popElementRecursionStack();
		}

	private:

		StylesheetExecutionContext&			m_executionContext;
	};
};






#endif	// STYLESHEETEXECUTIONCONTEXT_HEADER_GUARD_1357924680
