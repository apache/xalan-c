/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
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
#if !defined(STYLESHEETCONSTRUCTIONCONTEXTDEFAULT_HEADER_GUARD_1357924680)
#define STYLESHEETCONSTRUCTIONCONTEXTDEFAULT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XSLT/XSLTDefinitions.hpp>



#if defined(XALAN_AUTO_PTR_REQUIRES_DEFINITION)
#include <XPath/XPathProcessor.hpp>
#endif



#include <Include/XalanAutoPtr.hpp>



// Base class header file...
#include <XSLT/StylesheetConstructionContext.hpp>



#include <set>
#include <vector>



class XPathEnvSupport;
class XPathFactory;
class XPathProcessor;
class XSLTEngineImpl;



/**
 *
 * An default implementation of an abtract class which provides support for
 * constructing the internal representation  of a stylesheet.
 *
 */
class XALAN_XSLT_EXPORT StylesheetConstructionContextDefault : public StylesheetConstructionContext
{
public:

	/*
	 * Construct an instance.  If the stylesheet(s) constructed is/are meant to be reused (a.k.a. "compiled"),
	 * the XObjectFactory and XPathFactory instance must exist for the lifetime of the construction context
	 * and, therefore, for the lifetime of the stylesheet(s).  Otherwise, XObject and XPath instance will be
	 * destroyed when the corresponding factories are destryed, leaving pointers to destroyed objects in the.
	 * stylesheet(s).
	 *
	 * @param processor a reference to an XSLTEngineImpl instance.  Used for error reporting.
	 * @param xpathEnvSupport a reference to an XPathEnvSupport instance.
	 * @param xpathFactory a reference to an XPathFactory instance.  See comments above for important details.
	 *
	 */
	StylesheetConstructionContextDefault(
			XSLTEngineImpl&		processor,
			XPathEnvSupport&	xpathEnvSupport,
			XPathFactory&		xpathFactory);

	virtual
	~StylesheetConstructionContextDefault();

	// These interfaces are inherited from ExecutionContext...

	virtual void
	error(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode = 0,
			const XalanNode*		styleNode = 0) const;

	virtual void
	error(
			const char*			msg,
			const XalanNode* 	sourceNode = 0,
			const XalanNode* 	styleNode = 0) const;

	virtual void
	warn(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode = 0,
			const XalanNode* 		styleNode = 0) const;

	virtual void
	warn(
			const char*			msg,
			const XalanNode* 	sourceNode = 0,
			const XalanNode* 	styleNode = 0) const;

	virtual void
	message(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode = 0,
			const XalanNode* 		styleNode = 0) const;

	virtual void
	message(
			const char*			msg,
			const XalanNode* 	sourceNode = 0,
			const XalanNode* 	styleNode = 0) const;

	// These interfaces are inherited from StylesheetConstructionContext...

	virtual void
	reset();

	virtual StylesheetRoot*
	create(const XalanDOMString&	theBaseIdentifier);

	virtual StylesheetRoot*
	create(const XSLTInputSource&	theInputSource);

	virtual Stylesheet*
	create(
			StylesheetRoot&			theStylesheetRoot,
			const XalanDOMString&	theBaseIdentifier);

	virtual void
	destroy(StylesheetRoot*		theStylesheetRoot);

	virtual int
	getAttrTok(const XalanDOMString&	name) const;

	virtual int
	getAttrTok(const XalanDOMChar*	name) const;

	virtual URLAutoPtrType
	getURLFromString(const XalanDOMString&	urlString);

	virtual XalanDOMString
	getURLStringFromString(const XalanDOMString&	urlString);

	virtual URLAutoPtrType
	getURLFromString(
			const XalanDOMString&	urlString,
			const XalanDOMString&	base);

	virtual XalanDOMString
	getURLStringFromString(
			const XalanDOMString&	urlString,
			const XalanDOMString&	base);

	virtual const XalanDOMString&
	getXSLTNamespaceURI() const;

	virtual XPath*
	createMatchPattern(
			const XalanDOMString&	str,
			const PrefixResolver&	resolver);

	virtual XPath*
	createMatchPattern(
			const XalanDOMChar*		str,
			const PrefixResolver&	resolver);

	virtual XPath*
	createXPath(
			const XalanDOMString&	str,
			const PrefixResolver&	resolver);

	virtual XPath*
	createXPath(
			const XalanDOMChar*		str,
			const PrefixResolver&	resolver);

	virtual const Locator*
	getLocatorFromStack() const;

	virtual void
	pushLocatorOnStack(const Locator*	locator);

	virtual void
	popLocatorStack();

	virtual const XalanDOMString&
	getXalanXSLNameSpaceURL() const;

	virtual XalanDocument*
	parseXML(
			const XalanDOMString&	urlString,
			DocumentHandler*		docHandler, 
			XalanDocument*			docToRegister);

	virtual int
	getElementToken(const XalanDOMString&	name) const;

	virtual double
	getXSLTVersionSupported() const;

#if defined(XALAN_NO_NAMESPACES)
	typedef set<StylesheetRoot*,
				less<StylesheetRoot*> >		StylesheetSetType;
#else
	typedef std::set<StylesheetRoot*>		StylesheetSetType;
#endif

private:

	XSLTEngineImpl&						m_processor;
	XPathEnvSupport&					m_xpathEnvSupport;
	XPathFactory&						m_xpathFactory;

	typedef XalanAutoPtr<XPathProcessor>	XPathProcessAutoPtr;

	XPathProcessAutoPtr					m_xpathProcessor;

	StylesheetSetType					m_stylesheets;
};



#endif	// STYLESHEETCONSTRUCTIONCONTEXTDEFAULT_HEADER_GUARD_1357924680
