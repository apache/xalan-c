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
#if !defined(XMLPARSERLIAISON_HEADER_GUARD_1357924680)
#define XMLPARSERLIAISON_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XMLSupport/XMLSupportDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



XALAN_DECLARE_XERCES_CLASS(DocumentHandler)
XALAN_DECLARE_XERCES_CLASS(EntityResolver)
XALAN_DECLARE_XERCES_CLASS(ErrorHandler)
XALAN_DECLARE_XERCES_CLASS(InputSource)
XALAN_DECLARE_XERCES_CLASS(XMLEntityResolver)



XALAN_CPP_NAMESPACE_BEGIN



typedef XERCES_CPP_NAMESPACE_QUALIFIER DocumentHandler  DocumentHandlerType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER EntityResolver   EntityResolverType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER ErrorHandler     ErrorHandlerType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER InputSource      InputSourceType;

XALAN_USING_XERCES(DocumentHandler)
XALAN_USING_XERCES(EntityResolver)
XALAN_USING_XERCES(ErrorHandler)
XALAN_USING_XERCES(InputSource)
XALAN_USING_XERCES(XMLEntityResolver)



class ExecutionContext;
class FormatterListener;
class XalanAttr;
class XalanDocument;
class XalanElement;



class XALAN_XMLSUPPORT_EXPORT XMLParserLiaison
{
    
public:

    XMLParserLiaison();

    virtual
    ~XMLParserLiaison();


    // These interfaces are new to XMLParserLiaison

    /**
     * Reset the instance, freeing any XalanDocument instances created
     * through parseXMLStream().
     */
    virtual void
    reset() = 0;

    /**
     * Get a pointer to the current ExecutionContext instance, which
     * may be null.
     *
     * @return A pointer to the current ExecutionContext, if any.
     */
    virtual ExecutionContext*
    getExecutionContext() const = 0;

    /**
     * Get a reference to the current MemoryManager instance.
     *
     * @return A pointer to the current ExecutionContext, if any.
     */
    virtual MemoryManager&
    getMemoryManager() = 0;

    /**
     * Set the current ExecutionContext instance.
     *
     * @parameter theContext A reference to the new ExecutionContext instance.
     */
    virtual void
    setExecutionContext(ExecutionContext&   theContext) = 0;

    /**
     * Parse the text pointed at by the reader as XML, and return a DOM
     * Document interface.  It is recommended that you pass in some sort of
     * recognizable name, such as the filename or URI, with which the reader
     * can be recognized if the parse fails.
     *
     * The liaison owns the XalanDocument instance, and will delete it when
     * asked (see DestroyDocument()), or when the liaison is reset, or goes
     * out of scope.
     *
     * This function is not reentrant, so you cannot call it again until
     * the current call exits.
     *
     * @param reader     stream that should hold valid XML
     * @param identifier used for diagnostic purposes only, some sort of
     *                   identification for error reporting, default an empty
     *                   string
     * @return DOM document created
     */
    virtual XalanDocument*
    parseXMLStream(
            const InputSource&      inputSource,
            const XalanDOMString&   identifier) = 0;

    /**
     * Parse the text pointed at by the reader as XML. It is recommended that
     * you pass in some sort of recognizable name, such as the filename or URI,
     * with which the reader can be recognized if the parse fails.
     *
     * This function is reentrant, so you can call it again before any
     * other call exits.  However, it is not thread-safe.
     *
     * @param inputSource input source that should hold valid XML
     * @param handler        instance of a DocumentHandler
     * @param identifier     used for diagnostic purposes only, some sort of
     *                       identification for error reporting, default an
     *                       empty string
     */
    virtual void
    parseXMLStream(
            const InputSource&      inputSource,
            DocumentHandler&        handler,
            const XalanDOMString&   identifier) = 0;

    /**
     * Destroy the supplied XalanDocument instance.  It must be an instance that
     * was created by a previous call to parseXMLStream().
     *
     * @param theDocument The XalanDocument instance to destroy.
     */
    virtual void
    destroyDocument(XalanDocument*  theDocument) = 0;

    /**
     * Get the amount to indent when indent-result="yes".
     *
     * @deprecated
     *
     * @return number of characters to indent
     */
    virtual int
    getIndent() const = 0;

    /**
     * Set the amount to indent when indent-result="yes".
     *
     * @deprecated
     *
     * @param i number of characters to indent
     */
    virtual void
    setIndent(int   i) = 0;

