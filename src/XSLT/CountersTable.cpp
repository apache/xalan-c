/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2000 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */
#include "CountersTable.hpp"



#include <algorithm>



#include "ElemNumber.hpp"
#include "StylesheetExecutionContext.hpp"



inline void
CountersTable::appendBtoFList(
			NodeVectorType&		flist,
			NodeVectorType&		blist)
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::back_inserter;
	using std::copy;
#endif

	flist.reserve(flist.size() + blist.size());

	copy(
		blist.rbegin(),
		blist.rend(),
		back_inserter(flist));
}



int
CountersTable::countNode(
			StylesheetExecutionContext&		support,
			const ElemNumber*				numberElem,
			XalanNode*						node)
{
	int		count = 0;

	CounterVectorType&	counters = m_counterMap[numberElem];

	const CounterVectorType::size_type	nCounters = counters.size();

	XalanNode*	target = numberElem->getTargetNode(support, node);

	if(0 != target)
	{
		for(CounterVectorType::size_type i = 0; i < nCounters; i++)
		{    
			const Counter&	counter = counters[i];

			count = counter.getPreviouslyCounted(support, target);

			if(count > 0)
				return count;
		}

		// In the loop below, we collect the nodes in backwards doc order, so 
		// we don't have to do inserts, but then we store the nodes in forwards 
		// document order, so we don't have to insert nodes into that list, 
		// so that's what the appendBtoFList stuff is all about.  In cases 
		// of forward counting by one, this will mean a single node copy from 
		// the backwards list (m_newFound) to the forwards list
		// (counter.m_countNodes).
		count = 0;
		for(; 0 != target; target = numberElem->getPreviousNode(support, target))
		{   
			// First time in, we should not have to check for previous counts, 
			// since the original target node was already checked in the 
			// block above.
			if(0 != count)  
			{
				for(CounterVectorType::size_type i = 0; i < nCounters; ++i)
				{
					Counter&	counter = counters[i];

					const unsigned int	cacheLen = counter.m_countNodes.size();

					if(cacheLen > 0 && counter.m_countNodes[cacheLen - 1] == target)
					{
						count += cacheLen + counter.m_countNodesStartCount;

						if(cacheLen > 0)
						{
							appendBtoFList(counter.m_countNodes, m_newFound);
						}

						m_newFound.clear();

						return count;
					}
				}
			}

			m_newFound.push_back(target);

			++count;
		}

		// If we got to this point, then we didn't find a counter, so make 
		// one and add it to the list.
		counters.push_back(Counter(numberElem));

		Counter&	counter = counters.back();

		appendBtoFList(counter.m_countNodes, m_newFound);

		m_newFound.clear();
	}

	return count;
}



int
Counter::getPreviouslyCounted(
		StylesheetExecutionContext&		executionContext,
		const XalanNode*				node) const
{
	const int	n = m_countNodes.size();

	int			result = 0;

	for(int i = n - 1; i >= 0; --i)
	{
		const XalanNode* const	countedNode = m_countNodes[i];

		if(node == countedNode)
		{
			// Since the list is in backwards order, the count is 
			// how many are in the rest of the list.
			result = i + 1 + m_countNodesStartCount;
			break;
		}

		// Try to see if the given node falls after the counted node...
		// if it does, don't keep searching backwards.
		if(executionContext.isNodeAfter(*countedNode, *node))
			break;
	}

	return result;
}
