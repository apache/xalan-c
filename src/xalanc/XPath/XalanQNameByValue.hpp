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
#if !defined(XALANQNAMEBYVALUE_HEADER_GUARD_1357924680)
#define XALANQNAMEBYVALUE_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



// Base class header file...
#include <xalanc/XPath/XalanQName.hpp>



XALAN_DECLARE_XERCES_CLASS(Locator)



XALAN_CPP_NAMESPACE_BEGIN



class DOMSupport;



class XALAN_XPATH_EXPORT XalanQNameByValue : public XalanQName
{
public:

	typedef XERCES_CPP_NAMESPACE_QUALIFIER Locator	LocatorType;

	/**
	 * Construct an empty XalanQNameByValue.
	 */
	explicit
	XalanQNameByValue(MemoryManagerType& theManager);

    MemoryManagerType&
    getMemoryManager()
    {
        return m_namespace.getMemoryManager();
    }

	/**
	 * Copy constructor.
	 *
	 * @param theSource The source of the copy.
	 */
	XalanQNameByValue(const XalanQNameByValue&	theSource,
                    MemoryManagerType& theManager);

	/**
	 * Construct an instance from another XalanQName.
	 *
	 * @param theSource The source of the copy.
	 */
	explicit
	XalanQNameByValue(const XalanQName&	theSource,
                    MemoryManagerType& theManager);

	/**
	 * Construct a XalanQNameByValue, with the supplied namespace and local part.
	 *
	 * @param theNamespace namespace string
	 * @param theLocalPart local part string
	 */
	XalanQNameByValue(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	theLocalPart,
            MemoryManagerType&      theManager);

	static XalanQNameByValue*
    create(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	theLocalPart,
            MemoryManagerType&      theManager);

	/**
	 * Construct a XalanQNameByValue from a string, resolving the prefix using the given
	 * namespace vector stack.
	 *
	 * @param qname       QName string
	 * @param namespaces  namespace vector stack to use
	 * @param locator     The Locator instance for error reporting, if any
	 * @param fUseDefault If true, then elements with no prefix will have the default namespace URI, if there is one.
	 */
	XalanQNameByValue(
			const XalanDOMString&		qname,
			const NamespacesStackType&	namespaces,
            MemoryManagerType&          theManager,
			const LocatorType*			locator = 0,
			bool						fUseDefault = false);

	/**
	 * Construct a XalanQNameByValue from a string, resolving the prefix using the given
	 * namespace vector stack.
	 *
	 * @param qname       QName string
	 * @param namespaces  namespace vector stack to use
	 * @param locator     The Locator instance for error reporting, if any
	 * @param fUseDefault If true, then elements with no prefix will have the default namespace URI, if there is one.
	 */
	XalanQNameByValue(
			const XalanDOMChar*			qname,
			const NamespacesStackType&	namespaces,
            MemoryManagerType&          theManager,
			const LocatorType*			locator = 0,
			bool						fUseDefault = false);

	/**
	 * Construct a XalanQNameByValue from a string, resolving the prefix using the given
	 * namespace context. The default namespace is not resolved.
	 *
	 * @param qname            QName string
	 * @param namespaceContext context object for namespace resolution
	 * @param envSupport       XPathEnvSupport class instance
	 * @param domSupport       DOMSupport class instance
	 * @param locator          The Locator instance for error reporting, if any
	 */
	XalanQNameByValue(
			const XalanDOMString&	qname,
			const XalanElement*		namespaceContext,
			const XPathEnvSupport&	envSupport,
			const DOMSupport& 		domSupport,
            MemoryManagerType&      theManager,
			const LocatorType*		locator = 0);

