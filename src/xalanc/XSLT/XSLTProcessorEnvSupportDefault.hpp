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
#if !defined(XSLTPROCESSORENVSUPPORTDEFAULT_HEADER_GUARD_1357924680)
#define XSLTPROCESSORENVSUPPORTDEFAULT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XSLT/XSLTDefinitions.hpp>



// Base class header file...
#include <xalanc/XSLT/XSLTProcessorEnvSupport.hpp>



#include <xalanc/XPath/XPathEnvSupportDefault.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XSLTProcessor;



class XALAN_XSLT_EXPORT XSLTProcessorEnvSupportDefault : public XSLTProcessorEnvSupport
{
public:

	XSLTProcessorEnvSupportDefault( MemoryManagerType& theManager, 
                                    XSLTProcessor*	theProcessor = 0);

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


	// These interfaces are inherited from XSLTProcessorEnvSupport...

	virtual bool
	problem(
			eSource						where,
			eClassification				classification,
			const XalanNode*			sourceNode,
			const ElemTemplateElement*	styleNode,
			const XalanDOMString&		msg,
			const XalanDOMChar*			uri,
			int							lineNo,
			int							charOffset) const;

	// These interfaces are inherited from XPathEnvSupport...

	virtual XalanDocument*
	parseXML(
            MemoryManagerType&      theManager,
			const XalanDOMString&	urlString,
			const XalanDOMString&	base);

	virtual XalanDocument*
	getSourceDocument(const XalanDOMString&		theURI) const;

	virtual void
	setSourceDocument(
			const XalanDOMString&	theURI,
			XalanDocument*			theDocument);

	virtual const XalanDOMString&
	findURIFromDoc(const XalanDocument*		owner) const;

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
			const XObjectArgVectorType&		argVec,
			const LocatorType*				locator) const;

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



XALAN_CPP_NAMESPACE_END



#endif	// XSLTPROCESSORENVSUPPORTDEFAULT_HEADER_GUARD_1357924680
