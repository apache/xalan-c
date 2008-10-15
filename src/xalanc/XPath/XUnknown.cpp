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



#include "xalanc/PlatformSupport/DoubleSupport.hpp"
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
            MemoryManager&          theMemoryManager) :
    XObject(eTypeUnknown, theMemoryManager),
    m_value(theMemoryManager)
{
    XalanMessageLoader::getMessage(
        m_value,
        XalanMessages::VariableIsNotDefined_1Param,
        name);
}


    
XUnknown*
XUnknown::create(
            const XalanDOMString&   name,
            MemoryManager&          theMemoryManager)
{
    typedef XUnknown Type;

    XalanAllocationGuard    theGuard(theMemoryManager, theMemoryManager.allocate(sizeof(Type)));

    Type* const     theResult = new (theGuard.get()) Type(name, theMemoryManager);

    theGuard.release();

    return theResult;
}



XUnknown::XUnknown(
            const XUnknown&     source,
            MemoryManager&      theMemoryManager) :
    XObject(source, theMemoryManager),
    m_value(source.m_value, theMemoryManager)
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
XUnknown::num(XPathExecutionContext&    /* executionContext */) const
{
    return DoubleSupport::getNaN();
}



double
XUnknown::num() const
{
    return DoubleSupport::getNaN();
}



bool
XUnknown::boolean(XPathExecutionContext&    /* executionContext */) const
{
    return false;
}



const XalanDOMString&
XUnknown::str(XPathExecutionContext&    /* executionContext */) const
{
    return m_value;
}



const XalanDOMString&
XUnknown::str() const
{
    return m_value;
}



void
XUnknown::str(
            XPathExecutionContext&  /* executionContext */,
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function) const
{
    XObject::string(m_value, formatterListener, function);
}



void
XUnknown::str(
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function) const
{
    XObject::string(m_value, formatterListener, function);
}



void
XUnknown::str(
            XPathExecutionContext&  /* executionContext */,
            XalanDOMString&         theBuffer) const
{
    theBuffer.append(m_value);
}



void
XUnknown::str(XalanDOMString&   theBuffer) const
{
    theBuffer.append(m_value);
}



double
XUnknown::stringLength(XPathExecutionContext&   /* executionContext */) const
{
    return static_cast<double>(m_value.length());
}



void
XUnknown::ProcessXObjectTypeCallback(XObjectTypeCallback&   theCallbackObject)
{
    theCallbackObject.Unknown(
        *this,
        m_value);
}



void
XUnknown::ProcessXObjectTypeCallback(XObjectTypeCallback&   theCallbackObject) const
{
    theCallbackObject.Unknown(
        *this,
        m_value);
}



void
XUnknown::initialize(MemoryManager&     theManager)
{
    s_unknownString.reset(theManager, s_unknown);
}



void
XUnknown::terminate()
{
    releaseMemory(s_unknownString, XalanMemMgrs::getDummyMemMgr());
}



XALAN_CPP_NAMESPACE_END
