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
#if !defined(XALAN_KEYDECLARATION_HEADER_GUARD)
#define XALAN_KEYDECLARATION_HEADER_GUARD



// Base include file.  Must be first.
#include "XSLTDefinitions.hpp"



XALAN_CPP_NAMESPACE_BEGIN



#include <xalanc/PlatformSupport/XalanLocator.hpp>



class XPath;



/**
 * Holds the attribute declarations for the "xsl:key" element.
 */
class KeyDeclaration
{
public:

	/**
	 * Construct an object containing attributes of an "xsl:key" element
	 * 
	 * @param qname name of element
	 * @param matchPattern XPath for "match" attribute
	 * @param use XPath for "use" attribute
	 */
	KeyDeclaration(
			const XalanQName&			qname,
			const XPath&				matchPattern,
			const XPath&				use,
			const XalanDOMString&		uri,
			XalanLocator::size_type		lineNumber,
			XalanLocator::size_type		columnNumber) :
		m_qname(&qname),
		m_match(&matchPattern),
		m_use(&use),
		m_uri(&uri),
		m_lineNumber(lineNumber),
		m_columnNumber(columnNumber)
	{
	}

	explicit
	KeyDeclaration() :
		m_qname(0),
		m_match(0),
		m_use(0),
		m_uri(0),
		m_lineNumber(0),
		m_columnNumber(0)
	{
	}

	KeyDeclaration(const KeyDeclaration&	theSource) :
		m_qname(theSource.m_qname),
		m_match(theSource.m_match),
		m_use(theSource.m_use)
	{
	}

	~KeyDeclaration()
	{
	}

	/**
	 * Retrieves name of element
	 * 
	 * @return name string
	 */
	const XalanQName*
	getQName() const
	{
		return m_qname;
	}

	/**
	 * Retrieves "use" XPath
	 * 
	 * @return XPath for "use" attribute
	 */
	const XPath*
	getUse() const
	{
		return m_use;
	}

	/**
	 * Retrieves "match" XPath
	 * 
	 * @return XPath for "match" attribute
	 */
	const XPath*
	getMatchPattern() const
	{
		return m_match;
	}

	/**
	 * Retrieves the URI.
	 * 
	 * @return A pointer to a URI string.
	 */
	const XalanDOMString*
	getURI() const
	{
		return m_uri;
	}

	/**
	 * Retrieves the line number where the xsl:key element occurred.
	 *
	 * @return The line number
	 */
	XalanLocator::size_type
	getLineNumber() const
	{
		return m_lineNumber;
	}

	/**
	 * Retrieves the column number where the xsl:key element occurred.
	 *
	 * @return The column number
	 */
	XalanLocator::size_type
	getColumnNumber() const
	{
		return m_columnNumber;
	}

private:

	const XalanQName*			m_qname;

	const XPath*				m_match;

	const XPath*				m_use;

	const XalanDOMString*		m_uri;

	XalanLocator::size_type		m_lineNumber;

	XalanLocator::size_type		m_columnNumber;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALAN_KEYDECLARATION_HEADER_GUARD
