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

// Class header file
#include "StylesheetRoot.hpp"



#include <algorithm>
#include <memory>



#include <xalanc/XalanDOM/XalanDocumentFragment.hpp>



#include <xalanc/PlatformSupport/AttributeListImpl.hpp>
#include <xalanc/PlatformSupport/PrintWriter.hpp>
#include <xalanc/PlatformSupport/StringTokenizer.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include <xalanc/XPath/XalanQNameByReference.hpp>
#include <xalanc/XPath/XPathFactory.hpp>
#include <xalanc/XPath/XPathProcessor.hpp>



#include <xalanc/XMLSupport/FormatterToHTML.hpp>
#include <xalanc/XMLSupport/FormatterToText.hpp>
#include <xalanc/XMLSupport/FormatterToXML.hpp>



#include "Constants.hpp"
#include "ElemApplyTemplates.hpp"
#include "ElemAttributeSet.hpp"
#include "ElemTemplate.hpp"
#include "ElemValueOf.hpp"
#include "KeyTable.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"
#include "TraceListener.hpp"
#include "XSLTResultTarget.hpp"



//#define XALAN_VQ_SPECIAL_TRACE
#if defined(XALAN_VQ_SPECIAL_TRACE)
#include "C:/Program Files/Rational/Quantify/pure.h"
#endif



XALAN_CPP_NAMESPACE_BEGIN



StylesheetRoot::StylesheetRoot(
        const XalanDOMString&           baseIdentifier,
        StylesheetConstructionContext&  constructionContext) :
    Stylesheet(*this,
               baseIdentifier,
               constructionContext),    
    m_version(constructionContext.getMemoryManager()),
    m_indentResult(eIndentNoImplicit),
    m_encoding(constructionContext.getMemoryManager()),
    m_mediatype(constructionContext.getMemoryManager()),
    m_doctypeSystem(constructionContext.getMemoryManager()),
    m_doctypePublic(constructionContext.getMemoryManager()),
    m_omitxmlDecl(false),
    m_standalone(constructionContext.getMemoryManager()),
    m_resultNameSpaceURL(constructionContext.getMemoryManager()),
    m_outputMethod(FormatterListener::OUTPUT_METHOD_NONE),
    m_cdataSectionElems(constructionContext.getMemoryManager()),
    m_hasCDATASectionElems(false),
    m_importStack(constructionContext.getMemoryManager()),
    m_defaultTextRule(0),
    m_defaultRule(0),
    m_defaultRootRule(0),
    m_needToBuildKeysTable(false),
    m_outputEscapeURLs(true),
    m_indentAmount(-1),
    m_omitMETATag(false),
    m_elemNumberNextID(0),
    m_attributeSetsMap(constructionContext.getMemoryManager()),
    m_hasStripOrPreserveSpace(false)
{
    // Our base class has already resolved the URI and pushed it on
    // the back of the include stack, so get it from there...
    assert(getIncludeStack().empty() == false);

    m_importStack.push_back(getIncludeStack().back());
}               



StylesheetRoot*
StylesheetRoot::create(
            MemoryManager&                   theManager,
            const XalanDOMString&            baseIdentifier,
            StylesheetConstructionContext&   constructionContext)
{
    typedef StylesheetRoot  ThisType;

    XalanAllocationGuard    theGuard(theManager, theManager.allocate(sizeof(ThisType)));

    ThisType* const     theResult =
        new (theGuard.get()) ThisType(
                                baseIdentifier,
                                constructionContext);

    theGuard.release();

    return theResult;
}



StylesheetRoot::~StylesheetRoot()
{
}



