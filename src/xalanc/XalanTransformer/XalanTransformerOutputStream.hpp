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
#if !defined(XALANTRANSFORMEROUTPUTSTREAM_HEADER_GUARD_1357924680)
#define XALANTRANSFORMEROUTPUTSTREAM_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XalanTransformer/XalanTransformerDefinitions.hpp>



// Base class header file.
#include <xalanc/PlatformSupport/XalanOutputStream.hpp>



XALAN_CPP_NAMESPACE_BEGIN



// A class for output to a user defined (callback) function.
class XALAN_TRANSFORMER_EXPORT XalanTransformerOutputStream : public XalanOutputStream
{
public:

    /**
     * Construct a XalanTransformerOutputStream instance for output to a
     * user defined (callback) function.
     *
     * @param theOutputHandle   void pointer passed through to callback.
     * @param theOutputHandler  a user defined (callback) function.
     * @param theFlushHandler   a user defined (callback) function.
     */
    XalanTransformerOutputStream(
        MemoryManager&          theManager,
        void*                       theOutputHandle, 
        XalanOutputHandlerType      theOutputHandler,
        XalanFlushHandlerType       theFlushHandler = 0);


    virtual
    ~XalanTransformerOutputStream();

protected:

    virtual void
    writeData(
            const char*         theBuffer,
            size_type           theBufferLength);

    virtual void
    doFlush();

private:    
    
    // These are not implemented...
    XalanTransformerOutputStream(const XalanTransformerOutputStream&);

    XalanTransformerOutputStream&
    operator=(const XalanTransformerOutputStream&);

    void* const             m_outputHandle;

    XalanOutputHandlerType  m_outputHandler;

    XalanFlushHandlerType   m_flushHandler;
};



XALAN_CPP_NAMESPACE_END



#endif  // XALANTRANSFORMEROUTPUTSTREAM_HEADER_GUARD_1357924680


