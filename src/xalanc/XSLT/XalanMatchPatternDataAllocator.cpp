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
#include "XalanMatchPatternDataAllocator.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XalanMatchPatternDataAllocator::XalanMatchPatternDataAllocator(size_type    theBlockCount) :
	m_allocator(theBlockCount)
{
}



XalanMatchPatternDataAllocator::~XalanMatchPatternDataAllocator()
{
}



XalanMatchPatternDataAllocator::data_type*
XalanMatchPatternDataAllocator::create(
            const ElemTemplate&     theTemplate,
            data_type::size_type    thePosition,
            const XalanDOMString&   theTargetString,
            const XPath&            theMatchPattern,
            const XalanDOMString&   thePatternString,
            data_type::eMatchScore  thePriority)
{
	data_type* const	theBlock = m_allocator.allocateBlock();
	assert(theBlock != 0);

	data_type* const	theResult =
		new(theBlock) data_type(
				theTemplate,
				thePosition,
				theTargetString,
				theMatchPattern,
                thePatternString,
				thePriority);

	m_allocator.commitAllocation(theBlock);

	return theResult;
}



XALAN_CPP_NAMESPACE_END
