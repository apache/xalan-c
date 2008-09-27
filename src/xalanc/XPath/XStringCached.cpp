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
#include "XStringCached.hpp"



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XStringCached::XStringCached(
            GetCachedString&    val,
            MemoryManager&      theManager) :
    XStringBase(theManager),
    m_value(val)
{
}



XStringCached::XStringCached(
            const XStringCached&    source,
            MemoryManager&          theManager) :
    XStringBase(source, theManager),
    m_value(source.m_value.getExecutionContext())
{
    m_value.get() = source.m_value.get();
}



XStringCached::~XStringCached()
{
}



const XalanDOMString&
XStringCached::str(XPathExecutionContext&   /* executionContext */) const
{
    return m_value.get();
}



const XalanDOMString&
XStringCached::str() const
{
    return m_value.get();
}



void
XStringCached::str(
            XPathExecutionContext&  /* executionContext */,
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function) const
{
    string(m_value.get(), formatterListener, function);
}



void
XStringCached::str(
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function) const
{
    string(m_value.get(), formatterListener, function);
}



void
XStringCached::str(
            XPathExecutionContext&  /* executionContext */,
            XalanDOMString&         theBuffer) const
{
    theBuffer.append(m_value.get());
}



void
XStringCached::str(XalanDOMString&  theBuffer) const
{
    theBuffer.append(m_value.get());
}



double
XStringCached::stringLength(XPathExecutionContext&  /* executionContext */) const
{
    return static_cast<double>(m_value.get().length());
}



XStringCached::eObjectType
XStringCached::getRealType() const
{
    return eTypeStringCached;
}



XALAN_CPP_NAMESPACE_END