void
StylesheetRoot::postConstruction(StylesheetConstructionContext&     constructionContext)
{
    // Chain-up first...
    Stylesheet::postConstruction(constructionContext);

    initDefaultRule(constructionContext);

    {
        AttributeSetMapType::iterator           theCurrentMap = m_attributeSetsMap.begin();
        const AttributeSetMapType::iterator     theEndMap = m_attributeSetsMap.end();

        while(theCurrentMap != theEndMap)
        {
            AttributeSetVectorType::iterator        theCurrentVector = (*theCurrentMap).second.begin();
            const AttributeSetVectorType::iterator  theEndVector = (*theCurrentMap).second.end();

            while(theCurrentVector != theEndVector)
            {
                (*theCurrentVector)->postConstruction(constructionContext, getNamespacesHandler());

                ++theCurrentVector;
            }

            ++theCurrentMap;
        }   
    }

    // We may need to build keys, since we may have inherited them from
    // our imports.
    if (m_needToBuildKeysTable == false &&
        m_keyDeclarations.empty() == false)
    {
        m_needToBuildKeysTable = true;
    }

    // cdata-section-elements only applies to the XML output method...
    if (m_outputMethod != FormatterListener::OUTPUT_METHOD_XML &&
        m_outputMethod != FormatterListener::OUTPUT_METHOD_NONE)
    {
        m_cdataSectionElems.clear();
    }
    else if (m_cdataSectionElems.empty() == false)
    {
        XALAN_USING_STD(sort)

        sort(
            m_cdataSectionElems.begin(),
            m_cdataSectionElems.end(),
            pointer_less<XalanQName>());

        m_hasCDATASectionElems = true;
    }

    m_hasStripOrPreserveSpace = m_whitespaceElements.empty() == false;
}



typedef StylesheetExecutionContext::GetCachedString     GetCachedString;



void
StylesheetRoot::process(
            XalanNode*                      sourceTree, 
            XSLTResultTarget&               outputTarget,
            StylesheetExecutionContext&     executionContext) const
{
    assert(m_defaultRule != 0);
    assert(m_defaultTextRule != 0);
    assert(m_defaultRootRule != 0);

    // Find the root pattern in the XSL.
    const ElemTemplateElement*      rootRule =
            findTemplate(executionContext, sourceTree);

    if(0 == rootRule)
    {
        rootRule = m_defaultRootRule;
    }
    assert(rootRule != 0);

    executionContext.setStylesheetRoot(this);

    setupFormatterListener(outputTarget, executionContext);

    executionContext.setRootDocument(sourceTree);

#if defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
    typedef StylesheetExecutionContext::PushAndPopContextMarker PushAndPopContextMarker;
    typedef StylesheetExecutionContext::PushAndPopElementFrame  PushAndPopElementFrame;
    typedef StylesheetExecutionContext::ResolveAndClearTopLevelParams   ResolveAndClearTopLevelParams;

    PushAndPopContextMarker     thePushAndPopContextMarker(executionContext);

    PushAndPopElementFrame      thePushAndPopElementFrame(
                executionContext,
                0);

    ResolveAndClearTopLevelParams   theResolveAndClearTopLevelParams(executionContext);
#else
    executionContext.pushContextMarker();
    executionContext.pushElementFrame(0);
    executionContext.resolveTopLevelParams();
#endif

#if defined(XALAN_VQ_SPECIAL_TRACE)
    QuantifyStartRecordingData();
#endif

    executionContext.startDocument();

    executionContext.pushCurrentMode(&s_emptyQName);

    const XPathExecutionContext::CurrentNodePushAndPop  theCurrentNodePushAndPop(executionContext, sourceTree);

    // Output the action of the found root rule.  All processing
    // occurs from here.
    
    rootRule->execute(executionContext);

    // At this point, anything transient during the tranformation
    // may have been deleted, so we may not refer to anything the
    // execution context may have created for us.
    executionContext.endDocument();

#if defined(XALAN_VQ_SPECIAL_TRACE)
    QuantifyStopRecordingData();
#endif

#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)
    executionContext.clearTopLevelParams();
    executionContext.popElementFrame();
    executionContext.popContextMarker();
#endif
}



const XalanDOMString&
StylesheetRoot::getEncoding(const XSLTResultTarget&     outputTarget) const
{
    const XalanDOMString&   theEncoding = outputTarget.getEncoding();

    if (theEncoding.length() != 0)
    {
        return theEncoding;
    }
    else
    {
        return m_encoding;
    }
}



