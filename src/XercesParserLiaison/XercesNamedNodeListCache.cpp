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



#include "XercesNamedNodeListCache.hpp"



#include <algorithm>
#include <cassert>



#include <PlatformSupport/STLHelper.hpp>



#include <dom/DOM_Node.hpp>



#include "XercesNodeListBridge.hpp"



XercesNamedNodeListCache::XercesNamedNodeListCache(const XercesBridgeNavigator&		theNavigator) :
	m_navigator(theNavigator),
	m_cachedNodeLists(),
	m_cachedNodeListsNS()
{
}



XercesNamedNodeListCache::~XercesNamedNodeListCache()
{
#if !defined(XALAN_NO_NAMESPACES)
	using std::for_each;
#endif

	for_each(m_cachedNodeLists.begin(),
			 m_cachedNodeLists.end(),
			 makeMapValueDeleteFunctor(m_cachedNodeLists));

	for_each(m_cachedNodeListsNS.begin(),
			 m_cachedNodeListsNS.end(),
			 makeMapValueDeleteFunctor(m_cachedNodeListsNS));
}



XercesNodeListBridge*
XercesNamedNodeListCache::getElementsByTagName(const XalanDOMString&	tagname) const
{
	const NodeListCacheType::const_iterator		i =
		m_cachedNodeLists.find(tagname);

	if (i != m_cachedNodeLists.end())
	{
		return (*i).second;
	}
	else
	{
#if !defined(XALAN_NO_NAMESPACES)
		using std::make_pair;
#endif

		XercesNodeListBridge* const		theNewBridge =
			new XercesNodeListBridge(getXercesNodeList(tagname),
									 m_navigator);

#if defined(XALAN_NO_MUTABLE)
		return ((NodeListCacheType&)m_cachedNodeLists).insert(make_pair(tagname,
																		theNewBridge)).first->second;
#else
		return m_cachedNodeLists.insert(make_pair(tagname,
												  theNewBridge)).first->second;
#endif
	}
}



XercesNodeListBridge*
XercesNamedNodeListCache::getElementsByTagNameNS(
			const XalanDOMString&	namespaceURI,
			const XalanDOMString&	localName) const
{
	XalanDOMString	theSearchString(namespaceURI + localName);

	const NodeListCacheType::const_iterator		i =
		m_cachedNodeListsNS.find(theSearchString);

	if (i != m_cachedNodeLists.end())
	{
		return (*i).second;
	}
	else
	{
#if !defined(XALAN_NO_NAMESPACES)
		using std::make_pair;
#endif

		XercesNodeListBridge* const		theNewBridge =
			new XercesNodeListBridge(getXercesNodeList(namespaceURI, localName),
									 m_navigator);

#if defined(XALAN_NO_MUTABLE)
		return ((NodeListCacheType&)m_cachedNodeLists).insert(make_pair(theSearchString,
																		theNewBridge)).first->second;
#else
		return m_cachedNodeLists.insert(make_pair(theSearchString,
												  theNewBridge)).first->second;
#endif
	}
}
