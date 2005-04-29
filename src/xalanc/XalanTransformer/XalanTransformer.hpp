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
#if !defined(XALANTRANSFORMER_HEADER_GUARD)
#define XALANTRANSFORMER_HEADER_GUARD



// Base include file.  Must be first.
#include <xalanc/XalanTransformer/XalanTransformerDefinitions.hpp>



#include <cassert>
#if defined(XALAN_CLASSIC_IOSTREAMS)
class ostream;
#else
#include <iosfwd>
#endif
#include <algorithm>



#include <xalanc/Include/XalanVector.hpp>



#include <xalanc/XPath/XalanQNameByValue.hpp>



#include <xalanc/XSLT/XSLTInputSource.hpp>
#include <xalanc/XSLT/XSLTResultTarget.hpp>



XALAN_DECLARE_XERCES_CLASS(EntityResolver)
XALAN_DECLARE_XERCES_CLASS(ErrorHandler)



XALAN_CPP_NAMESPACE_BEGIN



typedef XERCES_CPP_NAMESPACE_QUALIFIER EntityResolver   EntityResolverType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER ErrorHandler     ErrorHandlerType;



class Function;
class StylesheetExecutionContextDefault;
class ProblemListener;
class TraceListener;
class XSLTInit;
class XalanDocumentBuilder;
class XalanCompiledStylesheet;
class XalanParsedSource;
class XalanTransformerOutputStream;



/**
 * This is a simple C++ interface for some common usage patterns. It's 
 * the user's responsibility to call initialize and terminate for Xerces 
 * and Xalan before creating and after deleting any XalanTransformer  
 * instances.
*/
class XALAN_TRANSFORMER_EXPORT XalanTransformer
{

public:
    
#if defined(XALAN_NO_STD_NAMESPACE)
    typedef ostream         StreamType;
#else
    typedef std::ostream    StreamType;
#endif

    XalanTransformer(MemoryManagerType& theManager = XalanMemMgrs::getDefaultXercesMemMgr());

    virtual
    ~XalanTransformer();

    MemoryManagerType& 
    getMemoryManager()
    {
        return m_memoryManager;
    }

    const MemoryManagerType& 
    getMemoryManager() const
    {
        return m_memoryManager;
    }

    /**
     * Initialize Xalan.
     *
     * Should be called only once per process before creating any
     * instances of XalanTransformer.  This call is not thread-safe,
     * so you must serialize any calls to it, and you must track the
     * initialization state, so you do not call it more than once.
     */
    static void
    initialize(MemoryManagerType&  theManager = XalanMemMgrs::getDefaultXercesMemMgr());

    /**
     * Terminate Xalan.
     *
     * Should be called only once per process after deleting all
     * instances of XalanTransformer.  This call is not thread-safe,
     * so you must serialize any calls to it, and you must track the
     * initialization state, so you do not call it more than once.
     *
     * This is handy when using leak-detection software, as all
     * static data allocated by Xalan is freed.
     *
     */
    static void
    terminate();

    /**
     * Clean up the ICU, if ICU integration is enabled.
     *
     * This should be called only once per process after calling
     * XalanTransformer::terminate() and XMLPlatformUtils::Terminate.
     *
     * This must only be done if the ICU will no longer be used by
     * the process, since the ICU will no longer be in a usable state.
     * See the ICU documentation for more details.
     *
     * This call is not thread-safe, so you must serialize any calls
     * to it, and you must track the initialization state, so you do
     * not call it more than once.
     *
     * This is handy when using leak-detection software, as all
     * static data allocated by the ICU is freed.
     *
     */
    static void
    ICUCleanUp();


    static void
    ICUStartUp(MemoryManagerType&  theManager = XalanMemMgrs::getDefaultXercesMemMgr());

    /**
     * Transform will apply the stylesheet source to the parsed xml source
     * and write the transformation output to the target. 
     *
     * @param theParsedXML          the parsed input source
     * @param theStylesheetSource   stylesheet source
     * @param theResultTarget       output source
     * @return  0 for success
     */
    int
    transform(
            const XalanParsedSource&    theParsedXML, 
            const XSLTInputSource&      theStylesheetSource,
            const XSLTResultTarget&     theResultTarget)
    {
        return doTransform(theParsedXML, 0, &theStylesheetSource, theResultTarget);
    }

