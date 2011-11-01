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

#include "StylesheetExecutionContextDefault.hpp"



#include <algorithm>
#include <cassert>



#include <xalanc/Include/STLHelper.hpp>



#include <xalanc/PlatformSupport/DOMStringPrintWriter.hpp>
#include <xalanc/PlatformSupport/XalanOutputStream.hpp>
#include <xalanc/PlatformSupport/XalanNumberFormat.hpp>
#include <xalanc/PlatformSupport/XalanOutputStreamPrintWriter.hpp>
#include <xalanc/PlatformSupport/XalanStdOutputStream.hpp>
#include <xalanc/PlatformSupport/XalanFileOutputStream.hpp>
#include <xalanc/PlatformSupport/XalanFStreamOutputStream.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>
#include <xalanc/PlatformSupport/XalanTranscodingServices.hpp>



#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/XPath.hpp>
#include <xalanc/XPath/XPathEnvSupport.hpp>
#include <xalanc/XPath/XPathExecutionContext.hpp>
#include <xalanc/XPath/XObject.hpp>



#include <xalanc/XMLSupport/XalanXMLSerializerFactory.hpp>
#include <xalanc/XMLSupport/FormatterToHTML.hpp>
#include <xalanc/XMLSupport/FormatterToText.hpp>
#include <xalanc/XMLSupport/XMLParserLiaison.hpp>



#include <xalanc/XalanSourceTree/FormatterToSourceTree.hpp>
#include <xalanc/XalanSourceTree/XalanSourceTreeDocument.hpp>
#include <xalanc/XalanSourceTree/XalanSourceTreeDocumentFragment.hpp>



#include "Constants.hpp"
#include "ElemTemplateElement.hpp"
#include "ElemWithParam.hpp"
#include "KeyTable.hpp"
#include "StylesheetConstructionContextDefault.hpp"
#include "StylesheetRoot.hpp"
#include "XSLTEngineImpl.hpp"
#include "XSLTProcessorException.hpp"



//#define XALAN_VQ_SPECIAL_TRACE
#if defined(XALAN_VQ_SPECIAL_TRACE)
#include "C:/Program Files/Rational/Quantify/pure.h"
#endif



XALAN_CPP_NAMESPACE_BEGIN



StylesheetExecutionContextDefault::XalanNumberFormatFactory         StylesheetExecutionContextDefault::s_defaultXalanNumberFormatFactory;

StylesheetExecutionContextDefault::XalanNumberFormatFactory*        StylesheetExecutionContextDefault::s_xalanNumberFormatFactory =
        &StylesheetExecutionContextDefault::getDefaultXalanNumberFormatFactory();

const StylesheetExecutionContextDefault::DefaultCollationCompareFunctor     StylesheetExecutionContextDefault::s_defaultCollationFunctor;



StylesheetExecutionContextDefault::StylesheetExecutionContextDefault(
            MemoryManager&      theManager,
            XSLTEngineImpl&         xsltProcessor,
            XPathEnvSupport&        theXPathEnvSupport,
            DOMSupport&             theDOMSupport,
            XObjectFactory&         theXObjectFactory,
            XalanNode*              theCurrentNode,
            const NodeRefListBase*  theContextNodeList,
            const PrefixResolver*   thePrefixResolver) :
    StylesheetExecutionContext(theManager, &theXObjectFactory),
    m_xpathExecutionContextDefault(theXPathEnvSupport,
                                   theDOMSupport,
                                   theXObjectFactory,
                                   theCurrentNode,
                                   theContextNodeList,
                                   thePrefixResolver),
    m_xsltProcessor(&xsltProcessor),
    m_rootDocument(0),
    m_elementRecursionStack(theManager),
    m_stylesheetRoot(0),
    m_formatterListeners(theManager),
    m_printWriters(theManager),
    m_outputStreams(theManager),
    m_collationCompareFunctor(0),
    m_formatNumberFunctor(0),
    m_variablesStack(theManager),
    m_paramsVector(theManager),
    m_matchPatternCache(theManager),
    m_keyTables(theManager),
    m_countersTable(theManager),
    m_sourceTreeResultTreeFactory(),
    m_mode(0),
    m_currentTemplateStack(theManager),
    m_indentAmount(-1),
    m_xresultTreeFragAllocator(theManager, eXResultTreeFragAllocatorBlockSize),
    m_documentFragmentAllocator(theManager, eDocumentFragmentAllocatorBlockSize),
    m_documentAllocator(theManager, eDocumentAllocatorBlockSize),
    m_copyTextNodesOnlyStack(theManager),
    m_modeStack(theManager),
    m_currentIndexStack(theManager),
#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
    m_xobjectPtrStack(theManager),
    m_mutableNodeRefListStack(theManager),
    m_nodesToTransformStack(theManager),
    m_processCurrentAttributeStack(theManager),
    m_executeIfStack(theManager),
    m_stringStack(theManager),
    m_formatterToTextStack(theManager),
    m_skipElementAttributesStack(theManager),
    m_formatterToSourceTreeStack(theManager),
    m_paramsVectorStack(theManager),
    m_elementInvokerStack(theManager),
    m_useAttributeSetIndexesStack(theManager),
    m_nodeSorter(theManager),
#else
    m_formatterToTextCache(theManager),
    m_formatterToSourceTreeCache(theManager),
    m_nodeSorterCache(theManager),
#endif
    m_usePerInstanceDocumentFactory(false),
    m_escapeURLs(eEscapeURLsDefault),
    m_omitMETATag(eOmitMETATagDefault)
{
    m_currentTemplateStack.push_back(0);
}



StylesheetExecutionContextDefault::StylesheetExecutionContextDefault(
            MemoryManager&          theManager,
            XalanNode*              theCurrentNode,
            const NodeRefListBase*  theContextNodeList,
            const PrefixResolver*   thePrefixResolver) :
    StylesheetExecutionContext(theManager),
    m_xpathExecutionContextDefault(theManager,
                                   theCurrentNode,
                                   theContextNodeList,
                                   thePrefixResolver),
    m_xsltProcessor(0),
    m_rootDocument(0),
    m_elementRecursionStack(theManager),
    m_stylesheetRoot(0),
    m_formatterListeners(theManager),
    m_printWriters(theManager),
    m_outputStreams(theManager),
    m_collationCompareFunctor(0),
    m_formatNumberFunctor(0),
    m_variablesStack(theManager),
    m_paramsVector(theManager),
    m_matchPatternCache(theManager),
    m_keyTables(theManager),
    m_countersTable(theManager),
    m_sourceTreeResultTreeFactory(),
    m_mode(0),
    m_currentTemplateStack(theManager),
    m_indentAmount(-1),
    m_xresultTreeFragAllocator(theManager, eXResultTreeFragAllocatorBlockSize),
    m_documentFragmentAllocator(theManager, eDocumentFragmentAllocatorBlockSize),
    m_documentAllocator(theManager, eDocumentAllocatorBlockSize),
    m_copyTextNodesOnlyStack(theManager),
    m_modeStack(theManager),
    m_currentIndexStack(theManager),
#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
    m_xobjectPtrStack(theManager),
    m_mutableNodeRefListStack(theManager),
    m_nodesToTransformStack(theManager),
    m_processCurrentAttributeStack(theManager),
    m_executeIfStack(theManager),
    m_stringStack(theManager),
    m_formatterToTextStack(theManager),
    m_skipElementAttributesStack(theManager),
    m_formatterToSourceTreeStack(theManager),
    m_paramsVectorStack(theManager),
    m_elementInvokerStack(theManager),
    m_useAttributeSetIndexesStack(theManager),
    m_nodeSorter(theManager),
#else
    m_formatterToTextCache(theManager),
    m_formatterToSourceTreeCache(theManager),
    m_nodeSorterCache(theManager),
#endif
    m_usePerInstanceDocumentFactory(false),
    m_escapeURLs(eEscapeURLsDefault),
    m_omitMETATag(eOmitMETATagDefault)
{
    m_currentTemplateStack.push_back(0);
}



StylesheetExecutionContextDefault*
StylesheetExecutionContextDefault::create(
            MemoryManager&          theManager,
            XalanNode*              theCurrentNode,
            const NodeRefListBase*  theContextNodeList,
            const PrefixResolver*   thePrefixResolver)
{
    typedef StylesheetExecutionContextDefault   ThisType;

    XalanAllocationGuard    theGuard(theManager, theManager.allocate(sizeof(ThisType)));

    ThisType* const     theResult =
        new (theGuard.get()) ThisType(
                                theManager,
                                theCurrentNode,
                                theContextNodeList,
                                thePrefixResolver);
    theGuard.release();

    return theResult;
}



StylesheetExecutionContextDefault::~StylesheetExecutionContextDefault()
{
    reset();
}



void
StylesheetExecutionContextDefault::problem(
            eSource                 source,
            eClassification         classification,
            const XalanDOMString&   msg,
            const Locator*          locator,
            const XalanNode*        sourceNode)
{
    m_xsltProcessor->problem(
        source,
        classification,
        msg,
        locator,
        sourceNode);
}



