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
#if !defined(XNODESETBASE_HEADER_GUARD_1357924680)
#define XNODESETBASE_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



// Base class header file.
#include <xalanc/XPath/XObject.hpp>



#include <xalanc/XPath/NodeRefListBase.hpp>
#include <xalanc/XPath/XNodeSetResultTreeFragProxy.hpp>



XALAN_CPP_NAMESPACE_BEGIN



/**
 * Class to hold XPath return types.
 */
class XALAN_XPATH_EXPORT XNodeSetBase : public XObject
{
public:

    typedef NodeRefListBase::size_type	size_type;

	virtual
	~XNodeSetBase();

    MemoryManagerType&
    getMemoryManager()const
    {
        return m_cachedStringValue.getMemoryManager();
    }

	// These methods are inherited from XObject ...


	virtual const XalanDOMString&
	getTypeString() const;

	virtual double
	num() const;

	virtual bool
	boolean() const;

	virtual const XalanDOMString&
	str() const;

	virtual void
	str(
			FormatterListener&	formatterListener,
			MemberFunctionPtr	function) const;

	virtual void
	str(XalanDOMString&	theBuffer) const;

	virtual double
	stringLength() const;

	virtual const XalanDocumentFragment&
	rtree() const;

	virtual const NodeRefListBase&
	nodeset() const = 0;

	virtual void
	ProcessXObjectTypeCallback(XObjectTypeCallback&		theCallbackObject);

	virtual void
	ProcessXObjectTypeCallback(XObjectTypeCallback&		theCallbackObject) const;

	virtual XalanNode*
	item(size_type	index) const = 0;

	virtual size_type
	getLength() const = 0;

protected:

	/**
	 * Create an XNodeSetBase
	 */
	XNodeSetBase(MemoryManagerType& theManager);

	/**
	 * Create an XNodeSetBase from another.
	 *
	 * @param source    object to copy
	 */
	XNodeSetBase(const XNodeSetBase&	source, MemoryManagerType& theManager);

	void
	clearCachedValues();

private:

	// Not implemented...
    XNodeSetBase();
    XNodeSetBase(const XNodeSetBase&);
	XNodeSetBase&
	operator=(const XNodeSetBase&);

	// Data members...
	XNodeSetResultTreeFragProxy		m_proxy;

	mutable XalanDOMString			m_cachedStringValue;

	mutable double					m_cachedNumberValue;
};



XALAN_CPP_NAMESPACE_END



#endif	// XNODESETBASE_HEADER_GUARD_1357924680
