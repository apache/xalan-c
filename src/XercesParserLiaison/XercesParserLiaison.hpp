/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *	  notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *	  notice, this list of conditions and the following disclaimer in
 *	  the documentation and/or other materials provided with the
 *	  distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *	  if any, must include the following acknowledgment:  
 *		 "This product includes software developed by the
 *		  Apache Software Foundation (http://www.apache.org/)."
 *	  Alternately, this acknowledgment may appear in the software itself,
 *	  if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *	  not be used to endorse or promote products derived from this
 *	  software without prior written permission. For written 
 *	  permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *	  nor may "Apache" appear in their name, without prior written
 *	  permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.	IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
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
 */
#if !defined(XercesPARSERLIAISON_HEADER_GUARD_1357924680)
#define XercesPARSERLIAISON_HEADER_GUARD_1357924680


// Base include file.  Must be first.
#include <XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



// Standard Library header files.
#include <map>



// Xerces DOM header files
#include <xercesc/sax/ErrorHandler.hpp>



// Base class header file.
#include <XMLSupport/XMLParserLiaison.hpp>



#include <XercesParserLiaison/XercesBridgeTypes.hpp>
#include <XercesParserLiaison/XercesWrapperTypes.hpp>



#if XERCES_VERSION_MAJOR >= 2
XALAN_DECLARE_XERCES_CLASS(XercesDOMParser);
#else
XALAN_DECLARE_XERCES_CLASS(DOMParser);
#endif
XALAN_DECLARE_XERCES_CLASS(SAXParser);



XALAN_CPP_NAMESPACE_BEGIN



class DOMSupport;
class XercesDOMSupport;
class XercesDocumentBridge;
class XercesDocumentWrapper;
class XSLProcessor;

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
	XercesParserLiaison(XercesDOMSupport&	theSupport);

	/**
	 * Construct a XercesParserLiaison instance.
	 */
	XercesParserLiaison();

	virtual
	~XercesParserLiaison();

	// These interfaces are inherited from XMLParserLiaison...

	virtual void
	reset();

	virtual ExecutionContext*
	getExecutionContext() const;

	virtual void
	setExecutionContext(ExecutionContext&	theContext);

	virtual XalanDocument*
	parseXMLStream(
			const InputSourceType&	reader,
			const XalanDOMString&	identifier = XalanDOMString());

	virtual void
	parseXMLStream(
			const InputSourceType&	urlInputSource,
			DocumentHandlerType&	handler,
			const XalanDOMString&	identifier = XalanDOMString());

	// Create a non-thread safe document, with no synchronization and no bridge...
	virtual XalanDocument*
	createDocument();

	// Create a non-thread safe document, with no synchronization and no bridge...
	virtual XalanDocument*
	createDOMFactory();

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

	virtual const XalanDOMString
	getParserDescription() const;

	virtual EntityResolverType*
	getEntityResolver() const;

	virtual void
	setEntityResolver(EntityResolverType*	resolver);


	// These interfaces are new to XercesParserLiaison...

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
	  * DOM_Text::isIgnorableWhitespace() will return true for those text
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

	/**
	 * This API is deprecated.
	 *
	 * Create a XalanDocument proxy for an existing Xerces document.
	 * The parser liaison owns the instance, and you must not delete
	 * it.	The liaison will delete it when reset() is called, or the
	 * liaison is destroyed.
	 *
	 * @deprecated This API is deprecated.
	 * @param theXercesDocument The Xerces document.
	 * @return a pointer to a new XalanDocument-derived instance.
	 */
	XalanDocument*
	createDocument(const DOM_DocumentType&	theXercesDocument)
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
	 * @deprecated This API is deprecated.
	 * @param theXercesDocument The Xerces document.
	 * @param threadSafe If true, read access to the tree will be thread-safe (implies buildBridge == true).
	 * @param buildBridge If true, the entire bridge structure is built.
	 * @return a pointer to a new XalanDocument-derived instance.
	 */
	XalanDocument*
	createDocument(
			const DOM_DocumentType& 	theXercesDocument,
			bool						threadSafe,
			bool						buildBridge);

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
	createDocument(const DOMDocumentType*	theXercesDocument)
	{
		return createDocument(theXercesDocument, m_threadSafe, m_buildWrapper);
	}

	/**
	 * Create a XalanDocument proxy for an existing Xerces document.
	 * The parser liaison owns the instance, and you must not delete
	 * it.	The liaison will delete it when reset() is called, or the
	 * liaison is destroyed.
	 *
	 * @param theXercesDocument The Xerces document.
	 * @param threadSafe If true, read access to the tree will be thread-safe (implies buildBridge == true).
	 * @param buildWrapper If true, the entire wrapper structure is built.
	 * @return a pointer to a new XalanDocument-derived instance.
	 */
	XalanDocument*
	createDocument(
			const DOMDocumentType*	theXercesDocument,
			bool					threadSafe,
			bool					buildWrapper);

	/**
	 * This API is deprecated.
	 *
	 * Map a pointer to a XalanDocument instance to its implementation
	 * class pointer.  Normally, you should have no reason for doing
	 * this.  The liaison will return a null pointer if it did not
	 * create the instance passed.
	 *
	 * @deprecated This API is deprecated.
	 * @param theDocument A pointer to a XalanDocument instance.
	 * @return A pointer to the XercesDocumentBridge instance.
	 */
	XercesDocumentBridge*
	mapDocument(const XalanDocument*	theDocument) const;

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

	/** 
	 * This API is deprecated.
	 *
	 * Map a pointer to a XalanDocument instance to its corresponding
	 * class pointer.  Normally, you should have no reason for doing
	 * this.  The liaison will return a null pointer if it did not
	 * create the instance passed.
	 *
	 * @deprecated This API is deprecated.
	 * @param theDocument A pointer to a XalanDocument instance.
	 * @return A pointer to the XercesDocumentBridge instance.
	 */
	DOM_DocumentType
	mapXercesDocument(const XalanDocument*	theDocument) const;

	/** 
	 * Map a pointer to a XalanDocument instance to its corresponding
	 * class pointer.  Normally, you should have no reason for doing
	 * this.  The liaison will return a null pointer if it did not
	 * create the instance passed.
	 *
	 * @param theDocument A pointer to a XalanDocument instance.
	 * @return A pointer to the XercesDocumentBridge instance.
	 */
	const DOMDocumentType*
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
		bool	m_isDeprecated;
		bool	m_isOwned;

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
	};

