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
#include "QNameByValue.hpp"



#include <PlatformSupport/XSLException.hpp>



#include <DOMSupport/DOMServices.hpp>



#include "ElementPrefixResolverProxy.hpp"
#include "PrefixResolver.hpp"
#include "XPathSupport.hpp"



QNameByValue::QNameByValue() :
	QName(),
	m_namespace(),
	m_localpart()
{
}



QNameByValue::QNameByValue(const QNameByValue&	theSource) :
	QName(),
	m_namespace(theSource.m_namespace),
	m_localpart(theSource.m_localpart)
{
}



QNameByValue::QNameByValue(const QName&		theSource) :
	QName(),
	m_namespace(theSource.getNamespace()),
	m_localpart(theSource.getLocalPart())
{
}



QNameByValue::QNameByValue(
			const XalanDOMString&	theNamespace,
			const XalanDOMString&	theLocalPart) :
	QName(),
	m_namespace(theNamespace),
	m_localpart(theLocalPart)
{
}



QNameByValue::QNameByValue(
			const XalanDOMString&		qname,
			const NamespacesStackType&	namespaces) :
	m_namespace(),
	m_localpart()
{
	initialize(c_wstr(qname), namespaces);
}



QNameByValue::QNameByValue(
			const XalanDOMChar*			qname,
			const NamespacesStackType&	namespaces) :
	m_namespace(),
	m_localpart()
{
	assert(qname != 0);

	initialize(qname, namespaces);
}



QNameByValue::QNameByValue(
			const XalanDOMString&	qname,
			const XalanElement*		namespaceContext,
			const XPathEnvSupport&	envSupport,
			const XPathSupport& 	support) :
	m_namespace(),
	m_localpart()
{
	ElementPrefixResolverProxy	theProxy(namespaceContext, envSupport, support);

	resolvePrefix(qname, theProxy);
}



QNameByValue::QNameByValue(
			const XalanDOMString&	qname,
			const PrefixResolver&	theResolver) :
	m_namespace(),
	m_localpart()
{
	resolvePrefix(qname, theResolver);
}



QNameByValue::~QNameByValue()
{
}



const XalanDOMString&
QNameByValue::getLocalPart() const
{
	return m_localpart;
}



const XalanDOMString&
QNameByValue::getNamespace() const
{
	return m_namespace;
}



void
QNameByValue::initialize(
			const XalanDOMChar*			qname,
			const NamespacesStackType&	namespaces)
{
	const unsigned int	indexOfNSSep = indexOf(qname, XalanUnicode::charColon);

	if(indexOfNSSep < length(qname))
	{
		const XalanDOMString		prefix = substring(qname, 0, indexOfNSSep);

		if(::equals(prefix, DOMServices::s_XMLNamespace))
			return;

		m_namespace = getNamespaceForPrefix(namespaces, prefix);

		if(0 == length(m_namespace))
		{
			throw XSLException(TranscodeFromLocalCodePage("Prefix must resolve to a namespace: ") + prefix);
		}

		m_localpart =  substring(qname, indexOfNSSep + 1);
	}
	else
	{
		m_localpart = qname;
	}
}



void
QNameByValue::resolvePrefix(
			const XalanDOMString&	qname,
			const PrefixResolver&	theResolver)
{
	const unsigned int	indexOfNSSep = indexOf(qname, XalanUnicode::charColon);
	const unsigned int	theLength = length(qname);

	if(indexOfNSSep < theLength)
	{
		const XalanDOMString	prefix = substring(qname, 0, indexOfNSSep);

		if(::equals(prefix, DOMServices::s_XMLString))
		{
			m_namespace = DOMServices::s_XMLNamespaceURI;
		}
		// The default namespace is not resolved.
		else if(::equals(prefix, DOMServices::s_XMLNamespace))
		{
			return;
		}
		else
		{
			m_namespace = theResolver.getNamespaceForPrefix(prefix);
		}  

		if(0 == length(m_namespace))
		{
			throw XSLException(TranscodeFromLocalCodePage("Prefix must resolve to a namespace: ") + prefix);
		}
	}
	else
	{
		// $$$ ToDo: error or warning...
	}

	m_localpart = indexOfNSSep == theLength ? qname : substring(qname, indexOfNSSep + 1);
}
