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
#include "XNull.hpp"



#include "XObjectTypeCallback.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XNull::XNull() :
	XObject(eTypeNull)
{
}



XNull::XNull(const XNull&	source) :
	XObject(source)
{
}



XNull::~XNull()
{
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XObject*
#else
XNull*
#endif
XNull::clone(void*	theAddress) const
{
	if (theAddress == 0)
	{
		return new XNull(*this);
	}
	else
	{
		return new (theAddress) XNull(*this);
	}
}



XalanDOMString
XNull::getTypeString() const
{
	return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("#NULL"));
}



double
XNull::num() const
{
	return 0.0;
}



bool
XNull::boolean() const
{
	return false;
}



const XalanDOMString&
XNull::str() const
{
	return s_nullString;
}



void
XNull::str(
			FormatterListener&	/* formatterListener */,
			MemberFunctionPtr	/* function */) const
{
}



double
XNull::stringLength() const
{
	return 0;
}



void
XNull::ProcessXObjectTypeCallback(XObjectTypeCallback&	theCallbackObject)
{
	theCallbackObject.Null(*this);
}



void
XNull::ProcessXObjectTypeCallback(XObjectTypeCallback&	theCallbackObject) const
{
	theCallbackObject.Null(*this);
}



XALAN_CPP_NAMESPACE_END
