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
#if !defined(XNODESET_HEADER_GUARD_1357924680)
#define XNODESET_HEADER_GUARD_1357924680



// Base include file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



#if defined(XALAN_AUTO_PTR_REQUIRES_DEFINITION)
#include <XPath/ResultTreeFragBase.hpp>
#endif



#include <PlatformSupport/XalanAutoPtr.hpp>



// Base class header file.
#include <XPath/XObject.hpp>



#include <XPath/XPathExecutionContext.hpp>



class ResultTreeFragBase;
class XalanNode;



/**
 * Class to hold XPath return types.
 */
class XALAN_XPATH_EXPORT XNodeSet : public XObject
{
public:

	typedef XPathExecutionContext::BorrowReturnMutableNodeRefList	BorrowReturnMutableNodeRefList;

	/**
	 * Create an XNodeSet from a node list.
	 *
	 * @param value Pointer to source node list.  The XNodeSet will adopt the pointer.
	 */
	XNodeSet(BorrowReturnMutableNodeRefList&	value);

	/**
	 * Create an XNodeSet from another.
	 *
	 * @param source    object to copy
	 * @param deepClone true to copy all children on nodeset nodes
	 */
	XNodeSet(
			const XNodeSet&		source,
			bool				deepClone = false);

	virtual
	~XNodeSet();

	// These methods are inherited from XObject ...

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual XObject*
#else
	virtual XNodeSet*
#endif
	clone(void*		theAddress = 0) const;

	virtual	eObjectType
	getType() const;

	virtual XalanDOMString
	getTypeString() const;

	virtual double
	num() const;

	virtual bool
	boolean() const;

	virtual XalanDOMString
	str() const;

	virtual const ResultTreeFragBase&
	rtree(XPathExecutionContext&	executionContext) const;

	virtual const NodeRefListBase&
	nodeset() const;

	virtual void
	ProcessXObjectTypeCallback(XObjectTypeCallback&		theCallbackObject);

	virtual void
	ProcessXObjectTypeCallback(XObjectTypeCallback&		theCallbackObject) const;

private:

	// Not implemented...
	XNodeSet&
	operator=(const XNodeSet&);

	// Data members...


	const BorrowReturnMutableNodeRefList		m_value;

	mutable XalanAutoPtr<ResultTreeFragBase>	m_resultTreeFrag;

	mutable XalanDOMString						m_cachedStringValue;

	mutable double								m_cachedNumberValue;
};



#endif	// XNODESET_HEADER_GUARD_1357924680
