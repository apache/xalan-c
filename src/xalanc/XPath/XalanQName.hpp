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
#if !defined(XALANQNAME_HEADER_GUARD_1357924680)
#define XALANQNAME_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



#include <xalanc/Include/XalanMap.hpp>
#include <xalanc/Include/XalanDeque.hpp>
#include <xalanc/Include/STLHelper.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/PrefixResolver.hpp>
#include <xalanc/PlatformSupport/XSLException.hpp>



#include <xalanc/XPath/NameSpace.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XalanElement;
class XPathEnvSupport;



/**
 * Class to represent a qualified name: "The name of an internal XSLT object, 
 * specifically a named template (see [7 Named Templates]), a mode (see [6.7 Modes]), 
 * an attribute set (see [8.1.4 Named Attribute Sets]), a key (see [14.2 Keys]), 
 * a locale (see [14.3 Number Formatting]), a variable or a parameter (see 
 * [12 Variables and Parameters]) is specified as a QName. If it has a prefix, 
 * then the prefix is expanded into a URI reference using the namespace declarations 
 * in effect on the attribute in which the name occurs. The expanded name 
 * consisting of the local part of the name and the possibly null URI reference 
 * is used as the name of the object. The default namespace is not used for 
 * unprefixed names."
 */

class XALAN_XPATH_EXPORT XalanQName
{
public:

    typedef	XalanDeque<NameSpace, ConstructWithMemoryManagerTraits<NameSpace> >
															NamespaceVectorType;
	typedef XalanDeque<NamespaceVectorType, ConstructWithMemoryManagerTraits<NamespaceVectorType> >
															NamespacesStackType;

	/**
	 * Construct an empty XalanQName.
	 *
	 */
	explicit
	XalanQName()
	{
	}

	virtual
	~XalanQName()
	{
	}

	/**
	 * Retrieve the local part of qualified name.
	 * 
	 * @return local part string
	 */
	virtual const XalanDOMString&
	getLocalPart() const = 0;

	/**
	 * Retrieve the namespace of qualified name.
	 * 
	 * @return namespace string
	 */
	virtual const XalanDOMString&
	getNamespace() const = 0;

	/**
	 * Determine if the qualified name is valid.
	 * 
	 * @return true if the instance is a valid QName, false if not.
	 */
	bool
	isValid() const
	{
		return isValidNCName(getLocalPart());
	}

	/**
	 * Whether the qualified name is empty.
	 * 
	 * @return true if namespace and local part are both empty
	 */
	bool
	isEmpty() const
	{
		return getNamespace().empty() && getLocalPart().empty();
	}

	/**
	 * Override equals and agree that we're equal if the passed object is a
	 * string and it matches the name of the arg.
	 * 
	 * @param theRHS namespace to compare
	 * @return true if namespace and local part are both empty
	 */
	bool
	equals(const XalanQName&	theRHS) const
	{
		// Note that we do not use our member variables here.  See
		// class QNameReference for details...
		return getLocalPart() == theRHS.getLocalPart() &&
			   getNamespace() == theRHS.getNamespace();
	}

	XalanDOMString::size_type
	hash() const
	{
		return getLocalPart().hash() % (getNamespace().hash() + 1);
	}

	class XALAN_XPATH_EXPORT PrefixResolverProxy : public PrefixResolver
	{
	public:

		/**
		 * Construct a PrefixResolver from a NamespacesStackType
		 * instance.
		 *
		 * @param theStack The stack to use for prefix resolution
		 * @param theURI The namespace URI of the resolver, if any.  Only a reference is kept, so this cannot be a temporary
		 * @return pointer to the string value if found, otherwise 0.
		 */
		PrefixResolverProxy(
				const NamespacesStackType&	theStack,
				const XalanDOMString&		theURI);

		virtual
		~PrefixResolverProxy();

		virtual const XalanDOMString*
		getNamespaceForPrefix(const XalanDOMString&		prefix) const;

		virtual const XalanDOMString&
		getURI() const;

	private:

		const NamespacesStackType&	m_stack;

		const XalanDOMString&		m_uri;
	};

	/**
	 * Get the namespace for a prefix by searching a vector of namespaces.
	 *
	 * @param namespaces vector of namespaces to search
	 * @param prefix     namespace prefix to find
	 * @return pointer to the string value if found, otherwise null.
	 */
	static const XalanDOMString*
	getNamespaceForPrefix(
			const NamespaceVectorType&	namespaces,
			const XalanDOMString&		prefix);

	/**
	 * Get the namespace for a prefix by searching a stack of namespace
	 * vectors.
	 *
	 * @param nsStack stack of namespace vectors to search
	 * @param prefix  namespace prefix to find
	 * @return pointer to the string value if found, otherwise null.
	 */
	static const XalanDOMString*
	getNamespaceForPrefix(
			const NamespacesStackType&	nsStack,
			const XalanDOMString&		prefix);

    static const XalanDOMString*
	getNamespaceForPrefix(
			const NamespacesStackType&	nsStack,
			const XalanDOMChar*	        prefix);

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
	 * Get the prefix for a namespace by searching a vector of namespaces.
	 *
	 * @param namespaces vector of namespaces to search
	 * @param uri        URI string for namespace to find
	 * @param reverse    true to search vector from last to first, default true
	 * @return pointer to the string value if found, otherwise null.
	 */
	static const XalanDOMString*
	getPrefixForNamespace(
			const NamespaceVectorType&	namespaces,
			const XalanDOMString&		uri);

