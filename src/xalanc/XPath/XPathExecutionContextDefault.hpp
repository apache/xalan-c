/*
 * Copyright 1999-2004 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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
#if !defined(XPATHEXECUTIONCONTEXTDEFAULT_HEADER_GUARD_1357924680)
#define XPATHEXECUTIONCONTEXTDEFAULT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



#include <xalanc/Include/XalanObjectCache.hpp>
#include <xalanc/Include/XalanVector.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



/**
 * @author <a href="mailto:david_n_bertoni@lotus.com">David N. Bertoni</a>
 */
// Base class include file.
#include <xalanc/XPath/XPathExecutionContext.hpp>



#include <xalanc/PlatformSupport/XalanDOMStringCache.hpp>



#include <xalanc/XPath/MutableNodeRefList.hpp>
#include <xalanc/XPath/XalanQNameByValue.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class DOMSupport;
class XPathEnvSupport;
class XalanQName;



/**
 * A basic implementation of the class XPathExecutionContext.
 */
class XALAN_XPATH_EXPORT XPathExecutionContextDefault : public XPathExecutionContext
{
public:

	typedef XalanVector<XalanNode*>				CurrentNodeStackType;
	typedef XalanVector<const NodeRefListBase*>	ContextNodeListStackType;

	/**
	 * Construct an XPathExecutionContextDefault object
	 *
	 * @param theXPathEnvSupport XPathEnvSupport class instance
	 * @param theDOMSupport		 DOMSupport class instance
	 * @param theXobjectFactory  factory class instance for XObjects
	 * @param theCurrentNode     current node in the source tree
	 * @param theContextNodeList node list for current context
	 * @param thePrefixResolver  pointer to prefix resolver to use
	 */	
	XPathExecutionContextDefault(
			XPathEnvSupport&		theXPathEnvSupport,
			DOMSupport&				theDOMSupport,
			XObjectFactory&			theXObjectFactory,
			XalanNode*				theCurrentNode = 0,
			const NodeRefListBase*	theContextNodeList = 0,
			const PrefixResolver*	thePrefixResolver = 0);

	/**
	 * Construct an XPathExecutionContextDefault object
	 *
	 * @param theXPathEnvSupport XPathEnvSupport class instance
	 * @param theXObjectFactory  factory class instance for XObjects
	 * @param theCurrentNode     current node in the source tree
	 * @param theContextNodeList node list for current context
	 * @param thePrefixResolver  pointer to prefix resolver to use
	 */
	explicit
	XPathExecutionContextDefault(
            MemoryManagerType&      theManager,
			XalanNode*				theCurrentNode = 0,
			const NodeRefListBase*	theContextNodeList = 0,
			const PrefixResolver*	thePrefixResolver = 0);

    static XPathExecutionContextDefault*
    create(
            MemoryManagerType&      theManager,
			XalanNode*				theCurrentNode = 0,
			const NodeRefListBase*	theContextNodeList = 0,
			const PrefixResolver*	thePrefixResolver = 0);

	virtual
	~XPathExecutionContextDefault();


	/**
	 * Get the XPathEnvSupport instance.
	 *
	 * @return a pointer to the instance.
	 */
	XPathEnvSupport*
	getXPathEnvSupport() const
	{
		return m_xpathEnvSupport;
	}

	/**
	 * Set the XPathEnvSupport instance.
	 *
	 * @param theSupport a reference to the instance to use.
	 */
	void
	setXPathEnvSupport(XPathEnvSupport*		theSupport)
	{
		m_xpathEnvSupport = theSupport;
	}

	/**
	 * Set the DOMSupport instance.
	 *
	 * @param theDOMSupport a reference to the instance to use.
	 */
	void
	setDOMSupport(DOMSupport*	theDOMSupport)
	{
		m_domSupport = theDOMSupport;
	}

	/**
	 * Set the XObjectFactory instance.
	 *
	 * @param theFactory a reference to the instance to use.
	 */
	void
	setXObjectFactory(XObjectFactory*	theXObjectFactory)
	{
		m_xobjectFactory = theXObjectFactory;
	}

	/**
	 * Get a reference to the scratch QNameByValue instance.
	 *
	 * @return A reference to a QNameByValue instance.
	 */
	XalanQNameByValue&
	getScratchQName() const
	{
#if defined(XALAN_NO_MUTABLE)
		return ((XPathExecutionContextDefault*)this)->m_scratchQName;
#else
		return m_scratchQName;
#endif
	}

	virtual void doFormatNumber(
			double								number,
			const XalanDOMString&				pattern,
			const XalanDecimalFormatSymbols*	theDFS,
			XalanDOMString&						theResult,
			const XalanNode*					context = 0,
			const LocatorType*					locator = 0);

	// These interfaces are inherited from XPathExecutionContext...

	virtual void
	reset();

	virtual XalanNode*
	getCurrentNode() const;

	virtual void
	pushCurrentNode(XalanNode*		theCurrentNode);

	virtual void
	popCurrentNode();

	virtual bool
	isNodeAfter(
			const XalanNode&	node1,
			const XalanNode&	node2) const;

