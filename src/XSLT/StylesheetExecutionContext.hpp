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
#if !defined(STYLESHEETEXECUTIONCONTEXT_HEADER_GUARD_1357924680)
#define STYLESHEETEXECUTIONCONTEXT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XSLT/XSLTDefinitions.hpp>


#define XALAN_NO_IOSFWD
#if defined(XALAN_NO_IOSFWD)
#include <ostream>
#else
#include <iosfwd>
#endif

#include <memory>



// Base class header file...
#include <XPath/XPathExecutionContext.hpp>



#include <XalanDOM/XalanDOMString.hpp>



// Base class header file...
#include <PlatformSupport/ExecutionContext.hpp>



#include <PlatformSupport/AttributeListImpl.hpp>



class ElemTemplateElement;
class FormatterListener;
class FormatterToDOM;
class FormatterToHTML;
class FormatterToText;
class FormatterToXML;
class GenerateEvent;
class PrefixResolver;
class NodeRefListBase;
class PrintWriter;
class QName;
class SelectionEvent;
class Stylesheet;
class StylesheetRoot;
class TextOutputStream;
class TracerEvent;
class Writer;
class XalanDocument;
class XalanDocumentFragment;
class XalanElement;
class XalanNode;
class XalanNumberFormat;
class XPath;
class XObject;



//
// An abstract class which provides support for executing stylesheets.
//
class XALAN_XSLT_EXPORT StylesheetExecutionContext : public XPathExecutionContext
{
public:

	explicit
	StylesheetExecutionContext();

	virtual
	~StylesheetExecutionContext();

	// These interfaces are new...

	/**
	 * Reset the instance, and prepare to re-use the
	 * context.
	 */
	virtual void
	reset() = 0;

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
	setStylesheetRoot(const StylesheetRoot*		theStylesheet) = 0;

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
	 * Retrieve name of the pending element currently being processed.
	 * 
	 * @return element name
	 */
	virtual XalanDOMString
	getPendingElementName() const = 0;

	/**
	 * Changes the currently pending element name.
	 * 
	 * @param elementName new name of element
	 */
	virtual void
	setPendingElementName(const XalanDOMString&		elementName) = 0;

	/**
	 * Retrieve list of attributes yet to be processed
	 * 
	 * @return attribute list
	 */
	virtual const AttributeList&
	getPendingAttributes() const = 0;

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

	/*
	 * See if there is a pending start document event waiting.
	 * @return true if there is a start document event waiting.
	 */
	virtual bool
	getHasPendingStartDocument() const = 0;

	/**
	 * Set the pending start document event state.
	 * @param the new value
	 */
	virtual void
	setHasPendingStartDocument(bool	b) = 0;

	/**
	 * See if a pending start document event must be flushed.
	 * @return true if the event must be flushed.
	 */
	virtual bool
	getMustFlushPendingStartDocument() const = 0;

	/**
	 * Set the pending start document event flush state.
	 * @param the new value
	 */
	virtual void
	setMustFlushPendingStartDocument(bool	b) = 0;

	// This next group of classes are used to save and restore
	// the execution state in an automated, and exception-safe
	// manner.

	class HasPendingStartDocumentSetAndRestore
	{
	public:

		/**
		 * Construct an object to set and restore the current pending start.
		 * document state.
		 * @param theExecutionContext a reference to the current execution context
		 * @param theNewState the new state to set.
		 */
		HasPendingStartDocumentSetAndRestore(
			StylesheetExecutionContext&		theExecutionContext,
			bool							theNewState) :
				m_executionContext(theExecutionContext),
				m_savedState(theExecutionContext.getHasPendingStartDocument())
		{
			theExecutionContext.setHasPendingStartDocument(theNewState);
		}

		~HasPendingStartDocumentSetAndRestore()
		{
			m_executionContext.setHasPendingStartDocument(m_savedState);
		}

	private:

		StylesheetExecutionContext&		m_executionContext;

