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

#include "XalanSourceTreeHelper.hpp"



#include <cassert>



#include <xalanc/XalanDOM/XalanDOMException.hpp>



#include "XalanSourceTreeComment.hpp"
#include "XalanSourceTreeDocument.hpp"
#include "XalanSourceTreeDocumentFragment.hpp"
#include "XalanSourceTreeElement.hpp"
#include "XalanSourceTreeProcessingInstruction.hpp"
#include "XalanSourceTreeText.hpp"



XALAN_CPP_NAMESPACE_BEGIN



inline XalanSourceTreeComment*
castToComment(XalanNode*    theNode)
{
    return static_cast<XalanSourceTreeComment*>(theNode);
}



inline XalanSourceTreeElement*
castToElement(XalanNode*    theNode)
{
    return static_cast<XalanSourceTreeElement*>(theNode);
}



inline XalanSourceTreeProcessingInstruction*
castToProcessingInstruction(XalanNode*  theNode)
{
    return static_cast<XalanSourceTreeProcessingInstruction*>(theNode);
}



inline XalanSourceTreeText*
castToText(XalanNode*   theNode)
{
    return static_cast<XalanSourceTreeText*>(theNode);
}



template<class NodeType>
inline void
doAppendSibling(
            XalanNode*  theSibling,
            NodeType*   theNewSibling)
{
    assert(theSibling != 0);
    assert(theNewSibling != 0);

    XalanNode* const    theLastSibling = XalanSourceTreeHelper::getLastSibling(theSibling);

    switch(theLastSibling->getNodeType())
    {
    case XalanNode::COMMENT_NODE:
        castToComment(theLastSibling)->appendSiblingNode(theNewSibling);
        break;

    case XalanNode::ELEMENT_NODE:
        castToElement(theLastSibling)->appendSiblingNode(theNewSibling);
        break;

    case XalanNode::PROCESSING_INSTRUCTION_NODE:
        castToProcessingInstruction(theLastSibling)->appendSiblingNode(theNewSibling);
        break;

    case XalanNode::TEXT_NODE:
        castToText(theLastSibling)->appendSiblingNode(theNewSibling);
        break;

    default:
        throw XalanDOMException(XalanDOMException::HIERARCHY_REQUEST_ERR);
        break;
    }
}



template<class NodeType>
inline void
doAppendToLastSibling(
            XalanNode*  theLastSibling,
            NodeType*   theNewSibling)
{
    assert(theLastSibling != 0);
    assert(theNewSibling != 0);
    assert(theLastSibling->getNextSibling() == 0);

    switch(theLastSibling->getNodeType())
    {
    case XalanNode::COMMENT_NODE:
        castToComment(theLastSibling)->appendSiblingNode(theNewSibling);
        break;

    case XalanNode::ELEMENT_NODE:
        castToElement(theLastSibling)->appendSiblingNode(theNewSibling);
        break;

    case XalanNode::PROCESSING_INSTRUCTION_NODE:
        castToProcessingInstruction(theLastSibling)->appendSiblingNode(theNewSibling);
        break;

    case XalanNode::TEXT_NODE:
        castToText(theLastSibling)->appendSiblingNode(theNewSibling);
        break;

    default:
        throw XalanDOMException(XalanDOMException::HIERARCHY_REQUEST_ERR);
        break;
    }
}



template <class NodeType1, class NodeType2>
inline void
append(
            NodeType1*      thePreviousSibling,
            XalanNode*&     theNextSiblingSlot,
            NodeType2*      theNewSibling)
{
    if (theNextSiblingSlot == 0)
    {
        theNextSiblingSlot = theNewSibling;

        if (thePreviousSibling != 0)
        {
            theNewSibling->setPreviousSibling(thePreviousSibling);
        }
    }
    else
    {
        doAppendSibling(theNextSiblingSlot, theNewSibling);
    }
}



template <class NodeType>
inline void
append(
            XalanNode*&     theNextSiblingSlot,
            NodeType*       theNewSibling)
{
    if (theNextSiblingSlot == 0)
    {
        theNextSiblingSlot = theNewSibling;
    }
    else
    {
        doAppendSibling(theNextSiblingSlot, theNewSibling);
    }
}