    /**
     * Transform will apply the compiled stylesheet to the parsed xml source
     * and write the transformation output to the target. 
     *
     * @param theParsedXML          the parsed input source
     * @param theCompiledStylesheet pointer to a compiled stylesheet.  Must not be null.
     * @param theResultTarget       output source 
     * @return  0 for success
     */
    int
    transform(
            const XalanParsedSource&        theParsedXML,
            const XalanCompiledStylesheet*  theCompiledStylesheet,
            const XSLTResultTarget&         theResultTarget)
    {
        assert(theCompiledStylesheet != 0);

        return doTransform(theParsedXML, theCompiledStylesheet, 0, theResultTarget);
    }

    /**
     * Transform will apply the stylesheet provided as a PI in the 
     * XML of the parsed xml source and write the transformation output to 
     * the target. 
     *
     * @param theParsedXML      the parsed input source
     * @param theResultTarget   output source tree
     * @return                  0 for success
     */
    int
    transform(
            const XalanParsedSource&    theParsedXML,                
            const XSLTResultTarget&     theResultTarget)
    {
        assert(s_emptyInputSource != 0);

        return transform(theParsedXML, *s_emptyInputSource, theResultTarget);
    }

    /**
     * Transform will apply the stylesheet source to the input source
     * and write the transformation output to the target. The input 
     * source and result target can be a file name, a stream or a root
     * node.
     *
     * @param theInputSource        input source
     * @param theStylesheetSource   stylesheet source
     * @param theResultTarget       output source
     * @return  0 for success
     */
    int
    transform(
            const XSLTInputSource&      theInputSource,
            const XSLTInputSource&      theStylesheetSource,
            const XSLTResultTarget&     theResultTarget);

    /**
     * Transform will apply the stylesheet provided as a PI in the 
     * XML of the input source and write the transformation output to 
     * the target. The input source and result target can be a file 
     * name, a stream or a root node.
     *
     * @param theInputSource        input source
     * @param theResultTarget       output source tree
     * @return  0 for success
     */
    int
    transform(
            const XSLTInputSource&      theInputSource,
            const XSLTResultTarget&     theResultTarget)
    {
        assert(s_emptyInputSource != 0);

        return transform(
                        theInputSource, 
                        *s_emptyInputSource,
                        theResultTarget);
    }

    /**
     * Transform will apply the stylesheet source to the input source
     * and write the transformation result to a callback function  
     * in pre-allocated blocks. The input source can be a file name, 
     * a stream or a root node. Upon termination, Xalan releases any 
     * allocated memory. Data passed to the callback is not guaranteed to 
     * be null terminated.
     *
     * - See XalanTransformerOutputStream and XalanOutputHandlerType 
     * for more details.
     * 
     * @param theInputSource        input source
     * @param theStylesheetSource   stylesheet source   
     * @param theOutputHandle       void pointer passed through to callback.
     * @param theOutputHandler      a user defined (callback) function.
     * @param theFlushHandler       (optional) a user defined (callback) function.
     * @return  0 for success 
     */
    int
    transform(
            const XSLTInputSource&      theInputSource, 
            const XSLTInputSource&      theStylesheetSource,
            void*                       theOutputHandle, 
            XalanOutputHandlerType      theOutputHandler,
            XalanFlushHandlerType       theFlushHandler = 0);
    
    /**
     * Transform will apply the compiled stylesheet to the input source
     * and write the transformation output to the target. The input 
     * source and result target can be a file name, a stream or a root
     * node. 
     *
     * @param theInputSource        input source
     * @param theCompiledStylesheet pointer to a compiled stylesheet.  Must not be null.
     * @param theResultTarget       output source 
     * @return  0 for success
     */
    int
    transform(
            const XSLTInputSource&          theInputSource, 
            const XalanCompiledStylesheet*  theCompiledStylesheet,
            const XSLTResultTarget&         theResultTarget);
 
