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
#if !defined(XALAN_ARRAYKEYMAP_HEADER_GUARD)
#define XALAN_ARRAYKEYMAP_HEADER_GUARD



#include <deque>
#include <map>
#include <vector>



XALAN_CPP_NAMESPACE_BEGIN



template<class KeyType, class ValueType, class CompareType>
class XalanArrayKeyMap
{
public:

#if defined(XALAN_NO_STD_NAMESPACE)
	typedef vector<KeyType>									VectorType;
	typedef map<const KeyType*, ValueType, CompareType>		MapType;
	typedef deque<VectorType>								VectorHolderType;
#else
	typedef std::vector<KeyType>								VectorType;
	typedef std::map<const KeyType*, ValueType, CompareType>	MapType;
	typedef std::deque<VectorType>								VectorHolderType;
#endif

	typedef typename MapType::key_type					key_type;
	typedef typename MapType::value_type				value_type;
	typedef ValueType									referent_type;
	typedef CompareType									key_compare;
#if !defined(XALAN_NO_STD_ALLOCATORS)
	typedef typename MapType::allocator_type			allocator_type;
#endif
	typedef typename MapType::size_type					size_type;
	typedef typename MapType::difference_type			difference_type;
	typedef typename MapType::reference					reference;
	typedef typename MapType::const_reference			const_reference;
	typedef typename MapType::iterator					iterator;
	typedef typename MapType::const_iterator			const_iterator;
	typedef typename MapType::reverse_iterator			reverse_iterator;
	typedef typename MapType::const_reverse_iterator	const_reverse_iterator;

#if defined(XALAN_NO_STD_NAMESPACE)
	typedef pair<iterator, bool>					insert_pair_type;
	typedef pair<iterator, iterator>				range_pair_type;
	typedef pair<const_iterator, const_iterator>	const_range_pair_type;
#else
	typedef std::pair<iterator, bool>					insert_pair_type;
	typedef std::pair<iterator, iterator>				range_pair_type;
	typedef std::pair<const_iterator, const_iterator>	const_range_pair_type;
#endif

	explicit
	XalanArrayKeyMap() :
		m_map(),
		m_keyData()
	{
	}

	XalanArrayKeyMap(const XalanArrayKeyMap<KeyType, ValueType, CompareType>&	theOther)
	{
		*this = theOther;
	}

	~XalanArrayKeyMap()
	{
	}

	XalanArrayKeyMap<KeyType, ValueType, CompareType>&
	operator=(const XalanArrayKeyMap<KeyType, ValueType, CompareType>&	theRHS)
	{
		if (&theRHS != this)
		{
			XalanArrayKeyMap<KeyType, ValueType, CompareType>	theTemp;

			const const_iterator	theEnd =
				theRHS.end();

			const_iterator	i = theRHS.begin();

			while(i != theEnd)
			{
				theTemp.insert(*i);

				++i;
			}

			swap(theTemp);
		}

		return *this;
	}

	bool
	operator==(const XalanArrayKeyMap<KeyType, ValueType, CompareType>&		theRHS) const
	{
		return m_map == theRHS.m_map;
	}

	size_type
	size() const
	{
		return m_map.size();
	}

	size_type
	max_size() const
	{
		return m_map.max_size();
	}

	bool
	empty() const
	{
		return m_map.empty();
	}

	iterator
	begin()
	{
		return m_map.begin();
	}

	const_iterator
	begin() const
	{
		return m_map.begin();
	}

	iterator
	end()
	{
		return m_map.end();
	}

	const_iterator
	end() const
	{
		return m_map.end();
	}

	reverse_iterator
	rbegin()
	{
		return m_map.rbegin();
	}

	const_reverse_iterator
	rbegin() const
	{
		return m_map.rbegin();
	}

	reverse_iterator
	rend()
	{
		return m_map.rend();
	}

	const_reverse_iterator
	rend() const
	{
		return m_map.rend();
	}

	insert_pair_type
	insert(const value_type&	thePair)
	{
		m_keyData.push_back(VectorHolderType::value_type(thePair.first, thePair.first + (length(thePair.first) + 1)));

		return m_map.insert(value_type(&*m_keyData.back().begin(), thePair.second));
	}

	referent_type&
	operator[](const key_type&	theKey)
	{
		const iterator	i = find(theKey);

		if (i == end())
		{
			return (*(insert(value_type(theKey, referent_type()))).first).second;
		}
		else
		{
			return (*i).second;
		}
	}

	iterator
	find(const key_type&	theKey)
	{
		return m_map.find(theKey);
	}

	const_iterator
	find(const key_type&	theKey) const
	{
		return m_map.find(theKey);
	}

	iterator
	lower_bound(const key_type&		theKey)
	{
		return m_map.lower_bound(theKey);
	}

	const_iterator
	lower_bound(const key_type&		theKey) const
	{
		return m_map.lower_bound(theKey);
	}

	iterator
	upper_bound(const key_type&		theKey)
	{
		return m_map.upper_bound(theKey);
	}

	const_iterator
	upper_bound(const key_type&		theKey) const
	{
		return m_map.upper_bound(theKey);
	}

	range_pair_type
	equal_range(const key_type&		theKey)
	{
		return m_map.equal_range(theKey);
	}

	const_range_pair_type
	equal_range(const key_type&		theKey) const
	{
		return m_map.equal_range(theKey);
	}

#if defined(XALAN_STLPORT_STL) && !defined(__STL_MEMBER_TEMPLATES)
	void
	erase(iterator	theIterator)
	{
		// $$$ ToDo: Does not empty vector in the
		// deque!!!
		m_map.erase(theIterator);
	}

	void
	erase(
			iterator	theFirst,
			iterator	theLast)
	{
		// $$$ ToDo: Does not empty vector in the
		// deque!!!
		m_map.erase(theFirst, theLast);
	}
#else
	iterator
	erase(iterator	theIterator)
	{
		// $$$ ToDo: Does not empty vector in the
		// deque!!!
		return m_map.erase(theIterator);
	}

	iterator
	erase(
			iterator	theFirst,
			iterator	theLast)
	{
		// $$$ ToDo: Does not empty vector in the
		// deque!!!
		return m_map.erase(theFirst, theLast);
	}
#endif

	size_type
	erase(const key_type&	theKey)
	{
		// $$$ ToDo: Does not empty vector in the
		// deque!!!
		return m_map.erase(theKey);
	}

	void
	swap(XalanArrayKeyMap<KeyType, ValueType, CompareType>&		theOther)
	{
		m_map.swap(theOther.m_map);

		m_keyData.swap(theOther.m_keyData);
	}

private:

	size_type
	length(const key_type&	theKey)
	{
		key_type	theCurrent = theKey;

		while(*theCurrent != 0)
		{
			++theCurrent;
		}

		return theCurrent - theKey;
	}

	MapType				m_map;

	VectorHolderType	m_keyData;
};



XALAN_CPP_NAMESPACE_END



#endif
