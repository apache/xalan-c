/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2000 The Apache Software Foundation.  All rights 
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
#if !defined(XALAN_RESULTNAMESPACESSTACK_HEADER_GUARD)
#define XALAN_RESULTNAMESPACESSTACK_HEADER_GUARD



// Base include file.  Must be first.
#include <XSLT/XSLTDefinitions.hpp>



#include <vector>



#include <XalanDOM/XalanDOMString.hpp>



#include <XPath/XalanQName.hpp>



class XALAN_XSLT_EXPORT ResultNamespacesStack
{
public:

#if defined(XALAN_NO_NAMESPACES)
	typedef vector<bool>					BoolVectorType;
#else
	typedef std::vector<bool>				BoolVectorType;
#endif

	typedef XalanQName::NamespaceVectorType		NamespaceVectorType;
	typedef XalanQName::NamespacesStackType		NamespacesStackType;

	typedef NamespacesStackType::size_type		size_type;


	explicit
	ResultNamespacesStack();

	~ResultNamespacesStack();

	void
	addDeclaration(
			const XalanDOMString&	thePrefix,
	        const XalanDOMString&	theNamespaceURI);

	void
	pushContext();

	void
	popContext();

	const XalanDOMString*
	getNamespaceForPrefix(const XalanDOMString&		thePrefix) const;

	const XalanDOMString*
	getPrefixForNamespace(const XalanDOMString&		theNamespaceURI) const;


	/**
	 * See if the prefix has been mapped to a namespace in the current
	 * context, without looking down the stack of namespaces.
	 */
	bool
	prefixIsPresentLocal(const XalanDOMString&	thePrefix);

	void
	clear();

	size_type
	size() const
	{
		return m_resultNamespaces.size();
	}

private:

	// not implemented
	ResultNamespacesStack(const ResultNamespacesStack&);

	bool
	operator==(const ResultNamespacesStack&) const;

	ResultNamespacesStack&
	operator=(const ResultNamespacesStack&);

	enum { eDefaultCreateNewContextStackSize = 100 };

	/**
	 * A stack to keep track of the result tree namespaces.
	 */
	NamespacesStackType		m_resultNamespaces;

	BoolVectorType			m_createNewContextStack;
};



#endif	// XALAN_RESULTNAMESPACESSTACK_HEADER_GUARD
