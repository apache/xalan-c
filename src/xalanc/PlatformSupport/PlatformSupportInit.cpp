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

#include "PlatformSupportInit.hpp"



#include "DoubleSupport.hpp"
#include "DOMStringHelper.hpp"
#include "XalanMessageLoader.hpp"
#include "XalanTranscodingServices.hpp"



XALAN_CPP_NAMESPACE_BEGIN



unsigned long   PlatformSupportInit::s_initCounter = 0;



PlatformSupportInit::PlatformSupportInit(MemoryManager&      theManager) :
    m_xalanDOMInit(theManager)
{
    ++s_initCounter;

    if (s_initCounter == 1)
    {
        initialize(theManager);
    }
}



PlatformSupportInit::~PlatformSupportInit()
{
    --s_initCounter;

    if (s_initCounter == 0)
    {
        terminate();
    }
}



void
PlatformSupportInit::initialize(MemoryManager&  theManager)
{
    DoubleSupport::initialize();

    DOMStringHelper::initialize(theManager);

    XalanMessageLoader::initialize(theManager);

    XalanTranscodingServices::initialize(theManager);
}



void
PlatformSupportInit::terminate()
{
    XalanTranscodingServices::terminate();

    XalanMessageLoader::terminate();

    DOMStringHelper::terminate();

    DoubleSupport::terminate();
}



XALAN_CPP_NAMESPACE_END
