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
#if !defined(STYLESHEETEXECUTIONCONTEXTDEFAULT_HEADER_GUARD_1357924680)
#define STYLESHEETEXECUTIONCONTEXTDEFAULT_HEADER_GUARD_1357924680



// Base class include file.
#include <xalanc/XSLT/StylesheetExecutionContext.hpp>



#include <ctime>
#include <memory>



#include <xalanc/Include/XalanVector.hpp>
#include <xalanc/Include/XalanMap.hpp>
#include <xalanc/Include/XalanSet.hpp>
#include <xalanc/Include/XalanObjectCache.hpp>
#include <xalanc/Include/XalanObjectStackCache.hpp>


#include <xalanc/PlatformSupport/DOMStringHelper.hpp>


#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
#include <xalanc/PlatformSupport/DOMStringPrintWriter.hpp>
#endif



#include <xalanc/XPath/XPathExecutionContextDefault.hpp>



#include <xalanc/XMLSupport/FormatterToText.hpp>



#if defined(XALAN_AUTO_PTR_REQUIRES_DEFINITION)
#include <xalanc/XalanSourceTree/XalanSourceTreeDocument.hpp>
#endif
#include <xalanc/XalanSourceTree/FormatterToSourceTree.hpp>
#include <xalanc/XalanSourceTree/XalanSourceTreeDocumentFragment.hpp>



#include <xalanc/XSLT/CountersTable.hpp>
#include <xalanc/XSLT/NodeSorter.hpp>
#include <xalanc/XSLT/Stylesheet.hpp>
#include <xalanc/XSLT/VariablesStack.hpp>
#include <xalanc/XSLT/XResultTreeFragAllocator.hpp>
#include <xalanc/XSLT/XalanSourceTreeDocumentAllocator.hpp>
#include <xalanc/XSLT/XalanSourceTreeDocumentFragmentAllocator.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XalanSourceTreeDocument;
class XPathProcessor;
class XSLTEngineImpl;

typedef VariablesStack::ParamsVectorType            ParamsVectorTypeDecl;
XALAN_USES_MEMORY_MANAGER(ParamsVectorTypeDecl)
//
// An class which provides support for executing stylesheets.
//
class XALAN_XSLT_EXPORT StylesheetExecutionContextDefault : public StylesheetExecutionContext
{



public:

#if defined(XALAN_STRICT_ANSI_HEADERS)
    typedef std::clock_t    ClockType;
#else
    typedef clock_t         ClockType;
#endif

    typedef XalanVector<FormatterListener*>             FormatterListenerVectorType;
    typedef XalanVector<PrintWriter*>                   PrintWriterVectorType;
    typedef XalanVector<XalanOutputStream*>             OutputStreamVectorType;

    typedef XalanVector<const ElemTemplateElement*> ElementTemplateElementStackType;
    typedef XALAN_STD_QUALIFIER pair<const XPath*, ClockType>       XPathCacheEntry;
    typedef XalanMap <XalanDOMString, XPathCacheEntry>              XPathCacheMapType;
    typedef XalanVector<const ElemTemplate*>            CurrentTemplateStackType;

    typedef Stylesheet::KeyTablesTableType              KeyTablesTableType;
    typedef ParamsVectorTypeDecl                        ParamsVectorType;

    /**
     * Construct a StylesheetExecutionContextDefault object
     *
     * @param theXPathEnvSupport XPath environment support class instance
     * @param theDOMSupport      DOMSupport class instance
     * @param theXobjectFactory  factory class instance for XObjects
     * @param theCurrentNode     current node in the source tree
     * @param theContextNodeList node list for current context
     * @param thePrefixResolver  pointer to prefix resolver to use
     */ 
    StylesheetExecutionContextDefault(
            MemoryManagerType&      theManager,
            XSLTEngineImpl&         xsltProcessor,
            XPathEnvSupport&        theXPathEnvSupport,
            DOMSupport&             theDOMSupport,
            XObjectFactory&         theXObjectFactory,
            XalanNode*              theCurrentNode = 0,
            const NodeRefListBase*  theContextNodeList = 0,
            const PrefixResolver*   thePrefixResolver = 0);

    /**
     * Construct a StylesheetExecutionContextDefault object
     *
     * @param theXPathEnvSupport XPath environment support class instance
     * @param theDOMSupport      DOMSupport class instance
     * @param theXobjectFactory  factory class instance for XObjects
     * @param theCurrentNode     current node in the source tree
     * @param theContextNodeList node list for current context
     * @param thePrefixResolver  pointer to prefix resolver to use
     */
    explicit
    StylesheetExecutionContextDefault(
            MemoryManagerType&      theManager,
            XalanNode*              theCurrentNode = 0,
            const NodeRefListBase*  theContextNodeList = 0,
            const PrefixResolver*   thePrefixResolver = 0);