FormatterListener*
StylesheetRoot::setupFormatterListener(
            XSLTResultTarget&               outputTarget,
            StylesheetExecutionContext&     executionContext) const
{
    typedef StylesheetExecutionContext::GetCachedString     GetCachedString;

    FormatterListener*  flistener = outputTarget.getFormatterListener();

    if (flistener != 0)
    {
        // Do encoding stuff here...
    }
    else if (0 != outputTarget.getCharacterStream() ||
             0 != outputTarget.getByteStream() ||
             0 != outputTarget.getStream() ||
             !outputTarget.getFileName().empty())
    {
        /*
         * Output target has a character or byte stream or file
         */
        Writer*     pw = 0;

        if (0 != outputTarget.getCharacterStream())
        {
            pw = outputTarget.getCharacterStream();
        }
        else
        {
            if (0 != outputTarget.getByteStream())
            {
                pw = executionContext.createPrintWriter(*outputTarget.getByteStream());
            }
            else if (0 != outputTarget.getStream())
            {
                pw = executionContext.createPrintWriter(outputTarget.getStream());
            }
            else if (!outputTarget.getFileName().empty())
            {
                const GetCachedString   theGuard(executionContext);

                pw = executionContext.createPrintWriter(
                            outputTarget.getFileName(),
                            theGuard.get());
            }
            else
            {
                const GetCachedString   theGuard(executionContext);

                executionContext.problem(
                    StylesheetExecutionContext::eXSLTProcessor,
                    StylesheetExecutionContext::eError,
                    XalanMessageLoader::getMessage(
                        theGuard.get(),
                        XalanMessages::NoValidResultTarget),
                    0,
                    executionContext.getCurrentNode());
            }
        }

        int     indentAmount = executionContext.getIndent();

        // If the indent amount is less than 0, that means use
        // the value specified in the stylesheet.
        if (indentAmount < 0)
        {
            indentAmount = m_indentAmount;
        }

        const bool  doIndent = (indentAmount > -1) ? true : getOutputIndent();

        const XalanDOMString&   theEncoding = getEncoding(outputTarget);

        switch(m_outputMethod)
        {
        case FormatterListener::OUTPUT_METHOD_HTML:
            {
                if (doIndent == true && indentAmount < 0)
                {
                    indentAmount = FormatterToHTML::eDefaultIndentAmount;
                }

                // Start with the default that was set in the stylesheet...
                bool    outputEscapeURLs = m_outputEscapeURLs;

                {
                    const StylesheetExecutionContext::eEscapeURLs   eEscapeURLs =
                        executionContext.getEscapeURLs();

                    // If it's anything other than StylesheetExecutionContext::eEscapeURLsDefault,
                    // use the property from the execution context...
                    if (eEscapeURLs == StylesheetExecutionContext::eEscapeURLsNo)
                    {
                        outputEscapeURLs = false;
                    }
                    else if (eEscapeURLs == StylesheetExecutionContext::eEscapeURLsYes)
                    {
                        outputEscapeURLs = true;
                    }
                }

                // Start with the default that was set in the stylesheet...
                bool    omitMETATag = m_omitMETATag;

                {
                    const StylesheetExecutionContext::eOmitMETATag  eOmitMETATag =
                        executionContext.getOmitMETATag();

                    // If it's anything other than StylesheetExecutionContext::eOmitMETATagDefault,
                    // use the property from the execution context...
                    if (eOmitMETATag == StylesheetExecutionContext::eOmitMETATagNo)
                    {
                        omitMETATag = false;
                    }
                    else if (eOmitMETATag == StylesheetExecutionContext::eOmitMETATagYes)
                    {
                        omitMETATag = true;
                    }
                }

                flistener = executionContext.createFormatterToHTML(
                                *pw,
                                theEncoding,
                                m_mediatype,
                                m_doctypeSystem,
                                m_doctypePublic,
                                doIndent,
                                indentAmount,
                                outputEscapeURLs,
                                omitMETATag);
            }
            break;

        case FormatterListener::OUTPUT_METHOD_TEXT:
            flistener = executionContext.createFormatterToText(*pw, theEncoding);
            break;

        case FormatterListener::OUTPUT_METHOD_NONE:
        case FormatterListener::OUTPUT_METHOD_XML:
        default:
            // Make sure we don't have a negative indent amount if we're
            // indenting
            if (doIndent == true && indentAmount < 0)
            {
                indentAmount = FormatterToXML::eDefaultIndentAmount;
            }

            flistener = executionContext.createFormatterToXML(
                        *pw, m_version, doIndent, indentAmount, theEncoding, m_mediatype,
                        m_doctypeSystem, m_doctypePublic, !m_omitxmlDecl, m_standalone);
            break;
        }

        executionContext.setFormatterListener(flistener);
    }
    else
    {
        const GetCachedString   theGuard(executionContext);

       executionContext.problem(
           StylesheetExecutionContext::eXSLTProcessor,
           StylesheetExecutionContext::eError,
            XalanMessageLoader::getMessage(
                theGuard.get(),
                XalanMessages::NoValidResultTarget),
            0,
            executionContext.getCurrentNode());
    }

    executionContext.setFormatterListener(flistener);

    return flistener;
}



