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

#include "XSLTInit.hpp"



#include "Constants.hpp"
#include "ElemNumber.hpp"
#include "StylesheetHandler.hpp"
#include "XSLTEngineImpl.hpp"



XALAN_CPP_NAMESPACE_BEGIN



unsigned long	XSLTInit::s_initCounter = 0;



XSLTInit::XSLTInit() :
	m_platformSupportInit(),
	m_domSupportInit(),
	m_xmlSupportInit(),
	m_xalanSourceTreeInit(),
	m_xpathInit()
{
	++s_initCounter;

	if (s_initCounter == 1)
	{
		initialize();
	}
}



XSLTInit::~XSLTInit()
{
	--s_initCounter;

	if (s_initCounter == 0)
	{
		terminate();
	}
}



void
XSLTInit::initialize()
{
	Constants::initialize();

	ElemNumber::initialize();

	XSLTEngineImpl::initialize();

	StylesheetHandler::initialize();
}



void
XSLTInit::terminate()
{
	StylesheetHandler::terminate();

	XSLTEngineImpl::terminate();

	ElemNumber::terminate();

	Constants::terminate();
}



XALAN_CPP_NAMESPACE_END
