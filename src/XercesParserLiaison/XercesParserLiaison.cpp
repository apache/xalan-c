/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2001 The Apache Software Foundation.  All rights 
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



#if defined(XALAN_OLD_STREAM_HEADERS)
#include <iostream.h>
#else
#include <iostream>
#endif



#include <xercesc/framework/URLInputSource.hpp>
#include <xercesc/parsers/DOMParser.hpp>
#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/sax/SAXParseException.hpp>



#include <Include/XalanAutoPtr.hpp>
#include <Include/STLHelper.hpp>



#include <PlatformSupport/ExecutionContext.hpp>
#include <PlatformSupport/XalanUnicode.hpp>



#include <DOMSupport/DOMSupport.hpp>



#include "XercesDocumentBridge.hpp"
#include "XercesDOMSupport.hpp"



static const XalanDOMChar	theDefaultSpecialCharacters[] =
{
		XalanUnicode::charLessThanSign,
		XalanUnicode::charGreaterThanSign,
		XalanUnicode::charAmpersand,
		XalanUnicode::charApostrophe,
		XalanUnicode::charQuoteMark,
		XalanUnicode::charCR,
		XalanUnicode::charLF,
		0
};



XercesParserLiaison::XercesParserLiaison(
			XercesDOMSupport&	/* theSupport */,
			DocumentNumberType	theStartingNumber) :
	m_documentNumber(theStartingNumber),
	m_indent(-1),
	m_useValidation(false),
	m_includeIgnorableWhitespace(true),
	m_doNamespaces(true),
	m_exitOnFirstFatalError(true),
	m_entityResolver(0),
	m_errorHandler(this),
	m_documentMap(),
	m_buildBridge(true),
	m_threadSafe(false),
	m_executionContext(0)
{
}



XercesParserLiaison::XercesParserLiaison(DocumentNumberType		theStartingNumber) :
	m_documentNumber(theStartingNumber),
	m_indent(-1),
	m_useValidation(false),
	m_includeIgnorableWhitespace(true),
	m_doNamespaces(true),
	m_exitOnFirstFatalError(true),
	m_entityResolver(0),
	m_errorHandler(this),
	m_documentMap(),
	m_buildBridge(true),
	m_threadSafe(false),
	m_executionContext(0)
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

	m_executionContext = 0;
}



ExecutionContext*
XercesParserLiaison::getExecutionContext() const
{
	return m_executionContext;
}



void
XercesParserLiaison::setExecutionContext(ExecutionContext&	theContext)
{
	m_executionContext = &theContext;
}



void
XercesParserLiaison::parseXMLStream(
			const InputSource&		urlInputSource,
			DocumentHandler&		handler,
			const XalanDOMString&	/* identifier */)
{
	XalanAutoPtr<SAXParser>		theParser(CreateSAXParser());

	theParser->setDocumentHandler(&handler);

	theParser->parse(urlInputSource);
}



XalanDocument*
XercesParserLiaison::parseXMLStream(
			const InputSource&		reader,
			const XalanDOMString&	/* identifier */)
{
	XalanAutoPtr<DOMParser>		theParser(CreateDOMParser());

	theParser->parse(reader);

	DOM_Document	theXercesDocument =
		theParser->getDocument();

	theXercesDocument.normalize();

	XercesDocumentBridge*	theNewDocument = 0;

	if (theXercesDocument.isNull() == false)
	{
		theNewDocument = doCreateDocument(theXercesDocument, m_threadSafe, m_buildBridge);

		m_documentMap[theNewDocument] = theNewDocument;
	}

	return theNewDocument;
}



XalanDocument*
XercesParserLiaison::createDocument()
{
	const DOM_Document	theXercesDocument =
		DOM_Document::createDocument();

	return createDocument(theXercesDocument, false, false);
}



XalanDocument*
XercesParserLiaison::createDOMFactory()
{
	return createDocument();
}



void
XercesParserLiaison::destroyDocument(XalanDocument*		theDocument)
{
	if (mapDocument(theDocument) != 0)
	{
		m_documentMap.erase(theDocument);

		delete theDocument;
	}
}