		const bool						m_savedState;
	};

	class MustFlushPendingStartDocumentSetAndRestore
	{
	public:

		/**
		 * Construct an object to set and restore the current flush pending start.
		 * document state.
		 * @param theExecutionContext a reference to the current execution context
		 * @param theNewState the new state to set.
		 */
		MustFlushPendingStartDocumentSetAndRestore(
			StylesheetExecutionContext&		theExecutionContext,
			bool							theNewState) :
				m_executionContext(theExecutionContext),
				m_savedState(theExecutionContext.getMustFlushPendingStartDocument())
		{
			theExecutionContext.setMustFlushPendingStartDocument(theNewState);
		}

		~MustFlushPendingStartDocumentSetAndRestore()
		{
			m_executionContext.setMustFlushPendingStartDocument(m_savedState);
		}

	private:

		StylesheetExecutionContext&		m_executionContext;

		const bool						m_savedState;
	};

	class FormatterListenerSetAndRestore
	{
	public:

		/**
		 * Construct an object to set and restore the current formatter listener.
		 *
		 * @param theExecutionContext a reference to the current execution context
		 * @param theNewListener the new listener to set.
		 */
		FormatterListenerSetAndRestore(
			StylesheetExecutionContext&		theExecutionContext,
			FormatterListener*				theNewListener = 0) :
				m_executionContext(theExecutionContext),
				m_savedListener(theExecutionContext.getFormatterListener())
		{
			theExecutionContext.setFormatterListener(theNewListener);
		}

		~FormatterListenerSetAndRestore()
		{
			m_executionContext.setFormatterListener(m_savedListener);
		}

	private:

		StylesheetExecutionContext&		m_executionContext;

		FormatterListener* const		m_savedListener;
	};

	class PendingElementNameSetAndRestore
	{
	public:

		/**
		 * Construct an object to set and restore the current pending element name.
		 *
		 * @param theExecutionContext a reference to the current execution context
		 * @param theNewPendingElementName the new pending element name to set.
		 */
		PendingElementNameSetAndRestore(
			StylesheetExecutionContext&		theExecutionContext,
			const XalanDOMString&			theNewPendingElementName = XalanDOMString()) :
				m_executionContext(theExecutionContext),
				m_savedPendingElementName(theExecutionContext.getPendingElementName())
		{
			theExecutionContext.setPendingElementName(theNewPendingElementName);
		}

		~PendingElementNameSetAndRestore()
		{
			m_executionContext.setPendingElementName(m_savedPendingElementName);
		}

	private:

		StylesheetExecutionContext&		m_executionContext;

		const DOMString					m_savedPendingElementName;
	};

	class PendingAttributesSetAndRestore
	{
	public:

		/**
		 * Construct an object to set and restore the current pending attributes.
		 *
		 * @param theExecutionContext a reference to the current execution context
		 * @param theNewPendingAttributes the new pending attributes to set.
		 */
		PendingAttributesSetAndRestore(
			StylesheetExecutionContext&		theExecutionContext,
			const AttributeListImpl&		theNewPendingAttributes = AttributeListImpl()) :
				m_executionContext(theExecutionContext),
				m_savedPendingAttributes(theExecutionContext.getPendingAttributes())
		{
			theExecutionContext.setPendingAttributes(theNewPendingAttributes);
		}

		~PendingAttributesSetAndRestore()
		{
			m_executionContext.setPendingAttributes(m_savedPendingAttributes);
		}

	private:

		StylesheetExecutionContext&		m_executionContext;

		const AttributeListImpl			m_savedPendingAttributes;
	};

	class ExecutionStateSetAndRestore
	{
	public:

