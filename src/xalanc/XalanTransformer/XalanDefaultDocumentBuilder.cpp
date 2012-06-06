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
#include "XalanDefaultDocumentBuilder.hpp"



#include <xalanc/XalanSourceTree/XalanSourceTreeDocument.hpp>



#include "XalanDefaultParsedSource.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XalanDefaultDocumentBuilder::XalanDefaultDocumentBuilder(MemoryManager&     theManager,
                                                         const XalanDOMString&  theURI) :
    m_domSupport(),
    m_parserLiaison(theManager),
    m_contentHandler(theManager, m_parserLiaison.createXalanSourceTreeDocument()),
    m_uri(theURI,theManager)
{
    m_domSupport.setParserLiaison(&m_parserLiaison);
}



XalanDefaultDocumentBuilder*
XalanDefaultDocumentBuilder::create(
            MemoryManager&          theManager,
            const XalanDOMString&   theURI)
{
    typedef XalanDefaultDocumentBuilder     ThisType;

    XalanAllocationGuard    theGuard(theManager, theManager.allocate(sizeof(ThisType)));

    ThisType* const     theResult =
        new (theGuard.get()) ThisType(
                                theManager,
                                theURI);

    theGuard.release();

    return theResult;
}



XalanDefaultDocumentBuilder::~XalanDefaultDocumentBuilder()
{
}



XalanDocument*
XalanDefaultDocumentBuilder::getDocument() const
{
    return m_contentHandler.getDocument();
}



XalanParsedSourceHelper*
XalanDefaultDocumentBuilder::createHelper(MemoryManager& theManager) const
{
    return XalanDefaultParsedSourceHelper::create(m_domSupport, theManager);
}



ContentHandlerType*
XalanDefaultDocumentBuilder::getContentHandler()
{
    return &m_contentHandler;
}



DTDHandlerType*
XalanDefaultDocumentBuilder::getDTDHandler()
{
    return &m_contentHandler;
}



LexicalHandlerType*
XalanDefaultDocumentBuilder::getLexicalHandler()
{
    return &m_contentHandler;
}



const XalanDOMString&
XalanDefaultDocumentBuilder::getURI() const
{
    return m_uri;
}



XALAN_CPP_NAMESPACE_END
