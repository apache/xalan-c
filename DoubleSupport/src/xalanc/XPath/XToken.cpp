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
#include "XToken.hpp"



#include <xalanc/PlatformSupport/DoubleSupport.hpp>



#include "XObjectTypeCallback.hpp"



XALAN_CPP_NAMESPACE_BEGIN



static const XalanDOMString     s_emptyString(XalanMemMgrs::getDummyMemMgr());



XToken::XToken() :
    XObject(eTypeString),
    m_stringValue(&s_emptyString),
    m_numberValue(DoubleSupport::getNaN()),
    m_isString(true)
{
}



XToken::XToken(
            const XalanDOMString&   theString,
            MemoryManagerType&      theManager) :
    XObject(eTypeString),
    m_stringValue(&theString),
    m_numberValue(DoubleSupport::toDouble(theString, theManager)),
    m_isString(true)
{
}



XToken::XToken(
            double                  theNumber,
            const XalanDOMString&   theString) :
    XObject(eTypeString),
    m_stringValue(&theString),
    m_numberValue(theNumber),
    m_isString(false)
{
}



XToken::XToken(const XToken&    theSource) :
    XObject(theSource),
    m_stringValue(theSource.m_stringValue),
    m_numberValue(theSource.m_numberValue),
    m_isString(theSource.m_isString)
{
    assert(m_stringValue != 0);
}



XToken::~XToken()
{
    assert(m_stringValue != 0);
}



const XalanDOMString&
XToken::getTypeString() const
{
    assert(m_stringValue != 0);

    return s_stringString;
}



double
XToken::num() const
{
    assert(m_stringValue != 0);

    return m_numberValue;
}



bool
XToken::boolean() const
{
    assert(m_stringValue != 0);

    return m_isString == true ? XObject::boolean(*m_stringValue) : XObject::boolean(m_numberValue);
}



const XalanDOMString&
XToken::str() const
{
    assert(m_stringValue != 0);

    return *m_stringValue;
}



void
XToken::str(
            FormatterListener&  formatterListener,
            MemberFunctionPtr   function) const
{
    assert(m_stringValue != 0);

    const XalanDOMString::size_type     theLength =
        m_stringValue->length();

    if (theLength != 0)
    {
        assert(theLength == FormatterListener::size_type(theLength));

        (formatterListener.*function)(
            m_stringValue->c_str(),
            FormatterListener::size_type(theLength));
    }
}



void
XToken::str(XalanDOMString&     theBuffer) const
{
    assert(m_stringValue != 0);

    theBuffer.append(*m_stringValue);
}



double
XToken::stringLength() const
{
    assert(m_stringValue != 0);

    return m_stringValue->length();
}



void
XToken::ProcessXObjectTypeCallback(XObjectTypeCallback&     theCallbackObject)
{
    assert(m_stringValue != 0);

    if (m_isString == true)
    {
        theCallbackObject.String(*this, *m_stringValue);
    }
    else
    {
        theCallbackObject.Number(*this, m_numberValue);
    }
}



void
XToken::ProcessXObjectTypeCallback(XObjectTypeCallback&     theCallbackObject) const
{
    assert(m_stringValue != 0);

    if (m_isString == true)
    {
        theCallbackObject.String(*this, *m_stringValue);
    }
    else
    {
        theCallbackObject.Number(*this, m_numberValue);
    }
}



void
XToken::set(const XalanDOMString&   theString,
            MemoryManagerType& theManager)
{
    m_stringValue = &theString;

    m_numberValue = DoubleSupport::toDouble(theString, theManager);

    m_isString = true;
}



void
XToken::set(
            double                  theNumber,
            const XalanDOMString&   theString,
            MemoryManagerType&      theManager)
{
    XalanDOMString theResult(theManager);

    assert(theString == DoubleToDOMString(theNumber,theResult));

    m_stringValue = &theString;

    m_numberValue = theNumber;

    m_isString = false;
}



void 
XToken::referenced()
{
}



void 
XToken::dereferenced()
{
}



XALAN_CPP_NAMESPACE_END
