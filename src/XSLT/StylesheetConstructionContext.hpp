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
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
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
 *
 * @author <a href="mailto:david_n_bertoni@lotus.com">David N. Bertoni</a>
 */
#if !defined(STYLESHEETCONSTRUCTIONCONTEXT_HEADER_GUARD_1357924680)
#define STYLESHEETCONSTRUCTIONCONTEXT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XSLT/XSLTDefinitions.hpp>



#include <PlatformSupport/URISupport.hpp>



#include <XalanDOM/XalanDOMString.hpp>



class DocumentHandler;
class ElemTemplateElement;
class Locator;
class PrefixResolver;
class Stylesheet;
class StylesheetRoot;
class XalanDocument;
class XalanNode;
class XPath;
class XMLURL;
class XSLTInputSource;



//
// An abstract class which provides support for constructing the internal
// representation  of a stylesheet.
//
class XALAN_XSLT_EXPORT StylesheetConstructionContext
{
public:

	/**
	 * IDs for XSL element types. These are the values
	 * that must be returned by getElementToken().
	 */
	enum eElementToken
	{
		ELEMNAME_UNDEFINED = -2,
		ELEMNAME_EMPTY = -1,
		ELEMNAME_WITH_PARAM,
		ELEMNAME_APPLY_TEMPLATES,
		ELEMNAME_CHOOSE,
		ELEMNAME_COMMENT,
		ELEMNAME_COPY,
		ELEMNAME_COPY_OF,
		ELEMNAME_ATTRIBUTE,
		ELEMNAME_ATTRIBUTE_SET,
		ELEMNAME_EXTENSION,
		ELEMNAME_FOR_EACH,
		ELEMNAME_KEY,
		ELEMNAME_IF,
		ELEMNAME_IMPORT,
		ELEMNAME_INCLUDE,
		ELEMNAME_CALL_TEMPLATE,
		ELEMNAME_PARAM,
		ELEMNAME_NUMBER,
		ELEMNAME_OTHERWISE,
		ELEMNAME_PI,
		ELEMNAME_PRESERVE_SPACE,
		ELEMNAME_TEMPLATE,
		ELEMNAME_SORT,
		ELEMNAME_STRIP_SPACE,
		ELEMNAME_STYLESHEET,
		ELEMNAME_TEXT,
		ELEMNAME_VALUE_OF,
		ELEMNAME_WHEN,
		ELEMNAME_FALLBACK,
		ELEMNAME_ELEMENT,

		ELEMNAME_APPLY_IMPORTS,

		ELEMNAME_VARIABLE,
		ELEMNAME_MESSAGE,

		ELEMNAME_OUTPUT,
		ELEMNAME_DECIMAL_FORMAT,
		ELEMNAME_NAMESPACE_ALIAS,

		// This one doesn't really have a name.
		// It's used for any literal result
		// element.
		ELEMNAME_LITERAL_RESULT,

		// This one doesn't really have a name.
		// It's used for any literal text in
		// the content of a literal result
		// element.
		ELEMNAME_TEXT_LITERAL_RESULT,

		// Extension handling.
		ELEMNAME_EXTENSION_CALL,
		ELEMNAME_EXTENSION_HANDLER
	};

	typedef URISupport::URLAutoPtrType	URLAutoPtrType;

	explicit
	StylesheetConstructionContext();

	virtual
	~StylesheetConstructionContext();


	/**
	 * Reset the StylesheetConstructionContext instance.  Any existing objects
	 * created by the instance will be destroyed.
	 */
	virtual void
	reset() = 0;

	/**
	 * Create a new StylesheetRoot instance.  The StylesheetConstructionContext
	 * instance owns the StylesheetRoot instance, and will delete it when asked
	 * or when the StylesheetConstructionContext instance is destroyed.
	 *
	 * @param theBaseIdentifier A URI to the stylesheet file.
	 * @return A pointer to a new StylesheetRoot instance.
	 */
	virtual StylesheetRoot*
	create(const XalanDOMString&	theBaseIdentifier) = 0;

	/**
	 * Create a new StylesheetRoot instance.  The StylesheetConstructionContext
	 * instance owns the StylesheetRoot instance, and will delete it when asked
	 * or when the StylesheetConstructionContext instance is destroyed.
	 *
	 * @param theInputSource A reference to the input source.
	 * @return A pointer to a new StylesheetRoot instance.
	 */
	virtual StylesheetRoot*
	create(const XSLTInputSource&	theInputSource) = 0;

	/**
	 * Create a new Stylesheet instance.  The StylesheetConstructionContext
	 * instance owns the Stylesheet instance, and will delete it when asked
	 * or when the StylesheetConstructionContext instance is destroyed.
	 *
	 * @param A reference to the StylesheetRoot instance.
	 * @param theBaseIdentifier A URI to the stylesheet file.
	 * @return A pointer to a new StylesheetRoot instance.
	 */
	virtual Stylesheet*
	create(
			StylesheetRoot&			theStylesheetRoot,
			const XalanDOMString&	theBaseIdentifier) = 0;