void 
StylesheetRoot::processOutputSpec(
            const XalanDOMChar*             name, 
            const AttributeListType&        atts,
            StylesheetConstructionContext&  constructionContext)
{
    const XalanSize_t   nAttrs = atts.getLength();

    const Locator* const    theLocator = constructionContext.getLocatorFromStack();

    for (XalanSize_t i = 0; i < nAttrs; i++)
    {
        const XalanDOMChar* const   aname = atts.getName(i);

        if (equals(aname, Constants::ATTRNAME_OUTPUT_METHOD))
        {
            const XalanDOMChar* const   method = atts.getValue(i);

            if (equals(method, Constants::ATTRVAL_OUTPUT_METHOD_HTML))
            {
                m_outputMethod = FormatterListener::OUTPUT_METHOD_HTML;
            }
            else if (equals(method, Constants::ATTRVAL_OUTPUT_METHOD_XML))
            {
                m_outputMethod = FormatterListener::OUTPUT_METHOD_XML;
            }
            else if (equals(method, Constants::ATTRVAL_OUTPUT_METHOD_TEXT))
            {
                m_outputMethod = FormatterListener::OUTPUT_METHOD_TEXT;
            }
            else
            {
                const StylesheetConstructionContext::GetCachedString    theGuard(constructionContext);

                constructionContext.problem(
                    StylesheetConstructionContext::eXSLTProcessor,
                    StylesheetConstructionContext::eWarning,
                    XalanMessageLoader::getMessage(
                        theGuard.get(),
                        XalanMessages::OutputHasAnUnknownMethod_1Param,
                        method),
                    theLocator,
                    0);
            }
        }
        else if (equals(aname, Constants::ATTRNAME_OUTPUT_VERSION))
        {
            m_version = atts.getValue(i);
        }
        else if (equals(aname,Constants::ATTRNAME_OUTPUT_INDENT))
        {
            m_indentResult =
                getYesOrNo(aname, atts.getValue(i), constructionContext) == true ?
                    eIndentYesExplicit :
                    eIndentNoExplicit;
        }
        else if (equals(aname,Constants::ATTRNAME_OUTPUT_ENCODING))
        {
            m_encoding = atts.getValue(i);
        }
        else if (equals(aname,Constants::ATTRNAME_OUTPUT_MEDIATYPE))
        {
            m_mediatype = atts.getValue(i);
        }
        else if (equals(aname,Constants::ATTRNAME_OUTPUT_DOCTYPE_SYSTEM))
        {
            m_doctypeSystem = atts.getValue(i);
        }
        else if (equals(aname,Constants::ATTRNAME_OUTPUT_DOCTYPE_PUBLIC))
        {
            m_doctypePublic = atts.getValue(i);
        }
        else if (equals(aname,Constants::ATTRNAME_OUTPUT_OMITXMLDECL))
        {
            m_omitxmlDecl = getYesOrNo(aname, atts.getValue(i), constructionContext);
        }
        else if (equals(aname,Constants::ATTRNAME_OUTPUT_STANDALONE))
        {
            m_standalone = atts.getValue(i);
        }
        else if (equals(aname,Constants::ATTRNAME_OUTPUT_CDATA_SECTION_ELEMENTS))
        {
            if (m_outputMethod == FormatterListener::OUTPUT_METHOD_NONE ||
                m_outputMethod == FormatterListener::OUTPUT_METHOD_XML)
            {
                StringTokenizer theTokenizer(atts.getValue(i));

                StringTokenizer::size_type  theTokenCount =
                    theTokenizer.countTokens();

                m_cdataSectionElems.reserve(m_cdataSectionElems.size() + theTokenCount);

                const StylesheetConstructionContext::GetCachedString    theGuard(constructionContext);

                XalanDOMString& theToken = theGuard.get();

                while (theTokenCount > 0)
                {
                    theTokenizer.nextToken(theToken);

                    --theTokenCount;

                    m_cdataSectionElems.push_back(
                        constructionContext.createXalanQName(theToken, getNamespaces(), theLocator, true));
                }

                assert(theTokenizer.hasMoreTokens() == false);
            }
        }
        else
        {
            const XalanQNameByValue     theAttributeName(aname, getNamespaces(), constructionContext.getMemoryManager(), theLocator );

            if (theAttributeName.getNamespace() == constructionContext.getXalanXSLNameSpaceURL())
            {
                if (theAttributeName.getLocalPart() == Constants::ATTRNAME_ESCAPE_URLS)
                {
                    m_outputEscapeURLs = getYesOrNo(aname, atts.getValue(i), constructionContext);
                }
                else if (theAttributeName.getLocalPart() == Constants::ATTRNAME_INDENTAMOUNT)
                {
                    m_indentAmount = WideStringToInt(atts.getValue(i));

                    if (m_indentAmount < 0)
                    {
                        m_indentAmount = 0;
                    }
                }
                else if (theAttributeName.getLocalPart() == Constants::ATTRNAME_OMIT_META_TAG)
                {
                    m_omitMETATag = getYesOrNo(aname, atts.getValue(i), constructionContext);
                }
                else
                {
                    const StylesheetConstructionContext::GetCachedString    theGuard(constructionContext);

                    constructionContext.problem(
                        StylesheetConstructionContext::eXSLTProcessor,
                        StylesheetConstructionContext::eWarning,
                        XalanMessageLoader::getMessage(
                            theGuard.get(),
                            XalanMessages::UnsupportedXalanSpecificAttribute_1Param,
                            theAttributeName.getLocalPart()),
                        theLocator,
                        0);
                }
            }
            else if (isAttrOK(aname, atts, i, constructionContext) == false)
            {
                const StylesheetConstructionContext::GetCachedString    theGuard(constructionContext);

                constructionContext.problem(
                    StylesheetConstructionContext::eXSLTProcessor,
                    StylesheetConstructionContext::eError,
                    XalanMessageLoader::getMessage(
                        theGuard.get(),
                        XalanMessages::HasIllegalAttribute_2Param,
                        name,
                        aname),
                    theLocator,
                    0);
            }
        }
    }

    // If HTML output 
    if (m_outputMethod == FormatterListener::OUTPUT_METHOD_HTML &&
        m_indentResult == eIndentNoImplicit)
    {
        m_indentResult = eIndentYesImplicit;
    }
}



