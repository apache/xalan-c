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
#if !defined(ELEMENTPREFIXRESOLVERPROXY_HEADER_GUARD_1357924680)
#define ELEMENTPREFIXRESOLVERPROXY_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



// Base class header file...
#include <xalanc/PlatformSupport/PrefixResolver.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class DOMSupport;
class XPathEnvSupport;
class XalanElement;



class XALAN_XPATH_EXPORT ElementPrefixResolverProxy : public PrefixResolver
{
public:

	/**
	 * Construct an instance of an element prefix resolver proxy.
	 * 
	 * @param namespaceContext context object for namespace resolution
	 * @param envSupport XPath environment support class instance
	 * @param domSupport DOMSupport class instance
	 *
	 * @deprecated
	 */
	ElementPrefixResolverProxy(
			const XalanElement*		namespaceContext,
			const XPathEnvSupport&	envSupport,
			const DOMSupport& 		domSupport,
            MemoryManagerType&      theManager XALAN_DEFAULT_MEMMGR);

	/**
	 * Construct an instance of an element prefix resolver proxy.
	 * 
	 * @param namespaceContext context object for namespace resolution
	 * @param envSupport XPath environment support class instance
	 *
	 */
	ElementPrefixResolverProxy(
			const XalanElement*		namespaceContext,
            MemoryManagerType&      theManager XALAN_DEFAULT_MEMMGR,
			const XPathEnvSupport*	envSupport = 0);

	virtual
	~ElementPrefixResolverProxy();

	// These methods are inherited from PrefixResolver ...

	virtual const XalanDOMString*
	getNamespaceForPrefix(const XalanDOMString&		prefix) const;

	virtual const XalanDOMString&
	getURI() const;

private:
    //notimplemented
    ElementPrefixResolverProxy(const ElementPrefixResolverProxy&);

	const XalanElement* const		m_namespaceContext;

	const XPathEnvSupport* const	m_envSupport;

	mutable XalanDOMString			m_uri;
};



XALAN_CPP_NAMESPACE_END



#endif	// ELEMENTPREFIXRESOLVERPROXY_HEADER_GUARD_1357924680
