/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
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
#include "XNodeSet.hpp"



#include <XalanDOM/XalanNode.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/DoubleSupport.hpp>



#include <DOMSupport/DOMServices.hpp>



#include "ResultTreeFrag.hpp"
#include "MutableNodeRefList.hpp"
#include "XObjectTypeCallback.hpp"
#include "XPathExecutionContext.hpp"



const double	theBogusNumberValue = 123456789;



XNodeSet::XNodeSet(BorrowReturnMutableNodeRefList&	value) :
	XObject(eTypeNodeSet),
	m_value(value),
	m_resultTreeFrag(),
	m_cachedStringValue(),
	m_cachedNumberValue(theBogusNumberValue)
{
}



XNodeSet::XNodeSet(const XNodeSet&	source,
				   bool				deepClone) :
	XObject(source),
	m_value(source.m_value.clone()),
	m_resultTreeFrag(source.m_resultTreeFrag.get() == 0 ?
						0 :
						source.m_resultTreeFrag->clone(deepClone)),
	m_cachedStringValue(source.m_cachedStringValue),
	m_cachedNumberValue(source.m_cachedNumberValue)
{
}



XNodeSet::~XNodeSet()
{
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XObject*
#else
XNodeSet*
#endif
XNodeSet::clone(void*	theAddress) const
{
	return theAddress == 0 ? new XNodeSet(*this) : new (theAddress) XNodeSet(*this);
};



XalanDOMString
XNodeSet::getTypeString() const
{
	return XALAN_STATIC_UCODE_STRING("#NODESET");
}



double
XNodeSet::num() const
{
	if (DoubleSupport::equal(m_cachedNumberValue, theBogusNumberValue) == true)
	{
#if defined(XALAN_NO_MUTABLE)
		((XNodeSet*)this)->m_cachedNumberValue = DoubleSupport::toDouble(str());
#else
		m_cachedNumberValue = DoubleSupport::toDouble(str());
#endif
	}

	return m_cachedNumberValue;
}



bool
XNodeSet::boolean() const
{
	return m_value->getLength() > 0 ? true : false;
}



const XalanDOMString&
XNodeSet::str() const
{
	if (isEmpty(m_cachedStringValue) == true &&
		m_value->getLength() > 0)
	{
		const XalanNode* const	theNode = m_value->item(0);
		assert(theNode != 0);

#if defined(XALAN_NO_MUTABLE)
		DOMServices::getNodeData(*theNode, ((XNodeSet*)this)->m_cachedStringValue);
#else
		DOMServices::getNodeData(*theNode, m_cachedStringValue);
#endif
	}

	return m_cachedStringValue;
}



void
XNodeSet::str(
			FormatterListener&	formatterListener,
			MemberFunctionPtr	function) const
{
	if (isEmpty(m_cachedStringValue) == false)
	{
		(formatterListener.*function)(c_wstr(m_cachedStringValue), length(m_cachedStringValue));
	}
	else if (m_value->getLength() > 0)
	{
		const XalanNode* const	theNode = m_value->item(0);
		assert(theNode != 0);

		DOMServices::getNodeData(*theNode, formatterListener, function);
	}
}



const ResultTreeFragBase&
XNodeSet::rtree(XPathExecutionContext&	executionContext) const
{
	if (m_resultTreeFrag.get() == 0)
	{
		XalanDocument* const	theFactory = executionContext.getDOMFactory();
		assert(theFactory != 0);

		ResultTreeFrag* const	theFrag =
			new ResultTreeFrag(theFactory);

		const int	nNodes = m_value->getLength();

		for(int i = 0; i < nNodes; i++)
		{
			theFrag->appendChild(m_value->item(i));
		}

#if defined(XALAN_NO_MUTABLE)
		((XNodeSet*)this)->m_resultTreeFrag.reset(theFrag);
#else
	    m_resultTreeFrag.reset(theFrag);
#endif
	}

	return *m_resultTreeFrag.get();
}



const NodeRefListBase&
XNodeSet::nodeset() const
{
	return *m_value.get();
}



void
XNodeSet::ProcessXObjectTypeCallback(XObjectTypeCallback&	theCallbackObject)
{
	theCallbackObject.NodeSet(*this,
							  nodeset());
}



void
XNodeSet::ProcessXObjectTypeCallback(XObjectTypeCallback&	theCallbackObject) const
{
	theCallbackObject.NodeSet(*this,
							  nodeset());
}



void
XNodeSet::release()
{
	m_value.release();

	m_cachedNumberValue = theBogusNumberValue;

	clear(m_cachedStringValue);
}



void
XNodeSet::set(BorrowReturnMutableNodeRefList&	value)
{
	release();

	m_value = value;
}
