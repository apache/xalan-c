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
#include "ElemMessage.hpp"



#include <xercesc/sax/AttributeList.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include "Constants.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



ElemMessage::ElemMessage(
            StylesheetConstructionContext&  constructionContext,
            Stylesheet&                     stylesheetTree,
            const AttributeListType&        atts,
            XalanFileLoc                    lineNumber, 
            XalanFileLoc                    columnNumber) :
    ElemTemplateElement(constructionContext,
                        stylesheetTree,
                        lineNumber,
                        columnNumber,
                        StylesheetConstructionContext::ELEMNAME_MESSAGE),
    m_terminate(false)
{
    const XalanSize_t  nAttrs = atts.getLength();

    for (XalanSize_t i = 0; i < nAttrs; i++)
    {
        const XalanDOMChar* const   aname = atts.getName(i);

        if (equals(aname, Constants::ATTRNAME_TERMINATE) == true)
        {
            const XalanDOMChar* const   avalue = atts.getValue(i);

            if (equals(avalue, Constants::ATTRVAL_YES) == true)
            {
                m_terminate = true;
            }
            else if (equals(avalue, Constants::ATTRVAL_NO) == false)
            {
                error(
                    constructionContext,
                    XalanMessages::ElementHasIllegalAttributeValue_3Param,
                    Constants::ELEMNAME_MESSAGE_WITH_PREFIX_STRING.c_str(),
                    aname,
                    avalue);
            }
        }
        else if (
            isAttrOK(
                aname,
                atts,
                i,
                constructionContext) == false &&
            processSpaceAttr(
                Constants::ELEMNAME_MESSAGE_WITH_PREFIX_STRING.c_str(),
                aname,
                atts,
                i,
                constructionContext) == false)
        {
            error(
                constructionContext,
                XalanMessages::ElementHasIllegalAttribute_2Param,
                Constants::ELEMNAME_MESSAGE_WITH_PREFIX_STRING.c_str(),
                aname);
        }
    }
}



const XalanDOMString&
ElemMessage::getElementName() const
{
    return Constants::ELEMNAME_MESSAGE_WITH_PREFIX_STRING;
}



#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
const ElemTemplateElement*
ElemMessage::startElement(StylesheetExecutionContext&       executionContext) const
{
    ElemTemplateElement::startElement(executionContext);
    
    XalanDOMString& theResult = executionContext.getAndPushCachedString();

    return beginChildrenToString(executionContext,theResult);
}



void
ElemMessage::endElement(StylesheetExecutionContext&     executionContext) const
{
    endChildrenToString(executionContext);

    XalanDOMString& theResult = executionContext.getAndPopCachedString();

    const Locator* const    theLocator = getLocator();

    executionContext.problem(
            StylesheetExecutionContext::eXSLTProcessor,
            StylesheetExecutionContext::eMessage,
            theResult,
            theLocator,
            executionContext.getCurrentNode());

    if (m_terminate == true)
    {
        throw ElemMessageTerminateException(executionContext.getMemoryManager(), theResult, theLocator);
    }
}
#endif



#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
void
ElemMessage::execute(StylesheetExecutionContext&    executionContext) const
{
    ElemTemplateElement::execute(executionContext);

    const StylesheetExecutionContext::GetCachedString   theResult(executionContext);

    const XalanDOMString&   theString =
        childrenToString(
            executionContext,
            theResult.get());

    const Locator* const    theLocator = getLocator();

    executionContext.message(
            theString,
            executionContext.getCurrentNode(),
            theLocator);

    if (m_terminate == true)
    {
        if (theLocator != 0)
        {
            throw ElemMessageTerminateException(
                        executionContext.getMemoryManager(),
                        *theLocator,
                        theString);
        }
        else
        {
            throw ElemMessageTerminateException(
                        executionContext.getMemoryManager(),
                        theString);
        }
    }
}
#endif

const XalanDOMChar  ElemMessage::ElemMessageTerminateException::m_type[] = 
{   
    XalanUnicode::charLetter_E,
    XalanUnicode::charLetter_l,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_m,
    XalanUnicode::charLetter_M,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_g,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_T,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_m,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_n,
    XalanUnicode::charLetter_a,
    XalanUnicode::charLetter_t,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_E,
    XalanUnicode::charLetter_x,
    XalanUnicode::charLetter_p,
    XalanUnicode::charLetter_r,
    XalanUnicode::charLetter_e,
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_s,
    XalanUnicode::charLetter_i,
    XalanUnicode::charLetter_o,
    XalanUnicode::charLetter_n,
    0
};



ElemMessage::ElemMessageTerminateException::ElemMessageTerminateException(
            MemoryManager&          theManager,
            const XalanDOMString&   theMessage,
            const Locator*          theLocator) :
    XSLTProcessorException(
            theManager,
            theMessage,
            theLocator)
{
}



ElemMessage::ElemMessageTerminateException::~ElemMessageTerminateException()
{
}



XALAN_CPP_NAMESPACE_END