void 
StylesheetRoot::initDefaultRule(StylesheetConstructionContext&  constructionContext)
{
    if (m_defaultRule == 0)
    {
        assert(m_defaultTextRule == 0);
        assert(m_defaultRootRule == 0);

        AttributeListImpl       attrs(constructionContext.getMemoryManager());

        attrs.addAttribute(
            Constants::ATTRNAME_MATCH.c_str(),
            Constants::ATTRTYPE_CDATA.c_str(),
            XPath::PSEUDONAME_ANY);

        m_defaultRule =
            constructionContext.createElement(
                StylesheetConstructionContext::ELEMNAME_TEMPLATE,
                *this,
                attrs);
        assert(m_defaultRule != 0);

        attrs.clear();

        ElemTemplateElement*    childrenElement =
            constructionContext.createElement(
                StylesheetConstructionContext::ELEMNAME_APPLY_TEMPLATES,
                *this,
                attrs);
        assert(childrenElement != 0);

        m_defaultRule->appendChildElem(childrenElement);

        m_defaultRule->setDefaultTemplate(true);

        // -----------------------------

        attrs.clear();
        attrs.addAttribute(
            Constants::ATTRNAME_MATCH.c_str(),
            Constants::ATTRTYPE_CDATA.c_str(),
            Constants::ATTRVAL_DEFAULT_TEXT_RULE.c_str());

        m_defaultTextRule =
            constructionContext.createElement(
                StylesheetConstructionContext::ELEMNAME_TEMPLATE,
                *this,
                attrs);
        assert(m_defaultTextRule != 0);

        attrs.clear();
        attrs.addAttribute(
            Constants::ATTRNAME_SELECT.c_str(),
            Constants::ATTRTYPE_CDATA.c_str(),
            Constants::ATTRVAL_THIS.c_str());

        childrenElement =
            constructionContext.createElement(
                StylesheetConstructionContext::ELEMNAME_VALUE_OF,
                *this,
                attrs);
        assert(childrenElement != 0);

        m_defaultTextRule->appendChildElem(childrenElement);

        m_defaultTextRule->setDefaultTemplate(true);

        //--------------------------------
    
        attrs.clear();
        attrs.addAttribute(
            Constants::ATTRNAME_MATCH.c_str(),
            Constants::ATTRTYPE_CDATA.c_str(),
            XPath::PSEUDONAME_ROOT);

        m_defaultRootRule =
            constructionContext.createElement(
                StylesheetConstructionContext::ELEMNAME_TEMPLATE,
                *this,
                attrs);
        assert(m_defaultRootRule != 0);

        attrs.clear();

        childrenElement =
            constructionContext.createElement(
                StylesheetConstructionContext::ELEMNAME_APPLY_TEMPLATES,
                *this,
                attrs);
        assert(childrenElement != 0);

        m_defaultRootRule->appendChildElem(childrenElement);

        m_defaultRootRule->setDefaultTemplate(true);
    }

    assert(m_defaultRule != 0);
    assert(m_defaultTextRule != 0);
    assert(m_defaultRootRule != 0);
}



