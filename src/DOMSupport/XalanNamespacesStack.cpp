/*
 * The Apache Software License, Version 1.1
 *
 *
 * Copyright (c) 1999-2002 The Apache Software Foundation.  All rights 
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
#include "XalanNamespacesStack.hpp"



#include "DOMServices.hpp"



XalanNamespacesStack::XalanNamespacesStackEntry::XalanNamespacesStackEntry() :
	m_namespaces(),
	m_position(m_namespaces.begin())
{
}



XalanNamespacesStack::XalanNamespacesStackEntry::XalanNamespacesStackEntry(const XalanNamespacesStackEntry&		theSource) :
	m_namespaces(theSource.m_namespaces),
	m_position(m_namespaces.begin() + (const_iterator(theSource.m_position) - theSource.m_namespaces.begin()))
{
}



XalanNamespacesStack::XalanNamespacesStackEntry&
XalanNamespacesStack::XalanNamespacesStackEntry::operator=(const XalanNamespacesStackEntry&		theRHS)
{
	if (this != &theRHS)
	{
		XalanNamespacesStackEntry	theCopy(theRHS);

		swap(theCopy);
	}

	return *this;
}



XalanNamespacesStack::XalanNamespacesStackEntry::~XalanNamespacesStackEntry()
{
}



void
XalanNamespacesStack::XalanNamespacesStackEntry::swap(XalanNamespacesStackEntry&	theOther)
{
	m_namespaces.swap(theOther.m_namespaces);

	const iterator	theTemp = theOther.m_position;

	theOther.m_position = m_position;

	m_position = theTemp;
}



void
XalanNamespacesStack::XalanNamespacesStackEntry::addDeclaration(
				const XalanDOMString&		thePrefix,
				const XalanDOMChar*			theURI,
				XalanDOMString::size_type	theLength)
{
	if (m_position == m_namespaces.end())
	{
		m_namespaces.resize(m_namespaces.size() + 1);

		m_position = m_namespaces.end() - 1;
	}

	XalanNamespace&		theNamespace = *m_position;

	theNamespace.setPrefix(thePrefix);

	theNamespace.setURI(theURI, theLength);

	++m_position;
}



const XalanDOMString*
XalanNamespacesStack::XalanNamespacesStackEntry::getNamespaceForPrefix(const XalanDOMString&	thePrefix) const
{
	if (m_namespaces.empty() == false)
	{
		const_iterator	i(m_position);

		do
		{
			--i;

			const XalanNamespace&	ns = (*i);

			const XalanDOMString&	thisPrefix = ns.getPrefix();

			if(equals(thePrefix, thisPrefix))
			{
				return &ns.getURI();
			}
		} while (i != m_namespaces.begin());
	}

	return 0;
}



const XalanDOMString*
XalanNamespacesStack::XalanNamespacesStackEntry::getPrefixForNamespace(const XalanDOMString&	theURI) const
{
	if (m_namespaces.empty() == false)
	{
		const_iterator	i(m_position);

		do
		{
			--i;

			const XalanNamespace&	ns = (*i);

			const XalanDOMString&	thisURI = ns.getURI();

			if(equals(theURI, thisURI))
			{
				return &ns.getPrefix();
			}
		} while (i != m_namespaces.begin());
	}

	return 0;
}



void
XalanNamespacesStack::XalanNamespacesStackEntry::clear()
{
	m_namespaces.clear();

	m_position = m_namespaces.begin();
}



XalanNamespacesStack::XalanNamespacesStack() :
	m_resultNamespaces(1),
	m_stackBegin(m_resultNamespaces.begin()),
	m_stackPosition(m_stackBegin),
	m_createNewContextStack()
{
	// m_resultNamespaces is initialized to a size of
	// 1, so we always have a dummy entry at the
	// beginning.  This makes the implementation
	// much simpler.
}



XalanNamespacesStack::~XalanNamespacesStack()
{
}



void
XalanNamespacesStack::addDeclaration(
			const XalanDOMString&		thePrefix,
	        const XalanDOMChar*			theURI,
			XalanDOMString::size_type	theLength)
{
	assert(theURI != 0);
	assert(m_createNewContextStack.size() != 0);

	// Check to see if we need to create a new context and do so if necessary...
	if (m_createNewContextStack.back() == true)
	{
		++m_stackPosition;

		if (m_stackPosition == m_resultNamespaces.end())
		{
			m_resultNamespaces.resize(m_resultNamespaces.size() + 1);

			m_stackPosition = m_resultNamespaces.end() - 1;
			m_stackBegin = m_resultNamespaces.begin();
		}

		m_createNewContextStack.back() = false;
	}

	XalanNamespacesStackEntry&	theCurrentEntry = *m_stackPosition;

	// Add a new namespace declaration...
	theCurrentEntry.addDeclaration(thePrefix, theURI, theLength);
}



void
XalanNamespacesStack::pushContext()
{
	if (m_createNewContextStack.empty() == true)
	{
		m_createNewContextStack.reserve(eDefaultCreateNewContextStackSize);
	}

	m_createNewContextStack.push_back(true);
}



void
XalanNamespacesStack::popContext()
{
	assert(m_createNewContextStack.empty() == false);

	if (m_createNewContextStack.back() == false)
	{
		assert(m_resultNamespaces.empty() == false &&
			   m_stackPosition != m_resultNamespaces.begin());

		(*m_stackPosition).reset();

		--m_stackPosition;
	}

	m_createNewContextStack.pop_back();
}



const XalanDOMString*
XalanNamespacesStack::getNamespaceForPrefix(const XalanDOMString&	thePrefix) const
{
	if(::equals(thePrefix, DOMServices::s_XMLString))
	{
		return &DOMServices::s_XMLNamespaceURI;
	}
	else if (::equals(thePrefix, DOMServices::s_XMLNamespace))
	{
		return &DOMServices::s_XMLNamespacePrefixURI;
	}
	else if (m_stackPosition == m_stackBegin)
	{
		return 0;
	}
	else
	{
		NamespacesStackType::const_iterator		theBegin(m_stackBegin);
		NamespacesStackType::const_iterator		theEnd(m_stackPosition + 1);

		const XalanDOMString*	nsURI = 0;

		if (theBegin != theEnd)
		{
			do
			{
				nsURI = (*(--theEnd)).getNamespaceForPrefix(thePrefix);

				if (nsURI != 0)
				{
					break;
				}
			} while(theBegin != theEnd);
		}

		return nsURI;
	}
}



const XalanDOMString*
XalanNamespacesStack::getPrefixForNamespace(const XalanDOMString&	theURI) const
{
	if (m_stackPosition == m_stackBegin)
	{
		return 0;
	}
	else
	{
		NamespacesStackType::const_iterator		theBegin(m_stackBegin);
		NamespacesStackType::const_iterator		theEnd(m_stackPosition + 1);

		const XalanDOMString*	prefix = 0;

		if (theBegin != theEnd)
		{
			do
			{
				prefix = (*(--theEnd)).getPrefixForNamespace(theURI);

				if (prefix != 0)
				{
					break;
				}
			} while(theBegin != theEnd);
		}

		return prefix;
	}
}



bool
XalanNamespacesStack::prefixIsPresentLocal(const XalanDOMString&	thePrefix)
{
	// Check to see if we need to create a new context.  If so, there are
	// no prefixes mapped at this level, so return false...
	if (m_createNewContextStack.back() == true)
	{
		return false;
	}
	else
	{
		const XalanNamespacesStackEntry&	theNamespaces =
			*m_stackPosition;

		return theNamespaces.isPrefixPresent(thePrefix);
	}
}



void
XalanNamespacesStack::clear()
{
	// Since we always keep one dummy entry at the beginning,
	// swap with an OutputContextStackType instance of size 1.
	NamespacesStackType(1).swap(m_resultNamespaces);

	m_stackBegin = m_resultNamespaces.begin();

	m_stackPosition = m_stackBegin;

	m_createNewContextStack.clear();
}



const XalanDOMString*
XalanNamespacesStack::getNamespaceForPrefix(
			NamespacesStackType::const_iterator		theBegin,
			NamespacesStackType::const_iterator		theEnd,
			const XalanDOMString&					prefix)
{
	const XalanDOMString*	nsURI = 0;

	if (theBegin != theEnd)
	{
		do
		{
			nsURI = (*(--theEnd)).getNamespaceForPrefix(prefix);

			if (nsURI != 0)
			{
				break;
			}
		} while(theBegin != theEnd);
	}

	return nsURI;
}
