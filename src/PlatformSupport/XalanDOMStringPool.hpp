/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2000 The Apache Software Foundation.  All rights 
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
#if !defined(XALANDOMSTRINGPOOL_HEADER_GUARD_1357924680)
#define XALANDOMSTRINGPOOL_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <PlatformSupport/PlatformSupportDefinitions.hpp>



#include <deque>
#include <map>



#include <PlatformSupport/DOMStringHelper.hpp>



class XALAN_PLATFORMSUPPORT_EXPORT XalanDOMStringPool
{
public:

	class StringKey
	{
	public:

		explicit
		StringKey();

		StringKey(
				const XalanDOMChar*		theString,
				unsigned int			theLength) :
			m_string(theString),
			m_length(theLength)
		{
		}

		~StringKey()
		{
		}

		bool
		operator<(const StringKey&	theRHS) const;

	private:

		const XalanDOMChar*		m_string;

		unsigned int			m_length;
	};

#if defined(XALAN_NO_NAMESPACES)
	typedef deque<XalanDOMString>								XalanDOMStringCollectionType;

	typedef map<
				StringKey,
				XalanDOMStringCollectionType::const_iterator,
				less<StringKey> >								IteratorMapType;
#else
	typedef std::deque<XalanDOMString>							XalanDOMStringCollectionType;

	typedef std::map<
				StringKey,
				XalanDOMStringCollectionType::const_iterator>	IteratorMapType;
#endif

	typedef XalanDOMStringCollectionType::size_type		size_type;

	explicit
	XalanDOMStringPool();

	virtual
	~XalanDOMStringPool();

	/**
	 * Clear the pool.
	 *
	 * @param thePair key-value pair
	 */
	virtual void
	clear();

	/**
	 * Get the number of strings in the pool
	 *
	 * @param thePair key-value pair
	 */
	virtual size_type
	size() const;

	/**
	 * Get a pooled string.  If the string is not pooled, it is added.
	 *
	 * @param theString The string to pool.
	 * @return a const reference to the pooled string.
	 */
	virtual const XalanDOMString&
	get(const XalanDOMString&	theString);

	/**
	 * Get a pooled string.  If the string is not pooled, it is added.
	 *
	 * @param theString The string to pool.
	 * @param theLength The length of the string.  If -1, the string is assumed to be null-terminated.
	 * @return a const reference to the pooled string.
	 */
	virtual const XalanDOMString&
	get(
			const XalanDOMChar*		theString,
			unsigned int			theLength = unsigned(-1));

private:

	// Not implemented, for now...
	XalanDOMStringPool(const XalanDOMStringPool&);

	XalanDOMStringPool&
	operator=(const XalanDOMStringPool&);

	bool
	operator==(const XalanDOMStringPool&) const;

	// Data members...
	XalanDOMStringCollectionType	m_strings;

	IteratorMapType					m_index;

	static const XalanDOMString		s_emptyString;
};



#endif	// !defined(XALANDOMSTRINGPOOL_HEADER_GUARD_1357924680)
