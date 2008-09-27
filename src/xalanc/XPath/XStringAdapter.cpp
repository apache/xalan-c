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
#include "XStringAdapter.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XStringAdapter::XStringAdapter(
            const XObjectPtr&       val,
            MemoryManager&          theManager,
            XPathExecutionContext&  theExecutionContext) :
    XStringBase(theManager, theExecutionContext),
    m_value(val)
{
}



XStringAdapter::XStringAdapter(
            const XStringAdapter&   source,
            MemoryManager&          theManager) :
    XStringBase(source, theManager),
    m_value(source.m_value)
{
}



XStringAdapter::~XStringAdapter()
{
}



const XalanDOMString&
XStringAdapter::str(XPathExecutionContext&  executionContext) const
{
    return m_value->str(executionContext);
}



const XalanDOMString&
XStringAdapter::str() const
{
    return m_value->str();
}



void
XStringAdapter::str(
            XPathExecutionContext&  executionContext,
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function) const
{
    m_value->str(
        executionContext,
        formatterListener,
        function);
}



void
XStringAdapter::str(
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function) const
{
    m_value->str(
        formatterListener,
        function);
}



void
XStringAdapter::str(
            XPathExecutionContext&  executionContext,
            XalanDOMString&         theBuffer) const
{
    m_value->str(
        executionContext,
        theBuffer);
}



void
XStringAdapter::str(XalanDOMString&     theBuffer) const
{
    m_value->str(theBuffer);
}



XStringAdapter::eObjectType
XStringAdapter::getRealType() const
{
    return eTypeStringAdapter;
}



double
XStringAdapter::stringLength(XPathExecutionContext&     executionContext) const
{
    return m_value->stringLength(executionContext);
}



XALAN_CPP_NAMESPACE_END
