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
#include "XercesDOMWrapperException.hpp"



#include <cassert>



XALAN_CPP_NAMESPACE_BEGIN



XercesDOMWrapperException::XercesDOMWrapperException(ExceptionCode  code) :
    XalanDOMException(code)
{
}



XercesDOMWrapperException::XercesDOMWrapperException(const DOMExceptionType&    theException) :
    XalanDOMException(translateErrorCode(theException.code))
{
}



XercesDOMWrapperException::XercesDOMWrapperException(const XercesDOMWrapperException&   theSource) :
    XalanDOMException(theSource)
{
}



XercesDOMWrapperException::~XercesDOMWrapperException()
{
}



XercesDOMWrapperException::ExceptionCode
XercesDOMWrapperException::translateErrorCode(DOMExceptionType::ExceptionCode   theCode)
{
    ExceptionCode   theXalanCode = UNKNOWN_ERR;

    switch(theCode)
    {
    case DOMExceptionType::INDEX_SIZE_ERR:
        theXalanCode = INDEX_SIZE_ERR;
        break;

    case DOMExceptionType::DOMSTRING_SIZE_ERR:
        theXalanCode = DOMSTRING_SIZE_ERR;
        break;

    case DOMExceptionType::HIERARCHY_REQUEST_ERR:
        theXalanCode = HIERARCHY_REQUEST_ERR;
        break;

    case DOMExceptionType::WRONG_DOCUMENT_ERR:
        theXalanCode = WRONG_DOCUMENT_ERR;
        break;

    case DOMExceptionType::INVALID_CHARACTER_ERR:
        theXalanCode = INVALID_CHARACTER_ERR;
        break;

    case DOMExceptionType::NO_DATA_ALLOWED_ERR:
        theXalanCode = NO_DATA_ALLOWED_ERR;
        break;

    case DOMExceptionType::NO_MODIFICATION_ALLOWED_ERR:
        theXalanCode = NO_MODIFICATION_ALLOWED_ERR;
        break;

    case DOMExceptionType::NOT_FOUND_ERR:
        theXalanCode = NOT_FOUND_ERR;
        break;

    case DOMExceptionType::NOT_SUPPORTED_ERR:
        theXalanCode = INDEX_SIZE_ERR;
        break;

    case DOMExceptionType::INUSE_ATTRIBUTE_ERR:
        theXalanCode = INUSE_ATTRIBUTE_ERR;
        break;

    case DOMExceptionType::INVALID_STATE_ERR:
        theXalanCode = INVALID_STATE_ERR;
        break;

    case DOMExceptionType::SYNTAX_ERR:
        theXalanCode = SYNTAX_ERR;
        break;

    case DOMExceptionType::INVALID_MODIFICATION_ERR:
        theXalanCode = INVALID_MODIFICATION_ERR;
        break;

    case DOMExceptionType::NAMESPACE_ERR:
        theXalanCode = NAMESPACE_ERR;
        break;

    case DOMExceptionType::INVALID_ACCESS_ERR:
        theXalanCode = INVALID_ACCESS_ERR;
        break;

    default:
        assert(false);
        break;
    };

    return theXalanCode;
}



XALAN_CPP_NAMESPACE_END
