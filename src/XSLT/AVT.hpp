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
#if !defined(XALAN_AVT_HEADER_GUARD)
#define XALAN_AVT_HEADER_GUARD 

// Base include file.  Must be first.
#include <XSLT/XSLTDefinitions.hpp>



#include <XalanDOM/XalanDOMString.hpp>



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
