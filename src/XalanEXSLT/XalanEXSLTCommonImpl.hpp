/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2002-2003 The Apache Software Foundation.  All rights 
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
#if !defined(EXSLT_COMMONIMPL_HEADER_GUARD_1357924680)
#define EXSLT_COMMONIMPL_HEADER_GUARD_1357924680



#include <XalanEXSLT/XalanEXSLTDefinitions.hpp>



#include <XPath/Function.hpp>



#include <XalanExtensions/FunctionNodeSet.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionNodeSet : public FunctionNodeSet
{
public:

	XalanEXSLTFunctionNodeSet() :
		FunctionNodeSet(true)
	{
	}

	virtual
	~XalanEXSLTFunctionNodeSet()
	{
	}

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual XalanEXSLTFunctionNodeSet*
#endif
	clone() const
	{
		return new XalanEXSLTFunctionNodeSet(*this);
	}

protected:

	virtual const XalanDOMString
	getError() const
	{
		return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("The EXSLT function node-set() accepts one argument"));
	}

	virtual const XalanDOMString
	getInvalidArgumentTypeError() const
	{
		return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("Invalid argument type in EXSLT function node-set()"));
	}

private:

	// Not implemented...
	XalanEXSLTFunctionNodeSet&
	operator=(const XalanEXSLTFunctionNodeSet&);

	bool
	operator==(const XalanEXSLTFunctionNodeSet&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionObjectType : public Function
{
public:

	typedef Function	ParentType;

	XalanEXSLTFunctionObjectType() :
		Function(),
		m_boolean(s_booleanString),
		m_external(s_externalString),
		m_nodeSet(s_nodeSetString),
		m_number(s_numberString),
		m_rtf(s_rtfString),
		m_string(s_stringString)
	{
	}

	// A dummy constructor for use internally.  Do not use this one!!!!
	XalanEXSLTFunctionObjectType(int	/* theDummy */) :
		Function(),
		m_boolean(),
		m_external(),
		m_nodeSet(),
		m_number(),
		m_rtf(),
		m_string()
	{
	}

	virtual
	~XalanEXSLTFunctionObjectType()
	{
	}

	// These methods are inherited from Function ...

#if !defined(XALAN_NO_USING_DECLARATION)
	using ParentType::execute;
#endif

	virtual XObjectPtr
	execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const LocatorType*				locator) const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual XalanEXSLTFunctionObjectType*
#endif
	clone() const
	{
		return new XalanEXSLTFunctionObjectType;
	}

protected:

	const XalanDOMString
	getError() const;

private:

	// Not implemented...
	XalanEXSLTFunctionObjectType(const XalanEXSLTFunctionObjectType&);

	XalanEXSLTFunctionObjectType&
	operator=(const XalanEXSLTFunctionObjectType&);

	bool
	operator==(const XalanEXSLTFunctionObjectType&) const;


	// Data members...
	const XalanDOMString	m_boolean;
	const XalanDOMString	m_external;
	const XalanDOMString	m_nodeSet;
	const XalanDOMString	m_number;
	const XalanDOMString	m_rtf;
	const XalanDOMString	m_string;

	static const XalanDOMChar	s_booleanString[];
	static const XalanDOMChar	s_externalString[];
	static const XalanDOMChar	s_nodeSetString[];
	static const XalanDOMChar	s_numberString[];
	static const XalanDOMChar	s_rtfString[];
	static const XalanDOMChar	s_stringString[];
};



XALAN_CPP_NAMESPACE_END



#endif	// EXSLT_COMMONIMPL_HEADER_GUARD_1357924680
