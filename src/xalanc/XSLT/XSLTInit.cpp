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

static MemoryManagerType* s_staticMemoryManager = 0;

XSLTInit::XSLTInit(MemoryManagerType&      theManager) :
	m_platformSupportInit(theManager),
	m_domSupportInit(theManager),
	m_xmlSupportInit(theManager),
	m_xalanSourceTreeInit(theManager),
	m_xpathInit(theManager)
{
	++s_initCounter;

	if (s_initCounter == 1)
	{
		initialize(theManager);
	}
}

XSLTInit*
XSLTInit::create(MemoryManagerType&      theManager)
{
        typedef XSLTInit ThisType;
        
        XalanMemMgrAutoPtr<ThisType, false> theGuard( theManager , (ThisType*)theManager.allocate(sizeof(ThisType)));

        ThisType* theResult = theGuard.get();

        new (theResult) ThisType(theManager);

        theGuard.release();

        return theResult;
}


XSLTInit::~XSLTInit()
{
	--s_initCounter;

	if (s_initCounter == 0)
	{
		terminate();
	}
}

MemoryManagerType&
XSLTInit::getMemoryManager()
{
    assert( s_staticMemoryManager !=0);

    return *s_staticMemoryManager;
}

void
XSLTInit::initialize(MemoryManagerType&  theManager)
{
	Constants::initialize(theManager);

	ElemNumber::initialize(theManager);

	XSLTEngineImpl::initialize(theManager);

	StylesheetHandler::initialize(theManager);

    s_staticMemoryManager = & theManager;

}



void
XSLTInit::terminate()
{
	StylesheetHandler::terminate();

	XSLTEngineImpl::terminate();

	ElemNumber::terminate();

	Constants::terminate();

    s_staticMemoryManager = 0;

}



XALAN_CPP_NAMESPACE_END
