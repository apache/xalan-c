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



#include <xalanc/Include/XalanMemoryManagement.hpp>



#include <xalanc/Include/XalanVector.hpp>
#include <xalanc/Include/XalanList.hpp>



XALAN_CPP_NAMESPACE_BEGIN

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
struct XalanMapEntry : public Value
{
	typedef Value	value_type;
	typedef size_t	size_type;
	
	size_type	bucketIndex;

	XalanMapEntry(const value_type& value, size_type index) : 
		value_type(value), bucketIndex(index)
	{
	}

	XalanMapEntry() : 
		value_type(),
        bucketIndex(size_type())
	{
	}
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
	 * Each map entry is stored in a linked list.
	 * The hash buckets are a vector of pointers into the entry list
	 * An empty bucket will point to the end of the list,
	 * A non-empty bucket will point to its first entry.  Remaining
	 * entries in the chain follow the first and have the same index value.
	 */

	typedef Key					key_type;
	typedef Value				data_type;
	typedef size_t				size_type;

	typedef XALAN_STD_QUALIFIER pair<const key_type, data_type>	value_type;
	typedef XalanMapEntry<value_type> Entry;

	typedef XalanList<Entry>									EntryListType;

	typedef typename EntryListType::iterator					iterator;
	typedef typename EntryListType::const_iterator				const_iterator;

	typedef XalanVector<typename EntryListType::iterator>		EntryPosVectorType;

	XalanMap(
			MemoryManagerType* theMemoryManager = 0,
			float loadFactor = 0.75,
			size_type minBuckets = 10) :
		m_memoryManager(theMemoryManager),
		m_loadFactor(loadFactor),
		m_minBuckets(minBuckets),
		m_size(0),
		m_entries(m_memoryManager),
		m_buckets(m_memoryManager)
	{
	}

	XalanMap(
			const XalanMap &theRhs,
			MemoryManagerType*  theManager = 0) :
        m_memoryManager(theManager != 0 ? theManager : theRhs.m_memoryManager),
		m_loadFactor(theRhs.m_loadFactor),
		m_minBuckets(10),
		m_size(0),
		m_entries(m_memoryManager),
		m_buckets(size_type(m_loadFactor * theRhs.size())+ 1, m_entries.end(), m_memoryManager)
	{
		const_iterator entry = theRhs.begin();
		while(entry != theRhs.end())
		{
			insert(*entry);
			++entry;
		}

		assert(m_size == theRhs.m_size);
	}

	~XalanMap()
	{
	}

	XalanMap & operator=(const XalanMap& theRhs) 
	{
		XalanMap theTemp(theRhs);
		swap(theTemp);
		return *this;
	}

	size_type size() const
	{
		return m_size;
	}

	bool empty() const {
		return m_size == 0;
	}

	iterator begin()
	{
		return m_entries.begin();
	}

	const_iterator begin() const
	{
		return m_entries.begin();
	}

	iterator end()
	{
		return m_entries.end();
	}

	const_iterator end() const 
	{
		return m_entries.end();
	}

	iterator find(const key_type& key)
	{
		if (!m_buckets.empty())
		{
			size_type index = doHash(key);

			iterator bucketPos = m_buckets[index];

			while (bucketPos != m_entries.end() &&
				bucketPos->bucketIndex == index)
			{
				if (m_equals(key,bucketPos->first))
				{
					return bucketPos;
				}
				++bucketPos;
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
			pos = doCreateEntry(key, data_type());
		}

		return (*pos).second;
	}

	void insert(const value_type& value)
	{
		const key_type& key = value.first;
		const data_type& data = value.second;

		iterator pos = find(key);

		if (pos == end())
		{
			doCreateEntry(key, data);
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
		iterator pos = find(key);

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
		m_size = 0;

		XALAN_STD_QUALIFIER fill(
            m_buckets.begin(),
            m_buckets.end(),
            m_entries.end());

		m_entries.clear();
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
		m_buckets.swap(theRhs.m_buckets);
	}

	protected:

	iterator doCreateEntry(const key_type & key, const data_type&  data)
	{
		// if there are no buckets, create initial minimum set of buckets
		if (m_buckets.empty())
		{
			m_buckets.insert(m_buckets.begin(), m_minBuckets, m_entries.end());
		}

		// if the load factor has been reached, rehash
		if (size_type(m_loadFactor * size()) > m_buckets.size())
		{
			rehash();
		}

		size_type index = doHash(key);

		iterator & bucketStartPos = m_buckets[index];

		// insert a new entry as the first position in the bucket
		bucketStartPos = m_entries.insert(bucketStartPos, Entry(value_type(key, data), index));
			
		++m_size;
		return bucketStartPos;
	}

	void doRemoveEntry(const iterator & toRemovePos)
	{
		size_type index = toRemovePos->bucketIndex;
		iterator nextPos = ++(iterator(toRemovePos));

		// if the entry to remove is the first in the bucket
		// set the next entry as the first or,
		// if there are no more entries set it to the end
		if (m_buckets[index] == toRemovePos)
		{
			if (nextPos->bucketIndex == index)
			{
				m_buckets[index] = nextPos;
			}
			else
			{
				m_buckets[index] = m_entries.end();
			}
		}

		m_entries.erase(toRemovePos);
		--m_size;
	}

	size_type doHash(const Key & key) const
	{
		return m_hash(key) % m_buckets.size();
	}

	void rehash()
	{
		// grow the number of buckets by 60%
		EntryPosVectorType temp(size_type(1.6 * size()), m_entries.end(), m_memoryManager);
		m_buckets.swap(temp);
	
		// move current entries into a temporary list
		EntryListType tempEntryList;
		tempEntryList.splice(tempEntryList.begin(),m_entries, m_entries.begin(), m_entries.end());

		// rehash each entry assign to bucket and insert into list
		while (tempEntryList.begin() != tempEntryList.end())
		{
			iterator entry = tempEntryList.begin();

			entry->bucketIndex = doHash(entry->first);
			iterator & bucketStartPos = m_buckets[entry->bucketIndex];

			// if the bucket is empty assign to the entry and insert 
			// into the list, otherwise insert into front of the 
			// current first entry
			if (bucketStartPos == m_entries.end())
			{
				bucketStartPos = entry;
				m_entries.splice(m_entries.begin(), tempEntryList, entry);
			}
			else
			{
				m_entries.splice(bucketStartPos, tempEntryList, entry);
				--bucketStartPos;
			}
		}
	}

	// Data members...
	typename KeyTraits::Hasher					m_hash;
		
	typename KeyTraits::Comparator				m_equals;

    MemoryManagerType*					m_memoryManager;

	float								m_loadFactor;

	size_type							m_minBuckets;

	size_type							m_size;

	EntryListType						m_entries;

    EntryPosVectorType					m_buckets;

};



XALAN_CPP_NAMESPACE_END



#endif  // XALANMAP_HEADER_GUARD_1357924680

