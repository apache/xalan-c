/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2004 The Apache Software Foundation.  All rights 
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
// Class header file.
#include "XToken.hpp"



#include <xalanc/PlatformSupport/DoubleSupport.hpp>



#include "XObjectTypeCallback.hpp"



XALAN_CPP_NAMESPACE_BEGIN



static const XalanDOMString		s_emptyString;

XToken::XToken() :
	XObject(eTypeString),
	m_stringValue(&s_emptyString),
	m_numberValue(DoubleSupport::getNaN()),
	m_isString(true)
{
}



XToken::XToken(const XalanDOMString&	theString) :
	XObject(eTypeString),
	m_stringValue(&theString),
	m_numberValue(DoubleSupport::toDouble(theString)),
	m_isString(true)
{
}



XToken::XToken(
			double					theNumber,
			const XalanDOMString&	theString) :
	XObject(eTypeString),
	m_stringValue(&theString),
	m_numberValue(theNumber),
	m_isString(false)
{
}



XToken::XToken(const XToken&	theSource) :
	XObject(theSource),
	m_stringValue(theSource.m_stringValue),
	m_numberValue(theSource.m_numberValue),
	m_isString(theSource.m_isString)
{
	assert(m_stringValue != 0);
}



XToken::~XToken()
{
	assert(m_stringValue != 0);
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XObject*
#else
XToken*
#endif
XToken::clone(void*	theAddress) const
{
	assert(m_stringValue != 0);

	if (theAddress == 0)
	{
		return new XToken(*this);
	}
	else
	{
		return new (theAddress) XToken(*this);
	}
}



XalanDOMString
XToken::getTypeString() const
{
	assert(m_stringValue != 0);

	return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("#TOKEN"));
}



double
XToken::num() const
{
	assert(m_stringValue != 0);

	return m_numberValue;
}



bool
XToken::boolean() const
{
	assert(m_stringValue != 0);

	return m_isString == true ? XObject::boolean(*m_stringValue) : XObject::boolean(m_numberValue);
}



const XalanDOMString&
XToken::str() const
{
	assert(m_stringValue != 0);

	return *m_stringValue;
}



void
XToken::str(
			FormatterListener&	formatterListener,
			MemberFunctionPtr	function) const
{
	assert(m_stringValue != 0);
	assert(m_stringValue->length() == FormatterListener::size_type(m_stringValue->length()));

	(formatterListener.*function)(m_stringValue->c_str(), FormatterListener::size_type(m_stringValue->length()));
}



void
XToken::str(XalanDOMString&		theBuffer) const
{
	assert(m_stringValue != 0);

	append(theBuffer, *m_stringValue);
}



double
XToken::stringLength() const
{
	assert(m_stringValue != 0);

	return length(*m_stringValue);
}



void
XToken::ProcessXObjectTypeCallback(XObjectTypeCallback&		theCallbackObject)
{
	assert(m_stringValue != 0);

	if (m_isString == true)
	{
		theCallbackObject.String(*this, *m_stringValue);
	}
	else
	{
		theCallbackObject.Number(*this, m_numberValue);
	}
}



void
XToken::ProcessXObjectTypeCallback(XObjectTypeCallback&		theCallbackObject) const
{
	assert(m_stringValue != 0);

	if (m_isString == true)
	{
		theCallbackObject.String(*this, *m_stringValue);
	}
	else
	{
		theCallbackObject.Number(*this, m_numberValue);
	}
}



void
XToken::set(const XalanDOMString&	theString)
{
	m_stringValue = &theString;

	m_numberValue = DoubleSupport::toDouble(theString);

	m_isString = true;
}



void
XToken::set(
			double					theNumber,
			const XalanDOMString&	theString)
{
	assert(theString == DoubleToDOMString(theNumber));

	m_stringValue = &theString;

	m_numberValue = theNumber;

	m_isString = false;
}



void 
XToken::referenced()
{
}



void 
XToken::dereferenced()
{
}



XALAN_CPP_NAMESPACE_END
