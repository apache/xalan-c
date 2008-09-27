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
// Base class header file.
#include "XPathParserException.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XPathParserException::XPathParserException(
            const XalanDOMString&   theMessage,
            MemoryManager&          theManager,
            const Locator*          theLocator) :
    XalanXPathException(
        theMessage,
        theManager,
        theLocator)
{
}



XPathParserException::XPathParserException(
            const XalanDOMString&   theMessage,
            MemoryManager&          theManager) :
    XalanXPathException(
        theMessage,
        theManager)
{
}



XPathParserException::XPathParserException(const XPathParserException&  other) :
    XalanXPathException(other)
{
}



XPathParserException::~XPathParserException()
{
}



static const XalanDOMChar   s_type[] = 
{   
    XalanUnicode::charLetter_X,
    XalanUnicode::charLetter_P,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_h,
    XalanUnicode::charLetter_P,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_r,
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
XPathParserException::getType() const
{
    return s_type;
}



XALAN_CPP_NAMESPACE_END
