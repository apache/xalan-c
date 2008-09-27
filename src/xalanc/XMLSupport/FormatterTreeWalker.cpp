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
#include "FormatterTreeWalker.hpp"



// Xerces header files...
#include <xalanc/XalanDOM/XalanElement.hpp>
#include <xalanc/XalanDOM/XalanNamedNodeMap.hpp>



#include <xalanc/PlatformSupport/AttributeListImpl.hpp>
#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/FormatterListener.hpp>
#include <xalanc/PlatformSupport/NamedNodeMapAttributeList.hpp>



XALAN_CPP_NAMESPACE_BEGIN



FormatterTreeWalker::FormatterTreeWalker(
            FormatterListener&  formatterListener,
            MemoryManager&      theManager) :
    TreeWalker(),
    m_formatterListener(formatterListener),
    m_memoryManager(theManager)
{
}



FormatterTreeWalker::~FormatterTreeWalker()
{
}



bool
FormatterTreeWalker::startNode(const XalanNode*     node)
{
    assert(node != 0);

    switch(node->getNodeType())
    {
    case XalanNode::COMMENT_NODE:
        {
            m_formatterListener.comment(node->getNodeValue().c_str());
        }
        break;

    case XalanNode::DOCUMENT_FRAGMENT_NODE:
        // ??
        break;

    case XalanNode::DOCUMENT_NODE:
        m_formatterListener.startDocument();
        break;

    case XalanNode::ELEMENT_NODE:
        {
            const XalanElement* theElementNode =
                static_cast<const XalanElement*>(node);

            const XalanNamedNodeMap*    atts = theElementNode->getAttributes();
            assert(atts != 0);

            NamedNodeMapAttributeList   theAttributeList(*atts, m_memoryManager);

            m_formatterListener.startElement(
                theElementNode->getNodeName().c_str(),
                theAttributeList);
        }
        break;

    case XalanNode::PROCESSING_INSTRUCTION_NODE:
        {
            m_formatterListener.processingInstruction(
                node->getNodeName().c_str(),
                node->getNodeValue().c_str());
        }
        break;

    case XalanNode::CDATA_SECTION_NODE:
        {
            const XalanDOMString&   data = node->getNodeValue();

            assert(data.length() == static_cast<FormatterListener::size_type>(data.length()));

            m_formatterListener.cdata(
                data.c_str(),
                static_cast<FormatterListener::size_type>(data.length()));
        }
        break;

    case XalanNode::TEXT_NODE:
        {
            const XalanDOMString&   data = node->getNodeValue();

            assert(data.length() == static_cast<FormatterListener::size_type>(data.length()));

            m_formatterListener.characters(
                data.c_str(),
                static_cast<FormatterListener::size_type>(data.length()));
        }
        break;

    case XalanNode::ENTITY_REFERENCE_NODE:
        m_formatterListener.entityReference(node->getNodeName().c_str());
        break;

    default:
        // Do nothing...
        break;
    }

    return false;
}



bool
FormatterTreeWalker::startNode(XalanNode*   node)
{
    assert(node != 0);

    return startNode(const_cast<const XalanNode*>(node));
}



bool
FormatterTreeWalker::endNode(const XalanNode*   node)
{
    assert(node != 0);

    switch(node->getNodeType())
    {
    case XalanNode::DOCUMENT_NODE:
        m_formatterListener.endDocument();
        break;

    case XalanNode::ELEMENT_NODE:
        m_formatterListener.endElement(node->getNodeName().c_str());
        break;

    default:
        // Do nothing
        break;
    }

    return false;
}



bool
FormatterTreeWalker::endNode(XalanNode*     node)
{
    return endNode(const_cast<const XalanNode*>(node));
}



XALAN_CPP_NAMESPACE_END
