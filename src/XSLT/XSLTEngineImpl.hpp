/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights 
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



#include <XalanDOM/XalanDOMString.hpp>



#if defined(XALAN_AUTO_PTR_REQUIRES_DEFINITION)
#include <XPath/XPathProcessor.hpp>
#endif



#include <Include/XalanAutoPtr.hpp>



#include <PlatformSupport/AttributeListImpl.hpp>
#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/PrefixResolver.hpp>



#include <XPath/Function.hpp>
#include <XPath/NameSpace.hpp>



#include "KeyDeclaration.hpp"
#include "OutputContextStack.hpp"
#include "ProblemListenerDefault.hpp"
#include "ResultNamespacesStack.hpp"
#include "StylesheetExecutionContext.hpp"
#include "XSLTProcessorException.hpp"



// Forward declarations...
class DocumentHandler;
class DOMSupport;
class GenerateEvent;
class InputSource;
class PrintWriter;
class ResultTreeFragBase;
class StylesheetConstructionContext;
class StylesheetExecutionContext;
class StylesheetRoot;
class XalanAttr;
class XalanSourceTreeDocument;
class XalanText;
class XLocator;
class XMLParserLiaison;
class XMLURL;
class XObject;
class XPathEnvSupport;
class XPathFactory;
class XPathProcessor;
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

class XALAN_XSLT_EXPORT XSLTEngineImpl : public XSLTProcessor, public PrefixResolver
{
public:

#if defined(XALAN_STRICT_ANSI_HEADERS)
	typedef std::clock_t	ClockType;
#else
	typedef clock_t			ClockType;
#endif

	struct LessXalanDOMStringPointers
	{
		bool
		operator()(
				const XalanDOMString*	theLHS,
				const XalanDOMString*	theRHS) const
		{
			if (theLHS == 0 && theRHS != 0)
			{
				return true;
			}
			else if (theRHS == 0)
			{
				return false;
			}
			else
			{
				return theLHS->compare(*theRHS) < 0 ? true : false;
			}
		}
	};

#if defined(XALAN_NO_NAMESPACES)
	typedef map<XalanDOMString,
				int,
				less<XalanDOMString> >		ElementKeysMapType;
	typedef map<const void*,
				ClockType,
				less<const void*> >			DurationsTableMapType;
	typedef vector<const Locator*>			LocatorStack;
	typedef vector<TraceListener*>			TraceListenerVectorType;
	typedef vector<bool>					BoolVectorType;
	typedef set<const XalanDOMString*,
				LessXalanDOMStringPointers>	XalanDOMStringPointerSetType;
#else
	typedef std::map<XalanDOMString, int>		ElementKeysMapType;
	typedef std::map<const void*, ClockType>	DurationsTableMapType;
	typedef std::vector<const Locator*>			LocatorStack;
	typedef std::vector<TraceListener*>			TraceListenerVectorType;
	typedef std::vector<bool>					BoolVectorType;
	typedef std::set<const XalanDOMString*,
					 LessXalanDOMStringPointers>	XalanDOMStringPointerSetType;
#endif

	typedef XalanAutoPtr<XPathProcessor>				XPathProcessorPtrType;
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
	 * @param xpathEnvSupport  An XPathEnvSupport instance
	 * @param domSupport	   A DOMSupport instance for the DOM being used
	 * @param xobjectFactory   object responsible for XObject objects
	 * @param xpathFactory	   object responsible for XPath objects
	 */
	XSLTEngineImpl(
			XMLParserLiaison&	parserLiaison,
			XPathEnvSupport&	xpathEnvSupport,
			DOMSupport&			domSupport,
			XObjectFactory& 	xobjectFactory,
			XPathFactory&		xpathFactory);

	virtual
	~XSLTEngineImpl();

	/**
	 * Perform initialization of statics -- must be called before any
	 * processing occurs.  See class XSLTInit.
	 */
	static void
	initialize();

	/**
	 * Perform termination of statics.  See class XSLTInit.
	 */
	static void
	terminate();

	// These methods are inherited from XSLTProcessor ...
	
