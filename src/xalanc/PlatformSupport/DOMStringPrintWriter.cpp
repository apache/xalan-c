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
#include "DOMStringPrintWriter.hpp"



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



XALAN_CPP_NAMESPACE_BEGIN



DOMStringPrintWriter::DOMStringPrintWriter(XalanDOMString&  theString) :
PrintWriter(true, theString.getMemoryManager()),
    m_outputString(&theString)
{
}



DOMStringPrintWriter::~DOMStringPrintWriter()
{
}



bool
DOMStringPrintWriter::checkError() const
{
    return false;
}



void
DOMStringPrintWriter::close()
{
}


void
DOMStringPrintWriter::flush()
{
}



void
DOMStringPrintWriter::write(
            const char*     s,
            size_t          theOffset,
            size_t          theLength)
{
    assert(XalanDOMString::size_type(theOffset) == theOffset);
    assert(XalanDOMString::size_type(theLength) == theLength);

    assert ( m_outputString != 0 );

    XalanDOMString tmpString( m_outputString->getMemoryManager() );

    tmpString = TranscodeFromLocalCodePage(s, tmpString);
    write(
        tmpString,
        XalanDOMString::size_type(theOffset),
        XalanDOMString::size_type(theLength));
}



void
DOMStringPrintWriter::write(
            const XalanDOMChar*         s,
            XalanDOMString::size_type   theOffset,
            XalanDOMString::size_type   theLength)
{
    assert(s != 0);

    m_outputString->append(s + theOffset, theLength);
}



void
DOMStringPrintWriter::write(XalanDOMChar    c)
{
    (*m_outputString) += c;
}



void
DOMStringPrintWriter::write(
            const XalanDOMString&       s,
            XalanDOMString::size_type   theOffset,
            XalanDOMString::size_type   theLength)
{
    assert(theLength == npos || s.length() >= theOffset + theLength);

    if (theOffset == 0 && theLength == npos)
    {
        (*m_outputString) += s;
    }
    else
    {
        (*m_outputString).append(s, theOffset, theLength);
    }
}



#if !defined(XALAN_BOOL_AS_INT)
void
DOMStringPrintWriter::print(bool    b)
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
DOMStringPrintWriter::print(char    c)
{
    write(c);
}



void
DOMStringPrintWriter::print(
            const char*     s,
            size_t          theLength)
{
    write(s,
          0,
          theLength);
}



void
DOMStringPrintWriter::print(
            const XalanDOMChar*         s,
            XalanDOMString::size_type   theLength)
{
    write(s,
          0,
          theLength);
}



void
DOMStringPrintWriter::print(double  d)
{
    assert ( m_outputString != 0 );

    XalanDOMString tmpString( m_outputString->getMemoryManager() );

    NumberToDOMString(d, tmpString);

    (*m_outputString) += tmpString;
}



void
DOMStringPrintWriter::print(int i)
{
    assert ( m_outputString != 0 );

    XalanDOMString tmpString( m_outputString->getMemoryManager() );

    NumberToDOMString(i, tmpString);

    (*m_outputString) += tmpString;
}



void
DOMStringPrintWriter::print(long    l)
{
    assert ( m_outputString != 0 );

    XalanDOMString tmpString( m_outputString->getMemoryManager() );

    NumberToDOMString(l, tmpString);

    (*m_outputString) += tmpString;
}



void
DOMStringPrintWriter::print(const XalanDOMString&   s)
{
    (*m_outputString) += s;
}



void
DOMStringPrintWriter::println()
{
    (*m_outputString) += s_newlineString;
}



#if !defined(XALAN_BOOL_AS_INT)
void
DOMStringPrintWriter::println(bool  b)
{
    print(b);

    println();
}
#endif



void
DOMStringPrintWriter::println(char  c)
{
    print(c);

    println();
}



void
DOMStringPrintWriter::println(
            const char*     s,
            size_t          theLength)
{
    print(s, theLength);

    println();
}



void
DOMStringPrintWriter::println(
            const XalanDOMChar*         s,
            XalanDOMString::size_type   theLength)
{
    print(s, theLength);

    println();
}



void
DOMStringPrintWriter::println(double    d)
{
    print(d);

    println();
}



void
DOMStringPrintWriter::println(int   i)
{
    print(i);

    println();
}



void
DOMStringPrintWriter::println(long  l)
{
    print(l);

    println();
}



void
DOMStringPrintWriter::println(const XalanDOMString&     s)
{
    print(s);

    println();
}



XALAN_CPP_NAMESPACE_END