		/**
		 * Construct an object to set and restore the current execution state.
		 *
		 * @param theExecutionContext a reference to the current execution context
		 * @param theNewListener the new listener to set.
		 * @param theNewPendingElementName the new pending element name to set.
		 * @param theNewPendingAttributes the new pending attributes to set.
		 */
		ExecutionStateSetAndRestore(
			StylesheetExecutionContext&		theExecutionContext,
			FormatterListener*				theNewListener = 0,
			bool							hasPendingStartDocument = false,
			bool							mustFlushPendingStartDocument = false,
			const XalanDOMString&			theNewPendingElementName = XalanDOMString(),
			const AttributeListImpl&		theNewPendingAttributes = AttributeListImpl()) :
				m_hasPendingSetAndRestore(theExecutionContext,
										  hasPendingStartDocument),
				m_flushPendingSetAndRestore(theExecutionContext,
											mustFlushPendingStartDocument),
				m_formatterListenerSetAndRestore(theExecutionContext,
												 theNewListener),
				m_pendingElementNameSetAndRestore(theExecutionContext,
												  theNewPendingElementName),
				m_pendingAttributesSetAndRestore(theExecutionContext,
												 theNewPendingAttributes)
		{
		}

		~ExecutionStateSetAndRestore()
		{
		}

	private:

		const HasPendingStartDocumentSetAndRestore			m_hasPendingSetAndRestore;

		const MustFlushPendingStartDocumentSetAndRestore	m_flushPendingSetAndRestore;

		const FormatterListenerSetAndRestore				m_formatterListenerSetAndRestore;

		const PendingElementNameSetAndRestore				m_pendingElementNameSetAndRestore;

		const PendingAttributesSetAndRestore				m_pendingAttributesSetAndRestore;
	};

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
	 * Get the top of the stack frame from where a search 
	 * for a variable or param should take place.
	 *
	 * @return current value of index
	 */
	virtual int
	getCurrentStackFrameIndex() const = 0;

	/**
	 * Set the top of the stack frame from where a search 
	 * for a variable or param should take place.
	 *
	 * @param currentStackFrameIndex new value of index
	 */
	virtual void
	setCurrentStackFrameIndex(int	currentStackFrameIndex = -1) = 0;

	/*
	 * A class to manage stack state during execution.
	 */
	class ParamsPushPop
	{
	public:

		ParamsPushPop(
			StylesheetExecutionContext&		executionContext,
			const ElemTemplateElement*		contextElement,
			const ElemTemplateElement&		xslCallTemplateElement,
			XalanNode*						sourceTree, 
			XalanNode*						sourceNode,
			const QName&					mode,
			const XalanNode*				targetTemplate);

		~ParamsPushPop();

		int
		getStackFrameIndex() const
		{
			return m_savedStackFrameIndex;
		}

	private:

		StylesheetExecutionContext&		m_executionContext;
	
		const int						m_savedStackFrameIndex;
	};

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


