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



#include <vector>



#include <dom/DOMString.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>



class DOM_Element;
class NameSpace;
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

	/**
	 * Construct a QName, with the supplied namespace and local part.
	 */
	QName(
			const DOMString&	theNamespace = DOMString(),
			const DOMString&	theLocalPart = DOMString());

#if defined(XALAN_NO_NAMESPACES)
	typedef	vector<NameSpace>					NamespaceVectorType;
	typedef	vector<NamespaceVectorType>			NamespacesStackType;
#else
	typedef	std::vector<NameSpace>				NamespaceVectorType;
	typedef	std::vector<NamespaceVectorType>	NamespacesStackType;
#endif

	/**
	 * Construct a QName from a string, resolving the prefix 
	 * using the given namespace stack. The default namespace is 
	 * not resolved.
	 */
	QName(
			const DOMString&				qname,
			const NamespacesStackType&	namespaces);

	/**
	 * Construct a QName from a string, resolving the prefix 
	 * using the given namespace stack. The default namespace is 
	 * not resolved.
	 */
	QName(
			const DOMString&		qname,
			const DOM_Element&		namespaceContext,
			const XPathEnvSupport&	envSupport,
			const XPathSupport&		support);

	/**
	 * Construct a QName from a string, resolving the prefix 
	 * using the given prefix resolver. The default namespace is 
	 * not resolved.
	 */
	QName(
			const DOMString&		qname,
			const PrefixResolver&	theResolver);

	~QName();

	const DOMString&
	getLocalPart() const
	{
		return m_localpart;
	}

	const DOMString&
	getNamespace() const
	{
		return m_namespace;
	}

	bool isEmpty() const
	{
		return (::isEmpty(m_namespace) && ::isEmpty(m_localpart));
	}

	/**
	 * Override equals and agree that we're equal if 
	 * the passed object is a string and it matches 
	 * the name of the arg.
	 */
	bool
	equals(const QName&		theRHS) const;

	bool
	operator==(const QName&		theRHS) const
	{
		return equals(theRHS);
	}

	/**
	 * Get the namespace from a prefix by searching a vector of namespaces, of
	 * if reverse is true, search vector from last to first
	 */
	static DOMString getNamespaceForPrefix(const NamespaceVectorType& namespaces,
			const DOMString& prefix, bool reverse=true);

	/**
	 * Get the namespace from a prefix by searching a vector of namespace
	 * vectors, if reverse is true, search vector from last to first
	 */
	static DOMString getNamespaceForPrefix(const NamespacesStackType& nsStack,
			const DOMString& prefix, bool reverse=true);

	/**
	 * Get the prefix from a namespace by searching a vector of namespaces, of
	 * if reverse is true, search vector from last to first
	 */
	static DOMString getPrefixForNamespace(const NamespaceVectorType& namespaces,
			const DOMString& uri, bool reverse=true);

	/**
	 * Get the prefix from a namespace by searching a vector of namespace
	 * vectors, if reverse is true, search vector from last to first
	 */
	static DOMString getPrefixForNamespace(const NamespacesStackType& nsStack,
			const DOMString& uri, bool reverse=true);

private:

	void
	resolvePrefix(
			const DOMString&		qname,
			const PrefixResolver&	theResolver);

	DOMString				m_namespace;

	DOMString				m_localpart;
};



inline bool
operator<(
			const QName&	theLHS,
			const QName&	theRHS)
{
	if (theLHS.getNamespace() < theRHS.getNamespace())
		return true;
	else if (equals( theLHS.getNamespace(), theRHS.getNamespace()))
	{
		return theLHS.getLocalPart() < theRHS.getLocalPart();
	}
	else
		return false;
}



#endif	// QNAME_HEADER_GUARD_1357924680
