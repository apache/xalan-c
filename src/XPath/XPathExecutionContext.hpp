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
 * @author <a href="mailto:david_n_bertoni@lotus.com">David N. Bertoni</a>
 */
#if !defined(XPATHEXECUTIONCONTEXT_HEADER_GUARD_1357924680)
#define XPATHEXECUTIONCONTEXT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



#include <vector>



#include <XalanDOM/XalanDOMString.hpp>



// Base class header file...
#include <PlatformSupport/ExecutionContext.hpp>



#include <XPath/Function.hpp>
#include <XPath/MutableNodeRefList.hpp>



class DecimalFormatSymbols;
class PrefixResolver;
class QName;
class XLocator;
class XMLURL;
class XObject;
class XObjectFactory;
class XalanDocument;
class XalanNode;
class XalanElement;



//
// An abstract class which provides support for executing XPath functions
// and extension functions.
//

class XALAN_XPATH_EXPORT XPathExecutionContext : public ExecutionContext
{
public:

	typedef Function::XObjectArgVectorType	XObjectArgVectorType;

	explicit
	XPathExecutionContext();

	virtual
	~XPathExecutionContext();

	/**
	 * Retrieve the node currently being executed.
	 * 
	 * @return current node
	 */
	virtual XalanNode*
	getCurrentNode() const = 0;

	/**
	 * Change the node currently being executed.
	 * 
	 * @param theCurrentNode new current node
	 */
	virtual void
	setCurrentNode(XalanNode*	theCurrentNode) = 0;

	class CurrentNodeSetAndRestore
	{
	public:

		CurrentNodeSetAndRestore(
				XPathExecutionContext&	theExecutionContext,
				XalanNode*				theNode) :
			m_executionContext(theExecutionContext),
			m_savedNode(theExecutionContext.getCurrentNode())
		{
			m_executionContext.setCurrentNode(theNode);
		}

		~CurrentNodeSetAndRestore()
		{
			m_executionContext.setCurrentNode(m_savedNode);
		}

	private:

		XPathExecutionContext&	m_executionContext;
		XalanNode* const		m_savedNode;
	};

	/**
	 * Retrieve the factory object for creating XObjects.
	 * 
	 * @return factory object instance
	 */
	virtual XObjectFactory&
	getXObjectFactory() const = 0;

	/**
	 * Retrieve namespace corresponding to a DOM node.
	 * 
	 * @param n DOM node queried
	 * @return namespace string corresponding to 'n'
	 */
	virtual XalanDOMString
	getNamespaceOfNode(const XalanNode&		n) const = 0;

	/**
	 * Returns the local name of the given node.
	 * 
	 * @param n node queried
	 * @return local name string corresponding to 'n'
	 */
	virtual XalanDOMString
	getLocalNameOfNode(const XalanNode&		n) const = 0;

	/**
	 * Returns the parent of the given node.
	 * 
	 * @param n DOM node queried
	 * @return parent node for 'n'
	 */
	virtual XalanNode*
	getParentOfNode(const XalanNode&	n) const = 0;

	/**
	 * Determine if a node is after another node, in document order.
	 *
	 * @param node1 The first node
	 * @param node2 The second node
	 * @return true if node1 one is after node2, or false if it is not.
	 */
	virtual bool
	isNodeAfter(
			const XalanNode&	node1,
			const XalanNode&	node2) const = 0;

	/**
	 * Get node data recursively.
	 * (Note whitespace issues.)
	 * 
	 * @param n DOM node queried
	 * @return string of data for node 'n'
	 */
	virtual XalanDOMString
	getNodeData(const XalanNode&	n) const = 0;

	/**
	 * Given a valid element id, return the corresponding element.
	 *
	 * @param id  string representing ID
	 * @param doc document to search
	 * @return element for ID
	 */
	virtual XalanElement*
	getElementByID(
			const XalanDOMString&		id,
			const XalanDocument&		doc) const = 0;

	/**
	 * Retrieve node list for current context.
	 * 
	 * @return node list
	 */
	virtual const NodeRefListBase&
	getContextNodeList() const = 0;

	/**
	 * Set node list for current context.
	 * 
	 * @param theList new node list
	 */
	virtual void	
	setContextNodeList(const NodeRefListBase&	theList) = 0;

	class ContextNodeListSetAndRestore
	{
	public:

		ContextNodeListSetAndRestore(
				XPathExecutionContext&		theExecutionContext,
				const NodeRefListBase&		theNodeList) :
			m_executionContext(theExecutionContext),
			m_savedNodeList(theExecutionContext.getContextNodeList())
		{
			m_executionContext.setContextNodeList(theNodeList);
		}

		~ContextNodeListSetAndRestore()
		{
			m_executionContext.setContextNodeList(m_savedNodeList);
		}

	private:

		XPathExecutionContext&		m_executionContext;
		const MutableNodeRefList	m_savedNodeList;
	};

