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
#if !defined(XALAN_XSLTENGINEIMPL_HEADER_GUARD)
#define XALAN_XSLTENGINEIMPL_HEADER_GUARD

/**
 * 
 * $Id$
 * 
 * $State$
 * 
 * @author Scott Boag (scott_boag@lotus.com)
 * 
 * It is the responsibility of the XSLTEngineImpl class to traverse the 
 * source DOM tree according to the instructions in the style DOM tree, 
 * and output nodes into the result DOM tree.
 * 
 * Basic flow of control:
 * Starting from process(Document sourceTree, Document out, String mode),
 * 1)  init the Stylesheet instances (which hold the state for each imported 
 *     XSL stylesheet) in the parseXSLRoot method, then 
 * 2)  locate the rootRule (all rules are found via Stylesheet.findTemplate), 
 *     then pass the root node of the source tree to 
 * 3)  buildResultFromTemplate, which processes each of the template's 
 *     child elements by calling 
 * 4)  processTemplateElement, which either processes the element 
 *     by handling it as an XSL directive (if it is in the xsl: namespace), 
 *     or by cloning the source node to the result tree.  
 *     If it is an XSL instruction,
 * 5)  processXSLTemplateInstruction is called, which calls various 
 *     subfunctions according to the tag name.
 *     The XSL directives that effect the source traversal are xsl:apply-templates 
 *     and xsl:for-each, which are handled in the 
 * 6)  handleApplyTemplatesInstruction, which selects the start node and 
 *     calls the 
 * 7)  transformSelectedChildren method, which performs a query or just 
 *     traverses the child list, then for each node in the selected list calls 
 * 8)  transformChild, which then calls 
 * 9)  Stylesheet.findTemplate (if it is not an xsl:for-each instruction)
 *     to locate the corresponding template, and then calls 
 * 10) buildResultFromTemplate, recursing back in to step 3 and starting the 
 *     traversal cycle all over again.
 * 
 * Glossary (abbreviations used in variable and method names in square
 * brackets):
 * 
 *   XSL Instruction [instruction]
 *     Any tag with an XSL namespace prefix.
 *   XSL Template Instruction [instruction]
 *     Any tag with an XSL namespace prefix
 *         that occurs inside an xsl:template element.
 *   Template Child [templateChild]
 *       Any node that is a child of an xsl:template element.
 *   Source Tree
 *     The tree input to the XSL process.
 *   Result Tree
 *     The tree that is output by the XSL process.
 *   Stylesheet Tree
 *     The stylesheet tree produced from the XSL file.
 *   Pattern List [patternList]
 *       A parsed query or match pattern.
 * 
 * 
 */


// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"



// Base class
#include "XSLTProcessor.hpp"



// Standard library headers
#include <cassert>
#include <ctime>
#if defined(XALAN_HASH_CONTAINERS_AVAILABLE)
#include <hash_map>
#endif
#include <map>
#include <set>

#if defined(__GNUC__)
#include <stdexcept>
#endif



// Xerces DOM header file
#include <dom/DOM_Document.hpp>
#include <dom/DOMString.hpp>

#include <sax/DocumentHandler.hpp>



// XSL header files.
#include <PlatformSupport/AttributeListImpl.hpp>
#include <PlatformSupport/DOMStringHelper.hpp>

#include <XPath/MutableNodeRefList.hpp>
#include <XPath/NameSpace.hpp>

#include "Arg.hpp"
#include "KeyDeclaration.hpp"
#include "ProblemListener.hpp"



// Forward definitions
class DispatcherFactory;
class ElemAttributeSet;
class Formatter;
class GenerateEvent;
class InputSource;
class PrintWriter;
class ResultTreeFragBase;
class StackEntry;
class Stylesheet;
class StylesheetConstructionContext;
class StylesheetExecutionContext;
class StylesheetRoot;
class XLocator;
class XMLParserLiaison;
class XMLURL;
class XObject;
class XObjectObject;
class XPathEnvSupport;
class XPathFactory;
class XPathProcessor;
class XPathSupport;
class XSLTResultTarget;


/**
 * It's the responsibility of the XSLTEngineImpl class, collaborating with the
 * XML parser liaison, the DOM, and the XPath engine, to transform a source
 * tree of nodes into a result tree according to instructions and templates
 * specified by a stylesheet tree.  The process method is the primary
 * public entry points.
 * 
 * 
 * If you reuse the processor instance, you should call reset() between calls.
 * 
 * 
 * @author   Scott Boag
 * @author David N. Bertoni (david_n_bertoni@lotus.com)
 */