    static StylesheetExecutionContextDefault*
    create(
            MemoryManagerType&      theManager,
            XalanNode*              theCurrentNode = 0,
            const NodeRefListBase*  theContextNodeList = 0,
            const PrefixResolver*   thePrefixResolver = 0);

    virtual
    ~StylesheetExecutionContextDefault();


    /**
     * Set the XPathEnvSupport instance.
     *
     * @param theSupport a reference to the instance to use.
     */
    void
    setXPathEnvSupport(XPathEnvSupport*     theSupport)
    {
        m_xpathExecutionContextDefault.setXPathEnvSupport(theSupport);
    }

    /**
     * Set the DOMSupport instance.
     *
     * @param theDOMSupport a reference to the instance to use.
     */
    void
    setDOMSupport(DOMSupport*   theDOMSupport)
    {
        m_xpathExecutionContextDefault.setDOMSupport(theDOMSupport);
    }

    /**
     * Set the XObjectFactory instance.
     *
     * @param theFactory a reference to the instance to use.
     */
    void
    setXObjectFactory(XObjectFactory*   theXObjectFactory)
    {
        m_xpathExecutionContextDefault.setXObjectFactory(theXObjectFactory);

        m_xobjectFactory = theXObjectFactory;
    }


    /**
     * Set the DOMSupport instance.
     *
     * @param theDOMSupport a reference to the instance to use.
     */
    void
    setXSLTProcessor(XSLTEngineImpl*    theProcessor)
    {
        m_xsltProcessor = theProcessor;
    }

    bool
    getUsePerInstanceDocumentFactory() const
    {
        return m_usePerInstanceDocumentFactory;
    }

    void
    setUsePerInstanceDocumentFactory(bool   fValue)
    {
        m_usePerInstanceDocumentFactory = fValue;
    }


    // These interfaces are inherited from StylesheetExecutionContext...

    virtual void
    error(
            const XalanDOMString&       msg,
            const ElemTemplateElement&  styleNode,
            const XalanNode*            sourceNode = 0) const;

    virtual void
    warn(
            const XalanDOMString&       msg,
            const ElemTemplateElement&  styleNode,
            const XalanNode*            sourceNode = 0) const;

    virtual void
    message(
            const XalanDOMString&       msg,
            const ElemTemplateElement&  styleNode,
            const XalanNode*            sourceNode = 0) const;

    virtual bool
    getQuietConflictWarnings() const;

    virtual bool
    getCopyTextNodesOnly() const;

    virtual void
    pushCopyTextNodesOnly(bool copyTextNodesOnly);

    virtual bool
    popCopyTextNodesOnly();

#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
    virtual void
    pushProcessCurrentAttribute(bool processAttribute);

    virtual bool
    popProcessCurrentAttribute();

    virtual void
    pushSkipElementAttributes(bool skipAttributes);

    virtual bool
    getSkipElementAttributes() const;

    virtual bool
    popSkipElementAttributes();

    virtual void
    pushExecuteIf(bool executeIf);
    
    virtual bool
    popExecuteIf();
#endif

    virtual XalanNode*
    getRootDocument() const;

    virtual void
    setRootDocument(XalanNode*  theDocument);

    virtual void
    setStylesheetRoot(const StylesheetRoot*     theStylesheet);

    virtual const XalanQName*
    getCurrentMode() const;

    virtual void
    pushCurrentMode(const XalanQName*   theMode);
    
    virtual void
    popCurrentMode();

    virtual const ElemTemplate*
    getCurrentTemplate() const;

    virtual void
    pushCurrentTemplate(const ElemTemplate* theTemplate);

    virtual void
    popCurrentTemplate();

    virtual bool
    isElementPending() const;

    virtual void
    replacePendingAttribute(
            const XalanDOMChar*     theName,
            const XalanDOMChar*     theNewType,
            const XalanDOMChar*     theNewValue);

    virtual void
    pushOutputContext(FormatterListener*    flistener = 0);

    virtual void
    popOutputContext();

    virtual void
    addResultAttribute(
            const XalanDOMString&   aname,
            const XalanDOMString&   value);

    virtual void
    addResultAttribute(
            const XalanDOMString&   aname,
            const XalanDOMChar*     value);

