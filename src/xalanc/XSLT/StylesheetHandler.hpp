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

#if !defined(XALAN_STYLESHEETHANDLER_HEADER_GUARD)
#define XALAN_STYLESHEETHANDLER_HEADER_GUARD

// Base include file.   Must be first.
#include "XSLTDefinitions.hpp"



#include <xalanc/Include/XalanVector.hpp>
#include <xalanc/Include/XalanSet.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/PlatformSupport/FormatterListener.hpp>



#include <xalanc/XPath/XalanQNameByValue.hpp>



#include <xalanc/XSLT/NamespacesHandler.hpp>
#include <xalanc/XSLT/Stylesheet.hpp>
#include <xalanc/XSLT/XalanElemEmptyAllocator.hpp>
#include <xalanc/XSLT/XalanElemTextAllocator.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class ElemTemplate;
class ElemTemplateElement;
class ElemTextLiteral;
class ExtensionNSHandler;
class StylesheetConstructionContext;



/**
 * This class processes a stylesheet via SAX events, and inits
 * the given stylesheet.    If you need to alter the code in here, 
 * it is not for the faint-of-heart, due to the state tracking 
 * that has to be done due to the SAX event model.
 */
class XALAN_XSLT_EXPORT StylesheetHandler : public FormatterListener
{

public:

    typedef XalanVector<ElemTemplateElement*>       ElemTemplateStackType;
    typedef XalanVector<ElemTemplateElement*>       ElemTextLiteralStackType;

    typedef XalanVector<bool>                       BoolStackType;
    typedef XalanSet<XalanQNameByReference>         QNameSetType;
    typedef XalanVector<QNameSetType, ConstructWithMemoryManagerTraits<QNameSetType> >
                                                    QNameSetVectorType;

    /**
     * Perform static initialization.  See class XMLSupportInit.
     */
    static void
    initialize(MemoryManagerType&  theManager);

    /**
     * Perform static shut down.  See class XMLSupportInit.
     */
    static void
    terminate();

    /**
     * Construct a StylesheetHandler ... it will add the DOM nodes 
     * to the document fragment.
     */
    StylesheetHandler(
            Stylesheet&                     stylesheetTree,
            StylesheetConstructionContext&  constructionContext);

    MemoryManagerType&
    getMemoryManager()
    {
        return m_constructionContext.getMemoryManager();
    }

    virtual
    ~StylesheetHandler();

    /**
     * Receive notification of character data.
     *
     * <p>The Parser will call this method to report each chunk of
     * character data.  SAX parsers may return all contiguous character
     * data in a single chunk, or they may split it into several
     * chunks; however, all of the characters in any single event
     * must come from the same external entity, so that the Locator
     * provides useful information.</p>
     *
     * <p>The application must not attempt to read from the array
     * outside of the specified range.</p>
     *
     * <p>Note that some parsers will report whitespace using the
     * ignorableWhitespace() method rather than this one (validating
     * parsers must do so).</p>
     *
     * @param chars  pointer to characters from the XML document
     * @param length number of characters to read from the array
     * @exception SAXException
     * @see #ignorableWhitespace 
     * @see org.xml.sax.Locator
     */
    virtual void characters (const XMLCh* const chars, const unsigned int length);

    /**
     * Receive notification of character data. If available, when the
     * disable-output-escaping attribute is used, output raw text without
     * escaping.
     *
     * @param ch pointer to characters from the XML document
     * @param start start position in the array
     * @param length number of characters to read from the array
     * @exception SAXException
     */
    virtual void charactersRaw(const XMLCh* const chars, const unsigned int length);