    /**
     * Transform will apply the compiled stylesheet to the source
     * and write the transformation result to a callback function  
     * in pre-allocated blocks. The input source can be a file name, 
     * a stream or a root node. Upon termination, Xalan releases any 
     * allocated memory. Data passed to the callback is not guaranteed to 
     * be null terminated.
     *
     * - See XalanTransformerOutputStream and XalanOutputHandlerType 
     * for more details.
     * 
     * @param theParsedSource       Parsed source instance
     * @param theCompiledStylesheet pointer to a compiled stylesheet.  Must not be null.
     * @param theOutputHandle       void pointer passed through to callback.
     * @param theOutputHandler      a user defined callback function.
     * @param theFlushHandler       An optional user-defined callback function.
     * @return  0 for success 
     */
    int
    transform(
            const XalanParsedSource&        theParsedSource,
            const XalanCompiledStylesheet*  theCompiledStylesheet,
            void*                           theOutputHandle,
            XalanOutputHandlerType          theOutputHandler,
            XalanFlushHandlerType           theFlushHandler = 0);

    /**
     * Transform will apply the stylesheet provided as a PI in the 
     * XML of the input source and write  the transformation result to a  
     * callback function in pre-allocated blocks. The input source can be 
     * a file name, a stream or a root node. Upon termination, Xalan 
     * releases any allocated memory. Data passed to the callback is not 
     * guaranteed to be null terminated.
     *
     * - See XalanTransformerOutputStream and XalanOutputHandlerType 
     * for more details.
     * 
     * @param theInputSource        input source
     * @param theOutputHandle       void pointer passed through to callback.
     * @param theOutputHandler      a user defined callback function.
     * @param theFlushHandler       an optional user-defined callback function.
     * @return  0 for success 
     */
    int
    transform(
            const XSLTInputSource&      theInputSource,
            void*                       theOutputHandle,
            XalanOutputHandlerType      theOutputHandler,
            XalanFlushHandlerType       theFlushHandler = 0);

    /**
     * Creates a compiled stylesheet.  The input source can be 
     * a file name, a stream or a root node.   The XalanTransformer
     * instance owns the XalanCompiledStylesheet instance and will
     * delete it when the XalanTransformer instance goes out of scope,
     * or you explicitly call destroyStylesheet().  You must not delete
     * the instance yourself.
     *
     * @param theStylesheetSource input source
     * @param theCompiledStylesheet a reference to a pointer to a XalanCompileStylesheet.
     * @return 0 for success 
     */
    int
    compileStylesheet(
            const XSLTInputSource&              theStylesheetSource,
            const XalanCompiledStylesheet*&     theCompiledStylesheet);

    /**
     * Destroy a XalanCompiledStylesheet instance created by a previous
     * call to compileStylesheet().
     *
     * @param theStylesheet The instance to destroy.
     * @return 0 for success 
     */
    int
    destroyStylesheet(const XalanCompiledStylesheet*    theStylesheet);

    /**
     * Parse a source XML document.  The input source can be 
     * a file name, a stream or a root node.  The XalanTransformer
     * instance owns the XalanParsedSource instance and will
     * delete it when the XalanTransformer instance goes out of scope,
     * or you explicitly call destroyParsedSource().  You must not
     * delete the instance yourself.
     *
     * @param theInputSource input source
     * @param theParsedSource a reference to a pointer to a XalanParsedSource.
     * @param useXercesDOM input use default or xerces DOM source tree
     * @return 0 for success 
     */
    int
    parseSource(
            const XSLTInputSource&      theInputSource,
            const XalanParsedSource*&   theParsedSource,
            bool                        useXercesDOM = false);

    /**
     * Destroy a parsed source created by a previous call to parseSource().
     *
     * @param theParsedSource The XalanParsedSource instance to destroy.
     * @return 0 for success 
     */
    int
    destroyParsedSource(const XalanParsedSource*    theParsedSource);

