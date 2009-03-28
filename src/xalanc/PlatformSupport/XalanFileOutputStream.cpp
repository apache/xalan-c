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
#include "XalanFileOutputStream.hpp"



#if !defined(XALAN_WINDOWS)
#include <cerrno>
#endif



#include "DOMStringHelper.hpp"
#include "XalanMessageLoader.hpp"



XALAN_CPP_NAMESPACE_BEGIN

const XalanDOMChar  XalanFileOutputStream::XalanFileOutputStreamOpenException::m_type[] = 
{   
    XalanUnicode::charLetter_X,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_F,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_e,
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
    XalanUnicode::charLetter_O,
    XalanUnicode::charLetter_p,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_n,
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


const XalanDOMChar  XalanFileOutputStream::XalanFileOutputStreamWriteException::m_type[] = 
{   
    XalanUnicode::charLetter_X,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_F,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_e,
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


static  XalanFileOutputStream::HandleType
openFile(
            const XalanDOMString&   theFileName,
            MemoryManager&          theManager)
{
    typedef XalanFileOutputStream::HandleType   HandleType;

#if defined(XALAN_WINDOWS)
    HandleType  theFileHandle = CreateFileW(
            theFileName.c_str(),
            GENERIC_WRITE,
            0,
            0,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            0);

    if (theFileHandle != INVALID_HANDLE_VALUE && theFileHandle != 0)
    {
        return theFileHandle;
    }
    else
    {
        
        CharVectorType  theResult(theManager);

        TranscodeToLocalCodePage(theFileName, theResult, true);

        if (theResult.empty() == true)
        {
            return INVALID_HANDLE_VALUE;
        }
        else
        {
            const char* const   tmpName = &theResult[0];

            if (tmpName == 0)
            {
                return INVALID_HANDLE_VALUE;
            }
            else
            {
                return CreateFile(
                            tmpName,
                            GENERIC_WRITE,
                            0,
                            0,
                            CREATE_ALWAYS,
                            FILE_ATTRIBUTE_NORMAL,
                            0);
            }
        }
    }
#else
#if defined(XALAN_STRICT_ANSI_HEADERS)
    using std::fopen;
#endif

    CharVectorType  theResult(theManager);
    TranscodeToLocalCodePage(theFileName, theResult, true);

    if (theResult.empty() == true)
    {
        return 0;
    }
    else
    {
        const char* const   tmpName = &theResult[0];

        if (tmpName == 0)
        {
            return 0;
        }
        else
        {
            return fopen(tmpName, "wb");
        }
    }
#endif
}



XalanFileOutputStream::XalanFileOutputStream(
            const XalanDOMString&   theFileName,
            MemoryManager&          theManager,
            size_type               theBufferSize) :
    XalanOutputStream(theManager, theBufferSize),
    m_fileName(theFileName, theManager),
    m_handle(openFile(theFileName, theManager))
{
#if defined(XALAN_WINDOWS)
    if (m_handle == INVALID_HANDLE_VALUE)
    {
        XalanDOMString theBuffer(theManager);

        throw XalanFileOutputStreamOpenException(
                    theFileName,
                    GetLastError(),
                    theBuffer);
    }
#else
    if (m_handle == 0)
    {
        XalanDOMString theBuffer(theManager);

        throw XalanFileOutputStreamOpenException(
                    theFileName,
                    errno,
                    theBuffer);
    }
#endif
}



XalanFileOutputStream*
XalanFileOutputStream::create(
            const XalanDOMString&   theFileName,
            MemoryManager&          theManager,
            size_type               theBufferSize)
{
    typedef XalanFileOutputStream   ThisType;

    XalanAllocationGuard    theGuard(theManager, theManager.allocate(sizeof(ThisType)));

    ThisType* const     theResult =
        new (theGuard.get()) ThisType(
                        theFileName,
                        theManager,
                        theBufferSize);

    theGuard.release();

    return theResult;
}



XalanFileOutputStream::~XalanFileOutputStream()
{
#if defined(XALAN_WINDOWS)
    if (m_handle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_handle);
    }
#else
#if defined(XALAN_STRICT_ANSI_HEADERS)
    using std::fclose;
#endif

    if (m_handle != 0)
    {
        fclose(m_handle);
    }
#endif
}



void
XalanFileOutputStream::doFlush()
{
#if !defined(XALAN_WINDOWS)
#if defined(XALAN_STRICT_ANSI_HEADERS)
    using std::fflush;
#endif

    if (fflush(m_handle) != 0)
    {
        XalanDOMString theBuffer(getMemoryManager());

        throw XalanFileOutputStreamWriteException(
            m_fileName,
            errno,
            theBuffer);
    }
#endif
}



void
XalanFileOutputStream::writeData(
            const char*     theBuffer,
            size_type       theBufferLength)
{
#if defined(XALAN_WINDOWS)
    DWORD           theBytesWritten;

    assert(size_t(DWORD(theBufferLength)) == theBufferLength);

    if (WriteFile(m_handle, theBuffer, DWORD(theBufferLength), &theBytesWritten, 0) == false ||
        theBytesWritten != theBufferLength)
    {
        XalanDOMString  theExceptionBuffer(getMemoryManager());

        throw XalanFileOutputStreamWriteException(
                m_fileName,
                GetLastError(),
                theExceptionBuffer);
    }
#else
#if defined(XALAN_STRICT_ANSI_HEADERS)
    using std::fwrite;
#endif

    const size_t    theBytesWritten =
        fwrite(theBuffer,
               1,
               theBufferLength,
               m_handle);

    if (theBytesWritten != theBufferLength)
    {
       XalanDOMString   theExceptionBuffer(getMemoryManager());

        throw XalanFileOutputStreamWriteException(
                m_fileName,
                errno,
                theExceptionBuffer);
    }
#endif
}


XalanFileOutputStream::XalanFileOutputStreamOpenException::XalanFileOutputStreamOpenException(
        const XalanDOMString&   theFileName,
        int                     theErrorCode,
        XalanDOMString&         theBuffer,
        const Locator*          theLocator) :
    XalanOutputStreamException(
        formatMessage(
            XalanMessageLoader::getMessage(
                theBuffer,
                XalanMessages::ErrorOpeningFile_1Param,
                theFileName),
            theErrorCode,
            theBuffer),
        theBuffer.getMemoryManager(),
        theLocator)
{
}



XalanFileOutputStream::XalanFileOutputStreamOpenException::~XalanFileOutputStreamOpenException()
{
}



XalanFileOutputStream::XalanFileOutputStreamWriteException::XalanFileOutputStreamWriteException(
        const XalanDOMString&   theFileName,
        int                     theErrorCode,
        XalanDOMString&         theBuffer,
        const Locator*          theLocator) :
    XalanOutputStreamException(
        formatMessage(
            XalanMessageLoader::getMessage(
                theBuffer,
                XalanMessages::ErrorWritingFile_1Param,
                theFileName),
            theErrorCode,
            theBuffer),
        theBuffer.getMemoryManager(),
        theLocator)
{
}



XalanFileOutputStream::XalanFileOutputStreamWriteException::~XalanFileOutputStreamWriteException()
{
}



XALAN_CPP_NAMESPACE_END
