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



#include <XalanDOM/XalanDOMString.hpp>



// Base class header file...
#include <PlatformSupport/ExecutionContext.hpp>



class AttributeList;
class ElemTemplateElement;
class FormatterListener;
class PrefixResolver;
class NodeRefListBase;
class QName;
class Stylesheet;
class StylesheetRoot;
class XalanElement;
class XalanNode;
class XalanDocument;
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

	// These interfaces are new...

	/**
	 * Retrieve parent of node
	 * 
	 * @param theNode child node
	 * @return parent node of 'theNode'
	 */
	virtual XalanNode*
	getParentOfNode(const XalanNode&	theNode) const = 0;

	/**
	 * Retrieve execution context
	 * 
	 * @return current execution context.
	 */
	virtual XPathExecutionContext&
	getXPathExecutionContext() = 0;

	/**
	 * Retrieve node list for current context.
	 * 
	 * @return node list
	 */
	virtual const NodeRefListBase&
	getContextNodeList() const = 0;

	/**
	 * Set node list for current context.
	 * 
	 * @param theContextNodeList new node list
	 */
	virtual void
	setContextNodeList(const NodeRefListBase&	theContextNodeList) = 0;

	/**
	 * Retrieve a top level variable corresponding to name.
	 * 
	 * @param name name of variable
	 * @return pointer to XObject for variable
	 */
	virtual XObject*
	getTopLevelVariable(const XalanDOMString&	theName) const = 0;

	/**
	 * Determine whether conflicts should be reported.
	 * 
	 * @return true if conflicts should not be warned
	 */
	virtual bool
	getQuietConflictWarnings() const = 0;

	/**
	 * Retrieve root document for stylesheet.
	 * 
	 * @return root document
	 */
	virtual XalanDocument*
	getRootDocument() const = 0;

	/**
	 * Set root document for stylesheet.
	 * 
	 * @param theDocument root document
	 */
	virtual void
	setRootDocument(XalanDocument*	theDocument) = 0;

	/**
	 * Create a new empty document.
	 * 
	 * @return new document
	 */
	virtual XalanDocument*
	createDocument() const = 0;

	/**
	 * Set root stylesheet for stylesheet.
	 * 
	 * @param theStylesheet root stylesheet
	 */
	virtual void
	setStylesheetRoot(StylesheetRoot*	theStylesheet) = 0;

	/**
	 * Reset the state of execution to node 'xmlNode' in source tree 'sourceTree.'
	 * 
	 * @param sourceTree source tree for execution
	 * @param xmlNode    node to execute
	 */
	virtual void
	resetCurrentState(
			XalanNode*	sourceTree,
			XalanNode*	xmlNode) = 0;

	/**
	 * Whether diagnostic output is to be generated
	 * 
	 * @return true for diagnostics output 
	 */
	virtual bool
	doDiagnosticsOutput() const = 0;

	/**
	 * Print a diagnostics string to the output device
	 * 
	 * @param theString string to print
	 */
	virtual void
	diag(const XalanDOMString&	theString) = 0;

	/**
	 * Mark the time, so that displayDuration can later display the elapsed
	 * clock ticks.
	 * 
	 * @param theKey element to push
	 */
	virtual void
	pushTime(const void*	theKey) = 0;

	/**
	 * Display the duration since pushTime was called.
	 *
	 * @param theMessage message to display
	 * @param theKey     key for which duration is displayed
	 */
	virtual void
	displayDuration(
			const XalanDOMString&	theMessage,
			const void*				theKey) = 0;

	/**
	 * Retrieve list of attributes yet to be processed
	 * 
	 * @return attribute list
	 */
	virtual const AttributeList&
	getPendingAttributes() const = 0;

	/**
	 * Retrieve name of the pending element currently being processed.
	 * 
	 * @return element name
	 */
	virtual XalanDOMString
	getPendingElementName() const = 0;

	/**
	 * Sets a list of attributes yet to be processed.
	 * 
	 * @param pendingAttributes attribute list
	 */
	virtual void
	setPendingAttributes(const AttributeList&	pendingAttributes) = 0;

	/**
	 * Replace the contents of a pending attribute.
	 * 
	 * @param pendingAttributes attribute list
	 * @param theName           name of attribute
	 * @param theNewType        type of attribute
	 * @param theNewValue       new value of attribute
	 */
	virtual void
	replacePendingAttribute(
			const XalanDOMChar*		theName,
			const XalanDOMChar*		theNewType,
			const XalanDOMChar*		theNewValue) = 0;

	/**
	 * Changes the currently pending element name.
	 * 
	 * @param elementName new name of element
	 */
	virtual void
	setPendingElementName(const XalanDOMString&		elementName) = 0;

	/**
	 * Add a result attribute to the list of pending attributes.
	 * 
	 * @param aname name of attribute
	 * @param value value of attribute
	 */
	virtual void
	addResultAttribute(
			const XalanDOMString&	aname,
			const XalanDOMString&	value) = 0;

	/**
	 * Add namespace attributes for a node to the list of pending attributes.
	 * 
	 * @param src                 source node
	 * @param srcIsStylesheetTree true if source node corresponds to a
	 *                            stylesheet
	 */
	virtual void
	copyNamespaceAttributes(
			const XalanNode&	src,
			bool				srcIsStylesheetTree) = 0;

	/**
	 * Retrieve the result prefix corresponding to a namespace.
	 * 
	 * @param theNamespace namespace for prefix
	 */
	virtual XalanDOMString
	getResultPrefixForNamespace(const XalanDOMString&	theNamespace) const = 0;

	/**
	 * Retrieve the result namespace corresponding to a prefix.
	 * 
	 * @param thePrefix prefix for namespace
	 */
	virtual XalanDOMString
	getResultNamespaceForPrefix(const XalanDOMString&	thePrefix) const = 0;

	/**
	 * Generate a random namespace prefix guaranteed to be unique.
	 * 
	 * @return unique namespace prefix
	 */
	virtual XalanDOMString
	getUniqueNameSpaceValue() const = 0;

	/**
	 * Get the current formatter listener.
	 * 
	 * @return pointer to formatter listener
	 */
	virtual FormatterListener*
	getFormatterListener() const = 0;

	/**
	 * Set the current formatter listener.
	 *
	 * @param flistener pointer to new formatter listener
	 */
	virtual void
	setFormatterListener(FormatterListener*		flistener) = 0;

	/**
	 * Retrieve the current number of spaces to indent.
	 * 
	 * @return number of spaces
	 */
	virtual int
	getIndent() const = 0;

	/**
	 * Execute an XPath and return the resulting XObject. The lifetime of this
	 * XObject is not necessarily that of the Stylesheet.
	 *
	 * @param str         string expression for XPath evaluation
	 * @param contextNode current node in the source tree
	 * @param resolver    resolver for namespace resolution
	 * @return pointer to resulting XObject
	 */
	virtual XObject*
	executeXPath(
			const XalanDOMString&	str,
			XalanNode*				contextNode,
			const PrefixResolver&	resolver) = 0;

	/**
	 * Execute an XPath and return the resulting XObject. The lifetime of this
	 * XObject is not necessarily that of the Stylesheet.
	 *
	 * @param str         string expression for XPath evaluation
	 * @param contextNode current node in the source tree
	 * @param resolver    resolver for namespace resolution
	 * @return pointer to resulting XObject
	 */
	virtual XObject*
	executeXPath(
			const XalanDOMString&	str,
			XalanNode*				contextNode,
			const XalanElement&		resolver) = 0;

	/**
	 * Create and initialize an xpath and return it. This is to be used to
	 * create an XPath that is only used during execution.
	 *
	 * @param str      string expression for XPath evaluation
	 * @param resolver resolver for namespace resolution
	 * @return pointer to resulting XPath
	 */
	virtual XPath*
	createMatchPattern(
			const XalanDOMString&	str,
			const PrefixResolver&	resolver) = 0;

	/**
	 * Evaluate the value of an attribute within the context of a specified
	 * context node and namespace
	 *
	 * @param contextNode      current context node
	 * @param namespaceContext context for namespace resolution
	 * @param stringedValue    value to evaluate
	 */
	virtual const XalanDOMString
	evaluateAttrVal(
			XalanNode*				contextNode,
			const XalanElement&		namespaceContext,
			const XalanDOMString&	stringedValue) = 0;

	/**
	 * Push a named variable onto the processor variable stack
	 *
	 * @param name    name of variable
	 * @param var     pointer to XObject value
	 * @param element element marker for variable
	 */
	virtual void
	pushVariable(
			const QName&		name,
			XObject*			var,
			const XalanNode*	element) = 0;

	/**
	 * Push a context marker onto the stack to let us know when to stop
	 * searching for a var.
	 *
	 * @param caller     caller node
	 * @param sourceNode source node
	 */
	virtual void
	pushContextMarker(
			const XalanNode*	caller,
			const XalanNode*	sourceNode) = 0;

	/**
	 * Pop the current context from the current context stack.
	 */
	virtual void
	popCurrentContext() = 0;

	/**
	 * Resolve the params that were pushed by the caller.
	 */
	virtual void
	resolveTopLevelParams() = 0;

	/**
	 * Reset the vector of top level parameters.
	 */
	virtual void
	clearTopLevelParams() = 0;

	/**
	 * Given a template, search for the arguments and push them on the stack.
	 * Also, push default arguments on the stack.
	 *
	 * @param xslCallTemplateElement "call-template" element
	 * @param sourceTree             source tree
	 * @param sourceNode             source node
	 * @param mode                   mode under which the template is operating
	 * @param targetTemplate         target template
	 */
	virtual	void
	pushParams(
			const ElemTemplateElement&	xslCallTemplateElement,
			XalanNode*					sourceTree, 
			XalanNode*					sourceNode,
			const QName&				mode,
			const XalanNode*			targetTemplate) = 0;

	/**
	 * Given a name, return a string representing the value, but don't look in
	 * the global space.
	 *
	 * @param theName name of variable
	 * @return pointer to XObject for variable
	 */
	virtual XObject*
	getParamVariable(const QName&	theName) const = 0;

	/**
	 * Receive notification of the beginning of a document.
	 *
	 * <p>The SAX parser will invoke this method only once, before any
	 * other methods in this interface or in DTDHandler (except for
	 * setDocumentLocator).</p>
	 *
	 * @exception SAXException
	 */
	virtual void
	startDocument() = 0;

	/**
	 * Receive notification of the end of a document.
	 *
	 * <p>The SAX parser will invoke this method only once, and it will
	 * be the last method invoked during the parse.  The parser shall
	 * not invoke this method until it has either abandoned parsing
	 * (because of an unrecoverable error) or reached the end of
	 * input.</p>
	 *
	 * @exception SAXException
	 */
	virtual void
	endDocument() = 0;

	/**
	 * Receive notification of the beginning of an element.
	 *
	 * <p>The Parser will invoke this method at the beginning of every
	 * element in the XML document; there will be a corresponding
	 * endElement() event for every startElement() event (even when the
	 * element is empty). All of the element's content will be
	 * reported, in order, before the corresponding endElement()
	 * event.</p>
	 *
	 * <p>If the element name has a namespace prefix, the prefix will
	 * still be attached.</p>
	 *
	 * @param name element type name
	 * @exception SAXException
	 */
	virtual void
	startElement(const XalanDOMChar*	name) = 0;

	/**
	 * Receive notification of the end of an element.
	 *
	 * <p>The SAX parser will invoke this method at the end of every
	 * element in the XML document; there will be a corresponding
	 * startElement() event for every endElement() event (even when the
	 * element is empty).</p>
	 *
	 * <p>If the element name has a namespace prefix, the prefix will
	 * still be attached to the name.</p>
	 *
	 * @param name element type name
	 * @exception SAXException
	 */
	virtual void
	endElement(const XalanDOMChar*	name) = 0;

	/**
	 * Receive notification of character data.
	 *
	 * <p>The Parser will call this method to report each chunk of
	 * character data.  SAX parsers may return all contiguous character
	 * data in a single chunk, or they may split it into several
	 * chunks; however, all of the characters in any single event
	 * must come from the same external entity, so that the Locator
	 * provides useful information.</p>
	 *
	 * <p>The application must not attempt to read from the array
	 * outside of the specified range.</p>
	 *
	 * <p>Note that some parsers will report whitespace using the
	 * ignorableWhitespace() method rather than this one (validating
	 * parsers must do so).</p>
	 *
	 * @param ch     pointer to characters from the XML document
	 * @param start  start position in the array
	 * @param length number of characters to read from the array
	 * @exception SAXException
	 */
	virtual void
	characters(
			const XalanDOMChar*		ch,
			unsigned int			start,
			unsigned int			length) = 0;

	/**
	 * Receive notification of character data. If available, when the
	 * disable-output-escaping attribute is used, output raw text without
	 * escaping.
	 *
	 * @param ch     pointer to characters from the XML document
	 * @param start  start position in the array
	 * @param length number of characters to read from the array
	 * @exception SAXException
	 */
	virtual void
	charactersRaw(
			const XalanDOMChar*		ch,
			unsigned int			start,
			unsigned int			length) = 0;

	/**
	 * Called when a Comment is to be constructed.
	 *
	 * @param   data	pointer to comment data
	 * @exception SAXException
	 */
	virtual void
	comment(const XalanDOMChar*		data) = 0;

	/**
	 * Receive notification of a processing instruction.
	 *
	 * <p>The Parser will invoke this method once for each processing
	 * instruction found: note that processing instructions may occur
	 * before or after the main document element.</p>
	 *
	 * <p>A SAX parser should never report an XML declaration (XML 1.0,
	 * section 2.8) or a text declaration (XML 1.0, section 4.3.1)
	 * using this method.</p>
	 *
	 * @param target processing instruction target
	 * @param data   processing instruction data, or null if none was supplied
	 * @exception SAXException
	 */
	virtual void
	processingInstruction(
			const XalanDOMChar*		target,
			const XalanDOMChar*		data) = 0;

	/**
	 * Flush the pending element.
	 */
	virtual void
	flushPending() = 0;

	/**
	 * Clone an element with or without children.
	 *
	 * @param node                  node to clone
	 * @param isLiteral             true if a literal element
	 * @param overrideStrip         false if white space stripping should be done
	 * @param shouldCloneAttributes true if attributes should be cloned
	 */
	virtual void
	cloneToResultTree(
			XalanNode&	node, 
			bool		isLiteral,
			bool		overrideStrip,
			bool		shouldCloneAttributes) = 0;

	/**
	 * Create an XObject that represents a Result tree fragment.
	 *
	 * @param templateChild result tree fragment to use.
	 * @param sourceTree node for source tree
	 * @param sourceNode source node
	 * @return XObject instance
	 */
	virtual XObject*
	createXResultTreeFrag(
			const ElemTemplateElement&	templateChild,
			XalanNode*					sourceTree,
			XalanNode*					sourceNode) = 0;

	/**
	 * Create an XObject that represents a Result tree fragment.
	 *
	 * @param templateChild result tree fragment to use.
	 * @param sourceTree node for source tree
	 * @param sourceNode source node
	 * @param mode current mode
	 * @return XObject instance
	 */
	virtual XObject*
	createXResultTreeFrag(
			const ElemTemplateElement&	templateChild,
			XalanNode*					sourceTree,
			XalanNode*					sourceNode,
			const QName&				mode) = 0;

	/**
	 * Given a result tree fragment, walk the tree and
	 * output it to the result stream.
	 *
	 * @param theTree result tree fragment
	 */
	virtual void
	outputResultTreeFragment(const XObject&		theTree) = 0;

	/**
	 * Determine the full XSLT Namespace URI.
	 *
	 * @return Xalan namespace URI
	 */
	virtual const XalanDOMString&
	getXSLNameSpaceURL() const = 0;

	/**
	 * Special Xalan namespace for built-in extensions.
	 *
	 * @return Xalan namespace for extensions
	 */
	virtual const XalanDOMString&
	getXalanXSLNameSpaceURL() const = 0;

	/**
	 * If this is set to true, simple traces of template calls are made.
	 *
	 * @return true if traces made
	 */
	virtual bool
	isTraceSelect() const = 0;

	/**
	 * Compose a diagnostic trace of the current selection
	 *
	 * @param theTemplate current context node
	 * @param nl          list of selected nodes
	 */
	virtual void
	traceSelect(
			const XalanElement&		theTemplate,
			const NodeRefListBase&	nl) const = 0;

	/**
	 * Determine if an element is on the recursion stack.
	 *
	 * @return true if element on stack
	 */
	virtual bool
	findOnElementRecursionStack(const ElemTemplateElement*	theElement) const = 0;

	/**
	 * Push an element onto the recursion stack.
	 *
	 * @param theElement pointer to element to push
	 */
	virtual void
	pushOnElementRecursionStack(const ElemTemplateElement*	theElement) = 0;

	/**
	 * Pop an element off the recursion stack.
	 *
	 * @return pointer to element popped
	 */
	virtual const ElemTemplateElement*
	popElementRecursionStack() = 0;

	/**
	 * Class for keeping track of elements pushed on the element recursion stack
	 */
	class ElementRecursionStackPusher
	{
	public:

	/**
	 * Construct an instance of the recursion stack pusher.
	 *
	 * @param executionContext current execution context
	 * @param element pointer to element to push
	 */
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

	// These interfaces are inherited from ExecutionContext...

	virtual void
	error(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode = 0,
			const XalanNode*		styleNode = 0) const = 0;

	virtual void
	warn(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode = 0,
			const XalanNode*		styleNode = 0) const = 0;

	virtual void
	message(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode = 0,
			const XalanNode*		styleNode = 0) const = 0;
};






#endif	// STYLESHEETEXECUTIONCONTEXT_HEADER_GUARD_1357924680
