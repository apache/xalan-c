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
#if !defined(XercesPARSERLIAISON_HEADER_GUARD_1357924680)
#define XercesPARSERLIAISON_HEADER_GUARD_1357924680


// Base include file.  Must be first.
#include <xalanc/XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



// Standard Library header files.
#include <xalanc/Include/XalanMap.hpp>



// Xerces DOM header files
#include <xercesc/sax/ErrorHandler.hpp>



// Base class header file.
#include <xalanc/XMLSupport/XMLParserLiaison.hpp>



#if defined(XALAN_BUILD_DEPRECATED_DOM_BRIDGE)
#include <xalanc/XercesParserLiaison/Deprecated/XercesBridgeTypes.hpp>
#endif
#include <xalanc/XercesParserLiaison/XercesWrapperTypes.hpp>



#if XERCES_VERSION_MAJOR >= 2
XALAN_DECLARE_XERCES_CLASS(XercesDOMParser)
#else
XALAN_DECLARE_XERCES_CLASS(DOMParser)
#endif
XALAN_DECLARE_XERCES_CLASS(SAXParser)



XALAN_CPP_NAMESPACE_BEGIN



class XercesDOMSupport;
#if defined(XALAN_BUILD_DEPRECATED_DOM_BRIDGE)
class XercesDocumentBridge;
#endif
class XercesDocumentWrapper;


typedef XERCES_CPP_NAMESPACE_QUALIFIER SAXParseException	SAXParseExceptionType;


