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
#if !defined(XNODESET_HEADER_GUARD_1357924680)
#define XNODESET_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



// Base class header file.
#include <xalanc/XPath/XNodeSetBase.hpp>



#include <xalanc/XPath/XPathExecutionContext.hpp>



XALAN_CPP_NAMESPACE_BEGIN



/**
 * Class to hold XPath return types.
 */
class XALAN_XPATH_EXPORT XNodeSet : public XNodeSetBase
{
public:

	typedef XPathExecutionContext::BorrowReturnMutableNodeRefList	BorrowReturnMutableNodeRefList;

	/**
	 * Create an XNodeSet from a node list.
	 *
	 * @param value Pointer to source node list.  The XNodeSet will adopt the pointer.
	 */
	XNodeSet(BorrowReturnMutableNodeRefList&	value,
             MemoryManagerType& theManager);
	/**
	 * Create an XNodeSet from another.
	 *
	 * @param source    object to copy
	 * @param deepClone true to copy all children on nodeset nodes
	 */
	XNodeSet(
			const XNodeSet&		source,
            MemoryManagerType&  theManager,
			bool				deepClone = false);

	virtual
	~XNodeSet();

	// These methods are inherited from XNodeSetBase...

	virtual const NodeRefListBase&
	nodeset() const;

	virtual XalanNode*
	item(size_type	index) const;

	virtual size_type
	getLength() const;

	/**
	 * Release the node set held by the instance.
	 */
	void
	release();

	/**
	 * Change the value of an XNodeSet
	 *
	 * @param theValue The new value.
	 */
	void
	set(BorrowReturnMutableNodeRefList&		value);

private:

	// Not implemented...
	XNodeSet&
	operator=(const XNodeSet&);

	// Data members...
	BorrowReturnMutableNodeRefList				m_value;
};



XALAN_CPP_NAMESPACE_END



#endif	// XNODESET_HEADER_GUARD_1357924680