	virtual void
	process(
			const XSLTInputSource&			inputSource, 
			const XSLTInputSource&			stylesheetSource,
			XSLTResultTarget&				outputTarget,
			StylesheetConstructionContext&	constructionContext,
			StylesheetExecutionContext& 	executionContext);

	virtual void
	process(
			const XSLTInputSource&			inputSource,
			XSLTResultTarget&				outputTarget,
			StylesheetExecutionContext& 	executionContext);

	virtual StylesheetRoot*
	processStylesheet(
			const XSLTInputSource&			stylesheetSource,
			StylesheetConstructionContext&	constructionContext);

	virtual StylesheetRoot*
	processStylesheet(
			const XalanDOMString&			xsldocURLString,
			StylesheetConstructionContext&	constructionContext);

	virtual XalanNode*
	getSourceTreeFromInput(const XSLTInputSource&	inputSource);

	virtual void
	resolveTopLevelParams(StylesheetExecutionContext&	executionContext);

	virtual XMLParserLiaison&
	getXMLParserLiaison() const;

	virtual const XalanDOMString
	getUniqueNamespaceValue();

	virtual void
	getUniqueNamespaceValue(XalanDOMString&		theValue);

	virtual void
	setStylesheetParam(	
					const XalanDOMString&	key,
					const XalanDOMString&	expression);

	virtual void
	setStylesheetParam(
			const XalanDOMString&	key,
			XObjectPtr				value);

	virtual bool
	shouldStripSourceNode(
			XPathExecutionContext&	executionContext,
			const XalanNode&		node) const;

	virtual FormatterListener*
	getFormatterListener() const;

	virtual void
	setFormatterListener(FormatterListener* flistener);

	// Trace-related functions...

	virtual size_type
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

	void
	traceSelect(
			StylesheetExecutionContext& 	executionContext,
			const ElemTemplateElement&		theTemplate,
			const NodeRefListBase&			nl,
			const XPath*					xpath) const;

	virtual void
	setQuietConflictWarnings(bool	b);

	virtual void
	setDiagnosticsOutput(PrintWriter*	pw);


	// Inherited from PrefixResolver...

	/**
	 * Retrieve a namespace corresponding to a prefix.  This assumes that 
	 * the PrevixResolver hold's its own namespace context, or is a namespace
	 * context itself.
	 *
	 * @param prefix Prefix to resolve
	 * @return namespace that prefix resolves to, or null if prefix is not found
	 */
	virtual const XalanDOMString*
	getNamespaceForPrefix(const XalanDOMString&		prefix) const;

	/**
	 * Retrieve the base URI for the resolver.
	 * 
	 * @return URI string
	 */
	virtual const XalanDOMString&
	getURI() const;

	/**
	 * Read in the XML file, either producing a Document or calling SAX events,
	 * and register the document in a table.  If the document has already been
	 * read in, it will not be reparsed.
	 *
	 * @param urlString location of the XML
	 * @param docHandler pointer to SAX event handler
	 * @param docToRegister if using a SAX event handler, the object to register in the source docs table. 
	 * @return document object, which represents the parsed XML
	 * @exception SAXException
	 */
	XalanDocument*
	parseXML(
			const XalanDOMString&	urlString,
			DocumentHandler*		docHandler,
			XalanDocument*			docToRegister);

	/**
	 * Read in the XML file, either producing a Document or calling SAX events,
	 * and register the document in a table.  If the document has already been
	 * read in, it will not be reparsed.
	 *
	 * @param inputSource location of the XML
	 * @param docHandler pointer to SAX event handler
	 * @param docToRegister if using a SAX event handler, the object to register in the source docs table. 
	 * @return document object, which represents the parsed XML
	 * @exception SAXException
	 */
	XalanDocument*
	parseXML(
			const InputSource&	inputSource,
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
			const XalanDOMString&	namespaceVal)
	{
		addResultNamespaceDecl(
			prefix,
			namespaceVal.c_str(),
			namespaceVal.length());
	}

