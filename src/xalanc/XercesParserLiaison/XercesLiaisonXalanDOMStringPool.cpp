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

// Class header file...
#include "XercesLiaisonXalanDOMStringPool.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XercesLiaisonXalanDOMStringPool::XercesLiaisonXalanDOMStringPool() :
	ParentType(),
	m_mutex()
{
}



XercesLiaisonXalanDOMStringPool::~XercesLiaisonXalanDOMStringPool()
{
}



void
XercesLiaisonXalanDOMStringPool::clear()
{
	XMLMutexLockType	theLock(&m_mutex);

	ParentType::clear();
}



size_t
XercesLiaisonXalanDOMStringPool::size() const
{
#if defined(XALAN_NO_MUTABLE)
	XMLMutexLockType	theLock(&((XercesLiaisonXalanDOMStringPool*)this)->m_mutex);
#else
	XMLMutexLockType	theLock(&m_mutex);
#endif

	return ParentType::size();
}



const XalanDOMString&
XercesLiaisonXalanDOMStringPool::get(const XalanDOMString&	theString)
{
	XMLMutexLockType	theLock(&m_mutex);

	return ParentType::get(theString);
}



const XalanDOMString&
XercesLiaisonXalanDOMStringPool::get(
			const XalanDOMChar*			theString,
			XalanDOMString::size_type	theLength)
{
	XMLMutexLockType	theLock(&m_mutex);

	return ParentType::get(theString, theLength);
}



XALAN_CPP_NAMESPACE_END