void
StylesheetExecutionContextDefault::problem(
            eSource                 source,
            eClassification         classification,
            const XalanDOMString&   msg,
            const XalanNode*        sourceNode)
{
    m_xsltProcessor->problem(
        source,
        classification,
        msg,
        sourceNode);
}



bool
StylesheetExecutionContextDefault::getQuietConflictWarnings() const
{
    assert(m_xsltProcessor != 0);

    return m_xsltProcessor->getQuietConflictWarnings();
}



bool
StylesheetExecutionContextDefault::getCopyTextNodesOnly() const
{
    if (m_copyTextNodesOnlyStack.size() == 0)
    {
        return false;
    }
    else
    {
        return m_copyTextNodesOnlyStack.back();
    }
}





XalanNode*
StylesheetExecutionContextDefault::getRootDocument() const
{
    return m_rootDocument;
}



void
StylesheetExecutionContextDefault::setRootDocument(XalanNode*   theDocument)
{
    m_rootDocument = theDocument;
}



void
StylesheetExecutionContextDefault::setStylesheetRoot(const StylesheetRoot*  theStylesheet)
{
    assert(m_xsltProcessor != 0);

    m_stylesheetRoot = theStylesheet;

    m_hasPreserveOrStripConditions = theStylesheet->hasPreserveOrStripSpaceElements();

    m_xsltProcessor->setStylesheetRoot(theStylesheet);

    if (theStylesheet == 0)
    {
        m_xsltProcessor->setExecutionContext(0);
    }
    else
    {
        m_xsltProcessor->setExecutionContext(this);

        m_countersTable.resize(theStylesheet->getElemNumberCount());
    }
}



const XalanQName*
StylesheetExecutionContextDefault::getCurrentMode() const
{
    if (m_modeStack.size() == 0) 
    {
        return 0;
    }
    else
    {
        return m_modeStack.back();
    }
}



void
StylesheetExecutionContextDefault::pushCurrentMode(const XalanQName*        theMode)
{       
    m_modeStack.push_back(theMode);
}



void
StylesheetExecutionContextDefault::popCurrentMode()
{       
    m_modeStack.pop_back();
}




const ElemTemplate*
StylesheetExecutionContextDefault::getCurrentTemplate() const
{
    return m_currentTemplateStack.back();
}



void
StylesheetExecutionContextDefault::pushCurrentTemplate(const ElemTemplate*  theTemplate)
{       
    m_currentTemplateStack.push_back(theTemplate);
}



void
StylesheetExecutionContextDefault::popCurrentTemplate()
{       
    m_currentTemplateStack.pop_back();
}



bool
StylesheetExecutionContextDefault::isElementPending() const
{
    assert(m_xsltProcessor != 0);

    return m_xsltProcessor->isElementPending();
}



void
StylesheetExecutionContextDefault::replacePendingAttribute(
            const XalanDOMChar*     theName,
            const XalanDOMChar*     theNewType,
            const XalanDOMChar*     theNewValue)
{
    assert(m_xsltProcessor != 0);

    // Remove the old attribute, then add the new one.  AttributeListImpl::addAttribute()
    // does this for us.
    m_xsltProcessor->replacePendingAttribute(theName, theNewType, theNewValue);
}



void
StylesheetExecutionContextDefault::pushOutputContext(FormatterListener*     flistener)
{
    assert(m_xsltProcessor != 0);

    m_xsltProcessor->pushOutputContext(flistener);
}



void
StylesheetExecutionContextDefault::popOutputContext()
{
    assert(m_xsltProcessor != 0);

    m_xsltProcessor->popOutputContext();
}



void
StylesheetExecutionContextDefault::addResultAttribute(
            const XalanDOMString&   aname,
            const XalanDOMString&   value)
{
    assert(m_xsltProcessor != 0);

    m_xsltProcessor->addResultAttribute(aname, value);
}



void
StylesheetExecutionContextDefault::addResultAttribute(
            const XalanDOMString&   aname,
            const XalanDOMChar*     value)
{
    assert(m_xsltProcessor != 0);

    m_xsltProcessor->addResultAttribute(aname, value);
}



void
StylesheetExecutionContextDefault::copyNamespaceAttributes(const XalanNode&     src)
{
    assert(m_xsltProcessor != 0);

    m_xsltProcessor->copyNamespaceAttributes(src);
}



const XalanDOMString*
StylesheetExecutionContextDefault::getResultPrefixForNamespace(const XalanDOMString&    theNamespace) const
{
    assert(m_xsltProcessor != 0);

    return m_xsltProcessor->getResultPrefixForNamespace(theNamespace);
}



const XalanDOMString*
StylesheetExecutionContextDefault::getResultNamespaceForPrefix(const XalanDOMString&    thePrefix) const
{
    assert(m_xsltProcessor != 0);

    return m_xsltProcessor->getResultNamespaceForPrefix(thePrefix);
}



bool
StylesheetExecutionContextDefault::isPendingResultPrefix(const XalanDOMString&  thePrefix)
{
    assert(m_xsltProcessor != 0);

    return m_xsltProcessor->isPendingResultPrefix(thePrefix);
}


void
StylesheetExecutionContextDefault::getUniqueNamespaceValue(XalanDOMString&  theValue) const
{
    assert(m_xsltProcessor != 0);

    m_xsltProcessor->getUniqueNamespaceValue(theValue);
}



FormatterListener*
StylesheetExecutionContextDefault::getFormatterListener() const
{
    assert(m_xsltProcessor != 0);

    return m_xsltProcessor->getFormatterListener();
}



void
StylesheetExecutionContextDefault::setFormatterListener(FormatterListener*  flistener)
{
    assert(m_xsltProcessor != 0);

    m_xsltProcessor->setFormatterListener(flistener);
}



int
StylesheetExecutionContextDefault::getIndent() const
{
    if (m_indentAmount != -1)
    {
        return m_indentAmount;
    }
    else
    {
        assert(m_xsltProcessor != 0);

        return m_xsltProcessor->getXMLParserLiaison().getIndent();
    }
}



void
StylesheetExecutionContextDefault::setIndent(int    indentAmount)
{
    m_indentAmount = indentAmount;
}



const XPath*
StylesheetExecutionContextDefault::createMatchPattern(
            const XalanDOMString&   str,
            const PrefixResolver&   resolver)
{
    assert(m_xsltProcessor != 0);

    const XPath*    theResult = 0;

    // We won't cache any xpath that has a namespace, since
    // we have no idea how that might be resolved.  We could
    // enhance XPath so that we can tell if str would match
    // the XPath, once the namespace is resolved, but it may
    // not be worth it...
    const XalanDOMString::size_type     index = indexOf(str, XalanUnicode::charColon);
    const XalanDOMString::size_type     len = str.length();

    // If we found a ':' before the end of the string, and
    // it's by itself (:: would indicate an axis), don't
    // try to cache the XPath...
    if (index < len - 1 && str[index + 1] != XalanUnicode::charColon)
    {
        theResult = m_xsltProcessor->createMatchPattern(str, resolver);
    }
    else
    {
        const XPathCacheMapType::iterator   i =
            m_matchPatternCache.find(str);

        if (i != m_matchPatternCache.end())
        {
            // Update hit time...
#if defined(XALAN_STRICT_ANSI_HEADERS)
            (*i).second.second = std::clock();
#else
            (*i).second.second = clock();
#endif

            theResult = (*i).second.first;
        }
        else
        {
            theResult = m_xsltProcessor->createMatchPattern(str, resolver);

            addToXPathCache(str, theResult);
        }
    }

    return theResult;
}



void
StylesheetExecutionContextDefault::returnXPath(const XPath*     xpath)
{
    assert(m_xsltProcessor != 0);

    if (isCached(xpath) == false)
    {
        m_xsltProcessor->returnXPath(xpath);
    }
}



void
StylesheetExecutionContextDefault::pushTopLevelVariables(const ParamVectorType&     topLevelParams)
{
    assert(m_stylesheetRoot != 0);

    m_stylesheetRoot->pushTopLevelVariables(*this, topLevelParams);
}



const XObjectPtr
StylesheetExecutionContextDefault::createVariable(
            const XPath&                xpath,
            XalanNode*                  contextNode,
            const PrefixResolver&       resolver)
{
    XalanNode* const    theCurrentNode = getCurrentNode();
            
    if (theCurrentNode == contextNode)
    {
        return xpath.execute(resolver, *this);
    }
    else
    {
        return xpath.execute(contextNode, resolver, *this);
    }
}


#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
const XObjectPtr
StylesheetExecutionContextDefault::createVariable(
            const ElemTemplateElement&  templateChild,
            XalanNode*                  sourceNode)
{
    return createXResultTreeFrag(templateChild, sourceNode);
}
#endif