    virtual void
    copyNamespaceAttributes(const XalanNode&    src);

    virtual const XalanDOMString*
    getResultPrefixForNamespace(const XalanDOMString&   theNamespace) const;

    virtual const XalanDOMString*
    getResultNamespaceForPrefix(const XalanDOMString&   thePrefix) const;

    virtual bool
    isPendingResultPrefix(const XalanDOMString& thePrefix);

    virtual void
    getUniqueNamespaceValue(XalanDOMString&     theValue) const;

    virtual FormatterListener*
    getFormatterListener() const;

    virtual void
    setFormatterListener(FormatterListener*     flistener);

    virtual int
    getIndent() const;

    virtual void
    setIndent(int   indentAmount);

    virtual const XPath*
    createMatchPattern(
            const XalanDOMString&   str,
            const PrefixResolver&   resolver);

    virtual void
    returnXPath(const XPath*    xpath);

    virtual void
    pushTopLevelVariables(const ParamVectorType&    topLevelParams);


    virtual const XObjectPtr
    createVariable(
            const XPath&                xpath,
            XalanNode*                  contextNode,
            const PrefixResolver&       resolver);

#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
    virtual const XObjectPtr
    createVariable(
            const ElemTemplateElement&  templateChild,
            XalanNode*                  sourceNode);
#endif

    virtual void
    pushVariable(
            const XalanQName&           name,
            const ElemTemplateElement*  element,
            const XalanDOMString&       str,
            XalanNode*                  contextNode,
            const PrefixResolver&       resolver);

    virtual void
    pushVariable(
            const XalanQName&           name,
            const XObjectPtr            val,
            const ElemTemplateElement*  element);

    virtual void
    pushVariable(
            const XalanQName&           name,
            const ElemVariable*         var,
            const ElemTemplateElement*  element);

    virtual void
    pushVariable(
            const XalanQName&           name,
            const ElemTemplateElement*  element,
            const XPath&                xpath,
            XalanNode*                  contextNode,
            const PrefixResolver&       resolver);

#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
    virtual void
    pushVariable(
            const XalanQName&           name,
            const ElemTemplateElement*  element,
            const ElemTemplateElement&  templateChild,
            XalanNode*                  sourceNode);
#endif


    virtual void
    pushContextMarker();

    virtual void
    popContextMarker();

    virtual void
    resolveTopLevelParams();

    virtual void
    clearTopLevelParams();

#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
    virtual void beginParams();

    virtual void endParams();

    virtual void pushParam(const XalanQName& qName,const XObjectPtr& theValue);
#else
    virtual void
    pushParams(const ElemTemplateElement&   xslCallTemplateElement);
#endif

    virtual const XObjectPtr
    getParamVariable(const XalanQName&  theName);

    virtual void
    pushElementFrame(const ElemTemplateElement*     elem);

    virtual void
    popElementFrame();

    virtual int
    getGlobalStackFrameIndex() const;

    virtual int
    getCurrentStackFrameIndex() const;

    virtual void
    pushCurrentStackFrameIndex(int currentStackFrameIndex = -1);

    virtual void
    popCurrentStackFrameIndex();

    virtual void
    startDocument();

    virtual void
    endDocument();

    virtual void
    startElement(const XalanDOMChar*    name);

    virtual void
    endElement(const XalanDOMChar*  name);

    virtual void
    characters(
            const XalanDOMChar*         ch,
            XalanDOMString::size_type   start,
            XalanDOMString::size_type   length);

    virtual void
    charactersRaw(
            const XalanDOMChar*         ch,
            XalanDOMString::size_type   start,
            XalanDOMString::size_type   length);

    virtual void
    comment(const XalanDOMChar*     data);

    virtual void
    processingInstruction(
            const XalanDOMChar*     target,
            const XalanDOMChar*     data);

    virtual void
    flushPending();

    virtual void
    cloneToResultTree(
            const XalanNode&    node,
            const LocatorType*  locator);

    virtual void
    cloneToResultTree(
            const XalanNode&        node,
            XalanNode::NodeType     nodeType,
            bool                    overrideStrip,
            bool                    shouldCloneAttributes,
            const LocatorType*      locator);

#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
    virtual void
    beginCreateXResultTreeFrag(
            XalanNode*                  sourceNode);

    
    virtual const XObjectPtr
    endCreateXResultTreeFrag();

    virtual void
    beginFormatToText(
            XalanDOMString&             theResult);

