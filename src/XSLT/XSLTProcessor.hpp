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
 * $ Id: $
 *
 */

#if !defined(XALAN_XSLTPROCESSOR_HEADER_GUARD)
#define XALAN_XSLTPROCESSOR_HEADER_GUARD

// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"

class Arg;
class DispatcherFactory;
class DOM_Element;
class ElemTemplateElement;
class Formatter;
class FormatterListener;
class InputSource;
class Locator;
class NodeRefListBase;
class PrefixResolver;
class PrintWriter;
class ProblemListener;
class QName;
class ResultTreeFrag;
class ResultTreeFragBase;
class StackEntry;
class Stylesheet;
class StylesheetConstructionContext;
class StylesheetExecutionContext;
class StylesheetRoot;
class TraceListener;
class XMLParserLiaison;
class XObject;
class XObjectFactory;
class XPathExecutionContext;
class XPathFactory;
class XPathProcessor;
class XSLTInputSource;
class XSLTResultTarget;



#include <dom/DOM_Node.hpp>
#include <dom/DOMString.hpp>


/**
 * This is an interface for an XSLT Processor engine. It's the responsibility
 * of the implementation of the XSLTProcessor interface, collaborating with a
 * XMLParserLiaison, the DOM, and the XPath engine, to transform a source tree
 * of nodes into a result tree according to instructions and templates
 * specified by a stylesheet tree.  The methods process(...) are
 * the primary public entry points.
 * 
 * If you reuse the processor instance, you should call reset() between calls.
 */
class XALAN_XSLT_EXPORT XSLTProcessor
{
public:

  	XSLTProcessor();

	virtual
	~XSLTProcessor();

	/**
	 * Transform the source tree to the output in the given result tree target.
	 *
	 * @param inputSource         input source,  may be null
	 * @param stylesheetSource    stylesheet source,  may be null if source
	 *                            has a xml-stylesheet PI
	 * @param outputTarget        output source tree
	 * @param constructionContext context for construction of objects
	 * @param executionContext    current execution context
	 * @exception XSLProcessorException 
	 */
	virtual void
	process(
		XSLTInputSource*				inputSource, 
		XSLTInputSource*				stylesheetSource,
		XSLTResultTarget&				outputTarget,
		StylesheetConstructionContext&	constructionContext,
		StylesheetExecutionContext&		executionContext) = 0;

	/**
	 * Given a stylesheet input source, compile the stylesheet into an internal
	 * representation. This will delete any existing stylesheet root.
	 *
	 * @param stylesheetSource    input source for the stylesheet
	 * @param constructionContext context for construction of objects
	 * @return pointer to the compiled stylesheet object
	 * @exception XSLProcessorException 
	 */
	virtual StylesheetRoot*
	processStylesheet(
			XSLTInputSource&				stylesheetSource,
			StylesheetConstructionContext&	constructionContext) = 0;
  
	/**
	 * Given a URI to an XSL stylesheet, compile the stylesheet into an internal
	 * representation. This will delete any existing stylesheet root.
	 *
	 * @param xmldocURLString URI to the input XML document
	 * @param constructionContext context for construction of objects
	 * @return pointer to compiled stylesheet object
	 * @exception XSLProcessorException 
	 */
	virtual StylesheetRoot*
	processStylesheet(
			const DOMString&				xsldocURLString,
			StylesheetConstructionContext&	constructionContext) = 0;
  
	/**
	 * Reset the state.  This needs to be called after a process() call 
	 * is invoked, if the processor is to be used again.
	 */
   virtual void reset() = 0;
  
	/**
	 * Given an input source, get the source tree.
	 *
	 * @param inputSource pointer to input source
	 * @return source tree
	 */
   virtual const DOM_Node getSourceTreeFromInput(XSLTInputSource* inputSource) = 0;

   /**
    * Output an object to the result tree by doing the right conversions.
    * This is public for access by extensions.
    *
    * @param obj the XObject to output
    */
	virtual void
	outputToResultTree(
			const XObject&		xobj) = 0;

