/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the  "License");
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
#include "XercesDOMException.hpp"



#include <cassert>



XALAN_CPP_NAMESPACE_BEGIN



template<class ExceptionType>
static XercesDOMException::ExceptionCode
translateErrorCode(ExceptionType    theException)
{
    XercesDOMException::ExceptionCode   theXalanCode = XercesDOMException::UNKNOWN_ERR;

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
}



XercesDOMException::XercesDOMException(ExceptionCode    code) :
    XalanDOMException(code)
{
}



#if defined(XALAN_BUILD_DEPRECATED_DOM_BRIDGE)
XercesDOMException::XercesDOMException(const DOM_DOMExceptionType&  theException) :
    XalanDOMException(translateErrorCode(theException))
{
}
#endif



#if XERCES_VERSION_MAJOR >= 2
XercesDOMException::XercesDOMException(const DOMExceptionType&  theException) :
    XalanDOMException(translateErrorCode(theException))
{
}
#endif



XercesDOMException::XercesDOMException(const XercesDOMException&    theSource) :
    XalanDOMException(theSource)
{
}



XercesDOMException::~XercesDOMException()
{
}



XALAN_CPP_NAMESPACE_END
