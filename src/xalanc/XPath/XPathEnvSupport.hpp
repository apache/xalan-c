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
#if !defined(XPATHENVSUPPORT_HEADER_GUARD_1357924680)
#define XPATHENVSUPPORT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



#include <map>
#include <vector>



// $$$ ToDo: Necessary while XalanDOMString is style a typedef.
#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



#include <xalanc/XPath/Function.hpp>



XALAN_DECLARE_XERCES_CLASS(Locator)



XALAN_CPP_NAMESPACE_BEGIN



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
	virtual XalanDOMString
	findURIFromDoc(const XalanDocument*		owner) const = 0;

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
