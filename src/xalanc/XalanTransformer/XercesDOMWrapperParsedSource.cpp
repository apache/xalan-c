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
#include "XercesDOMWrapperParsedSource.hpp"



#include <xalanc/XalanDOM/XalanDocument.hpp>



#include <xalanc/PlatformSupport/URISupport.hpp>



#include <xalanc/XercesParserLiaison/XercesParserLiaison.hpp>
#include <xalanc/XercesParserLiaison/XercesDOMSupport.hpp>



#include "XercesDOMParsedSource.hpp"



XALAN_CPP_NAMESPACE_BEGIN


#if defined(XALAN_BUILD_DEPRECATED_DOM_BRIDGE)
XercesDOMWrapperParsedSource::XercesDOMWrapperParsedSource(
            const DOM_Document_Type&    theDocument,
            XercesParserLiaison&        theParserLiaison,
            XercesDOMSupport&           theDOMSupport,
            const XalanDOMString&       theURI,
            MemoryManager&          theManager) :
    XalanParsedSource(),
    m_parserLiaison(theParserLiaison),
    m_domSupport(theDOMSupport),
    m_parsedSource(theParserLiaison.createDocument(theDocument, true, true)),
    m_uri(theURI,theManager)
{
    XalanDOMString& refURI = const_cast<XalanDOMString&>(m_uri);

    URISupport::NormalizeURIText(refURI);

    assert(m_parsedSource != 0);
}
#endif



XercesDOMWrapperParsedSource::XercesDOMWrapperParsedSource(
            const DOMDocument_Type*     theDocument,
            XercesParserLiaison&        theParserLiaison,
            XercesDOMSupport&           theDOMSupport,
            const XalanDOMString&       theURI,
            MemoryManager&          theManager) :
    XalanParsedSource(),
    m_parserLiaison(theParserLiaison),
    m_domSupport(theDOMSupport),
    m_parsedSource(theParserLiaison.createDocument(theDocument, true, true)),
    m_uri(theURI, theManager)
{
    XalanDOMString& refURI = const_cast<XalanDOMString&>(m_uri);

    URISupport::NormalizeURIText(refURI);


    assert(m_parsedSource != 0);
}



XercesDOMWrapperParsedSource::~XercesDOMWrapperParsedSource()
{
    m_parserLiaison.destroyDocument(m_parsedSource);
}



XalanDocument*
XercesDOMWrapperParsedSource::getDocument() const
{
    return m_parsedSource;
}



XalanParsedSourceHelper*
XercesDOMWrapperParsedSource::createHelper(MemoryManager& theManager) const
{
    return XercesDOMParsedSourceHelper::create(theManager);
}



const XalanDOMString&
XercesDOMWrapperParsedSource::getURI() const
{
    return m_uri;
}



XALAN_CPP_NAMESPACE_END
