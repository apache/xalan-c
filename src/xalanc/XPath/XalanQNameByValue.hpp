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
	XalanQNameByValue();

	/**
	 * Copy constructor.
	 *
	 * @param theSource The source of the copy.
	 */
	XalanQNameByValue(const XalanQNameByValue&	theSource);

	/**
	 * Construct an instance from another XalanQName.
	 *
	 * @param theSource The source of the copy.
	 */
	explicit
	XalanQNameByValue(const XalanQName&	theSource);

	/**
	 * Construct a XalanQNameByValue, with the supplied namespace and local part.
	 *
	 * @param theNamespace namespace string
	 * @param theLocalPart local part string
	 */
	XalanQNameByValue(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	theLocalPart);

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


	// Data members...
	XalanDOMString	m_namespace;

	XalanDOMString	m_localpart;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALANQNAMEBYVALUE_HEADER_GUARD_1357924680
