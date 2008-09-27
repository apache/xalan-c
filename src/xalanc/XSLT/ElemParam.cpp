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
#include "ElemParam.hpp"



#include <xalanc/XPath/XObjectFactory.hpp>



#include "Constants.hpp"
#include "Stylesheet.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"
#include "StylesheetRoot.hpp"
#include "TracerEvent.hpp"



XALAN_CPP_NAMESPACE_BEGIN



ElemParam::ElemParam(
            StylesheetConstructionContext&  constructionContext,
            Stylesheet&                     stylesheetTree,
            const AttributeListType&        atts,
            XalanFileLoc                    lineNumber, 
            XalanFileLoc                    columnNumber) :
    ElemVariable(constructionContext,
                 stylesheetTree,
                 atts,
                 lineNumber,
                 columnNumber,
                 StylesheetConstructionContext::ELEMNAME_PARAM)
{
}



const XalanDOMString&
ElemParam::getElementName() const
{
    return Constants::ELEMNAME_PARAM_WITH_PREFIX_STRING;
}



#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
const ElemTemplateElement*
ElemParam::startElement(StylesheetExecutionContext&     executionContext) const
{
    assert(m_qname != 0);

    const XObjectPtr    obj = executionContext.getParamVariable(*m_qname);

    // If not found, evaluate as variable for default value
    if (obj.null() == true)
    {
        return ElemVariable::startElement(executionContext);
    }
    else
    {
        if (0 != executionContext.getTraceListeners())
        {
            executionContext.fireTraceEvent(
                TracerEvent(
                    executionContext,                   
                    *this));
        }
    }
    return 0;
}



void
ElemParam::endElement(StylesheetExecutionContext&       executionContext) const
{
    assert(m_qname != 0);

    const XObjectPtr    obj = executionContext.getParamVariable(*m_qname);

    // If not found, evaluate as variable for default value
    if (obj.null() == true)
    {
        ElemVariable::endElement(executionContext);
    }
}
#endif



#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
void
ElemParam::execute(StylesheetExecutionContext&      executionContext) const
{
    assert(m_qname != 0);

    const XObjectPtr    obj = executionContext.getParamVariable(*m_qname);

    if (obj.null() == true)
    {
        ElemVariable::execute(executionContext);
    }
    else
    {
        if (0 != executionContext.getTraceListeners())
        {
            executionContext.fireTraceEvent(
                TracerEvent(
                    executionContext,                   
                    *this));
        }
    }
}
#endif



XALAN_CPP_NAMESPACE_END
