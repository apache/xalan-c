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

#include "XalanSourceTreeContentHandler.hpp"



#include <xalanc/XalanDOM/XalanDOMException.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



#include "XalanSourceTreeDocument.hpp"
#include "XalanSourceTreeElement.hpp"
#include "XalanSourceTreeHelper.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XalanSourceTreeContentHandler::XalanSourceTreeContentHandler(
            MemoryManager&          theManager,
            XalanSourceTreeDocument*    theDocument,
            bool                        fAccumulateText) :
    ContentHandler(),
    DTDHandler(),
    LexicalHandler(),
    m_document(theDocument),
    m_currentElement(0),
    m_elementStack(theManager),
    m_lastChild(0),
    m_lastChildStack(theManager),
    m_accumulateText(fAccumulateText),
    m_textBuffer(theManager),
    m_inDTD(false)
{
}



XalanSourceTreeContentHandler::~XalanSourceTreeContentHandler()
{
}



void
XalanSourceTreeContentHandler::characters(
            const   XMLCh* const    chars,
            const size_type         length)
{
    assert(m_inDTD == false);

    if (m_currentElement == 0)
    {
        if (isXMLWhitespace(chars) == false)
        {
            throw XalanDOMException(XalanDOMException::HIERARCHY_REQUEST_ERR);
        }
    }
    else if (m_accumulateText == true)
    {
        m_textBuffer.append(chars, length);
    }
    else
    {
        doCharacters(chars, length);
    }
}



void
XalanSourceTreeContentHandler::endDocument()
{
    assert(m_inDTD == false);

    // Pop off the dummy value that we pushed in 
    // startDocument()...
    m_elementStack.pop_back();

    assert(m_document->getDocumentElement() != 0);

    assert(m_elementStack.empty() == true);
    assert(m_lastChildStack.empty() == true);

    assert(m_textBuffer.empty() == true);
}



void
XalanSourceTreeContentHandler::endElement(
            const XMLCh* const  /* uri */, 
            const XMLCh* const  /* localname */, 
            const XMLCh* const  /* qname */)
{
    assert(m_inDTD == false);

    // Process any text that we may have accumulated...
    processAccumulatedText();

    assert(m_elementStack.empty() == false);

    // Pop the stack...
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
    }

    theLastChild = theNewChild;
}



// A helper function to manage appending the new child.
template <class ChildNodeType>
inline void
doAppendChildNode(
            XalanSourceTreeDocument*    theDocument,
            XalanSourceTreeElement*     theCurrentElement,
            XalanNode*&                 theLastChild,
            ChildNodeType               theNewChild)
{
    assert(theDocument != 0);
    assert(theNewChild != 0);

    if (theCurrentElement == 0)
    {
        // If there is no current element. it means we haven't
        // created the document element yet, so always append
        // to the document, rather than the last child.
        theDocument->appendChildNode(theNewChild);
    }
    else
    {
        doAppendChildNode(theCurrentElement, theLastChild, theNewChild);
    }
}



void
XalanSourceTreeContentHandler::ignorableWhitespace(
            const XMLCh* const  chars,
            const size_type     length)
{
    assert(m_inDTD == false);

    // Ignore any whitespace reported before the document element has been parsed.
    if (m_elementStack.empty() == false)
    {
        assert(m_currentElement != 0);

        processAccumulatedText();

        XalanSourceTreeText*    theNewTextNode =
            m_document->createTextIWSNode(chars, length, m_currentElement);

        doAppendChildNode(m_currentElement, m_lastChild, theNewTextNode);
    }
}



void
XalanSourceTreeContentHandler::processingInstruction(
        const XMLCh* const  target,
        const XMLCh* const  data)
{
    assert(m_inDTD == false);

    processAccumulatedText();

    XalanSourceTreeProcessingInstruction* const     theNewPI =
        m_document->createProcessingInstructionNode(target, data, m_currentElement);

    doAppendChildNode(
            m_document,
            m_currentElement,
            m_lastChild,
            theNewPI);
}



void
XalanSourceTreeContentHandler::setDocumentLocator(const Locator* const  /* locator */)
{
}



void
XalanSourceTreeContentHandler::startDocument()
{
    assert(m_inDTD == false);

    m_currentElement = 0;

    m_elementStack.clear();

    m_elementStack.reserve(eDefaultStackSize);

    m_lastChild = 0;

    m_lastChildStack.clear();

    m_lastChildStack.reserve(eDefaultStackSize);

    if (m_accumulateText == true)
    {
        m_textBuffer.clear();

        m_textBuffer.reserve(eDefaultTextBufferSize);
    }

    // Push a dummy value for the current element, so we
    // don't have to check for an empty stack in endElement().
    m_elementStack.push_back(ElementStackType::value_type(0));
}



