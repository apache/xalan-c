/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2000-2002 The Apache Software Foundation.  All rights 
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
#if !defined(XALAN_CAPI_HEADER_GUARD_1357924680)
#define XALAN_CAPI_HEADER_GUARD_1357924680



#include <XalanTransformer/XalanTransformerDefinitions.hpp>



/**
 * This is a simple C interface for the class XalanTransformer. It's 
 * the user's responsibility to call XalanInitialize() before making
 * any other API calls, and to call XalanTerminate() when finished
 * with the API.
 *
 * After calling XalanTransformToData or XalanTransformToDataCSS, the user   
 * should call XalanFreeData to release the memory allocated by that 
 * operation.
 */
#if defined(__cplusplus)
extern "C"
{
#endif

	/**
	 * Handle used to store the address of XalanTransformer instance.
	 */
	typedef void*			XalanHandle;

	/**
	 * Handle used to store the address of Compiled Stylesheet instance.
	 */
	typedef const void*		XalanCSSHandle;

	/**
	 * Handle used to store the address of Parsed Source instance.
	 */
	typedef const void*		XalanPSHandle;

	/**
	 * This is a typedef to work around limitations with
	 * the XALAN_TRANSFORMER_EXPORT_FUNCTION macro.
	 */
	typedef const char*		XalanCCharPtr;

	/**
	 * Initialize Xerces and Xalan.
	 *
	 * Should be called only once per process before making
	 * any other API calls.
	 *
	 * @return 0 if successful, -1 if initialization fails.
	 */
	XALAN_TRANSFORMER_EXPORT_FUNCTION(int)
#if defined(__cplusplus)
	XalanInitialize();
#else
	XalanInitialize(void);
#endif

	/**
	 * Terminate Xalan and Xerces.
	 *
	 * Should be called only once per process after deleting all
	 * instances of XalanTransformer.
	 *
	 * Once a process has called this function, it cannot use the
	 * API until another call to XalanInitialize has been made.
	 * 
	 * Optionally, if the ICU has been integrated, this will
	 * call the ICU clean up function.  This must only be done
	 * if the ICU will no longer be used by the process, since
	 * the ICU will no longer be in a usable state.  See the
	 * ICU documentation for more details.
	 *
	 * This is handy when using leak-detection software, as all
	 * static data allocated by Xalan (and optionally, the ICU)
	 * will be freed.
	 *
	 * @param fCleanUpICU If true, call the ICU clean up function.
	 */
	XALAN_TRANSFORMER_EXPORT_FUNCTION(void)
	XalanTerminate(int	fCleanUpICU);

	/**
	 * Create a XalanTransformer instance.
	 *
	 * @return  the XalanTransformer handle
	 */
	XALAN_TRANSFORMER_EXPORT_FUNCTION(XalanHandle)
#if defined(__cplusplus)
	CreateXalanTransformer();
#else
	CreateXalanTransformer(void);
#endif

	/**
	 * Delete a XalanTransformer instance.
	 * 
	 * @param theXalanHandle The XalanTransformer to destroy.
	 */
	XALAN_TRANSFORMER_EXPORT_FUNCTION(void)
	DeleteXalanTransformer(XalanHandle	theXalanHandle);

	/**
	 * Transform the XML source tree to the given result file.
	 * The processor will apply the stylesheet file to the input
	 * file and write the transformation result to a new output file.
	 *
	 * @param theXMLFileName	filename of XML input source
	 * @param theXSLFileName	filename of stylesheet source
	 * @param theOutFileName	filename of output source
	 * @param theXalanHandle	handle of XalanTransformer instance.
	 * @return	0 for success 
	 */
	XALAN_TRANSFORMER_EXPORT_FUNCTION(int)
	XalanTransformToFile(
				const char*		theXMLFileName, 
				const char*		theXSLFileName,
				const char*		theOutFileName,
				XalanHandle		theXalanHandle);

	/**
	 * Transform the XML source tree to the given result file.
	 * The processor will apply the compiled stylesheet to the input
	 * file and write the transformation result to a new output file.
	 *
	 * @param theParsedSource	handle of parsed source
	 * @param theCSSHandle		handle of compiled stylesheet 
	 * @param theOutFileName	filename of output source
	 * @param theXalanHandle	handle of XalanTransformer instance.
	 * @return	0 for success 
	 */
	XALAN_TRANSFORMER_EXPORT_FUNCTION(int)
	XalanTransformToFilePrebuilt(
			XalanPSHandle	theParsedSource, 
			XalanCSSHandle	theCSSHandle,
			const char*		theOutFileName,
			XalanHandle		theXalanHandle);

	/**
	 * Transform the XML source tree to a dynamically allocated buffer.
	 * The processor will apply the stylesheet file to the input file
	 * and assign the address of the dynamically allocated result to a 
	 * user supplied pointer. The user must call XalanFreeData with the  
	 * this pointer.
	 *
	 * @param theXMLFileName	filename of XML input source
	 * @param theXSLFileName	filename of stylesheet source
	 * @param theOutput			a pointer to a char* 
	 * @param theXalanHandle	handle of XalanTransformer instance.
	 * @return	0 for success 
	 */
	XALAN_TRANSFORMER_EXPORT_FUNCTION(int) 
	XalanTransformToData(
			const char*		theXMLFileName, 
			const char*		theXSLFileName,
			char**			theOutput,
			XalanHandle		theXalanHandle);

	/**
	 * Transform the XML source tree to a dynamically allocated buffer.
	 * The processor will apply the compiled stylesheet to the input file
	 * and assign the address of the dynamically allocated result to a 
	 * user supplied pointer. The user must call XalanFreeData with the  
	 * this pointer.
	 *
	 * @param theXMLFileName	filename of XML input source
	 * @param theCSSHandle		handle of compiled stylesheet 
	 * @param theOutput			a pointer to a char* 
	 * @param theXalanHandle	handle of XalanTransformer instance.
	 * @return	0 for success 
	 */
	XALAN_TRANSFORMER_EXPORT_FUNCTION(int) 
	XalanTransformToDataPrebuilt(
				XalanPSHandle	theParsedSource,
				XalanCSSHandle	theCSSHandle,
				char**			theOutput,
				XalanHandle		theXalanHandle);

	/**
	 * Free memory allocated as a result of calling
	 * XalanTransformToData.
	 * 
	 * @param theData The address of character data.
	 */
	XALAN_TRANSFORMER_EXPORT_FUNCTION(void)
	XalanFreeData(char*		theData);

	/**
	 * Transform the XML source tree to a callback function.
	 * The processor will apply the stylesheet file to the input file
	 * and allocate the transformation result to a callback function  
	 * in pre-allocated blocks. Once the transformation is complete,
	 * a second callback, to flush the buffer, is called. You can pass
	 * in NULL if you do not wish to implement a flush callback. Xalan 
	 * will release any memory allocated upon termination, and data passed 
	 * to the callback is not guaranteed to be null terminated. 
	 * 
	 * See XalanTransformerDefinitions.hpp for more details.
	 * 
	 * @param theXMLFileName	filename of XML input source
	 * @param theXSLFileName	filename of stylesheet source
	 * @param theXalanHandle	handle of XalanTransformer instance.
	 * @param theOutputHandle	void pointer passed through to callback.
	 * @param theOutputHandler	a user defined (callback) function.
	 * @param theFlushHandler	(can be NULL) a user defined (callback) function.
	 * @return	0 for success 
	 */
	XALAN_TRANSFORMER_EXPORT_FUNCTION(int) 
	XalanTransformToHandler(
				const char*				theXMLFileName, 
				const char*				theXSLFileName,
				XalanHandle				theXalanHandle,
				void*					theOutputHandle, 
				XalanOutputHandlerType	theOutputHandler,
				XalanFlushHandlerType	theFlushHandler);

	/**
	 * Transform the XML source tree to a callback function.
	 * The processor will apply the compiled stylesheet to the input file
	 * and allocate the transformation result to a callback function  
	 * in pre-allocated blocks. Once the transformation is complete,
	 * a second callback, to flush the buffer, is called. You can pass
	 * in NULL if you do not wish to implement a flush callback. Xalan 
	 * will release any memory allocated upon termination, and data passed 
	 * to the callback is not guaranteed to be null terminated. 
	 * 
	 * See XalanTransformerDefinitions.hpp for more details.
	 *
	 * @param thePSHandle		handle of parsed source
	 * @param theCSSHandle		handle of compiled stylesheet 
	 * @param theXalanHandle	handle of XalanTransformer instance.
	 * @param theOutputHandle	void pointer passed through to callback.
	 * @param theOutputHandler	a user defined (callback) function.
	 * @param theFlushHandler	(can be NULL) a user defined (callback) function.
	 * @return	0 for success 
	 */
	XALAN_TRANSFORMER_EXPORT_FUNCTION(int) 
	XalanTransformToHandlerPrebuilt(
				XalanPSHandle			thePSHandle,
				XalanCSSHandle			theCSSHandle,
				XalanHandle				theXalanHandle,
				void*					theOutputHandle,
				XalanOutputHandlerType	theOutputHandler,
				XalanFlushHandlerType	theFlushHandler);

	/**
	 * Creates a compiled stylesheet.
	 *
	 * @param theXSLFileName	filename of stylesheet source
	 * @param theXalanHandle	handle of XalanTransformer instance.
	 * @param theCSSHandle		a pointer to a XalanCSSHandle
	 * @return 0 for success.
	 */
	XALAN_TRANSFORMER_EXPORT_FUNCTION(int)
	XalanCompileStylesheet(
			const char*			theXSLFileName,
			XalanHandle			theXalanHandle,
			XalanCSSHandle*		theCSSHandle);

	/**
	 * Creates a compiled stylesheet.
	 *
	 * @param theXSLFileName The stream that contains the stylesheet xml
	 * @param theXSLStreamLength The length of the stream.
	 * @param theXalanHandle handle of XalanTransformer instance.
	 * @param theCSSHandle a pointer to a XalanCSSHandle
	 * @return 0 for success.
	 */
	XALAN_TRANSFORMER_EXPORT_FUNCTION(int)
	XalanCompileStylesheetFromStream(
			const char*			theXSLStream,
			unsigned long		theXSLStreamLength,
			XalanHandle			theXalanHandle,
			XalanCSSHandle*		theCSSHandle);

	/**
	 * Destroys a compiled stylesheet.
	 *
	 * @param theCSSHandle		handle of the compiled stylesheet
	 * @param theXalanHandle	handle of XalanTransformer instance.
	 * @return 0 for success
	 */
	XALAN_TRANSFORMER_EXPORT_FUNCTION(int)
	XalanDestroyCompiledStylesheet(
			XalanCSSHandle	theCSSHandle,
			XalanHandle		theXalanHandle);

	/**
	 * Parse source document.
	 *
	 * @param theXMLFileName The name of the file containing the source document.
	 * @param theXalanHandle The handle of XalanTransformer instance.	 
	 * @param thePSHandle A pointer to a XalanPSHandle
	 * @return 0 for success.
	 */	
	XALAN_TRANSFORMER_EXPORT_FUNCTION(int)
	XalanParseSource(
			const char*		theXMLFileName,
			XalanHandle		theXalanHandle,
			XalanPSHandle*	thePSHandle);

	/**
	 * Parse source document from a stream.
	 *
	 * @param theXMLStream The stream that contains the source xml
	 * @param theXSLStreamLength The length of the stream.
	 * @param theXalanHandle The handle of XalanTransformer instance.	 
	 * @param thePSHandle A pointer to a XalanPSHandle
	 * @return 0 for success.
	 */	
	XALAN_TRANSFORMER_EXPORT_FUNCTION(int)
	XalanParseSourceFromStream(
			const char*		theXMLStream,
			unsigned long	theXMLStreamLength,
			XalanHandle		theXalanHandle,
			XalanPSHandle*	thePSHandle);

	/**
	 * Destroys a parsed source.
	 * a file name, a stream or a root node.
	 *
	 * @param thePSHandle		handle of parsed source
	 * @param theXalanHandle	handle of XalanTransformer instance.	 
	 * @return 0 for success
	 */	
	XALAN_TRANSFORMER_EXPORT_FUNCTION(int)
	XalanDestroyParsedSource(
			XalanPSHandle	thePSHandle,
			XalanHandle		theXalanHandle);

	/**
	 * Set a top-level stylesheet parameter.  This value can be evaluated via
	 * xsl:param-variable.
	 *
	 * @param key name of the param
	 * @param expression expression that will be evaluated
	 * @param theXalanHandle	handle of XalanTransformer instance.
	 */
	XALAN_TRANSFORMER_EXPORT_FUNCTION(void)
	XalanSetStylesheetParam(
				const char*		key,
				const char*		expression,
				XalanHandle		theXalanHandle);

	/**
	 * Returns the last error that occurred as a 
	 * result of calling transform.
	 *
	 * The signature for following function is really:
	 * const char*
	 * XalanGetLastError(XalanHandle theXalanHandle) const;
	 *
	 * @return	error message const character pointer.
	 */
	XALAN_TRANSFORMER_EXPORT_FUNCTION(XalanCCharPtr)
	XalanGetLastError(XalanHandle	theXalanHandle);

#if defined(__cplusplus)
}
#endif



#endif	// XALAN_CAPI_HEADER_GUARD_1357924680
