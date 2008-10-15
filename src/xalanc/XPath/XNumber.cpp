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
#include "XNumber.hpp"



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XNumber::XNumber(
            double          val,
            MemoryManager&  theMemoryManager) :
    XNumberBase(theMemoryManager),
    m_value(val),
    m_cachedStringValue(theMemoryManager)
{
}



XNumber::XNumber(
            const XNumber&  source,
            MemoryManager&  theMemoryManager) :
    XNumberBase(source, theMemoryManager),
    m_value(source.m_value),
    m_cachedStringValue(source.m_cachedStringValue, theMemoryManager)
{
}



XNumber::~XNumber()
{
}


double
XNumber::num(XPathExecutionContext&     /* executionContext */) const
{
    return m_value;
}



double
XNumber::num() const
{
    return m_value;
}



const XalanDOMString&
XNumber::str(XPathExecutionContext&     /* executionContext */) const
{
    if (m_cachedStringValue.empty() == true)
    {
        NumberToDOMString(m_value, m_cachedStringValue);
    }

    return m_cachedStringValue;
}



const XalanDOMString&
XNumber::str() const
{
    if (m_cachedStringValue.empty() == true)
    {
        NumberToDOMString(m_value, m_cachedStringValue);
    }

    return m_cachedStringValue;
}



void
XNumber::str(
            XPathExecutionContext&  executionContext,
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function) const
{
    const XalanDOMString&   theValue = str(executionContext);

    assert(theValue.length() == FormatterListener::size_type(theValue.length()));

    (formatterListener.*function)(
        theValue.c_str(),
        FormatterListener::size_type(theValue.length()));
}



void
XNumber::str(
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function) const
{
    const XalanDOMString&   theValue = str();

    assert(theValue.length() == FormatterListener::size_type(theValue.length()));

    (formatterListener.*function)(
        theValue.c_str(),
        FormatterListener::size_type(theValue.length()));
}



void
XNumber::str(
            XPathExecutionContext&  /* executionContext */,
            XalanDOMString&         theBuffer) const
{
    if (m_cachedStringValue.empty() == false)
    {
        theBuffer.append(m_cachedStringValue);
    }
    else
    {
        NumberToDOMString(m_value, theBuffer);
    }
}



void
XNumber::str(XalanDOMString&    theBuffer) const
{
    if (m_cachedStringValue.empty() == false)
    {
        theBuffer.append(m_cachedStringValue);
    }
    else
    {
        NumberToDOMString(m_value, theBuffer);
    }
}



double
XNumber::stringLength(XPathExecutionContext&    executionContext) const
{
    return static_cast<double>(str(executionContext).length());
}



void
XNumber::set(double     theValue)
{
    m_value = theValue;

    m_cachedStringValue.clear();
}



XALAN_CPP_NAMESPACE_END