    virtual void
    endFormatToText();
#endif


#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
    virtual const XObjectPtr
    createXResultTreeFrag(
            const ElemTemplateElement&  templateChild,
            XalanNode*                  sourceNode);
#endif

    virtual void
    outputToResultTree(
            const XObject&      xobj,
            const LocatorType*  locator);

    virtual void
    outputResultTreeFragment(
            const XObject&      theTree,
            const LocatorType*  locator);

    virtual const XalanDOMString&
    getXSLNameSpaceURL() const;

    virtual const XalanDOMString&
    getXalanXSLNameSpaceURL() const;

    virtual bool
    findOnElementRecursionStack(const ElemTemplateElement*  theElement) const;

    virtual void
    pushOnElementRecursionStack(const ElemTemplateElement*  theElement);

    virtual const ElemTemplateElement*
    popElementRecursionStack();

    virtual bool
    returnXResultTreeFrag(XResultTreeFrag*  theXResultTreeFrag);

    virtual eEscapeURLs
    getEscapeURLs() const;

    virtual void
    setEscapeURLs(eEscapeURLs   value);

    virtual eOmitMETATag
    getOmitMETATag() const;

    void
    setOmitMETATag(eOmitMETATag     value);

    virtual FormatterListener*
    createFormatterToXML(
            Writer&                 writer,
            const XalanDOMString&   version = XalanDOMString(XalanMemMgrs::getDummyMemMgr()),
            bool                    doIndent = false,
            int                     indent = eDefaultXMLIndentAmount,
            const XalanDOMString&   encoding = XalanDOMString(XalanMemMgrs::getDummyMemMgr()),
            const XalanDOMString&   mediaType = XalanDOMString(XalanMemMgrs::getDummyMemMgr()),
            const XalanDOMString&   doctypeSystem = XalanDOMString(XalanMemMgrs::getDummyMemMgr()),
            const XalanDOMString&   doctypePublic = XalanDOMString(XalanMemMgrs::getDummyMemMgr()),
            bool                    xmlDecl = true,
            const XalanDOMString&   standalone = XalanDOMString(XalanMemMgrs::getDummyMemMgr()));

    virtual FormatterListener*
    createFormatterToHTML(
            Writer&                 writer,
            const XalanDOMString&   encoding = XalanDOMString(XalanMemMgrs::getDummyMemMgr()),
            const XalanDOMString&   mediaType = XalanDOMString(XalanMemMgrs::getDummyMemMgr()),
            const XalanDOMString&   doctypeSystem = XalanDOMString(XalanMemMgrs::getDummyMemMgr()),
            const XalanDOMString&   doctypePublic = XalanDOMString(XalanMemMgrs::getDummyMemMgr()),
            bool                    doIndent = true,
            int                     indent = eDefaultHTMLIndentAmount,
            bool                    escapeURLs = true,
            bool                    omitMetaTag = false);

    virtual FormatterListener*
    createFormatterToText(
            Writer&                 writer,
            const XalanDOMString&   encoding);

#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
    virtual NodeSorter*
    getNodeSorter();
#else
    virtual NodeSorter*
    borrowNodeSorter();

    virtual bool
    returnNodeSorter(NodeSorter*    theSorter);
#endif

    virtual XalanNumberFormatAutoPtr
    createXalanNumberFormat();

    // A basic class to create XalanNumberFormat instances...
    class XALAN_XSLT_EXPORT XalanNumberFormatFactory
    {
    public:

        explicit
        XalanNumberFormatFactory();

        virtual
        ~XalanNumberFormatFactory();

        virtual XalanNumberFormat*
        create(MemoryManagerType& theManager);
    };

    static XalanNumberFormatFactory&
    getDefaultXalanNumberFormatFactory()
    {
        return s_defaultXalanNumberFormatFactory;
    }

    /**
     * Static function to install a new XalanNumberFormatFactory.
     * The caller owns the XalanNumberFormatFactory instance.
     *
     * @param a pointer to the new factory instance to use.
     * @return a pointer to the old factory instance.
     */
    static XalanNumberFormatFactory*
    installXalanNumberFormatFactory(XalanNumberFormatFactory*   theFactory);


    virtual tl_size_type
    getTraceListeners() const;

    virtual void
    fireGenerateEvent(const GenerateEvent&  ge);

    virtual void
    fireTraceEvent(const TracerEvent&   te);

    virtual void
    fireSelectEvent(const SelectionEvent&   se);

    virtual bool
    getTraceSelects() const;

    virtual void
    traceSelect(
            const ElemTemplateElement&  theStylesheetElement,
            const NodeRefListBase&      nl,
            const XPath*                xpath);