	/**
	 * Add a namespace declaration to the namespace stack
	 *
	 * @param prefix namespace prefix
	 * @param namespaceVal value of namespace
	 * @param len length of namespace
	 */
	void
	addResultNamespaceDecl(
			const XalanDOMString&		prefix, 
			const XalanDOMChar*			namespaceVal,
			XalanDOMString::size_type	len)
	{
		m_resultNamespacesStack.addDeclaration(
			prefix,
			namespaceVal,
			len);
	}

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
			AttributeListImpl&		attList,
			const XalanDOMString&	aname,
			const XalanDOMString&	value)
	{
		addResultAttribute(
			attList,
			aname,
			value.c_str());
	}

	/**
	 * Add attribute to attribute list, and if it is a namespace, add it to the
	 * namespaces stack.
	 *
	 * @param attList attribute list added to
	 * @param aname name of attribute
	 * @param value value of attribute
	 */
	void
	addResultAttribute(
			AttributeListImpl&			attList,
			const XalanDOMString&		aname,
			const XalanDOMChar*			value);

	/**
	 * Add attribute to pending attributes list, and if it is a namespace, add
	 * it to the namespaces stack.
	 *
	 * @param aname name of attribute
	 * @param value value of attribute
	 */
	void
	addResultAttribute(
			const XalanDOMString&		aname,
			const XalanDOMChar*			value)
	{
		assert(m_outputContextStack.empty() == false);

		addResultAttribute(
				getPendingAttributesImpl(),
				aname,
				value);
	}

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
		assert(m_outputContextStack.empty() == false);

		addResultAttribute(
				getPendingAttributesImpl(),
				aname,
				value);
	}

	void
	setDocumentLocator(const Locator*	locator);

	void
	startDocument();

	void
	endDocument();
	
	void
	startElement(
			const XalanDOMChar*		name,
			AttributeList&			atts);

	void
	endElement(const XalanDOMChar*	name);

	void
	characters (
			const XalanDOMChar*			ch,
			XalanDOMString::size_type	length);

	void
	ignorableWhitespace(
			const XalanDOMChar*			ch,
			XalanDOMString::size_type	length);

	void
	processingInstruction(
			const XalanDOMChar*		target,
			const XalanDOMChar*		data);

	void
	resetDocument();

	/**
	 * Receive notification of character data.
	 *
	 * @param ch	 pointer to characters from the XML document
	 * @param start  startng offset in 'ch' array
	 * @param length number of characters to read from the array 
	 */
	void
	characters(
			const XalanDOMChar*			ch,
			XalanDOMString::size_type	start,
			XalanDOMString::size_type	length);

	/**
	 * Send character data from the node to the result tree.
	 *
	 * @param node The node to send.
	 */
	void
	characters(const XalanNode&		node);

	/**
	 * Send character data from an XObject to the result tree.
	 *
	 * @param node The xobject to send.
	 */
	void
	characters(const XObjectPtr&	xobject);

	/**
	 * Receive notification of the beginning of an element with an empty
	 * attribute list
	 *
	 * @param name element type name
	 */
	void
	startElement(const XalanDOMChar*	name);

	/**
	 * Receive notification of character data. If available, when the
	 * disable-output-escaping attribute is used, output raw text without
	 * escaping.
	 *
	 * @param ch pointer to characters from the XML document
	 * @param start start position in the array
	 * @param length number of characters to read from the array
	 */
	void
	charactersRaw(
			const XalanDOMChar*			ch,
			XalanDOMString::size_type	start,
			XalanDOMString::size_type	length);

	/**
	 * Send raw character data from the node to the result tree.
	 *
	 * @param node The node to send.
	 */
	void
	charactersRaw(const XalanNode&	node);

	/**
	 * Send raw character data from an XObject to the result tree.
	 *
	 * @param node The xobject to send.
	 */
	void
	charactersRaw(const XObjectPtr&		xobject);

	/**
	 * Called when a Comment is to be constructed.
	 *
	 * @param	data	pointer to comment data
	 */
	void
	comment(const XalanDOMChar*		data);

	/**
	 * Receive notification of a entityReference.
	 *
	 * @param data pointer to characters from the XML document
	 * @exception SAXException
	 */
	void
	entityReference(const XalanDOMChar*		data);

	/**
	 * Receive notification of cdata.
	 *
	 * @param ch	 pointer to characters from the XML document
	 * @param start  start position in the array
	 * @param length number of characters to read from the array
	 */
	void
	cdata(
			const XalanDOMChar*			ch,
			XalanDOMString::size_type	start,
			XalanDOMString::size_type	length);

	/**
	 * Clone a node to the result tree
	 *
	 * @param node      node to clone
	 * @param cloneTextNodesOnly    if true, only text nodes will be cloned
	 * @param styleNode	the stylesheet element that generated the clone.
	 */
	void
	cloneToResultTree(
			const XalanNode&			node,
			bool						cloneTextNodesOnly,
			const ElemTemplateElement*	styleNode);

	/**
	 * Clone a node to the result tree
	 *
	 * @param node					node to clone
	 * @param nodeType				the type of the node
	 * @param isLiteral 			true if a literal element
	 * @param overrideStrip 		false if white space stripping should be done
	 * @param shouldCloneAttributes true if attributes should be cloned
	 * @param cloneTextNodesOnly    if true, only text nodes will be cloned
	 * @param styleNode				the stylesheet element that generated the clone.
	 */
	void
	cloneToResultTree(
			const XalanNode& 			node,
			XalanNode::NodeType			nodeType,
			bool						isLiteral,
			bool						overrideStrip,
			bool						shouldCloneAttributes,
			bool						cloneTextNodesOnly,
			const ElemTemplateElement*	styleNode);

   /**
	* Output an object to the result tree by doing the right conversions.
	*
	* @param value the XObject to output
	* @param outputTextNodesOnly if true, only text nodes will be copied
	*/
	void
	outputToResultTree(
			const XObject& 				value,
			bool						outputTextNodesOnly,
			const ElemTemplateElement*	styleNode);

	/**
	 * Given a result tree fragment, walk the tree and output it to the result
	 * stream.
	 *
	 * @param theTree result tree fragment
	 * @param outputTextNodesOnly if true, only text nodes will be copied
	 * @param styleNode	the stylesheet element that generate the fragment.
	 */
	void
	outputResultTreeFragment(
			const XObject& 				theTree,
			bool						outputTextNodesOnly,
			const ElemTemplateElement*	styleNode);

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
		return s_XalanNamespaceURL;
	}

	/**
	 * Get the latest XSLT version currently supported.
	 *
	 * @return XSLT version number
	 */
	static double
	getXSLTVerSupported();

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
		ElementKeysMapType::const_iterator iter=
			s_elementKeys.find(name);

		return iter == s_elementKeys.end() ? -2 : (*iter).second;
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
			const XalanNode*		sourceNode = 0,
			const XalanNode*		styleNode = 0) const;

	virtual void
	message(
			const XalanDOMString&		msg,
			const XalanNode*			sourceNode,
			const ElemTemplateElement*	styleNode) const;

	/**
	 * Report a message
	 * 
	 * @param msg		 text of message to output
	 * @param sourceNode node in source where error occurred
	 * @param styleNode  node in stylesheet where error occurred
	 */
	virtual void
	message(
			const char*			msg,
			const XalanNode*	sourceNode = 0,
			const XalanNode*	styleNode = 0) const;

	virtual void
	message(
			const XalanDOMString&	msg,
			const Locator&			locator,
			const XalanNode*		sourceNode = 0) const;

	virtual void
	warn(
			const XalanDOMString&	msg,
			const XalanNode*		sourceNode = 0,
			const XalanNode*		styleNode = 0) const;

	virtual void
	warn(
			const XalanDOMString&		msg,
			const XalanNode*			sourceNode,
			const ElemTemplateElement*	styleNode) const;

	virtual void
	warn(
			const char*					msg,
			const XalanNode*			sourceNode,
			const ElemTemplateElement*	styleNode) const;

	virtual void
	warn(
			const char*			msg,
			const XalanNode*	sourceNode = 0,
			const XalanNode*	styleNode = 0) const;

	virtual void
	warn(
			const XalanDOMString&	msg,
			const Locator&			locator,
			const XalanNode*		sourceNode = 0) const;

	virtual void
	error(
			const XalanDOMString&	msg,
			const XalanNode*		sourceNode = 0,
			const XalanNode*		styleNode = 0) const;

	virtual void
	error(
			const XalanDOMString&		msg,
			const XalanNode*			sourceNode,
			const ElemTemplateElement*	styleNode) const;

	virtual void
	error(
			const XalanDOMString&	msg,
			const Locator&			locator,
			const XalanNode*		sourceNode = 0) const;

	/**
	 * Report an error, and throw an exception.
	 * 
	 * @param msg		 text of message to output
	 * @param sourceNode node in source where error occurred
	 * @param styleNode  node in stylesheet where error occurred
	 */
	virtual void
	error(
			const char*			msg,
			const XalanNode*	sourceNode = 0,
			const XalanNode*	styleNode = 0) const;

	/**
	 * Mark the time, so that displayDuration can later display the elapsed
	 * clock ticks.
	 * 
	 * @param theKey pointer to element to push
	 */
	void
	pushTime(const void*	key);

	/**
	 * Returns the duration since pushTime was called for element
	 * in milliseconds.
	 *
	 * @param key pointer to element involved
	 */
	ClockType
	popDuration(const void* 	key);

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
			const void* 			key);


	/**
	 * Whether diagnostic output is to be generated
	 * 
	 * @return true for diagnostics output 
	 */
	bool doDiagnosticsOutput()
	{
		return 0 != m_diagnosticsPrintWriter ? true : false;
	}

	/**
	 * Print a diagnostics string to the output device
	 * 
	 * @param s string to print
	 */
	void
	diag(const XalanDOMString&	s) const;

	/**
	 * Print a diagnostics string to the output device
	 * 
	 * @param s string to print
	 */
	void
	diag(const char*	s) const;

	/**
	 * Retrieve the result namespace corresponding to a prefix.
	 * 
	 * @param prefix prefix for namespace
	 * @return string for namespace URI
	 */
	const XalanDOMString*
	getResultNamespaceForPrefix(const XalanDOMString&	prefix) const;

	/**
	 * Retrieve the result prefix corresponding to a namespace.
	 * 
	 * @param theNamespace namespace for prefix
	 * @return string for namespace prefix
	 */
	const XalanDOMString*
	getResultPrefixForNamespace(const XalanDOMString&	theNamespace) const;

	/**
	 * Determine whether or not a prefix is in use on the pending element or
	 * the pending attributes.
	 * 
	 * @param thePrefix prefix for namespace
	 *
	 * @return true if the prefix is in use, false if not.
	 */
	bool
	isPendingResultPrefix(const XalanDOMString&		thePrefix) const;

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
	 */
	void
	copyNamespaceAttributes(const XalanNode&	src);

	/**
	 * Evaluate an xpath string and return the result.
	 *
	 * @param str			   string to evaluate
	 * @param executionContext current execution context
	 * @return pointer to XObject result
	 */
	const XObjectPtr
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
	const XObjectPtr
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
	const XObjectPtr
	evalXPathStr(
			const XalanDOMString&	str,
			XalanNode* 				contextNode,
			const XalanElement&		prefixResolver,
			XPathExecutionContext&	executionContext);

	/**
	 * Copy an attribute to an AttributeListImpl.
	 *
	 * @param attrName The name of the attribute
	 * @param attrValue The value of the attribute
	 * @param attrList The attribute list added to
	 */
	void
	copyAttributeToTarget(
			const XalanDOMString&	attrName,
			const XalanDOMString&	attrValue,
			AttributeListImpl&		attrList);

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
	 * Get a DOM document, primarily for creating result 
	 * tree fragments.
	 */
	virtual XalanDocument*
	getDOMFactory() const;

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
	 * Push a new output context using the provided FormatterListener.
	 *
	 * @param A pointer to the FormatterListener instance for the new context.
	 */
	void
	pushOutputContext(FormatterListener*	theListener)
	{
		m_outputContextStack.pushContext(theListener);
	}

	/*
	 * Pop the current output context.
	 */
	void
	popOutputContext()
	{
		m_outputContextStack.popContext();
	}

	/*
	 * See if there is a pending start document event waiting.
	 * @return true if there is a start document event waiting.
	 */
	bool
	getHasPendingStartDocument() const
	{
		return getHasPendingStartDocumentImpl();
	}

	/*
	 * Set the pending start document event state.
	 * @param the new value
	 */
	void
	setHasPendingStartDocument(bool	b)
	{
		setHasPendingStartDocumentImpl(b);
	}

	/*
	 * See if a pending start document event must be flushed.
	 * @return true if the event must be flushed.
	 */
	bool
	getMustFlushPendingStartDocument() const
	{
		return getMustFlushPendingStartDocumentImpl();
	}

	/*
	 * Set the pending start document event flush state.
	 * @param the new value
	 */
	void
	setMustFlushPendingStartDocument(bool	b)
	{
		setMustFlushPendingStartDocumentImpl(b);
	}

	/**
	 * Get the list of attributes yet to be processed
	 * 
	 * @return attribute list
	 */
	const AttributeList&
	getPendingAttributes() const
	{
		return getPendingAttributesImpl();
	}

	/**
	 * Set the list of attributes yet to be processed
	 * 
	 * @param pendingAttributes The attribute list
	 */
	void
	setPendingAttributes(const AttributeList&	pendingAttributes)
	{
		getPendingAttributesImpl() = pendingAttributes;
	}

	/**
	 * Replace the contents of a pending attribute.
	 * 
	 * @param theName           name of attribute
	 * @param theNewType        type of attribute
	 * @param theNewValue       new value of attribute
	 */
	void
	replacePendingAttribute(
			const XalanDOMChar*		theName,
			const XalanDOMChar*		theNewType,
			const XalanDOMChar*		theNewValue)
	{
		// Remove the old attribute, then add the new one.  AttributeListImpl::addAttribute()
		// does this for us.
		getPendingAttributesImpl().addAttribute(theName, theNewType, theNewValue);
	}

	bool
	isElementPending() const
	{
		return length(getPendingElementNameImpl()) != 0 ? true : false;
	}

	/**
	 * Retrieve name of the pending element currently being processed.
	 * 
	 * @return element name
	 */
	const XalanDOMString&
	getPendingElementName() const
	{
		return getPendingElementNameImpl();
	}

	/**
	 * Changes the currently pending element name.
	 * 
	 * @param elementName new name of element
	 */
	void
	setPendingElementName(const XalanDOMString&		elementName)
	{
		setPendingElementNameImpl(elementName);
	}

	void
	setPendingElementName(const XalanDOMChar*	elementName)
	{
		setPendingElementNameImpl(elementName);
	}

	/**
	 * Get the locator from the top of the locator stack.
	 *
	 * @return A pointer to the Locator, or 0 if there is nothing on the stack.
	 */
	const Locator*
	getLocatorFromStack() const
	{
		return m_stylesheetLocatorStack.empty() == true ? 0 : m_stylesheetLocatorStack.back();
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
		if (m_stylesheetLocatorStack.empty() == false)
		{
			m_stylesheetLocatorStack.pop_back();
		}
	}

