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
#include "XNumberAllocator.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XNumberAllocator::XNumberAllocator(size_type	theBlockCount) :
	m_allocator(theBlockCount)
{
}



XNumberAllocator::~XNumberAllocator()
{
}



XNumberAllocator::number_type*
XNumberAllocator::createNumber(double	theNumber) 
{
	number_type* const	theBlock = m_allocator.allocateBlock();
	assert(theBlock != 0);

	number_type* const	theResult = new(theBlock) number_type(theNumber);

	m_allocator.commitAllocation(theBlock);

	return theResult;
}



XNumberAllocator::number_type*
XNumberAllocator::clone(const XNumber&	value)
{
	number_type* const		theBlock = m_allocator.allocateBlock();
	assert(theBlock != 0);

	value.clone(theBlock);

	m_allocator.commitAllocation(theBlock);

	return theBlock;
}



bool
XNumberAllocator::destroy(number_type*	theNumber)
{
	return m_allocator.destroyObject(theNumber);
}



void 
XNumberAllocator::reset()
{
	m_allocator.reset();
}



XALAN_CPP_NAMESPACE_END
