/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
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

#include "Arg.hpp"



#include <PlatformSupport/DOMStringHelper.hpp>



Arg::Arg(
		const QName&			name,
		const XalanDOMString&	expr,
		bool					isParamVar) :
	StackEntry(eArgument),
	m_qname(name),
	m_argType(eExpression),
	m_isParamVar(isParamVar),
	m_val(0),
	m_expression(expr)
{
}



Arg::Arg(
		const QName&	name,
		XObject*		val,
		bool			isParamVar) :
	StackEntry(eArgument),
	m_qname(name),
	m_argType(eXObject),
	m_isParamVar(isParamVar),
	m_val(val),
	m_expression()
{
}



Arg::Arg(const Arg&	theSource) :
	StackEntry(theSource),
	m_qname(theSource.m_qname),
	m_argType(theSource.m_argType),
	m_isParamVar(theSource.m_isParamVar),
	m_val(theSource.m_val),
	m_expression(theSource.m_expression)
{
}



Arg::~Arg()
{
}



bool
Arg::equals(const StackEntry&	theRHS) const
{
	bool	theResult = false;

	if(theRHS.getType() == eArgument)
	{
		const Arg&	theArgRHS = static_cast<const Arg&>(theRHS);

		if (getArgType() == theArgRHS.getArgType() &&
			m_qname.equals(theArgRHS.m_qname) == true)
		{
			theResult = true;
		}
	}

	return theResult;
}


#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
StackEntry*
#else
Arg*
#endif
Arg::clone() const
{
	return new Arg(*this);
}
