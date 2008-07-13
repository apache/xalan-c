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
// Class header file.
#include "MutableNodeRefList.hpp"



#include <algorithm>
#include <cassert>
#include <functional>

#include <xalanc/Include/XalanMemMgrAutoPtr.hpp>

#include <xalanc/XalanDOM/XalanNamedNodeMap.hpp>
#include <xalanc/XalanDOM/XalanDocument.hpp>
#include <xalanc/XalanDOM/XalanNode.hpp>
#include <xalanc/XalanDOM/XalanNodeList.hpp>



#include "XPathExecutionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



MutableNodeRefList::MutableNodeRefList(MemoryManager&   theManager) :
    NodeRefList(theManager),
    m_order(eUnknownOrder)
{
}



MutableNodeRefList*
MutableNodeRefList::create(MemoryManager&   theManager)
{
    MutableNodeRefList*     theInstance = 0;

    return XalanConstruct(
            theManager,
            theInstance,
            theManager);
}



MutableNodeRefList::MutableNodeRefList(
            const MutableNodeRefList&   theSource,
            MemoryManager&              theManager) :
    NodeRefList(theSource, theManager),
    m_order(theSource.m_order)
{
}



MutableNodeRefList::MutableNodeRefList(
            const NodeRefListBase&  theSource,
            MemoryManager&          theManager) :
    NodeRefList(theSource, theManager),
    m_order(eUnknownOrder)
{
}


    
MutableNodeRefList::~MutableNodeRefList()
{
}



MutableNodeRefList&
MutableNodeRefList::operator=(const MutableNodeRefList&     theRHS)
{
    if (this != &theRHS)
    {
        // Chain up...
        NodeRefList::operator=(theRHS);

        m_order = theRHS.m_order;
    }

    return *this;
}



MutableNodeRefList&
MutableNodeRefList::operator=(const NodeRefList&    theRHS)
{
    if (this != &theRHS)
    {
        // Chain up...
        NodeRefList::operator=(theRHS);

        m_order = eUnknownOrder;
    }

    return *this;
}



MutableNodeRefList&
MutableNodeRefList::operator=(const NodeRefListBase&    theRHS)
{
    if (this != &theRHS)
    {
        // Chain up...
        NodeRefList::operator=(theRHS);

        m_order = eUnknownOrder;
    }

    return *this;
}



MutableNodeRefList&
MutableNodeRefList::operator=(const XalanNodeList*  theRHS)
{
    clear();

    if (theRHS != 0)
    {
        addNodes(*theRHS);
    }

    return *this;
}



void
MutableNodeRefList::addNode(XalanNode*  n)
{
    if (n != 0)
    {
        m_nodeList.push_back(n);
    }
}



void
MutableNodeRefList::insertNode(
            XalanNode*  n,
            size_type   pos)
{
    assert(m_nodeList.size() >= pos);

    if (n != 0)
    {
        m_nodeList.insert(m_nodeList.begin() + pos, n);
    }
}



void
MutableNodeRefList::removeNode(const XalanNode*     n)
{
    XALAN_USING_STD(find)

    NodeListVectorType::iterator    i =
        find(m_nodeList.begin(),
             m_nodeList.end(),
             n);

    if (i != m_nodeList.end())
    {
        m_nodeList.erase(i);
    }
}



void
MutableNodeRefList::removeNode(size_type    pos)
{
    assert(pos < m_nodeList.size());

    m_nodeList.erase(m_nodeList.begin() + pos);
}



void
MutableNodeRefList::clear()
{
    m_nodeList.clear();

    m_order = eUnknownOrder;
}



void
MutableNodeRefList::setNode(
            size_type   pos,
            XalanNode*  theNode)
{
    assert(pos < m_nodeList.size());

    m_nodeList[pos] = theNode;
}



void
MutableNodeRefList::addNodes(const XalanNodeList&   nodelist)
{
    const XalanSize_t     theLength = nodelist.getLength();

    for (XalanSize_t i = 0; i < theLength; i++)
    {
        XalanNode* const    theNode = nodelist.item(i);

        if (theNode != 0)
        {
            m_nodeList.push_back(theNode);
        }
    }
}