    /**
     * Create a document builder.  Using the document builder, you
     * can construct a document using SAX2 interfaces.  The XalanTransformer
     * instance owns the document builder and will delete it when the
     * XalanTransformer instance goes out of scope, or you explicitly call
     * destroyDocumentBuilder().  You must not delete the instance yourself.
     *
     * @param theURI The base URI for the document instance.
     * @return  a pointer to a XalanDocumentBuilder instance or 0 for failure.
     */
    XalanDocumentBuilder*
    createDocumentBuilder(const XalanDOMString&     theURI = XalanDOMString(XalanMemMgrs::getDummyMemMgr()));

    /**
     * Destroy a document builder created by a previous call to createDocumentBuilder().
     * Passing a pointer that is not created by a call to createDocumentBuilder() can
     * result in undefined behavior.
     *
     * @param theDocumentBuilder The document builder to destroy.
     */
    void
    destroyDocumentBuilder(XalanDocumentBuilder*    theDocumentBuilder);

    /**
     * Install an external function in the local space.
     *
     * @param theNamespace The namespace for the functionl
     * @param functionName The name of the function.
     * @param function The function to install.
     */
    void
    installExternalFunction(
            const XalanDOMString&   theNamespace,
            const XalanDOMString&   functionName,
            const Function&         function);

    /**
     * Install an external function in the global space.
     *
     * @param theNamespace The namespace for the functionl
     * @param functionName The name of the function.
     * @param function The function to install.
     */
    static void
    installExternalFunctionGlobal(
            const XalanDOMString&   theNamespace,
            const XalanDOMString&   functionName,
            const Function&         function);

    /**
     * Uninstall an external local function.
     *
     * @param theNamespace The namespace for the function
     * @param functionName The name of the function.
     */
    void
    uninstallExternalFunction(
            const XalanDOMString&   theNamespace,
            const XalanDOMString&   functionName);

    /**
     * Uninstall an external global function.
     *
     * @param theNamespace The namespace for the function
     * @param functionName The name of the function.
     */
    static void
    uninstallExternalFunctionGlobal(
            const XalanDOMString&   theNamespace,
            const XalanDOMString&   functionName);

    /**
     * Set a top-level stylesheet parameter.  This value can be evaluated via
     * xsl:param-variable.  These values are cleared after a call to transform().
     *
     * @param key name of the param
     * @param expression expression that will be evaluated
     */
    void
    setStylesheetParam(
            const XalanDOMString&   key,
            const XalanDOMString&   expression);

    /**
     * Set a top-level stylesheet parameter.  This value can be evaluated via
     * xsl:param-variable.  These values are cleared after a call to transform().
     *
     * @param key name of the param
     * @param expression expression that will be evaluated
     */
    void
    setStylesheetParam(
            const char*     key,
            const char*     expression);

    /**
     * Add a TraceListener instance.  TraceListeners instances are preserved
     * between calls to transform(), so they will be called until they are
     * removed.
     *
     * @param theTraceListener The instance to add.
     */
    void
    addTraceListener(TraceListener*     theTraceListener)
    {
        m_traceListeners.push_back(theTraceListener);
    }

    /**
     * Remove a TraceListener instance
     *
     * @param theTraceListener The instance to remove.
     * @return true if the instance was removed, false if not.
     */
    bool
    removeTraceListener(TraceListener*  theTraceListener);

    /**
     * Remove all TraceListener instances.
     */
    void
    removeTraceListeners()
    {
        m_traceListeners.clear();
    }

    /**
     * Set a flag to indicate whether or not the source file(s) for the
     * transformation will be validated.
     *
     * @param fValue the new value for the flag.
     */
    bool
    getUseValidation() const
    {
        return m_useValidation;
    }

    /**
     * Set a flag to indicate whether or not the source file(s) for the
     * transformation will be validated.
     *
     * @param fValue the new value for the flag.
     */
    void
    setUseValidation(bool   fValue)
    {
        m_useValidation = fValue;
    }

    /**
     * This method returns the installed entity resolver.
     *
     * @return The pointer to the installed entity resolver object.
     */
    EntityResolverType*
    getEntityResolver() const
    {
        return m_entityResolver;
    }

    /**
     * This method installs the user-specified entity resolver.
     * It allows applications to trap and redirect calls to
     * external entities.
     *
     * @param handler A pointer to the entity resolver to be called
     *             when the parser comes across references to
     *             entities in the XML file.
     */
    void
    setEntityResolver(EntityResolverType*   theResolver)
    {
        m_entityResolver = theResolver;
    }

