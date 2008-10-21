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
#include "ElemForEach.hpp"



#include <algorithm>
#include <cassert>



#include <xercesc/sax/AttributeList.hpp>



#include <xalanc/Include/STLHelper.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/XPath/XPath.hpp>



#include "ElemSort.hpp"
#include "NodeSorter.hpp"
#include "SelectionEvent.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



ElemForEach::ElemForEach(
            StylesheetConstructionContext&  constructionContext,
            Stylesheet&                     stylesheetTree,
            const AttributeListType&        atts,
            XalanFileLoc                    lineNumber,
            XalanFileLoc                    columnNumber) :
    ElemTemplateElement(constructionContext,
                        stylesheetTree,
                        lineNumber,
                        columnNumber,
                        StylesheetConstructionContext::ELEMNAME_FOR_EACH),
    m_selectPattern(0),
    m_sortElems(constructionContext.getMemoryManager()),
    m_sortElemsCount(0)
{
    const XalanSize_t  nAttrs = atts.getLength();
        
    for (XalanSize_t i = 0; i < nAttrs; i++)
    {
        const XalanDOMChar* const   aname = atts.getName(i);

        if (equals(aname, Constants::ATTRNAME_SELECT))
        {
            m_selectPattern = constructionContext.createXPath(getLocator(), atts.getValue(i), *this);
        }
        else if (isAttrOK(
                    aname,
                    atts,
                    i,
                    constructionContext) == false &&
                processSpaceAttr(
                    Constants::ELEMNAME_FOREACH_WITH_PREFIX_STRING.c_str(),
                    aname,
                    atts,
                    i,
                    constructionContext) == false)
        {
            error(
                constructionContext,
                XalanMessages::ElementHasIllegalAttribute_2Param,
                Constants::ELEMNAME_FOREACH_WITH_PREFIX_STRING.c_str(),
                aname);
        }
    }

    if (0 == m_selectPattern)
    {
        error(
            constructionContext,
            XalanMessages::ElementMustHaveAttribute_2Param,
            Constants::ELEMNAME_FOREACH_WITH_PREFIX_STRING,
            Constants::ATTRNAME_SELECT);
    }
}



