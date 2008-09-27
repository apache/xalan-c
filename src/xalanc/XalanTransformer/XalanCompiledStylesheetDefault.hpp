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
#if !defined(XALANCOMPILEDSTYLESHEETDEFAULT_HEADER_GUARD)
#define XALANCOMPILEDSTYLESHEETDEFAULT_HEADER_GUARD



// Base include file.  Must be first.
#include <xalanc/XalanTransformer/XalanTransformerDefinitions.hpp>



#include <xalanc/XPath/XPathFactoryBlock.hpp>



#include <xalanc/XSLT/StylesheetConstructionContextDefault.hpp>
#include <xalanc/XSLT/StylesheetExecutionContextDefault.hpp>
#include <xalanc/XSLT/StylesheetRoot.hpp>
#include <xalanc/XSLT/XSLTEngineImpl.hpp>
#include <xalanc/XSLT/XSLTInputSource.hpp>
#include <xalanc/XSLT/XSLTProcessorEnvSupportDefault.hpp>



#include <xalanc/XalanTransformer/XalanCompiledStylesheet.hpp>



XALAN_DECLARE_XERCES_CLASS(EntityResolver)
XALAN_DECLARE_XERCES_CLASS(ErrorHandler)



XALAN_CPP_NAMESPACE_BEGIN



typedef XERCES_CPP_NAMESPACE_QUALIFIER EntityResolver   EntityResolverType;
typedef XERCES_CPP_NAMESPACE_QUALIFIER ErrorHandler     ErrorHandlerType;

XALAN_USING_XERCES(EntityResolver)
XALAN_USING_XERCES(ErrorHandler)


class XALAN_TRANSFORMER_EXPORT XalanCompiledStylesheetDefault : public XalanCompiledStylesheet
{
public:
    
    XalanCompiledStylesheetDefault(
            MemoryManager&          theManager,
            const XSLTInputSource&  theStylesheetSource,
            XSLTEngineImpl&         theProcessor,
            ErrorHandler*           theErrorHandler = 0,
            EntityResolver*         theEntityResolver = 0);

    
    static XalanCompiledStylesheetDefault*
    create(
            MemoryManager&          theManager,
            const XSLTInputSource&  theStylesheetSource,
            XSLTEngineImpl&         theProcessor,
            ErrorHandler*           theErrorHandler = 0,
            EntityResolver*         theEntityResolver = 0);

    virtual
    ~XalanCompiledStylesheetDefault();

    virtual const StylesheetRoot*
    getStylesheetRoot() const;

private:

    XPathFactoryBlock                       m_stylesheetXPathFactory;

    StylesheetConstructionContextDefault    m_stylesheetConstructionContext;

    const StylesheetRoot* const             m_stylesheetRoot;
};



XALAN_CPP_NAMESPACE_END



#endif  // XALANCOMPILEDSTYLESHEETDEFAULT_HEADER_GUARD
