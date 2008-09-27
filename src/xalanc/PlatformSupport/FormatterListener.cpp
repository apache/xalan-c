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
#include "FormatterListener.hpp"



#include "DOMStringHelper.hpp"



XALAN_CPP_NAMESPACE_BEGIN



FormatterListener::FormatterListener(eFormat    theFormat) :
    ParentType(),
    m_prefixResolver(0),
    m_outputFormat(theFormat),
    m_XMLVersion(XML_VERSION_1_0)
{
    assert(length(s_piTarget) == s_piTargetLength);
    assert(length(s_piData) == s_piDataLength);
}



FormatterListener::~FormatterListener()
{
}



Writer*
FormatterListener::getWriter() const
{
    return 0;
}





const XalanDOMString    FormatterListener::s_emptyString(XalanMemMgrs::getDummyMemMgr());



const XalanDOMString&
FormatterListener::getDoctypeSystem() const
{
    return s_emptyString;
}



const XalanDOMString&
FormatterListener::getDoctypePublic() const
{
    return s_emptyString;
}



const XalanDOMString&
FormatterListener::getEncoding() const
{
    return s_emptyString;
}



const XalanDOMString&
FormatterListener::getMediaType() const
{
    return s_emptyString;
}



int
FormatterListener::getIndent() const
{
    return 0;
}



const XalanDOMChar  FormatterListener::s_piTarget[] =
{
    XalanUnicode::charLetter_X,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_n,
    0
};

const XalanDOMChar  FormatterListener::s_piData[] =
{
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_w,
    0
};

#if !defined(XALAN_INLINE_INITIALIZATION)
const XalanDOMString::size_type     FormatterListener::s_piTargetLength = 5;
const XalanDOMString::size_type     FormatterListener::s_piDataLength = 3;
#elif !defined(XALAN_INLINE_INITIALIZATION_IS_DEFINITION_BUG)
const XalanDOMString::size_type     FormatterListener::s_piTargetLength;
const XalanDOMString::size_type     FormatterListener::s_piDataLength;
#endif



XALAN_CPP_NAMESPACE_END
