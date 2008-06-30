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
#include "ElemCopyOf.hpp"



#include <xercesc/sax/AttributeList.hpp>



#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/XPath/XObject.hpp>
#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/XPath.hpp>



#include "Constants.hpp"
#include "ElemWhen.hpp"
#include "SelectionEvent.hpp"
#include "Stylesheet.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"
#include "StylesheetRoot.hpp"



XALAN_CPP_NAMESPACE_BEGIN



ElemCopyOf::ElemCopyOf(
            StylesheetConstructionContext&  constructionContext,
            Stylesheet&                     stylesheetTree,
            const AttributeListType&        atts,
            XalanFileLoc                    lineNumber,
            XalanFileLoc                    columnNumber) :
    ElemTemplateElement(constructionContext,
                        stylesheetTree,
                        lineNumber,
                        columnNumber,
                        StylesheetConstructionContext::ELEMNAME_COPY_OF),
    m_selectPattern(0)
{
    bool    isSelectCurrentNode = false;

    const XalanSize_t   nAttrs = atts.getLength();
    
    for (XalanSize_t i = 0; i < nAttrs; ++i)
    {
        const XalanDOMChar* const   aname = atts.getName(i);

        if (equals(aname, Constants::ATTRNAME_SELECT))
        {
            const XalanDOMChar* const   avalue = atts.getValue(i);
            assert(avalue != 0);

            if (avalue[0] == XalanUnicode::charFullStop && avalue[1] == 0)
            {
                isSelectCurrentNode = true;
            }
            else
            {
                m_selectPattern = constructionContext.createXPath(getLocator(), avalue, *this);
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
                Constants::ELEMNAME_COPY_OF_WITH_PREFIX_STRING.c_str(),
                aname);
        }
    }

    if (isSelectCurrentNode == false && m_selectPattern == 0)
    {
        error(
            constructionContext,
            XalanMessages::ElementMustHaveAttribute_2Param,
            Constants::ELEMNAME_COPY_OF_WITH_PREFIX_STRING,
            Constants::ATTRNAME_SELECT);
    }
}



const XalanDOMString&
ElemCopyOf::getElementName() const
{
    return Constants::ELEMNAME_COPY_OF_WITH_PREFIX_STRING;
}


#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
const ElemTemplateElement*
ElemCopyOf::startElement(StylesheetExecutionContext&        executionContext) const
{
    ElemTemplateElement::startElement(executionContext);

    XalanNode* const    sourceNode = executionContext.getCurrentNode();
    assert(sourceNode != 0);

    if (m_selectPattern == 0)
    {
        if (0 != executionContext.getTraceListeners())
        {
            StylesheetExecutionContext::BorrowReturnMutableNodeRefList  theNodeList(executionContext);

            theNodeList->addNode(sourceNode);

            executionContext.fireSelectEvent(
                SelectionEvent(
                    executionContext,
                    sourceNode,
                    *this,
                    XalanDOMString("select", executionContext.getMemoryManager()),
                    XalanDOMString(".", executionContext.getMemoryManager()),
                    executionContext.getXObjectFactory().createNodeSet(theNodeList)));
        }

        executionContext.cloneToResultTree(*sourceNode, getLocator());
    }
    else
    {
        const XObjectPtr    value(m_selectPattern->execute(*this, executionContext));
        assert(value.null() == false);

        if (0 != executionContext.getTraceListeners())
        {
            executionContext.fireSelectEvent(
                SelectionEvent(
                    executionContext,
                    sourceNode,
                    *this,
                    XalanDOMString("select", executionContext.getMemoryManager()),
                    *m_selectPattern,
                    value));
        }

        const XObject::eObjectType  type = value->getType();

        switch(type)
        {
        case XObject::eTypeBoolean:
        case XObject::eTypeNumber:
        case XObject::eTypeString:
            executionContext.characters(value);
            break;

        case XObject::eTypeNodeSet:
            {
                const NodeRefListBase&              theNodeList = value->nodeset();
                const NodeRefListBase::size_type    nChildren = theNodeList.getLength();

                for(NodeRefListBase::size_type i = 0; i < nChildren; i++)
                {
                    assert(theNodeList.item(i) != 0);

                    executionContext.cloneToResultTree(*theNodeList.item(i), getLocator());
                }
            }
            break;

        case XObject::eTypeResultTreeFrag:
            executionContext.outputResultTreeFragment(*value.get(), getLocator());
            break;

        default:
            executionContext.characters(value);
            break;
        }
    }
    return 0;
}
#endif



#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
void
ElemCopyOf::execute(StylesheetExecutionContext&     executionContext) const
{
    ElemTemplateElement::execute(executionContext);

    XalanNode* const    sourceNode = executionContext.getCurrentNode();
    assert(sourceNode != 0);

    if (m_selectPattern == 0)
    {
        if (0 != executionContext.getTraceListeners())
        {
            StylesheetExecutionContext::BorrowReturnMutableNodeRefList  theNodeList(executionContext);

            theNodeList->addNode(sourceNode);

            executionContext.fireSelectEvent(
                SelectionEvent(
                    executionContext,
                    sourceNode,
                    *this,
                    XalanDOMString("select", executionContext.getMemoryManager()),
                    XalanDOMString(".", executionContext.getMemoryManager()),
                    executionContext.getXObjectFactory().createNodeSet(theNodeList)));
        }

        executionContext.cloneToResultTree(*sourceNode, getLocator());
    }
    else
    {
        const XObjectPtr    value(m_selectPattern->execute(*this, executionContext));
        assert(value.null() == false);

        if (0 != executionContext.getTraceListeners())
        {
            executionContext.fireSelectEvent(
                SelectionEvent(
                    executionContext,
                    sourceNode,
                    *this,
                    XalanDOMString("select", executionContext.getMemoryManager()),
                    *m_selectPattern,
                    value));
        }

        const XObject::eObjectType  type = value->getType();

        switch(type)
        {
        case XObject::eTypeBoolean:
        case XObject::eTypeNumber:
        case XObject::eTypeString:
            executionContext.characters(value);
            break;

        case XObject::eTypeNodeSet:
            {
                const NodeRefListBase&              theNodeList = value->nodeset();
                const NodeRefListBase::size_type    nChildren = theNodeList.getLength();

                for(NodeRefListBase::size_type i = 0; i < nChildren; i++)
                {
                    assert(theNodeList.item(i) != 0);

                    executionContext.cloneToResultTree(*theNodeList.item(i), getLocator());
                }
            }
            break;

        case XObject::eTypeResultTreeFrag:
            executionContext.outputResultTreeFragment(*value.get(), getLocator());
            break;

        default:
            executionContext.characters(value);
            break;
        }
    }
}
#endif



const XPath*
ElemCopyOf::getXPath(XalanSize_t    index) const
{
    return index == 0 ? m_selectPattern : 0;
}



XALAN_CPP_NAMESPACE_END
