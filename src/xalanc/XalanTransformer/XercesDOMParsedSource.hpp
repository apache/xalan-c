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
#if !defined(XERCESDOMPARSEDSOURCE_HEADER_GUARD)
#define XERCESDOMPARSEDSOURCE_HEADER_GUARD



// Base include file.  Must be first.
#include <xalanc/XalanTransformer/XalanTransformerDefinitions.hpp>



#include <xalanc/XercesParserLiaison/XercesParserLiaison.hpp>
#include <xalanc/XercesParserLiaison/XercesDOMSupport.hpp>



#include <xalanc/XalanTransformer/XalanParsedSource.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_TRANSFORMER_EXPORT XercesDOMParsedSourceHelper : public XalanParsedSourceHelper
{
public:

    XercesDOMParsedSourceHelper(MemoryManager& theManager XALAN_DEFAULT_CONSTRUCTOR_MEMMGR);

    ~XercesDOMParsedSourceHelper();

    static XercesDOMParsedSourceHelper*
    create(MemoryManager& theManager);

    virtual DOMSupport&
    getDOMSupport();

    virtual XMLParserLiaison&
    getParserLiaison();

private:

    // There is an order dependency, because
    // XercesDOMSupport requires a XercesParserLiaison
    // instance.
    XercesParserLiaison     m_parserLiaison;

    XercesDOMSupport        m_domSupport;
};



/**
 * This is designed to allow a XalanTranfomer object to reuse a parsed
 * document. 
 */
class XALAN_TRANSFORMER_EXPORT XercesDOMParsedSource : public XalanParsedSource
{
public:
    
    XercesDOMParsedSource(
            const InputSource&      theInputSource,
            bool                    fValidate = false,
            ErrorHandler*           theErrorHandler = 0,
            EntityResolver*         theEntityResolver = 0,
            XMLEntityResolver*      theXMLEntityResolver = 0,
            const XalanDOMChar*     theExternalSchemaLocation = 0,
            const XalanDOMChar*     theExternalNoNamespaceSchemaLocation = 0,
            MemoryManager&          theManager XALAN_DEFAULT_MEMMGR);

    static XercesDOMParsedSource*
    create(
            MemoryManager&          theManager,
            const InputSource&      theInputSource,
            bool                    fValidate = false,
            ErrorHandler*           theErrorHandler = 0,
            EntityResolver*         theEntityResolver = 0,
            XMLEntityResolver*      theXMLEntityResolver = 0,
            const XalanDOMChar*     theExternalSchemaLocation = 0,
            const XalanDOMChar*     theExternalNoNamespaceSchemaLocation = 0);

    virtual
    ~XercesDOMParsedSource();

    virtual XalanDocument*
    getDocument() const;

    virtual XalanParsedSourceHelper*
    createHelper(MemoryManager&     theManager) const;

    virtual const XalanDOMString&
    getURI() const;

private:

    XercesParserLiaison     m_parserLiaison;

    XalanDocument*          m_parsedSource;

    XalanDOMString          m_uri;
};



XALAN_CPP_NAMESPACE_END



#endif  // XERCESDOMPARSEDSOURCE_HEADER_GUARD
