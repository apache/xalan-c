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
#if !defined(XPATHEXECUTIONCONTEXTDEFAULT_HEADER_GUARD_1357924680)
#define XPATHEXECUTIONCONTEXTDEFAULT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



#include <dom/DOMString.hpp>



// Base class include file.
#include <XPath/XPathExecutionContext.hpp>


class XPathEnvSupport;


//
// A basic implementation of the class XPathExecutionContext.
//
class XALAN_XPATH_EXPORT XPathExecutionContextDefault : public XPathExecutionContext
{
public:

	explicit
	XPathExecutionContextDefault(
			XPathEnvSupport&		theXPathEnvSupport,
			XPathSupport&			theXPathSupport,
			XObjectFactory&			theXObjectFactory,
			const DOM_Node&			theCurrentNode = DOM_Node(),
			const NodeRefListBase&	theContextNodeList = NodeRefList(),
			const PrefixResolver*	thePrefixResolver = 0);

	virtual
	~XPathExecutionContextDefault();

	// These interfaces are inherited from ExecutionContext...

	/**
	 * Tell the user of an error, and probably throw an 
	 * exception.
	 */
	virtual void
	error(
			const DOMString&	msg,
			const DOM_Node& 	sourceNode = DOM_Node(),
			const DOM_Node&		styleNode = DOM_Node()) const;

	/**
	 * Tell the user of an warning, and probably throw an 
	 * exception.
	 */
	virtual void
	warn(
			const DOMString&	msg,
			const DOM_Node& 	sourceNode = DOM_Node(),
			const DOM_Node&		styleNode = DOM_Node()) const;

	/**
	 * Output a message.
	 */
	virtual void
	message(
			const DOMString&	msg,
			const DOM_Node& 	sourceNode = DOM_Node(),
			const DOM_Node&		styleNode = DOM_Node()) const;

	// These interfaces are inherited from XPathExecutionContext...

	virtual DOM_Node
	getCurrentNode() const;

	virtual void
	setCurrentNode(const DOM_Node&	theCurrentNode);

	virtual XObjectFactory&
	getXObjectFactory() const;

	/**
	 * Returns the namespace of the given node.
	 */
	virtual DOMString
	getNamespaceOfNode(const DOM_Node&	n) const;

	/**
	 * Returns the local name of the given node.
	 */
	virtual DOMString
	getLocalNameOfNode(const DOM_Node&	n) const;

	/**
	 * Returns the parent of the given node.
	 */
	virtual DOM_Node
	getParentOfNode(const DOM_Node&	n) const;

	/**
	 * Get node data recursively.
	 * (Note whitespace issues.)
	 */
	virtual DOMString
	getNodeData(const DOM_Node&	n) const;

	/**
	 * Get an element from an ID.
	 */
	virtual DOM_Element
	getElementByID(
			const DOMString&		id,
			const DOM_Document&		doc) const;

	virtual const NodeRefListBase&
	getContextNodeList() const;

	/*
	 * Set the current context node list.
	 *
	 */
	virtual void	
	setContextNodeList(const NodeRefListBase&	theList);

	/*
	 * Get the count of nodes in the current context node list.
	 *
	 */
	virtual int
	getContextNodeListLength() const;

	/*
	 * Get the position of the node in the current context node list.
	 *
	 */
	virtual int
	getContextNodeListPosition(const DOM_Node&	contextNode) const;

	/**
	 * Determine if an external function is available.
	 */
	virtual bool
	functionAvailable(
			const DOMString&	theNamespace, 
			const DOMString&	extensionName) const;

	/**
	 * Handle an extension function.
	 */
	virtual XObject*
	extFunction(
			const DOMString&				theNamespace,
			const DOMString&				extensionName, 
			const std::vector<XObject*>&	argVec);

	virtual XLocator*
	getXLocatorFromNode(const DOM_Node&		node) const;

	virtual void
	associateXLocatorToNode(
			const DOM_Node&		node,
			XLocator*			xlocator);

	/**
	 * Provides support for XML parsing service.
	 */
	virtual DOM_Document
	parseXML(
			const DOMString&	urlString,
			const DOMString&	base) const;

	/**
	 * Create a MutableNodeRefList with the appropriate context.
	 */
	virtual MutableNodeRefList
	createMutableNodeRefList() const;

	/**
	 * Tells if namespaces should be supported.  For optimization purposes.
	 */
	virtual bool
	getProcessNamespaces() const;

	virtual const NodeRefListBase*
	getNodeSetByKey(
			const DOM_Node&		doc,
			const DOMString&	name,
			const DOMString&	ref,
			const DOM_Element&	nscontext);

	virtual const NodeRefListBase*
	getNodeSetByKey(
			const DOM_Node&			doc,
			const DOMString&		name,
			const DOMString&		ref);

	virtual const NodeRefListBase*
	getNodeSetByKey(
			const DOM_Node&			doc,
			const DOMString&		name,
			const DOMString&		ref,
			const PrefixResolver&	resolver);

	/**
	 * Given a name, locate a variable in the current context, and return 
	 * the Object.
	 */
	virtual XObject*
	getVariable(
			const QName&			name) const;

	virtual const PrefixResolver*
	getPrefixResolver() const;

	virtual void
	setPrefixResolver(const PrefixResolver*		thePrefixResolver);

	virtual DOMString
	getNamespaceForPrefix(const DOMString&	prefix) const;

	/**
	 * Given a DOM Document, tell what URI was used to parse it.
	 * Needed for relative resolution.
	 */
	virtual DOMString
	findURIFromDoc(const DOM_Document&	owner) const;

	/**
	 * The getUnparsedEntityURI function returns the URI of the unparsed
	 * entity with the specified name in the same document as the context
	 * node (see [3.3 Unparsed Entities]). It returns the empty string if
	 * there is no such entity.
	 */
	virtual DOMString
	getUnparsedEntityURI(
			const DOMString&		theName,
			const DOM_Document&		theDocument) const;

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
	 * These get and set a state to optimize certain XPath operations.
	 */
	virtual bool
	getThrowFoundIndex() const;

	virtual void
	setThrowFoundIndex(bool 	fThrow);

	virtual void
	setCurrentPattern(const DOMString&	thePattern);

	virtual DOMString
	getCurrentPattern() const;

private:

	XPathEnvSupport&		m_xpathEnvSupport;

	XPathSupport&			m_xpathSupport;

	XObjectFactory&			m_xobjectFactory;

	DOM_Node				m_currentNode;

	MutableNodeRefList		m_contextNodeList;

	const PrefixResolver*	m_prefixResolver;

	bool					m_throwFoundIndex;

	DOMString				m_currentPattern;
};



#endif	// XPATHEXECUTIONCONTEXTDEFAULT_HEADER_GUARD_1357924680
