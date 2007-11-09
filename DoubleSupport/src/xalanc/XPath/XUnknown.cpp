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
#include "XUnknown.hpp"



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include "XObjectTypeCallback.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XalanDOMString  XUnknown::s_unknownString(XalanMemMgrs::getDummyMemMgr());



static const XalanDOMChar   s_unknown[] =
{
    XalanUnicode::charLessThanSign,
    XalanUnicode::charLetter_u,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_k,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_w,
    XalanUnicode::charLetter_n,
    XalanUnicode::charGreaterThanSign,
    0
};



XUnknown::XUnknown(
            const XalanDOMString&   name,
            MemoryManagerType&      theManager) :
    XObject(eTypeUnknown),
    m_value(theManager)
{
    XalanMessageLoader::getMessage(
        m_value,
        XalanMessages::VariableIsNotDefined_1Param,
        name);
}


    
XUnknown*
XUnknown::create(const XalanDOMString&  name, MemoryManagerType& theManager)
{
    typedef XUnknown Type;

    XalanMemMgrAutoPtr<Type, false> theGuard( theManager , (Type*)theManager.allocate(sizeof(Type)));

    Type* theResult = theGuard.get();

    new (theResult) Type(name, theManager);

    theGuard.release();

    return theResult;
}



XUnknown::XUnknown(const XUnknown&  source, MemoryManagerType& theManager) :
    XObject(source),
    m_value(source.m_value, theManager)
{
}



XUnknown::~XUnknown()
{
}




const XalanDOMString&
XUnknown::getTypeString() const
{
    return s_unknownString;
}
double
XUnknown::num() const
{
    return 0.0;
}



bool
XUnknown::boolean() const
{
    return false;
}



const XalanDOMString&
XUnknown::str() const
{
    return m_value;
}



void
XUnknown::str(
            FormatterListener&  formatterListener,
            MemberFunctionPtr   function) const
{
    assert(length(m_value) == FormatterListener::size_type(length(m_value)));

    (formatterListener.*function)(c_wstr(m_value), FormatterListener::size_type(length(m_value)));
}



double
XUnknown::stringLength() const
{
    return length(m_value);
}



void
XUnknown::ProcessXObjectTypeCallback(XObjectTypeCallback&   theCallbackObject)
{
    theCallbackObject.Unknown(*this,
                              m_value);
}



void
XUnknown::ProcessXObjectTypeCallback(XObjectTypeCallback&   theCallbackObject) const
{
    theCallbackObject.Unknown(*this,
                              m_value);
}



void
XUnknown::initialize(MemoryManagerType& theManager)
{
    s_unknownString.reset(theManager, s_unknown);
}



void
XUnknown::terminate()
{
    releaseMemory(s_unknownString, XalanMemMgrs::getDummyMemMgr());
}



XALAN_CPP_NAMESPACE_END
