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
#include "XNodeSetBase.hpp"



#include <xalanc/XalanDOM/XalanNode.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/DoubleSupport.hpp>



#include <xalanc/DOMSupport/DOMServices.hpp>



#include "FormatterStringLengthCounter.hpp"
#include "XObjectTypeCallback.hpp"
#include "XPathExecutionContext.hpp"



XALAN_CPP_NAMESPACE_BEGIN



const double	theBogusNumberValue = 123456789;



XNodeSetBase::XNodeSetBase() :
	XObject(eTypeNodeSet),
	m_proxy(*this),
	m_cachedStringValue(),
	m_cachedNumberValue(theBogusNumberValue)
{
}



XNodeSetBase::XNodeSetBase(const XNodeSetBase&	source) :
	XObject(source),
	m_proxy(*this),
	m_cachedStringValue(source.m_cachedStringValue),
	m_cachedNumberValue(source.m_cachedNumberValue)
{
}



XNodeSetBase::~XNodeSetBase()
{
}



XalanDOMString
XNodeSetBase::getTypeString() const
{
	return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("#NODESET"));
}



double
XNodeSetBase::num() const
{
	if (DoubleSupport::equal(m_cachedNumberValue, theBogusNumberValue) == true)
	{
#if defined(XALAN_NO_MUTABLE)
		((XNodeSetBase*)this)->m_cachedNumberValue = DoubleSupport::toDouble(str());
#else
		m_cachedNumberValue = DoubleSupport::toDouble(str());
#endif
	}

	return m_cachedNumberValue;
}



bool
XNodeSetBase::boolean() const
{
	return getLength() > 0 ? true : false;
}



const XalanDOMString&
XNodeSetBase::str() const
{
	if (isEmpty(m_cachedStringValue) == true &&
		getLength() > 0)
	{
		const XalanNode* const	theNode = item(0);
		assert(theNode != 0);

#if defined(XALAN_NO_MUTABLE)
		DOMServices::getNodeData(*theNode, ((XNodeSetBase*)this)->m_cachedStringValue);
#else
		DOMServices::getNodeData(*theNode, m_cachedStringValue);
#endif
	}

	return m_cachedStringValue;
}



void
XNodeSetBase::str(
			FormatterListener&	formatterListener,
			MemberFunctionPtr	function) const
{
	if (isEmpty(m_cachedStringValue) == false)
	{
		assert(length(m_cachedStringValue) == FormatterListener::size_type(length(m_cachedStringValue)));

		(formatterListener.*function)(c_wstr(m_cachedStringValue), FormatterListener::size_type(length(m_cachedStringValue)));
	}
	else if (getLength() > 0)
	{
		const XalanNode* const	theNode = item(0);
		assert(theNode != 0);

		DOMServices::getNodeData(*theNode, formatterListener, function);
	}
}



void
XNodeSetBase::str(XalanDOMString&	theBuffer) const
{
	if (isEmpty(m_cachedStringValue) == false)
	{
		append(theBuffer, m_cachedStringValue);
	}
	else if (getLength() > 0)
	{
		const XalanNode* const	theNode = item(0);
		assert(theNode != 0);

		DOMServices::getNodeData(*theNode, theBuffer);
	}
}



double
XNodeSetBase::stringLength() const
{
	if (isEmpty(m_cachedStringValue) == false)
	{
		return length(m_cachedStringValue);
	}
	else if (getLength() == 0)
	{
		return 0;
	}
	else
	{
		const XalanNode* const	theNode = item(0);
		assert(theNode != 0);

		FormatterStringLengthCounter	theCounter;

		DOMServices::getNodeData(*theNode, theCounter, &FormatterListener::characters);

		return theCounter.getCount();
	}
}



const XalanDocumentFragment&
XNodeSetBase::rtree() const
{
	return m_proxy;
}



void
XNodeSetBase::ProcessXObjectTypeCallback(XObjectTypeCallback&	theCallbackObject)
{
	theCallbackObject.NodeSet(*this,
							  nodeset());
}



void
XNodeSetBase::ProcessXObjectTypeCallback(XObjectTypeCallback&	theCallbackObject) const
{
	theCallbackObject.NodeSet(*this,
							  nodeset());
}



void
XNodeSetBase::clearCachedValues()
{
	m_cachedNumberValue = theBogusNumberValue;

	clear(m_cachedStringValue);
}



XALAN_CPP_NAMESPACE_END
