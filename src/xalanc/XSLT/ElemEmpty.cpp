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
#include "ElemEmpty.hpp"



#include <cassert>


#include "StylesheetConstructionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



ElemEmpty::ElemEmpty(
            StylesheetConstructionContext&  constructionContext,
            Stylesheet&                     stylesheetTree,
            XalanFileLoc                    lineNumber, 
            XalanFileLoc                    columnNumber,
            const XalanDOMString*           elementName) :
    ElemTemplateElement(
        constructionContext,
        stylesheetTree,
        StylesheetConstructionContext::ELEMNAME_UNDEFINED,
        XalanDOMString(constructionContext.getMemoryManager()),
        lineNumber,
        columnNumber),
    m_elementName(elementName)
{
}



ElemEmpty::ElemEmpty(
            StylesheetConstructionContext&  constructionContext,
            Stylesheet&                     stylesheetTree,
            const XalanDOMString*           elementName) :
    ElemTemplateElement(
        constructionContext,
        stylesheetTree,
        StylesheetConstructionContext::ELEMNAME_UNDEFINED),
    m_elementName(elementName)
{
}



ElemEmpty::~ElemEmpty()
{
    assert(getNextSiblingElem() == 0 && getFirstChildElem() == 0);
}



const XalanDOMString&
ElemEmpty::getElementName() const
{
    return m_elementName != 0 ? *m_elementName : s_emptyString;
}


#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
const ElemTemplateElement*
ElemEmpty::startElement(StylesheetExecutionContext&     executionContext) const
{
    assert(false);  // really shouldn't be executing empty nodes

    ElemTemplateElement::startElement(executionContext);

    return 0;
}
#endif


#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
void
ElemEmpty::execute(StylesheetExecutionContext&      executionContext) const
{
    assert(false);  // really shouldn't be executing empty nodes

    ElemTemplateElement::execute(executionContext);
}
#endif



bool
ElemEmpty::childTypeAllowed(int     /* xslToken */) const
{
    return false;
}



XALAN_CPP_NAMESPACE_END