void
StylesheetExecutionContextDefault::pushVariable(
            const XalanQName&           name,
            const ElemTemplateElement*  element,
            const XalanDOMString&       str,
            XalanNode*                  contextNode,
            const PrefixResolver&       resolver)
{
    assert(m_xsltProcessor != 0);

    if (str.empty() == false)
    {
        m_variablesStack.pushVariable(
            name,
            m_xsltProcessor->evalXPathStr(
                            str,
                            contextNode,
                            resolver,
                            *this),
            element);
    }
}



void
StylesheetExecutionContextDefault::pushVariable(
            const XalanQName&           name,
            const XObjectPtr            val,
            const ElemTemplateElement*  element)
{
    m_variablesStack.pushVariable(name, val, element);
}



void
StylesheetExecutionContextDefault::pushVariable(
            const XalanQName&           name,
            const ElemVariable*         var,
            const ElemTemplateElement*  element)
{
    m_variablesStack.pushVariable(name, var, element);
}



void
StylesheetExecutionContextDefault::pushVariable(
            const XalanQName&           name,
            const ElemTemplateElement*  element,
            const XPath&                xpath,
            XalanNode*                  contextNode,
            const PrefixResolver&       resolver)
{
    m_variablesStack.pushVariable(name, xpath.execute(contextNode, resolver, *this), element);
}



#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
void
StylesheetExecutionContextDefault::pushVariable(
            const XalanQName&           name,
            const ElemTemplateElement*  element,
            const ElemTemplateElement&  templateChild,
            XalanNode*                  sourceNode)
{

    m_variablesStack.pushVariable(name, createXResultTreeFrag(templateChild, sourceNode), element);
}
#endif



void
StylesheetExecutionContextDefault::pushContextMarker()
{
    m_variablesStack.pushContextMarker();
}



void
StylesheetExecutionContextDefault::popContextMarker()
{
    m_variablesStack.popContextMarker();
}



void
StylesheetExecutionContextDefault::resolveTopLevelParams()
{
    assert(m_xsltProcessor != 0);

    m_xsltProcessor->resolveTopLevelParams(*this);

    m_variablesStack.markGlobalStackFrame();
}



void
StylesheetExecutionContextDefault::clearTopLevelParams()
{
    assert(m_xsltProcessor != 0);

    m_xsltProcessor->clearTopLevelParams();

    m_variablesStack.unmarkGlobalStackFrame();
}



#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
void
StylesheetExecutionContextDefault::beginParams()
{
    m_paramsVectorStack.resize(m_paramsVectorStack.size() + 1);
}



void
StylesheetExecutionContextDefault::endParams()
{
    assert(m_paramsVectorStack.size() > 0);

    m_variablesStack.pushParams(m_paramsVectorStack.back());

    m_paramsVectorStack.pop_back();
}



void
StylesheetExecutionContextDefault::pushParam(
        const XalanQName& qName,
        const XObjectPtr& theValue)
{
    assert(m_paramsVectorStack.empty() == false);

    ParamsVectorType&   currentParamVector = m_paramsVectorStack.back();

    currentParamVector.push_back(ParamsVectorType::value_type(&qName, theValue));
}
#endif


#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
void
StylesheetExecutionContextDefault::pushParams(const ElemTemplateElement&    xslCallTemplateElement)
{

    // We have a params vector that we reuse, but occasionally, a
    // param will result in recursive execution, so we'll use a
    // temporary when we detect such a situation.
    if(m_paramsVector.empty() == true)
    {
        // This will ensure that the contents of m_paramsVector are
        // cleared.
        CollectionClearGuard<ParamsVectorType>  theGuard(m_paramsVector);

        // Make sure we have the default capacity for the params
        // vector...
        if (m_paramsVector.capacity() == 0)
        {
             m_paramsVector.reserve(eDefaultParamsVectorSize);
        }

        getParams(xslCallTemplateElement, m_paramsVector);

        m_variablesStack.pushParams(m_paramsVector);
    }
    else
    {
        ParamsVectorType    tempParams(getMemoryManager());

        getParams(xslCallTemplateElement, tempParams);

        m_variablesStack.pushParams(tempParams);
    }
}
#endif



const XObjectPtr
StylesheetExecutionContextDefault::getParamVariable(const XalanQName&   theName)
{
    bool    fFound;

    return m_variablesStack.getParamVariable(theName, *this, fFound);
}



void
StylesheetExecutionContextDefault::pushElementFrame(const ElemTemplateElement*  elem)
{
    m_variablesStack.pushElementFrame(elem);
}



void
StylesheetExecutionContextDefault::popElementFrame()
{
    m_variablesStack.popElementFrame();
}



int
StylesheetExecutionContextDefault::getGlobalStackFrameIndex() const
{
    return m_variablesStack.getGlobalStackFrameIndex();
}



int
StylesheetExecutionContextDefault::getCurrentStackFrameIndex() const
{
    return m_variablesStack.getCurrentStackFrameIndex();
}




void
StylesheetExecutionContextDefault::pushCurrentStackFrameIndex(int currentStackFrameIndex)
{
    m_currentIndexStack.push_back(getCurrentStackFrameIndex());
    m_variablesStack.setCurrentStackFrameIndex(currentStackFrameIndex);

}



void
StylesheetExecutionContextDefault::popCurrentStackFrameIndex()
{
    assert (m_currentIndexStack.size() > 0);

    int previousStackFrameIndex = m_currentIndexStack.back();

    m_currentIndexStack.pop_back();

    m_variablesStack.setCurrentStackFrameIndex(previousStackFrameIndex);
}

void
StylesheetExecutionContextDefault::startDocument()
{
    assert(m_xsltProcessor != 0);

    m_xsltProcessor->startDocument();
}



void
StylesheetExecutionContextDefault::endDocument()
{
    assert(m_xsltProcessor != 0);

    m_xsltProcessor->endDocument();

    cleanUpTransients();

    setFormatterListener(0);
}



void
StylesheetExecutionContextDefault::characters(
            const XalanDOMChar*     ch,
            fl_size_type            start,
            fl_size_type            length)
{
    assert(m_xsltProcessor != 0);

    m_xsltProcessor->characters(ch, start, length);
}



void
StylesheetExecutionContextDefault::charactersRaw(
            const XalanDOMChar*     ch,
            fl_size_type            start,
            fl_size_type            length)
{
    assert(m_xsltProcessor != 0);

    m_xsltProcessor->charactersRaw(ch, start, length);
}



void
StylesheetExecutionContextDefault::comment(const XalanDOMChar*  data)
{
    assert(m_xsltProcessor != 0);

    m_xsltProcessor->comment(data);
}



void
StylesheetExecutionContextDefault::processingInstruction(
            const XalanDOMChar*     target,
            const XalanDOMChar*     data)
{
    assert(m_xsltProcessor != 0);

    m_xsltProcessor->processingInstruction(target, data);
}



void
StylesheetExecutionContextDefault::startElement(const XalanDOMChar*     name)
{
    assert(m_xsltProcessor != 0);

    m_xsltProcessor->startElement(name);
}



void
StylesheetExecutionContextDefault::endElement(const XalanDOMChar*   name)
{
    assert(m_xsltProcessor != 0);

    m_xsltProcessor->endElement(name);
}



void
StylesheetExecutionContextDefault::flushPending()
{
    assert(m_xsltProcessor != 0);

    m_xsltProcessor->flushPending();
}



void
StylesheetExecutionContextDefault::cloneToResultTree(
            const XalanNode&    node,
            const Locator*      locator)
{
    m_xsltProcessor->cloneToResultTree(node, getCopyTextNodesOnly(), locator);
}



void
StylesheetExecutionContextDefault::cloneToResultTree(
            const XalanNode&        node,
            XalanNode::NodeType     nodeType,
            bool                    overrideStrip,
            bool                    shouldCloneAttributes,
            const Locator*          locator)
{
    assert(m_xsltProcessor != 0);

    m_xsltProcessor->cloneToResultTree(
            node,
            nodeType,
            overrideStrip,
            shouldCloneAttributes,
            getCopyTextNodesOnly(),
            locator);
}


#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
void
StylesheetExecutionContextDefault::beginCreateXResultTreeFrag(XalanNode*    sourceNode)
{
    assert(m_xsltProcessor != 0);

    XalanSourceTreeDocument* const  theDocument = m_usePerInstanceDocumentFactory == true ?
        m_documentAllocator.create(
            eDefaultAttributeAllocatorBlockSize,
            eDefaultAttributeNSAllocatorBlockSize,
            eDefaultCommentAllocatorBlockSize,
            eDefaultElementAllocatorBlockSize,
            eDefaultElementNSAllocatorBlockSize,
            eDefaultPIAllocatorBlockSize,
            eDefaultTextAllocatorBlockSize,
            eDefaultTextIWSAllocatorBlockSize) :
        getSourceTreeFactory(getMemoryManager());
    assert(theDocument != 0);

    XalanSourceTreeDocumentFragment* const  theDocumentFragment =
        m_documentFragmentAllocator.create(*theDocument);
    assert(theDocumentFragment != 0);

    FormatterToSourceTree* const    theFormatter = m_formatterToSourceTreeStack.get();
    assert(theFormatter != 0);

    theFormatter->setDocument(theDocument);

    theFormatter->setDocumentFragment(theDocumentFragment);

    theFormatter->setPrefixResolver(m_xsltProcessor);

    pushOutputContext(theFormatter);

    theFormatter->startDocument();

    pushCurrentNode(sourceNode);
}



