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
#if !defined(XALANQNAMEBYREFERENCE_HEADER_GUARD_1357924680)
#define XALANQNAMEBYREFERENCE_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



// Base class header file...
#include <xalanc/XPath/XalanQName.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XPATH_EXPORT XalanQNameByReference : public XalanQName
{
public:

	/**
	 * Construct an empty XalanQNameByReference.
	 */
	explicit
	XalanQNameByReference();

	/**
	 * Construct a XalanQNameByReference, with the supplied namespace and local part.
	 * The instance keeps only a _reference_ to the string, to avoid making a
	 * copy.
	 *
	 * @param theNamespace namespace string
	 * @param theLocalPart local part string
	 */
	XalanQNameByReference(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	theLocalPart);

	/**
	 * Construct a XalanQNameByReference, with the supplied local part.
	 * The instance keeps only a _reference_ to the string, to avoid making a
	 * copy.
	 *
	 * @param theLocalPart local part string
	 */
	XalanQNameByReference(const XalanDOMString&		theLocalPart);

	/**
	 * Construct a XalanQNameByReference, from the supplied XalanQName.  The instance
	 * keeps only a _reference_ to the string, to avoid making a copy.
	 *
	 * @param theQName The source QName
	 */
	XalanQNameByReference(const XalanQName&		theQName);

	virtual
	~XalanQNameByReference();

	/**
	 * Retrieve the local part of qualified name.
	 * 
	 * @return local part string
	 */
	virtual const XalanDOMString&
	getLocalPart() const;

	/**
	 * Set the local part of qualified name.
	 * 
	 * @param theLocalPart local part string
	 */
	void
	setLocalPart(const XalanDOMString&	theLocalPart)
	{
		m_localpart = &theLocalPart;
	}

	/**
	 * Retrieve the namespace of qualified name.
	 * 
	 * @return namespace string
	 */
	virtual const XalanDOMString&
	getNamespace() const;

	/**
	 * Set the Namespace URI of qualified name.
	 * 
	 * @param theLocalPart local part string
	 */
	void
	setNamespace(const XalanDOMString&	theNamespace)
	{
		m_namespace = &theNamespace;
	}

	/**
	 * Clear the instance.
	 */
	void
	clear()
	{
		m_namespace = &s_emptyString;
		m_localpart = &s_emptyString;
	}

private:

	// OK, we said reference, but using pointers
	// allows for copy and assignment semantics.
	const XalanDOMString*	m_namespace;

	const XalanDOMString*	m_localpart;
};



template<>
struct XalanMapKeyTraits<XalanQNameByReference> : public XalanMapKeyTraits<XalanQName>
{
};



XALAN_CPP_NAMESPACE_END



#endif	// XALANQNAMEBYREFERENCE_HEADER_GUARD_1357924680
