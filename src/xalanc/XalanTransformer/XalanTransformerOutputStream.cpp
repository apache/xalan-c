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
#include "XalanTransformerOutputStream.hpp"



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XalanTransformerOutputStream::XalanTransformerOutputStream(
    MemoryManager&          theManager,
    void*                       theOutputHandle, 
    XalanOutputHandlerType      theOutputHandler,
    XalanFlushHandlerType       theFlushHandler):
    XalanOutputStream(theManager),
    m_outputHandle(theOutputHandle),
    m_outputHandler(theOutputHandler),
    m_flushHandler(theFlushHandler)
{
}



XalanTransformerOutputStream::~XalanTransformerOutputStream()
{
}



void
XalanTransformerOutputStream::doFlush()
{
    if(m_flushHandler != 0)
    {
        m_flushHandler(m_outputHandle);
    }
}



void
XalanTransformerOutputStream::writeData(
            const char*     theBuffer,
            size_type       theBufferLength)
{
    const size_t theBytesWritten = m_outputHandler(theBuffer, theBufferLength, m_outputHandle);     

    // We validate that the number of bytes written equals the number of bytes sent to
    // the output handler. Otherwise we will stop processing  and throw an exception. 
    // Thus the callback can alert us of memory allocation issues or buffer overflows.
    if(theBytesWritten != theBufferLength)
    {
        XalanDOMString  theBuffer(getMemoryManager());

        throw XalanOutputStreamException(
            XalanMessageLoader::getMessage(
                theBuffer,
                XalanMessages::NumberBytesWrittenDoesNotEqual),
            getMemoryManager(),
            0);
    }
}




XALAN_CPP_NAMESPACE_END
