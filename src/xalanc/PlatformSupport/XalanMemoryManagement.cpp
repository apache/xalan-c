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

#include <xalanc/Include/XalanMemoryManagement.hpp>

#include <xercesc/util/PlatformUtils.hpp>

#include <cassert>
#include <new>

XALAN_CPP_NAMESPACE_BEGIN

class XalanDummyMemoryManager : public MemoryManagerType
{
public:
	virtual 
	~XalanDummyMemoryManager()
	{
	}
	
	virtual void*
	allocate( size_t /*	size, */ )
	{
		XALAN_USING_STD(bad_alloc)
		
		throw bad_alloc();
		
		return 0;
	}	
	virtual void
	deallocate(  void* 	/*	pDataPointer */ )
	{
		XALAN_USING_STD(bad_alloc)
		
		throw bad_alloc();		
		
	}
	
	
};



static XalanDummyMemoryManager  s_dummyMemMgr;

MemoryManagerType&
XalanMemMgrs::getDummyMemMgr()
{
	return s_dummyMemMgr;
}


MemoryManagerType&
XalanMemMgrs::getDefaultXercesMemMgr()
{
	XALAN_USING_XERCES(XMLPlatformUtils)
	
	MemoryManagerType* ptr = XMLPlatformUtils::fgMemoryManager;
	
	assert (ptr != 0);
	
	return *ptr;
}




    
XALAN_CPP_NAMESPACE_END




