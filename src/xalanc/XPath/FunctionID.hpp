/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2004 The Apache Software Foundation.  All rights 
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
#if !defined(FUNCTIONID_HEADER_GUARD_1357924680)
#define FUNCTIONID_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



#include <set>



#include <xalanc/XalanDOM/XalanElement.hpp>
#include <xalanc/XalanDOM/XalanNode.hpp>
#include <xalanc/XalanDOM/XalanDocument.hpp>



// Base class header files...
#include <xalanc/XPath/Function.hpp>
#include <xalanc/XPath/XObjectTypeCallback.hpp>



#include <xalanc/PlatformSupport/DOMStringHelper.hpp>
#include <xalanc/PlatformSupport/StringTokenizer.hpp>



#include <xalanc/XPath/MutableNodeRefList.hpp>
#include <xalanc/XPath/NodeRefListBase.hpp>



XALAN_CPP_NAMESPACE_BEGIN



/**
 * XPath implementation of "id" function.
 */
class XALAN_XPATH_EXPORT FunctionID : public Function
{
public:

	typedef Function	ParentType;

	FunctionID();

	virtual
	~FunctionID();

	// These methods are inherited from Function ...

	virtual XObjectPtr
	execute(
			XPathExecutionContext&	executionContext,
			XalanNode*				context,
			const XObjectPtr		arg1,
			const LocatorType*		locator) const;

#if !defined(XALAN_NO_USING_DECLARATION)
	using ParentType::execute;
#endif

#if defined(XALAN_NO_COVARIANT_RETURN_TYPE)
	virtual Function*
#else
	virtual FunctionID*
#endif
	clone() const;

protected:

	const XalanDOMString
	getError() const;

private:

	class FunctionIDXObjectTypeCallback : public XObjectTypeCallback
	{
	public:

		FunctionIDXObjectTypeCallback(
				XPathExecutionContext&	theExecutionContext,
				XalanDOMString&			theString);

		void
		processCallback(const XObject&	theXObject);

		// These methods are inherited from XObjectTypeCallback ...

		virtual void
		Number(
			const XObject&	theXObject,
			double			/* theValue */);

		virtual void
		Boolean(
			const XObject&	theXObject,
			bool			/* theValue */);

		virtual void
		String(
			const XObject&			theXObject,
			const XalanDOMString&	/* theValue */);

		virtual void
		ResultTreeFragment(
			const XObject&					theXObject,
			const XalanDocumentFragment&	/* theValue */);

		virtual void
		ResultTreeFragment(
			const XObject&			theXObject,
			XalanDocumentFragment&	/* theValue */);

		virtual void
		NodeSet(
			const XObject&			/* theXObject */,
			const NodeRefListBase&	theValue);

		virtual void
		Unknown(
			const XObject&			/* theObject */,
			const XalanDOMString&	/* theName */);

		virtual void
		Null(const XObject&		/* theObject */);

	private:

		XalanDOMString&			m_resultString;

		XPathExecutionContext&	m_executionContext;
	};

	// Not implemented...
	FunctionID&
	operator=(const FunctionID&);

	bool
	operator==(const FunctionID&) const;
};



XALAN_CPP_NAMESPACE_END



#endif	// FUNCTIONID_HEADER_GUARD_1357924680
