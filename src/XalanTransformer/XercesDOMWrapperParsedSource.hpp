/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2001-2002 The Apache Software Foundation.  All rights 
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
#if !defined(XERCESDOMWRAPPERPARSEDSOURCE_HEADER_GUARD)
#define XERCESDOMWRAPPERPARSEDSOURCE_HEADER_GUARD



// Base include file.  Must be first.
#include <XalanTransformer/XalanTransformerDefinitions.hpp>



#include <XalanDOM/XalanDOMString.hpp>



#include <XalanTransformer/XalanParsedSource.hpp>



XALAN_DECLARE_XERCES_CLASS(DOM_Document)
XALAN_DECLARE_XERCES_CLASS(DOMDocument)



XALAN_CPP_NAMESPACE_BEGIN



typedef XERCES_CPP_NAMESPACE_QUALIFIER DOM_Document	DOM_Document_Type;
typedef XERCES_CPP_NAMESPACE_QUALIFIER DOMDocument	DOMDocument_Type;



class XercesParserLiaison;
class XercesDOMSupport;



/**
 * This is designed to allow a XalanTranfomer object to wrap a parsed
 * Xerces document. 
 */
class XALAN_TRANSFORMER_EXPORT XercesDOMWrapperParsedSource : public XalanParsedSource
{
public:

	/**
	 * Constructor
	 *
	 * @param theDocument The Xerces document to wrap
	 * @param theParserLiaison The XercesParserLiaison instance that will own the document.
	 * @param theDOMSupport The XercesDOMSupport instance
	 * @param theURI The URI of the document, if any.
	 *
	 * @deprecated This constructor is deprecated.
	 */
	XercesDOMWrapperParsedSource(
			const DOM_Document_Type&	theDocument,
			XercesParserLiaison&		theParserLiaison,
			XercesDOMSupport&			theDOMSupport,
			const XalanDOMString&		theURI = XalanDOMString());

	/**
	 * Constructor
	 *
	 * @param theDocument The Xerces document to wrap
	 * @param theParserLiaison The XercesParserLiaison instance that will own the document.
	 * @param theDOMSupport The XercesDOMSupport instance
	 * @param theURI The URI of the document, if any.
	 */
	XercesDOMWrapperParsedSource(
			const DOMDocument_Type*		theDocument,
			XercesParserLiaison&		theParserLiaison,
			XercesDOMSupport&			theDOMSupport,
			const XalanDOMString&		theURI = XalanDOMString());

	virtual
	~XercesDOMWrapperParsedSource();

	virtual XalanDocument*
	getDocument() const;

	virtual XalanParsedSourceHelper*
	createHelper() const;

	virtual const XalanDOMString&
	getURI() const;

private:

	XercesParserLiaison&	m_parserLiaison;

	XercesDOMSupport&		m_domSupport;

	XalanDocument* const	m_parsedSource;

	const XalanDOMString	m_uri;
};



XALAN_CPP_NAMESPACE_END



#endif	// XERCESDOMWRAPPERPARSEDSOURCE_HEADER_GUARD
