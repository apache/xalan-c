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
#if !defined(XPATHPARSEREXCEPTION_HEADER_GUARD_1357924680)
#define XPATHPARSEREXCEPTION_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



#include <xalanc/XPath/XalanXPathException.hpp>



XALAN_CPP_NAMESPACE_BEGIN



/**
 * Exception class thrown when a problem parsing an XPath is encountered
 */
class XALAN_XPATH_EXPORT XPathParserException : public XalanXPathException
{
public:

	/**
	 * Constructor
	 * 
	 * @param theMessage message to write when exception thrown
	 * @param theURI the URI of the related document, if known
	 * @param theLineNumber the line number of the related document, or -1 if not known
	 * @param theColumnNumber the column number of the related document, or -1 if not known
	 * @param theType type of exception, default is "XPathParserException"
	 */
	XPathParserException(
			const XalanDOMString&	theMessage,
			const XalanDOMString&	theURI,
			int						theLineNumber,
			int						theColumnNumber,
            MemoryManagerType&      theManager);

	/**
	 * Constructor
	 * 
	 * @param theLocator The locator instance for error reporting.
	 * @param theMessage message to write when exception thrown
	 * @param theType type of exception, default is "XPathParserException"
	 */
	XPathParserException(
			const LocatorType&		theLocator,
			const XalanDOMString&	theMessage,
            MemoryManagerType&      theManager);

	/**
	 * Constructor
	 * 
	 * @param theMessage message to write when exception thrown
	 * @param theType type of exception, default is "XPathParserException"
	 */
	XPathParserException(
			const XalanDOMString&	theMessage,
            MemoryManagerType&      theManager);

	virtual
		~XPathParserException();
	
	virtual const XalanDOMChar*
		getType() const
	{
		return m_type;
	}
private:
	
	static const XalanDOMChar	m_type[];
};



XALAN_CPP_NAMESPACE_END



#endif	// XPATHPARSEREXCEPTION_HEADER_GUARD_1357924680
