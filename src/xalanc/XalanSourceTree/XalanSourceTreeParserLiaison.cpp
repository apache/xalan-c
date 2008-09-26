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
#include "XalanSourceTreeParserLiaison.hpp"



#include <algorithm>



#include "xercesc/parsers/SAX2XMLReaderImpl.hpp"
#include "xercesc/util/XMLUni.hpp"



#include <xalanc/Include/XalanAutoPtr.hpp>
#include <xalanc/Include/STLHelper.hpp>



#include <xalanc/PlatformSupport/XalanUnicode.hpp>



#include "XalanSourceTreeContentHandler.hpp"
#include "XalanSourceTreeDOMSupport.hpp"
#include "XalanSourceTreeDocument.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XalanSourceTreeParserLiaison::XalanSourceTreeParserLiaison(
            XalanSourceTreeDOMSupport&  /* theSupport */,
            MemoryManager&          theManager) :
    m_xercesParserLiaison(theManager),
    m_documentMap(theManager),
    m_poolAllText(true),
    m_xmlReader(0)
{
}



XalanSourceTreeParserLiaison::XalanSourceTreeParserLiaison(MemoryManager&   theManager) :
    m_xercesParserLiaison(theManager),
    m_documentMap(theManager),
    m_poolAllText(true),
    m_xmlReader(0)
{
}



XalanSourceTreeParserLiaison::~XalanSourceTreeParserLiaison()
{
    reset();

    delete m_xmlReader;
}



void
XalanSourceTreeParserLiaison::reset()
{
    typedef DocumentMapType::iterator   iterator;

    MemoryManager&  theManager = getMemoryManager();

    for (iterator i = m_documentMap.begin(); i != m_documentMap.end(); ++i)
    {
        assert((*i).second != 0);

        XalanDestroy(
            theManager,
            *(*i).second);

        (*i).second = 0;
    }

    m_documentMap.clear();

    m_xercesParserLiaison.reset();
}



ExecutionContext*
XalanSourceTreeParserLiaison::getExecutionContext() const
{
    return m_xercesParserLiaison.getExecutionContext();
}



void
XalanSourceTreeParserLiaison::setExecutionContext(ExecutionContext&     theContext)
{
    m_xercesParserLiaison.setExecutionContext(theContext);
}



void
XalanSourceTreeParserLiaison::parseXMLStream(
            const InputSource&      inputSource,
            DocumentHandler&        handler,
            const XalanDOMString&   identifier)
{
    m_xercesParserLiaison.parseXMLStream(inputSource, handler, identifier);
}



void
XalanSourceTreeParserLiaison::ensureReader()
{
    XALAN_USING_XERCES(XMLUni)

    if (m_xmlReader == 0)
    {
        m_xmlReader = createReader();
    }

    const bool  fValidate =
        m_xercesParserLiaison.getUseValidation();

    if (fValidate == false)
    {
        m_xmlReader->setFeature(
            XMLUni::fgSAX2CoreValidation,
            false);

        m_xmlReader->setFeature(
            XMLUni::fgXercesDynamic,
            false);

        m_xmlReader->setFeature(
            XMLUni::fgXercesSchema,
            false);
    }
    else
    {
        m_xmlReader->setFeature(
            XMLUni::fgSAX2CoreValidation,
            true);

        m_xmlReader->setFeature(
            XMLUni::fgXercesDynamic,
            true);

        m_xmlReader->setFeature(
            XMLUni::fgXercesSchema,
            true);
    }

    ErrorHandler* const     theHandler = getErrorHandler();

    if (theHandler == 0)
    {
        m_xmlReader->setErrorHandler(&m_xercesParserLiaison);
    }
    else
    {
        m_xmlReader->setErrorHandler(theHandler);
    }

    EntityResolver* const   theEntityResolver =
        getEntityResolver();

    if (theEntityResolver != 0)
    {
        m_xmlReader->setEntityResolver(theEntityResolver);
    }
    else
    {
        m_xmlReader->setXMLEntityResolver(getXMLEntityResolver());
    }

    {
        const XalanDOMChar* const   theLocation =
            getExternalSchemaLocation();

        if (theLocation != 0)
        {
            m_xmlReader->setProperty(
                XMLUni::fgXercesSchemaExternalSchemaLocation,
                const_cast<XalanDOMChar*>(theLocation));
        }
    }

    {
        const XalanDOMChar* const   theLocation =
            getExternalNoNamespaceSchemaLocation();

        if (theLocation != 0)
        {
            m_xmlReader->setProperty(
                XMLUni::fgXercesSchemaExternalNoNameSpaceSchemaLocation,
                const_cast<XalanDOMChar*>(theLocation));
        }
    }
}



