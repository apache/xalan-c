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



// Class header file.
#include "XSLTEngineImpl.hpp"



#include <xercesc/sax/DocumentHandler.hpp>
#include <xercesc/sax/EntityResolver.hpp>
#include <xercesc/sax/Locator.hpp>
#include "xercesc/util/XMLEntityResolver.hpp"
#include "xercesc/util/XMLResourceIdentifier.hpp"



#include <xalanc/Include/STLHelper.hpp>



#include <xalanc/XalanDOM/XalanDOMException.hpp>
#include <xalanc/XalanDOM/XalanDocumentFragment.hpp>
#include <xalanc/XalanDOM/XalanNode.hpp>
#include <xalanc/XalanDOM/XalanAttr.hpp>
#include <xalanc/XalanDOM/XalanComment.hpp>
#include <xalanc/XalanDOM/XalanCDATASection.hpp>
#include <xalanc/XalanDOM/XalanNodeList.hpp>
#include <xalanc/XalanDOM/XalanNamedNodeMap.hpp>
#include <xalanc/XalanDOM/XalanProcessingInstruction.hpp>
#include <xalanc/XalanDOM/XalanText.hpp>



#include <xalanc/PlatformSupport/PrintWriter.hpp>
#include <xalanc/PlatformSupport/StringTokenizer.hpp>
#include <xalanc/PlatformSupport/XalanLocator.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>
#include <xalanc/PlatformSupport/XalanUnicode.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>
#include <xalanc/DOMSupport/DOMSupport.hpp>



#include <xalanc/XMLSupport/FormatterToHTML.hpp>
#include <xalanc/XMLSupport/FormatterTreeWalker.hpp>
#include <xalanc/XMLSupport/XMLParserLiaison.hpp>
#include <xalanc/XMLSupport/FormatterTreeWalker.hpp>



#include <xalanc/XPath/ElementPrefixResolverProxy.hpp>
#include <xalanc/XPath/XalanQNameByReference.hpp>
#include <xalanc/XPath/XObject.hpp>
#include <xalanc/XPath/XObjectFactory.hpp>
#include <xalanc/XPath/XPathEnvSupport.hpp>
#include <xalanc/XPath/XPathEnvSupportDefault.hpp>
#include <xalanc/XPath/XPathExecutionContextDefault.hpp>
#include <xalanc/XPath/XPathFactory.hpp>
#include <xalanc/XPath/XPathFunctionTable.hpp>
#include <xalanc/XPath/XPathProcessorImpl.hpp>



#include "Constants.hpp"
#include "ElemWithParam.hpp"
#include "FunctionCurrent.hpp"
#include "FunctionDocument.hpp"
#include "FunctionElementAvailable.hpp"
#include "FunctionFunctionAvailable.hpp"
#include "FunctionFormatNumber.hpp"
#include "FunctionGenerateID.hpp"
#include "FunctionKey.hpp"
#include "FunctionSystemProperty.hpp"
#include "FunctionUnparsedEntityURI.hpp"
#include "GenerateEvent.hpp"
#include "ProblemListener.hpp"
#include "ProblemListenerDefault.hpp"
#include "Stylesheet.hpp"
#include "StylesheetConstructionContext.hpp"
#include "StylesheetExecutionContext.hpp"
#include "StylesheetHandler.hpp"
#include "StylesheetRoot.hpp"
#include "TraceListener.hpp"
#include "XSLTInputSource.hpp"
#include "XSLTProcessorException.hpp"
#include "XSLTResultTarget.hpp"



//#define XALAN_VQ_SPECIAL_TRACE
#if defined(XALAN_VQ_SPECIAL_TRACE)
#include "C:/Program Files/Rational/Quantify/pure.h"
#endif



XALAN_CPP_NAMESPACE_BEGIN


const XalanDOMString    XSLTEngineImpl::s_emptyString(XalanMemMgrs::getDummyMemMgr());



//==========================================================
// SECTION: Constructors
//==========================================================

XSLTEngineImpl::XSLTEngineImpl(
            MemoryManager&  theManager,
            XMLParserLiaison&   parserLiaison,
            XPathEnvSupport&    xpathEnvSupport,
            DOMSupport&         domSupport,
            XObjectFactory&     xobjectFactory,
            XPathFactory&       xpathFactory) :
    XSLTProcessor(),
    PrefixResolver(),
    m_resultNameSpacePrefix(theManager),
    m_resultNameSpaceURL(theManager),
    m_xpathFactory(xpathFactory),
    m_xobjectFactory(xobjectFactory),
    m_xpathProcessor(theManager, XPathProcessorImpl::create(theManager)),
    m_cdataStack(theManager),
    m_stylesheetLocatorStack(theManager),
    m_defaultProblemListener(theManager),
    m_problemListener(&m_defaultProblemListener),
    m_stylesheetRoot(0),
    m_traceSelects(false),
    m_quietConflictWarnings(true),
    m_diagnosticsPrintWriter(0),
    m_traceListeners(theManager),
    m_uniqueNSValue(0),
    m_topLevelParams(theManager),
    m_parserLiaison(parserLiaison),
    m_xpathEnvSupport(xpathEnvSupport),
    m_domSupport(domSupport),
    m_executionContext(0),
    m_outputContextStack(theManager),
    m_resultNamespacesStack(theManager),
    m_dummyAttributesList(theManager),
    m_scratchString(theManager),
    m_attributeNamesVisited(theManager),
    m_hasCDATASectionElements(false),
    m_xpathConstructionContext(theManager),
    m_stylesheetParams(theManager)
{
    m_outputContextStack.pushContext();
}



void
XSLTEngineImpl::reset()
{
    m_topLevelParams.clear();
    m_stylesheetLocatorStack.clear();
    m_cdataStack.clear();

    m_stylesheetRoot = 0;

    m_outputContextStack.reset();

    m_outputContextStack.pushContext();

    m_xpathEnvSupport.reset();
    m_xpathFactory.reset();
    m_xobjectFactory.reset();
    m_domSupport.reset();

    m_resultNamespacesStack.clear();

    m_attributeNamesVisited.clear();

    m_hasCDATASectionElements = false;

    m_xpathConstructionContext.reset();
}



XSLTEngineImpl::~XSLTEngineImpl()
{
    reset();
}



void
XSLTEngineImpl::problem(
            eSource                 source,
            eClassification         classification,
            const XalanDOMString&   msg,
            const Locator*          locator,
            const XalanNode*        sourceNode)
{
    if (m_problemListener != 0)
    {
        m_problemListener->problem(
                    source,
                    classification,
                    msg,
                    locator,
                    sourceNode);
    }

    if (classification == ProblemListener::eERROR)
    {
        throw XSLTProcessorException(
                getMemoryManager(),
                msg,
                locator);
    }
}



void
XSLTEngineImpl::problem(
            eSource                 source,
            eClassification         classification,
            const XalanDOMString&   msg,
            const XalanNode*        sourceNode)
{
    if (m_problemListener != 0)
    {
        m_problemListener->problem(
                    source,
                    classification,
                    msg,
                    sourceNode);
    }

    if (classification == ProblemListener::eERROR)
    {
        throw XSLTProcessorException(
                getMemoryManager(),
                msg);
    }
}



void
XSLTEngineImpl::process(
            const XSLTInputSource&          inputSource, 
            const XSLTInputSource&          stylesheetSource,
            XSLTResultTarget&               outputTarget,
            StylesheetConstructionContext&  constructionContext,
            StylesheetExecutionContext&     executionContext)
{
    XalanDOMString  xslIdentifier(executionContext.getMemoryManager());

    if (0 == stylesheetSource.getSystemId())
    {
        XalanMessageLoader::getMessage(
            xslIdentifier,
            XalanMessages::InputXSL);
    }
    else
    {
        xslIdentifier = stylesheetSource.getSystemId();
    }

    XalanNode*  sourceTree = getSourceTreeFromInput(inputSource);

    m_stylesheetRoot = processStylesheet(stylesheetSource, constructionContext);

    if(0 != sourceTree && m_stylesheetRoot == 0)
    {
        // Didn't get a stylesheet from the input source, so look for a
        // stylesheet processing instruction...

        // The PI must be a child of the document...
        const XalanNode*    child = sourceTree->getFirstChild();

        XalanDOMString      theCurrentToken(executionContext.getMemoryManager());
        XalanDOMString      theStylesheetURI(executionContext.getMemoryManager());

        bool            isOK = false;

        while(child != 0 && isOK == false && theStylesheetURI.empty() == true)
        {
            if(XalanNode::PROCESSING_INSTRUCTION_NODE == child->getNodeType())
            {
                const XalanDOMString&   nodeName = child->getNodeName();

                if(equals(nodeName, s_stylesheetNodeName))
                {
                    StringTokenizer     tokenizer(child->getNodeValue(), s_piTokenizerString);

                    while(tokenizer.hasMoreTokens() == true && (isOK == false || theStylesheetURI.empty() == true))
                    {
                        tokenizer.nextToken(theCurrentToken);

                        if(equals(theCurrentToken, s_typeString))
                        {
                            tokenizer.nextToken(theCurrentToken);

                            const XalanDOMString::size_type     theLength =
                                    theCurrentToken.length();

                            if (theLength > 2)
                            {
                                theCurrentToken.erase(theLength - 1, 1);
                                theCurrentToken.erase(0, 1);

                                if(equals(theCurrentToken, s_typeValueString1) ||
                                    equals(theCurrentToken, s_typeValueString2) ||
                                    equals(theCurrentToken, s_typeValueString3) ||
                                    equals(theCurrentToken, s_typeValueString4))
                                {
                                    isOK = true;
                                }
                            }
                        }
                        else if(equals(theCurrentToken, s_hrefString))
                        {
                            tokenizer.nextToken(theCurrentToken);

                            const XalanDOMString::size_type     theLength =
                                    theCurrentToken.length();

                            if (theLength > 2)
                            {
                                // Trim of the starting and trailing delimiters...
                                theStylesheetURI.assign(theCurrentToken, 1, theLength - 2);
                            }
                        }
                    }
                }
            }

            child = child->getNextSibling();
        }

        if (isOK == true && theStylesheetURI.empty() == false)
        {
            const CCGetCachedString     theGuard(constructionContext);

            XalanDOMString&             sysid = theGuard.get();

            const XalanDOMChar* const   pxch = inputSource.getSystemId();

            if (pxch != 0)
            {
                sysid.assign(pxch);
            }

            getStylesheetFromPIURL(
                    theStylesheetURI,
                    *sourceTree,
                    sysid,
                    true,
                    constructionContext);
        }
    }

    if(0 == m_stylesheetRoot)
    {
        const ECGetCachedString     theGuard(executionContext);

        error(
            XalanMessageLoader::getMessage(
                theGuard.get(),
                XalanMessages::FailedToProcessStylesheet),
                0,
                sourceTree);
    }
    else if(0 != sourceTree)
    {
        executionContext.setStylesheetRoot(m_stylesheetRoot);

        FormatterListener* const    theFormatter =
                outputTarget.getFormatterListener();

        if (theFormatter != 0 && theFormatter->getPrefixResolver() == 0)
        {
            theFormatter->setPrefixResolver(this);
        }

        m_hasCDATASectionElements = m_stylesheetRoot->hasCDATASectionElements();

        m_stylesheetRoot->process(sourceTree, outputTarget, executionContext);
    }
}



