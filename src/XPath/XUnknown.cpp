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
// Class header file.
#include "XUnknown.hpp"



#include <PlatformSupport/DOMStringHelper.hpp>



#include "XObjectTypeCallback.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XalanDOMString	XUnknown::s_unknownVariableString;

XalanDOMString	XUnknown::s_unknownString;


XUnknown::XUnknown(const XalanDOMString&	name) :
	XObject(eTypeUnknown),
	m_value(s_unknownVariableString + name)
{
}



XUnknown::XUnknown(const XUnknown&	source) :
	XObject(source),
	m_value(source.m_value)
{
}



XUnknown::~XUnknown()
{
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XObject*
#else
XUnknown*
#endif
XUnknown::clone(void*	theAddress) const
{
	if (theAddress == 0)
	{
		return new XUnknown(*this);
	}
	else
	{
		return new (theAddress) XUnknown(*this);
	}
}



XalanDOMString
XUnknown::getTypeString() const
{
	return s_unknownString;
}



double
XUnknown::num() const
{
	return 0.0;
}



bool
XUnknown::boolean() const
{
	return false;
}



const XalanDOMString&
XUnknown::str() const
{
	return m_value;
}



void
XUnknown::str(
			FormatterListener&	formatterListener,
			MemberFunctionPtr	function) const
{
	assert(length(m_value) == FormatterListener::size_type(length(m_value)));

	(formatterListener.*function)(c_wstr(m_value), FormatterListener::size_type(length(m_value)));
}



double
XUnknown::stringLength() const
{
	return length(m_value);
}



void
XUnknown::ProcessXObjectTypeCallback(XObjectTypeCallback&	theCallbackObject)
{
	theCallbackObject.Unknown(*this,
							  m_value);
}



void
XUnknown::ProcessXObjectTypeCallback(XObjectTypeCallback&	theCallbackObject) const
{
	theCallbackObject.Unknown(*this,
							  m_value);
}



void
XUnknown::initialize()
{
	s_unknownVariableString = XALAN_STATIC_UCODE_STRING("Unknown variable: ");
	s_unknownString = XALAN_STATIC_UCODE_STRING("#UNKNOWN");
}



void
XUnknown::terminate()
{
	releaseMemory(s_unknownVariableString);
	releaseMemory(s_unknownString);
}



XALAN_CPP_NAMESPACE_END
