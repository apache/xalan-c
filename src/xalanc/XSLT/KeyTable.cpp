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
// Class header file
#include "KeyTable.hpp"



#include <memory>



#include <xalanc/XalanDOM/XalanElement.hpp>
#include <xalanc/XalanDOM/XalanNamedNodeMap.hpp>
#include <xalanc/XalanDOM/XalanNode.hpp>
#include <xalanc/XalanDOM/XalanNodeList.hpp>



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include <xalanc/XPath/XPath.hpp>



#include "KeyDeclaration.hpp"
#include "StylesheetExecutionContext.hpp"
#include "XSLTProcessorException.hpp"



XALAN_CPP_NAMESPACE_BEGIN



const MutableNodeRefList    KeyTable::s_dummyList(XalanMemMgrs::getDummyMemMgr());



KeyTable::KeyTable(
            XalanNode*                          startNode,
            const PrefixResolver&               resolver,
            const KeyDeclarationVectorType&     keyDeclarations,
            StylesheetExecutionContext&         executionContext) :
    m_keys(executionContext.getMemoryManager())
{
    XalanNode*  pos = startNode;

	m_allKeys=keyDeclarations;

    const KeyDeclarationVectorType::size_type   nDeclarations =
            keyDeclarations.size();

    // Do a non-recursive pre-walk over the tree.
    while (0 != pos)
    {
        // We're going to have to walk the attribute list 
        // if it's an element, so get the attributes.
        const XalanNamedNodeMap*    attrs = 0;

        XalanSize_t                 nNodes = 1;
        XalanSize_t                 nAttrNodes = 0;

        if (XalanNode::ELEMENT_NODE == pos->getNodeType())
        {
            attrs = pos->getAttributes();

            nAttrNodes = attrs->getLength();

            if (0 == nAttrNodes)
            {
                attrs = 0;
            }
            else
            {
                nNodes += nAttrNodes;
            }
        }

        // Walk the primary node, and each of the attributes.
        // This loop is a little strange... it is meant to always 
        // execute once, then execute for each of the attributes.
        XalanNode*      testNode = pos;
        XalanSize_t     nodeIndex = 0;

        for (XalanSize_t i = 0; i < nNodes; ++i)
        {
            // Walk through each of the declarations made with xsl:key
            for (KeyDeclarationVectorType::size_type i = 0; i < nDeclarations; ++i)
            {
                const KeyDeclaration&   kd = keyDeclarations[i];

                // See if our node matches the given key declaration according to 
                // the match attribute on xsl:key.
                assert(kd.getMatchPattern() != 0);

                const XPath::eMatchScore    score =
                        kd.getMatchPattern()->getMatchScore(
                            testNode,
                            resolver,
                            executionContext);

                if (score != XPath::eMatchScoreNone)
                {
                    processKeyDeclaration(
                        m_keys,
                        kd,
                        testNode,
                        resolver,
                        executionContext);
                }
            }

            if (0 != attrs && nodeIndex < nAttrNodes)
            {
                testNode = attrs->item(nodeIndex);

                ++nodeIndex;
            }
        }

        // The rest of this is getting the next pre-walk position in 
        // the tree.
        XalanNode*  nextNode = pos->getFirstChild();

        while(0 == nextNode)
        {
            if(startNode == pos)
            {
                break;
            }
            else
            {
                nextNode = pos->getNextSibling();

                if(0 == nextNode)
                {
                    pos = pos->getParentNode();

                    if((startNode == pos) || (0 == pos))
                    {
                        nextNode = 0;
                        break;
                    }
                }
            }
        }

        pos = nextNode;
    } // while(0 != pos)

    if (m_keys.empty() == false)
    {
        const KeysMapType::iterator     theEnd = m_keys.end();
        KeysMapType::iterator           theCurrent = m_keys.begin();
        assert(theCurrent != theEnd);

        do
        {
            NodeListMapType&    theCurrentNodeListMap = (*theCurrent).second;

            if (theCurrentNodeListMap.empty() == false)
            {
                const NodeListMapType::iterator     theEnd = theCurrentNodeListMap.end();
                NodeListMapType::iterator           theCurrent = theCurrentNodeListMap.begin();
                assert(theCurrent != theEnd);

                do
                {
                    (*theCurrent).second.setDocumentOrder();

                    ++theCurrent;
                }
                while(theCurrent != theEnd);
            }

            ++theCurrent;
        }
        while(theCurrent != theEnd);
    }   
} // end constructor



