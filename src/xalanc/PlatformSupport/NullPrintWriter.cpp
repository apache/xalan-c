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
#include "NullPrintWriter.hpp"



XALAN_CPP_NAMESPACE_BEGIN



NullPrintWriter::NullPrintWriter(MemoryManager& theManager) :
    PrintWriter(false, theManager)
{
}



NullPrintWriter::~NullPrintWriter()
{
}



bool
NullPrintWriter::checkError() const
{
    return false;
}



void
NullPrintWriter::close()
{
}



void
NullPrintWriter::flush()
{
}



void
NullPrintWriter::write(
            const char*     /* s */,
            size_t          /* theOffset */,
            size_t          /* theLength */)
{
}



void
NullPrintWriter::write(
            const XalanDOMChar*         /* s */,
            XalanDOMString::size_type   /* theOffset */,
            XalanDOMString::size_type   /* theLength */)
{
}



void
NullPrintWriter::write(XalanDOMChar /* c */)
{
}



void
NullPrintWriter::write(
            const XalanDOMString&       /* s */,
            XalanDOMString::size_type   /* theOffset */,
            XalanDOMString::size_type   /* theLength */)
{
}



#if !defined(XALAN_BOOL_AS_INT)
void
NullPrintWriter::print(bool /* b */)
{
}
#endif



void
NullPrintWriter::print(char /* c */)
{
}



void
NullPrintWriter::print(
            const char*     /* s */,
            size_t          /* theLength */)
{
}



void
NullPrintWriter::print(
            const XalanDOMChar*         /* s */,
            XalanDOMString::size_type   /* theLength */)
{
}



void
NullPrintWriter::print(double   /* d */)
{
}



void
NullPrintWriter::print(int  /* i */)
{
}



void
NullPrintWriter::print(long /* l */)
{
}



void
NullPrintWriter::print(const XalanDOMString&    /* s */)
{
}



void
NullPrintWriter::println()
{
}



#if !defined(XALAN_BOOL_AS_INT)
void
NullPrintWriter::println(bool   /* x */)
{
}
#endif



void
NullPrintWriter::println(char   /* x */)
{
}



void
NullPrintWriter::println(
            const char*     /* s */,
            size_t          /* theLength */)
{
}



void
NullPrintWriter::println(
            const XalanDOMChar*         /* s */,
            XalanDOMString::size_type   /* theLength */)
{
}



void
NullPrintWriter::println(double /* x */)
{
}



void
NullPrintWriter::println(int    /* x */)
{
}



void
NullPrintWriter::println(long   /* x */)
{
}



void
NullPrintWriter::println(const XalanDOMString&  /* s */)
{
}



XALAN_CPP_NAMESPACE_END
