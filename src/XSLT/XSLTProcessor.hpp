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
// $ Id: $

#if !defined(XALAN_XSLTPROCESSOR_HEADER_GUARD)
#define XALAN_XSLTPROCESSOR_HEADER_GUARD

// @@ JMD: Set values as pointers for now, until classes are defined, etc may
// want to revisit this

// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"





//@@ JMD: This makes it all work, I don't know why?? Need to fix this
// These were all declared in the original class XSLProcessor.hpp, but
// shouldn't be needed for an abstract class I would think
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
 * Look at the Process class
 * for an advanced example of usage by the main() function.
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
   * Transform the source tree to the output in the given 
   * result tree target.
   * @param inputSource  The input source.  May be null.
   * @param stylesheetSource  The stylesheet source.  May be null if source has a xml-stylesheet PI.
   * @param outputTarget The output source tree.
	* @exception XSLProcessorException thrown if the active ProblemListener and
	* XMLParserLiaison decide the error condition is severe enough to halt
	* processing.
   */
	virtual void
	process(
			XSLTInputSource*				inputSource, 
	        XSLTInputSource*				stylesheetSource,
	        XSLTResultTarget&				outputTarget,
			StylesheetConstructionContext&	constructionContext,
			StylesheetExecutionContext&		executionContext) = 0;

  /**
   * Given a URI to an XSL stylesheet, 
   * Compile the stylesheet into an internal representation.
   * This will delete any existing stylesheet root.  If you want to use this stylesheet multiple
   * times, or with multiple processor instances, you must use different XObjectFactory and
   * XPathFactory instances from the ones being used by the process itself.
   * @param stylesheetSource  The input source for the input XML.
   * @param xobjectFactory  The XObjectFactory to use for constructing the stylesheet.
   * @param xpathFactory  The XPathFactory to use for constructing the stylesheet.
   * @return The compiled stylesheet object.
   * @exception XSLProcessorException thrown if the active ProblemListener and XMLParserLiaison decide 
   * the error condition is severe enough to halt processing.
   */
	virtual StylesheetRoot*
	processStylesheet(
			XSLTInputSource&				stylesheetSource,
			StylesheetConstructionContext&	constructionContext) = 0;
    // throws XSLProcessorException
  
  /**
   * Given a URI to an XSL stylesheet, 
   * Compile the stylesheet into an internal representation.
   * This calls reset() before processing if the stylesheet root has been set 
   * to non-null.
   * @param xmldocURLString  The URL to the input XML document.
   * @return The compiled stylesheet object.
	* @exception XSLProcessorException thrown if the active ProblemListener and
	* XMLParserLiaison decide the error condition is severe enough to halt
	* processing.
   */
	virtual StylesheetRoot*
	processStylesheet(
			const DOMString&				xsldocURLString,
			StylesheetConstructionContext&	constructionContext) = 0;
    // throws XSLProcessorException
  
  /**
   * Reset the state.  This needs to be called after a process() call 
   * is invoked, if the processor is to be used again.
   */
   virtual void reset() = 0;
  
	/**
	 * Given an input source, get the source tree.
	 *
	 * @param inputSource pointer to input source
	 */
   virtual const DOM_Node getSourceTreeFromInput(XSLTInputSource* inputSource) = 0;

   /**
    * Output an object to the result tree by doing the right conversions.
    * This is public for access by extensions.
    *
    * @param obj the XObject to output.
    */
	virtual void
	outputToResultTree(
			const XObject&		xobj) = 0;

	virtual XObject*
	getTopLevelVariable(const DOMString&	theName) const = 0;

  /**
   * Reset the current element state
   */
	virtual void
	resetCurrentState(
			const DOM_Node&		sourceTree,
			const DOM_Node&		xmlNode) = 0;

	virtual DOM_Document
	getRootDoc() const = 0;

	virtual void
	setRootDoc(const DOM_Document& doc) = 0;

  /**
   * Evaluates attribute values for attribute templates
   * (Stuff in curly {} braces that hold expressions).
   *
   * @param contextNode the current node in the source tree
   * @param namespaceContext the current namespace context.
   * the pattern-by-example structures when parsing expressions.
   * @param stringedValue the attribute value to be processed.
   * @param executionContext the current execution context.
   * @return Processed stringedValue with attribute templates
   * resolved.
   * @exception XSLProcessorException thrown if the active ProblemListener and XMLParserLiaison decide 
   * the error condition is severe enough to halt processing.
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
   * @exception XSLProcessorException thrown if the active ProblemListener and
   * XMLParserLiaison decide the error condition is severe enough to halt
   * processing.
   * @param templateChild The template element that holds the fragment.
   * @param sourceTree The source tree document context.
   * @param sourceNode The current source context node.
   * @param mode The mode under which the template is operating.
   * @return An object that represents the result tree fragment.
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
   */
   virtual XMLParserLiaison&
   getXMLParserLiaison() const = 0;

   virtual const DOMString
   getUniqueNSValue() const = 0;

  /**
   * Convenience function to create an XObject that represents a Result tree fragment.
   * @param r The result tree fragment to use.
   * @return An XObject instance.
   */
   virtual XObject*
   createXResultTreeFrag(const ResultTreeFragBase&  r) const = 0;

   /**
   * Given a name, locate a variable in the current context, and return 
   * the object.
   * @return An XObject if the variable was found, 0 if it was not.
   */
   virtual XObject*
   getVariable(const QName& qname) const = 0;

	/**
	 * Given a name, locate a param variable in the current context, and return 
	 * the Object.
	 */
	virtual XObject*
	getParamVariable(const QName&	theName) const = 0;

	/**
	 * Given a name, a variable, and an element, push the variable on the variables
	 * stack.
	 */
	virtual void
	pushVariable(
			const QName&		name,
			XObject*			var,
			const DOM_Node&		element) = 0;

  /**
   * Push a top-level stylesheet parameter.  This value can 
   * be evaluated via xsl:param-variable.
   * @param key The name of the param.
   * @param value An XObject that will be used.
   */
   virtual void
   setStylesheetParam(
			const DOMString&	key,
			XObject*			value) = 0;
  
  /**
   * Push a top-level stylesheet parameter.  This value can 
   * be evaluated via xsl:param-variable.
   * @param key The name of the param.
   * @param expression An expression that will be evaluated.
   */
   virtual void setStylesheetParam(const DOMString& key, const DOMString& expression) = 0;


   /**
	* Given a valid element key, return the corresponding node list.
	*/
   virtual const NodeRefListBase*
   getNodeSetByKey(
					const DOM_Node&			doc, 
					const DOMString&		name, 
					const DOMString&		ref, 
					const PrefixResolver&	resolver,
					XPathExecutionContext&	executionContext) const = 0;

	/**
	 * Tells, through the combination of the default-space attribute
	 * on xsl:stylesheet, xsl:strip-space, xsl:preserve-space, and the
	 * xml:space attribute, whether or not extra whitespace should be stripped
	 * from the node.  Literal elements from template elements should
	 * <em>not</em> be tested with this function.
	 * @param textNode A text node from the source tree.
	 * @return true if the text node should be stripped of extra whitespace.
	 */
	virtual bool
	shouldStripSourceNode(const DOM_Node&	textNode) const = 0;

  /**
   * Get the current formatter listener.
   */
   virtual FormatterListener* getFormatterListener() const = 0;
  
  /**
   * Set the current formatter listener.
   */
   virtual void setFormatterListener(FormatterListener* flistener) = 0;  
  
	/**
	 * Add a trace listener for the purposes of debugging and diagnosis.
	 *
	 * @param tl trace listener to be added
	 */
   virtual void addTraceListener(TraceListener* tl) = 0;
    // throws TooManyListenersException
  
  /**
   * If this is set to true, simple traces of 
   * template calls are made.
   */
   virtual void setTraceTemplates(bool b) = 0;
  
  /**
   * If this is set to true, simple traces of 
   * template calls are made.
   */
   virtual void setTraceSelect(bool b) = 0;
  
  /**
   * If this is set to true, debug diagnostics about 
   * template children as they are being constructed 
   * will be written to the m_diagnosticsPrintWriter 
   * stream.  diagnoseTemplateChildren is false by
   * default.
   */
   virtual void setTraceTemplateChildren(bool b) = 0;
  
  /**
   * If the quietConflictWarnings property is set to 
   * true, warnings about pattern conflicts won't be 
   * printed to the diagnostics stream.
   * True by default.
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
  /**
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
   */
   virtual void setDiagnosticsOutput(PrintWriter* pw) = 0;

};

#endif	// XALAN_XSLTPROCESSOR_HEADER_GUARD

/*
 *	$ Log: $
 */
