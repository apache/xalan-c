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
#if !defined(XALAN_XALANNAMESPACESSTACK_HEADER_GUARD)
#define XALAN_XALANNAMESPACESSTACK_HEADER_GUARD



// Base include file.  Must be first.
#include <xalanc/DOMSupport/DOMSupportDefinitions.hpp>



#include <xalanc/Include/XalanVector.hpp>
#include <xalanc/Include/XalanDeque.hpp>



#include <xalanc/PlatformSupport/PrefixResolver.hpp>
#include <xalanc/PlatformSupport/XalanNamespace.hpp>



XALAN_CPP_NAMESPACE_BEGIN


class XalanDOMString;



class XALAN_DOMSUPPORT_EXPORT XalanNamespacesStack
{
public:

	class XALAN_DOMSUPPORT_EXPORT PrefixResolverProxy : public PrefixResolver
	{
	public:

		/**
		 * Construct a PrefixResolver from a XalanNamespaceStack
		 * instance.
		 *
		 * @param theStack The stack to use for prefix resolution
		 * @param theURI The namespace URI of the resolver, if any.  Only a reference is kept, so this cannot be a temporary
		 * @return pointer to the string value if found, otherwise 0.
		 */
		PrefixResolverProxy(
				const XalanNamespacesStack&		theStack,
				const XalanDOMString&			theURI);

		virtual
		~PrefixResolverProxy();

		virtual const XalanDOMString*
		getNamespaceForPrefix(const XalanDOMString&		prefix) const;

		virtual const XalanDOMString&
		getURI() const;

	private:

		const XalanNamespacesStack&		m_stack;

		const XalanDOMString&			m_uri;
	};

	class XALAN_DOMSUPPORT_EXPORT XalanNamespacesStackEntry
	{
	public:

		typedef XalanNamespace	value_type;

        typedef	XalanDeque<value_type>	NamespaceCollectionType;

		typedef const XalanDOMString& (value_type::*MemberFunctionType)() const;

		typedef NamespaceCollectionType::iterator					iterator;
		typedef NamespaceCollectionType::reverse_iterator			reverse_iterator;
		typedef NamespaceCollectionType::const_iterator				const_iterator;
		typedef NamespaceCollectionType::const_reverse_iterator		const_reverse_iterator;

		XalanNamespacesStackEntry(MemoryManagerType& theManager);

		XalanNamespacesStackEntry(const XalanNamespacesStackEntry&	theSource,
                                    MemoryManagerType&              theManager);

		~XalanNamespacesStackEntry();

		XalanNamespacesStackEntry&
		set(const XalanNamespacesStackEntry&	theRHS,
                  MemoryManagerType&                theManager);

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
		getNamespaceForPrefix(const XalanDOMString&		thePrefix) const
		{
			return findEntry(thePrefix, &XalanNamespace::getPrefix, &XalanNamespace::getURI);
		}

		/**
		 * Get the prefix for a namespace.
		 *
		 * @param theURI The namespace URI to find
		 * @return pointer to the string value if found, otherwise 0.
		 */
		const XalanDOMString*
		getPrefixForNamespace(const XalanDOMString&		theURI) const
		{
			return findEntry(theURI, &XalanNamespace::getURI, &XalanNamespace::getPrefix);
		}

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
        //Not implemented
        XalanNamespacesStackEntry();
		XalanNamespacesStackEntry(const XalanNamespacesStackEntry&	theSource);

		const XalanDOMString*
		findEntry(
			const XalanDOMString&	theKey,
			MemberFunctionType		theKeyFunction,
			MemberFunctionType		theValueFunction) const;

		NamespaceCollectionType		m_namespaces;

		iterator					m_position;
	};


	typedef XalanNamespacesStackEntry	value_type;

    typedef	XalanDeque<value_type, ConstructWithMemoryManagerTraits<value_type> >		NamespacesStackType;
	typedef XalanVector<bool>			BoolVectorType;

	typedef NamespacesStackType::iterator					iterator;
	typedef NamespacesStackType::reverse_iterator			reverse_iterator;
	typedef NamespacesStackType::const_iterator				const_iterator;
	typedef NamespacesStackType::const_reverse_iterator		const_reverse_iterator;

	typedef NamespacesStackType::size_type	size_type;

	typedef const XalanDOMString* (value_type::*MemberFunctionType)(const XalanDOMString&) const;


	explicit
	XalanNamespacesStack(MemoryManagerType& theManager);

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
	getPrefixForNamespace(const XalanDOMString&		theURI) const
	{
		return findEntry(theURI, &value_type::getPrefixForNamespace);
	}

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

	// not implemented
	XalanNamespacesStack(const XalanNamespacesStack&);
    XalanNamespacesStack();
	XalanNamespacesStackEntry&
	operator=(const XalanNamespacesStackEntry&	theRHS);

	bool
	operator==(const XalanNamespacesStack&) const;

	XalanNamespacesStack&
	operator=(const XalanNamespacesStack&);

	enum { eDefaultCreateNewContextStackSize = 25 };

	const XalanDOMString*
	findEntry(
			const XalanDOMString&	theKey,
			MemberFunctionType		theFunction) const;

	/**
	 * A stack to keep track of the result tree namespaces.
	 */
	NamespacesStackType				m_resultNamespaces;

	NamespacesStackType::iterator	m_stackBegin;

	NamespacesStackType::iterator	m_stackPosition;

	BoolVectorType					m_createNewContextStack;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALAN_XALANNAMESPACESSTACK_HEADER_GUARD
