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
// Class header file.
#include "XalanSourceTreeParserLiaison.hpp"



#include <algorithm>



#include <xercesc/sax2/XMLReaderFactory.hpp>



#include <xalanc/Include/XalanAutoPtr.hpp>
#include <xalanc/Include/STLHelper.hpp>



#include <xalanc/PlatformSupport/XalanUnicode.hpp>



#include "XalanSourceTreeContentHandler.hpp"
#include "XalanSourceTreeDOMSupport.hpp"
#include "XalanSourceTreeDocument.hpp"



XALAN_CPP_NAMESPACE_BEGIN



// http://xml.org/sax/features/validation
const XalanDOMChar	XalanSourceTreeParserLiaison::s_validationString[] = {
	XalanUnicode::charLetter_h,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_p,
	XalanUnicode::charColon,
	XalanUnicode::charSolidus,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_x,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_l,
	XalanUnicode::charFullStop,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_g,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_x,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_f,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_s,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_v,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_d,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_n,
	0
};



// http://apache.org/xml/features/validation/dynamic
const XalanDOMChar	XalanSourceTreeParserLiaison::s_dynamicValidationString[] = {
	XalanUnicode::charLetter_h,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_p,
	XalanUnicode::charColon,
	XalanUnicode::charSolidus,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_h,
	XalanUnicode::charLetter_e,
	XalanUnicode::charFullStop,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_g,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_x,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_l,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_f,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_s,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_v,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_d,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_n,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_d,
	XalanUnicode::charLetter_y,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_c,
	0
};



// http://xml.org/sax/features/namespaces
const XalanDOMChar	XalanSourceTreeParserLiaison::s_namespacesString[] = {
	XalanUnicode::charLetter_h,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_p,
	XalanUnicode::charColon,
	XalanUnicode::charSolidus,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_x,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_l,
	XalanUnicode::charFullStop,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_g,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_x,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_f,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_s,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_s,
	0
};



// http://xml.org/sax/features/namespace-prefixes
const XalanDOMChar	XalanSourceTreeParserLiaison::s_namespacePrefixesString[] = {
	XalanUnicode::charLetter_h,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_p,
	XalanUnicode::charColon,
	XalanUnicode::charSolidus,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_x,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_l,
	XalanUnicode::charFullStop,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_g,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_x,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_f,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_s,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_e,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_f,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_x,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_s,
	0
};


// http://apache.org/xml/features/validation/schema
const XalanDOMChar	XalanSourceTreeParserLiaison::s_schemaString[] =
{
	XalanUnicode::charLetter_h,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_p,
	XalanUnicode::charColon,
	XalanUnicode::charSolidus,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_h,
	XalanUnicode::charLetter_e,
	XalanUnicode::charFullStop,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_g,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_x,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_l,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_f,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_u,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_s,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_v,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_d,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_n,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_h,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_a,
	0
};



// http://apache.org/xml/properties/schema/external-schemaLocation
const XalanDOMChar	XalanSourceTreeParserLiaison::s_externalSchemaLocationString[] =
{
	XalanUnicode::charLetter_h,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_p,
	XalanUnicode::charColon,
	XalanUnicode::charSolidus,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_h,
	XalanUnicode::charLetter_e,
	XalanUnicode::charFullStop,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_g,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_x,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_l,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_s,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_h,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_a,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_x,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_l,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_h,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_L,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_n,
	0
};



// http://apache.org/xml/properties/schema/external-noNamespaceSchemaLocation
const XalanDOMChar	XalanSourceTreeParserLiaison::s_externalNoNamespaceSchemaLocationString[] =
{
	XalanUnicode::charLetter_h,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_p,
	XalanUnicode::charColon,
	XalanUnicode::charSolidus,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_h,
	XalanUnicode::charLetter_e,
	XalanUnicode::charFullStop,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_g,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_x,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_l,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_s,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_h,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_a,
	XalanUnicode::charSolidus,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_x,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_r,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_l,
	XalanUnicode::charHyphenMinus,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_N,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_s,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_S,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_h,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_m,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_L,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_n,
	0
};



XalanSourceTreeParserLiaison::XalanSourceTreeParserLiaison(
			XalanSourceTreeDOMSupport&	/* theSupport */,
            MemoryManagerType&          theManager) :
	m_xercesParserLiaison(theManager),
	m_documentMap(theManager),
	m_poolAllText(true),
	m_xmlReader(0)
{
}



XalanSourceTreeParserLiaison::XalanSourceTreeParserLiaison(MemoryManagerType&   theManager) :
	m_xercesParserLiaison(theManager),
	m_documentMap(theManager),
	m_poolAllText(true),
	m_xmlReader(0)
{
}



