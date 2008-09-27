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
#include "ElemCallTemplate.hpp"



#include <xercesc/sax/AttributeList.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include "AVT.hpp"
#include "Constants.hpp"
#include "ElemTemplate.hpp"
#include "Stylesheet.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"
#include "StylesheetRoot.hpp"



XALAN_CPP_NAMESPACE_BEGIN



ElemCallTemplate::ElemCallTemplate(
            StylesheetConstructionContext&  constructionContext,
            Stylesheet&                     stylesheetTree,
            const AttributeListType&        atts,
            XalanFileLoc                    lineNumber, 
            XalanFileLoc                    columnNumber) :
    ElemTemplateElement(constructionContext,
                        stylesheetTree,
                        lineNumber,
                        columnNumber,
                        StylesheetConstructionContext::ELEMNAME_CALL_TEMPLATE),
    m_templateName(0),
    m_template(0)
{
    const XalanSize_t  nAttrs = atts.getLength();

    for(XalanSize_t i = 0; i < nAttrs; i++)
    {
        const XalanDOMChar* const   aname = atts.getName(i);

        if (equals(aname, Constants::ATTRNAME_NAME))
        {
            m_templateName = constructionContext.createXalanQName(
                    atts.getValue(i),
                    getStylesheet().getNamespaces(),
                    getLocator());

            if (m_templateName->isValid() == false)
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
                Constants::ELEMNAME_CALLTEMPLATE_WITH_PREFIX_STRING.c_str(),
                aname);
        }
    }

    if (m_templateName == 0)
    {
        error(
            constructionContext,
            XalanMessages::ElementMustHaveAttribute_2Param,
            Constants::ELEMNAME_CALLTEMPLATE_WITH_PREFIX_STRING,
            Constants::ATTRNAME_NAME);
    }
}



ElemCallTemplate::~ElemCallTemplate()
{
}



const XalanDOMString&
ElemCallTemplate::getElementName() const
{
    return Constants::ELEMNAME_CALLTEMPLATE_WITH_PREFIX_STRING;
}



#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
const ElemTemplateElement*
ElemCallTemplate::startElement(StylesheetExecutionContext&      executionContext) const
{
    ElemTemplateElement::startElement(executionContext);

    assert(m_template != 0);

    executionContext.pushInvoker(this);

    return getFirstChildElemToExecute(executionContext);
}

void
ElemCallTemplate::endElement(StylesheetExecutionContext&        executionContext) const 
{
    executionContext.popContextMarker();

    executionContext.popInvoker();
}



const ElemTemplateElement*
ElemCallTemplate::getNextChildElemToExecute(
            StylesheetExecutionContext&     executionContext,
            const ElemTemplateElement*      currentElem) const
{
    if (currentElem == m_template)
    {
        return 0;
    }

    const ElemTemplateElement* const    nextElement =
        ElemTemplateElement::getNextChildElemToExecute(executionContext,currentElem);

    if (nextElement == 0)
    {
        executionContext.pushContextMarker();
        executionContext.endParams();
        
        return m_template;
    }

    return nextElement;
}



const ElemTemplateElement*
ElemCallTemplate::getFirstChildElemToExecute(
        StylesheetExecutionContext& executionContext) const
{
    const ElemTemplateElement* const    firstElement =
        ElemTemplateElement::getFirstChildElemToExecute(executionContext);

    if (firstElement != 0)
    {
        executionContext.beginParams();

        return firstElement;
    }
    else
    {
        executionContext.pushContextMarker();

        return m_template;
    }
}
#endif



#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
void
ElemCallTemplate::execute(StylesheetExecutionContext&   executionContext) const
{
    ElemTemplateElement::execute(executionContext);

    assert(m_template != 0);

    const StylesheetExecutionContext::ParamsPushPop     thePushPop(
                executionContext,
                *this);

    m_template->executeAsNamed(executionContext);
}
#endif



void
ElemCallTemplate::postConstruction(
            StylesheetConstructionContext&  constructionContext,
            const NamespacesHandler&        theParentHandler)
{
    assert(m_templateName != 0);

    m_template = getStylesheet().getStylesheetRoot().findNamedTemplate(*m_templateName);

    if(m_template == 0)
    {
        error(
            constructionContext,
            XalanMessages::CannotFindNamedTemplate);
    }

    // OK, now we can chain-up...
    ElemTemplateElement::postConstruction(constructionContext, theParentHandler);
}



bool
ElemCallTemplate::childTypeAllowed(int  xslToken) const
{
    if (xslToken == StylesheetConstructionContext::ELEMNAME_WITH_PARAM)
    {
        return true;
    }
    else
    {
        return false;
    }
}



XALAN_CPP_NAMESPACE_END
