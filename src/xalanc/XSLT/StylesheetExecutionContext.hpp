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
#if !defined(STYLESHEETEXECUTIONCONTEXT_HEADER_GUARD_1357924680)
#define STYLESHEETEXECUTIONCONTEXT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XSLT/XSLTDefinitions.hpp>



#include <cstddef>
#include <cstdio>
#include <memory>



// Base class header file...
#include <xalanc/XPath/XPathExecutionContext.hpp>




#include <xalanc/XalanDOM/XalanDOMString.hpp>



// Base class header file...
#include <xalanc/PlatformSupport/ExecutionContext.hpp>



#if defined(XALAN_AUTO_PTR_REQUIRES_DEFINITION)
#include <xalanc/PlatformSupport/XalanNumberFormat.hpp>
#endif



#include <xalanc/Include/XalanMemMgrAutoPtr.hpp>




#include <xalanc/PlatformSupport/AttributeListImpl.hpp>
#include <xalanc/PlatformSupport/XalanCollationServices.hpp>



#include <xalanc/XSLT/TopLevelArg.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class CountersTable;
class ElemTemplate;
class ElemTemplateElement;
class ElemVariable;
class FormatterListener;
class FormatterToText;
class GenerateEvent;
class PrefixResolver;
class NodeRefListBase;
class NodeSorter;
class PrintWriter;
class XalanQName;
class SelectionEvent;
class Stylesheet;
class StylesheetRoot;
class XalanOutputStream;
class TracerEvent;
class Writer;
class XalanDocument;
class XalanDocumentFragment;
class XalanElement;
class XalanNode;
class XalanNumberFormat;
class XPath;
class XObject;
class XObjectPtr;
class XResultTreeFrag;



//
// An abstract class which provides support for executing stylesheets.
//
class XALAN_XSLT_EXPORT StylesheetExecutionContext : public XPathExecutionContext
{
public:

#if defined(XALAN_STRICT_ANSI_HEADERS)
    typedef std::size_t     tl_size_type;
#else
    typedef size_t          tl_size_type;
#endif

#if defined(XALAN_NO_STD_NAMESPACE)
    typedef ostream         StreamType;
#else
    typedef std::ostream    StreamType;
#endif

    explicit
    StylesheetExecutionContext(MemoryManagerType& m_memoryManager, XObjectFactory*  theXObjectFactory = 0);

    virtual
    ~StylesheetExecutionContext();

    // These interfaces are new...

    /**
     * Report an error and throw an exception.
     * 
     * @param msg The text of the message.
     * @param styleNode The stylesheet node were the error occurred.
     * @param sourceNode The source node where the error occurred.  May be 0.
     */
    virtual void
    error(
            const XalanDOMString&       msg,
            const ElemTemplateElement&  styleNode,
            const XalanNode*            sourceNode = 0) const = 0;

    /**
     * Report a  warning.
     * 
     * @param msg The text of the message.
     * @param styleNode The stylesheet node were the warning occurred.
     * @param sourceNode The source node where the warning occurred.  May be 0.
     */
    virtual void
    warn(
            const XalanDOMString&       msg,
            const ElemTemplateElement&  styleNode,
            const XalanNode*            sourceNode = 0) const = 0;

    /**
     * Report a  message.
     * 
     * @param msg The text of the message.
     * @param styleNode The stylesheet node were the message occurred.
     * @param sourceNode The source node where the message occurred.  May be 0.
     */
    virtual void
    message(
            const XalanDOMString&       msg,
            const ElemTemplateElement&  styleNode,
            const XalanNode*            sourceNode = 0) const = 0;


    /**
     * Determine whether conflicts should be reported.
     * 
     * @return true if conflicts should not be warned
     */
    virtual bool
    getQuietConflictWarnings() const = 0;

    /**
     * If this function returns true, only text nodes can
     * be copied to the result tree.
     * 
     * @return true or false
     */
    virtual bool
    getCopyTextNodesOnly() const = 0;

    /**
     * Set the flag that determines if only text nodes
     * can be copied to the result tree.
     * 
     * @param copyTextNodesOnly The value of the flag
     */
    virtual void
    pushCopyTextNodesOnly(bool copyTextNodesOnly) = 0;

    /**
     * Pop the last flag setting that determines if only text nodes
     * can be copied to the result tree.
     */
    virtual bool
    popCopyTextNodesOnly() = 0;

    /*
     * A class to manage setting and restoring the flag
     * for restricting copying only text nodes to the
     * result tree
     */
    class SetAndRestoreCopyTextNodesOnly
    {
    public:

        SetAndRestoreCopyTextNodesOnly(
            StylesheetExecutionContext&     executionContext,
            bool                            fValue) :
            m_executionContext(executionContext)
        {
            executionContext.pushCopyTextNodesOnly(fValue);
        }

        ~SetAndRestoreCopyTextNodesOnly()
        {
            m_executionContext.popCopyTextNodesOnly();
        }

    private:

        // Not implemented...
        SetAndRestoreCopyTextNodesOnly(const SetAndRestoreCopyTextNodesOnly&);

        SetAndRestoreCopyTextNodesOnly&
        operator=(const SetAndRestoreCopyTextNodesOnly&);

        // Data members...
        StylesheetExecutionContext&     m_executionContext;

    };
    
#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
    /**
     * Set the flag that determines if the current attribute should be executed
     * @param processAttribute the value of the flag
     */
    virtual void
    pushProcessCurrentAttribute(bool processAttribute) = 0;

    /**
     * Pops the last flag setting that determines if the current attribute should be executed
     */
    virtual bool
    popProcessCurrentAttribute() = 0;

    /**
     * Set the flag that determines if an element's attributes should be skipped
     *
     * @param skipAttributes the value of the flag
     */
    virtual void
    pushSkipElementAttributes(bool skipAttributes) = 0;

    /**
     * Get the last flag setting that determines if an element's attributes should be skipped 
     * @returns the value of the flag
     */
    virtual bool
    getSkipElementAttributes() const = 0;

    /**
     * Pops the last flag setting that determines if an element's attributes should be skipped 
     */
    virtual bool
    popSkipElementAttributes() =  0;