protected:

	/**
	 * Get the list of attributes yet to be processed
	 * 
	 * @return attribute list
	 */
	const AttributeListImpl&
	getPendingAttributesImpl() const
	{
		return m_outputContextStack.getPendingAttributes();
	}

	/**
	 * Get the list of attributes yet to be processed
	 * 
	 * @return attribute list
	 */
	AttributeListImpl&
	getPendingAttributesImpl()
	{
		return m_outputContextStack.getPendingAttributes();
	}

	/**
	 * Set the list of attributes yet to be processed
	 * 
	 * @param pendingAttributes The attribute list
	 */
	void
	setPendingAttributesImpl(const AttributeList&	pendingAttributes)
	{
		getPendingAttributesImpl() = pendingAttributes;
	}

	/**
	 * Retrieve name of the pending element currently being processed.
	 * 
	 * @return element name
	 */
	const XalanDOMString&
	getPendingElementNameImpl() const
	{
		return m_outputContextStack.getPendingElementName();
	}

	/**
	 * Retrieve name of the pending element currently being processed.
	 * 
	 * @return element name
	 */
	XalanDOMString&
	getPendingElementNameImpl()
	{
		return m_outputContextStack.getPendingElementName();
	}

	/**
	 * Changes the currently pending element name.
	 * 
	 * @param elementName new name of element
	 */
	void
	setPendingElementNameImpl(const XalanDOMString&		elementName)
	{
		m_outputContextStack.getPendingElementName() = elementName;
	}

	/**
	 * Changes the currently pending element name.
	 * 
	 * @param elementName new name of element
	 */
	void
	setPendingElementNameImpl(const XalanDOMChar*	elementName)
	{
		assert(elementName != 0);

		m_outputContextStack.getPendingElementName() = elementName;
	}

	/*
	 * See if there is a pending start document event waiting.
	 * @return true if there is a start document event waiting.
	 */
	bool
	getHasPendingStartDocumentImpl() const
	{
		return m_outputContextStack.getHasPendingStartDocument();
	}

	/*
	 * Set the pending start document event state.
	 * @param the new value
	 */
	void
	setHasPendingStartDocumentImpl(bool		b)
	{
		m_outputContextStack.getHasPendingStartDocument() = b;
	}

	/*
	 * See if a pending start document event must be flushed.
	 * @return true if the event must be flushed.
	 */
	bool
	getMustFlushPendingStartDocumentImpl() const
	{
		return m_outputContextStack.getMustFlushPendingStartDocument();
	}

	/*
	 * Set the pending start document event flush state.
	 * @param the new value
	 */
	void
	setMustFlushPendingStartDocumentImpl(bool	b)
	{
		m_outputContextStack.getMustFlushPendingStartDocument() = b;
	}

	FormatterListener*
	getFormatterListenerImpl() const
	{
		return m_outputContextStack.getFormatterListener();
	}

	void
	setFormatterListenerImpl(FormatterListener*		flistener)
	{
		m_outputContextStack.getFormatterListener() = flistener;
	}

	/**
	 * If true, build DOM-based result tree fragments.
	 */
	bool					m_useDOMResultTreeFactory;

	/**
	 * The factory that will be used to create DOM-based result tree fragments.
	 */
	mutable XalanDocument*	m_domResultTreeFactory;

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
	 * Copy the attributes from the XSL element to the created 
	 * output element.
	 */
	void
	copyAttributesToAttList(
			const XalanNode&	node,
			AttributeListImpl&	attList);

	// Factory for creating xpaths.
	XPathFactory&			m_xpathFactory;

	// Factory for creating xobjects
	XObjectFactory& 		m_xobjectFactory;

	// The query/pattern-matcher object.
	const XPathProcessorPtrType		m_xpathProcessor;

	/**
	 * Stack of Booleans to keep track of if we should be outputting 
	 * cdata instead of escaped text.
	 */
	BoolVectorType	m_cdataStack;

