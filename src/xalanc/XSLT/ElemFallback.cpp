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
#include "ElemFallback.hpp"



#include <cassert>



#include <xercesc/sax/AttributeList.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include "Constants.hpp"
#include "StylesheetConstructionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



ElemFallback::ElemFallback(
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
        StylesheetConstructionContext::ELEMNAME_FALLBACK)
{
    const XalanSize_t  nAttrs = atts.getLength();

    for (XalanSize_t i = 0; i < nAttrs; i++)
    {
        const XalanDOMChar* const   aname = atts.getName(i);

        if (isAttrOK(
                aname,
                atts,
                i,
                constructionContext) == false &&
            processSpaceAttr(
                Constants::ELEMNAME_FALLBACK_WITH_PREFIX_STRING.c_str(),
                aname,
                atts,
                i,
                constructionContext) == false)
        {
            error(
                constructionContext,
                XalanMessages::ElementHasIllegalAttribute_2Param,
                Constants::ELEMNAME_FALLBACK_WITH_PREFIX_STRING.c_str(),
                aname);
        }
    }
}



const XalanDOMString&
ElemFallback::getElementName() const
{
    return Constants::ELEMNAME_FALLBACK_WITH_PREFIX_STRING;
}



ElemFallback::~ElemFallback()
{
}


#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
const ElemTemplateElement*
ElemFallback::startElement(StylesheetExecutionContext&      executionContext) const
{
    ElemTemplateElement::startElement(executionContext);

    return beginExecuteChildren(executionContext);
}


    
const ElemTemplateElement*
ElemFallback::getFirstChildElemToExecute(
            StylesheetExecutionContext& executionContext) const
{
    const int   parentXSLToken =
        getParentNodeElem()->getXSLToken();

    if (parentXSLToken == StylesheetConstructionContext::ELEMNAME_FORWARD_COMPATIBLE ||
        parentXSLToken == StylesheetConstructionContext::ELEMNAME_EXTENSION_CALL)
    {
        return ParentType::getFirstChildElemToExecute(executionContext);
    }
    else
    {
        return 0;
    }
}



const ElemTemplateElement*
ElemFallback::getNextChildElemToExecute(StylesheetExecutionContext& /*executionContext*/,
                                 const ElemTemplateElement*         currentElem) const
{
    const ElemTemplateElement* previousElement = currentElem;

    const ElemTemplateElement* nextElement = currentElem->getNextSiblingElem();

    while (nextElement != 0
           && nextElement->getXSLToken() == StylesheetConstructionContext::ELEMNAME_FALLBACK
           && previousElement->getXSLToken() == StylesheetConstructionContext::ELEMNAME_FORWARD_COMPATIBLE)
    {
        previousElement = nextElement;
        nextElement = nextElement->getNextSiblingElem();
    }

    return nextElement;
}
#endif



#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
void
ElemFallback::execute(StylesheetExecutionContext&       executionContext) const
{
    ElemTemplateElement::execute(executionContext);

    int     thePreviousToken = StylesheetConstructionContext::ELEMNAME_UNDEFINED;

    for (const ElemTemplateElement* child = getFirstChildElem(); child != 0; child = child->getNextSiblingElem())
    {
        const int   theCurrentToken = child->getXSLToken();

        if(theCurrentToken == StylesheetConstructionContext::ELEMNAME_FALLBACK)
        {
            // Don't execute a fallback if it's preceeded by a forward-compatible
            // element.
            if (thePreviousToken != StylesheetConstructionContext::ELEMNAME_FORWARD_COMPATIBLE)
            {
                child->execute(executionContext);
            }
        }
        else
        {
            child->execute(executionContext);
        }

        thePreviousToken = theCurrentToken;
    }
}
#endif


XALAN_CPP_NAMESPACE_END
