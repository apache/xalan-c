/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2001 The Apache Software Foundation.  All rights 
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
#include "XalanQName.hpp"



#include <Include/STLHelper.hpp>



#include <PlatformSupport/DOMStringHelper.hpp>
#include <PlatformSupport/XalanUnicode.hpp>



#include <DOMSupport/DOMServices.hpp>



const XalanDOMString	XalanQName::s_emptyString;



const XalanDOMString*
XalanQName::getNamespaceForPrefix(
			const NamespaceVectorType&	namespaces,
			const XalanDOMString&		prefix,
			bool						reverse)
{
	const XalanDOMString*	nsURI = 0;

	if(::equals(prefix, DOMServices::s_XMLString))
	{
		nsURI = &DOMServices::s_XMLNamespaceURI;
	}
	else if (::equals(prefix, DOMServices::s_XMLNamespace))
	{
		nsURI = &DOMServices::s_XMLNamespacePrefixURI;
	}
	else
	{
		if (reverse)
		{
			for(int j = namespaces.size()-1; j >= 0; j--)
			{
				const NameSpace&	ns = namespaces[j];

				const XalanDOMString&	thisPrefix = ns.getPrefix();

				if(::equals(prefix, thisPrefix))
				{
					nsURI = &ns.getURI();

					break;
				}
			}
		}
		else
		{
			for(unsigned int j = 0; j < namespaces.size(); j++)
			{
				const NameSpace&	ns = namespaces[j];

				const XalanDOMString&	thisPrefix = ns.getPrefix();

				if(::equals(prefix, thisPrefix))
				{
					nsURI = &ns.getURI();

					break;
				}
			}
		}
	}

	return nsURI;
}



const XalanDOMString*
XalanQName::getNamespaceForPrefix(
			const NamespacesStackType&	nsStack,
			const XalanDOMString&		prefix,
			bool						reverse)
{
	const XalanDOMString*	nsURI = 0;

	const int depth = nsStack.size();

	for(int i = depth-1; i >= 0; i--)
	{
		const NamespaceVectorType&	namespaces = nsStack[i];

		nsURI = getNamespaceForPrefix(namespaces, prefix, reverse);

		if (nsURI != 0)
			break;
	}

	return nsURI;
}



const XalanDOMString*
XalanQName::getPrefixForNamespace(
			const NamespaceVectorType&	namespaces,
			const XalanDOMString&		uri,
			bool						/* reverse */)
{
	const XalanDOMString*	thePrefix = 0;

	for(int j = namespaces.size()-1; j >= 0; j--)
	{
		const NameSpace&		ns = namespaces[j];
		const XalanDOMString&	thisURI = ns.getURI();

		if(::equals(uri, thisURI))
		{
			thePrefix = &ns.getPrefix();

			break;
		}
	}

	return thePrefix;
}		



const XalanDOMString*
XalanQName::getPrefixForNamespace(
			const NamespacesStackType&	nsStack,
			const XalanDOMString&		uri,
			bool						reverse)
{
	const XalanDOMString*	thePrefix = 0;

	const int		depth = nsStack.size();

	if (reverse)
	{
		for(int i = depth-1; i >= 0; i--)
		{
			const NamespaceVectorType&	namespaces = nsStack[i];

			thePrefix = getPrefixForNamespace(namespaces, uri, reverse);

			if (thePrefix != 0)
				break;
		}
	}
	else
	{
		for(int i = 0; i < depth; i++)
		{
			const NamespaceVectorType&	namespaces = nsStack[i];

			thePrefix = getPrefixForNamespace(namespaces, uri, reverse);

			if (thePrefix != 0)
				break;
		}
	}

	return thePrefix;
}
