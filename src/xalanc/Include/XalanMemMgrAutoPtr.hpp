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

    typedef XERCES_CPP_NAMESPACE_QUALIFIER MemoryManager    MemoryManagerType;

	XalanMemMgrAutoPtr(
			Type*	thePointer = 0, 
			MemoryManagerType*  theManager = 0) :
		m_memoryManager(theManager),
		m_pointer(thePointer)
	{
		invariants();
	}

	XalanMemMgrAutoPtr(const XalanMemMgrAutoPtr<Type>&	theSource) :
		m_memoryManager(theSource.m_memoryManager),
		m_pointer(((XalanMemMgrAutoPtr<Type>&)theSource).release())
	{
		invariants();
	}

	XalanMemMgrAutoPtr<Type>&
	operator=(XalanMemMgrAutoPtr<Type>&	theRHS)
	{
		invariants();
		
		theRHS.invariants();
		
		if (this != &theRHS)
		{

			if ( m_memoryManager != 0 && m_pointer != 0)
			{
				m_pointer->~Type();

				m_memoryManager->deallocate(m_pointer);
			}


			m_pointer = theRHS.release(m_memoryManager);
		}

		return *this;
	}

	~XalanMemMgrAutoPtr()
	{
		// See note in operator=() about this...
		if ( m_memoryManager != 0 && m_pointer!= 0 )
		{
			m_pointer->~Type();
			
			m_memoryManager->deallocate(m_pointer);
		}
	}

	Type&
	operator*() const
	{
		return *m_pointer;
	}

	Type*
	operator->() const
	{
		return m_pointer;
	}

	Type*
	get() const
	{
		return m_pointer;
	}

/**
 * Release , similar to the auto_ptr release , but returns a memory manager the pointer comes from
 * 
 * @param theManager   parameter to receive the memory manager
 */
	Type*
	release( /* OUT */ MemoryManagerType** theManager)
	{
		invariants();
		
		Type* const	temp = m_pointer;
		
		*theManager = m_memoryManager;
		
		m_pointer = 0;
		
		m_memoryManager = 0;
		
		return temp;
	}
	
/**
 * Release , similar to the auto_ptr release. Note , that the Type* pointer can't be "deleted"
 */
	Type*
	release()
	{
		invariants();
		
		Type* const	temp = m_pointer;
		
		m_pointer = 0;
		
		m_memoryManager = 0;
		
		return temp;
	}
	
	void
	reset(	 Type*		thePointer = 0,
			 MemoryManagerType*  theManager = 0 )
	{
		// check the correctness before ...
		invariants();
		
		// See note in operator=() about this...
		if ( m_memoryManager != 0 && m_pointer!=0 )
		{
			m_pointer->~Type();
			
			m_memoryManager->deallocate(m_pointer);
		}

		m_memoryManager = theManager;

		m_pointer = thePointer;
		
		// check the correctness after ...
		invariants();
	}

private:
#if defined(NDEBUG)
    void
    invariants() const
    {
    }
#else
    void
    invariants() const
    {
        assert ( (m_pointer != 0 && m_memoryManager != 0 ) || 
					(m_pointer == 0 && m_memoryManager == 0 ) );
    }
#endif

// data members
	MemoryManagerType* m_memoryManager;

	Type*	m_pointer;
};




XALAN_CPP_NAMESPACE_END



#endif	// if !defined(XALANMEMMGRAUTOPTR_HEADER_GUARD_1357924680)