const XObjectPtr
StylesheetExecutionContextDefault::endCreateXResultTreeFrag()
{    
    FormatterToSourceTree* const    theFormatter =
        m_formatterToSourceTreeStack.top();

    assert (theFormatter != 0);

    theFormatter->endDocument();

    XalanSourceTreeDocumentFragment* const  theDocumentFragment =
        theFormatter->getDocumentFragment();

    assert(theDocumentFragment != 0);

    XResultTreeFrag* const  theXResultTreeFrag =
        m_xresultTreeFragAllocator.create(*theDocumentFragment);

    theXResultTreeFrag->setExecutionContext(this);

    popCurrentNode();
    popOutputContext();

    m_formatterToSourceTreeStack.release();

    return XObjectPtr(theXResultTreeFrag);
}
#endif



#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
const XObjectPtr
StylesheetExecutionContextDefault::createXResultTreeFrag(
            const ElemTemplateElement&  templateChild,
            XalanNode*                  sourceNode)
{
    assert(m_xsltProcessor != 0);

    XalanSourceTreeDocument* const  theDocument = m_usePerInstanceDocumentFactory == true ?
        m_documentAllocator.create(
            eDefaultAttributeAllocatorBlockSize,
            eDefaultAttributeNSAllocatorBlockSize,
            eDefaultCommentAllocatorBlockSize,
            eDefaultElementAllocatorBlockSize,
            eDefaultElementNSAllocatorBlockSize,
            eDefaultPIAllocatorBlockSize,
            eDefaultTextAllocatorBlockSize,
            eDefaultTextIWSAllocatorBlockSize) :
        getSourceTreeFactory(getMemoryManager());
    assert(theDocument != 0);

    XalanSourceTreeDocumentFragment* const  theDocumentFragment =
        m_documentFragmentAllocator.create(*theDocument);
    assert(theDocumentFragment != 0);

    GuardCachedObject<FormatterToSourceTreeCacheType>   theGuard(m_formatterToSourceTreeCache);

    FormatterToSourceTree* const    theFormatter = theGuard.get();
    assert(theFormatter != 0);

    theFormatter->setDocument(theDocument);

    theFormatter->setDocumentFragment(theDocumentFragment);

    theFormatter->setPrefixResolver(m_xsltProcessor);

    StylesheetExecutionContext::OutputContextPushPop    theOutputContextPushPop(
                *this,
                theFormatter);

    theFormatter->startDocument();

    templateChild.executeChildren(*this, sourceNode);

    theFormatter->endDocument();

    XResultTreeFrag* const  theXResultTreeFrag =
        m_xresultTreeFragAllocator.create(*theDocumentFragment);

    theXResultTreeFrag->setExecutionContext(this);

    return XObjectPtr(theXResultTreeFrag);
}
#endif



void
StylesheetExecutionContextDefault::outputToResultTree(
            const XObject&      xobj,
            const Locator*      locator)
{
    assert(m_xsltProcessor != 0);

    m_xsltProcessor->outputToResultTree(xobj, getCopyTextNodesOnly(), locator);
}



void
StylesheetExecutionContextDefault::outputResultTreeFragment(
            const XObject&      theTree,
            const Locator*      locator)
{
    assert(m_xsltProcessor != 0);

    m_xsltProcessor->outputResultTreeFragment(theTree, getCopyTextNodesOnly(), locator);
}



const XalanDOMString&
StylesheetExecutionContextDefault::getXSLNameSpaceURL() const
{
    assert(m_xsltProcessor != 0);

    return m_xsltProcessor->getXSLNameSpaceURL();
}



const XalanDOMString&
StylesheetExecutionContextDefault::getXalanXSLNameSpaceURL() const
{
    assert(m_xsltProcessor != 0);

    return m_xsltProcessor->getXalanXSLNameSpaceURL();
}



bool
StylesheetExecutionContextDefault::getTraceSelects() const
{
    assert(m_xsltProcessor != 0);

    return m_xsltProcessor->getTraceSelects();
}



void
StylesheetExecutionContextDefault::traceSelect(
            const ElemTemplateElement&  theTemplate,
            const NodeRefListBase&      nl,
            const XPath*                xpath)
{
    assert(m_xsltProcessor != 0);

    m_xsltProcessor->traceSelect(*this, theTemplate, nl, xpath);
}



bool
StylesheetExecutionContextDefault::findOnElementRecursionStack(const ElemTemplateElement*   theElement) const
{
    assert(theElement != 0);

    XALAN_USING_STD(find)

    const ElementTemplateElementStackType::const_iterator   i =
                find(m_elementRecursionStack.begin(),
                     m_elementRecursionStack.end(),
                     theElement);

    return i == m_elementRecursionStack.end() ? false : true;
}



void
StylesheetExecutionContextDefault::pushOnElementRecursionStack(const ElemTemplateElement*   theElement)
{
    assert(theElement != 0);

    if (findOnElementRecursionStack(theElement) == true)
    {
        const Locator* const    theLocator = theElement->getLocator();

        const GetCachedString   theGuard(*this);

        throw XSLTProcessorException(
                getMemoryManager(),
                XalanMessageLoader::getMessage(
                    theGuard.get(),
                    XalanMessages::InfiniteRecursion_1Param,
                    theElement->getElementName()),
                theLocator);
    }

    m_elementRecursionStack.push_back(theElement);
}



const ElemTemplateElement*
StylesheetExecutionContextDefault::popElementRecursionStack()
{
    assert(m_elementRecursionStack.empty() == false);

    const ElemTemplateElement* const    theTemp =
        m_elementRecursionStack.back();

    m_elementRecursionStack.pop_back();

    return theTemp;
}



bool
StylesheetExecutionContextDefault::returnXResultTreeFrag(XResultTreeFrag*   theXResultTreeFrag)
{
    assert(theXResultTreeFrag != 0);

    if (m_xresultTreeFragAllocator.ownsObject(theXResultTreeFrag) == false)
    {
        return false;
    }
    else
    {
        XalanDocumentFragment* const    theDocumentFragment =
            theXResultTreeFrag->release();

        const KeyTablesTableType::iterator  i =
            m_keyTables.find(theDocumentFragment);

        if (i != m_keyTables.end())
        {
            KeyTable* const     theTable = (*i).second;

            m_keyTables.erase(i);

            theTable->~KeyTable();

            m_keyTables.getMemoryManager().deallocate((void*)theTable);
        }

        m_xresultTreeFragAllocator.destroy(theXResultTreeFrag);

        if (m_usePerInstanceDocumentFactory == true)
        {
            m_documentAllocator.destroy(static_cast<XalanSourceTreeDocument*>(theDocumentFragment->getOwnerDocument()));
        }

        m_documentFragmentAllocator.destroy(static_cast<XalanSourceTreeDocumentFragment*>(theDocumentFragment));

        return true;
    }
}



StylesheetExecutionContextDefault::eEscapeURLs
StylesheetExecutionContextDefault::getEscapeURLs() const
{
    return m_escapeURLs;
}



void
StylesheetExecutionContextDefault::setEscapeURLs(eEscapeURLs    value)
{
    m_escapeURLs = value;
}



StylesheetExecutionContextDefault::eOmitMETATag
StylesheetExecutionContextDefault::getOmitMETATag() const
{
    return m_omitMETATag;
}



void
StylesheetExecutionContextDefault::setOmitMETATag(eOmitMETATag  value)
{
    m_omitMETATag = value;
}



FormatterListener*
StylesheetExecutionContextDefault::createFormatterToXML(
            Writer&                 writer,
            const XalanDOMString&   version,
            bool                    doIndent,
            int                     indent,
            const XalanDOMString&   encoding,
            const XalanDOMString&   mediaType,
            const XalanDOMString&   doctypeSystem,
            const XalanDOMString&   doctypePublic,
            bool                    xmlDecl,
            const XalanDOMString&   standalone)
{
    m_formatterListeners.push_back(0);

    FormatterListener* const    theFormatterListener =
        XalanXMLSerializerFactory::create(
            getMemoryManager(),
            writer,
            version,
            doIndent,
            indent,
            encoding,
            mediaType,
            doctypeSystem,
            doctypePublic,
            xmlDecl,
            standalone);
    assert(theFormatterListener != 0);

    m_formatterListeners.back() = theFormatterListener;

    return theFormatterListener;
}



