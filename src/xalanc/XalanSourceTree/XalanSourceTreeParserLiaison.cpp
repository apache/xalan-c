/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2004 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
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
			XalanSourceTreeDOMSupport&	/* theSupport */) :
	m_xercesParserLiaison(),
	m_documentMap(),
	m_poolAllText(true)
{
}



XalanSourceTreeParserLiaison::XalanSourceTreeParserLiaison() :
	m_xercesParserLiaison(),
	m_documentMap(),
	m_poolAllText(true)
{
}



XalanSourceTreeParserLiaison::~XalanSourceTreeParserLiaison()
{
	reset();
}



void
XalanSourceTreeParserLiaison::reset()
{
	XALAN_USING_STD(for_each)

	// Delete any documents.
	for_each(
			m_documentMap.begin(),
			m_documentMap.end(),
			makeMapValueDeleteFunctor(m_documentMap));

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



XalanDocument*
XalanSourceTreeParserLiaison::parseXMLStream(
			const InputSourceType&	inputSource,
			const XalanDOMString&	/* identifier */)
{
	XalanSourceTreeContentHandler	theContentHandler(createXalanSourceTreeDocument());

	XalanAutoPtr<SAX2XMLReaderType>	theReader(createReader());

	theReader->setContentHandler(&theContentHandler);

	theReader->setDTDHandler(&theContentHandler);

	theReader->setLexicalHandler(&theContentHandler);

	theReader->parse(inputSource);

	return theContentHandler.getDocument();
}



XalanDocument*
XalanSourceTreeParserLiaison::createDocument()
{
	return createXalanSourceTreeDocument();
}



XalanDocument*
XalanSourceTreeParserLiaison::createDOMFactory()
{
	return m_xercesParserLiaison.createDocument();
}



void
XalanSourceTreeParserLiaison::destroyDocument(XalanDocument*	theDocument)
{
	if (mapDocument(theDocument) != 0)
	{
		m_documentMap.erase(theDocument);

		delete theDocument;
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



const XalanDOMString
XalanSourceTreeParserLiaison::getParserDescription() const
{
	return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("XalanSourceTree"));
}



void
XalanSourceTreeParserLiaison::parseXMLStream(
			const InputSourceType&	theInputSource,
			ContentHandlerType&		theContentHandler,
			DTDHandlerType*			theDTDHandler,
			LexicalHandlerType*		theLexicalHandler,
			const XalanDOMString&	/* theIdentifier */)
{
	XalanAutoPtr<SAX2XMLReaderType>		theReader(createReader());

	theReader->setContentHandler(&theContentHandler);

	theReader->setDTDHandler(theDTDHandler);

	theReader->setLexicalHandler(theLexicalHandler);

	theReader->parse(theInputSource);
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
		new XalanSourceTreeDocument(m_poolAllText);

	m_documentMap[theNewDocument] = theNewDocument;

	return theNewDocument;
}



SAX2XMLReaderType*
XalanSourceTreeParserLiaison::createReader()
{
	XalanAutoPtr<SAX2XMLReaderType>		theReader(XERCES_CPP_NAMESPACE_QUALIFIER XMLReaderFactory::createXMLReader());

	const bool	fValidate = m_xercesParserLiaison.getUseValidation();

	if (fValidate == false)
	{
		theReader->setFeature(
			s_validationString,
			false);

		theReader->setFeature(
			s_schemaString,
			false);
	}
	else
	{
		theReader->setFeature(
			s_dynamicValidationString,
			true);

		theReader->setFeature(
			s_schemaString,
			true);
	}

	theReader->setFeature(
		s_namespacesString,
		true);

	theReader->setFeature(
		s_namespacePrefixesString,
		true);

	ErrorHandlerType* const		theHandler = getErrorHandler();

	if (theHandler == 0)
	{
		theReader->setErrorHandler(&m_xercesParserLiaison);
	}
	else
	{
		theReader->setErrorHandler(theHandler);
	}

	theReader->setEntityResolver(getEntityResolver());

	{
		const XalanDOMChar* const	theLocation =
			getExternalSchemaLocation();

		if (theLocation != 0)
		{
			theReader->setProperty(
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
			theReader->setProperty(
				s_externalNoNamespaceSchemaLocationString,
#if defined(XALAN_OLD_STYLE_CASTS)
				(void*)theLocation);
#else
				const_cast<XalanDOMChar*>(theLocation));
#endif
		}
	}

	return theReader.release();
}



XALAN_CPP_NAMESPACE_END
