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
// Class header file.
#include "XalanOutputStreamPrintWriter.hpp"



#include <cassert>

#include <xalanc/Include/XalanMemMgrAutoPtr.hpp>

#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include "DOMStringHelper.hpp"
#include "XalanOutputStream.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XalanOutputStreamPrintWriter::XalanOutputStreamPrintWriter(
            XalanOutputStream&  theOutputStream,
            bool                fAutoFlush) :
    PrintWriter(fAutoFlush, theOutputStream.getMemoryManager()),
    m_outputStream(theOutputStream),
    m_buffer(theOutputStream.getMemoryManager()),
    m_flushWideChars(false)
{
}



XalanOutputStreamPrintWriter*
XalanOutputStreamPrintWriter::create(
            XalanOutputStream&  theOutputStream,
            bool                fAutoFlush) 
{
    typedef XalanOutputStreamPrintWriter    ThisType;

    MemoryManager&  theManager = theOutputStream.getMemoryManager();

    XalanAllocationGuard    theGuard(theManager, theManager.allocate(sizeof(ThisType)));

    ThisType* const     theResult =
        new (theGuard.get()) ThisType(
                        theOutputStream,
                        fAutoFlush);

    theGuard.release();

    return theResult;
}



XalanOutputStreamPrintWriter::~XalanOutputStreamPrintWriter()
{
    flush();
}



void
XalanOutputStreamPrintWriter::flushWideChars()
{
    if (m_flushWideChars == true)
    {
        m_outputStream.flushBuffer();

        m_flushWideChars = false;
    }
}



bool
XalanOutputStreamPrintWriter::checkError() const
{
    return false;
}



void
XalanOutputStreamPrintWriter::close()
{
    flush();
}


void
XalanOutputStreamPrintWriter::flush()
{
    m_outputStream.flush();
}



XalanOutputStream*
XalanOutputStreamPrintWriter::getStream()
{
    return &m_outputStream;
}



const XalanOutputStream*
XalanOutputStreamPrintWriter::getStream() const
{
    return &m_outputStream;
}



void
XalanOutputStreamPrintWriter::write(
            const char*     s,
            size_t          theOffset,
            size_t          theLength)
{
    assert(s != 0);
    assert(XalanDOMString::size_type(theLength) == theLength);

    flushWideChars();

    if (theLength != npos)
    {
        m_outputStream.write(
            s + theOffset,
            XalanDOMString::size_type(theLength));
    }
    else
    {
        if (theOffset == 0)
        {
            m_outputStream.write(s);
        }
        else
        {
            m_outputStream.write(s + theOffset);
        }
    }
}



void
XalanOutputStreamPrintWriter::write(
            const XalanDOMChar*         s,
            XalanDOMString::size_type   theOffset,
            XalanDOMString::size_type   theLength)
{
    assert(s != 0);

    if (theLength == XalanDOMString::npos)
    {
        if (theOffset == 0)
        {
            m_outputStream.write(s);
        }
        else
        {
            m_outputStream.write(s + theOffset);
        }
    }
    else
    {
        m_outputStream.write(s + theOffset, theLength);
    }

    m_flushWideChars = true;
}



void
XalanOutputStreamPrintWriter::write(XalanDOMChar    c)
{
    m_outputStream.write(c);

    m_flushWideChars = true;
}



void
XalanOutputStreamPrintWriter::write(
            const XalanDOMString&       s,
            XalanDOMString::size_type   theOffset,
            XalanDOMString::size_type   theLength)
{
    write(s.c_str(), theOffset, theLength);
}



#if !defined(XALAN_BOOL_AS_INT)
void
XalanOutputStreamPrintWriter::print(bool    b)
{
    if (b == true)
    {
        print(s_trueString);
    }
    else
    {
        print(s_falseString);
    }
}
#endif



void
XalanOutputStreamPrintWriter::print(char    c)
{
    write(c);
}



void
XalanOutputStreamPrintWriter::print(
            const char*     s,
            size_t          theLength)
{
    write(s,
          0,
          theLength);
}



void
XalanOutputStreamPrintWriter::print(
            const XalanDOMChar*         s,
            XalanDOMString::size_type   theLength)
{
    assert(s != 0);

    write(s,
          0,
          theLength);
}



void
XalanOutputStreamPrintWriter::print(double  d)
{
    m_buffer.clear();

    NumberToDOMString(d, m_buffer);

    print(m_buffer);
}



void
XalanOutputStreamPrintWriter::print(int     i)
{
    m_buffer.clear();

    NumberToDOMString(i, m_buffer);

    print(m_buffer);
}



void
XalanOutputStreamPrintWriter::print(long    l)
{
    m_buffer.clear();

    NumberToDOMString(l, m_buffer);

    print(m_buffer);
}



void
XalanOutputStreamPrintWriter::print(const XalanDOMString&   s)
{
    write(s.c_str(), 0, s.length());
}



void
XalanOutputStreamPrintWriter::println()
{
    write(s_newlineString, 0, length(s_newlineString));

    flush();
}



#if !defined(XALAN_BOOL_AS_INT)
void
XalanOutputStreamPrintWriter::println(bool  b)
{
    print(b);

    println();
}
#endif



void
XalanOutputStreamPrintWriter::println(char  c)
{
    print(c);

    println();
}



void
XalanOutputStreamPrintWriter::println(
            const char*     s,
            size_t          theLength)
{
    print(s, theLength);

    println();
}



void
XalanOutputStreamPrintWriter::println(
            const XalanDOMChar*         s,
            XalanDOMString::size_type   theLength)
{
    print(s, theLength);

    println();
}



void
XalanOutputStreamPrintWriter::println(double    d)
{
    print(d);

    println();
}



void
XalanOutputStreamPrintWriter::println(int   i)
{
    print(i);

    println();
}



void
XalanOutputStreamPrintWriter::println(long  l)
{
    print(l);

    println();
}



void
XalanOutputStreamPrintWriter::println(const XalanDOMString&     s)
{
    print(s);

    println();
}



XALAN_CPP_NAMESPACE_END
