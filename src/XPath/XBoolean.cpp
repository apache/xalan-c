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
#include "XBoolean.hpp"



#include "XObjectTypeCallback.hpp"



XBoolean::XBoolean(
			XPathEnvSupport&	support,
			bool				val) :
	XObject(&support),
	m_value(val)
{
}



XBoolean::XBoolean(
			XPathEnvSupport*	support,
			bool				val) :
	XObject(support),
	m_value(val)
{
}



XBoolean::XBoolean(const XBoolean&	source) :
	XObject(source),
	m_value(source.m_value)
{
}



XBoolean::~XBoolean()
{
}



XBoolean::eObjectType
XBoolean::getType() const
{
	return eTypeBoolean;
}



DOMString
XBoolean::getTypeString() const
{
	return "#BOOLEAN";
}



double
XBoolean::num() const
{
	return m_value == true ? 1.0 : 0.0;
}



bool
XBoolean::boolean() const
{
	return m_value;
}



DOMString
XBoolean::str() const
{
	return m_value == true ? "true" : "false";
}



const ResultTreeFragBase&
XBoolean::rtree() const
{
	error("Can't cast XBoolean to ResultTreeFragBase");

	// error will throw, so this is just a dummy
	// value to satisfy the compiler.
	return *static_cast<ResultTreeFragBase*>(0);
}



ResultTreeFragBase&
XBoolean::rtree()
{
	error("Can't cast XBoolean to ResultTreeFragBase");

	// error will throw, so this is just a dummy
	// value to satisfy the compiler.
	return *static_cast<ResultTreeFragBase*>(0);
}



const NodeRefListBase&
XBoolean::nodeset() const
{
	error("Can't cast XBoolean to NodeRefListBase");

	// error will throw, so this is just a dummy
	// value to satisfy the compiler.
	return *static_cast<NodeRefListBase*>(0);
}



const MutableNodeRefList&
XBoolean::mutableNodeset() const
{
	error("Can't cast XBoolean to MutableNodeRefList");

	// error will throw, so this is just a dummy
	// value to satisfy the compiler.
	return *static_cast<MutableNodeRefList*>(0);
}



MutableNodeRefList&
XBoolean::mutableNodeset()
{
	error("Can't cast XBoolean to MutableNodeRefList");

	// error will throw, so this is just a dummy
	// value to satisfy the compiler.
	return *static_cast<MutableNodeRefList*>(0);
}



void
XBoolean::ProcessXObjectTypeCallback(XObjectTypeCallback&	theCallbackObject)
{
	theCallbackObject.Boolean(*this,
							  boolean());
}



void
XBoolean::ProcessXObjectTypeCallback(XObjectTypeCallback&	theCallbackObject) const
{
	theCallbackObject.Boolean(*this,
							  boolean());
}



bool
XBoolean::equals(const XObject&	theRHS) const
{
	return m_value == theRHS.boolean();
}
