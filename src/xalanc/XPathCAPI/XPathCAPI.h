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
