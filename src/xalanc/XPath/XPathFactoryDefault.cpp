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
// Class header file.
#include "XPathFactoryDefault.hpp"



#include <algorithm>



#include "XPath.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XPathFactoryDefault::XPathFactoryDefault() :
	XPathFactory(),
	m_xpaths()
{
}



XPathFactoryDefault::~XPathFactoryDefault()
{
	reset();
}



void
XPathFactoryDefault::reset()
{
	XALAN_USING_STD(for_each)

	for_each(m_xpaths.begin(),
			 m_xpaths.end(),
			 DeleteXPathFunctor(*this, true));

	m_xpaths.clear();
}



bool
XPathFactoryDefault::doReturnObject(
			const XPath*	theXPath,
			bool			fInReset)
{
    const CollectionType::size_type     theCount =
        fInReset == true ? m_xpaths.count(theXPath) : m_xpaths.erase(theXPath);

    if (theCount == 0)
    {
        return false;
    }
    else
    {

#if defined(XALAN_CANNOT_DELETE_CONST)
		delete (XPath*)theXPath;
#else
		delete theXPath;
#endif

		return true;
	}
}



XPath*
XPathFactoryDefault::create()
{
	XPath* const	theXPath = new XPath;

	m_xpaths.insert(theXPath);

	return theXPath;
}



XALAN_CPP_NAMESPACE_END