void
XSLTEngineImpl::process(
            const XSLTInputSource&          inputSource, 
            XSLTResultTarget&               outputTarget,
            StylesheetExecutionContext&     executionContext)
{
    XalanNode* const    sourceTree = getSourceTreeFromInput(inputSource);

    if(0 != sourceTree)
    {
        if (m_stylesheetRoot == 0)
        {
            const ECGetCachedString     theGuard(executionContext);

            error(
                XalanMessageLoader::getMessage(
                    theGuard.get(),
                    XalanMessages::NoStylesheet),
                    0,
                    sourceTree);
        }

        FormatterListener* const    theFormatter =
                outputTarget.getFormatterListener();

        if (theFormatter != 0 && theFormatter->getPrefixResolver() == 0)
        {
            theFormatter->setPrefixResolver(this);
        }

        m_hasCDATASectionElements = m_stylesheetRoot->hasCDATASectionElements();

        m_stylesheetRoot->process(sourceTree, outputTarget, executionContext);
    }
}



StylesheetRoot*
XSLTEngineImpl::processStylesheet(
            const XalanDOMString&           xsldocURLString,
            StylesheetConstructionContext&  constructionContext)
{
    const XSLTInputSource   input(
                                xsldocURLString.c_str(),
                                constructionContext.getMemoryManager());

    return processStylesheet(input, constructionContext);
}



StylesheetRoot*
XSLTEngineImpl::processStylesheet(
            const XSLTInputSource&          stylesheetSource,
            StylesheetConstructionContext&  constructionContext)
{
    StylesheetRoot*     theStylesheet = 0;

    const XalanDOMChar* const   systemID = stylesheetSource.getSystemId();
    XalanNode* const            stylesheetNode = stylesheetSource.getNode();

    if (systemID != 0 || stylesheetNode != 0 || stylesheetSource.getStream() != 0)
    {
        const CCGetCachedString     theGuard(constructionContext);

        XalanDOMString&     xslIdentifier = theGuard.get();

        theStylesheet = constructionContext.create(stylesheetSource);

        StylesheetHandler   stylesheetProcessor(*theStylesheet, constructionContext);

        if(stylesheetNode != 0)
        {
            const XalanNode::NodeType   theType = stylesheetNode->getNodeType();

            if (theType != XalanNode::ELEMENT_NODE && theType != XalanNode::DOCUMENT_NODE)
            {
                const CCGetCachedString     theGuard(constructionContext);

                error(
                    XalanMessageLoader::getMessage(
                        theGuard.get(),
                        XalanMessages::CompilingDOMStylesheetReqDocument),
                        constructionContext.getLocatorFromStack(),
                        0);
            }
            else
            {

                XalanMessageLoader::getMessage(
                    xslIdentifier,
                    XalanMessages::InputXSL);

                FormatterTreeWalker tw(stylesheetProcessor, constructionContext.getMemoryManager());

                if (theType == XalanNode::DOCUMENT_NODE)
                {
                    tw.traverse(stylesheetNode);
                }
                else
                {
                    stylesheetProcessor.startDocument();

                    tw.traverseSubtree(stylesheetNode);

                    stylesheetProcessor.endDocument();
                }
            }
        }
        else
        {
            if (systemID != 0)
            {
                xslIdentifier = systemID;
            }

            m_parserLiaison.parseXMLStream(
                stylesheetSource,
                stylesheetProcessor,
                s_emptyString);
        }

        theStylesheet->postConstruction(constructionContext);
    }

    return theStylesheet;
}



//==========================================================
// SECTION: XML Parsing Functions
//==========================================================

XalanNode*
XSLTEngineImpl::getSourceTreeFromInput(const XSLTInputSource&   inputSource)
{
    XalanNode*      sourceTree = inputSource.getNode();

    if(0 == sourceTree)
    {
        const CCGetCachedString     theGuard1(m_xpathConstructionContext);

        XalanDOMString&     xmlIdentifier = theGuard1.get();

        const XalanDOMChar* const   theSystemID = inputSource.getSystemId();

        if (0 != theSystemID)
        {
            try
            {
                URISupport::getURLStringFromString(theSystemID, xmlIdentifier);
            }
            catch(const XERCES_CPP_NAMESPACE_QUALIFIER XMLException&)
            {
                xmlIdentifier = theSystemID;
            }
        }

#if defined(XALAN_VQ_SPECIAL_TRACE)
        QuantifyStartRecordingData();
#endif

        XalanDocument* const    theDocument =
            m_parserLiaison.parseXMLStream(
                inputSource,
                xmlIdentifier);
        assert(theDocument != 0);

#if defined(XALAN_VQ_SPECIAL_TRACE)
        QuantifyStopRecordingData();
#endif

        m_xpathEnvSupport.setSourceDocument(xmlIdentifier, theDocument);

        sourceTree = theDocument;
    }

    return sourceTree;
}



const XalanDOMString*
XSLTEngineImpl::getNamespaceForPrefix(const XalanDOMString&     prefix) const
{
    return m_resultNamespacesStack.getNamespaceForPrefix(prefix);
}



const XalanDOMString&
XSLTEngineImpl::getURI() const
{
    return s_emptyString;
}



XalanDocument*
XSLTEngineImpl::parseXML(
            const XalanDOMString&   urlString,
            DocumentHandler*        docHandler,
            XalanDocument*          docToRegister,
            ErrorHandler*           theErrorHandler)
{
    
    XalanDocument*  doc =
            m_xpathEnvSupport.getSourceDocument(urlString);

    if(doc == 0)
    {
        EntityResolver* const   theResolver = 
            m_parserLiaison.getEntityResolver();

        XMLEntityResolver* const    theXMLResolver = 
            m_parserLiaison.getXMLEntityResolver();

        XalanAutoPtr<InputSource>   resolverInputSource;

        if (theResolver != 0)
        {
            resolverInputSource.reset(theResolver->resolveEntity(0, urlString.c_str()));
        }
        else if (theXMLResolver != 0)
        {
            XALAN_USING_XERCES(XMLResourceIdentifier);

            XMLResourceIdentifier   theIndentifier(
                XMLResourceIdentifier::ExternalEntity,
                urlString.c_str());

            resolverInputSource.reset(
                theXMLResolver->resolveEntity(&theIndentifier));
        }

        if (resolverInputSource.get() != 0)
        {
            doc = parseXML(
                        *resolverInputSource.get(),
                        docHandler,
                        docToRegister,
                        theErrorHandler);
        }
        else
        {
            const XSLTInputSource   inputSource(
                                        urlString.c_str(),
                                        m_parserLiaison.getMemoryManager());

            doc = parseXML(
                        inputSource,
                        docHandler,
                        docToRegister,
                        theErrorHandler);
        }

        if (doc != 0)
        {
            m_xpathEnvSupport.setSourceDocument(urlString, doc);
        }
    }

    return doc;
}



XalanDocument*
XSLTEngineImpl::parseXML(
            const InputSource&  inputSource,
            DocumentHandler*    docHandler,
            XalanDocument*      docToRegister,
            ErrorHandler*       theErrorHandler)
{
    typedef XMLParserLiaison::EnsureResetErrorHandler   EnsureResetErrorHandler;

    EnsureResetErrorHandler     theGuard;

    if (theErrorHandler != 0)
    {
        theGuard.set(
            &m_parserLiaison,
            theErrorHandler);
    }

    if (0 != docHandler)
    {
        m_parserLiaison.parseXMLStream(
            inputSource,
            *docHandler,
            s_emptyString);

        return docToRegister;
    }
    else
    {
        return m_parserLiaison.parseXMLStream(
                    inputSource,
                    s_emptyString);
    }
}



