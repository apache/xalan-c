/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the  "License");
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

#if !defined(XALAN_XSLTPROCESSOR_HEADER_GUARD)
#define XALAN_XSLTPROCESSOR_HEADER_GUARD



// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"



#include <cstddef>



#include "xalanc/PlatformSupport/ProblemListenerBase.hpp"



XALAN_DECLARE_XERCES_CLASS(Locator)



XALAN_CPP_NAMESPACE_BEGIN



XALAN_USING_XERCES(Locator);



class ElemTemplateElement;
class FormatterListener;
class GenerateEvent;
class NodeRefListBase;
class PrefixResolver;
class PrintWriter;
class SelectionEvent;
class StylesheetConstructionContext;
class StylesheetExecutionContext;
class StylesheetRoot;
class TraceListener;
class TracerEvent;
class XalanDOMString;
class XalanDocument;
class XalanElement;
class XalanNode;
class XMLParserLiaison;
class XObject;
class XObjectPtr;
class XPath;
class XPathExecutionContext;
class XSLTInputSource;
class XSLTResultTarget;



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
class XALAN_XSLT_EXPORT XSLTProcessor : public ProblemListenerBase
{
public:

    typedef XalanSize_t     size_type;

    XSLTProcessor();

    virtual
    ~XSLTProcessor();


    // These interfaces are inherited from ProblemListenerBase.

    virtual void
    problem(
            eSource                 source,
            eClassification         classification,
            const XalanDOMString&   msg,
            const Locator*          locator,
            const XalanNode*        sourceNode) = 0;

    virtual void
    problem(
            eSource                 source,
            eClassification         classification,
            const XalanDOMString&   msg,
            const XalanNode*        sourceNode) = 0;

    // These interfaces are new to XSLTProcessor
    /**
     * Transform the source tree to the output in the given result tree target.
     * The processor will process the input source, the stylesheet source,
     * and transform to the output target.
     *
     * @param inputSource         input source
     * @param stylesheetSource    stylesheet source
     * @param outputTarget        output source tree
     * @param constructionContext context for construction of objects
     * @param executionContext    current execution context
     * @exception XSLProcessorException 
     */
    virtual void
    process(
        const XSLTInputSource&          inputSource,
        const XSLTInputSource&          stylesheetSource,
        XSLTResultTarget&               outputTarget,
        StylesheetConstructionContext&  constructionContext,
        StylesheetExecutionContext&     executionContext) = 0;

    /**
     * Transform the source tree to the output in the given result tree target.
     * This function does not create a stylesheet tree, it assumes the
     * provided StylesheetExecutionContext has the stylesheet tree to use. This
     * is set by calling StylesheetExecutionContext::setStylesheetRoot().
     *
     * @param inputSource         input source
     * @param outputTarget        output source tree
     * @param executionContext    current execution context
     * @exception XSLProcessorException 
     */
    virtual void
    process(
            const XSLTInputSource&          inputSource,
            XSLTResultTarget&               outputTarget,
            StylesheetExecutionContext&     executionContext) = 0;

    /**
     * Given a stylesheet input source, compile the stylesheet into an internal
     * representation.
     *
     * @param stylesheetSource    input source for the stylesheet
     * @param constructionContext context for construction of objects
     * @return pointer to the compiled stylesheet object
     * @exception XSLProcessorException 
     */
    virtual StylesheetRoot*
    processStylesheet(
            const XSLTInputSource&          stylesheetSource,
            StylesheetConstructionContext&  constructionContext) = 0;
  
    /**
     * Given a URI to an XSL stylesheet, compile the stylesheet into an internal
     * representation.
     *
     * @param xmldocURLString URI to the input XML document
     * @param constructionContext context for construction of objects
     * @return pointer to compiled stylesheet object
     * @exception XSLProcessorException 
     */
    virtual StylesheetRoot*
    processStylesheet(
            const XalanDOMString&           xsldocURLString,
            StylesheetConstructionContext&  constructionContext) = 0;
  
    /**
     * Reset the state.  This needs to be called after a process() call 
     * is invoked, if the processor is to be used again.
     */
    virtual void
    reset() = 0;
  
    /**
     * Given an input source, get the source tree.
     *
     * @param inputSource pointer to input source
     * @return source tree
     */
    virtual XalanNode*
    getSourceTreeFromInput(const XSLTInputSource&   inputSource) = 0;

