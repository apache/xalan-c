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
#include "XStringBase.hpp"



#include <xalanc/XalanDOM/XalanText.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/DoubleSupport.hpp>



#include "XObjectTypeCallback.hpp"
#include "XPathExecutionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



XStringBase::XStringBase() :
	XObject(eTypeString),
	m_cachedNumberValue(0.0),
	m_resultTreeFrag(*this)
{
}



XStringBase::XStringBase(const XStringBase&		source) :
	XObject(source),
	m_cachedNumberValue(source.m_cachedNumberValue),
	m_resultTreeFrag(*this)
{
}



XStringBase::~XStringBase()
{
}



XalanDOMString
XStringBase::getTypeString() const
{
	return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("#STRING"));
}



double
XStringBase::num() const
{
	if (m_cachedNumberValue == 0.0)
	{
#if defined(XALAN_NO_MUTABLE)
		((XStringBase*)this)->m_cachedNumberValue = DoubleSupport::toDouble(str());
#else
		m_cachedNumberValue = DoubleSupport::toDouble(str());
#endif
	}

	return m_cachedNumberValue;
}



bool
XStringBase::boolean() const
{
	return length(str()) > 0 ? true : false;
}



const XalanDocumentFragment&
XStringBase::rtree() const
{
	return m_resultTreeFrag;
}



void
XStringBase::ProcessXObjectTypeCallback(XObjectTypeCallback&	theCallbackObject)
{
	theCallbackObject.String(*this,
							 str());
}



void
XStringBase::ProcessXObjectTypeCallback(XObjectTypeCallback&	theCallbackObject) const
{
	theCallbackObject.String(*this,
							 str());
}



XALAN_CPP_NAMESPACE_END
