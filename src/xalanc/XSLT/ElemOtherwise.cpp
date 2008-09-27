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
#include "ElemOtherwise.hpp"



#include <xercesc/sax/AttributeList.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include "Constants.hpp"
#include "StylesheetConstructionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



ElemOtherwise::ElemOtherwise(
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
        StylesheetConstructionContext::ELEMNAME_OTHERWISE)
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
                Constants::ELEMNAME_OTHERWISE_WITH_PREFIX_STRING.c_str(),
                aname,
                atts,
                i,
                constructionContext) == false)
        {
            error(
                constructionContext,
                XalanMessages::ElementHasIllegalAttribute_2Param,
                Constants::ELEMNAME_OTHERWISE_WITH_PREFIX_STRING.c_str(),
                aname);
        }
    }
}



const XalanDOMString&
ElemOtherwise::getElementName() const
{
    return Constants::ELEMNAME_OTHERWISE_WITH_PREFIX_STRING;
}


#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
const ElemTemplateElement*
ElemOtherwise::startElement(StylesheetExecutionContext& executionContext) const
{
    ElemTemplateElement::startElement(executionContext);

    return beginExecuteChildren(executionContext);
}



void
ElemOtherwise::endElement(StylesheetExecutionContext&   executionContext) const
{
    endExecuteChildren(executionContext);
}
#endif



#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
void
ElemOtherwise::execute(StylesheetExecutionContext&  executionContext) const
{
    ElemTemplateElement::execute(executionContext);

    executeChildren(executionContext);
}
#endif


XALAN_CPP_NAMESPACE_END
