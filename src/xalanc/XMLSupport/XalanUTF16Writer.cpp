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


#include <xalanc/XMLSupport/XalanUTF16Writer.hpp>

#include <xalanc/PlatformSupport/XalanTranscodingServices.hpp>



XALAN_CPP_NAMESPACE_BEGIN

static XalanDOMString	s_localUTF16String(XalanMemMgrs::getDummyMemMgr());


const XalanDOMString&	XalanUTF16Writer::s_utf16String = s_localUTF16String;

void
XalanUTF16Writer::initialize(MemoryManagerType& theManager)
{
    XalanDOMString theTmp(XalanTranscodingServices::s_utf16String, theManager);
    
   	s_localUTF16String.swap(theTmp);
}

void
XalanUTF16Writer::terminate()
{
    XalanDOMString   temp(XalanMemMgrs::getDummyMemMgr());
    
    temp.swap(s_localUTF16String);
}    
    
XALAN_CPP_NAMESPACE_END