    static const XalanDOMString*
    getNamespaceForPrefix(
			const NamespaceVectorType&	namespaces,
			const XalanDOMChar*	        prefix);

	/**
	 * Get the prefix for a namespace by searching a stack of namespace
	 * vectors.
	 *
	 * @param nsStack stack of namespace vectors to search
	 * @param uri     URI string for namespace to find
	 * @return pointer to the string value if found, otherwise null.
	 */
	static const XalanDOMString*
	getPrefixForNamespace(
			const NamespacesStackType&	nsStack,
			const XalanDOMString&		uri);

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

    static const XalanDOMString*
    getNamespaceForPrefix(
			NamespacesStackType::const_iterator		theBegin,
			NamespacesStackType::const_iterator		theEnd,
			const XalanDOMChar*	                    prefix);
	/**
	 * Determine if the string supplied satisfies the grammar for
	 * an XML NCName.
	 *
	 * @param theNCName The string to check
	 * @return bool true if the string is a valid NCName, false if not.
	 */
	static bool
	isValidNCName(const XalanDOMString&		theNCName);

	/**
	 * Determine if the string supplied satisfies the grammar for
	 * an XML NCName.
	 *
	 * @param theNCName The string to check
	 * @param theLength The length of the string
	 * @return bool true if the string is a valid NCName, false if not
	 */
	static bool
	isValidNCName(
			const XalanDOMChar*			theNCName,
			XalanDOMString::size_type	theLength = XalanDOMString::npos);

	/**
	 * Determine if the string supplied satisfies the grammar for
	 * an XML QName.  Note that this function does not determine
	 * if any supplied prefix is bound to a namespace URI
	 *
	 * @param theQName The string to check
	 * @return bool true if the string is a valid QName, false if not
	 */
	static bool
	isValidQName(const XalanDOMString&	theQName);

	/**
	 * Determine if the string supplied satisfies the grammar for
	 * an XML QName.  Note that this function does not determine
	 * if any supplied prefix is bound to a namespace URI
	 *
	 * @param theQName The string to check
	 * @param theLength The length of the string
	 * @return bool true if the string is a valid QName, false if not
	 */
	static bool
	isValidQName(
			const XalanDOMChar*			theQName,
			XalanDOMString::size_type	theLength = XalanDOMString::npos);

	class InvalidQNameException : public XSLException
	{
	public:

		/**
		 * Constructor
		 * 
		 * @param theQName The QName string that is not valid.
		 * @param theQNameLength The length of the string.
		 * @param theURI the URI of the related document, if known
		 * @param theLineNumber the line number of the related document.
		 * @param theColumnNumber the column number of the related document.
		 * @param theType type of exception, default is "XSLException"
		 */
		InvalidQNameException(
				const XalanDOMChar*			theQName,
				XalanDOMString::size_type	theQNameLength,
				const XalanDOMString&		theURI,
				int							theLineNumber,
				int							theColumnNumber,
                XalanDOMString&         	theResult);

		/**
		 * Constructor
		 * 
		 * @param theLocator The locator instance for error reporting.
		 * @param theQName The QName string that is not valid.
		 * @param theQNameLength The length of the string.
		 * @param theType type of exception, default is "XSLException"
		 */
		InvalidQNameException(
				const LocatorType&			theLocator,
				const XalanDOMChar*			theQName,
				XalanDOMString::size_type	theQNameLength,
                XalanDOMString&         	theResult);

		/**
		 * Constructor
		 * 
		 * @param theQName The QName string that is not valid.
		 * @param theQNameLength The length of the string.
		 * @param theType type of exception, default is "XSLException"
		 */
		InvalidQNameException(
				const XalanDOMChar*			theQName,
				XalanDOMString::size_type	theQNameLength,
                XalanDOMString&         	theResult);

		virtual
		~InvalidQNameException();

		virtual const XalanDOMChar*
		getType() const
		{
			return m_type;
		}

	private:

		static const XalanDOMChar	m_type[];

		static const XalanDOMString&
		format(
				const XalanDOMChar*			theQName,
				XalanDOMString::size_type	theQNameLength,
                XalanDOMString&         	theResult);
	};

protected:

	static const XalanDOMString		s_emptyString;
};


inline bool
operator==(
			const XalanQName&	theLHS,
			const XalanQName&	theRHS)
{
	return theLHS.equals(theRHS);
}



inline bool
operator!=(
			const XalanQName&	theLHS,
			const XalanQName&	theRHS)
{
	return !(theLHS == theRHS);
}



inline bool
operator<(
			const XalanQName&	theLHS,
			const XalanQName&	theRHS)
{
	if (theLHS.getNamespace() < theRHS.getNamespace())
	{
		return true;
	}
	else if (equals(theLHS.getNamespace(), theRHS.getNamespace()))
	{
		return theLHS.getLocalPart() < theRHS.getLocalPart();
	}
	else
	{
		return false;
	}
}

template<>
struct XalanMapKeyTraits<XalanQName>
{
	typedef XalanHashMemberReference<XalanQName>		Hasher;
	typedef XALAN_STD_QUALIFIER equal_to<XalanQName>	Comparator;
};

template<>
struct XalanMapKeyTraits<const XalanQName*>
{
	typedef XalanHashMemberPointer<XalanQName>		Hasher;
	typedef	pointer_equal<XalanQName>				Comparator;
};


XALAN_CPP_NAMESPACE_END



#endif	// XALANQNAME_HEADER_GUARD_1357924680
