/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2001 The Apache Software Foundation.  All rights 
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
#include "XNodeSetBase.hpp"



#include <XalanDOM/XalanNode.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/DoubleSupport.hpp>



#include <DOMSupport/DOMServices.hpp>



#include "ResultTreeFrag.hpp"
#include "XObjectTypeCallback.hpp"
#include "XPathExecutionContext.hpp"



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



const ResultTreeFragBase&
XNodeSetBase::rtree(XPathExecutionContext&	/* executionContext */) const
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
