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
#include "XString.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XString::XString(const XalanDOMString&	val,
                 MemoryManagerType& theManager) :
	XStringBase(theManager),
	m_value(val,theManager)
{
}



XString::XString(const XalanDOMChar*	val,
                 MemoryManagerType& theManager) :
	XStringBase(theManager),
	m_value(val, theManager)
{
}



XString::XString(
			const XalanDOMChar*		val,
			unsigned int			len,
            MemoryManagerType&      theManager) :
	XStringBase(theManager),
	m_value(val, theManager, len)
{
}



XString::XString(const XString&	source,MemoryManagerType& theManager) :
	XStringBase(source, theManager),
	m_value(source.m_value, theManager)
{
}



XString::~XString()
{
}


const XalanDOMString&
XString::str() const
{
	return m_value;
}



void
XString::str(
			FormatterListener&	formatterListener,
			MemberFunctionPtr	function) const
{
	assert(length(m_value) == FormatterListener::size_type(length(m_value)));

	(formatterListener.*function)(c_wstr(m_value), FormatterListener::size_type(length(m_value)));
}



double
XString::stringLength() const
{
	return length(m_value);
}



XALAN_CPP_NAMESPACE_END