private:

	/**
	 * Issue a warning that only text nodes can be copied.
	 *
	 * @param sourceNode node in source where error occurred
	 * @param styleNode  node in stylesheet where error occurred
	 */
	void
	warnCopyTextNodesOnly(
			const XalanNode*			sourceNode,
			const ElemTemplateElement*	styleNode) const;

	/**
	 * Clone a text node to the result tree
	 *
	 * @param node					node to clone
	 * @param isLiteral 			true if a literal element
	 * @param overrideStrip 		false if white space stripping should be done
	 */
	void
	cloneToResultTree(
			const XalanText&	node,
			bool				isLiteral,
			bool				overrideStrip);

	/**
	 * Determine if any pending attributes is a default
	 * namespace.
	 */
	bool
	pendingAttributesHasDefaultNS() const; 

	void
	addResultNamespace(
			const XalanDOMString&	thePrefix,
			const XalanDOMString&	theName,
			const XalanNode&		theNode,
			AttributeListImpl&		thePendingAttributes,
			bool					fOnlyIfPrefixNotPresent);

	void
	addResultNamespace(
			const XalanNode&	theNode,
			AttributeListImpl&	thePendingAttributes,
			bool				fOnlyIfPrefixNotPresent = false);

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
	static const XalanDOMString&		s_XSLNameSpaceURL;	//"http://www.w3.org/1999/XSL/Transform"

	/**
	 * The minimum version of XSLT supported.
	 */
	static const double					s_XSLTVerSupported; // 1.0

	/**
	 * Special Xalan namespace for built-in extensions.
	 */
	static const XalanDOMString&		s_XalanNamespaceURL; // "http://xml.apache.org/xalan"

	/**
	 * Prefix to use when generating unique namespaces.
	 */
	static const XalanDOMString&		s_uniqueNamespacePrefix;

	/**
	 * Map of XSLT element IDs for element names.
	 */
	static const ElementKeysMapType&	s_elementKeys;

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
	DurationsTableMapType	m_durationsTable;

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
			const XalanNode*					sourceNode,
			const XalanNode*					styleNode) const;

	void
	problem(
			const XalanDOMString&				msg, 
			ProblemListener::eClassification	classification,
			const XalanNode*					sourceNode,
			const ElemTemplateElement*			styleNode) const;

	void
	problem(
			const XalanDOMString&				msg, 
			ProblemListener::eClassification	classification,
			const Locator&						locator,
			const XalanNode*					sourceNode) const;

  //==========================================================
  // SECTION: Function to do with attribute handling
  //==========================================================

	/**
	 * This is used whenever a unique namespace is needed.
	 */
	unsigned long	m_uniqueNSValue;

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

