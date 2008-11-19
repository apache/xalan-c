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
#include "XercesParserLiaison.hpp"



#if defined(XALAN_CLASSIC_IOSTREAMS)
#include <iostream.h>
#else
#include <iostream>
#endif



#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/framework/URLInputSource.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/sax/SAXParseException.hpp>



#include <xalanc/Include/XalanAutoPtr.hpp>
#include <xalanc/Include/XalanMemMgrAutoPtr.hpp>
#include <xalanc/Include/STLHelper.hpp>



#include <xalanc/PlatformSupport/ExecutionContext.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>
#include <xalanc/PlatformSupport/XalanUnicode.hpp>
#include <xalanc/PlatformSupport/XSLException.hpp>



#include <xalanc/DOMSupport/DOMSupport.hpp>



#if defined(XALAN_BUILD_DEPRECATED_DOM_BRIDGE)
#include <xalanc/XercesParserLiaison/Deprecated/XercesDocumentBridge.hpp>
#endif
#include <xalanc/XercesParserLiaison/XercesDocumentWrapper.hpp>
#include <xalanc/XercesParserLiaison/XercesDOMSupport.hpp>



XALAN_CPP_NAMESPACE_BEGIN


XercesParserLiaison::XercesParserLiaison(
            XercesDOMSupport&   /* theSupport */,
            MemoryManager&      theManager) :
    m_indent(-1),
    m_useValidation(false),
    m_includeIgnorableWhitespace(true),
    m_doNamespaces(true),
    m_exitOnFirstFatalError(true),
    m_entityResolver(0),
    m_xmlEntityResolver(0),
    m_errorHandler(this),
    m_externalSchemaLocation(theManager),
    m_externalNoNamespaceSchemaLocation(theManager),
    m_documentMap(theManager),
    m_buildWrapper(true),
    m_buildBridge(true),
    m_threadSafe(false),
    m_buildMaps(false),
    m_executionContext(0),
    m_domParser(0)
{
}



XercesParserLiaison::XercesParserLiaison(MemoryManager&     theManager) :
    m_indent(-1),
    m_useValidation(false),
    m_includeIgnorableWhitespace(true),
    m_doNamespaces(true),
    m_exitOnFirstFatalError(true),
    m_entityResolver(0),
    m_xmlEntityResolver(0),
    m_errorHandler(this),
    m_externalSchemaLocation(theManager),
    m_externalNoNamespaceSchemaLocation(theManager),
    m_documentMap(theManager),
    m_buildWrapper(true),
    m_buildBridge(true),
    m_threadSafe(false),
    m_buildMaps(false),
    m_executionContext(0),
    m_domParser(0)
{
}



XercesParserLiaison::~XercesParserLiaison()
{
    reset();

    delete m_domParser;
}



void
XercesParserLiaison::reset()
{
    // Delete any live documents...
    for(DocumentMapType::iterator i = m_documentMap.begin();
        i != m_documentMap.end();
        ++i)
    {
        if ((*i).second.isDeprecated() == false &&
            (*i).second.isOwned() == true)
        {
            delete (*i).second.m_wrapper->getXercesDocument();
        }

        XalanDocument* docToDelete = const_cast<XalanDocument*>((*i).first);

        if(docToDelete != 0)
        {
            docToDelete->~XalanDocument();

            getMemoryManager().deallocate((void*)docToDelete);
        }

    }

    m_documentMap.clear();

    m_executionContext = 0;
}



ExecutionContext*
XercesParserLiaison::getExecutionContext() const
{
    return m_executionContext;
}



void
XercesParserLiaison::setExecutionContext(ExecutionContext&  theContext)
{
    m_executionContext = &theContext;
}



void
XercesParserLiaison::ensureDOMParser()
{
    if (m_domParser == 0)
    {
        m_domParser = createDOMParser();
    }

    m_domParser->setValidationScheme(m_useValidation == true ? DOMParserType::Val_Auto : DOMParserType::Val_Never);

    m_domParser->setIncludeIgnorableWhitespace(m_includeIgnorableWhitespace);

    m_domParser->setDoNamespaces(m_doNamespaces);

    m_domParser->setExitOnFirstFatalError(m_exitOnFirstFatalError);

    if (m_entityResolver != 0)
    {
        m_domParser->setEntityResolver(m_entityResolver);
    }
    else
    {
        m_domParser->setXMLEntityResolver(m_xmlEntityResolver);
    }

    m_domParser->setErrorHandler(m_errorHandler);

    if (m_externalSchemaLocation.length() > 0)
    {
        m_domParser->setExternalSchemaLocation(m_externalSchemaLocation.c_str());
    }

    if (m_externalNoNamespaceSchemaLocation.length() > 0)
    {
        m_domParser->setExternalNoNamespaceSchemaLocation(m_externalNoNamespaceSchemaLocation.c_str());
    }
}



