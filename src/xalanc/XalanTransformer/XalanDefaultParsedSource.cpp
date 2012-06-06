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
#include "XalanDefaultParsedSource.hpp"



#include <xercesc/sax/InputSource.hpp>



#include <xalanc/XalanDOM/XalanDocument.hpp>



#include <xalanc/PlatformSupport/URISupport.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include <xalanc/XalanSourceTree/XalanSourceTreeDocument.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XalanDefaultParsedSourceDOMSupport::XalanDefaultParsedSourceDOMSupport(
            const XalanSourceTreeParserLiaison&     theParserLiaison,
            const XalanSourceTreeDOMSupport&        theDOMSupport) :
    XalanSourceTreeDOMSupport(theParserLiaison),
    m_domSupport(theDOMSupport)
{
}



XalanDefaultParsedSourceDOMSupport::~XalanDefaultParsedSourceDOMSupport()
{
}



void
XalanDefaultParsedSourceDOMSupport::reset()
{
}



const XalanDOMString&
XalanDefaultParsedSourceDOMSupport::getUnparsedEntityURI(
            const XalanDOMString&   theName,
            const XalanDocument&    theDocument) const
{
    // Check the wrapped XalanSourceTreeDOMSupport instance...
    const XalanDOMString&   theURI =
            m_domSupport.getUnparsedEntityURI(
                    theName,
                    theDocument);

    if (theURI.empty() == false)
    {
        return theURI;
    }
    else
    {
        // Chain up to our parent...
        return XalanSourceTreeDOMSupport::getUnparsedEntityURI(
                    theName,
                    theDocument);
    }
}



bool
XalanDefaultParsedSourceDOMSupport::isNodeAfter(
            const XalanNode&    node1,
            const XalanNode&    node2) const
{
    return m_domSupport.isNodeAfter(
                    node1, 
                    node2);
}



XalanDefaultParsedSourceHelper::XalanDefaultParsedSourceHelper(
            const XalanSourceTreeDOMSupport&    theSourceDOMSupport,
            MemoryManager&                      theManager) :
    m_parserLiaison(theManager),
    m_domSupport(m_parserLiaison, theSourceDOMSupport)
{
}



XalanDefaultParsedSourceHelper*
XalanDefaultParsedSourceHelper::create(
            const XalanSourceTreeDOMSupport&    theSourceDOMSupport,
            MemoryManager&                      theManager)
{
    typedef XalanDefaultParsedSourceHelper  ThisType;

    XalanAllocationGuard    theGuard(theManager, theManager.allocate(sizeof(ThisType)));

    ThisType* const     theResult =
        new (theGuard.get()) ThisType(
                                theSourceDOMSupport,
                                theManager);

    theGuard.release();

    return theResult;
}



XalanDefaultParsedSourceHelper::~XalanDefaultParsedSourceHelper()
{
}



DOMSupport&
XalanDefaultParsedSourceHelper::getDOMSupport()
{
    return m_domSupport;
}



XMLParserLiaison&
XalanDefaultParsedSourceHelper::getParserLiaison()
{
    return m_parserLiaison;
}



XalanDefaultParsedSource::XalanDefaultParsedSource(
            const InputSource&      theInputSource,
            bool                    fValidate,
            ErrorHandler*           theErrorHandler,
            EntityResolver*         theEntityResolver,
            XMLEntityResolver*      theXMLEntityResolver,
            const XalanDOMChar*     theExternalSchemaLocation,
            const XalanDOMChar*     theExternalNoNamespaceSchemaLocation,
            bool                    fPoolAllTextNodes,
            MemoryManager&          theManager) :
    XalanParsedSource(),
    m_parserLiaison(theManager),
    m_domSupport(m_parserLiaison),
    m_parsedSource(0),
    m_uri(theManager)
{
    m_parserLiaison.setUseValidation(fValidate);
    m_parserLiaison.setEntityResolver(theEntityResolver);
    m_parserLiaison.setXMLEntityResolver(theXMLEntityResolver);
    m_parserLiaison.setErrorHandler(theErrorHandler);
    m_parserLiaison.setExternalSchemaLocation(theExternalSchemaLocation);
    m_parserLiaison.setExternalNoNamespaceSchemaLocation(theExternalNoNamespaceSchemaLocation);
    m_parserLiaison.setPoolAllText(fPoolAllTextNodes);

    m_parsedSource = m_parserLiaison.mapDocument(m_parserLiaison.parseXMLStream(theInputSource));
    assert(m_parsedSource != 0);

    m_domSupport.setParserLiaison(&m_parserLiaison);

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


    
XalanDefaultParsedSource*
XalanDefaultParsedSource::create(
            MemoryManager&          theManager,
            const InputSource&      theInputSource,
            bool                    fValidate,
            ErrorHandler*           theErrorHandler,
            EntityResolver*         theEntityResolver,
            XMLEntityResolver*      theXMLEntityResolver,
            const XalanDOMChar*     theExternalSchemaLocation,
            const XalanDOMChar*     theExternalNoNamespaceSchemaLocation,
            bool                    fPoolAllTextNodes)
{
    typedef XalanDefaultParsedSource ThisType;

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
                                fPoolAllTextNodes,
                                theManager);

    theGuard.release();

    return theResult;
}



XalanDefaultParsedSource::~XalanDefaultParsedSource()
{
}



XalanDocument*  
XalanDefaultParsedSource::getDocument() const
{
    return m_parsedSource;
}



XalanParsedSourceHelper*
XalanDefaultParsedSource::createHelper(MemoryManager& theManager) const
{
    return XalanDefaultParsedSourceHelper::create(m_domSupport, theManager);
}



const XalanDOMString&
XalanDefaultParsedSource::getURI() const
{
    return m_uri;
}



XALAN_CPP_NAMESPACE_END
