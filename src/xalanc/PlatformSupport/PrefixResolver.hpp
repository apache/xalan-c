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
#if !defined(PREFIXRESOLVER_HEADER_GUARD_1357924680)
#define PREFIXRESOLVER_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>


XALAN_CPP_NAMESPACE_BEGIN



class XalanDOMString;



/**
 * This class defines an interface for classes that resolve namespace prefixes
 * to their URIs 
 */
class XALAN_PLATFORMSUPPORT_EXPORT PrefixResolver
{
public:


	PrefixResolver();

	PrefixResolver(const PrefixResolver&);

	virtual
	~PrefixResolver();

	/**
	 * Retrieve a namespace corresponding to a prefix.  This assumes that 
	 * the PrevixResolver holds its own namespace context, or is a namespace
	 * context itself.
	 *
	 * @param prefix Prefix to resolve
	 * @return namespace that prefix resolves to, or null if prefix is not found
	 */
	virtual const XalanDOMString*
	getNamespaceForPrefix(const XalanDOMString&		prefix) const = 0;

	/**
	 * Retrieve the base URI for the resolver.
	 * 
	 * @return URI string
	 */
	virtual const XalanDOMString&
	getURI() const = 0;
};



XALAN_CPP_NAMESPACE_END



#endif	// PREFIXRESOLVER_HEADER_GUARD_1357924680