bool
StylesheetRoot::isCDATASectionElementName(const XalanQName&     theQName) const
{
    XALAN_USING_STD(find_if)

    return find_if(
            m_cdataSectionElems.begin(),
            m_cdataSectionElems.end(),
            pointer_equals_predicate<XalanQName>(&theQName)) != m_cdataSectionElems.end() ? true : false;
}



inline XalanNode*
getKeyNode(XalanNode*   context)
{
    assert(context != 0);

    XalanDocument* const    docNode = 
            XalanNode::DOCUMENT_NODE == context->getNodeType() ?
                    static_cast<XalanDocument*>(context) :
                        context->getOwnerDocument();

    assert(docNode != 0);

    // This is a big hack for dealing with result tree fragments coerced to node-sets using the
    // node-set extension function.  For such cases, the "document" is really the
    // XalanDocumentFragment instance that owns the nodes, not the owner document.
    if (docNode->getFirstChild() != 0)
    {
        return docNode;
    }
    else
    {
        XalanNode*  currentNode = context;

        // OK, the current node belongs to the document, but the document
        // is just a factory for fragments.  This loop will find the
        // parent document fragment.
        for(;;)
        {
            if (currentNode->getNodeType() == XalanNode::DOCUMENT_FRAGMENT_NODE)
            {
                break;
            }
            else
            {
                currentNode = DOMServices::getParentOfNode(*currentNode);

                assert(currentNode != 0);
            }
        }

        return currentNode;
    }
}



void
StylesheetRoot::getNodeSetByKey(
            XalanNode*                      context,
            const XalanQName&               qname,
            const XalanDOMString&           ref,
            const PrefixResolver&           resolver,
            MutableNodeRefList&             nodelist,
            StylesheetExecutionContext&     executionContext,
            const Locator*                  locator,
            KeyTablesTableType&             theKeysTable) const
{
    assert(
        nodelist.empty() == true ||
        nodelist.getDocumentOrder() == true);

    const MutableNodeRefList* nl = 0;
    XalanNode* const    theKeyNode = getKeyNode(context);
    assert(theKeyNode != 0);

    if (m_needToBuildKeysTable == true)
    {
        assert(m_keyDeclarations.empty() == false);

        const KeyTablesTableType::const_iterator    i =
            theKeysTable.find(theKeyNode);

        if (i != theKeysTable.end())
        {
            nl = i->second->getNodeSetByKey(qname, ref);
        }
        else
        {
            XalanMemMgrAutoPtr<KeyTable>    kt(
                        executionContext.getMemoryManager(),
                        KeyTable::create(
                            executionContext.getMemoryManager(),
                             theKeyNode,
                             resolver,
                             m_keyDeclarations,
                             executionContext));

            theKeysTable[theKeyNode] = kt.get();

            const KeyTable* const   theNewTable = kt.releasePtr();

            nl = theNewTable->getNodeSetByKey(qname, ref);
        }
    }

    if (nl == 0)
    {
        typedef StylesheetExecutionContext::GetCachedString     GetCachedString;

        const GetCachedString   theGuard1(executionContext);
        const GetCachedString   theGuard2(executionContext);

        executionContext.problem(
            StylesheetExecutionContext::eXSLTProcessor,
            StylesheetExecutionContext::eError,
            XalanMessageLoader::getMessage(
                theGuard1.get(),
                XalanMessages::UnknownKey_1Param,
                qname.format(theGuard2.get())),
            locator,
            executionContext.getCurrentNode());
    }
    else if (nodelist.empty() == true)
    {
        nodelist = *nl;
    }
    else
    {
        nodelist.addNodesInDocOrder(*nl, executionContext);
    }
}



