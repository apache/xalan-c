/*
 * Copyright 1999-2004 The Apache Software Foundation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
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



XALAN_DECLARE_XERCES_CLASS(ContentHandler)
XALAN_DECLARE_XERCES_CLASS(DTDHandler)
XALAN_DECLARE_XERCES_CLASS(LexicalHandler)



XALAN_CPP_NAMESPACE_BEGIN



typedef XERCES_CPP_NAMESPACE_QUALIFIER ContentHandler		ContentHandlerType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DTDHandler			DTDHandlerType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER LexicalHandler		LexicalHandlerType;



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
	createHelper(MemoryManagerType& theManager XALAN_DEFAULT_CONSTRACTOR_MEMORY_MGR) const = 0;

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



XALAN_CPP_NAMESPACE_END



#endif	// XALANDOCUMENTBUILDER_HEADER_GUARD
