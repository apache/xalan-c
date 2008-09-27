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
#include "ElemTextLiteral.hpp"



#include <cassert>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



#include "Constants.hpp"
#include "Stylesheet.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



ElemTextLiteral::ElemTextLiteral(
            StylesheetConstructionContext&  constructionContext,
            Stylesheet&                     stylesheetTree,
            XalanFileLoc                    lineNumber, 
            XalanFileLoc                    columnNumber,
            const XMLCh*                    ch,
            XalanDOMString::size_type       start,
            XalanDOMString::size_type       length,
            bool                            fPreserveSpace,
            bool                            fDisableOutputEscaping) :
    ElemTemplateElement(
        constructionContext,
        stylesheetTree,
        StylesheetConstructionContext::ELEMNAME_TEXT_LITERAL_RESULT,
        stylesheetTree.getBaseIdentifier(),
        lineNumber,
        columnNumber),
    m_isWhitespace(isXMLWhitespace(ch, start, length)),
    // Always null-terminate our buffer, since we may need it that way.
    m_ch(constructionContext.allocateXalanDOMCharVector(ch + start, length, true)),
    m_length(length)
{
    disableOutputEscaping(fDisableOutputEscaping);

    preserveSpace(fPreserveSpace);
}



ElemTextLiteral::~ElemTextLiteral()
{
}



const XalanDOMString&
ElemTextLiteral::getElementName() const
{
    return Constants::ELEMNAME_TEXT_WITH_PREFIX_STRING;
}



bool
ElemTextLiteral::isWhitespace() const
{
    return m_isWhitespace;
}


#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
const ElemTemplateElement*
ElemTextLiteral::startElement(StylesheetExecutionContext&   executionContext) const
{
    ElemTemplateElement::startElement(executionContext);

    if (disableOutputEscaping() == false)
    {
        executionContext.characters(m_ch, 0, m_length);
    }
    else
    {
        executionContext.charactersRaw(m_ch, 0, m_length);
    }
    return 0;
}
#endif



#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
void
ElemTextLiteral::execute(StylesheetExecutionContext&    executionContext) const
{
    ElemTemplateElement::execute(executionContext);

    if (disableOutputEscaping() == false)
    {
        executionContext.characters(m_ch, 0, m_length);
    }
    else
    {
        executionContext.charactersRaw(m_ch, 0, m_length);
    }
}
#endif



XALAN_CPP_NAMESPACE_END
