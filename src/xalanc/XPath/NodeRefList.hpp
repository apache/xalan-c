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
#if !defined(NODEREFLIST_HEADER_GUARD_1357924680)
#define NODEREFLIST_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



#include <vector>



#include <xalanc/XPath/NodeRefListBase.hpp>



XALAN_CPP_NAMESPACE_BEGIN



/**
 * Local implementation of NodeRefList.  This class is for internal use only.
 */
class XALAN_XPATH_EXPORT NodeRefList : public NodeRefListBase
{
public:

	explicit
	NodeRefList();

	/**
	 * Construct a node list from another
	 *
	 * @param theSource source node list
	 */
	NodeRefList(const NodeRefList&	theSource);

	/**
	 * Construct a node list from another
	 *
	 * @param theSource source node list
	 */
	explicit
	NodeRefList(const NodeRefListBase&	theSource);

	virtual
	~NodeRefList();

	NodeRefList&
	operator=(const NodeRefListBase&	theRHS);

	NodeRefList&
	operator=(const NodeRefList&	theRHS);

	bool
	empty() const
	{
		return m_nodeList.empty();
	}


	// These methods are inherited from NodeRefListBase ...

	virtual XalanNode*
	item(size_type	index) const;

	virtual size_type
	getLength() const;

	virtual size_type
	indexOf(const XalanNode*	theNode) const;

#if !defined(NDEBUG)
	bool
	checkForDuplicates() const;
#endif

#if defined(XALAN_NO_STD_NAMESPACE)
	typedef vector<XalanNode*>			NodeListVectorType;
#else
	typedef std::vector<XalanNode*>		NodeListVectorType;
#endif

	void
	swap(NodeRefList&	theOther)
	{
		m_nodeList.swap(theOther.m_nodeList);
	}

protected:

	// Default vector allocation size.  It seems high, but
	// it's really worth it...
	enum
	{
		eDefaultVectorSize = 100
	};

	/**
	 * Ensure that an allocation is either the default allocation
	 * amount, or the amount specified in the parameter, whichever
	 * is larger.
	 *
	 * @param theSize The requested size.
	 */
	void
	ensureAllocation(NodeListVectorType::size_type	theSize = 0)
	{
		m_nodeList.reserve(eDefaultVectorSize > theSize ? eDefaultVectorSize : theSize);
	}

	NodeListVectorType	m_nodeList;
};



XALAN_CPP_NAMESPACE_END



#endif	// NODEREFLIST_HEADER_GUARD_1357924680