FormatterListener*
StylesheetExecutionContextDefault::createFormatterToHTML(
            Writer&                 writer,
            const XalanDOMString&   encoding,
            const XalanDOMString&   mediaType,
            const XalanDOMString&   doctypeSystem,
            const XalanDOMString&   doctypePublic,
            bool                    doIndent,
            int                     indent,
            bool                    escapeURLs,
            bool                    omitMetaTag)
{
    m_formatterListeners.push_back(0);

    FormatterToHTML* const  theFormatter =
        FormatterToHTML::create(
            getMemoryManager(),
            writer,
            encoding,
            mediaType,
            doctypeSystem,
            doctypePublic,
            doIndent,
            indent,
            escapeURLs,
            omitMetaTag);

    m_formatterListeners.back() = theFormatter;

    theFormatter->setPrefixResolver(m_xsltProcessor);

    return theFormatter;
}



FormatterListener*
StylesheetExecutionContextDefault::createFormatterToText(
            Writer&                 writer,
            const XalanDOMString&   encoding)
{
    m_formatterListeners.push_back(0);

    FormatterToText* const  theFormatter =
        FormatterToText::create(getMemoryManager(), writer, encoding);

    m_formatterListeners.back() = theFormatter;

    return theFormatter;
}


#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
FormatterToText*
StylesheetExecutionContextDefault::borrowFormatterToText()
{
    return m_formatterToTextCache.get();
}



bool
StylesheetExecutionContextDefault::returnFormatterToText(FormatterToText*   theFormatter)
{
    return m_formatterToTextCache.release(theFormatter);
}
#endif



#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
NodeSorter*
StylesheetExecutionContextDefault::getNodeSorter()
{
    return &m_nodeSorter;
}
#endif


#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
NodeSorter*
StylesheetExecutionContextDefault::borrowNodeSorter()
{
    return m_nodeSorterCache.get();
}



bool
StylesheetExecutionContextDefault::returnNodeSorter(NodeSorter*     theSorter)
{
    return m_nodeSorterCache.release(theSorter);
}
#endif



StylesheetExecutionContextDefault::XalanNumberFormatAutoPtr
StylesheetExecutionContextDefault::createXalanNumberFormat()
{
    return XalanNumberFormatAutoPtr(getMemoryManager(), s_xalanNumberFormatFactory->create(getMemoryManager()));
}



StylesheetExecutionContextDefault::XalanNumberFormatFactory::XalanNumberFormatFactory()
{
}



StylesheetExecutionContextDefault::XalanNumberFormatFactory::~XalanNumberFormatFactory()
{
}



XalanNumberFormat*
StylesheetExecutionContextDefault::XalanNumberFormatFactory::create(MemoryManager& theManager)
{
    return XalanNumberFormat::create(theManager);
}



StylesheetExecutionContextDefault::XalanNumberFormatFactory*
StylesheetExecutionContextDefault::installXalanNumberFormatFactory(XalanNumberFormatFactory*    theFactory)
{
    XalanNumberFormatFactory* const     theOldFactory =
        s_xalanNumberFormatFactory;

    if (theFactory == 0)
    {
        s_xalanNumberFormatFactory = &s_defaultXalanNumberFormatFactory;
    }
    else
    {
        s_xalanNumberFormatFactory = theFactory;
    }

    return theOldFactory;
}



int
StylesheetExecutionContextDefault::collationCompare(
            const XalanDOMString&               theLHS,
            const XalanDOMString&               theRHS,
            XalanCollationServices::eCaseOrder  theCaseOrder)
{
    if (m_collationCompareFunctor == 0)
    {
        return s_defaultCollationFunctor(theLHS.c_str(), theRHS.c_str(), theCaseOrder);
    }
    else
    {
        return (*m_collationCompareFunctor)(theLHS.c_str(), theRHS.c_str(), theCaseOrder);
    }
}



int
StylesheetExecutionContextDefault::collationCompare(
            const XalanDOMString&               theLHS,
            const XalanDOMString&               theRHS,
            const XalanDOMString&               theLocale,
            XalanCollationServices::eCaseOrder  theCaseOrder)
{
    if (m_collationCompareFunctor == 0)
    {
        return s_defaultCollationFunctor(theLHS.c_str(), theRHS.c_str(), theLocale.c_str(), theCaseOrder);
    }
    else
    {
        return (*m_collationCompareFunctor)(theLHS.c_str(), theRHS.c_str(), theLocale.c_str(), theCaseOrder);
    }
}



int
StylesheetExecutionContextDefault::collationCompare(
            const XalanDOMChar*                 theLHS,
            const XalanDOMChar*                 theRHS,
            XalanCollationServices::eCaseOrder  theCaseOrder)
{
    assert(theLHS != 0 && theRHS != 0);

    if (m_collationCompareFunctor == 0)
    {
        return s_defaultCollationFunctor(theLHS, theRHS, theCaseOrder);
    }
    else
    {
        return (*m_collationCompareFunctor)(theLHS, theRHS, theCaseOrder);
    }
}



int
StylesheetExecutionContextDefault::collationCompare(
            const XalanDOMChar*                 theLHS,
            const XalanDOMChar*                 theRHS,
            const XalanDOMChar*                 theLocale,
            XalanCollationServices::eCaseOrder  theCaseOrder)
{
    assert(theLHS != 0 && theRHS != 0);

    if (m_collationCompareFunctor == 0)
    {
        return s_defaultCollationFunctor(theLHS, theRHS, theLocale, theCaseOrder);
    }
    else
    {
        return (*m_collationCompareFunctor)(theLHS, theRHS, theLocale, theCaseOrder);
    }
}



StylesheetExecutionContextDefault::DefaultCollationCompareFunctor::DefaultCollationCompareFunctor()
{
}



StylesheetExecutionContextDefault::DefaultCollationCompareFunctor::~DefaultCollationCompareFunctor()
{
}



int
StylesheetExecutionContextDefault::DefaultCollationCompareFunctor::operator()(
            const XalanDOMChar*                 theLHS,
            const XalanDOMChar*                 theRHS,
            XalanCollationServices::eCaseOrder  /* theCaseOrder */) const
{
    return XALAN_CPP_NAMESPACE :: collationCompare(theLHS, theRHS);
}



int
StylesheetExecutionContextDefault::DefaultCollationCompareFunctor::operator()(
            const XalanDOMChar*                 theLHS,
            const XalanDOMChar*                 theRHS,
            const XalanDOMChar*                 /* theLocale */,
            XalanCollationServices::eCaseOrder  theCaseOrder) const
{
    return (*this)(theLHS, theRHS, theCaseOrder);
}



const StylesheetExecutionContextDefault::CollationCompareFunctor*
StylesheetExecutionContextDefault::installCollationCompareFunctor(CollationCompareFunctor*  theFunctor)
{
    assert(theFunctor != 0);

    const CollationCompareFunctor* const    temp = m_collationCompareFunctor;

    m_collationCompareFunctor = theFunctor;

    return temp;
}



StylesheetExecutionContextDefault::CollationCompareFunctor*
StylesheetExecutionContextDefault::uninstallCollationCompareFunctor()
{
    if (m_collationCompareFunctor == 0)
    {
        return 0;
    }
    else
    {
        CollationCompareFunctor* const  temp = m_collationCompareFunctor;

        m_collationCompareFunctor = 0;

        return temp;
    }
}



static const XalanQNameByValue  theEmptyQName(XalanMemMgrs::getDummyMemMgr());



void 
StylesheetExecutionContextDefault::formatNumber(
            double                              number,
            const XalanDOMString&               pattern,
            XalanDOMString&                     theResult,
            const XalanNode*                    context,
            const Locator*                      locator)
{
    const XalanDecimalFormatSymbols * theDFS = getDecimalFormatSymbols(theEmptyQName);

    if (m_formatNumberFunctor == 0) 
    {
        m_xpathExecutionContextDefault.doFormatNumber(
            number,
            pattern,
            theDFS,
            theResult,
            context,
            locator);
    } 
    else 
    {
        (*m_formatNumberFunctor)(
            *this,
            number,
            pattern,
            theDFS,
            theResult,
            context,
            locator);
    }
}
 


void 
StylesheetExecutionContextDefault::formatNumber(
            double                  number,
            const XalanDOMString&   pattern,
            const XalanDOMString&   dfsName,
            XalanDOMString&         theResult,
            const XalanNode*        context,
            const Locator*          locator)
{
    XalanQNameByValue&  theDFSQName = m_xpathExecutionContextDefault.getScratchQName();

    theDFSQName.set(dfsName, getPrefixResolver(), locator);

    const XalanDecimalFormatSymbols*    theDFS = getDecimalFormatSymbols(theDFSQName);

    if (theDFS == 0)
    {
        const GetCachedString   theGuard(*this);

        problem(
            eXSLTProcessor,
            eWarning,
            XalanMessageLoader::getMessage(
                theGuard.get(),
                XalanMessages::Decimal_formatElementNotFound_1Param,
                "format-number()"),
            locator,
            context);

        theDFS = getDecimalFormatSymbols(theEmptyQName);
        
    }

    if (m_formatNumberFunctor == 0) 
    {
        m_xpathExecutionContextDefault.doFormatNumber(number,pattern,theDFS,theResult,context,locator);
    } 
    else 
    {   
        (*m_formatNumberFunctor)(*this, number, pattern, theDFS, theResult, context, locator);
    }
}



