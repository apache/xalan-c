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
#include "XUnknown.hpp"



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/XalanMessageLoader.hpp>



#include "XObjectTypeCallback.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XalanDOMString	XUnknown::s_unknownString;


XUnknown::XUnknown(const XalanDOMString&	name) :
	XObject(eTypeUnknown),
	m_value(XalanMessageLoader::getMessage(XalanMessages::UnknownVariable_1Param, name))
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
	s_unknownString = XALAN_STATIC_UCODE_STRING("#UNKNOWN");
}



void
XUnknown::terminate()
{
	releaseMemory(s_unknownString);
}



XALAN_CPP_NAMESPACE_END
