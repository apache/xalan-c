/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2000 The Apache Software Foundation.  All rights 
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
#if !defined(XALANSOURCETREEPARSERLIAISON_HEADER_GUARD_1357924680)
#define XALANSOURCETREEPARSERLIAISON_HEADER_GUARD_1357924680


// Base include file.  Must be first.
#include <XalanSourceTree/XalanSourceTreeDefinitions.hpp>



// Standard Library header files.
#include <map>



#include <XercesParserLiaison/XercesDOMSupport.hpp>
#include <XercesParserLiaison/XercesParserLiaison.hpp>



class ContentHandler;
class DTDHandler;
class LexicalHandler;
class XalanSourceTreeDOMSupport;
class XalanSourceTreeDocument;



class XALAN_XALANSOURCETREE_EXPORT  XalanSourceTreeParserLiaison : public XMLParserLiaison
{
public:

	/**
	 * Construct a XalanSourceTreeParserLiaison instance.
	 *
	 * @param theSupport instance of DOMSupport object
	 */
	XalanSourceTreeParserLiaison(XalanSourceTreeDOMSupport&		theSupport);

	virtual
	~XalanSourceTreeParserLiaison();

	// These interfaces are inherited from XMLParserLiaison...

	virtual void
	reset();

	virtual bool
	supportsSAX() const;

	virtual XalanDocument*
	parseXMLStream(
			const InputSource&		reader,
			const XalanDOMString&	identifier = XalanDOMString());

	virtual void
	parseXMLStream(
			const InputSource&		inputSource,
			DocumentHandler&		handler,
			const XalanDOMString&	identifier = XalanDOMString());

	virtual XalanDocument*
	createDocument();

	virtual XalanDocument*
	getDOMFactory();

	virtual void
	setSpecialCharacters(const XalanDOMString&	str);

	virtual const XalanDOMString&
	getSpecialCharacters() const;

	virtual int
	getIndent() const;

	virtual void
	setIndent(int	i);

	virtual bool
	getShouldExpandEntityRefs() const;

	virtual void
	SetShouldExpandEntityRefs(bool	b);

	virtual bool
	getUseValidation() const;

	virtual void
	setUseValidation(bool	b);

	virtual const XalanDOMString
	getParserDescription() const;


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
			const InputSource&		theInputSource,
			ContentHandler&			theContentHandler,
			DTDHandler*				theDTDHandler = 0,
			LexicalHandler*			theLexicalHandler = 0,
			const XalanDOMString&	theIdentifier = XalanDOMString());

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
	  * This method returns the installed error handler. Suitable
	  * for 'lvalue' usages.
	  *
	  * @return The pointer to the installed error handler object.
	  */
	virtual ErrorHandler*
	getErrorHandler();

	/**
	  * This method returns the installed error handler. Suitable
	  * for 'rvalue' usages.
	  *
	  * @return A const pointer to the installed error handler object.
	  */
	virtual const ErrorHandler*
	getErrorHandler() const;

	/**
	  * This method installs the user specified error handler on
	  * the parser.
	  *
	  * @param handler A pointer to the error handler to be called
	  * 			   when the parser comes across 'error' events
	  * 			   as per the SAX specification.
	  *
	  * @see Parser#setErrorHandler
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
	  * This method returns the installed entity resolver. Suitable
	  * for 'lvalue' usages.
	  *
	  * @return The pointer to the installed entity resolver object.
	  */
	virtual EntityResolver*
	getEntityResolver();

	/**
	  * This method returns the installed entity resolver. Suitable
	  * for 'rvalue' usages.
	  *
	  * @return A const pointer to the installed entity resolver object.
	  */
	virtual const EntityResolver*
	getEntityResolver() const;

	/**
	  * This method installs the user specified entity resolver on the
	  * parser. It allows applications to trap and redirect calls to
	  * external entities.
	  *
	  * @param handler A pointer to the entity resolver to be called
	  * 			   when the parser comes across references to
	  * 			   entities in the XML file.
	  *
	  * @see Parser#setEntityResolver
	  */
	virtual void
	setEntityResolver(EntityResolver*	resolver);

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
	mapDocument(const XalanDocument*	theDocument) const;

	/** 
	 * Create a XalanSourceTreeDocument instance.
	 *
	 * @return A pointer to the XalanSourceTreeDocument instance.
	 */
	XalanSourceTreeDocument*
	createXalanSourceTreeDocument();

#if defined(XALAN_NO_NAMESPACES)
	typedef map<const XalanDocument*,
				XalanSourceTreeDocument*,
				less<const XalanDocument*> >	DocumentMapType;
#else
	typedef std::map<const XalanDocument*,
					 XalanSourceTreeDocument*>	DocumentMapType;
#endif

private:

	// Data members...
	XercesDOMSupport				m_xercesDOMSupport;	// Must be before m_xercesParserLiaison!!!

	XercesParserLiaison				m_xercesParserLiaison;

	DocumentMapType 				m_documentMap;

	XalanSourceTreeDOMSupport&		m_domSupport;
};



#endif	// XALANSOURCETREEPARSERLIAISON_HEADER_GUARD_1357924680