    /**
     * Set flag that determines if the if test was true
     *
     * @param executeIf the value of the flag
     */
    virtual void
    pushExecuteIf(bool executeIf) = 0;
    
    /**
     * Pop the flag that determines if the if test was true
     *
     * @param executeIf the value of the flag
     */
    virtual bool
    popExecuteIf() = 0;
#endif

    /**
     * Retrieve root document for stylesheet.  Note that
     * this does not have to be a XalanDocument -- it can
     * be any node in a document.
     * 
     * @return root document
     */
    virtual XalanNode*
    getRootDocument() const = 0;

    /**
     * Set root document for stylesheet.  Note that
     * this does not have to be a XalanDocument -- it can
     * be any node in a document.
     * 
     * @param theDocument root document
     */
    virtual void
    setRootDocument(XalanNode*  theDocument) = 0;

    /**
     * Set root stylesheet for stylesheet.
     * 
     * @param theStylesheet root stylesheet
     */
    virtual void
    setStylesheetRoot(const StylesheetRoot*     theStylesheet) = 0;

    /**
     * Retrieve the current mode.
     * 
     * @return QName for mode
     */
    virtual const XalanQName*
    getCurrentMode() const = 0;

    /**
     * Set the current mode.
     * 
     * @param theMode QName for mode
     */
    virtual void
    pushCurrentMode(const XalanQName* theMode) = 0; 


    /**
     * Pop the current mode
     */
    virtual void
    popCurrentMode() =0;

    /**
     * Retrieve the current template
     * 
     * @return The current template instance or null if there is no current template
     */
    virtual const ElemTemplate*
    getCurrentTemplate() const = 0;

    /**
     * Set the current template
     * 
     * @param theTemplate The current template instance
     */
    virtual void
    pushCurrentTemplate(const ElemTemplate*     theTemplate) = 0;

    virtual void
    popCurrentTemplate() = 0;

#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
    /*
     * A class to manage pushing and popping the current
     * template instance.
     */
    class PushAndPopCurrentTemplate
    {
    public:

        PushAndPopCurrentTemplate(
            StylesheetExecutionContext&     executionContext,
            const ElemTemplate*             theTemplate) :
            m_executionContext(executionContext)
        {
            executionContext.pushCurrentTemplate(theTemplate);
        }

        ~PushAndPopCurrentTemplate()
        {
            m_executionContext.popCurrentTemplate();
        }

    private:

        // Data members...
        StylesheetExecutionContext&     m_executionContext;
    };
#endif

    /**
     * See if there is an element pending.
     */
    virtual bool
    isElementPending() const = 0;

    /**
     * Replace the contents of a pending attribute.
     * 
     * @param theName           name of attribute
     * @param theNewType        type of attribute
     * @param theNewValue       new value of attribute
     */
    virtual void
    replacePendingAttribute(
            const XalanDOMChar*     theName,
            const XalanDOMChar*     theNewType,
            const XalanDOMChar*     theNewValue) = 0;

    /**
     * Get the current formatter listener.
     * 
     * @return pointer to formatter listener
     */
    virtual FormatterListener*
    getFormatterListener() const = 0;

    /**
     * Set the current formatter listener.
     *
     * @param flistener pointer to new formatter listener
     */
    virtual void
    setFormatterListener(FormatterListener*     flistener) = 0;

    virtual void
    pushOutputContext(FormatterListener*    flistener = 0) = 0;

    virtual void
    popOutputContext() = 0;

    class OutputContextPushPop
    {
    public:

        /**
         * Construct an object to push and pop the current output context.
         *
         * @param theExecutionContext a reference to the current execution context
         * @param theNewListener the new FormatterListener to set.
         */
        OutputContextPushPop(
            StylesheetExecutionContext&     theExecutionContext,
            FormatterListener*              theNewListener = 0) :
            m_executionContext(theExecutionContext)
        {
            m_executionContext.pushOutputContext(theNewListener);
        }

        ~OutputContextPushPop()
        {
            m_executionContext.popOutputContext();
        }

    private:

        StylesheetExecutionContext&     m_executionContext;
    };

    /**
     * Add a result attribute to the list of pending attributes.
     * 
     * @param aname name of attribute
     * @param value value of attribute
     */
    virtual void
    addResultAttribute(
            const XalanDOMString&   aname,
            const XalanDOMString&   value) = 0;

    /**
     * Add a result attribute to the list of pending attributes.
     * 
     * @param aname name of attribute
     * @param value value of attribute
     */
    virtual void
    addResultAttribute(
            const XalanDOMString&   aname,
            const XalanDOMChar*     value) = 0;

    /**
     * Add namespace attributes for a node to the list of pending attributes.
     * 
     * @param src                 source node
     */
    virtual void
    copyNamespaceAttributes(const XalanNode&    src) = 0;

    /**
     * Retrieve the result prefix corresponding to a namespace.
     * 
     * @param theNamespace namespace for prefix
     *
     * @return A pointer to a string containing the prefix, or 0 if the namespace is not mapped.
     */
    virtual const XalanDOMString*
    getResultPrefixForNamespace(const XalanDOMString&   theNamespace) const = 0;

    /**
     * Retrieve the result namespace corresponding to a prefix.
     * 
     * @param thePrefix prefix for namespace
     *
     * @return A pointer to a string containing the namespace, or 0 if the prefix is not mapped.
     */
    virtual const XalanDOMString*
    getResultNamespaceForPrefix(const XalanDOMString&   thePrefix) const = 0;

    /**
     * Determine whether or not a prefix is in use on the pending element or
     * the pending attributes.
     * 
     * @param thePrefix prefix for namespace
     *
     * @return true if the prefix is in use, false if not.
     */
    virtual bool
    isPendingResultPrefix(const XalanDOMString& thePrefix) = 0;


    /**
     * Generate a random namespace prefix guaranteed to be unique.
     * 
     * @param theValue A string for returning the new prefix
     */
    virtual void
    getUniqueNamespaceValue(XalanDOMString&     theValue) const = 0;

    /**
     * Retrieve the current number of spaces to indent.
     * 
     * @return number of spaces
     */
    virtual int
    getIndent() const = 0;