    /**
     * Receive notification of cdata.
     *
     * <p>The Parser will call this method to report each chunk of
     * character data.  SAX parsers may return all contiguous character
     * data in a single chunk, or they may split it into several
     * chunks; however, all of the characters in any single event
     * must come from the same external entity, so that the Locator
     * provides useful information.</p>
     *
     * <p>The application must not attempt to read from the array
     * outside of the specified range.</p>
     *
     * <p>Note that some parsers will report whitespace using the
     * ignorableWhitespace() method rather than this one (validating
     * parsers must do so).</p>
     *
     * @param ch     pointer to characters from the XML document
     * @param start  start position in the array
     * @param length number of characters to read from the array
     * @exception SAXException
     * @see #ignorableWhitespace 
     */
    virtual void cdata(const XMLCh* const ch, const unsigned int length);

    /**
     * Receive notification of ignorable whitespace in element content.
     *
     * <p>Validating Parsers must use this method to report each chunk
     * of ignorable whitespace (see the W3C XML 1.0 recommendation,
     * section 2.10): non-validating parsers may also use this method
     * if they are capable of parsing and using content models.</p>
     *
     * <p>SAX parsers may return all contiguous whitespace in a single
     * chunk, or they may split it into several chunks; however, all of
     * the characters in any single event must come from the same
     * external entity, so that the Locator provides useful
     * information.</p>
     *
     * <p>The application must not attempt to read from the array
     * outside of the specified range.</p>
     *
     * @param chars  characters from the XML document
     * @param start  start position in the array
     * @param length  number of characters to read from the array
     * @exception SAXException
     * @see #characters
     */
    virtual void ignorableWhitespace (const XMLCh* const chars, const unsigned int length);

    /**
     * Receive notification of a processing instruction.
     *
     * <p>The Parser will invoke this method once for each processing
     * instruction found: note that processing instructions may occur
     * before or after the main document element.</p>
     *
     * <p>A SAX parser should never report an XML declaration (XML 1.0,
     * section 2.8) or a text declaration (XML 1.0, section 4.3.1)
     * using this method.</p>
     *
     * @param target pointer to processing instruction target
     * @param data   pointer to processing instruction data, or null if none
     *               was supplied
     * @exception SAXException
     */
    virtual void processingInstruction (const XMLCh* const target, const XMLCh* const data);

    /**
     * Called when a Comment is to be constructed.
     *
     * @param data comment data
     * @exception SAXException
     */
    virtual void comment(const XMLCh* const data);

    /**
     * Receive notification of a entityReference.
     *
     * @param data pointer to characters from the XML document
     * @exception SAXException
     */
    virtual void entityReference(const XMLCh* const data);

    // These methods are inherited DocumentHandler ...
    
    // $$$ Theoretically, shouldn't need javadoc for these, since they are
    // inherited from DocumentHandler, but let's leave them in for now -- JMD

    /**
     * Receive an object for locating the origin of SAX document events.
     *
     * <p>SAX parsers are strongly encouraged (though not absolutely
     * required) to supply a locator: if it does so, it must supply
     * the locator to the application by invoking this method before
     * invoking any of the other methods in the DocumentHandler
     * interface.</p>
     *
     * <p>The locator allows the application to determine the end
     * position of any document-related event, even if the parser is
     * not reporting an error.  Typically, the application will
     * use this information for reporting its own errors (such as
     * character content that does not match an application's
     * business rules).  The information returned by the locator
     * is probably not sufficient for use with a search engine.</p>
     *
     * <p>Note that the locator will return correct information only
     * during the invocation of the events in this interface.  The
     * application should not attempt to use it at any other time.</p>
     *
     * @param locator object that can return the location of
     *                any SAX document event.
     * @see org.xml.sax.Locator
     */
    virtual void setDocumentLocator(const LocatorType* const    locator);

    /**
     * Receive notification of the beginning of a document.
     *
     * <p>The SAX parser will invoke this method only once, before any
     * other methods in this interface or in DTDHandler (except for
     * setDocumentLocator).</p>
     *
     * @exception SAXException
     */
    virtual void startDocument();

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
    virtual void endDocument();
    