XalanSourceTreeParserLiaison::~XalanSourceTreeParserLiaison()
{
	reset();

	delete m_xmlReader;
}



void
XalanSourceTreeParserLiaison::reset()
{
	XALAN_USING_STD(for_each)

    typedef XalanSourceTreeParserLiaison::DocumentMapType::iterator Iterator;

    MemoryManagerType& theManager = getMemoryManager();

    for ( Iterator i = m_documentMap.begin(); i != m_documentMap.end(); ++i)
    {
        assert( (*i).second != 0);

        (*i).second->~XalanSourceTreeDocument();

        theManager.deallocate((*i).second);

        (*i).second = 0;

    }

    m_documentMap.clear();

	m_xercesParserLiaison.reset();
}



ExecutionContext*
XalanSourceTreeParserLiaison::getExecutionContext() const
{
	return m_xercesParserLiaison.getExecutionContext();
}



void
XalanSourceTreeParserLiaison::setExecutionContext(ExecutionContext&		theContext)
{
	m_xercesParserLiaison.setExecutionContext(theContext);
}



void
XalanSourceTreeParserLiaison::parseXMLStream(
			const InputSourceType&	inputSource,
			DocumentHandlerType&	handler,
			const XalanDOMString&	identifier)
{
	m_xercesParserLiaison.parseXMLStream(inputSource, handler, identifier);
}



void
XalanSourceTreeParserLiaison::ensureReader()
{
	if (m_xmlReader == 0)
	{
		m_xmlReader = createReader();
	}

	const bool	fValidate = m_xercesParserLiaison.getUseValidation();

	if (fValidate == false)
	{
		m_xmlReader->setFeature(
			s_validationString,
			false);

		m_xmlReader->setFeature(
			s_schemaString,
			false);
	}
	else
	{
		m_xmlReader->setFeature(
			s_dynamicValidationString,
			true);

		m_xmlReader->setFeature(
			s_schemaString,
			true);
	}

	ErrorHandlerType* const		theHandler = getErrorHandler();

	if (theHandler == 0)
	{
		m_xmlReader->setErrorHandler(&m_xercesParserLiaison);
	}
	else
	{
		m_xmlReader->setErrorHandler(theHandler);
	}

	m_xmlReader->setEntityResolver(getEntityResolver());

	{
		const XalanDOMChar* const	theLocation =
			getExternalSchemaLocation();

		if (theLocation != 0)
		{
			m_xmlReader->setProperty(
				s_externalSchemaLocationString,
#if defined(XALAN_OLD_STYLE_CASTS)
				(void*)theLocation);
#else
				const_cast<XalanDOMChar*>(theLocation));
#endif
		}
	}

	{
		const XalanDOMChar* const	theLocation =
			getExternalNoNamespaceSchemaLocation();

		if (theLocation != 0)
		{
			m_xmlReader->setProperty(
				s_externalNoNamespaceSchemaLocationString,
#if defined(XALAN_OLD_STYLE_CASTS)
				(void*)theLocation);
#else
				const_cast<XalanDOMChar*>(theLocation));
#endif
		}
	}
}



XalanDocument*
XalanSourceTreeParserLiaison::parseXMLStream(
			const InputSourceType&	inputSource,
			const XalanDOMString&	identifier)
{
	XalanSourceTreeContentHandler	theContentHandler( getMemoryManager(), createXalanSourceTreeDocument());

	parseXMLStream(
		inputSource,
		theContentHandler,
		identifier,
		&theContentHandler,
		&theContentHandler);

	return theContentHandler.getDocument();
}



void
XalanSourceTreeParserLiaison::destroyDocument(XalanDocument*	theDocument)
{
	if (mapDocument(theDocument) != 0)
	{
		m_documentMap.erase(theDocument);

        if( theDocument!= 0 )
        {
            theDocument->~XalanDocument();
            
            MemoryManagerType& theManager = m_documentMap.getMemoryManager();

            theManager.deallocate(theDocument);

        }
		
	}
	else
	{
		m_xercesParserLiaison.destroyDocument(theDocument);
	}
}



int
XalanSourceTreeParserLiaison::getIndent() const
{
	return m_xercesParserLiaison.getIndent();
}



void
XalanSourceTreeParserLiaison::setIndent(int		i)
{
	m_xercesParserLiaison.setIndent(i);
}



bool
XalanSourceTreeParserLiaison::getUseValidation() const
{
	return m_xercesParserLiaison.getUseValidation();
}



void
XalanSourceTreeParserLiaison::setUseValidation(bool		b)
{
	m_xercesParserLiaison.setUseValidation(b);
}



const XalanDOMString&
XalanSourceTreeParserLiaison::getParserDescription(XalanDOMString& theResult) const
{
    theResult.assign("XalanSourceTree");
    
    return theResult;
}



