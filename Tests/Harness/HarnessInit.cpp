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

#include "HarnessInit.hpp"



#include <xalanc/XalanDOM/XalanDOMInit.hpp>



#include "FileUtility.hpp"



XALAN_CPP_NAMESPACE_BEGIN



static const XalanDOMInit*	s_xalanDOMInit = 0;



HarnessInit::HarnessInit()
{
	assert(s_xalanDOMInit == 0);

	s_xalanDOMInit = new XalanDOMInit;

	FileUtility::initialize();
}



HarnessInit::~HarnessInit()
{
	FileUtility::terminate();

	delete s_xalanDOMInit;

	s_xalanDOMInit = 0;
}



XALAN_CPP_NAMESPACE_END
