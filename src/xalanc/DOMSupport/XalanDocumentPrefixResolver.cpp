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
// Class header file...
#include "XalanDocumentPrefixResolver.hpp"



#include <xalanc/XalanDOM/XalanAttr.hpp>
#include <xalanc/XalanDOM/XalanElement.hpp>
#include <xalanc/XalanDOM/XalanNamedNodeMap.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XalanDocumentPrefixResolver::XalanDocumentPrefixResolver(
            const XalanDocument*    theDocument,
            const XalanDOMString&   theURI,
            MemoryManager&      theManager) :
    m_namespaces(theManager),
    m_uri(theURI, theManager)
{
    assert(theDocument != 0);

    NamespaceNodesTreeWalker    theWalker(m_namespaces);

    theWalker.traverse(theDocument);
}



XalanDocumentPrefixResolver::~XalanDocumentPrefixResolver()
{
}



const XalanDOMString*
XalanDocumentPrefixResolver::getNamespaceForPrefix(const XalanDOMString&    prefix) const
{
    const NamespacesMapType::const_iterator     i = m_namespaces.find(&prefix);

    if (i == m_namespaces.end())
    {
        return 0;
    }
    else
    {
        const AttributeVectorType&  theVector = (*i).second;

        assert(theVector.empty() == false);

        if (theVector.size() == 1)
        {
            assert(theVector.front() != 0);

            return &(theVector.front()->getNodeValue());
        }
        else
        {
            return duplicateBinding(theVector);
        }
    }
}



const XalanDOMString&
XalanDocumentPrefixResolver::getURI() const
{
    return m_uri;
}



const XalanDOMString*
XalanDocumentPrefixResolver::duplicateBinding(const AttributeVectorType&    theVector) const
{
    assert(theVector.front() != 0);

    return &(theVector.front()->getNodeValue());
}



XalanDocumentPrefixResolver::NamespaceNodesTreeWalker::NamespaceNodesTreeWalker(NamespacesMapType&  theMap) :
    TreeWalker(),
    m_map(theMap)
{
}



XalanDocumentPrefixResolver::NamespaceNodesTreeWalker::~NamespaceNodesTreeWalker()
{
}



bool
XalanDocumentPrefixResolver::NamespaceNodesTreeWalker::startNode(const XalanNode*   node)
{
    assert(node != 0);

    switch(node->getNodeType())
    {
    case XalanNode::ELEMENT_NODE:
        {
            const XalanElement* theElementNode =
                static_cast<const XalanElement*>(node);

            const XalanNamedNodeMap* const  atts = theElementNode->getAttributes();
            assert(atts != 0);

            const XalanSize_t   theSize = atts->getLength();

            for (XalanSize_t i = 0; i < theSize; ++i)
            {
                assert(atts->item(i) != 0 && atts->item(i)->getNodeType() == XalanNode::ATTRIBUTE_NODE);

                const XalanAttr* const  theAttr =
                    static_cast<const XalanAttr*>(atts->item(i));

                if (DOMServices::isNamespaceDeclaration(*theAttr) == true)
                {
                    m_map[&theAttr->getLocalName()].push_back(theAttr);
                }
            }
        }
        break;

    default:
        // Do nothing...
        break;
    }

    return false;
}



bool
XalanDocumentPrefixResolver::NamespaceNodesTreeWalker::startNode(XalanNode*     node)
{
    assert(node != 0);

    return startNode(const_cast<const XalanNode*>(node));
}



bool
XalanDocumentPrefixResolver::NamespaceNodesTreeWalker::endNode(const XalanNode*     /* node */)
{
    return false;
}



bool
XalanDocumentPrefixResolver::NamespaceNodesTreeWalker::endNode(XalanNode*   /* node */)
{
    return false;
}



XALAN_CPP_NAMESPACE_END
