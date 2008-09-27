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
#include "XalanCompiledStylesheetDefault.hpp"



#include <xalanc/XMLSupport/XMLParserLiaison.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class SetAndRestoreHandlers
{
public:

    SetAndRestoreHandlers(
            XMLParserLiaison&   theParserLiaison,
            ErrorHandler*       theErrorHandler,
            EntityResolver*     theEntityResolver) :
        m_parserLiaison(theParserLiaison),
        m_errorHandler(theParserLiaison.getErrorHandler()),
        m_entityResolver(theParserLiaison.getEntityResolver())
    {
        if (theErrorHandler != 0)
        {
            theParserLiaison.setErrorHandler(theErrorHandler);
        }

        if (theEntityResolver != 0)
        {
            theParserLiaison.setEntityResolver(theEntityResolver);
        }
    }

    ~SetAndRestoreHandlers()
    {
        m_parserLiaison.setEntityResolver(m_entityResolver);

        m_parserLiaison.setErrorHandler(m_errorHandler);
    }

private:

    XMLParserLiaison&       m_parserLiaison;

    ErrorHandler* const     m_errorHandler;

    EntityResolver* const   m_entityResolver;
};




inline const StylesheetRoot*
compileStylesheet(
            const XSLTInputSource&                  theStylesheetSource,
            XSLTEngineImpl&                         theProcessor,
            StylesheetConstructionContextDefault&   theConstructionContext,
            ErrorHandler*                           theErrorHandler,
            EntityResolver*                         theEntityResolver)
{
    const SetAndRestoreHandlers     theSetAndRestore(
            theProcessor.getXMLParserLiaison(),
            theErrorHandler,
            theEntityResolver);

    return theProcessor.processStylesheet(theStylesheetSource, theConstructionContext);
}



XalanCompiledStylesheetDefault::XalanCompiledStylesheetDefault(
            MemoryManager&          theManager,
            const XSLTInputSource&  theStylesheetSource,
            XSLTEngineImpl&         theProcessor,
            ErrorHandler*           theErrorHandler,
            EntityResolver*         theEntityResolver):
    XalanCompiledStylesheet(),
    m_stylesheetXPathFactory(theManager),
    m_stylesheetConstructionContext(
        theManager,
        theProcessor,
        m_stylesheetXPathFactory),
    m_stylesheetRoot(
        compileStylesheet(
            theStylesheetSource,
            theProcessor,
            m_stylesheetConstructionContext,
            theErrorHandler,
            theEntityResolver))
{
}

XalanCompiledStylesheetDefault*
XalanCompiledStylesheetDefault::create(
            MemoryManager&          theManager,
            const XSLTInputSource&  theStylesheetSource,
            XSLTEngineImpl&         theProcessor,
            ErrorHandler*           theErrorHandler,
            EntityResolver*         theEntityResolver)
{
    typedef XalanCompiledStylesheetDefault ThisType;

    XalanAllocationGuard    theGuard(
                                theManager,
                                theManager.allocate(sizeof(ThisType)));

    ThisType* theResult =
        new (theGuard.get()) ThisType(
                                theManager,
                                theStylesheetSource,
                                theProcessor,
                                theErrorHandler,
                                theEntityResolver);

    theGuard.release();

    return theResult;
}



XalanCompiledStylesheetDefault::~XalanCompiledStylesheetDefault()
{
}



const StylesheetRoot*
XalanCompiledStylesheetDefault::getStylesheetRoot() const
{
    return m_stylesheetRoot;
}



XALAN_CPP_NAMESPACE_END
