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
// Base class header file.
#include "XPathParserException.hpp"



XALAN_CPP_NAMESPACE_BEGIN




XPathParserException::XPathParserException(
			const XalanDOMString&	message,
			const XalanDOMString&	theURI,
			int						theLineNumber,
			int						theColumnNumber,
			const XalanDOMString&	theType) :
	XalanXPathException(message, theURI, theLineNumber, theColumnNumber, theType)
{
}



XPathParserException::XPathParserException(
			const LocatorType&		theLocator,
			const XalanDOMString&	theMessage,
			const XalanDOMString&	theType) :
	XalanXPathException(theLocator, theMessage, theType)
{
}



XPathParserException::XPathParserException(
			const XalanDOMString&	message,
			const XalanDOMString&	theType) :
	XalanXPathException(message, theType)
{
}



XPathParserException::~XPathParserException()
{
}



XALAN_CPP_NAMESPACE_END
