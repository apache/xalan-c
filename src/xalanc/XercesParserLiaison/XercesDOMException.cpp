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
#include "XercesDOMException.hpp"



#include <cassert>



XALAN_CPP_NAMESPACE_BEGIN



template<class ExceptionType>
static XercesDOMException::ExceptionCode
translateErrorCode(ExceptionType	theException)
{
	// Ugly hack for a bug in the Intel v6 compiler...
#if __INTEL_COMPILER <= 600
	return XercesDOMException::ExceptionCode(theException.code);
#else
	XercesDOMException::ExceptionCode	theXalanCode = XercesDOMException::UNKNOWN_ERR;

	switch(theException.code)
	{
	case ExceptionType::INDEX_SIZE_ERR:
		theXalanCode = XercesDOMException::INDEX_SIZE_ERR;
		break;

	case ExceptionType::DOMSTRING_SIZE_ERR:
		theXalanCode = XercesDOMException::DOMSTRING_SIZE_ERR;
		break;

	case ExceptionType::HIERARCHY_REQUEST_ERR:
		theXalanCode = XercesDOMException::HIERARCHY_REQUEST_ERR;
		break;

	case ExceptionType::WRONG_DOCUMENT_ERR:
		theXalanCode = XercesDOMException::WRONG_DOCUMENT_ERR;
		break;

	case ExceptionType::INVALID_CHARACTER_ERR:
		theXalanCode = XercesDOMException::INVALID_CHARACTER_ERR;
		break;

	case ExceptionType::NO_DATA_ALLOWED_ERR:
		theXalanCode = XercesDOMException::NO_DATA_ALLOWED_ERR;
		break;

	case ExceptionType::NO_MODIFICATION_ALLOWED_ERR:
		theXalanCode = XercesDOMException::NO_MODIFICATION_ALLOWED_ERR;
		break;

	case ExceptionType::NOT_FOUND_ERR:
		theXalanCode = XercesDOMException::NOT_FOUND_ERR;
		break;

	case ExceptionType::NOT_SUPPORTED_ERR:
		theXalanCode = XercesDOMException::INDEX_SIZE_ERR;
		break;

	case ExceptionType::INUSE_ATTRIBUTE_ERR:
		theXalanCode = XercesDOMException::INUSE_ATTRIBUTE_ERR;
		break;

	case ExceptionType::INVALID_STATE_ERR:
		theXalanCode = XercesDOMException::INVALID_STATE_ERR;
		break;

	case ExceptionType::SYNTAX_ERR:
		theXalanCode = XercesDOMException::SYNTAX_ERR;
		break;

	case ExceptionType::INVALID_MODIFICATION_ERR:
		theXalanCode = XercesDOMException::INVALID_MODIFICATION_ERR;
		break;

	case ExceptionType::NAMESPACE_ERR:
		theXalanCode = XercesDOMException::NAMESPACE_ERR;
		break;

	case ExceptionType::INVALID_ACCESS_ERR:
		theXalanCode = XercesDOMException::INVALID_ACCESS_ERR;
		break;

	default:
		assert(false);
		break;
	};

	return theXalanCode;
#endif
}



XercesDOMException::XercesDOMException(ExceptionCode 	code) :
	XalanDOMException(code)
{
}



XercesDOMException::XercesDOMException(const DOM_DOMExceptionType&	theException) :
	XalanDOMException(translateErrorCode(theException))
{
}



#if XERCES_VERSION_MAJOR >= 2
XercesDOMException::XercesDOMException(const DOMExceptionType&	theException) :
	XalanDOMException(translateErrorCode(theException))
{
}
#endif



XercesDOMException::XercesDOMException(const XercesDOMException&	theSource) :
	XalanDOMException(theSource)
{
}



XercesDOMException::~XercesDOMException()
{
}



XALAN_CPP_NAMESPACE_END
