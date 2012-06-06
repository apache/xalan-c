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
#if !defined(XERCESDOMWRAPPERPARSEDSOURCE_HEADER_GUARD)
#define XERCESDOMWRAPPERPARSEDSOURCE_HEADER_GUARD



// Base include file.  Must be first.
#include <xalanc/XalanTransformer/XalanTransformerDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/XalanTransformer/XalanParsedSource.hpp>



XALAN_DECLARE_XERCES_CLASS(DOM_Document)
XALAN_DECLARE_XERCES_CLASS(DOMDocument)



XALAN_CPP_NAMESPACE_BEGIN



typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_Document DOM_Document_Type;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument  DOMDocument_Type;



class XercesParserLiaison;
class XercesDOMSupport;



/**
 * This is designed to allow a XalanTranfomer object to wrap a parsed
 * Xerces document. 
 */
class XALAN_TRANSFORMER_EXPORT XercesDOMWrapperParsedSource : public XalanParsedSource
{
    
public:

#if defined(XALAN_BUILD_DEPRECATED_DOM_BRIDGE)
    /**
     * Constructor
     *
     * @param theDocument The Xerces document to wrap
     * @param theParserLiaison The XercesParserLiaison instance that will own the document.
     * @param theDOMSupport The XercesDOMSupport instance
     * @param theURI The URI of the document, if any.
     *
     * @deprecated This constructor is deprecated.
     */
    XercesDOMWrapperParsedSource(
            const DOM_Document_Type&    theDocument,
            XercesParserLiaison&        theParserLiaison,
            XercesDOMSupport&           theDOMSupport,
            const XalanDOMString&       theURI = XalanDOMString(XalanMemMgrs::getDummyMemMgr())
            MemoryManager&          theManager XALAN_DEFAULT_MEMMGR);
#endif

    /**
     * Constructor
     *
     * @param theDocument The Xerces document to wrap
     * @param theParserLiaison The XercesParserLiaison instance that will own the document.
     * @param theDOMSupport The XercesDOMSupport instance
     * @param theURI The URI of the document, if any.
     */
    XercesDOMWrapperParsedSource(
            const DOMDocument_Type*     theDocument,
            XercesParserLiaison&        theParserLiaison,
            XercesDOMSupport&           theDOMSupport,
            const XalanDOMString&       theURI = XalanDOMString(XalanMemMgrs::getDummyMemMgr()),
            MemoryManager&          theManager XALAN_DEFAULT_MEMMGR);

    virtual
    ~XercesDOMWrapperParsedSource();

    virtual XalanDocument*
    getDocument() const;

    virtual XalanParsedSourceHelper*
    createHelper(MemoryManager& theManager XALAN_DEFAULT_MEMMGR) const;

    virtual const XalanDOMString&
    getURI() const;

private:
    //Not implemented
    XercesDOMWrapperParsedSource();
    XercesDOMWrapperParsedSource(const XercesDOMWrapperParsedSource&);

    XercesParserLiaison&    m_parserLiaison;

    XercesDOMSupport&       m_domSupport;

    XalanDocument* const    m_parsedSource;

    const XalanDOMString    m_uri;
};



XALAN_CPP_NAMESPACE_END



#endif  // XERCESDOMWRAPPERPARSEDSOURCE_HEADER_GUARD