const StylesheetExecutionContextDefault::FormatNumberFunctor*
StylesheetExecutionContextDefault::installFormatNumberFunctor(FormatNumberFunctor*  theFunctor)
{
    assert(theFunctor != 0);

    const FormatNumberFunctor * const   temp = m_formatNumberFunctor;

    m_formatNumberFunctor = theFunctor;

    return temp;
}



StylesheetExecutionContextDefault::FormatNumberFunctor*
StylesheetExecutionContextDefault::uninstallFormatNumberFunctor()
{
    if (m_formatNumberFunctor == 0)
    {
        return 0;
    }
    else
    {
        FormatNumberFunctor * const temp = m_formatNumberFunctor;

        m_formatNumberFunctor = 0;

        return temp;
    }
}



void
StylesheetExecutionContextDefault::reset()
{
#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
    assert(m_elementRecursionStack.empty() == true);
#endif

    m_variablesStack.reset();

#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
    m_xobjectPtrStack.clear();
    m_paramsVectorStack.clear();
    m_elementRecursionStack.clear();
#endif

    if (m_xsltProcessor != 0)
    {
        m_xsltProcessor->reset();
    }

    m_rootDocument = 0;
    m_stylesheetRoot = 0;
    m_mode = 0;

    m_currentTemplateStack.clear();
    m_currentTemplateStack.push_back(0);

#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
    m_formatterToTextCache.reset();
    m_formatterToSourceTreeCache.reset();
    m_nodeSorterCache.reset();
#endif

    m_xresultTreeFragAllocator.reset();
    m_documentFragmentAllocator.reset();
    m_documentAllocator.reset();

    // Just in case endDocument() was not called,
    // clean things up...
    cleanUpTransients();

    // Destroy the source tree factory, which
    // will destroy all result tree fragment nodes
    // that were generated...
    m_sourceTreeResultTreeFactory.reset();

    // Reset the default execution context...
    m_xpathExecutionContextDefault.reset();

    m_copyTextNodesOnlyStack.clear();

    m_modeStack.clear();

    m_currentIndexStack.clear();

#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
    m_nodesToTransformStack.clear();
    m_processCurrentAttributeStack.clear();
    m_skipElementAttributesStack.clear();
    m_executeIfStack.clear();
    m_elementInvokerStack.clear();
    m_useAttributeSetIndexesStack.clear();
    m_formatterToSourceTreeStack.reset();
    m_stringStack.reset();
    m_mutableNodeRefListStack.reset();
    m_formatterToTextStack.reset();
#endif
}



XalanNode*
StylesheetExecutionContextDefault::getCurrentNode() const
{
    return m_xpathExecutionContextDefault.getCurrentNode();
}



void
StylesheetExecutionContextDefault::pushCurrentNode(XalanNode*   theCurrentNode)
{
    m_xpathExecutionContextDefault.pushCurrentNode(theCurrentNode);
}



void
StylesheetExecutionContextDefault::popCurrentNode()
{
    m_xpathExecutionContextDefault.popCurrentNode();
}



bool
StylesheetExecutionContextDefault::isNodeAfter(
            const XalanNode&    node1,
            const XalanNode&    node2) const
{
    return m_xpathExecutionContextDefault.isNodeAfter(node1, node2);
}



void
StylesheetExecutionContextDefault::pushContextNodeList(const NodeRefListBase&   theContextNodeList)
{
    m_xpathExecutionContextDefault.pushContextNodeList(theContextNodeList);
}



void
StylesheetExecutionContextDefault::popContextNodeList()
{
    m_xpathExecutionContextDefault.popContextNodeList();
}



const NodeRefListBase&
StylesheetExecutionContextDefault::getContextNodeList() const
{
    return m_xpathExecutionContextDefault.getContextNodeList();
}



StylesheetExecutionContextDefault::size_type
StylesheetExecutionContextDefault::getContextNodeListLength() const
{
    return m_xpathExecutionContextDefault.getContextNodeListLength();
}



StylesheetExecutionContextDefault::size_type
StylesheetExecutionContextDefault::getContextNodeListPosition(const XalanNode&  contextNode) const
{
    return m_xpathExecutionContextDefault.getContextNodeListPosition(contextNode);
}



bool
StylesheetExecutionContextDefault::elementAvailable(const XalanQName&   theQName) const
{
    if (theQName.getNamespace() == XSLTEngineImpl::getXSLNameSpaceURL())
    {
        const int   xslToken = StylesheetConstructionContextDefault::getElementNameToken(theQName.getLocalPart());

        return xslToken < 0 ? false : true;
    }
    else
    {
        return m_xpathExecutionContextDefault.elementAvailable(theQName);
    }
}



bool
StylesheetExecutionContextDefault::elementAvailable(
            const XalanDOMString&   theName, 
            const Locator*          theLocator) const
{
    XalanQNameByValue&  theQName = m_xpathExecutionContextDefault.getScratchQName();

    theQName.set(theName, getPrefixResolver(), theLocator);

    return elementAvailable(theQName);
}



bool
StylesheetExecutionContextDefault::functionAvailable(const XalanQName&  theQName) const
{
    return m_xpathExecutionContextDefault.functionAvailable(theQName);
}



bool
StylesheetExecutionContextDefault::functionAvailable(
            const XalanDOMString&   theName, 
            const Locator*          theLocator) const
{
    return m_xpathExecutionContextDefault.functionAvailable(theName, theLocator);
}



const XObjectPtr
StylesheetExecutionContextDefault::extFunction(
            const XalanDOMString&           theNamespace,
            const XalanDOMString&           functionName,
            XalanNode*                      context,
            const XObjectArgVectorType&     argVec,
            const Locator*                  locator)
{
    assert(m_xpathExecutionContextDefault.getXPathEnvSupport() != 0);

    return m_xpathExecutionContextDefault.getXPathEnvSupport()->extFunction(*this, theNamespace, functionName, context, argVec, locator);
}



XalanDocument*
StylesheetExecutionContextDefault::parseXML(
            MemoryManager&      theManager,
            const XalanDOMString&   urlString,
            const XalanDOMString&   base,
            ErrorHandler*           theErrorHandler) const
{
    return m_xpathExecutionContextDefault.parseXML(
                theManager,
                urlString,
                base,
                theErrorHandler);
}



MutableNodeRefList*
StylesheetExecutionContextDefault::borrowMutableNodeRefList()
{
    return m_xpathExecutionContextDefault.borrowMutableNodeRefList();
}



bool
StylesheetExecutionContextDefault::returnMutableNodeRefList(MutableNodeRefList*     theList)
{
    return m_xpathExecutionContextDefault.returnMutableNodeRefList(theList);
}



void
StylesheetExecutionContextDefault::pushCopyTextNodesOnly(bool copyTextNodesOnly)
{
    m_copyTextNodesOnlyStack.push_back(copyTextNodesOnly);
}



bool
StylesheetExecutionContextDefault::popCopyTextNodesOnly()
{
    assert (m_copyTextNodesOnlyStack.size() > 0);

    bool copyTextNodesOnly = m_copyTextNodesOnlyStack.back();

    m_copyTextNodesOnlyStack.pop_back();

    return copyTextNodesOnly;
}



MutableNodeRefList*
StylesheetExecutionContextDefault::createMutableNodeRefList(MemoryManager& theManager) const
{
    return m_xpathExecutionContextDefault.createMutableNodeRefList(theManager);
}



XalanDOMString&
StylesheetExecutionContextDefault::getCachedString()
{
    return m_xpathExecutionContextDefault.getCachedString();
}



bool
StylesheetExecutionContextDefault::releaseCachedString(XalanDOMString&  theString)
{
    return m_xpathExecutionContextDefault.releaseCachedString(theString);
}



void
StylesheetExecutionContextDefault::getNodeSetByKey(
            XalanNode*              context,
            const XalanQName&       qname,
            const XalanDOMString&   ref,
            const Locator*          locator,
            MutableNodeRefList&     nodelist)
{
    assert(m_stylesheetRoot != 0);

    m_stylesheetRoot->getNodeSetByKey(
        context,
        qname,
        ref,
        *getPrefixResolver(),
        nodelist,
        *this,
        locator,
        m_keyTables);
}



void
StylesheetExecutionContextDefault::getNodeSetByKey(
            XalanNode*              context,
            const XalanDOMString&   name,
            const XalanDOMString&   ref,
            const Locator*          locator,
            MutableNodeRefList&     nodelist)
{
    assert(m_stylesheetRoot != 0);

    const PrefixResolver* const     resolver =
                getPrefixResolver();
    assert(resolver != 0);

    XalanQNameByValue&  theQName =
        m_xpathExecutionContextDefault.getScratchQName();

    theQName.set(name, resolver, locator);

    m_stylesheetRoot->getNodeSetByKey(
        context,
        theQName,
        ref,
        *resolver,
        nodelist,
        *this,
        locator,
        m_keyTables);
}



