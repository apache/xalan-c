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
#include "FormatterToSourceTree.hpp"



#include <cassert>



#include <xercesc/sax/AttributeList.hpp>



#include <xalanc/XalanDOM/XalanDOMException.hpp>
#include <xalanc/XalanDOM/XalanDocumentFragment.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/PrefixResolver.hpp>



#include "XalanSourceTreeComment.hpp"
#include "XalanSourceTreeDocument.hpp"
#include "XalanSourceTreeDocumentFragment.hpp"
#include "XalanSourceTreeElement.hpp"
#include "XalanSourceTreeHelper.hpp"
#include "XalanSourceTreeProcessingInstruction.hpp"
#include "XalanSourceTreeText.hpp"



XALAN_CPP_NAMESPACE_BEGIN



FormatterToSourceTree::FormatterToSourceTree(MemoryManager&         theManager,
                                             XalanSourceTreeDocument*   theDocument) :
    FormatterListener(OUTPUT_METHOD_DOM),
    m_document(theDocument),
    m_documentFragment(0),
    m_currentElement(0),
    m_elementStack(theManager),
    m_lastChild(0),
    m_lastChildStack(theManager),
    m_textBuffer(theManager)
{
}



FormatterToSourceTree::FormatterToSourceTree(
            XalanSourceTreeDocument*            theDocument,
            XalanSourceTreeDocumentFragment*    theDocumentFragment,
            MemoryManager&                  theManager) :
    FormatterListener(OUTPUT_METHOD_DOM),
    m_document(theDocument),
    m_documentFragment(theDocumentFragment),
    m_currentElement(0),
    m_elementStack(theManager),
    m_lastChild(0),
    m_lastChildStack(theManager),
    m_textBuffer(theManager)
{
    assert(m_document != 0);
    assert(m_documentFragment != 0);
}



FormatterToSourceTree::~FormatterToSourceTree()
{
}



void
FormatterToSourceTree::setDocumentLocator(const Locator* const  /* locator */)
{
}



void
FormatterToSourceTree::startDocument()
{
    m_currentElement = 0;

    m_elementStack.clear();

    m_lastChild = 0;

    m_lastChildStack.clear();

    m_lastChildStack.reserve(eDefaultStackSize);

    m_textBuffer.clear();

    m_textBuffer.reserve(eDefaultTextBufferSize);

    // Push a dummy value for the current element, so we
    // don't have to check for an empty stack in endElement().
    m_elementStack.push_back(ElementStackType::value_type(0));
}



void
FormatterToSourceTree::endDocument()
{
    if (m_documentFragment != 0)
    {
        processAccumulatedText();
    }

    // Pop off the dummy value that we pushed in 
    // startDocument()...
    m_elementStack.pop_back();

    assert(m_elementStack.empty() == true);
    assert(m_lastChildStack.empty() == true);
    assert(m_textBuffer.empty() == true);
}



// A helper function to manage appending the new child.
template <class ParentNodeType, class ChildNodeType>
inline void
doAppendChildNode(
            ParentNodeType*     theParent,
            XalanNode*&         theLastChild,
            ChildNodeType       theNewChild)
{
    assert(theParent != 0);
    assert(theNewChild != 0);

    if (theLastChild == 0)
    {
        theParent->appendChildNode(theNewChild);
    }
    else
    {
        XalanSourceTreeHelper::appendSibling(theLastChild, theNewChild);

        theNewChild->setParent(theParent);
    }

    theLastChild = theNewChild;
}



// A helper function to manage appending the new child.
template <class ChildNodeType>
inline void
doAppendChildNode(
            XalanSourceTreeDocument*            theDocument,
            XalanSourceTreeDocumentFragment*    theDocumentFragment,
            XalanSourceTreeElement*             theCurrentElement,
            XalanNode*&                         theLastChild,
            ChildNodeType                       theNewChild)
{
    assert(theDocument != 0);
    assert(theNewChild != 0);

    if (theCurrentElement == 0)
    {
        if (theDocumentFragment != 0)
        {
            doAppendChildNode(theDocumentFragment, theLastChild, theNewChild);
        }
        else
        {
            // If there is no current element. it means we haven't
            // created the document element yet, so always append
            // to the document, rather than the last child.
            theDocument->appendChildNode(theNewChild);
        }
    }
    else
    {
        doAppendChildNode(theCurrentElement, theLastChild, theNewChild);
    }
}



void
FormatterToSourceTree::startElement(
            const XMLCh* const  name,
            AttributeListType&  attrs)
{
    processAccumulatedText();

    XalanSourceTreeElement* const   theNewElement =
        createElementNode(name, attrs, m_currentElement);

    doAppendChildNode(
            m_document,
            m_documentFragment,
            m_currentElement,
            m_lastChild,
            theNewElement);

    m_elementStack.push_back(theNewElement);

    m_lastChildStack.push_back(m_lastChild);

    m_currentElement = theNewElement;

    m_lastChild = 0;
}