XercesParserLiaison::DocumentNumberType
XercesParserLiaison::getNextDocumentNumber()
{
	return m_documentNumber++;
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
XercesParserLiaison::getUseValidation() const
{
	return m_useValidation;
}



void
XercesParserLiaison::setUseValidation(bool	b)
{
	m_useValidation = b;
}



const XalanDOMString
XercesParserLiaison::getParserDescription() const
{
	return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("Xerces"));
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
XercesParserLiaison::getErrorHandler() const
{
	return m_errorHandler;
}



void
XercesParserLiaison::setErrorHandler(ErrorHandler*	handler)
{
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
XercesParserLiaison::createDocument(
			const DOM_Document&		theXercesDocument,
			bool					threadSafe,
			bool					buildBridge)
{
	return doCreateDocument(theXercesDocument, threadSafe, buildBridge);
}



XercesDocumentBridge*
XercesParserLiaison::mapDocument(const XalanDocument*	theDocument) const
{
	const DocumentMapType::const_iterator	i =
		m_documentMap.find(theDocument);

	return i != m_documentMap.end() ? (*i).second : 0;
}



DOM_Document
XercesParserLiaison::mapXercesDocument(const XalanDocument*		theDocument) const
{
	const DocumentMapType::const_iterator	i =
		m_documentMap.find(theDocument);

	return i != m_documentMap.end() ? (*i).second->getXercesDocument() : DOM_Document();
}



void
XercesParserLiaison::fatalError(const SAXParseException&	e)
{
	XalanDOMString	theMessage("Fatal Error");

	formatErrorMessage(e, theMessage);

	if (m_executionContext != 0)
	{
		// We call warning() because we don't want the execution
		// context to potentially throw an exception.
		m_executionContext->warn(theMessage);
	}
	else
	{
#if !defined(XALAN_NO_NAMESPACES)
		using std::cerr;
		using std::endl;
#endif

		cerr << endl << theMessage << endl;
	}

	throw e;
}



void
XercesParserLiaison::error(const SAXParseException&		e)
{
	XalanDOMString	theMessage("Error ");

	formatErrorMessage(e, theMessage);

	if (m_executionContext != 0)
	{
		// We call warn() because we don't want the execution
		// context to potentially throw an exception.
		m_executionContext->warn(theMessage);
	}
	else
	{
#if !defined(XALAN_NO_NAMESPACES)
		using std::cerr;
		using std::endl;
#endif

		cerr << endl << theMessage << endl;
	}
}



void
XercesParserLiaison::warning(const SAXParseException&	e)
{
	XalanDOMString	theMessage("Warning ");

	formatErrorMessage(e, theMessage);

	if (m_executionContext != 0)
	{
		m_executionContext->warn(theMessage);
	}
	else
	{
#if !defined(XALAN_NO_NAMESPACES)
		using std::cerr;
		using std::endl;
#endif

		cerr << endl << theMessage << endl;
	}
}



void
XercesParserLiaison::formatErrorMessage(
			const SAXParseException&	e,
			XalanDOMString&				theMessage)
{
	append(theMessage, " at (file ");

	const XalanDOMChar* const	theSystemID = e.getSystemId();

	if (theSystemID == 0)
	{
		append(theMessage, "<unknown>");
	}
	else
	{
		append(theMessage, theSystemID);
	}

	append(theMessage, ", line ");
	append(theMessage, LongToDOMString(long(e.getLineNumber())));
	append(theMessage, ", column ");
	append(theMessage, LongToDOMString(long(e.getColumnNumber())));
	append(theMessage, "): ");
	append(theMessage, e.getMessage());
}



void
XercesParserLiaison::resetErrors()
{
}



DOMParser*
XercesParserLiaison::CreateDOMParser()
{
	DOMParser* const	theParser = new DOMParser;

	theParser->setDoValidation(m_useValidation);

	theParser->setIncludeIgnorableWhitespace(m_includeIgnorableWhitespace);

	theParser->setDoNamespaces(m_doNamespaces);

	theParser->setExitOnFirstFatalError(m_exitOnFirstFatalError);

	if (m_entityResolver != 0)
	{
		theParser->setEntityResolver(m_entityResolver);
	}

	theParser->setErrorHandler(m_errorHandler);

	// Xerces has a non-standard node type to represent the XML decl.
	// Why did they ever do this?
	theParser->setToCreateXMLDeclTypeNode(false);

	return theParser;
}



SAXParser*
XercesParserLiaison::CreateSAXParser()
{
	SAXParser* const	theParser = new SAXParser;

	theParser->setDoValidation(false);

	theParser->setDoNamespaces(false);

	theParser->setExitOnFirstFatalError(m_exitOnFirstFatalError);

	if (m_entityResolver != 0)
	{
		theParser->setEntityResolver(m_entityResolver);
	}

	theParser->setErrorHandler(m_errorHandler);

	return theParser;
}



XercesDocumentBridge*
XercesParserLiaison::doCreateDocument(
			const DOM_Document&		theXercesDocument,
			bool					threadSafe,
			bool					buildBridge)
{
	XercesDocumentBridge* const		theNewDocument =
		new XercesDocumentBridge(theXercesDocument, getNextDocumentNumber(), threadSafe, buildBridge);

	m_documentMap[theNewDocument] = theNewDocument;

	return theNewDocument;
}
