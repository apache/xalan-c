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
#include "SelectionEvent.hpp"



#include <xalanc/XPath/XPath.hpp>



XALAN_CPP_NAMESPACE_BEGIN



SelectionEvent::SelectionEvent(
            StylesheetExecutionContext&     executionContext,
            const XalanNode*                sourceNode,
            const ElemTemplateElement&      styleNode,
            const XalanDOMString&           attributeName,
            const XPath&                    xpath,
            const XObjectPtr                selection) :
    m_executionContext(executionContext),
    m_sourceNode(sourceNode),
    m_styleNode(styleNode),
    m_attributeName(attributeName),
    m_xpathExpression(xpath.getExpression().getCurrentPattern()),
    m_selection(selection),
    m_type(selection.null() == true ? eNone : eUnknown),
    m_boolean(false),
    m_nodeList(0)
{
}



SelectionEvent::SelectionEvent(
            StylesheetExecutionContext&     executionContext,
            const XalanNode*                sourceNode,
            const ElemTemplateElement&      styleNode,
            const XalanDOMString&           attributeName,
            const XalanDOMString&           xpathExpression,
            const XObjectPtr                selection) :
    m_executionContext(executionContext),
    m_sourceNode(sourceNode),
    m_styleNode(styleNode),
    m_attributeName(attributeName),
    m_xpathExpression(xpathExpression),
    m_selection(selection),
    m_type(selection.null() == true ? eNone : eUnknown),
    m_boolean(false),
    m_nodeList(0)
{
}



SelectionEvent::SelectionEvent(
            StylesheetExecutionContext&     executionContext,
            const XalanNode*                sourceNode,
            const ElemTemplateElement&      styleNode,
            const XalanDOMString&           attributeName,
            const XPath&                    xpath,
            bool                            selection) :
    m_executionContext(executionContext),
    m_sourceNode(sourceNode),
    m_styleNode(styleNode),
    m_attributeName(attributeName),
    m_xpathExpression(xpath.getExpression().getCurrentPattern()),
    m_selection(),
    m_type(eBoolean),
    m_boolean(selection),
    m_nodeList(0)
{
}



SelectionEvent::SelectionEvent(
            StylesheetExecutionContext&     executionContext,
            const XalanNode*                sourceNode,
            const ElemTemplateElement&      styleNode,
            const XalanDOMString&           attributeName,
            const XPath&                    xpath,
            const NodeRefListBase&          selection) :
    m_executionContext(executionContext),
    m_sourceNode(sourceNode),
    m_styleNode(styleNode),
    m_attributeName(attributeName),
    m_xpathExpression(xpath.getExpression().getCurrentPattern()),
    m_selection(),
    m_type(eNodeSet),
    m_boolean(false),
    m_nodeList(&selection)
{
}



SelectionEvent::~SelectionEvent()
{
}



XALAN_CPP_NAMESPACE_END