void
XalanSourceTreeParserLiaison::parseXMLStream(
			const InputSourceType&	theInputSource,
			ContentHandlerType&		theContentHandler,
            const XalanDOMString&	/* theIdentifier */,
			DTDHandlerType*			theDTDHandler,
			LexicalHandlerType*		theLexicalHandler)
{
	ensureReader();

	assert(m_xmlReader != 0);

	m_xmlReader->setContentHandler(&theContentHandler);

	m_xmlReader->setDTDHandler(theDTDHandler);

	m_xmlReader->setLexicalHandler(theLexicalHandler);

	m_xmlReader->parse(theInputSource);
}



DOMDocument_Type*
XalanSourceTreeParserLiaison::createDOMFactory()
{
	return m_xercesParserLiaison.createDOMFactory();
}



void
XalanSourceTreeParserLiaison::destroyDocument(DOMDocument_Type*     theDocument)
{
	m_xercesParserLiaison.destroyDocument(theDocument);
}



bool
XalanSourceTreeParserLiaison::getIncludeIgnorableWhitespace() const
{
	return m_xercesParserLiaison.getIncludeIgnorableWhitespace();
}



void
XalanSourceTreeParserLiaison::setIncludeIgnorableWhitespace(bool	include)
{
	m_xercesParserLiaison.setIncludeIgnorableWhitespace(include);
}



ErrorHandlerType*
XalanSourceTreeParserLiaison::getErrorHandler() const
{
	return m_xercesParserLiaison.getErrorHandler();
}



void
XalanSourceTreeParserLiaison::setErrorHandler(ErrorHandlerType*		handler)
{
	m_xercesParserLiaison.setErrorHandler(handler);
}



bool
XalanSourceTreeParserLiaison::getDoNamespaces() const
{
	return m_xercesParserLiaison.getDoNamespaces();
}



void
XalanSourceTreeParserLiaison::setDoNamespaces(bool	newState)
{
	m_xercesParserLiaison.setDoNamespaces(newState);
}



bool
XalanSourceTreeParserLiaison::getExitOnFirstFatalError() const
{
	return m_xercesParserLiaison.getExitOnFirstFatalError();
}



void
XalanSourceTreeParserLiaison::setExitOnFirstFatalError(bool		newState)
{
	m_xercesParserLiaison.setExitOnFirstFatalError(newState);
}



EntityResolverType*
XalanSourceTreeParserLiaison::getEntityResolver() const
{
	return m_xercesParserLiaison.getEntityResolver();
}



void
XalanSourceTreeParserLiaison::setEntityResolver(EntityResolverType*		resolver)
{
	m_xercesParserLiaison.setEntityResolver(resolver);
}



const XalanDOMChar*
XalanSourceTreeParserLiaison::getExternalSchemaLocation() const
{
	return m_xercesParserLiaison.getExternalSchemaLocation();
}



void
XalanSourceTreeParserLiaison::setExternalSchemaLocation(const XalanDOMChar*		location)
{
	m_xercesParserLiaison.setExternalSchemaLocation(location);
}



const XalanDOMChar*
XalanSourceTreeParserLiaison::getExternalNoNamespaceSchemaLocation() const
{
	return m_xercesParserLiaison.getExternalNoNamespaceSchemaLocation();
}



void
XalanSourceTreeParserLiaison::setExternalNoNamespaceSchemaLocation(const XalanDOMChar*	location)
{
	m_xercesParserLiaison.setExternalNoNamespaceSchemaLocation(location);
}



XalanSourceTreeDocument*
XalanSourceTreeParserLiaison::mapDocument(const XalanDocument*	theDocument) const
{
	DocumentMapType::const_iterator		i =
		m_documentMap.find(theDocument);

	return i != m_documentMap.end() ? (*i).second : 0;
}



XalanSourceTreeDocument*
XalanSourceTreeParserLiaison::createXalanSourceTreeDocument()
{
	XalanSourceTreeDocument* const	theNewDocument =
        XalanSourceTreeDocument::create( getMemoryManager(), m_poolAllText);

	m_documentMap[theNewDocument] = theNewDocument;

	return theNewDocument;
}



SAX2XMLReaderType*
XalanSourceTreeParserLiaison::createReader()
{
	XalanAutoPtr<SAX2XMLReaderType>		theReader(XERCES_CPP_NAMESPACE_QUALIFIER XMLReaderFactory::createXMLReader( &(getMemoryManager())));

	theReader->setFeature(
		s_namespacesString,
		true);

	theReader->setFeature(
		s_namespacePrefixesString,
		true);

	return theReader.release();
}



XALAN_CPP_NAMESPACE_END
