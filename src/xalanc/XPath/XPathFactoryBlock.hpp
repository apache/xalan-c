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
#if !defined(XPATHFACTORYBLOCK_HEADER_GUARD_1357924680)
#define XPATHFACTORYBLOCK_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



// Base class header file...
#include <xalanc/XPath/XPathFactory.hpp>



#include <xalanc/XPath/XPathAllocator.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XPATH_EXPORT XPathFactoryBlock : public XPathFactory
{

public:

	enum { eDefaultBlockSize = 20 };

	explicit
	XPathFactoryBlock(MemoryManagerType&      theManager, unsigned int	theBlockSize = eDefaultBlockSize);

	virtual
	~XPathFactoryBlock();


	// Inherited from XPathFactory...
	virtual void
	reset();

	// Inherited from XPathFactory...

	virtual XPath*
	create();

protected:

	// Inherited from XPathFactory...

	virtual bool
	doReturnObject(
			const XPath*	theXPath,
			bool			fInReset = false);

private:

	XPathAllocator	m_allocator;
};



XALAN_CPP_NAMESPACE_END



#endif	// XPATHFACTORYBLOCK_HEADER_GUARD_1357924680
