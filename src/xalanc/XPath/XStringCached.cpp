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
#include "XStringCached.hpp"



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XStringCached::XStringCached(
            GetAndReleaseCachedString&  val,
            MemoryManagerType&          theManager) :
    XStringBase(theManager),
    m_value(val)
{
}



XStringCached::XStringCached(
            const XStringCached&    source,
            MemoryManagerType&      theManager) :
    XStringBase(source, theManager),
    m_value(source.m_value.getExecutionContext())
{
    m_value.get() = source.m_value.get();
}



XStringCached::~XStringCached()
{
}



const XalanDOMString&
XStringCached::str() const
{
    return m_value.get();
}



void
XStringCached::str(
            FormatterListener&  formatterListener,
            MemberFunctionPtr   function) const
{
    const XalanDOMString&   theString = m_value.get();

    const XalanDOMString::size_type     theLength =
        theString.length();

    if (theLength != 0)
    {
        assert(theLength == FormatterListener::size_type(theLength));

        (formatterListener.*function)(
            theString.c_str(),
            FormatterListener::size_type(theLength));
    }
}



XStringCached::eObjectType
XStringCached::getRealType() const
{
    return eTypeStringCached;
}



double
XStringCached::stringLength() const
{
    return m_value.get().length();
}



XALAN_CPP_NAMESPACE_END
