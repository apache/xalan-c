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
#include "ElemApplyTemplates.hpp"



#include <xercesc/sax/AttributeList.hpp>



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/XPath/XalanQNameByValue.hpp>



#include "Constants.hpp"
#include "Stylesheet.hpp"
#include "StylesheetRoot.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"
#include "TracerEvent.hpp"



XALAN_CPP_NAMESPACE_BEGIN



static const XalanQNameByValue  s_defaultMode(XalanMemMgrs::getDummyMemMgr());



ElemApplyTemplates::ElemApplyTemplates(
            StylesheetConstructionContext&  constructionContext,
            Stylesheet&                     stylesheetTree,
            const AttributeListType&        atts,
            XalanFileLoc                    lineNumber,
            XalanFileLoc                    columnNumber) :
    ParentType(
        constructionContext,
        stylesheetTree,
        lineNumber,
        columnNumber,
        StylesheetConstructionContext::ELEMNAME_APPLY_TEMPLATES),
    m_mode(0)
{
    const XalanSize_t  nAttrs = atts.getLength();

    for (XalanSize_t i = 0; i < nAttrs; i++)
    {
        const XalanDOMChar* const   aname = atts.getName(i);

        if (equals(aname, Constants::ATTRNAME_SELECT))
        {
            m_selectPattern = constructionContext.createXPath(getLocator(), atts.getValue(i), *this);
        }
        else if (equals(aname, Constants::ATTRNAME_MODE))
        {
            m_mode = constructionContext.createXalanQName(atts.getValue(i), getStylesheet().getNamespaces(), getLocator());

            if (m_mode->isValid() == false)
            {
                error(
                    constructionContext,
                    XalanMessages::AttributeValueNotValidQName_2Param,
                    aname,
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
                Constants::ELEMNAME_APPLY_TEMPLATES_WITH_PREFIX_STRING.c_str(),
                aname);
        }
    }

    if(0 == m_selectPattern)
    {
        m_selectPattern = constructionContext.createXPath(
            getLocator(),
            Constants::PSEUDONAME_NODE,
            *this);
    }

    if (m_mode == 0)
    {
        m_mode = &s_defaultMode;
    }

    assert(m_selectPattern != 0 && m_mode != 0);
}



ElemApplyTemplates::~ElemApplyTemplates()
{
}



const XalanDOMString&
ElemApplyTemplates::getElementName() const
{
    return Constants::ELEMNAME_APPLY_TEMPLATES_WITH_PREFIX_STRING;
}



#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
const ElemTemplateElement*
ElemApplyTemplates::startElement(StylesheetExecutionContext&        executionContext) const
{
    ElemTemplateElement::startElement(executionContext);

    if (isDefaultTemplate() == false)
    {
        executionContext.pushCurrentMode(m_mode);
    }

    executionContext.pushInvoker(this);

    return getFirstChildElemToExecute(executionContext);


}



void
ElemApplyTemplates::endElement(StylesheetExecutionContext&      executionContext) const
{
    executionContext.popContextNodeList();
    executionContext.popNodesToTransformList();

    releaseSelectedAndSortedNodeList(executionContext);

    if (isDefaultTemplate() == false)
    {
        executionContext.popCurrentMode();
    }

    executionContext.popContextMarker();

    executionContext.popInvoker();

    ElemTemplateElement::endElement(executionContext);
}



const ElemTemplateElement*
ElemApplyTemplates::getNextChildElemToExecute(
               StylesheetExecutionContext& executionContext,
               const ElemTemplateElement* currentElem) const
{
    if (currentElem->getXSLToken() == StylesheetConstructionContext::ELEMNAME_TEMPLATE)
    {
        executionContext.popCurrentNode();

        return findNextTemplateToExecute(executionContext);
    }
    else
    {
        const ElemTemplateElement* nextElement = ElemTemplateElement::getNextChildElemToExecute(executionContext, currentElem);

        if (nextElement == 0)
        {
            const NodeRefListBase* nodesToTransform = createSelectedAndSortedNodeList(
                                                executionContext);

            executionContext.createAndPushNodesToTransformList(nodesToTransform);
    
            executionContext.pushContextNodeList(*nodesToTransform);

            executionContext.pushContextMarker();

            executionContext.endParams();
    
            return findNextTemplateToExecute(executionContext);

        }

        return nextElement;
    }
}



const ElemTemplateElement*
ElemApplyTemplates::getFirstChildElemToExecute(
               StylesheetExecutionContext& executionContext) const
               
{
    const ElemTemplateElement* firstElement = getFirstChildElem();

    if (firstElement != 0)
    {
        executionContext.beginParams();
        return firstElement;
    }
    else
    {
        const NodeRefListBase* nodesToTransform = createSelectedAndSortedNodeList(
                                                executionContext);

        executionContext.createAndPushNodesToTransformList(nodesToTransform);
    
        executionContext.pushContextNodeList(*nodesToTransform);
        
        executionContext.pushContextMarker();

        return findNextTemplateToExecute(executionContext);
    }
}

const ElemTemplateElement*
ElemApplyTemplates::findNextTemplateToExecute(
                StylesheetExecutionContext& executionContext) const
{
    const ElemTemplateElement* selectedTemplate = 0;

    do
    {
        XalanNode* nextNode =  executionContext.getNextNodeToTransform();
        if (nextNode == 0)
        {
            break;
        }

        executionContext.pushCurrentNode(nextNode);

        selectedTemplate = findTemplateToTransformChild(
                        executionContext,
                        *this,
                        0,
                        nextNode);

        if (0 == selectedTemplate)
        {
            executionContext.popCurrentNode();
        }

    } while (0 == selectedTemplate);

    return selectedTemplate;
}
#endif


#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
void
ElemApplyTemplates::execute(StylesheetExecutionContext&     executionContext) const
{
    assert(m_selectPattern != 0 && m_mode != 0);

    if(0 != executionContext.getTraceListeners())
    {
        executionContext.fireTraceEvent(TracerEvent(
          executionContext, *this));
    }

    ParentType::transformSelectedChildren(
            executionContext,
            0);
}



void
ElemApplyTemplates::transformChild(
            StylesheetExecutionContext&     executionContext,
            const ElemTemplateElement&      xslInstruction,
            const ElemTemplateElement*      theTemplate,
            XalanNode*                      child) const
{
    assert(child != 0);

    const XalanNode::NodeType   nodeType = child->getNodeType();

    // Filter out any attributes nodes that are namespace declarations
    if (nodeType != XalanNode::ATTRIBUTE_NODE ||
        DOMServices::isNamespaceDeclaration(static_cast<const XalanAttr&>(*child)) == false)
    {
        ParentType::transformChild(
            executionContext,
            xslInstruction,
            theTemplate,
            child,
            nodeType);
    }
}



void
ElemApplyTemplates::selectAndSortChildren(
            StylesheetExecutionContext&     executionContext,
            const ElemTemplateElement*      theTemplate,
            NodeSorter*                     sorter,
            int                             /* selectStackFrameIndex */) const
{
    int     savedStackFrameIndex;

    // Push the params & stack frame, but then execute the select
    // expression inside transformSelectedChildren, which must be
    // executed in the stack frame before the new stack frame.
    const StylesheetExecutionContext::ParamsPushPop     thePushPop(
            executionContext,
            *this,
            savedStackFrameIndex);

    const XalanQName* const     currentMode = executionContext.getCurrentMode();
    assert(currentMode != 0);

    if (isDefaultTemplate() == false &&
        !m_mode->equals(*currentMode))
    {
        executionContext.pushCurrentMode(m_mode);

        ParentType::selectAndSortChildren(
                executionContext,
                theTemplate,
                sorter,
                savedStackFrameIndex);

        executionContext.popCurrentMode();
    }
    else
    {
        ParentType::selectAndSortChildren(
                executionContext,
                theTemplate,
                sorter,
                savedStackFrameIndex);
    }
}
#endif



bool
ElemApplyTemplates::childTypeAllowed(int    xslToken) const
{
    bool    fResult = false;

    switch(xslToken)
    {
    case StylesheetConstructionContext::ELEMNAME_SORT:
    case StylesheetConstructionContext::ELEMNAME_WITH_PARAM:
        fResult = true;
        break;

    default:
        break;
    }

    return fResult;
}



XALAN_CPP_NAMESPACE_END
