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

	XercesDOMParsedSourceHelper(MemoryManagerType& theManager XALAN_DEFAULT_CONSTRACTOR_MEMORY_MGR);

	~XercesDOMParsedSourceHelper();

    static XercesDOMParsedSourceHelper*
    create(MemoryManagerType& theManager);

	virtual DOMSupport&
	getDOMSupport();

	virtual XMLParserLiaison&
	getParserLiaison();

private:

	XercesDOMSupport		m_domSupport;

	XercesParserLiaison		m_parserLiaison;
};



/**
 * This is designed to allow a XalanTranfomer object to reuse a parsed
 * document. 
 */
class XALAN_TRANSFORMER_EXPORT XercesDOMParsedSource : public XalanParsedSource
{
public:
	
	XercesDOMParsedSource(
			const InputSourceType&	theInputSource,
			bool					fValidate = false,
			ErrorHandlerType*		theErrorHandler = 0,
			EntityResolverType*		theEntityResolver = 0,
			const XalanDOMChar*		theExternalSchemaLocation = 0,
			const XalanDOMChar*		theExternalNoNamespaceSchemaLocation = 0,
            MemoryManagerType&      theManager XALAN_DEFAULT_MEMMGR);

	static XercesDOMParsedSource*
    create(
            MemoryManagerType&      theManager,
			const InputSourceType&	theInputSource,
			bool					fValidate = false,
			ErrorHandlerType*		theErrorHandler = 0,
			EntityResolverType*		theEntityResolver = 0,
			const XalanDOMChar*		theExternalSchemaLocation = 0,
			const XalanDOMChar*		theExternalNoNamespaceSchemaLocation = 0);

	virtual
	~XercesDOMParsedSource();

	virtual XalanDocument*
	getDocument() const;

	virtual XalanParsedSourceHelper*
	createHelper(MemoryManagerType& theManager XALAN_DEFAULT_CONSTRACTOR_MEMORY_MGR) const;

	virtual const XalanDOMString&
	getURI() const;

private:

	XercesParserLiaison		m_parserLiaison;

	XalanDocument*			m_parsedSource;

	XalanDOMString			m_uri;
};



XALAN_CPP_NAMESPACE_END



#endif	// XERCESDOMPARSEDSOURCE_HEADER_GUARD
