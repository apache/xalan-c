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
#if !defined(XALANDOCUMENTBUILDER_HEADER_GUARD)
#define XALANDOCUMENTBUILDER_HEADER_GUARD



// Base include file.  Must be first.
#include <xalanc/XalanTransformer/XalanTransformerDefinitions.hpp>


#include <xalanc/Include/XalanMemoryManagement.hpp>


// Base class include file.
#include <xalanc/XalanTransformer/XalanParsedSource.hpp>



namespace XERCES_CPP_NAMESPACE
{
    class ContentHandler;
    class DTDHandler;
    class LexicalHandler;
}



namespace XALAN_CPP_NAMESPACE {



typedef xercesc::ContentHandler       ContentHandlerType;
typedef xercesc::DTDHandler           DTDHandlerType;
typedef xercesc::LexicalHandler       LexicalHandlerType;



class DOMSupport;
class XalanDocument;



/**
 * This is abstract base class designed to allow a XalanTranfomer 
 * object to use a document that is build dynamically by a user.
 */
class XALAN_TRANSFORMER_EXPORT XalanDocumentBuilder : public XalanParsedSource
{
public:

    virtual
    ~XalanDocumentBuilder()
    {
    }

    // These are inherited from XalanParsedSource...
    virtual XalanDocument*
    getDocument() const = 0;    

    virtual XalanParsedSourceHelper*
    createHelper(MemoryManager& theManager XALAN_DEFAULT_CONSTRUCTOR_MEMMGR) const = 0;

    virtual const XalanDOMString&
    getURI() const = 0;

    // These are new to XalanDocumentBuilder...
    virtual ContentHandlerType*
    getContentHandler() = 0;

    virtual DTDHandlerType*
    getDTDHandler() = 0;

    virtual LexicalHandlerType*
    getLexicalHandler() = 0;

private:
};



}



#endif  // XALANDOCUMENTBUILDER_HEADER_GUARD
