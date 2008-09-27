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
#include "DOMServices.hpp"



#include <xalanc/XalanDOM/XalanAttr.hpp>
#include <xalanc/XalanDOM/XalanCDATASection.hpp>
#include <xalanc/XalanDOM/XalanComment.hpp>
#include <xalanc/XalanDOM/XalanDOMString.hpp>
#include <xalanc/XalanDOM/XalanDocument.hpp>
#include <xalanc/XalanDOM/XalanDocumentFragment.hpp>
#include <xalanc/XalanDOM/XalanElement.hpp>
#include <xalanc/XalanDOM/XalanNamedNodeMap.hpp>
#include <xalanc/XalanDOM/XalanNodeList.hpp>
#include <xalanc/XalanDOM/XalanProcessingInstruction.hpp>
#include <xalanc/XalanDOM/XalanText.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/PrefixResolver.hpp>
#include <xalanc/PlatformSupport/XalanUnicode.hpp>



#include "DOMSupportException.hpp"



XALAN_USING_XALAN(XalanDOMString)


// These XalanDOMString instances will hold the actual
// data.  This way, the DOMSupport references can be const,
// but we can initialize the data when we want to.

XALAN_USING_XALAN(XalanMemMgrs)

static XalanDOMString   s_XMLString(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString   s_XMLStringWithSeparator(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString   s_XMLNamespacePrefix(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString   s_XMLNamespaceURI(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString   s_XMLNamespace(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString   s_XMLNamespaceWithSeparator(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString   s_XMLNamespaceSeparatorString(XalanMemMgrs::getDummyMemMgr());
static XalanDOMString   s_XMLNamespacePrefixURI(XalanMemMgrs::getDummyMemMgr());



// These variables will hold the actual data.  This way,
// the DOMSupport references can be const, but we can initialize
// the data when we want to.
static XalanDOMString::size_type    s_XMLStringLength = 0;
static XalanDOMString::size_type    s_XMLStringWithSeparatorLength = 0;
static XalanDOMString::size_type    s_XMLNamespacePrefixLength = 0;
static XalanDOMString::size_type    s_XMLNamespaceURILength = 0;
static XalanDOMString::size_type    s_XMLNamespaceLength = 0;
static XalanDOMString::size_type    s_XMLNamespaceWithSeparatorLength = 0;
static XalanDOMString::size_type    s_XMLNamespaceSeparatorStringLength = 0;
static XalanDOMString::size_type    s_XMLNamespacePrefixURILength = 0;



XALAN_CPP_NAMESPACE_BEGIN



const XalanDOMString&   DOMServices::s_XMLString = ::s_XMLString;
const XalanDOMString&   DOMServices::s_XMLStringWithSeparator = ::s_XMLStringWithSeparator;
const XalanDOMString&   DOMServices::s_XMLNamespacePrefix = ::s_XMLNamespacePrefix;
const XalanDOMString&   DOMServices::s_XMLNamespaceURI = ::s_XMLNamespaceURI;
const XalanDOMString&   DOMServices::s_XMLNamespace = ::s_XMLNamespace;
const XalanDOMString&   DOMServices::s_XMLNamespaceWithSeparator = ::s_XMLNamespaceWithSeparator;
const XalanDOMString&   DOMServices::s_XMLNamespaceSeparatorString  = ::s_XMLNamespaceSeparatorString;
const XalanDOMString&   DOMServices::s_XMLNamespacePrefixURI = ::s_XMLNamespacePrefixURI;
const XalanDOMString    DOMServices::s_emptyString(XalanMemMgrs::getDummyMemMgr());



const XalanDOMString::size_type&    DOMServices::s_XMLStringLength = ::s_XMLStringLength;
const XalanDOMString::size_type&    DOMServices::s_XMLStringWithSeparatorLength = ::s_XMLStringWithSeparatorLength;
const XalanDOMString::size_type&    DOMServices::s_XMLNamespacePrefixLength = ::s_XMLNamespacePrefixLength;
const XalanDOMString::size_type&    DOMServices::s_XMLNamespaceURILength = ::s_XMLNamespaceURILength;
const XalanDOMString::size_type&    DOMServices::s_XMLNamespaceLength = ::s_XMLNamespaceLength;
const XalanDOMString::size_type&    DOMServices::s_XMLNamespaceWithSeparatorLength = ::s_XMLNamespaceWithSeparatorLength;
const XalanDOMString::size_type&    DOMServices::s_XMLNamespaceSeparatorStringLength = ::s_XMLNamespaceSeparatorStringLength;
const XalanDOMString::size_type&    DOMServices::s_XMLNamespacePrefixURILength = ::s_XMLNamespacePrefixURILength;


void
DOMServices::initialize(MemoryManager&  theManager)
{
    ::s_XMLString.reset( theManager, "xml");

    ::s_XMLStringWithSeparator.reset( theManager, "xml:" );
    ::s_XMLNamespacePrefix.reset( theManager, "xmlns:xml");
    ::s_XMLNamespaceURI.reset( theManager, "http://www.w3.org/XML/1998/namespace");
    ::s_XMLNamespace.reset( theManager, "xmlns");
    ::s_XMLNamespaceWithSeparator.reset( theManager, "xmlns:");
    ::s_XMLNamespaceSeparatorString.reset( theManager, ":");
    ::s_XMLNamespacePrefixURI.reset( theManager, "http://www.w3.org/2000/xmlns/");

    ::s_XMLStringLength = DOMServices::s_XMLString.length();
    ::s_XMLStringWithSeparatorLength = DOMServices::s_XMLStringWithSeparator.length();
    ::s_XMLNamespacePrefixLength = DOMServices::s_XMLNamespacePrefix.length();
    ::s_XMLNamespaceURILength = DOMServices::s_XMLNamespaceURI.length();
    ::s_XMLNamespaceLength = DOMServices::s_XMLNamespace.length();
    ::s_XMLNamespaceWithSeparatorLength = DOMServices::s_XMLNamespaceWithSeparator.length();
    ::s_XMLNamespaceSeparatorStringLength = DOMServices::s_XMLNamespaceSeparatorString.length();
    ::s_XMLNamespacePrefixURILength = DOMServices::s_XMLNamespacePrefixURI.length();
}



void
DOMServices::terminate()
{
    MemoryManager& theManager = XalanMemMgrs::getDummyMemMgr();

    releaseMemory(::s_XMLString, theManager );
    releaseMemory(::s_XMLStringWithSeparator, theManager );
    releaseMemory(::s_XMLNamespacePrefix, theManager );
    releaseMemory(::s_XMLNamespaceURI, theManager );
    releaseMemory(::s_XMLNamespace, theManager );
    releaseMemory(::s_XMLNamespaceWithSeparator, theManager );
    releaseMemory(::s_XMLNamespaceSeparatorString, theManager );
    releaseMemory(::s_XMLNamespacePrefixURI, theManager );

    ::s_XMLStringLength = 0;
    ::s_XMLStringWithSeparatorLength = 0;
    ::s_XMLNamespacePrefixLength = 0;
    ::s_XMLNamespaceURILength = 0;
    ::s_XMLNamespaceLength = 0;
    ::s_XMLNamespaceWithSeparatorLength = 0;
    ::s_XMLNamespaceSeparatorStringLength = 0;
    ::s_XMLNamespacePrefixURILength = 0;
}




void
DOMServices::getNodeData(
            const XalanNode&    node,
            XalanDOMString&     data)
{
    switch(node.getNodeType())
    {
    case XalanNode::DOCUMENT_FRAGMENT_NODE:
        {
            const XalanDocumentFragment&        theDocumentFragment =
                static_cast<const XalanDocumentFragment&>(node);

            getNodeData(theDocumentFragment, data);
        }
        break;

    case XalanNode::DOCUMENT_NODE:
        {
            const XalanDocument&    theDocument =
                static_cast<const XalanDocument&>(node);

            getNodeData(theDocument, data);
        }
        break;

    case XalanNode::ELEMENT_NODE:
        {
            const XalanElement&     theElement =
                static_cast<const XalanElement&>(node);

            getNodeData(theElement, data);
        }
        break;

    case XalanNode::TEXT_NODE:
    case XalanNode::CDATA_SECTION_NODE:
        {
            const XalanText&    theTextNode =
                static_cast<const XalanText&>(node);

                getNodeData(theTextNode, data);
        }
        break;

    case XalanNode::ATTRIBUTE_NODE:
        {
            const XalanAttr&        theAttr =
                static_cast<const XalanAttr&>(node);

            getNodeData(theAttr, data);
        }
        break;

    case XalanNode::COMMENT_NODE:
        {
            const XalanComment&     theComment =
                static_cast<const XalanComment&>(node);

            getNodeData(theComment, data);
        }
        break;

    case XalanNode::PROCESSING_INSTRUCTION_NODE:
        {
            const XalanProcessingInstruction&       thePI =
                static_cast<const XalanProcessingInstruction&>(node);

            getNodeData(thePI, data);
        }
        break;

    default:
        // ignore
        break;
    }
}




void
DOMServices::doGetNodeData(
            const XalanNode&    node,
            ExecutionContext&   executionContext,
            XalanDOMString&     data)
{
    assert(executionContext.hasPreserveOrStripSpaceConditions() == true);

    switch(node.getNodeType())
    {
    case XalanNode::DOCUMENT_FRAGMENT_NODE:
        {
            const XalanDocumentFragment&        theDocumentFragment =
                static_cast<const XalanDocumentFragment&>(node);

            doGetNodeData(theDocumentFragment, executionContext, data);
        }
        break;

    case XalanNode::DOCUMENT_NODE:
        {
            const XalanDocument&    theDocument =
                static_cast<const XalanDocument&>(node);

            doGetNodeData(theDocument, executionContext, data);
        }
        break;

    case XalanNode::ELEMENT_NODE:
        {
            const XalanElement&     theElement =
                static_cast<const XalanElement&>(node);

            doGetNodeData(theElement, executionContext, data);
        }
        break;

    case XalanNode::TEXT_NODE:
    case XalanNode::CDATA_SECTION_NODE:
        {
            const XalanText&    theTextNode =
                static_cast<const XalanText&>(node);

                doGetNodeData(theTextNode, executionContext, data);
        }
        break;

    case XalanNode::ATTRIBUTE_NODE:
        {
            const XalanAttr&        theAttr =
                static_cast<const XalanAttr&>(node);

            getNodeData(theAttr, data);
        }
        break;

    case XalanNode::COMMENT_NODE:
        {
            const XalanComment&     theComment =
                static_cast<const XalanComment&>(node);

            getNodeData(theComment, data);
        }
        break;

    case XalanNode::PROCESSING_INSTRUCTION_NODE:
        {
            const XalanProcessingInstruction&       thePI =
                static_cast<const XalanProcessingInstruction&>(node);

            getNodeData(thePI, data);
        }
        break;

    default:
        // ignore
        break;
    }
}




inline void
getChildData(
            const XalanNode*    child,
            XalanDOMString&     data)
{
    const XalanNode::NodeType   theType = child->getNodeType();

    if (theType == XalanNode::ELEMENT_NODE)
    {
        const XalanElement* const   theElementNode =
                static_cast<const XalanElement*>(child);

        DOMServices::getNodeData(*theElementNode, data);
    }
    else if (theType == XalanNode::TEXT_NODE ||
             theType == XalanNode::CDATA_SECTION_NODE)
    {
        const XalanText*    theTextNode =
                static_cast<const XalanText*>(child);

        DOMServices::getNodeData(*theTextNode, data);
    }
}



inline void
getChildData(
            const XalanNode*    child,
            ExecutionContext&   executionContext,
            XalanDOMString&     data)
{
    assert(executionContext.hasPreserveOrStripSpaceConditions() == true);

    const XalanNode::NodeType   theType = child->getNodeType();

    if (theType == XalanNode::ELEMENT_NODE)
    {
        const XalanElement* const   theElementNode =
                static_cast<const XalanElement*>(child);

        DOMServices::doGetNodeData(*theElementNode, executionContext, data);
    }
    else if (theType == XalanNode::TEXT_NODE ||
             theType == XalanNode::CDATA_SECTION_NODE)
    {
        const XalanText*    theTextNode =
                static_cast<const XalanText*>(child);

        DOMServices::doGetNodeData(*theTextNode, executionContext, data);
    }
}



inline void
getChildrenData(
            const XalanNode*    firstChild,
            XalanDOMString&     data)
{
    while(firstChild != 0)
    {
        getChildData(firstChild, data);

        firstChild = firstChild->getNextSibling();
    }
}



inline void
getChildrenData(
            const XalanNode*    firstChild,
            ExecutionContext&   executionContext,
            XalanDOMString&     data)
{
    assert(executionContext.hasPreserveOrStripSpaceConditions() == true);

    while(firstChild != 0)
    {
        getChildData(firstChild, executionContext, data);

        firstChild = firstChild->getNextSibling();
    }
}



void
DOMServices::getNodeData(
            const XalanDocument&    document,
            XalanDOMString&         data)
{
    assert(document.getDocumentElement() != 0);

    getChildrenData(document.getDocumentElement(), data);
}





void
DOMServices::doGetNodeData(
            const XalanDocument&    document,
            ExecutionContext&       executionContext,
            XalanDOMString&         data)
{
    assert(executionContext.hasPreserveOrStripSpaceConditions() == true);
    assert(document.getDocumentElement() != 0);

    getChildrenData(
        document.getDocumentElement(),
        executionContext,
        data);
}





void
DOMServices::getNodeData(
            const XalanDocumentFragment&    documentFragment,
            XalanDOMString&                 data)
{
    for(const XalanNode* child = documentFragment.getFirstChild(); child != 0; child = child->getNextSibling())
    {
        assert(child != 0);

        getChildData(child, data);
    }
}




void
DOMServices::doGetNodeData(
            const XalanDocumentFragment&    documentFragment,
            ExecutionContext&               executionContext,
            XalanDOMString&                 data)
{
    assert(executionContext.hasPreserveOrStripSpaceConditions() == true);

    for(const XalanNode* child = documentFragment.getFirstChild(); child != 0; child = child->getNextSibling())
    {
        assert(child != 0);

        getChildData(child, executionContext, data);
    }
}




void
DOMServices::getNodeData(
            const XalanElement&     element,
            XalanDOMString&         data)
{
    getChildrenData(element.getFirstChild(), data);
}



void
DOMServices::doGetNodeData(
            const XalanElement&     element,
            ExecutionContext&       executionContext,
            XalanDOMString&         data)
{
    assert(executionContext.hasPreserveOrStripSpaceConditions() == true);

    getChildrenData(element.getFirstChild(), executionContext, data);
}



void
DOMServices::getNodeData(
            const XalanNode&    node,
            FormatterListener&  formatterListener,
            MemberFunctionPtr   function)
{
    switch(node.getNodeType())
    {
    case XalanNode::DOCUMENT_FRAGMENT_NODE:
        {
            const XalanDocumentFragment&        theDocumentFragment =
                static_cast<const XalanDocumentFragment&>(node);

            getNodeData(theDocumentFragment, formatterListener, function);
        }
        break;

    case XalanNode::DOCUMENT_NODE:
        {
            const XalanDocument&    theDocument =
                static_cast<const XalanDocument&>(node);

            getNodeData(theDocument, formatterListener, function);
        }
        break;

    case XalanNode::ELEMENT_NODE:
        {
            const XalanElement&     theElement =
                static_cast<const XalanElement&>(node);

            getNodeData(theElement, formatterListener, function);
        }
        break;

    case XalanNode::TEXT_NODE:
    case XalanNode::CDATA_SECTION_NODE:
        {
            const XalanText&    theTextNode =
                static_cast<const XalanText&>(node);

                getNodeData(theTextNode, formatterListener, function);
        }
        break;

    case XalanNode::ATTRIBUTE_NODE:
        {
            const XalanAttr&        theAttr =
                static_cast<const XalanAttr&>(node);

            getNodeData(theAttr, formatterListener, function);
        }
        break;

    case XalanNode::COMMENT_NODE:
        {
            const XalanComment&     theComment =
                static_cast<const XalanComment&>(node);

            getNodeData(theComment, formatterListener, function);
        }
        break;

    case XalanNode::PROCESSING_INSTRUCTION_NODE:
        {
            const XalanProcessingInstruction&       thePI =
                static_cast<const XalanProcessingInstruction&>(node);

            getNodeData(thePI, formatterListener, function);
        }
        break;

    default:
        // ignore
        break;
    }
}



void
DOMServices::doGetNodeData(
            const XalanNode&    node,
            ExecutionContext&   executionContext,
            FormatterListener&  formatterListener,
            MemberFunctionPtr   function)
{
    assert(executionContext.hasPreserveOrStripSpaceConditions() == true);

    switch(node.getNodeType())
    {
    case XalanNode::DOCUMENT_FRAGMENT_NODE:
        {
            const XalanDocumentFragment&        theDocumentFragment =
                static_cast<const XalanDocumentFragment&>(node);

            doGetNodeData(theDocumentFragment, executionContext, formatterListener, function);
        }
        break;

    case XalanNode::DOCUMENT_NODE:
        {
            const XalanDocument&    theDocument =
                static_cast<const XalanDocument&>(node);

            doGetNodeData(theDocument, executionContext, formatterListener, function);
        }
        break;

    case XalanNode::ELEMENT_NODE:
        {
            const XalanElement&     theElement =
                static_cast<const XalanElement&>(node);

            doGetNodeData(theElement, executionContext, formatterListener, function);
        }
        break;

    case XalanNode::TEXT_NODE:
    case XalanNode::CDATA_SECTION_NODE:
        {
            const XalanText&    theTextNode =
                static_cast<const XalanText&>(node);

                doGetNodeData(theTextNode, executionContext, formatterListener, function);
        }
        break;

    case XalanNode::ATTRIBUTE_NODE:
        {
            const XalanAttr&        theAttr =
                static_cast<const XalanAttr&>(node);

            getNodeData(theAttr, formatterListener, function);
        }
        break;

    case XalanNode::COMMENT_NODE:
        {
            const XalanComment&     theComment =
                static_cast<const XalanComment&>(node);

            getNodeData(theComment, formatterListener, function);
        }
        break;

    case XalanNode::PROCESSING_INSTRUCTION_NODE:
        {
            const XalanProcessingInstruction&       thePI =
                static_cast<const XalanProcessingInstruction&>(node);

            getNodeData(thePI, formatterListener, function);
        }
        break;

    default:
        // ignore
        break;
    }
}



inline void
getChildData(
            const XalanNode*                child,
            FormatterListener&              formatterListener,
            DOMServices::MemberFunctionPtr  function)
{
    const XalanNode::NodeType   theType = child->getNodeType();

    if (theType == XalanNode::ELEMENT_NODE)
    {
        const XalanElement* const   theElementNode =
                static_cast<const XalanElement*>(child);

        DOMServices::getNodeData(*theElementNode, formatterListener, function);
    }
    else if (theType == XalanNode::TEXT_NODE ||
             theType == XalanNode::CDATA_SECTION_NODE)
    {
        const XalanText*    theTextNode =
                static_cast<const XalanText*>(child);

        DOMServices::getNodeData(*theTextNode, formatterListener, function);
    }
}



inline void
getChildData(
            const XalanNode*                child,
            ExecutionContext&               executionContext,
            FormatterListener&              formatterListener,
            DOMServices::MemberFunctionPtr  function)
{
    assert(executionContext.hasPreserveOrStripSpaceConditions() == true);

    const XalanNode::NodeType   theType = child->getNodeType();

    if (theType == XalanNode::ELEMENT_NODE)
    {
        const XalanElement* const   theElementNode =
                static_cast<const XalanElement*>(child);

        DOMServices::getNodeData(*theElementNode, executionContext, formatterListener, function);
    }
    else if (theType == XalanNode::TEXT_NODE ||
             theType == XalanNode::CDATA_SECTION_NODE)
    {
        const XalanText*    theTextNode =
                static_cast<const XalanText*>(child);

        DOMServices::getNodeData(*theTextNode, executionContext, formatterListener, function);
    }
}



inline void
getChildrenData(
            const XalanNode*                firstChild,
            FormatterListener&              formatterListener,
            DOMServices::MemberFunctionPtr  function)
{
    while(firstChild != 0)
    {
        getChildData(firstChild, formatterListener, function);

        firstChild = firstChild->getNextSibling();
    }
}



inline void
getChildrenData(
            const XalanNode*                firstChild,
            ExecutionContext&               executionContext,
            FormatterListener&              formatterListener,
            DOMServices::MemberFunctionPtr  function)
{
    assert(executionContext.hasPreserveOrStripSpaceConditions() == true);

    while(firstChild != 0)
    {
        getChildData(firstChild, executionContext, formatterListener, function);

        firstChild = firstChild->getNextSibling();
    }
}



void
DOMServices::getNodeData(
            const XalanDocument&    document,
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function)
{
    getChildrenData(
        document.getDocumentElement(),
        formatterListener,
        function);
}



void
DOMServices::doGetNodeData(
            const XalanDocument&    document,
            ExecutionContext&       executionContext,
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function)
{
    assert(executionContext.hasPreserveOrStripSpaceConditions() == true);

    getChildrenData(
        document.getDocumentElement(),
        executionContext,
        formatterListener,
        function);
}



void
DOMServices::getNodeData(
            const XalanDocumentFragment&    documentFragment,
            FormatterListener&              formatterListener,
            MemberFunctionPtr               function)
{
    for(const XalanNode* child = documentFragment.getFirstChild(); child != 0; child = child->getNextSibling())
    {
        assert(child != 0);

        getChildData(child, formatterListener, function);
    }
}



void
DOMServices::doGetNodeData(
            const XalanDocumentFragment&    documentFragment,
            ExecutionContext&               executionContext,
            FormatterListener&              formatterListener,
            MemberFunctionPtr               function)
{
    assert(executionContext.hasPreserveOrStripSpaceConditions() == true);

    for(const XalanNode* child = documentFragment.getFirstChild(); child != 0; child = child->getNextSibling())
    {
        assert(child != 0);

        getChildData(child, executionContext, formatterListener, function);
    }
}



void
DOMServices::getNodeData(
            const XalanElement&     element,
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function)
{
    getChildrenData(
        element.getFirstChild(),
        formatterListener,
        function);
}



void
DOMServices::doGetNodeData(
            const XalanElement&     element,
            ExecutionContext&       executionContext,
            FormatterListener&      formatterListener,
            MemberFunctionPtr       function)
{
    assert(executionContext.hasPreserveOrStripSpaceConditions() == true);

    getChildrenData(
        element.getFirstChild(),
        executionContext,
        formatterListener,
        function);
}



const XalanDOMString&
DOMServices::getNameOfNode(const XalanNode&     n)
{
    const XalanNode::NodeType   theNodeType =
                n.getNodeType();

    if (theNodeType == XalanNode::ATTRIBUTE_NODE)
    {
        return getNameOfNode(static_cast<const XalanAttr&>(n));
    }
    else if (theNodeType == XalanNode::ELEMENT_NODE)
    {
        return getNameOfNode(static_cast<const XalanElement&>(n));
    }
    else if (theNodeType == XalanNode::PROCESSING_INSTRUCTION_NODE)
    {
        return n.getNodeName();
    }
    else
    {
        return s_emptyString;
    }
}



const XalanDOMString&
DOMServices::getNamespaceOfNode(const XalanNode&    n)
{
    if (n.getNodeType() != XalanNode::ATTRIBUTE_NODE)
    {
        return n.getNamespaceURI();
    }
    else
    {
        const XalanDOMString&   theNodeName = n.getNodeName();

        // Special case for namespace nodes...
        if (startsWith(theNodeName, DOMServices::s_XMLNamespaceWithSeparator) == true ||
            equals(theNodeName, DOMServices::s_XMLNamespace) == true)
        {
            return s_emptyString;
        }
        else
        {
            return n.getNamespaceURI();
        }
    }
}



// Note functional overlap with NamespaceResolver's 
// getNamespaceOfNode() method.  
//
// ***** Also: although this may not yet impact Xalan,
// as of DOM Level 2 it is possible for a hand-constructed DOM to 
// have namespaced nodes with no declaration in scope. In DOM2 it's 
// considered the responsibility of application code such as DOM 
// serializers to recognize these cases and synthesize appropriate 
// declarations when necessary. DOM3 is expected to add 
// some form of namespaceNormalize() operation to assist this task.
//
// DOM3 may also add a resolveNamespacePrefix() operation 
// which is aware of these issues and can generate reasonable 
// results even for a non-NS-normalized tree. The expected logic
// is that a Namespaced node with a prefix will constitute an
// implicit declaration of that prefix. 
//
// If we cut over to DOM2 and want to accept DOMs from sources other
// than the parser, we need to decide between demanding a 
// namespace-normalized DOM as input, doing a normalize pass
// (full treewalk, expensive), or recognizing implicit declarations.
const XalanDOMString*
DOMServices::getNamespaceForPrefix(
            const XalanDOMString&   prefix,
            const XalanElement&     namespaceContext)
{
    const XalanDOMString*   theNamespace = 0;

    // Reserved xml: is hardcoded
    if(equals(prefix, s_XMLString) == true)
    {
        theNamespace = &s_XMLNamespaceURI;
    }
    else
    {
        const bool              isDefaultNamespace = prefix.empty();
        XalanNode::NodeType     type;
        const XalanNode*        parent = &namespaceContext;

        // Consider elements until NS is resolved, or we run out of
        // ancestors, or we hit something other than an Element or 
        // EntityReference node (ie, Document or DocumentFragment)
        while (parent != 0 && theNamespace == 0
            && ((type = parent->getNodeType()) == XalanNode::ELEMENT_NODE
                || type == XalanNode::ENTITY_REFERENCE_NODE)) 
        {
            if (type == XalanNode::ELEMENT_NODE) 
            {
                // Scan the attributes for xmlns:* or xmlns:
                // (The latter is the prefix=="" case.)
                const XalanNamedNodeMap* const  nnm = parent->getAttributes();
                assert(nnm != 0);

                const XalanSize_t   theLength = nnm->getLength();

                for (XalanSize_t i = 0;  i < theLength;  i ++) 
                {
                    const XalanNode* const  attr = nnm->item(i);
                    assert(attr != 0);

                    const XalanDOMString&   aname = attr->getNodeName();

                    if (isDefaultNamespace == true)
                    {
                        if (equals(aname, s_XMLNamespace) == true)
                        {
                            theNamespace = &attr->getNodeValue();

                            break;
                        }
                    }
                    else if (startsWith(aname, s_XMLNamespaceWithSeparator) == true) 
                    {
                        if (equals(
                                prefix,
                                aname.c_str() + s_XMLNamespaceWithSeparatorLength) == true)
                        {
                            theNamespace = &attr->getNodeValue();

                            break;
                        }
                    }
                }
            }

            parent = getParentOfNode(*parent);
        }
    }

    return theNamespace;
}



const XalanDOMString*
DOMServices::getNamespaceForPrefix(
            const XalanDOMChar*     theName,
            const PrefixResolver&   thePrefixResolver,
            bool                    isAttribute,
            XalanDOMString&         thePrefix)
{
    const XalanDOMString::size_type     theLength = length(theName);

    // Check for special default namespace value...
    if (isAttribute == true && equals(s_XMLNamespace, theName, theLength) == true)
    {
        return &s_XMLNamespacePrefixURI;
    }
    else
    {
        const XalanDOMString::size_type     theColonIndex = indexOf(theName, XalanUnicode::charColon);

        if (theColonIndex == theLength)
        {
            thePrefix.clear();

            if (isAttribute == true)
            {
                return 0;
            }
            else
            {
                return thePrefixResolver.getNamespaceForPrefix(s_emptyString);
            }
        }
        else
        {
            // Get the prefix from theName...
            thePrefix.assign(theName, theColonIndex);
            assert(thePrefix.empty() == false);

            return thePrefixResolver.getNamespaceForPrefix(thePrefix);
        }
    }
}



bool
DOMServices::isNodeAfter(
            const XalanNode&    node1,
            const XalanNode&    node2)
{
    assert(node1.getOwnerDocument() == node2.getOwnerDocument());
    assert(node1.getNodeType() != XalanNode::DOCUMENT_NODE &&
            node2.getNodeType() != XalanNode::DOCUMENT_NODE);

    if (node1.isIndexed() == true)
    {
        assert(node2.isIndexed() == true);

        return node1.getIndex() > node2.getIndex() ? true : false;
    }
    else
    {
        bool    isNodeAfter = false;

        const XalanNode*    parent1 = getParentOfNode(node1);

        const XalanNode*    parent2 = getParentOfNode(node2);

        // Optimize for most common case
        if (parent1 == parent2) // then we know they are siblings
        {
            isNodeAfter = isNodeAfterSibling(*parent1,
                                             node1,
                                             node2);
        }
        else
        {
            // General strategy: Figure out the lengths of the two 
            // ancestor chains, and walk up them looking for the 
            // first common ancestor, at which point we can do a 
            // sibling compare.  Edge condition where one is the 
            // ancestor of the other.

            // Count parents, so we can see if one of the chains 
            // needs to be equalized.
            XalanSize_t nParents1 = 2;
            XalanSize_t nParents2 = 2; // count node & parent obtained above

            while (parent1 != 0)
            {
                nParents1++;
                parent1 = getParentOfNode(*parent1);
            }

            while (parent2 != 0)
            {
                nParents2++;
                parent2 = getParentOfNode(*parent2);
            }

            // adjustable starting points
            const XalanNode*    startNode1 = &node1;
            const XalanNode*    startNode2 = &node2;

            // Do I have to adjust the start point in one of 
            // the ancesor chains?
            if (nParents1 < nParents2)
            {
                // adjust startNode2
                const XalanSize_t   adjust = nParents2 - nParents1;

                for (XalanSize_t i = 0; i < adjust; i++)
                {
                    startNode2 = getParentOfNode(*startNode2);
                }
            }
            else if(nParents1 > nParents2)
            {
                // adjust startNode1
                const XalanSize_t   adjust = nParents1 - nParents2;

                for (XalanSize_t i = 0; i < adjust; i++)
                {
                    startNode1 = getParentOfNode(*startNode1);
                }
            }

            // so we can "back up"
            const XalanNode*    prevChild1 = 0;
            const XalanNode*    prevChild2 = 0;
              
            // Loop up the ancestor chain looking for common parent.
            while (0 != startNode1)
            {
                if (startNode1 == startNode2) // common parent?
                {
                    if (0 == prevChild1) // first time in loop?
                    {
                        // Edge condition: one is the ancestor of the other.
                        isNodeAfter = (nParents1 < nParents2) ? true : false;

                        break; // from while loop
                    }
                    else
                    {
                        isNodeAfter = isNodeAfterSibling(*startNode1,
                                                         *prevChild1,
                                                         *prevChild2);

                        break; // from while loop
                    }
                }

                prevChild1 = startNode1;
                assert(prevChild1 != 0);

                startNode1 = getParentOfNode(*startNode1);
                assert(startNode1 != 0);

                prevChild2 = startNode2;
                assert(prevChild2 != 0);

                startNode2 = getParentOfNode(*startNode2);
                assert(startNode2 != 0);
            }
        }

        return isNodeAfter;
    }
}



bool
DOMServices::isNodeAfterSibling(
            const XalanNode&    parent,
            const XalanNode&    child1,
            const XalanNode&    child2)
{
    bool    isNodeAfterSibling = false;

    const XalanNode::NodeType   child1type = child1.getNodeType();
    const XalanNode::NodeType   child2type = child2.getNodeType();

    if (XalanNode::ATTRIBUTE_NODE != child1type &&
        XalanNode::ATTRIBUTE_NODE == child2type)
    {
        // always sort attributes before non-attributes.
        isNodeAfterSibling = true;
    }
    else if (XalanNode::ATTRIBUTE_NODE == child1type &&
             XalanNode::ATTRIBUTE_NODE != child2type)
    {
        // always sort attributes before non-attributes.
        isNodeAfterSibling = false;
    }
    else if (XalanNode::ATTRIBUTE_NODE == child1type)
    {
        const XalanNamedNodeMap*    children = parent.getAttributes();
      
        const XalanSize_t   nNodes = children->getLength();

        bool                found1 = false;
        bool                found2 = false;

        for (XalanSize_t i = 0; i < nNodes; i++)
        {
            const XalanNode*    child = children->item(i);

            if (&child1 == child)
            {
                if (found2 == true)
                {
                    isNodeAfterSibling = true;
                    break;
                }
          
                found1 = true;
            }
            else if (&child2 == child)
            {
                if (found1 == true)
                {
                    isNodeAfterSibling = false;
                    break;
                }
          
                found2 = true;
            }
        }
    }
    else
    {
        const XalanNode*    child = parent.getFirstChild();

        bool    found1 = false;
        bool    found2 = false;

        while (child != 0)
        {
            if (&child1 == child)
            {
                if (found2 == true)
                {
                    isNodeAfterSibling = true;
                    break;
                }

                found1 = true;
            }
            else if (&child2 == child)
            {
                if (found1 == true)
                {
                    isNodeAfterSibling = false;
                    break;
                }

                found2 = true;
            }

            child = child->getNextSibling();
        }

        assert(found1 != found2);
    }

    return isNodeAfterSibling;
}



XALAN_CPP_NAMESPACE_END
