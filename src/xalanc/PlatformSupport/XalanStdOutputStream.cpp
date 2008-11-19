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
// Class header file...
#include "XalanStdOutputStream.hpp"



#include <cerrno>



#if !defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream>
#endif



#include "DOMStringHelper.hpp"
#include "XalanMessageLoader.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XalanStdOutputStream::XalanStdOutputStream(StreamType&  theOutputStream,
                                           MemoryManager&  theManager) :
    XalanOutputStream(theManager),
#if !defined(XALAN_NEWLINE_IS_CRLF)
    m_outputStream(theOutputStream)
#else
    m_outputStream(theOutputStream),
    m_newlineString(0),
    m_newlineStringLength(0)
#endif
{
    // This will make sure that cerr is not buffered...
    if (&m_outputStream == &XALAN_STD_QUALIFIER cerr)
    {
        setBufferSize(0);

#if defined(XALAN_NEWLINE_IS_CRLF)
        m_newlineString = s_nlString;
        m_newlineStringLength = s_nlStringLength;
#endif
    }
#if defined(XALAN_NEWLINE_IS_CRLF)
    else if (&m_outputStream == &XALAN_STD_QUALIFIER cout)
    {
        m_newlineString = s_nlString;
        m_newlineStringLength = s_nlStringLength;
    }
    else
    {
        m_newlineString = s_nlCRString;
        m_newlineStringLength = s_nlCRStringLength;
    }
#endif
}



XalanStdOutputStream*
XalanStdOutputStream::create(
            StreamType&     theOutputStream,
            MemoryManager&  theManager)
{
    typedef XalanStdOutputStream    ThisType;

    XalanAllocationGuard    theGuard(theManager, theManager.allocate(sizeof(ThisType)));

    ThisType* const     theResult =
        new (theGuard.get()) ThisType(
                        theOutputStream,
                        theManager);

    theGuard.release();

    return theResult;
}



XalanStdOutputStream::~XalanStdOutputStream()
{
}



#if defined(XALAN_NEWLINE_IS_CRLF)
void
XalanStdOutputStream::newline()
{
    assert(m_newlineString != 0 && length(m_newlineString) == m_newlineStringLength);

    write(m_newlineString, m_newlineStringLength);
}



const XalanDOMChar*
XalanStdOutputStream::getNewlineString() const
{
    assert(m_newlineString != 0);

    return m_newlineString;
}
#endif



void
XalanStdOutputStream::doFlush()
{
    // Don't try to flush if the stream is in a bad state...
    if (m_outputStream)
    {
        m_outputStream.flush();

        if (!m_outputStream)
        {
#if defined(XALAN_STRICT_ANSI_HEADERS)
            using namespace std;
#endif
            XalanDOMString  thebuffer(getMemoryManager());

            throw XalanStdOutputStreamWriteException(errno, thebuffer);
        }
    }
}



void
XalanStdOutputStream::writeData(
            const char*     theBuffer,
            size_type       theBufferLength)
{
    assert(static_cast<XALAN_UINT64>(static_cast<StreamSizeType>(theBufferLength)) == theBufferLength);

    m_outputStream.write(theBuffer, StreamSizeType(theBufferLength));

    if (!m_outputStream)
    {
#if defined(XALAN_STRICT_ANSI_HEADERS)
        using namespace std;
#endif
        XalanDOMString  thebuffer(getMemoryManager());

        throw XalanStdOutputStreamWriteException(errno, thebuffer);
    }
}



static XalanDOMString&
FormatMessageLocal(
            const XalanDOMString&   theMessage,
            int                     theErrorCode,
            XalanDOMString&         theResult)
{
    theResult.assign(theMessage);

    XalanDOMString  theStrErrCode(theResult.getMemoryManager());

    XalanDOMString  theStrErrMsg(theResult.getMemoryManager());

    NumberToDOMString(theErrorCode, theStrErrCode);

    theResult.append(
        XalanMessageLoader::getMessage(
            theStrErrMsg,
            XalanMessages::MessageErrorCodeWas_1Param,
            theStrErrCode));

    return theResult;
}



const XalanDOMChar  XalanStdOutputStream::XalanStdOutputStreamWriteException::m_type[] = 
{   
    XalanUnicode::charLetter_X,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_S,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_d,
    XalanUnicode::charLetter_O,
    XalanUnicode::charLetter_u,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_p,
    XalanUnicode::charLetter_u,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_S,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_m,
    XalanUnicode::charLetter_W,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_E,
    XalanUnicode::charLetter_x,
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_p,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_n,
    0
};



XalanStdOutputStream::XalanStdOutputStreamWriteException::XalanStdOutputStreamWriteException(
        int                 theErrorCode,
        XalanDOMString&     theBuffer) :
    XalanOutputStreamException(
        FormatMessageLocal(
            XalanMessageLoader::getMessage(
                theBuffer,
                XalanMessages::ErrorWritingToStdStream),
        theErrorCode,
        theBuffer),
    theBuffer.getMemoryManager(),
    0)
{
}



XalanStdOutputStream::XalanStdOutputStreamWriteException::~XalanStdOutputStreamWriteException()
{
}



XALAN_CPP_NAMESPACE_END
