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
#if !defined(XPATHENVSUPPORT_HEADER_GUARD_1357924680)
#define XPATHENVSUPPORT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



#include <map>
#include <vector>



#include <dom/DOM_Document.hpp>
#include <dom/DOM_Element.hpp>
#include <dom/DOM_NodeList.hpp>
#include <dom/DOMString.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/Resettable.hpp>



class NodeRefListBase;
class PrefixResolver;
class XLocator;
class XObject;
class XPathExecutionContext;
class XObjectFactory;
class QName;



class XALAN_XPATH_EXPORT XPathEnvSupport : public Resettable
{
public:

	XPathEnvSupport();

	virtual
	~XPathEnvSupport();

	/**
	 * Given a valid element key, return the corresponding node list.
	 *
	 * @param doc              source document
	 * @param name             name of the key, which must match the 'name'
	 *                         attribute on xsl:key
	 * @param ref              value that must match the value found by the
	 *                         'match' attribute on xsl:key
	 * @param nscontext        context node for namespace resolution
	 * @param executionContext current execution context
	 * @return if the name was not declared with xsl:key, this will return
	 *         null, if the identifier is not found, it will return an empty
	 *         node set, otherwise it will return a nodeset of nodes.
	 */
	virtual const NodeRefListBase*
	getNodeSetByKey(
			const DOM_Node&			doc,
			const DOMString&		name,
			const DOMString&		ref,
			const DOM_Element&		nscontext,
			XPathExecutionContext&	executionContext) const = 0;

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
	 *         null, if the identifier is not found, it will return an empty
	 *         node set, otherwise it will return a nodeset of nodes.
	 */
	virtual const NodeRefListBase*
	getNodeSetByKey(
			const DOM_Node&			doc,
			const DOMString&		name,
			const DOMString&		ref,
			const PrefixResolver&	resolver,
			XPathExecutionContext&	executionContext) const = 0;

	/**
	 * Given a name, locate a variable in the current context, and return 
	 * a pointer to the object.
	 *
	 * @param theName name of variable
	 * @return pointer to an XObject if the variable was found, 0 if it was not
	 */
	virtual XObject*
	getVariable(
			XObjectFactory&		factory,
			const QName&		name) const = 0;

	/**
	 * Provides support for XML parsing service.
	 *
	 * @param urlString location of the XML
	 * @param base base location for URI
	 * @return parsed document
	 */
	virtual DOM_Document
	parseXML(
			const DOMString&	urlString,
			const DOMString&	base) const = 0;

	/**
	 * Get the source document for the given URI.
	 *
	 * @param theURI document URI
	 * @return source document
	 */
	virtual DOM_Document
	getSourceDocument(const DOMString&	theURI) const = 0;

	/**
	 * Associate a document with a given URI.
	 *
	 * @param theURI      document URI
	 * @param theDocument source document
	 */
	virtual void
	setSourceDocument(
			const DOMString&		theURI,
			const DOM_Document&		theDocument) = 0;

	/**
	 * Given a DOM Document, tell what URI was used to parse it. Needed for
	 * relative resolution.
	 *
	 * @param owner source document
	 * @return document URI
	 */
	virtual DOMString
	findURIFromDoc(const DOM_Document&	owner) const = 0;

	/**
	 * Get a DOM document, primarily for creating result tree fragments.
	 *
	 * @return DOM document
	 */
	virtual DOM_Document
	getDOMFactory() const = 0;

	/**
	 * Determine if an external function is available.
	 *
	 * @param theNamespace  namespace for function
	 * @param extensionName name of extension function
	 * @return whether the given function is available or not
	 */
	virtual bool
	functionAvailable(
			const DOMString&	theNamespace, 
			const DOMString&	extensionName) const = 0;

	/**
	 * Handle an extension function.
	 * 
	 * @param executionContext  current execution context
	 * @param theNamespace  namespace of function    
	 * @param extensionName extension function name
	 * @param argVec        vector of arguments to function
	 * @return pointer to XObject result
	 */
	virtual XObject*
	extFunction(
			XPathExecutionContext&			executionContext,
			const DOMString&				theNamespace,
			const DOMString&				extensionName, 
			const std::vector<XObject*>&	argVec) const = 0;

	/**
	 * Get an XLocator provider keyed by node.  This gets the association
	 * based on the root of the tree that is the node's parent.
	 *
	 * @param node node for locator
	 * @return pointer to locator
	 */
	virtual XLocator*
	getXLocatorFromNode(const DOM_Node&	node) const = 0;

	/**
	 * Associate an XLocator provider to a node.  This makes the association
	 * based on the root of the tree that is the node's parent.
	 *
	 * @param node     node for association
	 * @param xlocator locator to associate with node
	 */
	virtual void
	associateXLocatorToNode(
			const DOM_Node&		node,
			XLocator*			xlocator) const = 0;

