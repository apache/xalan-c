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
#if !defined(XALAN_TRANSFORMER_DEFINITIONS_HEADER_GUARD_1357924680)
#define XALAN_TRANSFORMER_DEFINITIONS_HEADER_GUARD_1357924680



#include <Include/PlatformDefinitions.hpp>



#include <stddef.h>



#if defined(XALAN_TRANSFORMER_BUILD_DLL)

#define XALAN_TRANSFORMER_EXPORT XALAN_PLATFORM_EXPORT

#define XALAN_TRANSFORMER_EXPORT_FUNCTION(T) XALAN_PLATFORM_EXPORT_FUNCTION(T)

#else

#define XALAN_TRANSFORMER_EXPORT XALAN_PLATFORM_IMPORT

#define XALAN_TRANSFORMER_EXPORT_FUNCTION(T) XALAN_PLATFORM_IMPORT_FUNCTION(T)

#endif

#if defined(__cplusplus)
extern "C"
{
#endif

	typedef size_t	CallbackSizeType;

/**
 * Callback function passed to the XalanTransformer APIs.
 * Used to process transformation output in blocks of data.
 * Caller is responsible for streaming or copying data to a user  
 * allocated buffer. Caller should not attempt to write to or  
 * free this data.
 *
 * The callback should return the number of bytes written, and
 * this number should match the length received. Otherwise the
 * XalanTransformToHandler function terminates and returns an error status.	
 *
 * static unsigned long xalanOutputHandler(const void* data, unsigned long length, const void *handle);
 *
 * @param data		a block of data
 * @param length	length of block
 * @param handle	handle of XalanTransformer instance.
 * @return number of bytes written
 */
typedef CallbackSizeType (*XalanOutputHandlerType) (const char*, CallbackSizeType, void*);
	
/**
 * Callback function passed to the XalanTransformer APIs.
 * Used to flush the buffer once transform is completed.
 *
 * static void xalanFlushHandler(const void *handle);
 *
 * @param handle	handle of XalanTransformer instance.
 */
typedef void (*XalanFlushHandlerType) (void*);

#if defined(__cplusplus)
}
#endif



#endif	// XALAN_TRANSFORMER_DEFINITIONS_HEADER_GUARD_1357924680
