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

#if !defined(XALANALLOCATOR_INCLUDE_GUARD_1357924680)
#define XALANALLOCATOR_INCLUDE_GUARD_1357924680



#include <cstddef>



XALAN_CPP_NAMESPACE_BEGIN



template <class Type>
class XalanAllocator
{
public:
	typedef size_t			size_type;
	typedef ptrdiff_t		difference_type;
	typedef Type*			pointer;
	typedef const Type*		const_pointer;
	typedef Type&			reference;
	typedef const Type&		const_reference;
	typedef Type			value_type;

	
	XalanAllocator(MemoryManagerType&      theManager) :
        m_memoryManager(theManager)
	{
	}


	~XalanAllocator()
	{
	}
	
    MemoryManagerType&
    getMemoryManager()
    {
        return m_memoryManager;
    }
    
	pointer
	address(reference	x) const
	{
		return &x;
	}

	const_pointer
	address(const_reference		x) const
	{
		return &x;
	}

	pointer
	allocate(
			size_type		size,
			const void*		/* hint */ = 0)
	{
		return (pointer)m_memoryManager.allocate(size * sizeof(Type));
	}

	void
	deallocate(
				pointer		p,
				size_type	/* n */)
	{
        if( p == 0 )
        {
            return;
        }

        m_memoryManager.deallocate(p);
	}

	size_type
	max_size() const
	{
		return ~0;
	}

	void
	construct(
			pointer			p,
			const Type&		val)
	{
		new (p) Type(val);
	}

	void
	destroy(pointer		p)
	{
		p->Type::~Type();
	}
	
private:
	XalanAllocator(const XalanAllocator<Type>&);
	
    XalanAllocator<Type>&
    operator=(const XalanAllocator<Type>&);
    
    MemoryManagerType&      m_memoryManager;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALANALLOCATOR_INCLUDE_GUARD_1357924680
