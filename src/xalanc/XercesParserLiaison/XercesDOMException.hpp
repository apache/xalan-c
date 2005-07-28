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
#if !defined(XERCESDOMEXCEPTION_HEADER_GUARD_1357924680)
#define XERCESDOMEXCEPTION_HEADER_GUARD_1357924680



#include <xalanc/XercesParserLiaison/XercesParserLiaisonDefinitions.hpp>



#if defined(XALAN_BUILD_DEPRECATED_DOM_BRIDGE)
#include <xercesc/dom/deprecated/DOM_DOMException.hpp>
#endif
#include <xercesc/dom/DOMException.hpp>



#include <xalanc/XalanDOM/XalanDOMException.hpp>



#if defined(XALAN_BUILD_DEPRECATED_DOM_BRIDGE)
#include <xalanc/XercesParserLiaison/Deprecated/XercesBridgeTypes.hpp>
#endif
#include <xalanc/XercesParserLiaison/XercesWrapperTypes.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XERCESPARSERLIAISON_EXPORT XercesDOMException : public XalanDOMException
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
	XercesDOMException(ExceptionCode 	code = UNKNOWN_ERR);

#if defined(XALAN_BUILD_DEPRECATED_DOM_BRIDGE)
	/**
	  * This constructor is deprecated.
	  * 
	  * Constructor which takes a Xerces exception and
	  * translates it into a XercesDOMException.
	  * 
	  * @deprecated The Xerces DOM bridge is deprecated.
	  * @param code The Xerces DOM_DOMException instance.
	  */
	XercesDOMException(const DOM_DOMExceptionType&	theException);
#endif

#if XERCES_VERSION_MAJOR >= 2
	/**
	  * Constructor which takes a Xerces exception and
	  * translates it into a XercesDOMException.
	  *
	  * @param code The Xerces DOMException instance.
	  */
	XercesDOMException(const DOMExceptionType&	theException);

#endif

	/**
	  * Copy constructor.
	  *
	  * @param other The object to be copied.
	  */
	XercesDOMException(const XercesDOMException&	theSource);

	//@}
	/** @name Destructor. */
	//@{

	 /**
	  * Destructor for XercesDOMException.
	  */
	virtual
	~XercesDOMException();
	//@}

private:
};



XALAN_CPP_NAMESPACE_END



#endif	// !defined(XERCESDOMEXCEPTION_HEADER_GUARD_1357924680)
