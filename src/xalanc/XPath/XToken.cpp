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



XToken::XToken(MemoryManager&   theMemoryManager) :
    XObject(eTypeString, theMemoryManager),
    m_stringValue(&s_emptyString),
    m_numberValue(DoubleSupport::getNaN()),
    m_isString(true)
{
}



XToken::XToken(
            const XalanDOMString&   theString,
            double                  theNumber,
            MemoryManager&          theMemoryManager) :
    XObject(eTypeString, theMemoryManager),
    m_stringValue(&theString),
    m_numberValue(theNumber),
    m_isString(true)
{
}



XToken::XToken(
            double                  theNumber,
            const XalanDOMString&   theString,
            MemoryManager&          theMemoryManager) :
    XObject(eTypeString, theMemoryManager),
    m_stringValue(&theString),
    m_numberValue(theNumber),
    m_isString(false)
{
}



XToken::XToken(
            const XToken&   theSource,
            MemoryManager&  theMemoryManager) :
    XObject(theSource, theMemoryManager),
    m_stringValue(theSource.m_stringValue),
    m_numberValue(theSource.m_numberValue),
    m_isString(theSource.m_isString)
{
    assert(m_stringValue != 0);
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
XToken::num(XPathExecutionContext&    /* executionContext */) const
{
    assert(m_stringValue != 0);

    return m_numberValue;
}



bool
XToken::boolean(XPathExecutionContext&  /* executionContext */) const
{
    assert(m_stringValue != 0);

    return m_isString == true ? XObject::boolean(*m_stringValue) : XObject::boolean(m_numberValue);
}



const XalanDOMString&
XToken::str(XPathExecutionContext&  /* executionContext */) const
{
    return *m_stringValue;
}



const XalanDOMString&
XToken::str() const
{
    return *m_stringValue;
}



void
XToken::str(
            XPathExecutionContext&  /* executionContext */,
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function) const
{
    assert(m_stringValue != 0);

    string(*m_stringValue, formatterListener, function);
}



void
XToken::str(
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function) const
{
    assert(m_stringValue != 0);

    string(*m_stringValue, formatterListener, function);
}



void
XToken::str(XalanDOMString&     theBuffer) const
{
    assert(m_stringValue != 0);

    theBuffer.append(*m_stringValue);
}



void
XToken::str(
            XPathExecutionContext&  /* executionContext */,
            XalanDOMString&         theBuffer) const
{
    assert(m_stringValue != 0);

    theBuffer.append(*m_stringValue);
}



double
XToken::stringLength(XPathExecutionContext&     /* executionContext */) const
{
    assert(m_stringValue != 0);

    return static_cast<double>(m_stringValue->length());
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
XToken::set(
            const XalanDOMString&   theString,
            double                  theNumber)
{
    m_stringValue = &theString;

    m_numberValue = theNumber;

    m_isString = true;
}



void
XToken::set(
            double                  theNumber,
            const XalanDOMString&   theString)
{
    m_numberValue = theNumber;

    m_stringValue = &theString;

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