class XALAN_XSLT_EXPORT XSLTEngineImpl : public XSLTProcessor, 
		private DocumentHandler
{
    
public:

class StackGuard;

#if defined(XALAN_NO_NAMESPACES)
#	define XALAN_STD
#else
#	define XALAN_STD std::
#endif

typedef XALAN_STD auto_ptr<XPathProcessor>    XPathProcessorPtrType;
typedef XALAN_STD map<DOMString, XObject*>    TopLevelVariablesMapType;
typedef XALAN_STD map<DOMString, int>         AttributeKeysMapType;
typedef XALAN_STD map<DOMString, int>         ElementKeysMapType;
typedef XALAN_STD map<const void*, clock_t>   DurationsTableMapType;
typedef XALAN_STD set<DOMString>              TranslateCSSSetType;
typedef XALAN_STD vector<Arg>                 ParamVectorType;
typedef XALAN_STD vector<ElemAttributeSet*>   AttrStackType;
typedef XALAN_STD vector<KeyDeclaration>      KeyDeclarationVectorType;
typedef XALAN_STD vector<Locator*>            LocatorStack;
typedef XALAN_STD vector<NameSpace>           NamespaceVectorType;
typedef XALAN_STD vector<NamespaceVectorType> NamespacesStackType;
typedef XALAN_STD vector<StackEntry*>         VariableStackStackType;
typedef XALAN_STD vector<StackGuard>          StackGuardStackType;
typedef XALAN_STD vector<TraceListener*>      TraceListenerVectorType;
typedef XALAN_STD vector<XObject*>            XObjectPtrVectorType;
typedef XALAN_STD vector<bool>                BoolVectorType;
typedef XALAN_STD runtime_error               RuntimeError;

#if defined(XALAN_HASH_CONTAINERS_AVAILABLE)
	typedef hash_map<DOM_Node, int, DOM_NodeHashFunction>	XSLDirectiveMapType;
	typedef hash_map<DOMString,
					 MutableNodeRefList,
					 DOMStringHashFunction,
					 DOMStringEqualsFunction>	NodeListMapType;
	typedef hash_map<DOMString,
					 NodeListMapType,
					 DOMStringHashFunction,
					 DOMStringEqualsFunction>	KeysMapType;
	typedef hash_map<DOMString,
					 DOM_Node,
					 DOMStringHashFunction,
					 DOMStringEqualsFunction>	NamedTemplatesMapType;
	typedef hash_map<DOMString,
					 Stylesheet*,
					 DOMStringHashFunction,
					 DOMStringEqualsFunction>		StylesheetMapType;
#else
	typedef XALAN_STD map<DOM_Node, int>							XSLDirectiveMapType;
	typedef XALAN_STD map<DOMString,
					 MutableNodeRefList>	NodeListMapType;
	typedef XALAN_STD map<DOMString,
					 NodeListMapType>		KeysMapType;
	typedef XALAN_STD map<DOMString,
					 DOM_Node>	NamedTemplatesMapType;
	typedef XALAN_STD map<DOMString,
					 Stylesheet*>		StylesheetMapType;
#endif

#undef XALAN_STD

	/**
	 * Construct an XSL processor that can call back to a XML processor, so it
	 * can handle included files, and the like. You must create a new instance
	 * of the XSLTEngineImpl class each time you run a transformation.
	 *
	 * @param XMLParserLiaison liaison to an object that can help with a
	 *                         specific parser implementation
	 * @param xpathSupport     $$$
	 * @param xpathEnvSupport  $$$
	 * @param xobjectFactory   object responsible for XObject objects
	 * @param xpathFactory     object responsible for XPath objects
	 */
	XSLTEngineImpl(
			XMLParserLiaison&	parserLiaison,
			XPathSupport&		xpathSupport,
			XPathEnvSupport&	xpathEnvSupport,
			XObjectFactory&		xobjectFactory,
			XPathFactory&		xpathFactory);

	virtual ~XSLTEngineImpl();

	/**
	 * Perform initialization of statics -- must be called before any
	 * processing occurs
	 */
	static void Initialize();

	// These methods are inherited from XSLTProcessor ...
	
	virtual void
	process(
			XSLTInputSource*				inputSource, 
	        XSLTInputSource*				stylesheetSource,
	        XSLTResultTarget&				outputTarget,
			StylesheetConstructionContext&	constructionContext,
			StylesheetExecutionContext&		executionContext);
	
	virtual StylesheetRoot*
	processStylesheet(
			XSLTInputSource&				stylesheetSource,
			StylesheetConstructionContext&	constructionContext);

	virtual StylesheetRoot*
	processStylesheet(
			const DOMString&				xsldocURLString,
			StylesheetConstructionContext&	constructionContext);

	virtual const DOM_Node getSourceTreeFromInput(XSLTInputSource *inputSource);

	virtual void
	outputToResultTree(
			const XObject&	xobj);

	virtual XObject*
	getTopLevelVariable(const DOMString&	theName) const;

	virtual void
	resetCurrentState(
			const DOM_Node&		sourceTree,
			const DOM_Node&		xmlNode);

	virtual DOM_Document
	getRootDoc() const;

	virtual void
	setRootDoc(const DOM_Document& doc);

	virtual DOMString
	evaluateAttrVal(
			const DOM_Node&			contextNode,
			const DOM_Element&		namespaceContext,
			const DOMString&		stringedValue,
			XPathExecutionContext&	executionContext);

	virtual ResultTreeFragBase*
	createResultTreeFrag(
			StylesheetExecutionContext&		executionContext,
			const ElemTemplateElement&		templateChild,
			const DOM_Node&					sourceTree,
			const DOM_Node&					sourceNode,
			const QName&					mode);

	virtual ResultTreeFragBase*
	createResultTreeFrag() const;

	virtual void
	resolveTopLevelParams(StylesheetExecutionContext&	executionContext);

	virtual XMLParserLiaison&
	getXMLParserLiaison() const;

   virtual XObject*
   createXResultTreeFrag(const ResultTreeFragBase&  r) const;

	virtual XObject*
	getVariable(const QName& qname) const;

	virtual XObject*
	getParamVariable(const QName&	theName) const;

	virtual void
	pushVariable(
			const QName&		name,
			XObject*			var,
			const DOM_Node&		element);

	virtual void setStylesheetParam(	
					const DOMString& key,
					const DOMString& expression);
	
	virtual void
	setStylesheetParam(
			const DOMString&	key,
			XObject*			value);

   virtual const NodeRefListBase*
   getNodeSetByKey(
			const DOM_Node&			doc, 
			const DOMString&		name, 
			const DOMString&		ref, 
			const PrefixResolver&	resolver,
			XPathExecutionContext&	executionContext) const;

	virtual bool
	shouldStripSourceNode(const DOM_Node&	textNode) const;

	virtual FormatterListener*
	getFormatterListener() const;

	virtual void
	setFormatterListener(FormatterListener*	flistener);

	virtual void addTraceListener(TraceListener* tl);
	
	virtual void
	setTraceTemplates(bool	b);

	bool isTraceSelect() const;

	virtual void
	setTraceSelect(bool	b);
  
	virtual void
	setTraceTemplateChildren(bool	b);

	virtual void
	setQuietConflictWarnings(bool	b)
	{
		m_quietConflictWarnings = b;
	}

	virtual void removeTraceListener(TraceListener* tl);
	
	virtual void
	setDiagnosticsOutput(PrintWriter*	pw);


	/**
	 * Read in the XML file, either producing a Document or calling SAX events,
	 * and register the document in a table.  If the document has already been
	 * read in, it will not be reparsed.
	 *
	 * @param url location of the XML
	 * @param docHandler pointer to SAX event handler
	 * @param docToRegister if using a SAX event handler, the object to register in the source docs table. 
	 * @return document object, which represents the parsed XML
	 * @exception SAXException
	 */
	DOM_Document
	parseXML(
			const XMLURL&		url,
			DocumentHandler*	docHandler, 
			const DOM_Document& docToRegister);

	/**
	 * Reset the state of the XSL processor by reading in a new XSL stylesheet
	 * from a processing instruction.
	 *
	 * @param xslURLString        valid URI to an XSL stylesheet
	 * @param fragBase            base of tree if URI represents document
	 *                            fragment
	 * @param xmlBaseIdent        base identifier for stylesheet URI
	 * @param isRoot              true if stylesheet is root of tree
	 * @param constructionContext context for construction of object
	 * @return pointer to stylesheet
	 */
	Stylesheet*
	getStylesheetFromPIURL(
			const DOMString&				xslURLString,
			const DOM_Node&					fragBase,
			const DOMString&				xmlBaseIdent,
			bool							isRoot,
			StylesheetConstructionContext&	constructionContext);


	/**
	 * Flush the pending element.
	 */
	void flushPending();

	/**
	 * Bottleneck addition of result tree attributes, so I can keep 
	 * track of namespaces.
	 */
	void addResultNamespaceDecl(const DOMString& prefix, 
	                                   const DOMString& namespaceVal);

	/**
	 * Bottleneck addition of result tree attributes, so I can keep 
	 * track of namespaces.
	 */
	void
	addResultAttribute(
			AttributeListImpl&	attList,
			const DOMString&	aname,
			const DOMString&	value);

	/**
	 * Bottleneck addition of result tree attributes, so I can keep 
	 * track of namespaces.
	 */
	void
	addResultAttribute(
			const DOMString&	aname,
			const DOMString&	value)
	{
		addResultAttribute(m_pendingAttributes,
						   aname,
						   value);
	}

	virtual void
	characters(
			const XMLCh* const	ch,
			const unsigned int	start,
			const unsigned int	length);

	virtual void
	startElement(
			const XMLCh* const	name);

	virtual void
	charactersRaw(
			const XMLCh* const	ch,
			const unsigned int	start,
			const unsigned int	length);

	virtual void
	comment(
			const XMLCh* const	data);

	virtual void
	entityReference(
			const XMLCh* const	name);

	virtual void
	cdata(
			const XMLCh* const	ch,
			const unsigned int 	start,
			const unsigned int 	length);

	// These methods are inherited from DocumentHandler ...
	
	virtual void setDocumentLocator( Locator* const	locator);

	virtual void
	startDocument();

	virtual void
	endDocument();
    
	virtual void
	startElement(
			const XMLCh* const	name,
			AttributeList&		atts);

	virtual void
	endElement(
			const XMLCh* const	name);

    virtual void characters (
        const   XMLCh* const    chars,
        const unsigned int    length);

	virtual void
	ignorableWhitespace(
			const XMLCh* const	ch,
			const unsigned int	length);

	virtual void
	processingInstruction(
			const XMLCh* const	target,
			const XMLCh* const	data);

	virtual void resetDocument();

  /**
   * Clone an element with or without children.
   * TODO: Fix or figure out node clone failure!
   * @exception XSLProcessorException thrown if the active ProblemListener and XMLParserLiaison decide 
   * the error condition is severe enough to halt processing.
   */
	void
	cloneToResultTree(
			const DOM_Node&		node, 
			bool				isLiteral,
			bool				overrideStrip,
			bool				shouldCloneAttributes);

  /**
   * Given a result tree fragment, walk the tree and
   * output it to the result stream.
   */
	void
	outputResultTreeFragment(const XObject&		theTree);


	/**
	 * Execute the function-available() function.
	 * @param context The current source tree context node.
	 * @param opPos The current position in the m_opMap array.
	 * @param args The function args.
	 * @returns whether the given function is available or not.
	 */
	bool
	functionAvailable(
			DOMString&	theNamespace, 
			DOMString&	extensionName) const;
	
	/**
	 * Handle an extension function.
	 */
	virtual XObject*
	extFunction(
			XPathExecutionContext&			executionContext,
			const DOMString&				theNamespace,
			const DOMString&				extensionName, 
			const XObjectPtrVectorType&	argVec) const;

	// This is public for class Stylesheet...
	/**
	 * In response to xsl:function, set up a function that can 
	 * be called from the expression language.
	 * @exception XSLProcessorException thrown if the active ProblemListener and XMLParserLiaison decide 
	 * the error condition is severe enough to halt processing.
	 */
	void
	handleFunctionsInstruction(
			DOM_Element			extensionElem);

	StylesheetRoot*
	getStylesheetRoot() const
	{
		return m_stylesheetRoot;
	}

	void
	setStylesheetRoot(StylesheetRoot*	theStylesheet)
	{
		m_stylesheetRoot = theStylesheet;
	}

	/**
	 * Table of stylesheet documents.
	 * Document objects are keyed by URL string.
	 */
	StylesheetMapType	m_stylesheets;

  /**
   * Get table of stylesheet documents.
   * Document objects are keyed by URL string.
   */
	StylesheetMapType&
	getStylesheetsTable()
	{
		return m_stylesheets;
	}

	/**
	 * The root document.
	 */
	DOM_Document			m_rootDoc;


	static const DOMString&
	getXSLNameSpaceURLPre()
	{
		return s_XSLNameSpaceURLPre;
	}

	const DOMString&
	getXSLNameSpaceURL() const
	{
		return m_XSLNameSpaceURL;
	}

	static const DOMString&
	getXSLT4JNameSpaceURL()
	{
		return s_XSLT4JNameSpaceURL;
	}

	void
	setXSLNameSpaceURL(const DOMString&		theXSLNameSpaceURL)
	{
		m_XSLNameSpaceURL = theXSLNameSpaceURL;
	}

	static double getXSLTVerSupported();

	static const DOMString&
	getXalanXSLNameSpaceURL();


	static const AttributeKeysMapType&
	getAttributeKeys()
	{
		return s_attributeKeys;
	}

	static const ElementKeysMapType&
	getElementKeys()
	{
		return s_elementKeys;
	}

	/**
	 * Given an XSL tag name, return an integer token that corresponds to
	 * ELEMNAME_XXX constants defined in Constants.java.
	 *
	 * @param node a probable xsl:xxx element
	 * @return Constants.ELEMNAME_XXX token, or -1 if in xsl 
	 * or Xalan namespace, -2 if not in known namespace
	 */
	int
	getAttrTok(const DOMString&	name) const
	{
		AttributeKeysMapType::const_iterator iter=
			s_attributeKeys.find(name);

		return iter == s_attributeKeys.end() ? -2 : (*iter).second;
	}

  /**
   * Given an XSL tag name, return an integer token
   * that corresponds to ELEMNAME_XXX constants defined 
   * in Constants.java.
   * Note: I tried to optimize this by caching the node to 
   * id lookups in a hash table, but it helped not a bit.
   * I'm not sure that it's spending too much time here anyway.
   * @param node a probable xsl:xxx element.
   * @return Constants.ELEMNAME_XXX token, or -1 if in xsl 
   * or Xalan namespace, -2 if not in known namespace.
   */
	int
	getXSLToken(
			const DOM_Node& node) const;

  /**
   * Find the type of an element using this method. This is slower 
   * than it ought to be... I'll find a way to optimize down the 
   * line if need be.
   * @param node a probable xsl:xxx element.
   * @param tagType Constants.ELEMNAME_XXX token.
   * @return true if node is of tagType.
   */
	//final
	bool
	isXSLTagOfType(
			const DOM_Node&	node,
			int			tagType) const;

  /**
   * The top of this stack should contain the currently processed
   * stylesheet SAX locator object.
   */
	LocatorStack  m_stylesheetLocatorStack;

	void addTraceListenersToStylesheet();

	/**
	 * Fire a generate event.
	 */
	void fireGenerateEvent(const GenerateEvent& te);
	
	/**
	 * Warn the user of an problem.
	 * This is public for access by extensions.
	 *
	 * @exception XSLProcessorException thrown if the active ProblemListener and XMLParserLiaison decide 
	 * the error condition is severe enough to halt processing.
	 */
	void message(const DOMString& msg);

	/**
	 * Warn the user of an problem.
	 * This is public for access by extensions.
	 *
	 * @exception XSLProcessorException thrown if the active ProblemListener and XMLParserLiaison decide 
	 * the error condition is severe enough to halt processing.
	 */
	void message(	const DOM_Node& styleNode,
						const DOM_Node& sourceNode,
						const DOMString& msg);
	
	/**
	 * Tell the user of an error, and probably throw an 
	 * exception.
	 * @exception XSLProcessorException thrown if the active ProblemListener and XMLParserLiaison decide 
	 * the error condition is severe enough to halt processing.
	 */

	bool
	getQuietConflictWarnings() const
	{
		return m_quietConflictWarnings;
	}

  /**
   * Warn the user of an problem.
   * @exception XSLProcessorException thrown if the active ProblemListener and XMLParserLiaison decide 
   * the error condition is severe enough to halt processing.
   */
	virtual void
	warn(const DOMString&	msg) const;

  /**
   * Warn the user of an problem.
   * @exception XSLProcessorException thrown if the active ProblemListener and XMLParserLiaison decide 
   * the error condition is severe enough to halt processing.
   */
	virtual void
	warn(
			const DOM_Node&	styleNode,
			const DOM_Node&	sourceNode,
			const DOMString&	msg) const;

  /**
   * Tell the user of an error, and probably throw an 
   * exception.
   * @exception XSLProcessorException thrown if the active ProblemListener and XMLParserLiaison decide 
   * the error condition is severe enough to halt processing.
   */
	void
	error(const DOMString&	msg) const;

	/**
	 * Tell the user of an error, and probably throw an 
	 * exception.
	 * @exception XSLProcessorException thrown if the active ProblemListener and XMLParserLiaison decide 
	 * the error condition is severe enough to halt processing.
	 */

//@@ HOLD until we figure out exceptions
//	void error(DOMString& msg, Exception e);

  /**
   * Tell the user of an error, and probably throw an 
   * exception.
   * @exception XSLProcessorException thrown if the active ProblemListener and XMLParserLiaison decide 
   * the error condition is severe enough to halt processing.
   */
	void
	error(
			const DOM_Node&	styleNode,
			const DOM_Node&	sourceNode,
			const DOMString&	msg) const;


	/**
	 * Mark the time, so that displayDuration can later
	 * display the elapsed clock ticks.
	 */
	void
	pushTime(const void*	key) const;

	/**
	 * Returns the duration since pushTime was called,
	 * in in clock ticks.
	 */
	clock_t
	popDuration(const void*		key) const;

	/**
	 * Display the duration since pushTime was called.
	 */
	void
	displayDuration(
			const DOMString&	info,
			const void*			key) const;

	/*
	 * Rather than exposing the member and testing it as in java
	 */
	bool doDiagnosticsOutput()
	{	return 0 != m_diagnosticsPrintWriter; }

	/**
	 * Bottleneck output of diagnostics.
	 */
	void
	diag(
			const DOMString&	s) const;
  
	/**
	 * Print a trace of a selection being made.
	 */
	void traceSelect(
			const DOM_Element&		theTemplate,
			const NodeRefListBase&	nl) const;

	virtual const DOMString
	getUniqueNSValue() const;
	
	/**
	 * Tell if a given element name should output it's text 
	 * as cdata.
	 */
	bool isCDataResultElem(const DOMString& elementName);
	
	/**
	 * Tell if a qualified name equals the current result tree name.
	 */
	bool qnameEqualsResultElemName(const QName& qname, const DOMString& elementName);
	
	  
	/**
	 * Given a prefix, return the namespace, 
	 */
	DOMString
	getResultNamespaceForPrefix(
			const DOMString&	prefix) const;
  
	/**
	 * This should probably be in the XMLParserLiaison interface.
	 */
	DOMString
	getResultPrefixForNamespace(
			const DOMString&	theNamespace) const;

	/**
	 * Evaluate an xpath string and return the result.
	 */
	double
	evalMatchPatternStr(
			const DOMString&		str,
			const DOM_Node&			context,
			XPathExecutionContext&	executionContext);

	/**
	 * Create and initialize an xpath and return it.
	 */
	XPath*
	createMatchPattern(
			const DOMString&		str,
			const PrefixResolver&	resolver);

	/**
	 * Copy xmlns: attributes in if not already in scope.
	 */
	void
	copyNamespaceAttributes(
			const DOM_Node&			src,
			bool					srcIsStylesheetTree);

	/**
	 * Evaluate an xpath string and return the result.
	 */
	XObject*
	evalXPathStr(
			const DOMString&		str,
			XPathExecutionContext&	executionContext);

	/**
	 * Evaluate an xpath string and return the result.
	 */
	XObject*
	evalXPathStr(
			const DOMString&		str,
			const DOM_Node&			contextNode,
			const PrefixResolver&	prefixResolver,
			XPathExecutionContext&	executionContext);

	/**
	 * Evaluate an xpath string and return the result.
	 */
	XObject*
	evalXPathStr(
			const DOMString&		str,
			const DOM_Node&			contextNode,
			const DOM_Element&		prefixResolver,
			XPathExecutionContext&	executionContext);

	/**
	 * Create an XPath whose lifetime can end after transforming
	 * is complete.  Do not use this to create an XPath to be
	 * held by a stylesheet.
	 */
	virtual XPath*
	createProcessingXPath(
		const DOMString&		str,
		XPathExecutionContext&	executionContext,
		const PrefixResolver&	resolver);

  /**
   * Given a valid element id, return the corresponding element.
   */
	DOM_Element
	getElementByID(
			const DOMString&		id,
			const DOM_Document&		doc) const;


	/**
	 * Push a context marker onto the stack to let us know when 
	 * to stop searching for a var.
	 */
	void
	pushContextMarker(
			const DOM_Node&	caller,
			const DOM_Node&	sourceNode)
	{
		m_variableStacks.pushContextMarker(caller, sourceNode);
	}

	/**
	 * Pop the current context from the current context stack.
	 */
	void
	popCurrentContext()
	{
		m_variableStacks.popCurrentContext();
	}

	void
	pushParams(
			StylesheetExecutionContext&		executionContext,
			const ElemTemplateElement&		xslCallTemplateElement,
			const DOM_Node&					sourceTree, 
			const DOM_Node&					sourceNode,
			const QName&					mode,
			const DOM_Node&					targetTemplate)
	{
		m_variableStacks.pushParams(executionContext,
									xslCallTemplateElement,
									sourceTree,
									sourceNode,
									mode,
									targetTemplate);
	}

	/**
	 * Get an Expression object from a select node.  Does not cache!
	 */
	XPath* getExpression(
					AttributeList& attrs,
					const DOMString& key,
					const PrefixResolver& resolver);

	/**
	 * Given an element, return an attribute value in 
	 * the form of a string.
	 * @param el The element from where to get the attribute.
	 * @param key The name of the attribute.
	 * @param contextNode The context to evaluate the 
	 * attribute value template.
	 * @return Attribute value.
	 */
	DOMString
	getAttrVal(
			const DOM_Element&	el,
			const DOMString&	key,
			const DOM_Node&		contextNode);
  
	/**
	 * Given an element, return an attribute value in 
	 * the form of a string.
	 */
	static DOMString
	getAttrVal(
			const DOM_Element&	el,
			const DOMString&	key);



  /**
   * Copy an attribute to the created output element, executing 
   * attribute templates as need be, and processing the xsl:use 
   * attribute.
   * @exception XSLProcessorException thrown if the active ProblemListener and XMLParserLiaison decide 
   * the error condition is severe enough to halt processing.
   */
	void
	copyAttributeToTarget(
			const DOM_Attr&		attr,
			const DOM_Node&		contextNode,
			const Stylesheet*	stylesheetTree,
			AttributeListImpl&	attrList, 
            const DOM_Element&	namespaceContext);
  
  /**
  * Translate CSS attributes on the output to a "style" attribute, for 
  * old submission support.  You probably shouldn't rely on this, though
  * I personally like it a lot.
  * @param b If true, translate CSS attributes on the output to a 
  * "style" attribute.
  * @deprecated
  */
	bool
	getTranslateCSS() 
	{
		return m_translateCSS;
	} 

	void
	setTranslateCSS(bool	b) 
	{
		m_translateCSS = b;
	} 

	/**
	 * This changes the default of the default-space attribute.
	 * @param b Sets the default of the default-space attribute 
	 * to "strip" if true, or "preserve" if false.
	 */
	bool
	getStripWhiteSpace() const
	{
		return m_stripWhiteSpace;
	}

	void
	setStripWhiteSpace(bool	fStrip)
	{
		m_stripWhiteSpace = fStrip;
	}

  /**
   * indent-result is by default no, which means an XSL processor must not
   * change the whitespace on output.
   */
	//bool	m_indentResult;
  
  /**
   * If true, output carriage returns.
   */
	bool	m_outputCarriageReturns;

  /**
   * If true, output linefeeds.
   */
	bool	m_outputLinefeeds;


  /**
   * (Code stolen and modified from XML4J)
   * Conditionally trim all leading and trailing whitespace in the specified String.  
   * All strings of white space are 
   * replaced by a single space character (#x20), except spaces after punctuation which 
   * receive double spaces if doublePunctuationSpaces is true.
   * This function may be useful to a formatter, but to get first class
   * results, the formatter should probably do it's own white space handling 
   * based on the semantics of the formatting object.
   * @param   string      String to be trimmed.
   * @param   trimHead    Trim leading whitespace?
   * @param   trimTail    Trim trailing whitespace?
   * @param   doublePunctuationSpaces    Use double spaces for punctuation?
   * @return              The trimmed string.
   */
	DOMString fixWhiteSpace(
			const DOMString&	string, 
			bool				trimHead, 
			bool				trimTail, 
			bool				doublePunctuationSpaces);
  
  /**
   * Control if carriage returns are put in the result tree.
   * Default is to output carriage returns.
   * @param b If true, output carriage returns.
   */
	void setOutputCarriageReturns(bool	b)
	{
		m_outputCarriageReturns = b;
	}

  /**
   * Control if linefeeds are put in the result tree.
   * Default is to output linefeeds.
   * @param b If true, output linefeeds.
   */
	void setOutputLinefeeds(bool		b)
	{
		m_outputLinefeeds = b;
	}

  /**
   * Normalize the linefeeds and/or carriage returns to 
   * be consistently 0x0D 0x0A.  This should almost 
   * certainly be done somewhere else... like in the 
   * XML parser.
   */
	DOMString getNormalizedText(const DOM_Text&	tx) const;


  /**
	* Set the stylesheet root.  If this is set, then the process calls that
	* take only the input .xml will use this instead of looking for a
	* stylesheet PI.
   */
   void setStylesheet(StylesheetRoot *stylesheetRoot) { m_stylesheetRoot = stylesheetRoot; }
 
  /**
	* Set the stylesheet root.  If this is set, then the process calls that
	* take only the input .xml will use this instead of looking for a
	* stylesheet PI.
   */
   StylesheetRoot *getStylesheet()
	{
		return m_stylesheetRoot;
	}

  /**
	* Get the filename of the output document, if it was set. This is for use
	* by multiple output documents, to determine the base directory for the
	* output document.  It needs to be set by the caller.
   */
   DOMString &getOutputFileName()
	{
		return m_outputFileName;
	}

  /**
	* Set the filename of the output document. This is for use by multiple
	* output documents, to determine the base directory for the output
	* document.  It needs to be set by the caller.
   */
   void setOutputFileName(const DOMString&	filename)
	{
		m_outputFileName = filename;
	}
  


	/**
	 * Get the factory for making xpaths.
	 */
	XPathFactory& getXPathFactory()
	{
		return m_xpathFactory;
	}

	/**
	 * Get the XPath processor object.
	 * @return The XPathProcessor interface being used.
	 */
	 XPathProcessor& getXPathProcessor()
	 {
		 return *m_xpathProcessor.get();
	 }


	/**
	 * Given a document, get the default stylesheet URI from the 
	 * xsl:stylesheet PI.  However, this will only get you the 
	 * first URL, and there may be many.
	 * @deprecated
	 */
	 DOMString getStyleSheetURIfromDoc(const DOM_Node&	sourceTree);
	  

  /**
   * Given a classID and codetype, tr to register a code dispatcher.
   */
	void
	registerExtensionHandlerByName(
			const DOMString&	classid,
			const DOMString&	codetype);
  
  /**
   * Register the given DispatcherFactor for a given mime type.
   */
	void
	registerExtensionHandler(
			const DOMString&	mimeType,
			DispatcherFactory*	factory);


  /**
   * Do everything possible to get a good URL from a string.
   */
    /**
   * Take a user string and try to parse XML, and also return 
   * the url.
   */
	XMLURL*
	getURLFromString(const DOMString&	urlString) const;

	XMLURL*
	getURLFromString(const DOMString&	urlString, const DOMString& base) const;

	/**
	 * The formatter interface, which has the toMarkup 
	 * method, and which will eventually hold other non-event 
	 * methods.  Not to be confused with the DocumentHandler
	 * interface.
	*/
	// @@ JMD: Temporarily public
	Formatter*			m_formatter;

	XPathSupport& getXPathSupport()
	{
		return m_xpathSupport;
	}

	XPathEnvSupport& getXPathEnvSupport()
	{
		return m_xpathEnvSupport;
	}


	/**
	 * Set the formatter interface, which has the toMarkup 
	 * method, and which will eventually hold other non-event 
	 * methods.  Not to be confused with the DocumentHandler
	 * interface.
	 */
	void
	setFormatter(Formatter*		formatter);

  /**
   * The factory that will be used to create result tree fragments.
   */
	mutable DOM_Document	m_resultTreeFactory;


  /**
   * The namespace that the result tree conforms to.  A null value 
   * indicates that result-ns is not used and there is no checking. 
   * A empty string indicates that the result tree conforms to the 
   * default namespace.
   */
	DOMString	m_resultNameSpacePrefix;

  /**
   * The URL that belongs to the result namespace.
   */
	DOMString	m_resultNameSpaceURL;

	const MutableNodeRefList& getContextNodeList() { return m_contextNodeList; }

	void setContextNodeList(const MutableNodeRefList& ref)
	{
		m_contextNodeList = ref;		
	}

	void
	addKeyDeclaration(const KeyDeclaration&		theDeclaration)
	{
		m_keyDeclarations.push_back(theDeclaration);

		setNeedToBuildKeysTable(true);
	}

	// $$$ ToDO: Are these next two really needed?
	/**
	 * Set the table of KeyDeclaration objects, which are set by the 
	 * xsl:key element.
	 * @param table Table of IDAttributeHolder objects.
	 */
	void
	setKeyDeclarationsTable(const KeyDeclarationVectorType&		table)
	{
		m_keyDeclarations = table;
	}

	/**
	 * Get the table of KeyDeclaration objects.
	 * @return Table of KeyDeclaration objects.
	 */
	const KeyDeclarationVectorType&
	getKeyDeclarations() const
	{
		return m_keyDeclarations;
	}

	/**
	 * Set the table of keys.  This will be used by the 
	 * key() and keyref() functions for lookup of a nodelist.
	 */
	void
	setKeyTable(const KeysMapType&	table)
	{
		m_keys = table;
	}

	/**
	 * Get the table of element IDs.	This table is used by the 
	 * id() functions for lookup of an element ID.
	 * @return Table of element IDs, keyed by ID string, with 
	 * values that are Element nodes.
	 */
	const KeysMapType&
	getKeysTable()
	{
		return m_keys;
	}

	/**
	 * Set needToBuildKeysTable to true if an xsl:key directive is found.
	 * Mainly for use by the XMLParserLiaison classes for 
	 * optimized processing of keys.
	 */
	void
	setNeedToBuildKeysTable(bool	b)
	{
		m_needToBuildKeysTable = b;
	}

	/**
	 * Set the needToBuildKeysTable property, which, if true, 
	 * will trigger the building of the keys table.
	 * Mainly for use by the XMLParserLiaison classes for 
	 * optimized processing of keys.
	 */
	bool
	getNeedToBuildKeysTable() const
	{
		return m_needToBuildKeysTable;
	}

	void
	addNamedTemplate(const DOMString&	theName,
					 const DOM_Node&	theNode)
	{
		m_namedTemplates[theName] = theNode;
	}

	DOM_Node
	getNamedTemplate(const DOMString&	theName) const
	{
		const NamedTemplatesMapType::const_iterator 	i =
			m_namedTemplates.find(theName);

		if(i != m_namedTemplates.end())
		{
			return (*i).second;
		}
		else
		{
			return DOM_Node();
		}
	}




	/**
	 * Set the problem listener property.
	 * The XSL class can have a single listener that can be informed 
	 * of errors and warnings, and can normally control if an exception
	 * is thrown or not (or the problem listeners can throw their 
	 * own RuntimeExceptions).
	 * @param l A ProblemListener interface.
	 */
	void setProblemListener(ProblemListener*		l)
	{
		m_problemListener = l;
	}

	/**
	 * Get the problem listener property.
	 * The XSL class can have a single listener that can be informed 
	 * of errors and warnings, and can normally control if an exception
	 * is thrown or not (or the problem listeners can throw their 
	 * own RuntimeExceptions).
	 * @return A ProblemListener interface.
	 */
	ProblemListener* getProblemListener() const
	{
		return m_problemListener;
	}


	/**
	 * An exception for that occurs when a given stylesheet
	 * goes into an infinite loop.
	 */
	class XSLInfiniteLoopException : public RuntimeError
	{
	public:
		XSLInfiniteLoopException() : RuntimeError("XSL infinite loop occurred!")
		{ }
	};

	/**
	 * Defines a class to keep track of a stack for macro arguments.
	 */
	class VariableStack
	{
	public:

		/**
		 * Constructor for a variable stack.
		 */
		VariableStack(XSLTEngineImpl&		theProcessor);

		~VariableStack();

		virtual void
		reset();

		/**
		 * Push a context marker onto the stack to let us know when 
		 * to stop searching for a var.
		 */
		void
		pushElementMarker(const DOM_Node&	elem);

		/**
		 * Pop the current context from the current context stack.
		 */
		void
		popElementMarker(const DOM_Node&	elem);

		/**
		 * Check to see if an element marker for the particular
		 * node has already been pushed.
		 */
		bool
		elementMarkerAlreadyPushed(const DOM_Node&	elem) const;

		/**
		 * Push a context marker onto the stack to let us know when 
		 * to stop searching for a var.
		 */
		void
		pushContextMarker(
				const DOM_Node&	caller,
				const DOM_Node&	sourceNode);

		/**
		 * Pop the current context from the current context stack.
		 */
		void
		popCurrentContext();

		/**
		 * Given a template, search for 
		 * the arguments and push them on the stack.  Also, 
		 * push default arguments on the stack.
		 * You <em>must</em> call popContext() when you are 
		 * done with the arguments.
		 */
		void
		pushParams(
				StylesheetExecutionContext&		executionContext,
				const ElemTemplateElement&		xslCallTemplateElement,
				const DOM_Node&					sourceTree, 
				const DOM_Node&					sourceNode,
				const QName&					mode,
				const DOM_Node&					targetTemplate);

		/**
		 * Same as getXObjectVariable, except don't look in the global space.
		 */
		XObject*
		getXObjectParamVariable(const QName& qname) const
		{
			return findXObject(qname, false);
		}

		/**
		 * Tell if there is a param variable on the stack.
		 */
		bool hasParamVariable(QName& qname);

		/**
		 * Same as getResultTreeFragVariable, except don't look
		 * in the global space.
		 */
/*
		@@ JMD: Obsolete??
		ResultTreeFrag*
		getResultTreeFragParamVariable(const QName&	name) const
		{
			return findResultTreeFrag(name, false);
		}
*/

		/**
		 * Given a name, return a pointer to the corresponding
		 * XObject.
		 */
		XObject*
		getXObjectVariable(const QName&		name) const
    	// java: Object getVariable(QName name)
		{
			return findXObject(name, true);
		}

		/**
		 * Push an argument onto the stack.  Don't forget 
		 * to call startContext before pushing a series of 
		 * arguments for a given macro call.
		 */
		void
		pushVariable(
				const QName&		name,
				XObject*			val,
				const DOM_Node&		e);

		/**
		 * Push an argument onto the stack.  Don't forget 
		 * to call startContext before pushing a series of 
		 * arguments for a given macro call.
		 */
/*
@@ JMD: assume we don't need this, see what happens
		void
		pushVariable(
				const QName&			name,
				ResultTreeFrag*				val,
				const ElemTemplateElement&	e);
*/

	private:

		XObject*
		findXObject(
				const QName&	name,
				bool			fSearchGlobalSpace) const;

		const Arg*
		findArg(
				const QName&	name,
				bool			fSearchGlobalSpace) const;

		// $$$ ToDo:  Is this really used?
		/**
		 * Holds caller, so that it may be searched for 
		 * xsl:params, in order to resolve xsl:param-arg.
		 */
		DOM_Element						m_caller;


		VariableStackStackType			m_stack;

		XSLTEngineImpl&					m_processor;
	}; // end VariableStack

	// Give VariableStack access to stuff.
	friend class VariableStack;

	VariableStack&
	getVariableStacks()
	{
		return m_variableStacks;
	}


	/**
	 * Create a document fragment.  This function may return null.
	 */
	ResultTreeFragBase*
	createDocFrag() const;
	



/*
JMD: 
These were inner classes in java:
	class ContextState;
	class ContextMarker extends ContextState;
	class ElementMarker;
 */

	AttributeListImpl&  getPendingAttributes();
	DOMString getPendingElementName() const;
	void setPendingAttributes(const AttributeList&	pendingAttributes) ;
	void setPendingElementName(const DOMString& elementName) ;

	/**
	 * A stack to keep track of the attribute elements.
	 */

	AttrStackType&
	getAttrSetStack()
	{ 
		return m_attrSetStack; 
	}

protected:

	/*
	 * The current input element that is being processed.
	 */
	DOM_Node	m_currentNode;

  /**
   * Given a tag name, an attribute name, and 
   * an attribute value, do a very crude recursive 
   * search and locate the first match.
   */
	static DOM_Element
	findElementByAttribute(
			const DOM_Element&	elem,
			const DOMString&	targetElementName, 
			const DOMString&	targetAttributeName,
			const DOMString&	targetAttributeValue);


  /**
   * Returns whether or not an attribute key 
   * is a CSS2 attribute.
   * @deprecated
   */
	bool
	isCSSAttribute(
			const DOMString&	name) const;

  /**
   * Stuff a hash table with CSS2 keys & dummy values
   * in order to quickly look up if a template child
   * attribute is a CSS attribute or not.
   * Note: This has something in it to screw up the
   * Dev Studio code format command, so that the 
   * last curly bracket goes all the way to the 
   * left.
   * @deprecated
   */
	void
	initCSS2Table();

  /**
   * Copy the attributes from the XSL element to the created 
   * output element, executing attribute templates and 
   * processing the xsl:use attribute as need be.
   * @exception XSLProcessorException thrown if the active ProblemListener and XMLParserLiaison decide 
   * the error condition is severe enough to halt processing.
   */
	void
	copyAttributesToAttList(
			const DOM_Node&		contextNode,
			const Stylesheet*	stylesheetTree,
			const DOM_Element&	templateChild,
			AttributeListImpl&	attList);

  /**
  * Keyed on CSS2 property names, and holding dummy 
  * values for quickly looking up if a result tree element
  * attribute is a CSS attribute or not.
  * @deprecated
  */
	TranslateCSSSetType		m_cssKeys;

 /*
  * If this is true, translate CSS attributes on the output to a "style"
  * attribute.
  */
	bool					m_translateCSS;

	/**
	 * Reset the state.  This needs to be called after a process() call 
	 * is invoked, if the processor is to be used again.
	 */
	virtual void reset();
	
  /**
   * The pending element.  We have to delay the call to 
   * m_flistener.startElement(name, atts) because of the 
   * xsl:attribute and xsl:copy calls.  In other words, 
   * the attributes have to be fully collected before you 
   * can call startElement.
   */

	DOMString	m_pendingElementName;

	/**
	 * The pending attributes.  We have to delay the call to 
     * m_flistener.startElement(name, atts) because of the 
	 * xsl:attribute and xsl:copy calls.  In other words, 
	 * the attributes have to be fully collected before you 
	 * can call startElement.
	 */
	AttributeListImpl	m_pendingAttributes;

	/**
	 * NOTE: This replaces the ResultNameSpace class in java, since it is the
	 * same as the NameSpace class
	 */

	/**
	 * A stack to keep track of the result tree namespaces.
	 */
	NamespacesStackType	m_resultNameSpaces;

	/**
	 * This is pushed on the m_resultNameSpaces stack until a 
	 * xmlns attribute is found.  It's special because it has
	 * and empty prefix and uri field.
	 */
	NameSpace			m_emptyNamespace;
	
	// Factory for creating xpaths.
	XPathFactory&		m_xpathFactory;

	/**
	 * XPath object to use for short evaluations, so we don't have to 
	 * create one every time.
	 */
	XPath* const		m_xpath;

	// Factory for creating xobjects
	XObjectFactory&		m_xobjectFactory;

	// The query/pattern-matcher object.
	XPathProcessorPtrType	m_xpathProcessor;

	/**
	 * Stack of Booleans to keep track of if we should be outputting 
	 * cdata instead of escaped text.
	 * ## Optimization: use array stack instead of object stack.
	 */
	BoolVectorType	m_cdataStack;

private:

	/**
	 * The stack of Variable stacks.  A VariableStack will be 
	 * pushed onto this stack for each template invocation.
	 */
	VariableStack	m_variableStacks;

	/**
	 * The XSL class can have a single listener that can be informed 
	 * of errors and warnings, and can normally control if an exception
	 * is thrown or not (or the problem listeners can throw their 
	 * own RuntimeExceptions).
	 */
	ProblemListener*	m_problemListener;

	/**
	 * This is set to true if an xsl:id directive is found.
	 * Mainly for use by the XMLParserLiaison classes for 
	 * optimized processing of ids.
	 */
	bool	m_needToBuildKeysTable;

	/**
	 * Print a trace of a template that is being called, either by 
	 * a match, name, or as part of for-each.
	 */
	void
	traceTemplate(
			const DOM_Element&		theTemplate) const;

	/**
	 * Print some diagnostics about the current 
	 * template child.
	 */
	void
	diagnoseTemplateChildren(
			const DOM_Node&	templateChild,
			const DOM_Node&	sourceNode) const;

  /**
   * The root of a linked set of stylesheets.
   */
	StylesheetRoot*					m_stylesheetRoot;

	StylesheetExecutionContext*		m_stylesheetExecutionContext;

/**
 * The full XSLT Namespace URL.  To be replaced by the one actually 
 * found.
 */
	DOMString					m_XSLNameSpaceURL;	//"http://www.w3.org/1999/XSL/Transform/1.0"



/**
 * The minimum version of XSLT supported.
 */
	static const double s_XSLTVerSupported;	// 1.0

/**
 * Out version of XSLT
 */
	static const DOMString s_DefaultXSLNameSpaceURL;
/**
 * The namespace that we must match as a minimum for XSLT.
 */
	static const	DOMString	s_XSLNameSpaceURLPre;	// "http://www.w3.org/1999/XSL/Transform"

/**
 * Special Xalan namespace for built-in extensions.
 */
	static const DOMString s_XSLT4JNameSpaceURL; // "http://xml.apache.org/xslt"


/**
 * Hash table that can look up XSLT4J extensions element IDs via name.
 */
	static ElementKeysMapType		s_XSLT4JElementKeys;

	/**
	 * Init the XSLT hashtable.
	 */
//	static void
//	initXSLTKeys();

	/**
	 *  Hash table that can look up xslt element IDs via attribute.
	 */
	static AttributeKeysMapType	s_attributeKeys;

  /**
   *  Hash table that can look up xslt element IDs via name.
   */
	static ElementKeysMapType	s_elementKeys;


	/**
	 * Node to xsl directive table, used so we don't have to keep 
	 * looking up the directives each time.  This isn't of huge 
	 * benifit, but is used because of the cost of getExpandedElementName.
	 */
	// Create a hash table that can look up xsl element IDs via name.
	mutable XSLDirectiveMapType		m_XSLDirectiveLookup;

  /**
   * If the template element is in the xsl:namespace, process the
   * element.
   * @return true if the given XSL template child is an XSL directive.
   * @exception XSLProcessorException thrown if the active ProblemListener and XMLParserLiaison decide 
   * the error condition is severe enough to halt processing.
   */
	bool
	processXSLTemplateInstruction(
			const Stylesheet*	stylesheetTree, 
			DOM_Element			xslInstruction, 
			DOM_Document		sourceTree, 
			const DOM_Node&	sourceNode,
			const DOMString&	mode);

	/**
	 * In response to 'xsl:import', call transformChild, or, if that 
	 * fails, transformChildren.
	 * @exception XSLProcessorException thrown if the active ProblemListener and XMLParserLiaison decide 
	 * the error condition is severe enough to halt processing.
	 */

	/**
	 * If this is set to true, do not warn about pattern 
	 * match conflicts.
	 */
	bool	m_quietConflictWarnings;

	/*
	 * If this is true, then the diag function will 
	 * be called.
	 */
	bool	m_traceTemplateChildren;
  
	/*
	 * If this is true, then the simple tracing of templates 
	 * will be performed.
	 */
	bool	m_traceTemplates;

	/*
	 * If this is true, then diagnostics of each select 
	 * will be performed.
	 */
	bool	m_traceSelects;

	/*
	 * A stream to print diagnostics to.
	 */
	PrintWriter*	m_diagnosticsPrintWriter;

  /* For diagnostics */
	mutable DurationsTableMapType	m_durationsTable;

	/**
	 * List of listeners who are interested in tracing what's 
	 * being generated.
	 */
	/*transient*/ TraceListenerVectorType m_traceListeners;
	
	// Common processing for errors and warnings
	void problem(const DOM_Node& styleNode,
					const DOM_Node& sourceNode,
					const DOMString& msg, 
					ProblemListener::eClassification		classification) const;
	/**
	 * This is for use by multiple output documents, to determine 
	 * the base directory for the output document.  It needs to 
	 * be set by the caller.
	 */
	DOMString m_outputFileName;
	
	/**
	 * Write the children of a stylesheet element to the given listener.
	 * @exception XSLProcessorException thrown if the active ProblemListener and XMLParserLiaison decide 
	 * the error condition is severe enough to halt processing.
	 * @param stylesheetTree The stylesheet object that holds the fragment.
	 * @param templateParent The template element that holds the fragment.
	 * @param sourceTree The source tree document context.
	 * @param sourceNode The current source context node.
	 * @param mode The mode under which the template is operating.
	 * @return An object that represents the result tree fragment.
	 */
	void
	writeChildren(
			FormatterListener*				flistener,
			StylesheetExecutionContext&		executionContext,
	        const ElemTemplateElement&		templateParent,
	        const DOM_Node&					sourceTree,
	        const DOM_Node&					sourceNode,
			const QName&					mode);


  //==========================================================
  // SECTION: Function to do with attribute handling
  //==========================================================

	/**
	 * This is used whenever a unique namespace is needed.
	 */
	mutable int m_uniqueNSValue;	// 0
  
	/**
	 * This should probably be in the XMLParserLiaison interface.
	 */
	DOMString
	getPrefixForNamespace(
			const DOMString&	theNamespace,
			const DOM_Element&	namespaceContext) const;

	bool	m_useATVsInSelects;

	/**
	 * Translate CSS attributes and put them in a style tag.
	 * @deprecated
	 */
	void
	translateCSSAttrsToStyleAttr(
			AttributeListImpl&	attList);
  
	// $$$ ToDo:  This must go!!!!!!
	/**
	 * Evaluate an xpath string and return the result.
	 */
	virtual XPath*
	createXPath(
		const DOMString&		str,
		const PrefixResolver&	resolver);

	/**
	 * Given a name, locate a variable in the current context, and return 
	 * the XObject.
	 * @exception XSLProcessorException thrown if the active ProblemListener and XMLParserLiaison decide 
	 * the error condition is severe enough to halt processing.
	 */
	XObject*
	getXObjectVariable(const DOMString&	name) const;

	/**
	 * Get an XLocator provider keyed by node.  This get's
	 * the association based on the root of the tree that the 
	 * node is parented by.
	 */
	XLocator*
	getXLocatorFromNode(const DOM_Node&		node) const;

	/**
	 * Associate an XLocator provider to a node.  This makes
	 * the association based on the root of the tree that the 
	 * node is parented by.
	 */
	 void
	 associateXLocatorToNode(
			const DOM_Node&		node,
			XLocator*			xlocator);

  /**
  * If this is true, the processor will do the best it can to strip 
  * unwanted white space. This is set in the stylesheet via the default-space 
  * attribute on xsl:stylesheet.
  */
	bool	m_stripWhiteSpace; // default default-space="preserve"

//	typedef std::set<DOMString>		WhitespaceLookupTableType;

	/**
	 * A lookup table of all space preserving elements.
	 */
//	WhitespaceLookupTableType	m_whitespacePreservingElements;

	/**
	 * A lookup table of all space stripping elements.
	 */
//	WhitespaceLookupTableType	m_whitespaceStrippingElements;

  /**
   * Control if the xsl:variable is resolved early or 
   * late. Resolving the xsl:variable
   * early is a drag because it means that the fragment 
   * must be created into a DocumentFragment, and then 
   * cloned each time to the result tree.  If we can resolve 
   * late, that means we can evaluate directly into the 
   * result tree.  Right now this must be kept on 'early' 
   * because you would need to set the call stack back to 
   * the point of xsl:invoke... which I can't quite work out 
   * at the moment.  I don't think this is worth fixing 
   * until NodeList variables are implemented.
   */
	static const bool	m_resolveContentsEarly;

	/**
	 * Set the factory for making XPaths.
	 */
//	void setXPathFactory(XPathFactory* factory) { m_xpathFactory = factory; }

	
	/**
	 * Get a DOM document, primarily for creating result 
	 * tree fragments.
	 */
	DOM_Document
	getDOMFactory() const;

	/**
	 * Set the XPath processor object.
	 * @param processor A XPathProcessor interface.
	 */
//	void setXPathProcessor(XPathProcessor* processor) { m_xpathProcessor = processor; }

	bool
	getResolveContentsEarly() const
	{
		return m_resolveContentsEarly;
	}

	ParamVectorType		m_topLevelParams;

	public:

	/**
	 * Reset the vector of top level parameters
	 */
	void
	clearTopLevelParams()
	{
		m_topLevelParams.clear();
	}

	/**
	 * Returns the current input node that is being 
	 * processed.
	 */
	DOM_Node
	getSourceNode() const
	{
		return m_currentNode;
	}

  /**
   * The liason to the XML parser, so the XSL processor 
   * can handle included files, and the like, and do the 
   * initial parse of the XSL document.
   */
	XMLParserLiaison&	m_parserLiaison;

	XPathSupport&		m_xpathSupport;

	XPathEnvSupport&		m_xpathEnvSupport;

	/**
	 * The listener for formatting events.  This should be 
	 * supplied by the Formatter object.
	 */

	// For now, we have to use the FormatterListener class,
	// since we can't use RTTI to determine what we have.
#if 1
	DocumentHandler*	m_flistener;
#else
	FormatterListener*	m_flistener;
#endif

	/**
	 * This holds the current context node list.  This should arguably be 
	 * passed by parameter.
	 */
	MutableNodeRefList	m_contextNodeList;

	/**
	 * Table of KeyDeclaration objects, which are set by the 
	 * xsl:key element.
	 */
	KeyDeclarationVectorType	m_keyDeclarations;

	/**
	 * Table of element keys.  The table will be built on demand, 
	 * when a key is requested, or set by the XMLParserLiaison or 
	 * the caller.  The table is:
	 * a) keyed by name,
	 * b) each with a value of a hashtable, keyed by the value returned by 
	 *    the use attribute,
	 * c) each with a value that is a nodelist.
	 * Thus, for a given key or keyref, look up hashtable by name, 
	 * look up the nodelist by the given reference.
	 */
	KeysMapType		m_keys;


	/**
	 * Keyed on string macro names, and holding values 
	 * that are macro elements in the XSL DOM tree.
	 * Initialized in initMacroLookupTable, and used in
	 * findNamedTemplate.
	 */
	NamedTemplatesMapType	m_namedTemplates;

	/**
	 * Table for defined constants, keyed on the names.
	 */
	TopLevelVariablesMapType	m_topLevelVariables;

	/**
	 * The StackGuard class guard against infinite loops.
	 */
	class StackGuard
	{
	public:

		StackGuard(
				XSLTEngineImpl&		processor,
				const DOM_Element&	xslTemplate = DOM_Element(),
				const DOM_Node&		sourceXML = DOM_Node());

		~StackGuard();

		bool operator==(const StackGuard&	theRHS) const
		{
			return m_xslRule == theRHS.m_xslRule &&
				   m_sourceXML == theRHS.m_sourceXML;
		}

		void print(PrintWriter&	pw) const;

		void push(
				const DOM_Element&	xslTemplate,
				const DOM_Node&		sourceXML);

		void pop();

		void clear() { m_stack.clear(); }

		void checkForInfiniteLoop(const StackGuard&	guard) const;

	private:

		XSLTEngineImpl*	m_processor;
		DOM_Element		m_xslRule;
		DOM_Node		m_sourceXML;

		StackGuardStackType		m_stack;
	};


	// This is set to true when the "ancestor" attribute of 
	// the select element is encountered.
	bool		m_needToCheckForInfiniteLoops;

	/**
	 * Object to guard agains infinite recursion when 
	 * doing queries.
	 */
	StackGuard	m_stackGuard;

	/**
	 * Stack for the purposes of flagging infinite recursion with 
	 * attribute sets.
	 */
	AttrStackType	m_attrSetStack;

	static void
	InstallFunctions();

	static void
	InitializeAttributeKeysTable();

	static void
	InitializeElementKeysTable();

	static void
	InitializeXSLT4JElementKeys();


}; // end XSLTEngineImpl class


#endif	// XALAN_XSLTENGINEIMPL_HEADER_GUARD

//	$ Log: $