	/*
	 * Get the count of nodes in the current context node list.
	 *
	 * @return length of list
	 */
	virtual int
	getContextNodeListLength() const = 0;

	/*
	 * Get the position of the node in the current context node list.
	 *
	 * @return position in list
	 */
	virtual int
	getContextNodeListPosition(const XalanNode&		contextNode) const = 0;

	/**
	 * Determine if an external element is available.
	 *
	 * @param theNamespace  namespace for the element
	 * @param extensionName name of extension element
	 * @return whether the given element is available or not
	 */
	virtual bool
	elementAvailable(
			const XalanDOMString&	theNamespace, 
			const XalanDOMString&	extensionName) const = 0;

	/**
	 * Determine if an external function is available.
	 *
	 * @param theNamespace  namespace for the function
	 * @param extensionName name of extension function
	 * @return whether the given function is available or not
	 */
	virtual bool
	functionAvailable(
			const XalanDOMString&	theNamespace, 
			const XalanDOMString&	extensionName) const = 0;

	/**
	 * Handle an extension function.
	 * 
	 * @param theNamespace  namespace of function    
	 * @param extensionName extension function name
	 * @param argVec        vector of arguments to function
	 * @return pointer to XObject result
	 */
	virtual XObject*
	extFunction(
			const XalanDOMString&			theNamespace,
			const XalanDOMString&			extensionName,
			XalanNode*						context,
			const XObjectArgVectorType&		argVec) = 0;

	/**
	 * Get an XLocator provider keyed by node.  This gets the association
	 * based on the root of the tree that is the node's parent.
	 *
	 * @param node node for locator
	 * @return pointer to locator
	 */
	virtual XLocator*
	getXLocatorFromNode(const XalanNode*	node) const = 0;

	/**
	 * Associate an XLocator provider to a node.  This makes the association
	 * based on the root of the tree that is the node's parent.
	 *
	 * @param node     node for association
	 * @param xlocator locator to associate with node
	 */
	virtual void
	associateXLocatorToNode(
			const XalanNode*	node,
			XLocator*			xlocator) = 0;

	/**
	 * Provides support for XML parsing service.
	 *
	 * @param urlString location of the XML
	 * @param base base location for URI
	 * @return parsed document
	 */
	virtual XalanDocument*
	parseXML(
			const XalanDOMString&	urlString,
			const XalanDOMString&	base) const = 0;

	/**
	 * Create a MutableNodeRefList with the appropriate context.
	 *
	 * @return node list created
	 */
	virtual MutableNodeRefList
	createMutableNodeRefList() const = 0;

	/**
	 * Tells if namespaces should be supported.  For optimization purposes.
	 *
	 * @return true if namespaces supported
	 */
	virtual bool
	getProcessNamespaces() const = 0;

	/**
	 * Given a valid element key, return the corresponding node list.
	 *
	 * @param doc              source document
	 * @param name             name of the key, which must match the 'name'
	 *                         attribute on xsl:key
	 * @param ref              value that must match the value found by the
	 *                         'match' attribute on xsl:key
	 * @param nscontext        context node for namespace resolution
	 * @return if the name was not declared with xsl:key, this will return
	 *         null, if the identifier is not found, it will return an empty
	 *         node set, otherwise it will return a nodeset of nodes.
	 */
	virtual const NodeRefListBase*
	getNodeSetByKey(
			const XalanNode&		doc,
			const XalanDOMString&	name,
			const XalanDOMString&	ref,
			const XalanElement&		nscontext) = 0;

	/**
	 * Given a valid element key, return the corresponding node list.
	 *
	 * @param doc              source document
	 * @param name             name of the key, which must match the 'name'
	 *                         attribute on xsl:key
	 * @param ref              value that must match the value found by the
	 *                         'match' attribute on xsl:key
	 * @return if the name was not declared with xsl:key, this will return
	 *         null, if the identifier is not found, it will return an empty
	 *         node set, otherwise it will return a nodeset of nodes.
	 */
	virtual const NodeRefListBase*
	getNodeSetByKey(
			const XalanNode&		doc,
			const XalanDOMString&	name,
			const XalanDOMString&	ref) = 0;

	/**
	 * Given a valid element key, return the corresponding node list.
	 *
	 * @param doc              source document
	 * @param name             name of the key, which must match the 'name'
	 *                         attribute on xsl:key
	 * @param ref              value that must match the value found by the
	 *                         'match' attribute on xsl:key
	 * @param resolver         resolver for namespace resolution
	 * @return if the name was not declared with xsl:key, this will return
	 *         null, if the identifier is not found, it will return an empty
	 *         node set, otherwise it will return a nodeset of nodes.
	 */
	virtual const NodeRefListBase*
	getNodeSetByKey(
			const XalanNode&		doc,
			const XalanDOMString&	name,
			const XalanDOMString&	ref,
			const PrefixResolver&	resolver) = 0;

