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
 *	  notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *	  notice, this list of conditions and the following disclaimer in
 *	  the documentation and/or other materials provided with the
 *	  distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *	  if any, must include the following acknowledgment:  
 *		 "This product includes software developed by the
 *		  Apache Software Foundation (http://www.apache.org/)."
 *	  Alternately, this acknowledgment may appear in the software itself,
 *	  if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *	  not be used to endorse or promote products derived from this
 *	  software without prior written permission. For written 
 *	  permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *	  nor may "Apache" appear in their name, without prior written
 *	  permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.	IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
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



// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"



// Base class
#include "XSLTProcessor.hpp"




// Standard library headers
#include <cassert>
#include <ctime>
#include <map>
#include <set>



#include <sax/DocumentHandler.hpp>



// XSL header files.
#include <XalanDOM/XalanDOMString.hpp>



#include <PlatformSupport/AttributeListImpl.hpp>
#include <PlatformSupport/DOMStringHelper.hpp>



#include <XPath/Function.hpp>
#include <XPath/MutableNodeRefList.hpp>
#include <XPath/NameSpace.hpp>



#include "Arg.hpp"
#include "KeyDeclaration.hpp"
#include "ProblemListener.hpp"
#include "XSLTProcessorException.hpp"



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
class XalanAttr;
class XalanText;
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
 * specified by a stylesheet tree.	The process method is the primary
 * public entry points.
 * 
 * 
 * If you reuse the processor instance, you should call reset() between calls.
 *
 */

class XALAN_XSLT_EXPORT XSLTEngineImpl : public XSLTProcessor, private DocumentHandler
{
public:

#if defined(XALAN_NO_NAMESPACES)
#	define XALAN_STD
#else
#	define XALAN_STD std::
#endif

	typedef XALAN_STD auto_ptr<XPathProcessor>	  XPathProcessorPtrType;
	typedef XALAN_STD map<XalanDOMString, XObject*>    TopLevelVariablesMapType;
	typedef XALAN_STD map<XalanDOMString, int>		   AttributeKeysMapType;
	typedef XALAN_STD map<XalanDOMString, int>		   ElementKeysMapType;
	typedef XALAN_STD map<const void*, clock_t>   DurationsTableMapType;
	typedef XALAN_STD set<XalanDOMString>			   TranslateCSSSetType;
	typedef XALAN_STD vector<Arg>				  ParamVectorType;
	typedef XALAN_STD vector<ElemAttributeSet*>   AttrStackType;
	typedef XALAN_STD vector<KeyDeclaration>	  KeyDeclarationVectorType;
	typedef XALAN_STD vector<const Locator*>	  LocatorStack;
	typedef XALAN_STD vector<NameSpace> 		  NamespaceVectorType;
	typedef XALAN_STD vector<NamespaceVectorType> NamespacesStackType;
	typedef XALAN_STD vector<TraceListener*>	  TraceListenerVectorType;
	typedef XALAN_STD vector<bool>				  BoolVectorType;


	typedef XALAN_STD map<const XalanNode*, int>	XSLDirectiveMapType;
	typedef XALAN_STD map<XalanDOMString,
						 const XalanNode*>			NamedTemplatesMapType;
	typedef XALAN_STD map<XalanDOMString,
						 Stylesheet*>		StylesheetMapType;
#undef XALAN_STD

	typedef Function::XObjectArgVectorType	XObjectArgVectorType;

	// Public members
	//---------------------------------------------------------------------

	/**
	 * The root document.
	 */
	XalanDocument*	m_rootDoc;

  /**
   * If true, output carriage returns.
   */
	bool	m_outputCarriageReturns;

  /**
   * If true, output linefeeds.
   */
	bool	m_outputLinefeeds;

	/**
	 * The formatter interface, which has the toMarkup 
	 * method, and which will eventually hold other non-event 
	 * methods.  Not to be confused with the DocumentHandler
	 * interface.
	*/
	// @@ JMD: Temporarily public
	Formatter*			m_formatter;
  
  /**
   * The factory that will be used to create result tree fragments.
   */
	mutable XalanDocument*	m_resultTreeFactory;

  /**
   * The namespace that the result tree conforms to.  A null value 
   * indicates that result-ns is not used and there is no checking. 
   * A empty string indicates that the result tree conforms to the 
   * default namespace.
   */
	XalanDOMString	m_resultNameSpacePrefix;

  /**
   * The URL that belongs to the result namespace.
   */
	XalanDOMString	m_resultNameSpaceURL;


	/**
	 * Construct an XSL processor that can call back to a XML processor, so it
	 * can handle included files, and the like. You must create a new instance
	 * of the XSLTEngineImpl class each time you run a transformation.
	 *
	 * @param XMLParserLiaison liaison to an object that can help with a
	 *						   specific parser implementation
	 * @param xpathSupport	   $$$
	 * @param xpathEnvSupport  $$$
	 * @param xobjectFactory   object responsible for XObject objects
	 * @param xpathFactory	   object responsible for XPath objects
	 */
	XSLTEngineImpl(
			XMLParserLiaison&	parserLiaison,
			XPathSupport&		xpathSupport,
			XPathEnvSupport&	xpathEnvSupport,
			XObjectFactory& 	xobjectFactory,
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
			XSLTInputSource&				inputSource, 
			XSLTInputSource*				stylesheetSource,
			XSLTResultTarget&				outputTarget,
			StylesheetConstructionContext&	constructionContext,
			StylesheetExecutionContext& 	executionContext);
	
	virtual StylesheetRoot*
	processStylesheet(
			XSLTInputSource&				stylesheetSource,
			StylesheetConstructionContext&	constructionContext);

	virtual StylesheetRoot*
	processStylesheet(
			const XalanDOMString&			xsldocURLString,
			StylesheetConstructionContext&	constructionContext);

	virtual XalanNode*
	getSourceTreeFromInput(XSLTInputSource&		inputSource);

	virtual void
	outputToResultTree(const XObject&	xobj);

	virtual XObject*
	getTopLevelVariable(const XalanDOMString&	theName) const;

	virtual void
	resetCurrentState(
			XalanNode*	sourceTree,
			XalanNode*	xmlNode);

	virtual XalanDocument*
	getRootDoc() const;

	virtual void
	setRootDoc(XalanDocument*	doc);

	virtual XalanDOMString
	evaluateAttrVal(
			XalanNode*				contextNode,
			const XalanElement& 	namespaceContext,
			const XalanDOMString&	stringedValue,
			XPathExecutionContext&	executionContext);

	virtual ResultTreeFragBase*
	createResultTreeFrag(
			StylesheetExecutionContext& 	executionContext,
			const ElemTemplateElement&		templateChild,
			XalanNode*						sourceTree,
			XalanNode*						sourceNode,
			const QName&					mode);

	virtual ResultTreeFragBase*
	createResultTreeFrag() const;

	virtual void
	resolveTopLevelParams(StylesheetExecutionContext&	executionContext);

	virtual XMLParserLiaison&
	getXMLParserLiaison() const;

	virtual const XalanDOMString
	getUniqueNSValue() const;
	
   virtual XObject*
   createXResultTreeFrag(const ResultTreeFragBase&	r) const;

	virtual XObject*
	getVariable(const QName& qname) const;

	virtual XObject*
	getParamVariable(const QName&	theName) const;

	virtual void
	pushVariable(
			const QName&		name,
			XObject*			var,
			const XalanNode*	element);

	virtual void setStylesheetParam(	
					const XalanDOMString&	key,
					const XalanDOMString&	expression);
	
	virtual void
	setStylesheetParam(
			const XalanDOMString&	key,
			XObject*				value);

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
	shouldStripSourceNode(const XalanNode&	textNode) const;

	virtual FormatterListener*
	getFormatterListener() const;

	virtual void
	setFormatterListener(FormatterListener* flistener);

	virtual void addTraceListener(TraceListener* tl);
	
	virtual void
	setTraceTemplates(bool	b);

	bool isTraceSelect() const;

	virtual void
	setTraceSelect(bool b);
  
	virtual void
	setTraceTemplateChildren(bool	b);

	virtual void
	setQuietConflictWarnings(bool	b);

	virtual void
	removeTraceListener(TraceListener*	tl);
	
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
	XalanDocument*
	parseXML(
			const XMLURL&		url,
			DocumentHandler*	docHandler, 
			XalanDocument*		docToRegister);

	/**
	 * Reset the state of the XSL processor by reading in a new XSL stylesheet
	 * from a processing instruction.
	 *
	 * @param xslURLString		  valid URI to an XSL stylesheet
	 * @param fragBase			  base of tree if URI represents document
	 *							  fragment
	 * @param xmlBaseIdent		  base identifier for stylesheet URI
	 * @param isRoot			  true if stylesheet is root of tree
	 * @param constructionContext context for construction of object
	 * @return pointer to stylesheet
	 */
	Stylesheet*
	getStylesheetFromPIURL(
			const XalanDOMString&			xslURLString,
			XalanNode& 						fragBase,
			const XalanDOMString&			xmlBaseIdent,
			bool							isRoot,
			StylesheetConstructionContext&	constructionContext);

	/**
	 * Flush the pending element.
	 */
	void flushPending();

	/**
	 * Add a namespace declaration to the namespace stack
	 *
	 * @param prefix	   namespace prefix
	 * @param namespaceVal value of namespace
	 */
	void addResultNamespaceDecl(const XalanDOMString& prefix, 
									   const XalanDOMString& namespaceVal);

	/**
	 * Add attribute to attribute list, and if it is a namespace, add it to the
	 * namespaces stack.
	 *
	 * @param attList attribute list added to
	 * @param aname   name of attribute
	 * @param value   value of attribute
	 */
	void
	addResultAttribute(
			AttributeListImpl&	attList,
			const XalanDOMString&	aname,
			const XalanDOMString&	value);

	/**
	 * Add attribute to pending attributes list, and if it is a namespace, add
	 * it to the namespaces stack.
	 *
	 * @param aname   name of attribute
	 * @param value   value of attribute
	 */
	void
	addResultAttribute(
			const XalanDOMString&	aname,
			const XalanDOMString&	value)
	{
		addResultAttribute(m_pendingAttributes,
						   aname,
						   value);
	}

	/**
	 * Receive notification of character data.
	 *
	 * <p>The Parser will call this method to report each chunk of
	 * character data.	SAX parsers may return all contiguous character
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
	 * NOTE: This method is only provided for compatibility with existing code
	 *
	 * @param ch	 pointer to characters from the XML document
	 * @param start  startng offset in 'ch' array
	 * @param length number of characters to read from the array 
	 * @deprecated
	 */
	virtual void
	characters(
			const XMLCh* const	ch,
			const unsigned int	start,
			const unsigned int	length);

	/**
	 * Receive notification of the beginning of an element with an empty
	 * attribute list
	 *
	 * @param name element type name
	 * @exception SAXException
	 */
	virtual void
	startElement(
			const XMLCh* const	name);

	/**
	 * Receive notification of character data. If available, when the
	 * disable-output-escaping attribute is used, output raw text without
	 * escaping.
	 *
	 * @param ch pointer to characters from the XML document
	 * @param start start position in the array
	 * @param length number of characters to read from the array
	 * @exception SAXException
	 */
	virtual void
	charactersRaw(
			const XMLCh* const	ch,
			const unsigned int	start,
			const unsigned int	length);

	/**
	 * Called when a Comment is to be constructed.
	 *
	 * @param	data	pointer to comment data
	 * @exception SAXException
	 */
	virtual void
	comment(
			const XMLCh* const	data);

	/**
	 * Receive notification of a entityReference.
	 *
	 * @param data pointer to characters from the XML document
	 * @exception SAXException
	 */
	virtual void
	entityReference(
			const XMLCh* const	data);

	/**
	 * Receive notification of cdata.
	 *
	 * <p>The Parser will call this method to report each chunk of
	 * character data.	SAX parsers may return all contiguous character
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
	 * @param ch	 pointer to characters from the XML document
	 * @param start  start position in the array
	 * @param length number of characters to read from the array
	 * @exception SAXException
	 */
	virtual void
	cdata(
			const XMLCh* const	ch,
			const unsigned int	start,
			const unsigned int	length);

	/**
	 * Clone an element with or without children.
	 *
	 * @param node					node to clone
	 * @param isLiteral 			true if a literal element
	 * @param overrideStrip 		false if white space stripping should be done
	 * @param shouldCloneAttributes true if attributes should be cloned
	 */
	void
	cloneToResultTree(
			XalanNode& 	node, 
			bool		isLiteral,
			bool		overrideStrip,
			bool		shouldCloneAttributes);

	/**
	 * Given a result tree fragment, walk the tree and output it to the result
	 * stream.
	 *
	 * @param theTree result tree fragment
	 */
	void
	outputResultTreeFragment(const XObject& 	theTree);

	/**
	 * Determine if an external function is available.
	 *
	 * @param theNamespace	namespace for function
	 * @param extensionName name of extension function
	 * @return whether the given function is available or not
	 */
	bool
	functionAvailable(
			XalanDOMString&		theNamespace, 
			XalanDOMString&		extensionName) const;
	
	/**
	 * Handle an extension function.
	 * 
	 * @param executionContext	current execution context
	 * @param theNamespace	namespace of function	 
	 * @param extensionName extension function name
	 * @param argVec		vector of arguments to function
	 * @return pointer to XObject result
	 */
	virtual XObject*
	extFunction(
			XPathExecutionContext&			executionContext,
			const XalanDOMString&			theNamespace,
			const XalanDOMString&			extensionName, 
			const XObjectArgVectorType&		argVec) const;

	// This is public for class Stylesheet...
	/**
	 * In response to xsl:function, set up a function that can be called from
	 * the expression language.
	 *
	 * NOTE: this method is not yet supported
	 *
	 * @param extensionElem extension element
	 */
	void
	handleFunctionsInstruction(XalanElement& 	extensionElem);

	/**
	 * Retrieve the root stylesheet.
	 * 
	 * @return pointer to root stylesheet
	 */
	virtual StylesheetRoot*
	getStylesheetRoot() const;

	/**
	 * Set the root stylesheet.
	 * 
	 * @param theStylesheet pointer to new root stylesheet
	 */
	virtual void
	setStylesheetRoot(StylesheetRoot*	theStylesheet);

	/**
	 * Table of stylesheet documents. Document objects are keyed by URL string.
	 */
	StylesheetMapType	m_stylesheets;

	/**
	 * Get table of stylesheet documents. Document objects are keyed by URL
	 * string.
	 *
	 * @return stylesheets table
	 */
	StylesheetMapType&
	getStylesheetsTable()
	{
		return m_stylesheets;
	}

	/**
	 * Retrieve the prefix for XSL namespace URI's, for example,
	 * "http://www.w3.org/1999/XSL/Transform" 
	 * 
	 * @return prefix string
	 */
	static const XalanDOMString&
	getXSLNameSpaceURLPre()
	{
		return s_XSLNameSpaceURLPre;
	}

	/**
	 * Retrieve the URI for the current XSL namespace, for example,
	 * "http://www.w3.org/1999/XSL/Transform/1.0" 
	 * 
	 * @return URI string
	 */
	const XalanDOMString&
	getXSLNameSpaceURL() const
	{
		return m_XSLNameSpaceURL;
	}

	/**
	 * Special Xalan namespace for built-in extensions.
	 *
	 * @return Xalan namespace for extensions
	 */
	static const XalanDOMString&
	getXalanXSLNameSpaceURL()
	{
		return s_XSLT4JNameSpaceURL;
	}

	/**
	 * Set the full XSLT Namespace URI.
	 *
	 * @param theXSLNameSpaceURL XSLT namespace URI
	 */
	void
	setXSLNameSpaceURL(const XalanDOMString&		theXSLNameSpaceURL)
	{
		m_XSLNameSpaceURL = theXSLNameSpaceURL;
	}

	/**
	 * Retieve the latest XSLT version currently supported.
	 *
	 * @return XSLT version number
	 */
	static double getXSLTVerSupported();

	/**
	 * Accessor method for hash table of XSLT IDs for attribute names.
	 * 
	 * @return hash table of key names and IDs
	 */
	static const AttributeKeysMapType&
	getAttributeKeys()
	{
		return s_attributeKeys;
	}

	/**
	 * Accessor method for hash table of XSLT IDs for element names.
	 * 
	 * @return hash table of key names and IDs
	 */
	static const ElementKeysMapType&
	getElementKeys()
	{
		return s_elementKeys;
	}

	/**
	 * Given an XSL tag name, return an integer token that corresponds to
	 * ELEMNAME_XXX constants defined in Constants.java.
	 *
	 * @param name a probable xsl:xxx element
	 * @return Constants.ELEMNAME_XXX token, -1 if in XSL or Xalan namespace,
	 *		   or -2 if not in known namespace
	 */
	int
	getAttrTok(const XalanDOMString&	name) const
	{
		AttributeKeysMapType::const_iterator iter=
			s_attributeKeys.find(name);

		return iter == s_attributeKeys.end() ? -2 : (*iter).second;
	}

	/**
	 * Given an XSL tag name, return an integer token
	 * that corresponds to ELEMNAME_XXX constants defined 
	 * in Constants.java.
	 *
	 * @param node a probable xsl:xxx element.
	 * @return Constants.ELEMNAME_XXX token, -1 if in XSL or Xalan namespace,
	 *		   or -2 if not in known namespace
	 */
	int
	getXSLToken(const XalanNode&	node) const;

  /**
   * Find the type of an element using this method.
   *
   * @param node	a probable xsl:xxx element
   * @param tagType Constants.ELEMNAME_XXX token
   * @return true if node is of tagType
   */
	bool
	isXSLTagOfType(
			const XalanNode& node,
			int 		tagType) const;

  /**
	* Add list of trace listeners to the root sylesheet for the purposes of
	* debugging and diagnosis.
   */
	void addTraceListenersToStylesheet();

	/**
	 * Fire a generate event.
	 *
	 * @param te generate event to fire
	 */
	void fireGenerateEvent(const GenerateEvent& te);
	
	/**
	 * Whether to warn about pattern match conflicts.
	 *
	 * @return true to not warn about pattern match conflicts
	 */
	bool
	getQuietConflictWarnings() const
	{
		return m_quietConflictWarnings;
	}

	virtual void
	message(
			const XalanDOMString&	msg,
			const XalanNode*		styleNode = 0,
			const XalanNode*		sourceNode = 0) const;

	/**
	 * Tell the user of an warning, and probably throw an exception.
	 * 
	 * @param msg		 text of message to output
	 * @param sourceNode node in source where error occurred
	 * @param styleNode  node in stylesheet where error occurred
	 * @exception XSLProcessorException
	 */
	virtual void
	warn(
			const XalanDOMString&	msg,
			const XalanNode*		styleNode = 0,
			const XalanNode*		sourceNode = 0) const;

//@@ HOLD until we figure out exceptions
//	void error(XalanDOMString& msg, Exception e);

	/**
	 * Tell the user of an error, and probably throw an exception.
	 * 
	 * @param msg		 text of message to output
	 * @param sourceNode node in source where error occurred
	 * @param styleNode  node in stylesheet where error occurred
	 * @exception XSLProcessorException
	 */
	virtual void
	error(
			const XalanDOMString&	msg,
			const XalanNode*		styleNode = 0,
			const XalanNode*		sourceNode = 0) const;

	/**
	 * Mark the time, so that displayDuration can later display the elapsed
	 * clock ticks.
	 * 
	 * @param theKey pointer to element to push
	 */
	void
	pushTime(const void*	key) const;

	/**
	 * Returns the duration since pushTime was called for element
	 * in milliseconds.
	 *
	 * @param key pointer to element involved
	 */
	clock_t
	popDuration(const void* 	key) const;

	/**
	 * Display the duration since pushTime was called for element in
	 * milliseconds, and a descriptive message
	 *
	 * @param info message to display
	 * @param key pointer to element involved
	 */
	void
	displayDuration(
			const XalanDOMString&	info,
			const void* 		key) const;


	/**
	 * Whether diagnostic output is to be generated
	 * 
	 * @return true for diagnostics output 
	 */
	bool doDiagnosticsOutput()
	{	return 0 != m_diagnosticsPrintWriter; }

	/**
	 * Print a diagnostics string to the output device
	 * 
	 * @param s string to print
	 */
	void
	diag(
			const XalanDOMString&	s) const;
  
	/**
	 * Compose a diagnostic trace of the current selection
	 *
	 * @param theTemplate current context node
	 * @param nl		  list of selected nodes
	 */
	void traceSelect(
			const XalanElement&		theTemplate,
			const NodeRefListBase&	nl) const;

	/**
	 * Tell if a given element name should output it's text 
	 * as cdata.
	 *
	 * @param elementName name of element
	 * @return true if it should output as cdata
	 */
	bool isCDataResultElem(const XalanDOMString& elementName);
	
	/**
	 * Tell if a qualified name equals the current result tree name.
	 *
	 * @param qname 	  QName to compare to
	 * @param elementName current result tree element
	 * @return true if names are the same
	 */
	bool qnameEqualsResultElemName(const QName& qname, const XalanDOMString& elementName);
	
	/**
	 * Retrieve the result namespace corresponding to a prefix.
	 * 
	 * @param prefix prefix for namespace
	 * @return string for namespace URI
	 */
	XalanDOMString
	getResultNamespaceForPrefix(
			const XalanDOMString&	prefix) const;
  
	/**
	 * Retrieve the result prefix corresponding to a namespace.
	 * 
	 * @param theNamespace namespace for prefix
	 * @return string for namespace prefix
	 */
	XalanDOMString
	getResultPrefixForNamespace(
			const XalanDOMString&	theNamespace) const;

	/**
	 * Evaluate an xpath string and return the result as a numberic score.
	 *
	 * @param str			   string to evaluate
	 * @param context		   context node
	 * @param executionContext current execution context
	 * @return score number, higher representing better match
	 */
	double
	evalMatchPatternStr(
			const XalanDOMString&	str,
			XalanNode* 				context,
			XPathExecutionContext&	executionContext);

	/**
	 * Create and initialize an xpath and return it.
	 *
	 * @param str	   string for XPath
	 * @param resolver resolver for namespace resolution
	 * @return pointer to XPath created
	 */
	XPath*
	createMatchPattern(
			const XalanDOMString&	str,
			const PrefixResolver&	resolver);

	/**
	 * Copy XMLNS: attributes in if not already in scope.
	 *
	 * @param src				  source node
	 * @param srcIsStylesheetTree true if source node is a stylesheet tree
	 */
	void
	copyNamespaceAttributes(
			const XalanNode&	src,
			bool				srcIsStylesheetTree);

	/**
	 * Evaluate an xpath string and return the result.
	 *
	 * @param str			   string to evaluate
	 * @param executionContext current execution context
	 * @return pointer to XObject result
	 */
	XObject*
	evalXPathStr(
			const XalanDOMString&		str,
			XPathExecutionContext&	executionContext);

	/**
	 * Evaluate an xpath string and return the result.
	 * 
	 * @param str			   string to evaluate
	 * @param contextNode	   context node
	 * @param prefixResolver prefix resolver to use
	 * @param executionContext current execution context
	 * @return pointer to XObject result
	 */
	XObject*
	evalXPathStr(
			const XalanDOMString&	str,
			XalanNode* 				contextNode,
			const PrefixResolver&	prefixResolver,
			XPathExecutionContext&	executionContext);

	/**
	 * Evaluate an xpath string and return the result.
	 * 
	 * @param str			   string to evaluate
	 * @param contextNode	   context node
	 * @param prefixResolver prefix resolver to use
	 * @param executionContext current execution context
	 * @return pointer to XObject result
	 */
	XObject*
	evalXPathStr(
			const XalanDOMString&	str,
			XalanNode* 				contextNode,
			const XalanElement&		prefixResolver,
			XPathExecutionContext&	executionContext);

	/**
	 * Create an XPath whose lifetime can end after transforming
	 * is complete.  Do not use this to create an XPath to be
	 * held by a stylesheet.
	 *
	 * @param str			   string to evaluate
	 * @param executionContext current execution context
	 * @param resolver		   resolver for namespace resolution
	 * @return pointer to XPath created
	 */
	virtual XPath*
	createProcessingXPath(
		const XalanDOMString&	str,
		XPathExecutionContext&	executionContext,
		const PrefixResolver&	resolver);

	/**
	 * Given a valid element id, return the corresponding element.
	 *
	 * @param id  string representing ID
	 * @param doc document to search
	 * @return element for ID
	 */
	XalanElement*
	getElementByID(
			const XalanDOMString&	id,
			const XalanDocument& 	doc) const;

	/**
	 * Push a context marker onto the stack to let us know when to stop
	 * searching for a var.
	 *
	 * @param caller	 caller node
	 * @param sourceNode source node
	 */
	void
	pushContextMarker(
			const XalanNode*	caller,
			const XalanNode*	sourceNode)
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

	/**
	 * Given a template, search for the arguments and push them on the stack.
	 * Also, push default arguments on the stack. You <em>must</em> call
	 * popContext() when you are done with the arguments.
	 *
	 * @param executionContext		 execution context
	 * @param xslCallTemplateElement "call-template" element
	 * @param sourceTree			 source tree
	 * @param sourceNode			 source node
	 * @param mode					 mode under which the template is operating
	 * @param targetTemplate		 target template
	 */
	void
	pushParams(
			StylesheetExecutionContext& 	executionContext,
			const ElemTemplateElement&		xslCallTemplateElement,
			XalanNode* 						sourceTree, 
			XalanNode* 						sourceNode,
			const QName&					mode,
			const XalanNode* 				targetTemplate)
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
	 *
	 * @param attrs    attribute list to search
	 * @param key	   key value to search for
	 * @param resolver resolver for namespace resolution
	 * @return pointer to XPath for expression
	 */
	XPath* getExpression(
					AttributeList& attrs,
					const XalanDOMString& key,
					const PrefixResolver& resolver);

	/**
	 * Given an element, return an attribute value in the form of a string.
	 *
	 * @param el		  element from where to get the attribute
	 * @param key		  name of the attribute
	 * @param contextNode context to evaluate the attribute value template
	 * @return string for attribute value
	 */
	XalanDOMString
	getAttrVal(
			const XalanElement&	el,
			const XalanDOMString&	key,
			const XalanNode& 	contextNode);
  
	/**
	 * Given an element, return an attribute value in the form of a string.
	 *
	 * @param el		  element from where to get the attribute
	 * @param key		  name of the attribute
	 * @return string for attribute value
	 */
	static XalanDOMString
	getAttrVal(
			const XalanElement&	el,
			const XalanDOMString&	key);

	/**
	 * Copy an attribute to the created output element, executing attribute
	 * templates as need be, and processing the 'xsl:use' attribute.
	 *
	 * @param attr			   attribute to copy
	 * @param contextNode	   current context node
	 * @param stylesheetTree   associated stylesheet object
	 * @param attrList		   attribute list added to
	 * @param namespaceContext context for namespace
	 * @exception XSLProcessorException 
	 */
	void
	copyAttributeToTarget(
			const XalanAttr& 		attr,
			XalanNode* 				contextNode,
			const Stylesheet*		stylesheetTree,
			AttributeListImpl&		attrList, 
			const XalanElement&		namespaceContext);
  
	/**
	 * Determine whether to translate CSS attributes on the output to a "style"
	 * attribute, for old submission support.
	 * 
	 * @return true to translate CSS attributes
	 * @deprecated
	 */
	bool
	getTranslateCSS() 
	{
		return m_translateCSS;
	} 

	/**
	 * Set whether to translate CSS attributes on the output to a "style"
	 * attribute, for old submission support.
	 * 
	 * @param b true to translate CSS attributes
	 * @deprecated
	 */
	void
	setTranslateCSS(bool	b) 
	{
		m_translateCSS = b;
	} 

	/**
	 * Determine the value of the default-space attribute.
	 *
	 * @return true if the default-space attribute is "strip," false  if
	 *			the attribute is "preserve"
	 */
	bool
	getStripWhiteSpace() const
	{
		return m_stripWhiteSpace;
	}

	/**
	 * Change the value of the default-space attribute.
	 *
	 * @param b sets the default of the default-space attribute to "strip" if
	 *			true, or "preserve" if false.
	 */
	void
	setStripWhiteSpace(bool fStrip)
	{
		m_stripWhiteSpace = fStrip;
	}

  /**
	* Conditionally trim all leading and trailing whitespace in the specified
	* String.  All strings of white space are replaced by a single space
	* character (#x20), except spaces after punctuation which receive double
	* spaces if doublePunctuationSpaces is true. This function may be useful to
	* a formatter, but to get first class results, the formatter should
	* probably do its own white space handling based on the semantics of the
	* formatting object.
	*
   * @param   string	  string to be trimmed
   * @param   trimHead	  whether to trim leading whitespace
   * @param   trimTail	  whether to trim trailing whitespace
   * @param   doublePunctuationSpaces true to use double spaces for punctuation
   * @return trimmed string
   */
	XalanDOMString fixWhiteSpace(
			const XalanDOMString&	string, 
			bool				trimHead, 
			bool				trimTail, 
			bool				doublePunctuationSpaces);
  
  /**
	* Control if carriage returns are put in the result tree. Default is to
	* output carriage returns.
	*
   * @param b true to output carriage returns
   */
	void setOutputCarriageReturns(bool	b)
	{
		m_outputCarriageReturns = b;
	}

	/**
	 * Control if linefeeds are put in the result tree. Default is to output
	 * linefeeds.
	 *
	 * @param b true to output linefeeds
	 */
	void
	setOutputLinefeeds(bool		b)
	{
		m_outputLinefeeds = b;
	}

	/**
	 * Normalize the linefeeds and/or carriage returns to be consistently 0x0D
	 * 0x0A. 
	 *
	 * @param   tx DOM text node to normalize
	 * @return normalized string
	 */
	/* $$$ ToDo:  This should almost certainly be done somewhere else... like in the XML
	 * parser.
	 */
	const XalanDOMString
	getNormalizedText(const XalanText&	tx) const;

	/**
	 * Set the stylesheet root.  If this is set, then the process calls that
	 * take only the input .xml will use this instead of looking for a
	 * stylesheet PI.
	 * 
	 * @param theStylesheet pointer to root stylesheet
	 */
	void
	setStylesheet(StylesheetRoot *stylesheetRoot)
	{
		m_stylesheetRoot = stylesheetRoot;
	}
 
	/**
	 * Get the stylesheet root.  If this is set, then the process calls that
	 * take only the input .xml will use this instead of looking for a
	 * stylesheet PI.
	 * 
	 * @return pointer to root stylesheet
	 */
	StylesheetRoot*
	getStylesheet() const
	{
		return m_stylesheetRoot;
	}

	/**
	 * Get the filename of the output document, if it was set. This is for use
	 * by multiple output documents, to determine the base directory for the
	 * output document.  It needs to be set by the caller.
	 * 
	 * @return name of output document
	 */
	const XalanDOMString&
	getOutputFileName() const
	{
		return m_outputFileName;
	}

	/**
	 * Set the filename of the output document. This is for use by multiple
	 * output documents, to determine the base directory for the output
	 * document.  It needs to be set by the caller.
	 * 
	 * @param filename name of output document
	 */
	void
	setOutputFileName(const XalanDOMString&		filename)
	{
		m_outputFileName = filename;
	}

	/**
	 * Get the factory for making xpaths.
	 *
	 * @return XPath factory object
	 */
	XPathFactory&
	getXPathFactory()
	{
		return m_xpathFactory;
	}

	/**
	 * Get the XPath processor object.
	 *
	 * @return XPathProcessor interface being used
	 */
	XPathProcessor&
	getXPathProcessor()
	{
		return *m_xpathProcessor.get();
	}

	/**
	 * Given a document, get the default stylesheet URI from the xsl:stylesheet
	 * PI.  However, this will only get you the first URL, and there may be
	 * many.
	 *
	 * @param sourceTree node for source tree
	 * @deprecated
	 */
	const XalanDOMString
	getStyleSheetURIFromDoc(const XalanNode&	sourceTree);

	/**
	 * Given a classID and codetype, try to register a code dispatcher.
	 *
	 * @param classid	class id for extension
	 * @param codetype type of extension
	 */
	void
	registerExtensionHandlerByName(
			const XalanDOMString&	classid,
			const XalanDOMString&	codetype);
  
	/**
	 * Register the given DispatcherFactory for a given mime type.
	 *
	 * @param mimeType MIME type string
	 * @param factory  factory to register
	 */
	void
	registerExtensionHandler(
			const XalanDOMString&	mimeType,
			DispatcherFactory*		factory);

	/**
	 * Determine the fully qualified URI for a string.
	 *
	 * @param urlString string to qualify
	 * @return pointer to fully qualified URI
	 */
	XMLURL*
	getURLFromString(const XalanDOMString&	urlString) const;

	/**
	 * Determine the fully qualified URI for a string.
	 *
	 * @param urlString string to qualify
	 * @param base base location for URI
	 * @return pointer to fully qualified URI
	 */
	XMLURL*
	getURLFromString(
			const XalanDOMString&	urlString,
			const XalanDOMString&	base) const;

	/**
	 * Retrieve the XPath support object
	 *
	 * @return XPath support object
	 */
	XPathSupport&
	getXPathSupport()
	{
		return m_xpathSupport;
	}

	/**
	 * Retrieve the XPath environment support object
	 *
	 * @return XPath environment support object
	 */
	XPathEnvSupport&
	getXPathEnvSupport()
	{
		return m_xpathEnvSupport;
	}

	/**
	 * Set the formatter interface, which has the toMarkup method, and which
	 * will eventually hold other non-event methods.  Not to be confused with
	 * the DocumentHandler interface.
	 *
	 * @param formatter pointer to formatter
	 */
	void
	setFormatter(Formatter* 	formatter);

	// $$$ ToDo: why isn't this just a NodeRefListBase?
	const MutableNodeRefList&
	getContextNodeList() const
	{
		return m_contextNodeList;
	}

	/**
	 * Set node list for current context.
	 * 
	 * @param ref new node list
	 */
	// $$$ ToDo: why isn't this just a NodeRefListBase?
	void
	setContextNodeList(const MutableNodeRefList&	ref)
	{
		m_contextNodeList = ref;		
	}

	/**
	 * Add a named template to the table of template name and value pairs.
	 *
	 * @param theName name of template
	 * @param theNode template node
	 */
	void
	addNamedTemplate(const XalanDOMString&	theName,
					 const XalanNode*		theNode)
	{
		m_namedTemplates[theName] = theNode;
	}

	/**
	 * Find a named template in the table of template name and value pairs.
	 *
	 * @param theName name of template
	 * @return template node if found, empty node otherwise
	 */
	const XalanNode*
	getNamedTemplate(const XalanDOMString&	theName) const
	{
		const NamedTemplatesMapType::const_iterator 	i =
			m_namedTemplates.find(theName);

		if(i != m_namedTemplates.end())
		{
			return (*i).second;
		}
		else
		{
			return 0;
		}
	}

	/**
	 * Set the problem listener property. The XSL class can have a single
	 * listener that can be informed of errors and warnings, and can normally
	 * control if an exception is thrown or not (or the problem listeners can
	 * throw their own RuntimeExceptions).
	 *
	 * @param l pointer to ProblemListener interface
	 */
	void setProblemListener(ProblemListener*		l)
	{
		m_problemListener = l;
	}

	/**
	 * Get the problem listener property. The XSL class can have a single
	 * listener that can be informed of errors and warnings, and can normally
	 * control if an exception is thrown or not (or the problem listeners can
	 * throw their own RuntimeExceptions).
	 *
	 * @return pointer to ProblemListener interface
	 */
	ProblemListener* getProblemListener() const
	{
		return m_problemListener;
	}

	/**
	 * An class for  exceptions that occur when a given stylesheet goes into an
	 * infinite loop.
	 */
	class XSLInfiniteLoopException : public XSLTProcessorException
	{
	public:
		XSLInfiniteLoopException() : XSLTProcessorException("XSLT infinite loop occurred!")
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
		 * 
		 * @param theProcessor XSL processor
		 */
		VariableStack(XSLTEngineImpl&		theProcessor);

		~VariableStack();

	
		/**
		 * Reset the stack.
		 */
		virtual void
		reset();

		/**
		 * Push a context marker onto the stack to let us know when 
		 * to stop searching for a var.
		 * 
		 * @param elem element for context
		 */
		void
		pushElementMarker(const XalanNode*	elem);

		/**
		 * Pop the current context from the current context stack.
		 * 
		 * @param elem element for context
		 */
		void
		popElementMarker(const XalanNode*	elem);

		/**
		 * Check to see if an element marker for the particular node has already
		 * been pushed.
		 * 
		 * @param elem node in question
		 * @return true if it has been pushed already
		 */
		bool
		elementMarkerAlreadyPushed(const XalanNode*		elem) const;

		/**
		 * Push a context marker onto the stack to let us know when to stop
		 * searching for a var.
		 *
		 * @param caller	 caller node
		 * @param sourceNode source node
		 */
		void
		pushContextMarker(
				const XalanNode*	caller,
				const XalanNode*	sourceNode);

		/**
		 * Pop the current context from the current context stack.
		 */
		void
		popCurrentContext();

		/**
		 * Given a template, search for the arguments and push them on the stack.
		 * Also, push default arguments on the stack. You <em>must</em> call
		 * popContext() when you are done with the arguments.
		 *
		 * @param executionContext		 execution context
		 * @param xslCallTemplateElement "call-template" element
		 * @param sourceTree			 source tree
		 * @param sourceNode			 source node
		 * @param mode					 mode under which the template is operating
		 * @param targetTemplate		 target template
		 */
		void
		pushParams(
				StylesheetExecutionContext& 	executionContext,
				const ElemTemplateElement&		xslCallTemplateElement,
				XalanNode*						sourceTree, 
				XalanNode*						sourceNode,
				const QName&					mode,
				const XalanNode*				targetTemplate);

		/**
		 * Given a name, return a string representing the value, but don't look
		 * in the global space.
		 *
		 * @param theName name of variable
		 * @return pointer to XObject for variable
		 */
		XObject*
		getXObjectParamVariable(const QName& qname) const
		{
			return findXObject(qname, false);
		}

		/**
		 * Given a name, find the corresponding XObject.
		 *
		 * @param qname name of variable
		 * @return pointer to the corresponding XObject
		 */
		XObject*
		getXObjectVariable(const QName& 	name) const
		{
			return findXObject(name, true);
		}

		/**
		 * Push a named variable onto the processor variable stack. Don't forget
		 * to call startContext before pushing a series of arguments for a given
		 * macro call.
		 *
		 * @param name	  name of variable
		 * @param val	  pointer to XObject value
		 * @param e 	  element marker for variable
		 */
		void
		pushVariable(
				const QName&		name,
				XObject*			val,
				const XalanNode*	e);

		/**
		 * Mark the top of the global stack frame.
		 */
		void
		markGlobalStackFrame()
		{
			m_globalStackFrameIndex = m_stack.size();
		}

		/**
		 * Set the top of the stack frame from where a search for a variable or
		 * param should take place.  Calling with no parameter will cause the
		 * index to be set to the size of the stack.
		 *
		 * @param currentStackFrameIndex new value of index
		 */
		void
		setCurrentStackFrameIndex(int	currentStackFrameIndex = -1)
		{
			if (currentStackFrameIndex == -1)
				m_currentStackFrameIndex = m_stack.size();
			else
				m_currentStackFrameIndex = currentStackFrameIndex;
		}

		/**
		 * Get the top of the stack frame from where a search 
		 * for a variable or param should take place.
		 *
		 * @return current value of index
		 */
		int
		getCurrentStackFrameIndex() const
		{
			return m_currentStackFrameIndex;
		}

		/**
		 * Push an entry onto the stack.
		 *
		 * @param stack entry to push
		 */
		void
		push(StackEntry*	theEntry)
		{
			assert(theEntry != 0);
			assert(theEntry->getType() < 4 && theEntry->getType() >= 0);

			if(m_currentStackFrameIndex == m_stack.size())
			{
				++m_currentStackFrameIndex;
			}

			m_stack.push_back(theEntry);
		}

		/**
		 * Override the pop in order to track the 
		 * m_currentStackFrameIndex correctly.
		 *
		 * @return stack entry popped
		 */
		void
		pop()
		{
			assert(m_stack.empty() == false);

			if(m_currentStackFrameIndex == m_stack.size())
			{
				--m_currentStackFrameIndex;
			}

			m_stack.pop_back();
		}

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
		const XalanElement* 			m_caller;


#if defined(XALAN_NO_NAMESPACES)
		typedef vector<StackEntry*>			VariableStackStackType;
		typedef set<StackEntry*>			StackEntrySetType;
#else
		typedef std::vector<StackEntry*>	VariableStackStackType;
		typedef std::set<StackEntry*>		StackEntrySetType;
#endif

		VariableStackStackType			m_stack;

		StackEntrySetType				m_stackEntries;

		XSLTEngineImpl&					m_processor;

		int								m_globalStackFrameIndex;

		/**
		 * This is the top of the stack frame from where a search 
		 * for a variable or param should take place.  It may not 
		 * be the real stack top.
		 */
		unsigned int					m_currentStackFrameIndex;
	
	}; // end VariableStack

	// Give VariableStack access to stuff.
	friend class VariableStack;

	/**
	 * Accessor method for variable stack.
	 * 
	 * @return variable stack
	 */
	VariableStack&
	getVariableStacks()
	{
		return m_variableStacks;
	}

	/**
	 * Create a document fragment.	This function may return null.
	 *
	 * @return pointer to new document fragment
	 */
	ResultTreeFragBase*
	createDocFrag() const;

	/**
	 * Retrieve list of attributes yet to be processed
	 * 
	 * @return attribute list
	 */
	AttributeListImpl&
	getPendingAttributes();

	/**
	 * Retrieve name of the pending element currently being processed.
	 * 
	 * @return element name
	 */
	const XalanDOMString
	getPendingElementName() const;

	/**
	 * Sets a list of attributes yet to be processed.
	 * 
	 * @param pendingAttributes attribute list
	 */
	void
	setPendingAttributes(const AttributeList&	pendingAttributes);

	/**
	 * Changes the currently pending element name.
	 * 
	 * @param elementName new name of element
	 */
	void
	setPendingElementName(const XalanDOMString&		elementName);

	/**
	 * Accessor method for stack that keeps track of the attribute elements.
	 *
	 * @return attribute stack
	 */
	AttrStackType&
	getAttrSetStack()
	{ 
		return m_attrSetStack; 
	}

	/**
	 * Get the locator from the top of the locator stack.
	 *
	 * @return A pointer to the Locator, or 0 if there is nothing on the stack.
	 */
	const Locator*
	getLocatorFromStack() const
	{
		return m_stylesheetLocatorStack.size() == 0 ? 0 : m_stylesheetLocatorStack.back();
	}

	/**
	 * Push a locator on to the locator stack.
	 *
	 * @param A pointer to the Locator to push.
	 */
	void
	pushLocatorOnStack(const Locator*	locator)
	{
		m_stylesheetLocatorStack.push_back(locator);
	}

	/**
	 * Pop the locator from the top of the locator stack.
	 */
	void
	popLocatorStack()
	{
		if (m_stylesheetLocatorStack.size() != 0)
		{
			m_stylesheetLocatorStack.pop_back();
		}
	}

	// These methods are inherited from DocumentHandler ...
	
	virtual void
	setDocumentLocator(const Locator* const		locator);

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
		const XMLCh* const		chars,
		const unsigned int	  length);

	virtual void
	ignorableWhitespace(
			const XMLCh* const	ch,
			const unsigned int	length);

	virtual void
	processingInstruction(
			const XMLCh* const	target,
			const XMLCh* const	data);

	virtual void resetDocument();

protected:

	/*
	 * The current input element that is being processed.
	 */
	XalanNode*	m_currentNode;

  /**
   * Given a tag name, an attribute name, and 
   * an attribute value, do a very crude recursive 
   * search and locate the first match.
   */
	static XalanElement*
	findElementByAttribute(
			XalanElement&			elem,
			const XalanDOMString&	targetElementName, 
			const XalanDOMString&	targetAttributeName,
			const XalanDOMString&	targetAttributeValue);


	/**
	 * Returns whether or not an attribute key 
	 * is a CSS2 attribute.
	 * @deprecated
	 */
	bool
	isCSSAttribute(const XalanDOMString&	name) const;

	/**
	 * Stuff a hash table with CSS2 keys & dummy values
	 * in order to quickly look up if a template child
	 * attribute is a CSS attribute or not.
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
			XalanNode* 				contextNode,
			const Stylesheet*		stylesheetTree,
			const XalanElement&		templateChild,
			AttributeListImpl&		attList);

  /**
  * Keyed on CSS2 property names, and holding dummy 
  * values for quickly looking up if a result tree element
  * attribute is a CSS attribute or not.
  * @deprecated
  */
	TranslateCSSSetType 	m_cssKeys;

 /*
  * If this is true, translate CSS attributes on the output to a "style"
  * attribute.
  */
	bool					m_translateCSS;

public:

	/**
	 * Reset the state.  This needs to be called after a process() call 
	 * is invoked, if the processor is to be used again.
	 */
	virtual void
	reset();

protected:

  /**
   * The pending element.  We have to delay the call to 
   * m_flistener.startElement(name, atts) because of the 
   * xsl:attribute and xsl:copy calls.	In other words, 
   * the attributes have to be fully collected before you 
   * can call startElement.
   */

	XalanDOMString	m_pendingElementName;

	/**
	 * The pending attributes.	We have to delay the call to 
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
	NamespacesStackType m_resultNameSpaces;

	/**
	 * This is pushed on the m_resultNameSpaces stack until a 
	 * xmlns attribute is found.  It's special because it has
	 * and empty prefix and uri field.
	 */
	NameSpace			m_emptyNamespace;
	
	// Factory for creating xpaths.
	XPathFactory&		m_xpathFactory;

	// Factory for creating xobjects
	XObjectFactory& 	m_xobjectFactory;

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
   * The top of this stack should contain the currently processed
   * stylesheet SAX locator object.
   */
	LocatorStack  m_stylesheetLocatorStack;

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
	 * Print a trace of a template that is being called, either by 
	 * a match, name, or as part of for-each.
	 */
	void
	traceTemplate(
			const XalanElement&		theTemplate) const;

	/**
	 * Print some diagnostics about the current 
	 * template child.
	 */
	void
	diagnoseTemplateChildren(
			const XalanNode& templateChild,
			const XalanNode& sourceNode) const;

  /**
   * The root of a linked set of stylesheets.
   */
	StylesheetRoot* 				m_stylesheetRoot;

	StylesheetExecutionContext* 	m_stylesheetExecutionContext;

/**
 * The full XSLT Namespace URL.  To be replaced by the one actually 
 * found.
 */
	XalanDOMString					m_XSLNameSpaceURL;	//"http://www.w3.org/1999/XSL/Transform/1.0"



/**
 * The minimum version of XSLT supported.
 */
	static const double s_XSLTVerSupported; // 1.0

/**
 * Out version of XSLT
 */
	static const XalanDOMString s_DefaultXSLNameSpaceURL;
/**
 * The namespace that we must match as a minimum for XSLT.
 */
	static const	XalanDOMString	s_XSLNameSpaceURLPre;	// "http://www.w3.org/1999/XSL/Transform"

/**
 * Special Xalan namespace for built-in extensions.
 */
	static const XalanDOMString s_XSLT4JNameSpaceURL; // "http://xml.apache.org/xslt"


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
	 * Hash table of XSLT IDs for attribute names.
	 */
	static AttributeKeysMapType s_attributeKeys;

	/**
	 * Hash table of XSLT element IDs for element names.
	 */
	static ElementKeysMapType	s_elementKeys;


	/**
	 * Node to xsl directive table, used so we don't have to keep 
	 * looking up the directives each time.  This isn't of huge 
	 * benifit, but is used because of the cost of getExpandedElementName.
	 */
	// Create a hash table that can look up xsl element IDs via name.
	mutable XSLDirectiveMapType 	m_XSLDirectiveLookup;

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
	TraceListenerVectorType 	m_traceListeners;
	
	// Common processing for errors and warnings
	void
	problem(
			const XalanDOMString&				msg, 
			ProblemListener::eClassification	classification,
			const XalanNode*					styleNode = 0,
			const XalanNode*					sourceNode = 0) const;

	/**
	 * This is for use by multiple output documents, to determine 
	 * the base directory for the output document.	It needs to 
	 * be set by the caller.
	 */
	XalanDOMString m_outputFileName;
	
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
			StylesheetExecutionContext& 	executionContext,
			const ElemTemplateElement&		templateParent,
			XalanNode& 						sourceTree,
			XalanNode& 						sourceNode,
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
	XalanDOMString
	getPrefixForNamespace(
			const XalanDOMString&	theNamespace,
			const XalanElement&		namespaceContext) const;

	bool	m_useATVsInSelects;

	/**
	 * Translate CSS attributes and put them in a style tag.
	 * @deprecated
	 */
	void
	translateCSSAttrsToStyleAttr(AttributeListImpl&		attList);

	// $$$ ToDo:  This must go!!!!!!
	/**
	 * Evaluate an xpath string and return the result.
	 */
	virtual XPath*
	createXPath(
		const XalanDOMString&	str,
		const PrefixResolver&	resolver);

	/**
	 * Given a name, locate a variable in the current context, and return 
	 * the XObject.
	 * @exception XSLProcessorException thrown if the active ProblemListener and XMLParserLiaison decide 
	 * the error condition is severe enough to halt processing.
	 */
	XObject*
	getXObjectVariable(const XalanDOMString&	name) const;

	/**
	 * Get an XLocator provider keyed by node.	This gets the association
	 * based on the root of the tree that is the node's parent.
	 *
	 * @param node node for locator
	 * @return pointer to locator
	 */
	XLocator*
	getXLocatorFromNode(const XalanNode* 	node) const;

	/**
	 * Associate an XLocator provider to a node.  This makes the association
	 * based on the root of the tree that is the node's parent.
	 *
	 * @param node	   node for association
	 * @param xlocator locator to associate with node
	 */
	 void
	 associateXLocatorToNode(
			const XalanNode* 	node,
			XLocator*			xlocator);

	/**
	 * If this is true, the processor will do the best it can to strip 
	 * unwanted white space. This is set in the stylesheet via the default-space 
	 * attribute on xsl:stylesheet.
	 */
	bool	m_stripWhiteSpace; // default default-space="preserve"

//	typedef std::set<XalanDOMString>		WhitespaceLookupTableType;

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
	static const bool	s_resolveContentsEarly;

	/**
	 * Set the factory for making XPaths.
	 */
//	void setXPathFactory(XPathFactory* factory) { m_xpathFactory = factory; }

	
	/**
	 * Get a DOM document, primarily for creating result 
	 * tree fragments.
	 */
	virtual XalanDocument*
	getDOMFactory() const;

	/**
	 * Set the XPath processor object.
	 * @param processor A XPathProcessor interface.
	 */
//	void setXPathProcessor(XPathProcessor* processor) { m_xpathProcessor = processor; }

	bool
	getResolveContentsEarly() const
	{
		return s_resolveContentsEarly;
	}

	ParamVectorType 	m_topLevelParams;

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
	XalanNode*
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

	XPathEnvSupport&	m_xpathEnvSupport;

	/**
	 * The listener for formatting events.	This should be 
	 * supplied by the Formatter object.
	 */
	FormatterListener*	m_flistener;

	/**
	 * This holds the current context node list.  This should arguably be 
	 * passed by parameter.
	 */
	MutableNodeRefList	m_contextNodeList;

	/**
	 * Keyed on string macro names, and holding values that are macro elements
	 * in the XSL DOM tree. Initialized in initMacroLookupTable, and used in
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

#if defined(XALAN_NO_NAMESPACES)
		typedef vector<StackGuard>			StackGuardStackType;
#else
		typedef std::vector<StackGuard>		StackGuardStackType;
#endif

		StackGuard(
				XSLTEngineImpl& 		processor,
				const XalanElement*		xslTemplate = 0,
				const XalanNode* 		sourceXML = 0);

		~StackGuard();

		bool operator==(const StackGuard&	theRHS) const
		{
			return m_xslRule == theRHS.m_xslRule &&
				   m_sourceXML == theRHS.m_sourceXML;
		}

		void print(PrintWriter& pw) const;

		void push(
				const XalanElement*		xslTemplate,
				const XalanNode* 		sourceXML);

		void pop();

		void clear()
		{
			m_stack.clear();
		}

		void
		checkForInfiniteLoop(const StackGuard&	guard) const;

	private:

		XSLTEngineImpl*			m_processor;
		const XalanElement* 	m_xslRule;
		const XalanNode*		m_sourceXML;

		StackGuardStackType 	m_stack;
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
};



#endif	// XALAN_XSLTENGINEIMPL_HEADER_GUARD
