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
#if !defined(XALAN_RESULTNAMESPACESSTACK_HEADER_GUARD)
#define XALAN_RESULTNAMESPACESSTACK_HEADER_GUARD



// Base include file.  Must be first.
#include <xalanc/XSLT/XSLTDefinitions.hpp>



#include <xalanc/Include/XalanVector.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/XPath/XalanQName.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XSLT_EXPORT ResultNamespacesStack
{
public:


#if defined(XALAN_USE_DEQUE_FOR_VECTOR_BOOL)
	#if defined(XALAN_NO_STD_NAMESPACE)
	typedef deque<bool>			BoolVectorType;
	#else
	typedef std::deque<bool>	BoolVectorType;
	#endif
#else
	typedef XalanVector<bool>		BoolVectorType;
#endif

	typedef XalanQName::NamespaceVectorType		NamespaceVectorType;
	typedef XalanQName::NamespacesStackType		NamespacesStackType;

	typedef NamespacesStackType::size_type		size_type;


	explicit
	ResultNamespacesStack(MemoryManagerType& theManager);

	~ResultNamespacesStack();

	void
	addDeclaration(
			const XalanDOMString&	thePrefix,
	        const XalanDOMString&	theNamespaceURI)
	{
		addDeclaration(
			thePrefix,
			theNamespaceURI.c_str(),
			theNamespaceURI.length());
	}

	void
	addDeclaration(
			const XalanDOMString&	thePrefix,
	        const XalanDOMChar*		theNamespaceURI)
	{
		addDeclaration(
			thePrefix,
			theNamespaceURI,
			length(theNamespaceURI));
	}

	void
	addDeclaration(
			const XalanDOMString&		thePrefix,
	        const XalanDOMChar*			theNamespaceURI,
			XalanDOMString::size_type	theLength);

	void
	pushContext();

	void
	popContext();

	const XalanDOMString*
	getNamespaceForPrefix(const XalanDOMString&		thePrefix) const;

	const XalanDOMString*
	getPrefixForNamespace(const XalanDOMString&		theNamespaceURI) const;

	/**
	 * See if the prefix has been mapped to a namespace in the current
	 * context, without looking down the stack of namespaces.
	 */
	bool
	prefixIsPresentLocal(const XalanDOMString&	thePrefix);

	void
	clear();

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
	ResultNamespacesStack(const ResultNamespacesStack&);

	bool
	operator==(const ResultNamespacesStack&) const;

	ResultNamespacesStack&
	operator=(const ResultNamespacesStack&);

	enum { eDefaultCreateNewContextStackSize = 25 };

	/**
	 * A stack to keep track of the result tree namespaces.
	 */
	NamespacesStackType				m_resultNamespaces;

	NamespacesStackType::iterator	m_stackBegin;

	NamespacesStackType::iterator	m_stackPosition;

	BoolVectorType					m_createNewContextStack;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALAN_RESULTNAMESPACESSTACK_HEADER_GUARD
