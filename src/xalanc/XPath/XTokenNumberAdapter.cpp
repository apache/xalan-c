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
#include "XTokenNumberAdapter.hpp"



#include "XToken.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XTokenNumberAdapter::XTokenNumberAdapter(
            const XToken&   theToken,
            MemoryManager&  theMemoryManager) :
    XNumberBase(theMemoryManager),
    m_value(theToken)
{
}



XTokenNumberAdapter::XTokenNumberAdapter(
            const XTokenNumberAdapter&  source,
            MemoryManager&              theMemoryManager) :
    XNumberBase(source, theMemoryManager),
    m_value(source.m_value)
{
}



XTokenNumberAdapter::~XTokenNumberAdapter()
{
}


double
XTokenNumberAdapter::num(XPathExecutionContext&     /* executionContext */) const
{
    return m_value.num();
}



double
XTokenNumberAdapter::num() const
{
    return m_value.num();
}



const XalanDOMString&
XTokenNumberAdapter::str(XPathExecutionContext&     /* executionContext */) const
{
    return m_value.str();
}



const XalanDOMString&
XTokenNumberAdapter::str() const
{
    return m_value.str();
}



void
XTokenNumberAdapter::str(
            XPathExecutionContext&  /* executionContext */,
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function) const
{
    m_value.str(formatterListener, function);
}



void
XTokenNumberAdapter::str(
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function) const
{
    m_value.str(formatterListener, function);
}



void
XTokenNumberAdapter::str(
            XPathExecutionContext&  /* executionContext */,
            XalanDOMString&         theBuffer) const
{
    theBuffer.append(m_value.str());
}



void
XTokenNumberAdapter::str(XalanDOMString&    theBuffer) const
{
    theBuffer.append(m_value.str());
}



double
XTokenNumberAdapter::stringLength(XPathExecutionContext&    /* executionContext */) const
{
    return m_value.stringLength();
}



XTokenNumberAdapter::eObjectType
XTokenNumberAdapter::getRealType() const
{
    return eTypeXTokenNumberAdapter;
}



XALAN_CPP_NAMESPACE_END
