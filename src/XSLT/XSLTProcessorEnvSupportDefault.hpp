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
#if !defined(XSLTPROCESSORENVSUPPORTDEFAULT_HEADER_GUARD_1357924680)
#define XSLTPROCESSORENVSUPPORTDEFAULT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XSLT/XSLTDefinitions.hpp>



// Base class header file...
#include <XSLT/XSLTProcessorEnvSupport.hpp>



#include <XPath/XPathEnvSupportDefault.hpp>



class XSLTProcessor;



class XALAN_XSLT_EXPORT XSLTProcessorEnvSupportDefault : public XSLTProcessorEnvSupport
{
public:

	XSLTProcessorEnvSupportDefault(XSLTProcessor*	theProcessor = 0);

	virtual
	~XSLTProcessorEnvSupportDefault();

	/**
	 * Reset the XLST processor instance
	 * 
	 * @param theProcessor pointer to new processor instance
	 */
	void
	setProcessor(XSLTProcessor*		theProcessor)
	{
		m_processor = theProcessor;
	}


	/**
	 * Install an external function in the global space.
	 *
	 * @param theNamespace The namespace for the functionl
	 * @param functionName The name of the function.
	 * @param function The function to install.
	 */
	static void
	installExternalFunctionGlobal(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	functionName,
			const Function&			function);

	/**
	 * Uninstall an external function from the global space.
	 *
	 * @param theNamespace The namespace for the functionl
	 * @param functionName The name of the function.
	 */
	static void
	uninstallExternalFunctionGlobal(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	functionName);

	// Interfaces to install and uninstall external functions in this instance.

	/**
	 * Install an external function in the local space.
	 *
	 * @param theNamespace The namespace for the functionl
	 * @param functionName The name of the function.
	 * @param function The function to install.
	 */
	virtual void
	installExternalFunctionLocal(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	functionName,
			const Function&			function);

	/**
	 * Uninstall an external function from the local space.
	 *
	 * @param theNamespace The namespace for the functionl
	 * @param functionName The name of the function.
	 */
	virtual void
	uninstallExternalFunctionLocal(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	functionName);

	// These interfaces are inherited from XPathEnvSupport...

	virtual XalanDocument*
	parseXML(
			const XalanDOMString&	urlString,
			const XalanDOMString&	base);

	virtual XalanDocument*
	getSourceDocument(const XalanDOMString&		theURI) const;

	virtual void
	setSourceDocument(
			const XalanDOMString&	theURI,
			XalanDocument*			theDocument);

	virtual XalanDOMString
	findURIFromDoc(const XalanDocument*		owner) const;

	virtual XalanDocument*
	getDOMFactory() const;

	virtual bool
	elementAvailable(
			const XalanDOMString&	theNamespace, 
			const XalanDOMString&	elementName) const;

	virtual bool
	functionAvailable(
			const XalanDOMString&	theNamespace, 
			const XalanDOMString&	functionName) const;

	virtual XObjectPtr
	extFunction(
			XPathExecutionContext&			executionContext,
			const XalanDOMString&			theNamespace,
			const XalanDOMString&			functionName,
			XalanNode*						context,
			const XObjectArgVectorType&		argVec) const;

	virtual bool
	problem(
			eSource					where,
			eClassification			classification,
			const XalanNode*		styleNode,
			const XalanNode*		sourceNode,
			const XalanDOMString&	msg,
			const XalanDOMChar*		uri,
			int						lineNo,
			int						charOffset) const;

	virtual bool
	problem(
			eSource					where,
			eClassification			classification,
			const PrefixResolver*	resolver,
			const XalanNode*		sourceNode,
			const XalanDOMString&	msg,
			const XalanDOMChar*		uri,
			int						lineNo,
			int						charOffset) const;

	// These interfaces are inherited from Resettable...

	virtual void
	reset();


	// These are not implemented...
	XSLTProcessorEnvSupportDefault(const XSLTProcessorEnvSupportDefault&);

	XSLTProcessorEnvSupportDefault&
	operator=(const XSLTProcessorEnvSupportDefault&);

	bool
	operator==(const XSLTProcessorEnvSupportDefault&) const;


	// Data members...

	XPathEnvSupportDefault		m_defaultSupport;

	XSLTProcessor*				m_processor;
};



#endif	// XSLTPROCESSORENVSUPPORTDEFAULT_HEADER_GUARD_1357924680