    /**
     * Set the current number of spaces to indent.
     * 
     * @param indentAmount The number of spaces to indent.  Use -1 for the default amount.
     */
    virtual void
    setIndent(int   indentAmount) = 0;

    /**
     * Create and initialize an xpath and return it. This is to be used to
     * create an XPath that is only used during execution.
     *
     * @param str      string expression for XPath evaluation
     * @param resolver resolver for namespace resolution
     * @return pointer to resulting XPath
     */
    virtual const XPath*
    createMatchPattern(
            const XalanDOMString&   str,
            const PrefixResolver&   resolver) = 0;

    /**
     * Return the XPath created by createMatchPattern().
     *
     * @param xpath The XPath to return.
     */
    virtual void
    returnXPath(const XPath*    xpath) = 0;

    // A helper class to automatically return an XPath instance.
    class XPathGuard
    {
    public:

        XPathGuard(
                StylesheetExecutionContext&     context,
                const XPath*                    xpath = 0) :
            m_context(context),
            m_xpath(xpath)
        {
        }

        ~XPathGuard()
        {
            if (m_xpath != 0)
            {
                m_context.returnXPath(m_xpath);
            }
        }

        const XPath*
        get() const
        {
            return m_xpath;
        }

        const XPath*
        release()
        {
            const XPath* const  temp = m_xpath;

            m_xpath = 0;

            return temp;
        }

        void
        reset(const XPath*  xpath)
        {
            if (m_xpath != 0)
            {
                m_context.returnXPath(m_xpath);
            }

            m_xpath = xpath;
        }

    private:

        StylesheetExecutionContext&     m_context;

        const XPath*                    m_xpath;
    };

    typedef XalanVector<TopLevelArg>            ParamVectorType;

    /**
     * Set a list of top level variables in the specified execution context
     * stylesheet.
     *
     * @param topLevelParams   list of top level parameters
     */
    virtual void
    pushTopLevelVariables(const ParamVectorType&    topLevelParams) = 0;

    /**
     * Execute the supplied XPath and and create a
     * variable in the current context.
     *
     * @param str         string expression for XPath evaluation
     * @param contextNode current node in the source tree
     * @param resolver    resolver for namespace resolution
     * @return a pointer to the XObject result
     */
    virtual const XObjectPtr
    createVariable(
            const XPath&                xpath,
            XalanNode*                  contextNode,
            const PrefixResolver&       resolver) = 0;

#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
    /**
     * Create an ResultTreeFragment as a variable and push it
     * on to the stack with the current context.
     *
     * @param templateChild result tree fragment to use.
     * @param sourceNode source node
     * @return a pointer to the XObject result
     */
    virtual const XObjectPtr
    createVariable(
            const ElemTemplateElement&  templateChild,
            XalanNode*                  sourceNode) = 0;
#endif

    /**
     * Execute an XPath using the provided expression, 
     * and push the result as a variable in the context of
     * the supplied element.
     *
     * @param name        name of variable
     * @param element     element marker for variable
     * @param str         string expression for XPath evaluation
     * @param contextNode current node in the source tree
     * @param resolver    resolver for namespace resolution
     * @return nothing
     */
    virtual void
    pushVariable(
            const XalanQName&           name,
            const ElemTemplateElement*  element,
            const XalanDOMString&       str,
            XalanNode*                  contextNode,
            const PrefixResolver&       resolver) = 0;

    /**
     * Execute the supplied XPath and push the result as a
     * variable in the current context.
     *
     * @param name        name of variable
     * @param element     element marker for variable
     * @param str         string expression for XPath evaluation
     * @param contextNode current node in the source tree
     * @param resolver    resolver for namespace resolution
     * @return nothing
     */
    virtual void
    pushVariable(
            const XalanQName&           name,
            const ElemTemplateElement*  element,
            const XPath&                xpath,
            XalanNode*                  contextNode,
            const PrefixResolver&       resolver) = 0;

#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
    /**
     * Create an ResultTreeFragment as a variable and push it
     * on to the stack with the current context.
     *
     * @param name    name of variable
     * @param element element marker for variable
     * @param templateChild result tree fragment to use.
     * @param sourceNode source node
     */
    virtual void
    pushVariable(
            const XalanQName&           name,
            const ElemTemplateElement*  element,
            const ElemTemplateElement&  templateChild,
            XalanNode*                  sourceNode) = 0;
#endif

    /**
     * Push a named variable onto the variables stack.
     * The variable has already been evaluated.
     *
     * @param name    name of variable
     * @param val     pointer to XObject value
     * @param element element marker for variable
     */
    virtual void
    pushVariable(
            const XalanQName&           name,
            const XObjectPtr            val,
            const ElemTemplateElement*  element) = 0;

    /**
     * Push a named variable onto the processor variable stack
     * The variable will be evaluated when first referenced.
     *
     * @param name    name of variable
     * @param var     pointer to ElemVariable instance
     * @param element element marker for variable
     */
    virtual void
    pushVariable(
            const XalanQName&           name,
            const ElemVariable*         var,
            const ElemTemplateElement*  element) = 0;

    /**
     * Push a context marker onto the stack to let us know when to stop
     * searching for a var.
     */
    virtual void
    pushContextMarker() = 0;

    /**
     * Pop the current context from the current context stack.
     */
    virtual void
    popContextMarker() = 0;

#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
    /*
     * A class to manage pushing and popping an element's stack
     * frame context.
     */
    class PushAndPopContextMarker
    {
    public:

        PushAndPopContextMarker(StylesheetExecutionContext&     executionContext) :
            m_executionContext(executionContext)
        {
            executionContext.pushContextMarker();
        }

        PushAndPopContextMarker(
                StylesheetExecutionContext&     executionContext,
                int&                            currentStackFrameIndex) :
            m_executionContext(executionContext)
        {
            currentStackFrameIndex = executionContext.getCurrentStackFrameIndex();

            executionContext.pushContextMarker();
        }

        ~PushAndPopContextMarker()
        {
            m_executionContext.popContextMarker();
        }

        StylesheetExecutionContext&
        getExecutionContext() const
        {
            return m_executionContext;
        }

    private:

        StylesheetExecutionContext&     m_executionContext;
    };
#endif

