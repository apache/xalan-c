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
#include "ElemUse.hpp"



#include <cassert>



#include "xercesc/sax/AttributeList.hpp"



#include "xalanc/PlatformSupport/StringTokenizer.hpp"



#include "Constants.hpp"
#include "ElemAttributeSet.hpp"
#include "StylesheetRoot.hpp"
#include "StylesheetConstructionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



/**
 * This acts as a superclass for ElemCopy, ElemAttributeSet,
 * ElemElement, and ElemLiteralResult, on order to implement 
 * shared behavior the use-attribute-sets attribute.
 */
ElemUse::ElemUse(
            StylesheetConstructionContext&  constructionContext,
            Stylesheet&                     stylesheetTree,
            XalanFileLoc                    lineNumber,
            XalanFileLoc                    columnNumber,
            int                             xslToken) :
    ElemTemplateElement(constructionContext,
                        stylesheetTree,
                        lineNumber,
                        columnNumber,
                        xslToken),
    m_attributeSetsNames(0),
    m_attributeSetsNamesCount(0)
{
}



ElemUse::~ElemUse()
{
}



const XalanDOMString&
ElemUse::getElementName() const
{
    return s_emptyString;
}



void
ElemUse::postConstruction(
            StylesheetConstructionContext&  constructionContext,
            const NamespacesHandler&        theParentHandler)
{
    if (m_attributeSetsNamesCount > 0)
    {
        canGenerateAttributes(true);
    }

    // OK, now we can chain-up...
    ElemTemplateElement::postConstruction(constructionContext, theParentHandler);
}


#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
const ElemTemplateElement*
ElemUse::startElement(StylesheetExecutionContext&   executionContext) const
{
    if (m_attributeSetsNamesCount > 0)
    {
        assert(canGenerateAttributes() == true);

        executionContext.pushInvoker(this);

        executionContext.createUseAttributeSetIndexesOnStack();

        return getNextAttributeSet(executionContext);
    }

    return 0;

}



void
ElemUse::endElement(StylesheetExecutionContext&     executionContext) const
{
    if (m_attributeSetsNamesCount > 0)
    {
        executionContext.popInvoker();

        executionContext.popUseAttributeSetIndexesFromStack();
    }
}



const ElemTemplateElement*
ElemUse::getNextChildElemToExecute(
            StylesheetExecutionContext&     executionContext,
            const ElemTemplateElement*      currentElem) const
{
    const ElemTemplateElement* nextElement = 0;
    
    if (m_attributeSetsNamesCount > 0)
    {
        nextElement = getNextAttributeSet(executionContext);
    }

    // no more attribute sets,  check for next child element
    if (0 == nextElement)
    {
        nextElement = ElemTemplateElement::getNextChildElemToExecute(executionContext, currentElem);
    }

    // if next element to execute is the first child after executing attribute set(s),  
    // evalute the element's AVTs first
    if (0 == nextElement && currentElem->getXSLToken() == StylesheetConstructionContext::ELEMNAME_ATTRIBUTE_SET)
    {
        evaluateAVTs(executionContext);
        nextElement = ElemTemplateElement::getFirstChildElemToExecute(executionContext);
    }

    return nextElement;
}



const ElemTemplateElement*
ElemUse::getFirstChildElemToExecute(StylesheetExecutionContext&     executionContext) const
{
    assert(executionContext.getCurrentNode() != 0);

    const ElemTemplateElement* nextElement = 0;

    if (getXSLToken() != StylesheetConstructionContext::ELEMNAME_COPY ||
        executionContext.getCurrentNode()->getNodeType() == XalanNode::ELEMENT_NODE)
    {
        if (m_attributeSetsNamesCount > 0)
        {
            // reset
            executionContext.getUseAttributeSetIndexes().attributeSetNameIndex = 0;
            executionContext.getUseAttributeSetIndexes().matchingAttributeSetIndex = 0;

            nextElement = getNextAttributeSet(executionContext);
        } 
        else 
        {
            evaluateAVTs(executionContext);
        }
    }

    if (0 == nextElement)
    {
        nextElement = ElemTemplateElement::getFirstChildElemToExecute(executionContext);
    }

    return nextElement;
}



const ElemTemplateElement*
ElemUse::getNextAttributeSet(
            StylesheetExecutionContext& executionContext) const
{
    const StylesheetRoot& theStylesheetRoot = getStylesheet().getStylesheetRoot();

    StylesheetExecutionContext::UseAttributeSetIndexes&     useAttributeSetIndexes =
        executionContext.getUseAttributeSetIndexes();

    const ElemTemplateElement* attributeSet = 0;

    while (0 == attributeSet &&
           useAttributeSetIndexes.attributeSetNameIndex < m_attributeSetsNamesCount)
    {
        attributeSet = theStylesheetRoot.getAttributeSet(
                executionContext,
                *m_attributeSetsNames[useAttributeSetIndexes.attributeSetNameIndex],
                 useAttributeSetIndexes.matchingAttributeSetIndex++,
                getLocator());

        if (0 == attributeSet)
        {

            useAttributeSetIndexes.attributeSetNameIndex++;
            useAttributeSetIndexes.matchingAttributeSetIndex = 0;
        }
    }

    return attributeSet;

}



void
ElemUse::evaluateAVTs(StylesheetExecutionContext&   /*executionContext*/) const
{
}
#endif


#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
void
ElemUse::execute(StylesheetExecutionContext&    executionContext) const
{
    doExecute(executionContext, true);
}



void
ElemUse::doExecute(
            StylesheetExecutionContext&     executionContext,
            bool                            applyAttributeSets) const
{
    assert(m_attributeSetsNamesCount == 0 || m_attributeSetsNames != 0);

    ElemTemplateElement::execute(executionContext);

    if(applyAttributeSets == true && m_attributeSetsNamesCount > 0)
    {
        assert(canGenerateAttributes() == true);

        const StylesheetRoot&   theStylesheetRoot = getStylesheet().getStylesheetRoot();
        const Locator* const    theLocator = getLocator();

        for(size_type i = 0; i < m_attributeSetsNamesCount; ++i)
        {
            theStylesheetRoot.executeAttributeSet(executionContext, *m_attributeSetsNames[i], theLocator);
        }
    }
}
#endif



bool
ElemUse::processUseAttributeSets(
            StylesheetConstructionContext&  constructionContext,
            const XalanDOMChar*             attrName,
            const AttributeListType&        atts,
            XalanSize_t                     which)
{
    bool isUAS = false;

    if(StylesheetConstructionContext::ELEMNAME_LITERAL_RESULT == getXSLToken())
    {
        isUAS = constructionContext.isXSLUseAttributeSetsAttribute(
            attrName,
            getStylesheet(),
            getLocator());
    }
    else
    {
        isUAS = equals(attrName, Constants::ATTRNAME_USEATTRIBUTESETS);
    }

    if(isUAS == true)
    {
        m_attributeSetsNames =
            constructionContext.tokenizeQNames(
                m_attributeSetsNamesCount,
                atts.getValue(which),
                getStylesheet().getNamespaces(),
                getLocator());
        assert(m_attributeSetsNamesCount == 0 || m_attributeSetsNames != 0);
    }

    return isUAS;
}



XALAN_CPP_NAMESPACE_END
