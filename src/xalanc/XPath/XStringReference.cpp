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
#include "XStringReference.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XStringReference::XStringReference(
            const XalanDOMString&   val,
            MemoryManager&          theManager) :
    XStringBase(theManager),
    m_value(val)
{
}



XStringReference::XStringReference(
            const XStringReference&     source,
            MemoryManager&              theManager) :
    XStringBase(source, theManager),
    m_value(source.m_value)
{
}



XStringReference::~XStringReference()
{
}



const XalanDOMString&
XStringReference::str(XPathExecutionContext&    /* executionContext */) const
{
    return m_value;
}



const XalanDOMString&
XStringReference::str() const
{
    return m_value;
}



void
XStringReference::str(
            XPathExecutionContext&  /* executionContext */,
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function) const
{
    string(m_value, formatterListener, function);
}



void
XStringReference::str(
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function) const
{
    string(m_value, formatterListener, function);
}



void
XStringReference::str(
            XPathExecutionContext&  /* executionContext */,
            XalanDOMString&         theBuffer) const
{
    theBuffer.append(m_value);
}



void
XStringReference::str(XalanDOMString&   theBuffer) const
{
    theBuffer.append(m_value);
}



double
XStringReference::stringLength(XPathExecutionContext&   /* executionContext */) const
{
    return static_cast<double>(m_value.length());
}



XStringReference::eObjectType
XStringReference::getRealType() const
{
    return eTypeStringReference;
}



XALAN_CPP_NAMESPACE_END
