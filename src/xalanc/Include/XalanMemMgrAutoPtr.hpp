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
#if !defined(XALANMEMMGRAUTOPTR_HEADER_GUARD_1357924680)
#define XALANMEMMGRAUTOPTR_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



#include <xercesc/framework/MemoryManager.hpp>




#include <cstddef>

#include <cassert>



XALAN_CPP_NAMESPACE_BEGIN



// We're using our own auto_ptr-like class due to wide
// variations amongst the varous platforms we have to
// support
template<class Type>
class XalanMemMgrAutoPtr
{
public:

    typedef XERCES_CPP_NAMESPACE_QUALIFIER MemoryManager		MemoryManagerType;
	typedef XALAN_STD_QUALIFIER pair<MemoryManagerType*, Type*> AutoPtrPairType;

	class MemMgrAutoPtrData : public AutoPtrPairType
	{
	public:
		MemMgrAutoPtrData():
			AutoPtrPairType(0,0)
		{
		}

		MemMgrAutoPtrData(	
			MemoryManagerType* memoryManager,
			Type* dataPointer): 
			AutoPtrPairType(memoryManager, dataPointer)
		{
			invariants();
		}
		
	
		bool
		isInitilized()const
		{
			return ( (first != 0) && (second != 0) )? true : false;
		}
	
		void
		deallocate()
		{
			invariants();

			if ( isInitilized() )
			{			
				second->~Type();

				first->deallocate(second);
			}
		}
		
		void 
		reset(	MemoryManagerType* _m_memoryManager ,
				Type*	_m_dataPointer )
		{	
			invariants();

			first = _m_memoryManager;
			
			second = _m_dataPointer;

			invariants();
		}	
	private:
		void
		invariants()const
		{
			assert( isInitilized() ||
					( (first == 0) && (second ==0) ) );
		}
		
	};
	
	
	XalanMemMgrAutoPtr(
			MemoryManagerType*  theManager, 
			Type* ptr  ) : 
		m_pointerInfo(theManager, ptr)
	{
	}	

	XalanMemMgrAutoPtr() :
		m_pointerInfo()
	{
	}
	
	XalanMemMgrAutoPtr(const XalanMemMgrAutoPtr<Type>&	theSource) :
		m_pointerInfo(((XalanMemMgrAutoPtr<Type>&)theSource).release())
	{
	}


	XalanMemMgrAutoPtr<Type>&
	operator=(XalanMemMgrAutoPtr<Type>&	theRHS)
	{		
		if (this != &theRHS)
		{
			m_pointerInfo.deallocate();

			m_pointerInfo = theRHS.release();
		}

		return *this;
	}

	~XalanMemMgrAutoPtr()
	{
		m_pointerInfo.deallocate();
	}

	Type&
	operator*() const
	{
		return *m_pointerInfo.second;
	}

	Type*
	operator->() const
	{
		return m_pointerInfo.second;
	}

	Type*
	get() const
	{
		return m_pointerInfo.second;
	}

	MemMgrAutoPtrData
	release()
	{		
		MemMgrAutoPtrData tmp = m_pointerInfo;
	
		m_pointerInfo.reset( 0 , 0 ); 
		
		return MemMgrAutoPtrData(tmp);
	}
	
	void
	reset(	MemoryManagerType*  theManager = 0,
			Type*		thePointer = 0 )
	{		
		m_pointerInfo.deallocate();

		m_pointerInfo.reset ( theManager , thePointer );
	}

private:


// data member
	MemMgrAutoPtrData m_pointerInfo;
};




XALAN_CPP_NAMESPACE_END



#endif	// if !defined(XALANMEMMGRAUTOPTR_HEADER_GUARD_1357924680)
