/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:  
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xalan" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written 
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */
// Class header file.
#include "XResultTreeFrag.hpp"



#include <XalanDOM/XalanNodeList.hpp>
#include <XalanDOM/XalanText.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/DoubleSupport.hpp>



#include <DOMSupport/DOMServices.hpp>



#include <XPath/FormatterStringLengthCounter.hpp>
#include <XPath/NodeRefListBase.hpp>
#include <XPath/ResultTreeFragBase.hpp>
#include <XPath/XObjectTypeCallback.hpp>



#include <XSLT/StylesheetExecutionContext.hpp>



XResultTreeFrag::XResultTreeFrag(ResultTreeFragBase&	value) :
	XObject(eTypeResultTreeFrag),
	m_value(&value),
	m_executionContext(0),
	m_cachedStringValue(),
	m_cachedNumberValue(0.0)
{
}



XResultTreeFrag::XResultTreeFrag(
			const XResultTreeFrag&	source,
			bool					/* deepClone */) :
	XObject(source),
	m_value(source.m_value),
	m_executionContext(0),
	m_cachedStringValue(source.m_cachedStringValue),
	m_cachedNumberValue(source.m_cachedNumberValue)
{
	assert(m_value != 0);
}



XResultTreeFrag::~XResultTreeFrag()
{
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XObject*
#else
XResultTreeFrag*
#endif
XResultTreeFrag::clone(void*	theAddress) const
{
	return theAddress == 0 ? new XResultTreeFrag(*this) : new (theAddress) XResultTreeFrag(*this);
}



XalanDOMString
XResultTreeFrag::getTypeString() const
{
	return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("#RESULT_TREE_FRAG"));
}



double
XResultTreeFrag::num() const
{
	if (m_cachedNumberValue == 0.0)
	{
#if defined(XALAN_NO_MUTABLE)
		((XResultTreeFrag*)this)->m_cachedNumberValue = DoubleSupport::toDouble(str());
#else
		m_cachedNumberValue = DoubleSupport::toDouble(str());
#endif
	}

	return m_cachedNumberValue;
}



bool
XResultTreeFrag::boolean() const
{
	// Result tree fragments always evaluate to true.
	return true;
}



const XalanDOMString&
XResultTreeFrag::str() const
{
	if (isEmpty(m_cachedStringValue) == true)
	{
#if defined(XALAN_NO_MUTABLE)
		DOMServices::getNodeData(*m_value, ((XResultTreeFrag*)this)->m_cachedStringValue);
#else
		DOMServices::getNodeData(*m_value, m_cachedStringValue);
#endif
	}

	return m_cachedStringValue;
}



void
XResultTreeFrag::str(
			FormatterListener&	formatterListener,
			MemberFunctionPtr	function) const
{
	if (isEmpty(m_cachedStringValue) == false)
	{
		assert(length(m_cachedStringValue) == FormatterListener::size_type(length(m_cachedStringValue)));

		(formatterListener.*function)(c_wstr(m_cachedStringValue), FormatterListener::size_type(length(m_cachedStringValue)));
	}
	else
	{
		DOMServices::getNodeData(*m_value, formatterListener, function);
	}
}



void
XResultTreeFrag::str(XalanDOMString&	theBuffer) const
{
	if (isEmpty(m_cachedStringValue) == false)
	{
		append(theBuffer, m_cachedStringValue);
	}
	else
	{
		DOMServices::getNodeData(*m_value, theBuffer);
	}
}



double
XResultTreeFrag::stringLength() const
{
	if (isEmpty(m_cachedStringValue) == false)
	{
		return length(m_cachedStringValue);
	}
	else
	{
		FormatterStringLengthCounter	theCounter;

		DOMServices::getNodeData(*m_value, theCounter, FormatterListener::characters);

		return theCounter.getCount();
	}
}



const ResultTreeFragBase&
XResultTreeFrag::rtree() const
{
	assert(m_value != 0);

	return *m_value;
}



const NodeRefListBase&
XResultTreeFrag::nodeset() const
{
	return ParentType::nodeset();
}



void
XResultTreeFrag::ProcessXObjectTypeCallback(XObjectTypeCallback&	theCallbackObject)
{
	theCallbackObject.ResultTreeFragment(*this,
										 rtree());
}



void
XResultTreeFrag::ProcessXObjectTypeCallback(XObjectTypeCallback&	theCallbackObject) const
{
	theCallbackObject.ResultTreeFragment(*this,
										 rtree());
}



ResultTreeFragBase*
XResultTreeFrag::release()
{
	clear(m_cachedStringValue);

	m_cachedNumberValue = 0.0;

	ResultTreeFragBase* const	temp = m_value;

	m_value = 0;

	return temp;
}



void
XResultTreeFrag::set(ResultTreeFragBase*	theValue)
{
	release();

	m_value = theValue;
}



void
XResultTreeFrag::dereferenced()
{
	if (m_executionContext == 0 ||
		// We're dead after we call this, if it returns true;
		m_executionContext->returnXResultTreeFrag(this) == false)
	{
		delete m_value;

		delete this;
	}
}
