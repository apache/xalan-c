/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
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
#include "XercesParserLiaison.hpp"



#include <algorithm>
#include <memory>
#include <iostream>



#include <framework/URLInputSource.hpp>
#include <parsers/DOMParser.hpp>
#include <parsers/SAXParser.hpp>
#include <sax/SAXParseException.hpp>



#include <PlatformSupport/STLHelper.hpp>



#include <DOMSupport/DOMSupport.hpp>



#include "XercesDocumentBridge.hpp"



static const XalanDOMString		theParserName(XALAN_STATIC_UCODE_STRING("Xerces"));
static const XalanDOMString		theDefaultSpecialCharacters(XALAN_STATIC_UCODE_STRING("<>&\"\'\r\n"));



XercesParserLiaison::XercesParserLiaison(DOMSupport&	theSupport) :
	m_DOMSupport(theSupport),
	m_specialCharacters(theDefaultSpecialCharacters),
	m_indent(-1),
	m_shouldExpandEntityRefs(false),
	m_useValidation(false),
	m_includeIgnorableWhitespace(true),
	m_doNamespaces(false),
	m_exitOnFirstFatalError(true),
	m_factory(0),
	m_entityResolver(0),
	m_errorHandler(this),
	m_documentMap()
{
}



XercesParserLiaison::~XercesParserLiaison()
{
	reset();
}



void
XercesParserLiaison::reset()
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::for_each;
#endif

	// Delete any live documents.
	for_each(m_documentMap.begin(),
			 m_documentMap.end(),
			 makeMapValueDeleteFunctor(m_documentMap));

	m_documentMap.clear();

	m_DOMSupport.reset();
}



bool
XercesParserLiaison::supportsSAX() const
{
	return true;
}



void
XercesParserLiaison::parseXMLStream(
			InputSource&			urlInputSource,
			DocumentHandler&		handler,
			const XalanDOMString&	/* identifier */)
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::auto_ptr;
#endif

	auto_ptr<SAXParser>		theParser(CreateSAXParser());

	theParser->setDocumentHandler(&handler);

	theParser->parse(urlInputSource);
}



XalanDocument*
XercesParserLiaison::parseXMLStream(
			InputSource&			reader,
			const XalanDOMString&	/* identifier */)
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::auto_ptr;
#endif

	auto_ptr<DOMParser>		theParser(CreateDOMParser());

	theParser->parse(reader);

	const DOM_Document	theXercesDocument =
		theParser->getDocument();

	XercesDocumentBridge*	theNewDocument = 0;

	if (theXercesDocument.isNull() == false)
	{
		theNewDocument = new XercesDocumentBridge(theXercesDocument);

		m_documentMap[theNewDocument] = theNewDocument;
	}

	return theNewDocument;
}



XalanDocument*
XercesParserLiaison::createDocument()
{
	const DOM_Document	theXercesDocument =
		DOM_Document::createDocument();

	return createDocument(theXercesDocument);
}



XalanDocument*
XercesParserLiaison::getDOMFactory()
{
	if (m_factory == 0)
	{
		m_factory = createDocument();
	}

	return m_factory;
}



/**
 * Returns the element name with the namespace expanded.
 */
XalanDOMString
XercesParserLiaison::getExpandedElementName(const XalanElement&		elem) const
{
	return m_DOMSupport.getExpandedElementName(elem);
}



/**
 * Returns the attribute name with the namespace expanded.
 */
XalanDOMString
XercesParserLiaison::getExpandedAttributeName(const XalanAttr&	attr) const
{
	return m_DOMSupport.getExpandedAttributeName(attr);
}



void
XercesParserLiaison::setSpecialCharacters(const XalanDOMString&	str)
{
	m_specialCharacters = str;
}



const XalanDOMString&
XercesParserLiaison::getSpecialCharacters() const
{
	return m_specialCharacters;
}



int
XercesParserLiaison::getIndent() const
{
	return m_indent;
}



void
XercesParserLiaison::setIndent(int	i)
{
	m_indent = i;
}



bool
XercesParserLiaison::getShouldExpandEntityRefs() const
{
	return m_shouldExpandEntityRefs;
}



void
XercesParserLiaison::SetShouldExpandEntityRefs(bool	b)
{
	m_shouldExpandEntityRefs = b;
}



bool
XercesParserLiaison::getUseValidation() const
{
	return m_useValidation;
}



void
XercesParserLiaison::setUseValidation(bool	b)
{
	m_useValidation = b;
}



const XalanDOMString&
XercesParserLiaison::getParserDescription() const
{
	return theParserName;
}



bool
XercesParserLiaison::getIncludeIgnorableWhitespace() const
{
	return m_includeIgnorableWhitespace;
}



