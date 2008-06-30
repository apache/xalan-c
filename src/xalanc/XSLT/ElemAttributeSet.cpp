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
#include "ElemAttributeSet.hpp"



#include <xercesc/sax/AttributeList.hpp>



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include "Constants.hpp"
#include "Stylesheet.hpp"
#include "StylesheetRoot.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



ElemAttributeSet::ElemAttributeSet(
            StylesheetConstructionContext&  constructionContext,
            Stylesheet&                     stylesheetTree,
            const AttributeListType&        atts,
            XalanFileLoc                    lineNumber,
            XalanFileLoc                    columnNumber) :
    ElemUse(constructionContext,
            stylesheetTree,
            lineNumber,
            columnNumber,
            StylesheetConstructionContext::ELEMNAME_ATTRIBUTE_SET),
    m_qname(0)
{
    const XalanSize_t   nAttrs = atts.getLength();

    for (XalanSize_t i = 0; i < nAttrs; i++)
    {
        const XalanDOMChar* const   aname = atts.getName(i);

        if (equals(aname, Constants::ATTRNAME_NAME))
        {
            m_qname = constructionContext.createXalanQName(
                atts.getValue(i),
                stylesheetTree.getNamespaces(),
                getLocator());

            if (m_qname->isValid() == false)
            {
                error(
                    constructionContext,
                    XalanMessages::AttributeValueNotValidQName_2Param,
                    aname,
                    atts.getValue(i));
            }
        }
        else if (processUseAttributeSets(
                    constructionContext,
                    aname,
                    atts,
                    i) == false &&
                 isAttrOK(
                    aname,
                    atts,
                    i,
                    constructionContext) == false)
        {
            error(
                constructionContext,
                XalanMessages::ElementHasIllegalAttribute_2Param,
                Constants::ELEMNAME_ATTRIBUTESET_WITH_PREFIX_STRING.c_str(),
                aname);
        }
    }

    if (m_qname == 0)
    {
        error(
            constructionContext,
            XalanMessages::ElementMustHaveAttribute_2Param,
            Constants::ELEMNAME_ATTRIBUTESET_WITH_PREFIX_STRING,
            Constants::ATTRNAME_NAME);
    }
    assert(m_qname->isValid() == true);
}



ElemAttributeSet::~ElemAttributeSet()
{
}



const XalanDOMString&
ElemAttributeSet::getElementName() const
{
    return Constants::ELEMNAME_ATTRIBUTESET_WITH_PREFIX_STRING;
}



#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
const ElemTemplateElement*
ElemAttributeSet::startElement(StylesheetExecutionContext& executionContext) const
{
    ElemUse::startElement(executionContext);

    executionContext.pushCurrentStackFrameIndex(executionContext.getGlobalStackFrameIndex());
    executionContext.pushOnElementRecursionStack(this);

    return getFirstChildElemToExecute(executionContext);
}



void
ElemAttributeSet::endElement(StylesheetExecutionContext& executionContext) const
{
    executionContext.popElementRecursionStack();
    executionContext.popCurrentStackFrameIndex();

    ElemUse::endElement(executionContext);
}



const ElemTemplateElement*
ElemAttributeSet::getInvoker(StylesheetExecutionContext&    executionContext) const
{
    return executionContext.getInvoker();
}
#endif



#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
void
ElemAttributeSet::execute(StylesheetExecutionContext&   executionContext) const
{
    typedef StylesheetExecutionContext::SetAndRestoreCurrentStackFrameIndex     SetAndRestoreCurrentStackFrameIndex;
    typedef StylesheetExecutionContext::ElementRecursionStackPusher             ElementRecursionStackPusher;

    // This will push and pop the stack automatically...
    ElementRecursionStackPusher     thePusher(executionContext, this);

    // Make sure only global variables are visible during execution...
    SetAndRestoreCurrentStackFrameIndex     theSetAndRestore(
                    executionContext, 
                    executionContext.getGlobalStackFrameIndex());

    ElemUse::execute(executionContext);

    const ElemTemplateElement*  attr = getFirstChildElem();

    while(0 != attr)
    {
        attr->execute(executionContext);

        attr = attr->getNextSiblingElem();
    }
}
#endif



void
ElemAttributeSet::addToStylesheet(
            StylesheetConstructionContext&  /* constructionContext */,
            Stylesheet&                     theStylesheet)
{
    theStylesheet.getStylesheetRoot().addAttributeSet(*this);
}



bool
ElemAttributeSet::childTypeAllowed(int  xslToken) const
{
    bool    fResult = false;

    switch(xslToken)
    {
    case StylesheetConstructionContext::ELEMNAME_ATTRIBUTE:
        fResult = true;
        break;
        
    default:
        break;
    }

    return fResult;
}



XALAN_CPP_NAMESPACE_END
