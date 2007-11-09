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

#include "XPathConstructionContextDefault.hpp"



#include "xalanc/XPath/XPathParserException.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XPathConstructionContextDefault::XPathConstructionContextDefault(MemoryManagerType& theManager) :
	XPathConstructionContext(theManager),
	m_stringPool(theManager),
	m_stringCache(theManager)
{
}

XPathConstructionContextDefault*
XPathConstructionContextDefault::create(MemoryManagerType& theManager)
{
    typedef XPathConstructionContextDefault ThisType;

    XalanMemMgrAutoPtr<ThisType, false> theGuard( theManager , (ThisType*)theManager.allocate(sizeof(ThisType)));

    ThisType* theResult = theGuard.get();

    new (theResult) ThisType(theManager);

    theGuard.release();

    return theResult;
}

XPathConstructionContextDefault::~XPathConstructionContextDefault()
{
	reset();
}



void
XPathConstructionContextDefault::reset()
{
	m_stringCache.reset();
}



const XalanDOMString&
XPathConstructionContextDefault::getPooledString(const XalanDOMString&	theString)
{
	return m_stringPool.get(theString);
}



const XalanDOMString&
XPathConstructionContextDefault::getPooledString(
			const XalanDOMChar*			theString,
			XalanDOMString::size_type	theLength)
{
	return m_stringPool.get(theString, theLength);
}



XalanDOMString&
XPathConstructionContextDefault::getCachedString()
{
	return m_stringCache.get();
}



bool
XPathConstructionContextDefault::releaseCachedString(XalanDOMString&	theString)
{
	return m_stringCache.release(theString);
}



void
XPathConstructionContextDefault::error(
			const XalanDOMString&	msg,
			const XalanNode* 		/* sourceNode */,
			const LocatorType* 		locator) const
{
    MemoryManagerType& theManager = const_cast<XPathConstructionContextDefault*>(this)-> getMemoryManager();

    if (locator != 0)
	{
        MemoryManagerType& theManager = const_cast<XPathConstructionContextDefault*>(this)-> getMemoryManager();

		throw XPathParserException(*locator, msg, theManager);
	}
	else
	{
		throw XPathParserException(msg, theManager);
	}
}


void
XPathConstructionContextDefault::warn(
			const XalanDOMString&	/* msg */,
			const XalanNode* 		/* sourceNode */,
			const LocatorType* 		/* locator */) const
{
}



XALAN_CPP_NAMESPACE_END
