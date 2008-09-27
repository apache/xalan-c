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
#if !defined(XERCESDOMWRAPPEREXCEPTION_HEADER_GUARD_1357924680)
#define XERCESDOMWRAPPEREXCEPTION_HEADER_GUARD_1357924680



#include <xalanc/XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



#include <xercesc/dom/DOMException.hpp>



#include <xalanc/XalanDOM/XalanDOMException.hpp>



#include <xalanc/XercesParserLiaison/XercesWrapperTypes.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XERCESPARSERLIAISON_EXPORT XercesDOMWrapperException : public XalanDOMException
{
public:

    /** @name Constructors and assignment operator */
    //@{

    /**
      * Constructor which takes an error code.
      *
      * @param code The error code which indicates the exception
      */
    explicit
    XercesDOMWrapperException(ExceptionCode     code = UNKNOWN_ERR);

    /**
      * Constructor which takes a Xerces exception and
      * translates it into a XercesDOMException.
      *
      * @param code The Xerces DOMException instance.
      */
    XercesDOMWrapperException(const DOMExceptionType&   theException);

    /**
      * Copy constructor.
      *
      * @param other The object to be copied.
      */
    XercesDOMWrapperException(const XercesDOMWrapperException&  theSource);

    //@}
    /** @name Destructor. */
    //@{

     /**
      * Destructor for XercesDOMException.
      */
    virtual
    ~XercesDOMWrapperException();
    //@}

private:

    static ExceptionCode
    translateErrorCode(DOMExceptionType::ExceptionCode  theCode);

    static ExceptionCode
    translateErrorCode(short    theCode)
    {
        return translateErrorCode(static_cast<DOMExceptionType::ExceptionCode>(theCode));
    }
};



XALAN_CPP_NAMESPACE_END



#endif  // !defined(XERCESDOMWRAPPEREXCEPTION_HEADER_GUARD_1357924680)
