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

#if !defined(XSLTINIT_INCLUDE_GUARD_1357924680)
#define XSLTINIT_INCLUDE_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XSLT/XSLTDefinitions.hpp>



#include <xalanc/PlatformSupport/PlatformSupportInit.hpp>



#include <xalanc/DOMSupport/DOMSupportInit.hpp>



#include <xalanc/XMLSupport/XMLSupportInit.hpp>



#include <xalanc/XalanSourceTree/XalanSourceTreeInit.hpp>



#include <xalanc/XPath/XPathInit.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XSLT_EXPORT XSLTInit
{
public:

	explicit
	XSLTInit(MemoryManagerType&      theManager);

	~XSLTInit();

    static XSLTInit*
    create(MemoryManagerType&      theManager);

    inline MemoryManagerType&
    getMemoryManager();


private:

	// Not implemented...
	XSLTInit(const XSLTInit&);

	XSLTInit&
	operator=(const XSLTInit&);

	
	static void
	initialize(MemoryManagerType&      theManager);

	static void
	terminate();

	const PlatformSupportInit	m_platformSupportInit;

	const DOMSupportInit		m_domSupportInit;

	const XMLSupportInit		m_xmlSupportInit;

	const XalanSourceTreeInit	m_xalanSourceTreeInit;

	const XPathInit				m_xpathInit;

	static unsigned long		s_initCounter;
};



XALAN_CPP_NAMESPACE_END



#endif	// !defined(XSLTINIT_INCLUDE_GUARD_1357924680)
