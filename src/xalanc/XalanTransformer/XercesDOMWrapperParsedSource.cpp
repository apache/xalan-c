/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2003 The Apache Software Foundation.  All rights 
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
#include "XercesDOMWrapperParsedSource.hpp"



#include <xalanc/XalanDOM/XalanDocument.hpp>



#include <xalanc/PlatformSupport/URISupport.hpp>



#include <xalanc/XercesParserLiaison/XercesParserLiaison.hpp>
#include <xalanc/XercesParserLiaison/XercesDOMSupport.hpp>
#include <xalanc/XercesParserLiaison/XercesDocumentBridge.hpp>



#include "XercesDOMParsedSource.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XercesDOMWrapperParsedSource::XercesDOMWrapperParsedSource(
			const DOM_Document_Type&	theDocument,
			XercesParserLiaison&		theParserLiaison,
			XercesDOMSupport&			theDOMSupport,
			const XalanDOMString&		theURI) :
	XalanParsedSource(),
	m_parserLiaison(theParserLiaison),
	m_domSupport(theDOMSupport),
	m_parsedSource(theParserLiaison.createDocument(theDocument, true, true)),
	m_uri(URISupport::NormalizeURIText(theURI))
{
	assert(m_parsedSource != 0);
}



XercesDOMWrapperParsedSource::XercesDOMWrapperParsedSource(
			const DOMDocument_Type*		theDocument,
			XercesParserLiaison&		theParserLiaison,
			XercesDOMSupport&			theDOMSupport,
			const XalanDOMString&		theURI) :
	XalanParsedSource(),
	m_parserLiaison(theParserLiaison),
	m_domSupport(theDOMSupport),
	m_parsedSource(theParserLiaison.createDocument(theDocument, true, true)),
	m_uri(URISupport::NormalizeURIText(theURI))
{
	assert(m_parsedSource != 0);
}



XercesDOMWrapperParsedSource::~XercesDOMWrapperParsedSource()
{
	m_parserLiaison.destroyDocument(m_parsedSource);
}



XalanDocument*
XercesDOMWrapperParsedSource::getDocument() const
{
	return m_parsedSource;
}



XalanParsedSourceHelper*
XercesDOMWrapperParsedSource::createHelper() const
{
	return new XercesDOMParsedSourceHelper;
}



const XalanDOMString&
XercesDOMWrapperParsedSource::getURI() const
{
	return m_uri;
}



XALAN_CPP_NAMESPACE_END