	/**
	 * Retrieve a top level variable corresponding to name.
	 * 
	 * @param theName name of variable
	 * @return pointer to XObject for variable
	 */
	virtual XObject*
	getTopLevelVariable(const DOMString&	theName) const = 0;

	/**
	 * Reset the state of execution to node 'xmlNode' in source tree
	 * 'sourceTree.'
	 * 
	 * @param sourceTree source tree for execution
	 * @param xmlNode    node to execute
	 */
	virtual void
	resetCurrentState(
			const DOM_Node&		sourceTree,
			const DOM_Node&		xmlNode) = 0;

	/**
	 * Retrieve root document for stylesheet.
	 * 
	 * @return root document
	 */
	virtual DOM_Document
	getRootDoc() const = 0;

	/**
	 * Set root document for stylesheet.
	 * 
	 * @param doc root document
	 */
	virtual void
	setRootDoc(const DOM_Document& doc) = 0;

	/**
	 * Evaluates attribute values for attribute templates (Stuff in curly {}
	 * braces that hold expressions).
	 *
	 * @param contextNode      current node in the source tree
	 * @param namespaceContext current namespace context for the
	 *                         pattern-by-example structures when parsing
	 *                         expressions
	 * @param stringedValue    attribute value to be processed
	 * @param executionContext current execution context
	 * @return processed stringedValue with attribute templates resolved
	 * @exception XSLProcessorException 
	 */
	virtual DOMString
	evaluateAttrVal(
			const DOM_Node&			contextNode,
			const DOM_Element&		namespaceContext,
			const DOMString&		stringedValue,
			XPathExecutionContext&	executionContext) = 0;

	/**
	 * Given a stylesheet element, create a result tree fragment from its 
	 * contents.  Caller owns the memory.
	 *
	 * @param templateChild template element that holds the fragment
	 * @param sourceTree    source tree document context
	 * @param sourceNode    current source context node
	 * @param mode          mode under which the template is operating
	 * @return pointer to an object that represents the result tree fragment
	 * @exception XSLProcessorException 
	 */
	virtual ResultTreeFragBase*
	createResultTreeFrag(
			StylesheetExecutionContext&		executionContext,
			const ElemTemplateElement&		templateChild,
			const DOM_Node&					sourceTree,
			const DOM_Node&					sourceNode,
			const QName&					mode) = 0;

	/**
	 * Create an empty result tree fragment. Caller owns the memory.
	 *
	 * @return pointer to an object that represents the result tree fragment
	 */
	virtual ResultTreeFragBase*
	createResultTreeFrag() const = 0;

	/**
	 * Resolve the params that were pushed by the caller.
	 */
	virtual void
	resolveTopLevelParams(StylesheetExecutionContext&	executionContext) = 0;

  /**
   * Get the XML Parser Liaison that this processor uses.
	 *
	 * @return XML parser liaison object
   */
   virtual XMLParserLiaison&
   getXMLParserLiaison() const = 0;

	/**
	 * Generate a random namespace prefix guaranteed to be unique.
	 * 
	 * @return unique namespace prefix
	 */
   virtual const DOMString
   getUniqueNSValue() const = 0;

	/**
	 * Convenience function to create an XObject that represents a Result tree
	 * fragment.
	 *
	 * @param r result tree fragment to use
	 * @return XObject instance
	 */
   virtual XObject*
   createXResultTreeFrag(const ResultTreeFragBase&  r) const = 0;

