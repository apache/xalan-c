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
#if !defined(XALANBITMAP_HEADER_GUARD_1357924680)
#define XALANBITMAP_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



#include <vector>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_PLATFORMSUPPORT_EXPORT XalanBitmap
{
public:

	// The basic storage unit for the bitmaps.
	typedef char		UnitType;

	// A handy typedef...
	typedef size_t		size_type;

	// Really all we're assuming is that a char is at least
	// 8 bits.  If it's more, then we'll just waste some
	// space.  This may need to be adjusted for various
	// platforms, or perhaps change to using an integral of
	// a known size, so that we don't waste any space.
	enum { eBitsPerUnit = 8 };


	/**
	 * Construct an instance with room for the specified number
	 * of bits.
	 *
	 * @param  theSize   The number of bits in the map.
	 */
	XalanBitmap(size_type	theSize);

	~XalanBitmap();


	/**
	 * Determine if a bit is set.
	 *
	 * @param  theBit   The number of the bit to check.
	 * @return true if the bit is set, false if not.
	 */
	bool
	isSet(size_type		theBit) const;

	/**
	 * Set a bit.
	 *
	 * @param theBit The number of the bit to set.
	 */
	void
	set(size_type	theBit);

	/**
	 * Clear a bit.
	 *
	 * @param theBit The number of the bit to clear.
	 */
	void
	clear(size_type		theBit);

	/**
	 * Toggle a bit.
	 *
	 * @param theBit The number of the bit to toggle.
	 */
	void
	toggle(size_type	theBit);

	/**
	 * Clear all of the bits.
	 */
	void
	clearAll();

	/**
	 * Get the size of the map.
	 *
	 * @return The number of bits in the map.
	 */
	size_type
	getSize() const
	{
		return m_size;
	}

private:

#if defined(XALAN_NO_STD_NAMESPACE)
	typedef vector<UnitType>		BitmapVectorType;
#else
	typedef std::vector<UnitType>	BitmapVectorType;
#endif

	const size_type		m_size;

	BitmapVectorType	m_bitmap;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALANBITMAP_HEADER_GUARD_1357924680