Stylesheet*
XSLTEngineImpl::getStylesheetFromPIURL(
            const XalanDOMString&           xslURLString,
            XalanNode&                      fragBase,
            const XalanDOMString&           xmlBaseIdent,
            bool                            isRoot,
            StylesheetConstructionContext&  constructionContext)
{
    MemoryManager&          theMemoryManager =
        constructionContext.getMemoryManager();

    Stylesheet*             stylesheet = 0;

    const CCGetCachedString     theGuard(constructionContext);
    XalanDOMString&             stringHolder = theGuard.get();

    const CCGetCachedString     theGuard1(constructionContext);
    XalanDOMString&     localXSLURLString = theGuard1.get();    
    
    trim(xslURLString, localXSLURLString);

    const XalanDOMString::size_type     fragIndex =
        indexOf(localXSLURLString, XalanUnicode::charNumberSign);

    if(fragIndex == 0)
    {
        const CCGetCachedString     theGuard(constructionContext);

        XalanDOMString& fragID = theGuard.get();

        fragID.assign(localXSLURLString);

        const XalanElement*     nsNode = 0;

        const XalanNode::NodeType   theType = fragBase.getNodeType();

        if (theType == XalanNode::DOCUMENT_NODE)
        {
            const XalanDocument&    doc =
                static_cast<const XalanDocument&>(fragBase);

            nsNode = doc.getDocumentElement(); 
        }
        else if (theType == XalanNode::ELEMENT_NODE)
        {
            nsNode = static_cast<const XalanElement*>(&fragBase);
        }
        else
        {
            XalanNode* const    node = fragBase.getParentNode();

            if  (node->getNodeType() == XalanNode::ELEMENT_NODE) 
            {
                nsNode = static_cast<const XalanElement*>(node);
            }
            else
            {
                const CCGetCachedString     theGuard(constructionContext);

                error(
                    XalanMessageLoader::getMessage(
                        theGuard.get(),
                        XalanMessages::CantFindFragment_1Param,
                        fragID),
                    constructionContext.getLocatorFromStack(),
                    &fragBase);
            }
        }

        // Try a bunch of really ugly stuff to find the fragment.
        // What's the right way to do this?
        const CCGetCachedString     theGuard3(constructionContext);

        XalanDOMString& ds = theGuard3.get();

        ds.append("id(");

        ds += fragID;
        ds.append(")");

        ElementPrefixResolverProxy      theProxy(nsNode, m_xpathEnvSupport, m_domSupport, theMemoryManager);

        XPathExecutionContextDefault    theExecutionContext(m_xpathEnvSupport,
                                                            m_domSupport,
                                                            m_xobjectFactory,
                                                            &fragBase,
                                                            0,
                                                            &theProxy);

        const XObjectPtr    xobj(evalXPathStr(ds, theExecutionContext));
        assert(xobj.null() == false);

        NodeRefList     nl( xobj->nodeset(), theMemoryManager);

        if(nl.getLength() == 0)
        {
            ds.assign("//*[@id='");
            ds += fragID;
            ds.append("']");

            const XObjectPtr    xobj(evalXPathStr(ds, theExecutionContext));
            assert(xobj.null() == false);

            nl = xobj->nodeset();

            if(nl.getLength() == 0)
            {
                ds.assign("//*[@name='");
                ds += fragID;
                ds.append("']");

                const XObjectPtr    xobj(evalXPathStr(ds, theExecutionContext));
                assert(xobj.null() == false);

                nl = xobj->nodeset();

                if(nl.getLength() == 0)
                {
                    // Well, hell, maybe it's an XPath...
                    const XObjectPtr    xobj(evalXPathStr(fragID, theExecutionContext));
                    assert(xobj.null() == false);

                    nl = xobj->nodeset();
                }
            }
        }

        if (nl.getLength() == 0)
        {
            const CCGetCachedString     theGuard(constructionContext);

            error(
                XalanMessageLoader::getMessage(
                    theGuard.get(),
                    XalanMessages::CantFindFragment_1Param,
                    fragID),
                constructionContext.getLocatorFromStack(),
                &fragBase);
        }

        XalanNode* const    frag = nl.item(0);

        if (XalanNode::ELEMENT_NODE == frag->getNodeType())
        {
            XalanMemMgrAutoPtr<Stylesheet>  theGuard;

            if (isRoot)
            {
                StylesheetRoot* const   theLocalRoot =
                    constructionContext.create(stringHolder);

                stylesheet = theLocalRoot;

                m_stylesheetRoot = theLocalRoot;
            }
            else
            {
                stylesheet = constructionContext.create(*const_cast<StylesheetRoot*>(m_stylesheetRoot), stringHolder);

                theGuard.reset(&theMemoryManager, stylesheet);
            }

            StylesheetHandler stylesheetProcessor(*stylesheet, constructionContext);

            FormatterTreeWalker tw(stylesheetProcessor,  theMemoryManager);

            stylesheetProcessor.startDocument();

            tw.traverseSubtree(frag);

            stylesheetProcessor.endDocument();

            stylesheet->postConstruction(constructionContext);

            theGuard.release();
        }
        else
        {
            const CCGetCachedString     theGuard(constructionContext);

            error(
                XalanMessageLoader::getMessage(
                    theGuard.get(),
                    XalanMessages::NodePointedByFragment_1Param,
                    fragID),
                constructionContext.getLocatorFromStack(),
                &fragBase);
        }
    }
    else
    {
        XalanMemMgrAutoPtr<Stylesheet>  theGuard;

        const XalanDocument* const  theOwnerDocument =
                fragBase.getNodeType() == XalanNode::DOCUMENT_NODE ?
                    static_cast<const XalanDocument*>(&fragBase) :
                    fragBase.getOwnerDocument();
        assert(theOwnerDocument != 0);

        // Catch any XMLExceptions thrown, since we may not
        // be able to resolve the URL.  In that case, the
        // parser will throw an error.  We do this because
        // we don't know what sort of EntityResolvers might
        // be active, so we never want to error out here.
        try
        {
            if (xmlBaseIdent.empty() == true)
            {
                URISupport::getURLStringFromString(
                            localXSLURLString,
                            m_xpathEnvSupport.findURIFromDoc(theOwnerDocument),
                            localXSLURLString);
            }
            else
            {
                URISupport::getURLStringFromString(
                            localXSLURLString,
                            xmlBaseIdent,
                            localXSLURLString);
            }
        }
        catch(const XERCES_CPP_NAMESPACE_QUALIFIER XMLException&)
        {
        }

        if(isRoot)
        {
            StylesheetRoot* const   theLocalRoot =
                    constructionContext.create(localXSLURLString);

            stylesheet = theLocalRoot;

            m_stylesheetRoot = theLocalRoot;
        }
        else
        {
            stylesheet = Stylesheet::create(
                            theMemoryManager,
                            *const_cast<StylesheetRoot*>(m_stylesheetRoot),
                            localXSLURLString,
                            constructionContext);

            theGuard.reset(&theMemoryManager, stylesheet);
        }

        StylesheetHandler stylesheetProcessor(*stylesheet, constructionContext);

        typedef StylesheetConstructionContext::URLAutoPtrType   URLAutoPtrType;

        URLAutoPtrType  xslURL(constructionContext.getURLFromString(localXSLURLString));

        XSLTInputSource     inputSource(xslURL->getURLText(), theMemoryManager);

        m_parserLiaison.parseXMLStream(inputSource, stylesheetProcessor, s_emptyString);

        stylesheet->postConstruction(constructionContext);

        theGuard.release();
    }

    return stylesheet;
}



const StylesheetRoot*
XSLTEngineImpl::getStylesheetRoot() const
{
    return m_stylesheetRoot;
}



void
XSLTEngineImpl::setStylesheetRoot(const StylesheetRoot*     theStylesheet)
{
    m_stylesheetRoot = theStylesheet;
}



void
XSLTEngineImpl::setExecutionContext(StylesheetExecutionContext*     theExecutionContext)
{
    m_executionContext = theExecutionContext;
}



//==========================================================
// SECTION: Diagnostic functions
//==========================================================

XSLTEngineImpl::size_type
XSLTEngineImpl::getTraceListeners() const
{
    return m_traceListeners.size();
}



void
XSLTEngineImpl::addTraceListener(TraceListener* tl)
{
    if (tl != 0)
    {
        m_traceListeners.push_back(tl);
    }
}



void
XSLTEngineImpl::removeTraceListener(TraceListener*  tl)
{
    XALAN_USING_STD(remove)

    const TraceListenerVectorType::iterator     i =
        remove(
            m_traceListeners.begin(),
            m_traceListeners.end(),
            tl);

    m_traceListeners.erase(i);
}



void
XSLTEngineImpl::fireGenerateEvent(const GenerateEvent&  ge)
{
    XALAN_USING_STD(for_each)

    for_each(
        m_traceListeners.begin(),
        m_traceListeners.end(),
        TraceListener::TraceListenerGenerateFunctor(ge));
}



void
XSLTEngineImpl::fireSelectEvent(const SelectionEvent&   se)
{
    XALAN_USING_STD(for_each)

    for_each(
        m_traceListeners.begin(),
        m_traceListeners.end(),
        TraceListener::TraceListenerSelectFunctor(se));
}



void
XSLTEngineImpl::fireTraceEvent(const TracerEvent& te)
{
    XALAN_USING_STD(for_each)

    for_each(
        m_traceListeners.begin(),
        m_traceListeners.end(),
        TraceListener::TraceListenerTraceFunctor(te));
}



bool
XSLTEngineImpl::getTraceSelects() const
{
    return m_traceSelects;
}



void
XSLTEngineImpl::setTraceSelects(bool    b)
{
    m_traceSelects = b;
}



void
XSLTEngineImpl::setDiagnosticsOutput(PrintWriter*   pw)
{
    m_diagnosticsPrintWriter = pw;

    m_problemListener->setPrintWriter(pw);
}



void
XSLTEngineImpl::setQuietConflictWarnings(bool   b)
{
    m_quietConflictWarnings = b;
}



void
XSLTEngineImpl::setDocumentLocator(const Locator*   /* locator */)
{
    // Do nothing for now
}



void
XSLTEngineImpl::traceSelect(
            StylesheetExecutionContext&     executionContext,
            const ElemTemplateElement&      theTemplate,
            const NodeRefListBase&          nl,
            const XPath*                    xpath) const
{
    if (0 != m_diagnosticsPrintWriter)
    {
        const ECGetCachedString     theGuard(executionContext);

        XalanDOMString& msg = theGuard.get();

        msg.assign(theTemplate.getElementName());
        msg.append(": ");

        if(xpath != 0)
        {
            msg += xpath->getExpression().getCurrentPattern();
            msg.append(", ");
        }
        else
        {
            msg.append("*|text(), (default select), ");
        }
        const ECGetCachedString     theGuard1(executionContext);

        XalanDOMString& theBuffer = theGuard1.get();
        NumberToDOMString(nl.getLength(), theBuffer);

        msg += theBuffer;
            
        msg.append(" selected");

        const XalanQName* const     mode = executionContext.getCurrentMode();

        if(mode != 0 && mode->isEmpty() == false)
        {
            msg += XalanDOMString(", mode = {", executionContext.getMemoryManager());
            msg += mode->getNamespace();
            msg += XalanDOMString("}", executionContext.getMemoryManager());
            msg += mode->getLocalPart();
        }

        m_diagnosticsPrintWriter->println(msg);
    }
}



void
XSLTEngineImpl::startDocument()
{
    assert(getFormatterListenerImpl() != 0);
    assert(m_executionContext != 0);

    if (getHasPendingStartDocument() == false)
    {
        assert(m_cdataStack.empty() == true || m_outputContextStack.size() != 1);

        m_resultNamespacesStack.pushContext();

        setHasPendingStartDocument(true);

        setMustFlushPendingStartDocument(false);

        if (m_hasCDATASectionElements == true)
        {
            m_cdataStack.push_back(false);
        }
    }
    else if (getMustFlushPendingStartDocument() == true)
    {
        getFormatterListenerImpl()->startDocument();

        if(getTraceListeners() > 0)
        {
            const GenerateEvent     ge(GenerateEvent::EVENTTYPE_STARTDOCUMENT,  getMemoryManager());

            fireGenerateEvent(ge);
        }

        // Reset this, but leave getMustFlushPendingStartDocument() alone,
        // since it will still be needed.
        setHasPendingStartDocument(false);
    }
}



void
XSLTEngineImpl::endDocument()
{
    assert(getFormatterListenerImpl() != 0);
    assert(m_executionContext != 0);

    setMustFlushPendingStartDocument(true);

    flushPending();

    getFormatterListenerImpl()->endDocument();

    if(getTraceListeners() > 0)
    {
        const GenerateEvent     ge(GenerateEvent::EVENTTYPE_ENDDOCUMENT,  getMemoryManager());

        fireGenerateEvent(ge);
    }

    if (m_hasCDATASectionElements == true)
    {
        m_cdataStack.pop_back();
    }
    assert(m_cdataStack.empty() == true);

    m_resultNamespacesStack.popContext();

    assert(m_resultNamespacesStack.empty() == true);
}