template <class NodeType>
inline void
doAppendSibling(
            NodeType*       thePreviousSibling,
            XalanNode*&     theNextSiblingSlot,
            XalanNode*      theNewSibling)
{
    assert(theNewSibling != 0);
    assert(theNextSiblingSlot == 0 || theNextSiblingSlot->getParentNode() == theNewSibling->getParentNode());

    switch(theNewSibling->getNodeType())
    {
    case XalanNode::COMMENT_NODE:
        append(thePreviousSibling, theNextSiblingSlot, castToComment(theNewSibling));
        break;

    case XalanNode::ELEMENT_NODE:
        append(thePreviousSibling, theNextSiblingSlot, castToElement(theNewSibling));
        break;

    case XalanNode::PROCESSING_INSTRUCTION_NODE:
        append(thePreviousSibling, theNextSiblingSlot, castToProcessingInstruction(theNewSibling));
        break;

    case XalanNode::TEXT_NODE:
        append(thePreviousSibling, theNextSiblingSlot, castToText(theNewSibling));
        break;

    default:
        throw XalanDOMException(XalanDOMException::HIERARCHY_REQUEST_ERR);
        break;
    }
}



void
XalanSourceTreeHelper::appendSibling(
            XalanSourceTreeDocument*    theDocument,
            XalanNode*&                 theNextSiblingSlot,
            XalanNode*                  theNewSibling)
{
    if (theNewSibling->getOwnerDocument() != theDocument)
    {
        throw XalanDOMException(XalanDOMException::WRONG_DOCUMENT_ERR);
    }
    else
    {
        switch(theNewSibling->getNodeType())
        {
        case XalanNode::COMMENT_NODE:
            append(theNextSiblingSlot, castToComment(theNewSibling));
            break;

        case XalanNode::ELEMENT_NODE:
            append(theNextSiblingSlot, castToElement(theNewSibling));
            break;

        case XalanNode::PROCESSING_INSTRUCTION_NODE:
            append(theNextSiblingSlot, castToProcessingInstruction(theNewSibling));
            break;

        default:
            throw XalanDOMException(XalanDOMException::HIERARCHY_REQUEST_ERR);
            break;
        }
    }
}



void
XalanSourceTreeHelper::appendSibling(
            XalanSourceTreeComment*     theNode,
            XalanNode*&                 theNextSiblingSlot,
            XalanNode*                  theNewSibling)
{
    doAppendSibling(theNode, theNextSiblingSlot, theNewSibling);
}



void
XalanSourceTreeHelper::appendSibling(
            XalanSourceTreeElement*     theNode,
            XalanNode*&                 theNextSiblingSlot,
            XalanNode*                  theNewSibling)
{
    doAppendSibling(theNode, theNextSiblingSlot, theNewSibling);
}



void
XalanSourceTreeHelper::appendSibling(
            XalanSourceTreeProcessingInstruction*   theNode,
            XalanNode*&                             theNextSiblingSlot,
            XalanNode*                              theNewSibling)
{
    doAppendSibling(theNode, theNextSiblingSlot, theNewSibling);
}



void
XalanSourceTreeHelper::appendSibling(
            XalanSourceTreeText*    theNode,
            XalanNode*&             theNextSiblingSlot,
            XalanNode*              theNewSibling)
{
    doAppendSibling(theNode, theNextSiblingSlot, theNewSibling);
}



void
XalanSourceTreeHelper::appendSibling(
            XalanNode*                  theLastSibling,
            XalanSourceTreeComment*     theNewLastSibling)
{
    doAppendToLastSibling(theLastSibling, theNewLastSibling);
}



void
XalanSourceTreeHelper::appendSibling(
            XalanNode*                  theLastSibling,
            XalanSourceTreeElement*     theNewLastSibling)
{
    doAppendToLastSibling(theLastSibling, theNewLastSibling);
}



void
XalanSourceTreeHelper::appendSibling(
            XalanNode*                              theLastSibling,
            XalanSourceTreeProcessingInstruction*   theNewLastSibling)
{
    doAppendToLastSibling(theLastSibling, theNewLastSibling);
}



void
XalanSourceTreeHelper::appendSibling(
            XalanNode*              theLastSibling,
            XalanSourceTreeText*    theNewLastSibling)
{
    doAppendToLastSibling(theLastSibling, theNewLastSibling);
}



XalanNode*
doGetLastSibling(XalanNode*     theNode)
{
    if (theNode == 0)
    {
        return 0;
    }
    else
    {
        XalanNode*  theNextSibling = theNode->getNextSibling();

        while(theNextSibling != 0)
        {
            theNode = theNextSibling;

            theNextSibling = theNode->getNextSibling();
        }

        return theNode;
    }
}



