/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights 
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
#if !defined(XALAN_XALANNAMESPACESSTACK_HEADER_GUARD)
#define XALAN_XALANNAMESPACESSTACK_HEADER_GUARD



// Base include file.  Must be first.
#include <DOMSupport/DOMSupportDefinitions.hpp>



#include <deque>
#include <vector>



#include <XalanDOM/XalanDOMString.hpp>



#include <PlatformSupport/XalanNamespace.hpp>



class XALAN_DOMSUPPORT_EXPORT XalanNamespacesStack
{
public:

	class XALAN_DOMSUPPORT_EXPORT XalanNamespacesStackEntry
	{
	public:

#if defined(XALAN_NO_NAMESPACES)
		typedef	deque<XalanNamespace>		NamespaceCollectionType;
#else
		typedef	std::deque<XalanNamespace>	NamespaceCollectionType;
#endif

		typedef NamespaceCollectionType::iterator					iterator;
		typedef NamespaceCollectionType::reverse_iterator			reverse_iterator;
		typedef NamespaceCollectionType::const_iterator				const_iterator;
		typedef NamespaceCollectionType::const_reverse_iterator		const_reverse_iterator;

		XalanNamespacesStackEntry();

		XalanNamespacesStackEntry(const XalanNamespacesStackEntry&	theSource);

		~XalanNamespacesStackEntry();

		XalanNamespacesStackEntry&
		operator=(const XalanNamespacesStackEntry&	theRHS);

		void
		addDeclaration(
				const XalanDOMString&		thePrefix,
				const XalanDOMChar*			theNamespaceURI,
				XalanDOMString::size_type	theLength);

		/**
		 * Get the namespace for a prefix.
		 *
		 * @param thePrefix The prefix to find
		 * @return pointer to the string value if found, otherwise 0.
		 */
		const XalanDOMString*
		getNamespaceForPrefix(const XalanDOMString&		thePrefix) const;

		/**
		 * Get the prefix for a namespace.
		 *
		 * @param theURI The namespace URI to find
		 * @return pointer to the string value if found, otherwise 0.
		 */
		const XalanDOMString*
		getPrefixForNamespace(const XalanDOMString&		theURI) const;

		bool
		isPrefixPresent(const XalanDOMString&	thePrefix) const
		{
			return getNamespaceForPrefix(thePrefix) == 0 ? false : true;
		}

		iterator
		begin()
		{
			return m_namespaces.begin();
		}

		const_iterator
		begin() const
		{
			return m_namespaces.begin();
		}

		iterator
		end()
		{
			return m_position;
		}

		const_iterator
		end() const
		{
			return const_iterator(m_position);
		}

		reverse_iterator
		rbegin()
		{
			return reverse_iterator(end());
		}

		const_reverse_iterator
		rbegin() const
		{
			return const_reverse_iterator(end());
		}

		reverse_iterator
		rend()
		{
			return reverse_iterator(begin());
		}

		const_reverse_iterator
		rend() const
		{
			return const_reverse_iterator(begin());
		}

		void
		clear();

		void
		reset()
		{
			m_position = m_namespaces.begin();
		}

		void
		swap(XalanNamespacesStackEntry&		theOther);

	private:

		NamespaceCollectionType		m_namespaces;

		iterator					m_position;
	};

#if defined(XALAN_NO_NAMESPACES)
	typedef	deque<XalanNamespacesStackEntry>	NamespacesStackType;
	typedef vector<bool>						BoolVectorType;
#else
	typedef	std::deque<XalanNamespacesStackEntry>	NamespacesStackType;
	typedef std::vector<bool>						BoolVectorType;
#endif

	typedef NamespacesStackType::iterator					iterator;
	typedef NamespacesStackType::reverse_iterator			reverse_iterator;
	typedef NamespacesStackType::const_iterator				const_iterator;
	typedef NamespacesStackType::const_reverse_iterator		const_reverse_iterator;

	typedef NamespacesStackType::size_type		size_type;