void
XercesParserLiaison::parseXMLStream(
            const InputSource&      inputSource,
            DocumentHandler&        handler,
            const XalanDOMString&   /* identifier */)
{
    XalanAutoPtr<SAXParserType>     theParser(createSAXParser());

    theParser->setExitOnFirstFatalError(m_exitOnFirstFatalError);

    if (m_entityResolver != 0)
    {
        theParser->setEntityResolver(m_entityResolver);
    }
    else
    {
        theParser->setXMLEntityResolver(m_xmlEntityResolver);
    }

    theParser->setErrorHandler(m_errorHandler);

    theParser->setDocumentHandler(&handler);

    theParser->parse(inputSource);
}



XalanDocument*
XercesParserLiaison::parseXMLStream(
            const InputSource&      inputSource,
            const XalanDOMString&   /* identifier */)
{
    ensureDOMParser();

    m_domParser->parse(inputSource);

    DOMDocument_Type* const theXercesDocument =
        m_domParser->getDocument();

    theXercesDocument->normalize();

    XercesDocumentWrapper*  theNewDocument = 0;

    if (theXercesDocument != 0)
    {
        theNewDocument = doCreateDocument(theXercesDocument, m_threadSafe, m_buildWrapper, m_buildMaps, true);

        m_domParser->adoptDocument();
    }

    return theNewDocument;
}



void
XercesParserLiaison::destroyDocument(XalanDocument*     theDocument)
{
    const DocumentMapType::iterator     i =
        m_documentMap.find(theDocument);

    if (i != m_documentMap.end())
    {
        const XalanMemMgrAutoPtr<XalanDocument>     theGuard(
                                                        m_documentMap.getMemoryManager(),
                                                        theDocument);

        m_documentMap.erase(i);
    }
}



int
XercesParserLiaison::getIndent() const
{
    return m_indent;
}



void
XercesParserLiaison::setIndent(int  i)
{
    m_indent = i;
}



bool
XercesParserLiaison::getUseValidation() const
{
    return m_useValidation;
}



void
XercesParserLiaison::setUseValidation(bool  b)
{
    m_useValidation = b;
}



const XalanDOMString&
XercesParserLiaison::getParserDescription(XalanDOMString& theResult) const
{
    theResult.assign("Xerces");

    return theResult;
}



DOMDocument_Type*
XercesParserLiaison::createDOMFactory()
{
    DOMDocument_Type* const     theXercesDocument =
        DOMImplementationType::getImplementation()->createDocument();

    createDocument(theXercesDocument, false, false);
    
    return theXercesDocument;
}



void
XercesParserLiaison::destroyDocument(DOMDocument_Type*  theDocument)
{
    // Delete any live documents...
    for(DocumentMapType::iterator i = m_documentMap.begin();
        i != m_documentMap.end();
        ++i)
    {
        if ((*i).second.isDeprecated() == false &&
            (*i).second.m_wrapper->getXercesDocument() == theDocument)
        {
            destroyDocument((XalanDocument*)(*i).first);
        }
    }
}



bool
XercesParserLiaison::getIncludeIgnorableWhitespace() const
{
    return m_includeIgnorableWhitespace;
}



void
XercesParserLiaison::setIncludeIgnorableWhitespace(bool include)
{
    m_includeIgnorableWhitespace = include;
}



ErrorHandler*
XercesParserLiaison::getErrorHandler() const
{
    return m_errorHandler;
}



void
XercesParserLiaison::setErrorHandler(ErrorHandler*  handler)
{
    if (handler == 0)
    {
        m_errorHandler = this;
    }
    else
    {
        m_errorHandler = handler;
    }
}



bool
XercesParserLiaison::getDoNamespaces() const
{
    return m_doNamespaces;
}



void
XercesParserLiaison::setDoNamespaces(bool   newState)
{
    m_doNamespaces = newState;
}



bool
XercesParserLiaison::getExitOnFirstFatalError() const
{
    return m_exitOnFirstFatalError;
}



void
XercesParserLiaison::setExitOnFirstFatalError(bool  newState)
{
    m_exitOnFirstFatalError = newState;
}



EntityResolver*
XercesParserLiaison::getEntityResolver() const
{
    return m_entityResolver;
}



