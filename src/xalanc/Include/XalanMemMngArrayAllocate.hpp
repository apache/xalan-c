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
#if !defined(XALANMEMMGRARRAYALLOCATE_HEADER_GUARD_1357924680)
#define XALANMEMMGRARRAYALLOCATE_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



#include <xalanc/Include/XalanMemMgrAutoPtr.hpp>



XALAN_CPP_NAMESPACE_BEGIN


template <class Type>
class XalanMemMngArrayAllocate
{
    typedef typename XalanMemMgrAutoPtrArray<Type>::size_type size_type;

public:
	static Type*
	allocate(	size_t				size, 
				MemoryManagerType& 	memoryManager)
	{		
		XalanMemMgrAutoPtrArray<Type> theGuard (	memoryManager,
													(Type*)memoryManager.allocate( sizeof(Type)*size),
													size_type(size) );

		size_t allocated = 0;

		for ( Type* runPtr = theGuard.get() ; allocated < size ; ++ allocated )
		{
			new ( runPtr + allocated ) Type();

			++theGuard;
		}

		Type*  theResult = theGuard.get();

		theGuard.release();

		return theResult;
	}
	
	static Type*
	allocateMemMgr(	size_t				size, 
				MemoryManagerType& 	memoryManager)
	{		
		XalanMemMgrAutoPtrArray<Type> theGuard (	memoryManager,
													(Type*)memoryManager.allocate( sizeof(Type)*size),
													size_type(size) );

		size_t allocated = 0;

		for ( Type* runPtr = theGuard.get() ; allocated < size ; ++ allocated )
		{
			new ( runPtr + allocated ) Type(memoryManager);

			++theGuard;
		}

		Type*  theResult = theGuard.get();

		theGuard.release();

		return theResult;
	}
	
	static void
	deallocate (	Type*				ptr,
					size_t				size,
					MemoryManagerType& 	memoryManager)
	{
		assert ( ptr != 0 );

		Type* runPtr = ptr;

		for ( size_t i = 0; i < size ; ++i )
		{
			runPtr->~Type();
		}

		memoryManager.deallocate ( ptr);
	}
 
};


XALAN_CPP_NAMESPACE_END



#endif	// if !defined(XALANMEMMGRARRAYALLOCATE_HEADER_GUARD_1357924680)