    virtual int
    collationCompare(
            const XalanDOMString&               theLHS,
            const XalanDOMString&               theRHS,
            XalanCollationServices::eCaseOrder  theCaseOrder = XalanCollationServices::eDefault);

    virtual int
    collationCompare(
            const XalanDOMString&               theLHS,
            const XalanDOMString&               theRHS,
            const XalanDOMString&               theLocale,
            XalanCollationServices::eCaseOrder  theCaseOrder = XalanCollationServices::eDefault);

    virtual int
    collationCompare(
            const XalanDOMChar*                 theLHS,
            const XalanDOMChar*                 theRHS,
            XalanCollationServices::eCaseOrder  theCaseOrder = XalanCollationServices::eDefault);

    virtual int
    collationCompare(
            const XalanDOMChar*                 theLHS,
            const XalanDOMChar*                 theRHS,
            const XalanDOMChar*                 theLocale,
            XalanCollationServices::eCaseOrder  theCaseOrder = XalanCollationServices::eDefault);

    typedef XalanCollationServices::CollationCompareFunctor     CollationCompareFunctor;

    class XALAN_XSLT_EXPORT DefaultCollationCompareFunctor : public CollationCompareFunctor
    {
    public:

        DefaultCollationCompareFunctor();

        virtual
        ~DefaultCollationCompareFunctor();

        virtual int
        operator()(
            const XalanDOMChar*                 theLHS,
            const XalanDOMChar*                 theRHS,
            XalanCollationServices::eCaseOrder  theCaseOrder = XalanCollationServices::eDefault) const;

        virtual int
        operator()(
            const XalanDOMChar*                 theLHS,
            const XalanDOMChar*                 theRHS,
            const XalanDOMChar*                 theLocale,
            XalanCollationServices::eCaseOrder  theCaseOrder = XalanCollationServices::eDefault) const;
    };


    const CollationCompareFunctor*
    installCollationCompareFunctor(CollationCompareFunctor*     theFunctor);

    CollationCompareFunctor*
    uninstallCollationCompareFunctor();


    class XALAN_XSLT_EXPORT FormatNumberFunctor
    {
    public:

        FormatNumberFunctor() {};

        virtual
        ~FormatNumberFunctor() {};

        virtual void
        operator() (
            XPathExecutionContext&              executionContext,
            double                              theNumber,
            const XalanDOMString&               thePattern,
            const XalanDecimalFormatSymbols*    theDFS,
            XalanDOMString&                     theResult,
            const XalanNode*                    context = 0,
            const LocatorType*                  locator = 0) const = 0;
    };

    virtual void
    formatNumber(
            double                  number,
            const XalanDOMString&   pattern,
            XalanDOMString&         theResult,
            const XalanNode*        context = 0,
            const LocatorType*      locator = 0);

    virtual void
    formatNumber(
            double                  number,
            const XalanDOMString&   pattern,
            const XalanDOMString&   dfsName,
            XalanDOMString&         theResult,
            const XalanNode*        context = 0,
            const LocatorType*      locator = 0);


    const FormatNumberFunctor* 
    installFormatNumberFunctor(FormatNumberFunctor*     formatNumberFunctor);
    
    FormatNumberFunctor*
    uninstallFormatNumberFunctor();

    virtual PrintWriter*
    createPrintWriter(XalanOutputStream*        theTextOutputStream);

    virtual PrintWriter*
    createPrintWriter(
            const XalanDOMString&       theFileName,
            const XalanDOMString&       theEncoding);

    virtual PrintWriter*
    createPrintWriter(StreamType&   theStream);

    virtual PrintWriter*
    createPrintWriter(FILE*     theStream);

    virtual CountersTable&
    getCountersTable();

    virtual void
    characters(const XalanNode&     node);

    virtual void
    characters(const XObjectPtr&    xobject);

    virtual void
    charactersRaw(const XalanNode&  node);

    virtual void
    charactersRaw(const XObjectPtr&     xobject);


    // These interfaces are inherited from XPathExecutionContext...

    virtual void
    reset();

    virtual XalanNode*
    getCurrentNode() const;

    virtual void
    pushCurrentNode(XalanNode*  theCurrentNode);

    virtual void
    popCurrentNode();

    virtual bool
    isNodeAfter(
            const XalanNode&    node1,
            const XalanNode&    node2) const;

    virtual void
    pushContextNodeList(const NodeRefListBase&  theList);

    virtual void    
    popContextNodeList();

    virtual const NodeRefListBase&
    getContextNodeList() const;