void
FormatterToSourceTree::endElement(const XMLCh* const    /* name */)
{
    processAccumulatedText();

    assert(m_elementStack.empty() == false);

    // Pop the element of the stack...
    m_elementStack.pop_back();

    assert(m_elementStack.empty() == false);

    // Get the element from the back of the
    // stack.
    m_currentElement = m_elementStack.back();

    assert(m_lastChildStack.empty() == false);

    m_lastChild = m_lastChildStack.back();

    // Pop the last child stack
    m_lastChildStack.pop_back();
}



void
FormatterToSourceTree::characters(
            const XMLCh* const  chars,
            const size_type     length)
{
    if (m_documentFragment != 0)
    {
        m_textBuffer.append(chars, length);
    }
    else if (m_currentElement == 0)
    {
        if (isXMLWhitespace(chars) == false)
        {
            throw XalanDOMException(XalanDOMException::HIERARCHY_REQUEST_ERR);
        }
    }
    else
    {
        m_textBuffer.append(chars, length);
    }
}



void
FormatterToSourceTree::charactersRaw(
        const XMLCh* const  chars,
        const size_type     length)
{
    assert(m_document != 0);

    processAccumulatedText();

    doProcessingInstruction(s_piTarget, s_piData);

    characters(chars, length);
}



void
FormatterToSourceTree::entityReference(const XMLCh* const   /* name */)
{
}



void
FormatterToSourceTree::ignorableWhitespace(
            const XMLCh* const  chars,
            const size_type     length)
{
    assert(m_document != 0);

    // Ignore any whitespace reported before the document element has been parsed.
    if (m_elementStack.size() > 1)
    {
        assert(m_documentFragment != 0 || m_document->getDocumentElement() != 0);

        processAccumulatedText();

        assert(m_currentElement != 0);

        doAppendChildNode(
            m_currentElement,
            m_lastChild,
            m_document->createTextIWSNode(chars, length, m_currentElement));
    }
    else if(m_documentFragment != 0)
    {
        processAccumulatedText();

        doAppendChildNode(
            m_documentFragment,
            m_lastChild,
            m_document->createTextIWSNode(chars, length, m_currentElement));
    }
}



void
FormatterToSourceTree::processingInstruction(
            const XMLCh* const  target,
            const XMLCh* const  data)
{
    assert(m_document != 0);

    processAccumulatedText();

    doProcessingInstruction(target, data);
}



void
FormatterToSourceTree::resetDocument()
{
}



void
FormatterToSourceTree::comment(const XMLCh* const   data)
{
    assert(m_document != 0);

    processAccumulatedText();

    XalanSourceTreeComment* const   theNewComment =
        m_document->createCommentNode(data, length(data), m_currentElement);

    doAppendChildNode(
            m_document,
            m_documentFragment,
            m_currentElement,
            m_lastChild,
            theNewComment);
}



void
FormatterToSourceTree::cdata(
            const XMLCh* const  /* ch */,
            const size_type     /* length */)
{
}



void
FormatterToSourceTree::processAccumulatedText()
{
    if (m_textBuffer.empty() == false)
    {
        doCharacters(
            m_textBuffer.c_str(),
            m_textBuffer.length());

        m_textBuffer.clear();
    }
}



void
FormatterToSourceTree::doCharacters(
            const XMLCh*    chars,
            size_type       length)
{
    if (m_currentElement != 0)
    {
        doAppendChildNode(
            m_currentElement,
            m_lastChild,
            m_document->createTextNode(chars, length, m_currentElement));
    }
    else if (m_documentFragment != 0)
    {
        doAppendChildNode(
            m_documentFragment,
            m_lastChild,
            m_document->createTextNode(chars, length, m_currentElement));
    }
    else
    {
        throw XalanDOMException(XalanDOMException::HIERARCHY_REQUEST_ERR);
    }
}



XalanSourceTreeElement*
FormatterToSourceTree::createElementNode(
            const XalanDOMChar*         name,
            AttributeListType&          attrs,
            XalanSourceTreeElement*     theParentElement)
{
    if (m_prefixResolver != 0)
    {
        return m_document->createElementNode(name, attrs, *m_prefixResolver, theParentElement);
    }
    else
    {
        return m_document->createElementNode(name, attrs, theParentElement);
    }
}



void
FormatterToSourceTree::doProcessingInstruction(
            const XMLCh*    target,
            const XMLCh*    data)
{
    doAppendChildNode(
            m_document,
            m_documentFragment,
            m_currentElement,
            m_lastChild,
            m_document->createProcessingInstructionNode(target, data));
}



XALAN_CPP_NAMESPACE_END
