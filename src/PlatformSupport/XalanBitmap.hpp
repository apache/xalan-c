/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2000 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */
#if !defined(XALANBITMAP_HEADER_GUARD_1357924680)
#define XALANBITMAP_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <PlatformSupport/PlatformSupportDefinitions.hpp>



#include <vector>



class XALAN_PLATFORMSUPPORT_EXPORT XalanBitmap
{
public:

	// The basic storage unit for the bitmaps.
	typedef char	UnitType;

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
	XalanBitmap(unsigned long	theSize);

	~XalanBitmap();


	/**
	 * Determine if a bit is set.
	 *
	 * @param  theBit   The number of the bit to check.
	 * @return true if the bit is set, false if not.
	 */
	bool
	isSet(unsigned long	theBit) const;

	/**
	 * Set a bit.
	 *
	 * @param theBit The number of the bit to set.
	 */
	void
	set(unsigned long	theBit);

	/**
	 * Clear a bit.
	 *
	 * @param theBit The number of the bit to clear.
	 */
	void
	clear(unsigned long	theBit);

	/**
	 * Toggle a bit.
	 *
	 * @param theBit The number of the bit to toggle.
	 */
	void
	toggle(unsigned long	theBit);

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
	unsigned long
	getSize() const
	{
		return m_size;
	}

private:

#if defined(XALAN_NO_NAMESPACES)
	typedef vector<UnitType>		BitmapVectorType;
#else
	typedef std::vector<UnitType>	BitmapVectorType;
#endif

	const unsigned long		m_size;

	BitmapVectorType		m_bitmap;
};



#endif	// XALANBITMAP_HEADER_GUARD_1357924680