    /**
     * Resolve the params that were pushed by the caller.
     */
    virtual void
    resolveTopLevelParams() = 0;

    /**
     * Reset the vector of top level parameters.
     */
    virtual void
    clearTopLevelParams() = 0;

#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
    class ResolveAndClearTopLevelParams
    {
    public:

        ResolveAndClearTopLevelParams(StylesheetExecutionContext&   executionContext) :
            m_executionContext(executionContext)
        {
            m_executionContext.resolveTopLevelParams();
        }

        ~ResolveAndClearTopLevelParams()
        {
            m_executionContext.clearTopLevelParams();
        }

    private:

        StylesheetExecutionContext&     m_executionContext;
    };

    /**
     * Given a template, search for the arguments and push them on the stack.
     * Also, push default arguments on the stack.
     *
     * @param xslCallTemplateElement "call-template" element
     */
    virtual void
    pushParams(const ElemTemplateElement&   xslCallTemplateElement) = 0;

#else

    /**
     *  Initiate context to accept a new set of parameters
     */
    virtual void beginParams() = 0;

    /**
     *  Indicate parameter set is complete
     */
    virtual void endParams() = 0;

    /**
     * Push a single paramter onto the latest initialized paramter set
     * @param qName     the name of the parameter
     * @param theValue  the value of the parameter
     */
    virtual void pushParam(const XalanQName& qName,const XObjectPtr& theValue) = 0;
#endif

    /**
     * Given a name, return a string representing the value, but don't look in
     * the global space.
     *
     * @param theName name of variable
     * @return An XObjectPtr instance.  Call XObjectPtr::null() on the instance
     *         to determine if the variable was found.  If XObjectPtr::null()
     *         returns true, the variable was not found, and no other operations
     *         on the XObject instance are permitted.
     */
    virtual const XObjectPtr
    getParamVariable(const XalanQName&  theName) = 0;

    /**
     * Push a frame marker for an element.
     *
     * @param elem the element
     */
    virtual void
    pushElementFrame(const ElemTemplateElement*     elem) = 0;

    /**
     * Pop a frame marker for an element.
     *
     * @param elem the element
     */
    virtual void
    popElementFrame() = 0;

#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
    /*
     * A class to manage pushing and popping an element's stack
     * frame context.
     */
    class PushAndPopElementFrame
    {
    public:

        PushAndPopElementFrame(
            StylesheetExecutionContext&     executionContext,
            const ElemTemplateElement*      element) :
            m_executionContext(executionContext)
        {
            executionContext.pushElementFrame(element);
        }

        ~PushAndPopElementFrame()
        {
            m_executionContext.popElementFrame();
        }

    private:

        StylesheetExecutionContext&     m_executionContext;
    };
#endif

    /**
     * Get the top of the global stack frame.
     *
     * @return current value of index
     */
    virtual int
    getGlobalStackFrameIndex() const = 0;

    /**
     * Get the top of the stack frame from where a search 
     * for a variable or param should take place.
     *
     * @return current value of index
     */
    virtual int
    getCurrentStackFrameIndex() const = 0;

    /**
     * Set the top of the stack frame from where a search 
     * for a variable or param should take place.
     *
     * @param currentStackFrameIndex new value of index
     */
    virtual void
    pushCurrentStackFrameIndex(int currentStackFrameIndex = -1) =  0;

    /**
     * Pop the last stack frame index setting
     */
    virtual void
    popCurrentStackFrameIndex() = 0;

    /*
     * A class to manage the state of the variable stacks frame index.
     */
    class SetAndRestoreCurrentStackFrameIndex
    {
    public:

        SetAndRestoreCurrentStackFrameIndex(
            StylesheetExecutionContext&     executionContext,
            int                             newIndex) :
            m_executionContext(executionContext),
            m_savedIndex(executionContext.getCurrentStackFrameIndex())
        {
            executionContext.pushCurrentStackFrameIndex(newIndex);
        }

        ~SetAndRestoreCurrentStackFrameIndex()
        {
            m_executionContext.popCurrentStackFrameIndex();
        }

    private:

        StylesheetExecutionContext&     m_executionContext;

        const int                       m_savedIndex;
    };

#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
    /*
     * A class to manage stack state during execution.
     */
    class ParamsPushPop : public PushAndPopContextMarker
    {
    public:

        ParamsPushPop(
                StylesheetExecutionContext&     executionContext,
                const ElemTemplateElement&      xslCallTemplateElement) :
            PushAndPopContextMarker(executionContext)
        {
            doPush(xslCallTemplateElement);
        }

        ParamsPushPop(
                StylesheetExecutionContext&     executionContext,
                const ElemTemplateElement&      xslCallTemplateElement,
                int&                            savedStackFrameIndex) :
            PushAndPopContextMarker(executionContext, savedStackFrameIndex)
        {
            doPush(
                xslCallTemplateElement,
                savedStackFrameIndex);
        }

        ~ParamsPushPop()
        {
        }

    private:

        void
        doPush(
            const ElemTemplateElement&      xslCallTemplateElement,
            int                             stackFrameIndex);

        void
        doPush(const ElemTemplateElement&   xslCallTemplateElement);
    };
#endif

    /**
     * Receive notification of the beginning of a document.
     *
     * <p>The SAX parser will invoke this method only once, before any
     * other methods in this interface or in DTDHandler (except for
     * setDocumentLocator).</p>
     *
     * @exception SAXException
     */
    virtual void
    startDocument() = 0;

    /**
     * Receive notification of the end of a document.
     *
     * <p>The SAX parser will invoke this method only once, and it will
     * be the last method invoked during the parse.  The parser shall
     * not invoke this method until it has either abandoned parsing
     * (because of an unrecoverable error) or reached the end of
     * input.</p>
     *
     * @exception SAXException
     */
    virtual void
    endDocument() = 0;

    /**
     * Receive notification of the beginning of an element.
     *
     * @param name element type name
     */
    virtual void
    startElement(const XalanDOMChar*    name) = 0;

    /**
     * Receive notification of the end of an element.
     *
     * @param name element type name
     */
    virtual void
    endElement(const XalanDOMChar*  name) = 0;