#if defined(XALAN_NO_STD_NAMESPACE)
	typedef map<const XalanDocument*,
				DocumentEntry,
				less<const XalanDocument*> >	DocumentMapType;
#else
	typedef std::map<const XalanDocument*,
					 DocumentEntry> 			DocumentMapType;
#endif

	/**
	 * This functions returns the state of the liaison's build-bridge-nodes flag.
	 *
	 * @deprecated This API is deprecated.
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
	 * @deprecated This API is deprecated.
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
	 * This API is deprecated.
	 *
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

	DOMParserType*
	CreateDOMParser();

	virtual SAXParserType*
	CreateSAXParser();

	/**
	 * Create a XalanDocument proxy for an existing Xerces document.
	 *
	 * @param theXercesDocument The Xerces document.
	 * @param threadSafe If true, read access to the tree will be thread-safe (implies buildBridge == true).
	 * @param buildBridge If true, the entire bridge structure is built.
	 * @return a pointer to a new XercesDocumentBridge instance.
	 */
	XercesDocumentBridge*
	doCreateDocument(
			const DOM_DocumentType& 	theXercesDocument,
			bool						threadSafe,
			bool						buildBridge);

	/**
	 * Create a XalanDocument proxy for an existing Xerces document.
	 *
	 * @param theXercesDocument The Xerces document.
	 * @param threadSafe If true, read access to the tree will be thread-safe (implies buildBridge == true).
	 * @param buildWrapper If true, the entire bridge structure is built.
	 * @return a pointer to a new XercesDocumentWrapper instance.
	 */
	XercesDocumentWrapper*
	doCreateDocument(
			const DOMDocumentType*	theXercesDocument,
			bool					threadSafe,
			bool					buildWrapper,
			bool					isOwned);

private:

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

	ExecutionContext*	m_executionContext;
};



XALAN_CPP_NAMESPACE_END



#endif	// XercesPARSERLIAISON_HEADER_GUARD_1357924680
