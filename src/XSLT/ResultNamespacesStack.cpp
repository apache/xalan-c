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

// Class header file...
#include "ResultNamespacesStack.hpp"



ResultNamespacesStack::ResultNamespacesStack() :
	m_resultNamespaces(),
	m_createNewContextStack()
{
}



ResultNamespacesStack::~ResultNamespacesStack()
{
}



void
ResultNamespacesStack::addDeclaration(
			const XalanDOMString&	thePrefix,
			const XalanDOMString&	theNamespaceURI)
{
	assert(m_createNewContextStack.size() != 0);

	// Check to see if we need to create a new context and do so if necessary...
	if (m_createNewContextStack.back() == true)
	{
		m_resultNamespaces.resize(m_resultNamespaces.size() + 1);

		m_resultNamespaces.back().resize(1);

		m_createNewContextStack.back() = false;
	}

	NamespaceVectorType&	theCurrentNamespaces = m_resultNamespaces.back();

	NameSpace&				theCurrentNamespace = theCurrentNamespaces.front();

	// If the last vector contains only an empty namespace, replace it with a
	// new vector containing only this namespace
	if(isEmpty(theCurrentNamespace.getURI()))
	{
		theCurrentNamespace.setPrefix(thePrefix);

		theCurrentNamespace.setURI(theNamespaceURI);
	}
	else
	{
		// Otherwise, add the namespace at the end of the current namespaces.
		theCurrentNamespaces.push_back(NameSpace(thePrefix, theNamespaceURI));
	}
}



void
ResultNamespacesStack::pushContext()
{
	if (m_createNewContextStack.size() == 0)
	{
		m_createNewContextStack.reserve(eDefaultCreateNewContextStackSize);
	}

	m_createNewContextStack.push_back(true);
}



void
ResultNamespacesStack::popContext()
{
	assert(m_createNewContextStack.size() != 0);

	if (m_createNewContextStack.back() == false)
	{
		assert(m_resultNamespaces.size() != 0);

		m_resultNamespaces.pop_back();
	}

	m_createNewContextStack.pop_back();
}



const XalanDOMString&
ResultNamespacesStack::getNamespaceForPrefix(const XalanDOMString&	thePrefix) const
{
	// Search vector from first element back
	return QName::getNamespaceForPrefix(m_resultNamespaces, thePrefix, false);
}



const XalanDOMString&
ResultNamespacesStack::getPrefixForNamespace(const XalanDOMString&	theNamespaceURI) const
{
	// Search vector from first element back
	return QName::getPrefixForNamespace(m_resultNamespaces, theNamespaceURI, false);
}



bool
ResultNamespacesStack::prefixIsPresentLocal(const XalanDOMString&	thePrefix)
{
	bool	fResult = false;

	// Check to see if we need to create a new context.  If so, there are
	// no prefixes mapped at this level, so return false...
	if (m_createNewContextStack.back() == false)
	{
		const NamespaceVectorType&	theNamespaces =
			m_resultNamespaces.back();

		NamespaceVectorType::const_iterator			i = theNamespaces.begin();
		const NamespaceVectorType::const_iterator	theEnd = theNamespaces.end();

		while(i != theEnd && fResult == false)
		{
			const NameSpace&		ns = (*i);

			const XalanDOMString&	thisPrefix = ns.getPrefix();

			if(::equals(thePrefix, thisPrefix))
			{
				fResult = true;
			}
			else
			{
				++i;
			}
		}
	}

	return fResult;
}



void
ResultNamespacesStack::clear()
{
	m_resultNamespaces.clear();

	m_createNewContextStack.clear();
}
