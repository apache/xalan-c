/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 2001-2002 The Apache Software Foundation.  All rights 
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


#include <XSLT/ResultTreeFragBaseNodeRefListBaseProxy.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class ResultTreeFragBaseXNodeSetBaseProxy : public XNodeSetBase
{
public:

	ResultTreeFragBaseXNodeSetBaseProxy(const XObjectPtr&	theXObject) :
		XNodeSetBase(),
		m_xobject(theXObject),
		m_proxy(theXObject->rtree())
	{
	}

	ResultTreeFragBaseXNodeSetBaseProxy(const ResultTreeFragBaseXNodeSetBaseProxy&	theSource) :
		XNodeSetBase(theSource),
		m_xobject(theSource.m_xobject),
		m_proxy(theSource.m_proxy)
	{
	}

	virtual
	~ResultTreeFragBaseXNodeSetBaseProxy()
	{
	}

	// These methods are inherited from XNodeSetBase...
#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual XObject*
#else
	virtual ResultTreeFragBaseXNodeSetBaseProxy*
#endif
	clone(void*		theAddress = 0) const
	{
		if (theAddress == 0)
		{
			return new ResultTreeFragBaseXNodeSetBaseProxy(*this);
		}
		else
		{
			return new (theAddress) ResultTreeFragBaseXNodeSetBaseProxy(*this);
		}
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
	item(size_type	index) const
	{
		return m_proxy.item(index);
	}

	virtual size_type
	getLength() const
	{
		return m_proxy.getLength();
	}

private:

	const XObjectPtr								m_xobject;

	const ResultTreeFragBaseNodeRefListBaseProxy	m_proxy;
};



FunctionNodeSet::FunctionNodeSet(bool	convertString) :
	m_convertString(convertString)
{
}



FunctionNodeSet::~FunctionNodeSet()
{
}



XObjectPtr
FunctionNodeSet::execute(
			XPathExecutionContext&			executionContext,
			XalanNode*						context,
			const XObjectArgVectorType&		args,
			const LocatorType*				locator) const
{
	if (args.size() != 1)
	{
		executionContext.error(getError(), context, locator);
	}

	assert(args[0].null() == false);

	const XObject::eObjectType	theType = args[0]->getType();

	if (theType == XObject::eTypeResultTreeFrag ||
		(theType == XObject::eTypeString && m_convertString == true))
	{
		return XObjectPtr(new ResultTreeFragBaseXNodeSetBaseProxy(args[0]));
	}
	else
	{
		executionContext.warn(
			getInvalidArgumentTypeError(),
			context,
			locator);

		return args[0];
	}
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
	return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("The nodeset() function accepts one argument"));
}



const XalanDOMString
FunctionNodeSet::getInvalidArgumentTypeError() const
{
	return StaticStringToDOMString(XALAN_STATIC_UCODE_STRING("Invalid argument type in function nodeset()"));
}



XALAN_CPP_NAMESPACE_END
