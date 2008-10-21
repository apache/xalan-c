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
#include "ElemValueOf.hpp"



#include <xercesc/sax/AttributeList.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/XPath.hpp>



#include "Constants.hpp"
#include "SelectionEvent.hpp"
#include "Stylesheet.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"
#include "StylesheetRoot.hpp"



XALAN_CPP_NAMESPACE_BEGIN



ElemValueOf::ElemValueOf(
            StylesheetConstructionContext&  constructionContext,
            Stylesheet&                     stylesheetTree,
            const AttributeListType&        atts,
            XalanFileLoc                    lineNumber, 
            XalanFileLoc                    columnNumber) :
    ElemTemplateElement(
        constructionContext,
        stylesheetTree,
        lineNumber,
        columnNumber,
        StylesheetConstructionContext::ELEMNAME_VALUE_OF),
    m_selectPattern(0)
{
    bool    isSelectCurrentNode = false;

    const XalanSize_t  nAttrs = atts.getLength();

    for (XalanSize_t i = 0; i < nAttrs; i++)
    {
        const XalanDOMChar* const   aname = atts.getName(i);

        if (equals(aname, Constants::ATTRNAME_SELECT))
        {
            const XalanDOMChar* const   avalue = atts.getValue(i);
            assert(avalue != 0);

            if (avalue[0] == XalanUnicode::charFullStop && avalue[1] == 0)
            {
                isSelectCurrentNode = true;
            }
            else
            {
                m_selectPattern =
                        constructionContext.createXPath(
                            getLocator(),
                            avalue,
                            *this);
            }
        }
        else if (equals(aname, Constants::ATTRNAME_DISABLE_OUTPUT_ESCAPING))
        {
            disableOutputEscaping(
                getStylesheet().getYesOrNo(
                    aname,
                    atts.getValue(i),
                    constructionContext));
        }
        else if (isAttrOK(
                    aname,
                    atts,
                    i,
                    constructionContext) == false &&
                 processSpaceAttr(
                    Constants::ELEMNAME_VALUEOF_WITH_PREFIX_STRING.c_str(),
                    aname,
                    atts,
                    i,
                    constructionContext) == false)
        {
            error(
                constructionContext,
                XalanMessages::ElementHasIllegalAttribute_2Param,
                Constants::ELEMNAME_VALUEOF_WITH_PREFIX_STRING.c_str(),
                aname);
        }
    }

    if (isSelectCurrentNode == false && m_selectPattern == 0)
    {
        error(
            constructionContext,
            XalanMessages::ElementRequiresAttribute_2Param,
            Constants::ELEMNAME_VALUEOF_WITH_PREFIX_STRING,
            Constants::ATTRNAME_SELECT);
    }
}



ElemValueOf::~ElemValueOf()
{
}



const XalanDOMString&
ElemValueOf::getElementName() const
{
    return Constants::ELEMNAME_VALUEOF_WITH_PREFIX_STRING;
}



class FormatterListenerAdapater : public FormatterListener
{
public:

    FormatterListenerAdapater(StylesheetExecutionContext&   executionContext) :
        FormatterListener(OUTPUT_METHOD_NONE),
        m_executionContext(executionContext)
    {
    }

    ~FormatterListenerAdapater()
    {
    }

    void
    setDocumentLocator(const Locator* const     /* locator */)
    {
    }

    void
    startDocument()
    {
    }

    void
    endDocument()
    {
    }

    void
    startElement(
                const   XMLCh* const    /* name */,
                AttributeListType&      /* attrs */)
    {
    }

    void
    endElement(const    XMLCh* const    /* name */)
    {
    }

    void
    characters(
                const XMLCh* const  chars,
                const size_type     length)
    {
        m_executionContext.characters(chars, 0, length);
    }

    void
    charactersRaw(
            const XMLCh* const  chars,
            const size_type     length)
    {
        m_executionContext.charactersRaw(chars, 0, length);
    }

    void
    entityReference(const XMLCh* const  /* name */)
    {
    }

    void
    ignorableWhitespace(
                const XMLCh* const  /* chars */,
                const size_type     /* length */)
    {
    }

    void
    processingInstruction(
                const XMLCh* const  target,
                const XMLCh* const  data)
    {
        m_executionContext.processingInstruction(target, data);
    }

    void
    resetDocument()
    {
    }

    void
    comment(const XMLCh* const  data)
    {
        m_executionContext.comment(data);
    }

    void
    cdata(
                const XMLCh* const  /* ch */,
                const size_type     /* length */)
    {
    }

private:

