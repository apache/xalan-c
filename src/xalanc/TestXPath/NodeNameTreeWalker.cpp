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
#include <xalanc/XalanDOM/XalanNode.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



// Class include file.
#include "NodeNameTreeWalker.hpp"



XALAN_CPP_NAMESPACE_BEGIN



NodeNameTreeWalker::NodeNameTreeWalker(MemoryManager& theManager) :
    TreeWalker(),
    m_nodeName(theManager),
    m_matchingNodes(theManager)
{
}



NodeNameTreeWalker::~NodeNameTreeWalker()
{
}



void
NodeNameTreeWalker::findMatchingNodes(
            const XalanDOMString&   theNodeName,
            const XalanNode*        theStartNode)
{
    assert(theStartNode != 0);

    m_nodeName = theNodeName;

    m_matchingNodes.clear();

    traverse(theStartNode->getFirstChild(),
             theStartNode);
}



void
NodeNameTreeWalker::findMatchingNodes(
            const XalanDOMString&   theNodeName,
            XalanNode*              theStartNode)
{
    assert(theStartNode != 0);

    m_nodeName = theNodeName;

    m_matchingNodes.clear();

    traverse(theStartNode->getFirstChild(),
             theStartNode);
}



bool
NodeNameTreeWalker::startNode(XalanNode*    node)
{
    const XalanDOMString&   theNodeName = node->getNodeName();

    if (equals(theNodeName, m_nodeName) == true)
    {
        m_matchingNodes.push_back(node);
    }

    return false;
}



bool
NodeNameTreeWalker::endNode(XalanNode*  /* node */)
{
    return false;
}



bool
NodeNameTreeWalker::startNode(const XalanNode*  node)
{
    const XalanDOMString&   theNodeName = node->getNodeName();

    if (equals(theNodeName, m_nodeName) == true)
    {
        m_matchingNodes.push_back(node);
    }

    return false;
}



bool
NodeNameTreeWalker::endNode(const XalanNode*    /* node */)
{
    return false;
}



XALAN_CPP_NAMESPACE_END