	/**
	 * Tells, through the combination of the default-space attribute on
	 * xsl:stylesheet, xsl:strip-space, xsl:preserve-space, and the xml:space
	 * attribute, whether or not extra whitespace should be stripped from the
	 * node.  Literal elements from template elements should <em>not</em> be
	 * tested with this function.
	 *
	 * @param node text node from the source tree
	 * @return true if the text node should be stripped of extra whitespace
	 */
	virtual bool
	shouldStripSourceNode(const DOM_Node&	node) const = 0;

	enum eSource { eXMLParser		= 1,
				   eXSLTProcessor	= 2,
				   eXPATHParser		= 3,
				   eXPATHProcessor	= 4,
				   eDataSource		= 5 };

	enum eClassification { eMessage = 0,
						   eWarning = 1,
						   eError = 2 };

	/**
	 * Function that is called when a problem event occurs.
	 * 
	 * @param   where 			Either eXMLParser, eXSLTProcessor,
	 *							eXPATHParser, eXPATHProcessor, or
	 *							eDataSource.
	 * @param   classification	Either eWarning, or eError
	 * @param   styleNode 		The style tree node where the problem
	 *							occurred.  May be null.
	 * @param   sourceNode		The source tree node where the problem	
	 *							occurred.  May be null.
	 * @param   msg				A string message explaining the problem.
	 * @param   lineNo			The line number where the problem occurred,  
	 *							if it is known. May be zero.
	 * @param   charOffset		The character offset where the problem,  
	 *							occurred if it is known. May be zero.
	 * 
	 * @return  true if the return is an ERROR, in which case
	 *		  exception will be thrown.  Otherwise the processor will 
	 *		  continue to process.
	 */

  /**
   * Function that is called when a problem event occurs.
   * 
	* @param   where 			     either eXMLParser, eXSLTProcessor,
	*							        eXPATHParser, eXPATHProcessor, or eDataSource.
	* @param   classification	  either eWarning, or eError
	* @param   styleNode         style tree node where the problem occurred
	*                            (may be null)
	* @param   sourceNode        source tree node where the problem occurred
	*                            (may be null)
   * @param   msg               string message explaining the problem.
   * @param   lineNo            line number where the problem occurred,  
   *                            if it is known, else zero
   * @param   charOffset        character offset where the problem,  
   *                            occurred if it is known, else zero
   * @return  true if the return is an ERROR, in which case
   *          exception will be thrown.  Otherwise the processor will 
   *          continue to process.
   */
	virtual bool
	problem(
			eSource				where,
			eClassification		classification,
			const DOM_Node&		styleNode,
			const DOM_Node&		sourceNode,
			const DOMString&	msg,
			int					lineNo,
			int					charOffset) const = 0;

  /**
   * Function that is called when a problem event occurs.
   * 
	* @param where 			either eXMLParser, eXSLTProcessor,
	*			 			      eXPATHParser, eXPATHProcessor, or eDataSource.
	* @param classification	either eWarning, or eError
	* @param resolver       resolver for namespace resolution
	* @param styleNode      style tree node where the problem occurred
	*                       (may be null)
	* @param sourceNode     source tree node where the problem occurred
	*                       (may be null)
   * @param msg            string message explaining the problem.
   * @param lineNo         line number where the problem occurred,  
   *                       if it is known, else zero
   * @param charOffset     character offset where the problem,  
   *                       occurred if it is known, else zero
	* @return true if the return is an ERROR, in which case exception will be
	*         thrown.  Otherwise the processor will continue to process.
   */
	virtual bool
	problem(
			eSource					where,
			eClassification			classification,
			const PrefixResolver*	resolver,
			const DOM_Node&			sourceNode,
			const DOMString&		msg,
			int						lineNo,
			int						charOffset) const = 0;

	/**
	 * Query the value of the extend support instance.
	 * 
	 * @return pointer to the  extended support instance, may be 0
	 */
	virtual XPathEnvSupport*
	GetExtendedEnvSupport() const = 0;

	/**
	 * This call is intended to allow extending via delegation.
	 * 
	 * @param   theExtendedSupport pointer to another XPathEnvSupport
	 *									    instance to delegate to, may be 0
	 * 
	 * @return  pointer to the previous extended instance, may be 0
	 */
	virtual XPathEnvSupport*
	SetExtendedEnvSupport(XPathEnvSupport*	theExtendedSupport) = 0;


	// These interfaces are inherited from Resettable...

	virtual void
	reset() = 0;

private:

	// These are not implemented...
	XPathEnvSupport(const XPathEnvSupport&);

	XPathEnvSupport&
	operator=(const XPathEnvSupport&);

	bool
	operator==(const XPathEnvSupport&) const;
};



#endif	// XPATHENVSUPPORT_HEADER_GUARD_1357924680
