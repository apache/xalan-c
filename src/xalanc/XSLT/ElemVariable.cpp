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
#include "ElemVariable.hpp"



#include <xercesc/sax/AttributeList.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/XPath.hpp>



#include "SelectionEvent.hpp"
#include "Stylesheet.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



ElemVariable::ElemVariable(
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
        StylesheetConstructionContext::ELEMNAME_VARIABLE),
    m_qname(0),
    m_selectPattern(0),
    m_isTopLevel(false),
    m_value(0),
    m_varContext(0)
{
    init(constructionContext, stylesheetTree, atts);
}



ElemVariable::ElemVariable(
            StylesheetConstructionContext&  constructionContext,
            Stylesheet&                     stylesheetTree,
            const AttributeListType&        atts,
            XalanFileLoc                    lineNumber, 
            XalanFileLoc                    columnNumber,
            int                             xslToken) :
    ParentType(
        constructionContext,
        stylesheetTree,
        lineNumber,
        columnNumber,
        xslToken),
    m_qname(0),
    m_selectPattern(0),
    m_isTopLevel(false),
    m_value(0),
    m_varContext(0)
{
    init(constructionContext, stylesheetTree, atts);
}



ElemVariable::~ElemVariable()
{
}



void
ElemVariable::init(
            StylesheetConstructionContext&  constructionContext,
            Stylesheet&                     stylesheetTree,
            const AttributeListType&        atts)
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
                    constructionContext) == false &&
                 processSpaceAttr(
                    Constants::ELEMNAME_VARIABLE_WITH_PREFIX_STRING.c_str(),
                    aname,
                    atts,
                    i,
                    constructionContext) == false)
        {
            error(
                constructionContext,
                XalanMessages::ElementHasIllegalAttribute_2Param,
                Constants::ELEMNAME_VARIABLE_WITH_PREFIX_STRING.c_str(),
                aname);
        }
    }

    if (m_qname == 0)
    {
        error(
            constructionContext,
            XalanMessages::ElementMustHaveAttribute_2Param,
            Constants::ELEMNAME_VARIABLE_WITH_PREFIX_STRING,
            Constants::ATTRNAME_NAME);
    }
}



const XalanQName&
ElemVariable::getNameAttribute() const
{
    assert(m_qname != 0);

    return *m_qname;
}



void
ElemVariable::addToStylesheet(
            StylesheetConstructionContext&  constructionContext,
            Stylesheet&                     theStylesheet)
{
    // Processing a top-level element only...
    if (&theStylesheet != &getStylesheet())
    {
        error(
            constructionContext,
            XalanMessages::ElemVariableInstanceAddedToWrongStylesheet);
    }
    else if (getParentNodeElem() != 0)
    {
        error(
            constructionContext,
            XalanMessages::ElemVariableInstanceIsAlreadyParented);
    }
    else
    {
        theStylesheet.setTopLevelVariable(this);

        m_isTopLevel = true;
    }
}



const XalanDOMString&
ElemVariable::getElementName() const
{
    return Constants::ELEMNAME_VARIABLE_WITH_PREFIX_STRING;
}



void
ElemVariable::setParentNodeElem(ElemTemplateElement*    theParent)
{
    if (m_isTopLevel == true)
    {
        throw XalanDOMException(XalanDOMException::HIERARCHY_REQUEST_ERR);
    }
    else
    {
        ParentType::setParentNodeElem(theParent);
    }
}



#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
const ElemTemplateElement*
ElemVariable::startElement(StylesheetExecutionContext& executionContext) const
{
    assert(m_qname != 0);

    ParentType::startElement(executionContext);

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
            executionContext.fireSelectEvent(
                SelectionEvent(
                    executionContext,
                    executionContext.getCurrentNode(),
                    *this,
                    XalanDOMString("select", executionContext.getMemoryManager()),
                    *m_selectPattern,
                    theValue));
        }

    }

    if (theValue.null() == false)
    {
        executionContext.pushVariable(
                *m_qname,
                theValue,
                getParentNodeElem());
    }
    else
    {
        executionContext.pushVariable(
                *m_qname,
                this,
                getParentNodeElem());
    }

    return 0;
}


void
ElemVariable::endElement(StylesheetExecutionContext& executionContext) const
{
    if (0 == m_selectPattern && 0 != getFirstChildElem())
    {
        endExecuteChildren(executionContext);

        executionContext.pushVariable(
                *m_qname,
                executionContext.endCreateXResultTreeFrag(),
                getParentNodeElem());
    }
}



#else
void
ElemVariable::execute(StylesheetExecutionContext&   executionContext) const
{
    assert(m_qname != 0);

    ParentType::execute(executionContext);

    const XObjectPtr    theValue(getValue(executionContext, executionContext.getCurrentNode()));

    if (theValue.null() == false)
    {
        executionContext.pushVariable(
                *m_qname,
                theValue,
                getParentNodeElem());
    }
    else
    {
        executionContext.pushVariable(
                *m_qname,
                this,
                getParentNodeElem());
    }
}
#endif



const XPath*
ElemVariable::getXPath(XalanSize_t  index) const
{
    return index == 0 ? m_selectPattern : 0;
}



const XObjectPtr
ElemVariable::getValue(
            StylesheetExecutionContext&     executionContext,
            XalanNode*                      sourceNode) const
{
    if (m_selectPattern == 0)
    {
        if (getFirstChildElem() == 0)
        {
            return executionContext.getXObjectFactory().createStringReference(s_emptyString);
        }
        else
        {
            
#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
            executionContext.beginCreateXResultTreeFrag(sourceNode);

            executeChildren(executionContext);

            return executionContext.endCreateXResultTreeFrag();
#else
            return executionContext.createXResultTreeFrag(*this, sourceNode);
#endif
        }
    }
    else
    {
        XObjectPtr  theValue;

        XalanNode* const    theCurrentNode = executionContext.getCurrentNode();
        
        if (theCurrentNode == sourceNode)
        {
            theValue = m_selectPattern->execute(*this, executionContext);
        }
        else
        {
            const XPathExecutionContext::CurrentNodePushAndPop  theCurrentNodePushAndPop(executionContext, sourceNode);

            theValue = m_selectPattern->execute(*this, executionContext);
        }

        if (0 != executionContext.getTraceListeners())
        {
            executionContext.fireSelectEvent(
                SelectionEvent(
                    executionContext,
                    sourceNode,
                    *this,
                    XalanDOMString("select", executionContext.getMemoryManager()),
                    *m_selectPattern,
                    theValue));
        }

        return theValue;
    }
}



XALAN_CPP_NAMESPACE_END