    /**
     * Receive notification of character data.
     *
     * @param ch     pointer to characters from the XML document
     * @param start  start position in the array
     * @param length number of characters to read from the array
     */
    virtual void
    characters(
            const XalanDOMChar*         ch,
            XalanDOMString::size_type   start,
            XalanDOMString::size_type   length) = 0;

    /**
     * Receive notification of character data. If available, when the
     * disable-output-escaping attribute is used, output raw text without
     * escaping.
     *
     * @param ch     pointer to characters from the XML document
     * @param start  start position in the array
     * @param length number of characters to read from the array
     */
    virtual void
    charactersRaw(
            const XalanDOMChar*         ch,
            XalanDOMString::size_type   start,
            XalanDOMString::size_type   length) = 0;

    /**
     * Called when a Comment is to be constructed.
     *
     * @param   data    pointer to comment data
     */
    virtual void
    comment(const XalanDOMChar*     data) = 0;

    /**
     * Receive notification of a processing instruction.
     *
     * @param target processing instruction target
     * @param data   processing instruction data, or null if none was supplied
     */
    virtual void
    processingInstruction(
            const XalanDOMChar*     target,
            const XalanDOMChar*     data) = 0;

    /**
     * Flush the pending element.
     */
    virtual void
    flushPending() = 0;

    /**
     * Clone a node to the result tree
     *
     * @param node node to clone
     * @param locator The Locator, if any
     */
    virtual void
    cloneToResultTree(
            const XalanNode&    node,
            const LocatorType*  locator) = 0;

    /**
     * Clone a node to the result tree
     *
     * @param node                  node to clone
     * @param nodeType              the type of the node
     * @param overrideStrip         false if white space stripping should be done
     * @param shouldCloneAttributes true if attributes should be cloned
     * @param locator               The Locator, if any
     */
    virtual void
    cloneToResultTree(
            const XalanNode&        node,
            XalanNode::NodeType     nodeType,
            bool                    overrideStrip,
            bool                    shouldCloneAttributes,
            const LocatorType*      locator) = 0;

#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)  
    /**
     * Initiate creation of a result tree fragment
     * @param sourceNode the source Node
     */
    virtual void
    beginCreateXResultTreeFrag(
            XalanNode*                  sourceNode) = 0;

    /**
     * Indicate sthe completion of result tree fragment
     * @return a pointer to the result tree fragment
     */
    virtual const XObjectPtr
    endCreateXResultTreeFrag() = 0;

    /**
     * Initiate to put execution result in string
     * 
     * @param theResult the string to contain the result
     */
    virtual void
    beginFormatToText(
            XalanDOMString&             theResult) = 0;

    /**
     * Indicates the completion of the result string
     *
     */
    virtual void
    endFormatToText() = 0;
#else
    /**
     * Create an XObject that represents a Result tree fragment.
     *
     * @param templateChild result tree fragment to use.
     * @param sourceNode source node
     * @return XObject instance
     */
    virtual const XObjectPtr
    createXResultTreeFrag(
            const ElemTemplateElement&  templateChild,
            XalanNode*                  sourceNode) = 0;
#endif

    /**
     * Output an object to the result tree by doing the right conversions.
     * This is public for access by extensions.
     *
     * @param obj the XObject to output
     * @param locator The Locator, if any
     */
    virtual void
    outputToResultTree(
            const XObject&      xobj,
            const LocatorType*  locator) = 0;

    /**
     * Given a result tree fragment, walk the tree and
     * output it to the result stream.
     *
     * @param theTree result tree fragment
     * @param locator The Locator, if any
     */
    virtual void
    outputResultTreeFragment(
            const XObject&      theTree,
            const LocatorType*  locator) = 0;

    /**
     * Determine the full XSLT Namespace URI.
     *
     * @return Xalan namespace URI
     */
    virtual const XalanDOMString&
    getXSLNameSpaceURL() const = 0;

    /**
     * Special Xalan namespace for built-in extensions.
     *
     * @return Xalan namespace for extensions
     */
    virtual const XalanDOMString&
    getXalanXSLNameSpaceURL() const = 0;

    /**
     * Determine if an element is on the recursion stack.
     *
     * @return true if element on stack
     */
    virtual bool
    findOnElementRecursionStack(const ElemTemplateElement*  theElement) const = 0;

    /**
     * Push an element onto the recursion stack.
     *
     * @param theElement pointer to element to push
     */
    virtual void
    pushOnElementRecursionStack(const ElemTemplateElement*  theElement) = 0;

    /**
     * Pop an element off the recursion stack.
     *
     * @return pointer to element popped
     */
    virtual const ElemTemplateElement*
    popElementRecursionStack() = 0;

    /**
     * Class for keeping track of elements pushed on the element recursion stack
     */
    class ElementRecursionStackPusher
    {
    public:

    /**
     * Construct an instance of the recursion stack pusher.
     *
     * @param executionContext current execution context
     * @param element pointer to element to push
     */
        ElementRecursionStackPusher(
                    StylesheetExecutionContext&     executionContext,
                    const ElemTemplateElement*      element) :
            m_executionContext(executionContext)
        {
            m_executionContext.pushOnElementRecursionStack(element);
        }

        ~ElementRecursionStackPusher()
        {
            m_executionContext.popElementRecursionStack();
        }

    private:

        StylesheetExecutionContext&         m_executionContext;
    };


    /**
     * This is a hook that XResultTreeFrag instances (which are reference
     * counted), can notify the owning StylesheetExecutionContext instance
     * when they are dereferenced and can be cleaned up.
     *
     * @param theXResultTreeFrag The instance that is being returned.
     *
     * @return true if the XResultTreeFrag instance belongs to the execution context. false if not.
     */
    virtual bool
    returnXResultTreeFrag(XResultTreeFrag*  theXResultTreeFrag) = 0;


    enum eDummy
    {
        eDefaultXMLIndentAmount = 0,
        eDefaultHTMLIndentAmount = 0
    };

    /**
     * Enums to determine whether or not run-time escaping of URLs has been set.
     */
    enum eEscapeURLs
    {
        eEscapeURLsDefault,     // Use the value in the stylesheet
        eEscapeURLsNo,          // Don't escape URLs
        eEscapeURLsYes          // Escape URLs
    };

