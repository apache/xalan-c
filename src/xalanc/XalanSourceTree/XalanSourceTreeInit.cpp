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

#include "XalanSourceTreeInit.hpp"



#include "XalanSourceTreeComment.hpp"
#include "XalanSourceTreeDocument.hpp"
#include "XalanSourceTreeText.hpp"



XALAN_CPP_NAMESPACE_BEGIN



unsigned long	XalanSourceTreeInit::s_initCounter = 0;



XalanSourceTreeInit::XalanSourceTreeInit() :
	m_platformSupportInit(),
	m_domSupportInit(),
	m_xmlSupportInit()
{
	++s_initCounter;

	if (s_initCounter == 1)
	{
		initialize();
	}
}



XalanSourceTreeInit::~XalanSourceTreeInit()
{
	--s_initCounter;

	if (s_initCounter == 0)
	{
		terminate();
	}
}



void
XalanSourceTreeInit::initialize()
{
	XalanSourceTreeDocument::initialize();

	XalanSourceTreeComment::initialize();

	XalanSourceTreeText::initialize();
}



void
XalanSourceTreeInit::terminate()
{
	XalanSourceTreeText::terminate();

	XalanSourceTreeComment::terminate();

	XalanSourceTreeDocument::terminate();
}



XALAN_CPP_NAMESPACE_END
