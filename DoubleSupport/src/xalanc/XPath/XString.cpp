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
#include "XString.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XString::XString(
            const XalanDOMString&   val,
            MemoryManagerType&      theManager) :
    XStringBase(theManager),
    m_value(val,theManager)
{
}



XString::XString(
            const XalanDOMChar*     val,
            MemoryManagerType&      theManager) :
    XStringBase(theManager),
    m_value(val, theManager)
{
}



XString::XString(
            const XalanDOMChar*     val,
            unsigned int            len,
            MemoryManagerType&      theManager) :
    XStringBase(theManager),
    m_value(val, theManager, len)
{
}



XString::XString(
            const XString&      source,
            MemoryManagerType&  theManager) :
    XStringBase(source, theManager),
    m_value(source.m_value, theManager)
{
}



XString::~XString()
{
}



const XalanDOMString&
XString::str() const
{
    return m_value;
}



void
XString::str(
            FormatterListener&  formatterListener,
            MemberFunctionPtr   function) const
{
    const XalanDOMString::size_type     theLength =
        m_value.length();

    if (theLength != 0)
    {
        assert(theLength == FormatterListener::size_type(theLength));

        (formatterListener.*function)(
            m_value.c_str(),
            FormatterListener::size_type(theLength));
    }
}



double
XString::stringLength() const
{
    return m_value.length();
}



XALAN_CPP_NAMESPACE_END