private:

	bool
	generateCDATASection() const
	{
		if (m_hasCDATASectionElements == false)
		{
			return false;
		}
		else
		{
			assert(m_cdataStack.empty() == false);

			return m_cdataStack.back();
		}
	}

	void
	doFlushPending()
	{
		setMustFlushPendingStartDocument(true);

		flushPending();
	}

	/**
	 * Tell if a given element name should output its text 
	 * as cdata.
	 *
	 * @param elementName name of element
	 * @return true if it should output as cdata
	 */
	bool
	isCDataResultElem(const XalanDOMString&		elementName) const;

	void
	fireCharacterGenerateEvent(
			const XalanNode&	theNode,
			bool				isCDATA);

	void
	fireCharacterGenerateEvent(
			const XObjectPtr&	theXObject,
			bool				isCDATA);

	void
	fireCharacterGenerateEvent(
			const XalanDOMString&	theString,
			bool					isCDATA);

	void
	fireCharacterGenerateEvent(
			const XalanDOMChar*			ch,
			XalanDOMString::size_type	start,
			XalanDOMString::size_type	length,
			bool						isCDATA);

	void
	checkDefaultNamespace(
			const XalanDOMString&	theElementName,
			const XalanDOMString&	theElementNamespaceURI);

	XMLParserLiaison&	m_parserLiaison;

	XPathEnvSupport&	m_xpathEnvSupport;

	DOMSupport&			m_domSupport;

	/**
	 * Current execution context...
	 */
	StylesheetExecutionContext*		m_executionContext;

	/*
	 * Stack of current output contexts...
	 */
	OutputContextStack				m_outputContextStack;

	/*
	 * Stack of current result namespaces...
	 */
	ResultNamespacesStack			m_resultNamespacesStack;

	/*
	 * Dummy AttributeListImpl
	 */
	AttributeListImpl				m_dummyAttributesList;

	XalanDOMString					m_scratchString;

	XalanDOMStringPointerSetType	m_attributeNamesVisited;

	const XalanDOMStringPointerSetType::iterator	m_attributeNamesVisitedEnd;

	bool							m_hasStripOrPreserveSpace;

	bool							m_hasCDATASectionElements;

	static void
	installFunctions();

	static void
	uninstallFunctions();

	static void
	initializeElementKeysTable(ElementKeysMapType&	theElementKeys);

	static const XalanDOMString		s_emptyString;

	static const XalanDOMString&	s_stylesheetNodeName;
	static const XalanDOMString&	s_typeString;
	static const XalanDOMString&	s_hrefString;
	static const XalanDOMString&	s_piTokenizerString;
	static const XalanDOMString&	s_typeValueString1;
	static const XalanDOMString&	s_typeValueString2;
	static const XalanDOMString&	s_typeValueString3;
	static const XalanDOMString&	s_typeValueString4;

	// Not implemented...
    XSLTEngineImpl(const XSLTEngineImpl&);

    XSLTEngineImpl&
    operator=(const XSLTEngineImpl&);

    bool
    operator==(const XSLTEngineImpl&) const;
};



#endif	// XALAN_XSLTENGINEIMPL_HEADER_GUARD
