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
#if !defined(XALANCOMPILEDSTYLESHEET_HEADER_GUARD)
#define XALANCOMPILEDSTYLESHEET_HEADER_GUARD



// Base include file.  Must be first.
#include <xalanc/XalanTransformer/XalanTransformerDefinitions.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class StylesheetRoot;



/**
 * This is an abstraction of the StylesheetRoot class. It is designed
 * to allow a XalanTranfomer object to reuse a compliled stylesheet. 
 */
class XALAN_TRANSFORMER_EXPORT XalanCompiledStylesheet
{
public:

	virtual
	~XalanCompiledStylesheet() {}

	virtual const StylesheetRoot*
	getStylesheetRoot() const = 0;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALANCOMPILEDSTYLESHEET_HEADER_GUARD
