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
#if !defined(QNAME_HEADER_GUARD_1357924680)
#define QNAME_HEADER_GUARD_1357924680



// Base header file.  Must be first.
#include <XPath/XPathDefinitions.hpp>



#include <deque>



#include <XalanDOM/XalanDOMString.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>



#include <XPath/NameSpace.hpp>



class XalanElement;
class PrefixResolver;
class XPathEnvSupport;
class XPathSupport;



/**
 * Class to represent a qualified name: "The name of an internal XSLT object, 
 * specifically a named template (see [7 Named Templates]), a mode (see [6.7 Modes]), 
 * an attribute set (see [8.1.4 Named Attribute Sets]), a key (see [14.2 Keys]), 
 * a locale (see [14.3 Number Formatting]), a variable or a parameter (see 
 * [12 Variables and Parameters]) is specified as a QName. If it has a prefix, 
 * then the prefix is expanded into a URI reference using the namespace declarations 
 * in effect on the attribute in which the name occurs. The expanded name 
 * consisting of the local part of the name and the possibly null URI reference 
 * is used as the name of the object. The default namespace is not used for 
 * unprefixed names."
 */
class XALAN_XPATH_EXPORT QName
{
public:

#if defined(XALAN_NO_NAMESPACES)
	typedef	deque<NameSpace>					NamespaceVectorType;
	typedef	deque<NamespaceVectorType>			NamespacesStackType;
#else
	typedef	std::deque<NameSpace>				NamespaceVectorType;
	typedef	std::deque<NamespaceVectorType>		NamespacesStackType;
#endif

	/**
	 * Construct an empty QName.
	 *
	 */
	explicit
	QName();

	virtual
	~QName();

	/**
	 * Retrieve the local part of qualified name.
	 * 
	 * @return local part string
	 */
	virtual const XalanDOMString&
	getLocalPart() const = 0;

	/**
	 * Retrieve the namespace of qualified name.
	 * 
	 * @return namespace string
	 */
	virtual const XalanDOMString&
	getNamespace() const = 0;

	/**
	 * Whether the qualified name is empty.
	 * 
	 * @return true if namespace and local part are both empty
	 */
	bool
	isEmpty() const
	{
		return ::isEmpty(getNamespace()) && ::isEmpty(getLocalPart());
	}

	/**
	 * Override equals and agree that we're equal if the passed object is a
	 * string and it matches the name of the arg.
	 * 
	 * @param theRHS namespace to compare
	 * @return true if namespace and local part are both empty
	 */
	bool
	equals(const QName&		theRHS) const
	{
		// Note that we do not use our member variables here.  See
		// class QNameReference for details...
		return ::equals(getLocalPart(), theRHS.getLocalPart()) &&
			   ::equals(getNamespace(), theRHS.getNamespace());
	}

	/**
	 * Get the namespace from a prefix by searching a vector of namespaces.
	 *
	 * @param namespaces vector of namespaces to search
	 * @param prefix     namespace prefix to find
	 * @param reverse    true to search vector from last to first, default true
	 */
	static const XalanDOMString&
	getNamespaceForPrefix(
			const NamespaceVectorType&	namespaces,
			const XalanDOMString&		prefix,
			bool						reverse = true);

	/**
	 * Get the namespace from a prefix by searching a stack of namespace
	 * vectors.
	 *
	 * @param nsStack stack of namespace vectors to search
	 * @param prefix  namespace prefix to find
	 * @param reverse true to search vector from last to first, default true
	 */
	static const XalanDOMString&
	getNamespaceForPrefix(
			const NamespacesStackType&	nsStack,
			const XalanDOMString&		prefix,
			bool						reverse = true);

	/**
	 * Get the prefix for a namespace by searching a vector of namespaces.
	 *
	 * @param namespaces vector of namespaces to search
	 * @param uri        URI string for namespace to find
	 * @param reverse    true to search vector from last to first, default true
	 */
	static const XalanDOMString&
	getPrefixForNamespace(
			const NamespaceVectorType&	namespaces,
			const XalanDOMString&		uri,
			bool						reverse = true);

	/**
	 * Get the prefix for a namespace by searching a stack of namespace
	 * vectors.
	 *
	 * @param nsStack stack of namespace vectors to search
	 * @param uri     URI string for namespace to find
	 * @param reverse true to search vector from last to first, default true
	 */
	static const XalanDOMString&
	getPrefixForNamespace(
			const NamespacesStackType&	nsStack,
			const XalanDOMString&		uri,
			bool						reverse = true);

protected:

	static const XalanDOMString		s_emptyString;
};



inline bool
operator==(
			const QName&	theLHS,
			const QName&	theRHS)
{
	return theLHS.equals(theRHS);
}



inline bool
operator!=(
			const QName&	theLHS,
			const QName&	theRHS)
{
	return !(theLHS == theRHS);
}



inline bool
operator<(
			const QName&	theLHS,
			const QName&	theRHS)
{
	if (theLHS.getNamespace() < theRHS.getNamespace())
	{
		return true;
	}
	else if (equals(theLHS.getNamespace(), theRHS.getNamespace()))
	{
		return theLHS.getLocalPart() < theRHS.getLocalPart();
	}
	else
	{
		return false;
	}
}



#endif	// QNAME_HEADER_GUARD_1357924680