void
XalanSourceTreeContentHandler::startElement(
            const XMLCh* const      uri,
            const XMLCh* const      localname,
            const XMLCh* const      qname,
            const AttributesType&   attrs)
{
#if 0
    assert(m_inDTD == false);
#else
    if (m_inDTD == true)
    {
        m_inDTD = false;
    }
#endif

    processAccumulatedText();

    XalanSourceTreeElement* const   theNewElement =
        createElement(uri, localname, qname, attrs, m_currentElement);

    doAppendChildNode(
            m_document,
            m_currentElement,
            m_lastChild,
            theNewElement);

    m_elementStack.push_back(theNewElement);

    m_lastChildStack.push_back(m_lastChild);

    m_currentElement = theNewElement;

    m_lastChild = 0;
}



void
XalanSourceTreeContentHandler::startPrefixMapping(
        const XMLCh* const  /* prefix */,
        const XMLCh* const  /* uri */)
{
    assert(m_inDTD == false);
}



void
XalanSourceTreeContentHandler::endPrefixMapping(const XMLCh* const  /* prefix */)
{
    assert(m_inDTD == false);
}



void
XalanSourceTreeContentHandler::skippedEntity(const XMLCh* const     /* name */)
{
}



void
XalanSourceTreeContentHandler::notationDecl(
            const XMLCh* const    /* name */,
            const XMLCh* const    /* publicId */,
            const XMLCh* const    /* systemId */)
{
}



void
XalanSourceTreeContentHandler::unparsedEntityDecl(
            const XMLCh* const    name,
            const XMLCh* const    publicId,
            const XMLCh* const    systemId,
            const XMLCh* const    notationName)
{
    assert(m_document != 0);

    m_document->unparsedEntityDeclaration(name, publicId, systemId, notationName);
}



void
XalanSourceTreeContentHandler::resetDocType()
{
}



void
XalanSourceTreeContentHandler::comment(
            const XMLCh* const  chars,
            const size_type     length)
{
    assert(m_document != 0);

    if (m_inDTD == false)
    {
        processAccumulatedText();

        XalanSourceTreeComment* const   theNewComment =
            m_document->createCommentNode(chars, length, m_currentElement);

        doAppendChildNode(
                m_document,
                m_currentElement,
                m_lastChild,
                theNewComment);
    }
}



void
XalanSourceTreeContentHandler::endCDATA()
{
    assert(m_inDTD == false);
}



void
XalanSourceTreeContentHandler::endDTD()
{
    assert(m_document != 0);
//  assert(m_inDTD == true);

    m_inDTD = false;
}



void
XalanSourceTreeContentHandler::endEntity(const XMLCh* const     /* name */)
{
    assert(m_document != 0);
}


void
XalanSourceTreeContentHandler::startCDATA()
{
    assert(m_inDTD == false);
}



void
XalanSourceTreeContentHandler::startDTD(
            const XMLCh* const  /* name */,
            const XMLCh* const  /* publicId */,
            const XMLCh* const  /* systemId */)
{
    assert(m_inDTD == false);
    assert(m_document != 0);

    m_inDTD = true;
}



void
XalanSourceTreeContentHandler::startEntity(const XMLCh* const   /* name */)
{
    assert(m_document != 0);
}



void
XalanSourceTreeContentHandler::setDocument(XalanSourceTreeDocument* theDocument)
{
    m_document = theDocument;
}



XalanSourceTreeElement*
XalanSourceTreeContentHandler::createElement(
            const XMLCh* const          uri,
            const XMLCh* const          localname,
            const XMLCh* const          qname,
            const AttributesType&       attrs,
            XalanSourceTreeElement*     theOwnerElement)
{
    assert(m_inDTD == false);

    // If we're creating the document element, add the special xml namespace attribute...
    const bool  fAddXMLNamespaceAttribute = theOwnerElement == 0 ? true : false;

    if (length(uri) != 0)
    {
        return m_document->createElementNode(uri, localname, qname, attrs, theOwnerElement, 0, 0, fAddXMLNamespaceAttribute);
    }
    else
    {
        return m_document->createElementNode(qname, attrs, theOwnerElement, 0, 0, fAddXMLNamespaceAttribute);
    }
}



void
XalanSourceTreeContentHandler::processAccumulatedText()
{
    assert(m_inDTD == false);

    if (m_textBuffer.empty() == false)
    {
        doCharacters(
            m_textBuffer.c_str(),
            m_textBuffer.length());

        m_textBuffer.clear();
    }
}



void
XalanSourceTreeContentHandler::doCharacters(
            const XMLCh*    chars,
            size_type       length)
{
    assert(m_inDTD == false);

    assert(m_currentElement != 0);

    XalanSourceTreeText*    theNewTextNode = 
                m_document->createTextNode(chars, length, m_currentElement);

    doAppendChildNode(m_currentElement, m_lastChild, theNewTextNode);
}



XALAN_CPP_NAMESPACE_END
