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



#include <xalanc/Include/XalanMemoryManagement.hpp>

#include <cstddef>

#include <cassert>

#include <utility>



XALAN_CPP_NAMESPACE_BEGIN



// We're using our own auto_ptr-like class due to wide
// variations amongst the varous platforms we have to
// support
template<	class	Type, 
			bool	toCallDestructor = true>
class XalanMemMgrAutoPtr
{
public:

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
				if ( toCallDestructor ) 
				{
					second->~Type();
				}

				first->deallocate(second);
			}
		}
		
		void 
		reset(	MemoryManagerType* m_memoryManager ,
				Type*	m_dataPointer )
		{	
			invariants();

			first = m_memoryManager;
			
			second = m_dataPointer;

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
			MemoryManagerType&  theManager, 
			Type* ptr  ) : 
		m_pointerInfo(&theManager, ptr)
	{
	}	

	XalanMemMgrAutoPtr() :
		m_pointerInfo()
	{
	}
	
	XalanMemMgrAutoPtr(const XalanMemMgrAutoPtr<Type,toCallDestructor>&	theSource) :
		m_pointerInfo(((XalanMemMgrAutoPtr<Type>&)theSource).release())
	{
	}

	XalanMemMgrAutoPtr<Type,toCallDestructor>&
	operator=(XalanMemMgrAutoPtr<Type,toCallDestructor>&	theRHS)
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
	
	Type*
	releasePtr()
	{		
		MemMgrAutoPtrData tmp = release();
	
		return tmp.second;
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




template<	class Type>
class XalanMemMgrAutoPtrArray
{
public:

	typedef unsigned int size_type;

	class MemMgrAutoPtrArrayData 
	{
	public:
		MemMgrAutoPtrArrayData():
			m_memoryManager(0),
			m_dataArray(0),
			m_size(0)
		{
		}

		MemMgrAutoPtrArrayData(	
				MemoryManagerType*	memoryManager,
				Type*				dataPointer,
				size_type				size): 
			m_memoryManager(memoryManager),
			m_dataArray(dataPointer),
			m_size(size)
		{
			invariants();
		}
		
	
		bool
		isInitilized()const
		{
			return ( (m_memoryManager != 0) && (m_dataArray != 0)  && (m_size != 0) )? true : false;
		}
	
		void
		deallocate()
		{
			invariants();

			if ( isInitilized() )
			{			
				assert ( m_dataArray != 0 );

				for ( size_type i = 0; i < m_size ; ++i )
				{
					m_dataArray[i].~Type();
				}
				m_memoryManager->deallocate ( m_dataArray);
			}
		}
		
		void 
		reset(	MemoryManagerType*	memoryManager ,
				Type*				dataPointer,
				size_type				size)
		{	
			invariants();

			m_memoryManager = memoryManager;
			
			m_dataArray =	dataPointer;

			m_size = size;

			invariants();
		}	


		MemoryManagerType*		m_memoryManager;
		Type*					m_dataArray;
		size_type					m_size;

	private:
		void
		invariants()const
		{
			assert( isInitilized() ||
					( (m_memoryManager == 0) && (m_dataArray ==0) && (m_size == 0)) );
		}		
	};
	
	XalanMemMgrAutoPtrArray(
			MemoryManagerType&  theManager, 
			Type*				ptr,
			size_type			size) : 
		m_pointerInfo(&theManager, ptr, size)
	{
	}	

	XalanMemMgrAutoPtrArray() :
		m_pointerInfo()
	{
	}
	
	XalanMemMgrAutoPtrArray(const XalanMemMgrAutoPtrArray<Type>&	theSource) :
		m_pointerInfo(((XalanMemMgrAutoPtr<Type>&)theSource).release())
	{
	}


	XalanMemMgrAutoPtrArray<Type>&
	operator=(XalanMemMgrAutoPtrArray<Type>&	theRHS)
	{		
		if (this != &theRHS)
		{
			m_pointerInfo.deallocate();

			m_pointerInfo = theRHS.release();
		}

		return *this;
	}

	~XalanMemMgrAutoPtrArray()
	{
		m_pointerInfo.deallocate();
	}

	Type&
	operator*() const
	{
		return *m_pointerInfo.m_dataArray;
	}

	Type*
	operator->() const
	{
		return m_pointerInfo.m_dataArray;
	}

	Type*
	get() const
	{
		return m_pointerInfo.m_dataArray;
	}

	size_type
	getSize()const
	{
		return m_pointerInfo.m_size;
	}

	XalanMemMgrAutoPtrArray<Type>&
	operator++ ()
	{
		++m_pointerInfo.m_size;

		return *this;
	}

	XalanMemMgrAutoPtrArray<Type>
	operator++ (int)
	{
		XalanMemMgrAutoPtrArray<Type> temp = *this;
		++*this;

		return temp;
	}

	MemMgrAutoPtrArrayData
	release()
	{		
		MemMgrAutoPtrArrayData tmp = m_pointerInfo;
	
		m_pointerInfo.reset( 0 , 0 , 0); 
		
		return MemMgrAutoPtrArrayData(tmp);
	}

	Type*
	releasePtr()
	{		
		MemMgrAutoPtrArrayData tmp = release();
	
		
		return tmp.dataPointer;
	}
	
	void
	reset(	MemoryManagerType*  theManager = 0,
			Type*				thePointer = 0 ,
			size_type				size = 0)
	{		
		m_pointerInfo.deallocate();

		m_pointerInfo.reset ( theManager , thePointer , size );
	}
	
	Type&
	operator[](size_type	index) const
	{
		return m_pointerInfo.m_dataArray[index];
	}
	
private:


	// data member
	MemMgrAutoPtrArrayData m_pointerInfo;
};




XALAN_CPP_NAMESPACE_END



#endif	// if !defined(XALANMEMMGRAUTOPTR_HEADER_GUARD_1357924680)