	/**
	 * Given a name, locate a variable in the current context, and return 
	 * the object.
	 *
	 * @param theName name of variable
	 * @return An XObject if the variable was found, 0 if it was not.
	 */
   virtual XObject*
   getVariable(const QName& qname) const = 0;

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
			const DOM_Node&		element) = 0;

  /**
	* Push a top-level stylesheet parameter.  This value can be evaluated via
	* xsl:param-variable.
	*
   * @param key   name of the parameter
   * @param value XObject value for parameter
   */
   virtual void
   setStylesheetParam(
			const DOMString&	key,
			XObject*			value) = 0;
  
  /**
	* Push a top-level stylesheet parameter.  This value can be evaluated via
	* xsl:param-variable.
	*
   * @param key name of the param
   * @param expression expression that will be evaluated
   */
   virtual void setStylesheetParam(
			const DOMString& key,
			const DOMString& expression) = 0;

	/**
	 * Given a valid element key, return the corresponding node list.
	 *
	 * @param doc              source document
	 * @param name             name of the key, which must match the 'name'
	 *                         attribute on xsl:key
	 * @param ref              value that must match the value found by the
	 *                         'match' attribute on xsl:key
	 * @param resolver         resolver for namespace resolution
	 * @param executionContext current execution context
	 * @return if the name was not declared with xsl:key, this will return
	 * null, if the identifier is not found, it will return an empty node set,
	 * otherwise it will return a nodeset of nodes.
	 */
   virtual const NodeRefListBase*
   getNodeSetByKey(
					const DOM_Node&			doc, 
					const DOMString&		name, 
					const DOMString&		ref, 
					const PrefixResolver&	resolver,
					XPathExecutionContext&	executionContext) const = 0;

	/**
	 * Tells, through the combination of the default-space attribute on
	 * xsl:stylesheet, xsl:strip-space, xsl:preserve-space, and the xml:space
	 * attribute, whether or not extra whitespace should be stripped from the
	 * node.  Literal elements from template elements should <em>not</em> be
	 * tested with this function.
	 *
	 * @param textNode text node from the source tree
	 * @return true if the text node should be stripped of extra whitespace
	 */
	virtual bool
	shouldStripSourceNode(const DOM_Node&	textNode) const = 0;

	/**
	 * Get the current formatter listener.
	 * 
	 * @return pointer to formatter listener
	 */
   virtual FormatterListener* getFormatterListener() const = 0;
  
	/**
	 * Set the current formatter listener.
	 *
	 * @param flistener pointer to new formatter listener
	 */
	virtual void setFormatterListener(FormatterListener* flistener) = 0;  
  
	/**
	 * Add a trace listener for the purposes of debugging and diagnosis.
	 * 
	 * @param tl pointer to listener to add
	 */
   virtual void addTraceListener(TraceListener* tl) = 0;
  
	/**
	 * If this is set to true, simple traces of template calls are made.
	 *
	 * @param b true to make traces of template calls
	 */
	virtual void setTraceTemplates(bool b) = 0;

	/**
	 * If this is set to true, simple traces of select calls are made.
	 *
	 * @param b true to make traces of select calls
	 */
	virtual void setTraceSelect(bool b) = 0;
  
	/**
	 * If this is set to true, debug diagnostics about 
	 * template children as they are being constructed 
	 * will be written to the m_diagnosticsPrintWriter 
	 * stream.  diagnoseTemplateChildren is false by
	 * default.
	 *
	 * @param b true to make traces of template children construction
	 */
	virtual void setTraceTemplateChildren(bool b) = 0;

	/**
	 * If the quietConflictWarnings property is set to 
	 * true, warnings about pattern conflicts won't be 
	 * printed to the diagnostics stream.
	 * True by default.
	 *
	 * @param b true if conflict warnings should be suppressed.
	 */
	virtual void setQuietConflictWarnings(bool b) = 0;

	/**
	 * Remove a trace listener.
	 *
	 * @param tl Trace listener to be removed.
	 */
	virtual void removeTraceListener(TraceListener* tl) = 0;
  
// @@TODO: what to do about output stream ??
  /*
   * If this is set, diagnostics will be 
   * written to the m_diagnosticsPrintWriter stream. If 
   * the value is null, then diagnostics will be turned 
   * off.
   */
//   virtual void setDiagnosticsOutput(java.io.OutputStream out) = 0;
  
	/**
	 * If this is set, diagnostics will be 
	 * written to the m_diagnosticsPrintWriter stream. If 
	 * the value is null, then diagnostics will be turned 
	 * off.
	 *
	 * @param pw pointer to print writer
	 */
	virtual void setDiagnosticsOutput(PrintWriter* pw) = 0;

};

#endif	// XALAN_XSLTPROCESSOR_HEADER_GUARD

/*
 *	$ Log: $
 */