	/**
	 * Construct a XalanQNameByValue from a string, resolving the prefix using the given
	 * prefix resolver. The default namespace is not resolved.
	 *
	 * @param qname       QName string
	 * @param theResolver prefix resolver to use
	 * @param locator     The Locator instance for error reporting, if any
	 */
	XalanQNameByValue(
			const XalanDOMString&	qname,
            MemoryManagerType&      theManager,
			const PrefixResolver*	theResolver = 0,
			const LocatorType*		locator = 0);

	virtual
	~XalanQNameByValue();

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
		m_localpart = theLocalPart;
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
		m_namespace = theNamespace;
	}

	/**
	 * Set the local part and namespace URI of a XalanQNameByValue from
	 * a string, resolving the prefix using the given namespace vector
	 * stack.
	 *
	 * @param qname       QName string
	 * @param namespaces  namespace vector stack to use
	 * @param locator     The Locator instance for error reporting, if any
	 * @param fUseDefault If true, then elements with no prefix will have the default namespace URI, if there is one.
	 */
	void
	set(
			const XalanDOMString&		qname,
			const NamespacesStackType&	namespaces,
			const LocatorType*			locator = 0,
			bool						fUseDefault = false);

	/**
	 * Set the local part and namespace URI of a XalanQNameByValue from
	 * a string, resolving the prefix using the given namespace vector
	 * stack.
	 *
	 * @param qname       QName string
	 * @param namespaces  namespace vector stack to use
	 * @param locator     The Locator instance for error reporting, if any
	 * @param fUseDefault If true, then elements with no prefix will have the default namespace URI, if there is one.
	 */
	void
	set(
			const XalanDOMChar*			qname,
			const NamespacesStackType&	namespaces,
			const LocatorType*			locator = 0,
			bool						fUseDefault = false);

	/**
	 * Set the local part and namespace URI of a XalanQNameByValue from
	 * a string, resolving the prefix using the resolver provided.  The
	 * default namespace is not resolved.
	 *
	 * @param qname       QName string
	 * @param theResolver prefix resolver to use
	 * @param locator     The Locator instance for error reporting, if any
	 */
	void
	set(
			const XalanDOMString&	qname,
			const PrefixResolver*	theResolver = 0,
			const LocatorType*		locator = 0);

	/**
	 * Set the local part and namespace URI of a XalanQNameByValue from
	 * a string, resolving the prefix using the resolver provided.  The
	 * default namespace is not resolved.
	 *
	 * @param qname       QName string
	 * @param theResolver prefix resolver to use
	 * @param locator     The Locator instance for error reporting, if any
	 */
	void
	set(
			const XalanDOMChar*		qname,
			const PrefixResolver*	theResolver = 0,
			const LocatorType*		locator = 0);
	/**
	 * Clear the instance.
	 */
	void
	clear()
	{
		m_namespace.clear();
		m_localpart.clear();
	}

	XalanQNameByValue&
	operator=(const XalanQNameByValue&	theRHS)
	{
		m_namespace = theRHS.m_namespace;
		m_localpart = theRHS.m_localpart;

		return *this;
	}

	XalanQNameByValue&
	operator=(const XalanQName&		theRHS)
	{
		m_namespace = theRHS.getNamespace();
		m_localpart = theRHS.getLocalPart();

		return *this;
	}

private:
    // not implemented
    XalanQNameByValue(const XalanQNameByValue&	theSource);
	void
	initialize(
			const XalanDOMChar*			qname,
			XalanDOMString::size_type	len,
			const NamespacesStackType&	namespaces,
			const LocatorType*			locator,
			bool						fUseDefault);

	void
	resolvePrefix(
			const XalanDOMChar*			qname,
			XalanDOMString::size_type	theLength,
			const PrefixResolver*		theResolver,
			const LocatorType*			locator);

	void
	validate(
			const XalanDOMChar*			qname,
			XalanDOMString::size_type	theLength,
			const LocatorType*			locator);

	// Data members...
	XalanDOMString	m_namespace;

	XalanDOMString	m_localpart;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALANQNAMEBYVALUE_HEADER_GUARD_1357924680
