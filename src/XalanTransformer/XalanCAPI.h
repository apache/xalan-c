/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2000 The Apache Software Foundation.  All rights 
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
 * the responsibility of the caller to call initialize and terminate 
 * before creating and after deleting any XalanTransformer instances 
 * respectively. As well, releasing any memory allocated from calling   
 * XalanTransformToData by calling XalanFreeData.
 */
#if defined(__cplusplus)
extern "C"
{
#endif

	/**
	 * Handle use to store the address of XalanTransformer instance.
	 */
	typedef void* XalanHandle;

	/**
	 * Callback function passed to XalanTransformToHandler. 
	 * Caller is responsible for streaming or coping data to a user  
	 * allocated buffer. Called should not attempt to write to or  
	 * free this data. Xalan will reuse the same buffer and free it 
	 * upon termination.
	 *
	 * The callback should return the number of bytes written and this 
	 * number should match the length received. Otherwise, the function 
	 * XalanTransformToHandler will terminate and return a error status.	
	 *
	 * static unsigned long xalanOutputHandler(const void* data, unsigned long length, const void *handle);
	 *
	 * @param data		a block of data
	 * @param length	length of block
	 * @param handle	handle of XalanTransformer instance.
	 * @return number of bytes written
	 */
	typedef unsigned long (*XalanOutputHandlerType) (const void*, unsigned long, const void*);

	/**
	 * Initialize Xerces and Xalan.
	 * Should be called only once before creating any
	 * instances of XalanTransformer.
	 */
	XALAN_TRANSFORMER_EXPORT_FUNCTION(void)
	XalanInitialize();

	/**
	 * Terminate Xalan and Xerces.
	 * Should be called only once after deleting all
	 * instances of XalanTransformer.
	 */
	XALAN_TRANSFORMER_EXPORT_FUNCTION(void)
	XalanTerminate();

	/**
	 * Create a XalanTransformer instance.
	 *
	 * @return pointer to current handler
	 */
	XALAN_TRANSFORMER_EXPORT_FUNCTION(XalanHandle)
	CreateXalanTransformer();

	/**
	 * Delete a XalanTransformer instance.
	 * 
	 * @param theXalanHandle The XalanTransformer to destroy.
	 */
	XALAN_TRANSFORMER_EXPORT_FUNCTION(void)
	DeleteXalanTransformer(XalanHandle	theXalanHandle);

	/**
	 * Transform the XML source tree to the given result file.
	 * The processor will process the input file, the stylesheet file,
	 * and transform to new output file.
	 *
	 * @param theXMLFileName	filename of XML input source
	 * @param theXSLFileName	filename of stylesheet source
	 * @param theOutFileName	filename of output source
	 * @param theXalanHandle	handle of XalanTransformer instance.
	 * @return	0 for success and 1 for failure 
	 */
	XALAN_TRANSFORMER_EXPORT_FUNCTION(int)
	XalanTransformToFile(
				const char*				theXMLFileName, 
				const char*				theXSLFileName,
				const char*				theOutFileName,
				XalanHandle				theXalanHandle);

	/**
	 * Transform the XML source tree to a dynamically allocated buffer.
	 * The processor will process the input file, the stylesheet file,
	 * and assign the address of dynamically allocated result to a 
	 * user defined pointer. The user must call XalanFreeData with the  
	 * address of this pointer.
	 *
	 * @param theXMLFileName	filename of XML input source
	 * @param theXSLFileName	filename of stylesheet source
	 * @param theOutFileName	filename of output source
	 * @param theXalanHandle	handle of XalanTransformer instance.
	 * @return	0 for success and 1 for failure 
	 */
	XALAN_TRANSFORMER_EXPORT_FUNCTION(int) 
	XalanTransformToData(
				const char*				theXMLFileName, 
				const char*				theXSLFileName,
				char**					theOutput,
				XalanHandle				theXalanHandle);

	/**
	 * Free memory allocated as a result of calling
	 * XalanTransformToData.
	 * 
	 * @param theStream The address of character data.
	 */
	XALAN_TRANSFORMER_EXPORT_FUNCTION(void)
	XalanFreeData(char*		theStream);

	/**
	 * Transform the XML source tree to the address of a callback.
	 * The processor will process the input file, the stylesheet file,
	 * and transform to output to a callback function in allocated 
	 * blocks. Xalan will release any memory allocated upon 
	 * termination. Data passed to the callback is not guaranteed to 
	 * be null terminated. - See XalanOutputHandlerType for more details.
	 * 
	 * @param theXMLFileName	filename of XML input source
	 * @param theXSLFileName	filename of stylesheet source
	 * @param theXalanHandle	handle of XalanTransformer instance.
	 * @param theOutputHandle	void pointer passed through to callback.
	 * @param theOutputHandler	address of a user defined (callback) function.
	 * @return	0 for success and 1 for failure 
	 */
	XALAN_TRANSFORMER_EXPORT_FUNCTION(int) 
	XalanTransformToHandler(
				const char*				theXMLFileName, 
				const char*				theXSLFileName,
				XalanHandle				theXalanHandle,
				const void*				theOutputHandle, 
				XalanOutputHandlerType	theOutputHandler);

#if defined(__cplusplus)
}
#endif



#endif	// XALAN_CAPI_HEADER_GUARD_1357924680




