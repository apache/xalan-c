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
#if !defined(XSPAN_HEADER_GUARD_1357924680)
#define XSPAN_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



// Base class header file.
#include <xalanc/XPath/XNodeSet.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XPATH_EXPORT XSpan : public XNodeSet
{
public:

	/**
	 * Construct an XSpan object from a node list.
	 * 
	 * @param value      source node list.  The instance will adopt the value instance.
	 */
	XSpan(BorrowReturnMutableNodeRefList&	value, MemoryManagerType& theManager);

	XSpan(const XSpan&	source, MemoryManagerType& theManager);

	virtual
	~XSpan();


	/**
	 * Get the start span offset from the first node, or -1 if offset isn't
	 * set.
	 *
	 * @return starting offset
	 */
	virtual int
	getStart() const;

	/**
	 * Set the start span offset from the first node, or pass -1 if offset
	 * isn't set.
	 *
	 * @param start starting offset
	 */
	virtual void
	setStart(int	start);

	/**
	 * Get the end span offset from start of the last node, or -1 if offset
	 * isn't set.
	 *
	 * @return ending offset
	 */
	virtual int
	getEnd() const;

	/**
	 * Set the end span offset from start of the last node, or -1 if offset
	 * isn't set.
	 *
	 * @param end ending offset
	 */
	virtual void
	setEnd(int	end);
  
	/**
	 * Set the range of the span, based on offsets from the first node and
	 * offsets from the last node.
	 *
	 * @param firstNodeOffset offset of first node
	 * @param lastNodeOffset  offset of last node   
	 */
	virtual void
	setSpan(
			int		firstNodeOffset,
			int		lastNodeOffset);

private:

	int		m_start;
	int		m_end;
};



XALAN_CPP_NAMESPACE_END



#endif	// XSPAN_HEADER_GUARD_1357924680
