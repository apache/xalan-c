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
#include "ElemComment.hpp"



#include <xercesc/sax/AttributeList.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include "Constants.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



ElemComment::ElemComment(
            StylesheetConstructionContext&  constructionContext,
            Stylesheet&                     stylesheetTree,
            const AttributeListType&        atts,
            XalanFileLoc                    lineNumber, 
            XalanFileLoc                    columnNumber) :
    ElemTemplateElement(constructionContext,
                        stylesheetTree,
                        lineNumber,
                        columnNumber,
                        StylesheetConstructionContext::ELEMNAME_COMMENT)
{
    const XalanSize_t nAttrs = atts.getLength();

    for(XalanSize_t i = 0; i < nAttrs; i++)
    {
        const XalanDOMChar* const   aname = atts.getName(i);

        if (isAttrOK(
                aname,
                atts,
                i,
                constructionContext) == false &&
            processSpaceAttr(
                Constants::ELEMNAME_COMMENT_WITH_PREFIX_STRING.c_str(),
                aname,
                atts,
                i,
                constructionContext) == false)
        {
            error(
                constructionContext,
                XalanMessages::ElementHasIllegalAttribute_2Param,
                Constants::ELEMNAME_COMMENT_WITH_PREFIX_STRING.c_str(),
                aname);
        }
    }   
}


ElemComment::~ElemComment()
{
}



const XalanDOMString&
ElemComment::getElementName() const
{
    return Constants::ELEMNAME_COMMENT_WITH_PREFIX_STRING;
}


#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
const ElemTemplateElement*
ElemComment::startElement(StylesheetExecutionContext&       executionContext) const
{
    ElemTemplateElement::startElement(executionContext);

    executionContext.pushCopyTextNodesOnly(true);

    XalanDOMString& theResult = executionContext.getAndPushCachedString();

    return beginChildrenToString(executionContext,theResult);

}



void
ElemComment::endElement(StylesheetExecutionContext&     executionContext) const
{
    endChildrenToString(executionContext);

    XalanDOMString&     theResult = executionContext.getAndPopCachedString();

    XalanDOMString::iterator    theEnd =
        theResult.end();

    XalanDOMString::iterator    theCurrent =
        theResult.begin();

    // We need to fix up any occurrences of the sequence '--' in
    // the comment's data by inserting a space between them.  Also,
    // if the data ends with a '-', then we must append a space to
    // the data.
    while(theCurrent != theEnd)
    {
        const XalanDOMChar  theChar = *theCurrent;

        if (theChar == XalanUnicode::charHyphenMinus)
        {
            XalanDOMString::iterator    theNext =
                theCurrent + 1;

            if (theNext == theEnd ||
                *theNext == XalanUnicode::charHyphenMinus)
            {
                theCurrent =
                    theResult.insert(
                        theNext,
                        XalanUnicode::charSpace);

                theEnd = theResult.end();
            }
        }

        ++theCurrent;
    }

    executionContext.comment(theResult.c_str());

    executionContext.popCopyTextNodesOnly();
}
#endif



#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
void
ElemComment::execute(StylesheetExecutionContext&    executionContext) const
{
    ElemTemplateElement::execute(executionContext);

    StylesheetExecutionContext::SetAndRestoreCopyTextNodesOnly  theSetAndRestore(executionContext, true);

    childrenToResultComment(executionContext);
}
#endif



bool
ElemComment::childTypeAllowed(int   xslToken) const
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
