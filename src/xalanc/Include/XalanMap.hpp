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

#if !defined(XALANMAP_HEADER_GUARD_1357924680)
#define XALANMAP_HEADER_GUARD_1357924680


// Base include file.  Must be first.
#include <xalanc/Include/PlatformDefinitions.hpp>



#include <cstddef>
#include <algorithm>
#include <functional>
#include <utility>


#include <xalanc/Include/XalanVector.hpp>
#include <xalanc/Include/XalanList.hpp>



XALAN_CPP_NAMESPACE_BEGIN

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4189)
#endif

typedef size_t	size_type;

template <class Key>
class XalanHasher : public XALAN_STD_QUALIFIER unary_function<Key, size_type>
{
public:
	size_type operator()(const Key& key) const
	{
		const char *byteArray = reinterpret_cast<const char*>(&key);

		size_type result = 0;

		for (size_type i = 0; i < sizeof(Key); ++i)
		{
			result = (result << 1) ^ byteArray[i];
		}

		return result;
	}
};

template <class Key>
struct XalanMapKeyTraits
{
	typedef XalanHasher<Key>					Hasher;
	typedef XALAN_STD_QUALIFIER equal_to<Key>	Comparator;
};


template <class Key>
struct XalanHashMemberPointer
{

	size_type operator() (const Key * key) const
	{
		assert (key != 0);
		return key->hash();
	}
};

template <class Key>
struct XalanHashMemberReference
{

	size_type operator() (const Key& key) const
	{
		return key.hash();
	}
};



template <class Value>
struct XalanMapIteratorTraits
{
	typedef Value	        value_type;
	typedef Value&	        reference;
	typedef Value*	        pointer;
};

template <class Value>
struct XalanMapConstIteratorTraits
{
	typedef Value	        value_type;
	typedef const Value&	reference;
	typedef const Value*	pointer;
};

template <class XalanMapTraits, class BaseIterator>
struct XalanMapIterator
{
    typedef typename XalanMapTraits::value_type         value_type;
    typedef typename XalanMapTraits::reference          reference;
    typedef typename XalanMapTraits::pointer            pointer;

	typedef ptrdiff_t		                    difference_type;
	typedef XALAN_STD_QUALIFIER bidirectional_iterator_tag iterator_category;

    typedef XalanMapIterator<
		XalanMapIteratorTraits<value_type>, 
  		BaseIterator>	                                Iterator; 

    XalanMapIterator(const Iterator & theRhs) :
        baseIterator(theRhs.baseIterator)
    {
    }

    XalanMapIterator(const BaseIterator & theRhs) :
        baseIterator(theRhs)
    {
    }

    XalanMapIterator operator++(int)
    {
	    XalanMapIterator temp(*this);
	    ++baseIterator;
	    return temp;
    }

    XalanMapIterator& operator++()
    {
        ++baseIterator;
        return *this;
    }

    reference operator*() const
    {
        return *baseIterator->value;
    }

    pointer operator->() const
    {
        return baseIterator->value;
    }

    bool operator==(const XalanMapIterator& theRhs) const
    {
        return theRhs.baseIterator == baseIterator;
    }

    bool operator!=(const XalanMapIterator& theRhs) const
    {
        return !(theRhs == *this);
    }

    BaseIterator baseIterator;
};



/**
  * Xalan implementation of a hashtable.
  *
  */
template <
		class Key, 
		class Value,
		class KeyTraits = XalanMapKeyTraits<Key> >
class XalanMap
{
public:
	/**
	 * Each map entry is stored in a linked list where an entry
     * consists of a pointer to the key/value pair and a flag to indicate
     * whether the entry has been erased.
     * The hash buckets are a vector of pointers into the entry list.
     * Deleted entries are spliced into another list and marked 'erased'.
     */

	typedef Key					key_type;
	typedef Value				data_type;
	typedef size_t				size_type;

	typedef XALAN_STD_QUALIFIER pair<const key_type, data_type>   value_type;

    struct Entry
    {
        value_type* value;
        bool        erased;

        Entry(value_type* theValue) :
            value(theValue),
            erased(false)
        {
        }
    };

    typedef XalanList<Entry>                                EntryListType;

    typedef XalanVector<typename EntryListType::iterator>	BucketType;
    typedef XalanVector<BucketType, ConstructWithMemoryManagerTraits<BucketType> >      BucketTableType;

