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
#if !defined(XALANQNAMEBYREFERENCE_HEADER_GUARD_1357924680)
#define XALANQNAMEBYREFERENCE_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <xalanc/XPath/XPathDefinitions.hpp>



// Base class header file...
#include <xalanc/XPath/XalanQName.hpp>



XALAN_CPP_NAMESPACE_BEGIN



class XALAN_XPATH_EXPORT XalanQNameByReference : public XalanQName
{
public:

	/**
	 * Construct an empty XalanQNameByReference.
	 */
	explicit
	XalanQNameByReference();

	/**
	 * Construct a XalanQNameByReference, with the supplied namespace and local part.
	 * The instance keeps only a _reference_ to the string, to avoid making a
	 * copy.
	 *
	 * @param theNamespace namespace string
	 * @param theLocalPart local part string
	 */
	XalanQNameByReference(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	theLocalPart);

	/**
	 * Construct a XalanQNameByReference, with the supplied local part.
	 * The instance keeps only a _reference_ to the string, to avoid making a
	 * copy.
	 *
	 * @param theLocalPart local part string
	 */
	XalanQNameByReference(const XalanDOMString&		theLocalPart);

	/**
	 * Construct a XalanQNameByReference, from the supplied XalanQName.  The instance
	 * keeps only a _reference_ to the string, to avoid making a copy.
	 *
	 * @param theQName The source QName
	 */
	XalanQNameByReference(const XalanQName&		theQName);

	virtual
	~XalanQNameByReference();

	/**
	 * Retrieve the local part of qualified name.
	 * 
	 * @return local part string
	 */
	virtual const XalanDOMString&
	getLocalPart() const;

	/**
	 * Set the local part of qualified name.
	 * 
	 * @param theLocalPart local part string
	 */
	void
	setLocalPart(const XalanDOMString&	theLocalPart)
	{
		m_localpart = &theLocalPart;
	}

	/**
	 * Retrieve the namespace of qualified name.
	 * 
	 * @return namespace string
	 */
	virtual const XalanDOMString&
	getNamespace() const;

	/**
	 * Set the Namespace URI of qualified name.
	 * 
	 * @param theLocalPart local part string
	 */
	void
	setNamespace(const XalanDOMString&	theNamespace)
	{
		m_namespace = &theNamespace;
	}

	/**
	 * Clear the instance.
	 */
	void
	clear()
	{
		m_namespace = &s_emptyString;
		m_localpart = &s_emptyString;
	}

private:

	// OK, we said reference, but using pointers
	// allows for copy and assignment semantics.
	const XalanDOMString*	m_namespace;

	const XalanDOMString*	m_localpart;
};



XALAN_CPP_NAMESPACE_END



#endif	// XALANQNAMEBYREFERENCE_HEADER_GUARD_1357924680
