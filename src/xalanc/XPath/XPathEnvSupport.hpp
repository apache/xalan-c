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
#if !defined(XPATHENVSUPPORT_HEADER_GUARD_1357924680)
#define XPATHENVSUPPORT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



// $$$ ToDo: Necessary while XalanDOMString is style a typedef.
#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



#include <xalanc/XPath/Function.hpp>



XALAN_DECLARE_XERCES_CLASS(Locator)



XALAN_CPP_NAMESPACE_BEGIN



/**
 * @author <a href="mailto:david_n_bertoni@lotus.com">David N. Bertoni</a>
 */
class NodeRefListBase;
class PrefixResolver;
class XObject;
class XPathExecutionContext;
class XObjectFactory;
class XalanDocument;
class XalanElement;
class XalanNode;
class XalanQName;



class XALAN_XPATH_EXPORT XPathEnvSupport
{
public:

	typedef XERCES_CPP_NAMESPACE_QUALIFIER Locator	LocatorType;

	typedef Function::XObjectArgVectorType	XObjectArgVectorType;

	XPathEnvSupport();

	virtual
	~XPathEnvSupport();

	/**
	 * Provides support for XML parsing service.
	 *
	 * @param urlString location of the XML
	 * @param base base location for URI
	 * @return parsed document
	 */
	virtual XalanDocument*
	parseXML(
            MemoryManagerType&      theManager,
			const XalanDOMString&	urlString,
			const XalanDOMString&	base) = 0;

	/**
	 * Get the source document for the given URI.
	 *
	 * @param theURI document URI
	 * @return source document
	 */
	virtual XalanDocument*
	getSourceDocument(const XalanDOMString&		theURI) const = 0;

	/**
	 * Associate a document with a given URI.
	 *
	 * @param theURI      document URI
	 * @param theDocument source document
	 */
	virtual void
	setSourceDocument(
			const XalanDOMString&	theURI,
			XalanDocument*			theDocument) = 0;

	/**
	 * Given a DOM Document, tell what URI was used to parse it. Needed for
	 * relative resolution.
	 *
	 * @param owner source document
	 * @return document URI
	 */
	virtual const XalanDOMString&
	findURIFromDoc(const XalanDocument*     owner) const = 0;

	/**
	 * Determine if an external element is available.
	 *
	 * @param theNamespace  namespace for the element
	 * @param elementName name of extension element
	 * @return whether the element is available or not
	 */
	virtual bool
	elementAvailable(
			const XalanDOMString&	theNamespace, 
			const XalanDOMString&	elementName) const = 0;

	/**
	 * Determine if a function is available.  For standard
	 * function availability, theNamespace should be an
	 * empty string.
	 *
	 * @param theNamespace  namespace for the function
	 * @param functionName name of the function
	 * @return whether the function is available or not
	 */
	virtual bool
	functionAvailable(
			const XalanDOMString&	theNamespace, 
			const XalanDOMString&	functionName) const = 0;

	/**
	 * Handle an extension function.
	 * 
	 * @param executionContext  current execution context
	 * @param theNamespace  namespace of function    
	 * @param functionName extension function name
	 * @param argVec        vector of arguments to function
	 * @param locator A LocatorType instance for error reporting.
	 * @return pointer to XObject result
	 */
	virtual XObjectPtr
	extFunction(
			XPathExecutionContext&			executionContext,
			const XalanDOMString&			theNamespace,
			const XalanDOMString&			functionName, 
			XalanNode*						context,
			const XObjectArgVectorType&		argVec,
			const LocatorType*				locator) const = 0;

	enum eSource { eXMLParser		= 1,
				   eXSLTProcessor	= 2,
				   eXPATHParser		= 3,
				   eXPATHProcessor	= 4,
				   eDataSource		= 5 };

	enum eClassification { eMessage = 0,
						   eWarning = 1,
						   eError = 2 };

	/**
	 * Function that is called when a problem event occurs.
	 * 
	 * @param where 			either eXMLParser, eXSLTProcessor,
	 *			 			      eXPATHParser, eXPATHProcessor, or eDataSource.
	 * @param classification	either eWarning, or eError
	 * @param resolver       resolver for namespace resolution
	 * @param sourceNode     source tree node where the problem occurred
	 *                       (may be 0)
	 * @param msg            string message explaining the problem.
	 * @param   uri				  the URI of the stylesheet, if available.  May be 0;
	 * @param lineNo         line number where the problem occurred.
	 * @param charOffset     character offset where the problem.
	 * @return true if the return is an ERROR, in which case exception will be
	 *         thrown.  Otherwise the processor will continue to process.
	 */
	virtual bool
	problem(
			eSource					where,
			eClassification			classification,
			const PrefixResolver*	resolver,
			const XalanNode*		sourceNode,
			const XalanDOMString&	msg,
			const XalanDOMChar*		uri,
			int						lineNo,
			int						charOffset) const = 0;

	/**
	 * Reset the instance.
	 */
	virtual void
	reset() = 0;

private:

	// These are not implemented...
	XPathEnvSupport(const XPathEnvSupport&);

	XPathEnvSupport&
	operator=(const XPathEnvSupport&);

	bool
	operator==(const XPathEnvSupport&) const;
};



XALAN_CPP_NAMESPACE_END



#endif	// XPATHENVSUPPORT_HEADER_GUARD_1357924680