    typedef XalanMapIterator<
                XalanMapIteratorTraits<value_type>, 
                typename EntryListType::iterator>           iterator;
    typedef XalanMapIterator<
                XalanMapConstIteratorTraits<value_type>, 
                typename EntryListType::iterator>     const_iterator;

    typedef typename MemoryManagedConstructionTraits<key_type>::Constructor  FirstConstructor;
    typedef typename MemoryManagedConstructionTraits<data_type>::Constructor SecondConstructor;

	XalanMap(
			MemoryManagerType& theMemoryManager,
			float loadFactor = 0.75,
			size_type minBuckets = 10) :
		m_memoryManager(&theMemoryManager),
		m_loadFactor(loadFactor),
		m_minBuckets(minBuckets),
		m_size(0),
		m_entries(theMemoryManager),
        m_freeEntries(theMemoryManager),
		m_buckets(theMemoryManager)
	{
	}

	XalanMap(
			const XalanMap &theRhs,
			MemoryManagerType&  theMemoryManager) :
        m_memoryManager(&theMemoryManager),
		m_loadFactor(theRhs.m_loadFactor),
		m_minBuckets(10),
		m_size(0),
		m_entries(theMemoryManager),
        m_freeEntries(theMemoryManager),
		m_buckets(size_type(m_loadFactor * theRhs.size())+ 1, BucketType(*m_memoryManager), theMemoryManager)
	{
		const_iterator entry = theRhs.begin();
		while(entry != theRhs.end())
		{
			insert(*entry);
			++entry;
		}

		assert(m_size == theRhs.m_size);
	}

    MemoryManagerType&
    getMemoryManager()
    {
        assert (m_memoryManager != 0);

        return *m_memoryManager;
    }

	~XalanMap()
	{
        doRemoveEntries();

        if (!m_buckets.empty())
        {
            typename EntryListType::iterator toRemove = m_freeEntries.begin();
            while(toRemove != m_freeEntries.end())
            {
                deallocate(toRemove->value);
                ++toRemove;
            }      
        }
	}

	XalanMap & operator=(const XalanMap& theRhs) 
	{
		XalanMap theTemp(theRhs, *m_memoryManager);
		swap(theTemp);
		return *this;
	}

	size_type size() const
	{
		return m_size;
	}

	bool empty() const
    {
		return m_size == 0;
	}

	iterator begin()
	{
		return m_entries.begin();
	}

	const_iterator begin() const
	{
        return  const_cast<XalanMap*>(this)->begin();
	}

	iterator end()
	{
		return m_entries.end();
	}

	const_iterator end() const 
	{
        return const_cast<XalanMap*>(this)->end();
	}

	iterator find(const key_type& key)
	{
		if (!m_buckets.empty())
		{
			const size_type     index = doHash(key);
            assert(index < m_buckets.size());

            BucketType & bucket = m_buckets[index];
            typename BucketType::iterator pos = bucket.begin();

			while (pos != bucket.end())
			{
				if (!(*pos)->erased && m_equals(key, (*pos)->value->first))
				{
					return iterator(*pos);
				}
				++pos;
			}
		}

		return end();
	}

	const_iterator find(const key_type& key) const 
	{
		return const_cast<XalanMap *>(this)->find(key);
	}

	data_type & operator[](const key_type& key)
	{
		iterator pos = find(key);

		if (pos == end())
		{
			pos = doCreateEntry(key);
		}

		return (*pos).second;
	}

	void insert(const value_type& value)
	{
		const key_type& key = value.first;
		const data_type& data = value.second;

		insert(key, data);
	}

    void insert(const key_type& key, const data_type& data)
	{
		const const_iterator    pos = find(key);

		if (pos == end())
		{
			doCreateEntry(key, &data);
		}
	}

	void erase(iterator pos)
	{
		if (pos != end())
		{
			doRemoveEntry(pos);
		}
	}

	size_type erase(const key_type& key)
	{
		const iterator  pos = find(key);

        if (pos != end())
        {
		    erase(pos);
            return 1;
        }
        else
        {
            return 0;
        }
	}

	void clear() 
	{
        doRemoveEntries();

		typename BucketTableType::iterator bucketPos = m_buckets.begin();
		while (bucketPos != m_buckets.end())
		{
			bucketPos->clear();
			++bucketPos;
		}

        assert(0 == m_size);
        assert(m_entries.empty());
    }