    /**
     * Receive notification of the beginning of an element.
     *
     * <p>The Parser will invoke this method at the beginning of every
     * element in the XML document; there will be a corresponding
     * endElement() event for every startElement() event (even when the
     * element is empty). All of the element's content will be
     * reported, in order, before the corresponding endElement()
     * event.</p>
     *
     * <p>If the element name has a namespace prefix, the prefix will
     * still be attached.  Note that the attribute list provided will
     * contain only attributes with explicit values (specified or
     * defaulted): #IMPLIED attributes will be omitted.</p>
     *
     * @param name   element type name
     * @param atts   attributes attached to the element, if any
     * @exception SAXException
     * @see #endElement
     * @see org.xml.sax.AttributeList
     */
    virtual void startElement(const XMLCh* const name, AttributeListType& attrs);
    
    /**
     * Receive notification of the end of an element.
     *
     * <p>The SAX parser will invoke this method at the end of every
     * element in the XML document; there will be a corresponding
     * startElement() event for every endElement() event (even when the
     * element is empty).</p>
     *
     * <p>If the element name has a namespace prefix, the prefix will
     * still be attached to the name.</p>
     *
     * @param name element type name
     * @exception SAXException
     */
    virtual void endElement(const XMLCh* const name);
    
    /**
     * This method allows the user installed Document Handler to 'reset'
     * itself, freeing all the memory resources. The scanner calls this
     * method before starting a new parse event.
     */
    virtual void resetDocument();

protected:

    /** 
     * See if this is a xmlns attribute, and, if so, process it.
     * 
     * @param attrName Qualified name of attribute.
     * @param atts The attribute list where the element comes from (not used at 
     *      this time).
     * @param which The index into the attribute list (not used at this time).
     * @return True if this is a namespace name.
     */
    bool
    isAttrOK(
            const XalanDOMChar*         attrName,
            const AttributeListType&    atts,
            int                         which);

    /** 
     * Tell whether or not this is a xml:space attribute and, if so, process it.
     * 
     * @param elementName The name of the element that owns the attribute
     * @param aname The name of the attribute in question.
     * @param atts The attribute list that owns the attribute.
     * @param which The index of the attribute into the attribute list.
     * @param locator A Locator instance for error reporting.
     * @param fPreserve set to true if the attribute value is "preserve"
     * @return True if this is a xml:space attribute.
     */
    bool
    processSpaceAttr(
            const XalanDOMChar*         elementName,
            const XalanDOMChar*         aname,
            const AttributeListType&    atts,
            int                         which,
            const LocatorType*          locator,
            bool&                       fPreserve);

    /** 
     * Tell whether or not this is a xml:space attribute and, if so, process it.
     * 
     * @param elementName The name of the element that owns the attributes
     * @param atts The attribute list that owns the attribute.
     * @param locator A Locator instance for error reporting.
     * @param fPreserve set to true if an xml:space attribute value is "preserve"
     * @return True if this is a xml:space attribute.
     */
    bool
    processSpaceAttr(
            const XalanDOMChar*         elementName,
            const AttributeListType&    atts,
            const LocatorType*          locator,
            bool&                       fPreserve);

    /**
     * Process xsl:import.
     */
    void
    processImport(
            const XalanDOMChar*         name,
            const AttributeListType&    atts,
            const LocatorType*          locator);

    /**
     * Process xsl:include.
     */
    void
    processInclude(
            const XalanDOMChar*         name,
            const AttributeListType&    atts,
            const LocatorType*          locator);

    void
    doCleanup();

private:

    enum { eElemEmptyAllocatorBlockSize = 10, eElemTextBlockSize = 10 };

    // not implemented
    StylesheetHandler(const StylesheetHandler&);

    StylesheetHandler&
    operator=(const StylesheetHandler&);

    // Utility functions...
    void
    illegalAttributeError(
            const XalanDOMChar*     theElementName,
            const XalanDOMChar*     theAttributeName,
            const LocatorType*      theLocator) const;

    void
    error(
            const XalanDOMChar*     theMessage1,
            const XalanDOMChar*     theMessage2,
            const LocatorType*      theLocator) const;

    void
    error(
            const XalanDOMChar*     theMessage1,
            const XalanDOMString&   theMessage2,
            const LocatorType*      theLocator) const;