KeyTable*
KeyTable::create(
            MemoryManager&                      theManager,
            XalanNode*                          startNode,
            const PrefixResolver&               resolver,
            const KeyDeclarationVectorType&     keyDeclarations,
            StylesheetExecutionContext&         executionContext)
{
    typedef KeyTable    ThisType;

    XalanAllocationGuard    theGuard(theManager, theManager.allocate(sizeof(ThisType)));

    ThisType* const     theResult =
        new (theGuard.get()) ThisType(
                                startNode,
                                resolver,
                                keyDeclarations,
                                executionContext);
    theGuard.release();

    return theResult;
}



KeyTable::~KeyTable()
{
}



const MutableNodeRefList*
KeyTable::getNodeSetByKey(
                      const XalanQName&         qname, 
                      const XalanDOMString&     ref) const
{
    const KeysMapType::const_iterator   i = m_keys.find(qname);

    if (i != m_keys.end())
    {
        const NodeListMapType&  theMap = (*i).second;

        const NodeListMapType::const_iterator   j = theMap.find(ref);

        if (j != theMap.end())
        {
            return &(*j).second;
        }
        else
        {
            return &s_dummyList;
        }
    }
    else
    {
        size_type nDeclarations=m_allKeys.size();
        for (KeyDeclarationVectorType::size_type i = 0; i < nDeclarations; ++i)
        {
            if (*m_allKeys[i].getQName()==qname)
            {
                return &s_dummyList;
            }
        }
    }

    return 0;
}



inline void
addIfNotFound(
            StylesheetExecutionContext&     executionContext,
            MutableNodeRefList&             theNodeList,
            XalanNode*                      theNode)
{
    theNodeList.addNodeInDocOrder(theNode, executionContext);
}


static const NodeRefList    theEmptyList(XalanMemMgrs::getDummyMemMgr());

void
KeyTable::processKeyDeclaration(
            KeysMapType&                    theKeys,
            const KeyDeclaration&           kd,
            XalanNode*                      testNode,
            const PrefixResolver&           resolver,
            StylesheetExecutionContext&     executionContext)
{
    // Query from the node, according the the select pattern in the
    // use attribute in xsl:key.
    assert(kd.getUse() != 0);

    const XObjectPtr    xuse(kd.getUse()->execute(testNode, resolver, theEmptyList, executionContext));

    if(xuse->getType() != XObject::eTypeNodeSet)
    {
        assert(kd.getQName() != 0);

        addIfNotFound(
            executionContext,
            theKeys[*kd.getQName()][xuse->str(executionContext)],
            testNode);
    }
    else
    {
        const NodeRefListBase&  nl = xuse->nodeset();

        // Use each node in the node list as a key value that we'll be 
        // able to use to look up the given node.
        const NodeRefListBase::size_type    nUseValues = nl.getLength();

        const StylesheetExecutionContext::GetCachedString   theGuard(executionContext);

        XalanDOMString&     nodeData = theGuard.get();

        // Use each node in the node list as a key value that we'll be 
        // able to use to look up the given node.
        for (NodeRefListBase::size_type i = 0; i < nUseValues; ++i)
        {
            // Get the string value of the node to use as the result of the
            // expression.
            assert(nl.item(i) != 0);

            DOMServices::getNodeData(*nl.item(i), executionContext, nodeData);

            assert(kd.getQName() != 0);

            addIfNotFound(
                executionContext,
                theKeys[*kd.getQName()][nodeData],
                testNode);

            nodeData.clear();
        }
    }  
}



XALAN_CPP_NAMESPACE_END