	virtual void
	pushContextNodeList(const NodeRefListBase&	theList);

	virtual void	
	popContextNodeList();

	virtual const NodeRefListBase&
	getContextNodeList() const;

	virtual size_type
	getContextNodeListLength() const;

	virtual size_type
	getContextNodeListPosition(const XalanNode&		contextNode) const;

	virtual bool
	elementAvailable(const XalanQName&	theQName) const;

	virtual bool
	elementAvailable(
			const XalanDOMString&	theName,
			const LocatorType*		locator) const;

	virtual bool
	functionAvailable(const XalanQName&		theQName) const;

	virtual bool
	functionAvailable(
			const XalanDOMString&	theName,
			const LocatorType*		locator) const;

	virtual const XObjectPtr
	extFunction(
			const XalanDOMString&			theNamespace,
			const XalanDOMString&			functionName,
			XalanNode*						context,
			const XObjectArgVectorType&		argVec,
			const LocatorType*				locator);

	virtual XalanDocument*
	parseXML(
            MemoryManagerType&      theManager,
			const XalanDOMString&	urlString,
			const XalanDOMString&	base) const;

	virtual MutableNodeRefList*
	borrowMutableNodeRefList();

	virtual bool
	returnMutableNodeRefList(MutableNodeRefList*	theList);

	virtual MutableNodeRefList*
	createMutableNodeRefList(MemoryManagerType& theManager) const;

	virtual XalanDOMString&
	getCachedString();

	virtual bool
	releaseCachedString(XalanDOMString&		theString);

	virtual void
	getNodeSetByKey(
			XalanDocument*			doc,
			const XalanQName&		qname,
			const XalanDOMString&	ref,
			MutableNodeRefList&		nodelist);

	virtual void
	getNodeSetByKey(
			XalanDocument*			doc,
			const XalanDOMString&	name,
			const XalanDOMString&	ref,
			const LocatorType*		locator,
			MutableNodeRefList&		nodelist);

	virtual const XObjectPtr
	getVariable(
			const XalanQName&	name,
			const LocatorType*	locator = 0);

	virtual const PrefixResolver*
	getPrefixResolver() const;

	virtual void
	setPrefixResolver(const PrefixResolver*		thePrefixResolver);

	virtual const XalanDOMString*
	getNamespaceForPrefix(const XalanDOMString&		prefix) const;

	virtual const XalanDOMString&
	findURIFromDoc(const XalanDocument*		owner) const;

	virtual const XalanDOMString&
	getUnparsedEntityURI(
			const XalanDOMString&   theName,
			const XalanDocument&    theDocument) const;

	virtual bool
	shouldStripSourceNode(const XalanText&	node);

	virtual XalanDocument*
	getSourceDocument(const XalanDOMString&		theURI) const;

	virtual void
	setSourceDocument(
			const XalanDOMString&	theURI,
			XalanDocument*			theDocument);

	// These interfaces are inherited from ExecutionContext...

	virtual void formatNumber(
			double								number,
			const XalanDOMString&				pattern,
			XalanDOMString&						theResult,
			const XalanNode*					context = 0,
			const LocatorType*					locator = 0);

	virtual void formatNumber(
			double								number,
			const XalanDOMString&				pattern,
			const XalanDOMString&				dfsName,
			XalanDOMString&						theResult,
			const XalanNode*					context = 0,
			const LocatorType*					locator = 0);


	virtual void
	error(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode = 0,
			const LocatorType* 		locator = 0) const;

	virtual void
	warn(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode = 0,
			const LocatorType* 		locator = 0) const;

	virtual void
	message(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode = 0,
			const LocatorType* 		locator = 0) const;

protected:

	typedef XalanObjectCache<MutableNodeRefList, DefaultCacheCreateFunctorMemMgr<MutableNodeRefList>, DeleteFunctor<MutableNodeRefList>, ClearCacheResetFunctor<MutableNodeRefList> >	NodeListCacheType;

	enum { eNodeListCacheListSize = 50 };

	struct ContextNodeListPositionCache
	{
		ContextNodeListPositionCache() :
			m_node(0),
			m_index(0)
		{
		}

		void
		clear()
		{
			if (m_node != 0)
			{
				m_node = 0;
			}
		}

		const XalanNode*	m_node;

		size_type			m_index;
	};

	XPathEnvSupport*						m_xpathEnvSupport;

	DOMSupport*								m_domSupport;

	CurrentNodeStackType					m_currentNodeStack;

	ContextNodeListStackType				m_contextNodeListStack;

	const PrefixResolver*					m_prefixResolver;

	XalanDOMString							m_currentPattern;

	NodeListCacheType						m_nodeListCache;

	XalanDOMStringCache						m_stringCache;

	mutable ContextNodeListPositionCache	m_cachedPosition;

	mutable XalanQNameByValue				m_scratchQName;

	static const NodeRefList	            s_dummyList;
};



XALAN_CPP_NAMESPACE_END



#endif	// XPATHEXECUTIONCONTEXTDEFAULT_HEADER_GUARD_1357924680
