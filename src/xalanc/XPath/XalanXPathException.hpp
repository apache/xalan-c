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
#if !defined(XALANXPATHEXCEPTION_HEADER_GUARD_1357924680)
#define XALANXPATHEXCEPTION_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>
#include <xalanc/XalanDOM/XalanNode.hpp>



// Base class header file.
#include <xalanc/PlatformSupport/XSLException.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XalanNode;



class XALAN_XPATH_EXPORT XalanXPathException : public XSLException
{
public:

	/**
	 * Construct an XPath exeption object.
	 * 
	 * @param message message explaining the problem. 
	 * @param theURI the URI of the related document, if known
	 * @param theLineNumber the line number of the related document.
	 * @param theColumnNumber the column number of the related document.
	 * @param styleNode the node in the stylesheet where the problem occurred
	 * @param theType type of exception, default is "XalanXPathException"
	 */
	XalanXPathException(
			const XalanDOMString&	message,
			const XalanDOMString&	theURI,
			int						theLineNumber,
			int						theColumnNumber,
            MemoryManagerType&      theManager);

	/**
	 * Constructor
	 * 
	 * @param theLocator The locator instance for error reporting.
	 * @param theMessage message to write when exception thrown
	 * @param styleNode the node in the stylesheet where the problem occurred
	 * @param theType type of exception, default is "XalanXPathException"
	 */
	XalanXPathException(
			const LocatorType&		theLocator,
			const XalanDOMString&	theMessage,
            MemoryManagerType&      theManager);

	/**
	 * Construct an XPath exeption object.
	 * 
	 * @param message message explaining the problem. 
	 * @param theType type of exception, default is "XalanXPathException"
	 */
	XalanXPathException(const XalanDOMString&	message,
                            MemoryManagerType& theManager);

    XalanXPathException(const XalanXPathException& other);

	virtual
	~XalanXPathException();

	virtual const XalanDOMChar*
	getType() const
	{
		return m_type;
	}

protected:

	/**
	 * Construct an XPath exeption object.
	 * 
	 */
	explicit
	XalanXPathException(MemoryManagerType& theManager);

private:
	static const XalanDOMChar	m_type[];

	const XalanNode*	m_styleNode;

	static const XalanDOMString		s_emptyString;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALANXPATHEXCEPTION_HEADER_GUARD_1357924680
