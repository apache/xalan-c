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



#include <PlatformSupport/DOMStringHelper.hpp>



#include "ResultTreeFrag.hpp"
#include "XObjectTypeCallback.hpp"
#include "XPathEnvSupport.hpp"
#include "XPathSupport.hpp"



XString::XString(
			XPathEnvSupport&	envSupport,
			XPathSupport&		support,
			const DOMString&	val) :
	XObject(&envSupport),
	m_support(support),
	m_value(val),
	m_resultTreeFrag(0)
{
}



XString::XString(const XString&	source) :
	XObject(source),
	m_support(source.m_support),
	m_value(source.m_value),
#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	m_resultTreeFrag(source.m_resultTreeFrag.get() == 0 ?
						0 :
						dynamic_cast<ResultTreeFragBase*>(source.m_resultTreeFrag->clone(true)))
#else
	m_resultTreeFrag(source.m_resultTreeFrag.get() == 0 ?
						0 :
						source.m_resultTreeFrag->clone(true))
#endif
{
}



XString::~XString()
{
}



XString::eObjectType
XString::getType() const
{
	return eTypeString;
}



DOMString
XString::getTypeString() const
{
	return "#STRING";
}



double
XString::num() const
{
	return DOMStringToDouble(m_value);
}



bool
XString::boolean() const
{
	return length(m_value) > 0 ? true : false;
}



DOMString
XString::str() const
{
	return m_value;
}



const ResultTreeFragBase&
XString::rtree() const
{
	assert(m_envSupport != 0);

	if (m_resultTreeFrag.get() == 0)
	{
		ResultTreeFrag* const	theFrag =
			new ResultTreeFrag(m_envSupport->getDOMFactory(),
							   m_support);

#if defined(XALAN_OLD_AUTO_PTR)
		m_resultTreeFrag = std::auto_ptr<ResultTreeFragBase>(theFrag);
#else
	    m_resultTreeFrag.reset(theFrag);
#endif

		const DOM_Node	textNode =
			m_envSupport->getDOMFactory().createTextNode(str());

		theFrag->appendChild(textNode);
	}

	return *m_resultTreeFrag.get();
}



ResultTreeFragBase&
XString::rtree()
{
	if (m_resultTreeFrag.get() == 0)
	{
		ResultTreeFrag* const	theFrag =
			new ResultTreeFrag(m_envSupport->getDOMFactory(),
							   m_support);

#if defined(XALAN_OLD_AUTO_PTR)
		m_resultTreeFrag = std::auto_ptr<ResultTreeFragBase>(theFrag);
#else
	    m_resultTreeFrag.reset(theFrag);
#endif

		const DOM_Node	textNode =
			m_envSupport->getDOMFactory().createTextNode(str());

		theFrag->appendChild(textNode);
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



bool
XString::equals(const XObject&	theRHS) const
{
	return ::equals(m_value, theRHS.str());
}
