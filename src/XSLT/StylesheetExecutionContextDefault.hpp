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
 *
 * @author <a href="mailto:david_n_bertoni@lotus.com">David N. Bertoni</a>
 */
#if !defined(STYLESHEETEXECUTIONCONTEXTDEFAULT_HEADER_GUARD_1357924680)
#define STYLESHEETEXECUTIONCONTEXTDEFAULT_HEADER_GUARD_1357924680



// Base class include file.
#include <XSLT/StylesheetExecutionContext.hpp>



#include <memory>
#include <set>
#include <vector>



#include <XPath/XPathExecutionContextDefault.hpp>



class TextOutputStream;
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
			XSLTEngineImpl&			xsltProcessor,
			XPathEnvSupport&		theXPathEnvSupport,
			XPathSupport&			theXPathSupport,
			XObjectFactory&			theXObjectFactory,
			XalanNode*				theCurrentNode = 0,
			const NodeRefListBase&	theContextNodeList = NodeRefList(),
			const PrefixResolver*	thePrefixResolver = 0);

	virtual
	~StylesheetExecutionContextDefault();

	// These interfaces are inherited from StylesheetExecutionContext...

	virtual void
	reset();

	virtual XObject*
	getTopLevelVariable(const XalanDOMString&	theName) const;

	virtual bool
	getQuietConflictWarnings() const;

	virtual XalanDocument*
	getRootDocument() const;

	virtual void
	setRootDocument(XalanDocument*	theDocument);

	virtual XalanDocument*
	createDocument() const;

	virtual void
	setStylesheetRoot(const StylesheetRoot*		theStylesheet);

	virtual void
	resetCurrentState(
			XalanNode*	sourceTree,
			XalanNode*	xmlNode);

	virtual bool
	doDiagnosticsOutput() const;

	virtual void
	diag(const XalanDOMString&	theString);

	virtual void
	pushTime(const void*	theKey);

	virtual void
	displayDuration(
			const XalanDOMString&	theMessage,
			const void*				theKey);

	virtual const AttributeList&
	getPendingAttributes() const;

	virtual XalanDOMString
	getPendingElementName() const;

	virtual void
	setPendingAttributes(const AttributeList&	pendingAttributes);

	virtual void
	replacePendingAttribute(
			const XalanDOMChar*		theName,
			const XalanDOMChar*		theNewType,
			const XalanDOMChar*		theNewValue);

	virtual void
	setPendingElementName(const XalanDOMString&		elementName);

	virtual void
	addResultAttribute(
			const XalanDOMString&	aname,
			const XalanDOMString&	value);

	virtual void
	copyNamespaceAttributes(
			const XalanNode&	src,
			bool				srcIsStylesheetTree);

	virtual XalanDOMString
	getResultPrefixForNamespace(const XalanDOMString&	theNamespace) const;

	virtual XalanDOMString
	getResultNamespaceForPrefix(const XalanDOMString&	thePrefix) const;

	virtual XalanDOMString
	getUniqueNameSpaceValue() const;

	virtual FormatterListener*
	getFormatterListener() const;

	virtual void
	setFormatterListener(FormatterListener*		flistener);

	virtual bool
	getHasPendingStartDocument() const;

	virtual void
	setHasPendingStartDocument(bool	b);

	virtual bool
	getMustFlushPendingStartDocument() const;

	virtual void
	setMustFlushPendingStartDocument(bool	b);

	virtual int
	getIndent() const;

	virtual XObject*
	executeXPath(
			const XalanDOMString&	str,
			XalanNode*				contextNode,
			const PrefixResolver&	resolver);

	virtual XObject*
	executeXPath(
			const XalanDOMString&	str,
			XalanNode*				contextNode,
			const XalanElement&		resolver);

	virtual XPath*
	createMatchPattern(
			const XalanDOMString&	str,
			const PrefixResolver&	resolver);

	virtual const XalanDOMString
	evaluateAttrVal(
			XalanNode*				contextNode,
			const XalanElement&		namespaceContext,
			const XalanDOMString&	stringedValue);

	virtual void
	pushVariable(
			const QName&		name,
			XObject*			var,
			const XalanNode*	element);

	virtual void
	pushContextMarker(
			const XalanNode*	caller,
			const XalanNode*	sourceNode);

	virtual void
	popCurrentContext();

	virtual void
	resolveTopLevelParams();

	virtual void
	clearTopLevelParams();

	virtual	void
	pushParams(
			const ElemTemplateElement&	xslCallTemplateElement,
			XalanNode*					sourceTree, 
			XalanNode*					sourceNode,
			const QName&				mode,
			const XalanNode*			targetTemplate);

	virtual XObject*
	getParamVariable(const QName&	theName) const;

	virtual void
	pushElementFrame(const ElemTemplateElement*		elem);

	virtual void
	popElementFrame(const ElemTemplateElement*	elem);

	virtual int
	getCurrentStackFrameIndex() const;

	virtual void
	setCurrentStackFrameIndex(int currentStackFrameIndex = -1);

	virtual void
	startDocument();

	virtual void
	endDocument();

	virtual void
	startElement(const XalanDOMChar*	name);

	virtual void
	endElement(const XalanDOMChar*	name);

	virtual void
	characters(
			const XalanDOMChar*		ch,
			unsigned int			start,
			unsigned int			length);

	virtual void
	charactersRaw(
			const XalanDOMChar*		ch,
			unsigned int			start,
			unsigned int			length);

	virtual void
	comment(const XalanDOMChar*		data);

	virtual void
	processingInstruction(
			const XalanDOMChar*		target,
			const XalanDOMChar*		data);

	virtual void
	flushPending();

	virtual void
	cloneToResultTree(
			XalanNode&	node, 
			bool		isLiteral,
			bool		overrideStrip,
			bool		shouldCloneAttributes);

	virtual XObject*
	createXResultTreeFrag(
			const ElemTemplateElement&	templateChild,
			XalanNode*					sourceTree,
			XalanNode*					sourceNode);

	virtual XObject*
	createXResultTreeFrag(
			const ElemTemplateElement&	templateChild,
			XalanNode*					sourceTree,
			XalanNode*					sourceNode,
			const QName&				mode);

	virtual void
	outputResultTreeFragment(const XObject&		theTree);

	virtual const XalanDOMString&
	getXSLNameSpaceURL() const;

	virtual const XalanDOMString&
	getXalanXSLNameSpaceURL() const;

	virtual bool
	findOnElementRecursionStack(const ElemTemplateElement*	theElement) const;

	virtual void
	pushOnElementRecursionStack(const ElemTemplateElement*	theElement);

	virtual const ElemTemplateElement*
	popElementRecursionStack();

	virtual FormatterToXML*
	createFormatterToXML(
			Writer&					writer,
			const XalanDOMString&	version = XalanDOMString(),
			bool					doIndent = false,
			int						indent = 0,
			const XalanDOMString&	encoding = XalanDOMString(),
			const XalanDOMString&	mediaType = XalanDOMString(),
			const XalanDOMString&	doctypeSystem = XalanDOMString(),
			const XalanDOMString&	doctypePublic = XalanDOMString(),
			bool					xmlDecl = true,
			const XalanDOMString&	standalone = XalanDOMString());

	virtual FormatterToHTML*
	createFormatterToHTML(
			Writer&					writer,
			const XalanDOMString&	encoding = XalanDOMString(),
			const XalanDOMString&	mediaType = XalanDOMString(),
			const XalanDOMString&	doctypeSystem = XalanDOMString(),
			const XalanDOMString&	doctypePublic = XalanDOMString(),
			bool					doIndent = true,
			int						indent = 4,
			const XalanDOMString&	version = XalanDOMString(),
			const XalanDOMString&	standalone = XalanDOMString(),
			bool					xmlDecl = false);

	virtual FormatterToDOM*
	createFormatterToDOM(
			XalanDocument*			doc,
			XalanDocumentFragment*	docFrag = 0,
			XalanElement*			currentElement = 0);

	virtual FormatterToDOM*
	createFormatterToDOM(
			XalanDocument*	doc,
			XalanElement*	elem);

	virtual FormatterToText*
	createFormatterToText(Writer&	writer);


	virtual XalanNumberFormatAutoPtr
	createXalanNumberFormat();

	// A basic class to create XalanNumberFormat instances...
	class XALAN_XSLT_EXPORT XalanNumberFormatFactory
	{
	public:

		explicit
		XalanNumberFormatFactory();

		virtual
		~XalanNumberFormatFactory();

		virtual XalanNumberFormat*
		create();
	};

	static XalanNumberFormatFactory&
	getDefaultXalanNumberFormatFactory()
	{
		return s_defaultXalanNumberFormatFactory;
	}

	/**
	 * Static function to install a new XalanNumberFormatFactory.
	 * The caller owns the XalanNumberFormatFactory instance.
	 *
	 * @param a pointer to the new factory instance to use.
	 * @return a pointer to the old factory instance.
	 */
	// 
	static XalanNumberFormatFactory*
	installXalanNumberFormatFactory(XalanNumberFormatFactory*	theFactory);

	// Trace interface...
	virtual unsigned long
	getTraceListeners() const;

	virtual void
	fireGenerateEvent(const GenerateEvent&	ge);

	virtual void
	fireTraceEvent(const TracerEvent&	te);

	virtual void
	fireSelectEvent(const SelectionEvent&	se);

	virtual bool
	getTraceSelects() const;

	virtual void
	traceSelect(
			const XalanElement&		theTemplate,
			const NodeRefListBase&	nl) const;


	// These interfaces are inherited from XPathExecutionContext...

	virtual XalanNode*
	getCurrentNode() const;

	virtual void
	setCurrentNode(XalanNode*	theCurrentNode);

	virtual XObjectFactory&
	getXObjectFactory() const;

	virtual XalanDOMString
	getNamespaceOfNode(const XalanNode&		n) const;

	virtual XalanDOMString
	getLocalNameOfNode(const XalanNode&		n) const;

	virtual XalanNode*
	getParentOfNode(const XalanNode&	n) const;

	virtual bool
	isNodeAfter(
			const XalanNode&	node1,
			const XalanNode&	node2) const;

	virtual XalanDOMString
	getNodeData(const XalanNode&	n) const;

	virtual XalanElement*
	getElementByID(
			const XalanDOMString&		id,
			const XalanDocument&		doc) const;

	virtual const NodeRefListBase&
	getContextNodeList() const;

	virtual void	
	setContextNodeList(const NodeRefListBase&	theList);

	virtual int
	getContextNodeListLength() const;

	virtual int
	getContextNodeListPosition(const XalanNode&		contextNode) const;

	virtual bool
	elementAvailable(
			const XalanDOMString&	theNamespace, 
			const XalanDOMString&	elementName) const;

	virtual bool
	functionAvailable(
			const XalanDOMString&	theNamespace, 
			const XalanDOMString&	functionName) const;

	virtual XObject*
	extFunction(
			const XalanDOMString&			theNamespace,
			const XalanDOMString&			functionName,
			XalanNode*						context,
			const XObjectArgVectorType&		argVec);

	virtual XLocator*
	getXLocatorFromNode(const XalanNode*	node) const;

	virtual void
	associateXLocatorToNode(
			const XalanNode*	node,
			XLocator*			xlocator);

	virtual XalanDocument*
	parseXML(
			const XalanDOMString&	urlString,
			const XalanDOMString&	base) const;

	virtual MutableNodeRefList
	createMutableNodeRefList() const;

	virtual bool
	getProcessNamespaces() const;

	virtual const NodeRefListBase*
	getNodeSetByKey(
			const XalanNode&		doc,
			const XalanDOMString&	name,
			const XalanDOMString&	ref,
			const XalanElement&		nscontext);

	virtual const NodeRefListBase*
	getNodeSetByKey(
			const XalanNode&		doc,
			const XalanDOMString&	name,
			const XalanDOMString&	ref);

	virtual const NodeRefListBase*
	getNodeSetByKey(
			const XalanNode&		doc,
			const XalanDOMString&	name,
			const XalanDOMString&	ref,
			const PrefixResolver&	resolver);

	virtual XObject*
	getVariable(const QName&	name) const;

	virtual const PrefixResolver*
	getPrefixResolver() const;

	virtual void
	setPrefixResolver(const PrefixResolver*		thePrefixResolver);

	virtual XalanDOMString
	getNamespaceForPrefix(const XalanDOMString&		prefix) const;

	virtual XalanDOMString
	findURIFromDoc(const XalanDocument*		owner) const;

	virtual XalanDOMString
	getUnparsedEntityURI(
			const XalanDOMString&	theName,
			const XalanDocument&	theDocument) const;

	virtual bool
	shouldStripSourceNode(const XalanNode&	node);

	virtual bool
	getThrowFoundIndex() const;

	virtual void
	setThrowFoundIndex(bool 	fThrow);

	virtual void
	setCurrentPattern(const XalanDOMString&		thePattern);

	virtual XalanDOMString
	getCurrentPattern() const;

	virtual XalanDocument*
	getSourceDocument(const XalanDOMString&		theURI) const;

	virtual void
	setSourceDocument(
			const XalanDOMString&	theURI,
			XalanDocument*			theDocument);


	virtual const XalanDecimalFormatSymbols*
	getDecimalFormatSymbols(const XalanDOMString&	name);

	virtual PrintWriter*
	createPrintWriter(TextOutputStream*		theTextOutputStream);

	virtual PrintWriter*
	createPrintWriter(
			const XalanDOMString&		theFileName,
			const XalanDOMString&		theEncoding);

	virtual PrintWriter*