const XObjectPtr
StylesheetExecutionContextDefault::getVariable(
            const XalanQName&   name,
            const Locator*      locator)
{
    bool                fFound;

    const XObjectPtr    theValue(m_variablesStack.getVariable(name, *this, fFound));

    if(fFound == true)
    {
        assert(theValue.null() == false);

        return theValue;
    }
    else
    {
        const GetCachedString   theGuard(*this);

        problem(
            eXSLTProcessor,
            eWarning,
            XalanMessageLoader::getMessage(
                theGuard.get(),
                XalanMessages::VariableIsNotDefined_1Param,
                name.getLocalPart()),
            locator,
            getCurrentNode());

        return getXObjectFactory().createUnknown(name.getLocalPart());
    }
}



const PrefixResolver*
StylesheetExecutionContextDefault::getPrefixResolver() const
{
    return m_xpathExecutionContextDefault.getPrefixResolver();
}



void
StylesheetExecutionContextDefault::setPrefixResolver(const PrefixResolver*  thePrefixResolver)
{
    m_xpathExecutionContextDefault.setPrefixResolver(thePrefixResolver);
}



const XalanDOMString*
StylesheetExecutionContextDefault::getNamespaceForPrefix(const XalanDOMString&  prefix) const
{
    return m_xpathExecutionContextDefault.getNamespaceForPrefix(prefix);
}



const XalanDOMString&
StylesheetExecutionContextDefault::findURIFromDoc(const XalanDocument*  owner) const
{
    return m_xpathExecutionContextDefault.findURIFromDoc(owner);
}



const XalanDOMString&
StylesheetExecutionContextDefault::getUnparsedEntityURI(
            const XalanDOMString&   theName,
            const XalanDocument&    theDocument) const
{
    return m_xpathExecutionContextDefault.getUnparsedEntityURI(theName, theDocument);
}



bool
StylesheetExecutionContextDefault::shouldStripSourceNode(const XalanText&   node)
{
    assert(m_stylesheetRoot != 0);
//    assert(node.getData().length() != 0);

    return m_stylesheetRoot->shouldStripSourceNode(node);
}



XalanDocument*
StylesheetExecutionContextDefault::getSourceDocument(const XalanDOMString&  theURI) const
{
    return m_xpathExecutionContextDefault.getSourceDocument(theURI);
}



void
StylesheetExecutionContextDefault::setSourceDocument(
            const XalanDOMString&   theURI,
            XalanDocument*          theDocument)
{
    m_xpathExecutionContextDefault.setSourceDocument(theURI, theDocument);
}



const XalanDecimalFormatSymbols*
StylesheetExecutionContextDefault::getDecimalFormatSymbols(const XalanQName&    qname)
{
    if (m_stylesheetRoot == 0)
    {
        return 0; 
    }
    else
    {
        return m_stylesheetRoot->getDecimalFormatSymbols(qname);
    }
}



PrintWriter*
StylesheetExecutionContextDefault::createPrintWriter(XalanOutputStream* theTextOutputStream)
{
    assert(theTextOutputStream != 0);

    PrintWriter* const  thePrintWriter =
        XalanOutputStreamPrintWriter::create(*theTextOutputStream);

    m_printWriters.push_back(thePrintWriter);

    return thePrintWriter;
}



PrintWriter*
StylesheetExecutionContextDefault::createPrintWriter(
            const XalanDOMString&       theFileName,
            const XalanDOMString&       /* theEncoding */)
{
    XalanOutputStream* const    theOutputStream =
        XalanFileOutputStream::create( theFileName, getMemoryManager());

    m_outputStreams.push_back(theOutputStream);

    return createPrintWriter(theOutputStream);
}



PrintWriter*
StylesheetExecutionContextDefault::createPrintWriter(StreamType&    theStream)
{
    XalanOutputStream* const        theOutputStream =
        XalanStdOutputStream::create(theStream, getMemoryManager());

    m_outputStreams.push_back(theOutputStream);

    return createPrintWriter(theOutputStream);
}



PrintWriter*
StylesheetExecutionContextDefault::createPrintWriter(FILE*  theStream)
{
    XalanOutputStream* const        theOutputStream =
        XalanFStreamOutputStream::create(theStream, getMemoryManager());

    m_outputStreams.push_back(theOutputStream);

    return createPrintWriter(theOutputStream);
}



CountersTable&
StylesheetExecutionContextDefault::getCountersTable()
{
    return m_countersTable;
}



void
StylesheetExecutionContextDefault::characters(const XalanNode&  node)
{
    m_xsltProcessor->characters(node);
}


void
StylesheetExecutionContextDefault::characters(const XObjectPtr&     xobject)
{
    m_xsltProcessor->characters(xobject);
}



void
StylesheetExecutionContextDefault::charactersRaw(const XalanNode&   node)
{
    m_xsltProcessor->charactersRaw(node);
}



void
StylesheetExecutionContextDefault::charactersRaw(const XObjectPtr&  xobject)
{
    m_xsltProcessor->charactersRaw(xobject);
}



StylesheetExecutionContextDefault::tl_size_type
StylesheetExecutionContextDefault::getTraceListeners() const
{
    assert(m_xsltProcessor != 0);

    return m_xsltProcessor->getTraceListeners();
}



void
StylesheetExecutionContextDefault::fireGenerateEvent(const GenerateEvent&   ge)
{
    assert(m_xsltProcessor != 0);

    m_xsltProcessor->fireGenerateEvent(ge);
}



void
StylesheetExecutionContextDefault::fireTraceEvent(const TracerEvent&    te)
{
    assert(m_xsltProcessor != 0);

    m_xsltProcessor->fireTraceEvent(te);
}



void
StylesheetExecutionContextDefault::fireSelectEvent(const SelectionEvent&    se)
{
    assert(m_xsltProcessor != 0);

    m_xsltProcessor->fireSelectEvent(se);
}



class PopAndPushContextMarker
{
public:

    PopAndPushContextMarker(StylesheetExecutionContext& theExecutionContext) :
        m_executionContext(theExecutionContext)
    {
        m_executionContext.popContextMarker();
    }

    ~PopAndPushContextMarker()
    {
        m_executionContext.pushContextMarker();
    }

private:

    StylesheetExecutionContext&     m_executionContext;
};



#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
void
StylesheetExecutionContextDefault::getParams(
            const ElemTemplateElement&  xslCallTemplateElement,
            ParamsVectorType&           params)
{
    assert(getCurrentNode() != 0);
    assert(params.empty() == true);

    const ElemTemplateElement*  child =
            xslCallTemplateElement.getFirstChildElem();

    if (0 != child)
    {
        // This object will take care of popping, then
        // pushing the context marker at the top of the
        // stack, even if an exception is thrown.
        PopAndPushContextMarker     thePopPush(*this);

        while(0 != child)
        {
            if(StylesheetConstructionContext::ELEMNAME_WITH_PARAM == child->getXSLToken())
            {
                const XPath* const  pxpath = child->getXPath();

                XObjectPtr  theXObject;

                if(0 != pxpath)
                {
                    theXObject =
                        createVariable(
                            *pxpath,
                            getCurrentNode(),
                            *child);
                }
                else
                {
                    theXObject =
                        createVariable(
                            *child,
                            getCurrentNode());
                }

                const ElemWithParam* const  xslParamElement =
                        static_cast<const ElemWithParam*>(child);

                params.push_back(ParamsVectorType::value_type(&xslParamElement->getQName(), theXObject));
            }

            child = child->getNextSiblingElem();
        }
    }
}
#endif


XalanSourceTreeDocument*
StylesheetExecutionContextDefault::getSourceTreeFactory(MemoryManager& theManager) const
{
    assert(m_xsltProcessor != 0);

    if (m_sourceTreeResultTreeFactory.get() == 0)
    {
        m_sourceTreeResultTreeFactory.reset(
            &theManager, 
            XalanSourceTreeDocument::create(theManager));
    }

    return m_sourceTreeResultTreeFactory.get();
}



bool
StylesheetExecutionContextDefault::isCached(const XPath*    theXPath)
{
    XPathCacheMapType::const_iterator   i =
        m_matchPatternCache.begin();

    const XPathCacheMapType::const_iterator     theEnd =
        m_matchPatternCache.end();

    while (i != theEnd)
    {
        if ((*i).second.first == theXPath)
        {
            return true;
        }
        else
        {
            ++i;
        }
    }

    return false;
}



void
StylesheetExecutionContextDefault::XPathCacheReturnFunctor::operator()(const XPathCacheMapType::value_type&     theCacheEntry)
{
    m_xsltProcessor.returnXPath(theCacheEntry.second.first);
}