    /**
     * Get the value for run-time escaping of URLs.  This can
     * override the property specified by the stylesheet.  The
     * default behavior is to honor the property in the stylesheet.
     *
     * @return The value of the enum
     */
    virtual eEscapeURLs
    getEscapeURLs() const = 0;

    /**
     * Set the value for run-time escaping of URLs.  This can
     * override the property specified by the stylesheet.  The
     * default behavior is to honor the property in the stylesheet.
     *
     * @param value The value of the enum
     */
    virtual void
    setEscapeURLs(eEscapeURLs   value) = 0;


    /**
     * Enums to determine whether or not run-time omission of the META tag has been set.
     */
    enum eOmitMETATag
    {
        eOmitMETATagDefault,    // Use the value in the stylesheet
        eOmitMETATagNo,         // Don't omit the META tag
        eOmitMETATagYes         // Omit the META tag
    };

    /**
     * Get the value for run-time omission of URLs.  This can
     * override the property specified by the stylesheet.  The
     * default behavior is to honor the property in the stylesheet.
     *
     * @return The value of the enum
     */
    virtual eOmitMETATag
    getOmitMETATag() const = 0;

    /**
     * Get the value for run-time omission of URLs.  This can
     * override the property specified by the stylesheet.  The
     * default behavior is to honor the property in the stylesheet.
     *
     * @param value The value of the enum
     */
    virtual void
    setOmitMETATag(eOmitMETATag     value) = 0;

    /**
     * Create a new FormatterToXML instance.  The execution context
     * owns the instance and will delete it when reset.
     *
     * @param writer            character output stream to use
     * @param version           version of the output method
     * @param doIndent          true if output is to be indented
     * @param indent            number of spaces to indent at each nesting level
     * @param encoding          character encoding for the writer
     * @param mediaType         media type (MIME content type) of the data
     * @param doctypeSystem     system identifier to be used in the document
     *                          type declaration
     * @param doctypePublic     public identifier to be used in the document
     *                          type declaration
     * @param xmlDecl           true if the XSLT processor should output an XML
     *                          declaration
     * @param standalone        true if the XSLT processor should output a
     *                          standalone document declaration
     * @return a pointer to the new instance.
     */

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
            const XalanDOMString&   standalone = XalanDOMString(XalanMemMgrs::getDummyMemMgr())) = 0;

    /**
     * Create a new FormatterToHTML instance.  The execution context
     * owns the instance and will delete it when reset.
     *
     * @param writer            character output stream to use
     * @param encoding          character encoding for the writer
     * @param mediaType         media type (MIME content type) of the data
     * @param doctypeSystem     system identifier to be used in the document
     *                          type declaration
     * @param doctypePublic     public identifier to be used in the document
     *                          type declaration
     * @param doIndent          true if output is to be indented
     * @param indent            number of spaces to indent at each nesting level
     * @param escapeURLs        Whether or not to escape URLs according to the recommendation.  The default is true.
     * @param omitMetaTag       Whether or not to output a META TAG according to the recommendation.  The default is false.
     * @return a pointer to the new instance.
     */
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
            bool                    omitMetaTag = false) = 0;

    /**
     * FormatterToText instance constructor.
     *
     * @param writer writer for output
     * @param encoding character encoding for the writer
     */
    virtual FormatterListener*
    createFormatterToText(
            Writer&                 writer,
            const XalanDOMString&   encoding) = 0;

#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
    class BorrowReturnFormatterToText
    {
    public:

        BorrowReturnFormatterToText(
                StylesheetExecutionContext&     executionContext,
                Writer&                         writer,
                bool                            normalizeLinefeed = true,
                bool                            handleIgnorableWhitespace = true);

        ~BorrowReturnFormatterToText()
        {
            assert(m_formatter != 0);

            m_executionContext.returnFormatterToText(m_formatter);
        }

        FormatterToText&
        operator*() const
        {
            assert(m_formatter != 0);

            return *m_formatter;
        }

        FormatterToText*
        get() const
        {
            assert(m_formatter != 0);

            return m_formatter;
        }

        FormatterToText*
        operator->() const
        {
            return get();
        }

    private:

        StylesheetExecutionContext&     m_executionContext;

        FormatterToText*                m_formatter;
    };


    friend class BorrowReturnFormatterToText;
#endif

#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
    /**
     * Get node sorter instance
     */
    virtual NodeSorter*
    getNodeSorter() = 0;

#else
    /**
     * Borrow a cached NodeSorter instance.
     *
     * @return A pointer to the instance.
     */
    virtual NodeSorter*
    borrowNodeSorter() = 0;

    /**
     * Return a previously borrowed NodeSorter instance.
     *
     * @param theSorter A pointer the to previously borrowed instance.
     * @return true if the instance was previously borrowed, false if not.
     */
    virtual bool
    returnNodeSorter(NodeSorter*    theSorter) = 0;

    class BorrowReturnNodeSorter
    {
    public:

        BorrowReturnNodeSorter(StylesheetExecutionContext&  executionContext) :
            m_executionContext(executionContext),
            m_sorter(executionContext.borrowNodeSorter())
        {
            assert(m_sorter != 0);
        }

        ~BorrowReturnNodeSorter()
        {
            assert(m_sorter != 0);

            m_executionContext.returnNodeSorter(m_sorter);
        }

        NodeSorter&
        operator*() const
        {
            assert(m_sorter != 0);

            return *m_sorter;
        }

        NodeSorter*
        get() const
        {
            assert(m_sorter != 0);

            return m_sorter;
        }

        NodeSorter*
        operator->() const
        {
            return get();
        }

    private:

        StylesheetExecutionContext&     m_executionContext;

        NodeSorter*                     m_sorter;
    };
#endif

    typedef XalanMemMgrAutoPtr<XalanNumberFormat,true>      XalanNumberFormatAutoPtr;

    /**
     * Create a new XalanNumberFormat instance.
     *
     * @return an XalanNumberFormatAutoPtr that owns a new
     * XalanNumberFormat instance.
     */
    virtual XalanNumberFormatAutoPtr
    createXalanNumberFormat() = 0;