void
XSLTEngineImpl::addResultAttribute(
            AttributeListImpl&      attList,
            const XalanDOMString&   aname,
            const XalanDOMChar*     value,
            size_type               theLength,
            bool                    fromCopy,
            const Locator*          locator)
{
    assert(value != 0);

    // Always exclude the implicit XML declaration...
    if (equals(aname, DOMServices::s_XMLNamespacePrefix) == false) 
    {
        bool    fExcludeAttribute = false;

        if (equals(aname, DOMServices::s_XMLNamespace) == true)
        {
            // OK, we're adding a default namespace declaration.  So see if the length
            // of the namespace is 0.  If it's not, go ahead and add the declaration.
            // If it's not, it means we're "turning off" the previous default
            // declaration.

            const XalanDOMString* const     currentDefaultNamespace =
                        getNamespaceForPrefix(s_emptyString);

            // Note that we use an empty string for the prefix, instead of "xmlns", since the
            // prefix really is "".
            if (theLength != 0)
            {
                if (currentDefaultNamespace != 0 &&
                    equals(*currentDefaultNamespace, value, theLength) == true)
                {
                        fExcludeAttribute = true;
                }
                else
                {
                    if (fromCopy == false ||
                        m_resultNamespacesStack.prefixIsPresentLocal(s_emptyString) == false)
                    {
                        addResultNamespaceDecl(s_emptyString, value, theLength);
                    }
                    else
                    {
                        reportDuplicateNamespaceNodeError(s_emptyString, locator);
                    }
                }
            }
            else
            {
                // OK, we're turning of the previous default namespace declaration.
                // Check to see if there is one, and if there isn't, don't add
                // the namespace declaration _and_ don't add the attribute.
                if (currentDefaultNamespace != 0 && currentDefaultNamespace->empty() == false)
                {
                    addResultNamespaceDecl(s_emptyString, value, theLength);
                }
                else
                {
                    fExcludeAttribute = true;
                }
            }
        }
        else if (startsWith(aname, DOMServices::s_XMLNamespaceWithSeparator) == true)
        {
            assert(m_executionContext != 0);

            const ECGetCachedString     prefixGuard(*m_executionContext);

            XalanDOMString&     prefix = prefixGuard.get();

            substring(aname, prefix, DOMServices::s_XMLNamespaceWithSeparatorLength);

            const XalanDOMString* const theNamespace = getResultNamespaceForPrefix(prefix);

            const XalanDOMString::size_type     theLength = length(value);

            if (theNamespace == 0)
            {
                addResultNamespaceDecl(prefix, value, theLength);
            }
            else if (equals(*theNamespace, value, theLength) == false)
            {
                if (fromCopy == false /* ||
                    m_resultNamespacesStack.prefixIsPresentLocal(prefix) == false */)
                {
                    addResultNamespaceDecl(prefix, value, theLength);
                }
                else
                {
                    reportDuplicateNamespaceNodeError(prefix, locator);
                }
            }
            else
            {
                fExcludeAttribute = true;
            }
        }

        if (fExcludeAttribute == false)
        {
            attList.addAttribute(
                aname.c_str(),
                Constants::ATTRTYPE_CDATA.c_str(),
                value);
        }
    }
}



void
XSLTEngineImpl::reportDuplicateNamespaceNodeError(
            const XalanDOMString&   theName,
            const Locator*          locator)
{
    assert(m_executionContext != 0);

    const ECGetCachedString     theGuard(*m_executionContext);

    XalanDOMString&     theMessage =
            theGuard.get();

    if (theName.length() == 0)
    {
        XalanMessageLoader::getMessage(
            theMessage,
            XalanMessages::ErrorCopyingNamespaceNodeForDefault);
    }
    else
    {
        XalanMessageLoader::getMessage(
            theMessage,
            XalanMessages::ErrorCopyingNamespaceNode_1Param,
            theName);
    }

    error(
        theMessage,
        locator,
        m_executionContext->getCurrentNode());
}



bool
XSLTEngineImpl::pendingAttributesHasDefaultNS() const
{
    const AttributeListImpl&    thePendingAttributes =
        (const AttributeListImpl&)getPendingAttributes();

    const XalanSize_t   n = thePendingAttributes.getLength();

    for (XalanSize_t i = 0; i < n; i++)
    {
        if (equals(
                thePendingAttributes.getName(i),
                DOMServices::s_XMLNamespace) == true)
        {
            return true;
        }
    }

    return false;
}



void
XSLTEngineImpl::flushPending()
{
    if(getHasPendingStartDocument() == true && isElementPending() == true)
    {
        assert(getFormatterListenerImpl() != 0);
        assert(m_executionContext != 0);

        if (m_stylesheetRoot->isOutputMethodSet() == false)
        {
            if (equalsIgnoreCaseASCII(
                    getPendingElementName(),
                    Constants::ELEMNAME_HTML_STRING) == true &&
                pendingAttributesHasDefaultNS() == false)
            {
                FormatterListener* const    theFormatter =
                            getFormatterListenerImpl();
                assert(theFormatter != 0);

                Writer* const theWriter = theFormatter->getWriter();

                if (theWriter == 0)
                {
                    const ECGetCachedString     theGuard(*m_executionContext);

                    XalanDOMString&     theMessage =
                        theGuard.get();

                    assert(m_stylesheetRoot->getDefaultRootRule() != 0);

                    const Locator* const theLocator =
                        m_stylesheetRoot->getDefaultRootRule()->getLocator();

                    XalanMessageLoader::getMessage(
                        theMessage,
                        XalanMessages::CannotSwitchToHTMLOutputMethod);

                    warn(
                        theMessage,
                        theLocator,
                        0);
                }
                else if (theFormatter->getOutputFormat() == FormatterListener::OUTPUT_METHOD_XML)
                {
                    // Yuck!!! Ugly hack to switch to HTML on-the-fly.
                    setFormatterListenerImpl(
                        m_executionContext->createFormatterToHTML(
                            *theFormatter->getWriter(),
                            theFormatter->getEncoding(),
                            theFormatter->getMediaType(),
                            theFormatter->getDoctypeSystem(),
                            theFormatter->getDoctypePublic(),
                            m_stylesheetRoot->getHTMLOutputIndent(),
                            theFormatter->getIndent() > 0 ?
                                theFormatter->getIndent() :
                                StylesheetExecutionContext::eDefaultHTMLIndentAmount));

                    if (m_hasCDATASectionElements == true)
                    {
                        m_hasCDATASectionElements = false;
                    }
                }
            }
        }
    }

    if(getHasPendingStartDocument() == true &&
       getMustFlushPendingStartDocument() == true)
    {
        startDocument();
    }

    XalanDOMString&     thePendingElementName = getPendingElementNameImpl();

    if(thePendingElementName.empty() == false &&
       getMustFlushPendingStartDocument() == true)
    {
        assert(getFormatterListenerImpl() != 0);
        assert(m_executionContext != 0);

        if(m_hasCDATASectionElements == true)
        {
            m_cdataStack.push_back(isCDataResultElem(thePendingElementName));
        }

        AttributeListImpl&  thePendingAttributes =
                getPendingAttributesImpl();

        getFormatterListenerImpl()->startElement(
                thePendingElementName.c_str(),
                thePendingAttributes);

        if(getTraceListeners() > 0)
        {
            const GenerateEvent     ge(
                GenerateEvent::EVENTTYPE_STARTELEMENT,
                 getMemoryManager(),
                thePendingElementName,
                &thePendingAttributes);

            fireGenerateEvent(ge);
        }

        thePendingAttributes.clear();

        thePendingElementName.clear();
    }
}



void
XSLTEngineImpl::startElement(const XalanDOMChar*    name)
{
    assert(getFormatterListenerImpl() != 0);
    assert(name != 0);

    flushPending();

    m_resultNamespacesStack.pushContext();

    setPendingElementName(name);

    setMustFlushPendingStartDocument(true);
}



void
XSLTEngineImpl::startElement(
            const XalanDOMChar*     name,
            AttributeListType&      atts)
{
    assert(getFormatterListenerImpl() != 0);
    assert(name != 0);

    flushPending();

    const XalanSize_t   nAtts = atts.getLength();

    assert(m_outputContextStack.empty() == false);

    AttributeListImpl&  thePendingAttributes =
        getPendingAttributesImpl();

    thePendingAttributes.clear();

    for (XalanSize_t i = 0; i < nAtts; i++)
    {
        thePendingAttributes.addAttribute(
            atts.getName(i),
            atts.getType(i),
            atts.getValue(i));
    }

    m_resultNamespacesStack.pushContext();

    setPendingElementName(name);
}



void
XSLTEngineImpl::endElement(const XalanDOMChar*  name)
{
    assert(getFormatterListenerImpl() != 0);
    assert(name != 0);

    flushPending();

    getFormatterListenerImpl()->endElement(name);

    if(getTraceListeners() > 0)
    {
        const GenerateEvent     ge(GenerateEvent::EVENTTYPE_ENDELEMENT, 
                                     getMemoryManager(),
                                     name, 
                                     &getPendingAttributesImpl());

        fireGenerateEvent(ge);
    }

    m_resultNamespacesStack.popContext();

    if(m_hasCDATASectionElements == true)
    {
        assert(m_cdataStack.empty() == false);

        m_cdataStack.pop_back();
    }
}



void
XSLTEngineImpl::characters(
            const XalanDOMChar*     ch,
            size_type               length)
{
    characters(
        ch,
        0,
        length);
}



void
XSLTEngineImpl::characters(
            const XalanDOMChar*     ch,
            size_type               start,
            size_type               length)
{
    assert(getFormatterListenerImpl() != 0);
    assert(ch != 0);
    assert(m_hasCDATASectionElements == m_stylesheetRoot->hasCDATASectionElements());
//    assert(length != 0);

    doFlushPending();

    if (generateCDATASection() == true)
    {
        getFormatterListenerImpl()->cdata(ch + start, length);

        if (getTraceListeners() > 0)
        {
            fireCharacterGenerateEvent(ch, start, length, true);
        }
    }
    else
    {
        getFormatterListenerImpl()->characters(ch + start, length);

        if (getTraceListeners() > 0)
        {
            fireCharacterGenerateEvent(ch, start, length, false);
        }
    }
}



void
XSLTEngineImpl::characters(const XalanNode&     node)
{
    assert(getFormatterListenerImpl() != 0);
    assert(m_hasCDATASectionElements == m_stylesheetRoot->hasCDATASectionElements());

    doFlushPending();

    if(generateCDATASection() == true)
    {
        DOMServices::getNodeData(
            node,
            *m_executionContext,
            *getFormatterListenerImpl(),
            &FormatterListener::cdata);

        if(getTraceListeners() > 0)
        {
            fireCharacterGenerateEvent(node, true);
        }
    }
    else
    {
        DOMServices::getNodeData(
            node,
            *m_executionContext,
            *getFormatterListenerImpl(),
            &FormatterListener::characters);

        if(getTraceListeners() > 0)
        {
            fireCharacterGenerateEvent(node, false);
        }
    }
}



