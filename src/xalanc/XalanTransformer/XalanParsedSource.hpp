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
#if !defined(XALANPARSEDSOURCE_HEADER_GUARD)
#define XALANPARSEDSOURCE_HEADER_GUARD



// Base include file.  Must be first.
#include <xalanc/XalanTransformer/XalanTransformerDefinitions.hpp>


#include <xalanc/Include/XalanMemoryManagement.hpp>


// We're bringing in XalanDocument so that its derivation from XalanNode
// is known...
#include <xalanc/XalanDOM/XalanDocument.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class DOMSupport;
class XMLParserLiaison;



class XALAN_TRANSFORMER_EXPORT XalanParsedSourceHelper
{
public:

	virtual
	~XalanParsedSourceHelper()
	{
	}

	/**
	 * Get the DOMSupport instance.
	 *
	 * @return A reference to a DOMSupport instance.
	 */
	virtual DOMSupport&
	getDOMSupport() = 0;

	/**
	 * Get the XMLParserLiaison instance.
	 *
	 * @return A rerefernce to an XMLParserLiaison instance.
	 */
	virtual XMLParserLiaison&
	getParserLiaison() = 0;
};



/**
 * This is abstract base class designed to allow a XalanTranfomer 
 * object to reuse a parsed document. 
 */
class XALAN_TRANSFORMER_EXPORT XalanParsedSource
{
public:

	XalanParsedSource();

	virtual
	~XalanParsedSource();

	/**
	 * Get a pointer to the XalanDocument instance for the source
	 * document
	 *
	 * @return A pointer to a XalanDocument instance.
	 */
	virtual XalanDocument*
	getDocument() const = 0;

	/**
	 * Create the appropriate XalanParsedSourceHelper instance to
	 * use for transforming with the instance.
	 *
	 * The caller is responsible for deleting the object when finished.
	 *
	 * @return A pointer to a XalanParsedSourceHelper instance.
	 */
	virtual XalanParsedSourceHelper*
	createHelper(MemoryManagerType& theManager XALAN_DEFAULT_CONSTRACTOR_MEMORY_MGR ) const = 0;

	/**
	 * Get the URI for the parsed source, if any.
	 * use for transforming with the instance.
	 *
	 * @return A const reference to a string containing the URI
	 */
	virtual const XalanDOMString&
	getURI() const = 0;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALANPARSEDSOURCE_HEADER_GUARD



