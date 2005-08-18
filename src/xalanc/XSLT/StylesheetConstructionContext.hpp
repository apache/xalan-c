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
#if !defined(STYLESHEETCONSTRUCTIONCONTEXT_HEADER_GUARD_1357924680)
#define STYLESHEETCONSTRUCTIONCONTEXT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XSLT/XSLTDefinitions.hpp>



#include <xalanc/PlatformSupport/URISupport.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/XPath/XalanQName.hpp>
#include <xalanc/XPath/XPath.hpp>
#include <xalanc/XPath/XPathConstructionContext.hpp>



XALAN_DECLARE_XERCES_CLASS(AttributeList)
XALAN_DECLARE_XERCES_CLASS(DocumentHandler)



XALAN_CPP_NAMESPACE_BEGIN



typedef XERCES_CPP_NAMESPACE_QUALIFIER AttributeList	AttributeListType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DocumentHandler	DocumentHandlerType;



class AVT;
class AVTPart;
class ElemTemplate;
class ElemTemplateElement;
class ExtensionNSHandler;
class PrefixResolver;
class Stylesheet;
class StylesheetRoot;
class XalanDocument;
class XalanMatchPatternData;
class XalanNode;
class XSLTInputSource;



//
// An abstract class which provides support for constructing the internal
// representation  of a stylesheet.
//
class XALAN_XSLT_EXPORT StylesheetConstructionContext : public XPathConstructionContext
{
public:

	typedef XalanQName::NamespacesStackType		NamespacesStackType;

#if defined(XALAN_STRICT_ANSI_HEADERS)
	typedef std::size_t		size_type;
#else
	typedef size_t			size_type;
#endif

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

		// This one doesn't really have a name.
		// It's used for forward-compatible
		// processing.
		ELEMNAME_FORWARD_COMPATIBLE,