XalanDocument*
XalanSourceTreeParserLiaison::parseXMLStream(
            const InputSource&      inputSource,
            const XalanDOMString&   identifier)
{
    XalanSourceTreeDocument* const  theDocument =
        createXalanSourceTreeDocument();

    EnsureDestroyDocument   theGuard(
                                *this,
                                theDocument);

    XalanSourceTreeContentHandler   theContentHandler(
                                        getMemoryManager(),
                                        theDocument);

    parseXMLStream(
        inputSource,
        theContentHandler,
        identifier,
        &theContentHandler,
        &theContentHandler);

    return theGuard.release();
}



void
XalanSourceTreeParserLiaison::destroyDocument(XalanDocument*    theDocument)
{
    if (mapDocument(theDocument) != 0)
    {
        m_documentMap.erase(theDocument);

        assert(theDocument != 0);

        XalanDestroy(
            m_documentMap.getMemoryManager(),
            *theDocument);
    }
    else
    {
        m_xercesParserLiaison.destroyDocument(theDocument);
    }
}



int
XalanSourceTreeParserLiaison::getIndent() const
{
    return m_xercesParserLiaison.getIndent();
}



void
XalanSourceTreeParserLiaison::setIndent(int     i)
{
    m_xercesParserLiaison.setIndent(i);
}



bool
XalanSourceTreeParserLiaison::getUseValidation() const
{
    return m_xercesParserLiaison.getUseValidation();
}



void
XalanSourceTreeParserLiaison::setUseValidation(bool     b)
{
    m_xercesParserLiaison.setUseValidation(b);
}



const XalanDOMString&
XalanSourceTreeParserLiaison::getParserDescription(XalanDOMString&  theResult) const
{
    theResult.assign("XalanSourceTree");
    
    return theResult;
}



void
XalanSourceTreeParserLiaison::parseXMLStream(
            const InputSource&      theInputSource,
            ContentHandler&         theContentHandler,
            const XalanDOMString&   /* theIdentifier */,
            DTDHandler*             theDTDHandler,
            LexicalHandler*         theLexicalHandler)
{
    ensureReader();

    assert(m_xmlReader != 0);

    m_xmlReader->setContentHandler(&theContentHandler);

    m_xmlReader->setDTDHandler(theDTDHandler);

    m_xmlReader->setLexicalHandler(theLexicalHandler);

    m_xmlReader->parse(theInputSource);
}



DOMDocument_Type*
XalanSourceTreeParserLiaison::createDOMFactory()
{
    return m_xercesParserLiaison.createDOMFactory();
}



void
XalanSourceTreeParserLiaison::destroyDocument(DOMDocument_Type*     theDocument)
{
    m_xercesParserLiaison.destroyDocument(theDocument);
}



bool
XalanSourceTreeParserLiaison::getIncludeIgnorableWhitespace() const
{
    return m_xercesParserLiaison.getIncludeIgnorableWhitespace();
}



void
XalanSourceTreeParserLiaison::setIncludeIgnorableWhitespace(bool    include)
{
    m_xercesParserLiaison.setIncludeIgnorableWhitespace(include);
}



