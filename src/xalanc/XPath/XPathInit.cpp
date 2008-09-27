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

#include "XPathInit.hpp"



#include "FunctionLang.hpp"
#include "XObject.hpp"
#include "XUnknown.hpp"
#include "XPath.hpp"
#include "XPathEnvSupportDefault.hpp"
#include "XPathProcessorImpl.hpp"



XALAN_CPP_NAMESPACE_BEGIN



unsigned long   XPathInit::s_initCounter = 0;



XPathInit::XPathInit(MemoryManager& theManager) :
    m_platformSupportInit(theManager),
    m_domSupportInit(theManager)
{
    ++s_initCounter;

    if (s_initCounter == 1)
    {
        initialize(theManager);
    }
}



XPathInit*
XPathInit::create(MemoryManager&    theManager)
{
    XPathInit*  theResult;

    return XalanConstruct(theManager, theResult, theManager);
}



XPathInit::~XPathInit()
{
    --s_initCounter;

    if (s_initCounter == 0)
    {
        terminate();
    }
}



void
XPathInit::initialize(MemoryManager& theManager)
{
    FunctionLang::initialize(theManager);

    XObject::initialize(theManager);

    XUnknown::initialize(theManager);

    XPath::initialize(theManager);

    XPathEnvSupportDefault::initialize(theManager);
}



void
XPathInit::terminate()
{
    XPathEnvSupportDefault::terminate();

    XPath::terminate();

    XUnknown::terminate();

    XObject::terminate();

    FunctionLang::terminate();
}



XALAN_CPP_NAMESPACE_END