    StylesheetExecutionContext&     m_executionContext;

};



#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
const ElemTemplateElement*
ElemValueOf::startElement(StylesheetExecutionContext&       executionContext) const
{
    ElemTemplateElement::startElement(executionContext);

    XalanNode* const    sourceNode = executionContext.getCurrentNode();
    assert(sourceNode != 0);

    if (m_selectPattern == 0)
    {
        if (disableOutputEscaping() == false)
        {
            executionContext.characters(*sourceNode);
        }
        else
        {
            executionContext.charactersRaw(*sourceNode);
        }

        if (0 != executionContext.getTraceListeners())
        {
            const StylesheetExecutionContext::GetCachedString   theString(executionContext);

            DOMServices::getNodeData(*sourceNode, executionContext, theString.get());

            fireSelectionEvent(executionContext, sourceNode, theString.get());
        }
    }
    else
    {
        FormatterListenerAdapater   theAdapter(executionContext);

        XPath::MemberFunctionPtr    theFunction = disableOutputEscaping() == false ?
            &FormatterListener::characters : &FormatterListener::charactersRaw;

        m_selectPattern->execute(*this, executionContext, theAdapter, theFunction);

        if (0 != executionContext.getTraceListeners())
        {
            const XObjectPtr    value(m_selectPattern->execute(sourceNode, *this, executionContext));

            if (value.null() == false)
            {
                fireSelectionEvent(executionContext, sourceNode, value);
            }
        }
    }

    return 0;
}
#endif



#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
void
ElemValueOf::execute(StylesheetExecutionContext&    executionContext) const
{
    ElemTemplateElement::execute(executionContext);

    XalanNode* const    sourceNode = executionContext.getCurrentNode();
    assert(sourceNode != 0);

    if (m_selectPattern == 0)
    {
        if (disableOutputEscaping() == false)
        {
            executionContext.characters(*sourceNode);
        }
        else
        {
            executionContext.charactersRaw(*sourceNode);
        }

        if (0 != executionContext.getTraceListeners())
        {
            const StylesheetExecutionContext::GetCachedString   theString(executionContext);

            DOMServices::getNodeData(*sourceNode, executionContext, theString.get());

            fireSelectionEvent(executionContext, sourceNode, theString.get());
        }
    }
    else
    {
        FormatterListenerAdapater   theAdapter(executionContext);

        XPath::MemberFunctionPtr    theFunction = disableOutputEscaping() == false ?
            &FormatterListener::characters : &FormatterListener::charactersRaw;

        m_selectPattern->execute(*this, executionContext, theAdapter, theFunction);

        if (0 != executionContext.getTraceListeners())
        {
            const XObjectPtr    value(m_selectPattern->execute(sourceNode, *this, executionContext));

            if (value.null() == false)
            {
                fireSelectionEvent(executionContext, sourceNode, value);
            }
        }
    }
}
#endif



const XPath*
ElemValueOf::getXPath(XalanSize_t   index) const
{
    return index == 0 ? m_selectPattern : 0;
}



void
ElemValueOf::fireSelectionEvent(
            StylesheetExecutionContext&     executionContext,
            XalanNode*                      sourceNode,
            const XalanDOMString&           theValue) const
{
    const XObjectPtr value(executionContext.getXObjectFactory().createStringReference(theValue));

    fireSelectionEvent(executionContext, sourceNode, value);
}



void
ElemValueOf::fireSelectionEvent(
            StylesheetExecutionContext&     executionContext,
            XalanNode*                      sourceNode,
            const XObjectPtr                theValue) const
{
    if (m_selectPattern != 0)
    {
        fireSelectionEvent(
            executionContext,
            sourceNode,
            theValue,
            m_selectPattern->getExpression().getCurrentPattern());
    }
    else
    {
        const StylesheetExecutionContext::GetCachedString   thePattern(executionContext);

        thePattern.get() = ".";

        fireSelectionEvent(
            executionContext,
            sourceNode,
            theValue,
            thePattern.get());
    }

}



void
ElemValueOf::fireSelectionEvent(
            StylesheetExecutionContext&     executionContext,
            XalanNode*                      sourceNode,
            const XObjectPtr                theValue,
            const XalanDOMString&           thePattern) const
{
    executionContext.fireSelectEvent(
        SelectionEvent(
            executionContext,
            sourceNode,
            *this,
            XalanDOMString("select", executionContext.getMemoryManager()),
            thePattern,
            theValue));
}



XALAN_CPP_NAMESPACE_END
