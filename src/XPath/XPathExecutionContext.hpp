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



#include <cassert>
#include <vector>



#include <XalanDOM/XalanDOMString.hpp>



// Base class header file...
#include <PlatformSupport/ExecutionContext.hpp>



#include <XPath/MutableNodeRefList.hpp>
#include <XPath/ResultTreeFragBase.hpp>



class XalanDecimalFormatSymbols;
class PrefixResolver;
class QName;
class XLocator;
class XMLURL;
class XObject;
class XObjectPtr;
class XObjectFactory;
class XalanDocument;
class XalanElement;
class XalanNode;
class XalanText;



//
// An abstract class which provides support for executing XPath functions
// and extension functions.
//

class XALAN_XPATH_EXPORT XPathExecutionContext : public ExecutionContext
{
public:

#if defined(XALAN_NO_NAMESPACES)
	typedef vector<XObjectPtr>			XObjectArgVectorType;
#else
	typedef std::vector<XObjectPtr>		XObjectArgVectorType;
#endif

	explicit
	XPathExecutionContext();

	virtual
	~XPathExecutionContext();

	/**
	 * Reset the instance.  This must be called before another
	 * execution is attempted.
	 */
	virtual void
	reset() = 0;

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
	 * Convenience function for creating a node set with
	 * the supplied node as the only member.
	 *
	 * @param node	The  node queried
	 * @return a pointer to the XObject instance.
	 */
	virtual XObjectPtr
	createNodeSet(XalanNode&	theNode) = 0;

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