void
MutableNodeRefList::addNodes(const NodeRefListBase&     nodelist)
{
    const size_type theLength = nodelist.getLength();

    for (size_type i = 0; i < theLength; i++)
    {
        XalanNode* const    theNode = nodelist.item(i);

        if (theNode != 0)
        {

            m_nodeList.push_back(theNode);
        }
    }
}



void
MutableNodeRefList::addNodesInDocOrder(
            const XalanNodeList&    nodelist,
            XPathExecutionContext&  executionContext)
{
    const XalanSize_t  theOtherLength = nodelist.getLength();

    for(XalanSize_t i = 0; i < theOtherLength; i++)
    {
        addNodeInDocOrder(nodelist.item(i), executionContext);
    }
}



void
MutableNodeRefList::addNodesInDocOrder(
            const NodeRefListBase&  nodelist,
            XPathExecutionContext&  executionContext)
{
    const XalanSize_t   theOtherLength = nodelist.getLength();

    for(XalanSize_t i = 0; i < theOtherLength; i++)
    {
        addNodeInDocOrder(nodelist.item(i), executionContext);
    }
}



void
MutableNodeRefList::addNodesInDocOrder(
            const MutableNodeRefList&   nodelist,
            XPathExecutionContext&      executionContext)
{
    XALAN_USING_STD(back_inserter);
    XALAN_USING_STD(copy);
    XALAN_USING_STD(for_each);

    const eOrder        theOtherOrder = nodelist.m_order;

    if (theOtherOrder == eUnknownOrder)
    {
        for_each(
            nodelist.m_nodeList.begin(),
            nodelist.m_nodeList.end(),
            addNodeInDocOrderFunctor(*this, executionContext));
    }
    else if (theOtherOrder == eDocumentOrder)
    {
        if (m_nodeList.empty() == true)
        {
            m_nodeList = nodelist.m_nodeList;
        }
        else
        {
            for_each(
                nodelist.m_nodeList.begin(),
                nodelist.m_nodeList.end(),
                addNodeInDocOrderFunctor(*this, executionContext));
        }
    }
    else
    {
        assert(theOtherOrder == eReverseDocumentOrder);

        if (m_nodeList.empty() == true)
        {
            copy(
                nodelist.m_nodeList.rbegin(),
                nodelist.m_nodeList.rend(),
                back_inserter(m_nodeList));
        }
        else
        {
            for_each(
                nodelist.m_nodeList.rbegin(),
                nodelist.m_nodeList.rend(),
                addNodeInDocOrderFunctor(*this, executionContext));
        }
    }
}



static bool
findInsertionPointBinarySearch(
            XalanNode*                                  node,
            MutableNodeRefList::NodeListIteratorType    begin,
            MutableNodeRefList::NodeListIteratorType    end,
            MutableNodeRefList::NodeListIteratorType&   insertionPoint)
{
    assert(node != 0);
    assert(
        node->getNodeType() == XalanNode::DOCUMENT_NODE ||
        node->getNodeType() == XalanNode::DOCUMENT_FRAGMENT_NODE ||
        (node->getOwnerDocument() != 0 && node->getOwnerDocument()->isIndexed() == true));

    bool    fInsert = true;

    // At this point, we are guaranteed that the range is only for this
    // document, and that the range is indexed...
    const XalanNode::IndexType  theIndex = node->getIndex();

    typedef MutableNodeRefList::NodeListIteratorType    NodeListIteratorType;

    // End points to one past the last valid point,
    // so subtract 1.
    NodeListIteratorType    last(end - 1);
    assert(*last != 0);

    // Do a quick check to see if we just need to append...
    if ((*last)->getIndex() < theIndex)
    {
        insertionPoint = end;
    }
    else
    {
        // Do a binary search for the insertion point...
        NodeListIteratorType    first(begin);
        NodeListIteratorType    current(end);

        XalanNode::IndexType    theCurrentIndex = 0;

        while (first <= last)
        {
            current = first + (last - first) / 2;
            assert(*current != 0);

            theCurrentIndex = (*current)->getIndex();

            if (theIndex < theCurrentIndex)
            {
                if (current == begin)
                {
                    break;
                }
                else
                {
                    last = current - 1;
                }
            }
            else if (theIndex > theCurrentIndex)
            {
                first = current + 1;
            }
            else if (theIndex == theCurrentIndex)
            {
                // Duplicate, don't insert...
                fInsert = false;

                break;
            }
        }

        if (theIndex != theCurrentIndex)
        {
            if (current == end || first == end)
            {
                // We either didn't search, or we're
                // at the end...
                insertionPoint = end;
            }
            else if (theCurrentIndex < theIndex)
            {
                // We're inserting after the current position...
                assert((*current)->getIndex() < theIndex &&
                       (current + 1 == end || (*(current + 1))->getIndex() > theIndex));

                insertionPoint = current + 1;
            }
            else
            {
                // We're inserting before the current position...
                assert(theCurrentIndex > theIndex);
                assert((*current)->getIndex() > theIndex &&
                       (current == begin || (*(current))->getIndex() > theIndex));

                insertionPoint = current;
            }
        }
    }

    return fInsert;
}