void
XercesParserLiaison::setEntityResolver(EntityResolver*  resolver)
{
    m_entityResolver = resolver;
}



XMLEntityResolver*
XercesParserLiaison::getXMLEntityResolver() const
{
    return m_xmlEntityResolver;
}



void
XercesParserLiaison::setXMLEntityResolver(XMLEntityResolver*  resolver)
{
    m_xmlEntityResolver = resolver;
}



const XalanDOMChar*
XercesParserLiaison::getExternalSchemaLocation() const
{
    return m_externalSchemaLocation.length() == 0 ? 0 : m_externalSchemaLocation.c_str();
}



void
XercesParserLiaison::setExternalSchemaLocation(const XalanDOMChar*  location)
{
    if (location == 0)
    {
        m_externalSchemaLocation.clear();
    }
    else
    {
        m_externalSchemaLocation = location;
    }
}



const XalanDOMChar*
XercesParserLiaison::getExternalNoNamespaceSchemaLocation() const
{
    return m_externalNoNamespaceSchemaLocation.length() == 0 ? 0 : m_externalNoNamespaceSchemaLocation.c_str();
}



void
XercesParserLiaison::setExternalNoNamespaceSchemaLocation(const XalanDOMChar*   location)
{
    if (location == 0)
    {
        m_externalNoNamespaceSchemaLocation.clear();
    }
    else
    {
        m_externalNoNamespaceSchemaLocation= location;
    }
}



#if defined(XALAN_BUILD_DEPRECATED_DOM_BRIDGE)
XalanDocument*
XercesParserLiaison::createDocument(
            const DOM_Document_Type&    theXercesDocument,
            bool                        threadSafe,
            bool                        buildBridge)
{
    return doCreateDocument(theXercesDocument, threadSafe, buildBridge);
}
#endif



XalanDocument*
XercesParserLiaison::createDocument(
            const DOMDocument_Type*     theXercesDocument,
            bool                        threadSafe,
            bool                        buildWrapper,
            bool                        buildMaps)
{
    // As we did not create the underlying DOMDocument - ensure we don't
    // delete it later.
    return doCreateDocument(theXercesDocument, threadSafe, buildWrapper, buildMaps, false);
}



#if defined(XALAN_BUILD_DEPRECATED_DOM_BRIDGE)
XercesDocumentBridge*
XercesParserLiaison::mapDocument(const XalanDocument*   theDocument) const
{
    const DocumentMapType::const_iterator   i =
        m_documentMap.find(theDocument);

    return i != m_documentMap.end() ? (*i).second.m_isDeprecated == true ? (*i).second.m_bridge : 0 : 0;
}
#endif



XercesDocumentWrapper*
XercesParserLiaison::mapDocumentToWrapper(const XalanDocument*  theDocument) const
{
    const DocumentMapType::const_iterator   i =
        m_documentMap.find(theDocument);

    return i != m_documentMap.end() ? (*i).second.isDeprecated() == false ? (*i).second.m_wrapper : 0 : 0;
}



#if defined(XALAN_BUILD_DEPRECATED_DOM_BRIDGE)
DOM_Document_Type
XercesParserLiaison::mapXercesDocument(const XalanDocument*     theDocument) const
{
    const DocumentMapType::const_iterator   i =
        m_documentMap.find(theDocument);

    return i != m_documentMap.end() ? (*i).second.isDeprecated() == true ? (*i).second.m_bridge->getXercesDocument() : DOM_Document_Type() : DOM_Document_Type();
}
#endif



const DOMDocument_Type*
XercesParserLiaison::mapToXercesDocument(const XalanDocument*   theDocument) const
{
    const DocumentMapType::const_iterator   i =
        m_documentMap.find(theDocument);

    return i != m_documentMap.end() ? (*i).second.isDeprecated() == false ? (*i).second.m_wrapper->getXercesDocument() : 0 : 0;
}



void
XercesParserLiaison::fatalError(const SAXParseExceptionType&    e)
{
    XalanDOMString  theMessage(getMemoryManager());
    
    XalanMessageLoader::getMessage(
        theMessage,
        XalanMessages::FatalError);

    formatErrorMessage(e, theMessage);

    if (m_executionContext != 0)
    {
        // We call warning() because we don't want the execution
        // context to potentially throw an exception.
        m_executionContext->problem(
            ExecutionContext::eXMLParser,
            ExecutionContext::eWarning,
            theMessage,
            0);
    }
    else
    {
        XALAN_USING_STD(cerr)
        XALAN_USING_STD(endl)

        cerr << endl << theMessage << endl;
    }

    throw e;
}



