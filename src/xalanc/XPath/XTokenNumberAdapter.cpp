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
#include "XTokenNumberAdapter.hpp"



#include "XToken.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XTokenNumberAdapter::XTokenNumberAdapter(const XToken&	theToken) :
	XNumberBase(),
	m_value(theToken)
{
}



XTokenNumberAdapter::XTokenNumberAdapter(const XTokenNumberAdapter&		source) :
	XNumberBase(source),
	m_value(source.m_value)
{
}



XTokenNumberAdapter::~XTokenNumberAdapter()
{
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XObject*
#else
XTokenNumberAdapter*
#endif
XTokenNumberAdapter::clone(void*	theAddress) const
{
	if (theAddress == 0)
	{
		return new XTokenNumberAdapter(*this);
	}
	else
	{
		return new (theAddress) XTokenNumberAdapter(*this);
	}
}



double
XTokenNumberAdapter::num() const
{
	return m_value.num();
}



const XalanDOMString&
XTokenNumberAdapter::str() const
{
	return m_value.str();
}



void
XTokenNumberAdapter::str(
			FormatterListener&	formatterListener,
			MemberFunctionPtr	function) const
{
	m_value.str(formatterListener, function);
}



void
XTokenNumberAdapter::str(XalanDOMString&	theBuffer) const
{
	m_value.str(theBuffer);
}



double
XTokenNumberAdapter::stringLength() const
{
	return m_value.stringLength();
}



XTokenNumberAdapter::eObjectType
XTokenNumberAdapter::getRealType() const
{
	return eTypeXTokenNumberAdapter;
}



XALAN_CPP_NAMESPACE_END
