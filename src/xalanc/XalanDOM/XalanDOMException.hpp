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
#if !defined(XALANDOMEXCEPTION_HEADER_GUARD_1357924680)
#define XALANDOMEXCEPTION_HEADER_GUARD_1357924680



#include <xalanc/XalanDOM/XalanDOMDefinitions.hpp>



XALAN_CPP_NAMESPACE_BEGIN



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
		// These are the errors as defined in the W3C DOM recommendation.
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
		UNKNOWN_ERR 				= 201,

		// Indicates that an error occurred transcoding a
		// string.
		TRANSCODING_ERR				= 202
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

    // Not implemented...
    XalanDOMException&
    operator=(const XalanDOMException&);


    // Data members...
	const ExceptionCode		m_code;
};



XALAN_CPP_NAMESPACE_END



#endif	// !defined(XALANDOMEXCEPTION_HEADER_GUARD_1357924680)
