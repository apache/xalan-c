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
#include "ElemPI.hpp"



#include <xercesc/sax/AttributeList.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include "AVT.hpp"
#include "Constants.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



ElemPI::ElemPI(
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
        StylesheetConstructionContext::ELEMNAME_PI),
    m_nameAVT(0)
{
    const XalanSize_t  nAttrs = atts.getLength();

    for(XalanSize_t i = 0; i < nAttrs; i++)
    {
        const XalanDOMChar* const   aname = atts.getName(i);

        if (equals(aname, Constants::ATTRNAME_NAME))
        {           
            m_nameAVT =
                    constructionContext.createAVT(getLocator(), aname, atts.getValue(i), *this);
        }
        else if (isAttrOK(
                    aname,
                    atts,
                    i,
                    constructionContext) == false &&
                 processSpaceAttr(
                    Constants::ELEMNAME_PI_WITH_PREFIX_STRING.c_str(),
                    aname,
                    atts,
                    i,
                    constructionContext) == false)
        {
            error(
                constructionContext,
                XalanMessages::ElementHasIllegalAttribute_2Param,
                Constants::ELEMNAME_PI_WITH_PREFIX_STRING.c_str(),
                aname);
        }
    }

    if (0 == m_nameAVT)
    {
        error(
            constructionContext,
            XalanMessages::ElementMustHaveAttribute_2Param,
            Constants::ELEMNAME_PI_WITH_PREFIX_STRING,
            Constants::ATTRNAME_NAME);
    }
}



ElemPI::~ElemPI()
{
}



const XalanDOMString&
ElemPI::getElementName() const
{
    return Constants::ELEMNAME_PI_WITH_PREFIX_STRING;
}



#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
const ElemTemplateElement*
ElemPI::startElement(StylesheetExecutionContext&    executionContext) const
{
    ElemTemplateElement::startElement(executionContext);
    
    XalanDOMString& piName = executionContext.getAndPushCachedString();

    m_nameAVT->evaluate(piName, *this, executionContext);

    if (equalsIgnoreCaseASCII(
            piName,
            Constants::ATTRVAL_OUTPUT_METHOD_XML) ||
       isValidNCName(piName) == false)
    {
        error(
            executionContext,
            XalanMessages::PINameInvalid_1Param,
            piName);
    }

    XalanDOMString& theResult = executionContext.getAndPushCachedString();

    executionContext.pushCopyTextNodesOnly(true);

    return beginChildrenToString(executionContext,theResult);
}



void
ElemPI::endElement(StylesheetExecutionContext&  executionContext) const
{
    endChildrenToString(executionContext);

    XalanDOMString&     piData =
        executionContext.getAndPopCachedString();

    const XalanDOMString&   piName =
        executionContext.getAndPopCachedString();

    XalanDOMString::iterator    theEnd =
        piData.end();

    XalanDOMString::iterator    theCurrent =
        piData.begin();

    // We need to fix up any occurrences of the sequence '?>' in
    // the PI's data by inserting a space between them.
    while (theCurrent != theEnd)
    {
        const XalanDOMChar  theChar = *theCurrent;

        if (theChar == XalanUnicode::charQuestionMark)
        {
            XalanDOMString::iterator    theNext =
                theCurrent + 1;

            if (theNext != theEnd &&
                *theNext == XalanUnicode::charGreaterThanSign)
            {
                theCurrent =
                    piData.insert(
                        theNext,
                        XalanUnicode::charSpace);

                theEnd = piData.end();

                // Move forward, since we're not interested in
                // the '>' character.
                ++theCurrent;
            }
        }

        ++theCurrent;
    }

    executionContext.processingInstruction(
                piName.c_str(),
                piData.c_str());

    executionContext.popCopyTextNodesOnly();
}
#endif



#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
void
ElemPI::execute(StylesheetExecutionContext&     executionContext) const
{
    ElemTemplateElement::execute(executionContext);
    
    const StylesheetExecutionContext::GetCachedString   theGuard(executionContext);

    XalanDOMString&     piName = theGuard.get();

    m_nameAVT->evaluate(piName, *this, executionContext);

    if (equalsIgnoreCaseASCII(
            piName,
            Constants::ATTRVAL_OUTPUT_METHOD_XML) ||
       isValidNCName(piName) == false)
    {
        error(
            executionContext,
            XalanMessages::PINameInvalid_1Param,
            piName);
    }

    StylesheetExecutionContext::SetAndRestoreCopyTextNodesOnly  theSetAndRestore(executionContext, true);

    childrenToResultPI(
            executionContext,
            piName);
}
#endif



bool
ElemPI::childTypeAllowed(int    xslToken) const
{
    bool    fResult = false;
    
    switch(xslToken)
    {
    // char-instructions 
    case StylesheetConstructionContext::ELEMNAME_TEXT_LITERAL_RESULT:
    case StylesheetConstructionContext::ELEMNAME_APPLY_TEMPLATES:
    case StylesheetConstructionContext::ELEMNAME_APPLY_IMPORTS:
    case StylesheetConstructionContext::ELEMNAME_CALL_TEMPLATE:
    case StylesheetConstructionContext::ELEMNAME_FOR_EACH:
    case StylesheetConstructionContext::ELEMNAME_VALUE_OF:
    case StylesheetConstructionContext::ELEMNAME_COPY_OF:
    case StylesheetConstructionContext::ELEMNAME_NUMBER:
    case StylesheetConstructionContext::ELEMNAME_CHOOSE:
    case StylesheetConstructionContext::ELEMNAME_IF:
    case StylesheetConstructionContext::ELEMNAME_TEXT:
    case StylesheetConstructionContext::ELEMNAME_COPY:
    case StylesheetConstructionContext::ELEMNAME_VARIABLE:
    case StylesheetConstructionContext::ELEMNAME_MESSAGE:
        fResult = true;
        break;
        
    default:
        break;
    }
    
    return fResult;
}



XALAN_CPP_NAMESPACE_END
