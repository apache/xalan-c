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
#include <list>
#include <algorithm>
#include <functional>
#include <utility>



#include <xercesc/framework/MemoryManager.hpp>



#include <xalanc/Include/XalanVector.hpp>



XALAN_CPP_NAMESPACE_BEGIN

typedef size_t	size_type;

template <class Key>
class XalanHash : public XALAN_STD_QUALIFIER unary_function<Key, size_type>
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
class XalanHashPointer
{
public:
	size_type operator() (const Key *key) const
	{
		assert(key != 0);
		return Hash<Key>()(*key);
	}
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

template <
		class Key, 
		class Value, 
		class Hash = XalanHash<Key>, 
		class Comparator = XALAN_STD_QUALIFIER equal_to<Key> >
class XalanMap
{
public:

	typedef XERCES_CPP_NAMESPACE_QUALIFIER MemoryManager    MemoryManagerType;

	typedef Key					key_type;
	typedef Value				data_type;
	typedef size_t				size_type;

	typedef XALAN_STD_QUALIFIER pair<const key_type, data_type>	value_type;

	typedef XalanMap<Key, Value, Hash, Comparator>		ThisType;
	
	typedef size_t	size_type;

	struct Entry : public value_type
	{
		typedef value_type Parent;
		size_type	bucketIndex;

		Entry(const key_type & key, const data_type& data, size_type index) : 
			value_type(key, data), bucketIndex(index)
		{
		}
		
	};

	typedef XALAN_STD_QUALIFIER list<Entry>					EntryListType;
	typedef typename EntryListType::iterator				EntryListIterator;
	typedef typename EntryListType::const_iterator			EntryListConstIterator;

	typedef XalanVector<typename EntryListType::iterator>	EntryPosVectorType;

	template<class Value, class Ref, class Ptr, class Iterator, class Map>
	struct iterator_base
	{	
		typedef Value	value_type;
		typedef Ref		reference_type;
		typedef Ptr		pointer_type;

		typedef ThisType MapType;

		typedef iterator_base<value_type, reference_type, pointer_type, Iterator, Map> ThisType;

		typedef iterator_base<value_type, value_type&, value_type*, EntryListIterator, MapType> iterator;

		iterator_base(
				Map&								map,
				Iterator							bucketPos) :
			m_map(&map),
			m_bucketPos(bucketPos)
		{
		}

		iterator_base(const iterator& theRhs) :
			m_map(theRhs.m_map),
			m_bucketPos(theRhs.m_bucketPos)
		{
		} 
		
		const ThisType & operator=(const ThisType& theRhs)
		{
			m_map = theRhs.m_map;
			m_bucketPos = theRhs.m_bucketPos;
			return *this;
		}

		reference_type operator*() const
		{
			return *m_bucketPos;
		}

		int operator!=(const ThisType& theRhs) const 
		{
			return !operator==(theRhs);
		}

		int operator==(const ThisType& theRhs) const 
		{
			return (theRhs.m_map == m_map)
				&& (theRhs.m_bucketPos == m_bucketPos);
		}

		ThisType& operator++()
		{
			m_bucketPos++;	
			return *this;
		}

		typename Map*		m_map;
		typename Iterator	m_bucketPos;
	};

	typedef iterator_base<
		value_type, 
		value_type&, 
		value_type*, 
		EntryListIterator, 
		ThisType> iterator;

	typedef iterator_base<
		value_type, 
		const value_type&, 
		const value_type*, 
		EntryListConstIterator,  
		const ThisType> const_iterator;

	XalanMap(
			float loadFactor = 0.75,
			MemoryManagerType* theMemoryManager = 0) :
		m_memoryManager(theMemoryManager),
		m_loadFactor(loadFactor),
		m_size(0),
		m_entries(/* m_memoryManager */),
		m_buckets(10, m_entries.end(), m_memoryManager),
		m_freeList()
	{
	}

	XalanMap(const XalanMap &theRhs) :
		m_memoryManager(theRhs.m_memoryManager),
		m_loadFactor(theRhs.m_loadFactor),
		m_size(0),
		m_entries(/* m_memoryManager */),
		m_buckets(size_type(m_loadFactor * theRhs.size())+ 1, m_entries.end(), m_memoryManager),
		m_freeList()
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
		return iterator(*this, m_entries.begin());
	}

	const_iterator begin() const
	{
		return const_iterator(*this, m_entries.begin());
	}

	iterator end()
	{
		return iterator(*this, m_entries.end());
	}

