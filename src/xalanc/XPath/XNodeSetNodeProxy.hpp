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
#if !defined(XNODESETNODEPROXY_HEADER_GUARD_1357924680)
#define XNODESETNODEPROXY_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



// Base class header file.
#include <xalanc/XPath/XNodeSetBase.hpp>



#include <xalanc/XPath/NodeRefListBase.hpp>



XALAN_CPP_NAMESPACE_BEGIN



/**
 * Class to hold XPath return types.
 */
class XALAN_XPATH_EXPORT XNodeSetNodeProxy : public XNodeSetBase
{
public:

	/**
	 * Create an XNodeSetNodeProxy
	 */
	XNodeSetNodeProxy(MemoryManagerType& theManager, XalanNode*	theNode);

	/**
	 * Create an XNodeSetNodeProxy from another.
	 *
	 * @param source    object to copy
	 */
	XNodeSetNodeProxy(const XNodeSetNodeProxy&	source, MemoryManagerType& theManager);

	virtual
	~XNodeSetNodeProxy();

	// These methods are inherited from XObject ...

	virtual const NodeRefListBase&
	nodeset() const;

	virtual XalanNode*
	item(size_type	index) const;

	virtual size_type
	getLength() const;

	// A proxy class to implement NodeRefListBase...
	class Proxy : public NodeRefListBase
	{
	public:

		Proxy(XalanNode*	theNode);

		virtual
		~Proxy();

		virtual XalanNode*
		item(size_type	index) const;

		virtual size_type
		getLength() const;

		virtual size_type
		indexOf(const XalanNode*	theNode) const;

		XalanNode*	m_node;
	};

protected:

	virtual eObjectType
	getRealType() const;

private:

	// Not implemented...
	XNodeSetNodeProxy&
	operator=(const XNodeSetNodeProxy&);
    XNodeSetNodeProxy(const XNodeSetNodeProxy&	source);


	// Data members...
	const Proxy		m_proxy;
};



XALAN_CPP_NAMESPACE_END



#endif	// XNODESETNODEPROXY_HEADER_GUARD_1357924680
