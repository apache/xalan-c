/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the  "License");
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
#if !defined(XALANSOURCETREEPARSERLIAISON_HEADER_GUARD_1357924680)
#define XALANSOURCETREEPARSERLIAISON_HEADER_GUARD_1357924680


// Base include file.  Must be first.
#include <xalanc/XalanSourceTree/XalanSourceTreeDefinitions.hpp>



#include <xalanc/Include/XalanMap.hpp>



#include <xalanc/XercesParserLiaison/XercesDOMSupport.hpp>
#include <xalanc/XercesParserLiaison/XercesParserLiaison.hpp>



XALAN_DECLARE_XERCES_CLASS(ContentHandler)
XALAN_DECLARE_XERCES_CLASS(DTDHandler)
XALAN_DECLARE_XERCES_CLASS(LexicalHandler)
XALAN_DECLARE_XERCES_CLASS(SAX2XMLReaderImpl)



XALAN_CPP_NAMESPACE_BEGIN



class XalanSourceTreeDOMSupport;
class XalanSourceTreeDocument;



/*
typedef XERCES_CPP_NAMESPACE_QUALIFIER ContentHandler       ContentHandlerType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DTDHandler           DTDHandlerType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER LexicalHandler       LexicalHandlerType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER SAX2XMLReader        SAX2XMLReaderType;
*/

XALAN_USING_XERCES(ContentHandler)
XALAN_USING_XERCES(DTDHandler)
XALAN_USING_XERCES(LexicalHandler)
XALAN_USING_XERCES(SAX2XMLReaderImpl)


class XALAN_XALANSOURCETREE_EXPORT  XalanSourceTreeParserLiaison : public XMLParserLiaison
{
    
public:

    /**
     * Construct a XalanSourceTreeParserLiaison instance.
     *
     * @param theSupport instance of DOMSupport object
     *
     * @deprecated This constructor is deprecated.  Use the next constructor instead.
     */
    XalanSourceTreeParserLiaison(
                XalanSourceTreeDOMSupport&  theSupport, 
                MemoryManager&          theManager XALAN_DEFAULT_MEMMGR);

    /**
     * Construct a XalanSourceTreeParserLiaison instance.
     */
    XalanSourceTreeParserLiaison(MemoryManager& theManager XALAN_DEFAULT_MEMMGR);

    MemoryManager&
    getMemoryManager()
    {
        return m_documentMap.getMemoryManager();
    }

    virtual
    ~XalanSourceTreeParserLiaison();

    /**
     * Get the value of the flag which determines if the data of all
     * text nodes are pooled, or just whitespace text nodes.
     *
     * @return true if the data of all text nodes are pooled, false otherwise.
     */
    bool
    getPoolAllText() const
    {
        return m_poolAllText;
    }

    /**
     * Set the value of the flag which determines if the data of all
     * text nodes are pooled, or just whitespace text nodes.
     *
     * @param fValue The new value for the flag.
     */
    void
    setPoolAllText(bool fValue)
    {
        m_poolAllText = fValue;
    }

    // These interfaces are inherited from XMLParserLiaison...

    virtual void
    reset();

    virtual ExecutionContext*
    getExecutionContext() const;

    virtual void
    setExecutionContext(ExecutionContext&   theContext);

    virtual XalanDocument*
    parseXMLStream(
            const InputSource&      reader,
            const XalanDOMString&   identifier = XalanDOMString(XalanMemMgrs::getDummyMemMgr()));

    virtual void
    parseXMLStream(
            const InputSource&      inputSource,
            DocumentHandler&        handler,
            const XalanDOMString&   identifier = XalanDOMString(XalanMemMgrs::getDummyMemMgr()));

    virtual void
    destroyDocument(XalanDocument*  theDocument);

    virtual int
    getIndent() const;

    virtual void
    setIndent(int   i);

    virtual bool
    getUseValidation() const;

    virtual void
    setUseValidation(bool   b);

    virtual const XalanDOMString&
    getParserDescription(XalanDOMString&    theResult) const;

    virtual EntityResolver*
    getEntityResolver() const;

    virtual void
    setEntityResolver(EntityResolver*   resolver);

    virtual XMLEntityResolver*
    getXMLEntityResolver() const;

    virtual void
    setXMLEntityResolver(XMLEntityResolver*     resolver);

    virtual ErrorHandler*
    getErrorHandler() const;

    virtual void
    setErrorHandler(ErrorHandler*   handler);


    // These interfaces are new to XalanSourceTreeParserLiaison...

    /**
     * Parse using a SAX2 ContentHandler, DTDHandler, and LexicalHandler.
     *
     * @param theInputSource The input source for the parser
     * @param theContentHandler The ContentHandler to use
     * @param theDTDHandler The DTDHandler to use.  May be null.
     * @param theLexicalHandler The LexicalHandler to use.  May be null.
     * @param identifier Used for error reporting only.
     */
    virtual void
    parseXMLStream(
            const InputSource&      theInputSource,
            ContentHandler&         theContentHandler,
            const XalanDOMString&   theIdentifier,
            DTDHandler*             theDTDHandler = 0,
            LexicalHandler*         theLexicalHandler = 0);

    virtual DOMDocument_Type*
    createDOMFactory();

    virtual void
    destroyDocument(DOMDocument_Type*   theDocument);

