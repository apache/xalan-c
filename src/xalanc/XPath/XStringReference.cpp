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
#include "XStringReference.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XStringReference::XStringReference(const XalanDOMString&	val) :
	XStringBase(),
	m_value(val)
{
}



XStringReference::XStringReference(const XStringReference&	source) :
	XStringBase(source),
	m_value(source.m_value)
{
}



XStringReference::~XStringReference()
{
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XObject*
#else
XStringReference*
#endif
XStringReference::clone(void*	theAddress) const
{
	if (theAddress == 0)
	{
		return new XStringReference(*this);
	}
	else
	{
		return new (theAddress) XStringReference(*this);
	}
}



const XalanDOMString&
XStringReference::str() const
{
	return m_value;
}



void
XStringReference::str(
			FormatterListener&	formatterListener,
			MemberFunctionPtr	function) const
{
	assert(length(m_value) == FormatterListener::size_type(length(m_value)));

	(formatterListener.*function)(c_wstr(m_value), FormatterListener::size_type(length(m_value)));
}



XStringReference::eObjectType
XStringReference::getRealType() const
{
	return eTypeStringReference;
}



double
XStringReference::stringLength() const
{
	return length(m_value);
}



XALAN_CPP_NAMESPACE_END