    /**
     * This method returns the installed error handler.
     *
     * @return The pointer to the installed error handler object.
     */
    ErrorHandlerType*
    getErrorHandler() const
    {
        return m_errorHandler;
    }

    /**
     * This method installs the user-specified error handler.
     *
     * @param handler A pointer to the error handler to be called upon error.
     */
    void
    setErrorHandler(ErrorHandlerType*   theErrorHandler)
    {
        m_errorHandler = theErrorHandler;
    }

    /**
      * This method returns the location for an external schema document
      * for parsing.
      *
      * @return A string representing the location of the external schema document
      */
    const XalanDOMChar*
    getExternalSchemaLocation() const
    {
        return m_externalSchemaLocation.empty() == true ? 0 : m_externalSchemaLocation.c_str();
    }

    /**
      * This method sets the location for an external schema document
      * for parsing.
      *
      * @param location A string representing the location of the external schema document
      */
    void
    setExternalSchemaLocation(const XalanDOMChar*   location)
    {
        assert(location != 0);

        m_externalSchemaLocation = location;
    }

    /**
      * This method returns the location for an external schema document
      * for parsing.
      *
      * @return A string representing the location of the external schema document
      */
    const XalanDOMChar*
    getExternalNoNamespaceSchemaLocation() const
    {
        return m_externalNoNamespaceSchemaLocation.empty() == true ? 0 : m_externalNoNamespaceSchemaLocation.c_str();
    }

    /**
      * This method sets the location for an external schema document
      * for parsing.
      *
      * @param location A string representing the location of the external schema document
      */
    void
    setExternalNoNamespaceSchemaLocation(const XalanDOMChar*    location)
    {
        assert(location != 0);

        m_externalNoNamespaceSchemaLocation = location;
    }

    /**
      * This member functions gets the flag to determine of a default parsed source
      * tree will pool the values of all of the text nodes in the XML document.
      *
      * @return The boolean value for the flag.
      */
    bool
    getPoolAllTextNodes() const
    {
        return m_poolAllTextNodes;
    }

    /**
      * This member functions sets the flag to determine of a default parsed source
      * tree will pool the values of all of the text nodes in the XML document.  This
      * can result in significant memory savings if there are lots of repeated values
      * in text nodes.
      *
      * @param fPool The boolean value for the flag.
      */
    void
    setPoolAllTextNodes(bool    fPool)
    {
        m_poolAllTextNodes = fPool;
    }

    /**
     * This method returns the installed ProblemListener instance.
     *
     * @return The pointer to the installed ProblemListener instance.
     */
    ProblemListener*
    getProblemListener() const
    {
        return  m_problemListener;
    }

    /**
     * This method installs the user-specified ProblemListener instance.
     *
     * @param handler A pointer to the ProblemListener to be called when a problem occurs.
     */
    void
    setProblemListener(ProblemListener*     theProblemListener)
    {
        m_problemListener = theProblemListener;
    }

    /**
     * Returns the last error that occurred as a 
     * result of calling transform. 
     *
     * @return  error message const character pointer.
     */
    const char*
    getLastError() const
    {
        assert(m_errorMessage.empty() == false);

        return &m_errorMessage[0]; 
    }

    /**
     * Get the current output encoding, if any.  Note this is not the output encoding
     * specified in a stylesheet.  Rather, it can override that encoding, if the
     *
     * @return A string containing the current encoding
     */
    const XalanDOMString&
    getOutputEncoding() const
    {
        return m_outputEncoding;
    }

    /**
     * Set the current output encoding.  Note this will override the encoding
     * specified in the stylesheet, if the encoding is available.  However,
     * it will not override any encoding specified by an XSLTResultTarget
     * parameter to one of the transform() member functions.
     *
     * @param theEncoding The encoding to use.
     */
    void
    setOutputEncoding(const XalanDOMString&     theEncoding)
    {
        m_outputEncoding = theEncoding;
    }

