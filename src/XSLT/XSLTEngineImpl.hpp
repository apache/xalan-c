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
#include <XPath/NameSpace.hpp>



#include "KeyDeclaration.hpp"
#include "ProblemListenerDefault.hpp"
#include "StylesheetExecutionContext.hpp"
#include "XSLTProcessorException.hpp"



// Forward definitions
class GenerateEvent;
class PrintWriter;
class ResultTreeFragBase;
class StylesheetConstructionContext;
class StylesheetExecutionContext;
class StylesheetRoot;
class XalanAttr;
class XalanText;
class XLocator;
class XMLParserLiaison;
class XMLURL;
class XObject;
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

	typedef XALAN_STD auto_ptr<XPathProcessor>			XPathProcessorPtrType;
	typedef XALAN_STD map<XalanDOMString, int>			AttributeKeysMapType;
	typedef XALAN_STD map<XalanDOMString, int>			ElementKeysMapType;
	typedef XALAN_STD map<const void*, clock_t>			DurationsTableMapType;
	typedef XALAN_STD vector<const Locator*>			LocatorStack;
	typedef XALAN_STD vector<NameSpace> 				NamespaceVectorType;
	typedef XALAN_STD vector<NamespaceVectorType>		NamespacesStackType;
	typedef XALAN_STD vector<TraceListener*>			TraceListenerVectorType;
	typedef XALAN_STD vector<bool>						BoolVectorType;