		XPathExecutionContext&	m_executionContext;
		const NodeRefListBase&	m_savedNodeList;
	};

	/*
	 * Get the count of nodes in the current context node list.
	 *
	 * @return length of list
	 */
	virtual unsigned int
	getContextNodeListLength() const = 0;

	/*
	 * Get the position of the node in the current context node list.
	 * Note that this is 1-based indexing (XPath/XSLT-style), not 0-based.
	 * Thus, 0 will be returned if the node was not found.
	 *
	 * @return position in list
	 */
	virtual unsigned int
	getContextNodeListPosition(const XalanNode&		contextNode) const = 0;

	/**
	 * Determine if an external element is available.
	 *
	 * @param theNamespace  namespace for the element
	 * @param elementName name of extension element
	 * @return whether the given element is available or not
	 */
	virtual bool
	elementAvailable(
			const XalanDOMString&	theNamespace, 
			const XalanDOMString&	elementName) const = 0;

	/**
	 * Determine if a function is available.  For standard
	 * function availability, theNamespace should be an
	 * empty string.
	 *
	 * @param theNamespace  namespace for the function
	 * @param functionName name of the function
	 * @return whether the function is available or not
	 */
	virtual bool
	functionAvailable(
			const XalanDOMString&	theNamespace, 
			const XalanDOMString&	functionName) const = 0;

	/**
	 * Handle an extension function.
	 * 
	 * @param theNamespace  namespace of function    
	 * @param functionName extension function name
	 * @param argVec        vector of arguments to function
	 * @return pointer to XObject result
	 */
	virtual const XObjectPtr
	extFunction(
			const XalanDOMString&			theNamespace,
			const XalanDOMString&			functionName,
			XalanNode*						context,
			const XObjectArgVectorType&		argVec) = 0;

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
	 * Borrow a cached MutableNodeRefList instance.
	 *
	 * @return A pointer to the instance.
	 */
	virtual MutableNodeRefList*
	borrowMutableNodeRefList() = 0;

	/**
	 * Return a previously borrowed MutableNodeRefList instance.
	 *
	 * @param theList A pointer the to previously borrowed instance.
	 * @return true if the list was borrowed (at therefore, destroyed), false if not.
	 */
	virtual bool
	returnMutableNodeRefList(MutableNodeRefList*	theList) = 0;

	class BorrowReturnMutableNodeRefList
	{
	public:

		BorrowReturnMutableNodeRefList(XPathExecutionContext&	executionContext) :
			m_xpathExecutionContext(&executionContext),
			m_mutableNodeRefList(executionContext.borrowMutableNodeRefList())
		{
			assert(m_mutableNodeRefList != 0);
		}

		// N.B. Non-const copy constructor semantics (like std::auto_ptr)
		BorrowReturnMutableNodeRefList(const BorrowReturnMutableNodeRefList&	theSource) :
			m_xpathExecutionContext(theSource.m_xpathExecutionContext),
			m_mutableNodeRefList(theSource.m_mutableNodeRefList)
		{
			assert(m_mutableNodeRefList != 0);

			((BorrowReturnMutableNodeRefList&)theSource).m_mutableNodeRefList = 0;
		}

		~BorrowReturnMutableNodeRefList()
		{
			release();
		}

		MutableNodeRefList&
		operator*() const
		{
			assert(m_mutableNodeRefList != 0);

			return *m_mutableNodeRefList;
		}

		MutableNodeRefList*
		get() const
		{
			return m_mutableNodeRefList;
		}

		MutableNodeRefList*
		operator->() const
		{
			return get();
		}

		void
		release()
		{
			assert(m_xpathExecutionContext != 0);

			if (m_mutableNodeRefList != 0)
			{
				m_xpathExecutionContext->returnMutableNodeRefList(m_mutableNodeRefList);

				m_mutableNodeRefList = 0;
			}
		}

		BorrowReturnMutableNodeRefList
		clone() const
		{
			assert(m_xpathExecutionContext != 0);

			BorrowReturnMutableNodeRefList	theResult(*m_xpathExecutionContext);

			*theResult = *m_mutableNodeRefList;

			return theResult;
		}

		// N.B. Non-const assignment operator semantics.
		BorrowReturnMutableNodeRefList&
		operator=(BorrowReturnMutableNodeRefList&	theRHS)
		{
			release();

			m_xpathExecutionContext = theRHS.m_xpathExecutionContext;

			m_mutableNodeRefList = theRHS.m_mutableNodeRefList;

			theRHS.m_mutableNodeRefList = 0;

			return *this;
		}

	private:

		XPathExecutionContext*	m_xpathExecutionContext;

		MutableNodeRefList*		m_mutableNodeRefList;
	};

	virtual XalanDOMString&
	getCachedString() = 0;

	virtual bool
	releaseCachedString(XalanDOMString&		theString) = 0;

	class GetAndReleaseCachedString
	{
	public:

		GetAndReleaseCachedString(XPathExecutionContext&	theExecutionContext) :
			m_executionContext(&theExecutionContext),
			m_string(&theExecutionContext.getCachedString())
		{
		}

		// Note non-const copy semantics...
		GetAndReleaseCachedString(GetAndReleaseCachedString&	theSource) :
			m_executionContext(theSource.m_executionContext),
			m_string(theSource.m_string)
		{
			theSource.m_string = 0;
		}

		~GetAndReleaseCachedString()
		{
			if (m_string != 0)
			{
				m_executionContext->releaseCachedString(*m_string);
			}
		}

		XalanDOMString&
		get() const
		{
			assert(m_string != 0);

			return *m_string;
		}

		XPathExecutionContext&
		getExecutionContext() const
		{
			return *m_executionContext;
		}

	private:

		// Not implemented...
		GetAndReleaseCachedString&
		operator=(const GetAndReleaseCachedString&);


		// Data members...
		XPathExecutionContext*	m_executionContext;

		XalanDOMString*			m_string;
	};

	/**
	 * Borrow a cached ResultTreeFragBase instance.
	 *
	 * @return A pointer to the instance.
	 */
	virtual ResultTreeFragBase*
	borrowResultTreeFrag() = 0;

	/**
	 * Return a previously borrowed ResultTreeFragBase instance.
	 *
	 * @param theList A pointer the to previously borrowed instance.
	 * @return true if the list was borrowed (at therefore, destroyed), false if not.
	 */
	virtual bool
	returnResultTreeFrag(ResultTreeFragBase*	theResultTreeFragBase) = 0;


	class BorrowReturnResultTreeFrag
	{
	public:

		BorrowReturnResultTreeFrag(XPathExecutionContext&	executionContext) :
			m_xpathExecutionContext(&executionContext),
			m_resultTreeFrag(executionContext.borrowResultTreeFrag())
		{
			assert(m_resultTreeFrag != 0);
		}

		// N.B. Non-const copy constructor semantics (like std::auto_ptr)
		BorrowReturnResultTreeFrag(const BorrowReturnResultTreeFrag&	theSource) :
			m_xpathExecutionContext(theSource.m_xpathExecutionContext),
			m_resultTreeFrag(theSource.m_resultTreeFrag)
		{
			assert(m_resultTreeFrag != 0);

			((BorrowReturnResultTreeFrag&)theSource).m_resultTreeFrag = 0;
		}

		~BorrowReturnResultTreeFrag()
		{
			if (m_resultTreeFrag != 0)
			{
				if (m_xpathExecutionContext->returnResultTreeFrag(m_resultTreeFrag) == false)
				{
					delete m_resultTreeFrag;
				}
			}
		}

		// N.B. Non-const assignment operator semantics.
		BorrowReturnResultTreeFrag&
		operator=(BorrowReturnResultTreeFrag&	theRHS)
		{
			release();

			m_xpathExecutionContext = theRHS.m_xpathExecutionContext;

			m_resultTreeFrag = theRHS.m_resultTreeFrag;

			theRHS.m_resultTreeFrag = 0;

			return *this;
		}

		ResultTreeFragBase&
		operator*() const
		{
			return *m_resultTreeFrag;
		}

		ResultTreeFragBase*
		get() const
		{
			return m_resultTreeFrag;
		}

		ResultTreeFragBase*
		operator->() const
		{
			return get();
		}

		void
		release()
		{
			assert(m_xpathExecutionContext != 0);

			if (m_resultTreeFrag != 0)
			{
				m_xpathExecutionContext->returnResultTreeFrag(m_resultTreeFrag);

				m_resultTreeFrag = 0;
			}
		}

		BorrowReturnResultTreeFrag
		clone(bool	deep = false) const
		{
			assert(m_xpathExecutionContext != 0);

			BorrowReturnResultTreeFrag	theResult(
						*m_xpathExecutionContext,
						m_resultTreeFrag->clone(deep));

			return theResult;
		}

	private:

		BorrowReturnResultTreeFrag(
				XPathExecutionContext&	executionContext,
				ResultTreeFragBase*		resultTreeFrag) :
			m_xpathExecutionContext(&executionContext),
			m_resultTreeFrag(resultTreeFrag)
		{
			assert(m_resultTreeFrag != 0);
		}

		// Data members...
		XPathExecutionContext*	m_xpathExecutionContext;

		ResultTreeFragBase*		m_resultTreeFrag;
	};

	friend class BorrowReturnResultTreeFrag;

	/**
	 * Create a MutableNodeRefList with the appropriate context.
	 *
	 * @return pointer to node list created
	 */
	virtual MutableNodeRefList*
	createMutableNodeRefList() const = 0;

	/**
	 * Given a valid element key, return the corresponding node list.
	 *
	 * @param doc              source document
	 * @param name             name of the key, which must match the 'name'
	 *                         attribute on xsl:key
	 * @param ref              value that must match the value found by the
	 *                         'match' attribute on xsl:key
	 * @param resolver         resolver for namespace resolution
	 * @param nodelist         A node list to contain the nodes found
	 */
	virtual void
	getNodeSetByKey(
			XalanNode*				doc,
			const XalanDOMString&	name,
			const XalanDOMString&	ref,
			const PrefixResolver&	resolver,
			MutableNodeRefList&		nodelist) = 0;

	/**
	 * Given a name, locate a variable in the current context, and return 
	 * a pointer to the object.
	 *
	 * @param theName name of variable
	 * @return pointer to an XObject if the variable was found, 0 if it was not
	 */
	virtual const XObjectPtr
	getVariable(const QName&	name) = 0;

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

		PrefixResolverSetAndRestore(
				XPathExecutionContext&	theExecutionContext,
				const PrefixResolver*	theOldResolver,
				const PrefixResolver*	theNewResolver) :
			m_executionContext(theExecutionContext),
			m_savedResolver(theOldResolver)
		{
			m_executionContext.setPrefixResolver(theNewResolver);
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
	virtual const XalanDOMString*
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
	 * Get a DOM document, primarily for creating result tree fragments.
	 *
	 * @return DOM document
	 */
	virtual XalanDocument*
	getDOMFactory() const = 0;

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
	virtual const XalanDOMString&
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
	shouldStripSourceNode(const XalanNode&	node) = 0;

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
	 * Retrieve the XalanDecimalFormatSymbols instance associated with
	 * the name.
	 *
	 * @param name the name for the lookup
	 * @return a pointer to the matching instance, or 0 if none was found
	 */
	virtual const XalanDecimalFormatSymbols*
	getDecimalFormatSymbols(const XalanDOMString&	name) = 0;

	// These interfaces are inherited from ExecutionContext...

	virtual void
	error(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode = 0,
			const XalanNode*		styleNode = 0) const = 0;

	virtual void
	error(
			const char*			msg,
			const XalanNode* 	sourceNode = 0,
			const XalanNode* 	styleNode = 0) const = 0;

	virtual void
	warn(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode = 0,
			const XalanNode* 		styleNode = 0) const = 0;

	virtual void
	warn(
			const char*			msg,
			const XalanNode* 	sourceNode = 0,
			const XalanNode* 	styleNode = 0) const = 0;

	virtual void
	message(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode = 0,
			const XalanNode* 		styleNode = 0) const = 0;

	virtual void
	message(
			const char*			msg,
			const XalanNode* 	sourceNode = 0,
			const XalanNode* 	styleNode = 0) const = 0;
};



#endif	// XPATHEXECUTIONCONTEXT_HEADER_GUARD_1357924680