#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
    /*
     * A class to manage the attribute sets that have been executed
     * by an element
     */
    struct UseAttributeSetIndexes
    {
        UseAttributeSetIndexes() :
            attributeSetNameIndex(0),
            matchingAttributeSetIndex(0) {}
            
        size_type attributeSetNameIndex;
        size_type matchingAttributeSetIndex;


    };

    virtual void
    createUseAttributeSetIndexesOnStack() = 0;

    virtual UseAttributeSetIndexes&
    getUseAttributeSetIndexes() = 0;

    virtual void
    popUseAttributeSetIndexesFromStack() = 0;

    /**
     * Push the element that will invoke 
     * non children elements (i.e templates, attribute-sets)
     *
     * @param invokers the element that will invoke non children elements
     */
    virtual void
    pushInvoker(const ElemTemplateElement * invoker) = 0;

    /**
     * Pop the element that invoked non children elements
     */
    virtual void
    popInvoker() = 0;

    /**
     * Get the lastest element that has invoked 
     * a non-child element
     */
    virtual const ElemTemplateElement*
    getInvoker() const = 0;
#endif

    /**
     * Determine the number of trace listeners.
     * 
     * @return number of listeners
     */
    virtual tl_size_type
    getTraceListeners() const = 0;

    /**
     * Fire a generate event.
     * 
     * @param ge generate event to fire
     */
    virtual void
    fireGenerateEvent(const GenerateEvent&  ge) = 0;

    /**
     * Fire a trace event.
     * 
     * @param te trace event to fire
     */
    virtual void
    fireTraceEvent(const TracerEvent&   te) = 0;

    /**
     * Fire a selection event.
     * 
     * @param se selection event to fire
     */
    virtual void
    fireSelectEvent(const SelectionEvent&   se) = 0;

    /**
     * If this is set to true, simple traces of template calls are made.
     *
     * @return true if traces made
     */
    virtual bool
    getTraceSelects() const = 0;

    /**
     * Compose a diagnostic trace of the current selection
     *
     * @param theStylesheetElement The executing stylesheet element
     * @param nl The list of selected nodes
     * @param xpath A pointer to the XPath which generated the list of nodes, if any.
     */
    virtual void
    traceSelect(
            const ElemTemplateElement&  theStylesheetElement,
            const NodeRefListBase&      nl,
            const XPath*                xpath) = 0;

    /**
     * Compare two strings using the collation of the
     * current locale.
     *
     * @param theLHS a string to compare
     * @param theRHS a string to compare
     * @param theCaseOrder the case order for the comparison
     * @return < 0 if theLHS is before theRHS, 0 if they are equal, or > 0 if theLHS is after theRHS
     */
    virtual int
    collationCompare(
            const XalanDOMString&               theLHS,
            const XalanDOMString&               theRHS,
            XalanCollationServices::eCaseOrder  theCaseOrder = XalanCollationServices::eDefault) = 0;

    /**
     * Compare two strings using the collation of the
     * supplied locale.
     *
     * @param theLHS a string to compare
     * @param theRHS a string to compare
     * @param theLocal a string that specifies the locale
     * @param theCaseOrder the case order for the comparison
     * @return < 0 if theLHS is before theRHS, 0 if they are equal, or > 0 if theLHS is after theRHS
     */
    virtual int
    collationCompare(
            const XalanDOMString&               theLHS,
            const XalanDOMString&               theRHS,
            const XalanDOMString&               theLocale,
            XalanCollationServices::eCaseOrder  theCaseOrder = XalanCollationServices::eDefault) = 0;

    /**
     * Compare two strings using the collation of the
     * current locale.
     *
     * @param theLHS a string to compare
     * @param theRHS a string to compare
     * @param theCaseOrder the case order for the comparison
     * @return < 0 if theLHS is before theRHS, 0 if they are equal, or > 0 if theLHS is after theRHS
     */
    virtual int
    collationCompare(
            const XalanDOMChar*                 theLHS,
            const XalanDOMChar*                 theRHS,
            XalanCollationServices::eCaseOrder  theCaseOrder = XalanCollationServices::eDefault) = 0;

    /**
     * Compare two strings using the collation of the
     * current locale.
     *
     * @param theLHS a string to compare
     * @param theRHS a string to compare
     * @param theLocal a string that specifies the locale
     * @param theCaseOrder the case order for the comparison
     * @return < 0 if theLHS is before theRHS, 0 if they are equal, or > 0 if theLHS is after theRHS
     */
    virtual int
    collationCompare(
            const XalanDOMChar*                 theLHS,
            const XalanDOMChar*                 theRHS,
            const XalanDOMChar*                 theLocale,
            XalanCollationServices::eCaseOrder  theCaseOrder = XalanCollationServices::eDefault) = 0;

    /**
     * Create a PrintWriter for the provided stream.
     * 
     * @param theTextOutputStream The output stream for the PrintWriter.
     * @return The new instance.
     */
    virtual PrintWriter*
    createPrintWriter(XalanOutputStream*        theTextOutputStream) = 0;

    /**
     * Create a PrintWriter.  Create an appropriate output stream
     * using the provided file name and encoding.
     * 
     * @param theFileName The file name for the output stream
     * @param theEncoding The encoding for the output stream
     * @return The new instance.
     */
    virtual PrintWriter*
    createPrintWriter(
            const XalanDOMString&       theFileName,
            const XalanDOMString&       theEncoding) = 0;

    /**
     * Create a PrintWriter using the provided ostream instance.
     * 
     * @param theStream The output stream for the PrintWriter.
     * @return The new instance.
     */
    virtual PrintWriter*
    createPrintWriter(StreamType&   theStream) = 0;

    /**
     * Create a PrintWriter using the provided FILE instance.
     * 
     * @param theStream The output stream for the PrintWriter.
     * @return The new instance.
     */
    virtual PrintWriter*
    createPrintWriter(FILE*     theStream) = 0;

    /**
     * Get the counters table, which is a table of cached
     * results that is used by ElemNumber.
     * 
     * @return A reference to the counters table.
     */
    virtual CountersTable&
    getCountersTable() = 0;

    /**
     * Send character data from a node to the result tree.
     *
     * @param node The node to send.
     */
    virtual void
    characters(const XalanNode&     node) = 0;

    /**
     * Send character data from an XObject to the result tree.
     *
     * @param node The xobject to send.
     */
    virtual void
    characters(const XObjectPtr&    xobject) = 0;

    /**
     * Send raw character data from a node to the result tree.
     *
     * @param node The node to send.
     * @param length number of characters to read from the array
     */
    virtual void
    charactersRaw(const XalanNode&  node) = 0;

    /**
     * Send raw character data from an XObject to the result tree.
     *
     * @param node The xobject to send.
     */
    virtual void
    charactersRaw(const XObjectPtr&     xobject) = 0;


    // These interfaces are inherited from XPathExecutionContext...

    virtual void
    reset() = 0;

    virtual XalanNode*
    getCurrentNode() const = 0;

    virtual void
    pushCurrentNode(XalanNode*  theCurrentNode) = 0;

    virtual void
    popCurrentNode() = 0;

    virtual bool
    isNodeAfter(
            const XalanNode&    node1,
            const XalanNode&    node2) const = 0;

    virtual void
    pushContextNodeList(const NodeRefListBase&  theList) = 0;

    virtual void    
    popContextNodeList() = 0;

    virtual const NodeRefListBase&
    getContextNodeList() const = 0;

    virtual size_type
    getContextNodeListLength() const = 0;

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

    virtual const XObjectPtr
    extFunction(
            const XalanDOMString&           theNamespace,
            const XalanDOMString&           functionName,
            XalanNode*                      context,
            const XObjectArgVectorType&     argVec,
            const LocatorType*              locator) = 0;

    virtual XalanDocument*
    parseXML(
            MemoryManagerType&      theManager,
            const XalanDOMString&   urlString,
            const XalanDOMString&   base) const = 0;

    virtual MutableNodeRefList*
    borrowMutableNodeRefList() = 0;

    virtual bool
    returnMutableNodeRefList(MutableNodeRefList*    theList) = 0;

    virtual MutableNodeRefList*
    createMutableNodeRefList(MemoryManagerType& theManager) const = 0;