    /**
     * Get the current number of spaces to indent.
     * 
     * @return number of spaces
     */
    int
    getIndent() const;

    /**
     * Set the current number of spaces to indent.
     * 
     * @param indentAmount The number of spaces to indent.  Use -1 for the default amount.
     */
    void
    setIndent(int   indentAmount);

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
    eEscapeURLs
    getEscapeURLs() const;

    /**
     * Set the value for run-time escaping of URLs.  This can
     * override the property specified by the stylesheet.  The
     * default behavior is to honor the property in the stylesheet.
     *
     * @param value The value of the enum
     */
    void
    setEscapeURLs(eEscapeURLs   value);

    /**
     * Enums to determine whether or not run-time omission of the META tag has been set.
     * Relevant only for HTML output
     */
    enum eOmitMETATag
    {
        eOmitMETATagDefault,    // Use the value in the stylesheet
        eOmitMETATagNo,         // Don't omit the META tag
        eOmitMETATagYes         // Omit the META tag
    };

    /**
     * Get the value for run-time omission of the META tag.  This can
     * override the property specified by the stylesheet.  The
     * default behavior is to honor the property in the stylesheet.
     *
     * @return The value of the enum
     */
    eOmitMETATag
    getOmitMETATag() const;

    /**
     * Get the value for run-time omission of the META tag.  This can
     * override the property specified by the stylesheet.  The
     * default behavior is to honor the property in the stylesheet.
     *
     * @param value The value of the enum
     */
    void
    setOmitMETATag(eOmitMETATag     value);

    /**
     * Set the ostream instance for reporting warnings and messages.  The default
     * is std::cerr.  If set to 0, no warnings or messages will be written.  If
     * there is a user-installed ProblemListener instance, this is not used, as
     * all such warnings and messages are handled by the user's ProblemListener.
     *
     * @param theStream A pointer to the ostream instance.
     */
    void
    setWarningStream(StreamType*    theStream)
    {
        m_warningStream = theStream;
    }

    /**
     * Get the current warning stream ostream instance.
     *
     * @return A pointer to the instance.  Can be 0.
     */
    StreamType*
    getWarningStream() const
    {
        return m_warningStream;
    }

    typedef XalanVector<const XalanCompiledStylesheet*> CompiledStylesheetPtrVectorType;
    typedef XalanVector<const XalanParsedSource*>       ParsedSourcePtrVectorType;
    typedef XALAN_STD_QUALIFIER pair<XalanDOMString*, XalanDOMString*>      ParamPairType;
    typedef XalanVector<ParamPairType>                  ParamPairVectorType;
    typedef XALAN_STD_QUALIFIER pair<XalanQNameByValue*, Function*>         FunctionPairType;
    typedef XalanVector<FunctionPairType>               FunctionParamPairVectorType;
    typedef XalanVector<TraceListener*>                 TraceListenerVectorType;

    class EnsureFunctionsInstallation
    {
    public:
        
        EnsureFunctionsInstallation(MemoryManagerType& theManager) : 
          m_memoryManagement(theManager),
          m_release(false)
        {    
        }

        ~EnsureFunctionsInstallation();

        void
        install();

        void
        release()
        {
            m_release = true;
        }

    private:
        MemoryManagerType& m_memoryManagement;

        bool m_release;
    };

    class EnsureDestroyParsedSource
    {
    public:

        EnsureDestroyParsedSource(
                XalanTransformer&           theTransformer,
                const XalanParsedSource*    theParsedSource) :
            m_transformer(theTransformer),
            m_parsedSource(theParsedSource)
        {
        }

        ~EnsureDestroyParsedSource()
        {
            m_transformer.destroyParsedSource(m_parsedSource);
        }

    private:

        XalanTransformer&               m_transformer;

        const XalanParsedSource* const  m_parsedSource;
    };

    struct EnsureDestroyCompiledStylesheet
    {
        EnsureDestroyCompiledStylesheet(
                XalanTransformer&               theTransformer,
                const XalanCompiledStylesheet*  theCompiledStylesheet) :
            m_transformer(theTransformer),
            m_compiledStylesheet(theCompiledStylesheet)
        {
        }