class XALAN_XERCESPARSERLIAISON_EXPORT XercesParserLiaison :
	public XMLParserLiaison,
	public ErrorHandlerType
{
    
public:

	typedef XERCES_CPP_NAMESPACE_QUALIFIER SAXParser			SAXParserType;

	/**
	 * Construct a XercesParserLiaison instance.
	 *
	 * @param theSupport instance of DOMSupport object
	 *
	 * @deprecated This constructor is deprecated.  Use the next constructor instead.
	 */
	XercesParserLiaison( XercesDOMSupport&	theSupport, MemoryManagerType& theManager XALAN_DEFAULT_MEMMGR);

	/**
	 * Construct a XercesParserLiaison instance.
	 */
	XercesParserLiaison(MemoryManagerType& theManager XALAN_DEFAULT_MEMMGR);

	virtual
	~XercesParserLiaison();



	// These interfaces are inherited from XMLParserLiaison...
    MemoryManagerType&
    getMemoryManager()
    {
        return m_externalSchemaLocation.getMemoryManager();
    }

	virtual void
	reset();

	virtual ExecutionContext*
	getExecutionContext() const;

	virtual void
	setExecutionContext(ExecutionContext&	theContext);

	virtual XalanDocument*
	parseXMLStream(
			const InputSourceType&	reader,
			const XalanDOMString&	identifier = XalanDOMString(XalanMemMgrs::getDummyMemMgr()));

	virtual void
	parseXMLStream(
			const InputSourceType&	urlInputSource,
			DocumentHandlerType&	handler,
			const XalanDOMString&	identifier = XalanDOMString(XalanMemMgrs::getDummyMemMgr()));

	virtual void
	destroyDocument(XalanDocument*	theDocument);

	virtual int
	getIndent() const;

	virtual void
	setIndent(int	i);

	virtual bool
	getUseValidation() const;

	virtual void
	setUseValidation(bool	b);

	virtual const XalanDOMString&
	getParserDescription(XalanDOMString& theResult) const;

	virtual EntityResolverType*
	getEntityResolver() const;

	virtual void
	setEntityResolver(EntityResolverType*	resolver);


	// These interfaces are new to XercesParserLiaison...

	/**
	  * Create an instance of the Xerces default document that
	  * is suitable as a raw document.  The new document instance
      * is owned by this instance and will be destroyed when this
      * instance goes out of scope, or by an explicit call to
      * destroyDocument()
	  *
	  * @return a pointer to the new instance
	  */
	virtual DOMDocument_Type*
	createDOMFactory();

	/**
	  * Destroy an instance created by a call to createDOMFactory().
	  *
	  * @theDocument a pointer to the instance to be destroyed
	  */
	virtual void
	destroyDocument(DOMDocument_Type*   theDocument);

	/** Get the 'include ignorable whitespace' flag.
	  *
	  * This method returns the state of the parser's include ignorable
	  * whitespace flag.
	  *
	  * @return 'true' if the include ignorable whitespace flag is set on
	  * 		the parser, 'false' otherwise.
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
	  * <code>DOMText::isIgnorableWhitespace</code> will return true for those text
	  * nodes only.
	  * <p>When set to false, all ignorable whitespace will be discarded and
	  * no text node is added to the DOM tree.	Note: applications intended
	  * to process the "xml:space" attribute should not set this flag to false.
	  *
	  * @param include The new state of the include ignorable whitespace
	  * 			   flag.
	  *
	  * @see #getIncludeIgnorableWhitespace
	  */
	virtual void
	setIncludeIgnorableWhitespace(bool	include);

	/**
	  * This method returns the installed error handler.
	  *
	  * @return A pointer to the installed error handler object.
	  */
	virtual ErrorHandlerType*
	getErrorHandler() const;

	/**
	  * This method installs the user specified error handler on
	  * the parser.
	  *
	  * @param handler A pointer to the error handler to be called
	  * 			   when the parser comes across 'error' events
	  * 			   as per the SAX specification.
	  */
	virtual void
	setErrorHandler(ErrorHandlerType*	handler);

	/**
	  * This method returns the state of the parser's namespace
	  * handling capability.
	  *
	  * @return true, if the parser is currently configured to
	  * 		understand namespaces, false otherwise.
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
	  * 				be enforced or not.
	  *
	  * @see #getDoNamespaces
	  */
	virtual void
	setDoNamespaces(bool	newState);

	/**
	  * This method returns the state of the parser's
	  * exit-on-First-Fatal-Error flag.
	  *
	  * @return true, if the parser is currently configured to
	  * 		exit on the first fatal error, false otherwise.
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
	  * 				continue or exit when it encounters the first
	  * 				fatal error.
	  *
	  * @see #getExitOnFirstFatalError
	  */
	virtual void
	setExitOnFirstFatalError(bool	newState);

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
	setExternalSchemaLocation(const XalanDOMChar*	location);

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
	setExternalNoNamespaceSchemaLocation(const XalanDOMChar*	location);

#if defined(XALAN_BUILD_DEPRECATED_DOM_BRIDGE)
	/**
	 * This API is deprecated.
	 *
	 * Create a XalanDocument proxy for an existing Xerces document.
	 * The parser liaison owns the instance, and you must not delete
	 * it.	The liaison will delete it when reset() is called, or the
	 * liaison is destroyed.
	 *
	 * @deprecated The Xerces DOM bridge is deprecated.
	 * @param theXercesDocument The Xerces document.
	 * @return a pointer to a new XalanDocument-derived instance.
	 */
	XalanDocument*
	createDocument(const DOM_Document_Type&		theXercesDocument)
	{
		return createDocument(theXercesDocument, m_threadSafe, m_buildBridge);
	}

	/**
	 * This API is deprecated.
	 *
	 * Create a XalanDocument proxy for an existing Xerces document.
	 * The parser liaison owns the instance, and you must not delete
	 * it.	The liaison will delete it when reset() is called, or the
	 * liaison is destroyed.
	 *
	 * @deprecated The Xerces DOM bridge is deprecated.
	 * @param theXercesDocument The Xerces document.
	 * @param threadSafe If true, read access to the tree will be thread-safe (implies buildBridge == true).
	 * @param buildBridge If true, the entire bridge structure is built.
	 * @return a pointer to a new XalanDocument-derived instance.
	 */
	XalanDocument*
	createDocument(
			const DOM_Document_Type& 	theXercesDocument,
			bool						threadSafe,
			bool						buildBridge);
#endif

	/**
	 * Create a XalanDocument proxy for an existing Xerces document.
	 * The parser liaison owns the instance, and you must not delete
	 * it.	The liaison will delete it when reset() is called, or the
	 * liaison is destroyed.
	 *
	 * @param theXercesDocument The Xerces document.
	 * @return a pointer to a new XalanDocument-derived instance.
	 */
	XalanDocument*
	createDocument(const DOMDocument_Type*	theXercesDocument)
	{
		return createDocument(theXercesDocument, m_threadSafe, m_buildWrapper, m_buildMaps);
	}

	/**
	 * Create a XalanDocument proxy for an existing Xerces document.
	 * The parser liaison owns the instance, and you must not delete
	 * it.	The liaison will delete it when reset() is called, or the
	 * liaison is destroyed.
	 *
	 * @param theXercesDocument The Xerces document.
	 * @param threadSafe If true, read access to the tree will be thread-safe (implies buildWrapper == true).
	 * @param buildWrapper If true, the entire wrapper structure is built.
	 * @param buildMaps If true, the map of Xerces to Xalan nodes is always built.
	 * @return a pointer to a new XalanDocument-derived instance.
	 */
	XalanDocument*
	createDocument(
			const DOMDocument_Type*		theXercesDocument,
			bool						threadSafe,
			bool						buildWrapper,
			bool						buildMaps = false);

#if defined(XALAN_BUILD_DEPRECATED_DOM_BRIDGE)
	/**
	 * This API is deprecated.
	 *
	 * Map a pointer to a XalanDocument instance to its implementation
	 * class pointer.  Normally, you should have no reason for doing
	 * this.  The liaison will return a null pointer if it did not
	 * create the instance passed.
	 *
	 * @deprecated The Xerces DOM bridge has been deprecated.
	 * @param theDocument A pointer to a XalanDocument instance.
	 * @return A pointer to the XercesDocumentBridge instance.
	 */
	XercesDocumentBridge*
	mapDocument(const XalanDocument*	theDocument) const;
#endif

	/**
	 * Map a pointer to a XalanDocument instance to its implementation
	 * class pointer.  Normally, you should have no reason for doing
	 * this.  The liaison will return a null pointer if it did not
	 * create the instance passed.
	 *
	 * @param theDocument A pointer to a XalanDocument instance.
	 * @return A pointer to the XercesDocumentWrapper instance.
	 */
	XercesDocumentWrapper*
	mapDocumentToWrapper(const XalanDocument*	theDocument) const;

#if defined(XALAN_BUILD_DEPRECATED_DOM_BRIDGE)
	/** 
	 * This API is deprecated.
	 *
	 * Map a pointer to a XalanDocument instance to its corresponding
	 * class pointer.  Normally, you should have no reason for doing
	 * this.  The liaison will return a null pointer if it did not
	 * create the instance passed.
	 *
	 * @deprecated The Xerces DOM bridge has been deprecated.
	 * @param theDocument A pointer to a XalanDocument instance.
	 * @return A pointer to the XercesDocumentBridge instance.
	 */
	DOM_Document_Type
	mapXercesDocument(const XalanDocument*	theDocument) const;
#endif

	/** 
	 * Map a pointer to a XalanDocument instance to its corresponding
	 * class pointer.  Normally, you should have no reason for doing
	 * this.  The liaison will return a null pointer if it did not
	 * create the instance passed.
	 *
	 * @param theDocument A pointer to a XalanDocument instance.
	 * @return A pointer to the XercesDocumentBridge instance.
	 */
	const DOMDocument_Type*
	mapToXercesDocument(const XalanDocument*	theDocument) const;

	// Implementations for SAX ErrorHandler

	virtual void
	warning(const SAXParseExceptionType&	exception);

	virtual void
	error(const SAXParseExceptionType&	exception);
   
	virtual void
	fatalError(const SAXParseExceptionType&		exception);

	virtual void
	resetErrors();

	struct DocumentEntry
	{
#if defined(XALAN_BUILD_DEPRECATED_DOM_BRIDGE)
		bool	m_isDeprecated;

        bool
        isDeprecated() const
        {
            return m_isDeprecated;
        }
#else
        bool
        isDeprecated() const
        {
            return false;
        }
#endif

        bool	m_isOwned;

        bool
        isOwned() const
        {
            return m_isOwned;
        }

#if defined(XALAN_BUILD_DEPRECATED_DOM_BRIDGE)
		union
		{
			XercesDocumentBridge*	m_bridge;
			XercesDocumentWrapper*	m_wrapper;
		};

		DocumentEntry&
		operator=(XercesDocumentBridge* 	theBridge)
		{
			m_isDeprecated = true;

			m_bridge = theBridge;

			m_isOwned = true;

			return *this;
		}

        DocumentEntry&
		operator=(XercesDocumentWrapper*	theWrapper)
		{
			m_isDeprecated = false;

			m_wrapper = theWrapper;

			m_isOwned = true;

			return *this;
		}
#else
        XercesDocumentWrapper*  m_wrapper;

        DocumentEntry&
		operator=(XercesDocumentWrapper*	theWrapper)
		{
			m_wrapper = theWrapper;

			m_isOwned = true;

			return *this;
		}
#endif
	};

	typedef XalanMap<const XalanDocument*, DocumentEntry>	DocumentMapType;

	/**
	 * This API is deprecated.
	 * 
	 * This functions returns the state of the liaison's build-bridge-nodes flag.
	 *
	 * @deprecated The Xerces DOM bridge is deprecated.
	 * @return true, if the bridge nodes are automatically built, false otherwise.
	 */
	bool
	getBuildBridgeNodes() const
	
	{
		return m_buildBridge;
	}

	/**
	 * This API is deprecated.
	 *
	 * This functions sets the state of the liaison's build-bridge-nodes flag.
	 * This flag must be set for the document to be thread safe.  It can also be
	 * set to true to increase performance.  If this flag is set to false, then
	 * the thread-safe flag will also be set to false.
	 *
	 * @deprecated The Xerces DOM bridge is deprecated.
	 * @param newState The new state for the flag.
	 *
	 */
	void
	setBuildBridgeNodes(bool	newState)
	{
		m_buildBridge = newState;

		if (newState == false)
		{
			m_threadSafe = false;
		}
	}

	/**
	 * This functions returns the state of the liaison's build-wrapper-nodes flag.
	 *
	 * @return true, if the wrapper nodes are automatically built, false otherwise.
	 */
	bool
	getBuildWrapperNodes() const
	
	{
		return m_buildWrapper;
	}

	/**
	 * This functions sets the state of the liaison's build-wrapper-nodes flag.
	 * This flag must be set for the document to be thread safe.  It can also be
	 * set to true to increase performance.  If this flag is set to false, then
	 * the thread-safe flag will also be set to false.
	 *
	 * @param newState The new state for the flag.
	 *
	 */
	void
	setBuildWrapperNodes(bool	newState)
	{
		m_buildWrapper = newState;

		if (newState == false)
		{
			m_threadSafe = false;
		}
	}

	/**
	 * This functions returns the state of the liaison's thread-safe flag.
	 * If true, documents created will be safe when data is read.  By default,
	 * documents are _not_ thread-safe.
	 *
	 * Note -- modifications are _never_ synchronized.
	 *
	 * @return true, if the new documents will be thread safe, false otherwise.
	 */
	bool
	getThreadSafe() const
	
	{
		return m_threadSafe;
	}

	/**
	 * This functions sets the state of the liaison's thread-safe flag.
	 * This flag must be set for the document to be thread safe.  If this
	 * flag is set to true, then the build-bridge-nodes flag will also be
	 * set to true.
	 *
	 * @param newState The new state for the flag.
	 *
	 */
	void
	setThreadSafe(bool	newState)
	{
		m_threadSafe = newState;

		if (m_threadSafe == true)
		{
			m_buildWrapper = true;
			m_buildBridge = true;
		}
	}

	/**
	 * This functions returns the state of the liaison's buildMaps flag.
	 * If true, maps will be created to allow mapping of Xalan<->Xerces mapping
	 * in both directions for XercesWrapper classes.
	 *
	 * @return true, if the new documents will be built with Maps
	 */
	bool
	getBuildMaps() const
	
	{
		return m_buildMaps;
	}
 
	/**
	 * This functions sets the state of the liaison's buildMaps flag.
	 * When this flag is true, maps will be built providing Xerces<->Xalan
	 * mapping in Wrapper classes.
	 *
	 * @note The maps created use a large amount of memory.  If only
	 * Xalan->Xerces node mapping is required, do not set this to true.
	 *
	 * @param newState The new state for the flag.
	 *
	 */
	void
	setBuildMaps(bool	newState)
	{
		m_buildMaps = newState;
	}

#if XERCES_VERSION_MAJOR >= 2
	typedef XERCES_CPP_NAMESPACE_QUALIFIER XercesDOMParser 	DOMParserType;
#else
	typedef XERCES_CPP_NAMESPACE_QUALIFIER DOMParser		DOMParserType;
#endif

protected:

	static void
	formatErrorMessage(
			const SAXParseExceptionType&	e,
			XalanDOMString& 				theMessage);

#if defined(XALAN_BUILD_DEPRECATED_DOM_BRIDGE)
	/**
	 * Create a XalanDocument proxy for an existing Xerces document.
	 *
	 * This API is deprecated.
	 *
	 * @param theXercesDocument The Xerces document.
	 * @param threadSafe If true, read access to the tree will be thread-safe (implies buildBridge == true).
	 * @param buildBridge If true, the entire bridge structure is built.
	 * @return a pointer to a new XercesDocumentBridge instance.
	 */
	XercesDocumentBridge*
	doCreateDocument(
			const DOM_Document_Type& 	theXercesDocument,
			bool						threadSafe,
			bool						buildBridge);
#endif

	/**
	 * Create a XalanDocument proxy for an existing Xerces document.
	 *
	 * @param theXercesDocument The Xerces document.
	 * @param threadSafe If true, read access to the tree will be thread-safe (implies buildBridge == true).
	 * @param buildWrapper If true, the entire bridge structure is built.
	 * @param buildMaps If true, the map of Xerces to Xalan nodes is always built.
	 * @return a pointer to a new XercesDocumentWrapper instance.
	 */
	XercesDocumentWrapper*
	doCreateDocument(
			const DOMDocument_Type*		theXercesDocument,
			bool						threadSafe,
			bool						buildWrapper,
			bool						buildMaps,
			bool						isOwned);

private:

	void
	ensureSAXParser();

	void
	ensureDOMParser();

	DOMParserType*
	createDOMParser();

	SAXParserType*
	createSAXParser();



	// Data members...
	int 				m_indent;

	bool				m_useValidation;

	bool				m_includeIgnorableWhitespace;

	bool				m_doNamespaces;

	bool				m_exitOnFirstFatalError;

	EntityResolverType* m_entityResolver;

	ErrorHandlerType*	m_errorHandler;

	XalanDOMString		m_externalSchemaLocation;

	XalanDOMString		m_externalNoNamespaceSchemaLocation;

	DocumentMapType 	m_documentMap;

	bool				m_buildWrapper;

	bool				m_buildBridge;

	bool				m_threadSafe;

	bool				m_buildMaps;

	ExecutionContext*	m_executionContext;

	SAXParserType*		m_saxParser;

	DOMParserType*		m_domParser;
};



XALAN_CPP_NAMESPACE_END



#endif	// XercesPARSERLIAISON_HEADER_GUARD_1357924680
