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
#include "XTokenNumberAdapterAllocator.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XTokenNumberAdapterAllocator::XTokenNumberAdapterAllocator(size_type	theBlockCount) :
	m_allocator(theBlockCount)
{
}



XTokenNumberAdapterAllocator::~XTokenNumberAdapterAllocator()
{
}



XTokenNumberAdapterAllocator::object_type*
XTokenNumberAdapterAllocator::create(const XToken&	theXToken) 
{
	object_type* const	theBlock = m_allocator.allocateBlock();
	assert(theBlock != 0);

	object_type* const	theResult = new(theBlock) object_type(theXToken);

	m_allocator.commitAllocation(theBlock);

	return theResult;
}



XTokenNumberAdapterAllocator::object_type*
XTokenNumberAdapterAllocator::clone(const object_type&	theObject)
{
	object_type* const		theBlock = m_allocator.allocateBlock();
	assert(theBlock != 0);

	theObject.clone(theBlock);

	m_allocator.commitAllocation(theBlock);

	return theBlock;
}



bool 
XTokenNumberAdapterAllocator::destroy(object_type*	theObject)
{
	return m_allocator.destroyObject(theObject);
}



void 
XTokenNumberAdapterAllocator::reset()
{
	m_allocator.reset();
}



XALAN_CPP_NAMESPACE_END
