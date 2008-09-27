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
#include "ElemWithParam.hpp"



#include <xercesc/sax/AttributeList.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/XPath/XObjectFactory.hpp>



#include "SelectionEvent.hpp"
#include "Constants.hpp"
#include "Stylesheet.hpp"
#include "StylesheetConstructionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



ElemWithParam::ElemWithParam(
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
        StylesheetConstructionContext::ELEMNAME_WITH_PARAM),
    m_selectPattern(0),
    m_qname(0)
{
    const XalanSize_t  nAttrs = atts.getLength();

    for (XalanSize_t i = 0; i < nAttrs; i++)
    {
        const XalanDOMChar* const   aname = atts.getName(i);

        if (equals(aname, Constants::ATTRNAME_SELECT))
        {
            m_selectPattern = constructionContext.createXPath(getLocator(), atts.getValue(i), *this);
        }
        else if (equals(aname, Constants::ATTRNAME_NAME))
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
                    Constants::ATTRNAME_NAME.c_str(),
                    atts.getValue(i));
            }
        }
        else if (isAttrOK(
                    aname,
                    atts,
                    i,
                    constructionContext) == false)
        {
            error(
                constructionContext,
                XalanMessages::ElementHasIllegalAttribute_2Param,
                Constants::ELEMNAME_WITHPARAM_WITH_PREFIX_STRING.c_str(),
                aname);
        }
    }

    if (m_qname == 0)
    {
        error(
            constructionContext,
            XalanMessages::ElementMustHaveAttribute_2Param,
            Constants::ELEMNAME_WITHPARAM_WITH_PREFIX_STRING,
            Constants::ATTRNAME_NAME);
    }
}



ElemWithParam::~ElemWithParam()
{
}



const XalanDOMString&
ElemWithParam::getElementName() const
{
    return Constants::ELEMNAME_WITHPARAM_WITH_PREFIX_STRING;
}



const XPath*
ElemWithParam::getXPath(XalanSize_t     index) const
{
    return index == 0 ? m_selectPattern : 0;
}



#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)

const ElemTemplateElement* 
ElemWithParam::startElement(StylesheetExecutionContext& executionContext) const
{
    assert(m_qname != 0);

    ElemTemplateElement::startElement(executionContext);

    XObjectPtr theValue;

    if (m_selectPattern == 0)
    {
        if (getFirstChildElem() == 0)
        {
            theValue = executionContext.getXObjectFactory().createStringReference(s_emptyString);
        }
        else
        {
            executionContext.beginCreateXResultTreeFrag(executionContext.getCurrentNode());

            return beginExecuteChildren(executionContext);
        }
    }
    else
    {
        theValue = m_selectPattern->execute(*this, executionContext);
    
        if (0 != executionContext.getTraceListeners())
        {
            const StylesheetExecutionContext::GetCachedString   theGuard(executionContext);

            XalanDOMString&     theString = theGuard.get();

            theString = "select";

            executionContext.fireSelectEvent(
                SelectionEvent(
                    executionContext,
                    executionContext.getCurrentNode(),
                    *this,
                    theString,
                    *m_selectPattern,
                    theValue));
        }
    }

    assert(theValue.null() == false);

    executionContext.pushParam(
            *m_qname,
            theValue);

    return 0;
}



void
ElemWithParam::endElement(StylesheetExecutionContext& executionContext) const
{
    if (0 == m_selectPattern && 0 != getFirstChildElem())
    {
        endExecuteChildren(executionContext);

        executionContext.pushParam(
                    *m_qname,
                    executionContext.endCreateXResultTreeFrag());
    }
}
#endif



XALAN_CPP_NAMESPACE_END
