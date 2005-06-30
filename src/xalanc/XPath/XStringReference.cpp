/*
 * Copyright 1999-2004 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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
            MemoryManagerType&      theManager) :
    XStringBase(theManager),
    m_value(val)
{
}



XStringReference::XStringReference(
            const XStringReference&     source,
            MemoryManagerType&          theManager) :
    XStringBase(source, theManager),
    m_value(source.m_value)
{
}



XStringReference::~XStringReference()
{
}



const XalanDOMString&
XStringReference::str() const
{
    return m_value;
}



void
XStringReference::str(
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



XStringReference::eObjectType
XStringReference::getRealType() const
{
    return eTypeStringReference;
}



double
XStringReference::stringLength() const
{
    return length(m_value);
}



XALAN_CPP_NAMESPACE_END