void
XercesParserLiaison::setIncludeIgnorableWhitespace(bool	include)
{
	m_includeIgnorableWhitespace = include;
}



ErrorHandler*
XercesParserLiaison::getErrorHandler()
{
	return m_errorHandler;
}



const ErrorHandler*
XercesParserLiaison::getErrorHandler() const
{
	return m_errorHandler;
}



void
XercesParserLiaison::setErrorHandler(ErrorHandler*	handler)
{
	assert(handler != 0);

	m_errorHandler = handler;
}



bool
XercesParserLiaison::getDoNamespaces() const
{
	return m_doNamespaces;
}



void
XercesParserLiaison::setDoNamespaces(bool	newState)
{
	m_doNamespaces = newState;
}



bool
XercesParserLiaison::getExitOnFirstFatalError() const
{
	return m_exitOnFirstFatalError;
}



void
XercesParserLiaison::setExitOnFirstFatalError(bool	newState)
{
	m_exitOnFirstFatalError = newState;
}



EntityResolver*
XercesParserLiaison::getEntityResolver()
{
	return m_entityResolver;
}



const EntityResolver*
XercesParserLiaison::getEntityResolver() const
{
	return m_entityResolver;
}



void
XercesParserLiaison::setEntityResolver(EntityResolver*	resolver)
{
	m_entityResolver = resolver;
}



XalanDocument*
XercesParserLiaison::createDocument(const DOM_Document&		theXercesDocument)
{
	XercesDocumentBridge* const		theNewDocument =
		new XercesDocumentBridge(theXercesDocument);

	m_documentMap[theNewDocument] = theNewDocument;

	return theNewDocument;
}



XercesDocumentBridge*
XercesParserLiaison::mapDocument(const XalanDocument*	theDocument) const
{
	const DocumentMapType::const_iterator	i =
		m_documentMap.find(theDocument);

	return i != m_documentMap.end() ? i->second : 0;
}



DOM_Document
XercesParserLiaison::mapXercesDocument(const XalanDocument*		theDocument) const
{
	const DocumentMapType::const_iterator	i =
		m_documentMap.find(theDocument);

	return i != m_documentMap.end() ? i->second->getXercesDocument() : DOM_Document();
}



void
XercesParserLiaison::fatalError(const SAXParseException& e)
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::cerr;
	using std::endl;
#endif

	cerr << "\nFatal Error at (file " << DOMStringToStdString(e.getSystemId())
		 << ", line " << e.getLineNumber()
		 << ", char " << e.getColumnNumber()
         << "): " << DOMStringToStdString(e.getMessage()) << endl;

	throw e;
}

void XercesParserLiaison::error(const SAXParseException& e)
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::cerr;
	using std::endl;
#endif

	cerr << "\nError at (file " << DOMStringToStdString(e.getSystemId())
		 << ", line " << e.getLineNumber()
		 << ", char " << e.getColumnNumber()
         << "): " << DOMStringToStdString(e.getMessage()) << endl;
}


void XercesParserLiaison::warning(const SAXParseException& e)
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::cerr;
	using std::endl;
#endif

	cerr << "\nWarning at (file " << DOMStringToStdString(e.getSystemId())
		 << ", line " << e.getLineNumber()
		 << ", char " << e.getColumnNumber()
         << "): " << DOMStringToStdString(e.getMessage()) << endl;
}



void
XercesParserLiaison::resetErrors()
{
}



DOMParser*
XercesParserLiaison::CreateDOMParser()
{
	DOMParser* const	theParser = new DOMParser;

	theParser->setExpandEntityReferences(m_shouldExpandEntityRefs);

	theParser->setDoValidation(m_useValidation);

	theParser->setIncludeIgnorableWhitespace(m_includeIgnorableWhitespace);

	theParser->setDoNamespaces(m_doNamespaces);

	theParser->setExitOnFirstFatalError(m_exitOnFirstFatalError);

	if (m_entityResolver != 0)
	{
		theParser->setEntityResolver(m_entityResolver);
	}

	theParser->setErrorHandler(m_errorHandler);

	return theParser;
}



SAXParser*
XercesParserLiaison::CreateSAXParser()
{
	SAXParser* const	theParser = new SAXParser;

	theParser->setDoValidation(m_useValidation);

	theParser->setDoNamespaces(m_doNamespaces);

	theParser->setExitOnFirstFatalError(m_exitOnFirstFatalError);

	if (m_entityResolver != 0)
	{
		theParser->setEntityResolver(m_entityResolver);
	}

	theParser->setErrorHandler(m_errorHandler);

	return theParser;
}