void
XSLTEngineImpl::characters(const XObjectPtr&    xobject)
{
    assert(getFormatterListenerImpl() != 0);
    assert(xobject.null() == false);
    assert(m_hasCDATASectionElements == m_stylesheetRoot->hasCDATASectionElements());

    doFlushPending();

    if(generateCDATASection() == true)
    {
        xobject->str(
            *m_executionContext,
            *getFormatterListenerImpl(),
            &FormatterListener::cdata);

        if(getTraceListeners() > 0)
        {
            fireCharacterGenerateEvent(xobject, true);
        }
    }
    else
    {
        xobject->str(
            *m_executionContext,
            *getFormatterListenerImpl(),
            &FormatterListener::characters);

        if(getTraceListeners() > 0)
        {
            fireCharacterGenerateEvent(xobject, false);
        }
    }
}



void 
XSLTEngineImpl::charactersRaw(
            const XalanDOMChar*     ch,
            size_type               start,
            size_type               length)
{
    assert(ch != 0);
    assert(length != 0);

    doFlushPending();

    getFormatterListenerImpl()->charactersRaw(ch, length);

    if(getTraceListeners() > 0)
    {
        fireCharacterGenerateEvent(ch, start, length, false);
    }
}



void
XSLTEngineImpl::charactersRaw(const XalanNode&  node)
{
    doFlushPending();

    DOMServices::getNodeData(
        node,
        *m_executionContext,
        *getFormatterListenerImpl(),
        &FormatterListener::charactersRaw);

    if(getTraceListeners() > 0)
    {
        fireCharacterGenerateEvent(node, false);
    }
}



void
XSLTEngineImpl::charactersRaw(const XObjectPtr&     xobject)
{
    doFlushPending();

    xobject->str(
        *m_executionContext,
        *getFormatterListenerImpl(),
        &FormatterListener::charactersRaw);

    if(getTraceListeners() > 0)
    {
        fireCharacterGenerateEvent(xobject, false);
    }
}



void
XSLTEngineImpl::resetDocument()
{
    assert(getFormatterListenerImpl() != 0);

    flushPending();
    
    getFormatterListenerImpl()->resetDocument();
}



void
XSLTEngineImpl::ignorableWhitespace(
            const XalanDOMChar*     ch,
            size_type               length)
{
    assert(getFormatterListenerImpl() != 0);
    assert(ch != 0);
    assert(length != 0);

    doFlushPending();

    getFormatterListenerImpl()->ignorableWhitespace(ch, length);

    if(getTraceListeners() > 0)
    {
        GenerateEvent ge(GenerateEvent::EVENTTYPE_IGNORABLEWHITESPACE,
                         getMemoryManager(), ch, 0, length);

        fireGenerateEvent(ge);
    }
}



void
XSLTEngineImpl::processingInstruction(
            const XalanDOMChar*     target,
            const XalanDOMChar*     data)
{
    assert(getFormatterListenerImpl() != 0);
    assert(target != 0);
    assert(data != 0);

    doFlushPending();

    getFormatterListenerImpl()->processingInstruction(target, data);

    if(getTraceListeners() > 0)
    {
        GenerateEvent ge(
                GenerateEvent::EVENTTYPE_PI,
                 getMemoryManager(),
                target,
                data);

        fireGenerateEvent(ge);
    }
}



void
XSLTEngineImpl::comment(const XalanDOMChar*     data)
{
    assert(getFormatterListenerImpl() != 0);
    assert(data != 0);

    doFlushPending();

    getFormatterListenerImpl()->comment(data);

    if(getTraceListeners() > 0)
    {
        GenerateEvent ge(GenerateEvent::EVENTTYPE_COMMENT,
                                           getMemoryManager(),
                                          data);
        fireGenerateEvent(ge);
    }
}



void
XSLTEngineImpl::entityReference(const XalanDOMChar*     name)
{
    assert(getFormatterListenerImpl() != 0);
    assert(name != 0);

    doFlushPending();

    getFormatterListenerImpl()->entityReference(name);

    if(getTraceListeners() > 0)
    {
        GenerateEvent ge(GenerateEvent::EVENTTYPE_ENTITYREF,
                                          getMemoryManager(),  name);

        fireGenerateEvent(ge);
    }
}



void
XSLTEngineImpl::cdata(
            const XalanDOMChar*     ch,
            size_type               start,
            size_type               length)
{
    assert(getFormatterListenerImpl() != 0);
    assert(ch != 0);
    assert(length != 0);

    setMustFlushPendingStartDocument(true);

    flushPending();

    getFormatterListenerImpl()->cdata(ch, length);

    if(getTraceListeners() > 0)
    {
        GenerateEvent ge(GenerateEvent::EVENTTYPE_CDATA, getMemoryManager(), ch, start,
                    length);

        fireGenerateEvent(ge);
    }
}



void
XSLTEngineImpl::checkDefaultNamespace(
            const XalanDOMString&   theElementName,
            const XalanDOMString&   theElementNamespaceURI)
{
    // Check for elements with no prefix...
    if (indexOf(theElementName, XalanUnicode::charColon) == theElementName.length())
    {
        // Get the current default namespace URI..
        const XalanDOMString* const     theResultNamespace =
            getResultNamespaceForPrefix(s_emptyString);

        // If there is one, and the URIs are different, add a new declaration.  This
        // will also "turn-off" the default namespace, if necessary.
        if (theResultNamespace != 0 && theElementNamespaceURI != *theResultNamespace)
        {
            addResultAttribute(DOMServices::s_XMLNamespace, theElementNamespaceURI, false, 0);
        }
    }
}



void
XSLTEngineImpl::warnCopyTextNodesOnly(
            const XalanNode*    sourceNode,
            const Locator*      locator)
{
    assert(m_executionContext != 0);

    const ECGetCachedString     theGuard(*m_executionContext);

    const XalanDOMString&   theMessage =
        XalanMessageLoader::getMessage(
            theGuard.get(),
            XalanMessages::OnlyTextNodesCanBeCopied);

    warn(
        theMessage,
        locator,
        sourceNode);
}



inline void
XSLTEngineImpl::cloneToResultTree(
            const XalanText&    node,
            bool                overrideStrip)
{
    assert(m_executionContext != 0 && m_stylesheetRoot != 0);
    assert(node.getParentNode() == 0 ||
           node.getParentNode()->getNodeType() != XalanNode::DOCUMENT_NODE);

    if (overrideStrip == true ||
        m_executionContext->shouldStripSourceNode(node) == false)
    {
        const XalanDOMString&   data = node.getData();
//        assert(data.empty() == false);

        characters(data.c_str(), 0, data.length());
    }
}



void
XSLTEngineImpl::cloneToResultTree(
            const XalanNode&    node,
            bool                cloneTextNodesOnly,
            const Locator*      locator)
{
    XalanNode::NodeType     posNodeType = node.getNodeType();

    if (posNodeType == XalanNode::DOCUMENT_FRAGMENT_NODE)
    {
        outputResultTreeFragment(
                static_cast<const XalanDocumentFragment&>(node),
            cloneTextNodesOnly,
            locator);
    }
    else if (cloneTextNodesOnly == true &&
        posNodeType != XalanNode::TEXT_NODE)
    {
        warnCopyTextNodesOnly(
            &node,
            locator);
    }
    else
    {
        const XalanNode*    pos = &node;

        while(pos != 0)
        {
            if(posNodeType != XalanNode::ATTRIBUTE_NODE)
            {
                flushPending();
            }

            cloneToResultTree(
                            *pos,
                            posNodeType,
                            false,
                            true,
                            false,
                            locator);

            const XalanNode*    nextNode = pos->getFirstChild();

            while(nextNode == 0)
            {
                if(XalanNode::ELEMENT_NODE == posNodeType)
                {
                    endElement(pos->getNodeName().c_str());
                }

                if(&node == pos)
                    break;

                nextNode = pos->getNextSibling();

                if(nextNode == 0)
                {
                    pos = pos->getParentNode();
                    assert(pos != 0);

                    posNodeType = pos->getNodeType();

                    if (&node == pos)
                    {
                        if(XalanNode::ELEMENT_NODE == posNodeType)
                        {
                            endElement(pos->getNodeName().c_str());
                        }

                        nextNode = 0;
                        break;
                    }
                }
            }

            pos = nextNode;

            if (pos != 0)
            {
                posNodeType = pos->getNodeType();
            }
        }
    }
}



inline void
createAndAddNamespaceResultAttribute(
            StylesheetExecutionContext&     theExecutionContext,
            const XalanDOMString&           thePrefix,
            const XalanDOMString&           theNamespaceURI,
            XalanDOMString&                 theTempString)
{
    theTempString = DOMServices::s_XMLNamespaceWithSeparator;
    theTempString.append(thePrefix);

    theExecutionContext.addResultAttribute(
        theTempString,
        theNamespaceURI);
}



inline void
createAndAddNamespaceResultAttribute(
            StylesheetExecutionContext&     theExecutionContext,
            const XalanDOMString&           thePrefix,
            const XalanDOMString&           theNamespaceURI)
{
    const XSLTEngineImpl::ECGetCachedString     theCachedString(theExecutionContext);

    createAndAddNamespaceResultAttribute(
        theExecutionContext,
        thePrefix,
        theNamespaceURI,
        theCachedString.get());
}



inline void
createFixedUpResultAttribute(
            StylesheetExecutionContext&     theExecutionContext,
            const XalanDOMString&           theLocalPart,
            const XalanDOMString&           theNamespaceURI,
            const XalanDOMString&           theValue)
{
    typedef StylesheetExecutionContext::GetCachedString     GetCachedString;

    const GetCachedString   theCachedString(theExecutionContext);

    XalanDOMString&     theBuffer = theCachedString.get();

    theExecutionContext.getUniqueNamespaceValue(theBuffer);

    createAndAddNamespaceResultAttribute(
        theExecutionContext,
        theBuffer,
        theNamespaceURI);

    theBuffer += XalanUnicode::charColon;

    theBuffer += theLocalPart;

    theExecutionContext.addResultAttribute(
        theBuffer,
        theValue);
}



