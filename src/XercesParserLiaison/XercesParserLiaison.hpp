/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2001 The Apache Software Foundation.  All rights 
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
#include <xercesc/util/XercesDefs.hpp>
#include <xercesc/sax/ErrorHandler.hpp>
#include <xercesc/dom/DOM_Document.hpp>



// Base class header file.
#include <XMLSupport/XMLParserLiaison.hpp>



class DOMParser;
class DOMSupport;
class EntityResolver;
class InputSource;
class SAXParser;
class XercesDOMSupport;
class XercesDocumentBridge;
class XSLProcessor;



class XALAN_XERCESPARSERLIAISON_EXPORT XercesParserLiaison : public XMLParserLiaison, public ErrorHandler
{
public:

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
			const InputSource&		reader,
			const XalanDOMString&	identifier = XalanDOMString());

	virtual void
	parseXMLStream(
			const InputSource&		urlInputSource,
			DocumentHandler&		handler,
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

	virtual EntityResolver*
	getEntityResolver() const;

	virtual void
	setEntityResolver(EntityResolver*	resolver);


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
	virtual ErrorHandler*
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
	setErrorHandler(ErrorHandler*	handler);

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
	 * Create a XalanDocument proxy for an existing Xerces document.
	 * The parser liaison owns the instance, and you must not delete
	 * it.	The liaison will delete it when reset() is called, or the
	 * liaison is destroyed.
	 *
	 * @param theXercesDocument The Xerces document.
	 * @param threadSafe If true, read access to the tree will be thread-safe (implies buildBridge == true).
	 * @param buildBridge If true, the entire bridge structure is built.
	 * @return a pointer to a new XalanDocument-derived instance.
	 */
	virtual XalanDocument*
	createDocument(
			const DOM_Document&		theXercesDocument,
			bool					threadSafe = false,
			bool					buildBridge = false);

	/** 
	 * Map a pointer to a XalanDocument instance to its implementation
	 * class pointer.  Normally, you should have no reason for doing
	 * this.  The liaison will return a null pointer if it did not
	 * create the instance passed.
	 *
	 * @param theDocument A pointer to a XalanDocument instance.
	 * @return A pointer to the XercesDocumentBridge instance.
	 */
	XercesDocumentBridge*
	mapDocument(const XalanDocument*	theDocument) const;

	/** 
	 * Map a pointer to a XalanDocument instance to its corresponding
	 * class pointer.  Normally, you should have no reason for doing
	 * this.  The liaison will return a null pointer if it did not
	 * create the instance passed.
	 *
	 * @param theDocument A pointer to a XalanDocument instance.
	 * @return A pointer to the XercesDocumentBridge instance.
	 */
	DOM_Document
	mapXercesDocument(const XalanDocument*	theDocument) const;

	// Implementations for SAX ErrorHandler

	virtual void
	warning(const SAXParseException& exception);

	virtual void
	error(const SAXParseException& exception);
   
	virtual void
	fatalError(const SAXParseException& exception);

	virtual void
	resetErrors();

#if defined(XALAN_NO_NAMESPACES)
	typedef map<const XalanDocument*,
				XercesDocumentBridge*,
				less<const XalanDocument*> >	DocumentMapType;
#else
	typedef std::map<const XalanDocument*,
					 XercesDocumentBridge*>		DocumentMapType;
#endif

	/**
	 * This functions returns the state of the liaison's build-bridge-nodes flag.
	 *
	 * @return true, if the bridge nodes are automatically built, false otherwise.
	 */
	bool
	getBuildBridgeNodes() const
	
	{
		return m_buildBridge;
	}

	/**
	 * This functions sets the state of the liaison's build-bridge-nodes flag.
	 * This flag must be set for the document to be thread safe.  It can also be
	 * set to true to increase performance.  If this flag is set to false, then
	 * the thread-safe flag will also be set to false.
	 *
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
			m_buildBridge = true;
		}
	}

protected:

	static void
	formatErrorMessage(
			const SAXParseException&	e,
			XalanDOMString&				theMessage);

	virtual DOMParser*
	CreateDOMParser();

	virtual SAXParser*
	CreateSAXParser();

	/**
	 * Create a XalanDocument proxy for an existing Xerces document.
	 *
	 * @param theXercesDocument The Xerces document.
	 * @param threadSafe If true, read access to the tree will be thread-safe (implies buildBridge == true).
	 * @param buildBridge If true, the entire bridge structure is built.
	 * @return a pointer to a new XercesDocumentBridge instance.
	 */
	virtual XercesDocumentBridge*
	doCreateDocument(
			const DOM_Document&		theXercesDocument,
			bool					threadSafe,
			bool					buildBridge);

private:

	// Data members...
	int 				m_indent;

	bool				m_useValidation;

	bool				m_includeIgnorableWhitespace;

	bool				m_doNamespaces;

	bool				m_exitOnFirstFatalError;

	EntityResolver* 	m_entityResolver;

	ErrorHandler*		m_errorHandler;

	DocumentMapType 	m_documentMap;

	bool				m_buildBridge;

	bool				m_threadSafe;

	ExecutionContext*	m_executionContext;
};



#endif	// XercesPARSERLIAISON_HEADER_GUARD_1357924680