	/**
	 * Destroy a StylesheetRoot instance.  If this StylesheetConstructionContext
	 * instance does not own the StylesheetRoot, it will not delete it
	 *
	 * @param theStylesheet A pointer to the StylesheetRoot instance to delete.
	 */
	virtual void
	destroy(StylesheetRoot*		theStylesheetRoot) = 0;

	/**
	 * Determine the fully qualified URI for a string.
	 *
	 * @param urlString string to qualify
	 * @return auto pointer to fully qualified URI
	 */
	virtual URLAutoPtrType
	getURLFromString(const XalanDOMString&	urlString) = 0;

	/**
	 * Determine the fully qualified URI for a string.
	 *
	 * @param urlString string to qualify
	 * @return string to fully qualified URI
	 */
	virtual XalanDOMString
	getURLStringFromString(const XalanDOMString&	urlString) = 0;

	/**
	 * Determine the fully qualified URI for a string.
	 *
	 * @param urlString string to qualify
	 * @param base base location for URI
	 * @return auto pointer to fully qualified URI
	 */
	virtual URLAutoPtrType
	getURLFromString(
			const XalanDOMString&	urlString,
			const XalanDOMString&	base) = 0;

	/**
	 * Determine the fully qualified URI for a string.
	 *
	 * @param urlString string to qualify
	 * @param base base location for URI
	 * @return string to fully qualified URI
	 */
	virtual XalanDOMString
	getURLStringFromString(
			const XalanDOMString&	urlString,
			const XalanDOMString&	base) = 0;

	/**
	 * Retrieve the URI for the current XSLT namespace, for example,
	 * "http://www.w3.org/1999/XSL/Transform" 
	 * 
	 * @return URI string
	 */
	virtual const XalanDOMString&
	getXSLTNamespaceURI() const = 0;

	/**
	 * Create and initialize an xpath for a match pattern and return it. This
	 * is to be used by stylesheet elements that need an XPath that is
	 * guaranteed to persist while it lives.
	 *
	 * @param locator the locator for the XPath. May be null.
	 * @param str string to match
	 * @param resolver resolver for namespace resolution
	 * @return XPath for match pattern
	 */
	virtual XPath*
	createMatchPattern(
			const Locator*			locator,
			const XalanDOMString&	str,
			const PrefixResolver&	resolver) = 0;

	/**
	 * Create and initialize an xpath for a match pattern and return it. This
	 * is to be used by stylesheet elements that need an XPath that is
	 * guaranteed to persist while it lives.
	 *
	 * @param locator the locator for the XPath. May be null.
	 * @param str string to match
	 * @param resolver resolver for namespace resolution
	 * @return XPath for match pattern
	 */
	virtual XPath*
	createMatchPattern(
			const Locator*			locator,
			const XalanDOMChar*		str,
			const PrefixResolver&	resolver) = 0;

	/**
	 * Create and initialize an xpath and return it. This is to be used by
	 * stylesheet elements that need an XPath that is guaranteed to persist
	 * while it lives.
	 *
	 * @param locator the locator for the XPath. May be null.
	 * @param str string to match
	 * @param resolver resolver for namespace resolution
	 * @return XPath for string matched
	 */
	virtual XPath*
	createXPath(
			const Locator*			locator,
			const XalanDOMString&	str,
			const PrefixResolver&	resolver) = 0;

	/**
	 * Create and initialize an xpath and return it. This is to be used by
	 * stylesheet elements that need an XPath that is guaranteed to persist
	 * while it lives.
	 *
	 * @param locator the locator for the XPath. May be null.
	 * @param str string to match
	 * @param resolver resolver for namespace resolution
	 * @return XPath for string matched
	 */
	virtual XPath*
	createXPath(
			const Locator*			locator,
			const XalanDOMChar*		str,
			const PrefixResolver&	resolver) = 0;

	/**
	 * Get the locator from the top of the locator stack.
	 *
	 * @return A pointer to the Locator, or 0 if there is nothing on the stack.
	 */
	virtual const Locator*
	getLocatorFromStack() const = 0;

	/**
	 * Push a locator on to the locator stack.
	 *
	 * @param A pointer to the Locator to push.
	 */
	virtual void
	pushLocatorOnStack(const Locator*	locator) = 0;

	/**
	 * Pop the locator from the top of the locator stack.
	 */
	virtual void
	popLocatorStack() = 0;

	/**
	 * Get the Xalan namespace for built-in extensions.
	 *
	 * @return Xalan namespace for extensions
	 */
	virtual const XalanDOMString&
	getXalanXSLNameSpaceURL() const = 0;

