/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2002 The Apache Software Foundation.  All rights 
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



#include <XalanEXSLT/XalanEXSLTDefinitions.hpp>



#include <XPath/Function.hpp>



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionAlign : public Function
{
public:

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
			const Locator*					locator) const;

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
		return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("The EXLT function align() accepts two or three arguments"));
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
			const Locator*					locator) const;

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
		return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("The EXLT function concat() accepts one argument"));
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
			const Locator*					locator) const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual XalanEXSLTFunctionPadding*
#endif
	clone() const
	{
		return new XalanEXSLTFunctionPadding(*this);
	}

protected:

	const XalanDOMString
	getError() const
	{
		return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("The EXLT function padding() accepts one or two arguments"));
	}

private:

	// Not implemented...
	XalanEXSLTFunctionPadding&
	operator=(const XalanEXSLTFunctionPadding&);

	bool
	operator==(const XalanEXSLTFunctionPadding&) const;


	// Data members...
	const XalanDOMString		m_space;

	static const XalanDOMChar	s_spaceString[];
};



#if 0
class XALAN_EXSLT_EXPORT XalanEXSLTFunctionAsin : public Function
{
public:

	XalanEXSLTFunctionAsin()
	{
	}

	virtual
	~XalanEXSLTFunctionAsin()
	{
	}

	// These methods are inherited from Function ...

	virtual XObjectPtr
	execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const Locator*					locator) const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual XalanEXSLTFunctionAsin*
#endif
	clone() const
	{
		return new XalanEXSLTFunctionAsin(*this);
	}

protected:

	const XalanDOMString
	getError() const;

private:

	// Not implemented...
	XalanEXSLTFunctionAsin&
	operator=(const XalanEXSLTFunctionAsin&);

	bool
	operator==(const XalanEXSLTFunctionAsin&) const;


	// Data members...
	static const XalanDOMString		s_emptyString;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionAtan : public Function
{
public:

	XalanEXSLTFunctionAtan()
	{
	}

	virtual
	~XalanEXSLTFunctionAtan()
	{
	}

	// These methods are inherited from Function ...

	virtual XObjectPtr
	execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const Locator*					locator) const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual XalanEXSLTFunctionAtan*
#endif
	clone() const
	{
		return new XalanEXSLTFunctionAtan(*this);
	}

protected:

	const XalanDOMString
	getError() const;

