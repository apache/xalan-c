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
#include "DOMSupportException.hpp"




XALAN_CPP_NAMESPACE_BEGIN



DOMSupportException::DOMSupportException(
            const XalanDOMString&   theMessage,
            MemoryManager&          theManager,
            const Locator*          theLocator) :
    XSLException(
        theMessage,
        theManager,
        theLocator)
{
}



DOMSupportException::DOMSupportException(
            const XalanDOMString&   theMessage,
            MemoryManager&          theManager) :
    XSLException(
        theMessage,
        theManager)
{
}



DOMSupportException::~DOMSupportException()
{
}



static const XalanDOMChar   s_type[] = 
{   
    XalanUnicode::charLetter_D,
    XalanUnicode::charLetter_O,
    XalanUnicode::charLetter_M,
    XalanUnicode::charLetter_S,
    XalanUnicode::charLetter_u,
    XalanUnicode::charLetter_p,
    XalanUnicode::charLetter_p,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_E,
    XalanUnicode::charLetter_x,
    XalanUnicode::charLetter_c,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_p,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_n,
    0
};



const XalanDOMChar*
DOMSupportException::getType() const
{
    return s_type;
}



XALAN_CPP_NAMESPACE_END