    /** Get the 'include ignorable whitespace' flag.
      *
      * This method returns the state of the parser's include ignorable
      * whitespace flag.
      *
      * @return 'true' if the include ignorable whitespace flag is set on
      *         the parser, 'false' otherwise.
      *
      * @see #setIncludeIgnorableWhitespace
      */
    virtual bool
    getIncludeIgnorableWhitespace() const;

    /** Set the 'include ignorable whitespace' flag
      *
      * This method allows the user to specify whether a validating parser
      * should include ignorable whitespaces as text nodes.  It has no effect
      * on non-validating parsers which always include non-markup text.
      * <p>When set to true (also the default), ignorable whitespaces will be
      * added to the DOM tree as text nodes.  The method
      * DOM_Text::isWhitespace() will return true for those text
      * nodes only.
      * <p>When set to false, all ignorable whitespace will be discarded and
      * no text node is added to the DOM tree.  Note: applications intended
      * to process the "xml:space" attribute should not set this flag to false.
      *
      * @param include The new state of the include ignorable whitespace
      *                flag.
      *
      * @see #getIncludeIgnorableWhitespace
      */
    virtual void
    setIncludeIgnorableWhitespace(bool  include);

    /**
      * This method returns the state of the parser's namespace
      * handling capability.
      *
      * @return true, if the parser is currently configured to
      *         understand namespaces, false otherwise.
      *
      * @see #setDoNamespaces
      */
    virtual bool
    getDoNamespaces() const;

    /**
      * This method allows users to enable or disable the parser's
      * namespace processing. When set to true, parser starts enforcing
      * all the constraints / rules specified by the NameSpace
      * specification.
      *
      * <p>The parser's default state is: false.</p>
      *
      * <p>This flag is ignored by the underlying scanner if the installed
      * validator indicates that namespace constraints should be
      * enforced.</p>
      *
      * @param newState The value specifying whether NameSpace rules should
      *                 be enforced or not.
      *
      * @see #getDoNamespaces
      */
    virtual void
    setDoNamespaces(bool    newState);

    /**
      * This method returns the state of the parser's
      * exit-on-First-Fatal-Error flag.
      *
      * @return true, if the parser is currently configured to
      *         exit on the first fatal error, false otherwise.
      *
      * @see #setExitOnFirstFatalError
      */
    virtual bool
    getExitOnFirstFatalError() const;

    /**
      * This method allows users to set the parser's behaviour when it
      * encounters the first fatal error. If set to true, the parser
      * will exit at the first fatal error. If false, then it will
      * report the error and continue processing.
      *
      * <p>The default value is 'true' and the parser exits on the
      * first fatal error.</p>
      *
      * @param newState The value specifying whether the parser should
      *                 continue or exit when it encounters the first
      *                 fatal error.
      *
      * @see #getExitOnFirstFatalError
      */
    virtual void
    setExitOnFirstFatalError(bool   newState);

    /**
      * This method returns the location for an external schema document
      * for parsing.
      *
      * @return A string representing the location of the external schema document
      */
    virtual const XalanDOMChar*
    getExternalSchemaLocation() const;

    /**
      * This method sets the location for an external schema document
      * for parsing.
      *
      * @param location A string representing the location of the external schema document
      */
    virtual void
    setExternalSchemaLocation(const XalanDOMChar*   location);

    /**
      * This method returns the location for an external schema document
      * for parsing.
      *
      * @return A string representing the location of the external schema document
      */
    virtual const XalanDOMChar*
    getExternalNoNamespaceSchemaLocation() const;

    /**
      * This method sets the location for an external schema document
      * for parsing.
      *
      * @param location A string representing the location of the external schema document
      */
    virtual void
    setExternalNoNamespaceSchemaLocation(const XalanDOMChar*    location);

    /** 
     * Map a pointer to a XalanDocument instance to its implementation
     * class pointer.  Normally, you should have no reason for doing
     * this.  The liaison will return a null pointer if it did not
     * create the instance passed.
     *
     * @param theDocument A pointer to a XalanDocument instance.
     * @return A pointer to the XalanSourceTreeDocument instance.
     */
    XalanSourceTreeDocument*
    mapDocument(const XalanDocument*    theDocument) const;

    /** 
     * Create a XalanSourceTreeDocument instance.
     *
     * @return A pointer to the XalanSourceTreeDocument instance.
     */
    XalanSourceTreeDocument*
    createXalanSourceTreeDocument();

    typedef XalanMap<const XalanDocument*,
                     XalanSourceTreeDocument*>  DocumentMapType;


protected:

    virtual SAX2XMLReaderImpl*
    createReader();

private:

    void
    ensureReader();


    // Not implemented...
    XalanSourceTreeParserLiaison(const XalanSourceTreeParserLiaison&);

    XalanSourceTreeParserLiaison&
    operator=(const XalanSourceTreeParserLiaison&);


    // Data members...
    XercesParserLiaison         m_xercesParserLiaison;

    DocumentMapType             m_documentMap;

    bool                        m_poolAllText;

    SAX2XMLReaderImpl*          m_xmlReader;
};



XALAN_CPP_NAMESPACE_END



#endif  // XALANSOURCETREEPARSERLIAISON_HEADER_GUARD_1357924680
