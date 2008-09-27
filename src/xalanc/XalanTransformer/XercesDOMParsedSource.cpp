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
#include "XercesDOMParsedSource.hpp"



#include <xercesc/sax/InputSource.hpp>



#include <xalanc/XalanDOM/XalanDocument.hpp>



#include <xalanc/PlatformSupport/URISupport.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XercesDOMParsedSourceHelper::XercesDOMParsedSourceHelper(MemoryManager&     theManager) :
    m_parserLiaison(theManager),
    m_domSupport(m_parserLiaison)
{
}



XercesDOMParsedSourceHelper*
XercesDOMParsedSourceHelper::create(MemoryManager&  theManager)
{
    XercesDOMParsedSourceHelper*    theInstance = 0;

    return XalanConstruct(
        theManager,
        theInstance,
        theManager);
}



XercesDOMParsedSourceHelper::~XercesDOMParsedSourceHelper()
{
}



DOMSupport&
XercesDOMParsedSourceHelper::getDOMSupport()
{
    return m_domSupport;
}



XMLParserLiaison&
XercesDOMParsedSourceHelper::getParserLiaison()
{
    return m_parserLiaison;
}



XercesDOMParsedSource::XercesDOMParsedSource(
            const InputSource&      theInputSource,
            bool                    fValidate,
            ErrorHandler*           theErrorHandler,
            EntityResolver*         theEntityResolver,
            XMLEntityResolver*      theXMLEntityResolver,
            const XalanDOMChar*     theExternalSchemaLocation,
            const XalanDOMChar*     theExternalNoNamespaceSchemaLocation,
            MemoryManager&          theManager) :
    XalanParsedSource(),
    m_parserLiaison(theManager),
    m_parsedSource(0),
    m_uri(theManager)
{
    m_parserLiaison.setUseValidation(fValidate);
    m_parserLiaison.setEntityResolver(theEntityResolver);
    m_parserLiaison.setXMLEntityResolver(theXMLEntityResolver);
    m_parserLiaison.setErrorHandler(theErrorHandler);
    m_parserLiaison.setExternalSchemaLocation(theExternalSchemaLocation);
    m_parserLiaison.setExternalNoNamespaceSchemaLocation(theExternalNoNamespaceSchemaLocation);

    m_parsedSource = m_parserLiaison.parseXMLStream(theInputSource);
    assert(m_parsedSource != 0);

    const XalanDOMChar* const   theSystemID = theInputSource.getSystemId();

    if (theSystemID != 0)
    {
        try
        {
            URISupport::getURLStringFromString(theSystemID, m_uri);
        }
        catch(const XERCES_CPP_NAMESPACE_QUALIFIER XMLException&)
        {
            // Assume that any exception here relates to get the url from
            // the system ID.  We'll assume that it's just a fake base identifier
            // since the parser would have thrown an error if the system ID
            // wasn't resolved.
            m_uri = theSystemID;
        }
    }
}



XercesDOMParsedSource*
XercesDOMParsedSource::create(
            MemoryManager&          theManager,
            const InputSource&      theInputSource,
            bool                    fValidate,
            ErrorHandler*           theErrorHandler,
            EntityResolver*         theEntityResolver,
            XMLEntityResolver*      theXMLEntityResolver,
            const XalanDOMChar*     theExternalSchemaLocation,
            const XalanDOMChar*     theExternalNoNamespaceSchemaLocation)
{
    typedef XercesDOMParsedSource ThisType;

    XalanAllocationGuard    theGuard(
                                theManager,
                                theManager.allocate(sizeof(ThisType)));

    ThisType* theResult =
        new (theGuard.get()) ThisType(
                                theInputSource,
                                fValidate,
                                theErrorHandler,
                                theEntityResolver,
                                theXMLEntityResolver,
                                theExternalSchemaLocation,
                                theExternalNoNamespaceSchemaLocation,
                                theManager);


    theGuard.release();

    return theResult;
}



XercesDOMParsedSource::~XercesDOMParsedSource()
{
}



XalanDocument*
XercesDOMParsedSource::getDocument() const
{
    return m_parsedSource;
}



XalanParsedSourceHelper*
XercesDOMParsedSource::createHelper(MemoryManager& theManager) const
{
    return XercesDOMParsedSourceHelper::create(theManager);
}



const XalanDOMString&
XercesDOMParsedSource::getURI() const
{
    return m_uri;
}



XALAN_CPP_NAMESPACE_END
