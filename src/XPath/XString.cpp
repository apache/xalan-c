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
#include "XString.hpp"



#include <XalanDOM/XalanText.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/DoubleSupport.hpp>



#include "ResultTreeFrag.hpp"
#include "XObjectTypeCallback.hpp"
#include "XPathEnvSupport.hpp"
#include "XPathSupport.hpp"



XString::XString(
			XPathEnvSupport&		envSupport,
			XPathSupport&			support,
			const XalanDOMString&	val) :
	XObject(&envSupport, &support),
	m_value(val),
	m_cachedNumberValue(0.0),
	m_resultTreeFrag(0)
{
}



XString::XString(const XString&	source) :
	XObject(source),
	m_value(source.m_value),
	m_resultTreeFrag(source.m_resultTreeFrag.get() == 0 ?
						0 :
						source.m_resultTreeFrag->clone(true))
{
}



XString::~XString()
{
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
XObject*
#else
XString*
#endif
XString::clone(void*	theAddress) const
{
	return theAddress == 0 ? new XString(*this) : new (theAddress) XString(*this);
};



XString::eObjectType
XString::getType() const
{
	return eTypeString;
}



XalanDOMString
XString::getTypeString() const
{
	return XALAN_STATIC_UCODE_STRING("#STRING");
}



double
XString::num() const
{
	if (m_cachedNumberValue == 0.0)
	{
#if defined(XALAN_NO_MUTABLE)
		((XString*)this)->m_cachedNumberValue = DoubleSupport::toDouble(m_value);
#else
		m_cachedNumberValue = DoubleSupport::toDouble(m_value);
#endif
	}

	return m_cachedNumberValue;
}



bool
XString::boolean() const
{
	return length(m_value) > 0 ? true : false;
}



XalanDOMString
XString::str() const
{
	return m_value;
}



const ResultTreeFragBase&
XString::rtree() const
{
	assert(m_support != 0);
	assert(m_envSupport != 0);

	if (m_resultTreeFrag.get() == 0)
	{
		XalanDocument* const	theFactory =
				m_envSupport->getDOMFactory();
		assert(theFactory != 0);

		ResultTreeFrag* const	theFrag =
			new ResultTreeFrag(*theFactory,
							   *m_support);

		XalanNode* const	textNode =
			theFactory->createTextNode(str());
		assert(textNode != 0);

		theFrag->appendChild(textNode);

#if defined(XALAN_OLD_AUTO_PTR)

#if !defined (XALAN_NO_NAMESPACES)
		using std::auto_ptr;
#endif

#if defined(XALAN_NO_MUTABLE)
		((XString*)this)->m_resultTreeFrag = auto_ptr<ResultTreeFragBase>(theFrag);
#else
		m_resultTreeFrag = auto_ptr<ResultTreeFragBase>(theFrag);
#endif

#else

#if defined(XALAN_NO_MUTABLE)
		((XString*)this)->m_resultTreeFrag.reset(theFrag);
#else
		m_resultTreeFrag.reset(theFrag);
#endif

#endif
	}

	return *m_resultTreeFrag.get();
}



ResultTreeFragBase&
XString::rtree()
{
	assert(m_support != 0);
	assert(m_envSupport != 0);

	if (m_resultTreeFrag.get() == 0)
	{
		XalanDocument* const	theFactory =
				m_envSupport->getDOMFactory();
		assert(theFactory != 0);

		ResultTreeFrag* const	theFrag =
			new ResultTreeFrag(*theFactory,
							   *m_support);

		XalanNode* const	textNode =
			theFactory->createTextNode(str());
		assert(textNode != 0);

		theFrag->appendChild(textNode);

#if defined(XALAN_OLD_AUTO_PTR)

#if !defined (XALAN_NO_NAMESPACES)
		using std::auto_ptr;
#endif

#if defined(XALAN_NO_MUTABLE)
		((XString*)this)->m_resultTreeFrag = auto_ptr<ResultTreeFragBase>(theFrag);
#else
		m_resultTreeFrag = auto_ptr<ResultTreeFragBase>(theFrag);
#endif

#else

#if defined(XALAN_NO_MUTABLE)
		((XString*)this)->m_resultTreeFrag.reset(theFrag);
#else
		m_resultTreeFrag.reset(theFrag);
#endif

#endif
	}

	return *m_resultTreeFrag.get();
}



const NodeRefListBase&
XString::nodeset() const
{
	error("Can't cast XString to NodeRefListBase");

	// error will throw, so this is just a dummy
	// value to satisfy the compiler.
	return *static_cast<NodeRefListBase*>(0);
}



const MutableNodeRefList&
XString::mutableNodeset() const
{
	error("Can't cast XString to MutableNodeRefList");

	// error will throw, so this is just a dummy
	// value to satisfy the compiler.
	return *static_cast<MutableNodeRefList*>(0);
}



MutableNodeRefList&
XString::mutableNodeset()
{
	error("Can't cast XString to MutableNodeRefList");

	// error will throw, so this is just a dummy
	// value to satisfy the compiler.
	return *static_cast<MutableNodeRefList*>(0);
}



void
XString::ProcessXObjectTypeCallback(XObjectTypeCallback&	theCallbackObject)
{
	theCallbackObject.String(*this,
							 str());
}



void
XString::ProcessXObjectTypeCallback(XObjectTypeCallback&	theCallbackObject) const
{
	theCallbackObject.String(*this,
							 str());
}