    virtual size_type
    getContextNodeListLength() const;

    virtual size_type
    getContextNodeListPosition(const XalanNode&     contextNode) const;

    virtual bool
    elementAvailable(const XalanQName&  theQName) const;

    virtual bool
    elementAvailable(
            const XalanDOMString&   theName,
            const LocatorType*      locator) const;

    virtual bool
    functionAvailable(const XalanQName&     theQName) const;

    virtual bool
    functionAvailable(
            const XalanDOMString&   theName,
            const LocatorType*      locator) const;

    virtual const XObjectPtr
    extFunction(
            const XalanDOMString&           theNamespace,
            const XalanDOMString&           functionName,
            XalanNode*                      context,
            const XObjectArgVectorType&     argVec,
            const LocatorType*              locator);

    virtual XalanDocument*
    parseXML(
            MemoryManagerType&      theManager,
            const XalanDOMString&   urlString,
            const XalanDOMString&   base) const;

    virtual MutableNodeRefList*
    borrowMutableNodeRefList();

    virtual bool
    returnMutableNodeRefList(MutableNodeRefList*    theList);

    virtual MutableNodeRefList*
    createMutableNodeRefList(MemoryManagerType& theManager) const;

#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
    virtual void
    createUseAttributeSetIndexesOnStack();

    virtual UseAttributeSetIndexes&
    getUseAttributeSetIndexes();

    virtual void
    popUseAttributeSetIndexesFromStack();
    
    virtual void
    pushInvoker(const ElemTemplateElement * invoker);

    virtual void
    popInvoker();

    virtual const ElemTemplateElement*
    getInvoker() const;

    virtual MutableNodeRefList& 
    createAndPushMutableNodeRefList();

    virtual void 
    releaseAndPopMutableNodeRefList();

    virtual void
    pushXObjectPtr(const XObjectPtr& xobjectPtr);

    virtual void 
    popXObjectPtr();

    virtual void
    createAndPushNodesToTransformList(const NodeRefListBase* nodeList);

    virtual XalanNode* 
    getNextNodeToTransform();

    virtual void 
    popNodesToTransformList();

    virtual XalanDOMString&
    getAndPushCachedString();

    virtual XalanDOMString&
    getLastCachedString();

    virtual XalanDOMString&
    getAndPopCachedString();
#endif

    virtual XalanDOMString&
    getCachedString();

    virtual bool
    releaseCachedString(XalanDOMString&     theString);


    virtual void
    getNodeSetByKey(
            XalanDocument*          doc,
            const XalanQName&       qname,
            const XalanDOMString&   ref,
            MutableNodeRefList&     nodelist);

    virtual void
    getNodeSetByKey(            
            XalanDocument*          doc,
            const XalanDOMString&   name,
            const XalanDOMString&   ref,
            const LocatorType*      locator,
            MutableNodeRefList&     nodelist);

    virtual const XObjectPtr
    getVariable(
            const XalanQName&   name,
            const LocatorType*  locator = 0);

    virtual const PrefixResolver*
    getPrefixResolver() const;

    virtual void
    setPrefixResolver(const PrefixResolver*     thePrefixResolver);

    virtual const XalanDOMString*
    getNamespaceForPrefix(const XalanDOMString&     prefix) const;

    virtual const XalanDOMString&
    findURIFromDoc(const XalanDocument*     owner) const;

    virtual const XalanDOMString&
    getUnparsedEntityURI(
            const XalanDOMString&   theName,
            const XalanDocument&    theDocument) const;

    virtual bool
    shouldStripSourceNode(const XalanText&  node);

    virtual XalanDocument*
    getSourceDocument(const XalanDOMString&     theURI) const;

    virtual void
    setSourceDocument(
            const XalanDOMString&   theURI,
            XalanDocument*          theDocument);

    // These interfaces are inherited from ExecutionContext...

    virtual void
    error(
            const XalanDOMString&   msg,
            const XalanNode*        sourceNode,
            const LocatorType*      locator) const;

    virtual void
    warn(
            const XalanDOMString&   msg,
            const XalanNode*        sourceNode,
            const LocatorType*      locator) const;

    virtual void
    message(
            const XalanDOMString&   msg,
            const XalanNode*    sourceNode,
            const LocatorType*  locator) const;


    class XPathCacheReturnFunctor
    {
    public:

        XPathCacheReturnFunctor(XSLTEngineImpl&     xsltProcessor) :
            m_xsltProcessor(xsltProcessor)
        {
        }

