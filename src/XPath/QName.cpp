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
// Class header file...
#include "QName.hpp"



#include <Include/DOMHelper.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/STLHelper.hpp>
#include <PlatformSupport/XSLException.hpp>



#include <DOMSupport/DOMServices.hpp>



#include "NameSpace.hpp"
#include "ElementPrefixResolverProxy.hpp"
#include "PrefixResolver.hpp"
#include "XPathSupport.hpp"



QName::QName(
			const DOMString&	theNamespace,
			const DOMString&	theLocalPart) :
	m_namespace(theNamespace),
	m_localpart(theLocalPart)
{
}



QName::QName(
			const DOMString&				qname,
			const NamespacesStackType&	namespaces) :
	m_namespace(),
	m_localpart()
{
	const int	indexOfNSSep = indexOf(qname, ':');

	if(indexOfNSSep > 0)
	{
		const DOMString		prefix = substring(qname, 0, indexOfNSSep);
		if(prefix.equals("xmlns"))
			return;
		m_namespace = getNamespaceForPrefix(namespaces, prefix);
		if(0 == length(m_namespace))
		{
			throw XSLException(DOMString("Prefix must resolve to a namespace: ") + prefix);
		}
		m_localpart =  substring(qname, indexOfNSSep + 1);
	}
	else
		m_localpart = qname;
}



QName::QName(
			const DOMString&		qname,
			const DOM_Element&		namespaceContext,
			const XPathSupport& 	support) :
	m_namespace(),
	m_localpart()
{
	ElementPrefixResolverProxy	theProxy(namespaceContext, support);

	resolvePrefix(qname, theProxy);
}



QName::QName(
			const DOMString&		qname,
			const PrefixResolver&	theResolver) :
	m_namespace(),
	m_localpart()
{
	resolvePrefix(qname, theResolver);
}



QName::~QName()
{
}



bool
QName::equals(const QName&	theRHS) const
{
	return ::equals(m_localpart, theRHS.m_localpart) &&
		   ::equals(m_namespace, theRHS.m_namespace);
}



void
QName::resolvePrefix(
			const DOMString&		qname,
			const PrefixResolver&	theResolver)
{
	const int	indexOfNSSep = indexOf(qname, ':');

	if(indexOfNSSep > 0)
	{
		const DOMString		prefix = substring(qname, 0, indexOfNSSep);

		if(::equals(prefix, "xml"))
		{
			m_namespace = DOMServices::s_XMLNamespaceURI;
		}
		// The default namespace is not resolved.
      else if(prefix.equals("xmlns"))
      {
        return;
      }
		else
		{
			m_namespace = theResolver.getNamespaceForPrefix(prefix);
		}  

		if(0 == length(m_namespace))
		{
			throw XSLException(DOMString("Prefix must resolve to a namespace: ") + prefix);
		}
	}
	else
	{
		// $$$ ToDo: error or warning...
	}

	m_localpart = indexOfNSSep < 0 ? qname : substring(qname, indexOfNSSep + 1);
}

DOMString QName::getNamespaceForPrefix(const NamespaceVectorType& namespaces,
		const DOMString& prefix, bool reverse /* true */)
{
	DOMString nsURI;
	if(::equals(prefix, "xml"))
		nsURI = DOMServices::s_XMLNamespaceURI;
	else
	{
		if (reverse)
		{
			for(int j = namespaces.size()-1; j >= 0; j--)
			{
				const NameSpace ns = namespaces[j];
				const DOMString& thisPrefix = ns.getPrefix();
				if((0 != thisPrefix.length()) && prefix.equals(thisPrefix))
					return ns.getURI();
			}
		}
		else
		{
			for(int j = 0; j < namespaces.size(); j++)
			{
				const NameSpace ns = namespaces[j];
				const DOMString& thisPrefix = ns.getPrefix();
				if((0 != thisPrefix.length()) && prefix.equals(thisPrefix))
					return ns.getURI();
			}
		}
	}
	return nsURI;
}
      
DOMString QName::getNamespaceForPrefix(const NamespacesStackType& nsStack,
		const DOMString& prefix, bool reverse /* true */)
{
	DOMString nsURI;
	int depth = nsStack.size();
	for(int i = depth-1; i >= 0; i--)
	{
		const NamespaceVectorType& namespaces = nsStack[i];
		nsURI = QName::getNamespaceForPrefix(namespaces, prefix, reverse);
		if (! ::isEmpty(nsURI))
			return nsURI;
	}
	return nsURI;
}
      
DOMString QName::getPrefixForNamespace(const NamespaceVectorType& namespaces,
		const DOMString& uri, bool reverse /* true */)
{
	for(int j = namespaces.size()-1; j >= 0; j--)
	{
		const NameSpace& ns = namespaces[j];
		const DOMString& thisPrefix = ns.getURI();
		if((0 != thisPrefix.length()) && uri.equals(thisPrefix))
			return ns.getPrefix();
	}
	return DOMString();
}		

DOMString QName::getPrefixForNamespace(const NamespacesStackType& nsStack,
		const DOMString& uri, bool reverse /* true */)
{
	DOMString prefix;
	int depth = nsStack.size();
	if (reverse)
	{
		for(int i = depth-1; i >= 0; i--)
		{
			const NamespaceVectorType& namespaces = nsStack[i];
			prefix = QName::getPrefixForNamespace(namespaces, uri, reverse);
			if (! ::isEmpty(prefix))
				return prefix;
		}
	}
	else
	{
		for(int i = 0; i < depth; i++)
		{
			const NamespaceVectorType& namespaces = nsStack[i];
			prefix = QName::getPrefixForNamespace(namespaces, uri, reverse);
			if (! ::isEmpty(prefix))
				return prefix;
		}
	}
	return prefix;
}