ErrorHandler*
XalanSourceTreeParserLiaison::getErrorHandler() const
{
    return m_xercesParserLiaison.getErrorHandler();
}



void
XalanSourceTreeParserLiaison::setErrorHandler(ErrorHandler*     handler)
{
    m_xercesParserLiaison.setErrorHandler(handler);
}



bool
XalanSourceTreeParserLiaison::getDoNamespaces() const
{
    return m_xercesParserLiaison.getDoNamespaces();
}



void
XalanSourceTreeParserLiaison::setDoNamespaces(bool  newState)
{
    m_xercesParserLiaison.setDoNamespaces(newState);
}



bool
XalanSourceTreeParserLiaison::getExitOnFirstFatalError() const
{
    return m_xercesParserLiaison.getExitOnFirstFatalError();
}



void
XalanSourceTreeParserLiaison::setExitOnFirstFatalError(bool     newState)
{
    m_xercesParserLiaison.setExitOnFirstFatalError(newState);
}



EntityResolver*
XalanSourceTreeParserLiaison::getEntityResolver() const
{
    return m_xercesParserLiaison.getEntityResolver();
}



void
XalanSourceTreeParserLiaison::setEntityResolver(EntityResolver*     resolver)
{
    m_xercesParserLiaison.setEntityResolver(resolver);
}



XMLEntityResolver*
XalanSourceTreeParserLiaison::getXMLEntityResolver() const
{
    return m_xercesParserLiaison.getXMLEntityResolver();
}



void
XalanSourceTreeParserLiaison::setXMLEntityResolver(XMLEntityResolver*   resolver)
{
    m_xercesParserLiaison.setXMLEntityResolver(resolver);
}



const XalanDOMChar*
XalanSourceTreeParserLiaison::getExternalSchemaLocation() const
{
    return m_xercesParserLiaison.getExternalSchemaLocation();
}



void
XalanSourceTreeParserLiaison::setExternalSchemaLocation(const XalanDOMChar*     location)
{
    m_xercesParserLiaison.setExternalSchemaLocation(location);
}



const XalanDOMChar*
XalanSourceTreeParserLiaison::getExternalNoNamespaceSchemaLocation() const
{
    return m_xercesParserLiaison.getExternalNoNamespaceSchemaLocation();
}



void
XalanSourceTreeParserLiaison::setExternalNoNamespaceSchemaLocation(const XalanDOMChar*  location)
{
    m_xercesParserLiaison.setExternalNoNamespaceSchemaLocation(location);
}



XalanSourceTreeDocument*
XalanSourceTreeParserLiaison::mapDocument(const XalanDocument*  theDocument) const
{
    DocumentMapType::const_iterator     i =
        m_documentMap.find(theDocument);

    return i != m_documentMap.end() ? (*i).second : 0;
}



XalanSourceTreeDocument*
XalanSourceTreeParserLiaison::createXalanSourceTreeDocument()
{
    XalanSourceTreeDocument* const  theNewDocument =
        XalanSourceTreeDocument::create(getMemoryManager(), m_poolAllText);

    m_documentMap[theNewDocument] = theNewDocument;

    return theNewDocument;
}



SAX2XMLReaderImpl*
XalanSourceTreeParserLiaison::createReader()
{
    XALAN_USING_XERCES(XMLUni)

    typedef XalanAutoPtr<SAX2XMLReaderImpl> AutoPtrType;

    MemoryManager&  theManager = getMemoryManager();

    AutoPtrType     theReader(new (&theManager) SAX2XMLReaderImpl(&theManager));

    theReader->setFeature(
        XMLUni::fgSAX2CoreNameSpaces,
        true); 

    theReader->setFeature(
        XMLUni::fgSAX2CoreNameSpacePrefixes,
        true);

    return theReader.release();
}



XALAN_CPP_NAMESPACE_END