        void
        operator()(const XPathCacheMapType::value_type&     theCacheEntry);

    private:

        XSLTEngineImpl&     m_xsltProcessor;
    };

    /**
     * Get a XalanSourceTreeDocument, primarily for creating result 
     * tree fragments.
     */
    XalanSourceTreeDocument*
    getSourceTreeFactory(MemoryManagerType& theManager) const;

#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
protected:

    virtual FormatterToText*
    borrowFormatterToText();

    virtual bool
    returnFormatterToText(FormatterToText*  theFormatter);
#endif

private:

    const XalanDecimalFormatSymbols*
    getDecimalFormatSymbols(const XalanQName&   qname);

#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
    /**
     * Given a context, create the params for a template
     * call.
     *
     * @param xslCallTemplateElement "call-template" element
     * @param params The params
     */
    void
    getParams(
            const ElemTemplateElement&  xslCallTemplateElement,
            ParamsVectorType&           params);
#endif

    /**
     * Determine if the XPath is one that we have cached.
     *
     * @param theXPath the XPath instance to check
     * @return true if the instance has been cached, false if not.
     */
    bool
    isCached(const XPath*   theXPath);

    /**
     * Clear out the cache of XPath instances.
     */
    void
    clearXPathCache();

    /**
     * Add an XPath instance to the cache, clearing out an old entry
     * if the cache is full.
     *
     * @param pattern the key for looking up the XPath instance in the cache.
     * @param theXPath the XPath instance to cache
     */
    void
    addToXPathCache(
            const XalanDOMString&   pattern,
            const XPath*            theXPath);


    /**
     * Clean up anything that was created for use only during the transformation.
     */
    void
    cleanUpTransients();

    XPathExecutionContextDefault    m_xpathExecutionContextDefault;

    XSLTEngineImpl*                 m_xsltProcessor;

    XalanNode*                      m_rootDocument;

    enum { eXPathCacheMax = 50,
           eDefaultParamsVectorSize = 10,
           eXResultTreeFragAllocatorBlockSize = 10,
           eDocumentAllocatorBlockSize = 10,
           eDocumentFragmentAllocatorBlockSize = 10,
           eDefaultAttributeAllocatorBlockSize = 10,
           eDefaultAttributeNSAllocatorBlockSize = 10,
           eDefaultCommentAllocatorBlockSize = 10,
           eDefaultElementAllocatorBlockSize = 10,
           eDefaultElementNSAllocatorBlockSize = 10,
           eDefaultPIAllocatorBlockSize = 10,
           eDefaultTextAllocatorBlockSize = 20,
           eDefaultTextIWSAllocatorBlockSize = 20 };

    ElementTemplateElementStackType     m_elementRecursionStack;

    const StylesheetRoot*               m_stylesheetRoot;

    FormatterListenerVectorType         m_formatterListeners;

    PrintWriterVectorType               m_printWriters;

    OutputStreamVectorType              m_outputStreams;

    CollationCompareFunctor*            m_collationCompareFunctor;

    FormatNumberFunctor *               m_formatNumberFunctor;

    /**
     * Holds all information about variables during execution.
     */
    VariablesStack                      m_variablesStack;

    ParamsVectorType                    m_paramsVector;

    XPathCacheMapType                   m_matchPatternCache;

    KeyTablesTableType                  m_keyTables;

    CountersTable                       m_countersTable;

    /**
     * The factory that will be used to create result tree fragments based on our
	 * internal source tree.
     */
    mutable XalanMemMgrAutoPtr<XalanSourceTreeDocument, true>   m_sourceTreeResultTreeFactory;

    // Holds the current mode.
    const XalanQName*                   m_mode;

    CurrentTemplateStackType            m_currentTemplateStack;

    int                                 m_indentAmount;

    XResultTreeFragAllocator            m_xresultTreeFragAllocator;

    XalanSourceTreeDocumentFragmentAllocator    m_documentFragmentAllocator;

    XalanSourceTreeDocumentAllocator    m_documentAllocator;

    typedef XalanVector<bool>       BooleanStackType;
    typedef XalanVector<const XalanQName*>  ModeStackType;
    typedef XalanVector<int>            IntStackType;

    BooleanStackType                    m_copyTextNodesOnlyStack;
    ModeStackType                       m_modeStack;
    IntStackType                        m_currentIndexStack;

#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
    typedef XalanMemoryManagerObjectCacheDefault<FormatterToText>            FormatterToTextCacheType;
    typedef XalanMemoryManagerObjectCacheDefault<FormatterToSourceTree>     FormatterToSourceTreeCacheType;
    typedef XalanMemoryManagerObjectCacheDefault<NodeSorter>                NodeSorterCacheType;
    
