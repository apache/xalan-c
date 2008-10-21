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
#include "XSLTProcessorEnvSupportDefault.hpp"



#include <algorithm>



#include <xercesc/sax/EntityResolver.hpp>
#include "xercesc/util/XMLEntityResolver.hpp"
#include "xercesc/util/XMLResourceIdentifier.hpp"
#include <xercesc/util/XMLURL.hpp>



#include <xalanc/Include/STLHelper.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/URISupport.hpp>
#include "xalanc/PlatformSupport/XSLException.hpp"



#include <xalanc/XPath/ElementPrefixResolverProxy.hpp>
#include <xalanc/XPath/XPathExecutionContext.hpp>



#include <xalanc/XMLSupport/XMLParserLiaison.hpp>



#include "KeyTable.hpp"
#include "StylesheetRoot.hpp"
#include "XSLTProcessor.hpp"
#include "XSLTInputSource.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XSLTProcessorEnvSupportDefault::XSLTProcessorEnvSupportDefault(
            MemoryManager&  theManager,
            XSLTProcessor*  theProcessor) :
    XSLTProcessorEnvSupport(),
    m_defaultSupport(theManager),
    m_processor(theProcessor)
{
}



XSLTProcessorEnvSupportDefault::~XSLTProcessorEnvSupportDefault()
{
    reset();
}



void
XSLTProcessorEnvSupportDefault::installExternalFunctionGlobal(
            const XalanDOMString&   theNamespace,
            const XalanDOMString&   functionName,
            const Function&         function)
{
    XPathEnvSupportDefault::installExternalFunctionGlobal(theNamespace, functionName, function);
}



void
XSLTProcessorEnvSupportDefault::uninstallExternalFunctionGlobal(
            const XalanDOMString&   theNamespace,
            const XalanDOMString&   functionName)
{
    XPathEnvSupportDefault::uninstallExternalFunctionGlobal(theNamespace, functionName);
}



void
XSLTProcessorEnvSupportDefault::installExternalFunctionLocal(
            const XalanDOMString&   theNamespace,
            const XalanDOMString&   functionName,
            const Function&         function)
{
    m_defaultSupport.installExternalFunctionLocal(theNamespace, functionName, function);
}



void
XSLTProcessorEnvSupportDefault::uninstallExternalFunctionLocal(
            const XalanDOMString&   theNamespace,
            const XalanDOMString&   functionName)
{
    m_defaultSupport.uninstallExternalFunctionLocal(theNamespace, functionName);
}



void
XSLTProcessorEnvSupportDefault::reset()
{
    m_defaultSupport.reset();
}



XalanDocument*
XSLTProcessorEnvSupportDefault::parseXML(
            MemoryManager&          theManager,
            const XalanDOMString&   urlString,
            const XalanDOMString&   base,
            ErrorHandler*           theErrorHandler)
{
    if (m_processor == 0)
    {
        return m_defaultSupport.parseXML(
                theManager,
                urlString,
                base,
                theErrorHandler);
    }
    else
    {
        typedef URISupport::URLAutoPtrType  URLAutoPtrType;

        // $$$ ToDo: we should re-work this code to only use
        // XMLRUL when necessary.
        const URLAutoPtrType    xslURL =
            URISupport::getURLFromString(urlString, base, theManager);

        const XalanDOMString    urlText(xslURL->getURLText(), theManager);

        // First see if it's already been parsed...
        XalanDocument*      theDocument =
            getSourceDocument(urlText);

        if (theDocument == 0)
        {
            XMLParserLiaison&   parserLiaison =
                m_processor->getXMLParserLiaison();

            EntityResolver* const   theResolver = 
                parserLiaison.getEntityResolver();

            XMLEntityResolver* const    theXMLResolver = 
                parserLiaison.getXMLEntityResolver();

            XalanAutoPtr<InputSource>   resolverInputSource;

            const XalanDOMString    theEmptyString(theManager);

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

            typedef XMLParserLiaison::EnsureResetErrorHandler   EnsureResetErrorHandler;

            EnsureResetErrorHandler     theGuard;

            if (theErrorHandler != 0)
            {
                theGuard.set(
                    &parserLiaison,
                    theErrorHandler);
            }

            if (resolverInputSource.get() != 0)
            {
                theDocument = parserLiaison.parseXMLStream(
                                *resolverInputSource.get(),
                                theEmptyString);
            }
            else
            {
                const XSLTInputSource   inputSource(urlText.c_str(), theManager);

                theDocument = parserLiaison.parseXMLStream(
                                inputSource,
                                theEmptyString);
            }

            if (theDocument != 0)
            {
                setSourceDocument(urlText, theDocument);
            }
        }

        return theDocument;
    }
}



XalanDocument*
XSLTProcessorEnvSupportDefault::getSourceDocument(const XalanDOMString&     theURI) const
{
    return m_defaultSupport.getSourceDocument(theURI);
}



void
XSLTProcessorEnvSupportDefault::setSourceDocument(
            const XalanDOMString&   theURI,
            XalanDocument*          theDocument)
{
    m_defaultSupport.setSourceDocument(theURI, theDocument);
}



const XalanDOMString&
XSLTProcessorEnvSupportDefault::findURIFromDoc(const XalanDocument*     owner) const
{
    return m_defaultSupport.findURIFromDoc(owner);
}



bool
XSLTProcessorEnvSupportDefault::elementAvailable(
            const XalanDOMString&   theNamespace,
            const XalanDOMString&   functionName) const
{
    return m_defaultSupport.elementAvailable(theNamespace,
                                             functionName);
}



bool
XSLTProcessorEnvSupportDefault::functionAvailable(
            const XalanDOMString&   theNamespace,
            const XalanDOMString&   functionName) const
{
    return m_defaultSupport.functionAvailable(theNamespace,
                                              functionName);
}



XObjectPtr
XSLTProcessorEnvSupportDefault::extFunction(
            XPathExecutionContext&          executionContext,
            const XalanDOMString&           theNamespace,
            const XalanDOMString&           functionName,
            XalanNode*                      context,
            const XObjectArgVectorType&     argVec,
            const Locator*                  locator) const
{
    return m_defaultSupport.extFunction(
            executionContext,
            theNamespace,
            functionName,
            context,
            argVec,
            locator);
}



void
XSLTProcessorEnvSupportDefault::problem(
            eSource                 source,
            eClassification         classification,
            const XalanDOMString&   msg,
            const Locator*          locator,
            const XalanNode*        sourceNode)
{
    m_processor->problem(
        source,
        classification,
        msg,
        locator,
        sourceNode);
}



void
XSLTProcessorEnvSupportDefault::problem(
            eSource                 source,
            eClassification         classification,
            const XalanDOMString&   msg,
            const XalanNode*        sourceNode)
{
    m_processor->problem(
        source,
        classification,
        msg,
        sourceNode);
}



XALAN_CPP_NAMESPACE_END
