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
#if !defined(XSLEXCEPTION_HEADER_GUARD_1357924680)
#define XSLEXCEPTION_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <xalanc/PlatformSupport/PlatformSupportDefinitions.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanLocator.hpp>



XALAN_CPP_NAMESPACE_BEGIN



typedef XERCES_CPP_NAMESPACE_QUALIFIER Locator	LocatorType;



class XALAN_PLATFORMSUPPORT_EXPORT XSLException
{
public:

	typedef XalanLocator::size_type					size_type;

	/**
	 * Constructor
	 * 
	 * @param theMessage message to write when exception thrown
	 * @param theURI the URI of the related document, if known
	 * @param theLineNumber the line number of the related document.
	 * @param theColumnNumber the column number of the related document.
	 * @param theType type of exception, default is "XSLException"
	 */
	XSLException(
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
	 * @param theType type of exception, default is "XSLException"
	 */
	XSLException(
			const LocatorType&		theLocator,
			const XalanDOMString&	theMessage,
            MemoryManagerType&      theManager);
	/**
	 * Constructor
	 * 
	 * @param theMessage message to write when exception thrown
	 * @param theType type of exception, default is "XSLException"
	 */
	XSLException(
			const XalanDOMString&	theMessage,
            MemoryManagerType&      theManager);


	XSLException(const XSLException&	other);

	virtual
	~XSLException();

	/**
	 * Retrieve type of exception
	 * 
	 * @return type of exception
	 */
	virtual const XalanDOMChar*
	getType() const = 0;

	/**
	 * Retrieve message for exception
	 * 
	 * @return exception message
	 */
	const XalanDOMString&
	getMessage() const
	{
		return m_message;
	}

	/**
	 * Get the URI for the associated document, if any
	 * 
	 * @return The URI.
	 */
	const XalanDOMString&
	getURI() const
	{
		return m_uri;
	}

	/**
	 * Retrieve the line number.
	 * 
	 * @return the line number
	 */
	size_type
	getLineNumber() const
	{
		return m_lineNumber;
	}

	/**
	 * Retrieve the column number.
	 * 
	 * @return the column number
	 */
	size_type
	getColumnNumber() const
	{
		return m_columnNumber;
	}


    MemoryManagerType& 
    getMemoryManager()const
    {
//#pragma message ("Breaks the const-correctness !")
        XalanDOMString* pnt = const_cast<XalanDOMString*>(&m_message);
        return pnt->getMemoryManager();
    }

	void
	defaultFormat(XalanDOMString&	theBuffer) const;

	static void
	defaultFormat(
			const XalanDOMString&	theMessage,
			const XalanDOMString&	theURI,
			size_type				theLineNumber,
			size_type				theColumnNumber,
			const XalanDOMChar*		theType,
			XalanDOMString&			theBuffer)
	{
		defaultFormat(
			theMessage.c_str(),
			theMessage.size(),
			theURI.c_str(),
			theURI.size(),
			theLineNumber,
			theColumnNumber,
			theType,
			XalanDOMString::length(theType),
			theBuffer);
	}

	static void
	defaultFormat(
			const XalanDOMChar*					theMessage,
			const XalanDOMChar*					theURI,
			size_type							theLineNumber,
			size_type							theColumnNumber,
			const XalanDOMChar*					theType,
			XalanDOMString&						theBuffer)
	{
		assert(theMessage != 0 && theURI != 0 && theType != 0);

		defaultFormat(
			theMessage,
			XalanDOMString::length(theMessage),
			theURI,
			XalanDOMString::length(theURI),
			theLineNumber,
			theColumnNumber,
			theType,
			XalanDOMString::length(theType),
			theBuffer);
	}

	static void
	defaultFormat(
			const XalanDOMChar*					theMessage,
			const XalanDOMString::size_type		theMessageLength,
			const XalanDOMChar*					theURI,
			const XalanDOMString::size_type		theURILength,
			size_type							theLineNumber,
			size_type							theColumnNumber,
			const XalanDOMChar*					theType,
			const XalanDOMString::size_type		theTypeLength,
			XalanDOMString&						theBuffer);

private:
	
	const XalanDOMString	m_message;
	const XalanDOMString	m_uri;

	const size_type			m_lineNumber;
	const size_type			m_columnNumber;


};



XALAN_CPP_NAMESPACE_END



#endif	// XSLEXCEPTION_HEADER_GUARD_1357924680