	/**
	 * Create a new FormatterToXML instance.  The execution context
	 * owns the instance and will delete it when reset.
	 *
	 * @param writer            character output stream to use
	 * @param version           version of the output method
	 * @param doIndent          true if output is to be indented
	 * @param indent            number of spaces to indent at each nesting level
	 * @param encoding          character encoding in use by <VAR>writer</VAR>
	 * @param mediaType         media type (MIME content type) of the data
	 * @param doctypeSystem     system identifier to be used in the document
	 *                          type declaration
	 * @param doctypePublic     public identifier to be used in the document
	 *                          type declaration
	 * @param xmlDecl           true if the XSLT processor should output an XML
	 *                          declaration
	 * @param standalone        true if the XSLT processor should output a
	 *                          standalone document declaration
	 * @return a pointer to the new instance.
	 */
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
			const XalanDOMString&	standalone = XalanDOMString()) = 0;

	enum eDummy
	{
		eDefaultHTMLIndentAmount = 4
	};

	/**
	 * Create a new FormatterToHTML instance.  The execution context
	 * owns the instance and will delete it when reset.
	 *
	 * @param writer            character output stream to use
	 * @param encoding          character encoding in use by <VAR>writer</VAR>
	 * @param mediaType         media type (MIME content type) of the data
	 * @param doctypeSystem     system identifier to be used in the document
	 *                          type declaration
	 * @param doctypePublic     public identifier to be used in the document
	 *                          type declaration
	 * @param doIndent          true if output is to be indented
	 * @param indent            number of spaces to indent at each nesting level
	 * @param version           version of the output method
	 * @param standalone        true if the XSLT processor should output a
	 *                          standalone document declaration
	 * @param xmlDecl           true if the XSLT processor should output an XML
	 *                          declaration
	 * @return a pointer to the new instance.
	 */
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
			bool					xmlDecl = false) = 0;

	/**
	 * Construct a FormatterToDOM instance.  it will add the DOM nodes 
	 * to the document fragment.
	 *
	 * @param doc            document for nodes
	 * @param docFrag        document fragment for nodes, default none
	 * @param currentElement current element for nodes, default none
	 */
	virtual FormatterToDOM*
	createFormatterToDOM(
			XalanDocument*			doc,
			XalanDocumentFragment*	docFrag = 0,
			XalanElement*			currentElement = 0) = 0;

	/**
	 * Construct a FormatterToDOM instance.  it will add the DOM nodes 
	 * to the document fragment.
	 *
	 * @param doc            document for nodes
	 * @param docFrag        document fragment for nodes, default none
	 * @param currentElement current element for nodes, default none
	 */
	virtual FormatterToDOM*
	createFormatterToDOM(
			XalanDocument*	doc,
			XalanElement*	elem) = 0;

	/**
	 * FormatterToText instance constructor.
	 *
	 * @param pw print writer for output
	 */
	virtual FormatterToText*
	createFormatterToText(Writer&	writer) = 0;


#if defined(XALAN_NO_NAMESPACES)
	typedef auto_ptr<XalanNumberFormat>			XalanNumberFormatAutoPtr;
#else
	typedef std::auto_ptr<XalanNumberFormat>	XalanNumberFormatAutoPtr;
