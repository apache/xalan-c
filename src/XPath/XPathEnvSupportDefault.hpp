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
#if !defined(XPATHENVSUPPORTDEFAULT_HEADER_GUARD_1357924680)
#define XPATHENVSUPPORTDEFAULT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



#include <vector>



#include <XPath/XPathEnvSupport.hpp>



class XUnknown;



/**
 * Dummy class in order to make the XPath object happy 
 * for diagnostic purposes.
 */
class XALAN_XPATH_EXPORT XPathEnvSupportDefault : public XPathEnvSupport
{
public:

	XPathEnvSupportDefault();

	virtual
	~XPathEnvSupportDefault();

	// These interfaces are inherited from Resettable...

	/**
	 * Reset the instance.
	 */
	virtual void
	reset();

	// These interfaces are inherited from XPathEnvSupport...

	/**
	 * Given a valid element key, return the corresponding node list.
	 */
	virtual const NodeRefListBase*
	getNodeSetByKey(
			const DOM_Node&		doc,
			const DOMString&	name,
			const DOMString&	ref,
			const DOM_Element&	nscontext) const;

	/**
	 * Given a valid element key, return the corresponding node list.
	 */
	virtual const NodeRefListBase*
	getNodeSetByKey(
			const DOM_Node&			doc,
			const DOMString&		name,
			const DOMString&		ref,
			const PrefixResolver&	nscontext) const;

	/**
	 * Given a name, locate a variable in the current context, and return 
	 * the Object.
	 */
	XObject*
	getVariable(
			XPathExecutionContext&	executionContext,
			const QName&			name) const;

	/**
	 * Get table of source tree documents.
	 * Document objects are keyed by URL string.
	 */
	virtual SourceDocsTableType&
	getSourceDocsTable();

	/**
	 * Given a DOM Document, tell what URI was used to parse it.
	 * Needed for relative resolution.
	 */
	virtual DOMString
	findURIFromDoc(const DOM_Document&	owner) const;

	/**
	 * Get a DOM document, primarily for creating result 
	 * tree fragments.
	 */
	virtual DOM_Document
	getDOMFactory() const;

	/**
	 * Execute the function-available() function.
	 */
	virtual bool
	functionAvailable(
			const DOMString&	theNamespace, 
			const DOMString&	extensionName) const;

	// $$$ ToDo: How do we implement this?
	/**
	 * Handle an extension function.
	 */
	virtual XObject*
	extFunction(
			XPathExecutionContext&			executionContext,
			const DOMString&				theNamespace,
			const DOMString&				extensionName, 
			const std::vector<XObject*>&	argVec) const;

	virtual XLocator*
	getXLocatorFromNode(const DOM_Node&		node) const;

	virtual void
	associateXLocatorToNode(
			const DOM_Node&		node,
			XLocator*			xlocator) const;

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
	shouldStripSourceNode(const DOM_Node&	node) const;

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

	virtual bool
	problem(
			eSource				where,
			eClassification		classification,
			const DOM_Node&		styleNode,
			const DOM_Node&		sourceNode,
			const DOMString&	msg,
			int					lineNo,
			int					charOffset) const;

	virtual bool
	problem(
			eSource					where,
			eClassification			classification,
			const PrefixResolver&	resolver,
			const DOM_Node&			sourceNode,
			const DOMString&		msg,
			int						lineNo,
			int						charOffset) const;

	/**
	 * Query the value of the extend support instance.
	 * 
	 * @return  A pointer to the  extended support instance.  May be 0.
	 */
	virtual XPathEnvSupport*
	GetExtendedEnvSupport() const;

	/**
	 * This call is intended to allow extending via delegation.
	 * 
	 * @param   theExtendedSupport 		A pointer to another XPathEnvSupport
	 *									instance to delegate to.  This may be
	 *									0.
	 * 
	 * @return  A pointer to the previous extended instance.  May be 0.
	 */
	virtual XPathEnvSupport*
	SetExtendedEnvSupport(XPathEnvSupport*	theExtendedSupport);

private:

	// These are not implemented...
	XPathEnvSupportDefault(const XPathEnvSupportDefault&);

	XPathEnvSupportDefault&
	operator=(const XPathEnvSupportDefault&);

	bool
	operator==(const XPathEnvSupportDefault&) const;

	// Data members...

	XPathEnvSupport*		m_extendedSupport;

	SourceDocsTableType		m_sourceDocs;
};



#endif	// XPATHENVSUPPORTDEFAULT_HEADER_GUARD_1357924680
