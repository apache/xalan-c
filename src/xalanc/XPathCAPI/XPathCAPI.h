/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2003 The Apache Software Foundation.  All rights 
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
 */
#if !defined(XALAN_XPATHCAPI_HEADER_GUARD_1357924680)
#define XALAN_XPATHCAPI_HEADER_GUARD_1357924680



#if defined(_MSC_VER)

#if defined(XALAN_XPATHCAPI_BUILD_DLL)

#define XALAN_XPATHCAPI_EXPORT __declspec(dllexport)
#define XALAN_XPATHCAPI_EXPORT_FUNCTION(T) T __declspec(dllexport)

#else

#define XALAN_XPATHCAPI_EXPORT __declspec(dllimport)
#define XALAN_XPATHCAPI_EXPORT_FUNCTION(T) T __declspec(dllimport)

#endif

#else

#define XALAN_XPATHCAPI_EXPORT
#define XALAN_XPATHCAPI_EXPORT_FUNCTION(T) T

#endif



/**
 * This is a simple C interface for the class XPathEvaluator. It's 
 * the user's responsibility to call XalanXPathAPIInitialize() before making
 * any other API calls, and to call XalanXPathAPITerminate() when finished
 * with the API.
 */
#if defined(__cplusplus)
extern "C"
{
#endif

#define XALAN_XPATH_API_SUCCESS						0
#define XALAN_XPATH_API_ERROR_ALREADY_INITIALIZED	1
#define XALAN_XPATH_API_ERROR_ALREADY_TERMINATED	2
#define XALAN_XPATH_API_ERROR_INITIALIZATION_FAILED	3
#define XALAN_XPATH_API_ERROR_TERMINATION_FAILED	4
#define XALAN_XPATH_API_ERROR_NOT_INITIALIZED		5
#define XALAN_XPATH_API_ERROR_CANNOT_REINITIALIZE	6
#define XALAN_XPATH_API_ERROR_INVALID_PARAMETER		7
#define XALAN_XPATH_API_ERROR_INVALID_EXPRESSION	8
#define XALAN_XPATH_API_ERROR_BAD_XML				9
#define XALAN_XPATH_API_ERROR_UNSUPPORTED_ENCODING	10
#define XALAN_XPATH_API_ERROR_TRANSCODING			11
#define XALAN_XPATH_API_ERROR_INVALID_XPATH			12
#define XALAN_XPATH_API_ERROR_UNKNOWN				13


/**
 * Handle used to store the address of XalanTransformer instance.
 */
typedef void*	XalanXPathEvaluatorHandle;

/**
 * Handle used to store the address of compiled XPath instance.
 */
typedef void*	XalanXPathHandle;


#if defined(OS390)
#pragma export(XalanXPathAPIInitialize)
#pragma export(XalanXPathAPITerminate)
#pragma export(XalanCreateXPathEvaluator)
#pragma export(XalanDestroyXPathEvaluator)
#pragma export(XalanCreateXPath)
#pragma export(XalanDestroyXPath)
#pragma export(XalanEvaluateXPathAsBoolean)
#pragma export(XalanEvaluateXPathExpressionAsBoolean)
#endif

/**
 * Initialize Xerces and the XPath API.
 *
 * Should be called only once per process before making
 * any other API calls.
 *
 * @return 0 for success, or a non-zero error code upon failure
 */
XALAN_XPATHCAPI_EXPORT_FUNCTION(int)
XalanXPathAPIInitialize();

/**
 * Terminate Xerces and the XPath API.
 *
 * Should be called only once per process after deleting all
 * instances of XalanXPathEvaluator.  Once a process has called
 * this function, it cannot use the API for the remaining
 * lifetime of the process.
 *
 * @return 0 for success, or a non-zero error code upon failure
 */
XALAN_XPATHCAPI_EXPORT_FUNCTION(int)
XalanXPathAPITerminate();

/**
 * Create a XalanXPathEvaluator instance.
 *
 * @param theXalanHandle A pointer to the handle for the new instance.	 
 * @return 0 for success, or a non-zero error code upon failure
 */
XALAN_XPATHCAPI_EXPORT_FUNCTION(int)
XalanCreateXPathEvaluator(XalanXPathEvaluatorHandle*	theHandle);

/**
 * Destroy a XalanXPathEvaluator instance.
 * 
 * @param theXalanHandle The handle of the instance to destroy.
 * @return 0 for success, or a non-zero error code upon failure
 */
XALAN_XPATHCAPI_EXPORT_FUNCTION(int)
XalanDestroyXPathEvaluator(XalanXPathEvaluatorHandle	theXalanHandle);

/**
 * Creates a compiled XPath instance.
 *
 * @param theXalanHandle The handle of XalanXPathEvaluatorHandle instance.	 
 * @param theXPathExpression The expression to compile
 * @param theXPathExpressionEncoding The encoding of the expression.  If NULL or an empty string, the local code page is assumed.
 * @param theXPathHandle A pointer to the handle for the new instance
 * @return 0 for success, or a non-zero error code upon failure
 */
XALAN_XPATHCAPI_EXPORT_FUNCTION(int)
XalanCreateXPath(
			XalanXPathEvaluatorHandle	theXalanHandle,
			const char*					theXPathExpression,
			const char*					theXPathExpressionEncoding,
			XalanXPathHandle*			theXPathHandle);

/**
 * Destroys a compiled XPath instance.
 *
 * @param theXalanHandle The handle of XalanXPathEvaluatorHandle instance.	 
 * @param theXPathHandle The handle of XPath instance
 * @return 0 for success, or a non-zero error code upon failure
 */
XALAN_XPATHCAPI_EXPORT_FUNCTION(int)
XalanDestroyXPath(
			XalanXPathEvaluatorHandle	theXalanHandle,
			XalanXPathHandle			theXPathHandle);

/**
 * Evaluates a compiled XPath instance.
 *
 * @param theXalanHandle The handle of XalanXPathEvaluatorHandle instance.	 
 * @param theXPathHandle The pointer to handle for instance
 * @param theXML The XML document to use for evaluating the XPath expression
 * @param theResult A pointer to an int to receive the result.
 * @return 0 for success, or a non-zero error code upon failure
 */
XALAN_XPATHCAPI_EXPORT_FUNCTION(int)
XalanEvaluateXPathAsBoolean(
			XalanXPathEvaluatorHandle	theXalanHandle,
			XalanXPathHandle			theXPathHandle,
			const char*					theXML,
			int*						theResult);

/**
 * Evaluates an XPath expression
 *
 * @param theXalanHandle The handle of XalanXPathEvaluatorHandle instance.	 
 * @param theXPathExpression The expression to evaluate
 * @param theXPathExpressionEncoding The encoding of the expression.  If NULL or an empty string, the local code page is assumed.
 * @param theXML The XML document to use for evaluating the XPath expression
 * @param theResult A pointer to an int to receive the result.
 * @return 0 for success, or a non-zero error code upon failure
 */
XALAN_XPATHCAPI_EXPORT_FUNCTION(int)
XalanEvaluateXPathExpressionAsBoolean(
			XalanXPathEvaluatorHandle	theXalanHandle,
			const char*					theXPathExpression,
			const char*					theXPathExpressionEncoding,
			const char*					theXML,
			int*						theResult);

#if defined(__cplusplus)
}
#endif



#endif