#endif

	/**
	 * Create a new XalanNumberFormat instance.
	 *
	 * @return an auto_ptr that owns a new XalanNumberFormat instance.
	 */
	virtual XalanNumberFormatAutoPtr
	createXalanNumberFormat() = 0;


	// Trace interface...

	/**
	 * Determine the number of trace listeners.
	 * 
	 * @return number of listeners
	 */
	virtual unsigned long
	getTraceListeners() const = 0;

	/**
	 * Fire a generate event.
	 * 
	 * @param ge generate event to fire
	 */
	virtual void
	fireGenerateEvent(const GenerateEvent&	ge) = 0;

	/**
	 * Fire a trace event.
	 * 
	 * @param te trace event to fire
	 */
	virtual void
	fireTraceEvent(const TracerEvent&	te) = 0;

	/**
	 * Fire a selection event.
	 * 
	 * @param se selection event to fire
	 */
	virtual void
	fireSelectEvent(const SelectionEvent&	se) = 0;

	/**
	 * If this is set to true, simple traces of template calls are made.
	 *
	 * @return true if traces made
	 */
	virtual bool
	getTraceSelects() const = 0;

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


	// These interfaces are inherited from XPathExecutionContext...

	virtual XalanNode*
	getCurrentNode() const = 0;

	virtual void
	setCurrentNode(XalanNode*	theCurrentNode) = 0;

	virtual XObjectFactory&
	getXObjectFactory() const = 0;

	virtual XalanDOMString
	getNamespaceOfNode(const XalanNode&		n) const = 0;

	virtual XalanDOMString
	getLocalNameOfNode(const XalanNode&		n) const = 0;

	virtual XalanNode*
	getParentOfNode(const XalanNode&	n) const = 0;

	virtual bool
	isNodeAfter(
			const XalanNode&	node1,
			const XalanNode&	node2) const = 0;

	virtual XalanDOMString
	getNodeData(const XalanNode&	n) const = 0;

	virtual XalanElement*
	getElementByID(
			const XalanDOMString&		id,
			const XalanDocument&		doc) const = 0;

	virtual const NodeRefListBase&
	getContextNodeList() const = 0;

	virtual void	
	setContextNodeList(const NodeRefListBase&	theList) = 0;

	virtual int
	getContextNodeListLength() const = 0;

	virtual int
	getContextNodeListPosition(const XalanNode&		contextNode) const = 0;

	virtual bool
	elementAvailable(
			const XalanDOMString&	theNamespace, 
			const XalanDOMString&	elementName) const = 0;

	virtual bool
	functionAvailable(
			const XalanDOMString&	theNamespace, 
			const XalanDOMString&	functionName) const = 0;

	virtual XObject*
	extFunction(
			const XalanDOMString&			theNamespace,
			const XalanDOMString&			functionName,
			XalanNode*						context,
			const XObjectArgVectorType&		argVec) = 0;

	virtual XLocator*
	getXLocatorFromNode(const XalanNode*	node) const = 0;

	virtual void
	associateXLocatorToNode(
			const XalanNode*	node,
			XLocator*			xlocator) = 0;

	virtual XalanDocument*
	parseXML(
			const XalanDOMString&	urlString,
			const XalanDOMString&	base) const = 0;

	virtual MutableNodeRefList
	createMutableNodeRefList() const = 0;

	virtual bool
	getProcessNamespaces() const = 0;

	virtual const NodeRefListBase*
	getNodeSetByKey(
			const XalanNode&		doc,
			const XalanDOMString&	name,
			const XalanDOMString&	ref,
			const XalanElement&		nscontext) = 0;

	virtual const NodeRefListBase*
	getNodeSetByKey(
			const XalanNode&		doc,
			const XalanDOMString&	name,
			const XalanDOMString&	ref) = 0;

	virtual const NodeRefListBase*
	getNodeSetByKey(
			const XalanNode&		doc,
			const XalanDOMString&	name,
			const XalanDOMString&	ref,
			const PrefixResolver&	resolver) = 0;

	virtual XObject*
	getVariable(const QName&	name) const = 0;

	virtual const PrefixResolver*
	getPrefixResolver() const = 0;

	virtual void
	setPrefixResolver(const PrefixResolver*		thePrefixResolver) = 0;

	virtual XalanDOMString
	getNamespaceForPrefix(const XalanDOMString&		prefix) const = 0;

	virtual XalanDOMString
	findURIFromDoc(const XalanDocument*		owner) const = 0;

	virtual XalanDOMString
	getUnparsedEntityURI(
			const XalanDOMString&	theName,
			const XalanDocument&	theDocument) const = 0;

	virtual bool
	shouldStripSourceNode(const XalanNode&	node) = 0;

	virtual bool
	getThrowFoundIndex() const = 0;

	virtual void
	setThrowFoundIndex(bool 	fThrow) = 0;

	virtual void
	setCurrentPattern(const XalanDOMString&		thePattern) = 0;

	virtual XalanDOMString
	getCurrentPattern() const = 0;

	virtual XalanDocument*
	getSourceDocument(const XalanDOMString&		theURI) const = 0;

	virtual void
	setSourceDocument(
			const XalanDOMString&	theURI,
			XalanDocument*			theDocument) = 0;


	virtual const XalanDecimalFormatSymbols*
	getDecimalFormatSymbols(const XalanDOMString&	name) = 0;

	virtual PrintWriter*
	createPrintWriter(TextOutputStream*		theTextOutputStream) = 0;

	virtual PrintWriter*
	createPrintWriter(
			const XalanDOMString&		theFileName,
			const XalanDOMString&		theEncoding) = 0;

	virtual PrintWriter*
#if defined(XALAN_NO_NAMESPACES)
	createPrintWriter(ostream&			theStream) = 0;
#else
	createPrintWriter(std::ostream&		theStream) = 0;
#endif

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