void
XercesParserLiaison::error(const SAXParseExceptionType&     e)
{
    XalanDOMString  theMessage(getMemoryManager());

    XalanMessageLoader::getMessage(
        theMessage,
        XalanMessages::Error2);

    formatErrorMessage(e, theMessage);

    if (m_executionContext != 0)
    {
        // We call warn() because we don't want the execution
        // context to potentially throw an exception.
        m_executionContext->problem(
            ExecutionContext::eXMLParser,
            ExecutionContext::eWarning,
            theMessage,
            0);
    }
    else
    {
        XALAN_USING_STD(cerr)
        XALAN_USING_STD(endl)

        cerr << endl << theMessage << endl;
    }

    if (m_useValidation == true)
    {
        throw e;
    }
}



void
XercesParserLiaison::warning(const SAXParseExceptionType&   e)
{
    XalanDOMString  theMessage(getMemoryManager());

    XalanMessageLoader::getMessage(
        theMessage,
        XalanMessages::Warning2);

    formatErrorMessage(e, theMessage);

    if (m_executionContext != 0)
    {
        m_executionContext->problem(
            ExecutionContext::eXMLParser,
            ExecutionContext::eWarning,
            theMessage,
            0);
    }
    else
    {
        XALAN_USING_STD(cerr)
        XALAN_USING_STD(endl)

        cerr << endl << theMessage << endl;
    }
}



void
XercesParserLiaison::formatErrorMessage(
            const SAXParseExceptionType&    e,
            XalanDOMString&                 theMessage)
{
    XalanDOMString  theErrorBuffer(theMessage.getMemoryManager());

    const XalanDOMChar* const   theExceptionMessage = e.getMessage();
    assert(theExceptionMessage != 0);

    const XalanDOMChar* const   theSystemID = e.getSystemId();

    XSLException::defaultFormat(
        theExceptionMessage,
        length(theExceptionMessage),
        theSystemID,
        theSystemID == 0 ? 0 : length(theSystemID),
        e.getLineNumber(),
        e.getColumnNumber(),
        0,
        0,
        theMessage);
}



void
XercesParserLiaison::resetErrors()
{
}



XercesParserLiaison::DOMParserType*
XercesParserLiaison::createDOMParser()
{
    MemoryManager&  theMemoryManager =
        getMemoryManager();

    DOMParserType* const    theParser =
        new (&theMemoryManager) DOMParserType(0, &theMemoryManager);

#if XERCES_VERSION_MAJOR < 3
    theParser->setExpandEntityReferences(true);
#else
    theParser->setCreateEntityReferenceNodes(false);
#endif

    theParser->setDoNamespaces(m_doNamespaces);

    return theParser;
}



XercesParserLiaison::SAXParserType*
XercesParserLiaison::createSAXParser()
{
    MemoryManager&  theMemoryManager =
        getMemoryManager();

    SAXParserType* const    theParser =
        new (&theMemoryManager) SAXParserType(0, &theMemoryManager);

#if XERCES_VERSION_MAJOR < 3
    theParser->setDoValidation(false);
#else
    theParser->setValidationScheme(SAXParserType::Val_Never);
#endif

    theParser->setDoNamespaces(false);

    return theParser;
}



#if defined(XALAN_BUILD_DEPRECATED_DOM_BRIDGE)
XercesDocumentBridge*
XercesParserLiaison::doCreateDocument(
            const DOM_Document_Type&    theXercesDocument,
            bool                        threadSafe,
            bool                        buildBridge)
{
    XercesDocumentBridge* const     theNewDocument =
        new XercesDocumentBridge(theXercesDocument, threadSafe, buildBridge);

    m_documentMap[theNewDocument] = theNewDocument;

    return theNewDocument;
}
#endif



XercesDocumentWrapper*
XercesParserLiaison::doCreateDocument(
            const DOMDocument_Type*     theXercesDocument,
            bool                        threadSafe,
            bool                        buildWrapper,
            bool                        buildMaps,
            bool                        isOwned)
{
    XercesDocumentWrapper* const        theNewDocument =
        XercesDocumentWrapper::create(getMemoryManager(),theXercesDocument, threadSafe, buildWrapper, buildMaps);

    DocumentEntry&  theEntry = m_documentMap[theNewDocument];
    
    theEntry = theNewDocument;
    theEntry.m_isOwned = isOwned;

    return theNewDocument;
}



XALAN_CPP_NAMESPACE_END