	explicit
	XalanNamespacesStack();

	~XalanNamespacesStack();

	void
	addDeclaration(
			const XalanDOMString&	thePrefix,
	        const XalanDOMString&	theURI)
	{
		addDeclaration(
			thePrefix,
			theURI.c_str(),
			theURI.length());
	}

	void
	addDeclaration(
			const XalanDOMString&	thePrefix,
	        const XalanDOMChar*		theURI)
	{
		addDeclaration(
			thePrefix,
			theURI,
			length(theURI));
	}

	void
	addDeclaration(
			const XalanDOMString&		thePrefix,
	        const XalanDOMChar*			theURI,
			XalanDOMString::size_type	theLength);

	void
	pushContext();

	void
	popContext();

	const XalanDOMString*
	getNamespaceForPrefix(const XalanDOMString&		thePrefix) const;

	const XalanDOMString*
	getPrefixForNamespace(const XalanDOMString&		theURI) const;

	/**
	 * See if the prefix has been mapped to a namespace in the current
	 * context, without looking down the stack of namespaces.
	 */
	bool
	prefixIsPresentLocal(const XalanDOMString&	thePrefix);

	void
	clear();

	iterator
	begin()
	{
		return m_stackBegin + 1;
	}

	const_iterator
	begin() const
	{
		return const_iterator(m_stackBegin + 1);
	}

	iterator
	end()
	{
		return m_stackPosition + 1;
	}

	const_iterator
	end() const
	{
		return const_iterator(m_stackPosition + 1);
	}

	reverse_iterator
	rbegin()
	{
		return reverse_iterator(end());
	}

	const_reverse_iterator
	rbegin() const
	{
		return const_reverse_iterator(end());
	}

	reverse_iterator
	rend()
	{
		return reverse_iterator(begin());
	}

	const_reverse_iterator
	rend() const
	{
		return const_reverse_iterator(begin());
	}

	size_type
	size() const
	{
		return m_resultNamespaces.size() - 1;
	}

	bool
	empty() const
	{
		return NamespacesStackType::const_iterator(m_stackPosition) == m_resultNamespaces.begin() ? true : false;
	}

private:

	/**
	 * Get the namespace for a prefix by searching a range of iterators.
	 * The search is done in reverse, from the end of the range to the
	 * beginning.
	 *
	 * @param theBegin The beginning iterator for the range
	 * @param theBegin The ending iterator for the range
	 * @param prefix  namespace prefix to find
	 * @return pointer to the string value if found, otherwise null.
	 */
	static const XalanDOMString*
	getNamespaceForPrefix(
			NamespacesStackType::const_iterator		theBegin,
			NamespacesStackType::const_iterator		theEnd,
			const XalanDOMString&					prefix);

	/**
	 * Get the prefix for a namespace by searching a range of iterators.
	 * The search is done in reverse, from the end of the range to the
	 * beginning.
	 *
	 * @param theBegin The beginning iterator for the range to search
	 * @param theBegin The ending iterator for the range to search
	 * @param uri     URI string for namespace to find
	 * @return pointer to the string value if found, otherwise null.
	 */
	static const XalanDOMString*
	getPrefixForNamespace(
			NamespacesStackType::const_iterator		theBegin,
			NamespacesStackType::const_iterator		theEnd,
			const XalanDOMString&					uri);

	// not implemented
	XalanNamespacesStack(const XalanNamespacesStack&);

	bool
	operator==(const XalanNamespacesStack&) const;

	XalanNamespacesStack&
	operator=(const XalanNamespacesStack&);

	enum { eDefaultCreateNewContextStackSize = 25 };

	/**
	 * A stack to keep track of the result tree namespaces.
	 */
	NamespacesStackType				m_resultNamespaces;

	NamespacesStackType::iterator	m_stackBegin;

	NamespacesStackType::iterator	m_stackPosition;

	BoolVectorType					m_createNewContextStack;
};



#endif	// XALAN_XALANNAMESPACESSTACK_HEADER_GUARD
