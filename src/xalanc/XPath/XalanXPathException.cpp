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
// Class header file.
#include "XalanXPathException.hpp"



XALAN_CPP_NAMESPACE_BEGIN




const XalanDOMString	XalanXPathException::s_emptyString(XalanMemMgrs::getDummyMemMgr());

const XalanDOMChar	XalanXPathException::m_type[] = 
{	
	XalanUnicode::charLetter_X,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_l,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_n,
	XalanUnicode::charLetter_X,
	XalanUnicode::charLetter_P,
	XalanUnicode::charLetter_a,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_h,
	XalanUnicode::charLetter_E,
	XalanUnicode::charLetter_x,
	XalanUnicode::charLetter_c,
	XalanUnicode::charLetter_e,
	XalanUnicode::charLetter_p,
	XalanUnicode::charLetter_t,
	XalanUnicode::charLetter_i,
	XalanUnicode::charLetter_o,
	XalanUnicode::charLetter_n,
	0
};

XalanXPathException::XalanXPathException(
			const XalanDOMString&	message,
			const XalanDOMString&	theURI,
			int						theLineNumber,
			int						theColumnNumber,
            MemoryManagerType&      theManager) :
	XSLException(message, theURI, theLineNumber, theColumnNumber, theManager)
{
}



XalanXPathException::XalanXPathException(
			const XalanDOMString&	message,
            MemoryManagerType&      theManager) :
	XSLException(message, theManager)
{
}



XalanXPathException::XalanXPathException(
			const LocatorType&		theLocator,
			const XalanDOMString&	theMessage,
            MemoryManagerType&      theManager) :
	XSLException(theLocator, theMessage, theManager)
{
}



XalanXPathException::XalanXPathException(MemoryManagerType& theManager) :
	XSLException(s_emptyString, theManager)
{
}

XalanXPathException::XalanXPathException(const XalanXPathException& other) :
    XSLException(other)
{
}

XalanXPathException::~XalanXPathException()
{
}



XALAN_CPP_NAMESPACE_END
