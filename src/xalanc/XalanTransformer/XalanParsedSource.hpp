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
#if !defined(XALANPARSEDSOURCE_HEADER_GUARD)
#define XALANPARSEDSOURCE_HEADER_GUARD



// Base include file.  Must be first.
#include <xalanc/XalanTransformer/XalanTransformerDefinitions.hpp>



// We're bringing in XalanDocument so that its derivation from XalanNode
// is known...
#include <xalanc/XalanDOM/XalanDocument.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class DOMSupport;
class XMLParserLiaison;



class XALAN_TRANSFORMER_EXPORT XalanParsedSourceHelper
{
public:

	virtual
	~XalanParsedSourceHelper()
	{
	}

	/**
	 * Get the DOMSupport instance.
	 *
	 * @return A reference to a DOMSupport instance.
	 */
	virtual DOMSupport&
	getDOMSupport() = 0;

	/**
	 * Get the XMLParserLiaison instance.
	 *
	 * @return A rerefernce to an XMLParserLiaison instance.
	 */
	virtual XMLParserLiaison&
	getParserLiaison() = 0;
};



/**
 * This is abstract base class designed to allow a XalanTranfomer 
 * object to reuse a parsed document. 
 */
class XALAN_TRANSFORMER_EXPORT XalanParsedSource
{
public:

	XalanParsedSource();

	virtual
	~XalanParsedSource();

	/**
	 * Get a pointer to the XalanDocument instance for the source
	 * document
	 *
	 * @return A pointer to a XalanDocument instance.
	 */
	virtual XalanDocument*
	getDocument() const = 0;

	/**
	 * Create the appropriate XalanParsedSourceHelper instance to
	 * use for transforming with the instance.
	 *
	 * The caller is responsible for deleting the object when finished.
	 *
	 * @return A pointer to a XalanParsedSourceHelper instance.
	 */
	virtual XalanParsedSourceHelper*
	createHelper() const = 0;

	/**
	 * Get the URI for the parsed source, if any.
	 * use for transforming with the instance.
	 *
	 * @return A const reference to a string containing the URI
	 */
	virtual const XalanDOMString&
	getURI() const = 0;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALANPARSEDSOURCE_HEADER_GUARD



