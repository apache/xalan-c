/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights 
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
#if !defined(XSLEXCEPTION_HEADER_GUARD_1357924680)
#define XSLEXCEPTION_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <PlatformSupport/PlatformSupportDefinitions.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/XalanLocator.hpp>



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
			const XalanDOMString&	theType = XalanDOMString(XALAN_STATIC_UCODE_STRING("XSLException")));

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
			const XalanDOMString&	theType = XalanDOMString(XALAN_STATIC_UCODE_STRING("XSLException")));

	/**
	 * Constructor
	 * 
	 * @param theMessage message to write when exception thrown
	 * @param theType type of exception, default is "XSLException"
	 */
	XSLException(
			const XalanDOMString&	theMessage,
			const XalanDOMString&	theType = XalanDOMString(XALAN_STATIC_UCODE_STRING("XSLException")));

	virtual
	~XSLException();

	/**
	 * Retrieve type of exception
	 * 
	 * @return type of exception
	 */
	const XalanDOMString&
	getType() const
	{
		return m_type;
	}

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

	XalanDOMString
	defaultFormat() const;

	void
	defaultFormat(XalanDOMString&	theBuffer) const;

	static void
	defaultFormat(
			const XalanDOMString&	theMessage,
			const XalanDOMString&	theURI,
			size_type				theLineNumber,
			size_type				theColumnNumber,
			const XalanDOMString&	theType,
			XalanDOMString&			theBuffer)
	{
		defaultFormat(
			theMessage.c_str(),
			theMessage.size(),
			theURI.c_str(),
			theURI.size(),
			theLineNumber,
			theColumnNumber,
			theType.c_str(),
			theType.size(),
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

	const XalanDOMString	m_type;
};



XALAN_CPP_NAMESPACE_END



#endif	// XSLEXCEPTION_HEADER_GUARD_1357924680
