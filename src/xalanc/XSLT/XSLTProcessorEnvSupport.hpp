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
#if !defined(XSLTPROCESSORENVSUPPORT_HEADER_GUARD_1357924680)
#define XSLTPROCESSORENVSUPPORT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XSLT/XSLTDefinitions.hpp>



// Base class header file...
#include <xalanc/XPath/XPathEnvSupport.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class ElemTemplateElement;
class XSLTProcessor;



class XALAN_XSLT_EXPORT XSLTProcessorEnvSupport : public XPathEnvSupport
{
public:

	XSLTProcessorEnvSupport();

	virtual
	~XSLTProcessorEnvSupport();


	// These interfaces are new...

	/**
	 * Function that is called when a problem event occurs.
	 * 
	 * @param where 			either eXMLParser, eXSLTProcessor,
	 *			 			      eXPATHParser, eXPATHProcessor, or eDataSource.
	 * @param classification	either eWarning, or eError
	 * @param resolver       resolver for namespace resolution
	 * @param sourceNode     source tree node where the problem occurred
	 *                       (may be 0)
	 * @param styleNode     stylesheet node where the problem occurred
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
			eSource						where,
			eClassification				classification,
			const XalanNode*			sourceNode,
			const ElemTemplateElement*	styleNode,
			const XalanDOMString&		msg,
			const XalanDOMChar*			uri,
			int							lineNo,
			int							charOffset) const = 0;

	// These interfaces are inherited from XPathEnvSupport...

	virtual XalanDocument*
	parseXML(
            MemoryManagerType&      theManager,
			const XalanDOMString&	urlString,
			const XalanDOMString&	base) = 0;

	virtual XalanDocument*
	getSourceDocument(const XalanDOMString&		theURI) const = 0;

	virtual void
	setSourceDocument(
			const XalanDOMString&	theURI,
			XalanDocument*			theDocument) = 0;

	virtual const XalanDOMString&
	findURIFromDoc(const XalanDocument*		owner) const = 0;

	virtual bool
	elementAvailable(
			const XalanDOMString&	theNamespace, 
			const XalanDOMString&	elementName) const = 0;

	virtual bool
	functionAvailable(
			const XalanDOMString&	theNamespace, 
			const XalanDOMString&	functionName) const = 0;

	virtual XObjectPtr
	extFunction(
			XPathExecutionContext&			executionContext,
			const XalanDOMString&			theNamespace,
			const XalanDOMString&			functionName,
			XalanNode*						context,
			const XObjectArgVectorType&		argVec,
			const LocatorType*				locator) const = 0;

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

	// These interfaces are inherited from Resettable...

	virtual void
	reset() = 0;

private:

	// These are not implemented...
	XSLTProcessorEnvSupport(const XSLTProcessorEnvSupport&);

	XSLTProcessorEnvSupport&
	operator=(const XSLTProcessorEnvSupport&);

	bool
	operator==(const XSLTProcessorEnvSupport&) const;
};



XALAN_CPP_NAMESPACE_END



#endif	// XSLTPROCESSORENVSUPPORT_HEADER_GUARD_1357924680