	/**
	 * Given a name, locate a variable in the current context, and return 
	 * a pointer to the object.
	 *
	 * @param theName name of variable
	 * @return pointer to an XObject if the variable was found, 0 if it was not
	 */
	virtual XObject*
	getVariable(const QName&	name) const = 0;

	/**
	 * Retrieve the resolver for namespaces.
	 * 
	 * @return object for namespace resolution
	 */
	virtual const PrefixResolver*
	getPrefixResolver() const = 0;

	/**
	 * Change the resolver for namespaces.
	 * 
	 * @param thePrefixResolver new object for namespace resolution
	 */
	virtual void
	setPrefixResolver(const PrefixResolver*		thePrefixResolver) = 0;

	class PrefixResolverSetAndRestore
	{
	public:

		PrefixResolverSetAndRestore(
				XPathExecutionContext&	theExecutionContext,
				const PrefixResolver*	theResolver) :
			m_executionContext(theExecutionContext),
			m_savedResolver(theExecutionContext.getPrefixResolver())
		{
			m_executionContext.setPrefixResolver(theResolver);
		}

		~PrefixResolverSetAndRestore()
		{
			m_executionContext.setPrefixResolver(m_savedResolver);
		}

	private:

		XPathExecutionContext&			m_executionContext;
		const PrefixResolver* const		m_savedResolver;
	};

	/**
	 * Retrieve the URI corresponding to a namespace prefix
	 * 
	 * @param prefix prefix for a namespace
	 * @return URI corresponding to namespace
	 */
	virtual XalanDOMString
	getNamespaceForPrefix(const XalanDOMString&		prefix) const = 0;

	/**
	 * Given a DOM Document, tell what URI was used to parse it. Needed for
	 * relative resolution.
	 *
	 * @param owner source document
	 * @return document URI
	 */
	virtual XalanDOMString
	findURIFromDoc(const XalanDocument*		owner) const = 0;

	/**
	 * The getUnparsedEntityURI function returns the URI of the unparsed
	 * entity with the specified name in the same document as the context
	 * node (see [3.3 Unparsed Entities]). It returns the empty string if
	 * there is no such entity.
	 *
	 * @param theName     name of entity
	 * @param theDocument document containing entity
	 * @return URI for the entity
	 */
	virtual XalanDOMString
	getUnparsedEntityURI(
			const XalanDOMString&	theName,
			const XalanDocument&	theDocument) const = 0;

	/**
	 * Tells, through the combination of the default-space attribute
	 * on xsl:stylesheet, xsl:strip-space, xsl:preserve-space, and the
	 * xml:space attribute, whether or not extra whitespace should be stripped
	 * from the node.  Literal elements from template elements should
	 * <em>not</em> be tested with this function.
	 *
	 * @param textNode text node from the source tree
	 * @return true if the text node should be stripped of extra whitespace
	 */
	virtual bool
	shouldStripSourceNode(const XalanNode&	node) const = 0;

	/**
	 * Tells if FoundIndex should be thrown if index is found. This is an
	 * optimization for match patterns, and is used internally by the XPath
	 * engine.
	 *
	 * @return true to throw FoundIndex
	 */
	virtual bool
	getThrowFoundIndex() const = 0;

	/**
	 * Changes whether FoundIndex should be thrown if index is found. This is an
	 * optimization for match patterns, and is used internally by the XPath
	 * engine.
	 *
	 * @param fThrow true to throw FoundIndex
	 */
	virtual void
	setThrowFoundIndex(bool 	fThrow) = 0;

	/**
	 * Sets the current match pattern.
	 *
	 * @param thePattern new match pattern
	 */
	virtual void
	setCurrentPattern(const XalanDOMString&		thePattern) = 0;

	/**
	 * Retrieve the current match pattern.
	 *
	 * @return current match pattern
	 */
	virtual XalanDOMString
	getCurrentPattern() const = 0;

	virtual XalanDocument*
	getSourceDocument(const XalanDOMString&		theURI) const = 0;

	/**
	 * Associate a document with a given URI.
	 *
	 * @param theURI      document URI
	 * @param theDocument source document
	 */
	virtual void
	setSourceDocument(
			const XalanDOMString&	theURI,
			XalanDocument*			theDocument) = 0;


	/**
	 * Retrieve the DecimalFormatSymbols instance associated with
	 * the name.
	 *
	 * @param name the name for the lookup
	 * @return a pointer to the matching instance, or 0 if none was found
	 */
	virtual const DecimalFormatSymbols*
	getDecimalFormatSymbols(const XalanDOMString&	name) = 0;

	// These interfaces are inherited from ExecutionContext...

	virtual void
	error(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode = 0,
			const XalanNode*		styleNode = 0) const = 0;

	virtual void
	warn(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode = 0,
			const XalanNode*		styleNode = 0) const = 0;

	virtual void
	message(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode = 0,
			const XalanNode*		styleNode = 0) const = 0;
};



#endif	// XPATHEXECUTIONCONTEXT_HEADER_GUARD_1357924680