ElemForEach::ElemForEach(
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
    m_selectPattern(0),
    m_sortElems(constructionContext.getMemoryManager()),
    m_sortElemsCount(0)
{
}



ElemForEach::~ElemForEach()
{
    XALAN_USING_STD(for_each)

     MemoryManager& theManager = m_sortElems.getMemoryManager();

    for_each(m_sortElems.begin(),
             m_sortElems.end(),
             DeleteFunctor<ElemSort>(theManager));
}



void
ElemForEach::processSortElement(
            StylesheetConstructionContext&  constructionContext,
            Stylesheet&                     theStylesheet,
            const AttributeListType&        atts,
            const Locator*                  locator)
{
    const XalanFileLoc  lineNumber = XalanLocator::getLineNumber(locator);
    const XalanFileLoc  columnNumber = XalanLocator::getColumnNumber(locator);

    m_sortElems.reserve(m_sortElems.size() + 1);

    ElemSort* sortElem = ElemSort::create(
        constructionContext.getMemoryManager(),
        constructionContext,
        theStylesheet,
        atts,
        lineNumber,
        columnNumber);

    m_sortElems.push_back(sortElem);
}



const XalanDOMString&
ElemForEach::getElementName() const
{
    return Constants::ELEMNAME_FOREACH_WITH_PREFIX_STRING;
}



void
ElemForEach::postConstruction(
            StylesheetConstructionContext&  constructionContext,
            const NamespacesHandler&        theParentHandler)
{
    ElemTemplateElement::postConstruction(constructionContext, theParentHandler);

    m_sortElemsCount = m_sortElems.size();
}



#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
const ElemTemplateElement*
ElemForEach::startElement(StylesheetExecutionContext&       executionContext) const
{
    assert(executionContext.getCurrentNode() != 0);

    if (hasChildren() == true)
    {
        executionContext.pushCurrentTemplate(0);
        const NodeRefListBase * nodeList = createSelectedAndSortedNodeList(
                executionContext);
        executionContext.createAndPushNodesToTransformList(nodeList);
        executionContext.pushContextNodeList(*nodeList);
        
        XalanNode* currentNode = executionContext.getNextNodeToTransform();
        if (currentNode == 0)
        {
            return 0;
        }
        executionContext.pushCurrentNode(currentNode);

        return beginExecuteChildren(executionContext);  
    }
    
    return 0;
}



const ElemTemplateElement*
ElemForEach::getNextChildElemToExecute(
                   StylesheetExecutionContext& executionContext,
                   const ElemTemplateElement* currentElem) const
{
    if (hasDirectTemplate() != true)
    {
        ElemTemplateElement* nextElement = currentElem->getNextSiblingElem();

        if (nextElement != 0)
        {
            return nextElement;
        }
    }

    executionContext.popCurrentNode();


    XalanNode * nextNode = executionContext.getNextNodeToTransform();

    if (nextNode == 0)
    {
        return 0;
    }

    executionContext.pushCurrentNode(nextNode);
    endExecuteChildren(executionContext);
    return beginExecuteChildren(executionContext);
}



void
ElemForEach::endElement(StylesheetExecutionContext&     executionContext) const
{
    if (hasChildren() == true)
    {
        // Children only executed if there were selected nodes
        if(executionContext.getContextNodeList().getLength() > 0)
        {
            endExecuteChildren(executionContext);
        }

        executionContext.popNodesToTransformList();
        executionContext.popContextNodeList();
        releaseSelectedAndSortedNodeList(executionContext);
        executionContext.popCurrentTemplate();
    }   
}



const NodeRefListBase*
ElemForEach::createSelectedAndSortedNodeList(
            StylesheetExecutionContext&     executionContext) const 
{
    assert(m_selectPattern != 0);

    typedef StylesheetExecutionContext::SetAndRestoreCurrentStackFrameIndex     SetAndRestoreCurrentStackFrameIndex;

    MutableNodeRefList& selectedNodeList = executionContext.createAndPushMutableNodeRefList();  

    XObjectPtr xobjectResult;
        
    const NodeRefListBase*  nodesToTransform = 0;

    {
        xobjectResult = m_selectPattern->execute(
                        *this,
                        executionContext,
                        selectedNodeList);

        if (xobjectResult.null() == true)
        {
            nodesToTransform = &selectedNodeList;
        }
        else
        {
            nodesToTransform = &xobjectResult->nodeset();
        }
    }
    executionContext.pushXObjectPtr(xobjectResult);

    if(0 != executionContext.getTraceListeners())
    {
        executionContext.fireSelectEvent(
                SelectionEvent(
                    executionContext, 
                    executionContext.getCurrentNode(),
                    *this,
                    XalanDOMString("select", executionContext.getMemoryManager()),
                    *m_selectPattern,
                    *nodesToTransform));
    }

    if (m_sortElemsCount > 0) 
    {
        MutableNodeRefList& sortedNodeList = executionContext.createAndPushMutableNodeRefList();

        if (nodesToTransform->getLength() > 1)
        {
            nodesToTransform = sortChildren(
                    executionContext,
                    *nodesToTransform,
                    sortedNodeList);
        }
    }

    return nodesToTransform;
}



void
ElemForEach::releaseSelectedAndSortedNodeList(
        StylesheetExecutionContext& executionContext) const
{
    executionContext.popXObjectPtr();
    executionContext.releaseAndPopMutableNodeRefList();
    if (m_sortElemsCount > 0)
    {
        executionContext.releaseAndPopMutableNodeRefList();
    }
}



const NodeRefListBase*
ElemForEach::sortChildren(  
            StylesheetExecutionContext&     executionContext,
            const NodeRefListBase&          selectedNodeList,
            MutableNodeRefList&             sortedNodeList) const
{
    typedef NodeSorter::NodeSortKeyVectorType                   NodeSortKeyVectorType;
    typedef StylesheetExecutionContext::SetAndRestoreCurrentStackFrameIndex     SetAndRestoreCurrentStackFrameIndex;
    typedef StylesheetExecutionContext::ContextNodeListPushAndPop               ContextNodeListPushAndPop;
    
    NodeSorter* sorter = executionContext.getNodeSorter();

    NodeSortKeyVectorType&  keys = sorter->getSortKeys();
    assert(keys.empty() == true);

    CollectionClearGuard<NodeSortKeyVectorType>     guard(keys);

    // Reserve the space now...
    keys.reserve(m_sortElemsCount);

    // Get some temporary strings to use for evaluting the AVTs...
    const StylesheetExecutionContext::GetCachedString   theTemp1(executionContext);

    XalanDOMString&     langString = theTemp1.get();

    const StylesheetExecutionContext::GetCachedString   theTemp2(executionContext);

    XalanDOMString&     scratchString = theTemp2.get();

    // March backwards, performing a sort on each xsl:sort child.
    // Probably not the most efficient method.
    for(SortElemsVectorType::size_type  i = 0; i < m_sortElemsCount; i++)
    {
        const ElemSort* const   sort = m_sortElems[i];
        assert(sort != 0);

        const AVT* avt = sort->getLangAVT();

        if(0 != avt)
        {
            avt->evaluate(langString, *this, executionContext);
        }

        avt = sort->getDataTypeAVT();

        if(0 != avt)
        {
            avt->evaluate(scratchString, *this, executionContext);
        }           

        bool    treatAsNumbers = false;

        if (scratchString.empty() == false)
        {
            if (equals(scratchString, Constants::ATTRVAL_DATATYPE_NUMBER) == true)
            {
                treatAsNumbers = true;
            }
            else if (equals(scratchString, Constants::ATTRVAL_DATATYPE_TEXT) == false)
            {
                const XalanQNameByValue     theQName(scratchString, executionContext.getMemoryManager(), this);

                if (theQName.getNamespace().length() == 0)
                {
                    error(
                        executionContext,
                        XalanMessages::SortDataTypeMustBe,
                        sort->getLocator());
                }
                else
                {
                    warn(
                        executionContext,
                        XalanMessages::SortHasUnknownDataType,
                        sort->getLocator());
                }
            }
        }

        scratchString.clear();

        avt = sort->getOrderAVT();

        if(0 != avt)
        {
            avt->evaluate(scratchString, *this, executionContext);
        }           

        bool    descending = false;
        
        if (scratchString.empty() == false)
        {
            if (equals(scratchString, Constants::ATTRVAL_ORDER_DESCENDING) == true)
            {
                descending = true;
            }
            else if (equals(scratchString, Constants::ATTRVAL_ORDER_ASCENDING) == false)
            {
                error(
                    executionContext,
                    XalanMessages::SortMustBeAscendOrDescend,
                    sort->getLocator());
            }
        }

        scratchString.clear();

        avt = sort->getCaseOrderAVT();

        if(0 != avt)
        {
            avt->evaluate(scratchString, *this, executionContext);
        }           

        XalanCollationServices::eCaseOrder  caseOrder = XalanCollationServices::eDefault;

        if (scratchString.empty() == false)
        {
            if (equals(scratchString, Constants::ATTRVAL_CASEORDER_UPPER) == true)
            {
                caseOrder = XalanCollationServices::eUpperFirst;
            }
            else if (equals(scratchString, Constants::ATTRVAL_CASEORDER_LOWER) == true)
            {
                caseOrder = XalanCollationServices::eLowerFirst;
            }
            else
            {
                error(
                    executionContext,
                    XalanMessages::SortCaseOrderMustBe,
                    sort->getLocator());
            }
        }

        scratchString.clear();

        keys.push_back(
                NodeSortKey(
                    executionContext,
                    sort->getSelectPattern(),
                    treatAsNumbers,
                    descending,
                    caseOrder,
                    langString,
                    *this));
    }

    sortedNodeList = selectedNodeList;

    {
        ContextNodeListPushAndPop   theContextNodeListPushAndPop(
                executionContext,
                selectedNodeList);

        sorter->sort(executionContext, sortedNodeList);
    }

    return &sortedNodeList;
}
#endif



#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
void
ElemForEach::execute(StylesheetExecutionContext&    executionContext) const
{
    assert(m_selectPattern != 0);
    assert(executionContext.getCurrentNode() != 0);

    StylesheetExecutionContext::PushAndPopCurrentTemplate   thePushAndPop(executionContext, 0);

    if (hasChildren() == true)
    {
        transformSelectedChildren(
            executionContext,
            this);
    }
}



void
ElemForEach::transformSelectedChildren(
            StylesheetExecutionContext&     executionContext,
            const ElemTemplateElement*      theTemplate) const
{
    assert(m_selectPattern != 0);
    assert(m_sortElemsCount == m_sortElems.size());

    if (m_sortElemsCount == 0)
    {
        selectAndSortChildren(
                    executionContext,
                    theTemplate,
                    0,
                    executionContext.getCurrentStackFrameIndex());
    }
    else
    {
        typedef NodeSorter::NodeSortKeyVectorType                   NodeSortKeyVectorType;
        typedef StylesheetExecutionContext::BorrowReturnNodeSorter  BorrowReturnNodeSorter;

        BorrowReturnNodeSorter  sorter(executionContext);

        NodeSortKeyVectorType&  keys = sorter->getSortKeys();
        assert(keys.empty() == true);

        CollectionClearGuard<NodeSortKeyVectorType>     guard(keys);

        // Reserve the space now...
        keys.reserve(m_sortElemsCount);

        // Get some temporary strings to use for evaluting the AVTs...
        StylesheetExecutionContext::GetCachedString     theTemp1(executionContext);

        XalanDOMString&     langString = theTemp1.get();

        const StylesheetExecutionContext::GetCachedString   theTemp2(executionContext);

        XalanDOMString&     scratchString = theTemp2.get();

        // March backwards, performing a sort on each xsl:sort child.
        // Probably not the most efficient method.
        for(SortElemsVectorType::size_type  i = 0; i < m_sortElemsCount; i++)
        {
            const ElemSort* const   sort = m_sortElems[i];
            assert(sort != 0);

            const AVT* avt = sort->getLangAVT();

            if(0 != avt)
            {
                avt->evaluate(langString, *this, executionContext);
            }

            avt = sort->getDataTypeAVT();

            if(0 != avt)
            {
                avt->evaluate(scratchString, *this, executionContext);
            }           

            bool    treatAsNumbers = false;

            if (scratchString.empty() == false)
            {
                if (equals(scratchString, Constants::ATTRVAL_DATATYPE_NUMBER) == true)
                {
                    treatAsNumbers = true;
                }
                else if (equals(scratchString, Constants::ATTRVAL_DATATYPE_TEXT) == false)
                {
                    const XalanQNameByValue     theQName(scratchString, executionContext.getMemoryManager(), this);

                    if (theQName.getNamespace().length() == 0)
                    {
                        error(
                            executionContext,
                            XalanMessages::SortDataTypeMustBe);
                    }
                    else
                    {
                        warn(
                            executionContext,
                            XalanMessages::SortHasUnknownDataType);
                    }
                }
            }

            clear(scratchString);

            avt = sort->getOrderAVT();

            if(0 != avt)
            {
                avt->evaluate(scratchString, *this, executionContext);
            }           

            bool    descending = false;
            
            if (scratchString.empty() == false)
            {
                if (equals(scratchString, Constants::ATTRVAL_ORDER_DESCENDING) == true)
                {
                    descending = true;
                }
                else if (equals(scratchString, Constants::ATTRVAL_ORDER_ASCENDING) == false)
                {
                    error(
                        executionContext,
                        XalanMessages::SortMustBeAscendOrDescend);
                }
            }

            clear(scratchString);

            avt = sort->getCaseOrderAVT();

            if(0 != avt)
            {
                avt->evaluate(scratchString, *this, executionContext);
            }           

            XalanCollationServices::eCaseOrder  caseOrder = XalanCollationServices::eDefault;

            if (scratchString.empty() == false)
            {
                if (equals(scratchString, Constants::ATTRVAL_CASEORDER_UPPER) == true)
                {
                    caseOrder = XalanCollationServices::eUpperFirst;
                }
                else if (equals(scratchString, Constants::ATTRVAL_CASEORDER_LOWER) == true)
                {
                    caseOrder = XalanCollationServices::eLowerFirst;
                }
                else
                {
                    error(
                        executionContext,
                        XalanMessages::SortCaseOrderMustBe);
                }
            }

            clear(scratchString);

            keys.push_back(
                    NodeSortKey(
                        executionContext,
                        sort->getSelectPattern(),
                        treatAsNumbers,
                        descending,
                        caseOrder,
                        langString,
                        *this));
        }

        selectAndSortChildren(
                    executionContext,
                    theTemplate,
                    sorter.get(),
                    executionContext.getCurrentStackFrameIndex());
    }
}



void
ElemForEach::selectAndSortChildren(
            StylesheetExecutionContext&     executionContext,
            const ElemTemplateElement*      theTemplate,
            NodeSorter*                     sorter,
            int                             selectStackFrameIndex) const
{
    typedef StylesheetExecutionContext::SetAndRestoreCurrentStackFrameIndex     SetAndRestoreCurrentStackFrameIndex;

    assert(m_selectPattern != 0);

    typedef XPathExecutionContext::BorrowReturnMutableNodeRefList   BorrowReturnMutableNodeRefList;

    BorrowReturnMutableNodeRefList  theGuard(executionContext);

    const NodeRefListBase*  sourceNodes = 0;

    XObjectPtr              xobjectResult;

    {
        SetAndRestoreCurrentStackFrameIndex     theSetAndRestore(
                    executionContext,
                    selectStackFrameIndex);

        xobjectResult = m_selectPattern->execute(
                        *this,
                        executionContext,
                        *theGuard);

        if (xobjectResult.null() == true)
        {
            sourceNodes = &*theGuard;
        }
        else
        {
            theGuard.release();

            sourceNodes = &xobjectResult->nodeset();
        }
    }

    if(0 != executionContext.getTraceListeners())
    {
        executionContext.fireSelectEvent(
                SelectionEvent(
                    executionContext, 
                    executionContext.getCurrentNode(),
                    *this,
                    XalanDOMString("select", executionContext.getMemoryManager()),
                    *m_selectPattern,
                    *sourceNodes));
    }

    const NodeRefListBase::size_type    nNodes = sourceNodes->getLength();

    if (nNodes > 0)
    {
        // If there's not NodeSorter, or we've only selected one node,
        // then just do the transform...
        if (sorter == 0 || nNodes == 1)
        {
            transformSelectedChildren(
                executionContext,
                theTemplate,
                *sourceNodes,
                nNodes);
        }
        else
        {
            typedef StylesheetExecutionContext::SetAndRestoreCurrentStackFrameIndex     SetAndRestoreCurrentStackFrameIndex;
            typedef StylesheetExecutionContext::ContextNodeListPushAndPop               ContextNodeListPushAndPop;
            typedef StylesheetExecutionContext::BorrowReturnMutableNodeRefList          BorrowReturnMutableNodeRefList;

            BorrowReturnMutableNodeRefList  sortedSourceNodes(executionContext);

            *sortedSourceNodes = *sourceNodes;

            {
                SetAndRestoreCurrentStackFrameIndex     theStackFrameSetAndRestore(
                        executionContext,
                        selectStackFrameIndex);

                ContextNodeListPushAndPop   theContextNodeListPushAndPop(
                        executionContext,
                        *sourceNodes);

                sorter->sort(executionContext, *sortedSourceNodes);
            }

            transformSelectedChildren(
                executionContext,
                theTemplate,
                *sortedSourceNodes,
                nNodes);
        }
    }
}



void
ElemForEach::transformSelectedChildren(
            StylesheetExecutionContext&     executionContext,
            const ElemTemplateElement*      theTemplate,
            const NodeRefListBase&          sourceNodes,
            NodeRefListBase::size_type      sourceNodesCount) const
{
    if(executionContext.getTraceSelects() == true)
    {
        executionContext.traceSelect(
            *this,
            sourceNodes,
            m_selectPattern);
    }

    // Create an object to set and restore the context node list...
    const StylesheetExecutionContext::ContextNodeListPushAndPop     theContextNodeLisPushAndPop(
                executionContext,
                sourceNodes);

    for(NodeRefListBase::size_type i = 0; i < sourceNodesCount; i++) 
    {
        XalanNode* const        childNode = sourceNodes.item(i);
        assert(childNode != 0);

        transformChild(
                executionContext,
                *this,
                theTemplate,
                childNode);
    }
}
#endif



const XPath*
ElemForEach::getXPath(XalanSize_t   index) const
{
    return index == 0 ? m_selectPattern : 0;
}



XALAN_CPP_NAMESPACE_END