    FormatterToTextCacheType            m_formatterToTextCache;

    FormatterToSourceTreeCacheType      m_formatterToSourceTreeCache;

    NodeSorterCacheType                 m_nodeSorterCache;
#else

    class FormatterToTextDOMString : public FormatterToText
    {
    public:

        FormatterToTextDOMString(MemoryManagerType& theManager);

        virtual
        ~FormatterToTextDOMString();

        void
        setDOMString(XalanDOMString&    theString)
        {
            m_printWriter.setString(theString);
        }

    private:

        // These are not defined...
        FormatterToTextDOMString(const FormatterToTextDOMString&);

        FormatterToTextDOMString&
        operator=(const FormatterToTextDOMString&);

        bool
        operator==(const FormatterToTextDOMString&) const;


        // Data members...
        DOMStringPrintWriter    m_printWriter;

        static XalanDOMString   s_dummyString;
    };

    typedef XalanVector<XObjectPtr>                             XObjectPtrStackType;
    typedef XalanVector<ParamsVectorType>                       ParamsVectorStackType;
    typedef XalanVector<UseAttributeSetIndexes>                 UseAttributeSetIndexesStackType;
    typedef XalanObjectStackCache<MutableNodeRefList,DefaultCacheCreateFunctorMemMgr<MutableNodeRefList> >          
                                                                MutableNodeRefListStackType;

    typedef XalanObjectStackCache<XalanDOMString,DefaultCacheCreateFunctorMemMgr<XalanDOMString> >              
                                                                StringStackType;

    typedef XalanObjectStackCache<FormatterToTextDOMString,DefaultCacheCreateFunctorMemMgr<FormatterToTextDOMString> >
                                                                FormatterToTextStackType;
    typedef XalanObjectStackCache<FormatterToSourceTree,DefaultCacheCreateFunctorMemMgr<FormatterToSourceTree> >
                                                                FormatterToSourceTreeStackType;

    /*
     * class to maintain the list of nodes to be transformed by an element
     */
    class NodesToTransform
    {
    public:
        NodesToTransform(const NodeRefListBase* nodeList) : 
            m_nodeList(nodeList), m_index(0)
        {
            assert(m_nodeList != 0);
        }

        const NodeRefListBase* operator() () 
        {
            return m_nodeList;
        }

        NodeRefListBase::size_type& index()
        {
            return m_index;
        }

        XalanNode* next()
        {
            if (m_index < m_nodeList->getLength())
            {
                return m_nodeList->item(m_index++);
            }
            return 0;
        }

    private:
        const NodeRefListBase*  m_nodeList;
        NodeRefListBase::size_type  m_index;
    };

    typedef XalanVector<NodesToTransform>           NodesToTransformStackType;

    XObjectPtrStackType                 m_xobjectPtrStack;
    MutableNodeRefListStackType         m_mutableNodeRefListStack;
    NodesToTransformStackType           m_nodesToTransformStack;
    BooleanStackType                    m_processCurrentAttributeStack;
    BooleanStackType                    m_executeIfStack;
    StringStackType                     m_stringStack;
    FormatterToTextStackType            m_formatterToTextStack;
    BooleanStackType                    m_skipElementAttributesStack;
    FormatterToSourceTreeStackType      m_formatterToSourceTreeStack;
    ParamsVectorStackType               m_paramsVectorStack;
    ElementTemplateElementStackType     m_elementInvokerStack;
    UseAttributeSetIndexesStackType     m_useAttributeSetIndexesStack;

    NodeSorter                          m_nodeSorter;
#endif

    // If true, we will use a separate document factory for
    // result tree fragments.
    bool                                m_usePerInstanceDocumentFactory;

    // Determines whether or not to override the property in the stylesheet.
    eEscapeURLs                         m_escapeURLs;

    // Determines whether or not to override the property in the stylesheet.
    eOmitMETATag                        m_omitMETATag;

    bool                                m_hasStripOrPreserveSpace;

    static XalanNumberFormatFactory     s_defaultXalanNumberFormatFactory;

    static XalanNumberFormatFactory*    s_xalanNumberFormatFactory;

    static const DefaultCollationCompareFunctor     s_defaultCollationFunctor;

};



XALAN_CPP_NAMESPACE_END



#endif  // STYLESHEETEXECUTIONCONTEXTDEFAULT_HEADER_GUARD_1357924680
