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
#if !defined(XALAN_OBJECTSTACKCACHE_HEADER_GUARD)
#define XALAN_OBJECTSTACKCACHE_HEADER_GUARD



#include <algorithm>



#include <xalanc/Include/XalanVector.hpp>
#include <xalanc/Include/STLHelper.hpp>
#include <xalanc/Include/XalanAutoPtr.hpp>
#include <xalanc/Include/XalanObjectCache.hpp>



XALAN_CPP_NAMESPACE_BEGIN


template<
class ObjectType,
#if defined(XALAN_NO_DEFAULT_TEMPLATE_ARGUMENTS)
class CreateFunctorType,
class DeleteFunctorType,
class ResetFunctorType>
#else
class CreateFunctorType = DefaultCacheCreateFunctor<ObjectType>,
class DeleteFunctorType = DeleteFunctor<ObjectType>,
class ResetFunctorType = DefaultCacheResetFunctor<ObjectType> >
#endif
class XalanObjectStackCache
{
public:

	typedef XalanVector<ObjectType*>			VectorType;

	typedef ObjectType	CacheObjectType;

	explicit
	XalanObjectStackCache(MemoryManagerType& theManager, 
                          unsigned int	initialListSize = 0) :
        m_createFunctor(),
        m_deleteFunctor(theManager),
	m_stack(theManager),
        m_numObjectsOnStack(0)
	{
		m_stack.reserve(initialListSize);

	}

	~XalanObjectStackCache()
	{
#if !defined(XALAN_NO_STD_NAMESPACE)
		using std::for_each;
#endif
		for_each(
				m_stack.begin(),
				m_stack.end(),
				m_deleteFunctor);
	}

	ObjectType*
	get()
	{
		
		if (m_stack.size() == m_numObjectsOnStack)
		{
            ObjectType* const	theNewObject = m_createFunctor(m_stack.getMemoryManager());
			m_stack.push_back(theNewObject);
			++m_numObjectsOnStack;
			return theNewObject;
		}
		else
		{
			return m_stack[m_numObjectsOnStack++];
		}
	}

	ObjectType*
	top()
	{
		assert (m_numObjectsOnStack > 0);

		return m_stack[m_numObjectsOnStack-1];
	}

	ObjectType*
	release()
	{
		assert(m_numObjectsOnStack > 0);

		return m_stack[--m_numObjectsOnStack];
	}

	void
	reset()
	{
		typename VectorType::iterator iterator;

		for (iterator = m_stack.begin(); iterator < m_stack.end(); iterator++)
		{
			m_resetFunctor(*iterator);
		}
	}

	// Functors for various operations...
	CreateFunctorType	m_createFunctor;

	DeleteFunctorType	m_deleteFunctor;

	ResetFunctorType	m_resetFunctor;

private:

	// There are not defined...
	XalanObjectStackCache(const XalanObjectCache<ObjectType, CreateFunctorType, DeleteFunctorType, ResetFunctorType>&	theRHS);

	XalanObjectStackCache<ObjectType, CreateFunctorType, DeleteFunctorType, ResetFunctorType>&
	operator=(const XalanObjectCache<ObjectType, CreateFunctorType, DeleteFunctorType, ResetFunctorType>&	theRHS);


	// Data members...
	VectorType			m_stack;

	typename VectorType::size_type m_numObjectsOnStack;

};




template<class ObjectType>
class XalanObjectStackCacheDefault : public XalanObjectStackCache<ObjectType, DefaultCacheCreateFunctor<ObjectType>, DeleteFunctor<ObjectType>, DefaultCacheResetFunctor<ObjectType> >
{
public:

	typedef XalanObjectStackCache<ObjectType, DefaultCacheCreateFunctor<ObjectType>, DeleteFunctor<ObjectType>, DefaultCacheResetFunctor<ObjectType> >		BaseClassType;

	explicit
	XalanObjectStackCacheDefault(unsigned int	initialListSize = 0) :
		BaseClassType(initialListSize)
	{
	}
};



XALAN_CPP_NAMESPACE_END



#endif
