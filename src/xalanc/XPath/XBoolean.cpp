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
#include "XBoolean.hpp"



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>



#include "XObjectTypeCallback.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XBoolean::XBoolean(bool		val) :
	XObject(eTypeBoolean),
	m_value(val)
{
}



XBoolean::XBoolean(const XBoolean&	source) :
	XObject(source),
	m_value(source.m_value)
{
}



XBoolean::~XBoolean()
{
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XObject*
#else
XBoolean*
#endif
XBoolean::clone(void*	theAddress) const
{
	if (theAddress == 0)
	{
		return new XBoolean(*this);
	}
	else
	{
		return new (theAddress) XBoolean(*this);
	}
}



XalanDOMString
XBoolean::getTypeString() const
{
	return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("#BOOLEAN"));
}



double
XBoolean::num() const
{
	return number(m_value);
}



bool
XBoolean::boolean() const
{
	return m_value;
}



const XalanDOMString&
XBoolean::str() const
{
	return string(m_value);
}



void
XBoolean::str(
			FormatterListener&	formatterListener,
			MemberFunctionPtr	function) const
{
	string(m_value, formatterListener, function);
}



double
XBoolean::stringLength() const
{
	return m_value == true ? s_trueString.length() : s_falseString.length();
}


	
void
XBoolean::ProcessXObjectTypeCallback(XObjectTypeCallback&	theCallbackObject)
{
	theCallbackObject.Boolean(*this,
							  boolean());
}



void
XBoolean::ProcessXObjectTypeCallback(XObjectTypeCallback&	theCallbackObject) const
{
	theCallbackObject.Boolean(*this,
							  boolean());
}



XALAN_CPP_NAMESPACE_END
