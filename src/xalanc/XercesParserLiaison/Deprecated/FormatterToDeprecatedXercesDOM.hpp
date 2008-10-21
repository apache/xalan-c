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
#if !defined(FORMATTERTODEPRECATEDXERCESDOM_HEADER_GUARD_1357924680)
#define FORMATTERTODEPRECATEDXERCESDOM_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



#include <xalanc/Include/XalanVector.hpp>



// Base class header file.
#include <xalanc/PlatformSupport/FormatterListener.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/XercesParserLiaison/Deprecated/XercesBridgeTypes.hpp>

// Required Xerces headers

#if XERCES_VERSION_MAJOR >= 2
#include <xercesc/dom/deprecated/DOM_Document.hpp>
#include <xercesc/dom/deprecated/DOM_DocumentFragment.hpp>
#include <xercesc/dom/deprecated/DOM_Element.hpp>
#else
#include <xercesc/dom/DOM_Document.hpp>
#include <xercesc/dom/DOM_DocumentFragment.hpp>
#include <xercesc/dom/DOM_Element.hpp>
#endif


XALAN_CPP_NAMESPACE_BEGIN



/**
 * This class takes SAX events (in addition to some extra events that SAX
 * doesn't handle yet) and adds the result to a document or document fragment.
 *
 * It is provided as assist developers transition away from the deprecated DOM.
 *
 * @deprecated The Xerces DOM bridge is deprecated.
 */
class XALAN_XERCESPARSERLIAISON_EXPORT FormatterToDeprecatedXercesDOM : public FormatterListener
{
public:

    /**
     * Construct a FormatterToDeprecatedXercesDOM instance.  it will add the DOM nodes 
     * to the document fragment.
     *
     * @deprecated This API is deprecated and is only provided as a transition tool
     * @param doc            document for nodes
     * @param docFrag        document fragment for nodes
     * @param currentElement current element for nodes
     */
    FormatterToDeprecatedXercesDOM(
            DOM_Document_Type           &doc,
            DOM_DocumentFragmentType    &docFrag,
            DOM_ElementType             &currentElement);

    /**
     * Construct a FormatterToDeprecatedXerces DOM instance.  it will add the DOM nodes 
     * to the document.
     *
     * @deprecated This API is deprecated and is only provided as a transition tool
     * @param doc  document for nodes
     * @param elem current element for nodes
     */
    FormatterToDeprecatedXercesDOM(
            DOM_Document_Type           &doc,
            DOM_ElementType             &currentElement);

    /**
     * Construct a FormatterToDeprecatedXercesDOM instance.  it will add the DOM nodes 
     * to the document.
     *
     * @deprecated This API is deprecated and is only provided as a transition tool
     * @param doc  document for nodes
     */
    FormatterToDeprecatedXercesDOM(
            DOM_Document_Type           &doc);

    virtual
    ~FormatterToDeprecatedXercesDOM();


    // These methods are inherited from DocumentHandler ...

    virtual void
    charactersRaw(
            const XMLCh* const  chars,
            const unsigned int  length);

    virtual void
    comment(const XMLCh* const  data);

    virtual void
    cdata(
            const XMLCh* const  ch,
            const unsigned int  length);

    virtual void
    entityReference(const XMLCh* const  name);

    virtual void
    setDocumentLocator(const Locator* const     locator);

    virtual void
    startDocument();

    virtual void
    endDocument();

    virtual void
    startElement(
                const   XMLCh* const    name,
                AttributeListType&      attrs);

    virtual void
    endElement(const XMLCh* const   name);

    virtual void
    characters(
                const XMLCh* const  chars,
                const unsigned int  length);

    virtual void
    ignorableWhitespace(
                const XMLCh* const  chars,
                const unsigned int  length);

    virtual void
    processingInstruction(
            const XMLCh* const  target,
            const XMLCh* const  data);

    virtual void
    resetDocument();

    DOM_Document_Type
    getDocument() const
    {
        return m_doc;
    }

    void
    setDocument(DOM_Document_Type   &theDocument)
    {
        m_doc = theDocument;
    }

    DOM_DocumentFragmentType
    getDocumentFragment() const
    {
        return m_docFrag;
    }

    void
    setDocumentFragment(DOM_DocumentFragmentType    &theDocumentFragment)
    {
        m_docFrag = theDocumentFragment;
    }

    DOM_ElementType
    getCurrentElement() const
    {
        return m_currentElem;
    }

    void
    setCurrentElement(DOM_ElementType       &theElement)
    {
        m_currentElem = theElement;
    }

private:

    /**
     * Process any accumulated text and create a node for it.
     */
    void
    processAccumulatedText();

    /**
     * Append a node to the current container.
     */
    void
    append(DOM_NodeType     &newNode);

    /**
     * Create the appropriate element, complete with attributes set.
     *
     * @param theElementName The name for the new element
     * @param attrs The SAX AttributeList for the new attributes.
     * @return A pointer to the new instance.
     */
    DOM_ElementType
    createElement(
            const XalanDOMChar*     theElementName,
            AttributeListType&      attrs);

    void
    addAttributes(
            DOM_ElementType         &theElement,
            AttributeListType&  attrs);


    // Data members...
    DOM_Document_Type               m_doc;

    DOM_DocumentFragmentType        m_docFrag;

    DOM_ElementType                 m_currentElem;

    typedef XalanVector<DOM_ElementType>            ElementStackType;

    ElementStackType                m_elemStack;

    XalanDOMString                  m_buffer;

    XalanDOMString                  m_textBuffer;

    static const XalanDOMString     s_emptyString;
};



XALAN_CPP_NAMESPACE_END



#endif  // FORMATTERTODEPRECATEDXERCESDOM_HEADER_GUARD_1357924680
