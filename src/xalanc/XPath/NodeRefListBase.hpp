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
#if !defined(NODEREFLISTBASE_HEADER_GUARD_1357924680)
#define NODEREFLISTBASE_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



#include <cstddef>



XALAN_CPP_NAMESPACE_BEGIN



class XalanNode;



/**
 * Local implementation of NodeRefList.  This class is for internal use only.
 */

class XALAN_XPATH_EXPORT NodeRefListBase
{
public:

	NodeRefListBase();

	virtual
	~NodeRefListBase();

//#if defined(XALAN_STRICT_ANSI_HEADERS)
//	typedef std::size_t		size_type;
//#else
//	typedef size_t			size_type;
//#endif
    typedef unsigned int    size_type;

	/**
	 * Returns the <code>index</code>th item in the collection. If
	 * <code>index</code> is greater than or equal to the number of nodes in
	 * the list, this returns <code>null</code>.
	 *
	 * @param index index into the collection
	 * @return node at the <code>index</code>th position in the
	 *         <code>NodeList</code>, or <code>null</code> if that is not a
	 *         valid index
	 */
	virtual XalanNode*
	item(size_type	index) const = 0;

	/**
	 * Determine the number of nodes in the list. The range of valid child node
	 * indices is 0 to <code>length-1</code> inclusive. 
	 *
	 * @return number of nodes
	 */
	virtual size_type
	getLength() const = 0;

	/**
	 * Retrieve the zero based index of the node in the list
	 * 
	 * @param theNode node whose index to find
	 * @return index of node
	 */
	virtual size_type
	indexOf(const XalanNode*	theNode) const = 0;

#if defined(XALAN_INLINE_INITIALIZATION)
	static const size_type	npos = ~0u;
#else
	static const size_type	npos;
#endif

protected:

	NodeRefListBase(const NodeRefListBase&)
	{
	}

	NodeRefListBase&
	operator=(const NodeRefListBase&)
	{
		return *this;
	}
};



XALAN_CPP_NAMESPACE_END



#endif	// NODEREFLISTBASE_HEADER_GUARD_1357924680
