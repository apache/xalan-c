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
			const XalanDOMString&		prefix)
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
		const NamespaceVectorType::size_type	theSize = namespaces.size();

		for(NamespaceVectorType::size_type j = theSize; j > 0; --j)
		{
			const NameSpace&	ns = namespaces[j - 1];

			const XalanDOMString&	thisPrefix = ns.getPrefix();

			if(::equals(prefix, thisPrefix))
			{
				nsURI = &ns.getURI();

				break;
			}
		}
	}

	return nsURI;
}



const XalanDOMString*
XalanQName::getNamespaceForPrefix(
			const NamespacesStackType&	nsStack,
			const XalanDOMString&		prefix)
{
#if 1
	return getNamespaceForPrefix(nsStack.begin(), nsStack.end(), prefix);
#else
	const XalanDOMString*	nsURI = 0;

	for(NamespacesStackType::size_type i = nsStack.size(); i > 0; --i)
	{
		const NamespaceVectorType&	namespaces = nsStack[i - 1];

		nsURI = getNamespaceForPrefix(namespaces, prefix);

		if (nsURI != 0)
			break;
	}

	return nsURI;
#endif
}



const XalanDOMString*
XalanQName::getNamespaceForPrefix(
			NamespacesStackType::const_iterator		theBegin,
			NamespacesStackType::const_iterator		theEnd,
			const XalanDOMString&					prefix)
{
	const XalanDOMString*	nsURI = 0;

	if (theBegin != theEnd)
	{
		do
		{
			nsURI = getNamespaceForPrefix(*(--theEnd), prefix);

			if (nsURI != 0)
			{
				break;
			}
		} while(theBegin != theEnd);
	}

	return nsURI;
}



const XalanDOMString*
XalanQName::getPrefixForNamespace(
			const NamespaceVectorType&	namespaces,
			const XalanDOMString&		uri)
{
	const XalanDOMString*	thePrefix = 0;

	const NamespaceVectorType::size_type	theSize = namespaces.size();

	for(NamespaceVectorType::size_type j = theSize; j > 0; --j)
	{
		const NameSpace&		ns = namespaces[j - 1];
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
			const XalanDOMString&		uri)
{
#if 1
	return getPrefixForNamespace(nsStack.begin(), nsStack.end(), uri);
#else
	const XalanDOMString*					thePrefix = 0;

	const NamespacesStackType::size_type	depth = nsStack.size();

	for(NamespacesStackType::size_type i = depth; i > 0; --i)
	{
		const NamespaceVectorType&	namespaces = nsStack[i - 1];

		thePrefix = getPrefixForNamespace(namespaces, uri);

		if (thePrefix != 0)
		{
			break;
		}
	}

	return thePrefix;
#endif
}



const XalanDOMString*
XalanQName::getPrefixForNamespace(
			NamespacesStackType::const_iterator		theBegin,
			NamespacesStackType::const_iterator		theEnd,
			const XalanDOMString&					uri)
{
	const XalanDOMString*	thePrefix = 0;

	if (theBegin != theEnd)
	{
		do
		{
			thePrefix = getPrefixForNamespace(*(--theEnd), uri);

			if (thePrefix != 0)
			{
				break;
			}
		} while(theBegin != theEnd);
	}

	return thePrefix;
}



bool
XalanQName::isValidNCName(const XalanDOMString&		theNCName)
{
	return isValidNCName(c_wstr(theNCName), length(theNCName));
}



bool
XalanQName::isValidNCName(
			const XalanDOMChar*			theNCName,
			XalanDOMString::size_type	theLength)
{
	assert(theNCName != 0);

	if (theLength == XalanDOMString::npos)
	{
		theLength = length(theNCName);
	}

	if (theLength == 0)
	{
		return false;
	}
	else
	{
		XalanDOMChar	c = theNCName[0];

		if(!(XalanXMLChar::isLetter(c) || c == XalanUnicode::charLowLine))
		  return false;

		if(theLength > 1)
		{
			for(XalanDOMString::size_type i = 1; i < theLength; i++)
			{
				c = theNCName[i]; 

				if(!(XalanXMLChar::isLetter(c) ||
					 XalanXMLChar::isDigit(c) ||
					 XalanXMLChar::isExtender(c) ||
					 XalanXMLChar::isCombiningChar(c) ||
					 c == XalanUnicode::charLowLine ||
					 c == XalanUnicode::charHyphenMinus ||
					 c == XalanUnicode::charFullStop))
				{
					return false;
				}
			}
		}

		return true;
	}
}



bool
XalanQName::isValidQName(const XalanDOMString&	theQName)
{
	return isValidQName(c_wstr(theQName), length(theQName));
}



bool
XalanQName::isValidQName(
			const XalanDOMChar*			theQName,
			XalanDOMString::size_type	theLength)
{
	const XalanDOMString::size_type		theIndex =
		indexOf(theQName, XalanUnicode::charColon);

	if (theIndex == theLength)
	{
		return isValidNCName(theQName, theLength);
	}
	else
	{
		const XalanDOMChar* const	thePrefix = c_wstr(theQName);
		const XalanDOMChar*	const	theLocalName = thePrefix + theIndex + 1;

		return XalanQName::isValidNCName(thePrefix, theIndex) &&
			   XalanQName::isValidNCName(theLocalName, theLength - theIndex - 1);
	}
}