template<class PredicateType>
inline bool
findInsertionPointLinearSearch(
            XalanNode*                                  node,
            MutableNodeRefList::NodeListIteratorType    begin,
            MutableNodeRefList::NodeListIteratorType    end,
            MutableNodeRefList::NodeListIteratorType&   insertionPoint,
            const PredicateType                         isNodeAfterPredicate)
{
    assert(node != 0);

    bool    fInsert = true;

    typedef MutableNodeRefList::NodeListIteratorType    NodeListIteratorType;

    NodeListIteratorType    current(begin);

    // Loop, looking for the node, or for a
    // node that's before the one we're adding...
    while(current != end)
    {
        const XalanNode*    child = *current;
        assert(child != 0);

        if(child == node)
        {
            // Duplicate, don't insert...
            fInsert = false;

            break;
        }
        else if (isNodeAfterPredicate(*node, *child) == false)
        {
            // We found the insertion point...
            break;
        }
        else
        {
            ++current;
        }
    }

    insertionPoint = current;

    return fInsert;
}



struct DocumentPredicate
{
    bool
    operator()(
            const XalanNode&    node1,
            const XalanNode&    node2) const
    {
        // Always order a document node, or a node from another
        // document after another node...
        const XalanNode::NodeType   node1Type =
            node1.getNodeType();

        const XalanNode::NodeType   node2Type =
            node2.getNodeType();

        if ((node1Type == XalanNode::DOCUMENT_NODE ||
             node1Type == XalanNode::DOCUMENT_FRAGMENT_NODE) &&
            (node2Type == XalanNode::DOCUMENT_NODE ||
             node2Type == XalanNode::DOCUMENT_FRAGMENT_NODE))
        {
            return true;
        }
        else
        {
            return node1.getOwnerDocument() != node2.getOwnerDocument();
        }
    }
};



struct IndexPredicate
{
    bool
    operator()(
            const XalanNode&    node1,
            const XalanNode&    node2) const
    {
        assert(node1.getOwnerDocument() == node2.getOwnerDocument());

        return m_documentPredicate(node1, node2) == true ? true : node1.getIndex() > node2.getIndex() ? true : false;
    }

    DocumentPredicate   m_documentPredicate;
};




struct ExecutionContextPredicate
{
    ExecutionContextPredicate(XPathExecutionContext&    executionContext) :
        m_executionContext(executionContext)
    {
    }

    bool
    operator()(
            const XalanNode&    node1,
            const XalanNode&    node2) const
    {
        if (m_documentPredicate(node1, node2) == true)
        {
            return true;
        }
        else
        {
            assert(node1.getOwnerDocument() == node2.getOwnerDocument());
            assert(
                node1.getNodeType() != XalanNode::DOCUMENT_NODE &&
                node1.getNodeType() != XalanNode::DOCUMENT_FRAGMENT_NODE &&
                node2.getNodeType() != XalanNode::DOCUMENT_NODE &&
                node2.getNodeType() != XalanNode::DOCUMENT_FRAGMENT_NODE);

            return  m_executionContext.isNodeAfter(node1, node2);
        }
    }