    /**
     * Retrieve the root stylesheet.
     * 
     * @return pointer to root stylesheet
     */
    virtual const StylesheetRoot*
    getStylesheetRoot() const = 0;

    /**
     * Set the root stylesheet.
     * 
     * @param theStylesheet pointer to new root stylesheet
     */
    virtual void
    setStylesheetRoot(const StylesheetRoot*     theStylesheet) = 0;

    /**
     * Set the execution context. Must be set if after calling
     * setStylesheetRoot.
     * 
     * @param theExecutionContext pointer to new execution context.
     */
    virtual void
    setExecutionContext(StylesheetExecutionContext*     theExecutionContext) = 0;

    /**
     * Resolve the params that were pushed by the caller.
     */
    virtual void
    resolveTopLevelParams(StylesheetExecutionContext&   executionContext) = 0;

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
     * @param theValue A string for returning the new prefix
     */
    virtual void
    getUniqueNamespaceValue(XalanDOMString&     theValue) = 0;

   /**
    * Set a top-level parameter, which the stylesheet can access
    * with a top-level xsl:param.  Top-level params are "sticky,"
    * and must be removed with a call to clearStylesheetParams().
    *
    * @param key   name of the parameter
    * @param value XObject value for parameter
    */
   virtual void
   setStylesheetParam(
            const XalanDOMString&   key,
            XObjectPtr              value) = 0;

  /**
    * Set a top-level parameter, which the stylesheet can access
    * with a top-level xsl:param.  Top-level params are "sticky,"
    * and must be removed with a call to clearStylesheetParams().
    *
    * @param key name of the param
    * @param expression expression that will be evaluated
    */
    virtual void
    setStylesheetParam(
            const XalanDOMString&   key,
            const XalanDOMString&   expression) = 0;

  /**
    * Clear any stylesheet params.
    */
    virtual void
    clearStylesheetParams() = 0;

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
    setFormatterListener(FormatterListener*     flistener) = 0;  

    /**
     * Determine the number of trace listeners.
     * 
     * @return number of listeners
     */
    virtual size_type
    getTraceListeners() const = 0;

    /**
     * Add a trace listener for the purposes of debugging and diagnosis.
     * 
     * @param tl pointer to listener to add
     */
    virtual void
    addTraceListener(TraceListener*     tl) = 0;

    /**
     * Remove a trace listener.
     *
     * @param tl Trace listener to be removed.
     */
    virtual void
    removeTraceListener(TraceListener*  tl) = 0;

    /**
     * Fire a generate event.
     * 
     * @param ge generate event to fire
     */
    virtual void
    fireGenerateEvent(const GenerateEvent&  ge) = 0;

    /**
     * Fire a trace event.
     * 
     * @param te trace event to fire
     */
    virtual void
    fireTraceEvent(const TracerEvent&   te) = 0;

    /**
     * Fire a selection event.
     * 
     * @param se selection event to fire
     */
    virtual void
    fireSelectEvent(const SelectionEvent&   se) = 0;

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
     * @param executionContext The current execution context
     * @param theStylesheetElement The executing stylesheet element
     * @param nl The list of selected nodes
     * @param xpath A pointer to the XPath which generated the list of nodes, if any.
     */
    virtual void
    traceSelect(
            StylesheetExecutionContext&     executionContext,
            const ElemTemplateElement&      theStylesheetElement,
            const NodeRefListBase&          nl,
            const XPath*                    xpath) const = 0;

    /**
     * If the quietConflictWarnings property is set to 
     * true, warnings about pattern conflicts won't be 
     * printed to the diagnostics stream.
     * True by default.
     *
     * @param b true if conflict warnings should be suppressed.
     */
    virtual void
    setQuietConflictWarnings(bool b) = 0;

    /**
     * If this is set, diagnostics will be 
     * written to the m_diagnosticsPrintWriter stream. If 
     * the value is null, then diagnostics will be turned 
     * off.
     *
     * @param pw pointer to print writer
     */
    virtual void
    setDiagnosticsOutput(PrintWriter* pw) = 0;
};



XALAN_CPP_NAMESPACE_END



#endif  // XALAN_XSLTPROCESSOR_HEADER_GUARD
