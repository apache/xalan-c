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
#include "XStringAdapter.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XStringAdapter::XStringAdapter(const XObjectPtr&	val) :
	XStringBase(),
	m_value(val)
{
}



XStringAdapter::XStringAdapter(const XStringAdapter&	source) :
	XStringBase(source),
	m_value(source.m_value)
{
}



XStringAdapter::~XStringAdapter()
{
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XObject*
#else
XStringAdapter*
#endif
XStringAdapter::clone(void*	theAddress) const
{
	if (theAddress == 0)
	{
		return new XStringAdapter(*this);
	}
	else
	{
		return new (theAddress) XStringAdapter(*this);
	}
}



const XalanDOMString&
XStringAdapter::str() const
{
	return m_value->str();
}



void
XStringAdapter::str(
			FormatterListener&	formatterListener,
			MemberFunctionPtr	function) const
{
	m_value->str(formatterListener, function);
}



XStringAdapter::eObjectType
XStringAdapter::getRealType() const
{
	return eTypeStringAdapter;
}



double
XStringAdapter::stringLength() const
{
	return m_value->stringLength();
}



XALAN_CPP_NAMESPACE_END