        ~EnsureDestroyCompiledStylesheet()
        {
            m_transformer.destroyStylesheet(m_compiledStylesheet);
        }

    private:

        XalanTransformer&                       m_transformer;

        const XalanCompiledStylesheet* const    m_compiledStylesheet;
    };

    struct EnsureDestroyDocumentBuilder
    {
        EnsureDestroyDocumentBuilder(
                XalanTransformer&       theTransformer,
                XalanDocumentBuilder*   theDocumentBuilder) :
            m_transformer(theTransformer),
            m_documentBuilder(theDocumentBuilder)
        {
        }

        ~EnsureDestroyDocumentBuilder()
        {
            m_transformer.destroyDocumentBuilder(m_documentBuilder);
        }

    private:

        XalanTransformer&           m_transformer;

        XalanDocumentBuilder* const m_documentBuilder;
    };
    template <class T>
#if defined(XALAN_NO_STD_NAMESPACE)
    struct DeleteParamPairFunctor : public unary_function<T&, void>
#else
    struct DeleteParamPairFunctor : public std::unary_function<T&, void>
#endif
    {
#if defined(XALAN_NO_STD_NAMESPACE)
        typedef unary_function<T&, void>    BaseClassType;
#else
        typedef std::unary_function<T&, void>   BaseClassType;
#endif

        typedef typename BaseClassType::result_type     result_type;
        typedef typename BaseClassType::argument_type   argument_type;

        DeleteParamPairFunctor(MemoryManagerType&      theManager) :
        m_memoryManager(theManager)
        {
        }

        template<class PtrT>
    void
        deletePtr(PtrT* ptr)const
        {
            ptr->~PtrT();
        }

        /**
        * Delete the object pointed to by argument.
        *
        * @param thePointer pointer to object to be deleted
        */
        result_type
            operator()(argument_type    thePair) const
        {
            T&  tmpPair = const_cast<T&>(thePair);

            if (tmpPair.first != 0)
            {
                deletePtr(tmpPair.first);
                
                m_memoryManager.deallocate((void*)tmpPair.first);
            }

            if (tmpPair.second != 0)
            {
                deletePtr(tmpPair.second);
                
                m_memoryManager.deallocate((void*)tmpPair.second);
            }            

            
        }

    private:

        MemoryManagerType& m_memoryManager;

    };
protected:

private:

    void
    reset();

    class EnsureReset
    {
    public:

        EnsureReset(XalanTransformer&   theTransformer) :
            m_transformer(theTransformer)
        {
        }

        ~EnsureReset();

    private:

        XalanTransformer&   m_transformer;
    };

    friend class EnsureReset;

    int
    doTransform(
            const XalanParsedSource&        theParsedXML, 
            const XalanCompiledStylesheet*  theCompiledStylesheet,
            const XSLTInputSource*          theStylesheetSource,
            const XSLTResultTarget&         theResultTarget);


    // Data members...
    MemoryManagerType&                      m_memoryManager;

    CompiledStylesheetPtrVectorType         m_compiledStylesheets;

    ParsedSourcePtrVectorType               m_parsedSources;

    ParamPairVectorType                     m_paramPairs;

    FunctionParamPairVectorType             m_functionPairs;

    TraceListenerVectorType                 m_traceListeners;

    CharVectorType                          m_errorMessage;

    bool                                    m_useValidation;

    EntityResolverType*                     m_entityResolver;

    ErrorHandlerType*                       m_errorHandler;

    XalanDOMString                          m_externalSchemaLocation;

    XalanDOMString                          m_externalNoNamespaceSchemaLocation;

    ProblemListener*                        m_problemListener;

    StreamType*                             m_warningStream;

    XalanDOMString                          m_outputEncoding;

    bool                                    m_poolAllTextNodes;

    // This should always be the latest data member!!!
    StylesheetExecutionContextDefault*      m_stylesheetExecutionContext;

    static const XSLTInputSource*           s_emptyInputSource;

    static const XSLTInit*                  s_xsltInit;
};



XALAN_CPP_NAMESPACE_END



#endif  // XALANTRANSFORMER_HEADER_GUARD
