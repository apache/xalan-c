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
			const DOMSupport& 		domSupport);

	/**
	 * Construct an instance of an element prefix resolver proxy.
	 * 
	 * @param namespaceContext context object for namespace resolution
	 * @param envSupport XPath environment support class instance
	 *
	 */
	ElementPrefixResolverProxy(
			const XalanElement*		namespaceContext,
			const XPathEnvSupport*	envSupport = 0);

	virtual
	~ElementPrefixResolverProxy();

	// These methods are inherited from PrefixResolver ...

	virtual const XalanDOMString*
	getNamespaceForPrefix(const XalanDOMString&		prefix) const;

	virtual const XalanDOMString&
	getURI() const;

private:

	const XalanElement* const		m_namespaceContext;

	const XPathEnvSupport* const	m_envSupport;

	mutable XalanDOMString			m_uri;
};



XALAN_CPP_NAMESPACE_END



#endif	// ELEMENTPREFIXRESOLVERPROXY_HEADER_GUARD_1357924680
