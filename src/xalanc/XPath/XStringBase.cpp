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
#include "XStringBase.hpp"



#include <xalanc/XalanDOM/XalanText.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/DoubleSupport.hpp>



#include "XObjectTypeCallback.hpp"
#include "XPathExecutionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XStringBase::XStringBase(MemoryManager&  theMemoryManager) :
    XObject(eTypeString, theMemoryManager),
    m_cachedNumberValue(0.0),
    m_resultTreeFrag(*this, theMemoryManager, 0)
{
}



XStringBase::XStringBase(
            MemoryManager&          theMemoryManager,
            XPathExecutionContext&  theExecutionContext) :
    XObject(eTypeString, theMemoryManager),
    m_cachedNumberValue(0.0),
    m_resultTreeFrag(*this, theMemoryManager, &theExecutionContext)
{
}



XStringBase::XStringBase(
            const XStringBase&  source,
            MemoryManager&      theMemoryManager) :
    XObject(source, theMemoryManager),
    m_cachedNumberValue(source.m_cachedNumberValue),
    m_resultTreeFrag(
        *this,
        theMemoryManager,
        source.m_resultTreeFrag.getExecutionContext())
{
}



XStringBase::~XStringBase()
{
}



const XalanDOMString&
XStringBase::getTypeString() const
{
    return s_stringString;
}



double
XStringBase::num(XPathExecutionContext&     executionContext) const
{
    if (m_cachedNumberValue == 0.0)
    {
        m_cachedNumberValue =
            DoubleSupport::toDouble(
                str(executionContext),
                getMemoryManager());
    }

    return m_cachedNumberValue;
}



bool
XStringBase::boolean(XPathExecutionContext&     executionContext) const
{
    return !str(executionContext).empty();
}



const XalanDocumentFragment&
XStringBase::rtree() const
{
    return m_resultTreeFrag;
}



void
XStringBase::ProcessXObjectTypeCallback(XObjectTypeCallback&    theCallbackObject)
{
    theCallbackObject.String(
        *this,
        str(theCallbackObject.getExecutionContext()));
}



void
XStringBase::ProcessXObjectTypeCallback(XObjectTypeCallback&    theCallbackObject) const
{
    theCallbackObject.String(
        *this,
        str(theCallbackObject.getExecutionContext()));
}



const XalanDOMString&
XStringBase::str() const
{
    MemoryManager&  theMemoryManager =
        XalanMemoryManager::getExceptionMemoryManager(getMemoryManager());

    XalanDOMString  theBuffer(theMemoryManager);

    throw XObjectInvalidConversionException(
                theMemoryManager,
                getTypeString(),
                s_numberString,
                theBuffer);
}



XALAN_CPP_NAMESPACE_END