    void
    error(
            const XalanDOMString&   theMessage1,
            const XalanDOMChar*     theMessage2,
            const LocatorType*      theLocator) const;

    void
    error(
            const XalanDOMString&   theMessage1,
            const XalanDOMString&   theMessage2,
            const LocatorType*      theLocator) const;

    void
    warn(
            const XalanDOMChar*     theMessage1,
            const XalanDOMChar*     theMessage2,
            const LocatorType*      theLocator) const;

    void
    warn(
            const XalanDOMChar*     theMessage1,
            const XalanDOMString&   theMessage2,
            const LocatorType*      theLocator) const;
    void
    warn(
            const XalanDOMString&   theMessage,
            const LocatorType*      theLocator) const;

    void
    error(
            const XalanDOMString&   theMessage,
            const LocatorType*      theLocator) const;

    void
    processText(
            const XMLCh*                chars,
            XalanDOMString::size_type   length);

    void
    accumulateText(
            const XMLCh*                chars,
            XalanDOMString::size_type   length);

    void
    processAccumulatedText();

    void
    processTopLevelElement(
            const XalanDOMChar*         name,
            const AttributeListType&    atts,
            int                         xslToken,
            const LocatorType*          locator,
            bool&                       fPreserveSpace,
            bool&                       fSpaceAttrProcessed);

    void
    processStylesheet(
            const XalanDOMChar*         name,
            const AttributeListType&    atts,
            const LocatorType*          locator,
            bool&                       fPreserveSpace,
            bool&                       fSpaceAttrProcessed);

    void
    processPreserveStripSpace(
            const XalanDOMChar*         name,
            const AttributeListType&    atts,
            const LocatorType*          locator,
            int                         xslToken);

    void
    appendChildElementToParent(
            ElemTemplateElement*    elem,
            const LocatorType*      locator);

    void
    appendChildElementToParent(
            ElemTemplateElement*    parent,
            ElemTemplateElement*    elem);

    void
    appendChildElementToParent(
            ElemTemplateElement*    parent,
            ElemTemplateElement*    elem,
            const LocatorType*      locator);

    bool
    inExtensionElement() const;

    void
    processExtensionElement(
            const XalanDOMChar*         name,
            const XalanDOMString&       localName,
            const AttributeListType&    atts,
            const LocatorType*          locator);

    void
    checkForOrAddVariableName(
            const XalanQName&   theVariableName,
            const LocatorType*  theLocator);

    // Data members...

    /**
     * The owning stylesheet.
     */
    Stylesheet&     m_stylesheet;

    /**
     * The construction context.
     */
    StylesheetConstructionContext&  m_constructionContext;

    /**
     * An allocator for ElemEmpty instances.
     */
    XalanElemEmptyAllocator     m_elemEmptyAllocator;

    /**
     * An allocator for ElemText instances.
     */
    XalanElemTextAllocator      m_elemTextAllocator;

    /**
     * The stack of elements, pushed and popped as events occur.
     */
    ElemTemplateStackType   m_elemStack;

    /**
     * Need to keep a stack of found whitespace elements so that 
     * whitespace elements next to non-whitespace elements can 
     * be merged.  For instance: &lt;out> &lt;![CDATA[test]]> &lt;/out>
     */
    ElemTextLiteralStackType    m_whiteSpaceElems;

    /**
     * The current template.
     */
    ElemTemplateElement*    m_pTemplate;

    class LastPoppedHolder
    {
    public:

        LastPoppedHolder(StylesheetHandler&     theStylesheetHandler) :
            m_stylesheetHandler(theStylesheetHandler),
            m_lastPopped(0)
        {
        }

        ~LastPoppedHolder()
        {
            cleanup();
        }

        ElemTemplateElement*
        operator->() const
        {
            return m_lastPopped;
        }

        bool
        operator==(ElemTemplateElement*     theRHS)
        {
            return m_lastPopped == theRHS;
        }

