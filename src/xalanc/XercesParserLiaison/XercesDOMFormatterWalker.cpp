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
#include "XercesDOMFormatterWalker.hpp"



// Xerces header files...
#include <xercesc/dom/DOMElement.hpp>



#include <xalanc/PlatformSupport/AttributeListImpl.hpp>
#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/FormatterListener.hpp>



#include "XercesNamedNodeMapAttributeList.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XercesDOMFormatterWalker::XercesDOMFormatterWalker(FormatterListener&   formatterListener) :
    XercesDOMWalker(),
    m_formatterListener(formatterListener)
{
}



XercesDOMFormatterWalker::~XercesDOMFormatterWalker()
{
}



bool
XercesDOMFormatterWalker::startNode(const DOMNodeType*  node)
{
    assert(node != 0);

    switch(node->getNodeType())
    {
    case DOMNodeType::COMMENT_NODE:
        {
            m_formatterListener.comment(node->getNodeValue());
        }
        break;

    case DOMNodeType::DOCUMENT_FRAGMENT_NODE:
        // ??
        break;

    case DOMNodeType::DOCUMENT_NODE:
        m_formatterListener.startDocument();
        break;

    case DOMNodeType::ELEMENT_NODE:
        {
            const DOMElementType* const     theElementNode =
                static_cast<const DOMElementType*>(node);

            const DOMNamedNodeMapType* const    atts = theElementNode->getAttributes();
            assert(atts != 0);

            XercesNamedNodeMapAttributeList     theAttributeList(atts);

            m_formatterListener.startElement(
                    theElementNode->getNodeName(),
                    theAttributeList);
        }
        break;

    case DOMNodeType::PROCESSING_INSTRUCTION_NODE:
        {
            m_formatterListener.processingInstruction(
                    node->getNodeName(),
                    node->getNodeValue());
        }
        break;

    case DOMNodeType::CDATA_SECTION_NODE:
        {
            const XMLCh* const  data = node->getNodeValue();

            assert(length(data) == FormatterListener::size_type(length(data)));

            m_formatterListener.cdata(data, FormatterListener::size_type(length(data)));
        }
        break;

    case DOMNodeType::TEXT_NODE:
        {
            const XMLCh* const  data = node->getNodeValue();

            assert(length(data) == FormatterListener::size_type(length(data)));

            m_formatterListener.characters(data, FormatterListener::size_type(length(data)));
        } 
        break;

    case DOMNodeType::ENTITY_REFERENCE_NODE:
        m_formatterListener.entityReference(node->getNodeName());
        break;

    default:
        // Do nothing...
        break;
    }

    return false;
}



bool
XercesDOMFormatterWalker::endNode(const DOMNodeType*    node)
{
    assert(node != 0);

    switch(node->getNodeType())
    {
    case DOMNodeType::DOCUMENT_NODE:
        m_formatterListener.endDocument();
        break;

    case DOMNodeType::ELEMENT_NODE:
        m_formatterListener.endElement(node->getNodeName());
        break;

    default:
        // Do nothing
        break;
    }

    return false;
}



XALAN_CPP_NAMESPACE_END