	/**
	 * Read in the XML file, either producing a Document or calling SAX events,
	 * and register the document in a table.  If the document has already been
	 * read in, it will not be reparsed.
	 *
	 * @param urlString location of the XML
	 * @param docHandler pointer to SAX event handler
	 * @param docToRegister if using a SAX event handler, the object to register in the source docs table. 
	 * @return document object, which represents the parsed XML
	 * @exception SAXException
	 */
	virtual XalanDocument*
	parseXML(
			const XalanDOMString&	urlString,
			DocumentHandler*		docHandler, 
			XalanDocument*			docToRegister) = 0;

	/**
	 * Given an name, determine if it is the xml:space attribute
	 *
	 * @param name a name
	 * @return true if the string is the xml:space attribute name
	 */
	virtual bool
	isXMLSpaceAttribute(
			const XalanDOMChar*		theAttributeName,
			const Stylesheet&		theStylesheet,
			const Locator*			theLocator) = 0;

	/**
	 * Given an XSL tag name, return an integer token that corresponds to
	 * the enums defined above.
	 *
	 * @param name a probable xsl:xxx element
	 * @return The enum value for that token, or ELEMNAME_UNDEFINED
	 */
	virtual int
	getElementToken(const XalanDOMString&	name) const = 0;

	/**
	 * Get the latest XSLT version currently supported.
	 *
	 * @return XSLT version number
	 */
	virtual double
	getXSLTVersionSupported() const = 0;

	/**
	 * Get a pooled string given the source string.  If
	 * the string already exists in the pool, no copy
	 * will be made.  If not, a copy will be made and
	 * kept for later use.
	 *
	 * @param theString The source string
	 * @return a const reference to a pooled string.
	 */
	virtual const XalanDOMString&
	getPooledString(const XalanDOMString&	theString) = 0;

	/**
	 * Get a pooled string given the source character
	 * array.  If the string already exists in the pool,
	 * no copy will be made.  If not, a copy will be made
	 * and kept for later use.
	 *
	 * @param theString The source character array
	 * @param theLength The length of the character array
	 * @return a const reference to a pooled string.
	 */
	virtual const XalanDOMString&
	getPooledString(
			const XalanDOMChar*			theString,
			XalanDOMString::size_type	theLength = XalanDOMString::npos) = 0;

	/**
	 * Allocate a vector of XalanDOMChar of the specified
	 * size.
	 *
	 * @param theLength The length of the character vector
	 */
	virtual XalanDOMChar*
	allocateVector(XalanDOMString::size_type		theLength) = 0;

	/**
	 * Allocate a vector of XalanDOMChar of the specified
	 * size.
	 *
	 * @param theString The source character array
	 * @param theLength The length of the character vector
	 * @param fTerminate If true, terminate the new vector with 0
	 */
	virtual XalanDOMChar*
	allocateVector(
			const XalanDOMChar*			theString,
			XalanDOMString::size_type	theLength = XalanDOMString::npos,
			bool						fTerminate = true) = 0;

	// These interfaces are inherited from ExecutionContext...

	virtual void
	error(
			const XalanDOMString&		msg,
			const XalanNode* 			sourceNode = 0,
			const ElemTemplateElement*	styleNode = 0) const = 0;

	virtual void
	error(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode,
			const Locator* 			locator) const = 0;

	virtual void
	error(
			const char*					msg,
			const XalanNode* 			sourceNode = 0,
			const ElemTemplateElement* 	styleNode = 0) const = 0;

	virtual void
	error(
			const char*			msg,
			const XalanNode* 	sourceNode,
			const Locator* 		locator) const = 0;

	virtual void
	warn(
			const XalanDOMString&		msg,
			const XalanNode* 			sourceNode = 0,
			const ElemTemplateElement* 	styleNode = 0) const = 0;

	virtual void
	warn(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode,
			const Locator* 			locator) const = 0;

	virtual void
	warn(
			const char*					msg,
			const XalanNode* 			sourceNode = 0,
			const ElemTemplateElement* 	styleNode = 0) const = 0;

	virtual void
	warn(
			const char*			msg,
			const XalanNode* 	sourceNode,
			const Locator* 		locator) const = 0;

	virtual void
	message(
			const XalanDOMString&		msg,
			const XalanNode* 			sourceNode = 0,
			const ElemTemplateElement* 	styleNode = 0) const = 0;

	virtual void
	message(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode,
			const Locator* 			locator) const = 0;

	virtual void
	message(
			const char*					msg,
			const XalanNode* 			sourceNode = 0,
			const ElemTemplateElement* 	styleNode = 0) const = 0;

	virtual void
	message(
			const char*			msg,
			const XalanNode* 	sourceNode,
			const Locator* 		locator) const = 0;
};



#endif	// STYLESHEETCONSTRUCTIONCONTEXT_HEADER_GUARD_1357924680