#undef XALAN_STD

	typedef Function::XObjectArgVectorType				XObjectArgVectorType;
	typedef StylesheetExecutionContext::ParamVectorType	ParamVectorType;

	// Public members
	//---------------------------------------------------------------------

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
	static void
	Initialize();

	/**
	 * Perform termination of statics -- can be called to make memory
	 * leak detection easier.
	 */
	static void
	Terminate();

	// These methods are inherited from XSLTProcessor ...
	
	virtual void
	process(
			XSLTInputSource&				inputSource, 
			XSLTInputSource&				stylesheetSource,
			XSLTResultTarget&				outputTarget,
			StylesheetConstructionContext&	constructionContext,
			StylesheetExecutionContext& 	executionContext);

	virtual void
	process(
			XSLTInputSource&				inputSource,
			XSLTResultTarget&				outputTarget,
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
	outputToResultTree(
			StylesheetExecutionContext&		executionContext,
			const XObject&					xobj);

	virtual void
	resetCurrentState(
			XalanNode*	sourceTree,
			XalanNode*	xmlNode);

	virtual XalanDOMString
	evaluateAttrVal(
			XalanNode*				contextNode,
			const PrefixResolver& 	namespaceContext,
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

	virtual void
	setStylesheetParam(	
					const XalanDOMString&	key,
					const XalanDOMString&	expression);

	virtual void
	setStylesheetParam(
			const XalanDOMString&	key,
			XObject*				value);

	virtual bool
	shouldStripSourceNode(
			XPathExecutionContext&	executionContext,
			const XalanNode&		node) const;

	virtual FormatterListener*
	getFormatterListener() const;

	virtual void
	setFormatterListener(FormatterListener* flistener);

	// Trace-related functions...

	virtual unsigned long
	getTraceListeners() const;

	virtual void
	addTraceListener(TraceListener* 	tl);

	virtual void
	removeTraceListener(TraceListener*	tl);

	virtual void
	fireGenerateEvent(const GenerateEvent&	ge);
	  
	virtual void
	fireTraceEvent(const TracerEvent&	te);

	virtual void
	fireSelectEvent(const SelectionEvent&	se);

	virtual bool
	getTraceSelects() const;

	virtual void
	setTraceSelects(bool	b);

	virtual void
	traceSelect(
			const XalanElement&		theTemplate,
			const NodeRefListBase&	nl) const;

	virtual void
	setQuietConflictWarnings(bool	b);

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
	void
	flushPending();

	/**
	 * Add a namespace declaration to the namespace stack
	 *
	 * @param prefix	   namespace prefix
	 * @param namespaceVal value of namespace
	 */
	void
	addResultNamespaceDecl(
			const XalanDOMString&	prefix, 
			const XalanDOMString&	namespaceVal);

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
	outputResultTreeFragment(
			StylesheetExecutionContext&		executionContext,
			const XObject& 					theTree);

	/**
	 * Retrieve the root stylesheet.
	 * 
	 * @return pointer to root stylesheet
	 */
	virtual const StylesheetRoot*
	getStylesheetRoot() const;

	/**
	 * Set the root stylesheet.
	 * 
	 * @param theStylesheet pointer to new root stylesheet
	 */
	virtual void
	setStylesheetRoot(const StylesheetRoot*		theStylesheet);

	/**
	 * Set the execution context.
	 * 
	 * @param theExecutionContext pointer to new execution context.
	 */
	virtual void
	setExecutionContext(StylesheetExecutionContext*		theExecutionContext);

	/**
	 * Retrieve the URI for the current XSL namespace, for example,
	 * "http://www.w3.org/1999/XSL/Transform"
	 * 
	 * @return URI string
	 */
	static const XalanDOMString&
	getXSLNameSpaceURL()
	{
		return s_XSLNameSpaceURL;
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
	 * Get the latest XSLT version currently supported.
	 *
	 * @return XSLT version number
	 */
	static double
	getXSLTVerSupported();

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
	 * ELEMNAME_XXX constants defined in Constants.hpp
	 *
	 * @param name a probable xsl:xxx element
	 * @return Constants.ELEMNAME_XXX token, -1 if in XSL or Xalan namespace,
	 *		   or -2 if not in known namespace
	 */
	static int
	getElementToken(const XalanDOMString&	name)
	{
		AttributeKeysMapType::const_iterator iter=
			s_elementKeys.find(name);

		return iter == s_elementKeys.end() ? -2 : (*iter).second;
	}

	/**
	 * Given an XSL tag name, return an integer token that corresponds to
	 * ELEMNAME_XXX constants defined in Constants.hpp.
	 *
	 * @param name a probable xsl:xxx element
	 * @return Constants.ELEMNAME_XXX token, -1 if in XSL or Xalan namespace,
	 *		   or -2 if not in known namespace
	 */
	static int
	getAttrTok(const XalanDOMString&	name)
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
	diag(const XalanDOMString&	s) const;
  
	/**
	 * Tell if a given element name should output it's text 
	 * as cdata.
	 *
	 * @param elementName name of element
	 * @return true if it should output as cdata
	 */
	bool
	isCDataResultElem(const XalanDOMString&		elementName) const;
	
	/**
	 * Tell if a qualified name equals the current result tree name.
	 *
	 * @param qname 	  QName to compare to
	 * @param elementName current result tree element
	 * @return true if names are the same
	 */
	bool
	qnameEqualsResultElemName(
			const QName&			qname,
			const XalanDOMString&	elementName) const;
	
	/**
	 * Retrieve the result namespace corresponding to a prefix.
	 * 
	 * @param prefix prefix for namespace
	 * @return string for namespace URI
	 */
	XalanDOMString
	getResultNamespaceForPrefix(const XalanDOMString&	prefix) const;
  
	/**
	 * Retrieve the result prefix corresponding to a namespace.
	 * 
	 * @param theNamespace namespace for prefix
	 * @return string for namespace prefix
	 */
	XalanDOMString
	getResultPrefixForNamespace(const XalanDOMString&	theNamespace) const;

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
	const XPath*
	createMatchPattern(
			const XalanDOMString&	str,
			const PrefixResolver&	resolver);

	/**
	 * Return the xpath created by createXPath() or
	 * createMatchPattern().
	 *
	 * @param xpath	   The XPath to return.
	 */
	void
	returnXPath(const XPath*	xpath);

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
	const XObject*
	evalXPathStr(
			const XalanDOMString&	str,
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
	const XObject*
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
	const XObject*
	evalXPathStr(
			const XalanDOMString&	str,
			XalanNode* 				contextNode,
			const XalanElement&		prefixResolver,
			XPathExecutionContext&	executionContext);

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
	 * Given an element, return an attribute value in the form of a string.
	 *
	 * @param el		  element from where to get the attribute
	 * @param key		  name of the attribute
	 * @param contextNode context to evaluate the attribute value template
	 * @return string for attribute value
	 */
	XalanDOMString
	getAttrVal(
			const XalanElement&		el,
			const XalanDOMString&	key,
			const XalanNode& 		contextNode);
  
	/**
	 * Given an element, return an attribute value in the form of a string.
	 *
	 * @param el		  element from where to get the attribute
	 * @param key		  name of the attribute
	 * @return string for attribute value
	 */
	static XalanDOMString
	getAttrVal(
			const XalanElement&		el,
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
	 * Reset the state.  This needs to be called after a process() call 
	 * is invoked, if the processor is to be used again.
	 */
	virtual void
	reset();

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
	 * Set the problem listener property. The XSL class can have a single
	 * listener that can be informed of errors and warnings, and can normally
	 * control if an exception is thrown or not (or the problem listeners can
	 * throw their own RuntimeExceptions).
	 *
	 * @param l pointer to ProblemListener interface
	 */
	void
	setProblemListener(ProblemListener*		l)
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
	ProblemListener*
	getProblemListener() const
	{
		return m_problemListener;
	}

	/*
	 * See if there is a pending start document event waiting.
	 * @return true if there is a start document event waiting.
	 */
	bool
	getHasPendingStartDocument() const
	{
		return m_hasPendingStartDocument;
	}

	/*
	 * Set the pending start document event state.
	 * @param the new value
	 */
	void
	setHasPendingStartDocument(bool	b)
	{
		m_hasPendingStartDocument = b;
	}

	/*
	 * See if a pending start document event must be flushed.
	 * @return true if the event must be flushed.
	 */
	bool
	getMustFlushPendingStartDocument() const
	{
		return m_mustFlushStartDocument;
	}

	/*
	 * Set the pending start document event flush state.
	 * @param the new value
	 */
	void
	setMustFlushPendingStartDocument(bool	b)
	{
		m_hasPendingStartDocument = b;
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
		const XMLCh* const	chars,
		const unsigned int	length);

	virtual void
	ignorableWhitespace(
			const XMLCh* const	ch,
			const unsigned int	length);

	virtual void
	processingInstruction(
			const XMLCh* const	target,
			const XMLCh* const	data);

	virtual void
	resetDocument();

protected:

	/**
	 * If true, output carriage returns.
	 */
	bool	m_outputCarriageReturns;

	/**
	 * If true, output linefeeds.
	 */
	bool	m_outputLinefeeds;

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
	 * The pending element.  We have to delay the call to 
	 * m_flistener.startElement(name, atts) because of the 
	 * xsl:attribute and xsl:copy calls.	In other words, 
	 * the attributes have to be fully collected before you 
	 * can call startElement.
	 */

	XalanDOMString		m_pendingElementName;

	/**
	 * The pending attributes.	We have to delay the call to 
	 * m_flistener.startElement(name, atts) because of the 
	 * xsl:attribute and xsl:copy calls.  In other words, 
	 * the attributes have to be fully collected before you 
	 * can call startElement.
	 */
	AttributeListImpl	m_pendingAttributes;

	/*
	 * true if a startDocument() event has been fired, but we
	 * haven't yet calld startDocument() on our formatter.
	 */
	bool				m_hasPendingStartDocument;

	/*
	 * true if a pending startDocument() must be flushed.
	 */
	bool				m_mustFlushStartDocument;

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
	 * Determine if any pending attributes is a default
	 * namespace.
	 */
	bool
	pendingAttributesHasDefaultNS() const; 

  /**
   * The top of this stack should contain the currently processed
   * stylesheet SAX locator object.
   */
	LocatorStack  m_stylesheetLocatorStack;

	/**
	 * The XSL class can have a single listener that can be informed 
	 * of errors and warnings, and can normally control if an exception
	 * is thrown or not (or the problem listeners can throw their 
	 * own RuntimeExceptions).
	 */
	ProblemListenerDefault	m_defaultProblemListener;

	ProblemListener*	m_problemListener;

	/**
	 * The root of a linked set of stylesheets.
	 */
	const StylesheetRoot* 				m_stylesheetRoot;

	/**
	 * The namespace that we must match as a minimum for XSLT.
	 */
	static const XalanDOMString		s_XSLNameSpaceURL;	//"http://www.w3.org/1999/XSL/Transform"

	/**
	 * The minimum version of XSLT supported.
	 */
	static const double s_XSLTVerSupported; // 1.0

	/**
	 * Special Xalan namespace for built-in extensions.
	 */
	static const XalanDOMString s_XSLT4JNameSpaceURL; // "http://xml.apache.org/xslt"


	/**
	 * Hash table that can look up XSLT4J extensions element IDs via name.
	 */
	static ElementKeysMapType		s_XSLT4JElementKeys;

	/**
	 * Hash table of XSLT IDs for attribute names.
	 */
	static AttributeKeysMapType s_attributeKeys;

	/**
	 * Hash table of XSLT element IDs for element names.
	 */
	static ElementKeysMapType	s_elementKeys;

	/**
	 * If this is set to true, selects will be traced
	 */
	bool	m_traceSelects;

	/**
	 * If this is set to true, do not warn about pattern 
	 * match conflicts.
	 */
	bool	m_quietConflictWarnings;

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


  //==========================================================
  // SECTION: Function to do with attribute handling
  //==========================================================

	/**
	 * This is used whenever a unique namespace is needed.
	 */
	mutable unsigned long	m_uniqueNSValue;	// 0
  
	/**
	 * This should probably be in the XMLParserLiaison interface.
	 */
	XalanDOMString
	getPrefixForNamespace(
			const XalanDOMString&	theNamespace,
			const XalanElement&		namespaceContext) const;

	/**
	 * Translate CSS attributes and put them in a style tag.
	 * @deprecated
	 */
	void
	translateCSSAttrsToStyleAttr(AttributeListImpl&		attList);

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
	 * Get a DOM document, primarily for creating result 
	 * tree fragments.
	 */
	virtual XalanDocument*
	getDOMFactory() const;

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
	 * Current execution context...
	 */
	StylesheetExecutionContext*		m_executionContext;


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
