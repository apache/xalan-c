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
#include "XNumberBase.hpp"



#include <xalanc/PlatformSupport/DoubleSupport.hpp>



#include "XObjectTypeCallback.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XNumberBase::XNumberBase(MemoryManager&  theMemoryManager) :
    XObject(eTypeNumber, theMemoryManager)
{
}



XNumberBase::XNumberBase(
            const XNumberBase&  source,
            MemoryManager&      theMemoryManager) :
    XObject(source, theMemoryManager)
{
}



XNumberBase::~XNumberBase()
{
}



const XalanDOMString&
XNumberBase::getTypeString() const
{
    return s_numberString;
}



bool
XNumberBase::boolean(XPathExecutionContext&     executionContext) const
{
    return XObject::boolean(num(executionContext));
}



void
XNumberBase::ProcessXObjectTypeCallback(XObjectTypeCallback&    theCallbackObject)
{
    theCallbackObject.Number(
        *this,
        num(theCallbackObject.getExecutionContext()));
}



void
XNumberBase::ProcessXObjectTypeCallback(XObjectTypeCallback&    theCallbackObject) const
{
    theCallbackObject.Number(
        *this,
        num(theCallbackObject.getExecutionContext()));
}



XALAN_CPP_NAMESPACE_END