    /**
     * Get whether or not validation will be performed.  Validation is off by
     * default.
     *
     * @return true to perform validation
     */
    virtual bool
    getUseValidation() const = 0;

    /**
     * If set to true, validation will be performed.  Validation is off by
     * default.
     *
     * @param b true to perform validation
     */
    virtual void
    setUseValidation(bool   b) = 0;

    /**
     * Return a string suitable for telling the user what parser is being used.
     *
     * @return string describing parser
     */
    virtual const XalanDOMString&
    getParserDescription(XalanDOMString& theResult) const = 0;

    /**
      * This method returns the installed EntityResolver.
      *
      * @return The pointer to the installed EntityResolver object.
      */
    virtual EntityResolver*
    getEntityResolver() const = 0;

    /**
      * This method installs the user-specified EntityResolver on the
      * parser. It allows applications to trap and redirect calls to
      * external entities.
      *
      * A call to setEntityResolver with a non-null pointer will
      * uninstall any XMLEntityResolver previously installed.
      *
      * @param handler A pointer to the EntityResolver to be called
      *                when the parser encounters references to
      *                external entities.
      */
    virtual void
    setEntityResolver(EntityResolver*   resolver) = 0;

    /**
      * This method returns the installed XMLEntityResolver.
      *
      * @return The pointer to the installed XMLEntityResolver object.
      */
    virtual XMLEntityResolver*
    getXMLEntityResolver() const = 0;

    /**
      * This method installs the user-specified XMLEntityResolver on the
      * parser. It allows applications to trap and redirect calls to
      * external entities.
      *
      * A call to setXMLEntityResolver with a non-null pointer will
      * uninstall any EntityResolver previously installed.
      *
      * @param handler A pointer to the entity resolver to be called
      *                when the parser encounters references to
      *                external entities.
      */
    virtual void
    setXMLEntityResolver(XMLEntityResolver*     resolver) = 0;

    /**
      * This method returns the installed error handler.
      *
      * @return The pointer to the installed error handler object.
      */
    virtual ErrorHandler*
    getErrorHandler() const = 0;

    /**
      * This method installs the user-specified error handler.
      *
      * @param handler A pointer to the error handler to be called upon error.
      */
    virtual void
    setErrorHandler(ErrorHandler*   handler) = 0;

    // A utility class for classes to use.
    class EnsureResetErrorHandler
    {
    public:

        EnsureResetErrorHandler(
                XMLParserLiaison*   theLiaison = 0,
                ErrorHandler*       theErrorHandler = 0) :
            m_liaison(theLiaison),
            m_errorHandler(theErrorHandler)
        {
            if (theLiaison != 0)
            {
                theLiaison->setErrorHandler(theErrorHandler);
            }
        }

        ~EnsureResetErrorHandler()
        {
            if (m_liaison != 0)
            {
                m_liaison->setErrorHandler(m_errorHandler);
            }
        }

        void
        set(
                XMLParserLiaison*   theLiaison,
                ErrorHandler*       theErrorHandler)
        {
            m_liaison = theLiaison;

            m_errorHandler = theErrorHandler;

            if (theLiaison != 0)
            {
                theLiaison->setErrorHandler(theErrorHandler);
            }
        }

    private:

        XMLParserLiaison*   m_liaison;

        ErrorHandler*       m_errorHandler;
    };

protected:

    // A utility class for derived classes to use.
    class EnsureDestroyDocument
    {
    public:

        EnsureDestroyDocument(
                XMLParserLiaison&   theLiaison,
                XalanDocument*      theDocument) :
            m_liaison(theLiaison),
            m_document(theDocument)
        {
        }

        ~EnsureDestroyDocument()
        {
            if (m_document != 0)
            {
                m_liaison.destroyDocument(m_document);
            }
        }

        XalanDocument*
        get() const
        {
            return m_document;
        }

        XalanDocument*
        release()
        {
            XalanDocument*  theDocument = m_document;

            m_document = 0;

            return theDocument;
        }

    private:

        XMLParserLiaison&   m_liaison;

        XalanDocument*      m_document;
    };

private:

    // Not implemented
    XMLParserLiaison(const XMLParserLiaison&);

    XMLParserLiaison&
    operator=(const XMLParserLiaison&);
};



XALAN_CPP_NAMESPACE_END



#endif  // XMLPARSERLIAISON_HEADER_GUARD_1357924680