void
StylesheetExecutionContextDefault::clearXPathCache()
{
    XALAN_USING_STD(for_each)

    assert(m_matchPatternCache.empty() == true || m_xsltProcessor != 0);

    if (m_xsltProcessor != 0)
    {
        for_each(m_matchPatternCache.begin(),
                 m_matchPatternCache.end(),
                 XPathCacheReturnFunctor(*m_xsltProcessor));
    }

    m_matchPatternCache.clear();
}



void
StylesheetExecutionContextDefault::addToXPathCache(
            const XalanDOMString&   pattern,
            const XPath*            theXPath)
{
    assert(m_xsltProcessor != 0);

#if defined(XALAN_STRICT_ANSI_HEADERS)
    ClockType   addClock = std::clock();
#else
    ClockType   addClock = clock();
#endif

    if (m_matchPatternCache.size() == eXPathCacheMax)
    {
        // OK, we need to clear something out of the cache...

        // Initialize the lowest clock time found so far
        // with the current clock...
        ClockType   lowest = addClock;

        // Get some iterators ready to search the cache...
        XPathCacheMapType::iterator     i =
            m_matchPatternCache.begin();

        const XPathCacheMapType::iterator   theEnd =
            m_matchPatternCache.end();

        XPathCacheMapType::iterator     earliest(theEnd);

        while(i != theEnd)
        {
            const ClockType     current = (*i).second.second;

            if (current < lowest)
            {
                // OK, found a lower clock time, so
                // update the everything...
                lowest = current;

                earliest = i;
            }
            else
            {
                ++i;
            }
        }
        assert(earliest != theEnd);

        // Return the XPath and erase it from the cache.
        m_xsltProcessor->returnXPath((*earliest).second.first);

        m_matchPatternCache.erase(earliest);
    }

    // Add the XPath with the current clock
    m_matchPatternCache.insert(pattern, XPathCacheEntry(theXPath, addClock));
}



void
StylesheetExecutionContextDefault::cleanUpTransients()
{
    XALAN_USING_STD(for_each)

    for_each(m_formatterListeners.begin(),
             m_formatterListeners.end(),
             DeleteFunctor<FormatterListener>(getMemoryManager()));

    m_formatterListeners.clear();

    for_each(m_printWriters.begin(),
             m_printWriters.end(),
             DeleteFunctor<PrintWriter>(getMemoryManager()));

    m_printWriters.clear();

    for_each(m_outputStreams.begin(),
             m_outputStreams.end(),
             DeleteFunctor<XalanOutputStream>(getMemoryManager()));

    m_outputStreams.clear();

    // Clean up the key table vector
    for_each(m_keyTables.begin(),
             m_keyTables.end(),
             makeMapValueDeleteFunctor(m_keyTables));

    m_keyTables.clear();

    m_countersTable.reset();

    // Clear any cached XPaths...
    clearXPathCache();
  
    assert(m_matchPatternCache.empty() == true);
}



#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
void
StylesheetExecutionContextDefault::createUseAttributeSetIndexesOnStack()
{
    UseAttributeSetIndexes useAttributeSetIndexes;
    m_useAttributeSetIndexesStack.push_back(useAttributeSetIndexes);
}



StylesheetExecutionContextDefault::UseAttributeSetIndexes&
StylesheetExecutionContextDefault::getUseAttributeSetIndexes()
{
    assert(m_useAttributeSetIndexesStack.size() > 0);

    return m_useAttributeSetIndexesStack.back();
}



void
StylesheetExecutionContextDefault::popUseAttributeSetIndexesFromStack()
{   
    assert(m_useAttributeSetIndexesStack.size() > 0);

    m_useAttributeSetIndexesStack.pop_back();

}



void
StylesheetExecutionContextDefault::pushInvoker(const ElemTemplateElement * invoker)
{
    m_elementInvokerStack.push_back(invoker);
}



void
StylesheetExecutionContextDefault::popInvoker()
{
    assert (m_elementInvokerStack.size() > 0);

    m_elementInvokerStack.pop_back();
}



const ElemTemplateElement*
StylesheetExecutionContextDefault::getInvoker() const
{
     assert (m_elementInvokerStack.size() > 0);

     return m_elementInvokerStack.back();
}




XalanDOMString&
StylesheetExecutionContextDefault::getAndPushCachedString()
{
    XalanDOMString& theString = *(m_stringStack.get());

    theString.clear();

    return theString;
}



XalanDOMString&
StylesheetExecutionContextDefault::getLastCachedString()
{
    return *(m_stringStack.top());
}



XalanDOMString&
StylesheetExecutionContextDefault::getAndPopCachedString()
{
    return *(m_stringStack.release());
}



void
StylesheetExecutionContextDefault::pushProcessCurrentAttribute(const bool processAttribute)
{
    m_processCurrentAttributeStack.push_back(processAttribute);
}



bool
StylesheetExecutionContextDefault::popProcessCurrentAttribute()
{
    assert (m_processCurrentAttributeStack.size() > 0);

    bool processAttribute = m_processCurrentAttributeStack.back();
    m_processCurrentAttributeStack.pop_back();
    return processAttribute;
}



void
StylesheetExecutionContextDefault::pushSkipElementAttributes(bool skipAttributes)
{
    m_skipElementAttributesStack.push_back(skipAttributes);
}



bool
StylesheetExecutionContextDefault::getSkipElementAttributes() const
{
    if (m_skipElementAttributesStack.size() == 0)
    {
        return false;
    }
    else
    {
        return m_skipElementAttributesStack.back();
    }
}



bool
StylesheetExecutionContextDefault::popSkipElementAttributes()
{
    assert(m_skipElementAttributesStack.size() > 0);

    bool skipAttributes = m_skipElementAttributesStack.back();

    m_skipElementAttributesStack.pop_back();

    return skipAttributes;
}



void
StylesheetExecutionContextDefault::pushExecuteIf(bool executeIf)
{
    m_executeIfStack.push_back(executeIf);
}



bool
StylesheetExecutionContextDefault::popExecuteIf()
{
    assert(m_executeIfStack.size() > 0);

    bool executeIf = m_executeIfStack.back();

    m_executeIfStack.pop_back();

    return executeIf;
}



void
StylesheetExecutionContextDefault::beginFormatToText(XalanDOMString&  theResult)
{
    FormatterToTextDOMString* const     theFormatter =
        m_formatterToTextStack.get();
    assert(theFormatter != 0);

    theFormatter->setDOMString(theResult);

    pushOutputContext(theFormatter);
    
    theFormatter->startDocument();
}



void
StylesheetExecutionContextDefault::endFormatToText()
{
    FormatterToText* const  theFormatter = m_formatterToTextStack.top();
    assert(
        theFormatter != 0 &&
        theFormatter->getEncoding().empty() == true &&
        theFormatter->getNormalizeLinefeed() == false &&
        theFormatter->getHandleIgnorableWhitespace() == true);

    theFormatter->endDocument();

    popOutputContext();

    m_formatterToTextStack.release();
}



void
StylesheetExecutionContextDefault::pushXObjectPtr(const XObjectPtr & xobjectPtr)
{   
    m_xobjectPtrStack.push_back(xobjectPtr);
}



void 
StylesheetExecutionContextDefault::popXObjectPtr()
{
    assert(m_xobjectPtrStack.size() > 0);

    m_xobjectPtrStack.back().release();
    m_xobjectPtrStack.pop_back();
}



MutableNodeRefList&
StylesheetExecutionContextDefault::createAndPushMutableNodeRefList()
{
    MutableNodeRefList& nodeList = *(m_mutableNodeRefListStack.get());

    nodeList.clear();

    return nodeList;
}



void 
StylesheetExecutionContextDefault::releaseAndPopMutableNodeRefList()
{
    m_mutableNodeRefListStack.release();
}



void
StylesheetExecutionContextDefault::createAndPushNodesToTransformList(const NodeRefListBase* nodeList)
{
    assert(nodeList != 0);

    NodesToTransform nodesToTransform(nodeList);
    m_nodesToTransformStack.push_back(nodesToTransform);
}



XalanNode* 
StylesheetExecutionContextDefault::getNextNodeToTransform()
{
    assert(m_nodesToTransformStack.size() > 0);

    return m_nodesToTransformStack.back().next();
}



void 
StylesheetExecutionContextDefault::popNodesToTransformList()
{
    assert(m_nodesToTransformStack.size() > 0);

    m_nodesToTransformStack.pop_back();
}


XalanDOMString  StylesheetExecutionContextDefault::FormatterToTextDOMString::s_dummyString(XalanMemMgrs::getDummyMemMgr());



StylesheetExecutionContextDefault::FormatterToTextDOMString::FormatterToTextDOMString(MemoryManager&    theManager) :
    FormatterToText(theManager),
    m_printWriter(s_dummyString)
{
    setNormalizeLinefeed(false);

    setWriter(&m_printWriter);
}


StylesheetExecutionContextDefault::FormatterToTextDOMString::~FormatterToTextDOMString()
{
    assert(s_dummyString.capacity() == 0);
}
#endif


XALAN_CPP_NAMESPACE_END
