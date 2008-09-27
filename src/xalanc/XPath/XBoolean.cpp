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
#include "XBoolean.hpp"



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



#include "XObjectTypeCallback.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XBoolean::XBoolean(
            bool            val,
            MemoryManager&  theMemoryManager) :
    XObject(eTypeBoolean, theMemoryManager),
    m_value(val)
{
}



XBoolean::XBoolean(
            const XBoolean&     source,
            MemoryManager&      theMemoryManager) :
    XObject(source, theMemoryManager),
    m_value(source.m_value)
{
}



XBoolean::~XBoolean()
{
}





const XalanDOMString&
XBoolean::getTypeString() const
{
    return s_booleanString;
}



double
XBoolean::num(XPathExecutionContext&    /* executionContext */) const
{
    return number(m_value);
}



bool
XBoolean::boolean(XPathExecutionContext&    /* executionContext */) const
{
    return m_value;
}



const XalanDOMString&
XBoolean::str(XPathExecutionContext&    /* executionContext */) const
{
    return string(m_value);
}



const XalanDOMString&
XBoolean::str() const
{
    return string(m_value);
}



void
XBoolean::str(
            XPathExecutionContext&  /* executionContext */,
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function) const
{
    string(m_value, formatterListener, function);
}



void
XBoolean::str(
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function) const
{
    string(m_value, formatterListener, function);
}



void
XBoolean::str(
            XPathExecutionContext&  /* executionContext */,
            XalanDOMString&         theBuffer) const
{
    string(m_value, theBuffer);
}



void
XBoolean::str(XalanDOMString&   theBuffer) const
{
    string(m_value, theBuffer);
}



double
XBoolean::stringLength(XPathExecutionContext&   /* executionContext */) const
{
    assert(
        s_trueString.length() == 4 &&
        s_falseString.length() == 5);

    return m_value == true ? 4 : 5;
}


    
void
XBoolean::ProcessXObjectTypeCallback(XObjectTypeCallback&   theCallbackObject)
{
    theCallbackObject.Boolean(
        *this,
        boolean(theCallbackObject.getExecutionContext()));
}



void
XBoolean::ProcessXObjectTypeCallback(XObjectTypeCallback&   theCallbackObject) const
{
    theCallbackObject.Boolean(
        *this,
        boolean(theCallbackObject.getExecutionContext()));
}



XALAN_CPP_NAMESPACE_END
