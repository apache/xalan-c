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
#include "FunctionNodeSet.hpp"



#include <XPath/XNodeSetBase.hpp>
#include <XPath/XObjectFactory.hpp>



class XResultTreeFragNodeSetProxy : public XNodeSetBase
{
public:

	XResultTreeFragNodeSetProxy(const XObjectPtr&	theXObject) :
		m_xobject(theXObject),
		m_proxy(*static_cast<const XResultTreeFrag*>(theXObject.get()))
	{
		assert(theXObject.null() == false);
		assert(theXObject->getType() == XObject::eTypeResultTreeFrag);
	}

	XResultTreeFragNodeSetProxy(const XResultTreeFragNodeSetProxy&	theSource) :
		m_xobject(theSource.m_xobject),
		m_proxy(*static_cast<const XResultTreeFrag*>(m_xobject.get()))
	{
	}

	virtual
	~XResultTreeFragNodeSetProxy()
	{
	}

	// These methods are inherited from XNodeSetBase...
#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual XObject*
#else
	virtual XResultTreeFragNodeSetProxy*
#endif
	clone(void*		theAddress = 0) const
	{
		return theAddress == 0 ? new XResultTreeFragNodeSetProxy(*this) :
				new (theAddress) XResultTreeFragNodeSetProxy(*this);
	}

	virtual const NodeRefListBase&
	nodeset() const
	{
		return m_proxy;
	}

	virtual void 
	dereferenced()
	{
		delete this;
	}

	virtual XalanNode*
	item(unsigned int	index) const
	{
		return m_proxy.item(index);
	}

	virtual unsigned int
	getLength() const
	{
		return m_proxy.getLength();
	}

private:

	const XObjectPtr								m_xobject;

	const XResultTreeFrag::NodeRefListBaseProxy		m_proxy;
};



FunctionNodeSet::FunctionNodeSet()
{
}



FunctionNodeSet::~FunctionNodeSet()
{
}



XObjectPtr
FunctionNodeSet::execute(
		XPathExecutionContext&			executionContext,
		XalanNode*						context)
{
	executionContext.error(getError(), context);

	return XObjectPtr(0);
}



XObjectPtr
FunctionNodeSet::execute(
		XPathExecutionContext&	executionContext,
		XalanNode*				/* context */,			
		const XObjectPtr		arg1)
{
	assert(arg1.null() == false);	

	if (arg1->getType() != XObject::eTypeResultTreeFrag)
	{
		return arg1;
	}
	else
	{
		return XObjectPtr(new XResultTreeFragNodeSetProxy(arg1));
	}
}



XObjectPtr
FunctionNodeSet::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				context,			
			const XObjectPtr		/* arg1 */,
			const XObjectPtr		/* arg2 */)
{
	executionContext.error(getError(), context);

	return XObjectPtr(0);
}



XObjectPtr
FunctionNodeSet::execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				context,			
			const XObjectPtr		/* arg1 */,
			const XObjectPtr		/* arg2 */,
			const XObjectPtr		/* arg3 */)
{
	executionContext.error(getError(), context);

	return XObjectPtr(0);
}



XObjectPtr
FunctionNodeSet::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			int								/* opPos */,
			const XObjectArgVectorType&		/* args */)
{
	executionContext.error(getError(), context);

	return XObjectPtr(0);
}



#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
Function*
#else
FunctionNodeSet*
#endif
FunctionNodeSet::clone() const
{
	return new FunctionNodeSet(*this);
}



const XalanDOMString
FunctionNodeSet::getError() const
{
	return XALAN_STATIC_UCODE_STRING(
		"The node-set() function takes one argument!");
}
