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
#if !defined(EXSLT_STRINGIMPL_HEADER_GUARD_1357924680)
#define EXSLT_STRINGIMPL_HEADER_GUARD_1357924680



#include <xalanc/XalanEXSLT/XalanEXSLTDefinitions.hpp>



#include <xalanc/XPath/Function.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionAlign : public Function
{
public:

	typedef Function	ParentType;

	XalanEXSLTFunctionAlign()
	{
	}

	virtual
	~XalanEXSLTFunctionAlign()
	{
	}

	// These methods are inherited from Function ...

	virtual XObjectPtr
	execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const LocatorType*				locator) const;

#if !defined(XALAN_NO_USING_DECLARATION)
	using ParentType::execute;
#endif

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual XalanEXSLTFunctionAlign*
#endif
	clone() const
	{
		return new XalanEXSLTFunctionAlign(*this);
	}

protected:

	const XalanDOMString
	getError() const
	{
		return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("The EXSLT function align() accepts two or three arguments"));
	}

private:

	// Not implemented...
	XalanEXSLTFunctionAlign&
	operator=(const XalanEXSLTFunctionAlign&);

	bool
	operator==(const XalanEXSLTFunctionAlign&) const;


	// Data members...
	static const XalanDOMChar	s_centerString[];
	static const XalanDOMChar	s_rightString[];
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionConcat : public Function
{
public:

	typedef Function	ParentType;

	XalanEXSLTFunctionConcat() :
		Function()
	{
	}

	virtual
	~XalanEXSLTFunctionConcat()
	{
	}

	// These methods are inherited from Function ...

	virtual XObjectPtr
	execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const LocatorType*				locator) const;

#if !defined(XALAN_NO_USING_DECLARATION)
	using ParentType::execute;
#endif

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual XalanEXSLTFunctionConcat*
#endif
	clone() const
	{
		return new XalanEXSLTFunctionConcat(*this);
	}

protected:

	const XalanDOMString
	getError() const
	{
		return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("The EXSLT function concat() accepts one argument"));
	}

private:

	// Not implemented...
	XalanEXSLTFunctionConcat&
	operator=(const XalanEXSLTFunctionConcat&);

	bool
	operator==(const XalanEXSLTFunctionConcat&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionPadding : public Function
{
public:

	typedef Function	ParentType;

	XalanEXSLTFunctionPadding() :
		Function(),
		m_space(s_spaceString)
	{
	}

	// A dummy constructor for use internally.  Do not use this one!!!!
	XalanEXSLTFunctionPadding(int	/* theDummy */) :
		Function(),
		m_space()
	{
	}

	virtual
	~XalanEXSLTFunctionPadding()
	{
	}

	// These methods are inherited from Function ...

	virtual XObjectPtr
	execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const LocatorType*				locator) const;

#if !defined(XALAN_NO_USING_DECLARATION)
	using ParentType::execute;
#endif

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual XalanEXSLTFunctionPadding*
#endif
	clone() const
	{
		return new XalanEXSLTFunctionPadding;
	}

protected:

	const XalanDOMString
	getError() const
	{
		return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("The EXSLT function padding() accepts one or two arguments"));
	}

private:

	// Not implemented...
	XalanEXSLTFunctionPadding(const XalanEXSLTFunctionPadding&);

	XalanEXSLTFunctionPadding&
	operator=(const XalanEXSLTFunctionPadding&);

	bool
	operator==(const XalanEXSLTFunctionPadding&) const;


	// Data members...
	const XalanDOMString		m_space;

	static const XalanDOMChar	s_spaceString[];
};



XALAN_CPP_NAMESPACE_END



#endif	// EXSLT_STRINGIMPL_HEADER_GUARD_1357924680
