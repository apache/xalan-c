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
#if !defined(XPATHCONSTRUCTIONCONTEXTDEFAULT_HEADER_GUARD_1357924680)
#define XPATHCONSTRUCTIONCONTEXTDEFAULT_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



#include <xalanc/PlatformSupport/XalanDOMStringCache.hpp>
#include <xalanc/PlatformSupport/XalanDOMStringPool.hpp>



/**
 * @author <a href="mailto:david_n_bertoni@lotus.com">David N. Bertoni</a>
 */
// Base class header file...
#include <xalanc/XPath/XPathConstructionContext.hpp>



XALAN_CPP_NAMESPACE_BEGIN



/**
 *
 * An default implementation of an abtract class which provides support for
 * constructing the internal representation  of an XPath.
 *
 */
class XALAN_XPATH_EXPORT XPathConstructionContextDefault : public XPathConstructionContext
{
public:

	/*
	 * Construct an instance.
	 */
	XPathConstructionContextDefault(MemoryManagerType& theManager XALAN_DEFAULT_MEMMGR);

    static XPathConstructionContextDefault*
    create(MemoryManagerType& theManager);

	virtual
	~XPathConstructionContextDefault();

	virtual void
	reset();

	virtual const XalanDOMString&
	getPooledString(const XalanDOMString&	theString);

	virtual const XalanDOMString&
	getPooledString(
			const XalanDOMChar*			theString,
			XalanDOMString::size_type	theLength = XalanDOMString::npos);

	virtual XalanDOMString&
	getCachedString();

	virtual bool
	releaseCachedString(XalanDOMString&		theString);

	virtual void
	error(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode,
			const LocatorType* 		locator) const;

	virtual void
	warn(
			const XalanDOMString&	msg,
			const XalanNode* 		sourceNode,
			const LocatorType* 		locator) const;

private:

	XalanDOMStringPool		m_stringPool;

	XalanDOMStringCache		m_stringCache;
};



XALAN_CPP_NAMESPACE_END



#endif	// XPATHCONSTRUCTIONCONTEXTDEFAULT_HEADER_GUARD_1357924680
