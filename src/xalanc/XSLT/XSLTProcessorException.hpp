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
#if !defined(XSLTPROCESSOREXCEPTION_HEADER_GUARD_1357924680)
#define XSLTPROCESSOREXCEPTION_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XSLT/XSLTDefinitions.hpp>



// Base class header file...
#include <xalanc/PlatformSupport/XSLException.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XSLT_EXPORT XSLTProcessorException : public XSLException
{
public:

	/**
	 * Construct an XSLT Processor exception object
	 * 
	 * @param theMessage message to print when exception thrown
	 * @param theURI the URI of the related document, if known
	 * @param theLineNumber the line number of the related document.
	 * @param theColumnNumber the column number of the related document.
	 * @param theType    type of exception string, default is "XSLTProcessorException"
	 */
	XSLTProcessorException(
		const XalanDOMString&	theMessage,
		const XalanDOMString&	theURI,
		int						theLineNumber,
		int						theColumnNumber,
		const XalanDOMString&	theType = XalanDOMString(XALAN_STATIC_UCODE_STRING("XSLTProcessorException")));

	/**
	 * Constructor
	 * 
	 * @param theLocator The locator instance for error reporting.
	 * @param theMessage message to write when exception thrown
	 * @param theType type of exception, default is "XSLTProcessorException"
	 */
	XSLTProcessorException(
			const LocatorType&		theLocator,
			const XalanDOMString&	theMessage,
			const XalanDOMString&	theType = XalanDOMString(XALAN_STATIC_UCODE_STRING("XSLTProcessorException")));

	/**
	 * Construct an XSLT Processor exception object
	 * 
	 * @param theMessage message to print when exception thrown
	 * @param theType    type of exception string, default is "XSLTProcessorException"
	 */
	XSLTProcessorException(
		const XalanDOMString&	theMessage,
		const XalanDOMString&	theType = XalanDOMString(XALAN_STATIC_UCODE_STRING("XSLTProcessorException")));

	virtual
	~XSLTProcessorException();
};



XALAN_CPP_NAMESPACE_END



#endif	// XSLTPROCESSOREXCEPTION_HEADER_GUARD_1357924680