void
XSLTEngineImpl::cloneToResultTree(
            const XalanNode&        node,
            XalanNode::NodeType     nodeType,
            bool                    overrideStrip,
            bool                    shouldCloneAttributes,
            bool                    cloneTextNodesOnly,
            const Locator*          locator)
{
    assert(nodeType == node.getNodeType());
    assert(m_executionContext != 0);

    if(cloneTextNodesOnly == true)
    {
        if (nodeType != XalanNode::TEXT_NODE)
        {
            warnCopyTextNodesOnly(
                    &node,
                    locator);
        }
        else
        {
            const XalanText&    tx =
                static_cast<const XalanText&>(node);

            cloneToResultTree(tx, overrideStrip);
        }
    }
    else
    {
        switch(nodeType)
        {
        case XalanNode::TEXT_NODE:
            {
                const XalanText&    tx =
                    static_cast<const XalanText&>(node);

                cloneToResultTree(tx, overrideStrip);
            }
            break;

        case XalanNode::ELEMENT_NODE:
            {
                const XalanDOMString&   theElementName =
                    node.getNodeName();

                startElement(theElementName.c_str());

                if(shouldCloneAttributes == true)
                {
                    copyAttributesToAttList(
                        node,
                        getPendingAttributesImpl());

                    copyNamespaceAttributes(node);
                }

                checkDefaultNamespace(theElementName, node.getNamespaceURI());
            }
            break;

        case XalanNode::CDATA_SECTION_NODE:
            {
                const XalanDOMString&   data = node.getNodeValue();

                cdata(data.c_str(), 0, data.length());
            }
            break;

        case XalanNode::ATTRIBUTE_NODE:
            if (isElementPending() == true)
            {
                addResultAttribute(
                        getPendingAttributesImpl(),
                        node.getNodeName(),
                        node.getNodeValue(),
                        true,
                        locator);
            }
            else
            {
                const ECGetCachedString     theGuard(*m_executionContext);

                const XalanDOMString&   theMessage =
                    XalanMessageLoader::getMessage(
                        theGuard.get(),
                        XalanMessages::AttributeCannotBeAdded);

                warn(
                    theMessage,
                    locator,
                    &node);
            }
            break;

        case XalanNode::COMMENT_NODE:
            comment(node.getNodeValue().c_str());
            break;

        case XalanNode::ENTITY_REFERENCE_NODE:
            entityReference(node.getNodeName().c_str());
            break;

        case XalanNode::PROCESSING_INSTRUCTION_NODE:
            processingInstruction(
                    node.getNodeName().c_str(),
                    node.getNodeValue().c_str());
            break;

        case XalanNode::DOCUMENT_FRAGMENT_NODE:
            assert(false);
            break;

        // Can't really do this, but we won't throw an error so that copy-of will
        // work
        case XalanNode::DOCUMENT_NODE:
        case XalanNode::DOCUMENT_TYPE_NODE:
        break;

        default:
            {
                const ECGetCachedString     theGuard(*m_executionContext);

                const XalanDOMString&   theMessage =
                    XalanMessageLoader::getMessage(
                        theGuard.get(),
                        XalanMessages::CantCreateItemInResultTree);

                warn(
                    theMessage,
                    locator,
                    &node);
            }
        break;
        }
    }
}



void
XSLTEngineImpl::outputToResultTree(
            const XObject&      value,
            bool                outputTextNodesOnly,
            const Locator*      locator)
{
    const XObject::eObjectType  type = value.getType();

    switch(type)
    {
    case XObject::eTypeBoolean:
    case XObject::eTypeNumber:
    case XObject::eTypeString:
        {
            const XalanDOMString&   s = value.str(*m_executionContext);

            characters(s.c_str(), 0, s.length());
        }
        break;

    case XObject::eTypeNodeSet:
        {
            const NodeRefListBase&  nl = value.nodeset();

            const NodeRefListBase::size_type    nChildren = nl.getLength();

            for(NodeRefListBase::size_type i = 0; i < nChildren; i++)
            {
                XalanNode*          pos = nl.item(i);
                assert(pos != 0);

                XalanNode::NodeType     posNodeType = pos->getNodeType();

                if (outputTextNodesOnly == true &&
                    posNodeType != XalanNode::TEXT_NODE)
                {
                    warnCopyTextNodesOnly(
                            pos,
                            locator);
                }
                else
                {
                    XalanNode* const    top = pos;

                    while(0 != pos)
                    {
                        flushPending();

                        XalanNode::NodeType     posNodeType = pos->getNodeType();

                        cloneToResultTree(*pos, posNodeType, false, false, false, locator);

                        XalanNode*  nextNode = pos->getFirstChild();

                        while(0 == nextNode)
                        {
                            if(XalanNode::ELEMENT_NODE == posNodeType)
                            {
                                endElement(pos->getNodeName().c_str());
                            }

                            if(top == pos)
                                break;

                            nextNode = pos->getNextSibling();

                            if(0 == nextNode)
                            {
                                pos = pos->getParentNode();
                                assert(pos != 0);

                                posNodeType = pos->getNodeType();

                                if(top == pos)
                                {
                                    if(XalanNode::ELEMENT_NODE == posNodeType)
                                    {
                                        endElement(pos->getNodeName().c_str());
                                    }

                                    nextNode = 0;
                                    break;
                                }
                            }
                        }

                        pos = nextNode;

                        if (pos != 0)
                        {
                            posNodeType = pos->getNodeType();
                        }
                    }
                }
            }
        }
        break;
        
    case XObject::eTypeResultTreeFrag:
        outputResultTreeFragment(value, outputTextNodesOnly, locator);
        break;

    case XObject::eTypeNull:
    case XObject::eTypeUnknown:
    case XObject::eUnknown:
    default:
        assert(false);
        break;
    }
}



void
XSLTEngineImpl::outputResultTreeFragment(
            const XalanDocumentFragment&    theTree,
            bool                            outputTextNodesOnly,
            const Locator*                  locator)
{
    for(XalanNode* child = theTree.getFirstChild(); child != 0; child = child->getNextSibling())
    {
        XalanNode*  pos = child;

        XalanNode::NodeType     posNodeType = pos->getNodeType();

        if (outputTextNodesOnly == true &&
            posNodeType != XalanNode::TEXT_NODE)
        {
            warnCopyTextNodesOnly(
                    pos,
                    locator);
        }
        else
        {
            XalanNode* const    top = pos;

            while(0 != pos)
            {
                flushPending();

                cloneToResultTree(*pos, posNodeType, true, true, false, locator);

                XalanNode*  nextNode = pos->getFirstChild();

                while(0 == nextNode)
                {
                    if(XalanNode::ELEMENT_NODE == posNodeType)
                    {
                        endElement(pos->getNodeName().c_str());
                    }

                    if(top == pos)
                        break;

                    nextNode = pos->getNextSibling();

                    if(0 == nextNode)
                    {
                        pos = pos->getParentNode();

                        if(0 == pos)
                        {
                            nextNode = 0;

                            break;
                        }
                        else
                        {
                            assert(0 != pos);

                            posNodeType = pos->getNodeType();

                            if(top == pos)
                            {
                                if(XalanNode::ELEMENT_NODE == posNodeType)
                                {
                                    endElement(pos->getNodeName().c_str());
                                }

                                nextNode = 0;

                                break;
                            }
                        }
                    }
                }

                pos = nextNode;

                if (pos != 0)
                {
                    posNodeType = pos->getNodeType();
                }
            }
        }
    }
}



bool
XSLTEngineImpl::isCDataResultElem(const XalanDOMString&     elementName)
{
    assert(m_executionContext != 0);
    assert(m_hasCDATASectionElements == true);

    bool    fResult = false;

    // We only want to worry about cdata-section-elements when we're serializing the
    // result tree, which is only when the output context is 1.
    if (m_outputContextStack.size() == 1)
    {
        const XalanDOMString::size_type     indexOfNSSep = indexOf(elementName, XalanUnicode::charColon);

        if(indexOfNSSep == elementName.length())
        {
            const XalanDOMString* const     elemNS =
                        getResultNamespaceForPrefix(s_emptyString);

            if (elemNS != 0)
            {
                fResult = m_stylesheetRoot->isCDATASectionElementName(XalanQNameByReference(*elemNS, elementName));
            }
            else
            {
                fResult = m_stylesheetRoot->isCDATASectionElementName(XalanQNameByReference(s_emptyString, elementName));
            }
        }
        else
        {
            const ECGetCachedString     elemLocalNameGuard(*m_executionContext);
            const ECGetCachedString     prefixGuard(*m_executionContext);

            XalanDOMString&     elemLocalName = elemLocalNameGuard.get();
            XalanDOMString&     prefix = prefixGuard.get();

            substring(elementName, prefix, 0, indexOfNSSep);
            substring(elementName, elemLocalName, indexOfNSSep + 1);

            if(equals(prefix, DOMServices::s_XMLString))
            {
                fResult =
                    m_stylesheetRoot->isCDATASectionElementName(
                        XalanQNameByReference(
                            DOMServices::s_XMLNamespaceURI,
                            elemLocalName));
            }
            else
            {
                const XalanDOMString* const     elemNS =
                    getResultNamespaceForPrefix(prefix);

                if(elemNS == 0)
                {
                    ECGetCachedString     theGuard(*m_executionContext);

                    error(
                        XalanMessageLoader::getMessage(
                            theGuard.get(),
                            XalanMessages::PrefixIsNotDeclared_1Param,
                            prefix),
                        0,
                        m_executionContext->getCurrentNode());
                }
                else
                {
                    fResult =
                        m_stylesheetRoot->isCDATASectionElementName(
                            XalanQNameByReference(
                                *elemNS,
                                elemLocalName));
                }
            }
        }
    }

    return fResult;
}
    


const XalanDOMString*
XSLTEngineImpl::getResultNamespaceForPrefix(const XalanDOMString&   prefix) const
{
    return m_resultNamespacesStack.getNamespaceForPrefix(prefix);
}
  


const XalanDOMString*
XSLTEngineImpl::getResultPrefixForNamespace(const XalanDOMString&   theNamespace) const
{
    return m_resultNamespacesStack.getPrefixForNamespace(theNamespace);
}