	void swap(XalanMap& theRhs)
	{
		size_type tempSize = m_size;
		m_size = theRhs.m_size;
		theRhs.m_size = tempSize;

		MemoryManagerType* tempMemoryManager = m_memoryManager;
		m_memoryManager = theRhs.m_memoryManager;
		theRhs.m_memoryManager = tempMemoryManager;

		m_entries.swap(theRhs.m_entries);
        m_freeEntries.swap(theRhs.m_freeEntries);
		m_buckets.swap(theRhs.m_buckets);
	}

protected:

    iterator doCreateEntry(const key_type & key, const data_type*  data = 0)
	{
		// if there are no buckets, create initial minimum set of buckets
		if (m_buckets.empty())
		{
			m_buckets.insert(m_buckets.begin(), m_minBuckets, BucketType(*m_memoryManager));
		}

		// if the load factor has been reached, rehash
		if (size_type(m_loadFactor * size()) > m_buckets.size())
		{
			rehash();
		}

		size_type index = doHash(key);

        
        if (m_freeEntries.empty())
        {
            m_freeEntries.push_back(Entry(allocate(1)));
        }
        
        // insert a new entry as the first position in the bucket
        Entry& newEntry = m_freeEntries.back();
        newEntry.erased = false;

        FirstConstructor::construct(const_cast<key_type*>(&newEntry.value->first), key, *m_memoryManager);
        if (data != 0)
        {
            SecondConstructor::construct(&newEntry.value->second, *data, *m_memoryManager);
        }
        else
        {
             SecondConstructor::construct(&newEntry.value->second, *m_memoryManager);
        }


        m_entries.splice(m_entries.end(), m_freeEntries, --m_freeEntries.end());

        m_buckets[index].push_back(--m_entries.end());

        ++m_size;

		return iterator(--m_entries.end());
	}

	void doRemoveEntry(const iterator & toRemovePos)
	{   
        value_type& toRemove = *toRemovePos;
#if defined(_MSC_VER) && _MSC_VER <= 1300
        toRemove.value_type::~value_type();
#else
        toRemove.~value_type();
#endif
        m_freeEntries.splice(
                m_freeEntries.end(), 
                m_entries, 
                toRemovePos.baseIterator);

        toRemovePos.baseIterator->erased = true;

		--m_size;
	}

    void
    doRemoveEntries()
    {
        while(size() > 0)
        {
            doRemoveEntry(begin());
        }
    }

	size_type doHash(const Key & key) const
	{
		return m_hash(key) % m_buckets.size();
	}

	void rehash()
	{
		// grow the number of buckets by 60%
		BucketTableType temp(size_type(1.6 * size()), BucketType(*m_memoryManager), *m_memoryManager);
		m_buckets.swap(temp);
	
        // rehash each entry assign to bucket and insert into list
		typename EntryListType::iterator entryPos = m_entries.begin();
		while (entryPos != m_entries.end())
		{
        	size_type index = doHash(entryPos->value->first);
			m_buckets[index].push_back(entryPos);
            ++entryPos;
		}
	}

    value_type*
    allocate(size_type  size)
    {
        const size_type     theBytesNeeded = size * sizeof(value_type);

        assert( m_memoryManager != 0 );

        void*   pointer = m_memoryManager->allocate(theBytesNeeded);

        assert(pointer != 0);

        return (value_type*) pointer;
    }

    void
    deallocate(value_type*  pointer)
    {
        if (m_memoryManager == 0)
        {
            ::operator delete(pointer);
        }
        else
        {
            m_memoryManager->deallocate(pointer);
        }
    }

	// Data members...
	typename KeyTraits::Hasher			m_hash;
		
	typename KeyTraits::Comparator		m_equals;

    MemoryManagerType*					m_memoryManager;

	float								m_loadFactor;

	size_type							m_minBuckets;

	size_type							m_size;

    EntryListType						m_entries;

    EntryListType                       m_freeEntries;

    BucketTableType 					m_buckets;

private:
    // not defined
    XalanMap();
    XalanMap(const XalanMap&);
};



#if defined(_MSC_VER)
#pragma warning(pop)
#endif



XALAN_CPP_NAMESPACE_END



#endif  // XALANMAP_HEADER_GUARD_1357924680

