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
#if !defined(XOBJECTRESULTTREEFRAGPROXY_HEADER_GUARD_1357924680)
#define XOBJECTRESULTTREEFRAGPROXY_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>

#include <xalanc/Include/XalanMemoryManagement.hpp>


#include <xalanc/XPath/XObjectResultTreeFragProxyBase.hpp>
#include <xalanc/XPath/XObjectResultTreeFragProxyText.hpp>




XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XPATH_EXPORT XObjectResultTreeFragProxy : public XObjectResultTreeFragProxyBase
{
public:

	XObjectResultTreeFragProxy(const XObject&	value,
                                MemoryManagerType& theManager);

	virtual
	~XObjectResultTreeFragProxy();

    MemoryManagerType&
    getMemoryManager()
    {
        return m_proxy.getMemoryManager();
    }
	// These interfaces are inherited from XalanDocumentFragment...

	virtual XalanNode*
	getFirstChild() const;

	virtual XalanNode*
	getLastChild() const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual XalanNode*
#else
	virtual XObjectResultTreeFragProxy*
#endif
	cloneNode(bool deep) const;

	virtual bool
	hasChildNodes() const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual XalanDocumentFragment*
#else
	virtual XObjectResultTreeFragProxy*
#endif
	clone(bool	deep) const;

private:

	// Not implemented...
	XObjectResultTreeFragProxy(const XObjectResultTreeFragProxy&	theSource);

	XObjectResultTreeFragProxy&
	operator=(const XObjectResultTreeFragProxy&	theRHS);

	bool
	operator==(const XObjectResultTreeFragProxy&	theRHS);


	// Data members...
	mutable XObjectResultTreeFragProxyText	m_proxy;
};



XALAN_CPP_NAMESPACE_END



#endif	// XOBJECTRESULTTREEFRAGPROXY_HEADER_GUARD_1357924680
