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

#include "PlatformSupportInit.hpp"



#include "XalanMessageLoader.hpp"
#include "XalanTranscodingServices.hpp"



XALAN_CPP_NAMESPACE_BEGIN



unsigned long	PlatformSupportInit::s_initCounter = 0;



PlatformSupportInit::PlatformSupportInit() :
	m_xalanDOMInit()
{
	++s_initCounter;

	if (s_initCounter == 1)
	{
		initialize();
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
PlatformSupportInit::initialize()
{
	XalanMessageLoader::createLoader();

	XalanTranscodingServices::initialize();
}



void
PlatformSupportInit::terminate()
{
	XalanTranscodingServices::terminate();

	XalanMessageLoader::destroyLoader();
}



XALAN_CPP_NAMESPACE_END