    XPathExecutionContext&  m_executionContext;

    DocumentPredicate       m_documentPredicate;
};




void
MutableNodeRefList::addNodeInDocOrder(
            XalanNode*              node,
            XPathExecutionContext&  executionContext)
{
    if (node != 0)
    {
        if (m_nodeList.size() == 0)
        {
            addNode(node);
        }
        else
        {
            assert(m_nodeList[0] != 0);

            // Do some quick optimizations, since we tend to append
            // the same node a lot.
            const XalanNode* const  theLastNode = m_nodeList.back();
            assert(theLastNode != 0);

            // Is it a duplicate?
            if (theLastNode != node)
            {
                bool                    fInsert = false;

                NodeListIteratorType    insertionPoint;

                const XalanNode* const  theFirstNode = m_nodeList.front();
                assert(theFirstNode != 0);

                // Normalize so that if we have a document node, it owns
                // itself, which is not how DOM works...
                const XalanNode::NodeType   theFirstNodeType =
                    theFirstNode->getNodeType();

                const XalanNode* const  theFirstNodeOwner =
                     theFirstNodeType == XalanNode::DOCUMENT_NODE ||
                     theFirstNodeType == XalanNode::DOCUMENT_FRAGMENT_NODE ?
                            theFirstNode : theFirstNode->getOwnerDocument();
                assert(theFirstNodeOwner != 0);

                if (node->isIndexed() == true &&
                    node->getOwnerDocument() == theFirstNodeOwner)
                {
                    // If it's indexed, then see if the entire list consists of
                    // nodes from the same document.
                    // Normalize so that if we have a document node, it owns
                    // itself, which is not how DOM works...
                    const XalanNode::NodeType   theLastNodeType =
                            theLastNode->getNodeType();
                    const XalanNode* const  theLastNodeOwner =
                        theLastNodeType == XalanNode::DOCUMENT_NODE ||
                        theLastNodeType == XalanNode::DOCUMENT_FRAGMENT_NODE ?
                                theLastNode : theLastNode->getOwnerDocument();
                    assert(theLastNodeOwner != 0);

                    // If the owner document is 0, then it's a document node, so there's not
                    // much we can do except a linear search...
                    if (theFirstNodeOwner == theLastNodeOwner)
                    {
                        fInsert =
                            findInsertionPointBinarySearch(
                                    node,
                                    m_nodeList.begin(),
                                    m_nodeList.end(),
                                    insertionPoint);
                    }
                    else
                    {
                        fInsert =
                            findInsertionPointLinearSearch(
                                    node,
                                    m_nodeList.begin(),
                                    m_nodeList.end(),
                                    insertionPoint,
                                    IndexPredicate());
                    }
                }
                else
                {
                    fInsert =
                            findInsertionPointLinearSearch(
                                    node,
                                    m_nodeList.begin(),
                                    m_nodeList.end(),
                                    insertionPoint,
                                    ExecutionContextPredicate(executionContext));
                }

                if (fInsert == true)
                {
                    m_nodeList.insert(insertionPoint, node);
                }
            }
        }
    }
}



void
MutableNodeRefList::clearNulls()
{
    XALAN_USING_STD(remove);

    m_nodeList.erase(
        remove(
            m_nodeList.begin(),
            m_nodeList.end(), 
            NodeListVectorType::value_type(0)),
        m_nodeList.end());

    if (m_nodeList.empty() == true)
    {
        m_order = eUnknownOrder;
    }

    assert(checkForDuplicates(getMemoryManager()) == false);
}



void
MutableNodeRefList::reverse()
{
#if defined(XALAN_NO_STD_NAMESPACE)
    ::reverse(
#else
    std::reverse(
#endif
        m_nodeList.begin(),
        m_nodeList.end());

    if (m_order == eDocumentOrder)
    {
        m_order = eReverseDocumentOrder;
    }
    else if (m_order == eReverseDocumentOrder)
    {
        m_order = eDocumentOrder;
    }
}



XALAN_CPP_NAMESPACE_END
