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
#if !defined(XALAN_AVT_HEADER_GUARD)
#define XALAN_AVT_HEADER_GUARD 

// Base include file.  Must be first.
#include <xalanc/XSLT/XSLTDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



XALAN_DECLARE_XERCES_CLASS(Locator)



XALAN_CPP_NAMESPACE_BEGIN



typedef XERCES_CPP_NAMESPACE_QUALIFIER Locator	LocatorType;



class AVTPart;
class PrefixResolver;
class XPathExecutionContext;
class XalanNode;
class StringTokenizer;
class StylesheetConstructionContext;



/**
 * Class to hold an Attribute Value Template.
 */
class AVT
{
public:

#if defined(XALAN_STRICT_ANSI_HEADERS)
	typedef std::size_t		size_type;
#else
	typedef size_t			size_type;
#endif

	/**
	 * Construct an Attribute Value Template(AVT) by parsing the string, and
	 * either constructing a vector of AVTParts, or simply hold on to the
	 * string if the AVT is simple.
	 *
	 * @param constructionContext context for construction of AVT
	 * @param locator		  the Locator for the AVT.  May be null.
	 * @param name                name of AVT
	 * @param stringedValue       string value to parse
	 * @param resolver            resolver for namespace resolution
	 */
	AVT(
			StylesheetConstructionContext&	constructionContext,
			const LocatorType*				locator,
			const XalanDOMChar*				name,
			const XalanDOMChar*				stringedValue,
			const PrefixResolver&			resolver);

	virtual
	~AVT();

	/**
	 * Retrieve the name of the Attribute Value Template
	 * 
	 * @return name of AVT
	 */
    const XalanDOMString&
	getName() const
	{
		return m_name;
	}

	/**
	 * Append the value to the buffer.
	 *
	 * @param buf              buffer to write into
	 * @param contextNode      current context node
	 * @param prefixResolver   prefix resolver to use
	 * @param executionContext execution context
	 */
	void
	evaluate(
			XalanDOMString&			buf,
			XalanNode*				contextNode,
			const PrefixResolver&	prefixResolver,
			XPathExecutionContext&	executionContext) const
	{
		if(m_simpleString != 0)
		{
			buf.assign(m_simpleString, m_simpleStringLength);
		}
		else
		{
			doEvaluate(buf, contextNode, prefixResolver, executionContext);
		}
	}

	/**
	 * Append the value to the buffer.
	 *
	 * @param buf              buffer to write into
	 * @param prefixResolver   prefix resolver to use
	 * @param executionContext execution context
	 */
	void
	evaluate(
			XalanDOMString&			buf,
			const PrefixResolver&	prefixResolver,
			XPathExecutionContext&	executionContext) const
	{
		if(m_simpleString != 0)
		{
			buf.assign(m_simpleString, m_simpleStringLength);
		}
		else
		{
			doEvaluate(buf, prefixResolver, executionContext);
		}
	}

private:

	void
	doEvaluate(
			XalanDOMString&			buf,
			const PrefixResolver&	prefixResolver,
			XPathExecutionContext&	executionContext) const;

	void
	doEvaluate(
			XalanDOMString&			buf,
			XalanNode*				contextNode,
			const PrefixResolver&	prefixResolver,
			XPathExecutionContext&	executionContext) const;

	void
	nextToken(
			StylesheetConstructionContext&	constructionContext,
			const LocatorType*				locator,
			StringTokenizer&				tokenizer,
			XalanDOMString&					token);

	// not implemented
	AVT(const AVT&);

	AVT&
	operator=(const AVT&);

	bool
	operator==(const AVT&) const;


	// Data members...
	const AVTPart**					m_parts;

	size_type						m_partsSize;

	const XalanDOMChar*				m_simpleString;

	XalanDOMString::size_type		m_simpleStringLength;

	const XalanDOMString&			m_name;

	static const XalanDOMString		s_emptyString;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALAN_AVT_HEADER_GUARD
