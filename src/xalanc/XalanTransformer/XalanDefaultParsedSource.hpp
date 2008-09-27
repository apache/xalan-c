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
#if !defined(XALANDEFAULTPARSEDSOURCE_HEADER_GUARD)
#define XALANDEFAULTPARSEDSOURCE_HEADER_GUARD



// Base include file.  Must be first.
#include <xalanc/XalanTransformer/XalanTransformerDefinitions.hpp>



#include <xalanc/XalanSourceTree/XalanSourceTreeDocument.hpp>
#include <xalanc/XalanSourceTree/XalanSourceTreeDOMSupport.hpp>
#include <xalanc/XalanSourceTree/XalanSourceTreeParserLiaison.hpp>



#include <xalanc/XalanTransformer/XalanParsedSource.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XSLTInputSource;



class XalanDefaultParsedSourceDOMSupport : public XalanSourceTreeDOMSupport
{
public:

    XalanDefaultParsedSourceDOMSupport(
            const XalanSourceTreeParserLiaison&     theParserLiaison,
            const XalanSourceTreeDOMSupport&        theDOMSupport);

    virtual
    ~XalanDefaultParsedSourceDOMSupport();

    virtual void
    reset();

    // These interfaces are inherited from DOMSupport...

    virtual const XalanDOMString&
    getUnparsedEntityURI(
            const XalanDOMString&   theName,
            const XalanDocument&    theDocument) const;

    virtual bool
    isNodeAfter(
            const XalanNode&    node1,
            const XalanNode&    node2) const;

private:

    // Not implemented...
    XalanDefaultParsedSourceDOMSupport(const XalanDefaultParsedSourceDOMSupport&);

    XalanDefaultParsedSourceDOMSupport&
    operator=(const XalanDefaultParsedSourceDOMSupport&);


    // Data members...
    const XalanSourceTreeDOMSupport&    m_domSupport;
};



class XALAN_TRANSFORMER_EXPORT XalanDefaultParsedSourceHelper : public XalanParsedSourceHelper
{
public:

    XalanDefaultParsedSourceHelper(const XalanSourceTreeDOMSupport&     theSourceDOMSupport,
                                    MemoryManager&                  theManager XALAN_DEFAULT_MEMMGR);

    static XalanDefaultParsedSourceHelper*
    create(const XalanSourceTreeDOMSupport&     theSourceDOMSupport,
            MemoryManager&                  theManager);

    ~XalanDefaultParsedSourceHelper();

    virtual DOMSupport&
    getDOMSupport();

    virtual XMLParserLiaison&
    getParserLiaison();

private:

    // Not implemented...
    XalanDefaultParsedSourceHelper(const XalanDefaultParsedSourceHelper&);

    XalanDefaultParsedSourceHelper&
    operator=(const XalanDefaultParsedSourceHelper&);


    // Data members...
    XalanSourceTreeParserLiaison        m_parserLiaison;

    XalanDefaultParsedSourceDOMSupport  m_domSupport;
};



/**
 * This is designed to allow a XalanTranfomer object to reuse a parsed
 * document. 
 */
class XALAN_TRANSFORMER_EXPORT XalanDefaultParsedSource : public XalanParsedSource
{
public:

    XalanDefaultParsedSource(
            const InputSource&      theInputSource,
            bool                    fValidate = false,
            ErrorHandler*           theErrorHandler = 0,
            EntityResolver*         theEntityResolver = 0,
            XMLEntityResolver*      theXMLEntityResolver = 0,
            const XalanDOMChar*     theExternalSchemaLocation = 0,
            const XalanDOMChar*     theExternalNoNamespaceSchemaLocation = 0,
            bool                    fPoolAllTextNodes = XalanSourceTreeDocument::getPoolAllTextNodes(),
            MemoryManager&          theManager XALAN_DEFAULT_MEMMGR);

    static XalanDefaultParsedSource*
    create(
            MemoryManager&          theManager,
            const InputSource&      theInputSource,
            bool                    fValidate = false,
            ErrorHandler*           theErrorHandler = 0,
            EntityResolver*         theEntityResolver = 0,
            XMLEntityResolver*      theXMLEntityResolver = 0,
            const XalanDOMChar*     theExternalSchemaLocation = 0,
            const XalanDOMChar*     theExternalNoNamespaceSchemaLocation = 0,
            bool                    fPoolAllTextNodes = XalanSourceTreeDocument::getPoolAllTextNodes());

    virtual
    ~XalanDefaultParsedSource();

    virtual XalanDocument*
    getDocument() const;

    virtual XalanParsedSourceHelper*
    createHelper(MemoryManager&     theManager) const;

    virtual const XalanDOMString&
    getURI() const;

private:

    // Not implemented...
    XalanDefaultParsedSource(const XalanDefaultParsedSource&);

    XalanDefaultParsedSource&
    operator=(const XalanDefaultParsedSource&);


    // Data members...
    XalanSourceTreeParserLiaison    m_parserLiaison;

    XalanSourceTreeDOMSupport       m_domSupport;

    XalanSourceTreeDocument*        m_parsedSource;

    XalanDOMString                  m_uri;
};



XALAN_CPP_NAMESPACE_END



#endif  // XALANDEFAULTPARSEDSOURCE_HEADER_GUARD



