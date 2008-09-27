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
#if !defined(XALANSOURCETREEWRAPPERPARSEDSOURCE_HEADER_GUARD)
#define XALANSOURCETREEWRAPPERPARSEDSOURCE_HEADER_GUARD



// Base include file.  Must be first.
#include <xalanc/XalanTransformer/XalanTransformerDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/XalanTransformer/XalanParsedSource.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XalanSourceTreeDOMSupport;
class XalanSourceTreeDocument;
class XalanSourceTreeParserLiaison;



/**
 * This is designed to allow a XalanTranfomer object to wrap a parsed
 * Xerces document. 
 */
class XALAN_TRANSFORMER_EXPORT XalanSourceTreeWrapperParsedSource : public XalanParsedSource
{
    
public:

    XalanSourceTreeWrapperParsedSource(
            XalanSourceTreeDocument*        theDocument,
            XalanSourceTreeParserLiaison&   theParserLiaison,
            XalanSourceTreeDOMSupport&      theDOMSupport,
            const XalanDOMString&           theURI = XalanDOMString(XalanMemMgrs::getDummyMemMgr()),
            MemoryManager&              theManager XALAN_DEFAULT_MEMMGR);

    virtual
    ~XalanSourceTreeWrapperParsedSource();

    virtual XalanDocument*
    getDocument() const;

    virtual XalanParsedSourceHelper*
    createHelper(MemoryManager& theManager XALAN_DEFAULT_MEMMGR) const;

    virtual const XalanDOMString&
    getURI() const;

private:
    //not implemented
    XalanSourceTreeWrapperParsedSource();
    XalanSourceTreeWrapperParsedSource(const XalanSourceTreeWrapperParsedSource&);

    XalanSourceTreeParserLiaison&   m_parserLiaison;

    XalanSourceTreeDOMSupport&      m_domSupport;

    XalanSourceTreeDocument* const  m_parsedSource;

    const XalanDOMString            m_uri;
};



XALAN_CPP_NAMESPACE_END



#endif  // XALANSOURCETREEWRAPPERPARSEDSOURCE_HEADER_GUARD