		// Extension handling.
		ELEMNAME_EXTENSION_CALL,
		ELEMNAME_EXTENSION_HANDLER
	};

	typedef URISupport::URLAutoPtrType	URLAutoPtrType;

	explicit
	StylesheetConstructionContext(MemoryManagerType&  theManager);

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
	virtual XalanDOMString&
	getURLStringFromString(const XalanDOMString&	urlString,
                            XalanDOMString& theResult) = 0;

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
	virtual XalanDOMString&
	getURLStringFromString(
			const XalanDOMString&	urlString,
			const XalanDOMString&	base,
            XalanDOMString& theResult) = 0;

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
     * @param allowVariableReferences If true, variable references are allowed.
     * @param allowKeyFunction If true, calls to the key() function are allowed.
	 * @return XPath for match pattern
	 */
	virtual XPath*
	createMatchPattern(
			const LocatorType*		locator,
			const XalanDOMString&	str,
			const PrefixResolver&	resolver,
            bool                    allowVariableReferences = true,
            bool                    allowKeyFunction = true) = 0;

	/**
	 * Create and initialize an xpath for a match pattern and return it. This
	 * is to be used by stylesheet elements that need an XPath that is
	 * guaranteed to persist while it lives.
	 *
	 * @param locator the locator for the XPath. May be null.
	 * @param str string to match
	 * @param resolver resolver for namespace resolution
     * @param allowVariableReferences If true, variable references are allowed.
     * @param allowKeyFunction If true, calls to the key() function are allowed.
	 * @return XPath for match pattern
	 */
	virtual XPath*
	createMatchPattern(
			const LocatorType*		locator,
			const XalanDOMChar*		str,
			const PrefixResolver&	resolver,
            bool                    allowVariableReferences = true,
            bool                    allowKeyFunction = true) = 0;

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
			const LocatorType*		locator,
			const XalanDOMString&	str,
			const PrefixResolver&	resolver,
            bool                    allowVariableReferences = true,
            bool                    allowKeyFunction = true) = 0;

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
			const LocatorType*		locator,
			const XalanDOMChar*		str,
			const PrefixResolver&	resolver,
            bool                    allowVariableReferences = true,
            bool                    allowKeyFunction = true) = 0;

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
			const LocatorType*			locator,
			const XalanDOMChar*			str,
			XalanDOMString::size_type	len,
			const PrefixResolver&		resolver,
            bool                        allowVariableReferences = true,
            bool                        allowKeyFunction = true) = 0;

	/**
	 * Get the locator from the top of the locator stack.
	 *
	 * @return A pointer to the Locator, or 0 if there is nothing on the stack.
	 */
	virtual const LocatorType*
	getLocatorFromStack() const = 0;

	/**
	 * Push a locator on to the locator stack.
	 *
	 * @param A pointer to the Locator to push.
	 */
	virtual void
	pushLocatorOnStack(const LocatorType*	locator) = 0;

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
			DocumentHandlerType*	docHandler, 
			XalanDocument*			docToRegister) = 0;

	/**
	 * Given an name, determine if it is the xml:space attribute
	 *
	 * @param theAttributeName The name to check
	 * @param theStylesheet The current Stylesheet instance
	 * @param theLocator The Locator for error reporting.  May be 0.
	 *
	 * @return true if the string is the xml:space attribute name
	 */
	virtual bool
	isXMLSpaceAttribute(
			const XalanDOMChar*		theAttributeName,
			const Stylesheet&		theStylesheet,
			const LocatorType*		theLocator = 0) = 0;

	/**
	 * Given an name, determine if it is the xsl:use-attribute-sets attribute
	 *
	 * @param theAttributeName The name to check
	 * @param theStylesheet The current Stylesheet instance
	 * @param theLocator The Locator for error reporting.  May be 0.
	 *
	 * @return true if the string is the xsl:use-attribute-sets attribute name
	 */
	virtual bool
	isXSLUseAttributeSetsAttribute(
			const XalanDOMChar*		theAttributeName,
			const Stylesheet&		theStylesheet,
			const LocatorType*		theLocator = 0) = 0;

	/**
	 * Given an name, determine if it is a valid QName
	 *
	 * @param theName The name to check
	 * @param theStylesheet The current Stylesheet instance
	 * @param theLocator The Locator for error reporting.  May be 0.
	 *
	 * @return true if the string is a valid QName.
	 */
	virtual bool
	isValidQName(
			const XalanDOMChar*		theName,
			const Stylesheet&		theStylesheet,
			const LocatorType*		theLocator = 0) = 0;

	/**
	 * Given an XSL tag name, return an integer token that corresponds to
	 * the enums defined above.
	 *
	 * @param name a probable xsl:xxx element
	 * @return The enum value for that token, or ELEMNAME_UNDEFINED
	 */
	virtual eElementToken
	getElementToken(const XalanDOMString&	name) const = 0;

	/**
	 * Get the latest XSLT version currently supported.
	 *
	 * @return XSLT version number
	 */
	virtual double
	getXSLTVersionSupported() const = 0;

	virtual const XalanDOMString&
	getPooledString(const XalanDOMString&	theString) = 0;

	virtual const XalanDOMString&
	getPooledString(
			const XalanDOMChar*			theString,
			XalanDOMString::size_type	theLength = XalanDOMString::npos) = 0;

	virtual XalanDOMString&
	getCachedString() = 0;

	virtual bool
	releaseCachedString(XalanDOMString&		theString) = 0;

	/**
	 * Allocate a vector of XalanDOMChar of the specified
	 * size.
	 *
	 * @param theLength The length of the character vector
	 * @return A pointer to the vector.
	 */
	virtual XalanDOMChar*
	allocateXalanDOMCharVector(XalanDOMString::size_type	theLength) = 0;

	/**
	 * Allocate a vector of XalanDOMChar of the specified
	 * size.
	 *
	 * @param theString The source character array
	 * @param theLength The length of the character vector
	 * @param fTerminate If true, terminate the new vector with 0
	 * @return A pointer to the array.
	 */
	virtual XalanDOMChar*
	allocateXalanDOMCharVector(
			const XalanDOMChar*			theString,
			XalanDOMString::size_type	theLength = XalanDOMString::npos,
			bool						fTerminate = true) = 0;

	/**
	 * Create an AVT instance.
	 *
	 * @param locator the Locator for the instance.  May be null.
	 * @param name name of AVT
	 * @param stringedValue string value to parse
	 * @param resolver resolver for namespace resolution
	 * @return A pointer to the instance.
	 */
	virtual const AVT*
	createAVT(
			const LocatorType*		locator,
			const XalanDOMChar*		name,
			const XalanDOMChar*		stringedValue,
			const PrefixResolver&	resolver) = 0;

	/**
	 * Create an AVTPart instance.
	 *
	 * @param theString The source character array
	 * @param theLength The length of the character vector
	 * @param fTerminate If true, terminate the new vector with 0
	 * @return A pointer to the instance.
	 */
	virtual const AVTPart*
	createAVTPart(
			const XalanDOMChar*			theString,
			XalanDOMString::size_type	theLength = XalanDOMString::npos) = 0;

	/**
	 * Create an AVTPart instance.
	 *
	 * @param locator the Locator for the instance.  May be null.
	 * @param str The XPath expression for the instance
	 * @param len The length of the expression
	 * @param resolver resolver for namespace resolution
	 * @return A pointer to the instance.
	 */
	virtual const AVTPart*
	createAVTPart(
			const LocatorType*			locator,
			const XalanDOMChar*			str,
			XalanDOMString::size_type	len,
			const PrefixResolver&		resolver) = 0;

	/**
	 * Allocate a vector of const AVT* of the specified
	 * length.
	 *
	 * @param theLength The length of the vector
	 * @return A pointer to the vector.
	 */
	virtual const AVT**
	allocateAVTPointerVector(size_type	theLength) = 0;

	/**
	 * Allocate a vector of const AVTPart* of the specified
	 * length.
	 *
	 * @param theLength The length of the vector
	 * @return A pointer to the vector.
	 */
	virtual const AVTPart**
	allocateAVTPartPointerVector(size_type	theLength) = 0;

	/**
	 * Create a XalanQName-derived instance.
	 *
	 * @param qname The qname string
	 * @param namespaces The stack of namespaces
	 * @param Locator The current Locator, if any
	 * @param fUseDefault If true, a qname without a prefix will use the default namespace
	 * @return A pointer to the new instance
	 */
	virtual const XalanQName*
	createXalanQName(
			const XalanDOMString&		qname,
			const NamespacesStackType&	namespaces,
			const LocatorType*			locator = 0,
			bool						fUseDefault = false) = 0;

	/**
	 * Create a XalanQName-derived instance.
	 *
	 * @param qname The qname string
	 * @param namespaces The stack of namespaces
	 * @param Locator The current Locator, if any
	 * @param fUseDefault If true, a qname without a prefix will use the default namespace
	 * @return A pointer to the new instance
	 */
	virtual const XalanQName*
	createXalanQName(
			const XalanDOMChar*			qname,
			const NamespacesStackType&	namespaces,
			const LocatorType*			locator = 0,
			bool						fUseDefault = false) = 0;

	/**
	 * Tokenize a string and return the QNames corresponding to
	 * those tokens.
	 *
	 * @param count The number of namespaces in the vector returned
	 * @param qnameTokens The string to tokenize
	 * @param namespaces The stack of namespaces
	 * @param Locator The current Locator, if any
	 * @param fUseDefault If true, qnames without prefixes will use the default namespace
	 * @return The resulting vector of XalanQName instances.
	 */
	virtual const XalanQName**
	tokenizeQNames(
			size_type&					count,
			const XalanDOMChar*			qnameTokens,
			const NamespacesStackType&	namespaces,
			const LocatorType*			locator = 0,
			bool						fUseDefault = false) = 0;

	/**
	 * Create a stylesheet element for the provided type.   The
	 * instance owns the memory and will delete the element when
	 * it goes out of scope and the containing stylesheet is
	 * destroyed.
	 *
	 * @param token The enum value of the element to create.
	 * @param stylesheetTree The stylesheet containing the element
	 * @param atts The list of attributes for the element
	 * @param locator The Locator instance for error reporting.  May be 0.
	 *
	 * @return A pointer to the new instance.
	 */
	virtual ElemTemplateElement*
	createElement(
			int							token,
			Stylesheet&					stylesheetTree,
			const AttributeListType&	atts,
			const LocatorType*			locator = 0) = 0;

	/**
	 * Create a elements which have specific names. The instance
	 * owns the memory and will delete the element when it goes
	 * out of scope and the containing stylesheet is destroyed.
	 *
	 * @param token The enum value of the element to create.
	 * @param stylesheetTree The stylesheet containing the element
	 * @param name The name of the element
	 * @param atts The list of attributes for the element
	 * @param locator The Locator instance for error reporting.  May be 0.
	 *
	 * @return A pointer to the new instance.
	 */
	virtual ElemTemplateElement*
	createElement(
			int							token,
			Stylesheet&					stylesheetTree,
			const XalanDOMChar*			name,
			const AttributeListType&	atts,
			const LocatorType*			locator = 0) = 0;

	/**
	 * Create a an element for literal text. The instance owns the
	 * memory and will delete the element when it goes out of
	 * scope and the containing stylesheet is destroyed.
	 *
	 * @param stylesheetTree The stylesheet containing the element
	 * @param chars The pointer to character string for element
	 * @param length length of the chars parameter.
	 * @param preserveSpace true is space should be preserved
	 * @param disableOutputEscaping true if output escaping should be disabled
	 * @param locator The Locator instance for error reporting.  May be 0.
	 *
	 * @return A pointer to the new instance.
	 */
	virtual ElemTemplateElement*
	createElement(
			Stylesheet&					stylesheetTree,
            const XalanDOMChar*			chars,
			XalanDOMString::size_type	length,
			bool						preserveSpace,
            bool						disableOutputEscaping,
			const LocatorType*			locator = 0) = 0;

	/**
	 * Create an element to handle an extension element.   The
	 * instance owns the memory and will delete the element when
	 * it goes out of scope and the containing stylesheet is
	 * destroyed.
	 *
	 * @param stylesheetTree The stylesheet containing the element
	 * @param name The name of the element
	 * @param atts The list of attributes for the element
	 * @param handler The handler for elements in the extension namespace
	 * @param locator The Locator instance for error reporting.  May be 0.
	 *
	 * @return A pointer to the new instance.
	 */
	virtual ElemTemplateElement*
	createElement(
			Stylesheet&					stylesheetTree,
			const XalanDOMChar*			name,
			const AttributeListType&	atts,
			ExtensionNSHandler&			handler,
			const LocatorType*			locator = 0) = 0;

	/**
	 * Create an instance of XalanMatchPatternData, which describes
	 * data related to a match pattern and template in stylesheet.
	 * The instance owns the memory and will delete the element when
	 * it goes out of scope and the containing stylesheet is
	 * destroyed.
     *
     * @param theTemplate The ElemTemplate node that contains the template for this pattern
     * @param thePosition The position in the stylesheet
     * @param theTargetString The target string for match pattern
     * @param TheMatchPattern The match pattern
     * @param thePatternString the pattern string
     * @param thePriority The priority for the match pattern.
     *
     * @return A pointer to the new instance.
	 */
    virtual const XalanMatchPatternData*
    createXalanMatchPatternData(
            const ElemTemplate&     theTemplate,
            size_type               thePosition,
            const XalanDOMString&   theTargetString,
            const XPath&            theMatchPattern,
            const XalanDOMString&   thePatternString,
            XPath::eMatchScore      thePriority) = 0;


	// These are inherited from XPathConstructionContext...

	virtual void
	error(
			const XalanDOMString&		msg,
			const XalanNode* 			sourceNode = 0,
			const ElemTemplateElement*	styleNode = 0) const = 0;

	virtual void
	error(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode,
			const LocatorType* 		locator) const = 0;

	virtual void
	warn(
			const XalanDOMString&		msg,
			const XalanNode* 			sourceNode = 0,
			const ElemTemplateElement* 	styleNode = 0) const = 0;

	virtual void
	warn(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode,
			const LocatorType* 		locator) const = 0;

	virtual void
	message(
			const XalanDOMString&		msg,
			const XalanNode* 			sourceNode = 0,
			const ElemTemplateElement* 	styleNode = 0) const = 0;

	virtual void
	message(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode,
			const LocatorType* 		locator) const = 0;

};



XALAN_CPP_NAMESPACE_END



#endif	// STYLESHEETCONSTRUCTIONCONTEXT_HEADER_GUARD_1357924680
