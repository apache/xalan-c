/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the  "License");
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
#include "CountersTable.hpp"



#include <algorithm>



#include "ElemNumber.hpp"
#include "StylesheetExecutionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



inline void
appendBtoFList(
            CountersTable::NodeVectorType&          flist,
            const CountersTable::NodeVectorType&    blist)
{
    XALAN_USING_STD(back_inserter)
    XALAN_USING_STD(copy)

    flist.reserve(flist.size() + blist.size());

    copy(
        blist.rbegin(),
        blist.rend(),
        back_inserter(flist));
}



CountersTable::CountType
CountersTable::countNode(
            StylesheetExecutionContext&     support,
            const ElemNumber&               numberElem,
            XalanNode*                      node)
{
    assert(numberElem.getID() < m_countersVector.size());

    CountType   count = 0;

    CounterVectorType&  counters = m_countersVector[numberElem.getID()];

    const CounterVectorType::size_type  nCounters = counters.size();

    XalanNode*  target = numberElem.getTargetNode(support, node);

    if(0 != target)
    {
        for(CounterVectorType::size_type i = 0; i < nCounters; i++)
        {    
            const Counter&  counter = counters[i];

            count = counter.getPreviouslyCounted(support, target);

            if(count > 0)
            {
                return count;
            }
        }

        // In the loop below, we collect the nodes in backwards doc order, so 
        // we don't have to do inserts, but then we store the nodes in forwards 
        // document order, so we don't have to insert nodes into that list, 
        // so that's what the appendBtoFList stuff is all about.  In cases 
        // of forward counting by one, this will mean a single node copy from 
        // the backwards list (m_newFound) to the forwards list
        // (counter.m_countNodes).
        count = 0;

        for(; 0 != target; target = numberElem.getPreviousNode(support, target))
        {   
            // First time in, we should not have to check for previous counts, 
            // since the original target node was already checked in the 
            // block above.
            if(0 != count)  
            {
                for(CounterVectorType::size_type i = 0; i < nCounters; ++i)
                {
                    Counter&    counter = counters[i];

                    const Counter::NodeVectorType::size_type    cacheLen = counter.m_countNodes.size();
                    assert(cacheLen == CountType(cacheLen));

                    if(cacheLen > 0 && counter.m_countNodes[cacheLen - 1] == target)
                    {
                        count += CountType(cacheLen) + counter.m_countNodesStartCount;

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
        counters.resize(counters.size() + 1);

        Counter&    counter = counters.back();

        counter.m_numberElem = &numberElem;

        appendBtoFList(counter.m_countNodes, m_newFound);

        m_newFound.clear();
    }

    return count;
}



Counter::CountType
Counter::getPreviouslyCounted(
        StylesheetExecutionContext&     executionContext,
        const XalanNode*                node) const
{
    const NodeVectorType::size_type     n = m_countNodes.size();
    assert(CountType(n) == n);

    CountType   result = 0;

    for(NodeVectorType::size_type i = n; i > 0; --i)
    {
        const XalanNode* const  countedNode = m_countNodes[i - 1];

        if(node == countedNode)
        {
            // Since the list is in backwards order, the count is 
            // how many are in the rest of the list.
            result = CountType(i) + m_countNodesStartCount;

            break;
        }

        // Try to see if the given node falls after the counted node...
        // if it does, don't keep searching backwards.
        if(executionContext.isNodeAfter(*countedNode, *node))
        {
            break;
        }
    }

    return result;
}



XALAN_CPP_NAMESPACE_END