bool
StylesheetRoot::internalShouldStripSourceNode(const XalanText&  textNode) const
{
    assert(
        textNode.isWhitespace() == true &&
        hasPreserveOrStripSpaceElements() == true);

    const XalanNode* const  parent = textNode.getParentNode();

//  assert(parent != 0);
//  Constructed nodesets may not have parents - see xalan:nodeset()
    if (parent == 0)
        return false;

    if (parent->getNodeType() == XalanNode::ELEMENT_NODE)
    {
        const XalanElement* const   theElement =
                static_cast<const XalanElement*>(parent);

        typedef WhitespaceElementsVectorType::const_iterator    const_iterator;

        const_iterator i = m_whitespaceElements.begin();

        do
        {
            const XalanSpaceNodeTester&     theTester = *i;

            if (theTester(*theElement) != XPath::eMatchScoreNone)
            {
                return theTester.getType() == XalanSpaceNodeTester::eStrip;
            }

            ++i;
        } while (i != m_whitespaceElements.end());
    }

    return false;
}



void
StylesheetRoot::addAttributeSet(ElemAttributeSet&   theAttributeSet)
{
    m_attributeSetsMap[&theAttributeSet.getQName()].push_back(&theAttributeSet);
}



#if !defined(XALAN_RECURSIVE_STYLESHEET_EXECUTION)

const ElemAttributeSet*
StylesheetRoot::getAttributeSet(
            StylesheetExecutionContext&     executionContext,
            const XalanQName&               theQName,
            size_type                       matchingIndex,
            const Locator*                  theLocator) const
{
    const AttributeSetMapType::const_iterator   i =
        m_attributeSetsMap.find(&theQName);

    if (i == m_attributeSetsMap.end())
    {
        const StylesheetExecutionContext::GetCachedString   theGuard(executionContext);

        executionContext.problem(
            StylesheetExecutionContext::eXSLTProcessor,
            StylesheetExecutionContext::eError,
            XalanMessageLoader::getMessage(
                theGuard.get(),
                XalanMessages::UnknownNodeType_1Param,
                Constants::ELEMNAME_ATTRIBUTESET_WITH_PREFIX_STRING),
            theLocator,
            executionContext.getCurrentNode());
    }
    else
    {
        const AttributeSetVectorType&   theAttributeSets = i->second;

        if (matchingIndex < theAttributeSets.size())
        {
            return theAttributeSets[matchingIndex];
        }
    }

    return 0;

}

#else

void
StylesheetRoot::executeAttributeSet(
            StylesheetExecutionContext&     theExecutionContext,
            const XalanQName&               theQName,
            const Locator*                  theLocator) const
{
    const AttributeSetMapType::const_iterator   i =
        m_attributeSetsMap.find(&theQName);

    if (i == m_attributeSetsMap.end())
    {
        const GetCachedString   theGuard(theExecutionContext);

        theExecutionContext.error(
            XalanMessageLoader::getMessage(
                theGuard.get(),
                XalanMessages::UnknownNodeType_1Param,
                Constants::ELEMNAME_ATTRIBUTESET_WITH_PREFIX_STRING),
            theExecutionContext.getCurrentNode(),
            theLocator);
    }
    else
    {
        const AttributeSetVectorType&                   theAttributeSets = (*i).second;
        const AttributeSetVectorType::const_iterator    theEnd = theAttributeSets.end();

        for(AttributeSetVectorType::const_iterator i = theAttributeSets.begin(); i != theEnd; ++i)
        {
            assert(*i != 0);

            (*i)->execute(theExecutionContext);
        }
    }
}
#endif



XALAN_CPP_NAMESPACE_END
