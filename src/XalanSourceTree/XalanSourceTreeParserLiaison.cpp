/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2000 The Apache Software Foundation.  All rights 
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



#include <sax2/XMLReaderFactory.hpp>



#include <Include/XalanAutoPtr.hpp>
#include <Include/STLHelper.hpp>



#include <PlatformSupport/XalanUnicode.hpp>



#include "XalanSourceTreeContentHandler.hpp"
#include "XalanSourceTreeDOMSupport.hpp"
#include "XalanSourceTreeDocument.hpp"



XalanSourceTreeParserLiaison::XalanSourceTreeParserLiaison(XalanSourceTreeDOMSupport&	theSupport) :
	m_xercesDOMSupport(),
	m_xercesParserLiaison(m_xercesDOMSupport),
	m_documentMap(),
	m_domSupport(theSupport),
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
#if !defined(XALAN_NO_NAMESPACES)
	using std::for_each;
#endif

	// Delete any live documents.
	for_each(m_documentMap.begin(),
			 m_documentMap.end(),
			 makeMapValueDeleteFunctor(m_documentMap));

	m_documentMap.clear();

	m_domSupport.reset();

	m_xercesParserLiaison.reset();

	m_xercesDOMSupport.reset();
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



bool
XalanSourceTreeParserLiaison::supportsSAX() const
{
	return m_xercesParserLiaison.supportsSAX();
}



void
XalanSourceTreeParserLiaison::parseXMLStream(
			const InputSource&		inputSource,
			DocumentHandler&		handler,
			const XalanDOMString&	identifier)
{
	m_xercesParserLiaison.parseXMLStream(inputSource, handler, identifier);
}



XalanDocument*
XalanSourceTreeParserLiaison::parseXMLStream(
			const InputSource&		inputSource,
			const XalanDOMString&	/* identifier */)
{
	XalanSourceTreeContentHandler	theContentHandler(createXalanSourceTreeDocument());

	XalanAutoPtr<SAX2XMLReader>		theReader(XMLReaderFactory::createXMLReader());

	theReader->setContentHandler(&theContentHandler);

	theReader->setDTDHandler(&theContentHandler);

	theReader->setErrorHandler(&m_xercesParserLiaison);

	theReader->setLexicalHandler(&theContentHandler);

	EntityResolver* const	theResolver = getEntityResolver();

	if (theResolver != 0)
	{
		theReader->setEntityResolver(theResolver);
	}

	theReader->parse(inputSource);

	XalanSourceTreeDocument* const	theDocument = theContentHandler.detachDocument();

	return theDocument;
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
XalanSourceTreeParserLiaison::destroyDocument(XalanDocument*		theDocument)
{
	if (mapDocument(theDocument) != 0)
	{
		m_documentMap.erase(theDocument);

		delete theDocument;
	}
}



void
XalanSourceTreeParserLiaison::setSpecialCharacters(const XalanDOMString&	str)
{
	m_xercesParserLiaison.setSpecialCharacters(str);
}



const XalanDOMString&
XalanSourceTreeParserLiaison::getSpecialCharacters() const
{
	return m_xercesParserLiaison.getSpecialCharacters();
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
XalanSourceTreeParserLiaison::getShouldExpandEntityRefs() const
{
	return m_xercesParserLiaison.getShouldExpandEntityRefs();
}



void
XalanSourceTreeParserLiaison::SetShouldExpandEntityRefs(bool	b)
{
	m_xercesParserLiaison.SetShouldExpandEntityRefs(b);
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
	return XALAN_STATIC_UCODE_STRING("XalanSourceTree");
}



void
XalanSourceTreeParserLiaison::parseXMLStream(
			const InputSource&		theInputSource,
			ContentHandler&			theContentHandler,
			DTDHandler*				theDTDHandler,
			LexicalHandler*			theLexicalHandler,
			const XalanDOMString&	/* theIdentifier */)
{
	XalanAutoPtr<SAX2XMLReader>		theReader(XMLReaderFactory::createXMLReader());

	theReader->setContentHandler(&theContentHandler);

	theReader->setDTDHandler(theDTDHandler);

	theReader->setErrorHandler(&m_xercesParserLiaison);

	theReader->setLexicalHandler(theLexicalHandler);

	EntityResolver* const	theResolver = getEntityResolver();

	if (theResolver != 0)
	{
		theReader->setEntityResolver(theResolver);
	}

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



ErrorHandler*
XalanSourceTreeParserLiaison::getErrorHandler()
{
	return m_xercesParserLiaison.getErrorHandler();
}



const ErrorHandler*
XalanSourceTreeParserLiaison::getErrorHandler() const
{
	return m_xercesParserLiaison.getErrorHandler();
}



void
XalanSourceTreeParserLiaison::setErrorHandler(ErrorHandler*	handler)
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



EntityResolver*
XalanSourceTreeParserLiaison::getEntityResolver()
{
	return m_xercesParserLiaison.getEntityResolver();
}



void
XalanSourceTreeParserLiaison::setEntityResolver(EntityResolver*	resolver)
{
	m_xercesParserLiaison.setEntityResolver(resolver);
}



XalanSourceTreeDocument*
XalanSourceTreeParserLiaison::mapDocument(const XalanDocument*	theDocument) const
{
	const DocumentMapType::const_iterator	i =
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
