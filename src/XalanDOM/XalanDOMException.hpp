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
#if !defined(XALANDOMEXCEPTION_HEADER_GUARD_1357924680)
#define XALANDOMEXCEPTION_HEADER_GUARD_1357924680



#include <XalanDOM/XalanDOMDefinitions.hpp>



/*
 * <meta name="usage" content="experimental"/>
 *
 * Base class for the DOM Exception interface.
 *
 * This class is experimental and subject to change!!
 */

class XALAN_DOM_EXPORT XalanDOMException
{
public:

	/** @name Enumerators for DOM Exceptions */
	//@{
	enum ExceptionCode
	{
		INDEX_SIZE_ERR				= 1,
		DOMSTRING_SIZE_ERR			= 2,
		HIERARCHY_REQUEST_ERR		= 3,
		WRONG_DOCUMENT_ERR			= 4,
		INVALID_CHARACTER_ERR		= 5,
		NO_DATA_ALLOWED_ERR 		= 6,
		NO_MODIFICATION_ALLOWED_ERR = 7,
		NOT_FOUND_ERR				= 8,
		NOT_SUPPORTED_ERR			= 9,
		INUSE_ATTRIBUTE_ERR 		= 10,
		INVALID_STATE_ERR			= 11,
		SYNTAX_ERR					= 12,
		INVALID_MODIFICATION_ERR	= 13,
		NAMESPACE_ERR				= 14,
		INVALID_ACCESS_ERR			= 15,

		// This is the first available number,
		// according to the spec.
		UNKNOWN_ERR 				= 201
	};
	//@}

	/** @name Constructors and assignment operator */
	//@{

	/**
	  * Constructor which takes an error code and a message.
	  *
	  * @param code The error code which indicates the exception
	  */
	explicit
	XalanDOMException(ExceptionCode 	code = UNKNOWN_ERR);

	/**
	  * Copy constructor.
	  *
	  * @param other The object to be copied.
	  */
	XalanDOMException(const XalanDOMException&	theSource);

	//@}
	/** @name Destructor. */
	//@{

	 /**
	  * Destructor for XalanDOMException.
	  */
	virtual
	~XalanDOMException();
	//@}

	/** @name Get functions. */
	 //@{
	 /**
	  * Returns a code value, from the set defined by the ExceptionCode enum,
	  * indicating the type of error that occurred.
	  * @return The exception code.
	  */
	virtual ExceptionCode
	getExceptionCode() const;

private:

	const ExceptionCode		m_code;
};



#endif	// !defined(XALANDOMEXCEPTION_HEADER_GUARD_1357924680)