#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)

    virtual MutableNodeRefList& 
    createAndPushMutableNodeRefList() = 0;

    virtual void 
    releaseAndPopMutableNodeRefList() = 0;

    virtual void
    pushXObjectPtr(const XObjectPtr& xobjectPtr) = 0;

    virtual void 
    popXObjectPtr() = 0;

    virtual void 
    createAndPushNodesToTransformList(const NodeRefListBase* nodeList) = 0;

    virtual XalanNode* 
    getNextNodeToTransform() = 0;

    virtual void 
    popNodesToTransformList() = 0;

    /**
     * Get a string that is cached on a stack
     * @returns a cached string
     */
    virtual XalanDOMString&
    getAndPushCachedString() = 0;

    /**
     * Gets the last string that was cached on the stack
     * @returns the last string to be cached
     */
    virtual XalanDOMString&
    getLastCachedString() = 0;

    /**
     * Gets the last string to be cached on the stack and 
     * pops it from the stack.   The reference is valid until
     * the next request is made for a cached string
     *
     * @returns the last string to be cached
     */
    virtual XalanDOMString&
    getAndPopCachedString() = 0;
#endif

    virtual XalanDOMString&
    getCachedString() = 0;

    virtual bool
    releaseCachedString(XalanDOMString&     theString) = 0;


    virtual void
    getNodeSetByKey(
            XalanDocument*          doc,
            const XalanQName&       qname,
            const XalanDOMString&   ref,
            MutableNodeRefList&     nodelist) = 0;

    virtual void
    getNodeSetByKey(
            XalanDocument*          doc,
            const XalanDOMString&   name,
            const XalanDOMString&   ref,
            const LocatorType*      locator,
            MutableNodeRefList&     nodelist) = 0;

    virtual const XObjectPtr
    getVariable(
            const XalanQName&   name,
            const LocatorType*  locator = 0) = 0;

    virtual const PrefixResolver*
    getPrefixResolver() const = 0;

    virtual void
    setPrefixResolver(const PrefixResolver*     thePrefixResolver) = 0;

    virtual const XalanDOMString*
    getNamespaceForPrefix(const XalanDOMString&     prefix) const = 0;

    virtual const XalanDOMString&
    findURIFromDoc(const XalanDocument*     owner) const = 0;

    virtual const XalanDOMString&
    getUnparsedEntityURI(
            const XalanDOMString&   theName,
            const XalanDocument&    theDocument) const = 0;

    virtual bool
    shouldStripSourceNode(const XalanText&  node) = 0;

    virtual XalanDocument*
    getSourceDocument(const XalanDOMString&     theURI) const = 0;

    virtual void
    setSourceDocument(
            const XalanDOMString&   theURI,
            XalanDocument*          theDocument) = 0;

    virtual void
    formatNumber(
            double                  number,
            const XalanDOMString&   pattern,
            XalanDOMString&         theResult,
            const XalanNode*        context = 0,
            const LocatorType*      locator = 0) = 0;

    virtual void
    formatNumber(
            double                  number,
            const XalanDOMString&   pattern,
            const XalanDOMString&   dfsName,
            XalanDOMString&         theResult,
            const XalanNode*        context = 0,
            const LocatorType*      locator = 0) = 0;

    // These interfaces are inherited from ExecutionContext...

    virtual void
    error(
            const XalanDOMString&   msg,
            const XalanNode*        sourceNode,
            const LocatorType*      locator) const = 0;

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

#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
protected:
    /**
     * Borrow a cached FormatterToText instance.
     *
     * @return A pointer to the instance.
     */
    virtual FormatterToText*
    borrowFormatterToText() = 0;

    /**
     * Return a previously borrowed FormatterToText instance.
     *
     * @param theFormatter A pointer the to previously borrowed instance.
     * @return true if the instance was previously borrowed, false if not.
     */
    virtual bool
    returnFormatterToText(FormatterToText*  theFormatter) = 0;
#endif
};



XALAN_CPP_NAMESPACE_END



#endif  // STYLESHEETEXECUTIONCONTEXT_HEADER_GUARD_1357924680
