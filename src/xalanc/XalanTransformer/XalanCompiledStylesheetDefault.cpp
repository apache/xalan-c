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
#include "XalanCompiledStylesheetDefault.hpp"



#include <xalanc/XMLSupport/XMLParserLiaison.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class SetAndRestoreHandlers
{
public:

	SetAndRestoreHandlers(
			XMLParserLiaison&		theParserLiaison,
			ErrorHandlerType*		theErrorHandler,
			EntityResolverType*		theEntityResolver) :
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

	XMLParserLiaison&			m_parserLiaison;

	ErrorHandlerType* const		m_errorHandler;

	EntityResolverType*	const	m_entityResolver;
};




inline const StylesheetRoot*
compileStylesheet(
			const XSLTInputSource&					theStylesheetSource,
			XSLTEngineImpl&							theProcessor,
			StylesheetConstructionContextDefault&	theConstructionContext,
			ErrorHandlerType*						theErrorHandler,
			EntityResolverType*						theEntityResolver)
{
	const SetAndRestoreHandlers		theSetAndRestore(
			theProcessor.getXMLParserLiaison(),
			theErrorHandler,
			theEntityResolver);

	return theProcessor.processStylesheet(theStylesheetSource, theConstructionContext);
}



XalanCompiledStylesheetDefault::XalanCompiledStylesheetDefault(
			const XSLTInputSource&	theStylesheetSource,
			XSLTEngineImpl&			theProcessor,
			ErrorHandlerType*		theErrorHandler,
			EntityResolverType*		theEntityResolver):
	XalanCompiledStylesheet(),
	m_stylesheetXPathFactory(),
	m_stylesheetConstructionContext(
				theProcessor,
				m_stylesheetXPathFactory),
	m_stylesheetRoot(compileStylesheet(
				theStylesheetSource,
				theProcessor,
				m_stylesheetConstructionContext,
				theErrorHandler,
				theEntityResolver))
{
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