#if defined(XALAN_NO_NAMESPACES)
	createPrintWriter(ostream&			theStream);
#else
	createPrintWriter(std::ostream&		theStream);
#endif

	// These interfaces are inherited from ExecutionContext...

	virtual void
	error(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode = 0,
			const XalanNode*		styleNode = 0) const;

	virtual void
	warn(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode = 0,
			const XalanNode*		styleNode = 0) const;

	virtual void
	message(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode = 0,
			const XalanNode*		styleNode = 0) const;

private:

	XPathExecutionContextDefault	m_xpathExecutionContextDefault;

	// $$ ToDo: Try to remove this dependency, and rely only on XSLTProcessor...
	XSLTEngineImpl&					m_xsltProcessor;

#if defined(XALAN_NO_NAMESPACES)
	typedef vector<const ElemTemplateElement*>			ElementRecursionStackType;
	typedef set<FormatterListener*>						FormatterListenerSetType;
	typedef set<PrintWriter*>							PrintWriterSetType;
	typedef set<TextOutputStream*>						TextOutputStreamSetType;
#else
	typedef std::vector<const ElemTemplateElement*>		ElementRecursionStackType;
	typedef std::set<FormatterListener*>				FormatterListenerSetType;
	typedef std::set<PrintWriter*>						PrintWriterSetType;
	typedef std::set<TextOutputStream*>					TextOutputStreamSetType;
#endif

	ElementRecursionStackType			m_elementRecursionStack;

	const PrefixResolver*				m_prefixResolver;

	const StylesheetRoot*				m_stylesheetRoot;

	FormatterListenerSetType			m_formatterListeners;

	PrintWriterSetType					m_printWriters;

	TextOutputStreamSetType				m_textOutputStreams;

	static XalanNumberFormatFactory		s_defaultXalanNumberFormatFactory;

	static XalanNumberFormatFactory*	s_xalanNumberFormatFactory;
};



#endif	// STYLESHEETEXECUTIONCONTEXTDEFAULT_HEADER_GUARD_1357924680
