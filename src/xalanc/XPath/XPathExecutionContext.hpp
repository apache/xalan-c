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
#if !defined(XPATHEXECUTIONCONTEXT_HEADER_GUARD_1357924680)
#define XPATHEXECUTIONCONTEXT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



#include <cassert>



#include <xalanc/Include/XalanVector.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



/**
 * @author <a href="mailto:david_n_bertoni@lotus.com">David N. Bertoni</a>
 */
// Base class header file...
#include <xalanc/PlatformSupport/ExecutionContext.hpp>



#include <xalanc/XPath/MutableNodeRefList.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XalanDecimalFormatSymbols;
class PrefixResolver;
class XalanQName;
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

    typedef XalanVector<XObjectPtr>         XObjectArgVectorType;

    typedef NodeRefListBase::size_type  size_type;

    explicit
    XPathExecutionContext(MemoryManagerType& m_memoryManager, XObjectFactory*   theXObjectFactory = 0);

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
    pushCurrentNode(XalanNode*  theCurrentNode) = 0;

    /**
     * Reset the node currently being executed.
     */
    virtual void
    popCurrentNode() = 0;

    class CurrentNodePushAndPop
    {
    public:

        CurrentNodePushAndPop(
                XPathExecutionContext&  theExecutionContext,
                XalanNode*              theNewNode) :
            m_executionContext(theExecutionContext)
        {
            theExecutionContext.pushCurrentNode(theNewNode);
        }

        ~CurrentNodePushAndPop()
        {
            m_executionContext.popCurrentNode();
        }

    private:

        XPathExecutionContext&  m_executionContext;
    };

    /**
     * Retrieve the factory object for creating XObjects.
     * 
     * @return factory object instance
     */
    XObjectFactory&
    getXObjectFactory() const
    {
        assert(m_xobjectFactory != 0);

        return *m_xobjectFactory;
    }

    /**
     * Determine if a node is after another node, in document order.
     *
     * @param node1 The first node
     * @param node2 The second node
     * @return true if node1 one is after node2, or false if it is not.
     */
    virtual bool
    isNodeAfter(
            const XalanNode&    node1,
            const XalanNode&    node2) const = 0;

    /**
     * Push the node list for current context.
     * 
     * @param theList new node list
     */
    virtual void
    pushContextNodeList(const NodeRefListBase&  theList) = 0;

    /**
     * Pop the node list for current context.
     */
    virtual void    
    popContextNodeList() = 0;

    class ContextNodeListPushAndPop
    {
    public:

        ContextNodeListPushAndPop(
                XPathExecutionContext&      theExecutionContext,
                const NodeRefListBase&      theNodeList) :
            m_executionContext(theExecutionContext)
        {
            m_executionContext.pushContextNodeList(theNodeList);
        }

        ~ContextNodeListPushAndPop()
        {
            m_executionContext.popContextNodeList();
        }

    private:

        XPathExecutionContext&  m_executionContext;
    };

    /**
     * Get the node list for current context.
     * 
     * @return node list
     */
    virtual const NodeRefListBase&
    getContextNodeList() const = 0;

    /*
     * Get the count of nodes in the current context node list.
     *
     * @return length of list
     */
    virtual size_type
    getContextNodeListLength() const = 0;

    /*
     * Get the position of the node in the current context node list.
     * Note that this is 1-based indexing (XPath/XSLT-style), not 0-based.
     * Thus, 0 will be returned if the node was not found.
     *
     * @return position in list
     */
    virtual size_type
    getContextNodeListPosition(const XalanNode&     contextNode) const = 0;

    /**
     * Determine if an external element is available.
     *
     * @param theQName The QName of the element
     *
     * @return whether the given element is available or not
     */
    virtual bool
    elementAvailable(const XalanQName&  theQName) const = 0;

    /**
     * Determine if an external element is available by resolving
     * a string to a QName.
     *
     * @param theName The name of the element
     * @param locator A LocatorType instance for error reporting
     *
     * @return whether the given element is available or not
     */
    virtual bool
    elementAvailable(
            const XalanDOMString&   theName,
            const LocatorType*      locator) const = 0;

    /**
     * Determine if a function is available.
     *
     * @param theQName The QName of the function
     *
     * @return whether the function is available or not
     */
    virtual bool
    functionAvailable(const XalanQName&     theQName) const = 0;

    /**
     * Determine if a function is available.
     *
     * @param theName The name of the function
     * @param locator A LocatorType instance for error reporting
     *
     * @return whether the function is available or not
     */
    virtual bool
    functionAvailable(
            const XalanDOMString&   theName,
            const LocatorType*      locator) const = 0;

    /**
     * Handle an extension function.
     * 
     * @param theNamespace  namespace of function    
     * @param functionName extension function name
     * @param context The context node
     * @param argVec vector of arguments to function
     * @param locator A LocatorType instance for error reporting
     * @return pointer to XObject result
     */
    virtual const XObjectPtr
    extFunction(
            const XalanDOMString&           theNamespace,
            const XalanDOMString&           functionName,
            XalanNode*                      context,
            const XObjectArgVectorType&     argVec,
            const LocatorType*              locator) = 0;

    /**
     * Provides support for XML parsing service.
     *
     * @param urlString location of the XML
     * @param base base location for URI
     * @return parsed document
     */
    virtual XalanDocument*
    parseXML(
            MemoryManagerType&      theManager,
            const XalanDOMString&   urlString,
            const XalanDOMString&   base) const = 0;

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
    returnMutableNodeRefList(MutableNodeRefList*    theList) = 0;

    class BorrowReturnMutableNodeRefList
    {
    public:

        BorrowReturnMutableNodeRefList(XPathExecutionContext&   executionContext) :
            m_xpathExecutionContext(&executionContext),
            m_mutableNodeRefList(executionContext.borrowMutableNodeRefList())
        {
            assert(m_mutableNodeRefList != 0);
        }

        // N.B. Non-const copy constructor semantics (like std::auto_ptr)
        BorrowReturnMutableNodeRefList(const BorrowReturnMutableNodeRefList&    theSource) :
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

            BorrowReturnMutableNodeRefList  theResult(*m_xpathExecutionContext);

            *theResult = *m_mutableNodeRefList;

            return theResult;
        }

        // N.B. Non-const assignment operator semantics.
        BorrowReturnMutableNodeRefList&
        operator=(BorrowReturnMutableNodeRefList&   theRHS)
        {
            release();

            m_xpathExecutionContext = theRHS.m_xpathExecutionContext;

            m_mutableNodeRefList = theRHS.m_mutableNodeRefList;

            theRHS.m_mutableNodeRefList = 0;

            return *this;
        }

    private:

        XPathExecutionContext*  m_xpathExecutionContext;

        MutableNodeRefList*     m_mutableNodeRefList;
    };

    /**
     * Get a cached string for temporary use.
     *
     * @return A reference to the string
     */
    virtual XalanDOMString&
    getCachedString() = 0;

    /**
     * Return a cached string.
     *
     * @param theString The string to release.
     *
     * @return true if the string was released successfully.
     */
    virtual bool
    releaseCachedString(XalanDOMString&     theString) = 0;

    class GetAndReleaseCachedString
    {
    public:

        GetAndReleaseCachedString(XPathExecutionContext&    theExecutionContext) :
            m_executionContext(&theExecutionContext),
            m_string(&theExecutionContext.getCachedString())
        {
        }

        // Note non-const copy semantics...
        GetAndReleaseCachedString(GetAndReleaseCachedString&    theSource) :
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
        XPathExecutionContext*  m_executionContext;

        XalanDOMString*         m_string;
    };

    typedef GetAndReleaseCachedString   GetCachedString;

    /**
     * Create a MutableNodeRefList with the appropriate context.
     *
     * @return pointer to node list created
     */
    virtual MutableNodeRefList*
    createMutableNodeRefList(MemoryManagerType& theManager) const = 0;

    /**
     * Given a valid element key, return the corresponding node list.
     *
     * @param doc              source document
     * @param name             qname of the key, which must match the 'name'
     *                         attribute on xsl:key
     * @param ref              value that must match the value found by the
     *                         'match' attribute on xsl:key
     * @param nodelist         A node list to contain the nodes found
     */
    virtual void
    getNodeSetByKey(
            XalanDocument*          doc,
            const XalanQName&       qname,
            const XalanDOMString&   ref,
            MutableNodeRefList&     nodelist) = 0;

    /**
     * Given a valid element key, return the corresponding node list.
     *
     * @param doc              source document
     * @param name             name of the key, which must match the 'name'
     *                         attribute on xsl:key.  Will be resolved to a
     *                         qname using the provided resolver.
     * @param ref              value that must match the value found by the
     *                         'match' attribute on xsl:key
     * @param locator          The LocatorType to use for error reporting.  Can be 0.
     * @param nodelist         A node list to contain the nodes found
     */
    virtual void
    getNodeSetByKey(
            XalanDocument*          doc,
            const XalanDOMString&   name,
            const XalanDOMString&   ref,
            const LocatorType*      locator,
            MutableNodeRefList&     nodelist) = 0;

    /**
     * Given a name, locate a variable in the current context, and return 
     * a pointer to the object.
     *
     * @param theName name of variable
     * @return An XObjectPtr instance.  If the variable is not found, an exception
     *         is thrown, or the routine returns an instance of XUnknown.
     */
    virtual const XObjectPtr
    getVariable(
            const XalanQName&   name,
            const LocatorType*  locator = 0) = 0;

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
    setPrefixResolver(const PrefixResolver*     thePrefixResolver) = 0;

    class PrefixResolverSetAndRestore
    {
    public:

        PrefixResolverSetAndRestore(
                XPathExecutionContext&  theExecutionContext,
                const PrefixResolver*   theResolver) :
            m_executionContext(theExecutionContext),
            m_savedResolver(theExecutionContext.getPrefixResolver())
        {
            m_executionContext.setPrefixResolver(theResolver);
        }

        PrefixResolverSetAndRestore(
                XPathExecutionContext&  theExecutionContext,
                const PrefixResolver*   theOldResolver,
                const PrefixResolver*   theNewResolver) :
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

        XPathExecutionContext&          m_executionContext;
        const PrefixResolver* const     m_savedResolver;
    };

    /**
     * Retrieve the URI corresponding to a namespace prefix
     * 
     * @param prefix prefix for a namespace
     * @return URI corresponding to namespace
     */
    virtual const XalanDOMString*
    getNamespaceForPrefix(const XalanDOMString&     prefix) const = 0;

    /**
     * Given a DOM Document, tell what URI was used to parse it. Needed for
     * relative resolution.
     *
     * @param owner source document
     * @return document URI
     */
    virtual const XalanDOMString&
    findURIFromDoc(const XalanDocument*     owner) const = 0;

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
            const XalanDOMString&   theName,
            const XalanDocument&    theDocument) const = 0;

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
    shouldStripSourceNode(const XalanText&  node) = 0;

    /**
     * Get the document associated with the given URI.
     *
     * @param theURI      document URI
     * @return a pointer to the document instance, if any.
     */
    virtual XalanDocument*
    getSourceDocument(const XalanDOMString&     theURI) const = 0;

    /**
     * Associate a document with a given URI.
     *
     * @param theURI      document URI
     * @param theDocument source document
     */
    virtual void
    setSourceDocument(
            const XalanDOMString&   theURI,
            XalanDocument*          theDocument) = 0;

    /**
     * Formats a number according to the specified pattern.
     * 
     * @param number the number to be formatted
     * @param pattern the format pattern
     * @param theResult the formatted number
     * @param context the source node
     * @param locator the locator 
     */
    virtual void formatNumber(
            double                              number,
            const XalanDOMString&               pattern,
            XalanDOMString&                     theResult,
            const XalanNode*                    context = 0,
            const LocatorType*                  locator = 0) = 0;

    /**
     * Formats a number according to the specified pattern.
     * 
     * @param number the number to be formatted
     * @param pattern the format pattern
     * @param dfsName the name of decimal format to use 
     * @param theResult the formatted number
     * @param context the source node
     * @param locator the locator 
     * @return a pointer to the functor, 0 if none was found
     */
    virtual void formatNumber(
            double                              number,
            const XalanDOMString&               pattern,
            const XalanDOMString&               dfsName,
            XalanDOMString&                     theResult,
            const XalanNode*                    context = 0,
            const LocatorType*                  locator = 0) = 0;

    // These interfaces are inherited from ExecutionContext...

    virtual void
    error(
            const XalanDOMString&   msg,
            const XalanNode*        sourceNode = 0,
            const LocatorType*      locator = 0) const = 0;

    virtual void
    warn(
            const XalanDOMString&   msg,
            const XalanNode*        sourceNode = 0,
            const LocatorType*      locator = 0) const = 0;

    virtual void
    message(
            const XalanDOMString&   msg,
            const XalanNode*        sourceNode = 0,
            const LocatorType*      locator = 0) const = 0;

protected:

    XObjectFactory*     m_xobjectFactory;
};



XALAN_CPP_NAMESPACE_END



#endif  // XPATHEXECUTIONCONTEXT_HEADER_GUARD_1357924680