private:

	// Not implemented...
	XalanEXSLTFunctionAtan&
	operator=(const XalanEXSLTFunctionAtan&);

	bool
	operator==(const XalanEXSLTFunctionAtan&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionAtan2 : public Function
{
public:

	XalanEXSLTFunctionAtan2()
	{
	}

	virtual
	~XalanEXSLTFunctionAtan2()
	{
	}

	// These methods are inherited from Function ...

	virtual XObjectPtr
	execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const Locator*					locator) const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual XalanEXSLTFunctionAtan2*
#endif
	clone() const
	{
		return new XalanEXSLTFunctionAtan2(*this);
	}

protected:

	const XalanDOMString
	getError() const;

private:

	// Not implemented...
	XalanEXSLTFunctionAtan2&
	operator=(const XalanEXSLTFunctionAtan2&);

	bool
	operator==(const XalanEXSLTFunctionAtan2&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionConstant : public Function
{
public:

	XalanEXSLTFunctionConstant()
	{
	}

	virtual
	~XalanEXSLTFunctionConstant()
	{
	}

	// These methods are inherited from Function ...

	virtual XObjectPtr
	execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const Locator*					locator) const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual XalanEXSLTFunctionConstant*
#endif
	clone() const
	{
		return new XalanEXSLTFunctionConstant(*this);
	}

protected:

	const XalanDOMString
	getError() const
	{
		return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("The EXLT function constant() function accepts two arguments"));
	}

private:

	static const XalanDOMChar	s_eString[];
	static const XalanDOMChar	s_ln10String[];
	static const XalanDOMChar	s_ln2String[];
	static const XalanDOMChar	s_log2EString[];
	static const XalanDOMChar	s_piString[];
	static const XalanDOMChar	s_sqrt1_2String[];
	static const XalanDOMChar	s_sqrt2String[];

	static const char	s_eValueString[];
	static const char	s_ln10ValueString[];
	static const char	s_ln2ValueString[];
	static const char	s_log2EValueString[];
	static const char	s_piValueString[];
	static const char	s_sqrt1_2ValueString[];
	static const char	s_sqrt2ValueString[];


	// Not implemented...
	XalanEXSLTFunctionConstant&
	operator=(const XalanEXSLTFunctionConstant&);

	bool
	operator==(const XalanEXSLTFunctionConstant&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionCos : public Function
{
public:

	XalanEXSLTFunctionCos()
	{
	}

	virtual
	~XalanEXSLTFunctionCos()
	{
	}

	// These methods are inherited from Function ...

	virtual XObjectPtr
	execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const Locator*					locator) const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual XalanEXSLTFunctionCos*
#endif
	clone() const
	{
		return new XalanEXSLTFunctionCos(*this);
	}

protected:

	const XalanDOMString
	getError() const;

private:

	// Not implemented...
	XalanEXSLTFunctionCos&
	operator=(const XalanEXSLTFunctionCos&);

	bool
	operator==(const XalanEXSLTFunctionCos&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionExp : public Function
{
public:

	XalanEXSLTFunctionExp()
	{
	}

	virtual
	~XalanEXSLTFunctionExp()
	{
	}

	// These methods are inherited from Function ...

	virtual XObjectPtr
	execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const Locator*					locator) const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual XalanEXSLTFunctionExp*
#endif
	clone() const
	{
		return new XalanEXSLTFunctionExp(*this);
	}

protected:

	const XalanDOMString
	getError() const;

private:

	// Not implemented...
	XalanEXSLTFunctionExp&
	operator=(const XalanEXSLTFunctionExp&);

	bool
	operator==(const XalanEXSLTFunctionExp&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionHighest : public Function
{
public:

	XalanEXSLTFunctionHighest()
	{
	}

	virtual
	~XalanEXSLTFunctionHighest()
	{
	}

	// These methods are inherited from Function ...

	virtual XObjectPtr
	execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const Locator*					locator) const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual XalanEXSLTFunctionHighest*
#endif
	clone() const
	{
		return new XalanEXSLTFunctionHighest(*this);
	}

protected:

	const XalanDOMString
	getError() const;

private:

	// Not implemented...
	XalanEXSLTFunctionHighest&
	operator=(const XalanEXSLTFunctionHighest&);

	bool
	operator==(const XalanEXSLTFunctionHighest&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionLog : public Function
{
public:

	XalanEXSLTFunctionLog()
	{
	}

	virtual
	~XalanEXSLTFunctionLog()
	{
	}

	// These methods are inherited from Function ...

	virtual XObjectPtr
	execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const Locator*					locator) const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual XalanEXSLTFunctionLog*
#endif
	clone() const
	{
		return new XalanEXSLTFunctionLog(*this);
	}

protected:

	const XalanDOMString
	getError() const;

private:

	// Not implemented...
	XalanEXSLTFunctionLog&
	operator=(const XalanEXSLTFunctionLog&);

	bool
	operator==(const XalanEXSLTFunctionLog&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionLowest : public Function
{
public:

	XalanEXSLTFunctionLowest()
	{
	}

	virtual
	~XalanEXSLTFunctionLowest()
	{
	}

	// These methods are inherited from Function ...

	virtual XObjectPtr
	execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const Locator*					locator) const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual XalanEXSLTFunctionLowest*
#endif
	clone() const
	{
		return new XalanEXSLTFunctionLowest(*this);
	}

protected:

	const XalanDOMString
	getError() const;

private:

	// Not implemented...
	XalanEXSLTFunctionLowest&
	operator=(const XalanEXSLTFunctionLowest&);

	bool
	operator==(const XalanEXSLTFunctionLowest&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionMax : public Function
{
public:

	XalanEXSLTFunctionMax()
	{
	}

	virtual
	~XalanEXSLTFunctionMax()
	{
	}

	// These methods are inherited from Function ...

	virtual XObjectPtr
	execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const Locator*					locator) const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual XalanEXSLTFunctionMax*
#endif
	clone() const
	{
		return new XalanEXSLTFunctionMax(*this);
	}

protected:

	const XalanDOMString
	getError() const;

private:

	// Not implemented...
	XalanEXSLTFunctionMax&
	operator=(const XalanEXSLTFunctionMax&);

	bool
	operator==(const XalanEXSLTFunctionMax&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionMin : public Function
{
public:

	XalanEXSLTFunctionMin()
	{
	}

	virtual
	~XalanEXSLTFunctionMin()
	{
	}

	// These methods are inherited from Function ...

	virtual XObjectPtr
	execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const Locator*					locator) const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual XalanEXSLTFunctionMin*
#endif
	clone() const
	{
		return new XalanEXSLTFunctionMin(*this);
	}

protected:

	const XalanDOMString
	getError() const;

private:

	// Not implemented...
	XalanEXSLTFunctionMin&
	operator=(const XalanEXSLTFunctionMin&);

	bool
	operator==(const XalanEXSLTFunctionMin&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionPower : public Function
{
public:

	XalanEXSLTFunctionPower()
	{
	}

	virtual
	~XalanEXSLTFunctionPower()
	{
	}

	// These methods are inherited from Function ...

	virtual XObjectPtr
	execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const Locator*					locator) const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual XalanEXSLTFunctionPower*
#endif
	clone() const
	{
		return new XalanEXSLTFunctionPower(*this);
	}

protected:

	const XalanDOMString
	getError() const;

private:

	// Not implemented...
	XalanEXSLTFunctionPower&
	operator=(const XalanEXSLTFunctionPower&);

	bool
	operator==(const XalanEXSLTFunctionPower&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionSin : public Function
{
public:

	XalanEXSLTFunctionSin()
	{
	}

	virtual
	~XalanEXSLTFunctionSin()
	{
	}

	// These methods are inherited from Function ...

	virtual XObjectPtr
	execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const Locator*					locator) const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual XalanEXSLTFunctionSin*
#endif
	clone() const
	{
		return new XalanEXSLTFunctionSin(*this);
	}

protected:

	const XalanDOMString
	getError() const;

private:

	// Not implemented...
	XalanEXSLTFunctionSin&
	operator=(const XalanEXSLTFunctionSin&);

	bool
	operator==(const XalanEXSLTFunctionSin&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionSqrt : public Function
{
public:

	XalanEXSLTFunctionSqrt()
	{
	}

	virtual
	~XalanEXSLTFunctionSqrt()
	{
	}

	// These methods are inherited from Function ...

	virtual XObjectPtr
	execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const Locator*					locator) const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual XalanEXSLTFunctionSqrt*
#endif
	clone() const
	{
		return new XalanEXSLTFunctionSqrt(*this);
	}

protected:

	const XalanDOMString
	getError() const;

private:

	// Not implemented...
	XalanEXSLTFunctionSqrt&
	operator=(const XalanEXSLTFunctionSqrt&);

	bool
	operator==(const XalanEXSLTFunctionSqrt&) const;
};



class XALAN_EXSLT_EXPORT XalanEXSLTFunctionTan : public Function
{
public:

	XalanEXSLTFunctionTan()
	{
	}

	virtual
	~XalanEXSLTFunctionTan()
	{
	}

	// These methods are inherited from Function ...

	virtual XObjectPtr
	execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const Locator*					locator) const;

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual XalanEXSLTFunctionTan*
#endif
	clone() const
	{
		return new XalanEXSLTFunctionTan(*this);
	}

protected:

	const XalanDOMString
	getError() const;

private:

	// Not implemented...
	XalanEXSLTFunctionTan&
	operator=(const XalanEXSLTFunctionTan&);

	bool
	operator==(const XalanEXSLTFunctionTan&) const;
};
#endif


#endif	// EXSLT_STRINGIMPL_HEADER_GUARD_1357924680
