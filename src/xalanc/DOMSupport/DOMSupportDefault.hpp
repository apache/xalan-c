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
#if !defined(DOMSUPPORTDEFAULT_HEADER_GUARD_1357924680)
#define DOMSUPPORTDEFAULT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/DOMSupport/DOMSupportDefinitions.hpp>



#include <xalanc/PlatformSupport/XalanDOMStringPool.hpp>



#include <xalanc/DOMSupport/DOMSupport.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_DOMSUPPORT_EXPORT DOMSupportDefault : public DOMSupport
{
public:

	DOMSupportDefault(MemoryManagerType& theManager XALAN_DEFAULT_CONSTRACTOR_MEMORY_MGR);

	virtual
	~DOMSupportDefault();

	// These interfaces are inherited from Resettable...

	virtual void
	reset();


	// These interfaces are inherited from DOMSupport...

	virtual const XalanDOMString&
	getUnparsedEntityURI(
			const XalanDOMString&	theName,
			const XalanDocument&	theDocument) const;

	virtual bool
	isNodeAfter(
			const XalanNode&	node1,
			const XalanNode&	node2) const;

private:

	mutable XalanDOMStringPool	m_pool;
};



XALAN_CPP_NAMESPACE_END



#endif	// DOMSUPPORTDEFAULT_HEADER_GUARD_1357924680