        bool
        operator!=(ElemTemplateElement*     theRHS)
        {
            return m_lastPopped != theRHS;
        }

        void
        operator=(ElemTemplateElement*  theRHS)
        {
            if (theRHS != m_lastPopped)
            {
                cleanup();

                m_lastPopped = theRHS;
            }
        }

        void
        swap(LastPoppedHolder&  theOther)
        {
            ElemTemplateElement* const  theTemp = m_lastPopped;

            m_lastPopped = theOther.m_lastPopped;

            theOther.m_lastPopped = theTemp;
        }

        ElemTemplateElement*
        get() const
        {
            return m_lastPopped;
        }

    private:

        void
        set(ElemTemplateElement*    theNewElement)
        {
            if (theNewElement != m_lastPopped)
            {
                cleanup();

                m_lastPopped = theNewElement;
            }
        }

        // Not implemented...
        LastPoppedHolder&
        operator=(const LastPoppedHolder&);

        LastPoppedHolder(const LastPoppedHolder&);

        // Helper functions...
        void
        cleanup();

        // Data members...
        StylesheetHandler&      m_stylesheetHandler;

        ElemTemplateElement*    m_lastPopped;
    };

    friend class LastPoppedHolder;

    /**
     * Manages the last element popped from the stack.
     */
    LastPoppedHolder    m_lastPopped;
    
    /**
     * True if the process is in a template context.
     */
    bool m_inTemplate;
    
    /**
     * True if the stylesheet element was found, or if it was determined that 
     * the stylesheet is wrapperless.
     */
    bool m_foundStylesheet;
    
    /**
     * Flag to let us know when we've found an element inside the 
     * stylesheet that is not an xsl:import, so we can restrict imports 
     * to being the first elements.
     */
    bool m_foundNotImport;

    XalanDOMString      m_elementLocalName;

    /**
     * Accumulate character buffer to create contiguous character data
     * where possible.
     */
    XalanDOMString  m_accumulateText;

    XalanDOMString  m_includeBase;

    BoolStackType   m_inExtensionElementStack;

    BoolStackType   m_preserveSpaceStack;

    // Note that these variables must not be saved by
    // PushPopIncludeState...
    unsigned long   m_locatorsPushed;

    QNameSetType    m_globalVariableNames;

    enum { eVariablesStackDefault = 20 };

    QNameSetVectorType  m_inScopeVariableNamesStack;

    /**
     * Init the wrapperless template
     */
    ElemTemplateElement*
    initWrapperless(
            const XalanDOMChar*         name,
            const AttributeListType&    atts,
            const LocatorType*          locator);

    const XalanDOMString*
    getNamespaceFromStack(const XalanDOMChar*   theName,
                            XalanDOMString&     theBuffer) const;

    const XalanDOMString*
    getNamespaceForPrefixFromStack(const XalanDOMString&    thePrefix) const;

    class PushPopIncludeState;

    friend class StylesheetHandler::PushPopIncludeState;

    class PushPopIncludeState
    {
    public:

        PushPopIncludeState(StylesheetHandler&      theHandler);

        ~PushPopIncludeState();

    private:

        StylesheetHandler&                  m_handler;

        ElemTemplateStackType               m_elemStack;

        ElemTemplateElement* const          m_pTemplate;

        LastPoppedHolder                    m_lastPopped;       

        const bool                          m_inTemplate;       

        const bool                          m_foundStylesheet;

        const XalanDOMString                m_XSLNameSpaceURL;

        const bool                          m_foundNotImport;

        Stylesheet::NamespaceVectorType     m_namespaceDecls;

        Stylesheet::NamespacesStackType     m_namespaces;

        NamespacesHandler                   m_namespacesHandler;

        BoolStackType                       m_inExtensionElementStack;

        BoolStackType                       m_preserveSpaceStack;
    };

    static const XalanDOMString             s_emptyString;
};



XALAN_CPP_NAMESPACE_END



#endif  // XALAN_STYLESHEETHANDLER_HEADER_GUARD
