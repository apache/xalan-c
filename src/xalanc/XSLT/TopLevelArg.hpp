/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2003 The Apache Software Foundation.  All rights 
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
 *
 * $ Id: $
 *
 */

#if !defined(XALAN_TOPLEVELARG_HEADER_GUARD)
#define XALAN_TOPLEVELARG_HEADER_GUARD




// Base include file.  Must be first.
#include <xalanc/XSLT/XSLTDefinitions.hpp>



#include <xalanc/XalanDOM/XalanDOMString.hpp>



#include <xalanc/XPath/XalanQNameByValue.hpp>
#include <xalanc/XPath/XObject.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XObjectPtr;



/**
 * This class holds an instance of an argument on the stack.
 */
class XALAN_XSLT_EXPORT TopLevelArg
{
public:

	/**
	 * Construct an argument object from a string expression
	 * 
	 * @param name	name of argument
	 * @param expr	expression argument represents
	 */
	TopLevelArg(
		const XalanQName&		name,
		const XalanDOMString&	expr);

	/**
	 * Construct an argument object from an XObject instance.
	 * 
	 * @param name	name of argument
	 * @param variable	the XObject instance.
	 */
	TopLevelArg(
		const XalanQName&	name = XalanQNameByValue(),
		const XObjectPtr	variable = XObjectPtr());

	/**
	 * Copy constructor
	 * 
	 * @param theSource	the TopLevelArg to copy.
	 */
	TopLevelArg(const TopLevelArg&	theSource);

	/**
	 * Destructor
	 */
	~TopLevelArg();

	/**
	 * Retrieve object name
	 * 
	 * @return qualified name of object
	 */
	const XalanQName&
	getName() const
	{
		return m_qname;
	}

	/**
	 * Retrieve object's expression
	 * 
	 * @return string representation of expression
	 */
	const XalanDOMString&
	getExpression() const
	{
		return m_expression;
	};

	/**
	 * Retrieve object's XObject variable.
	 * 
	 * @return pointer to the XObject instance
	 */
	const XObjectPtr
	getXObject() const
	{
		return m_xobject;
	}

	/**
	 * Assignment operator
	 */
	TopLevelArg&
	operator=(const TopLevelArg&	theRHS)
	{
		if (&theRHS != this)
		{
			m_qname = theRHS.m_qname;

			m_expression = theRHS.m_expression;
		}

		return *this;
	}

private:

	XalanQNameByValue	m_qname;

	XalanDOMString		m_expression;

	const XObjectPtr	m_xobject;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALAN_TOPLEVELARG_HEADER_GUARD
