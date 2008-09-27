/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the  "License");
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
#if !defined(XALAN_TRANSFORMER_DEFINITIONS_HEADER_GUARD_1357924680)
#define XALAN_TRANSFORMER_DEFINITIONS_HEADER_GUARD_1357924680



#include <xalanc/Include/PlatformDefinitions.hpp>



#include <stddef.h>



#if defined(XALAN_BUILD_DLL)

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

    typedef size_t  CallbackSizeType;

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
 * @param data      a block of data
 * @param length    length of block
 * @param handle    handle of XalanTransformer instance.
 * @return number of bytes written
 */
typedef CallbackSizeType (*XalanOutputHandlerType) (const char*, CallbackSizeType, void*);
    
/**
 * Callback function passed to the XalanTransformer APIs.
 * Used to flush the buffer once transform is completed.
 *
 * static void xalanFlushHandler(const void *handle);
 *
 * @param handle    handle of XalanTransformer instance.
 */
typedef void (*XalanFlushHandlerType) (void*);

#if defined(__cplusplus)
}
#endif



#endif  // XALAN_TRANSFORMER_DEFINITIONS_HEADER_GUARD_1357924680