template <class NodeType>
void
doAppendSiblingToChild(
            XalanSourceTreeElement*     theOwnerElement,
            XalanNode*&                 theFirstChildSlot,
            NodeType*                   theNewSibling)
{
    assert(theOwnerElement != 0);
    assert(theNewSibling != 0);

    if (theNewSibling->getParentNode() != theOwnerElement)
    {
        theNewSibling->setParent(theOwnerElement);
    }

    if (theFirstChildSlot == 0)
    {
        append(theFirstChildSlot, theNewSibling);
    }
    else
    {
        XalanNode* const    theLastSibling = doGetLastSibling(theFirstChildSlot);

        doAppendSibling(theLastSibling, theNewSibling); 
    }
}



template <class NodeType>
void
doAppendSiblingToChild(
            XalanSourceTreeDocumentFragment*    theOwnerDocumentFragment,
            XalanNode*&                         theFirstChildSlot,
            NodeType*                           theNewSibling)
{
    assert(theOwnerDocumentFragment != 0);
    assert(theNewSibling != 0);

    if (theNewSibling->getParentNode() != theOwnerDocumentFragment)
    {
        theNewSibling->setParent(theOwnerDocumentFragment);
    }

    if (theFirstChildSlot == 0)
    {
        append(theFirstChildSlot, theNewSibling);
    }
    else
    {
        XalanNode* const    theLastSibling = doGetLastSibling(theFirstChildSlot);

        doAppendSibling(theLastSibling, theNewSibling); 
    }
}



void
XalanSourceTreeHelper::appendSiblingToChild(
            XalanSourceTreeElement*     theOwnerElement,
            XalanNode*&                 theFirstChildSlot,
            XalanSourceTreeComment*     theNewSibling)
{
    doAppendSiblingToChild(theOwnerElement, theFirstChildSlot, theNewSibling);
}



void
XalanSourceTreeHelper::appendSiblingToChild(
            XalanSourceTreeElement*     theOwnerElement,
            XalanNode*&                 theFirstChildSlot,
            XalanSourceTreeElement*     theNewSibling)
{
    doAppendSiblingToChild(theOwnerElement, theFirstChildSlot, theNewSibling);
}



void
XalanSourceTreeHelper::appendSiblingToChild(
            XalanSourceTreeElement*                 theOwnerElement,
            XalanNode*&                             theFirstChildSlot,
            XalanSourceTreeProcessingInstruction*   theNewSibling)
{
    doAppendSiblingToChild(theOwnerElement, theFirstChildSlot, theNewSibling);
}



void
XalanSourceTreeHelper::appendSiblingToChild(
            XalanSourceTreeElement*     theOwnerElement,
            XalanNode*&                 theFirstChildSlot,
            XalanSourceTreeText*        theNewSibling)
{
    doAppendSiblingToChild(theOwnerElement, theFirstChildSlot, theNewSibling);
}



void
XalanSourceTreeHelper::appendSiblingToChild(
            XalanSourceTreeDocumentFragment*    theOwnerDocumentFragment,
            XalanNode*&                         theFirstChildSlot,
            XalanSourceTreeComment*             theNewSibling)
{
    doAppendSiblingToChild(theOwnerDocumentFragment, theFirstChildSlot, theNewSibling);
}



void
XalanSourceTreeHelper::appendSiblingToChild(
            XalanSourceTreeDocumentFragment*    theOwnerDocumentFragment,
            XalanNode*&                         theFirstChildSlot,
            XalanSourceTreeElement*             theNewSibling)
{
    doAppendSiblingToChild(theOwnerDocumentFragment, theFirstChildSlot, theNewSibling);
}



void
XalanSourceTreeHelper::appendSiblingToChild(
            XalanSourceTreeDocumentFragment*        theOwnerDocumentFragment,
            XalanNode*&                             theFirstChildSlot,
            XalanSourceTreeProcessingInstruction*   theNewSibling)
{
    doAppendSiblingToChild(theOwnerDocumentFragment, theFirstChildSlot, theNewSibling);
}



void
XalanSourceTreeHelper::appendSiblingToChild(
            XalanSourceTreeDocumentFragment*    theOwnerDocumentFragment,
            XalanNode*&                         theFirstChildSlot,
            XalanSourceTreeText*                theNewSibling)
{
    doAppendSiblingToChild(theOwnerDocumentFragment, theFirstChildSlot, theNewSibling);
}



XalanNode*
XalanSourceTreeHelper::getLastSibling(XalanNode*    theNode)
{
    return doGetLastSibling(theNode);
}



XALAN_CPP_NAMESPACE_END
