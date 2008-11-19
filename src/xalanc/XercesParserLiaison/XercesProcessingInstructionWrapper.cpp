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
#include "XercesProcessingInstructionWrapper.hpp"



#include <xercesc/dom/DOMProcessingInstruction.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "XercesDOMWrapperException.hpp"
#include "XercesWrapperHelper.hpp"
#include "XercesWrapperNavigator.hpp"
#include "XercesDocumentWrapper.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XercesProcessingInstructionWrapper::XercesProcessingInstructionWrapper(
            const DOMProcessingInstructionType*     theXercesDOMProcessingInstruction,
            const XercesWrapperNavigator&           theNavigator) :
    XalanProcessingInstruction(),
    m_xercesNode(theXercesDOMProcessingInstruction),
    m_navigator(theNavigator)
{
    assert(theXercesDOMProcessingInstruction != 0);
}


    
XercesProcessingInstructionWrapper*
XercesProcessingInstructionWrapper::create(
            MemoryManager&                          theManager,
            const DOMProcessingInstructionType*     theXercesDOMProcessingInstruction,
            const XercesWrapperNavigator&           theNavigator)

{
    typedef XercesProcessingInstructionWrapper  ThisType;

    XalanAllocationGuard    theGuard(theManager, theManager.allocate(sizeof(ThisType)));

    ThisType* const     theResult =
        new (theGuard.get()) ThisType(
                                theXercesDOMProcessingInstruction,
                                theNavigator);

    theGuard.release();

    return theResult;
}



XercesProcessingInstructionWrapper::~XercesProcessingInstructionWrapper()
{
}



const XalanDOMString&
XercesProcessingInstructionWrapper::getNodeName() const
{
    return m_navigator.getPooledString(m_xercesNode->getNodeName());
}



const XalanDOMString&
XercesProcessingInstructionWrapper::getNodeValue() const
{
    return m_navigator.getPooledString(m_xercesNode->getNodeValue());
}



XercesProcessingInstructionWrapper::NodeType
XercesProcessingInstructionWrapper::getNodeType() const
{
    return PROCESSING_INSTRUCTION_NODE;
}



XalanNode*
XercesProcessingInstructionWrapper::getParentNode() const
{
    return m_navigator.getParentNode(m_xercesNode);
}



const XalanNodeList*
XercesProcessingInstructionWrapper::getChildNodes() const
{
    return 0;
}



XalanNode*
XercesProcessingInstructionWrapper::getFirstChild() const
{
    return 0;
}



XalanNode*
XercesProcessingInstructionWrapper::getLastChild() const
{
    return 0;
}



XalanNode*
XercesProcessingInstructionWrapper::getPreviousSibling() const
{
    return m_navigator.getPreviousSibling(m_xercesNode);
}



XalanNode*
XercesProcessingInstructionWrapper::getNextSibling() const
{
    return m_navigator.getNextSibling(m_xercesNode);
}



const XalanNamedNodeMap*
XercesProcessingInstructionWrapper::getAttributes() const
{
    return 0;
}



XalanDocument*
XercesProcessingInstructionWrapper::getOwnerDocument() const
{
    return m_navigator.getOwnerDocument();
}



const XalanDOMString&
XercesProcessingInstructionWrapper::getNamespaceURI() const
{
    return m_navigator.getPooledString(m_xercesNode->getNamespaceURI());
}



const XalanDOMString&
XercesProcessingInstructionWrapper::getPrefix() const
{
    return m_navigator.getPooledString(m_xercesNode->getPrefix());
}



const XalanDOMString&
XercesProcessingInstructionWrapper::getLocalName() const
{
    return m_navigator.getPooledString(m_xercesNode->getLocalName());
}



bool
XercesProcessingInstructionWrapper::isIndexed() const
{
    return m_navigator.getOwnerDocument()->isIndexed();
}



XercesProcessingInstructionWrapper::IndexType
XercesProcessingInstructionWrapper::getIndex() const
{
    return m_navigator.getIndex();
}



const XalanDOMString&
XercesProcessingInstructionWrapper::getTarget() const
{
    return m_navigator.getPooledString(m_xercesNode->getTarget());
}



const XalanDOMString&
XercesProcessingInstructionWrapper::getData() const
{
    return m_navigator.getPooledString(m_xercesNode->getData());
}



XALAN_CPP_NAMESPACE_END
