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
// Class header file.
#include "XNumber.hpp"



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



XALAN_CPP_NAMESPACE_BEGIN



XNumber::XNumber(double		val) :
	XNumberBase(),
	m_value(val),
	m_cachedStringValue()
{
}



XNumber::XNumber(const XNumber&		source) :
	XNumberBase(source),
	m_value(source.m_value),
	m_cachedStringValue(source.m_cachedStringValue)
{
}



XNumber::~XNumber()
{
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XObject*
#else
XNumber*
#endif
XNumber::clone(void*	theAddress) const
{
	if (theAddress == 0)
	{
		return new XNumber(*this);
	}
	else
	{
		return new (theAddress) XNumber(*this);
	}
}



double
XNumber::num() const
{
	return m_value;
}



const XalanDOMString&
XNumber::str() const
{
	if (isEmpty(m_cachedStringValue) == true)
	{
#if defined(XALAN_NO_MUTABLE)
		DoubleToDOMString(m_value, ((XNumber*)this)->m_cachedStringValue);
#else
		DoubleToDOMString(m_value, m_cachedStringValue);
#endif
	}

	return m_cachedStringValue;
}



void
XNumber::str(
			FormatterListener&	formatterListener,
			MemberFunctionPtr	function) const
{
	const XalanDOMString&	theValue = str();

	assert(length(theValue) == FormatterListener::size_type(length(theValue)));

	(formatterListener.*function)(c_wstr(theValue), FormatterListener::size_type(length(theValue)));
}



void
XNumber::str(XalanDOMString&	theBuffer) const
{
	if (isEmpty(m_cachedStringValue) == false)
	{
		append(theBuffer, m_cachedStringValue);
	}
	else
	{
		DoubleToDOMString(m_value, theBuffer);
	}
}



void
XNumber::set(double		theValue)
{
	m_value = theValue;

	clear(m_cachedStringValue);
}



double
XNumber::stringLength() const
{
	return length(str());
}



XALAN_CPP_NAMESPACE_END
