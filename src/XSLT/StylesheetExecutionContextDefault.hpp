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
#if !defined(STYLESHEETEXECUTIONCONTEXTDEFAULT_HEADER_GUARD_1357924680)
#define STYLESHEETEXECUTIONCONTEXTDEFAULT_HEADER_GUARD_1357924680



// Base class include file.
#include <XSLT/StylesheetExecutionContext.hpp>



#include <memory>
#include <vector>



class XPathProcessor;
class XPathSupport;
class XObjectFactory;
class XSLTEngineImpl;



//
// An class which provides support for executing stylesheets.
//
class XALAN_XSLT_EXPORT StylesheetExecutionContextDefault : public StylesheetExecutionContext
{
public:

	StylesheetExecutionContextDefault(
			XPathExecutionContext&	xpathExecutionContext,
			XSLTEngineImpl&			xsltProcessor);

	virtual
	~StylesheetExecutionContextDefault();

	// These interfaces are inherited from StylesheetExecutionContext...

	virtual const DOM_Node
	getParentOfNode(const DOM_Node&	theNode) const;

	virtual XPathExecutionContext&
	getXPathExecutionContext();

	virtual const NodeRefListBase&
	getContextNodeList() const;

	virtual void
	setContextNodeList(const NodeRefListBase&	theContextNodeList);

	virtual XObject*
	getTopLevelVariable(const DOMString&	theName) const;

	virtual bool
	getQuietConflictWarnings() const;

	virtual const DOM_Document
	getRootDocument() const;

	virtual void
	setRootDocument(const DOM_Document&		theDocument);

	virtual const DOM_Document
	createDocument() const;

	virtual void
	setStylesheetRoot(StylesheetRoot*	theStylesheet);

	virtual void
	resetCurrentState(
			const DOM_Node&		sourceTree,
			const DOM_Node&		xmlNode);

	virtual bool
	doDiagnosticsOutput() const;

	virtual void
	diag(const DOMString&	theString);

	virtual void
	pushTime(const void*	theKey);

	virtual void
	displayDuration(
			const DOMString&	theMessage,
			const void*			theKey);

	virtual const AttributeList&
	getPendingAttributes() const;

	virtual DOMString
	getPendingElementName() const;

	virtual void
	setPendingAttributes(const AttributeList&	pendingAttributes);

	virtual void
	replacePendingAttribute(
			const XMLCh*	theName,
			const XMLCh*	theNewType,
			const XMLCh*	theNewValue);

	virtual void
	setPendingElementName(const DOMString&	elementName);

	virtual void
	addResultAttribute(
			const DOMString&	aname,
			const DOMString&	value);

	virtual void
	copyNamespaceAttributes(
			const DOM_Node&		src,
			bool				srcIsStylesheetTree);

	virtual DOMString
	getResultPrefixForNamespace(
			const DOMString&	theNamespace) const;

	virtual DOMString
	getResultNamespaceForPrefix(
			const DOMString&	thePrefix) const;

	virtual DOMString
	getUniqueNameSpaceValue() const;

	virtual FormatterListener*
	getFormatterListener() const;

	virtual void
	setFormatterListener(FormatterListener*		flistener);

	virtual int
	getIndent() const;

	virtual XObject*
	executeXPath(
			const DOMString&		str,
			const DOM_Node&			contextNode,
			const PrefixResolver&	resolver);

	virtual XObject*
	executeXPath(
			const DOMString&	str,
			const DOM_Node&		contextNode,
			const DOM_Element&	resolver);

	virtual XPath*
	createMatchPattern(
			const DOMString&		str,
			const PrefixResolver&	resolver);

	virtual const DOMString
	evaluateAttrVal(
			const DOM_Node&		contextNode,
			const DOM_Element&	namespaceContext,
			const DOMString&	stringedValue);

	virtual void
	pushVariable(
			const QName&		name,
			XObject*			var,
			const DOM_Node&		element);

	virtual void
	pushContextMarker(
			const DOM_Node&		caller,
			const DOM_Node&		sourceNode);

	/**
	 * Pop the current context from the current context stack.
	 */
	virtual void
	popCurrentContext();

	virtual void
	resolveTopLevelParams();

	virtual void
	clearTopLevelParams();

	virtual	void
	pushParams(
			const ElemTemplateElement&	xslCallTemplateElement,
			const DOM_Node&				sourceTree, 
			const DOM_Node&				sourceNode,
			const QName&				mode,
			const DOM_Node&				targetTemplate);

	virtual XObject*
	getParamVariable(const QName&	theName) const;

	virtual void
	startDocument();

	virtual void
	endDocument();

	virtual void
	characters(
			const XMLCh*	ch,
			unsigned int	start,
			unsigned int	length);

	virtual void
	charactersRaw(
			const XMLCh*	ch,
			unsigned int	start,
			unsigned int	length);

	virtual void
	comment(
			const XMLCh*	data);

	virtual void
	processingInstruction(
			const XMLCh*	target,
			const XMLCh*	data);

	virtual void
	startElement(
			const XMLCh*	name);

	virtual void
	endElement(
			const XMLCh*	name);

	virtual void
	flushPending();

	virtual void
	cloneToResultTree(
			const DOM_Node&			node, 
			bool					isLiteral,
			bool					overrideStrip,
			bool					shouldCloneAttributes);

	virtual XObject*
	createXResultTreeFrag(
			const ElemTemplateElement&		templateChild,
			const DOM_Node&					sourceTree,
			const DOM_Node&					sourceNode);

	virtual XObject*
	createXResultTreeFrag(
			const ElemTemplateElement&		templateChild,
			const DOM_Node&					sourceTree,
			const DOM_Node&					sourceNode,
			const QName&					mode);

	/**
	 * Given a result tree fragment, walk the tree and
	 * output it to the result stream.
	 */
	virtual void
	outputResultTreeFragment(const XObject&		theTree);

	virtual const DOMString&
	getXSLNameSpaceURL() const;

	virtual const DOMString&
	getXSLT4JNameSpaceURL() const;

	virtual bool
	isTraceSelect() const;

	virtual void
	traceSelect(
			const DOM_Element&		theTemplate,
			const NodeRefListBase&	nl) const;

	virtual bool
	findOnElementRecursionStack(
			const ElemTemplateElement*	theElement) const;

	virtual void
	pushOnElementRecursionStack(
			const ElemTemplateElement*	theElement);

	virtual const ElemTemplateElement*
	popElementRecursionStack();

	// These interfaces are inherited from ExecutionContext...

	virtual void
	error(
			const DOMString&	msg,
			const DOM_Node& 	sourceNode = DOM_Node(),
			const DOM_Node&		styleNode = DOM_Node()) const;

	virtual void
	warn(
			const DOMString&	msg,
			const DOM_Node& 	sourceNode = DOM_Node(),
			const DOM_Node&		styleNode = DOM_Node()) const;

	virtual void
	message(
			const DOMString&	msg,
			const DOM_Node& 	sourceNode = DOM_Node(),
			const DOM_Node&		styleNode = DOM_Node()) const;

private:

	XPathExecutionContext&			m_xpathExecutionContext;

	// $$ ToDo: Try to remove this dependency, and rely only on XSLTProcessor...
	XSLTEngineImpl&					m_xsltProcessor;

	typedef std::vector<const ElemTemplateElement*>		ElementRecursionStackType;

	ElementRecursionStackType		m_elementRecursionStack;

	const PrefixResolver*			m_prefixResolver;

	StylesheetRoot*					m_stylesheetRoot;
};



#endif	// STYLESHEETEXECUTIONCONTEXTDEFAULT_HEADER_GUARD_1357924680