inline bool
isPrefixUsed(
            const XalanDOMString&       thePrefix,
            XalanDOMString::size_type   thePrefixLength,
            const XalanDOMChar*         theName,
            XalanDOMString::size_type   theNameLength)
{
    assert(thePrefixLength != 0);

    // The name must be greater than the length of the prefix + 1, since
    // there must be a ':' to separate the prefix from the local part...
    if (theNameLength <= thePrefixLength + 1)
    {
        return false;
    }
    else
    {
        assert(theName != 0);

        const XalanDOMString::size_type     theIndex = indexOf(
            theName,
            XalanUnicode::charColon);

        // OK, if the index of the ':' is the same as the length of the prefix,
        // and theElementName starts with thePrefix, then the prefix is in use.
        if (theIndex == thePrefixLength &&
            startsWith(theName, thePrefix) == true)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}



inline bool
isPrefixUsed(
            const XalanDOMString&       thePrefix,
            XalanDOMString::size_type   thePrefixLength,
            const XalanDOMString&       theName)
{
    return isPrefixUsed(thePrefix, thePrefixLength, theName.c_str(), theName.length());
}



inline bool
isPrefixUsedOrDeclared(
            const XalanDOMString&       thePrefix,
            XalanDOMString::size_type   thePrefixLength,
            const XalanDOMChar*         theName,
            XalanDOMString::size_type   theNameLength)
{
    if (isPrefixUsed(thePrefix, thePrefixLength, theName, theNameLength) == true)
    {
        return true;
    }
    else
    {
        const XalanDOMString::size_type     theDeclarationLength =
            thePrefixLength + DOMServices::s_XMLNamespaceWithSeparatorLength;

        // If this is a namespace declaration for this prefix, then all of
        // these conditions must be true...
        if (theDeclarationLength == theNameLength &&
            startsWith(theName, DOMServices::s_XMLNamespaceWithSeparator) == true &&
            endsWith(theName, thePrefix.c_str()) == true)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}



inline bool
isPendingAttributePrefix(
            const AttributeListType&    thePendingAttributes,
            const XalanDOMString&       thePrefix,
            XalanDOMString::size_type   thePrefixLength)
{
    const XalanSize_t   thePendingAttributesCount =
                thePendingAttributes.getLength();

    if (thePendingAttributesCount == 0)
    {
        // No attributes, no problem...
        return false;
    }
    else
    {
        bool    fResult = false;

        // Check each attribute...
        for (XalanSize_t i = 0; i < thePendingAttributesCount; ++i)
        {
            const XalanDOMChar* const   thePendingAttributeName =
                            thePendingAttributes.getName(i);
            assert(thePendingAttributeName != 0);

            if (isPrefixUsedOrDeclared(
                    thePrefix,
                    thePrefixLength,
                    thePendingAttributeName,
                    length(thePendingAttributeName)) == true)
            {
                fResult = true;

                break;
            }
        }

        return fResult;
    }
}



bool
XSLTEngineImpl::isPendingResultPrefix(const XalanDOMString&     thePrefix) const
{
    const XalanDOMString::size_type     thePrefixLength = thePrefix.length();
    assert(thePrefixLength > 0);

    // The element name must be greater than the length of the prefix + 1, since
    // there must be a ':' to separate the prefix from the local part...
    if (isPrefixUsed(thePrefix, thePrefixLength, getPendingElementName()) == true)
    {
        return true;
    }
    else
    {
        // The element is not using the prefix, so check the
        // pending attributes...
        return isPendingAttributePrefix(
                        getPendingAttributes(),
                        thePrefix,
                        thePrefixLength);
    }
}



void
XSLTEngineImpl::addResultNamespace(
            const XalanDOMString&   thePrefix,
            const XalanDOMString&   theName,
            const XalanNode&        theNode,
            AttributeListImpl&      thePendingAttributes,
            bool                    fOnlyIfPrefixNotPresent)
{
    if (fOnlyIfPrefixNotPresent == false ||
        m_resultNamespacesStack.prefixIsPresentLocal(thePrefix) == false)
    {
        const XalanDOMString* const     desturi = getResultNamespaceForPrefix(thePrefix);
        const XalanDOMString&           srcURI = theNode.getNodeValue();

        if(desturi == 0 || equals(srcURI, *desturi) == false)
        {
            addResultAttribute(thePendingAttributes, theName, srcURI, false, 0);
            addResultNamespaceDecl(thePrefix, srcURI);
        }
    }
}



void
XSLTEngineImpl::addResultNamespace(
            const XalanNode&    theNode,
            AttributeListImpl&  thePendingAttributes,
            bool                fOnlyIfPrefixNotPresent)
{
    assert(m_executionContext != 0);
    assert(theNode.getNodeType() == XalanNode::ATTRIBUTE_NODE);

    const XalanDOMString&   aname = theNode.getNodeName();

    if (equals(aname, DOMServices::s_XMLNamespace) == true)
    {
        // Default namespace declaration...
        addResultNamespace(s_emptyString, aname, theNode, thePendingAttributes, fOnlyIfPrefixNotPresent);
    }
    else if (startsWith(aname, DOMServices::s_XMLNamespaceWithSeparator))
    {
        const ECGetCachedString     prefixGuard(*m_executionContext);

        XalanDOMString&     thePrefix = prefixGuard.get();

        substring(aname, thePrefix, DOMServices::s_XMLNamespaceWithSeparatorLength);

        addResultNamespace(thePrefix, aname, theNode, thePendingAttributes, fOnlyIfPrefixNotPresent);
    }
}



void
XSLTEngineImpl::copyNamespaceAttributes(const XalanNode&    src) 
{
    assert(m_attributeNamesVisited.empty() == true);

    const XalanNode*    parent = &src;

    while (parent != 0 &&
           parent->getNodeType() == XalanNode::ELEMENT_NODE) 
    {
        const XalanNamedNodeMap* const  nnm =
                parent->getAttributes();
        assert(nnm != 0);

        const XalanSize_t   nAttrs = nnm->getLength();

        assert(m_outputContextStack.empty() == false);

        AttributeListImpl&  thePendingAttributes =
                getPendingAttributesImpl();

        for (XalanSize_t i = 0;  i < nAttrs; i++) 
        {
            const XalanNode* const  attr = nnm->item(i);
            assert(attr != 0);

            const XalanDOMString&   nodeName = attr->getNodeName();

            XALAN_USING_STD(find_if)

            if (find_if(
                    m_attributeNamesVisited.begin(),
                    m_attributeNamesVisited.end(),
                    FindStringPointerFunctor(nodeName)) == m_attributeNamesVisited.end())
            {
                addResultNamespace(*attr, thePendingAttributes, true);

                m_attributeNamesVisited.push_back(&nodeName);
            }
        }

        parent = parent->getParentNode();
    }

    m_attributeNamesVisited.clear();
}



const XObjectPtr
XSLTEngineImpl::evalXPathStr(
            const XalanDOMString&   str,
            XPathExecutionContext&  executionContext)
{
    assert(executionContext.getPrefixResolver() != 0);

    XPath* const        theXPath = m_xpathFactory.create();

    const XPathGuard    theGuard(m_xpathFactory, theXPath);

    m_xpathProcessor->initXPath(
            *theXPath,
            m_xpathConstructionContext,
            str,
            *executionContext.getPrefixResolver(),
            getLocatorFromStack());

    return theXPath->execute(
            executionContext.getCurrentNode(),
            *executionContext.getPrefixResolver(),
            executionContext);
}



const XObjectPtr
XSLTEngineImpl::evalXPathStr(
            const XalanDOMString&   str,
            XalanNode*              contextNode,
            const PrefixResolver&   prefixResolver,
            XPathExecutionContext&  executionContext)
{
    XPath* const        theXPath = m_xpathFactory.create();

    const XPathGuard    theGuard(m_xpathFactory, theXPath);

    m_xpathProcessor->initXPath(
            *theXPath,
            m_xpathConstructionContext,
            str,
            prefixResolver,
            getLocatorFromStack());

    return theXPath->execute(contextNode, prefixResolver, executionContext);
}



const XObjectPtr
XSLTEngineImpl::evalXPathStr(
            const XalanDOMString&   str,
            XalanNode*              contextNode,
            const XalanElement&     prefixResolver,
            XPathExecutionContext&  executionContext)
{
    ElementPrefixResolverProxy  theProxy(&prefixResolver,
                                         m_xpathEnvSupport,
                                         m_domSupport, 
                                         executionContext.getMemoryManager());

    return evalXPathStr(str, contextNode, theProxy, executionContext);
}




/**
 * Create and initialize an xpath and return it.
 */
const XPath*
XSLTEngineImpl::createMatchPattern(
            const XalanDOMString&   str,
            const PrefixResolver&   resolver)
{
    XPath* const    xpath = m_xpathFactory.create();

    m_xpathProcessor->initMatchPattern(
            *xpath,
            m_xpathConstructionContext,
            str,
            resolver,
            getLocatorFromStack());

    return xpath;
}



void
XSLTEngineImpl::returnXPath(const XPath*    xpath)
{
    m_xpathFactory.returnObject(xpath);
}



inline void
XSLTEngineImpl::copyAttributeToTarget(
            const XalanDOMString&   attrName,
            const XalanDOMString&   attrValue,
            AttributeListImpl&      attrList)
{
    addResultAttribute(attrList, attrName, attrValue, false, 0);
}



void
XSLTEngineImpl::copyAttributesToAttList(
            const XalanNode&    node,
            AttributeListImpl&  attList)
{
    assert(m_stylesheetRoot != 0);

    const XalanNamedNodeMap* const  attributes =
        node.getAttributes();

    if (attributes != 0)
    {
        const XalanSize_t   nAttributes = attributes->getLength();

        for (XalanSize_t i = 0; i < nAttributes; ++i)  
        {   
            const XalanNode* const  attr = attributes->item(i);
            assert(attr != 0);

            copyAttributeToTarget(
                attr->getNodeName(),
                attr->getNodeValue(),
                attList);
        }
    }
}



XMLParserLiaison&
XSLTEngineImpl::getXMLParserLiaison() const
{
    return m_parserLiaison;
}



void
XSLTEngineImpl::getUniqueNamespaceValue(XalanDOMString&     theValue)
{
    do
    {
        m_scratchString.assign(s_uniqueNamespacePrefix);

        NumberToDOMString(m_uniqueNSValue++, m_scratchString);
    } while(getResultNamespaceForPrefix(m_scratchString) != 0);

    theValue.append(m_scratchString);
}



void
XSLTEngineImpl::setStylesheetParam(
            const XalanDOMString&   theName,
            const XalanDOMString&   theExpression)
{
    m_stylesheetParams[theName].m_expression = theExpression;;
}



void
XSLTEngineImpl::setStylesheetParam(
            const XalanDOMString&   theName,
            XObjectPtr              theValue)
{
    m_stylesheetParams[theName].m_value = theValue;
}



void
XSLTEngineImpl::clearStylesheetParams()
{
    m_stylesheetParams.clear();
}



void
XSLTEngineImpl::resolveTopLevelParams(StylesheetExecutionContext&   executionContext)
{
    typedef ParamMapType::const_iterator    const_iterator;

    MemoryManager&  theManager = getMemoryManager();

    assert(m_topLevelParams.size() == 0);

    m_topLevelParams.reserve(m_stylesheetParams.size());

    for (const_iterator i = m_stylesheetParams.begin();
            i != m_stylesheetParams.end();
                ++i)
    {
        const XalanDOMString&   theName = (*i).first;
        const XalanParamHolder& theCurrent = (*i).second;

        const XalanQNameByValue     theQName(theName, theManager, m_stylesheetRoot);

        const XalanDOMString&   theExpression = theCurrent.m_expression;

        if (theExpression.length() > 0)
        {
            const TopLevelArg   theArg(
                                    theManager,
                                    theQName,
                                    theExpression);

            m_topLevelParams.push_back(theArg);
        }
        else
        {
            const TopLevelArg   theArg(
                                    theManager,
                                    theQName,
                                    theCurrent.m_value);

            m_topLevelParams.push_back(theArg);
        }
    }

    executionContext.pushTopLevelVariables(m_topLevelParams);
}



FormatterListener*
XSLTEngineImpl::getFormatterListener() const
{
    return getFormatterListenerImpl();
}



void
XSLTEngineImpl::setFormatterListener(FormatterListener*     flistener)
{
    if (getHasPendingStartDocument() == true &&
        getFormatterListenerImpl() != 0)
    {
        setMustFlushPendingStartDocument(true);

        flushPending();
    }

    setFormatterListenerImpl(flistener);
}



void
XSLTEngineImpl::fireCharacterGenerateEvent(
            const XalanNode&    theNode,
            bool                isCDATA)
{
    XalanDOMString  theBuffer(getMemoryManager());
    
    DOMServices::getNodeData(theNode, *m_executionContext, theBuffer);

    fireCharacterGenerateEvent(theBuffer, isCDATA);
}



void
XSLTEngineImpl::fireCharacterGenerateEvent(
            const XObjectPtr&   theXObject,
            bool                isCDATA)
{
    fireCharacterGenerateEvent(
        theXObject->str(*m_executionContext),
        isCDATA);
}



void
XSLTEngineImpl::fireCharacterGenerateEvent(
            const XalanDOMString&   theString,
            bool                    isCDATA)
{
    fireCharacterGenerateEvent(
        theString.c_str(),
        0,
        theString.length(),
        isCDATA);
}



void
XSLTEngineImpl::fireCharacterGenerateEvent(
            const XalanDOMChar*     ch,
            size_type               start,
            size_type               length,
            bool                    isCDATA)
{
    const GenerateEvent     ge(
        isCDATA == true ? GenerateEvent::EVENTTYPE_CDATA : GenerateEvent::EVENTTYPE_CHARACTERS,
         getMemoryManager(),
        ch,
        start,
        length);

    fireGenerateEvent(ge);
}


void
XSLTEngineImpl::error(
            const XalanDOMString&   theMessage,
            const Locator*          theLocator,
            const XalanNode*        theSourceNode)
{
    problem(
        eXSLTProcessor,
        eError,
        theMessage,
        theLocator,
        theSourceNode);
}



void
XSLTEngineImpl::warn(
            const XalanDOMString&   theMessage,
            const Locator*          theLocator,
            const XalanNode*        theSourceNode)
{
    problem(
        eXSLTProcessor,
        eWarning,
        theMessage,
        theLocator,
        theSourceNode);
}



void
XSLTEngineImpl::message(
            const XalanDOMString&   theMessage,
            const Locator*          theLocator,
            const XalanNode*        theSourceNode)
{
    problem(
        eXSLTProcessor,
        eMessage,
        theMessage,
        theLocator,
        theSourceNode);
}



void
XSLTEngineImpl::installFunctions(MemoryManager& theManager)
{
    XPath::installFunction(XPathFunctionTable::s_current, FunctionCurrent());
    XPath::installFunction(XPathFunctionTable::s_document, FunctionDocument());
    XPath::installFunction(XPathFunctionTable::s_elementAvailable, FunctionElementAvailable());
    XPath::installFunction(XPathFunctionTable::s_functionAvailable, FunctionFunctionAvailable());
    XPath::installFunction(XPathFunctionTable::s_formatNumber, FunctionFormatNumber());
    XPath::installFunction(XPathFunctionTable::s_generateId, FunctionGenerateID());
    XPath::installFunction(XPathFunctionTable::s_key, FunctionKey());

    const FunctionSystemProperty    temp(theManager);

    XPath::installFunction(XPathFunctionTable::s_systemProperty, temp);
    XPath::installFunction(XPathFunctionTable::s_unparsedEntityUri, FunctionUnparsedEntityURI());
}



void
XSLTEngineImpl::uninstallFunctions()
{
    XPath::uninstallFunction(XPathFunctionTable::s_current);
    XPath::uninstallFunction(XPathFunctionTable::s_document);
    XPath::uninstallFunction(XPathFunctionTable::s_elementAvailable);
    XPath::uninstallFunction(XPathFunctionTable::s_functionAvailable);
    XPath::uninstallFunction(XPathFunctionTable::s_formatNumber);
    XPath::uninstallFunction(XPathFunctionTable::s_generateId);
    XPath::uninstallFunction(XPathFunctionTable::s_key);
    XPath::uninstallFunction(XPathFunctionTable::s_systemProperty);
    XPath::uninstallFunction(XPathFunctionTable::s_unparsedEntityUri);
}



XALAN_CPP_NAMESPACE_END


XALAN_USING_XALAN(XalanDOMString)

XALAN_USING_XALAN(XalanMemMgrs)

static XalanDOMString   s_XSLNameSpaceURL(XalanMemMgrs::getDummyMemMgr());

static XalanDOMString   s_XalanNamespaceURL(XalanMemMgrs::getDummyMemMgr());

static XalanDOMString   s_uniqueNamespacePrefix(XalanMemMgrs::getDummyMemMgr());

static XalanDOMString   s_stylesheetNodeName(XalanMemMgrs::getDummyMemMgr());

static XalanDOMString   s_typeString(XalanMemMgrs::getDummyMemMgr());

static XalanDOMString   s_hrefString(XalanMemMgrs::getDummyMemMgr());

static XalanDOMString   s_piTokenizerString(XalanMemMgrs::getDummyMemMgr());

static XalanDOMString   s_typeValueString1(XalanMemMgrs::getDummyMemMgr());

static XalanDOMString   s_typeValueString2(XalanMemMgrs::getDummyMemMgr());

static XalanDOMString   s_typeValueString3(XalanMemMgrs::getDummyMemMgr());

static XalanDOMString   s_typeValueString4(XalanMemMgrs::getDummyMemMgr());



XALAN_CPP_NAMESPACE_BEGIN



const XalanDOMString&   XSLTEngineImpl::s_XSLNameSpaceURL = ::s_XSLNameSpaceURL;

const XalanDOMString&   XSLTEngineImpl::s_XalanNamespaceURL = ::s_XalanNamespaceURL;

const XalanDOMString&   XSLTEngineImpl::s_uniqueNamespacePrefix = ::s_uniqueNamespacePrefix;

const XalanDOMString&   XSLTEngineImpl::s_stylesheetNodeName = ::s_stylesheetNodeName;

const XalanDOMString&   XSLTEngineImpl::s_typeString = ::s_typeString;

const XalanDOMString&   XSLTEngineImpl::s_hrefString = ::s_hrefString;

const XalanDOMString&   XSLTEngineImpl::s_piTokenizerString = ::s_piTokenizerString;

const XalanDOMString&   XSLTEngineImpl::s_typeValueString1 = ::s_typeValueString1;

const XalanDOMString&   XSLTEngineImpl::s_typeValueString2 = ::s_typeValueString2;

const XalanDOMString&   XSLTEngineImpl::s_typeValueString3 = ::s_typeValueString3;

const XalanDOMString&   XSLTEngineImpl::s_typeValueString4 = ::s_typeValueString4;


#if 0
#include <fstream>

void
dumpTable(
            const XSLTEngineImpl::ElementKeysMapType&   theTable,
            std::ostream&                               theSourceStream,
            std::ostream&                               theHeaderStream)
{
    XSLTEngineImpl::ElementKeysMapType::const_iterator  i = theTable.begin();

    while(i != theTable.end())
    {
        theSourceStream << "const XalanDOMChar\tXSLTEngineImpl::s_";

        const XalanDOMString&   theString = (*i).first;

        theHeaderStream << "\t// The string \"" << theString << "\"\n\tstatic const XalanDOMChar\ts_";

        bool    nextCap = false;

        XalanDOMString::const_iterator  j = theString.begin();

        while(*j)
        {
            if (*j == '-')
            {
                nextCap = true;
            }
            else
            {
                assert(*j >= 'a' && *j <= 'z');

                if (nextCap)
                {
                    theSourceStream << char(*j -'a' + 'A');
                    theHeaderStream << char(*j -'a' + 'A');

                    nextCap = false;
                }
                else
                {
                    theSourceStream << char(*j);
                    theHeaderStream << char(*j);
                }
            }

            ++j;
        }

        j = theString.begin();

        theSourceStream << "[] =\n{\n";
        theHeaderStream << "[];\n\n";

        while(*j)
        {
            if (*j == '-')
            {
                theSourceStream << "\tXalanUnicode::charHyphenMinus,\n";
            }
            else
            {
                assert(*j >= 'a' && *j <= 'z');

                theSourceStream << "\tXalanUnicode::charLetter_";

                theSourceStream << char(*j) << ",\n";
            }

            ++j;
        }

        theSourceStream << "\t0\n};\n\n";

        ++i;
    }
}
#endif


void
XSLTEngineImpl::initialize(MemoryManager&      theManager)
{
    ::s_XSLNameSpaceURL.reset( theManager, "http://www.w3.org/1999/XSL/Transform");

    ::s_XalanNamespaceURL.reset( theManager, "http://xml.apache.org/xalan");

    ::s_uniqueNamespacePrefix.reset( theManager, "ns");

    ::s_stylesheetNodeName.reset( theManager, "xml-stylesheet");

    ::s_typeString.reset( theManager, "type");

    XalanDOMString tmpString1(Constants::ATTRNAME_HREF, theManager);

    ::s_hrefString.swap(tmpString1);

    ::s_piTokenizerString.reset( theManager, " \t=");

    ::s_typeValueString1.reset( theManager, "text/xml");

    ::s_typeValueString2.reset( theManager, "text/xsl");

    ::s_typeValueString3.reset( theManager, "application/xml");

    ::s_typeValueString4.reset( theManager, "application/xml+xslt");

    installFunctions(theManager);
}



void
XSLTEngineImpl::terminate()
{
    uninstallFunctions();

    MemoryManager& theManager = XalanMemMgrs::getDummyMemMgr();

    releaseMemory(::s_uniqueNamespacePrefix, theManager);

    releaseMemory(::s_XalanNamespaceURL, theManager);

    releaseMemory(::s_XSLNameSpaceURL, theManager);

    releaseMemory(::s_stylesheetNodeName, theManager);

    releaseMemory(::s_typeString, theManager);

    releaseMemory(::s_hrefString, theManager);

    releaseMemory(::s_piTokenizerString, theManager);

    releaseMemory(::s_typeValueString1, theManager);

    releaseMemory(::s_typeValueString2, theManager);

    releaseMemory(::s_typeValueString3, theManager);

    releaseMemory(::s_typeValueString4, theManager);
}



XALAN_CPP_NAMESPACE_END