	const_iterator end() const 
	{
		return const_iterator(*this, m_entries.end());
	}

	iterator find(const key_type& key)
	{
		size_type index = doHash(key);

		EntryListIterator bucketPos = m_buckets[index];

		while (bucketPos != m_entries.end() &&
			   bucketPos->bucketIndex == index)
		{
			if (m_equals(key,bucketPos->first))
			{
				return iterator(*this,bucketPos);
			}
			++bucketPos;
		}

		return end();
	}

	const_iterator find(const key_type& key) const 
	{
		size_type index = doHash(key);

		EntryListConstIterator bucketPos = m_buckets[index];

		while (bucketPos != m_entries.end() &&
			   bucketPos->bucketIndex == index)
		{
			if (m_equals(key,bucketPos->first))
			{
				return const_iterator(*this,bucketPos);
			}
			++bucketPos;
		}

		return end();
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
			doRemoveEntry(pos.m_bucketPos);
		}
	}

	void erase(const key_type& key)
	{
		iterator pos = find(key);

		erase(pos);
	}

	void clear() 
	{
		m_size = 0;

		fill(m_buckets.begin(), m_buckets.end(), m_entries.end());

		m_freeList.splice(m_freeList.begin(), m_entries, m_entries.begin(), m_entries.end());
	}

	void swap(ThisType& theRhs)
	{
		size_type tempSize = m_size;
		m_size = theRhs.m_size;
		theRhs.m_size = tempSize;

		MemoryManagerType* tempMemoryManager = m_memoryManager;
		m_memoryManager = theRhs.m_memoryManager;
		theRhs.m_memoryManager = tempMemoryManager;

		m_entries.swap(theRhs.m_entries);
		m_buckets.swap(theRhs.m_buckets);
		m_freeList.swap(theRhs.m_freeList);

	}

	protected:

	iterator doCreateEntry(const key_type & key, const data_type&  data)
	{
		if (size_type(m_loadFactor * size()) > m_buckets.size())
		{
			rehash();
		}

		size_type index = doHash(key);

		EntryListIterator & bucketStartPos = m_buckets[index];

		if (m_freeList.empty() == true)
		{
			Entry newEntry = Entry(key, data, index);

			if (bucketStartPos == m_entries.end())
			{
				bucketStartPos = m_entries.insert(m_entries.end(), newEntry);
			}
			else
			{
				bucketStartPos = m_entries.insert(bucketStartPos, newEntry);
			}
		}
		else
		{
			(*m_freeList.begin()).~Entry();
			new (&*m_freeList.begin()) Entry(key, data, index);
		
			m_entries.splice(bucketStartPos, m_freeList, m_freeList.begin());
			--bucketStartPos;
		}

		++m_size;
		return iterator(*this, bucketStartPos);
	}

	void doRemoveEntry(const EntryListIterator & toRemoveIter)
	{
		size_type index = toRemoveIter->bucketIndex;
		EntryListType::iterator nextPosition = ++(EntryListType::iterator(toRemoveIter));

		if (m_buckets[index] == toRemoveIter)
		{
			if (nextPosition->bucketIndex == index)
			{
				m_buckets[index] = nextPosition;
			}
			else
			{
				m_buckets[index] = m_entries.end();
			}
		}

		m_freeList.splice(m_freeList.begin(), m_entries, toRemoveIter, nextPosition);
		--m_size;
	}

	size_type doHash(const Key & key) const
	{
		return m_hash(key) % m_buckets.size();
	}

	void rehash()
	{
		EntryPosVectorType temp(size_type(1.6 * size()), m_entries.end(), m_memoryManager);
		m_buckets.swap(temp);
		doRehashEntries();
	}

	void doRehashEntries()
	{
		EntryListType tempEntryList;
		tempEntryList.splice(tempEntryList.begin(),m_entries, m_entries.begin(), m_entries.end());

		
		while (tempEntryList.begin() != tempEntryList.end())
		{
			EntryListIterator entry = tempEntryList.begin();

			entry->bucketIndex = doHash(entry->first);
			EntryListIterator & bucketStartPos = m_buckets[entry->bucketIndex];

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
	Hash				m_hash;

	Comparator			m_equals;

    MemoryManagerType*  m_memoryManager;

	float				m_loadFactor;

	size_type			m_size;

	EntryListType		m_entries;

    EntryPosVectorType	m_buckets;

	EntryListType		m_freeList;

};



XALAN_CPP_NAMESPACE_END



#endif  // XALANMAP_HEADER_GUARD_1357924680

