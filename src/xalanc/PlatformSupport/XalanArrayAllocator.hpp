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
#if !defined(XALANARRAYALLOCATOR_HEADER_GUARD_1357924680)
#define XALANARRAYALLOCATOR_HEADER_GUARD_1357924680



#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



#include <cassert>
#include <utility>


#include <xalanc/Include/XalanMemMgrHelper.hpp>
#include <xalanc/Include/XalanVector.hpp>
#include <xalanc/Include/XalanList.hpp>



XALAN_CPP_NAMESPACE_BEGIN



template<class Type>
class XALAN_PLATFORMSUPPORT_EXPORT XalanArrayAllocator
{
public:

	typedef XalanVector<Type>				VectorType;
	typedef typename VectorType::size_type	size_type;

	typedef XALAN_STD_QUALIFIER pair<size_type, VectorType * >		ListEntryType;
	typedef XalanList<ListEntryType>							    ListType;

	typedef Type							value_type;

	typedef typename ListType::iterator		ListIteratorType;

    // Default size for vector allocation.
	enum { eDefaultBlockSize = 500 };

	/**
	 * Constructor.
	 *
	 * @param theBlockSize The block size when allocating.
	 */
	XalanArrayAllocator(MemoryManagerType&       theManager,
                        size_type	theBlockSize = eDefaultBlockSize) :
		m_list(theManager),
		m_blockSize(theBlockSize),
		m_lastEntryFound(0)
	{
	}

    ~XalanArrayAllocator()
    {        
        typename ListType::iterator iter = m_list.begin();

        MemoryManagerType& theManager = m_list.getMemoryManager();

        for( iter = m_list.begin(); iter != m_list.end(); ++iter)
        {
            if( (*iter).second != 0)
            {
#if defined(XALAN_REQUIRES_QUALIFIED_DESTRUCTOR)
				(*iter).second->VectorType::~VectorType();
#else
                (*iter).second->~VectorType();
#endif               
                theManager.deallocate((void*)(*iter).second);
            }
        }
    }

	/**
	 * Clear the instance, and release all allocated memory
	 */
	void
	clear()
	{
		m_list.clear();

		m_lastEntryFound = 0;
	}

	/**
	 * Reset the instance, but keep all memory so it can be
	 * reused for allocations.  This invalidates all previous
	 * allocations.
	 */
	void
	reset()
	{
		if (m_list.empty() == true)
		{
			m_lastEntryFound = 0;
		}
		else
		{
			const ListIteratorType	theEnd = m_list.end();
			ListIteratorType		theCurrent = m_list.begin();

			do
			{
				(*theCurrent).first = (*theCurrent).second->size();

				++theCurrent;
			} while(theCurrent != theEnd);

			m_lastEntryFound = &*m_list.begin();
		}
	}

	/**
	 * Allocate slots for the given number of Types
	 * instance and return the address of the slots.
	 *
	 * @param theCount The number of slots to allocate
	 */
	Type*
	allocate(size_type	theCount)
	{
		// Handle the case of theCount being greater than the block size first...
		if (theCount >= m_blockSize)
		{
			return createEntry(theCount, theCount);
		}
		else
		{
			ListEntryType*	theEntry =
				findEntry(theCount);

			// Did we find a slot?
			if (theEntry == 0)
			{
				// Nope, create a new one...
				return createEntry(m_blockSize, theCount);
			}
			else
			{
				// The address we want is that of the first free element in the
				// vector...
                assert( theEntry->second != 0);
				Type* const		thePointer =
					&*theEntry->second->begin() + (theEntry->second->size() - theEntry->first);

				// Resize the vector to the appropriate size...
				theEntry->first -= theCount;

				return thePointer;
			}
		}
	}

private:

	// Utility functions...
	Type*
	createEntry(
			size_type	theBlockSize,
			size_type	theCount)
	{
		assert(theBlockSize >= theCount);

		// Push on a new entry.  The entry has no open space,
		// since it's greater than our block size...
        m_list.push_back(ListEntryType(0, VectorType::create(m_list.getMemoryManager())));

		// Get the new entry...
		ListEntryType&	theNewEntry = m_list.back();

		// Resize the vector to the appropriate size...
        assert(theNewEntry.second);

		theNewEntry.second->resize(theBlockSize, value_type());

		// Set the number of free spaces accordingly...
		theNewEntry.first = theBlockSize - theCount;

		if (theNewEntry.first != 0)
		{
			m_lastEntryFound = &theNewEntry;
		}

		// Return a pointer to the beginning of the allocated memory...
		return &*theNewEntry.second->begin();
	}

	ListEntryType*
	findEntry(size_type		theCount)
	{
		// Search for an entry that has some free space.

		if (m_lastEntryFound != 0 && m_lastEntryFound->first >= theCount)
		{
			return m_lastEntryFound;
		}
		else
		{
			const ListIteratorType	theEnd = m_list.end();
			ListIteratorType		theCurrent = m_list.begin();

			ListEntryType*	theEntry = 0;

			while(theCurrent != theEnd)
			{
				// We're looking for the best fit, so
				// if the free space and the count we're
				// looking for are equal, that's pretty
				// much the best we can do...
				if ((*theCurrent).first == theCount)
				{
					theEntry = &*theCurrent;

					break;
				}
				else if ((*theCurrent).first >= theCount)
				{
					// If we haven't found anything yet, the first
					// entry we find that's large enough becomes
					// our best fit.
					//
					// Otherwise, we'll assume that a smaller
					// slot is a better fit, though I may be
					// wrong about this...
					if (theEntry == 0 ||
						(*theCurrent).first < theEntry->first)
					{
						// Nope, so this becomes our best-fit so far.
						theEntry = &*theCurrent;
					}

					++theCurrent;
				}
				else
				{
					// Won't fit, so just continue...
					++theCurrent;
				}
			}

			m_lastEntryFound = theEntry;

			return theEntry;
		}
	}

	// Not implemented...
	XalanArrayAllocator(const XalanArrayAllocator<Type>&	theSource);

	XalanArrayAllocator<Type>&
	operator=(const XalanArrayAllocator<Type>&	theSource);

	bool
	operator==(const XalanArrayAllocator<Type>&	theRHS) const;


	// Data members...
	ListType			m_list;

	const size_type		m_blockSize;

	ListEntryType*		m_lastEntryFound;
};



XALAN_CPP_NAMESPACE_END



#endif	// !defined(XALANARRAYALLOCATOR_HEADER_GUARD_1357924680)
